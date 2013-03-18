
#ifndef __CCOUNTDOWNLATCH_H__
#define __CCOUNTDOWNLATCH_H__

#include "_CCountDownLatch.h"
#include "CountDownLatch.h"

CarClass(CCountDownLatch), public CountDownLatch
{
public:
    CARAPI AWait();

    CARAPI AWaitEx(
        /* [in] */ Int64 timeout,
        /* [in] */ ITimeUnit * pUnit,
        /* [out] */ Boolean * pIsWait);

    CARAPI CountDown();

    CARAPI GetCount(
        /* [out] */ Int64 * pCount);

    CARAPI ToString();

    CARAPI constructor(
        /* [in] */ Int32 count);

private:
//    CCountDownLatch(
//        /* [in] */ Int32 arg);
    // TODO: Add your private member variables here.
};

#endif // __CCOUNTDOWNLATCH_H__
