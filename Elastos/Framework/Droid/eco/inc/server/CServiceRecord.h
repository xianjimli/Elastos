
#ifndef __CSERVICERECORD_H__
#define __CSERVICERECORD_H__

#include "_CServiceRecord.h"
#include "ext/frameworkdef.h"
#include <elastos/List.h>
#include <elastos/AutoPtr.h>
#include "os/BatteryStatsImpl.h"
#include "capsule/CServiceInfo.h"
#include "content/CIntent.h"
#include "server/CActivityManagerService.h"
#include "server/IntentBindRecord.h"
#include "server/AppBindRecord.h"
#include "server/ProcessRecord.h"
#include "app/CNotification.h"
#include "server/UriPermissionOwner.h"

using namespace Elastos;

class CActivityManagerService;

CarClass(CServiceRecord)
{
public:
    // Maximum number of delivery attempts before giving up.
    static const Int32 MAX_DELIVERY_COUNT = 3;

    // Maximum number of times it can fail during execution before giving up.
    static const Int32 MAX_DONE_EXECUTING_COUNT = 6;

    class StartItem
    {
    public:
        StartItem(
            /* [in] */ CServiceRecord* sr,
            /* [in] */ Int32 id,
            /* [in] */ IIntent* intent,
            /* [in] */ Int32 targetPermissionUid);

        CARAPI_(UriPermissionOwner*) GetUriPermissionsLocked();

        CARAPI_(void) RemoveUriPermissionsLocked();

    public:
        AutoPtr<CServiceRecord> mSr;
        Int32 mId;
        AutoPtr<IIntent> mIntent;
        Int32 mTargetPermissionUid;
        Millisecond64 mDeliveredTime;
        Int32 mDeliveryCount;
        Int32 mDoneExecutingCount;
//        UriPermissionOwner uriPermissions;

        String stringName;      // caching of toString
    };

public:
    CServiceRecord();

    ~CServiceRecord();

    CARAPI GetDescription(
        /* [out] */ String* description);

public:
    CARAPI_(void) Init(
        /* [in] */ CActivityManagerService* ams,
        /* [in] */ BatteryStatsImpl::Uid::Cap::Serv* servStats,
        /* [in] */ IComponentName* name,
        /* [in] */ CIntent::FilterComparison* intent,
        /* [in] */ IServiceInfo* sInfo,
        /* [in] */ IRunnable* restarter);

    CARAPI_(AppBindRecord*) RetrieveAppBindingLocked(
        /* [in] */ IIntent* intent,
        /* [in] */ ProcessRecord* app);

    CARAPI_(void) ResetRestartCounter();

    CARAPI_(StartItem*) FindDeliveredStart(
        /* [in] */ Int32 id,
        /* [in] */ Boolean remove);

    CARAPI_(void) PostNotification();

    CARAPI_(void) CancelNotification();

    CARAPI_(void) ClearDeliveredStartsLocked();

public:
    AutoPtr<CActivityManagerService> mAms;
    AutoPtr<BatteryStatsImpl::Uid::Cap::Serv> mStats;
    AutoPtr<IComponentName> mName; // service component.
    String mShortName; // name.flattenToShortString().
    CIntent::FilterComparison* mIntent; // original intent used to find service.
    AutoPtr<CServiceInfo> mServiceInfo;  // all information about the service.
    AutoPtr<CApplicationInfo> mAppInfo; // information about service's app.
    String mCapsuleName; // the package implementing intent's component
    String mProcessName; // process where this component wants to run
    String mPermission; // permission needed to access service
    String mBaseDir;   // where activity source (resources etc) located
    String mResDir;   // where public activity source (public resources etc) located
    String mDataDir;   // where activity data should go
    Boolean mExported; // from ServiceInfo.exported
    AutoPtr<IRunnable> mRestarter; // used to schedule retries of starting the service
    Millisecond64 mCreateTime;  // when this service was created
    Map<CIntent::FilterComparison*, IntentBindRecord*> mBindings; // All active bindings to the service.
    Map<AutoPtr<IServiceConnectionInner>, List<ConnectionRecord*>*> mConnections; // IBinder -> ConnectionRecord of all bound clients

    ProcessRecord* mApp;  // where this service is running or null.
    Boolean mIsForeground;   // asked to run as a foreground service?
    Int32 mForegroundId;       // Notification ID of last foreground req.
    AutoPtr<CNotification> mForegroundNoti; // Notification record of foreground state.
    Millisecond64 mLastActivity;      // last time there was some activity on the service.
    Boolean mStartRequested; // someone explicitly called start?
    Boolean mStopIfKilled;   // last onStart() said to stop if service killed?
    Boolean mCallStart;      // last onStart() has asked to alway be called on restart.
    Int32 mLastStartId;        // identifier of most recent start request.
    Int32 mExecuteNesting;     // number of outstanding operations keeping foreground.
    Millisecond64 mExecutingStart;    // start time of last execute request.
    Int32 mCrashCount;         // number of times proc has crashed with service running
    Int32 mTotalRestartCount;  // number of times we have had to restart.
    Int32 mRestartCount;       // number of restarts performed in a row.
    Millisecond64 mRestartDelay;      // delay until next restart attempt.
    Millisecond64 mRestartTime;       // time of last restart.
    Millisecond64 mNextRestartTime;   // time when restartDelay will expire.

    List<StartItem*> mDeliveredStarts;
                            // start() arguments which been delivered.
    List<StartItem*> mPendingStarts;
                            // start() arguments that haven't yet been delivered.

};

#endif //__CSERVICERECORD_H__
