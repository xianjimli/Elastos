
#include "server/ActivityStack.h"
#include "utils/EventLogTags.h"
#include "os/SystemClock.h"
#include "os/Process.h"
#include "os/Binder.h"
#include "view/WindowManagerPolicy.h"
#include <StringBuffer.h>
#include <Slogger.h>

using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

#define UNUSED(x) (void)x

const char* ActivityStack::TAG  = CActivityManagerService::TAG;
const Boolean ActivityStack::localLOGV = CActivityManagerService::localLOGV;
const Boolean ActivityStack::DEBUG_SWITCH = CActivityManagerService::DEBUG_SWITCH;
const Boolean ActivityStack::DEBUG_PAUSE = CActivityManagerService::DEBUG_PAUSE;
const Boolean ActivityStack::DEBUG_VISBILITY = CActivityManagerService::DEBUG_VISBILITY;
const Boolean ActivityStack::DEBUG_USER_LEAVING = CActivityManagerService::DEBUG_USER_LEAVING;
const Boolean ActivityStack::DEBUG_TRANSITION = CActivityManagerService::DEBUG_TRANSITION;
const Boolean ActivityStack::DEBUG_RESULTS = CActivityManagerService::DEBUG_RESULTS;
const Boolean ActivityStack::DEBUG_CONFIGURATION = CActivityManagerService::DEBUG_CONFIGURATION;
const Boolean ActivityStack::DEBUG_TASKS = CActivityManagerService::DEBUG_TASKS;
const Boolean ActivityStack::VALIDATE_TOKENS = CActivityManagerService::VALIDATE_TOKENS;
const Int32 ActivityStack::PAUSE_TIMEOUT;
const Int32 ActivityStack::LAUNCH_TIMEOUT;
const Millisecond64 ActivityStack::ACTIVITY_INACTIVE_RESET_TIME;
const Boolean ActivityStack::SHOW_APP_STARTING_PREVIEW;
const Int32 ActivityStack::FINISH_IMMEDIATELY;
const Int32 ActivityStack::FINISH_AFTER_PAUSE;
const Int32 ActivityStack::FINISH_AFTER_VISIBLE;

ActivityStack::ActivityStack(
    /* [in] */ CActivityManagerService* service,
    /* [in] */ IContext* context,
    /* [in] */ Boolean mainStack) :
    mInitialStartTime(0)
{
    mService = service;
    mContext = context;
    mMainStack = mainStack;

    AutoPtr<IApartmentHelper> helper;
    assert(SUCCEEDED(CApartmentHelper::AcquireSingleton((IApartmentHelper**)&helper)));
    assert(SUCCEEDED(helper->GetMainApartment((IApartment**)&mApartment))
            && (mApartment != NULL));

//    PowerManager pm =
//        (PowerManager)context.getSystemService(Context.POWER_SERVICE);
//    mGoingToSleep = pm.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, "ActivityManager-Sleep");
//    mLaunchingActivity = pm.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, "ActivityManager-Launch");
//    mLaunchingActivity.setReferenceCounted(false);
}

ICapsuleManager* ActivityStack::GetCapsuleManager()
{
    AutoPtr<IServiceManager> serviceManager;
    AutoPtr<ICapsuleManager> capsuleManager;
    Elastos::GetServiceManager((IServiceManager**)&serviceManager);
    assert(serviceManager != NULL);
    serviceManager->GetService(String("capsule"),
            (IInterface**)(ICapsuleManager**)&capsuleManager);
    assert(capsuleManager != NULL);
    return capsuleManager;
}

CActivityRecord* ActivityStack::GetTopRunningActivityLocked(
    /* [in] */ CActivityRecord* notTop)
{
    List<AutoPtr<CActivityRecord> >::ReverseIterator rit = mHistory.RBegin();
    while (rit != mHistory.REnd()) {
        CActivityRecord* r = *rit;
        if (!r->mFinishing && r != notTop) {
            return r;
        }
        ++rit;
    }
    return NULL;
}

CActivityRecord* ActivityStack::GetTopRunningNonDelayedActivityLocked(
    /* [in] */ CActivityRecord* notTop)
{
    List<AutoPtr<CActivityRecord> >::ReverseIterator rit = mHistory.RBegin();
    while (rit != mHistory.REnd()) {
        CActivityRecord* r = *rit;
        if (!r->mFinishing && !r->mDelayedResume && r != notTop) {
            return r;
        }
        ++rit;
    }
    return NULL;
}

/**
 * This is a simplified version of topRunningActivityLocked that provides a number of
 * optional skip-over modes.  It is intended for use with the ActivityController hook only.
 *
 * @param token If non-null, any history records matching this token will be skipped.
 * @param taskId If non-zero, we'll attempt to skip over records with the same task ID.
 *
 * @return Returns the HistoryRecord of the next activity on the stack.
 */
CActivityRecord* ActivityStack::GetTopRunningActivityLocked(
    /* [in] */ IBinder* token,
    /* [in] */ Int32 taskId)
{
    List<AutoPtr<CActivityRecord> >::ReverseIterator rit = mHistory.RBegin();
    while (rit != mHistory.REnd()) {
        CActivityRecord* r = *rit;
        // Note: the taskId check depends on real taskId fields being non-zero
        if (!r->mFinishing &&
                (token != (IBinder*)r) && (taskId != r->mTask->mTaskId)) {
            return r;
        }
        ++rit;
    }
    return NULL;
}

Int32 ActivityStack::GetIndexOfTokenLocked(
    /* [in] */ IBinder* token)
{
    Int32 i = mHistory.GetSize() - 1;
    List<AutoPtr<CActivityRecord> >::ReverseIterator
        rit = mHistory.RBegin();

    // convert the token to an entry in the history.
    Int32 index = -1;
    for (; rit != mHistory.REnd() && i >= 0; rit++, i--) {
        CActivityRecord* r = *rit;
        if ((IBinder*)r == token) {
            index = i;
            break;
        }
    }

    return index;
}

Boolean ActivityStack::UpdateLRUListLocked(
    /* [in] */ CActivityRecord* r)
{
    Boolean hadit = FALSE;
    List<AutoPtr<CActivityRecord> >::Iterator it;

    it = Find(mLRUActivities.Begin(), mLRUActivities.End(),
            AutoPtr<CActivityRecord>(r));
    if (it != mLRUActivities.End()) {
        mLRUActivities.Erase(it);
        hadit = TRUE;
    }
    mLRUActivities.PushBack(r);
    return hadit;
}

/**
 * Returns the top activity in any existing task matching the given
 * Intent.  Returns null if no such task is found.
 */
CActivityRecord* ActivityStack::FindTaskLocked(
    /* [in] */ IIntent* intent,
    /* [in] */ IActivityInfo* info)
{
    AutoPtr<IComponentName> cls;
    intent->GetComponent((IComponentName**)&cls);
    String cname, target;
    info->GetCapsuleName(&cname);
    info->GetTargetActivity(&target);
    if (!target.IsNull()) {
        CComponentName::New(cname, target, (IComponentName**)&cls);
    }

    TaskRecord* cp = NULL;

    List<AutoPtr<CActivityRecord> >::ReverseIterator rit;
    for (rit = mHistory.RBegin(); rit != mHistory.REnd(); ++rit) {
        CActivityRecord* r = *rit;
        if (!r->mFinishing && r->mTask != cp
                && r->mLaunchMode != ActivityInfo_LAUNCH_SINGLE_INSTANCE) {
            cp = r->mTask;
            //Slog.i(TAG, "Comparing existing cls=" + r.task.intent.getComponent().flattenToShortString()
            //        + "/aff=" + r.task.affinity + " to new cls="
            //        + intent.getComponent().flattenToShortString() + "/aff=" + taskAffinity);
            AutoPtr<IComponentName> component;
            Boolean isEqual;
            if (!r->mTask->mAffinity.IsNull()) {
                String task;
                info->GetTaskAffinity(&task);
                if (!r->mTask->mAffinity.Compare(task)) {
                    //Slog.i(TAG, "Found matching affinity!");
                    return r;
                }
            }
            else if (r->mTask->mIntent != NULL
                    && (r->mTask->mIntent->GetComponent((IComponentName**)&component),
                        component->Equals(cls, &isEqual), isEqual)) {
                //Slog.i(TAG, "Found matching class!");
                //dump();
                //Slog.i(TAG, "For Intent " + intent + " bringing to top: " + r.intent);
                return r;
            }
            else if (r->mTask->mAffinityIntent != NULL
                    && (r->mTask->mAffinityIntent->GetComponent((IComponentName**)&component),
                        component->Equals(cls, &isEqual), isEqual)) {
                //Slog.i(TAG, "Found matching class!");
                //dump();
                //Slog.i(TAG, "For Intent " + intent + " bringing to top: " + r.intent);
                return r;
            }
        }
    }

    return NULL;
}

/**
 * Returns the first activity (starting from the top of the stack) that
 * is the same as the given activity.  Returns null if no such activity
 * is found.
 */
CActivityRecord* ActivityStack::FindActivityLocked(
    /* [in] */ IIntent* intent,
    /* [in] */ IActivityInfo* info)
{
    AutoPtr<IComponentName> cls;
    intent->GetComponent((IComponentName**)&cls);
    String cname, target;
    info->GetCapsuleName(&cname);
    info->GetTargetActivity(&target);
    if (!target.IsNull()) {
        CComponentName::New(cname, target, (IComponentName**)&cls);
    }

    List<AutoPtr<CActivityRecord> >::ReverseIterator rit;
    for (rit = mHistory.RBegin(); rit != mHistory.REnd(); ++rit) {
        CActivityRecord* r = *rit;
        if (!r->mFinishing) {
            AutoPtr<IComponentName> component;
            Boolean isEqual;
            r->mIntent->GetComponent((IComponentName**)&component);
            if (component->Equals(cls, &isEqual), isEqual) {
                //Slog.i(TAG, "Found matching class!");
                //dump();
                //Slog.i(TAG, "For Intent " + intent + " bringing to top: " + r.intent);
                return r;
            }
        }
    }

    return NULL;
}

ECode ActivityStack::RealStartActivityLocked(
    /* [in] */ CActivityRecord* r,
    /* [in] */ ProcessRecord* app,
    /* [in] */ Boolean andResume,
    /* [in] */ Boolean checkConfig,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)
    r->StartFreezingScreenLocked(app, 0);
    mService->mWindowManager->SetAppVisibility(r, TRUE);

    // Have the window manager re-evaluate the orientation of
    // the screen based on the new activity order.  Note that
    // as a result of this, it can call back into the activity
    // manager with a new orientation.  We don't care about that,
    // because the activity is not currently running so we are
    // just restarting it anyway.
    if (checkConfig) {
        AutoPtr<IConfiguration> config;
        mService->mWindowManager->UpdateOrientationFromAppTokens(
                mService->mConfiguration,
                r->MayFreezeScreenLocked(app) ? r : NULL,
                (IConfiguration**)&config);
        mService->UpdateConfigurationLocked(config, r);
    }

    r->mApp = app;

    if (localLOGV) {
        String arDes;
        r->GetDescription(&arDes);
        Slogger::V(TAG, StringBuffer("Launching: ") + arDes);
    }

    List<AutoPtr<CActivityRecord> >::Iterator it = Find(
            app->mActivities.Begin(), app->mActivities.End(),
            AutoPtr<CActivityRecord>(r));
    if (it == app->mActivities.End()) {
        app->mActivities.PushBack(r);
    }
    mService->UpdateLRUProcessLocked(app, TRUE, TRUE);

//    try {
    if (app->mAppApartment == NULL) {
        return E_REMOTE_EXCEPTION;
    }
    AutoPtr<IObjectContainer> results;
    AutoPtr<IObjectContainer> newIntents;
    if (andResume) {
        if (r->mResults != NULL && r->mResults->GetSize() > 0) {
            CParcelableObjectContainer::New((IObjectContainer**)&results);
            List<ActivityResult*>::Iterator it;
            for(it = r->mResults->Begin(); it != r->mResults->End(); ++it) {
                results->Add((*it)->mResultInfo);
            }
        }
        if (r->mNewIntents != NULL && r->mNewIntents->GetSize() > 0) {
            CParcelableObjectContainer::New((IObjectContainer**)&newIntents);
            List<AutoPtr<IIntent> >::Iterator it;
            for(it = r->mNewIntents->Begin(); it != r->mNewIntents->End(); ++it) {
                results->Add((IIntent*)*it);
            }
        }
    }
    if (DEBUG_SWITCH) {
        String arDes, bdDes;
        r->GetDescription(&arDes);
        r->mIcicle->GetDescription(&bdDes);
        Slogger::V(TAG, "Launching: " + arDes
            + " icicle=" + bdDes
//            + " with results=" + results + " newIntents=" + newIntents
            + " andResume=" + andResume);
    }
    if (andResume) {
//        EventLog.writeEvent(EventLogTags.AM_RESTART_ACTIVITY,
//                System.identityHashCode(r),
//                r.task.taskId, r.shortComponentName);
    }
    if (r->mIsHomeActivity) {
        mService->mHomeProcess = app;
    }
    AutoPtr<IComponentName> component;
    r->mIntent->GetComponent((IComponentName**)&component);
    String cname;
    component->GetCapsuleName(&cname);
    mService->EnsurePackageDexOpt(cname);
    ECode ec = app->mAppApartment->ScheduleLaunchActivity(
            r->mIntent, (IBinder*)(CActivityRecord*)r,
            0 /* System.identityHashCode(r) */,
            r->mInfo, r->mIcicle, results, newIntents, !andResume,
            mService->IsNextTransitionForward());

    Int32 flags;
    app->mInfo->GetFlags(&flags);
    if ((flags & ApplicationInfo_FLAG_CANT_SAVE_STATE) != 0) {
        // This may be a heavy-weight process!  Note that the package
        // manager will ensure that only activity can run in the main
        // process of the .apk, which is the only thing that will be
        // considered heavy-weight.
        String cName;
        app->mInfo->GetCapsuleName(&cName);
        if (app->mProcessName.Equals(cName)) {

//            if (mService.mHeavyWeightProcess != null
//                    && mService.mHeavyWeightProcess != app) {
//                Log.w(TAG, "Starting new heavy weight process " + app
//                        + " when already running "
//                        + mService.mHeavyWeightProcess);
//            }
//            mService.mHeavyWeightProcess = app;
//            Message msg = mService.mHandler.obtainMessage(
//                    ActivityManagerService.POST_HEAVY_NOTIFICATION_MSG);
//            msg.obj = r;
//            mService.mHandler.sendMessage(msg);
        }
    }

#if defined(_DEBUG)
    if (SUCCEEDED(ec)) {
        AutoPtr<IComponentName> component;
        String capsuleName, className;

        r->mIntent->GetComponent((IComponentName**)&component);
        component->GetCapsuleName(&capsuleName);
        component->GetClassName(&className);
        Slogger::D(TAG, StringBuffer("Schedule launch ") + capsuleName + "." +
                className + " activity.");

    }
#endif
//    } catch (RemoteException e) {
//        if (r.launchFailed) {
//            // This is the second time we failed -- finish activity
//            // and give up.
//            Slog.e(TAG, "Second failure launching "
//                  + r.intent.getComponent().flattenToShortString()
//                  + ", giving up", e);
//            mService.appDiedLocked(app, app.pid, app.thread);
//            requestFinishActivityLocked(r, Activity.RESULT_CANCELED, null,
//                    "2nd-crash");
//            return false;
//        }
//
//        // This is the first time we failed -- restart process and
//        // retry.
//        app.activities.remove(r);
//        throw e;
//    }

    r->mLaunchFailed = FALSE;
    if (UpdateLRUListLocked(r)) {
        String arDes;
        r->GetDescription(&arDes);
        Slogger::W(TAG, StringBuffer("Activity ") + arDes
              + " being launched, but already in LRU list");
    }

    if (andResume) {
        // As part of the process of launching, ActivityThread also performs
        // a resume.
        r->mState = ActivityState_RESUMED;
        r->mIcicle = NULL;
        r->mHaveState = FALSE;
        r->mStopped = FALSE;
        mResumedActivity = r;
        r->mTask->TouchActiveTime();
        CompleteResumeLocked(r);
        PauseIfSleepingLocked();
    } else {
        // This activity is not starting in the resumed state... which
        // should look like we asked it to pause+stop (but remain visible),
        // and it has done so and reported back the current icicle and
        // other state.
        r->mState = ActivityState_STOPPED;
        r->mStopped = TRUE;
    }

    // Launch the new version setup screen if needed.  We do this -after-
    // launching the initial activity (that is, home), so that it can have
    // a chance to initialize itself while in the background, making the
    // switch back to it faster and look better.
    if (mMainStack) {
        mService->StartSetupActivityLocked();
    }

    *succeeded = TRUE;
    return NOERROR;
}

ECode ActivityStack::StartSpecificActivityLocked(
    /* [in] */ CActivityRecord* r,
    /* [in] */ Boolean andResume,
    /* [in] */ Boolean checkConfig)
{
    // Is this activity's application already running?
    AutoPtr<IApplicationInfo> appInfo;
    r->mInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
    Int32 uid;
    appInfo->GetUid(&uid);
    ProcessRecord* app = mService->GetProcessRecordLocked(r->mProcessName, uid);

    if (r->mLaunchTime == 0) {
        r->mLaunchTime = SystemClock::GetUptimeMillis();
        if (mInitialStartTime == 0) {
            mInitialStartTime = r->mLaunchTime;
        }
    }
    else if (mInitialStartTime == 0) {
        mInitialStartTime = SystemClock::GetUptimeMillis();
    }

    if (app != NULL && app->mAppApartment != NULL) {
        Boolean succeeded;
        if (SUCCEEDED(RealStartActivityLocked(r, app,
                andResume, checkConfig, &succeeded))) {
            return NOERROR;
        }
        else {
            String acDes;
            AutoPtr<IComponentName> component;
            r->mIntent->GetComponent((IComponentName**)&component);
            component->FlattenToShortString(&acDes);
            Slogger::W(TAG, StringBuffer("Exception when starting activity ") + acDes);
    }

        // If a dead object exception was thrown -- fall through to
        // restart the application.
    }

    AutoPtr<IComponentName> componentName;
    r->mIntent->GetComponent((IComponentName**)&componentName);
    mService->StartProcessLocked(r->mProcessName, appInfo,
            TRUE, 0, "activity", componentName, FALSE);
    return NOERROR;
}

void ActivityStack::PauseIfSleepingLocked()
{
    if (mService->mSleeping || mService->mShuttingDown) {
//        if (!mGoingToSleep.isHeld()) {
//            mGoingToSleep.acquire();
//            if (mLaunchingActivity.isHeld()) {
//                mLaunchingActivity.release();
//                mService.mHandler.removeMessages(LAUNCH_TIMEOUT_MSG);
//            }
//        }

        // If we are not currently pausing an activity, get the current
        // one to pause.  If we are pausing one, we will just let that stuff
        // run and release the wake lock when all done.
        if (mPausingActivity == NULL) {
            if (DEBUG_PAUSE) {
                Slogger::V(TAG, StringBuffer("Sleep needs to pause..."));
            }
            if (DEBUG_USER_LEAVING) {
                Slogger::V(TAG, StringBuffer("Sleep => pause with userLeaving=false"));
            }
            StartPausingLocked(FALSE, TRUE);
        }
    }
}

ECode ActivityStack::StartPausingLocked(
    /* [in] */ Boolean userLeaving,
    /* [in] */ Boolean uiSleeping)
{
    if (mPausingActivity != NULL) {
        String arDes;
        mPausingActivity->GetDescription(&arDes);
        Slogger::E(TAG, StringBuffer("Trying to pause when pause is already pending for ")
                + arDes);
    }
    AutoPtr<CActivityRecord> prev = mResumedActivity;
    if (prev == NULL) {
        Slogger::E(TAG, StringBuffer("Trying to pause when nothing is resumed"));
        ResumeTopActivityLocked(NULL);
        return NOERROR;
    }
    if (DEBUG_PAUSE) {
        String arDes;
        prev->GetDescription(&arDes);
        Slogger::V(TAG, StringBuffer("Start pausing: ") + arDes);
    }
    mResumedActivity = NULL;
    mPausingActivity = prev;
    mLastPausedActivity = prev;
    prev->mState = ActivityState_PAUSING;
    prev->mTask->TouchActiveTime();

    mService->UpdateCpuStats();

    if (prev->mApp != NULL && prev->mApp->mAppApartment != NULL) {
        if (DEBUG_PAUSE) {
            String arDes;
            prev->GetDescription(&arDes);
            Slogger::V(TAG, StringBuffer("Enqueueing pending pause: ") + arDes);
        }
//        EventLog.writeEvent(EventLogTags.AM_PAUSE_ACTIVITY,
//                System.identityHashCode(prev),
//                prev.shortComponentName);
        if (SUCCEEDED(prev->mApp->mAppApartment->SchedulePauseActivity(prev,
                prev->mFinishing, userLeaving, prev->mConfigChangeFlags))) {
            if (mMainStack) {
                mService->UpdateUsageStats(prev, FALSE);
            }
        }
        else {
            // Ignore exception, if process died other code will cleanup.
            Slogger::W(TAG, "Exception thrown during pause");
            mPausingActivity = NULL;
            mLastPausedActivity = NULL;
        }
    } else {
        mPausingActivity = NULL;
        mLastPausedActivity = NULL;
    }

    // If we are not going to sleep, we want to ensure the device is
    // awake until the next activity is started.
    if (!mService->mSleeping && !mService->mShuttingDown) {
//        mLaunchingActivity.acquire();
//        if (!mHandler.hasMessages(LAUNCH_TIMEOUT_MSG)) {
//            // To be safe, don't allow the wake lock to be held for too long.
//            Message msg = mHandler.obtainMessage(LAUNCH_TIMEOUT_MSG);
//            mHandler.sendMessageDelayed(msg, LAUNCH_TIMEOUT);
//        }
    }

    if (mPausingActivity != NULL) {
        // Have the window manager pause its key dispatching until the new
        // activity has started.  If we're pausing the activity just because
        // the screen is being turned off and the UI is sleeping, don't interrupt
        // key dispatch; the same activity will pick it up again on wakeup.
        if (!uiSleeping) {
            prev->PauseKeyDispatchingLocked();
        } else {
            if (DEBUG_PAUSE) {
                Slogger::V(TAG, "Key dispatch not paused for screen off");
            }
        }

        // Schedule a pause timeout in case the app doesn't respond.
        // We don't give it much time because this directly impacts the
        // responsiveness seen by the user.
//        Message msg = mHandler.obtainMessage(PAUSE_TIMEOUT_MSG);
//        msg.obj = prev;
//        mHandler.sendMessageDelayed(msg, PAUSE_TIMEOUT);
        ECode (STDCALL ActivityStack::*pHandlerFunc)(IBinder* token);
        pHandlerFunc = &ActivityStack::HandlePauseTimeout;
        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        params->WriteInterfacePtr((IBinder*)prev);
        mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
        if (DEBUG_PAUSE) Slogger::V(TAG, String("Waiting for pause to complete..."));
    } else {
        // This activity failed to schedule the
        // pause, so just treat it as being paused now.
        if (DEBUG_PAUSE) {
            Slogger::V(TAG, "Activity not running, resuming next.");
        }
        ResumeTopActivityLocked(NULL);
    }

    return NOERROR;
}

ECode ActivityStack::ActivityPaused(
    /* [in] */ IBinder* token,
    /* [in] */ IBundle* icicle,
    /* [in] */ Boolean timeout)
{
    if (DEBUG_PAUSE) {
        String tokenDes, bdDes;
        token->GetDescription(&tokenDes);
        icicle->GetDescription(&bdDes);
        Slogger::V(TAG, StringBuffer("Activity paused: token=") + tokenDes
                + ", icicle=" + bdDes + ", timeout=" + timeout);
    }

    AutoPtr<CActivityRecord> r;

    mService->Lock();
    Int32 index = GetIndexOfTokenLocked(token);
    if (index >= 0) {
        r = mHistory[index];
        if (!timeout) {
            r->mIcicle = icicle;
            r->mHaveState = TRUE;
        }
//        mHandler.removeMessages(PAUSE_TIMEOUT_MSG, r);
        if (mPausingActivity == r) {
            r->mState = ActivityState_PAUSED;
            CompletePauseLocked();
        } else {
//            EventLog.writeEvent(EventLogTags.AM_FAILED_TO_PAUSE,
//                    System.identityHashCode(r), r.shortComponentName,
//                    mPausingActivity != null
//                        ? mPausingActivity.shortComponentName : "(none)");
        }
    }
    mService->Unlock();
    return NOERROR;
}

void ActivityStack::CompletePauseLocked()
{
    AutoPtr<CActivityRecord> prev = mPausingActivity;
    if (DEBUG_PAUSE) {
        String arDes;
        prev->GetDescription(&arDes);
        Slogger::V(TAG, StringBuffer("Complete pause: ") + arDes);
    }

    if (prev != NULL) {
        if (prev->mFinishing) {
            if (DEBUG_PAUSE) {
                String arDes;
                prev->GetDescription(&arDes);
                Slogger::V(TAG, StringBuffer("Executing finish of activity: ") + arDes);
            }
            AutoPtr<CActivityRecord> r;
            FinishCurrentActivityLocked(prev,
                    FINISH_AFTER_VISIBLE, (CActivityRecord**)&r);
            prev = r;
        } else if (prev->mApp != NULL) {
            if (DEBUG_PAUSE) {
                String arDes;
                prev->GetDescription(&arDes);
                Slogger::V(TAG, StringBuffer("Enqueueing pending stop: ") + arDes);
            }
            if (prev->mWaitingVisible) {
                prev->mWaitingVisible = FALSE;
                mWaitingVisibleActivities.Remove(prev);
                if (DEBUG_SWITCH || DEBUG_PAUSE) {
                    String arDes;
                    prev->GetDescription(&arDes);
                    Slogger::V(TAG, StringBuffer("Complete pause, no longer waiting: ")
                            + arDes);
                }
            }
            if (prev->mConfigDestroy) {
                // The previous is being paused because the configuration
                // is changing, which means it is actually stopping...
                // To juggle the fact that we are also starting a new
                // instance right now, we need to first completely stop
                // the current instance before starting the new one.
                if (DEBUG_PAUSE) {
                    String arDes;
                    prev->GetDescription(&arDes);
                    Slogger::V(TAG, StringBuffer("Destroying after pause: ") + arDes);
                }
                DestroyActivityLocked(prev, TRUE);
            } else {
                mStoppingActivities.PushBack(prev);
                if (mStoppingActivities.GetSize() > 3) {
                    // If we already have a few activities waiting to stop,
                    // then give up on things going idle and start clearing
                    // them out.
                    if (DEBUG_PAUSE) Slogger::V(TAG, String("To many pending stops, forcing idle"));
                    ECode (STDCALL ActivityStack::*pHandlerFunc)(IBinder* token);
                    pHandlerFunc = &ActivityStack::HandleIdleNow;
                    AutoPtr<IParcel> params;
                    CCallbackParcel::New((IParcel**)&params);
                    params->WriteInterfacePtr(NULL);
                    mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
                    }
            }
        } else {
            if (DEBUG_PAUSE) {
                String arDes;
                prev->GetDescription(&arDes);
                Slogger::V(TAG, StringBuffer("App died during pause, not stopping: ") + arDes);
            }
            prev = NULL;
        }
        mPausingActivity = NULL;
    }

    if (!mService->mSleeping && !mService->mShuttingDown) {
        ResumeTopActivityLocked(prev);
    }
//    else {
//        if (mGoingToSleep.isHeld()) {
//            mGoingToSleep.release();
//        }
//        if (mService.mShuttingDown) {
//            mService.notifyAll();
//        }
//    }
//
    if (prev != NULL) {
        prev->ResumeKeyDispatchingLocked();
        prev->mCpuTimeAtResume = 0; // reset it
    }
//
//    if (prev.app != null && prev.cpuTimeAtResume > 0
//            && mService.mBatteryStatsService.isOnBattery()) {
//        long diff = 0;
//        synchronized (mService.mProcessStatsThread) {
//            diff = mService.mProcessStats.getCpuTimeForPid(prev.app.pid)
//                    - prev.cpuTimeAtResume;
//        }
//        if (diff > 0) {
//            BatteryStatsImpl bsi = mService.mBatteryStatsService.getActiveStatistics();
//            synchronized (bsi) {
//                BatteryStatsImpl.Uid.Proc ps =
//                        bsi.getProcessStatsLocked(prev.info.applicationInfo.uid,
//                        prev.info.packageName);
//                if (ps != null) {
//                    ps.addForegroundTimeLocked(diff);
//                }
//            }
//        }
//    }
}

/**
 * Once we know that we have asked an application to put an activity in
 * the resumed state (either by launching it or explicitly telling it),
 * this function updates the rest of our state to match that fact.
 */
void ActivityStack::CompleteResumeLocked(
    /* [in] */ CActivityRecord* next)
{
    next->mIdle = FALSE;
    next->mResults = NULL;
    next->mNewIntents = NULL;

    // schedule an idle timeout in case the app doesn't do it for us.
//    Message msg = mHandler.obtainMessage(IDLE_TIMEOUT_MSG);
//    msg.obj = next;
//    mHandler.sendMessageDelayed(msg, IDLE_TIMEOUT);
    ECode (STDCALL ActivityStack::*pHandlerFunc)(IBinder* token);
    pHandlerFunc = &ActivityStack::HandleIdleTimeout;
    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr((IBinder*)next);
    mApartment->PostCppCallbackDelayed((Handle32)this, *(Handle32*)&pHandlerFunc, 
        params, 0, IDLE_TIMEOUT);
    if (FALSE) {
        // The activity was never told to pause, so just keep
        // things going as-is.  To maintain our own state,
        // we need to emulate it coming back and saying it is
        // idle.
//        msg = mHandler.obtainMessage(IDLE_NOW_MSG);
//        msg.obj = next;
//        mHandler.sendMessage(msg);
        AutoPtr<IParcel> nowparams;
        CCallbackParcel::New((IParcel**)&nowparams);
        nowparams->WriteInterfacePtr((IBinder*)next);
        mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, 
            nowparams, 0); 
    }

    if (mMainStack) {
        mService->ReportResumedActivityLocked(next);
    }

    next->mThumbnail = NULL;
    if (mMainStack) {
        mService->SetFocusedActivityLocked(next);
    }
    next->ResumeKeyDispatchingLocked();
    EnsureActivitiesVisibleLocked(NULL, 0);
    mService->mWindowManager->ExecuteAppTransition();
    mNoAnimActivities.Clear();

    // Mark the point when the activity is resuming
    // TODO: To be more accurate, the mark should be before the onCreate,
    //       not after the onResume. But for subsequent starts, onResume is fine.
    if (next->mApp != NULL) {
//        synchronized (mService.mProcessStatsThread) {
//            next.cpuTimeAtResume = mService.mProcessStats.getCpuTimeForPid(next.app.pid);
//        }
    } else {
        next->mCpuTimeAtResume = 0; // Couldn't get the cpu time of process
    }
}

/**
 * Make sure that all activities that need to be visible (that is, they
 * currently can be seen by the user) actually are.
 */
void ActivityStack::EnsureActivitiesVisibleLocked(
    /* [in] */ CActivityRecord* top,
    /* [in] */ CActivityRecord* starting,
    /* [in] */ const char* onlyThisProcess,
    /* [in] */ Int32 configChanges)
{
    if (DEBUG_VISBILITY) {
        String arDes;
        top->GetDescription(&arDes);
        Slogger::V(TAG, StringBuffer("ensureActivitiesVisible behind ")
                + arDes + " configChanges=" + configChanges);
    }

    // If the top activity is not fullscreen, then we need to
    // make sure any activities under it are now visible.
    List<AutoPtr<CActivityRecord> >::ReverseIterator rit = mHistory.RBegin();
    while ((*rit).Get() != top) {
        ++rit;
    }
    CActivityRecord* r;
    Boolean behindFullscreen = FALSE;
    for (; rit != mHistory.REnd(); ++rit) {
        r = (CActivityRecord*)*rit;
        if (DEBUG_VISBILITY) {
            String arDes;
            r->GetDescription(&arDes);
            Slogger::V(TAG, StringBuffer("Make visible? ") + arDes
                    + " finishing=" + r->mFinishing + " state=" + r->mState);
    }
        if (r->mFinishing) {
            continue;
        }

        Boolean doThisProcess = onlyThisProcess == NULL
                || r->mProcessName.Equals(onlyThisProcess);

        // First: if this is not the current activity being started, make
        // sure it matches the current configuration.
        if (r != starting && doThisProcess) {
            EnsureActivityConfigurationLocked(r, 0);
        }

        if (r->mApp == NULL || r->mApp->mAppApartment == NULL) {
            if (onlyThisProcess == NULL
                    || r->mProcessName.Equals(onlyThisProcess)) {
                // This activity needs to be visible, but isn't even
                // running...  get it started, but don't resume it
                // at this point.
                if (DEBUG_VISBILITY) {
                    String arDes;
                    r->GetDescription(&arDes);
                    Slogger::V(TAG,
                            StringBuffer("Start and freeze screen for ") + arDes);
                }
                if (r != starting) {
                    r->StartFreezingScreenLocked(r->mApp, configChanges);
                }
                if (!r->mVisible) {
                    if (DEBUG_VISBILITY) {
                        String arDes;
                        r->GetDescription(&arDes);
                        Slogger::V(TAG,
                                StringBuffer("Starting and making visible: ") + arDes);
                    }
                    mService->mWindowManager->SetAppVisibility(r, TRUE);
                }
                if (r != starting) {
                    StartSpecificActivityLocked(r, FALSE, FALSE);
                }
            }
        } else if (r->mVisible) {
            // If this activity is already visible, then there is nothing
            // else to do here.
            if (DEBUG_VISBILITY) {
                String arDes;
                r->GetDescription(&arDes);
                Slogger::V(TAG,
                        StringBuffer("Skipping: already visible at ") + arDes);
            }
            r->StopFreezingScreenLocked(FALSE);
        } else if (onlyThisProcess == NULL) {
            // This activity is not currently visible, but is running.
            // Tell it to become visible.
            r->mVisible = TRUE;
            if (r->mState != ActivityState_RESUMED && r != starting) {
                // If this activity is paused, tell it
                // to now show its window.
                if (DEBUG_VISBILITY) {
                    String arDes;
                    r->GetDescription(&arDes);
                    Slogger::V(TAG,
                            StringBuffer("Making visible and scheduling visibility: ")
                            + arDes);
                }
//                try {
                    mService->mWindowManager->SetAppVisibility(r, TRUE);
                    r->mApp->mAppApartment->ScheduleWindowVisibility(r, TRUE);
                    r->StopFreezingScreenLocked(FALSE);
//                } catch (Exception e) {
//                    // Just skip on any failure; we'll make it
//                    // visible when it next restarts.
//                    Slog.w(TAG, "Exception thrown making visibile: "
//                            + r.intent.getComponent(), e);
//                }
            }
        }

        // Aggregate current change flags.
        configChanges |= r->mConfigChangeFlags;

        if (r->mFullscreen) {
            // At this point, nothing else needs to be shown
            if (DEBUG_VISBILITY) {
                String arDes;
                r->GetDescription(&arDes);
                Slogger::V(TAG, StringBuffer("Stopping: fullscreen at ") + arDes);

            }
            behindFullscreen = TRUE;
            ++rit;
            break;
        }
    }

    // Now for any activities that aren't visible to the user, make
    // sure they no longer are keeping the screen frozen.
    while (rit != mHistory.REnd()) {
        r = (CActivityRecord*)*rit;
        if (DEBUG_VISBILITY) {
            String arDes;
            r->GetDescription(&arDes);
            Slogger::V(TAG, StringBuffer("Make invisible? ") + arDes
                + " finishing=" + r->mFinishing
                + " state=" + r->mState
                + " behindFullscreen=" + behindFullscreen);

        }
        if (!r->mFinishing) {
            if (behindFullscreen) {
                if (r->mVisible) {
                    if (DEBUG_VISBILITY) {
                        String arDes;
                        r->GetDescription(&arDes);
                        Slogger::V(TAG, StringBuffer("Making invisible: ") + arDes);
                    }
                    r->mVisible = FALSE;
//                    try {
                        mService->mWindowManager->SetAppVisibility(r, FALSE);
                        if ((r->mState == ActivityState_STOPPING
                                || r->mState == ActivityState_STOPPED)
                                && r->mApp != NULL && r->mApp->mAppApartment != NULL) {
                            if (DEBUG_VISBILITY) {
                                String arDes;
                                r->GetDescription(&arDes);
                                Slogger::V(TAG,
                                        StringBuffer("Scheduling invisibility: ") + arDes);
                            }
                            r->mApp->mAppApartment->ScheduleWindowVisibility(r, FALSE);
                        }
//                    } catch (Exception e) {
//                        // Just skip on any failure; we'll make it
//                        // visible when it next restarts.
//                        Slog.w(TAG, "Exception thrown making hidden: "
//                                + r.intent.getComponent(), e);
//                    }
                } else {
                    if (DEBUG_VISBILITY) {
                        String arDes;
                        r->GetDescription(&arDes);
                        Slogger::V(TAG,
                                StringBuffer("Already invisible: ") + arDes);
                    }
                }
            } else if (r->mFullscreen) {
                if (DEBUG_VISBILITY) {
                    String arDes;
                    r->GetDescription(&arDes);
                    Slogger::V(TAG,
                            StringBuffer("Now behindFullscreen: ") + arDes);
                }
                behindFullscreen = TRUE;
            }
        }
        ++rit;
    }
}

/**
 * Version of ensureActivitiesVisible that can easily be called anywhere.
 */
void ActivityStack:: EnsureActivitiesVisibleLocked(
    /* [in] */ CActivityRecord* starting,
    /* [in] */ Int32 configChanges)
{
    CActivityRecord* r = GetTopRunningActivityLocked(NULL);
    if (r != NULL) {
        EnsureActivitiesVisibleLocked(r, starting, NULL, configChanges);
    }
}

/**
 * Ensure that the top activity in the stack is resumed.
 *
 * @param prev The previously resumed activity, for when in the process
 * of pausing; can be null to call from elsewhere.
 *
 * @return Returns true if something is being resumed, or false if
 * nothing happened.
 */
Boolean ActivityStack::ResumeTopActivityLocked(
    /* [in] */ CActivityRecord* prev)
{
    // Find the first activity that is not finishing.
    AutoPtr<CActivityRecord> next = GetTopRunningActivityLocked(NULL);

    // Remember how we'll process this pause/resume situation, and ensure
    // that the state is reset however we wind up proceeding.
    Boolean userLeaving = mUserLeaving;
    mUserLeaving = FALSE;

    if (next == NULL) {
        // There are no more activities!  Let's just start up the
        // Launcher...
        if (mMainStack) {
            return mService->StartHomeActivityLocked();
        }
    }

    next->mDelayedResume = FALSE;

    // If the top activity is the resumed one, nothing to do.
    if (mResumedActivity == next && next->mState == ActivityState_RESUMED) {
        // Make sure we have executed any pending transitions, since there
        // should be nothing left to do at this point.
        mService->mWindowManager->ExecuteAppTransition();
        mNoAnimActivities.Clear();
        return FALSE;
    }

    // If we are sleeping, and there is no resumed activity, and the top
    // activity is paused, well that is the state we want.
    if ((mService->mSleeping || mService->mShuttingDown)
            && mLastPausedActivity == next && next->mState == ActivityState_PAUSED) {
        // Make sure we have executed any pending transitions, since there
        // should be nothing left to do at this point.
        mService->mWindowManager->ExecuteAppTransition();
        mNoAnimActivities.Clear();
        return FALSE;
    }

    // The activity may be waiting for stop, but that is no longer
    // appropriate for it.
    mStoppingActivities.Remove(next);
    mWaitingVisibleActivities.Remove(next);

    if (DEBUG_SWITCH) {
        String arDes;
        next->GetDescription(&arDes);
        Slogger::V(TAG, StringBuffer("Resuming ") + arDes);
    }

    // If we are currently pausing an activity, then don't do anything
    // until that is done.
    if (mPausingActivity != NULL) {
        if (DEBUG_SWITCH) {
            String arDes;
            mPausingActivity->GetDescription(&arDes);
            Slogger::V(TAG, "Skip resume: pausing=" + arDes);
        }
        return FALSE;
    }

    // Okay we are now going to start a switch, to 'next'.  We may first
    // have to pause the current activity, but this is an important point
    // where we have decided to go to 'next' so keep track of that.
    // XXX "App Redirected" dialog is getting too many false positives
    // at this point, so turn off for now.
    if (0) {
        if (mLastStartedActivity != NULL && !mLastStartedActivity->mFinishing) {
            Int32 now = SystemClock::GetUptimeMillis();
            const Boolean inTime = mLastStartedActivity->mStartTime != 0
                    && (mLastStartedActivity->mStartTime + START_WARN_TIME) >= now;
            AutoPtr<IApplicationInfo> appInfo;
            mLastStartedActivity->mInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
            Int32 uid;
            appInfo->GetUid(&uid);
            const Int32 lastUid = uid;
            appInfo->Release();
            next->mInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
            appInfo->GetUid(&uid);
            const Int32 nextUid = uid;
            Int32 permission;
            mService->CheckPermission(
                            String("elastos.permission.STOP_APP_SWITCHES")/*android.Manifest.permission.STOP_APP_SWITCHES*/,
                            -1, next->mLaunchedFromUid, &permission);
            if (inTime && lastUid != nextUid
                    && lastUid != next->mLaunchedFromUid
                    && permission != CapsuleManager_PERMISSION_GRANTED) {
                mService->ShowLaunchWarningLocked(mLastStartedActivity, next);
            }
            else {
                next->mStartTime = now;
                mLastStartedActivity = next;
            }
        }
        else {
            next->mStartTime = SystemClock::GetUptimeMillis();
            mLastStartedActivity = next;
        }
    }

    // We need to start pausing the current activity so the top one
    // can be resumed...
    if (mResumedActivity != NULL) {
        if (DEBUG_SWITCH) {
            Slogger::V(TAG, StringBuffer("Skip resume: need to start pausing"));
        }
        StartPausingLocked(userLeaving, FALSE);
        return TRUE;
    }

    if (prev != NULL && prev != next) {
        if (!prev->mWaitingVisible && next != NULL && !next->mNowVisible) {
            prev->mWaitingVisible = TRUE;
            mWaitingVisibleActivities.PushBack(prev);
            if (DEBUG_SWITCH) {
                String arDes;
                prev->GetDescription(&arDes);
                Slogger::V(TAG, StringBuffer("Resuming top, waiting visible to hide: ")
                        + arDes);
            }
        } else {
            // The next activity is already visible, so hide the previous
            // activity's windows right now so we can show the new one ASAP.
            // We only do this if the previous is finishing, which should mean
            // it is on top of the one being resumed so hiding it quickly
            // is good.  Otherwise, we want to do the normal route of allowing
            // the resumed activity to be shown so we can decide if the
            // previous should actually be hidden depending on whether the
            // new one is found to be full-screen or not.
            if (prev->mFinishing) {
                mService->mWindowManager->SetAppVisibility(prev, FALSE);
                if (DEBUG_SWITCH) {
                    String arDes;
                    prev->GetDescription(&arDes);
                    Slogger::V(TAG, StringBuffer("Not waiting for visible to hide: ")
                            + arDes + ", waitingVisible=" + prev->mWaitingVisible
                            + ", nowVisible=" + next->mNowVisible);
                }
            } else {
                if (DEBUG_SWITCH) {
                    String arDes;
                    prev->GetDescription(&arDes);
                    Slogger::V(TAG,
                        StringBuffer("Previous already visible but still waiting to hide: ")
                        + arDes + ", waitingVisible=" + prev->mWaitingVisible
                        + ", nowVisible=" + next->mNowVisible);
                }
            }
        }
    }

    // We are starting up the next activity, so tell the window manager
    // that the previous one will be hidden soon.  This way it can know
    // to ignore it when computing the desired screen orientation.
    if (prev != NULL) {
        if (prev->mFinishing) {
            if (DEBUG_TRANSITION) {
                String arDes;
                prev->GetDescription(&arDes);
                Slogger::V(TAG, StringBuffer("Prepare close transition: prev=") + arDes);
            }
            if (Find(mNoAnimActivities.Begin(), mNoAnimActivities.End(),
                    AutoPtr<CActivityRecord>(prev)) != mNoAnimActivities.End()) {
                        mService->mWindowManager->PrepareAppTransition(WindowManagerPolicy::TRANSIT_NONE);
            } else {
                // if use WindowManagerPolicy::TRANSIT_TASK_CLOSE directly,
                // it has link errors in linux_android. I don't know the reason!
                Int32 taskClose = WindowManagerPolicy::TRANSIT_TASK_CLOSE;
                mService->mWindowManager->PrepareAppTransition(prev->mTask == next->mTask
                    ? WindowManagerPolicy::TRANSIT_ACTIVITY_CLOSE
                    : taskClose);
            }
            mService->mWindowManager->SetAppWillBeHidden(prev);
            mService->mWindowManager->SetAppVisibility(prev, FALSE);
        } else {
            if (DEBUG_TRANSITION) {
                String arDes;
                prev->GetDescription(&arDes);
                Slogger::V(TAG, StringBuffer("Prepare open transition: prev=") + arDes);
            }
            if (Find(mNoAnimActivities.Begin(), mNoAnimActivities.End(), next) !=
                    mNoAnimActivities.End()) {
                        mService->mWindowManager->PrepareAppTransition(
                            WindowManagerPolicy::TRANSIT_NONE);
            } else {
                Int32 taskOpen = WindowManagerPolicy::TRANSIT_TASK_OPEN;
                mService->mWindowManager->PrepareAppTransition(prev->mTask == next->mTask
                    ? WindowManagerPolicy::TRANSIT_ACTIVITY_OPEN
                    : taskOpen);
            }
        }
        if (false) {
            mService->mWindowManager->SetAppWillBeHidden(prev);
            mService->mWindowManager->SetAppVisibility(prev, FALSE);
        }
    } else if (mHistory.GetSize() > 1) {
        if (DEBUG_TRANSITION) {
            Slogger::V(TAG, StringBuffer("Prepare open transition: no previous"));
        }
        if (Find(mNoAnimActivities.Begin(), mNoAnimActivities.End(), next) !=
                mNoAnimActivities.End()) {
                    mService->mWindowManager->PrepareAppTransition(
                        WindowManagerPolicy::TRANSIT_NONE);
        } else {
            mService->mWindowManager->PrepareAppTransition(
                WindowManagerPolicy::TRANSIT_ACTIVITY_OPEN);
        }
    }

    if (next->mApp != NULL && next->mApp->mAppApartment != NULL) {
        if (DEBUG_SWITCH) {
            String arDes;
            next->GetDescription(&arDes);
            Slogger::V(TAG, "Resume running: " + arDes);
        }

        // This activity is now becoming visible.
        mService->mWindowManager->SetAppVisibility(next, TRUE);

        AutoPtr<CActivityRecord> lastResumedActivity = mResumedActivity;
        ActivityState lastState = next->mState;
        UNUSED(lastState);

        mService->UpdateCpuStats();

        next->mState = ActivityState_RESUMED;
        mResumedActivity = next;
        next->mTask->TouchActiveTime();
        mService->UpdateLRUProcessLocked(next->mApp, TRUE, TRUE);
        UpdateLRUListLocked(next);

        // Have the window manager re-evaluate the orientation of
        // the screen based on the new activity order.
        Boolean updated = FALSE;
        if (mMainStack) {
            Mutex::Autolock lock(mService->_m_syncLock);

            AutoPtr<IConfiguration> config;
            mService->mWindowManager->UpdateOrientationFromAppTokens(
                    mService->mConfiguration,
                    next->MayFreezeScreenLocked(next->mApp) ? next : NULL,
                    (IConfiguration**)&config);
            if (config != NULL) {
                next->mFrozenBeforeDestroy = TRUE;
            }
            updated = mService->UpdateConfigurationLocked(config, next);
        }
        if (!updated) {
            // The configuration update wasn't able to keep the existing
            // instance of the activity, and instead started a new one.
            // We should be all done, but let's just make sure our activity
            // is still at the top and schedule another run if something
            // weird happened.
            AutoPtr<CActivityRecord> nextNext = GetTopRunningActivityLocked(NULL);
            if (DEBUG_SWITCH) {
                String nDes, nnDes;
                next->GetDescription(&nDes);
                nextNext->GetDescription(&nnDes);
                Slogger::I(TAG, StringBuffer("Activity config changed during resume: ")
                        + nDes + ", new next: " + nnDes);
            }
//            if (nextNext != next) {
//                // Do over!
//                mHandler.sendEmptyMessage(RESUME_TOP_ACTIVITY_MSG);
//            }
            if (mMainStack) {
                mService->SetFocusedActivityLocked(next);
            }
            EnsureActivitiesVisibleLocked(NULL, 0);
            mService->mWindowManager->ExecuteAppTransition();
            mNoAnimActivities.Clear();
            return TRUE;
        }

//        try {
        // Deliver all pending results.
        if (next->mResults != NULL) {
            List<ActivityResult*>::Iterator it = next->mResults->Begin();
            if (!next->mFinishing && it != next->mResults->End()) {
                AutoPtr<IObjectContainer> res;
                CParcelableObjectContainer::New((IObjectContainer**)&res);
                for (; it != next->mResults->End(); ++it) {
                    res->Add((*it)->mResultInfo);
                }
    //            if (DEBUG_RESULTS) Slog.v(
    //                    TAG, "Delivering results to " + next
    //                    + ": " + a);
                next->mApp->mAppApartment->ScheduleSendResult(next,
                        (IObjectContainer*)res);
            }
        }

        if (next->mNewIntents != NULL) {
            AutoPtr<IObjectContainer> res;
            CParcelableObjectContainer::New((IObjectContainer**)&res);
            List<AutoPtr<IIntent> >::Iterator it;
            for (it = next->mNewIntents->Begin();
                 it != next->mNewIntents->End(); ++it) {
                res->Add((IIntent*)*it);
            }

            next->mApp->mAppApartment->ScheduleNewIntent(
                    res, next);
        }

//        EventLog.writeEvent(EventLogTags.AM_RESUME_ACTIVITY,
//                System.identityHashCode(next),
//                next.task.taskId, next.shortComponentName);

        next->mApp->mAppApartment->ScheduleResumeActivity(next,
                mService->IsNextTransitionForward());

        PauseIfSleepingLocked();

//        } catch (Exception e) {
//            // Whoops, need to restart this activity!
//            next.state = lastState;
//            mResumedActivity = lastResumedActivity;
//            Slog.i(TAG, "Restarting because process died: " + next);
//            if (!next.hasBeenLaunched) {
//                next.hasBeenLaunched = true;
//            } else {
//                if (SHOW_APP_STARTING_PREVIEW && mMainStack) {
//                    mService.mWindowManager.setAppStartingWindow(
//                            next, next.packageName, next.theme,
//                            next.nonLocalizedLabel,
//                            next.labelRes, next.icon, null, true);
//                }
//            }
//            startSpecificActivityLocked(next, true, false);
//            return true;
//        }

        // From this point on, if something goes wrong there is no way
        // to recover the activity.
//       try {
        next->mVisible = TRUE;
        CompleteResumeLocked(next);
//        } catch (Exception e) {
//            // If any exception gets thrown, toss away this
//            // activity and try the next one.
//            String arDes;
//            next->GetDescription(&arDes);
//            Slogger::W(TAG, StringBuffer("Exception thrown during resume of ") + arDes);
//            RequestFinishActivityLocked(next, Activity_RESULT_CANCELED, NULL,
//                    "resume-exception");
//            return TRUE;
//        }

        // Didn't need to use the icicle, and it is now out of date.
        next->mIcicle = NULL;
        next->mHaveState = FALSE;
        next->mStopped = FALSE;

    } else {
        // Whoops, need to restart this activity!
        if (!next->mHasBeenLaunched) {
            next->mHasBeenLaunched = TRUE;
        } else {
            if (SHOW_APP_STARTING_PREVIEW) {
                mService->mWindowManager->SetAppStartingWindow(
                        (CActivityRecord*)next, next->mCapsuleName, next->mTheme,
                        next->mNonLocalizedLabel,
                        next->mLabelRes, next->mIcon, NULL, TRUE);
            }
            if (DEBUG_SWITCH) {
                String arDes;
                next->GetDescription(&arDes);
                Slogger::V(TAG, StringBuffer("Restarting: ") + arDes);
            }
        }
        StartSpecificActivityLocked(next, TRUE, TRUE);
    }

    return TRUE;
}

void ActivityStack::StartActivityLocked(
    /* [in] */ CActivityRecord* r,
    /* [in] */ Boolean newTask,
    /* [in] */ Boolean doResume)
{
    Int32 NH = mHistory.GetSize();

    Int32 addPos = -1;

    if (!newTask) {
        // If starting in an existing task, find where that is...
        Boolean startIt = TRUE;
        for (int i = NH - 1; i >= 0; i--) {
            AutoPtr<CActivityRecord> p = mHistory[i];
            if (p->mFinishing) {
                continue;
            }
            if (p->mTask == r->mTask) {
                // Here it is!  Now, if this is not yet visible to the
                // user, then just add it without starting; it will
                // get started when the user navigates back to it.
                addPos = i + 1;
                if (!startIt) {
                    mHistory.Insert(addPos, r);
                    r->mInHistory = TRUE;
                    r->mTask->mNumActivities++;
                    Int32 orientation;
                    r->mInfo->GetScreenOrientation(&orientation);
                    mService->mWindowManager->AddAppToken(addPos, r, r->mTask->mTaskId,
                            orientation, r->mFullscreen);
                    if (VALIDATE_TOKENS) {
                        mService->mWindowManager->ValidateAppTokens(mHistory);
                    }
                    return;
                }
                break;
            }
            if (p->mFullscreen) {
                startIt = FALSE;
            }
        }
    }

    // Place a new activity at top of stack, so it is next to interact
    // with the user.
    if (addPos < 0) {
        addPos = NH;
    }

    // If we are not placing the new activity frontmost, we do not want
    // to deliver the onUserLeaving callback to the actual frontmost
    // activity
    if (addPos < NH) {
        mUserLeaving = FALSE;
        if (DEBUG_USER_LEAVING) {
            Slogger::V(TAG, StringBuffer("startActivity() behind front, mUserLeaving=false"));
        }
    }

    // Slot the activity into the history stack and proceed
    mHistory.Insert(addPos, r);
    r->mInHistory = TRUE;
    r->mFrontOfTask = newTask;
    r->mTask->mNumActivities++;
    if (NH > 0) {
        // We want to show the starting preview window if we are
        // switching to a new task, or the next activity's process is
        // not currently running.
        Boolean showStartingIcon = newTask;
        ProcessRecord* proc = r->mApp;
        if (proc == NULL) {
            AutoPtr<IApplicationInfo> appInfo;
            r->mInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
            Int32 uid;
            appInfo->GetUid(&uid);
            proc = mService->mProcessNames->Get(r->mProcessName, uid);
        }
        if (proc == NULL || proc->mAppApartment == NULL) {
            showStartingIcon = TRUE;
        }
        if (DEBUG_TRANSITION) {
            String arDes;
            r->GetDescription(&arDes);
            Slogger::V(TAG, StringBuffer("Prepare open transition: starting ") + arDes);
        }
        Int32 flags;
        r->mIntent->GetFlags(&flags);
        if ((flags & Intent_FLAG_ACTIVITY_NO_ANIMATION) != 0) {
            mService->mWindowManager->PrepareAppTransition(WindowManagerPolicy::TRANSIT_NONE);
            mNoAnimActivities.PushBack(r);
        }
        else if ((flags & Intent_FLAG_ACTIVITY_CLEAR_WHEN_TASK_RESET) != 0) {
            mService->mWindowManager->PrepareAppTransition(WindowManagerPolicy::TRANSIT_TASK_OPEN);
            mNoAnimActivities.Remove(r);
        }
        else {
            Int32 activityOpen = WindowManagerPolicy::TRANSIT_ACTIVITY_OPEN;
            mService->mWindowManager->PrepareAppTransition(newTask
                ? WindowManagerPolicy::TRANSIT_TASK_OPEN
                : activityOpen);
            mNoAnimActivities.Remove(r);
        }
        Int32 orientation;
        r->mInfo->GetScreenOrientation(&orientation);
        mService->mWindowManager->AddAppToken(
                addPos, r, r->mTask->mTaskId, orientation, r->mFullscreen);
        Boolean doShow = TRUE;
        if (newTask) {
            // Even though this activity is starting fresh, we still need
            // to reset it to make sure we apply affinities to move any
            // existing activities from other tasks in to it.
            // If the caller has requested that the target task be
            // reset, then do so.
            if ((flags & Intent_FLAG_ACTIVITY_RESET_TASK_IF_NEEDED) != 0) {
                ResetTaskIfNeededLocked(r, r);
                doShow = GetTopRunningNonDelayedActivityLocked(NULL) == r;
            }
        }
        if (SHOW_APP_STARTING_PREVIEW && doShow) {
            // Figure out if we are transitioning from another activity that is
            // "has the same starting icon" as the next one.  This allows the
            // window manager to keep the previous window it had previously
            // created, if it still had one.
            AutoPtr<CActivityRecord> prev = mResumedActivity;
            if (prev != NULL) {
                // We don't want to reuse the previous starting preview if:
                // (1) The current activity is in a different task.
                if (prev->mTask != r->mTask) prev = NULL;
                // (2) The current activity is already displayed.
                else if (prev->mNowVisible) prev = NULL;
            }
            mService->mWindowManager->SetAppStartingWindow(
                    (IBinder*)r, r->mCapsuleName, r->mTheme, r->mNonLocalizedLabel,
                    r->mLabelRes, r->mIcon, (IBinder*)prev.Get(), showStartingIcon);
        }
    }
    else {
        Int32 orientation;
        r->mInfo->GetScreenOrientation(&orientation);
        // If this is the first activity, don't do any fancy animations,
        // because there is nothing for it to animate on top of.
        mService->mWindowManager->AddAppToken(addPos, r, r->mTask->mTaskId,
                orientation, r->mFullscreen);
    }
    if (VALIDATE_TOKENS) {
        mService->mWindowManager->ValidateAppTokens(mHistory);
    }

    if (doResume) {
        ResumeTopActivityLocked(NULL);
    }
}

/**
 * Perform a reset of the given task, if needed as part of launching it.
 * Returns the new HistoryRecord at the top of the task.
 */
CActivityRecord* ActivityStack::ResetTaskIfNeededLocked(
    /* [in] */ CActivityRecord* taskTop,
    /* [in] */ CActivityRecord* newActivity)
{
    Int32 newFlags;
    newActivity->mInfo->GetFlags(&newFlags);
    Boolean forceReset = (newFlags & ActivityInfo_FLAG_CLEAR_TASK_ON_LAUNCH) != 0;
    if (taskTop->mTask->GetInactiveDuration() > ACTIVITY_INACTIVE_RESET_TIME) {
        if ((newFlags & ActivityInfo_FLAG_ALWAYS_RETAIN_TASK_STATE) == 0) {
            forceReset = TRUE;
        }
    }

    TaskRecord* task = taskTop->mTask;

    // We are going to move through the history list so that we can look
    // at each activity 'target' with 'below' either the interesting
    // activity immediately below it in the stack or null.
    AutoPtr<CActivityRecord> target;
    Int32 targetI = 0;
    Int32 taskTopI = -1;
    Int32 replyChainEnd = -1;
    Int32 lastReparentPos = -1;
    for (Int32 i = mHistory.GetSize() - 1; i >= -1; i--) {
        AutoPtr<CActivityRecord> below = i >= 0 ? mHistory[i] : NULL;

        if (below != NULL && below->mFinishing) {
            continue;
        }
        if (target == NULL) {
            target = below;
            targetI = i;
            // If we were in the middle of a reply chain before this
            // task, it doesn't appear like the root of the chain wants
            // anything interesting, so drop it.
            replyChainEnd = -1;
            continue;
        }

        Int32 flags;
        target->mInfo->GetFlags(&flags);

        Boolean finishOnTaskLaunch =
            (flags & ActivityInfo_FLAG_FINISH_ON_TASK_LAUNCH) != 0;
        Boolean allowTaskReparenting =
            (flags & ActivityInfo_FLAG_ALLOW_TASK_REPARENTING) != 0;

        if (target->mTask == task) {
            // We are inside of the task being reset...  we'll either
            // finish this activity, push it out for another task,
            // or leave it as-is.  We only do this
            // for activities that are not the root of the task (since
            // if we finish the root, we may no longer have the task!).
            if (taskTopI < 0) {
                taskTopI = targetI;
            }
            if (below != NULL && below->mTask == task) {
                Int32 iflags;
                target->mIntent->GetFlags(&iflags);
                Boolean clearWhenTaskReset =
                        (iflags & Intent_FLAG_ACTIVITY_CLEAR_WHEN_TASK_RESET) != 0;
                if (!finishOnTaskLaunch && !clearWhenTaskReset && target->mResultTo != NULL) {
                    // If this activity is sending a reply to a previous
                    // activity, we can't do anything with it now until
                    // we reach the start of the reply chain.
                    // XXX note that we are assuming the result is always
                    // to the previous activity, which is almost always
                    // the case but we really shouldn't count on.
                    if (replyChainEnd < 0) {
                        replyChainEnd = targetI;
                    }
                }
                else if (!finishOnTaskLaunch && !clearWhenTaskReset && allowTaskReparenting
                        && !target->mTaskAffinity.IsNull()
                        && target->mTaskAffinity.Compare(task->mAffinity)) {
                    // If this activity has an affinity for another
                    // task, then we need to move it out of here.  We will
                    // move it as far out of the way as possible, to the
                    // bottom of the activity stack.  This also keeps it
                    // correctly ordered with any activities we previously
                    // moved.
                    AutoPtr<CActivityRecord> p = mHistory[0];
                    if (!target->mTaskAffinity.IsNull()
                            && !target->mTaskAffinity.Compare(p->mTask->mAffinity)) {
                        // If the activity currently at the bottom has the
                        // same task affinity as the one we are moving,
                        // then merge it into the same task.
                        target->mTask = p->mTask;
                        if (DEBUG_TASKS) {
                            String arDes, trDes;
                            target->GetDescription(&arDes);
                            p->mTask->GetDescription(&trDes);
                            Slogger::V(TAG, StringBuffer("Start pushing activity ") + arDes
                                + " out to bottom task " + trDes);
                        }
                    }
                    else {
                        mService->mCurTask++;
                        if (mService->mCurTask <= 0) {
                            mService->mCurTask = 1;
                        }
                        target->mTask = new TaskRecord(mService->mCurTask, target->mInfo, NULL,
                                (flags & ActivityInfo_FLAG_CLEAR_TASK_ON_LAUNCH) != 0);
                        target->mTask->mAffinityIntent = target->mIntent;
                        if (DEBUG_TASKS) {
                            String arDes, trDes;
                            target->GetDescription(&arDes);
                            target->mTask->GetDescription(&trDes);
                            Slogger::V(TAG, StringBuffer("Start pushing activity ") + arDes
                                + " out to new task " + trDes);
                        }
                    }
                    mService->mWindowManager->SetAppGroupId(target, task->mTaskId);
                    if (replyChainEnd < 0) {
                        replyChainEnd = targetI;
                    }
                    Int32 dstPos = 0;
                    for (Int32 srcPos = targetI; srcPos <= replyChainEnd; srcPos++) {
                        p = mHistory[srcPos];
                        if (p->mFinishing) {
                            continue;
                        }
                        if (DEBUG_TASKS) {
                            String arDes, trDes;
                            p->GetDescription(&arDes);
                            target->mTask->GetDescription(&trDes);
                            Slogger::V(TAG, StringBuffer("Pushing next activity ") + arDes
                                + " out to target's task " + trDes);
                        }
                        task->mNumActivities--;
                        p->mTask = target->mTask;
                        target->mTask->mNumActivities++;
                        List<AutoPtr<CActivityRecord> >::Iterator it1 = mHistory.Begin();
                        for (Int32 pos = srcPos; pos > 0; --pos) ++it1;
                        mHistory.Erase(it1);
                        it1 = mHistory.Begin();
                        for (Int32 pos = dstPos; pos > 0; --pos) ++it1;
                        mHistory.Insert(it1, p);
                        mService->mWindowManager->MoveAppToken(dstPos, p);
                        mService->mWindowManager->SetAppGroupId(p, p->mTask->mTaskId);
                        dstPos++;
                        if (VALIDATE_TOKENS) {
                            mService->mWindowManager->ValidateAppTokens(mHistory);
                        }
                        i++;
                    }
                    if (taskTop == p) {
                        taskTop = below;
                    }
                    if (taskTopI == replyChainEnd) {
                        taskTopI = -1;
                    }
                    replyChainEnd = -1;
                    if (mMainStack) {
                        mService->AddRecentTaskLocked(target->mTask);
                    }
                } else if (forceReset || finishOnTaskLaunch
                        || clearWhenTaskReset) {
                    // If the activity should just be removed -- either
                    // because it asks for it, or the task should be
                    // cleared -- then finish it and anything that is
                    // part of its reply chain.
                    if (clearWhenTaskReset) {
                        // In this case, we want to finish this activity
                        // and everything above it, so be sneaky and pretend
                        // like these are all in the reply chain.
                        replyChainEnd = targetI + 1;
                        while (replyChainEnd < (Int32)mHistory.GetSize() &&
                                (mHistory[replyChainEnd])->mTask == task) {
                            replyChainEnd++;
                        }
                        replyChainEnd--;
                    } else if (replyChainEnd < 0) {
                        replyChainEnd = targetI;
                    }
                    AutoPtr<CActivityRecord> p;
                    for (Int32 srcPos = targetI; srcPos <= replyChainEnd; srcPos++) {
                        p = mHistory[srcPos];
                        if (p->mFinishing) {
                            continue;
                        }
                        if (FinishActivityLocked(p, srcPos,
                                Activity_RESULT_CANCELED, NULL, "reset")) {
                            replyChainEnd--;
                            srcPos--;
                        }
                    }
                    if (taskTop == p) {
                        taskTop = below;
                    }
                    if (taskTopI == replyChainEnd) {
                        taskTopI = -1;
                    }
                    replyChainEnd = -1;
                } else {
                    // If we were in the middle of a chain, well the
                    // activity that started it all doesn't want anything
                    // special, so leave it all as-is.
                    replyChainEnd = -1;
                }
            } else {
                // Reached the bottom of the task -- any reply chain
                // should be left as-is.
                replyChainEnd = -1;
            }

        } else if (target->mResultTo != NULL) {
            // If this activity is sending a reply to a previous
            // activity, we can't do anything with it now until
            // we reach the start of the reply chain.
            // XXX note that we are assuming the result is always
            // to the previous activity, which is almost always
            // the case but we really shouldn't count on.
            if (replyChainEnd < 0) {
                replyChainEnd = targetI;
            }

        } else if (taskTopI >= 0 && allowTaskReparenting
                && !task->mAffinity.IsNull()
                && !task->mAffinity.Compare(target->mTaskAffinity)) {
            // We are inside of another task...  if this activity has
            // an affinity for our task, then either remove it if we are
            // clearing or move it over to our task.  Note that
            // we currently punt on the case where we are resetting a
            // task that is not at the top but who has activities above
            // with an affinity to it...  this is really not a normal
            // case, and we will need to later pull that task to the front
            // and usually at that point we will do the reset and pick
            // up those remaining activities.  (This only happens if
            // someone starts an activity in a new task from an activity
            // in a task that is not currently on top.)
            if (forceReset || finishOnTaskLaunch) {
                if (replyChainEnd < 0) {
                    replyChainEnd = targetI;
                }
                AutoPtr<CActivityRecord> p;
                for (Int32 srcPos = targetI; srcPos <= replyChainEnd; srcPos++) {
                    p = mHistory[srcPos];
                    if (p->mFinishing) {
                        continue;
                    }
                    if (FinishActivityLocked(p, srcPos,
                            Activity_RESULT_CANCELED, NULL, "reset")) {
                        taskTopI--;
                        lastReparentPos--;
                        replyChainEnd--;
                        srcPos--;
                    }
                }
                replyChainEnd = -1;
            } else {
                if (replyChainEnd < 0) {
                    replyChainEnd = targetI;
                }
                for (Int32 srcPos = replyChainEnd; srcPos >= targetI; srcPos--) {
                    AutoPtr<CActivityRecord> p = mHistory[srcPos];
                    if (p->mFinishing) {
                        continue;
                    }
                    if (lastReparentPos < 0) {
                        lastReparentPos = taskTopI;
                        taskTop = p;
                    } else {
                        lastReparentPos--;
                    }
                    List<AutoPtr<CActivityRecord> >::Iterator it1 = mHistory.Begin();
                    for (Int32 pos = srcPos; pos > 0; --pos) ++it1;
                    mHistory.Erase(it1);
                    p->mTask->mNumActivities--;
                    p->mTask = task;
                    it1 = mHistory.Begin();
                    for (Int32 pos = lastReparentPos; pos > 0; --pos) ++it1;
                    mHistory.Insert(it1, p);
                    if (DEBUG_TASKS) {
                        String arDes, trDes;
                        p->GetDescription(&arDes);
                        task->GetDescription(&trDes);
                        Slogger::V(TAG, StringBuffer("Pulling activity ") + arDes
                            + " in to resetting task " + trDes);
                    }
                    task->mNumActivities++;
                    mService->mWindowManager->MoveAppToken(lastReparentPos, p);
                    mService->mWindowManager->SetAppGroupId(p, p->mTask->mTaskId);
                    if (VALIDATE_TOKENS) {
                        mService->mWindowManager->ValidateAppTokens(mHistory);
                    }
                }
                replyChainEnd = -1;

                // Now we've moved it in to place...  but what if this is
                // a singleTop activity and we have put it on top of another
                // instance of the same activity?  Then we drop the instance
                // below so it remains singleTop.
                Int32 mode;
                target->mInfo->GetLaunchMode(&mode);
                if (mode == ActivityInfo_LAUNCH_SINGLE_TOP) {
                    for (Int32 j = lastReparentPos - 1; j >= 0; j--) {
                        AutoPtr<CActivityRecord> p = mHistory[j];
                        if (p->mFinishing) {
                            continue;
                        }
                        AutoPtr<IComponentName> component;
                        p->mIntent->GetComponent((IComponentName**)&component);
                        AutoPtr<IComponentName> targetComponent;
                        target->mIntent->GetComponent((IComponentName**)&targetComponent);
                        Boolean isEqual;
                        if (component->Equals(targetComponent, &isEqual), isEqual) {
                            if (FinishActivityLocked(p, j,
                                    Activity_RESULT_CANCELED, NULL, "replace")) {
                                taskTopI--;
                                lastReparentPos--;
                            }
                        }
                    }
                }
            }
        }

        target = below;
        targetI = i;
    }

    return taskTop;
}

/**
 * Perform clear operation as requested by
 * {@link Intent#FLAG_ACTIVITY_CLEAR_TOP}: search from the top of the
 * stack to the given task, then look for
 * an instance of that activity in the stack and, if found, finish all
 * activities on top of it and return the instance.
 *
 * @param newR Description of the new activity being started.
 * @return Returns the old activity that should be continue to be used,
 * or null if none was found.
 */
CActivityRecord* ActivityStack::PerformClearTaskLocked(
    /* [in] */ Int32 taskId,
    /* [in] */ CActivityRecord* newR,
    /* [in] */ Int32 launchFlags,
    /* [in] */ Boolean doClear)
{
    List<AutoPtr<CActivityRecord> >::ReverseIterator rit = mHistory.RBegin();
    Int32 i = mHistory.GetSize() - 1;

    // First find the requested task.
    while (rit != mHistory.REnd() && i >= 0) {
        CActivityRecord* r = *rit;
        if (r->mTask->mTaskId == taskId) {
            break;
        }
        rit++;
        i--;
    }

    // Now clear it.
    while (rit != mHistory.REnd() && i >= 0) {
        CActivityRecord* r = *rit;
        if (r->mFinishing) {
            continue;
        }
        if (r->mTask->mTaskId != taskId) {
            return NULL;
        }
        Boolean isEqual;
        if (r->mRealActivity->Equals(newR->mRealActivity, &isEqual), isEqual) {
            // Here it is!  Now finish everything in front...
            CActivityRecord* ret = r;
            if (doClear) {
                while (i < (Int32)(mHistory.GetSize() - 1)) {
                    rit--;
                    i++;
                    r = *rit;
                    if (r->mFinishing) {
                        continue;
                    }
                    if (FinishActivityLocked(r, i, Activity_RESULT_CANCELED,
                            NULL, "clear")) {
                        rit++;
                        i--;
                    }
                }
            }

            // Finally, if this is a normal launch mode (that is, not
            // expecting onNewIntent()), then we will finish the current
            // instance of the activity so a new fresh one can be started.
            if (ret->mLaunchMode == ActivityInfo_LAUNCH_MULTIPLE
                        && (launchFlags & Intent_FLAG_ACTIVITY_SINGLE_TOP) == 0) {
                if (!ret->mFinishing) {
                    Int32 index = GetIndexOfTokenLocked(ret);
                    if (index >= 0) {
                        FinishActivityLocked(ret, index, Activity_RESULT_CANCELED,
                                NULL, "clear");
                    }
                    return NULL;
                }
            }

            return ret;
        }
        rit++;
        i--;
    }

    return NULL;
}

/**
 * Find the activity in the history stack within the given task.  Returns
 * the index within the history at which it's found, or < 0 if not found.
 */
Int32 ActivityStack::FindActivityInHistoryLocked(
    /* [in] */ CActivityRecord* r,
    /* [in] */ Int32 task)
{
    List<AutoPtr<CActivityRecord> >::ReverseIterator rit = mHistory.RBegin();
    Int32 i = mHistory.GetSize() - 1;
    while (rit != mHistory.REnd() && i >= 0) {
        CActivityRecord* candidate = *rit;
        if (candidate->mTask->mTaskId != task) {
            break;
        }
        Boolean isEqual;
        if (candidate->mRealActivity->Equals(r->mRealActivity, &isEqual), isEqual) {
            return i;
        }
        rit++;
        i--;
    }

    return -1;
}

/**
 * Reorder the history stack so that the activity at the given index is
 * brought to the front.
 */
CActivityRecord* ActivityStack::MoveActivityToFrontLocked(
    /* [in] */ Int32 where)
{
    List<AutoPtr<CActivityRecord> >::Iterator next = mHistory.Begin();
    for (; where > 0; --where) ++next;
    AutoPtr<CActivityRecord> newTop = *next;
    mHistory.Erase(next);
    AutoPtr<CActivityRecord> oldTop = mHistory.GetBack();
    mHistory.Insert(mHistory.End(), newTop);
    oldTop->mFrontOfTask = FALSE;
    newTop->mFrontOfTask = TRUE;
    return newTop;
}

ECode ActivityStack::StartActivityLocked(
    /* [in] */ IApplicationApartment* caller,
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ List<AutoPtr<IUri> >* grantedUriPermissions,
    /* [in] */ Int32 grantedMode,
    /* [in] */ IActivityInfo* aInfo,
    /* [in] */ IBinder* resultTo,
    /* [in] */ String resultWho,
    /* [in] */ Int32 requestCode,
    /* [in] */ Int32 callingPid,
    /* [in] */ Int32 callingUid,
    /* [in] */ Boolean onlyIfNeeded,
    /* [in] */ Boolean componentSpecified,
    /* [out] */ Int32* status)
{
    Int32 err = ActivityManager_START_SUCCESS;

    ProcessRecord* callerApp = NULL;
    if (caller != NULL) {
        callerApp = mService->GetProcessRecordForAppLocked(caller);
        if (callerApp != NULL) {
            callingPid = callerApp->mPid;
            callerApp->mInfo->GetUid(&callingUid);
        }
        else {
            String appApDes, intDes;
            caller->GetDescription(&appApDes);
            intent->GetDescription(&intDes);
            Slogger::W(TAG, StringBuffer("Unable to find app for caller ")
                    + appApDes + " (pid=" + callingPid + ") when starting: "
                    + intDes);
            err = ActivityManager_START_PERMISSION_DENIED;
        }
    }

    if (err == ActivityManager_START_SUCCESS) {
        String intDes;
        intent->GetDescription(&intDes);
        Slogger::I(TAG, StringBuffer("Starting: ") + intDes + " from pid "
                + (callerApp != NULL ? callerApp->mPid : callingPid));
    }

    AutoPtr<CActivityRecord> sourceRecord;
    AutoPtr<CActivityRecord> resultRecord;
    if (resultTo != NULL) {
        Int32 index = GetIndexOfTokenLocked(resultTo);
        if (DEBUG_RESULTS) {
            String ibDes;
            resultTo->GetDescription(&ibDes);
            Slogger::V(TAG, StringBuffer("Sending result to ") + ibDes
                    + " (index " + index + ")");
        }
        if (index >= 0) {
            sourceRecord = mHistory[index];
            if (requestCode >= 0 && !sourceRecord->mFinishing) {
                resultRecord = sourceRecord;
            }
        }
    }

    Int32 launchFlags;
    intent->GetFlags(&launchFlags);
    if ((launchFlags & Intent_FLAG_ACTIVITY_FORWARD_RESULT) != 0
            && sourceRecord != NULL) {
        // Transfer the result target from the source activity to the new
        // one being started, including any failures.
        if (requestCode >= 0) {
            *status = ActivityManager_START_FORWARD_AND_REQUEST_CONFLICT;
            return NOERROR;
        }
        resultRecord = sourceRecord->mResultTo;
        resultWho = sourceRecord->mResultWho;
        requestCode = sourceRecord->mRequestCode;
        sourceRecord->mResultTo = NULL;
        if (resultRecord != NULL) {
            resultRecord->RemoveResultsLocked(
                sourceRecord, resultWho, requestCode);
        }
    }

    AutoPtr<IComponentName> component;
    intent->GetComponent((IComponentName**)&component);
    if (err == ActivityManager_START_SUCCESS && component == NULL) {
        // We couldn't find a class that can handle the given Intent.
        // That's the end of that!
        err = ActivityManager_START_INTENT_NOT_RESOLVED;
    }

    if (err == ActivityManager_START_SUCCESS && aInfo == NULL) {
        // We couldn't find the specific class specified in the Intent.
        // Also the end of the line.
        err = ActivityManager_START_CLASS_NOT_FOUND;
    }

    if (err != ActivityManager_START_SUCCESS) {
        if (resultRecord != NULL) {
            SendActivityResultLocked(-1,
                    resultRecord, resultWho, requestCode,
                    Activity_RESULT_CANCELED, NULL);
        }
        *status = err;
        return NOERROR;
    }

//    Int32 perm = mService->CheckComponentPermission(
//            ((CActivityInfo*)aInfo)->mPermission, callingPid, callingUid,
//            ((CActivityInfo*)aInfo)->mExported ?
//            -1 : ((CActivityInfo*)aInfo)->mApplicationInfo->mUid);
    Int32 perm = CapsuleManager_PERMISSION_GRANTED;
    if (perm != CapsuleManager_PERMISSION_GRANTED) {
        if (resultRecord != NULL) {
            SendActivityResultLocked(-1,
                resultRecord, resultWho, requestCode,
                Activity_RESULT_CANCELED, NULL);
        }
        String intDes, appDes, permission;
        intent->GetDescription(&intDes);
        callerApp->GetDescription(&appDes);
        aInfo->GetPermission(&permission);
        StringBuffer msg;
        msg += "Permission Denial: starting " + intDes
                + " from " + appDes + " (pid=" + callingPid
                + ", uid=" + callingUid + ")"
                + " requires " + permission;
        Slogger::W(TAG, msg);
        return E_SECURITY_EXCEPTION;
    }

    if (mMainStack) {
        if (mService->mController != NULL) {
            Boolean abort = FALSE;
            // The Intent we give to the watcher has the extra data
            // stripped off, since it can contain private information.
            AutoPtr<IIntent> watchIntent;
            intent->CloneFilter((IIntent**)&watchIntent);
            AutoPtr<IApplicationInfo> appInfo;
            aInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
            String cname;
            appInfo->GetCapsuleName(&cname);
            if (SUCCEEDED(mService->mController->ActivityStarting(
                    watchIntent, cname, &abort))) {
                abort = !abort;
            }
            else {
                mService->mController = NULL;
            }

            if (abort) {
                if (resultRecord != NULL) {
                    SendActivityResultLocked(-1,
                        resultRecord, resultWho, requestCode,
                        Activity_RESULT_CANCELED, NULL);
                }
                // We pretend to the caller that it was really started, but
                // they will just get a cancel result.
                *status = ActivityManager_START_SUCCESS;
                return NOERROR;
            }
        }
    }

    AutoPtr<CActivityRecord> r;
    CActivityRecord::NewByFriend((CActivityRecord**)&r);
    r->Init(mService, this, callerApp, callingUid,
            intent, resolvedType, aInfo, mService->mConfiguration,
            resultRecord, resultWho, requestCode, componentSpecified);

    if (mMainStack) {
        Int32 uid;
        if (mResumedActivity != NULL) {
            AutoPtr<IApplicationInfo> appInfo;
            mResumedActivity->mInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
            appInfo->GetUid(&uid);
        }

        if (mResumedActivity == NULL || uid != callingUid) {
            if (!mService->CheckAppSwitchAllowedLocked(callingPid, callingUid, "Activity start")) {
                CActivityManagerService::PendingActivityLaunch* pal = \
                        new CActivityManagerService::PendingActivityLaunch();
                pal->mR = r;
                pal->mSourceRecord = sourceRecord;
                pal->mGrantedUriPermissions = grantedUriPermissions;
                pal->mGrantedMode = grantedMode;
                pal->mOnlyIfNeeded = onlyIfNeeded;
                mService->mPendingActivityLaunches.PushBack(pal);
                return ActivityManager_START_SWITCHES_CANCELED;
            }
        }

        if (mService->mDidAppSwitch) {
            // This is the second allowed switch since we stopped switches,
            // so now just generally allow switches.  Use case: user presses
            // home (switches disabled, switch to home, mDidAppSwitch now true);
            // user taps a home icon (coming from home so allowed, we hit here
            // and now allow anyone to switch again).
            mService->mAppSwitchesAllowedTime = 0;
        } else {
            mService->mDidAppSwitch = TRUE;
        }

        mService->DoPendingActivityLaunchesLocked(FALSE);
    }

    return StartActivityUncheckedLocked(r, sourceRecord,
            grantedUriPermissions, grantedMode, onlyIfNeeded, TRUE, status);
}

ECode ActivityStack::StartActivityUncheckedLocked(
    /* [in] */ CActivityRecord* r,
    /* [in] */ CActivityRecord* sourceRecord,
    /* [in] */ List<AutoPtr<IUri> >* grantedUriPermissions,
    /* [in] */ Int32 grantedMode,
    /* [in] */ Boolean onlyIfNeeded,
    /* [in] */ Boolean doResume,
    /* [out] */ Int32* status)
{
    AutoPtr<IIntent> intent = r->mIntent;
    Int32 callingUid = r->mLaunchedFromUid;

    Int32 launchFlags;
    intent->GetFlags(&launchFlags);

    // We'll invoke onUserLeaving before onPause only if the launching
    // activity did not explicitly state that this is an automated launch.
    mUserLeaving = (launchFlags & Intent_FLAG_ACTIVITY_NO_USER_ACTION) == 0;
    if (DEBUG_USER_LEAVING) {
        Slogger::V(TAG, StringBuffer("startActivity() => mUserLeaving=") + mUserLeaving);
    }

    // If the caller has asked not to resume at this point, we make note
    // of this in the record so that we can skip it when trying to find
    // the top running activity.
    if (!doResume) {
        r->mDelayedResume = TRUE;
    }

    AutoPtr<CActivityRecord> notTop =
            (launchFlags & Intent_FLAG_ACTIVITY_PREVIOUS_IS_TOP) != 0 ? r : NULL;

    // If the onlyIfNeeded flag is set, then we can do this if the activity
    // being launched is the same as the one making the call...  or, as
    // a special case, if we do not know the caller then we count the
    // current top activity as the caller.
    Boolean isEqual = FALSE;
    if (onlyIfNeeded) {
        AutoPtr<CActivityRecord> checkedCaller = sourceRecord;
        if (checkedCaller == NULL) {
            checkedCaller = GetTopRunningNonDelayedActivityLocked(notTop);
        }
        if (checkedCaller->mRealActivity->Equals(r->mRealActivity, &isEqual), !isEqual) {
            // Caller is not the same as launcher, so always needed.
            onlyIfNeeded = FALSE;
        }
    }

    if (sourceRecord == NULL) {
        // This activity is not being started from another...  in this
        // case we -always- start a new task.
        if ((launchFlags & Intent_FLAG_ACTIVITY_NEW_TASK) == 0) {
            String intDes;
            intent->GetDescription(&intDes);
            Slogger::W(TAG,
                StringBuffer("startActivity called from non-Activity context; forcing Intent.FLAG_ACTIVITY_NEW_TASK for: ")
                + intDes);
            launchFlags |= Intent_FLAG_ACTIVITY_NEW_TASK;
        }
    } else if (sourceRecord->mLaunchMode == ActivityInfo_LAUNCH_SINGLE_INSTANCE) {
        // The original activity who is starting us is running as a single
        // instance...  this new activity it is starting must go on its
        // own task.
        launchFlags |= Intent_FLAG_ACTIVITY_NEW_TASK;
    } else if (r->mLaunchMode == ActivityInfo_LAUNCH_SINGLE_INSTANCE
            || r->mLaunchMode == ActivityInfo_LAUNCH_SINGLE_TASK) {
        // The activity being started is a single instance...  it always
        // gets launched into its own task.
        launchFlags |= Intent_FLAG_ACTIVITY_NEW_TASK;
    }

    if (r->mResultTo != NULL && (launchFlags & Intent_FLAG_ACTIVITY_NEW_TASK) != 0) {
        // For whatever reason this activity is being launched into a new
        // task...  yet the caller has requested a result back.  Well, that
        // is pretty messed up, so instead immediately send back a cancel
        // and let the new task continue launched as normal without a
        // dependency on its originator.
        Slogger::W(TAG, "Activity is launching as a new task, so cancelling activity result.");
        SendActivityResultLocked(-1,
                r->mResultTo, r->mResultWho, r->mRequestCode,
                Activity_RESULT_CANCELED, NULL);
        r->mResultTo = NULL;
    }

    Boolean addingToTask = FALSE;
    if (((launchFlags & Intent_FLAG_ACTIVITY_NEW_TASK) != 0 &&
            (launchFlags & Intent_FLAG_ACTIVITY_MULTIPLE_TASK) == 0)
            || r->mLaunchMode == ActivityInfo_LAUNCH_SINGLE_TASK
            || r->mLaunchMode == ActivityInfo_LAUNCH_SINGLE_INSTANCE) {
        // If bring to front is requested, and no result is requested, and
        // we can find a task that was started with this same
        // component, then instead of launching bring that one to the front.
        if (r->mResultTo == NULL) {
            // See if there is a task to bring to the front.  If this is
            // a SINGLE_INSTANCE activity, there can be one and only one
            // instance of it in the history, and it is always in its own
            // unique task, so we do a special search.
            AutoPtr<CActivityRecord> taskTop =
                    r->mLaunchMode != ActivityInfo_LAUNCH_SINGLE_INSTANCE
                    ? FindTaskLocked(intent, r->mInfo)
                    : FindActivityLocked(intent, r->mInfo);
            if (taskTop != NULL) {
                if (taskTop->mTask->mIntent == NULL) {
                    // This task was started because of movement of
                    // the activity based on affinity...  now that we
                    // are actually launching it, we can assign the
                    // base intent.
                    taskTop->mTask->SetIntent(intent, r->mInfo);
                }
                // If the target task is not in the front, then we need
                // to bring it to the front...  except...  well, with
                // SINGLE_TASK_LAUNCH it's not entirely clear.  We'd like
                // to have the same behavior as if a new instance was
                // being started, which means not bringing it to the front
                // if the caller is not itself in the front.
                AutoPtr<CActivityRecord> curTop =
                        GetTopRunningNonDelayedActivityLocked(notTop);
                if (curTop->mTask != taskTop->mTask) {
                    r->mIntent->AddFlags(Intent_FLAG_ACTIVITY_BROUGHT_TO_FRONT);
                    Boolean callerAtFront = sourceRecord == NULL
                            || curTop->mTask == sourceRecord->mTask;
                    if (callerAtFront) {
                        // We really do want to push this one into the
                        // user's face, right now.
                        MoveTaskToFrontLocked(taskTop->mTask, r);
                    }
                }
                // If the caller has requested that the target task be
                // reset, then do so.
                if ((launchFlags & Intent_FLAG_ACTIVITY_RESET_TASK_IF_NEEDED) != 0) {
                    taskTop = ResetTaskIfNeededLocked(taskTop, r);
                }
                if (onlyIfNeeded) {
                    // We don't need to start a new activity, and
                    // the client said not to do anything if that
                    // is the case, so this is it!  And for paranoia, make
                    // sure we have correctly resumed the top activity.
                    if (doResume) {
                        ResumeTopActivityLocked(NULL);
                    }
                    *status = ActivityManager_START_RETURN_INTENT_TO_CALLER;
                    return NOERROR;
                }
                if ((launchFlags & Intent_FLAG_ACTIVITY_CLEAR_TOP) != 0
                        || r->mLaunchMode == ActivityInfo_LAUNCH_SINGLE_TASK
                        || r->mLaunchMode == ActivityInfo_LAUNCH_SINGLE_INSTANCE) {
                    // In this situation we want to remove all activities
                    // from the task up to the one being started.  In most
                    // cases this means we are resetting the task to its
                    // initial state.
                    AutoPtr<CActivityRecord> top = PerformClearTaskLocked(
                            taskTop->mTask->mTaskId, r, launchFlags, TRUE);
                    if (top != NULL) {
                        if (top->mFrontOfTask) {
                            // Activity aliases may mean we use different
                            // intents for the top activity, so make sure
                            // the task now has the identity of the new
                            // intent.
                            top->mTask->SetIntent(r->mIntent, r->mInfo);
                        }
                        LogStartActivity(EventLogTags::AM_NEW_INTENT, r, top->mTask);
                        top->DeliverNewIntentLocked(callingUid, r->mIntent);
                    } else {
                        // A special case: we need to
                        // start the activity because it is not currently
                        // running, and the caller has asked to clear the
                        // current task to have this activity at the top.
                        addingToTask = TRUE;
                        // Now pretend like this activity is being started
                        // by the top of its task, so it is put in the
                        // right place.
                        sourceRecord = taskTop;
                    }
                } else if (r->mRealActivity->Equals(taskTop->mTask->mRealActivity,
                        &isEqual), isEqual) {
                    // In this case the top activity on the task is the
                    // same as the one being launched, so we take that
                    // as a request to bring the task to the foreground.
                    // If the top activity in the task is the root
                    // activity, deliver this new intent to it if it
                    // desires.
                    if ((launchFlags & Intent_FLAG_ACTIVITY_SINGLE_TOP) != 0
                            && (taskTop->mRealActivity->Equals(r->mRealActivity, &isEqual), isEqual)) {
                        LogStartActivity(EventLogTags::AM_NEW_INTENT, r, taskTop->mTask);
                        if (taskTop->mFrontOfTask) {
                            taskTop->mTask->SetIntent(r->mIntent, r->mInfo);
                        }
                        taskTop->DeliverNewIntentLocked(callingUid, r->mIntent);
                    } else if (r->mIntent->FilterEquals(taskTop->mTask->mIntent, &isEqual), !isEqual) {
                        // In this case we are launching the root activity
                        // of the task, but with a different intent.  We
                        // should start a new instance on top.
                        addingToTask = TRUE;
                        sourceRecord = taskTop;
                    }
                } else if ((launchFlags & Intent_FLAG_ACTIVITY_RESET_TASK_IF_NEEDED) == 0) {
                    // In this case an activity is being launched in to an
                    // existing task, without resetting that task.  This
                    // is typically the situation of launching an activity
                    // from a notification or shortcut.  We want to place
                    // the new activity on top of the current task.
                    addingToTask = TRUE;
                    sourceRecord = taskTop;
                } else if (!taskTop->mTask->mRootWasReset) {
                    // In this case we are launching in to an existing task
                    // that has not yet been started from its front door.
                    // The current task has been brought to the front.
                    // Ideally, we'd probably like to place this new task
                    // at the bottom of its stack, but that's a little hard
                    // to do with the current organization of the code so
                    // for now we'll just drop it.
                    taskTop->mTask->SetIntent(r->mIntent, r->mInfo);
                }
                if (!addingToTask) {
                    // We didn't do anything...  but it was needed (a.k.a., client
                    // don't use that intent!)  And for paranoia, make
                    // sure we have correctly resumed the top activity.
                    if (doResume) {
                        ResumeTopActivityLocked(NULL);
                    }
                    *status = ActivityManager_START_TASK_TO_FRONT;
                    return NOERROR;
                }
            }
        }
    }

    if (!r->mCapsuleName.IsNull()) {
        // If the activity being launched is the same as the one currently
        // at the top, then we need to check if it should only be launched
        // once.
        AutoPtr<CActivityRecord> top = GetTopRunningNonDelayedActivityLocked(notTop);
        if (top != NULL && r->mResultTo == NULL) {
            if (top->mRealActivity->Equals(r->mRealActivity, &isEqual), isEqual) {
                if (top->mApp != NULL && top->mApp->mAppApartment != NULL) {
                    if ((launchFlags & Intent_FLAG_ACTIVITY_SINGLE_TOP) != 0
                        || r->mLaunchMode == ActivityInfo_LAUNCH_SINGLE_TOP
                        || r->mLaunchMode == ActivityInfo_LAUNCH_SINGLE_TASK) {
                        LogStartActivity(EventLogTags::AM_NEW_INTENT, top, top->mTask);
                        // For paranoia, make sure we have correctly
                        // resumed the top activity.
                        if (doResume) {
                            ResumeTopActivityLocked(NULL);
                        }
                        if (onlyIfNeeded) {
                            // We don't need to start a new activity, and
                            // the client said not to do anything if that
                            // is the case, so this is it!
                            *status = ActivityManager_START_RETURN_INTENT_TO_CALLER;
                            return NOERROR;
                        }
                        top->DeliverNewIntentLocked(callingUid, r->mIntent);
                        *status = ActivityManager_START_DELIVERED_TO_TOP;
                        return NOERROR;
                    }
                }
            }
        }
    } else {
        if (r->mResultTo != NULL) {
            SendActivityResultLocked(-1,
                    r->mResultTo, r->mResultWho, r->mRequestCode,
                    Activity_RESULT_CANCELED, NULL);
        }
        *status = ActivityManager_START_CLASS_NOT_FOUND;
        return NOERROR;
    }

    Boolean newTask = FALSE;

    // Should this be considered a new task?
    if (r->mResultTo == NULL && !addingToTask
            && (launchFlags & Intent_FLAG_ACTIVITY_NEW_TASK) != 0) {
        // todo: should do better management of integers.
        mService->mCurTask++;
        if (mService->mCurTask <= 0) {
            mService->mCurTask = 1;
        }
        Int32 flags;
        r->mInfo->GetFlags(&flags);
        r->mTask = new TaskRecord(mService->mCurTask, r->mInfo, intent,
                (flags & ActivityInfo_FLAG_CLEAR_TASK_ON_LAUNCH) != 0);
        if (DEBUG_TASKS) {
            String arDes, trDes;
            r->GetDescription(&arDes);
            r->mTask->GetDescription(&trDes);
            Slogger::V(TAG, StringBuffer("Starting new activity ") + arDes
                    + " in new task " + trDes);
        }
        newTask = TRUE;
        if (mMainStack) {
            mService->AddRecentTaskLocked(r->mTask);
        }
    } else if (sourceRecord != NULL) {
        if (!addingToTask &&
                (launchFlags & Intent_FLAG_ACTIVITY_CLEAR_TOP) != 0) {
            // In this case, we are adding the activity to an existing
            // task, but the caller has asked to clear that task if the
            // activity is already running.
            AutoPtr<CActivityRecord> top = PerformClearTaskLocked(
                    sourceRecord->mTask->mTaskId, r, launchFlags, TRUE);
            if (top != NULL) {
                LogStartActivity(EventLogTags::AM_NEW_INTENT, r, top->mTask);
                top->DeliverNewIntentLocked(callingUid, r->mIntent);
                // For paranoia, make sure we have correctly
                // resumed the top activity.
                if (doResume) {
                    ResumeTopActivityLocked(NULL);
                }
                *status = ActivityManager_START_DELIVERED_TO_TOP;
                return NOERROR;
            }
        } else if (!addingToTask &&
                (launchFlags & Intent_FLAG_ACTIVITY_REORDER_TO_FRONT) != 0) {
            // In this case, we are launching an activity in our own task
            // that may already be running somewhere in the history, and
            // we want to shuffle it to the front of the stack if so.
            Int32 where = FindActivityInHistoryLocked(r, sourceRecord->mTask->mTaskId);
            if (where >= 0) {
                AutoPtr<CActivityRecord> top = MoveActivityToFrontLocked(where);
                LogStartActivity(EventLogTags::AM_NEW_INTENT, r, top->mTask);
                top->DeliverNewIntentLocked(callingUid, r->mIntent);
                if (doResume) {
                    ResumeTopActivityLocked(NULL);
                }
                *status = ActivityManager_START_DELIVERED_TO_TOP;
                return NOERROR;
            }
        }
        // An existing activity is starting this new activity, so we want
        // to keep the new one in the same task as the one that is starting
        // it.
        r->mTask = sourceRecord->mTask;
        if (DEBUG_TASKS) {
            String arDes, trDes;
            r->GetDescription(&arDes);
            r->mTask->GetDescription(&trDes);
            Slogger::V(TAG, StringBuffer("Starting new activity ") + arDes
                    + " in existing task " + trDes);
        }
    } else {
        // This not being started from an existing activity, and not part
        // of a new task...  just put it in the top task, though these days
        // this case should never happen.
        Int32 size = mHistory.GetSize();
        AutoPtr<CActivityRecord> prev = size > 0 ? mHistory[size - 1] : NULL;
        Int32 flags;
        r->mInfo->GetFlags(&flags);
        r->mTask = prev != NULL ? prev->mTask
                : new TaskRecord(mService->mCurTask, r->mInfo, intent,
                        (flags & ActivityInfo_FLAG_CLEAR_TASK_ON_LAUNCH) != 0);
        if (DEBUG_TASKS) {
            String arDes, trDes;
            r->GetDescription(&arDes);
            r->mTask->GetDescription(&trDes);
            Slogger::V(TAG, StringBuffer("Starting new activity ") + arDes
                    + " in new guessed " + trDes);
        }
    }

    if (grantedUriPermissions != NULL && callingUid > 0) {
        List<AutoPtr<IUri> >::Iterator it = grantedUriPermissions->Begin();
        for (; it != grantedUriPermissions->End(); ++ it) {
            mService->GrantUriPermissionLocked(callingUid, r->mCapsuleName,
                    (IUri*)*it, grantedMode, r->GetUriPermissionsLocked());
        }
    }

    mService->GrantUriPermissionFromIntentLocked(callingUid, r->mCapsuleName,
            intent, r->GetUriPermissionsLocked());

//    if (newTask) {
//        EventLog.writeEvent(EventLogTags.AM_CREATE_TASK, r.task.taskId);
//    }
    LogStartActivity(EventLogTags::AM_CREATE_ACTIVITY, r, r->mTask);
    StartActivityLocked(r, newTask, doResume);
    *status = ActivityManager_START_SUCCESS;
    return NOERROR;
}

ECode ActivityStack::StartActivityMayWait(
    /* [in] */ IApplicationApartment* caller,
    /* [in] */ IIntent* intent,
    /* [in] */ String resolvedType,
    /* [in] */ IObjectContainer* grantedUriPermissions,
    /* [in] */ Int32 grantedMode,
    /* [in] */ IBinder* resultTo,
    /* [in] */ const String& resultWho,
    /* [in] */ Int32 requestCode,
    /* [in] */ Boolean onlyIfNeeded,
    /* [in] */ Boolean debug,
    /* [in] */ IWaitResult* outResult,
    /* [in] */ IConfiguration* config,
    /* [out] */ Int32* status)
{
    VALIDATE_NOT_NULL(status);
    // Refuse possible leaked file descriptors
    Boolean hasFD = FALSE;
    if (intent != NULL && (intent->HasFileDescriptors(&hasFD), hasFD)) {
        Slogger::E(TAG, "File descriptors passed in Intent.\n");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<IComponentName> component;
    intent->GetComponent((IComponentName**)&component);
    Boolean componentSpecified = component != NULL;

    // Don't modify the client's object!
    AutoPtr<IIntent> newIntent;
    CIntent::New(intent, (IIntent**)&newIntent);

    // Collect information about the target of the Intent.
    AutoPtr<IActivityInfo> aInfo;
    AutoPtr<IResolveInfo> rInfo;
    ECode ec = GetCapsuleManager()->ResolveIntent(newIntent, resolvedType,
             CapsuleManager_MATCH_DEFAULT_ONLY | CActivityManagerService::STOCK_PM_FLAGS,
            (IResolveInfo**)&rInfo);
    if (SUCCEEDED(ec) && rInfo.Get()) {
        rInfo->GetActivityInfo((IActivityInfo**)&aInfo);
    }

    if (aInfo != NULL) {
        // Store the found target back into the intent, because now that
        // we have it we never want to do this again.  For example, if the
        // user navigates back to this point in the history, we should
        // always restart the exact same activity.
        String cname, name;
        AutoPtr<IApplicationInfo> appInfo;
        aInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
        appInfo->GetCapsuleName(&cname);
        aInfo->GetName(&name);
        AutoPtr<IComponentName> newComponent;
        CComponentName::New(cname, name, (IComponentName**)&newComponent);
        newIntent->SetComponent((IComponentName*)newComponent);

        // Don't debug things in the system process
//        if (debug) {
//            if (!aInfo.processName.equals("system")) {
//                mService.setDebugApp(aInfo.processName, true, false);
//            }
//        }
    }

    Mutex::Autolock lock(mService->_m_syncLock);
    Int32 callingPid;
    Int32 callingUid;
    if (caller == NULL) {
        callingPid = Binder::GetCallingPid();
        callingUid = Binder::GetCallingUid();
    } else {
        callingPid = callingUid = -1;
    }

    Int32 result;
    mConfigWillChange = config != NULL &&
            (mService->mConfiguration->Diff(config, &result), result != 0);
    if (DEBUG_CONFIGURATION) {
        Slogger::V(TAG, StringBuffer("Starting activity when config will change = ")
                + mConfigWillChange);
    }

    const Int64 origId = Binder::ClearCallingIdentity();
    if (mMainStack && aInfo != NULL) {
        AutoPtr<IApplicationInfo> appInfo;
        aInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
        Int32 flags;
        appInfo->GetFlags(&flags);
        if ((flags & ApplicationInfo_FLAG_CANT_SAVE_STATE) != 0) {
            // This may be a heavy-weight process!  Check to see if we already
            // have another, different heavy-weight process running.
            String pname, cname;
            Int32 uid;
            aInfo->GetProcessName(&pname);
            appInfo->GetCapsuleName(&cname);
            appInfo->GetUid(&uid);
            if (!pname.Compare(cname)) {
                if (mService->mHeavyWeightProcess != NULL) {
                    Int32 uid2;
                    mService->mHeavyWeightProcess->mInfo->GetUid(&uid2);
                    if (uid2 != uid || mService->mHeavyWeightProcess->mProcessName.Compare(pname)) {
                        Int32 realCallingPid = callingPid;
                        Int32 realCallingUid = callingUid;
                        if (caller != NULL) {
                            ProcessRecord* callerApp = mService->GetProcessRecordForAppLocked(caller);
                            if (callerApp != NULL) {
                                realCallingPid = callerApp->mPid;
                                callerApp->mInfo->GetUid(&realCallingUid);
                            }
                            else {
                                String appApDes, intDes;
                                caller->GetDescription(&appApDes);
                                intent->GetDescription(&intDes);
                                Slogger::W(TAG, StringBuffer("Unable to find app for caller ") + appApDes
                                        + " (pid=" + realCallingPid + ") when starting: " + intDes);
                                return ActivityManager_START_PERMISSION_DENIED;
                            }
                        }

                        AutoPtr<IIntentSender> target;
                        mService->GetIntentSenderLocked(
                                ActivityManager_INTENT_SENDER_ACTIVITY, String("elastos"),
                                realCallingUid, NULL, String(NULL), 0, intent,
                                resolvedType, PendingIntent_FLAG_CANCEL_CURRENT
                                | PendingIntent_FLAG_ONE_SHOT, (IIntentSender**)&target);

                        AutoPtr<IIntent> newIntent;
                        CIntent::New((IIntent**)&newIntent);
                        if (requestCode >= 0) {
                            // Caller is requesting a result.
                            newIntent->PutBooleanExtra(
                                    String("has_result")/*HeavyWeightSwitcherActivity.KEY_HAS_RESULT*/,
                                    TRUE);
                        }
                        AutoPtr<IParcelable> is;
                        CIntentSender::New(target.Get(), (IParcelable**)&is);
                        newIntent->PutParcelableExtra(
                                String("intent")/*HeavyWeightSwitcherActivity.KEY_INTENT*/,
                                is.Get());
                        if (mService->mHeavyWeightProcess->mActivities.GetSize() > 0) {
                            CActivityRecord* hist = mService->mHeavyWeightProcess->mActivities.GetFront();
                            newIntent->PutStringExtra(
                                    String("cur_app")/*HeavyWeightSwitcherActivity.KEY_CUR_APP*/,
                                    hist->mCapsuleName);
                            newIntent->PutInt32Extra(
                                    String("cur_task")/*HeavyWeightSwitcherActivity.KEY_CUR_TASK*/,
                                    hist->mTask->mTaskId);
                        }
                        String acname;
                        aInfo->GetCapsuleName(&acname);
                        newIntent->PutStringExtra(
                                String("new_app")/*HeavyWeightSwitcherActivity.KEY_NEW_APP*/,
                                acname);
                        Int32 flags;
                        intent->GetFlags(&flags);
                        newIntent->SetFlags(flags);
        //                newIntent.setClassName("android",
        //                        HeavyWeightSwitcherActivity.class.getName());
                        intent = newIntent.Get();
                        resolvedType = NULL;
                        caller = NULL;
                        callingUid = Binder::GetCallingUid();
                        callingPid = Binder::GetCallingPid();
                        componentSpecified = TRUE;
                        if (rInfo != NULL) rInfo->Release();
                        ECode ec = GetCapsuleManager()->ResolveIntent(newIntent, resolvedType,
                                 CapsuleManager_MATCH_DEFAULT_ONLY | CActivityManagerService::STOCK_PM_FLAGS,
                                (IResolveInfo**)&rInfo);
                        if (SUCCEEDED(ec) && rInfo.Get()) {
                            if (aInfo != NULL) aInfo->Release();
                            rInfo->GetActivityInfo((IActivityInfo**)&aInfo);
                        }
                        else {
                            aInfo = NULL;
                        }
                    }
                }
            }
        }
    }

    List<AutoPtr<IUri> >* _grantedUriPermissions = NULL;
    if (grantedUriPermissions != NULL) {
        Int32 count;
        grantedUriPermissions->GetObjectCount(&count);
        if (count > 0) {
            _grantedUriPermissions = new List<AutoPtr<IUri> >();
            AutoPtr<IObjectEnumerator> objEnum;
            grantedUriPermissions->GetObjectEnumerator(
                    (IObjectEnumerator**)&objEnum);
            Boolean hasNext;
            while(objEnum->MoveNext(&hasNext), hasNext) {
                AutoPtr<IInterface> obj;
                objEnum->Current((IInterface**)&obj);
                _grantedUriPermissions->PushBack((IUri*)obj->Probe(EIID_IUri));
            }
        }
    }

    ec = StartActivityLocked(caller, newIntent, resolvedType,
            _grantedUriPermissions, grantedMode, aInfo,
            resultTo, resultWho, requestCode, callingPid, callingUid,
            onlyIfNeeded, componentSpecified, status);

    //ToDo: when to free _grantedUriPermissions;

    if (mConfigWillChange && mMainStack) {
        // If the caller also wants to switch to a new configuration,
        // do so now.  This allows a clean switch, as we are waiting
        // for the current activity to pause (so we will not destroy
        // it), and have not yet started the next activity.
        mService->EnforceCallingPermission(
                String("elastos.permission.CHANGE_CONFIGURATION")/*android.Manifest.permission.CHANGE_CONFIGURATION*/,
                "updateConfiguration()");
        mConfigWillChange = FALSE;
        if (DEBUG_CONFIGURATION) {
            Slogger::V(TAG, "Updating to new configuration after starting activity.");
        }
        mService->UpdateConfigurationLocked(config, NULL);
    }

    Binder::RestoreCallingIdentity(origId);

    if (outResult != NULL) {
        outResult->SetResult(*status);
        if (*status == ActivityManager_START_SUCCESS) {
            mWaitingActivityLaunched.PushBack(outResult);
            Boolean isTimeout;
            AutoPtr<IComponentName> who;
            do {
//                mService.wait();
                who = NULL;
                outResult->IsTimeout(&isTimeout);
                outResult->GetWho((IComponentName**)&who);
            } while (!isTimeout && who == NULL);
        }
        else if (*status == ActivityManager_START_TASK_TO_FRONT) {
            CActivityRecord* r = GetTopRunningActivityLocked(NULL);
            if (r->mNowVisible) {
                outResult->SetTimeout(FALSE);
                String cname, name;
                r->mInfo->GetComponentName(&cname, &name);
                AutoPtr<IComponentName> who;
                CComponentName::New(cname, name, (IComponentName**)&who);
                outResult->SetWho(who);
                outResult->SetTotalTime(0);
                outResult->SetThisTime(0);
            }
            else {
                outResult->SetThisTime(SystemClock::GetUptimeMillis());
                mWaitingActivityVisible.PushBack(outResult);
                Boolean isTimeout;
                AutoPtr<IComponentName> who;
                do {
//                    mService.wait();
                    who = NULL;
                    outResult->IsTimeout(&isTimeout);
                    outResult->GetWho((IComponentName**)&who);
                } while (!isTimeout && who == NULL);
            }
        }
    }
    return ec;
}

void ActivityStack::ReportActivityLaunchedLocked(
    /* [in] */ Boolean timeout,
    /* [in] */ CActivityRecord* r,
    /* [in] */ Int64 thisTime,
    /* [in] */ Int64 totalTime)
{
    List<AutoPtr<IWaitResult> >::ReverseIterator it;
    for(it = mWaitingActivityLaunched.RBegin(); it != mWaitingActivityLaunched.REnd();
        ++it) {
        AutoPtr<IWaitResult> w = *it;
        if (r != NULL) {
            AutoPtr<IComponentName> component;
            String cName,name;
            r->mInfo->GetCapsuleName(&cName);
            r->mInfo->GetName(&name);
            CComponentName::New(cName, name, (IComponentName**)&component);
            w->SetWho(component);
        }
        w->SetThisTime(thisTime);
        w->SetTotalTime(totalTime);
    }
//    mService.notifyAll();
}

ECode ActivityStack::SendActivityResultLocked(
    /* [in] */ Int32 callingUid,
    /* [in] */ CActivityRecord* r,
    /* [in] */ const String& resultWho,
    /* [in] */ Int32 requestCode,
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent* data)
{
    ECode ec = NOERROR;
    if (callingUid > 0) {
        mService->GrantUriPermissionFromIntentLocked(callingUid, r->mCapsuleName,
                data, r->GetUriPermissionsLocked());
    }

    if (DEBUG_RESULTS) {
        String arDes, intDes;
        r->GetDescription(&arDes);
        data->GetDescription(&intDes);
        Slogger::V(TAG, StringBuffer("Send activity result to ") + arDes
            + " : who=" + resultWho + " req=" + requestCode
            + " res=" + resultCode + " data=" + intDes);
    }
    if ((CActivityRecord*)mResumedActivity == r &&
            r->mApp != NULL && r->mApp->mAppApartment != NULL) {
        AutoPtr<IResultInfo> info;
        CResultInfo::New(resultWho, requestCode,
                resultCode, data, (IResultInfo**)&info);

        AutoPtr<IObjectContainer> list;
        CParcelableObjectContainer::New((IObjectContainer**)&list);
        list->Add(info);

        ec = r->mApp->mAppApartment->ScheduleSendResult(r, list);
        if (SUCCEEDED(ec)) return ec;
        else {
            String arDes;
            r->GetDescription(&arDes);
            Slogger::W(TAG, StringBuffer("Exception thrown sending result to ") + arDes);
        }
    }

    r->AddResultLocked(NULL, resultWho, requestCode, resultCode, data);
    return ec;
}

void ActivityStack::StopActivityLocked(
    /* [in] */ CActivityRecord* r)
{
    if (DEBUG_SWITCH) Slogger::D(TAG, StringBuffer("Stopping: ") + r);
    Int32 intentFlags, infoFlags;
    r->mIntent->GetFlags(&intentFlags);
    r->mInfo->GetFlags(&infoFlags);
    if ((intentFlags & Intent_FLAG_ACTIVITY_NO_HISTORY) != 0
            || (infoFlags & ActivityInfo_FLAG_NO_HISTORY) != 0) {
        if (!r->mFinishing) {
            RequestFinishActivityLocked(r, Activity_RESULT_CANCELED, NULL,
                    "no-history");
        }
    } else if (r->mApp != NULL && r->mApp->mAppApartment != NULL) {
        if (mMainStack) {
            if ((CActivityRecord*)(mService->mFocusedActivity) == r) {
                mService->SetFocusedActivityLocked(GetTopRunningActivityLocked(NULL));
            }
        }
        r->ResumeKeyDispatchingLocked();
//        try {
            r->mStopped = FALSE;
            r->mState = ActivityState_STOPPING;
            if (DEBUG_VISBILITY) Slogger::V(
                    TAG, StringBuffer("Stopping visible=") + r->mVisible + StringBuffer(" for ") + r);
            ECode res1,res2;
            if (!r->mVisible) {
                res1 = mService->mWindowManager->SetAppVisibility(r, FALSE);
            }
            res2 = r->mApp->mAppApartment->ScheduleStopActivity(r, r->mVisible, r->mConfigChangeFlags);
//        } catch (Exception e) {
            if(res1 != NOERROR || res2 != NOERROR){
            // Maybe just ignore exceptions here...  if the process
            // has crashed, our death notification will clean things
            // up.
            Slogger::W(TAG, StringBuffer("Exception thrown during pause"));
            // Just in case, assume it to be stopped.
            r->mStopped = TRUE;
            r->mState = ActivityState_STOPPED;
            if (r->mConfigDestroy) {
                DestroyActivityLocked(r, TRUE);
            }
        }
    }
}

List<AutoPtr<CActivityRecord> >*
    ActivityStack::ProcessStoppingActivitiesLocked(
        /* [in] */ Boolean remove)
{
    if (mStoppingActivities.Begin() == mStoppingActivities.End()) return NULL;

    List<AutoPtr<CActivityRecord> >* stops = NULL;

    const Boolean nowVisible = mResumedActivity != NULL
            && mResumedActivity->mNowVisible
            && !mResumedActivity->mWaitingVisible;
    List<AutoPtr<CActivityRecord> >::Iterator it;
    for(it = mStoppingActivities.Begin(); it != mStoppingActivities.End();) {
        AutoPtr<CActivityRecord> s = *it;
        if (localLOGV) Slogger::V(TAG, StringBuffer("Stopping ") + s + StringBuffer(": nowVisible=")
                + nowVisible + StringBuffer(" waitingVisible=") + s->mWaitingVisible
                + StringBuffer(" finishing=") + s->mFinishing);
        if (s->mWaitingVisible && nowVisible) {
            mWaitingVisibleActivities.Remove(s);
            s->mWaitingVisible = FALSE;
            if (s->mFinishing) {
                // If this activity is finishing, it is sitting on top of
                // everyone else but we now know it is no longer needed...
                // so get rid of it.  Otherwise, we need to go through the
                // normal flow and hide it once we determine that it is
                // hidden by the activities in front of it.
                if (localLOGV) Slogger::V(TAG, StringBuffer("Before stopping, can hide: ") + s);
                mService->mWindowManager->SetAppVisibility(s, FALSE);
            }
        }
        if (!s->mWaitingVisible && remove) {
            if (localLOGV) Slogger::V(TAG, StringBuffer("Ready to stop: ") + s);
            if (stops == NULL) {
                stops = new List<AutoPtr<CActivityRecord> >();
            }
            stops->PushBack(s);
            it = mStoppingActivities.Erase(it);
        }
        else {
            ++it;
        }
    }

    return stops;
}

void ActivityStack::ActivityIdleInternal(
    /* [in] */ IBinder* token,
    /* [in] */ Boolean fromTimeout,
    /* [in] */ IConfiguration* config)
{
     if (localLOGV) Slogger::V(TAG, StringBuffer("Activity idle: ") + token);

     List<AutoPtr<CActivityRecord> >* stops = NULL;
     List<AutoPtr<CActivityRecord> >* finishes = NULL;
     List<AutoPtr<CActivityRecord> >* thumbnails = NULL;
     AutoPtr<IApplicationApartment> sendThumbnail = NULL;
     Boolean booting = FALSE;
     Boolean enableScreen = FALSE;

     {
        Mutex::Autolock lock(mService->_m_syncLock);
         if (token != NULL) {
    //         mHandler.removeMessages(IDLE_TIMEOUT_MSG, token);
         }

         // Get the activity record.
         Int32 index = GetIndexOfTokenLocked(token);
         if (index >= 0) {
             AutoPtr<CActivityRecord> r = (CActivityRecord*)mHistory[index];

             if (fromTimeout) {
                 ReportActivityLaunchedLocked(fromTimeout, r, -1, -1);
             }

             // This is a hack to semi-deal with a race condition
             // in the client where it can be constructed with a
             // newer configuration from when we asked it to launch.
             // We'll update with whatever configuration it now says
             // it used to launch.
             if (config != NULL) {
                 r->mConfiguration = config;
             }

             // No longer need to keep the device awake.
         //    if (mResumedActivity == r && mLaunchingActivity.isHeld()) {
         //        mHandler.removeMessages(LAUNCH_TIMEOUT_MSG);
         //        mLaunchingActivity.release();
         //    }

             // We are now idle.  If someone is waiting for a thumbnail from
             // us, we can now deliver.
             r->mIdle = TRUE;
             mService->ScheduleAppGcsLocked();
             if (r->mThumbnailNeeded && r->mApp != NULL && r->mApp->mAppApartment != NULL) {
                 sendThumbnail = r->mApp->mAppApartment;
                 r->mThumbnailNeeded = FALSE;
             }

             // If this activity is fullscreen, set up to hide those under it.

             if (DEBUG_VISBILITY) Slogger::V(TAG, StringBuffer("Idle activity for ") + r);
             EnsureActivitiesVisibleLocked(NULL, 0);

             //Slogger::I(TAG, StringBuffer("IDLE: mBooted=") + mBooted + StringBuffer(", fromTimeout=") + fromTimeout);
             if (mMainStack) {
                 if (!mService->mBooted && !fromTimeout) {
                     mService->mBooted = TRUE;
                     enableScreen = TRUE;
                 }
             }

         } else if (fromTimeout) {
             ReportActivityLaunchedLocked(fromTimeout, NULL, -1, -1);
         }

         // Atomically retrieve all of the other things to do.
         stops = ProcessStoppingActivitiesLocked(TRUE);
         if (mFinishingActivities.GetSize() > 0) {
            finishes = new List<AutoPtr<CActivityRecord> >(mFinishingActivities);
            mFinishingActivities.Clear();
         }
         if (mService->mCancelledThumbnails.GetSize() > 0) {
             thumbnails = new List<AutoPtr<CActivityRecord> >(mService->mCancelledThumbnails);
             mService->mCancelledThumbnails.Clear();
         }

         if (mMainStack) {
             booting = mService->mBooting;
             mService->mBooting = FALSE;
         }
     }

     Int32 i;

     // Send thumbnail if requested.
     if (sendThumbnail != NULL) {
    //     try {
         ECode res = sendThumbnail->RequestThumbnail(token);
    //     } catch (Exception e) {
        if(res != NOERROR){
             Slogger::W(TAG, StringBuffer("Exception thrown when requesting thumbnail") + res);
             mService->SendPendingThumbnail(NULL, token, NULL, NULL, TRUE);
         }
     }

     // Stop any activities that are scheduled to do so but have been
     // waiting for the next one to start.
     List<AutoPtr<CActivityRecord> >::Iterator it;
     if (stops != NULL) {
         for(it = stops->Begin(); it != stops->End(); ++it) {
            AutoPtr<CActivityRecord> r = *it;
             {
                Mutex::Autolock lock(mService->_m_syncLock);
                 if (r->mFinishing) {
                    AutoPtr<CActivityRecord> outR;
                     FinishCurrentActivityLocked(r, FINISH_IMMEDIATELY, (CActivityRecord**)&outR);
                 } else {
                     StopActivityLocked(r);
                 }
             }
         }
         delete stops;
     }

     // Finish any activities that are scheduled to do so but have been
     // waiting for the next one to start.
     if (finishes != NULL) {
         for(it = finishes->Begin(); it != finishes->End(); ++it) {
            AutoPtr<CActivityRecord> r = *it;
             {
                Mutex::Autolock lock(mService->_m_syncLock);
                 DestroyActivityLocked(r, TRUE);
             }
         }
         delete finishes;
     }

     // Report back to any thumbnail receivers.
     if (thumbnails != NULL) {
         for(it = thumbnails->Begin(); it != thumbnails->End(); ++it) {
            AutoPtr<CActivityRecord> r = *it;
             mService->SendPendingThumbnail(r, NULL, NULL, NULL, TRUE);
         }
         delete thumbnails;
     }

     if (booting) {
         mService->FinishBooting();
     }

     mService->TrimApplications();
     //dump();
     //mWindowManager.dump();

     if (enableScreen) {
         mService->EnableScreenAfterBoot();
     }
}

/**
 * @return Returns true if the activity is being finished, false if for
 * some reason it is being left as-is.
 */
Boolean ActivityStack::RequestFinishActivityLocked(
    /* [in] */ IBinder* token,
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent* resultData,
    /* [in] */ const char* reason)
{
    if (DEBUG_RESULTS) {
        String tkDes, intDes;
        token->GetDescription(&tkDes);
        resultData->GetDescription(&intDes);
        Slogger::V(TAG, StringBuffer("Finishing activity: token=") + tkDes
                + ", result=" + resultCode + ", data=" + intDes);
    }

    Int32 index = GetIndexOfTokenLocked(token);
    if (index < 0) {
        return FALSE;
    }
    CActivityRecord* r = mHistory[index];

    // Is this the last activity left?
    Boolean lastActivity = TRUE;
    List<AutoPtr<CActivityRecord> >::ReverseIterator
        rit = mHistory.RBegin();
    for (; rit != mHistory.REnd(); ++rit) {
        CActivityRecord* p = *rit;
        if (!p->mFinishing && p != r) {
            lastActivity = FALSE;
            break;
        }
    }

    // If this is the last activity, but it is the home activity, then
    // just don't finish it.
    if (lastActivity) {
        Boolean result;
        if (r->mIntent->HasCategory(String(Intent_CATEGORY_HOME), &result), result) {
            return FALSE;
        }
    }

    FinishActivityLocked(r, index, resultCode, resultData, reason);
    return TRUE;
}

/**
 * @return Returns true if this activity has been removed from the history
 * list, or false if it is still in the list and will be removed later.
 */
Boolean ActivityStack::FinishActivityLocked(
    /* [in] */ CActivityRecord* r,
    /* [in] */ Int32 index,
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent* resultData,
    /* [in] */ const char* reason)
{
    if (r->mFinishing) {
        String arDes;
        r->GetDescription(&arDes);
        Slogger::W(TAG, StringBuffer("Duplicate finish request for ") + arDes);
        return TRUE;
    }

    r->mFinishing = TRUE;
//    EventLog.writeEvent(EventLogTags.AM_FINISH_ACTIVITY,
//            System.identityHashCode(r),
//            r.task.taskId, r.shortComponentName, reason);
    r->mTask->mNumActivities--;
    if (index < (Int32)(mHistory.GetSize() - 1)) {
        CActivityRecord* next = (CActivityRecord*)mHistory[index + 1];
        if (next->mTask == r->mTask) {
            if (r->mFrontOfTask) {
                // The next activity is now the front of the task.
                next->mFrontOfTask = TRUE;
            }
            Int32 flags;
            r->mIntent->GetFlags(&flags);
            if ((flags & Intent_FLAG_ACTIVITY_CLEAR_WHEN_TASK_RESET) != 0) {
                // If the caller asked that this activity (and all above it)
                // be cleared when the task is reset, don't lose that information,
                // but propagate it up to the next activity.
                next->mIntent->AddFlags(Intent_FLAG_ACTIVITY_CLEAR_WHEN_TASK_RESET);
            }
        }
    }

    r->PauseKeyDispatchingLocked();
    if (mMainStack) {
        if ((CActivityRecord*)mService->mFocusedActivity == r) {
            mService->SetFocusedActivityLocked(GetTopRunningActivityLocked(NULL));
        }
    }

    // send the result
    AutoPtr<CActivityRecord> resultTo = r->mResultTo;
    if (resultTo != NULL) {
        if (DEBUG_RESULTS) {
            String arDes, intDes;
            resultTo->GetDescription(&arDes);
            resultData->GetDescription(&intDes);
            Slogger::V(TAG, StringBuffer("Adding result to ") + arDes
                + " who=" + r->mResultWho + " req=" + r->mRequestCode
                + " res=" + resultCode + " data=" + intDes);
        }
        AutoPtr<IApplicationInfo> appInfo;
        r->mInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
        Int32 uid;
        appInfo->GetUid(&uid);
        if (uid > 0) {
            mService->GrantUriPermissionFromIntentLocked(
                    uid, resultTo->mCapsuleName, resultData,
                    resultTo->GetUriPermissionsLocked());
        }
        resultTo->AddResultLocked(r, r->mResultWho, r->mRequestCode,
                resultCode, resultData);
        r->mResultTo = NULL;
    }
    else if (DEBUG_RESULTS) {
        String arDes;
        r->GetDescription(&arDes);
        Slogger::V(TAG, StringBuffer("No result destination from ") + arDes);
    }

    // Make sure this HistoryRecord is not holding on to other resources,
    // because clients have remote IPC references to this object so we
    // can't assume that will go away and want to avoid circular IPC refs.
    r->mResults = NULL;
    if (r->mPendingResults != NULL) {
        r->mPendingResults->Clear();
        delete r->mPendingResults;
        r->mPendingResults = NULL;
    }
    r->mNewIntents = NULL;
    r->mIcicle = NULL;

    if (mService->mPendingThumbnails.GetSize() > 0) {
        // There are clients waiting to receive thumbnails so, in case
        // this is an activity that someone is waiting for, add it
        // to the pending list so we can correctly update the clients.
        mService->mCancelledThumbnails.PushBack(r);
    }

    if ((CActivityRecord*)mResumedActivity == r) {
        Boolean endTask = index <= 0
                || ((CActivityRecord*)mHistory[index - 1])->mTask != r->mTask;
        if (DEBUG_TRANSITION) {
            String arDes;
            r->GetDescription(&arDes);
            Slogger::V(TAG, StringBuffer("Prepare close transition: finishing ") + arDes);
        }

        Int32 activityClose = WindowManagerPolicy::TRANSIT_ACTIVITY_CLOSE;
        mService->mWindowManager->PrepareAppTransition(endTask
                ? WindowManagerPolicy::TRANSIT_TASK_CLOSE
                : activityClose);

        // Tell window manager to prepare for this one to be removed.
        mService->mWindowManager->SetAppVisibility(r, FALSE);

        if (mPausingActivity == NULL) {
            if (DEBUG_PAUSE) {
                String arDes;
                r->GetDescription(&arDes);
                Slogger::V(TAG, StringBuffer("Finish needs to pause: ") + arDes);
            }
            if (DEBUG_USER_LEAVING) {
                Slogger::V(TAG, StringBuffer("finish() => pause with userLeaving=false"));
            }
            StartPausingLocked(FALSE, FALSE);
        }

    } else if (r->mState != ActivityState_PAUSING) {
        // If the activity is PAUSING, we will complete the finish once
        // it is done pausing; else we can just directly finish it here.
        if (DEBUG_PAUSE) {
            String arDes;
            r->GetDescription(&arDes);
            Slogger::V(TAG, StringBuffer("Finish not pausing: ") + arDes);
        }
        AutoPtr<CActivityRecord> fr;
        FinishCurrentActivityLocked(r, index,
                FINISH_AFTER_PAUSE, (CActivityRecord**)&fr);
        return fr == NULL;
    } else {
        if (DEBUG_PAUSE) {
            String arDes;
            r->GetDescription(&arDes);
            Slogger::V(TAG, StringBuffer("Finish waiting for pause of: ") + arDes);
        }
    }

    return FALSE;
}

ECode ActivityStack::FinishCurrentActivityLocked(
    /* [in] */ CActivityRecord* r,
    /* [in] */ Int32 mode,
    /* [out] */ CActivityRecord** fr)
{
    Int32 index = GetIndexOfTokenLocked(r);
    if (index < 0) {
        *fr = NULL;
        return NOERROR;
    }

    return FinishCurrentActivityLocked(r, index, mode, fr);
}

ECode ActivityStack::FinishCurrentActivityLocked(
    /* [in] */ CActivityRecord* r,
    /* [in] */ Int32 index,
    /* [in] */ Int32 mode,
    /* [out] */ CActivityRecord** fr)
{
    // First things first: if this activity is currently visible,
    // and the resumed activity is not yet visible, then hold off on
    // finishing until the resumed one becomes visible.
    if (mode == FINISH_AFTER_VISIBLE && r->mNowVisible) {
        if (Find(mStoppingActivities.Begin(), mStoppingActivities.End(),
                AutoPtr<CActivityRecord>(r)) == mStoppingActivities.End()) {
            mStoppingActivities.PushBack(r);
            if (mStoppingActivities.GetSize() > 3) {
                // If we already have a few activities waiting to stop,
                // then give up on things going idle and start clearing
                // them out.
                void (STDCALL ActivityStack::*pHandlerFunc)(
                    IBinder*, Boolean, IConfiguration*);
                pHandlerFunc = &ActivityStack::ActivityIdleInternal;

                AutoPtr<IParcel> params;
                CCallbackParcel::New((IParcel**)&params);
                params->WriteInterfacePtr(NULL);
                params->WriteBoolean(false);
                params->WriteInterfacePtr(NULL);

                mApartment->PostCppCallback(
                    (Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
            }
        }
        r->mState = ActivityState_STOPPING;
        mService->UpdateOomAdjLocked();
        *fr = r;
        if (*fr) (*fr)->AddRef();
        return NOERROR;
    }

    // make sure the record is cleaned out of other places.
    mStoppingActivities.Remove(r);
    mWaitingVisibleActivities.Remove(r);
    if (mResumedActivity.Get() == r) {
        mResumedActivity = NULL;
    }
    ActivityState prevState = r->mState;
    r->mState = ActivityState_FINISHING;

    if (mode == FINISH_IMMEDIATELY
            || prevState == ActivityState_STOPPED
            || prevState == ActivityState_INITIALIZING) {
        // If this activity is already stopped, we can just finish
        // it right now.
        if (DestroyActivityLocked(r, TRUE)) {
            *fr = NULL;
        }
        else {
            *fr = r;
            if (*fr) (*fr)->AddRef();
        }
        return NOERROR;
    } else {
        // Need to go through the full pause cycle to get this
        // activity into the stopped state and then finish it.
        if (localLOGV) {
            String arDes;
            r->GetDescription(&arDes);
            Slogger::V(TAG, StringBuffer("Enqueueing pending finish: ") + arDes);
        }
        mFinishingActivities.PushBack(r);
        ResumeTopActivityLocked(NULL);
    }
    *fr = r;
    if (*fr) (*fr)->AddRef();
    return NOERROR;
}

/**
 * Perform the common clean-up of an activity record.  This is called both
 * as part of destroyActivityLocked() (when destroying the client-side
 * representation) and cleaning things up as a result of its hosting
 * processing going away, in which case there is no remaining client-side
 * state to destroy so only the cleanup here is needed.
 */
void ActivityStack::CleanUpActivityLocked(
    /* [in] */ CActivityRecord* r,
    /* [in] */ Boolean cleanServices)
{
    if ((CActivityRecord*)mResumedActivity == r) {
        mResumedActivity = NULL;
    }
    if (mService->mFocusedActivity.Get() == r) {
        mService->mFocusedActivity = NULL;
    }

    r->mConfigDestroy = FALSE;
    r->mFrozenBeforeDestroy = FALSE;

    // Make sure this record is no longer in the pending finishes list.
    // This could happen, for example, if we are trimming activities
    // down to the max limit while they are still waiting to finish.
    mFinishingActivities.Remove(r);
    mWaitingVisibleActivities.Remove(r);

    // Remove any pending results.
    if (r->mFinishing && r->mPendingResults != NULL) {
        Set<CPendingIntentRecord*>::Iterator it = r->mPendingResults->Begin();
        for (; it != r->mPendingResults->End(); ++it) {
            CPendingIntentRecord* rec = *it;
            if (rec != NULL) {
                mService->CancelIntentSenderLocked(rec, FALSE);
            }
        }
        r->mPendingResults->Clear();
        delete r->mPendingResults;
        r->mPendingResults = NULL;
    }

    if (cleanServices) {
        CleanUpActivityServicesLocked(r);
    }

    if (mService->mPendingThumbnails.GetSize() > 0) {
        // There are clients waiting to receive thumbnails so, in case
        // this is an activity that someone is waiting for, add it
        // to the pending list so we can correctly update the clients.
        mService->mCancelledThumbnails.PushBack(r);
    }

//    // Get rid of any pending idle timeouts.
//    mHandler.removeMessages(PAUSE_TIMEOUT_MSG, r);
//    mHandler.removeMessages(IDLE_TIMEOUT_MSG, r);
}

void ActivityStack::RemoveActivityFromHistoryLocked(
    /* [in] */ CActivityRecord* r)
{
    if (r->mState != ActivityState_DESTROYED) {
        List<AutoPtr<CActivityRecord> >::Iterator it;
        it = Find(mHistory.Begin(), mHistory.End(), AutoPtr<CActivityRecord>(r));
        if (it != mHistory.End()) mHistory.Erase(it);
        r->mInHistory = FALSE;
        r->mState = ActivityState_DESTROYED;
        mService->mWindowManager->RemoveAppToken(r);
        if (VALIDATE_TOKENS) {
            mService->mWindowManager->ValidateAppTokens(mHistory);
        }
        CleanUpActivityServicesLocked(r);
        r->RemoveUriPermissionsLocked();
    }
}

/**
 * Perform clean-up of service connections in an activity record.
 */
void ActivityStack::CleanUpActivityServicesLocked(
    /* [in] */ CActivityRecord* r)
{
    // Throw away any services that have been bound by this activity.
    if (r->mConnections != NULL) {
        Set<ConnectionRecord*>::Iterator it = r->mConnections->Begin();
        for (; it != r->mConnections->End(); ++it) {
            ConnectionRecord* c = *it;
            mService->RemoveConnectionLocked(c, NULL, r);
        }
        r->mConnections->Clear();
        delete r->mConnections;
        r->mConnections = NULL;
    }
}

/**
 * Destroy the current CLIENT SIDE instance of an activity.  This may be
 * called both when actually finishing an activity, or when performing
 * a configuration switch where we destroy the current client-side object
 * but then create a new client-side object for this same HistoryRecord.
 */
Boolean ActivityStack::DestroyActivityLocked(
    /* [in] */ CActivityRecord* r,
    /* [in] */ Boolean removeFromApp)
{
    if (DEBUG_SWITCH) {
        String arDes;
        r->GetDescription(&arDes);
        Slogger::V(TAG, StringBuffer("Removing activity: token=") + arDes
                + ", app=" + (r->mApp != NULL ? r->mApp->mProcessName : String("(null)")));
    }
//    EventLog.writeEvent(EventLogTags.AM_DESTROY_ACTIVITY,
//            System.identityHashCode(r),
//            r.task.taskId, r.shortComponentName);

    Boolean removedFromHistory = FALSE;

    CleanUpActivityLocked(r, false);

    Boolean hadApp = r->mApp != NULL;

    if (hadApp) {
        if (removeFromApp) {
            r->mApp->mActivities.Remove(r);
            if (mService->mHeavyWeightProcess == r->mApp &&
                    r->mApp->mActivities.GetSize() <= 0) {
                mService->mHeavyWeightProcess = NULL;
//                mService.mHandler.sendEmptyMessage(
//                        ActivityManagerService.CANCEL_HEAVY_NOTIFICATION_MSG);
            }
            if (r->mApp->mActivities.GetSize() == 0) {
                // No longer have activities, so update location in
                // LRU list.
                mService->UpdateLRUProcessLocked(r->mApp, TRUE, FALSE);
            }
        }

        Boolean skipDestroy = FALSE;

        if (DEBUG_SWITCH) {
            String arDes;
            r->GetDescription(&arDes);
            Slogger::I(TAG, StringBuffer("Destroying: ") + arDes);
        }
        if (FAILED(r->mApp->mAppApartment->ScheduleDestroyActivity(r, r->mFinishing,
                    r->mConfigChangeFlags))) {
            // We can just ignore exceptions here...  if the process
            // has crashed, our death notification will clean things
            // up.
            Slogger::W(TAG, "Exception thrown during finish");
            if (r->mFinishing) {
                RemoveActivityFromHistoryLocked(r);
                removedFromHistory = TRUE;
                skipDestroy = TRUE;
            }
        }

        r->mApp = NULL;
        r->mNowVisible = FALSE;

        if (r->mFinishing && !skipDestroy) {
            r->mState = ActivityState_DESTROYING;
//            Message msg = mHandler.obtainMessage(DESTROY_TIMEOUT_MSG);
//            msg.obj = r;
//            mHandler.sendMessageDelayed(msg, DESTROY_TIMEOUT);
        ECode (STDCALL ActivityStack::*pHandlerFunc)(IBinder* token);
        pHandlerFunc = &ActivityStack::HandleDestroyTimeout;
        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        params->WriteInterfacePtr((IBinder*)r);
        mApartment->PostCppCallbackDelayed((Handle32)this, *(Handle32*)&pHandlerFunc, 
            params, 0, DESTROY_TIMEOUT);

        } else {
            r->mState = ActivityState_DESTROYED;
        }
    } else {
        // remove this record from the history.
        if (r->mFinishing) {
            RemoveActivityFromHistoryLocked(r);
            removedFromHistory = TRUE;
        } else {
            r->mState = ActivityState_DESTROYED;
        }
    }

    r->mConfigChangeFlags = 0;

    List<AutoPtr<CActivityRecord> >::Iterator it;
    it = Find(mLRUActivities.Begin(), mLRUActivities.End(), AutoPtr<CActivityRecord>(r));
    if (it == mLRUActivities.End() && hadApp) {
        String arDes;
        r->GetDescription(&arDes);
        Slogger::W(TAG, StringBuffer("Activity ") + arDes
                + " being finished, but not in LRU list");
    }
    else {
        mLRUActivities.Erase(it);
    }

    return removedFromHistory;
}

ECode ActivityStack::ActivityDestroyed(
    /* [in] */ IBinder* token)
{
    mService->Lock();
//    mHandler.removeMessages(DESTROY_TIMEOUT_MSG, token);

    Int32 index = GetIndexOfTokenLocked(token);
    if (index >= 0) {
        AutoPtr<CActivityRecord> r = mHistory[index];
        if (r->mState == ActivityState_DESTROYING) {
//            final long origId = Binder.clearCallingIdentity();
            RemoveActivityFromHistoryLocked(r);
//            Binder.restoreCallingIdentity(origId);
        }
    }
    mService->Unlock();

    return NOERROR;
}

void ActivityStack::RemoveHistoryRecordsForAppLocked(
    /* [in] */ List<AutoPtr<CActivityRecord> > & list,
    /* [in] */ ProcessRecord* app)
{
    if (localLOGV) {
        String appDes;
        app->GetDescription(&appDes);
        Slogger::V(TAG, StringBuffer("Removing app ") + appDes
                + " from list " /*+ list*/
                + " with " + (Int32)list.GetSize() + " entries");
    }

    List<AutoPtr<CActivityRecord> >::ReverseIterator rit = list.RBegin();
    for (; rit != list.REnd(); ++rit) {
        CActivityRecord* r = (CActivityRecord*)*rit;
        if (localLOGV) {
            String arDes, appDes;
            r->GetDescription(&arDes);
            r->mApp->GetDescription(&appDes);
            Slogger::V(TAG, StringBuffer("Record ") + arDes + ": app=" + appDes);
        }
        if (r->mApp == app) {
            if (localLOGV) {
                Slogger::V(TAG, "Removing this entry!");
            }
            list.Erase(--rit.GetBase());
        }
    }
}

void ActivityStack::RemoveHistoryRecordsForAppLocked(
    /* [in] */ ProcessRecord* app)
{
    RemoveHistoryRecordsForAppLocked(mLRUActivities, app);
    RemoveHistoryRecordsForAppLocked(mStoppingActivities, app);
    RemoveHistoryRecordsForAppLocked(mWaitingVisibleActivities, app);
    RemoveHistoryRecordsForAppLocked(mFinishingActivities, app);
}

void ActivityStack::MoveTaskToFrontLocked(
    /* [in] */ TaskRecord* tr,
    /* [in] */ CActivityRecord* reason)
{
    if (DEBUG_SWITCH) {
        String trDes;
        tr->GetDescription(&trDes);
        Slogger::V(TAG, StringBuffer("moveTaskToFront: ") + trDes);
    }

    Int32 task = tr->mTaskId;
    Int32 top = mHistory.GetSize() - 1;

    if (top < 0 || mHistory[top]->mTask->mTaskId == task) {
        // nothing to do!
        return;
    }

    AutoPtr<IObjectContainer> moved;
    CParcelableObjectContainer::New((IObjectContainer**)&moved);

    // Applying the affinities may have removed entries from the history,
    // so get the size again.
    top = mHistory.GetSize() - 1;
    Int32 pos = top;

    // Shift all activities with this task up to the top
    // of the stack, keeping them in the same internal order.
    while (pos >= 0) {
        AutoPtr<CActivityRecord> r = mHistory[pos];
        if (localLOGV) {
            String trDes, arDes;
            r->mTask->GetDescription(&trDes);
            r->GetDescription(&arDes);
            Slogger::V(TAG, StringBuffer("At ") + pos + " ckp "
                    + trDes + ": " + arDes);
        }
        Boolean first = TRUE;
        if (r->mTask->mTaskId == task) {
            if (localLOGV) {
                Slogger::V(TAG, StringBuffer("Removing and adding at ") + top);
            }
            List<AutoPtr<CActivityRecord> >::Iterator it1 = mHistory.Begin();
            for (Int32 idx = pos; idx > 0; --idx) ++it1;
            mHistory.Erase(it1);
            it1 = mHistory.Begin();
            for (Int32 idx = top; idx > 0; --idx) ++it1;
            mHistory.Insert(it1, r);
            moved->Add((IBinder*)r);
            top--;
            if (first && mMainStack) {
                mService->AddRecentTaskLocked(r->mTask);
                first = FALSE;
            }
        }
        pos--;
    }

    if (DEBUG_TRANSITION) {
        String trDes;
        tr->GetDescription(&trDes);
        Slogger::V(TAG, StringBuffer("Prepare to front transition: task=") + trDes);
    }
    if (reason != NULL) {
        Int32 flags;
        reason->mIntent->GetFlags(&flags);
        if ((flags & Intent_FLAG_ACTIVITY_NO_ANIMATION) != 0) {
            mService->mWindowManager->PrepareAppTransition(WindowManagerPolicy::TRANSIT_NONE);
            CActivityRecord* r = GetTopRunningActivityLocked(NULL);
            if (r != NULL) {
                mNoAnimActivities.PushBack(r);
            }
        }
        else {
            mService->mWindowManager->PrepareAppTransition(WindowManagerPolicy::TRANSIT_TASK_TO_FRONT);
        }
    }
    else {
        mService->mWindowManager->PrepareAppTransition(WindowManagerPolicy::TRANSIT_TASK_TO_FRONT);
    }

    mService->mWindowManager->MoveAppTokensToTop(moved.Get());
    if (VALIDATE_TOKENS) {
        mService->mWindowManager->ValidateAppTokens(mHistory);
    }

    FinishTaskMoveLocked(task);
//    EventLog.writeEvent(EventLogTags.AM_TASK_TO_FRONT, task);
}

void ActivityStack::FinishTaskMoveLocked(
    /* [in] */ Int32 task)
{
    ResumeTopActivityLocked(NULL);
}

Boolean ActivityStack::MoveTaskToBackLocked(
    /* [in] */ Int32 task,
    /* [in] */ CActivityRecord* reason)
{
    Slogger::I(TAG, StringBuffer("moveTaskToBack: ") + task);

    // If we have a watcher, preflight the move before committing to it.  First check
    // for *other* available tasks, but if none are available, then try again allowing the
    // current task to be selected.
    if (mMainStack && mService->mController != NULL) {
        AutoPtr<CActivityRecord> next = GetTopRunningActivityLocked(NULL, task);
        if (next == NULL) {
            next = GetTopRunningActivityLocked(NULL, 0);
        }
        if (next != NULL) {
            // ask watcher if this is allowed
            Boolean moveOK = TRUE;
//            try {
                mService->mController->ActivityResuming(next->mCapsuleName,&moveOK);
//            } catch (RemoteException e) {
//                mService.mController = null;
//            }
            if (!moveOK) {
                mService->mController = NULL;
                return FALSE;
            }
        }
    }

    AutoPtr<IObjectContainer> moved;

    if (DEBUG_TRANSITION) Slogger::V(TAG,
            StringBuffer("Prepare to back transition: task=") + task);

    const Int32 N = mHistory.GetSize();
    Int32 bottom = 0;
    Int32 pos = 0;

    // Shift all activities with this task down to the bottom
    // of the stack, keeping them in the same internal order.
    while (pos < N) {
        AutoPtr<CActivityRecord> r = (CActivityRecord*)mHistory[pos];
        if (localLOGV) Slogger::V(
            TAG, StringBuffer("At ") + pos + StringBuffer(" ckp ") + r->mTask + StringBuffer(": ") + r);
        if (r->mTask->mTaskId == task) {
            if (localLOGV) Slogger::V(TAG, StringBuffer("Removing and adding at ") + (N-1));
            mHistory.Remove(pos);
            mHistory.Insert(bottom, r);
            moved->Add((IBinder*)r);
            bottom++;
        }
        pos++;
    }

    Int32 gFlags;
    reason->mIntent->GetFlags(&gFlags);
    if (reason != NULL &&
            (gFlags & Intent_FLAG_ACTIVITY_NO_ANIMATION) != 0) {
        mService->mWindowManager->PrepareAppTransition(WindowManagerPolicy_TRANSIT_NONE);
        AutoPtr<CActivityRecord> r = GetTopRunningActivityLocked(NULL);
        if (r != NULL) {
            mNoAnimActivities.PushBack(r);
        }
    } else {
        mService->mWindowManager->PrepareAppTransition(WindowManagerPolicy_TRANSIT_TASK_TO_BACK);
    }
    mService->mWindowManager->MoveAppTokensToBottom(moved);
    if (VALIDATE_TOKENS) {
        mService->mWindowManager->ValidateAppTokens(mHistory);
    }

    FinishTaskMoveLocked(task);
    return TRUE;
}


void ActivityStack::LogStartActivity(
    /* [in] */ Int32 tag,
    /* [in] */ CActivityRecord* r,
    /* [in] */ TaskRecord* task)
{
//    EventLog.writeEvent(tag,
//                System.identityHashCode(r), task.taskId,
//                r.shortComponentName, r.intent.getAction(),
//                r.intent.getType(), r.intent.getDataString(),
//                r.intent.getFlags());
}

/**
 * Make sure the given activity matches the current configuration.  Returns
 * false if the activity had to be destroyed.  Returns true if the
 * configuration is the same, or the activity will remain running as-is
 * for whatever reason.  Ensures the HistoryRecord is updated with the
 * correct configuration and all other bookkeeping is handled.
 */
Boolean ActivityStack::EnsureActivityConfigurationLocked(
    /* [in] */ CActivityRecord* r,
    /* [in] */ Int32 globalChanges)
{
    if (mConfigWillChange) {
        if (DEBUG_SWITCH || DEBUG_CONFIGURATION) {
            String arDes;
            r->GetDescription(&arDes);
            Slogger::V(TAG, StringBuffer("Skipping config check (will change): ")
                    + arDes);
        }
        return TRUE;
    }

    if (DEBUG_SWITCH || DEBUG_CONFIGURATION) {
        String arDes;
        r->GetDescription(&arDes);
        Slogger::V(TAG, StringBuffer("Ensuring correct configuration: ") + arDes);
    }

    // Short circuit: if the two configurations are the exact same
    // object (the common case), then there is nothing to do.
    AutoPtr<IConfiguration> newConfig = mService->mConfiguration;
    if (r->mConfiguration == newConfig) {
        if (DEBUG_SWITCH || DEBUG_CONFIGURATION) {
            String arDes;
            r->GetDescription(&arDes);
            Slogger::V(TAG, StringBuffer("Configuration unchanged in ") + arDes);
        }
        return TRUE;
    }

    // We don't worry about activities that are finishing.
    if (r->mFinishing) {
        if (DEBUG_SWITCH || DEBUG_CONFIGURATION) {
            String arDes;
            r->GetDescription(&arDes);
            Slogger::V(TAG, StringBuffer("Configuration doesn't matter in finishing ")
                    + arDes);
        }
        r->StopFreezingScreenLocked(FALSE);
        return TRUE;
    }

    // Okay we now are going to make this activity have the new config.
    // But then we need to figure out how it needs to deal with that.
    AutoPtr<IConfiguration> oldConfig = r->mConfiguration;
    r->mConfiguration = newConfig;

    // If the activity isn't currently running, just leave the new
    // configuration and it will pick that up next time it starts.
    if (r->mApp == NULL || r->mApp->mAppApartment == NULL) {
        if (DEBUG_SWITCH || DEBUG_CONFIGURATION) {
            String arDes;
            r->GetDescription(&arDes);
            Slogger::V(TAG, StringBuffer("Configuration doesn't matter not running ")
                    + arDes);
        }
        r->StopFreezingScreenLocked(FALSE);
        return TRUE;
    }

    // Figure out what has changed between the two configurations.
    Int32 changes;
    oldConfig->Diff(newConfig.Get(), &changes);
    if (DEBUG_SWITCH || DEBUG_CONFIGURATION) {
        String cfgDes, name;
        Int32 confChanges;
        newConfig->GetDescription(&cfgDes);
        r->mInfo->GetName(&name);
        r->mInfo->GetConfigChanges(&confChanges);
        Slogger::V(TAG, StringBuffer("Checking to restart ")
                + name + ": changed="
                + changes + ", handles="
                + confChanges
                + ", newConfig=" + cfgDes);
    }
    Int32 confChanges;
    r->mInfo->GetConfigChanges(&confChanges);
    if ((changes & ~confChanges) != 0) {
        // Aha, the activity isn't handling the change, so DIE DIE DIE.
        r->mConfigChangeFlags |= changes;
        r->StartFreezingScreenLocked(r->mApp, globalChanges);
        if (r->mApp == NULL || r->mApp->mAppApartment == NULL) {
            if (DEBUG_SWITCH || DEBUG_CONFIGURATION) {
                String arDes;
                r->GetDescription(&arDes);
                Slogger::V(TAG,
                        StringBuffer("Switch is destroying non-running ") + arDes);
            }
            DestroyActivityLocked(r, TRUE);
        } else if (r->mState == ActivityState_PAUSING) {
            // A little annoying: we are waiting for this activity to
            // finish pausing.  Let's not do anything now, but just
            // flag that it needs to be restarted when done pausing.
            if (DEBUG_SWITCH || DEBUG_CONFIGURATION) {
                String arDes;
                r->GetDescription(&arDes);
                Slogger::V(TAG,
                        StringBuffer("Switch is skipping already pausing ") + arDes);
            }
            r->mConfigDestroy = TRUE;
            return TRUE;
        } else if (r->mState == ActivityState_RESUMED) {
            // Try to optimize this case: the configuration is changing
            // and we need to restart the top, resumed activity.
            // Instead of doing the normal handshaking, just say
            // "restart!".
            if (DEBUG_SWITCH || DEBUG_CONFIGURATION) {
                String arDes;
                r->GetDescription(&arDes);
                Slogger::V(TAG,
                        StringBuffer("Switch is restarting resumed ") + arDes);
            }
            RelaunchActivityLocked(r, r->mConfigChangeFlags, TRUE);
            r->mConfigChangeFlags = 0;
        } else {
            if (DEBUG_SWITCH || DEBUG_CONFIGURATION) {
                String arDes;
                r->GetDescription(&arDes);
                Slogger::V(TAG,
                        StringBuffer("Switch is restarting non-resumed ") + arDes);
            }
            RelaunchActivityLocked(r, r->mConfigChangeFlags, FALSE);
            r->mConfigChangeFlags = 0;
        }

        // All done...  tell the caller we weren't able to keep this
        // activity around.
        return FALSE;
    }

    // Default case: the activity can handle this new configuration, so
    // hand it over.  Note that we don't need to give it the new
    // configuration, since we always send configuration changes to all
    // process when they happen so it can just use whatever configuration
    // it last got.
    if (r->mApp != NULL && r->mApp->mAppApartment != NULL) {
        if (DEBUG_CONFIGURATION) {
            String arDes;
            r->GetDescription(&arDes);
            Slogger::V(TAG, StringBuffer("Sending new config to ") + arDes);
        }
        r->mApp->mAppApartment->ScheduleActivityConfigurationChanged(r);
    }
    r->StopFreezingScreenLocked(FALSE);

    return TRUE;
}

Boolean ActivityStack::RelaunchActivityLocked(
    /* [in] */ CActivityRecord* r,
    /* [in] */ Int32 changes,
    /* [in] */ Boolean andResume)
{
    AutoPtr<IObjectContainer> results, newIntents;
    if (andResume) {
        if (r->mResults != NULL && r->mResults->GetSize() > 0) {
            CParcelableObjectContainer::New((IObjectContainer**)&results);
            List<ActivityResult*>::Iterator it = r->mResults->Begin();
            for (; it != r->mResults->End(); ++it) {
                ActivityResult* ar = *it;
                results->Add((IParcelable*)ar->mResultInfo.Get());
            }
        }
        if (r->mNewIntents != NULL && r->mNewIntents->GetSize() > 0) {
            CParcelableObjectContainer::New((IObjectContainer**)&newIntents);
            List<AutoPtr<IIntent> >::Iterator it = r->mNewIntents->Begin();
            for (; it != r->mNewIntents->End(); ++it) {
                newIntents->Add((*it).Get());
            }
        }
    }

    if (DEBUG_SWITCH) {
        String arDes;
        r->GetDescription(&arDes);
        Slogger::V(TAG, StringBuffer("Relaunching: ") + arDes
//            + " with results=" + results + " newIntents=" + newIntents
                + " andResume=" + andResume);
    }
//    EventLog.writeEvent(andResume ? EventLogTags.AM_RELAUNCH_RESUME_ACTIVITY
//            : EventLogTags.AM_RELAUNCH_ACTIVITY, System.identityHashCode(r),
//            r.task.taskId, r.shortComponentName);

    r->StartFreezingScreenLocked(r->mApp, 0);

    if (DEBUG_SWITCH) {
        String arDes;
        r->GetDescription(&arDes);
        Slogger::I(TAG, StringBuffer("Switch is restarting resumed ") + arDes);
    }
    if (SUCCEEDED(r->mApp->mAppApartment->ScheduleRelaunchActivity(
            r, results, newIntents, changes, !andResume, mService->mConfiguration))) {
        // Note: don't need to call pauseIfSleepingLocked() here, because
        // the caller will only pass in 'andResume' if this activity is
        // currently resumed, which implies we aren't sleeping.
    }
    else {
        return FALSE;
    }

    if (andResume) {
        if (r->mResults) {
            r->mResults->Clear();
            delete r->mResults;
            r->mResults = NULL;
        }
        if (r->mNewIntents) {
            r->mNewIntents->Clear();
            delete r->mNewIntents;
            r->mNewIntents = NULL;
        }
        if (mMainStack) {
            mService->ReportResumedActivityLocked(r);
        }
    }

    return TRUE;
}


Millisecond64 ActivityStack::GetInitialStartTime()
{
    return mInitialStartTime;
}

void ActivityStack::SetInitialStartTime(
    /* [in] */ Millisecond64 time)
{
    mInitialStartTime = time;
}

void ActivityStack::ReportActivityVisibleLocked(
        /* [in] */ CActivityRecord* r)
{
    List<AutoPtr<IWaitResult> >::ReverseIterator it;
    Int32 i = mWaitingActivityVisible.GetSize();
    for(it = mWaitingActivityVisible.RBegin(); it != mWaitingActivityVisible.REnd(); it){
        AutoPtr<IWaitResult> w = *it;
        w->SetTimeout(FALSE);
        String cName, name;
        r->mInfo->GetCapsuleName(&cName);
        r->mInfo->GetName(&name);
        if(r != NULL){
            AutoPtr<IComponentName> component;
            CComponentName::New(cName, name, (IComponentName**)&component);
            w->SetWho(component);
        }
        Int64 thisTime;
        w->GetThisTime(&thisTime);
        w->SetTotalTime(SystemClock::GetUptimeMillis() - thisTime);
        Int64 totalTime;
        w->GetTotalTime(&totalTime);
        w->SetThisTime(totalTime);
    }
//    mService.notifyAll();
}

List<AutoPtr<CActivityRecord> >* ActivityStack::GetWaitingVisibleActivities()
{
    return &mWaitingVisibleActivities;
}

AutoPtr<CActivityRecord> ActivityStack::GetResumedActivity()
{
    return mResumedActivity;
}

AutoPtr<CActivityRecord> ActivityStack::GetPausingActivity()
{
    return mPausingActivity;
}

ECode ActivityStack::HandlePauseTimeout(
    /* [in] */ IBinder* token)
{
    // We don't at this point know if the activity is fullscreen,
    // so we need to be conservative and assume it isn't.
    Slogger::W(TAG, String("Activity pause timeout for ") + token);
    return ActivityPaused(token, NULL, TRUE);
}

ECode ActivityStack::HandleIdleTimeout(
    /* [in] */ IBinder* token)
{
    if (mService->mDidDexOpt) {
        mService->mDidDexOpt = FALSE;
        ECode (STDCALL ActivityStack::*pHandlerFunc)(
           IBinder* token);
        pHandlerFunc = &ActivityStack::HandleIdleTimeout;
        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        params->WriteInterfacePtr((IBinder*)token);
        mApartment->PostCppCallbackDelayed((Handle32)this, *(Handle32*)&pHandlerFunc, 
            params, 0, IDLE_TIMEOUT);
        return NOERROR;
    }
    // We don't at this point know if the activity is fullscreen,
    // so we need to be conservative and assume it isn't.
    Slogger::W(TAG, String("Activity idle timeout for ") + token);
    ActivityIdleInternal(token, TRUE, NULL);
    return NOERROR;
}

ECode ActivityStack::HandleDestroyTimeout(
    /* [in] */ IBinder* token)
{
    // We don't at this point know if the activity is fullscreen,
    // so we need to be conservative and assume it isn't.
    Slogger::W(TAG, String("Activity destroy timeout for ") + token);
    return ActivityDestroyed(token);
}

ECode ActivityStack::HandleIdleNow(
    /* [in] */ IBinder* token)
{
    ActivityIdleInternal(token, FALSE, NULL);
    return NOERROR;
}

ECode ActivityStack::HandleLaunchTimeout()
{
    if (mService->mDidDexOpt) {
        mService->mDidDexOpt = FALSE;
        ECode (STDCALL ActivityStack::*pHandlerFunc)();
        pHandlerFunc = &ActivityStack::HandleLaunchTimeout;
        mApartment->PostCppCallbackDelayed((Handle32)this, *(Handle32*)&pHandlerFunc, 
            NULL, 0, LAUNCH_TIMEOUT);
        return NOERROR;
    }
    Mutex::Autolock lock(mService->_m_syncLock);
    // if (mLaunchingActivity->IsHeld()) {
    //     Slogger::W(TAG, "Launch timeout has expired, giving up wake lock!");
    //     mLaunchingActivity->Release();
    // }
    return NOERROR;
}
