#include "CountDownLatch.h"


Int64 CountDownLatch::Sync::m_SerialVersionUID = 4982264981922014374L;

CountDownLatch::Sync::Sync(
            /* [in] */ Int32 count)
{
    SetState(count);
}

ECode CountDownLatch::Sync::GetCount(
            /* [out] */ Int32* pState)
{
    *pState = (Int32) GetState();
    return NOERROR;
}


Int32 CountDownLatch::Sync::TryAcquireShared(
            /* [in] */ Int32 acquires)
{
    Int32 state;
    state = (Int32) GetState();
    return state == 0? 1 : -1;
}

Boolean CountDownLatch::Sync::TryReleaseShared(
            /* [in] */ Int32 releases)
{
    for (;;) {
        Int32 c;
        c = (Int32) GetState();
        if (c == 0)
            return FALSE;
        Int32 nextc = c-1;
        if (CompareAndSetState(c, nextc))
            return nextc == 0;
    }
}

CountDownLatch::CountDownLatch(
        /* [in] */ Int32 count)
{
}

CountDownLatch::CountDownLatch()
{
}

ECode CountDownLatch::AWait()
{
    return m_Sync->AcquireSharedInterruptibly(1);
}

ECode CountDownLatch::AWait(
    /* [in] */ Int64 timeout,
    /* [in] */ ITimeUnit* pUnit,
    /* [out] */ Boolean* pValue)
{
    return m_Sync->TryAcquireSharedNanos(1, timeout, pValue);
}

ECode CountDownLatch::CountDown()
{
    return m_Sync->ReleaseShared(1, NULL);
}

ECode CountDownLatch::GetCount(
    /* [out] */ Int64* pValue)
{
    return m_Sync->GetCount((Int32*)pValue);
    return NOERROR;
}

String CountDownLatch::ToString()
{
}