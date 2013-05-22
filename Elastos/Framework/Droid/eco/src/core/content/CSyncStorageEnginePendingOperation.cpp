
#include "content/CSyncStorageEnginePendingOperation.h"
#include "ext/frameworkext.h"
#include "os/CBundle.h"

ECode CSyncStorageEnginePendingOperation::GetAccount(
    /* [out] */ IAccount** account)
{
    VALIDATE_NOT_NULL(account);
    *account = mAccount;
    return NOERROR;
}

ECode CSyncStorageEnginePendingOperation::GetSyncSource(
    /* [out] */ Int32* syncSource)
{
    VALIDATE_NOT_NULL(syncSource);
    *syncSource = mSyncSource;
    return NOERROR;
}

ECode CSyncStorageEnginePendingOperation::GetAuthority(
    /* [out] */ String* authority)
{
    VALIDATE_NOT_NULL(authority);
    *authority = mAuthority;
    return NOERROR;
}

ECode CSyncStorageEnginePendingOperation::GetExtras(
    /* [out] */ IBundle** extras)
{
    VALIDATE_NOT_NULL(extras);
    *extras = mExtras;
    return NOERROR;
}

ECode CSyncStorageEnginePendingOperation::GetExpendited(
    /* [out] */ Boolean* expedited)
{
    VALIDATE_NOT_NULL(expedited);
    *expedited = mExpedited;
    return NOERROR;
}

ECode CSyncStorageEnginePendingOperation::GetAuthorityId(
    /* [out] */ Int32* authorityId)
{
    VALIDATE_NOT_NULL(authorityId);
    *authorityId = mAuthorityId;
    return NOERROR;
}

ECode CSyncStorageEnginePendingOperation::SetAuthorityId(
    /* [in] */ Int32 authorityId)
{
    mAuthorityId = authorityId;
    return NOERROR;
}

ECode CSyncStorageEnginePendingOperation::GetFlatExtras(
    /* [out, callee] */ ArrayOf<Byte>** flatExtras)
{
    VALIDATE_NOT_NULL(flatExtras);
    *flatExtras = mFlatExtras->Clone();
    return NOERROR;
}

ECode CSyncStorageEnginePendingOperation::SetFlatExtras(
    /* [in] */ ArrayOf<Byte>* flatExtras)
{
    VALIDATE_NOT_NULL(flatExtras);
    mFlatExtras->Replace(0, flatExtras->GetPayload(), flatExtras->GetLength());
    return NOERROR;
}

ECode CSyncStorageEnginePendingOperation::constructor(
    /* [in] */ IAccount* account,
    /* [in] */ Int32 source,
    /* [in] */ const String& authority,
    /* [in] */ IBundle* extras,
    /* [in] */ Boolean expedited)
{
    mAccount = account;
    mSyncSource = source;
    mAuthority = authority;

    if (NULL != extras) {
        FAIL_RETURN(CBundle::New(extras, (IBundle**)&mExtras));
    } else{
        mExtras = extras;
    }

    mExpedited = expedited;
    mAuthorityId = -1;
    return NOERROR;
}

ECode CSyncStorageEnginePendingOperation::constructor(
    /* [in] */ ISyncStorageEnginePendingOperation* other)
{
    VALIDATE_NOT_NULL(other);
    FAIL_RETURN(other->GetAccount((IAccount**)&mAccount));
    FAIL_RETURN(other->GetSyncSource(&mSyncSource));
    FAIL_RETURN(other->GetAuthority(&mAuthority));
    FAIL_RETURN(other->GetExtras((IBundle**)&mExtras));
    FAIL_RETURN(other->GetAuthorityId(&mAuthorityId));
    FAIL_RETURN(other->GetExpendited(&mExpedited));
    return NOERROR;
}

