
#include "content/CSyncStorageEnginePendingOperation.h"

ECode CSyncStorageEnginePendingOperation::GetAccount(
    /* [out] */ IAccount** account)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEnginePendingOperation::GetSyncSource(
    /* [out] */ Int32* syncSource)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEnginePendingOperation::GetAuthority(
    /* [out] */ String* authority)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEnginePendingOperation::GetExtras(
    /* [out] */ IBundle** extras)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEnginePendingOperation::GetExpendited(
    /* [out] */ Boolean* expedited)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEnginePendingOperation::GetAuthorityId(
    /* [out] */ Int32* authorityId)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEnginePendingOperation::SetAuthorityId(
    /* [in] */ Int32 authorityId)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEnginePendingOperation::GetFlatExtras(
    /* [out, callee] */ ArrayOf<Byte>** flatExtras)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEnginePendingOperation::SetFlatExtras(
    /* [in] */ ArrayOf<Byte>* flatExtras)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEnginePendingOperation::constructor(
    /* [in] */ IAccount* account,
    /* [in] */ Int32 source,
    /* [in] */ const String& authority,
    /* [in] */ IBundle* extras,
    /* [in] */ Boolean expedited)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEnginePendingOperation::constructor(
    /* [in] */ ISyncStorageEnginePendingOperation* other)
{
    return E_NOT_IMPLEMENTED;
}

