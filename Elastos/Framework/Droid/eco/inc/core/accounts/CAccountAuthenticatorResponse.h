
#ifndef __CACCOUNTAUTHENTICATORRESPONSE_H__
#define __CACCOUNTAUTHENTICATORRESPONSE_H__

#include "_CAccountAuthenticatorResponse.h"
#include <elastos/AutoPtr.h>

CarClass(CAccountAuthenticatorResponse)
{
public:
    CARAPI OnResult(
        /* [in] */ IBundle* value);

    CARAPI OnRequestContinued();

    CARAPI OnError(
        /* [in] */ Int32 errorCode,
        /* [in] */ const String& errorMessage);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor(
        /* [in] */ IAccountAuthenticatorResponse* response);

    CARAPI constructor(
        /* [in] */ IParcel* parcel);

private:
    static const CString TAG;

    AutoPtr<IAccountAuthenticatorResponse> mAccountAuthenticatorResponse;
};

#endif //__CACCOUNTAUTHENTICATORRESPONSE_H__
