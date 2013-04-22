
#ifndef __CACCOUNTMANAGERRESPONSE_H__
#define __CACCOUNTMANAGERRESPONSE_H__

#include "_CAccountManagerResponse.h"
#include <elastos/AutoPtr.h>

CarClass(CAccountManagerResponse)
{
public:
    CARAPI OnResult(
        /* [in] */ IBundle* bundle);

    CARAPI OnError(
        /* [in] */ Int32 errorCode,
        /* [in] */ const String& errorMessage);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor(
        /* [in] */ IAccountManagerResponse* response);

    CARAPI constructor(
        /* [in] */ IParcel* source);

private:
    AutoPtr<IAccountManagerResponse> mResponse;
};

#endif //__CACCOUNTMANAGERRESPONSE_H__
