
#include "ext/frameworkdef.h"
#include "accounts/CAccountAuthenticatorResponse.h"

const CString CAccountAuthenticatorResponse::TAG = "AccountAuthenticator";

ECode CAccountAuthenticatorResponse::OnResult(
    /* [in] */ IBundle* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountAuthenticatorResponse::OnRequestContinued()
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountAuthenticatorResponse::OnError(
    /* [in] */ Int32 errorCode,
    /* [in] */ const String& errorMessage)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountAuthenticatorResponse::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountAuthenticatorResponse::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountAuthenticatorResponse::constructor(
    /* [in] */ IAccountAuthenticatorResponse* response)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountAuthenticatorResponse::constructor(
    /* [in] */ IParcel* parcel)
{
    return E_NOT_IMPLEMENTED;
}
