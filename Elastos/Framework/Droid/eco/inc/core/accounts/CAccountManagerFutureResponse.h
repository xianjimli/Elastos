
#ifndef __CACCOUNTMANAGERFUTURERESPONSE_H__
#define __CACCOUNTMANAGERFUTURERESPONSE_H__

#include "_CAccountManagerFutureResponse.h"

CarClass(CAccountManagerFutureResponse)
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

#endif //__CACCOUNTMANAGERFUTURERESPONSE_H__
