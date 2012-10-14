
#ifndef __ACTIVITYSTACK_H__
#define __ACTIVITYSTACK_H__

#include "server/CActivityManagerService.h"
#include "server/ProcessRecord.h"
#include "server/CActivityRecord.h"
#include <elastos/AutoPtr.h>

class CActivityManagerService;
class CActivityRecord;

/**
 * State and management of a single stack of activities.
 */
class ActivityStack
{
public:
    static const char* TAG;
    static const Boolean localLOGV;
    static const Boolean DEBUG_SWITCH;
    static const Boolean DEBUG_PAUSE;
    static const Boolean DEBUG_VISBILITY;
    static const Boolean DEBUG_USER_LEAVING;
    static const Boolean DEBUG_TRANSITION;
    static const Boolean DEBUG_RESULTS;
    static const Boolean DEBUG_CONFIGURATION;
    static const Boolean DEBUG_TASKS;

    static const Boolean VALIDATE_TOKENS;

    // How long we wait until giving up on the last activity to pause.  This
    // is short because it directly impacts the responsiveness of starting the
    // next activity.
    static const Int32 PAUSE_TIMEOUT = 500;

    /**
     * How long we can hold the launch wake lock before giving up.
     */
    static const Int32 LAUNCH_TIMEOUT = 10*1000;

    // How long until we reset a task when the user returns to it.  Currently
    // 30 minutes.
    static const Millisecond64 ACTIVITY_INACTIVE_RESET_TIME = 1000*60*30;

    // Set to false to disable the preview that is shown while a new activity
    // is being started.
    static const Boolean SHOW_APP_STARTING_PREVIEW = TRUE;

    static const Int32 FINISH_IMMEDIATELY = 0;
    static const Int32 FINISH_AFTER_PAUSE = 1;
    static const Int32 FINISH_AFTER_VISIBLE = 2;


public:
    ActivityStack(
        /* [in] */ CActivityManagerService* service,
        /* [in] */ IContext* context,
        /* [in] */ Boolean mainStack);

    CARAPI_(CActivityRecord*) GetTopRunningActivityLocked(
        /* [in] */ CActivityRecord* notTop);

    CARAPI_(CActivityRecord*) GetTopRunningNonDelayedActivityLocked(
        /* [in] */ CActivityRecord* notTop);

    CARAPI_(CActivityRecord*) GetTopRunningActivityLocked(
        /* [in] */ IBinder* token,
        /* [in] */ Int32 taskId);

    CARAPI_(Int32) GetIndexOfTokenLocked(
        /* [in] */ IBinder* token);

    CARAPI_(Boolean) UpdateLRUListLocked(
        /* [in] */ CActivityRecord* r);

    CARAPI_(CActivityRecord*) FindTaskLocked(
        /* [in] */ IIntent* intent,
        /* [in] */ IActivityInfo* info);

    CARAPI_(CActivityRecord*) FindActivityLocked(
        /* [in] */ IIntent* intent,
        /* [in] */ IActivityInfo* info);

    CARAPI RealStartActivityLocked(
        /* [in] */ CActivityRecord* r,
        /* [in] */ ProcessRecord* app,
        /* [in] */ Boolean andResume,
        /* [in] */ Boolean checkConfig,
        /* [out] */ Boolean* succeeded);

    CARAPI StartSpecificActivityLocked(
        /* [in] */ CActivityRecord* r,
        /* [in] */ Boolean andResume,
        /* [in] */ Boolean checkConfig);

    CARAPI_(void) PauseIfSleepingLocked();

    CARAPI StartPausingLocked(
        /* [in] */ Boolean userLeaving,
        /* [in] */ Boolean uiSleeping);

    CARAPI ActivityPaused(
        /* [in] */ IBinder* token,
        /* [in] */ IBundle* icicle,
        /* [in] */ Boolean timeout);

    CARAPI_(void) CompletePauseLocked();

    CARAPI_(void) CompleteResumeLocked(
        /* [in] */ CActivityRecord* next);

    CARAPI_(void) EnsureActivitiesVisibleLocked(
        /* [in] */ CActivityRecord* top,
        /* [in] */ CActivityRecord* starting,
        /* [in] */ const char* onlyThisProcess,
        /* [in] */ Int32 configChanges);

    CARAPI_(void) EnsureActivitiesVisibleLocked(
        /* [in] */ CActivityRecord* starting,
        /* [in] */ Int32 configChanges);

    CARAPI_(Boolean) ResumeTopActivityLocked(
        /* [in] */ CActivityRecord* prev);

    CARAPI_(void) StartActivityLocked(
        /* [in] */ CActivityRecord* r,
        /* [in] */ Boolean newTask,
        /* [in] */ Boolean doResume);

    CARAPI_(CActivityRecord*) ResetTaskIfNeededLocked(
        /* [in] */ CActivityRecord* taskTop,
        /* [in] */ CActivityRecord* newActivity);

    CARAPI_(CActivityRecord*) PerformClearTaskLocked(
        /* [in] */ Int32 taskId,
        /* [in] */ CActivityRecord* newR,
        /* [in] */ Int32 launchFlags,
        /* [in] */ Boolean doClear);

    CARAPI_(Int32) FindActivityInHistoryLocked(
        /* [in] */ CActivityRecord* r,
        /* [in] */ Int32 task);

    CARAPI_(CActivityRecord*) MoveActivityToFrontLocked(
        /* [in] */ Int32 where);

    CARAPI StartActivityLocked(
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
        /* [in] */ Int32* status);

    CARAPI StartActivityUncheckedLocked(
        /* [in] */ CActivityRecord* r,
        /* [in] */ CActivityRecord* sourceRecord,
        /* [in] */ List<AutoPtr<IUri> >* grantedUriPermissions,
        /* [in] */ Int32 grantedMode,
        /* [in] */ Boolean onlyIfNeeded,
        /* [in] */ Boolean doResume,
        /* [out] */ Int32* status);

    CARAPI StartActivityMayWait(
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
        /* [out] */ Int32* status);

    CARAPI SendActivityResultLocked(
        /* [in] */ Int32 callingUid,
        /* [in] */ CActivityRecord* r,
        /* [in] */ const String& resultWho,
        /* [in] */ Int32 requestCode,
        /* [in] */ Int32 resultCode,
        /* [in] */ IIntent* data);

    CARAPI_(Boolean) RequestFinishActivityLocked(
        /* [in] */ IBinder* token,
        /* [in] */ Int32 resultCode,
        /* [in] */ IIntent* resultData,
        /* [in] */ const char* reason);

    CARAPI_(Boolean)FinishActivityLocked(
        /* [in] */ CActivityRecord* r,
        /* [in] */ Int32 index,
        /* [in] */ Int32 resultCode,
        /* [in] */ IIntent* resultData,
        /* [in] */ const char* reason);

    CARAPI FinishCurrentActivityLocked(
        /* [in] */ CActivityRecord* r,
        /* [in] */ Int32 mode,
        /* [out] */ CActivityRecord** fr);

    CARAPI FinishCurrentActivityLocked(
        /* [in] */ CActivityRecord* r,
        /* [in] */ Int32 index,
        /* [in] */ Int32 mode,
        /* [out] */ CActivityRecord** fr);

    CARAPI_(void) CleanUpActivityLocked(
        /* [in] */ CActivityRecord* r,
        /* [in] */ Boolean cleanServices);

    CARAPI_(void) RemoveActivityFromHistoryLocked(
        /* [in] */ CActivityRecord* r);

    CARAPI_(void) CleanUpActivityServicesLocked(
        /* [in] */ CActivityRecord* r);

    CARAPI_(Boolean) DestroyActivityLocked(
        /* [in] */ CActivityRecord* r,
        /* [in] */ Boolean removeFromApp);

    CARAPI ActivityDestroyed(
        /* [in] */ IBinder* token);

    CARAPI_(void) MoveTaskToFrontLocked(
        /* [in] */ TaskRecord* tr,
        /* [in] */ CActivityRecord* reason);

    CARAPI_(void) FinishTaskMoveLocked(
        /* [in] */ Int32 task);

    CARAPI_(void) LogStartActivity(
        /* [in] */ Int32 tag,
        /* [in] */ CActivityRecord* r,
        /* [in] */ TaskRecord* task);

    CARAPI_(Boolean) EnsureActivityConfigurationLocked(
        /* [in] */ CActivityRecord* r,
        /* [in] */ Int32 globalChanges);

    CARAPI_(Boolean) RelaunchActivityLocked(
        /* [in] */ CActivityRecord* r,
        /* [in] */ Int32 changes,
        /* [in] */ Boolean andResume);

private:
    CARAPI_(void) RemoveHistoryRecordsForAppLocked(
        /* [in] */ List<AutoPtr<CActivityRecord> > & list,
        /* [in] */ ProcessRecord* app);

    CARAPI_(void) RemoveHistoryRecordsForAppLocked(
        /* [in] */ ProcessRecord* app);

    CARAPI_(ICapsuleManager*) GetCapsuleManager();

private:
    friend class CActivityManagerService;

    AutoPtr<CActivityManagerService> mService;
    Boolean mMainStack;

    AutoPtr<IContext> mContext;
     /**
     * The back history of all previous (and possibly still
     * running) activities.  It contains HistoryRecord objects.
     */
    List<AutoPtr<CActivityRecord> > mHistory;

    /**
     * List of running activities, sorted by recent usage.
     * The first entry in the list is the least recently used.
     * It contains HistoryRecord objects.
     */
    List<AutoPtr<CActivityRecord> > mLRUActivities;

    /**
     * List of activities that are waiting for a new activity
     * to become visible before completing whatever operation they are
     * supposed to do.
     */
    List<AutoPtr<CActivityRecord> > mWaitingVisibleActivities;

    /**
     * List of activities that are ready to be stopped, but waiting
     * for the next activity to settle down before doing so.  It contains
     * HistoryRecord objects.
     */
    List<AutoPtr<CActivityRecord> > mStoppingActivities;

    /**
     * Animations that for the current transition have requested not to
     * be considered for the transition animation.
     */
    List<AutoPtr<CActivityRecord> > mNoAnimActivities;

    /**
     * List of activities that are ready to be finished, but waiting
     * for the previous activity to settle down before doing so.  It contains
     * HistoryRecord objects.
     */
    List<AutoPtr<CActivityRecord> > mFinishingActivities;

    /**
     * List of people waiting to find out about the next launched activity.
     */
    List<AutoPtr<IWaitResult> > mWaitingActivityLaunched;

    /**
     * List of people waiting to find out about the next visible activity.
     */
    List<AutoPtr<IWaitResult> > mWaitingActivityVisible;

    /**
     * When we are in the process of pausing an activity, before starting the
     * next one, this variable holds the activity that is currently being paused.
     */
    AutoPtr<CActivityRecord> mPausingActivity;

    /**
     * This is the last activity that we put into the paused state.  This is
     * used to determine if we need to do an activity transition while sleeping,
     * when we normally hold the top activity paused.
     */
    AutoPtr<CActivityRecord> mLastPausedActivity;

    /**
     * Current activity that is resumed, or null if there is none.
     */
    AutoPtr<CActivityRecord> mResumedActivity;

    /**
     * Set when we know we are going to be calling updateConfiguration()
     * soon, so want to skip intermediate config checks.
     */
    Boolean mConfigWillChange;

    /**
     * Set to indicate whether to issue an onUserLeaving callback when a
     * newly launched activity is being brought in front of us.
     */
    Boolean mUserLeaving;

    Millisecond64 mInitialStartTime;
};

#endif //__ACTIVITYSTACK_H__
