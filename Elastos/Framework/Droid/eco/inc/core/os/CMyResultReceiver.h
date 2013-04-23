
#ifndef __CMYRESULTRECEIVER_H__
#define __CMYRESULTRECEIVER_H__


#include "_CMyResultReceiver.h"

CarClass(CMyResultReceiver)
{
public:
    CARAPI constructor(
        /* [in] */ Handle32 host);

    CARAPI Send(
        /* [in] */ Int32 resultCode,
        /* [in] */ IBundle* resultData);

private:
    Handle32 mHost;
};

#endif //__CMYRESULTRECEIVER_H__
