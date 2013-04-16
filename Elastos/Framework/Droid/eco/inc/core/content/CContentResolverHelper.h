
#ifndef __CCONTENTRESOLVERHELPER_H__
#define __CCONTENTRESOLVERHELPER_H__

#include "_CContentResolverHelper.h"

CarClass(CContentResolverHelper)
{
public:
    CARAPI ModeToMode(
        /* [in] */ IUri * uri,
        /* [in] */ const String& mode,
        /* [out] */ Int32 * result);

    CARAPI RequestSync(
        /* [in] */ IAccount * account,
        /* [in] */ const String& authority,
        /* [in] */ IBundle * extras);

    CARAPI ValidateSyncExtrasBundle(
        /* [in] */ IBundle * extras);

    CARAPI CancelSync(
        /* [in] */ IAccount * account,
        /* [in] */ const String& authority);

    CARAPI GetSyncAdapterTypes(
        /* [out, callee] */ ArrayOf<ISyncAdapterType *> ** type);

    CARAPI GetSyncAutomatically(
        /* [in] */ IAccount * account,
        /* [in] */ const String& authority,
        /* [out] */ Boolean * result);

    CARAPI SetSyncAutomatically(
        /* [in] */ IAccount * account,
        /* [in] */ const String& authority,
        /* [in] */ Boolean sync);

    CARAPI AddPeriodicSync(
        /* [in] */ IAccount * account,
        /* [in] */ const String& authority,
        /* [in] */ IBundle * extras,
        /* [in] */ Int64 pollFrequency);

    CARAPI RemovePeriodicSync(
        /* [in] */ IAccount * account,
        /* [in] */ const String& authority,
        /* [in] */ IBundle * extras);

    CARAPI GetPeriodicSyncs(
        /* [in] */ IAccount * account,
        /* [in] */ const String& authority,
        /* [out] */ IObjectContainer ** periodicSyncs);

    CARAPI GetIsSyncable(
        /* [in] */ IAccount * account,
        /* [in] */ const String& authority,
        /* [out] */ Int32 * isSyncable);

    CARAPI SetIsSyncable(
        /* [in] */ IAccount * account,
        /* [in] */ const String& authority,
        /* [in] */ Int32 syncable);

    CARAPI GetMasterSyncAutomatically(
        /* [out] */ Boolean * result);

    CARAPI SetMasterSyncAutomatically(
        /* [in] */ Boolean sync);

    CARAPI IsSyncActive(
        /* [in] */ IAccount * account,
        /* [in] */ const String& authority,
        /* [out] */ Boolean * isSyncActive);

    CARAPI GetCurrentSync(
        /* [out] */ ISyncInfo ** syncInfo);

    CARAPI GetSyncStatus(
        /* [in] */ IAccount * account,
        /* [in] */ const String& authority,
        /* [out] */ ISyncStatusInfo ** syncStatusInfo);

    CARAPI IsSyncPending(
        /* [in] */ IAccount * account,
        /* [in] */ const String& authority,
        /* [out] */ Boolean * isSyncPending);

    CARAPI AddStatusChangeListener(
        /* [in] */ Int32 mask,
        /* [in] */ ISyncStatusObserver * observerCallback,
        /* [out] */ IInterface ** item);

    CARAPI RemoveStatusChangeListener(
        /* [in] */ IInterface * item);

    CARAPI GetContentService(
        /* [out] */ IContentService ** contentService);

private:
    // TODO: Add your private member variables here.
};

#endif // __CCONTENTRESOLVERHELPER_H__
