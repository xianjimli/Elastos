#include "AbstractQueuedSynchronizer.h"
#include <elastos/Thread.h>
#define E_THREAD_INTERRUPTED 0x80808080
#define E_ILLEGAL_MONITOR_STATE 0x80808081
//AbstractQueuedSynchronizer::Node* AbstractQueuedSynchronizer::Node::m_SHARED = new AbstractQueuedSynchronizer::Node();
//
//AbstractQueuedSynchronizer::Node* AbstractQueuedSynchronizer::Node::m_EXCLUSIVE = NULL;
//
//        /** waitStatus value to indicate thread has cancelled */
//Int32 AbstractQueuedSynchronizer::Node::m_CANCELLED =  1;
//        /** waitStatus value to indicate successor's thread needs unparking */
//Int32 AbstractQueuedSynchronizer::Node::m_SIGNAL    = -1;
//        /** waitStatus value to indicate thread is waiting on condition */
//Int32 AbstractQueuedSynchronizer::Node::m_CONDITION = -2;
//        /**
//         * waitStatus value to indicate the next acquireShared should
//         * unconditionally propagate
//         */
//Int32 AbstractQueuedSynchronizer::Node::m_PROPAGATE = -3;

Int64 AbstractQueuedSynchronizer::m_SpinForTimeoutThreshold = 1000L;
//AbstractQueuedSynchronizer::Node::Node()
//{
//}
//
//AbstractQueuedSynchronizer::Node::Node(
//            /* [in] */ IThread* pThread,
//            /* [in] */ Node* mode)
//{
//}
//
//AbstractQueuedSynchronizer::Node::Node(
//            /* [in] */ IThread* pThread,
//            /* [in] */ Int32 waitStatus)
//{
//}
//
//Boolean AbstractQueuedSynchronizer::Node::IsShared()
//{
//}
//
//AbstractQueuedSynchronizer::Node* AbstractQueuedSynchronizer::Node::Predecessor()
//{
//}

ECode AbstractQueuedSynchronizer::AcquireQueued(
        /* [in] */ Node* node,
        /* [in] */ Int32 arg,
        /* [out] */ Boolean* pValue)
{
    Boolean failed = TRUE;
    ECode ec = NOERROR;

    Boolean interrupted = FALSE;
    for (;;) {
        Node* p = node->Predecessor();
        if (p == m_Head) {
            Boolean isAcquired;
            ec = TryAcquire(arg, &isAcquired);
            if (FAILED(ec)) {
                if (failed)
                    CancelAcquire(node);
                return ec;
            }
            SetHead(node);
            p->m_Next = NULL; // help GC
            failed = FALSE;
            return interrupted;
        }
        if (ShouldParkAfterFailedAcquire(p, node) &&
            ParkAndCheckInterrupt())
            interrupted = true;
    }
    return ec;
}

ECode AbstractQueuedSynchronizer::AcquireInterruptibly(
    /* [in] */ Int32 arg)
{
    Boolean isInter;
    AutoPtr<IThread> curThread = Thread::GetCurrentThread();
    curThread->IsInterrupted(&isInter);
    if (isInter) {
        return E_THREAD_INTERRUPTED;
    }

    ECode ec = NOERROR;
    Boolean isAcquired;
    ec = TryAcquire(arg, &isAcquired);
    if (FAILED(ec))
        return DoAcquireInterruptibly(arg);
}

ECode AbstractQueuedSynchronizer::TryAcquireNanos(
    /* [in] */ Int32 arg,
    /* [in] */ Int64 nanosTimeout,
    /* [in] */ Boolean* pValue)
{
    Boolean isInter;
    ECode ec = NOERROR;
    AutoPtr<IThread> curThread = Thread::GetCurrentThread();
    curThread->IsInterrupted(&isInter);
    if (isInter) {
        return E_THREAD_INTERRUPTED;
    }

    Boolean isAcquired;
    TryAcquire(arg, &isAcquired);
    Boolean isNano;
    DoAcquireNanos(arg, nanosTimeout, &isNano);
    *pValue = isAcquired || isNano;
    return ec;
}

ECode AbstractQueuedSynchronizer::Release(
    /* [in] */ Int32 arg,
    /* [out] */ Boolean* pValue)
{
    Boolean isRelease;
    ECode ec = NOERROR;
    ec = TryRelease(arg, &isRelease);
    if (FAILED(ec)) {
        Node* h = m_Head;
        if (h != NULL && h->m_WaitStatus != 0)
            UnparkSuccessor(h);
        *pValue = TRUE;
    }
    *pValue = FALSE;
    return ec;
}

void AbstractQueuedSynchronizer::AcquireShared(
    /* [in] */ Int32 arg)
{
    Int64 isReleaseShare;
    TryAcquireShared(arg, &isReleaseShare);
    if (isReleaseShare < 0)
        DoAcquireShared(arg);
}

ECode AbstractQueuedSynchronizer::AcquireSharedInterruptibly(
    /* [in] */ Int32 arg)
{
    AutoPtr<IThread> curThread = Thread::GetCurrentThread();
    Boolean isInter;
    curThread->IsInterrupted(&isInter);
    if (isInter) {
        return E_THREAD_INTERRUPTED;
    }

    Int64 isReleaseShare;
    TryAcquireShared(arg, &isReleaseShare);
    if (isReleaseShare < 0)
        DoAcquireSharedInterruptibly(arg);

    return NOERROR;
}

ECode AbstractQueuedSynchronizer::TryAcquireSharedNanos(
    /* [in] */ Int32 arg,
    /* [in] */ Int64 nanosTimeout,
    /* [out] */ Boolean* pValue)
{
    AutoPtr<IThread> curThread = Thread::GetCurrentThread();
    Boolean isInter;
    curThread->IsInterrupted(&isInter);
    if (isInter) {
        return E_THREAD_INTERRUPTED;
    }

    Int64 isReleaseShare;
    TryAcquireShared(arg, &isReleaseShare);
    Boolean isNanos;
    DoAcquireSharedNanos(arg, nanosTimeout, &isNanos);
    *pValue = isReleaseShare || isNanos;
    return NOERROR;
}

ECode AbstractQueuedSynchronizer::ReleaseShared(
    /* [in] */ Int32 arg,
    /* [out] */ Boolean* pValue)
{
    Boolean isRls;
    TryReleaseShared(arg, &isRls);
    if (isRls) {
        DoReleaseShared();
        *pValue = TRUE;
    }
    *pValue = FALSE;
    return NOERROR;
}

ECode AbstractQueuedSynchronizer::HasQueuedThreads(
    /* [out] */ Boolean* pValue)
{
    *pValue = m_Head != m_Tail;
    return NOERROR;
}

ECode AbstractQueuedSynchronizer::HasContended(
    /* [out] */ Boolean* pValue)
{
    *pValue = m_Head != NULL;
    return NOERROR;
}

ECode AbstractQueuedSynchronizer::GetFirstQueuedThread(
    /* [out] */ IThread** ppThread)
{
    if (m_Head == m_Tail) {
        *ppThread == NULL;
    } else {
        *ppThread = FullGetFirstQueuedThread();
    }
    return NOERROR;;
}

ECode AbstractQueuedSynchronizer::IsQueued(
    /* [in] */ IThread* pThread,
    /* [out] */ Boolean* pValue)
{
    if (pThread == NULL)
        return E_INVALID_ARGUMENT;
    for (Node* p = m_Tail; p != NULL; p = p->m_Prev)
        if (p->m_Thread == pThread)
            *pValue = TRUE;
    *pValue = FALSE;
    return NOERROR;
}

ECode AbstractQueuedSynchronizer::ApparentlyFirstQueuedIsExclusive(
    /* [out] */ Boolean* pValue)
{
    Node *h, *s;
    *pValue = (h = m_Head) != NULL && (s = h->m_Next)  != NULL && !s->IsShared() && s->m_Thread != NULL;
    return NOERROR;
}

ECode AbstractQueuedSynchronizer::HasQueuedPredecessors(
    /* [out] */ Boolean* pValue)
{
    Node *t = m_Tail; // Read fields in reverse initialization order
    Node *h = m_Head;
    Node *s;
    *pValue = h != t && ((s = h->m_Next) == NULL || s->m_Thread != (IThread *)Thread::GetCurrentThread());
    return NOERROR;
}

ECode AbstractQueuedSynchronizer::GetQueueLength(
    /* [out] */ Int32* pValue)
{
    Int32 n = 0;
    for (Node* p = m_Tail; p != NULL; p = p->m_Prev) {
        if (p->m_Thread != NULL)
            ++n;
    }
    *pValue = n;
    return NOERROR;
}

ECode AbstractQueuedSynchronizer::GetQueuedThreads(
    /* [out] */ IObjectEnumerator** ppEmu)
{
    ECode ec = NOERROR;
    AutoPtr<IObjectContainer> container;
    ec = CObjectContainer::New((IObjectContainer **) &container);
    if (FAILED(ec)) {
        return ec;
    }

    for (Node* p = m_Tail; p != NULL; p = p->m_Prev) {
        IThread* t = p->m_Thread;
        if (t != NULL)
            container->Add(t);
    }

    ec = container->GetObjectEnumerator((IObjectEnumerator**)&ppEmu);
    return ec;
}

ECode AbstractQueuedSynchronizer::GetExclusiveQueuedThreads(
    /* [out] */ IObjectEnumerator** ppEmu)
{
    ECode ec = NOERROR;
    AutoPtr<IObjectContainer> container;
    ec = CObjectContainer::New((IObjectContainer **) &container);
    if (FAILED(ec)) {
        return ec;
    }

    for (Node* p = m_Tail; p != NULL; p = p->m_Prev) {
        if (!p->IsShared()) {
            IThread* t = p->m_Thread;
            if (t != NULL)
                container->Add(t);
        }
    }

    ec = container->GetObjectEnumerator((IObjectEnumerator**)&ppEmu);
    return ec;
}

ECode AbstractQueuedSynchronizer::GetSharedQueuedThreads(
    /* [out] */ IObjectEnumerator** ppEmu)
{
    ECode ec = NOERROR;
    AutoPtr<IObjectContainer> container;
    ec = CObjectContainer::New((IObjectContainer **) &container);
    if (FAILED(ec)) {
        return ec;
    }

    for (Node* p = m_Tail; p != NULL; p = p->m_Prev) {
        if (!p->IsShared()) {
            IThread* t = p->m_Thread;
            if (t != NULL)
                container->Add(t);
        }
    }
    ec = container->GetObjectEnumerator((IObjectEnumerator**)&ppEmu);
    return ec;
}

ECode AbstractQueuedSynchronizer::Owns(
    /* [in] */ IConditionObject* condition,
    /* [out] */ Boolean* pValue)
{
    if (condition == NULL)
        return E_INVALID_ARGUMENT;
    return NOERROR;
    //return condition->IsOwnedBy(this, pValue);
}

ECode AbstractQueuedSynchronizer::HasWaiters(
    /* [in] */ IConditionObject* condition,
    /* [out] */ Boolean* pValue)
{
//    if (!IsHeldExclusively())
//        return E_ILLEGAL_MONITOR_STATE;
//
//    for (Node* w = m_FirstWaiter; w != NULL; w = w->m_NextWaiter) {
//        if (w->m_WaitStatus == Node::m_CONDITION) {
//            *pValue = TRUE;
//            return NOERROR;
//        }
//    }
//
//    *pValue = FALSE;
    return NOERROR;
}

ECode AbstractQueuedSynchronizer::GetWaitQueueLength(
    /* [in] */ IConditionObject* condition,
    /* [out] */ Int32* pValue)
{
//    if (!IsHeldExclusively())
//        return E_ILLEGAL_MONITOR_STATE;
//
//    Int32 n = 0;
//    for (Node* w = m_FirstWaiter; w != NULL; w = w->m_NextWaiter) {
//        if (w->m_WaitStatus == Node::m_CONDITION)
//            ++n;
//    }
//
//    *pValue = n;
    return NOERROR;
}

ECode AbstractQueuedSynchronizer::GetWaitingThreads(
    /* [in] */ IConditionObject* condition,
    /* [out] */ IObjectEnumerator** ppEmu)
{
//    if (!IsHeldExclusively())
//        return E_ILLEGAL_MONITOR_STATE;
//
//    ECode ec = NOERROR;
//    AutoPtr<IObjectContainer> container;
//    ec = CObjectContainer::New((IObjectContainer **) &container);
//    if (FAILED(ec)) {
//        return ec;
//    }
//
//    for (Node* w = m_FirstWaiter; w != NULL; w = w->m_NextWaiter) {
//        if (w->m_WaitStatus == Node::m_CONDITION) {
//            IThread* t = w->m_Thread;
//            if (t != NULL)
//                container->Add(t);
//        }
//    }
//
//    ec = container->GetObjectEnumerator((IObjectEnumerator**)&ppEmu);
    return NOERROR;
}

Int64 AbstractQueuedSynchronizer::GetState()
{
    return m_State;
}

ECode AbstractQueuedSynchronizer::SetState(
    /* [in] */ Int32 newState)
{
    m_State = newState;
    return NOERROR;
}

Boolean AbstractQueuedSynchronizer::CompareAndSetState(
    /* [in] */ Int32 expect,
    /* [in] */ Int32 update)
{
    return TRUE;
}

ECode AbstractQueuedSynchronizer::TryAcquire(
    /* [in] */ Int32 arg,
    /* [out] */ Boolean* pValue)
{
    return E_NOT_IMPLEMENTED;
}

ECode AbstractQueuedSynchronizer::TryRelease(
    /* [in] */ Int32 arg,
    /* [out] */ Boolean* pValue)
{
    return E_NOT_IMPLEMENTED;
}

ECode AbstractQueuedSynchronizer::TryAcquireShared(
    /* [in] */ Int32 arg,
    /* [out] */ Int64* pValue)
{
    return E_NOT_IMPLEMENTED;
}

ECode AbstractQueuedSynchronizer::TryReleaseShared(
    /* [in] */ Int32 arg,
    /* [out] */ Boolean *pValue)
{
    return E_NOT_IMPLEMENTED;
}

Boolean AbstractQueuedSynchronizer::IsHeldExclusively()
{
    return TRUE;
}

Node* AbstractQueuedSynchronizer::Enq(
    /* [in] */ Node* node)
{
    for (;;) {
        Node* t = m_Tail;
        if (t == NULL) { // Must initialize
            if (CompareAndSetHead(new Node()))
                m_Tail = m_Head;
        } else {
            node->m_Prev = t;
            if (CompareAndSetTail(t, node)) {
                t->m_Next = node;
                return t;
            }
        }
    }
}

Node* AbstractQueuedSynchronizer::AddWaiter(
    /* [in] */ Node* mode)
{
    return NULL;
}

void AbstractQueuedSynchronizer::SetHead(
    /* [in] */ Node* node)
{
    return;
}

void AbstractQueuedSynchronizer::UnparkSuccessor(
    /* [in] */ Node* node)
{
    return;
}

void AbstractQueuedSynchronizer::DoReleaseShared()
{
    return;
}

void AbstractQueuedSynchronizer::SetHeadAndPropagate(
    /* [in] */ Node* node,
    /* [in] */ Int32 propagate)
{
    return;
}

void AbstractQueuedSynchronizer::CancelAcquire(
    /* [in] */ Node* node)
{
    return;
}

Boolean AbstractQueuedSynchronizer::ShouldParkAfterFailedAcquire(
    /* [in] */ Node* pred,
    /* [in] */ Node* node)
{
    return TRUE;
}

void AbstractQueuedSynchronizer::SelfInterrupt()
{
    return;
}

Boolean AbstractQueuedSynchronizer::ParkAndCheckInterrupt()
{
    return TRUE;
}

ECode AbstractQueuedSynchronizer::DoAcquireInterruptibly(
    /* [in] */ Int32 arg)
{
    Node* node = AddWaiter(Node::m_EXCLUSIVE);
    Boolean failed = TRUE;
    ECode ec = NOERROR;
    for (;;) {
        Node* p = node->Predecessor();
        if (p == m_Head) {
            Boolean isAcquired;
            ec = TryAcquire(arg, &isAcquired);
            if (FAILED(ec)) return ec;
            SetHead(node);
            p->m_Next = NULL; // help GC
            failed = false;
            return ec;
        }
        if (ShouldParkAfterFailedAcquire(p, node) &&
            ParkAndCheckInterrupt())
                return E_THREAD_INTERRUPTED;
    }

}

ECode AbstractQueuedSynchronizer::DoAcquireNanos(
    /* [in] */ Int32 arg,
    /* [in] */ Int64 nanosTimeout,
    /* [out] */ Boolean* pValue)
{

    Int64 lastTime;// = System.nanoTime();
    Node* node = AddWaiter(Node::m_EXCLUSIVE);
    Boolean failed = TRUE;
    ECode ec = NOERROR;

    for (;;) {
        Node* p = node->Predecessor();
        if (p == m_Head) {
            Boolean isAcquired;
            ec = TryAcquire(arg, &isAcquired);
            if (FAILED(ec)) {
                if (failed)
                    CancelAcquire(node);
                return ec;
            }
            SetHead(node);
            p->m_Next = NULL; // help GC
            failed = FALSE;
            *pValue = TRUE;
        }
        if (nanosTimeout <= 0)
            *pValue = FALSE;
        if (ShouldParkAfterFailedAcquire(p, node) &&
            nanosTimeout > m_SpinForTimeoutThreshold);
            //LockSupport.parkNanos(this, nanosTimeout);
        Int64 now;// = System.nanoTime();
        nanosTimeout -= now - lastTime;
        lastTime = now;
        AutoPtr<IThread> curThread = Thread::GetCurrentThread();
        Boolean isInter;
        curThread->IsInterrupted(&isInter);
        if (isInter) {
            return E_THREAD_INTERRUPTED;
        }
    }
    return ec;
}

void AbstractQueuedSynchronizer::DoAcquireShared(
    /* [in] */ Int32 arg)
{
    Node* node = AddWaiter(Node::m_SHARED);
    Boolean failed = TRUE;
    Boolean interrupted = FALSE;
    for (;;) {
        Node* p = node->Predecessor();
        if (p == m_Head) {
            Int64 r;
            TryAcquireShared(arg, &r);
            if (r >= 0) {
                SetHeadAndPropagate(node, r);
                p->m_Next = NULL; // help GC
                if (interrupted)
                    SelfInterrupt();
                failed = FALSE;
                return;
            }
        }
        if (ShouldParkAfterFailedAcquire(p, node) &&
            ParkAndCheckInterrupt())
            interrupted = TRUE;
    }
    return;
}

void AbstractQueuedSynchronizer::DoAcquireSharedInterruptibly(
    /* [in] */ Int32 arg)
{
    return;
}

ECode AbstractQueuedSynchronizer::DoAcquireSharedNanos(
    /* [in] */ Int32 arg,
    /* [in] */ Int64 nanosTimeout,
    /* [out] */ Boolean* pValue)
{
    Int64 lastTime;// = System.nanoTime();
    Node* node = AddWaiter(Node::m_SHARED);
    Boolean failed = TRUE;
    ECode ec = NOERROR;

    for (;;) {
        Node* p = node->Predecessor();
        if (p == m_Head) {
            Int64 r;
            ec = TryAcquireShared(arg, &r);
            if (FAILED(ec)) {
            }
            if (r >= 0) {
                SetHeadAndPropagate(node, r);
                p->m_Next = NULL; // help GC
                failed = false;
                *pValue = TRUE;
            }
        }
        if (nanosTimeout <= 0)
            *pValue = FALSE;
        if (ShouldParkAfterFailedAcquire(p, node) &&
            nanosTimeout > m_SpinForTimeoutThreshold);
            //LockSupport.parkNanos(this, nanosTimeout);
        Int64 now;// = System.nanoTime();
        nanosTimeout -= now - lastTime;
        lastTime = now;
        AutoPtr<IThread> curThread = Thread::GetCurrentThread();
        Boolean isInter;
        curThread->IsInterrupted(&isInter);
        if (isInter) {
            return E_THREAD_INTERRUPTED;
        }
    }
}

IThread* AbstractQueuedSynchronizer::FullGetFirstQueuedThread()
{
    Node* h;
    Node* s;
    IThread* st;
    if (((h = m_Head) != NULL && (s = h->m_Next) != NULL &&
         s->m_Prev == m_Head && (st = s->m_Thread) != NULL) ||
        ((h = m_Head) != NULL && (s = h->m_Next) != NULL &&
         s->m_Prev == m_Head && (st = s->m_Thread) != NULL))
        return st;

    /*
     * Head's next field might not have been set yet, or may have
     * been unset after setHead. So we must check to see if tail
     * is actually first node. If not, we continue on, safely
     * traversing from tail back to head to find first,
     * guaranteeing termination.
     */

    Node* t = m_Tail;
    IThread* firstThread = NULL;
    while (t != NULL && t != m_Head) {
        IThread* tt = t->m_Thread;
        if (tt != NULL)
            firstThread = tt;
        t = t->m_Prev;
    }
    return firstThread;
}

ECode AbstractQueuedSynchronizer::IsOnSyncQueue(
    /* [in] */ Node* node,
    /* [out] */ Boolean* pValue)
{
    ECode ec = NOERROR;
    if (node->m_WaitStatus == Node::m_CONDITION || node->m_Prev == NULL) {
        *pValue = FALSE;
        return ec;
    }
    if (node->m_Next != NULL) {
        *pValue = TRUE;
        return ec;
    }

    return FindNodeFromTail(node, pValue);
}

ECode AbstractQueuedSynchronizer::FindNodeFromTail(
    /* [in] */ Node* node,
    /* [out] */ Boolean* pValue)
{
    ECode ec = NOERROR;
    Node* t = m_Tail;
    for (;;) {
        if (t == node) {
            *pValue = TRUE;
            return ec;
        }
        if (t == NULL) {
            *pValue = FALSE;
            return ec;
        }
        t = t->m_Prev;
    }
    return ec;
}

ECode AbstractQueuedSynchronizer::TransferForSignal(
    /* [in] */ Node* node,
    /* [out] */ Boolean* pValue )
{
    ECode ec = NOERROR;
    if (1./*!CompareAndSetWaitStatus(node, Node.CONDITION, 0)*/) {
        *pValue = FALSE;
        return ec;
    }

    /*
     * Splice onto queue and try to set waitStatus of predecessor to
     * indicate that thread is (probably) waiting. If cancelled or
     * attempt to set waitStatus fails, wake up to resync (in which
     * case the waitStatus can be transiently and harmlessly wrong).
     */
    Node* p = Enq(node);
    Int32 ws = p->m_WaitStatus;
    if (ws > 0 /*|| !CompareAndSetWaitStatus(p, ws, Node.SIGNAL)*/)
        //LockSupport.unpark(node.thread);
    *pValue = TRUE;
    return ec;
}

ECode AbstractQueuedSynchronizer::TransferAfterCancelledWait(
    /* [in] */ Node* node,
    /* [out] */ Int32* pValue)
{
    AutoPtr<IThread> curThread = Thread::GetCurrentThread();
    Boolean isInter;
    curThread->IsInterrupted(&isInter);
    if (isInter) {
        return E_THREAD_INTERRUPTED;
    }
}

ECode AbstractQueuedSynchronizer::FullyRelease(
    /* [in] */ Node* node,
    /* [out] */ Int64* pValue)
{
    Boolean failed = TRUE;
    Int64 savedState = GetState();
    Boolean isRls;
    Release(savedState, &isRls);
    if (isRls) {
        failed = FALSE;
        *pValue = savedState;
        return NOERROR;
    } else {
        if (failed)
            node->m_WaitStatus = Node::m_CANCELLED;
        return E_ILLEGAL_MONITOR_STATE;
    }
}

Boolean AbstractQueuedSynchronizer::CompareAndSetHead(
    /* [in] */ Node* update)
{
    return TRUE;
}

Boolean AbstractQueuedSynchronizer::CompareAndSetTail(
    /* [in] */ Node* expect,
    /* [in] */ Node* update)
{
    return NOERROR;
}

ECode AbstractQueuedSynchronizer::Acquire(
    /* [in] */ Int32 arg)
{
}
