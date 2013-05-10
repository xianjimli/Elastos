
#include "content/CSyncStorageEngineAuthorityInfo.h"

CSyncStorageEngineAuthorityInfo::CSyncStorageEngineAuthorityInfo()
    : mPeriodicSyncs(NULL)
{}

CSyncStorageEngineAuthorityInfo::~CSyncStorageEngineAuthorityInfo()
{
    if (NULL != mPeriodicSyncs) {
        mPeriodicSyncs->Clear();
        delete mPeriodicSyncs;
    }
}

ECode CSyncStorageEngineAuthorityInfo::GetAccount(
    /* [out] */ IAccount** account)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngineAuthorityInfo::GetAuthority(
    /* [out] */ String* authority)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngineAuthorityInfo::GetIdent(
    /* [out] */ Int32* ident)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngineAuthorityInfo::GetEnabled(
    /* [out] */ Boolean* enabled)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngineAuthorityInfo::SetEnabled(
    /* [in] */ Boolean enabled)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngineAuthorityInfo::GetSyncable(
    /* [out] */ Int32* syncable)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngineAuthorityInfo::SetSyncable(
    /* [in] */ Int32 syncable)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngineAuthorityInfo::GetBackoffTime(
    /* [out] */ Int64* backoffTime)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngineAuthorityInfo::SetBackoffTime(
    /* [in] */ Int64 backoffTime)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngineAuthorityInfo::GetBackoffDelay(
    /* [out] */ Int64* backoffDelay)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngineAuthorityInfo::SetBackoffDelay(
    /* [in] */ Int64 backoffDelay)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngineAuthorityInfo::GetDelayUntil(
    /* [out] */ Int64* delayUntil)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngineAuthorityInfo::SetDelayUntil(
    /* [in] */ Int64 delayUntil)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngineAuthorityInfo::GetPeriodicSyncs(
    /* [out] */ IObjectContainer** periodicSyncs)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngineAuthorityInfo::constructor(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [in] */ Int32 ident)
{
    return E_NOT_IMPLEMENTED;
}

