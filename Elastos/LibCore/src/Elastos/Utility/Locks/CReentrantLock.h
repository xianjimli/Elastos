
#ifndef __CREENTRANTLOCK_H__
#define __CREENTRANTLOCK_H__

#include "_CReentrantLock.h"

CarClass(CReentrantLock)
{
public:
    CARAPI Lock();

    CARAPI LockInterruptibly();

    CARAPI TryLock(
        /* [out] */ Boolean * pValue);

    CARAPI TryLockEx(
        /* [in] */ Int64 time,
        /* [in] */ ITimeUnit * pUnit);

    CARAPI Unlock();

    CARAPI NewCondition(
        /* [out] */ ICondition ** ppCondition);

    CARAPI GetHoldCount(
        /* [out] */ Int32 * pValue);

    CARAPI IsHeldByCurrentThread(
        /* [out] */ Boolean * pValue);

    CARAPI IsLocked(
        /* [out] */ Boolean * pIsLocked);

    CARAPI IsFair(
        /* [out] */ Boolean * pIsFair);

    CARAPI GetOwner(
        /* [out] */ IThread ** ppThread);

    CARAPI HasQueuedThreads(
        /* [out] */ Boolean * pValue);

    CARAPI HasQueuedThread(
        /* [in] */ IThread * pThread,
        /* [out] */ Boolean * pValue);

    CARAPI GetQueueLength(
        /* [out] */ Int32 * pValue);

    CARAPI HasWaiters(
        /* [in] */ ICondition * pCondition,
        /* [out] */ Boolean * pValue);

    CARAPI GetWaitQueueLength(
        /* [in] */ ICondition * pCondition,
        /* [out] */ Int32 * pValue);

    CARAPI ToString();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Boolean fair);

private:
    // TODO: Add your private member variables here.
};

#endif // __CREENTRANTLOCK_H__
