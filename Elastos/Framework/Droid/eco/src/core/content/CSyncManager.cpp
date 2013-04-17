
#include "content/CSyncManager.h"

ECode CSyncManager::GetSyncQueue(
    /* [out] */ ISyncQueue** syncQueue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSyncManager::GetSyncStorageEngine(
    /* [out] */ ISyncStorageEngine** engine)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSyncManager::ScheduleSync(
    /* [in] */ IAccount* requestedAccount,
    /* [in] */ const String& requestedAuthority,
    /* [in] */ IBundle* extras,
    /* [in] */ Int64 delay,
    /* [in] */ Boolean onlyThoseWithUnkownSyncableState)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSyncManager::ScheduleLocalSync(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSyncManager::GetSyncAdapterTypes(
    /* [out, callee] */ ArrayOf<ISyncAdapterType *>** syncAdapterTypes)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSyncManager::CancelActiveSync(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSyncManager::ScheduleSyncOperation(
    /* [in] */ ISyncOperation* syncOperation)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSyncManager::ClearScheduledSyncOperations(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSyncManager::OnAccountsUpdated(
    /* [in] */ IObjectContainer* accounts)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSyncManager::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Boolean factoryTest)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

