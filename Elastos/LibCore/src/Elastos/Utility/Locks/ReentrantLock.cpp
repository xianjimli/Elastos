#include "ReentrantLock.h"
#include <elastos/Thread.h>

ReentrantLock::ReentrantLock() {
    m_Sync = new NonfairSync();
}

ReentrantLock::ReentrantLock(
    /* [in] */ Boolean fair) {
    m_Sync = (fair)? (Sync *)new FairSync() : (Sync *)new NonfairSync();
}


ECode ReentrantLock::Sync::NonfairTryAcquire(
    /* [in] */ Int32 acquires,
    /* [out] */ Boolean* pValue)
{
    AutoPtr<IThread> curThread = Thread::GetCurrentThread();

    Int32 c = (Int32) GetState();
    if (c == 0) {
        if (CompareAndSetState(0, acquires)) {
            SetExclusiveOwnerThread((IThread*)curThread);
            *pValue = TRUE;
            return NOERROR;
        }
    }
    else if (curThread == GetExclusiveOwnerThread()) {
        Int32 nextc = c + acquires;
        if (nextc < 0) // overflow
            return E_FAIL;

        SetState(nextc);
        *pValue = TRUE;
        return NOERROR;
    }
    *pValue = FALSE;
    return NOERROR;
}

//Boolean ReentrantLock::Sync::NonfairTryAcquire(
//    /* [in] */ Int32 acquires)
//{
//}
#define E_ILLEGAL_MONITOR_STATE 0x80808081;
ECode ReentrantLock::Sync::TryRelease(
    /* [in] */ Int32 releases,
    /* [out] */ Boolean* pValue)
{
    Int32 c = (Int32)GetState() - releases;
    if (Thread::GetCurrentThread() != GetExclusiveOwnerThread())
        return E_ILLEGAL_MONITOR_STATE;

    Boolean free = FALSE;
    if (c == 0) {
        free = TRUE;
        SetExclusiveOwnerThread(NULL);
    }
    SetState(c);
    *pValue = free;
    return NOERROR;
}

Boolean ReentrantLock::Sync::IsHeldExclusively()
{
    return GetExclusiveOwnerThread() == Thread::GetCurrentThread();
}

IConditionObject* ReentrantLock::Sync::NewCondition()
{
}

AutoPtr<IThread> ReentrantLock::Sync::GetOwner()
{
    return GetState() == 0 ? NULL : GetExclusiveOwnerThread();
}

Int32 ReentrantLock::Sync::GetHoldCount()
{
    return IsHeldExclusively() ? (Int32)GetState() : 0;
}

Boolean ReentrantLock::Sync::IsLocked()
{
    return GetState() != 0;
}


void ReentrantLock::NonfairSync::Lock()
{
    if (CompareAndSetState(0, 1))
        SetExclusiveOwnerThread(Thread::GetCurrentThread());
    else
        Acquire(1);
}

ECode ReentrantLock::NonfairSync::TryAcquire(
    /* [in] */ Int32 acquires,
    /* [out] */ Boolean* pValue)
{
    return NonfairTryAcquire(acquires, pValue);
}

void ReentrantLock::FairSync::Lock()
{
    Acquire(1);
}

ECode ReentrantLock::FairSync::TryAcquire(
    /* [in] */ Int32 acquires,
    /* [out] */ Boolean* pValue)
{
    AutoPtr<IThread> current = Thread::GetCurrentThread();
    Int32 c = (Int32)GetState();
    if (c == 0) {
        Boolean has;
        HasQueuedPredecessors(&has);
        if (!has &&
            CompareAndSetState(0, acquires)) {
            SetExclusiveOwnerThread(current);
            *pValue = TRUE;
            return NOERROR;
        }
    }
    else if (current == GetExclusiveOwnerThread()) {
        Int32 nextc = c + acquires;
        if (nextc < 0)
            return E_FAIL;
        SetState(nextc);
        *pValue = TRUE;
        return NOERROR;
    }
    *pValue = TRUE;
    return NOERROR;
}

void ReentrantLock::Lock()
{
    m_Sync->Lock();
}

ECode ReentrantLock::LockInterruptibly()
{
    return m_Sync->AcquireInterruptibly(1);
}

ECode ReentrantLock::TryLock(
    /* [out] */ Boolean* pValue)
{
    return m_Sync->nonfairTryAcquire(1, pValue);
}

ECode ReentrantLock::TryLock(
    /* [in] */ Int64 timeout,
    /* [in] */ ITimeUnit* pUnit,
    /* [out] */ Boolean* pValue)
{
    return  m_Sync->TryAcquireNanos(1, NULL, pValue);
}

void ReentrantLock::Unlock()
{
    m_Sync->Release(1, NULL);
}

ICondition* ReentrantLock::NewCondition()
{
}

Int32 ReentrantLock::GetHoldCount()
{
}

Boolean ReentrantLock::IsHeldByCurrentThread()
{
}

Boolean ReentrantLock::IsLocked()
{
}

Boolean ReentrantLock::IsFair()
{
}

IThread* ReentrantLock::GetOwner()
{
}

Boolean ReentrantLock::HasQueuedThreads()
{
}

Boolean ReentrantLock::HasQueuedThread(
        /* [in] */ IThread* pThread)
{
}

Int32 ReentrantLock::GetQueueLength()
{
}

Int32 ReentrantLock::GetWaitQueueLength(
        /* [in] */ ICondition* condition)
{
}

Boolean ReentrantLock::HasWaiters(
        /* [in] */ ICondition* condition)
{
}

String ReentrantLock::ToString()
{
}

ECode ReentrantLock::GetQueuedThreads(
        /* [out] */ IObjectEnumerator **ppEmu)
{
}

ECode ReentrantLock::GetWaitingThreads(
        /* [in] */ ICondition* condition,
        /* [out] */ IObjectEnumerator **ppEmu)
{
}
