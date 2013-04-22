
#ifndef __CCHOOSERESPONSE_H__
#define __CCHOOSERESPONSE_H__

#include "_CChooseResponse.h"

CarClass(CChooseResponse)
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

#endif //__CCHOOSERESPONSE_H__
