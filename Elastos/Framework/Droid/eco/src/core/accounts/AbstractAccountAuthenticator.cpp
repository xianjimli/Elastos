
#include "accounts/AbstractAccountAuthenticator.h"
#include "accounts/CAccountAuthenticatorTransport.h"

const CString AbstractAccountAuthenticator::TAG = "AccountAuthenticator";

AbstractAccountAuthenticator::AbstractAccountAuthenticator(
    /* [in] */ IContext* context)
{
    ASSERT_SUCCEEDED(CAccountAuthenticatorTransport::New(
            (IAccountAuthenticator**)&mTransport));
}

ECode AbstractAccountAuthenticator::GetAccountAuthenticator(
    /* [out] */ IAccountAuthenticator** authenticator)
{
    return E_NOT_IMPLEMENTED;
}

ECode AbstractAccountAuthenticator::GetAccountRemovalAllowed(
    /* [in] */ IAccountAuthenticatorResponse* response,
    /* [in] */ IAccount* account,
    /* [out] */ IBundle** bundle)
{
    return E_NOT_IMPLEMENTED;
}

void AbstractAccountAuthenticator::CheckBinderPermission()
{}
