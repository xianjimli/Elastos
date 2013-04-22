
#include "ext/frameworkdef.h"
#include "accounts/CAccountManagerResponse.h"

ECode CAccountManagerResponse::OnResult(
    /* [in] */ IBundle* bundle)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManagerResponse::OnError(
    /* [in] */ Int32 errorCode,
    /* [in] */ const String& errorMessage)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManagerResponse::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManagerResponse::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManagerResponse::constructor(
    /* [in] */ IAccountManagerResponse* response)
{
    mResponse = response;
    return NOERROR;
}

ECode CAccountManagerResponse::constructor(
    /* [in] */ IParcel* source)
{
    return E_NOT_IMPLEMENTED;
}
