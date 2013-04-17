
#ifndef __CSYNCMANAGER_H__
#define __CSYNCMANAGER_H__

#include "_CSyncManager.h"
#include <ext/frameworkdef.h>

CarClass(CSyncManager)
{
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

private:
    // TODO: Add your private member variables here.
};

#endif // __CSYNCMANAGER_H__
