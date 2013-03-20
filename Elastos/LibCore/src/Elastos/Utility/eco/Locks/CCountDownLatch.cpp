
#include "CCountDownLatch.h"
ECode CCountDownLatch::AWait()
{
    // TODO: Add your code here
    return CountDownLatch::AWait();
}

ECode CCountDownLatch::AWaitEx(
    /* [in] */ Int64 timeout,
    /* [in] */ ITimeUnit * pUnit,
    /* [out] */ Boolean * pIsWait)
{
    // TODO: Add your code here
    return CountDownLatch::AWait(timeout, pUnit, pIsWait);
}

ECode CCountDownLatch::CountDown()
{
    // TODO: Add your code here
    return CountDownLatch::CountDown();
}

ECode CCountDownLatch::GetCount(
    /* [out] */ Int64 * pCount)
{
    // TODO: Add your code here
    return CountDownLatch::GetCount(pCount);
}

ECode CCountDownLatch::ToString()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCountDownLatch::constructor(
    /* [in] */ Int32 count)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

//CCountDownLatch::CCountDownLatch(
//    /* [in] */ Int32 arg)
//{
//}