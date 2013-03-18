
#ifndef __ABSTRACTQUEUEDSYNCHRONIZER_H__
#define __ABSTRACTQUEUEDSYNCHRONIZER_H__

#include <elastos.h>
#include <elastos/Mutex.h>
#include "Elastos.Core.h"
#include "CConditionObject.h"
#include "Node.h"
#include <elastos/AutoPtr.h>
#include "AbstractOwnableSynchronizer.h"

using namespace Elastos;
using namespace Elastos::Core::Threading;

class AbstractQueuedSynchronizer : public AbstractOwnableSynchronizer
{
public:
    ECode AcquireQueued(
        /* [in] */ Node* node,
        /* [in] */ Int32 arg,
        /* [out] */ Boolean* pValue);

    ECode AcquireInterruptibly(
        /* [in] */ Int32 arg);

    ECode TryAcquireNanos(
        /* [in] */ Int32 arg,
        /* [in] */ Int64 nanosTimeout,
        /* [out] */ Boolean* pValue);

    ECode Release(
        /* [in] */ Int32 arg,
        /* [out] */ Boolean* pValue);

    void AcquireShared(
        /* [in] */ Int32 arg);

    ECode AcquireSharedInterruptibly(
        /* [in] */ Int32 arg);

    ECode TryAcquireSharedNanos(
        /* [in] */ Int32 arg,
        /* [in] */ Int64 nanosTimeout,
        /* [out] */ Boolean* pValue);

    ECode ReleaseShared(
        /* [in] */ Int32 arg,
        /* [out] */ Boolean* pValue);

    ECode HasQueuedThreads(
        /* [out] */ Boolean* pValue);

    ECode HasContended(
        /* [out] */ Boolean* pValue);

    ECode GetFirstQueuedThread(
        /* [out] */ IThread** ppThread);

    ECode IsQueued(
        /* [in] */ IThread* pThread,
        /* [out] */ Boolean* pValue);

    ECode ApparentlyFirstQueuedIsExclusive(
        /* [out] */ Boolean* pValue);

    ECode HasQueuedPredecessors(
        /* [out] */ Boolean* pValue);

    ECode GetQueueLength(
        /* [out] */ Int32* pValue);

    ECode GetQueuedThreads(
        /* [out] */ IObjectEnumerator** ppEmu);

    ECode GetExclusiveQueuedThreads(
        /* [out] */ IObjectEnumerator** ppEmu);

    ECode GetSharedQueuedThreads(
        /* [out] */ IObjectEnumerator** ppEmu);

    ECode Owns(
        /* [in] */ IConditionObject* condition,
        /* [out] */ Boolean* pValue);

    ECode HasWaiters(
        /* [in] */ IConditionObject* condition,
        /* [out] */ Boolean* pValue);

    ECode GetWaitQueueLength(
        /* [in] */ IConditionObject* condition,
        /* [out] */ Int32* pValue);

    ECode GetWaitingThreads(
        /* [in] */ IConditionObject* condition,
        /* [out] */ IObjectEnumerator** ppEmu);

    ECode Acquire(
        /* [in] */ Int32 arg);

protected:
    Int64 GetState();

    ECode SetState(
        /* [in] */ Int32 newState);

    Boolean CompareAndSetState(
        /* [in] */ Int32 expect,
        /* [in] */ Int32 update);

    ECode TryAcquire(
        /* [in] */ Int32 arg,
        /* [out] */ Boolean* pValue);

    ECode TryRelease(
        /* [in] */ Int32 arg,
        /* [out */ Boolean* pValue);

    ECode TryAcquireShared(
        /* [in] */ Int32 arg,
        /* [out] */ Int64* pValue);

    ECode TryReleaseShared(
        /* [in] */ Int32 arg,
        /* [out] */ Boolean* pValue);

    Boolean IsHeldExclusively();

private:
    Node* Enq(
        /* [in] */ Node* node);

    Node* AddWaiter(
        /* [in] */ Node* mode);

    void SetHead(
        /* [in] */ Node* node);

    void UnparkSuccessor(
        /* [in] */ Node* node);

    void DoReleaseShared();

    void SetHeadAndPropagate(
        /* [in] */ Node* node,
        /* [in] */ Int32 propagate);

    void CancelAcquire(
        /* [in] */ Node* node);

    static Boolean ShouldParkAfterFailedAcquire(
        /* [in] */ Node* pred,
        /* [in] */ Node* node);

    static void SelfInterrupt();

    Boolean ParkAndCheckInterrupt();

    ECode DoAcquireInterruptibly(
        /* [in] */ Int32 arg);

    ECode DoAcquireNanos(
        /* [in] */ Int32 arg,
        /* [in] */ Int64 nanosTimeout,
        /* [out] */ Boolean* pValue);

    void DoAcquireShared(
        /* [in] */ Int32 arg);

    void DoAcquireSharedInterruptibly(
        /* [in] */ Int32 arg);

    ECode DoAcquireSharedNanos(
        /* [in] */ Int32 arg,
        /* [in] */ Int64 nanosTimeout,
        /* [out] */ Boolean* pValue);

    IThread* FullGetFirstQueuedThread();

    ECode IsOnSyncQueue(
        /* [in] */ Node* node,
        /* [out */ Boolean* pValue);

    ECode FindNodeFromTail(
        /* [in] */ Node* node,
        /* [out */ Boolean* pValue);

    ECode TransferForSignal(
        /* [in] */ Node* node,
        /* [out */ Boolean* pValue);

    ECode TransferAfterCancelledWait(
        /* [in] */ Node* node,
        /* [out] */ Int32* pValue);

    ECode FullyRelease(
        /* [in] */ Node* node,
        /* [out] */ Int64* pValue);

    Boolean CompareAndSetHead(
        /* [in] */ Node* update);

    Boolean CompareAndSetTail(
        /* [in] */ Node* expect,
        /* [in] */ Node* update);

private:
    Node* m_Head;
    Node* m_Tail;
    Int64 m_State;

public:
    static Int64 m_SpinForTimeoutThreshold;
};

#endif // __ABSTRACTQUEUEDSYNCHRONIZER_H__
