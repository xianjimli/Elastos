
#ifndef __CWAITRESULT_H__
#define __CWAITRESULT_H__

#include "_CWaitResult.h"
#include <elastos/AutoPtr.h>

CarClass(CWaitResult)
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IParcel* source);

    CARAPI DescribeContents(
        /* [out] */ Int32* result);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);

public:
    Int32 mResult;
    Boolean mTimeout;
    AutoPtr<IComponentName> mWho;
    Millisecond64 mThisTime;
    Millisecond64 mTotalTime;
};

#endif //__CWAITRESULT_H__
