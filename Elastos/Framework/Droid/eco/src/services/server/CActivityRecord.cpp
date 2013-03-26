
#include "server/CActivityRecord.h"
#include "server/AttributeCache.h"
#include "server/CActivityManagerService.h"
#include "os/Process.h"


CActivityRecord::CActivityRecord() :
    mRealActivity(NULL),
//    Taskaffinity = null;
//    Statenotneeded = false;
//    Basedir = null;
//    Resdir = null;
//    Datadir = null;
    mCapsuleName(NULL),
    mProcessName(NULL),
    mFullscreen(TRUE),
//    Ishomeactivity = false;
    mApp(NULL),
    mState(ActivityState_INITIALIZING),
    mFrontOfTask(FALSE),
//    launchFailed = false;
    mHaveState(FALSE),
    mStopped(FALSE),
    mDelayedResume(FALSE),
    mFinishing(FALSE),
    mConfigDestroy(FALSE),
//    keysPaused = false;
    mInHistory(FALSE),
//    persistent = false;
//    visible = true;
//    waitingVisible = false;
    mNowVisible(FALSE),
    mThumbnailNeeded(FALSE),
//    idle = false;
    mHasBeenLaunched(FALSE)
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
    return E_NOT_IMPLEMENTED;
}

ECode CActivityRecord::WindowsGone()
{
    return E_NOT_IMPLEMENTED;
}

ECode CActivityRecord::KeyDispatchingTimedOut(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CActivityRecord::GetKeyDispatchingTimeout(
    /* [out] */ Millisecond64* timeout)
{
    return E_NOT_IMPLEMENTED;
}

Int32 CActivityRecord::GetHashCode()
{
    return (Int32)this;
}
