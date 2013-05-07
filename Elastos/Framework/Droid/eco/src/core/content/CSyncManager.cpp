#include "content/CSyncManager.h"
#include "os/CBundle.h"

Int64 CSyncManager::LOCAL_SYNC_DELAY;
Int64 CSyncManager::MAX_TIME_PER_SYNC;
ArrayOf<IAccount*>* CSyncManager::INITIAL_ACCOUNTS_ARRAY;
Boolean CSyncManager::mIsStaticMemberInitialized = FALSE;

const CString CSyncManager::TAG("SyncManager");
const String CSyncManager::SYNC_WAKE_LOCK("*sync*");
const String CSyncManager::HANDLE_SYNC_ALARM_WAKE_LOCK("SyncManagerHandleSyncAlarm");
const String CSyncManager::ACTION_SYNC_ALARM("android.content.syncmanager.SYNC_ALARM");

CSyncManager::CSyncManager()
{
    InitStaticMembers();
    mAccounts = INITIAL_ACCOUNTS_ARRAY;
    mDataConnectionIsConnected = FALSE;
    mStorageIsLow = FALSE;
    mNotificationMgr = NULL;
//    mAlarmService = NULL;
    mSyncStorageEngine = NULL;
    mActiveSyncContext = NULL;
    mNeedSyncErrorNotification = FALSE;
    mNeedSyncActiveNotification = FALSE;
    mSyncAlarmIntent = NULL;
    mConnManagerDoNotUseDirectly = NULL;
//    mSyncAdapters = NULL;
//    mSyncHandler = NULL;
    mMainHandler = NULL;
    mBootCompleted = FALSE;
}

CSyncManager::~CSyncManager()
{
//    if(mSyncHandler != NULL){
//        delete mSyncHandler;
//        mSyncHandler = NULL;
//    }
}

ECode CSyncManager::GetSyncQueue(
/* [out] */ISyncQueue** syncQueue)
{
    *syncQueue = (ISyncQueue*)mSyncQueue;
    return NOERROR;
}

ECode CSyncManager::GetSyncStorageEngine(
/* [out] */ISyncStorageEngine** engine)
{
    *engine = (ISyncStorageEngine*)mSyncStorageEngine;
    return NOERROR;
}

ECode CSyncManager::ScheduleSync(
/* [in] */IAccount* requestedAccount,
/* [in] */String* requestedAuthority,
/* [in] */IBundle* extras,
/* [in] */Int64 delay,
/* [in] */Boolean onlyThoseWithUnkownSyncableState)
{
    Boolean isLoggable = Logger::IsLoggable(CSyncManager::TAG, Logger::VERBOSE);


    Boolean allowBackgroundData = FALSE;
    GetConnectivityManager()->GetBackgroundDataSetting(&allowBackgroundData);

    Boolean backgroundDataUsageAllowed = !mBootCompleted || allowBackgroundData;

    if (extras == NULL) {
        CBundle::New((IBundle**)&extras);
    }

    Boolean expedited = FALSE;
    extras->GetBooleanEx(String(ContentResolver_SYNC_EXTRAS_EXPEDITED), FALSE, &expedited);

    if (expedited) {
        delay = -1; // this means schedule at the front of the queue
    }

    ArrayOf<IAccount*>* accounts;

    if (requestedAccount != NULL) {
//        accounts = new Account[]{requestedAccount};
        accounts = ArrayOf<IAccount*>::Alloc(1);
        (*accounts)[0] = requestedAccount;
    } else {
        // if the accounts aren't configured yet then we can't support an account-less
        // sync request
        accounts = mAccounts;

        if (accounts->GetLength() == 0) {
            if (isLoggable) {
                Logger::V(CSyncManager::TAG, String("scheduleSync: no accounts configured, dropping"));
            }

            return NOERROR;
        }
    }

    /*final */ Boolean uploadOnly = FALSE;
    extras->GetBooleanEx(String(ContentResolver_SYNC_EXTRAS_UPLOAD), FALSE, &uploadOnly);

    /*final*/ Boolean manualSync= FALSE;
    extras->GetBooleanEx(String(ContentResolver_SYNC_EXTRAS_MANUAL), FALSE, &manualSync);

    if (manualSync) {
        extras->PutBoolean(String(ContentResolver_SYNC_EXTRAS_IGNORE_BACKOFF), TRUE);
        extras->PutBoolean(String(ContentResolver_SYNC_EXTRAS_IGNORE_SETTINGS), TRUE);
    }

    /*final*/ Boolean ignoreSettings;
    extras->GetBooleanEx(String(ContentResolver_SYNC_EXTRAS_IGNORE_SETTINGS), FALSE, &ignoreSettings);

    Int32 source = 0;

    if (uploadOnly) {
//        source = SyncStorageEngine::SOURCE_LOCAL;
    } else if (manualSync) {
//        source = SyncStorageEngine::SOURCE_USER;
    } else if (requestedAuthority == NULL) {
//        source = SyncStorageEngine::SOURCE_POLL;
    } else {
        // this isn't strictly server, since arbitrary callers can (and do) request
        // a non-forced two-way sync on a specific url
//        source = SyncStorageEngine::SOURCE_SERVER;
    }

    // Compile a list of authorities that have sync adapters.
    // For each authority sync each account that matches a sync adapter.


//    /*final*/ HashSet<String> syncableAuthorities; // = new HashSet<String>();

    /*
    for (RegisteredServicesCache.ServiceInfo<SyncAdapterType> syncAdapter :
            mSyncAdapters->getAllServices()) {
        syncableAuthorities.Insert(syncAdapter.type.authority);
    }

    // if the url was specified then replace the list of authorities with just this authority
    // or clear it if this authority isn't syncable
    if (requestedAuthority != NULL) {
        Boolean hasSyncAdapter = syncableAuthorities.contains(requestedAuthority);
        syncableAuthorities.clear();
        if (hasSyncAdapter) syncableAuthorities.add(requestedAuthority);
    }

    final boolean masterSyncAutomatically = mSyncStorageEngine.getMasterSyncAutomatically();

    for (String authority : syncableAuthorities) {
        for (Account account : accounts) {
            int isSyncable = mSyncStorageEngine.getIsSyncable(account, authority);
            if (isSyncable == 0) {
                continue;
            }
            if (onlyThoseWithUnkownSyncableState && isSyncable >= 0) {
                continue;
            }
            final RegisteredServicesCache.ServiceInfo<SyncAdapterType> syncAdapterInfo =
                    mSyncAdapters.getServiceInfo(
                            SyncAdapterType.newKey(authority, account.type));
            if (syncAdapterInfo != null) {
                if (!syncAdapterInfo.type.supportsUploading() && uploadOnly) {
                    continue;
                }

                // always allow if the isSyncable state is unknown
                boolean syncAllowed =
                        (isSyncable < 0)
                        || ignoreSettings
                        || (backgroundDataUsageAllowed && masterSyncAutomatically
                            && mSyncStorageEngine.getSyncAutomatically(account, authority));
                if (!syncAllowed) {
                    if (isLoggable) {
                        Log.d(TAG, "scheduleSync: sync of " + account + ", " + authority
                                + " is not allowed, dropping request");
                    }
                    continue;
                }

                if (isLoggable) {
                    Log.v(TAG, "scheduleSync:"
                            + " delay " + delay
                            + ", source " + source
                            + ", account " + account
                            + ", authority " + authority
                            + ", extras " + extras);
                }
                scheduleSyncOperation(
                        new SyncOperation(account, source, authority, extras, delay));
            }
        }
    }*/

    return E_NOT_IMPLEMENTED;
}

ECode CSyncManager::ScheduleLocalSync(
/* [in] */ IAccount* account,
/* [in] */ String* authority)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSyncManager::GetSyncAdapterTypes(
/* [out, callee] */ArrayOf<ISyncAdapterType *>** syncAdapterTypes)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSyncManager::CancelActiveSync(
/* [in] */ IAccount* account,
/* [in] */ String* authority)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSyncManager::ScheduleSyncOperation(
/* [in] */ISyncOperation* syncOperation)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSyncManager::ClearScheduledSyncOperations(
/* [in] */ IAccount* account,
/* [in] */ String* authority)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSyncManager::OnAccountsUpdated(
/* [in] */IObjectContainer* accounts)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSyncManager::constructor(
/* [in] */IContext* context,
/* [in] */Boolean factoryTest)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

void CSyncManager::InitStaticMembers()
{
    if(mIsStaticMemberInitialized == TRUE){
        return;
    }

    String localSyncDelayString = SystemProperties::Get("sync.local_sync_delay");
    Int64 localSyncDelay = 30 * 1000; // 30 seconds
    if (localSyncDelayString != NULL && !localSyncDelayString.IsEmpty())
    {
//        try
//        {
            Int64 tempSyncDelay = localSyncDelayString.ToInt64();
            if (tempSyncDelay > 0)
            {
                localSyncDelay = tempSyncDelay;
            }
//        }
//        catch (NumberFormatException nfe)
//        {
//            // ignore, use default
//        }
    }

    LOCAL_SYNC_DELAY = localSyncDelay;

    String maxTimePerSyncString = SystemProperties::Get("sync.max_time_per_sync");
    long maxTimePerSync = 5 * 60 * 1000; // 5 minutes

    if (maxTimePerSyncString != NULL && !maxTimePerSyncString.IsEmpty())
    {
        Int64 tempTimePerSync = maxTimePerSyncString.ToInt64();

        if (tempTimePerSync > 0)
        {
            maxTimePerSync = tempTimePerSync;
        }
    }

    MAX_TIME_PER_SYNC = maxTimePerSync;

    INITIAL_ACCOUNTS_ARRAY = ArrayOf<IAccount*>::Alloc(0);

    mIsStaticMemberInitialized = TRUE;
}

AutoPtr<IConnectivityManager> CSyncManager::GetConnectivityManager()
{
    // TODO: Add your code here

    return NULL;
}

void CSyncManager::SendCheckAlarmsMessage()
{
    // TODO: Add your code here
}

/**************************************************************************************
 * implement class CSyncManager::InitializerServiceConnection below
 **************************************************************************************/
CSyncManager::InitializerServiceConnection::InitializerServiceConnection(
/* [in] */ IAccount* account,
/* [in] */ String* authority,
/* [in] */ IContext* context,
/* [in] */ IHandler* handler):
    mAccount(account),
    mAuthority(authority),
    mHandler(handler),
    mContext(context),
    mInitialized(FALSE)
{
}

CSyncManager::InitializerServiceConnection::~InitializerServiceConnection()
{
}

PInterface CSyncManager::InitializerServiceConnection::Probe(
/* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*) (IServiceConnection*) this;
    }
    else if (riid == EIID_IServiceConnection) {
        return (IServiceConnection*) this;
    }

    return NULL;
}

UInt32 CSyncManager::InitializerServiceConnection::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CSyncManager::InitializerServiceConnection::Release()
{
    return ElRefBase::Release();
}

ECode CSyncManager::InitializerServiceConnection::GetInterfaceID(
/* [in] */ IInterface *pObject,
/* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IServiceConnection*)this) {
        *pIID = EIID_IServiceConnection;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode CSyncManager::InitializerServiceConnection::OnServiceConnected(
/* [in] */ IComponentName* name,
/* [in] */ IBinder* service)
{
//    try {
        if (!mInitialized) {
            mInitialized = TRUE;
            if (Logger::IsLoggable(CSyncManager::TAG, Logger::VERBOSE)) {
                Logger::V(CSyncManager::TAG, String("calling initialize: ")/*+ mAccount->ToString()*/ + String(", authority ") + (mAuthority != NULL ? *mAuthority : String("NULL")));
            }
//            ISyncAdapter.Stub.asInterface(service).initialize(mAccount, mAuthority);
        }
//    }
//    catch (RemoteException e) {
//        // doesn't matter, we will retry again later
//        Log.d(TAG, "error while initializing: " + mAccount + ", authority " + mAuthority, e);
//    }
//    finally {
//        // give the sync adapter time to initialize before unbinding from it
//        // TODO: change this API to not rely on this timing, http://b/2500805
//        mHandler.postDelayed(new Runnable() {
//                public void run() {
//                        if (mContext != null) {
//                            mContext.unbindService(InitializerServiceConnection.this);
//                            mContext = null;
//                        }
//                    }
//                }, INITIALIZATION_UNBIND_DELAY_MS);
//    }

    return E_NOT_IMPLEMENTED;
}

ECode CSyncManager::InitializerServiceConnection::OnServiceDisconnected(
/* [in] */ IComponentName* name)
{
    if (mContext != NULL) {
        mContext->UnbindService((IServiceConnection*) this);
        mContext = NULL;
    }

    return NOERROR;
}

/**************************************************************************************
 * implement class CSyncManager::SyncAlarmIntentReceiver below
 **************************************************************************************/
CSyncManager::SyncAlarmIntentReceiver::SyncAlarmIntentReceiver(
/* [in] */ CSyncManager* manager):
    mSyncmanager(manager)
{
}

CSyncManager::SyncAlarmIntentReceiver::~SyncAlarmIntentReceiver()
{
}

PInterface CSyncManager::SyncAlarmIntentReceiver::Probe(
/* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*) (IBroadcastReceiver*) this;
    }
    else if (riid == EIID_IBroadcastReceiver) {
        return (IBroadcastReceiver*) this;
    }

    return NULL;
}

UInt32 CSyncManager::SyncAlarmIntentReceiver::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CSyncManager::SyncAlarmIntentReceiver::Release()
{
    return ElRefBase::Release();
}

ECode CSyncManager::SyncAlarmIntentReceiver::GetInterfaceID(
/* [in] */ IInterface *pObject,
/* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IBroadcastReceiver*)this) {
        *pIID = EIID_IBroadcastReceiver;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode CSyncManager::SyncAlarmIntentReceiver::OnReceive(
/* [in] */ IContext* context,
/* [in] */ IIntent* intent)
{
//    mHandleAlarmWakeLock->Acquire();
//    mSyncmanager->SendSyncAlarmMessage();

    return E_NOT_IMPLEMENTED;
}

/**************************************************************************************
 * implement class CSyncManager::SyncHandlerMessagePayload below
 **************************************************************************************/
CSyncManager::SyncHandlerMessagePayload::SyncHandlerMessagePayload(
    /* [in] */ ActiveSyncContext* syncContext,
    /* [in] */ ISyncResult* syncResult):
    activeSyncContext(syncContext),
    syncResult(syncResult)
{
}

CSyncManager::SyncHandlerMessagePayload::~SyncHandlerMessagePayload()
{
}

UInt32 CSyncManager::SyncHandlerMessagePayload::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CSyncManager::SyncHandlerMessagePayload::Release()
{
    return ElRefBase::Release();
}

/**************************************************************************************
 * implement class CSyncManager::SyncTimeTracker below
 **************************************************************************************/
CSyncManager::SyncTimeTracker::SyncTimeTracker(
/* [in] */CSyncManager* manager) :
mSyncmanager(manager)
{
    mLastWasSyncing = FALSE;
    mWhenSyncStarted = 0;
    mTimeSpentSyncing = 0;
}

CSyncManager::SyncTimeTracker::~SyncTimeTracker()
{
}

void CSyncManager::SyncTimeTracker::update()
{
    Boolean isSyncInProgress = (mSyncmanager->mActiveSyncContext != NULL);

    if (isSyncInProgress == mLastWasSyncing) {
        return;
    }

    Int64 now = SystemClock::GetElapsedRealtime();

    if (isSyncInProgress) {
        mWhenSyncStarted = now;
    } else {
        mTimeSpentSyncing += now - mWhenSyncStarted;
    }

    mLastWasSyncing = isSyncInProgress;
}

Int64 CSyncManager::SyncTimeTracker::timeSpentSyncing()
{
    if (!mLastWasSyncing) {
        return mTimeSpentSyncing;
    }

    Int64 now = SystemClock::GetElapsedRealtime();

    return mTimeSpentSyncing + (now - mWhenSyncStarted);

}

UInt32 CSyncManager::SyncTimeTracker::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CSyncManager::SyncTimeTracker::Release()
{
    return ElRefBase::Release();
}

/**************************************************************************************
 * implement class CSyncManager::ServiceConnectionData below
 **************************************************************************************/
CSyncManager::ServiceConnectionData::ServiceConnectionData(
/* [in] */ ActiveSyncContext* activeSyncContext,
/* [in] */ ISyncAdapterStub* syncAdapter):
activeSyncContext(activeSyncContext),
syncAdapter(syncAdapter)
{

}

CSyncManager::ServiceConnectionData::~ServiceConnectionData()
{

}

UInt32 CSyncManager::ServiceConnectionData::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CSyncManager::ServiceConnectionData::Release()
{
    return ElRefBase::Release();
}

/**************************************************************************************
 * implement class CSyncManager::StorageIntentReceiver below
 **************************************************************************************/
CSyncManager::StorageIntentReceiver::StorageIntentReceiver(
    /* [in] */ CSyncManager* manager) :
    mSyncmanager(manager)
{

}

CSyncManager::StorageIntentReceiver::~StorageIntentReceiver()
{

}

ECode CSyncManager::StorageIntentReceiver::OnReceiver(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    String action;
    intent->GetAction(&action);

    Boolean isLoggable = Logger::IsLoggable(CSyncManager::TAG, Logger::VERBOSE);

    if (String(Intent_ACTION_DEVICE_STORAGE_LOW).Equals(action)) {
        if (isLoggable) {
            Logger::V(CSyncManager::TAG, "Internal storage is low.");
        }

        mSyncmanager->mStorageIsLow = TRUE;
        mSyncmanager->CancelActiveSync(NULL /* any account */, NULL /* any authority */);
    } else if (String(Intent_ACTION_DEVICE_STORAGE_OK).Equals(action)) {
        if (isLoggable) {
            Logger::V(CSyncManager::TAG, "Internal storage is ok.");
        }

        mSyncmanager->mStorageIsLow = FALSE;
        mSyncmanager->SendCheckAlarmsMessage();
    }

    return NOERROR;
}

/**************************************************************************************
 * implement class CSyncManager::BootCompletedReceiver below
 **************************************************************************************/
CSyncManager::BootCompletedReceiver::BootCompletedReceiver(
    /* [in] */ CSyncManager* manager) :
    mSyncmanager(manager)
{

}

CSyncManager::BootCompletedReceiver::~BootCompletedReceiver()
{

}

ECode CSyncManager::BootCompletedReceiver::OnReceiver(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    return E_NOT_IMPLEMENTED;
}

/**************************************************************************************
 * implement class CSyncManager::BackgroundDataSettingChangedReceiver below
 **************************************************************************************/
CSyncManager::BackgroundDataSettingChangedReceiver::BackgroundDataSettingChangedReceiver(
    /* [in] */ CSyncManager* manager) :
    mSyncmanager(manager)
{

}

CSyncManager::BackgroundDataSettingChangedReceiver::~BackgroundDataSettingChangedReceiver()
{

}

ECode CSyncManager::BackgroundDataSettingChangedReceiver::OnReceiver(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    return E_NOT_IMPLEMENTED;
}

/**************************************************************************************
 * implement class CSyncManager::ConnectivityIntentReceiver below
 **************************************************************************************/
CSyncManager::ConnectivityIntentReceiver::ConnectivityIntentReceiver(
    /* [in] */ CSyncManager* manager) :
    mSyncmanager(manager)
{

}

CSyncManager::ConnectivityIntentReceiver::~ConnectivityIntentReceiver()
{

}

ECode CSyncManager::ConnectivityIntentReceiver::OnReceiver(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    return E_NOT_IMPLEMENTED;
}

/**************************************************************************************
 * implement class CSyncManager::ShutdownIntentReceiver below
 **************************************************************************************/
CSyncManager::ShutdownIntentReceiver::ShutdownIntentReceiver(
    /* [in] */ CSyncManager* manager) :
    mSyncmanager(manager)
{

}

CSyncManager::ShutdownIntentReceiver::~ShutdownIntentReceiver()
{

}

ECode CSyncManager::ShutdownIntentReceiver::OnReceiver(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    return E_NOT_IMPLEMENTED;
}
