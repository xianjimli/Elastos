
#include "server/CActivityRecord.h"
#include "server/AttributeCache.h"
#include "server/CActivityManagerService.h"
#include "os/Process.h"
#include "os/SystemClock.h"
#include <Slogger.h>

using namespace Elastos::Utility::Logging;


CActivityRecord::CActivityRecord() :
    mSendResult(FALSE),
    mTaskAffinity(String(NULL)),
    mStateNotNeeded(FALSE),
    mFullscreen(TRUE),
    mComponentSpecified(FALSE),
    mIsHomeActivity(FALSE),
    mTask(NULL),
    mApp(NULL),
    mState(ActivityState_INITIALIZING),
    mFrontOfTask(FALSE),
    mLaunchFailed(FALSE),
    mHaveState(FALSE),
    mStopped(FALSE),
    mDelayedResume(FALSE),
    mFinishing(FALSE),
    mConfigDestroy(FALSE),
    mKeysPaused(FALSE),
    mInHistory(FALSE),
    mVisible(TRUE),
    mWaitingVisible(FALSE),
    mNowVisible(FALSE),
    mThumbnailNeeded(FALSE),
    mIdle(FALSE),
    mHasBeenLaunched(FALSE),
    mFrozenBeforeDestroy(FALSE)
{}

CActivityRecord::~CActivityRecord()
{}

ECode CActivityRecord::Init(
    /* [in] */ CActivityManagerService* service,
    /* [in] */ ActivityStack* stack,
    /* [in] */ ProcessRecord* caller,
    /* [in] */ Int32 launchedFromUid,
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ IActivityInfo* aInfo,
    /* [in] */ IConfiguration* configuration,
    /* [in] */ CActivityRecord* resultTo,
    /* [in] */ const String& resultWho,
    /* [in] */ Int32 reqCode,
    /* [in] */ Boolean componentSpecified)
{
    mService = service;
    mStack = stack;
    mInfo = aInfo;
    mLaunchedFromUid = launchedFromUid;
    mIntent = intent;
    AutoPtr<IComponentName> component;
    intent->GetComponent((IComponentName**)&component);
    component->FlattenToShortString(&mShortComponentName);
    mResolvedType = resolvedType;
    mComponentSpecified = componentSpecified;
    mConfiguration = configuration;
    mResultTo = resultTo;
    mResultWho = resultWho;
    mRequestCode = reqCode;

    if (mInfo != NULL) {
        String target;
        mInfo->GetTargetActivity(&target);
        Int32 mode;
        mInfo->GetLaunchMode(&mode);
        if (target.IsNull()
                || mode == ActivityInfo_LAUNCH_MULTIPLE
                || mode == ActivityInfo_LAUNCH_SINGLE_TOP) {
            mIntent->GetComponent((IComponentName**)&mRealActivity);
        }
        else {
            String cname;
            mInfo->GetCapsuleName(&cname);
            CComponentName::New(cname, target, (IComponentName**)&mRealActivity);
        }
        mInfo->GetTaskAffinity(&mTaskAffinity);
        Int32 flags;
        mInfo->GetFlags(&flags);
        mStateNotNeeded = (flags & ActivityInfo_FLAG_STATE_NOT_NEEDED) != 0;
        AutoPtr<IApplicationInfo> appInfo;
        mInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
        appInfo->GetSourceDir(&mBaseDir);
        appInfo->GetPublicSourceDir(&mResDir);
        appInfo->GetDataDir(&mDataDir);
        mInfo->GetNonLocalizedLabel((ICharSequence**)&mNonLocalizedLabel);
        mInfo->GetLabelRes(&mLabelRes);
        if (mNonLocalizedLabel == NULL && mLabelRes == 0) {
            AutoPtr<IApplicationInfo> app = appInfo;
            app->GetNonLocalizedLabel((ICharSequence**)&mNonLocalizedLabel);
            app->GetLabelRes(&mLabelRes);
        }
        mInfo->GetIconResource(&mIcon);
        mInfo->GetThemeResource(&mTheme);
        if ((flags & ActivityInfo_FLAG_MULTIPROCESS) != 0
                && caller != NULL) {
            Int32 appUid, aUid;
            appInfo->GetUid(&appUid);
            caller->mInfo->GetUid(&aUid);
            if  (appUid == Process::SYSTEM_UID || appUid == aUid) {
                mProcessName = caller->mProcessName;
            }
            else {
                mInfo->GetProcessName(&mProcessName);
            }
        }
        else {
            mInfo->GetProcessName(&mProcessName);
        }

        if (intent != NULL && (flags & ActivityInfo_FLAG_EXCLUDE_FROM_RECENTS) != 0) {
            intent->AddFlags(Intent_FLAG_ACTIVITY_EXCLUDE_FROM_RECENTS);
        }

        appInfo->GetCapsuleName(&mCapsuleName);
        mLaunchMode = mode;
//        AttributeCache.Entry ent = AttributeCache.instance().get(packageName,
//                theme != 0 ? theme : android.R.style.Theme,
//                com.android.internal.R.styleable.Window);
//        fullscreen = ent != null && !ent.array.getBoolean(
//                com.android.internal.R.styleable.Window_windowIsFloating, false)
//                && !ent.array.getBoolean(
//                com.android.internal.R.styleable.Window_windowIsTranslucent, false);
        if (!mComponentSpecified || mLaunchedFromUid == Process::MyUid()
                || mLaunchedFromUid == 0) {
            // If we know the system has determined the component, then
            // we can consider this to be a home activity...
            String action;
            mIntent->GetAction(&action);
            Boolean category;
            mIntent->HasCategory(String(Intent_CATEGORY_HOME), &category);
            ArrayOf<String>* categories;
            mIntent->GetCategories((ArrayOf<String>**)&categories);
            AutoPtr<IUri> data;
            mIntent->GetData((IUri**)&data);
            String type;
            mIntent->GetType(&type);
            Int32 flags;
            mIntent->GetFlags(&flags);
            String name;
            mRealActivity->GetClassName(&name);
            if (String(Intent_ACTION_MAIN).Equals(action) && category &&
                categories->GetLength() == 1 &&
                data == NULL &&
                type == NULL &&
                (flags & Intent_FLAG_ACTIVITY_NEW_TASK) != 0 &&
                String("android").Equals(name)) {
                // This sure looks like a home activity!
                // Note the last check is so we don't count the resolver
                // activity as being home...  really, we don't care about
                // doing anything special with something that comes from
                // the core framework package.
                mIsHomeActivity = TRUE;
            }
            else {
                mIsHomeActivity = FALSE;
            }
        }
        else {
            mIsHomeActivity = FALSE;
        }
    }

    return NOERROR;
}

UriPermissionOwner* CActivityRecord::GetUriPermissionsLocked()
{
    if (mUriPermissions == NULL) {
        mUriPermissions = new UriPermissionOwner(mService, (Handle32)this);
    }
    return mUriPermissions;
}

void CActivityRecord::AddResultLocked(
    /* [in] */ CActivityRecord* from,
    /* [in] */ const String& resultWho,
    /* [in] */ Int32 requestCode,
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent* resultData)
{
    ActivityResult* r = new ActivityResult(from, resultWho,
            requestCode, resultCode, resultData);
    if (mResults == NULL) {
        mResults = new List<ActivityResult*>();
    }
    mResults->PushBack(r);
}

void CActivityRecord::RemoveResultsLocked(
    /* [in] */ CActivityRecord* from,
    /* [in] */ const String& resultWho,
    /* [in] */ Int32 requestCode)
{
    if (mResults != NULL) {
        List<ActivityResult*>::Iterator it1 = mResults->Begin();
        while (it1 != mResults->End()) {
            ActivityResult* r = *it1;
            if ((CActivityRecord*)r->mFrom != from) {
                ++it1;
                continue;
            }
            String who;
            r->mResultInfo->GetResultWho(&who);
            if (who.IsNull()) {
                if (!resultWho.IsNull()) {
                    ++it1;
                    continue;
                }
            } else {
                if (who.Compare(resultWho)) {
                    ++it1;
                    continue;
                }
            }
            Int32 code;
            r->mResultInfo->GetRequestCode(&code);
            if (code != requestCode) {
                ++it1;
                continue;
            }

            it1 = mResults->Erase(it1);
        }
    }
}

void CActivityRecord::AddNewIntentLocked(
    /* [in] */ IIntent* intent)
{
    if (mNewIntents == NULL) {
        mNewIntents = new List<AutoPtr<IIntent> >();
    }
    mNewIntents->PushBack(intent);
}

/**
 * Deliver a new Intent to an existing activity, so that its onNewIntent()
 * method will be called at the proper time.
 */
void CActivityRecord::DeliverNewIntentLocked(
    /* [in] */ Int32 callingUid,
    /* [in] */ IIntent* intent)
{
    Boolean sent = FALSE;
    if (mState == ActivityState_RESUMED
            && mApp != NULL && mApp->mAppApartment != NULL) {
        AutoPtr<IIntent> intent;
        CIntent::New(intent, (IIntent**)&intent);
        AutoPtr<IObjectContainer> ar;
        CParcelableObjectContainer::New((IObjectContainer**)&ar);
        ar->Add((IIntent*)intent);
//        service.grantUriPermissionFromIntentLocked(callingUid, packageName,
//                        intent, getUriPermissionsLocked());
        ECode ec = mApp->mAppApartment->ScheduleNewIntent(ar, (IBinder*)this);
        if (SUCCEEDED(ec)) {
            sent = TRUE;
        }
        else {
//            Slog.w(TAG, "Exception thrown sending new intent to " + this, e);
        }
    }
    if (!sent) {
        AutoPtr<IIntent> intent;
        CIntent::New(intent, (IIntent**)&intent);
        AddNewIntentLocked(intent);
    }
}

void CActivityRecord::RemoveUriPermissionsLocked()
{
    if (mUriPermissions != NULL) {
        mUriPermissions->RemoveUriPermissionsLocked();
        delete mUriPermissions;
        mUriPermissions = NULL;
    }
}

void CActivityRecord::PauseKeyDispatchingLocked()
{
    if (!mKeysPaused) {
        mKeysPaused = TRUE;
        mService->mWindowManager->PauseKeyDispatching(this);
    }
}

void CActivityRecord::ResumeKeyDispatchingLocked()
{
    if (mKeysPaused) {
        mKeysPaused = FALSE;
        mService->mWindowManager->ResumeKeyDispatching(this);
    }
}

Boolean CActivityRecord::MayFreezeScreenLocked(
    /* [in] */ ProcessRecord* app)
{
    // Only freeze the screen if this activity is currently attached to
    // an application, and that application is not blocked or unresponding.
    // In any other case, we can't count on getting the screen unfrozen,
    // so it is best to leave as-is.
    return app == NULL || (!app->mCrashing && !app->mNotResponding);
}

void CActivityRecord::StartFreezingScreenLocked(
    /* [in] */ ProcessRecord* app,
    /* [in] */ Int32 configChanges)
{
    if (MayFreezeScreenLocked(app)) {
        mService->mWindowManager->StartAppFreezingScreen(this, configChanges);
    }
}

void CActivityRecord::StopFreezingScreenLocked(
    /* [in] */ Boolean force)
{
    if (force || mFrozenBeforeDestroy) {
        mFrozenBeforeDestroy = FALSE;
        mService->mWindowManager->StopAppFreezingScreen(this, force);
    }
}

ECode CActivityRecord::GetDescription(
    /* [out] */ String* description)
{
    return E_NOT_IMPLEMENTED;
}

ECode CActivityRecord::WindowsVisible()
{
    Mutex::Autolock lock(mService->_m_syncLock);
    if (mLaunchTime != 0) {
        const Int32 curTime = SystemClock::GetUptimeMillis();
        const Int32 thisTime = curTime - mLaunchTime;
        const Int32 totalTime = mStack->GetInitialStartTime() != 0
                ? (curTime - mStack->GetInitialStartTime()) : thisTime;
        if (CActivityManagerService::SHOW_ACTIVITY_START_TIME) {
//             EventLog.writeEvent(EventLogTags.ACTIVITY_LAUNCH_TIME,
//                 System.identityHashCode(this), shortComponentName,
//                 thisTime, totalTime);
            StringBuffer sb;
            mService->GetStringBuffer(&sb);
            // sb.SetLength(0);
            sb += String("Displayed ");
            sb += String(mShortComponentName);
            sb += String(": ");
//            TimeUtils::FormatDuration(thisTime, sb);
//            if (thisTime != totalTime) {
//                sb.append(" (total ");
//                TimeUtils::FormatDuration(totalTime, sb);
//                sb.append(")");
//            }
            Slogger::I(CActivityManagerService::TAG, sb);
        }
        mStack->ReportActivityLaunchedLocked(FALSE, this, thisTime, totalTime);
        if (totalTime > 0) {
//            mService.mUsageStatsService.noteLaunchTime(realActivity, (int)totalTime);
        }
        mLaunchTime = 0;
        mStack->SetInitialStartTime(0);
    }
    mStartTime = 0;
    mStack->ReportActivityVisibleLocked(this);
    if (CActivityManagerService::DEBUG_SWITCH)
        Slogger::V(CActivityManagerService::TAG, String("windowsVisible(): ") + this);
    if (!mNowVisible) {
        mNowVisible = TRUE;
        if (!mIdle) {
            // Instead of doing the full stop routine here, let's just
            // hide any activities we now can, and let them stop when
            // the normal idle happens.
            mStack->ProcessStoppingActivitiesLocked(FALSE);
        }
        else {
            // If this activity was already idle, then we now need to
            // make sure we perform the full stop of any activities
            // that are waiting to do so.    This is because we won't
            // do that while they are still waiting for this one to
            // become visible.
            List<AutoPtr<CActivityRecord> >* activities;
            activities = mStack->GetWaitingVisibleActivities();
            if (activities->Begin() != activities->End()) {
                    List<AutoPtr<CActivityRecord> >::Iterator it;
                    for (it = activities->Begin(); it != activities->End(); ++it){
                        AutoPtr<CActivityRecord> r = *it;
                        r->mWaitingVisible = FALSE;
                        if(CActivityManagerService::DEBUG_SWITCH)
                            Slogger::V(CActivityManagerService::TAG,
                                String("Was waiting for visible: ") + r);
                        }
                    activities->Clear();
    //                 Message msg = Message.obtain();
    //                 msg.what = ActivityStack.IDLE_NOW_MSG;
    //                 stack.mHandler.sendMessage(msg);
                    }
                }
            mService->ScheduleAppGcsLocked();
        }
        return NOERROR;
}

ECode CActivityRecord::WindowsGone()
{
    if (CActivityManagerService::DEBUG_SWITCH)
        Slogger::V(CActivityManagerService::TAG, String("windowsGone(): ") + this);
    mNowVisible = FALSE;
    return NOERROR;
}

ECode CActivityRecord::KeyDispatchingTimedOut(
    /* [out] */ Boolean* result)
{
    AutoPtr<CActivityRecord> r;
    ProcessRecord* anrApp = NULL;
    {
        Mutex::Autolock lock(mService->_m_syncLock);
        r = GetWaitingHistoryRecordLocked();
        if (r != NULL && r->mApp != NULL) {
            if (r->mApp->mDebugging) {
                *result = FALSE;
                return NOERROR;
            }
            Boolean dDexOpt;
            mService->GetDidDexOpt(&dDexOpt);
            if (dDexOpt) {
                // Give more time since we were dexopting.
                mService->SetDidDexOpt(FALSE);
                *result = FALSE;
                return NOERROR;
            }

            if (r->mApp->mInstrumentationClass == NULL) {
                    anrApp = r->mApp;
            }
            else {
                AutoPtr<IBundle> info;
                info->PutString(String("shortMsg"), String("keyDispatchingTimedOut"));
                info->PutString(String("longMsg"), String("Timed out while dispatching key event"));
                mService->FinishInstrumentationLocked(
                    r->mApp, Activity_RESULT_CANCELED, info);
            }
        }
    }

    if (anrApp != NULL) {
        mService->AppNotResponding(anrApp, r, this,
            "keyDispatchingTimedOut");
    }

    *result = TRUE;
}

AutoPtr<CActivityRecord> CActivityRecord::GetWaitingHistoryRecordLocked()
{
    // First find the real culprit...  if we are waiting
    // for another app to start, then we have paused dispatching
    // for this activity.
    AutoPtr<CActivityRecord> r = this;
    if (r->mWaitingVisible) {
        // Hmmm, who might we be waiting for?
        r = mStack->GetResumedActivity();
        if (r == NULL) {
            r = mStack->GetPausingActivity();
        }
        // Both of those null?  Fall back to 'this' again
        if (r == NULL) {
            r = this;
        }
    }

    return r;
}

ECode CActivityRecord::GetKeyDispatchingTimeout(
    /* [out] */ Millisecond64* timeout)
{
    Mutex::Autolock lock(mService->_m_syncLock);
    AutoPtr<CActivityRecord> r = GetWaitingHistoryRecordLocked();
    if (r == NULL || r->mApp == NULL
        || r->mApp->mInstrumentationClass == NULL) {
        *timeout = CActivityManagerService::KEY_DISPATCHING_TIMEOUT;
        return NOERROR;
    }

    *timeout = CActivityManagerService::INSTRUMENTATION_KEY_DISPATCHING_TIMEOUT;
    return NOERROR;
}

Int32 CActivityRecord::GetHashCode()
{
    return (Int32)this;
}


Boolean CActivityRecord::IsInterestingToUserLocked()
{
    return mVisible || mNowVisible || mState == ActivityState_PAUSING ||
        mState == ActivityState_RESUMED;
}
