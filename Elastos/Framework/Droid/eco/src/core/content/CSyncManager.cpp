#include "content/CSyncManager.h"

Int64 CSyncManager::LOCAL_SYNC_DELAY;
Int64 CSyncManager::MAX_TIME_PER_SYNC;
ArrayOf<IAccount*>* CSyncManager::INITIAL_ACCOUNTS_ARRAY;
Boolean CSyncManager::mIsStaticMemberInitialized = FALSE;

const String CSyncManager::TAG("SyncManager");
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
//    mActiveSyncContext = NULL;
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
//        try
//        {
            Int64 tempTimePerSync = maxTimePerSyncString.ToInt64();
            if (tempTimePerSync > 0)
            {
                maxTimePerSync = tempTimePerSync;
            }
//        }
//        catch (NumberFormatException nfe)
//        {
//            // ignore, use default
//        }
    }

    MAX_TIME_PER_SYNC = maxTimePerSync;

    INITIAL_ACCOUNTS_ARRAY = ArrayOf<IAccount*>::Alloc(0);

    mIsStaticMemberInitialized = TRUE;
}

