#include "content/CSyncManager.h"

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
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSyncManager::GetSyncStorageEngine(
/* [out] */ISyncStorageEngine** engine)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSyncManager::ScheduleSync(
/* [in] */IAccount* requestedAccount,
/* [in] */const String& requestedAuthority,
/* [in] */IBundle* extras,
/* [in] */Int64 delay,
/* [in] */Boolean onlyThoseWithUnkownSyncableState)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSyncManager::ScheduleLocalSync(
/* [in] */IAccount* account,
/* [in] */const String& authority)
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
/* [in] */IAccount* account,
/* [in] */const String& authority)
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
/* [in] */IAccount* account,
/* [in] */const String& authority)
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

/**************************************************************************************
 * implement class CSyncManager::InitializerServiceConnection below
 **************************************************************************************/
CSyncManager::InitializerServiceConnection::InitializerServiceConnection(
/* [in] */ IAccount* account,
/* [in] */ String authority,
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
                Logger::V(CSyncManager::TAG, String("calling initialize: ")/*+ mAccount->ToString()*/ + String(", authority ") + mAuthority);
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
