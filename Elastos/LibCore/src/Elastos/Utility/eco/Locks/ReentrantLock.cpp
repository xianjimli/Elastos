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
    return m_Sync->NonfairTryAcquire(1, pValue);
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
    IConditionObject *pCondition;
    CConditionObject::New((IConditionObject**)&pCondition);
    return (ICondition *)pCondition;
}

Int32 ReentrantLock::GetHoldCount()
{
    return m_Sync->GetHoldCount();
}

Boolean ReentrantLock::IsHeldByCurrentThread()
{
    return m_Sync->IsHeldExclusively();
}

Boolean ReentrantLock::IsLocked()
{
    return m_Sync->IsLocked();
}

Boolean ReentrantLock::IsFair()
{
    return TRUE;
}

IThread* ReentrantLock::GetOwner()
{
    return m_Sync->GetOwner();
}

Boolean ReentrantLock::HasQueuedThreads()
{
    Boolean has;
    m_Sync->HasQueuedThreads(&has);
    return has;
}

Boolean ReentrantLock::HasQueuedThread(
        /* [in] */ IThread* pThread)
{
    Boolean has;
    m_Sync->IsQueued(pThread, &has);
    return has;
}

Int32 ReentrantLock::GetQueueLength()
{
    Int32 len;
    m_Sync->GetQueueLength(&len);
    return len;
}

ECode ReentrantLock::GetWaitQueueLength(
        /* [in] */ ICondition* condition,
        /* [out] */ Int32* pValue)
{
    if (condition == NULL) {
        return E_INVALID_ARGUMENT;
    }

    return m_Sync->GetWaitQueueLength((IConditionObject *)condition, pValue);
}

ECode ReentrantLock::HasWaiters(
        /* [in] */ ICondition* condition,
        /* [out] */ Boolean* pValue)
{
    if (condition == NULL) {
        return E_INVALID_ARGUMENT;
    }

    return m_Sync->HasWaiters((IConditionObject *)condition, pValue);
}

String ReentrantLock::ToString()
{
}

ECode ReentrantLock::GetQueuedThreads(
        /* [out] */ IObjectEnumerator **ppEmu)
{
    return m_Sync->GetQueuedThreads(ppEmu);
}

ECode ReentrantLock::GetWaitingThreads(
        /* [in] */ ICondition* condition,
        /* [out] */ IObjectEnumerator **ppEmu)
{
    return m_Sync->GetWaitingThreads((IConditionObject*)condition, ppEmu);
}
