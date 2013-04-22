
#ifndef __CACCOUNTMANAGERAMSRESPONSE_H__
#define __CACCOUNTMANAGERAMSRESPONSE_H__

#include "_CAccountManagerAmsResponse.h"

CarClass(CAccountManagerAmsResponse)
{
public:
    CARAPI OnResult(
        /* [in] */ IBundle* bundle);

    CARAPI OnError(
        /* [in] */ Int32 errorCode,
        /* [in] */ const String& errorMessage);

    CARAPI constructor(
        /* [in] */ Handle32 host);

private:
    Handle32 mHost;
};

#endif //__CACCOUNTMANAGERAMSRESPONSE_H__
