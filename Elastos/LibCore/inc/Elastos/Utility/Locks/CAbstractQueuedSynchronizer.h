
#ifndef __CABSTRACTQUEUEDSYNCHRONIZER_H__
#define __CABSTRACTQUEUEDSYNCHRONIZER_H__

#include "_CAbstractQueuedSynchronizer.h"

CarClass(CAbstractQueuedSynchronizer)
{
public:
    CARAPI Acquire(
        /* [in] */ Int32 arg);

    CARAPI AcquireInterruptibly(
        /* [in] */ Int32 arg);

    CARAPI TryAcquireNanos(
        /* [in] */ Int32 arg,
        /* [in] */ Int64 nanosTimeout,
        /* [out] */ Boolean * pIsAcquire);

    CARAPI ReleaseArg(
        /* [in] */ Int32 arg,
        /* [out] */ Boolean * pIsRelease);

    CARAPI AcquireShared(
        /* [in] */ Int32 arg);

    CARAPI AcquireSharedInterruptibly(
        /* [in] */ Int32 arg);

    CARAPI TryAcquireSharedNanos(
        /* [in] */ Int32 arg,
        /* [in] */ Int64 nanosTimeout,
        /* [out] */ Boolean * pIsAcquireShared);

    CARAPI ReleaseShared(
        /* [in] */ Int32 arg,
        /* [out] */ Boolean * pIsReleaseShared);

    CARAPI HasQueuedThreads(
        /* [out] */ Boolean * pHasQueueThreads);

    CARAPI HasContended(
        /* [out] */ Boolean * pHasContended);

    CARAPI GetFirstQueuedThread(
        /* [out] */ IThread ** ppThread);

    CARAPI IsQueued(
        /* [in] */ IThread * pThread,
        /* [out] */ Boolean * pIsQueue);

    CARAPI ApparentlyFirstQueuedIsExclusive(
        /* [out] */ Boolean * pIsApparent);

    CARAPI HasQueuedPredecessors(
        /* [out] */ Boolean * pHasQueuePredecessors);

    CARAPI GetQueueLength(
        /* [out] */ Int32 * pLen);

    CARAPI GetQueuedThreads(
        /* [out] */ IObjectEnumerator ** ppEmu);

    CARAPI GetExclusiveQueuedThreads(
        /* [out] */ IObjectEnumerator ** ppEmu);

    CARAPI GetSharedQueuedThreads(
        /* [out] */ IObjectEnumerator ** ppEmu);

    CARAPI ToString();

    CARAPI Owns(
        /* [in] */ IConditionObject * pCondition,
        /* [out] */ Boolean * pIsOwn);

    CARAPI HasWaiters(
        /* [in] */ IConditionObject * pCondition,
        /* [out] */ Boolean * pHasWaiters);

    CARAPI GetWaitQueueLength(
        /* [in] */ IConditionObject * pCondition,
        /* [out] */ Int32 * pLen);

    CARAPI GetWaitingThreads(
        /* [in] */ IConditionObject * pCondition,
        /* [out] */ IObjectEnumerator ** ppEmu);

    CARAPI constructor();

private:
    // TODO: Add your private member variables here.
};

#endif // __CABSTRACTQUEUEDSYNCHRONIZER_H__
