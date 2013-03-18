#ifndef __COUNTDOWNLATCH_H__
#define __COUNTDOWNLATCH_H__


#include <elastos.h>
#include "AbstractQueuedSynchronizer.h"

using namespace Elastos;

class CountDownLatch
{
    class Sync : public AbstractQueuedSynchronizer
    {
    private:
        static Int64 m_SerialVersionUID;;

    public:
        Sync(
            /* [in] */ Int32 count);

        ECode GetCount(
            /* [out] */ Int32* pState);

    protected:
        Int32 TryAcquireShared(
            /* [in] */ Int32 acquires);

        Boolean TryReleaseShared(
            /* [in] */ Int32 releases);
    };
public:
    CountDownLatch(
        /* [in] */ Int32 count);

    CountDownLatch();

    ECode AWait();

    ECode AWait(
        /* [in] */ Int64 timeout,
        /* [in] */ ITimeUnit* pUnit,
        /* [out] */ Boolean* pValue);

    ECode CountDown();

    ECode GetCount(
        /* [out] */ Int64* pValue);

    String ToString();

private:
    Sync* m_Sync;
};

#endif // __COUNTDOWNLATCH_H__



