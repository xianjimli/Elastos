
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

    CARAPI GetResult(
        /* [out] */ Int32* result);

    CARAPI SetResult(
        /* [in] */ Int32 result);

    CARAPI IsTimeout(
        /* [out] */ Boolean* timeout);

    CARAPI SetTimeout(
        /* [in] */ Boolean timeout);

    CARAPI GetWho(
        /* [out] */ IComponentName** who);

    CARAPI SetWho(
        /* [in] */ IComponentName* who);

    CARAPI GetThisTime(
        /* [out] */ Int64* time);

    CARAPI SetThisTime(
        /* [in] */ Int64 time);

    CARAPI GetTotalTime(
        /* [out] */ Int64* time);

    CARAPI SetTotalTime(
        /* [in] */ Int64 time);

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
