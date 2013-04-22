
#ifndef __CSYNCMANAGER_H__
#define __CSYNCMANAGER_H__

#include "_CSyncManager.h"
#include <ext/frameworkdef.h>
#include <Logger.h>
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>

#include "os/SystemProperties.h"

using namespace Elastos;
using namespace Elastos::Utility::Logging;

CarClass(CSyncManager)
{

public:
    CSyncManager();
    ~CSyncManager();

public:

    CARAPI GetSyncQueue(
        /* [out] */ ISyncQueue** syncQueue);

    CARAPI GetSyncStorageEngine(
        /* [out] */ ISyncStorageEngine** engine);

    CARAPI ScheduleSync(
        /* [in] */ IAccount* requestedAccount,
        /* [in] */ const String& requestedAuthority,
        /* [in] */ IBundle* extras,
        /* [in] */ Int64 delay,
        /* [in] */ Boolean onlyThoseWithUnkownSyncableState);

    CARAPI ScheduleLocalSync(
        /* [in] */ IAccount* account,
        /* [in] */ const String& authority);

    CARAPI GetSyncAdapterTypes(
        /* [out, callee] */ ArrayOf<ISyncAdapterType *>** syncAdapterTypes);

    CARAPI CancelActiveSync(
        /* [in] */ IAccount* account,
        /* [in] */ const String& authority);

    CARAPI ScheduleSyncOperation(
        /* [in] */ ISyncOperation* syncOperation);

    CARAPI ClearScheduledSyncOperations(
        /* [in] */ IAccount* account,
        /* [in] */ const String& authority);

    CARAPI OnAccountsUpdated(
        /* [in] */ IObjectContainer* accounts);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Boolean factoryTest);

public:
    AutoPtr<ISyncQueue> mSyncQueue;

private:

    /** Delay a sync due to local changes this long. In milliseconds */
    static Int64 LOCAL_SYNC_DELAY;

    /**
     * If a sync takes longer than this and the sync queue is not empty then we will
     * cancel it and add it back to the end of the sync queue. In milliseconds.
     */
    static Int64 MAX_TIME_PER_SYNC;

    static const CString TAG;

    static const Int64 SYNC_NOTIFICATION_DELAY = 30 * 1000; // 30 seconds

    /**
     * When retrying a sync for the first time use this delay. After that
     * the retry time will double until it reached MAX_SYNC_RETRY_TIME.
     * In milliseconds.
     */
    static const Int64 INITIAL_SYNC_RETRY_TIME_IN_MS = 30 * 1000; // 30 seconds

    /**
     * Default the max sync retry time to this value.
     */
    static const Int64 DEFAULT_MAX_SYNC_RETRY_TIME_IN_SECONDS = 60 * 60; // one hour

    /**
     * How long to wait before retrying a sync that failed due to one already being in progress.
     */
    static const Int64 DELAY_RETRY_SYNC_IN_PROGRESS_IN_SECONDS = 10;

    /**
     * An error notification is sent if sync of any of the providers has been failing for this long.
     */
    static const Int64 ERROR_NOTIFICATION_DELAY_MS = 1000 * 60 * 10; // 10 minutes

    static const Int32 INITIALIZATION_UNBIND_DELAY_MS = 5000;

    static const String SYNC_WAKE_LOCK;
    static const String HANDLE_SYNC_ALARM_WAKE_LOCK;

    AutoPtr<IContext> mContext;

    volatile ArrayOf<IAccount*>* mAccounts; // = INITIAL_ACCOUNTS_ARRAY;

//    volatile PowerManager.WakeLock mSyncWakeLock;
//    volatile PowerManager.WakeLock mHandleAlarmWakeLock;

    volatile Boolean mDataConnectionIsConnected; // = false;
    volatile Boolean mStorageIsLow; // = false;

    /*final*/ AutoPtr<INotificationManager> mNotificationMgr;
//    AutoPtr<IAlarmManager> mAlarmService; // = null;

    /*final*/ AutoPtr<ISyncStorageEngine> mSyncStorageEngine;

//    AutoPtr<ActiveSyncContext> mActiveSyncContext; // = null;

    // set if the sync error indicator should be reported.
    Boolean mNeedSyncErrorNotification; // = false;
    // set if the sync active indicator should be reported
    Boolean mNeedSyncActiveNotification; // = false;

    AutoPtr<IPendingIntent> mSyncAlarmIntent;

    // Synchronized on "this". Instead of using this directly one should instead call
    // its accessor, getConnManager().
    AutoPtr<IConnectivityManager> mConnManagerDoNotUseDirectly;

//    /*final*/ AutoPtr<ISyncAdaptersCache> mSyncAdapters;

    static /*final*/ ArrayOf<IAccount*>* INITIAL_ACCOUNTS_ARRAY; // = new Account[0];

    static const String ACTION_SYNC_ALARM; // = "android.content.syncmanager.SYNC_ALARM";

//    /*final */ SyncHandler* mSyncHandler;
    /*final */ AutoPtr<IHandler> mMainHandler;

    volatile Boolean mBootCompleted; // = false;

private:
    static void InitStaticMembers();

private:
    static Boolean mIsStaticMemberInitialized;

private:
    class InitializerServiceConnection : public ElRefBase, public IServiceConnection
    {
    public:
        InitializerServiceConnection(
            /* [in] */ IAccount* account,
            /* [in] */ String authority,
            /* [in] */ IContext* context,
            /* [in] */ IHandler* handler);

        ~InitializerServiceConnection();

    public:
        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);


        CARAPI OnServiceConnected(
            /* [in] */ IComponentName* name,
            /* [in] */ IBinder* service);

        CARAPI OnServiceDisconnected(
            /* [in] */ IComponentName* name);

    private:
        const AutoPtr<IAccount> mAccount;
        const String mAuthority;
        const AutoPtr<IHandler> mHandler;
        /*volatile*/ AutoPtr<IContext> mContext;
        volatile Boolean mInitialized;
    };


public:
    class SyncAlarmIntentReceiver : public ElRefBase, public IBroadcastReceiver
    {
    public:
        // Need a parent class reference, so we add a constructor
        SyncAlarmIntentReceiver(CSyncManager* manager);
        ~SyncAlarmIntentReceiver();

    public:
        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

    private:
        AutoPtr<CSyncManager> mSyncmanager;
    };

};

#endif // __CSYNCMANAGER_H__
