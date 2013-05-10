
#ifndef __CSYNCSTORAGEENGINEPENDINGOPERATION_H__
#define __CSYNCSTORAGEENGINEPENDINGOPERATION_H__

#include "_CSyncStorageEnginePendingOperation.h"
#include <elastos/AutoPtr.h>
#include <elastos/AutoFree.h>

CarClass(CSyncStorageEnginePendingOperation)
{
public:
    CARAPI GetAccount(
        /* [out] */ IAccount** account);

    CARAPI GetSyncSource(
        /* [out] */ Int32* syncSource);

    CARAPI GetAuthority(
        /* [out] */ String* authority);

    CARAPI GetExtras(
        /* [out] */ IBundle** extras);

    CARAPI GetExpendited(
        /* [out] */ Boolean* expedited);

    CARAPI GetAuthorityId(
        /* [out] */ Int32* authorityId);

    CARAPI SetAuthorityId(
        /* [in] */ Int32 authorityId);

    CARAPI GetFlatExtras(
        /* [out, callee] */ ArrayOf<Byte>** flatExtras);

    CARAPI SetFlatExtras(
        /* [in] */ ArrayOf<Byte>* flatExtras);

    CARAPI constructor(
        /* [in] */ IAccount* account,
        /* [in] */ Int32 source,
        /* [in] */ const String& authority,
        /* [in] */ IBundle* extras,
        /* [in] */ Boolean expedited);

    CARAPI constructor(
        /* [in] */ ISyncStorageEnginePendingOperation* other);

public:
    AutoPtr<IAccount> mAccount;
    Int32 mSyncSource;
    String mAuthority;
    AutoPtr<IBundle> mExtras; // note: read-only.
    Boolean mExpedited;
    Int32 mAuthorityId;
    AutoFree<ArrayOf<Byte> > mFlatExtras;
    
};

#endif // __CSYNCSTORAGEENGINEPENDINGOPERATION_H__
