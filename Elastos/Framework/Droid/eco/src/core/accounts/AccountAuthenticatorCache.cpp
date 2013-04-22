
#include "accounts/AccountAuthenticatorCache.h"

const CString AccountAuthenticatorCache::TAG = "Account";
AccountAuthenticatorCache::MySerializer* AccountAuthenticatorCache::sSerializer
        = new AccountAuthenticatorCache::MySerializer();

AccountAuthenticatorCache::AccountAuthenticatorCache(
    /* [in] */ IContext* context)
{}

ECode AccountAuthenticatorCache::ParseServiceAttributes(
    /* [in] */ IResources* res,
    /* [in] */ const String& capsuleName,
    /* [in] */ IAttributeSet* attrs,
    /* [out] */ IInterface** description)
{
    return E_NOT_IMPLEMENTED;
}
