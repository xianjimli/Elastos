
#include "accounts/GrantCredentialsPermissionActivity.h"

const CString GrantCredentialsPermissionActivity::EXTRAS_ACCOUNT = "account";
const CString GrantCredentialsPermissionActivity::EXTRAS_AUTH_TOKEN_LABEL = "authTokenLabel";
const CString GrantCredentialsPermissionActivity::EXTRAS_AUTH_TOKEN_TYPE = "authTokenType";
const CString GrantCredentialsPermissionActivity::EXTRAS_RESPONSE = "response";
const CString GrantCredentialsPermissionActivity::EXTRAS_ACCOUNT_TYPE_LABEL = "accountTypeLabel";
const CString GrantCredentialsPermissionActivity::EXTRAS_PACKAGES = "application";
const CString GrantCredentialsPermissionActivity::EXTRAS_REQUESTING_UID = "uid";

GrantCredentialsPermissionActivity::GrantCredentialsPermissionActivity(
    /* [in] */ IContext* context)
    : mUid(0)
{}

ECode GrantCredentialsPermissionActivity::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    return E_NOT_IMPLEMENTED;
}

ECode GrantCredentialsPermissionActivity::OnClick(
    /* [in] */ IView* v)
{
    return E_NOT_IMPLEMENTED;
}

void GrantCredentialsPermissionActivity::SetAccountAuthenticatorResult(
    /* [in] */ IBundle* result)
{}

/**
 * Sends the result or a {@link AccountManager#ERROR_CODE_CANCELED} error if a
 * result isn't present.
 */
ECode GrantCredentialsPermissionActivity::Finish()
{
    return E_NOT_IMPLEMENTED;
}

ECode GrantCredentialsPermissionActivity::NewCapsuleView(
    /* [in] */ const String& capsuleLabel,
    /* [out] */ IView** view)
{
    return E_NOT_IMPLEMENTED;
}
