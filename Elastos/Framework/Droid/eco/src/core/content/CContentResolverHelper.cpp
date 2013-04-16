
#include "ext/frameworkext.h"
#include "content/ContentResolver.h"
#include "CContentResolverHelper.h"
ECode CContentResolverHelper::ModeToMode(
    /* [in] */ IUri * uri,
    /* [in] */ const String& mode,
    /* [out] */ Int32 * result)
{
    return ContentResolver::ModeToMode(uri, mode, result);
}

ECode CContentResolverHelper::RequestSync(
    /* [in] */ IAccount * account,
    /* [in] */ const String& authority,
    /* [in] */ IBundle * extras)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CContentResolverHelper::ValidateSyncExtrasBundle(
    /* [in] */ IBundle * extras)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CContentResolverHelper::CancelSync(
    /* [in] */ IAccount * account,
    /* [in] */ const String& authority)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CContentResolverHelper::GetSyncAdapterTypes(
    /* [out, callee] */ ArrayOf<ISyncAdapterType *> ** type)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CContentResolverHelper::GetSyncAutomatically(
    /* [in] */ IAccount * account,
    /* [in] */ const String& authority,
    /* [out] */ Boolean * result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CContentResolverHelper::SetSyncAutomatically(
    /* [in] */ IAccount * account,
    /* [in] */ const String& authority,
    /* [in] */ Boolean sync)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CContentResolverHelper::AddPeriodicSync(
    /* [in] */ IAccount * account,
    /* [in] */ const String& authority,
    /* [in] */ IBundle * extras,
    /* [in] */ Int64 pollFrequency)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CContentResolverHelper::RemovePeriodicSync(
    /* [in] */ IAccount * account,
    /* [in] */ const String& authority,
    /* [in] */ IBundle * extras)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CContentResolverHelper::GetPeriodicSyncs(
    /* [in] */ IAccount * account,
    /* [in] */ const String& authority,
    /* [out] */ IObjectContainer ** periodicSyncs)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CContentResolverHelper::GetIsSyncable(
    /* [in] */ IAccount * account,
    /* [in] */ const String& authority,
    /* [out] */ Int32 * isSyncable)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CContentResolverHelper::SetIsSyncable(
    /* [in] */ IAccount * account,
    /* [in] */ const String& authority,
    /* [in] */ Int32 syncable)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CContentResolverHelper::GetMasterSyncAutomatically(
    /* [out] */ Boolean * result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CContentResolverHelper::SetMasterSyncAutomatically(
    /* [in] */ Boolean sync)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CContentResolverHelper::IsSyncActive(
    /* [in] */ IAccount * account,
    /* [in] */ const String& authority,
    /* [out] */ Boolean * isSyncActive)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CContentResolverHelper::GetCurrentSync(
    /* [out] */ ISyncInfo ** syncInfo)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CContentResolverHelper::GetSyncStatus(
    /* [in] */ IAccount * account,
    /* [in] */ const String& authority,
    /* [out] */ ISyncStatusInfo ** syncStatusInfo)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CContentResolverHelper::IsSyncPending(
    /* [in] */ IAccount * account,
    /* [in] */ const String& authority,
    /* [out] */ Boolean * isSyncPending)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CContentResolverHelper::AddStatusChangeListener(
    /* [in] */ Int32 mask,
    /* [in] */ ISyncStatusObserver * observerCallback,
    /* [out] */ IInterface ** item)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CContentResolverHelper::RemoveStatusChangeListener(
    /* [in] */ IInterface * item)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CContentResolverHelper::GetContentService(
    /* [out] */ IContentService ** contentService)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

