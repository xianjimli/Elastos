
#include "ext/frameworkdef.h"
#include "accounts/CAccountAuthenticatorTransport.h"

ECode CAccountAuthenticatorTransport::AddAccount(
    /* [in] */ IAccountAuthenticatorResponse* response,
    /* [in] */ const String& accountType,
    /* [in] */ const String& authTokenType,
    /* [in] */ const ArrayOf<String>& requiredFeatures,
    /* [in] */ IBundle* options)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountAuthenticatorTransport::ConfirmCredentials(
    /* [in] */ IAccountAuthenticatorResponse* response,
    /* [in] */ IAccount* account,
    /* [in] */ IBundle* options)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountAuthenticatorTransport::GetAuthToken(
    /* [in] */ IAccountAuthenticatorResponse* response,
    /* [in] */ IAccount* account,
    /* [in] */ const String& authTokenType,
    /* [in] */ IBundle* options)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountAuthenticatorTransport::GetAuthTokenLabel(
    /* [in] */ IAccountAuthenticatorResponse* response,
    /* [in] */ const String& authTokenType)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountAuthenticatorTransport::UpdateCredentials(
    /* [in] */ IAccountAuthenticatorResponse* response,
    /* [in] */ IAccount* account,
    /* [in] */ const String& authTokenType,
    /* [in] */ IBundle* options)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountAuthenticatorTransport::EditProperties(
    /* [in] */ IAccountAuthenticatorResponse* response,
    /* [in] */ const String& accountType)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountAuthenticatorTransport::HasFeatures(
    /* [in] */ IAccountAuthenticatorResponse* response,
    /* [in] */ IAccount* account,
    /* [in] */ const ArrayOf<String>& features)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountAuthenticatorTransport::GetAccountRemovalAllowed(
    /* [in] */ IAccountAuthenticatorResponse* response,
    /* [in] */ IAccount* account)
{
    return E_NOT_IMPLEMENTED;
}
