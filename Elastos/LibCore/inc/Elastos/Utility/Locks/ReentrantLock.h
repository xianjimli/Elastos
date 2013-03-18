#ifndef __REENTRANTLOCK_H__
#define __REENTRANTLOCK_H__

#include <elastos.h>
#include <elastos/Mutex.h>
#include "Elastos.Core.h"
#include "CConditionObject.h"
#include <elastos/AutoPtr.h>
#include "AbstractQueuedSynchronizer.h"
#include "Elastos.Utility.Locks_server.h"

class ReentrantLock {
public:
    ReentrantLock();

    ReentrantLock(
        /* [in] */ Boolean fair);

    class Sync : public AbstractQueuedSynchronizer
    {
    public:
        virtual void Lock() = 0;

        ECode NonfairTryAcquire(
            /* [in] */ Int32 acquires,
            /* [out] */ Boolean* pValue);

        ECode TryRelease(
            /* [in] */ Int32 releases,
            /* [out] */ Boolean* pValue);

        Boolean IsHeldExclusively();

        IConditionObject* NewCondition();

        AutoPtr<IThread> GetOwner();

        Int32 GetHoldCount();

        Boolean IsLocked();
    };

    class NonfairSync : public Sync {
        void Lock();

        ECode TryAcquire(
                /* [in] */ Int32 acquires,
                /* [out] */ Boolean* pValue);
    };

    class FairSync : public Sync {
        void Lock();

        ECode TryAcquire(
                /* [in] */ Int32 acquires,
                /* [out] */ Boolean* pValue);
    };

public:
    void Lock();


    ECode LockInterruptibly();

    Boolean TryLock();

    ECode TryLock(
        /* [in] */ Int64 timeout,
        /* [in] */ ITimeUnit* pUnit,
        /* [out] */ Boolean* pValue);

    void Unlock();

    ICondition* NewCondition();

    Int32 GetHoldCount();

    Boolean IsHeldByCurrentThread();

    Boolean IsLocked();

    Boolean IsFair();

    IThread* GetOwner();

    Boolean HasQueuedThreads();

    Boolean HasQueuedThread(
        /* [in] */ IThread* pThread);

    Int32 GetQueueLength();

    Int32 GetWaitQueueLength(
        /* [in] */ ICondition* condition);

    Boolean HasWaiters(
        /* [in] */ ICondition* condition);

    String ToString();
protected:

    ECode GetQueuedThreads(
        /* [out] */ IObjectEnumerator **ppEmu);

    ECode GetWaitingThreads(
        /* [in] */ ICondition* condition,
        /* [out] */ IObjectEnumerator **ppEmu);


private:
    Sync* m_Sync;

};

#endif