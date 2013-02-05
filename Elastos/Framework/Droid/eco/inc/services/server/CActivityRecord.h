
#ifndef __CHISTORYRECORD_H__
#define __CHISTORYRECORD_H__

#include "_CActivityRecord.h"
#include "server/ActivityStack.h"
#include "server/ProcessRecord.h"
#include "server/ActivityState.h"
#include "server/ActivityResult.h"
#include "server/TaskRecord.h"
#include "server/CPendingIntentRecord.h"
#include "server/UriPermissionOwner.h"
#include <elastos/List.h>
#include <elastos/AutoPtr.h>
#include <elastos/Set.h>

class ActivityStack;
class ProcessRecord;
class ActivityResult;
class ConnectionRecord;
class CActivityManagerService;
class UriPermissionOwner;

CarClass(CActivityRecord)
{
public:
    CActivityRecord();

    ~CActivityRecord();

    CARAPI GetDescription(
        /* [out] */ String* description);

    CARAPI WindowsVisible();

    CARAPI WindowsGone();

    CARAPI KeyDispatchingTimedOut(
        /* [out] */ Boolean* result);

    CARAPI GetKeyDispatchingTimeout(
        /* [out] */ Millisecond64* timeout);

public:
    CARAPI Init(
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
        /* [in] */ Boolean componentSpecified);

    CARAPI_(UriPermissionOwner*) GetUriPermissionsLocked();

    CARAPI_(void) AddResultLocked(
        /* [in] */ CActivityRecord* from,
        /* [in] */ const String& resultWho,
        /* [in] */ Int32 requestCode,
        /* [in] */ Int32 resultCode,
        /* [in] */ IIntent* resultData);

    CARAPI_(void) RemoveResultsLocked(
        /* [in] */ CActivityRecord* from,
        /* [in] */ const String& resultWho,
        /* [in] */ Int32 requestCode);

    CARAPI_(void) AddNewIntentLocked(
        /* [in] */ IIntent* intent);

    CARAPI_(void) DeliverNewIntentLocked(
        /* [in] */ Int32 callingUid,
        /* [in] */ IIntent* intent);

    CARAPI_(void) RemoveUriPermissionsLocked();

    CARAPI_(void) PauseKeyDispatchingLocked();

    CARAPI_(void) ResumeKeyDispatchingLocked();

    CARAPI_(Boolean) MayFreezeScreenLocked(
        /* [in] */ ProcessRecord* app);

    CARAPI_(void) StartFreezingScreenLocked(
        /* [in] */ ProcessRecord* app,
        /* [in] */ Int32 configChanges);

    CARAPI_(void) StopFreezingScreenLocked(
        /* [in] */ Boolean force);

    CARAPI_(Int32) GetHashCode();

public:
    AutoPtr<CActivityManagerService> mService; // owner
    ActivityStack* mStack; // owner
    AutoPtr<IActivityInfo> mInfo; // all about me
    Int32 mLaunchedFromUid; // always the uid who started the activity.
    AutoPtr<IIntent> mIntent;
    AutoPtr<IComponentName> mRealActivity;  // the intent component, or target of an alias.

    AutoPtr<IBinder> mCaller;
    Boolean mSendResult;
    String mActivityName;
    String mResolvedType; // as per original caller;
    String mCapsuleName; // the package implementing intent's component
    String mProcessName; // process where this component wants to run
    String mTaskAffinity; // as per ActivityInfo.taskAffinity
    Boolean mStateNotNeeded; // As per ActivityInfo.flags
    Boolean mFullscreen;     // covers the full screen?
    Boolean mIsHomeActivity; // do we consider this to be a home activity?
    ArrayOf<Char8>* mNonLocalizedLabel;  // the label information from the package mgr.
    Int32 mLabelRes;           // the label information from the package mgr.
    Int32 mIcon;               // resource identifier of activity's icon.
    Int32 mTheme;              // resource identifier of activity's theme.
    TaskRecord* mTask;        // the task this is in.
    Millisecond64 mLaunchTime;        // when we starting launching this activity
    Millisecond64 mStartTime;         // last time this activity was started
    Millisecond64 mCpuTimeAtResume;   // the cpu time of host process at the time of resuming activity
    AutoPtr<IConfiguration> mConfiguration; // configuration activity was last running in
    AutoPtr<CActivityRecord> mResultTo; // who started this entry, so will get our reply
    String mResultWho; // additional identifier for use by resultTo.
    Int32 mRequestCode;  // code given by requester (resultTo)
    List<ActivityResult*>* mResults;      // pending ActivityResult objs we have received
    Set<CPendingIntentRecord*>* mPendingResults; // all pending intents for this act
    List<AutoPtr<IIntent> >* mNewIntents;   // any pending new intents for single-top mode
    Set<ConnectionRecord*>* mConnections; // All ConnectionRecord we hold
    UriPermissionOwner* mUriPermissions; // current special URI access perms.
    ProcessRecord* mApp;
    AutoPtr<IBitmap> mThumbnail;       // icon representation of paused screen
    ArrayOf<Char8>* mDescription; // textual description of paused screen
    ActivityState mState;    // current state we are in
    AutoPtr<IBundle> mIcicle;         // last saved activity state
    Boolean mFrontOfTask;    // is this the root activity of its task?
    Boolean mLaunchFailed;   // set if a launched failed, to abort on 2nd try
    Boolean mHaveState;      // have we gotten the last activity state?
    Boolean mStopped;        // is activity pause finished?
    Boolean mDelayedResume;  // not yet resumed because of stopped app switches?
    Boolean mFinishing;      // activity in pending finish list?
    Boolean mConfigDestroy;  // need to destroy due to config change?
    Int32 mConfigChangeFlags;  // which config values have changed
    Boolean mKeysPaused;     // has key dispatching been paused for it?
    Boolean mInHistory;      // are we in the history stack?
    Int32 mLaunchMode;         // the launch mode activity attribute.
    Boolean mVisible;        // does this activity's window need to be shown?
    Boolean mWaitingVisible; // true if waiting for a new act to become vis
    Boolean mNowVisible;     // is this activity's window visible?
    Boolean mThumbnailNeeded;// has someone requested a thumbnail?
    Boolean mIdle;           // has the activity gone idle?
    Boolean mHasBeenLaunched; // has this activity ever been launched?
    Boolean mFrozenBeforeDestroy;// has been frozen but not yet destroyed.

private:

};

#endif // __CHISTORYRECORD_H__
