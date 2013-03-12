#ifndef __COUNTDOWNLATCH_H__
#define __COUNTDOWNLATCH_H__


class CountDownLatch
{
public:
    CountDownLatch(
        /* [in] */ Int32 count);

    void AWait();

    Boolean AWait(
        /* [in] */ Int64 timeout,
        /* [in] */ ITimeUnit* pUnit);

    void CountDown();

    Int64 GetCount();

    String ToString();
};

#endif // __COUNTDOWNLATCH_H__



