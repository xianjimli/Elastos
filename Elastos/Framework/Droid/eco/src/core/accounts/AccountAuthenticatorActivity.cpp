
#include "accounts/AccountAuthenticatorActivity.h"

ECode AccountAuthenticatorActivity::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    return E_NOT_IMPLEMENTED;
}

void AccountAuthenticatorActivity::SetAccountAuthenticatorResult(
    /* [in] */ IBundle* result)
{}

/**
 * Sends the result or a {@link AccountManager#ERROR_CODE_CANCELED} error if a
 * result isn't present.
 */
ECode AccountAuthenticatorActivity::Finish()
{
    return E_NOT_IMPLEMENTED;
}
