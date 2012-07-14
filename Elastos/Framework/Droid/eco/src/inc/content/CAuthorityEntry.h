
#ifndef __CAUTHORITYENTRY_H__
#define __CAUTHORITYENTRY_H__

#include "_CAuthorityEntry.h"

CarClass(CAuthorityEntry)
{
public:
    CARAPI GetHost(
        /* [out] */ String* host);

    CARAPI GetPort(
        /* [out] */ Int32* port);

    CARAPI Match(
        /* [in] */ IUri* data,
        /* [out] */ Int32* result);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);

    CARAPI constructor(
        /* [in] */ const String& host,
        /* [in] */ const String& port);

private:
    String mOrigHost;
    String mHost;
    Boolean mWild;
    Int32 mPort;
};

#endif //__CAUTHORITYENTRY_H__
