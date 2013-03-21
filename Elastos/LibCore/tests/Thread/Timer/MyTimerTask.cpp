
#include <elastos/Thread.h>
#include "MyTimerTask.h"
#include "stdio.h"

static Int32 count = 0;
MyTimerTask::MyTimerTask()
{
    printf("-----------------------\n");
}

UInt32 MyTimerTask::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 MyTimerTask::Release()
{
    return ElRefBase::Release();
}

PInterface MyTimerTask::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IRunnable) {
        return (IRunnable*)this;
    }else if (riid == EIID_ITimerTask) {
        return (IRunnable*)this;
    }

    return NULL;
}

ECode MyTimerTask::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IRunnable*)this) {
        *pIID = EIID_IRunnable;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode MyTimerTask::Run()
{
    printf("\n\n*************\n");
    count++;
    printf("count is %d\n", count);
    printf("======Timer Excute...======\n");
    printf("\n\n*************\n");
    return NOERROR;
}

Mutex* MyTimerTask::GetSelfLock()
{
    return &mLock;
}

ECode MyTimerTask::Cancel(
    /* [out] */ Boolean * pValue)
{
    assert(pValue != NULL);
    Mutex::Autolock lock(GetSelfLock());
    Boolean willRun = !mCancelled && mWhen > 0;
    mCancelled = TRUE;
    *pValue = willRun;
    return NOERROR;
}

ECode MyTimerTask::ScheduledExecutionTime(
    /* [out] */ Int64 * pValue)
{
    assert(time != NULL);
    Mutex::Autolock lock(GetSelfLock());
    *pValue = mScheduledTime;
    return NOERROR;
}

ECode MyTimerTask::GetWhen(
        /* [out] */ Int64* when)
{
    assert(when != NULL);
    Mutex::Autolock lock(GetSelfLock());
    *when = mWhen;
    return NOERROR;
}

ECode MyTimerTask::SetWhen(
        /* [in] */ Int64 when)
{
    Mutex::Autolock lock(GetSelfLock());
    mWhen = when;
    return NOERROR;
}

ECode MyTimerTask::IsScheduled(
        /* [out] */ Boolean* scheduled)
{
    assert(scheduled != NULL);
    Mutex::Autolock lock(GetSelfLock());
    *scheduled = mWhen > 0 || mScheduledTime > 0;
    return NOERROR;
}

ECode MyTimerTask::IsCancelled(
        /* [out] */ Boolean* cancelled)
{
    assert(cancelled != NULL);
    Mutex::Autolock lock(GetSelfLock());
    *cancelled = mCancelled;
    return NOERROR;
}

ECode MyTimerTask::GetPeriod(
        /* [out] */ Int64* period)
{
    assert(period != NULL);
    Mutex::Autolock lock(GetSelfLock());
    *period = mPeriod;
    return NOERROR;
}

ECode MyTimerTask::SetPeriod(
        /* [in] */ Int64 period)
{
    Mutex::Autolock lock(GetSelfLock());
    mPeriod = period;
    return NOERROR;
}

ECode MyTimerTask::IsFixedRate(
        /* [out] */ Boolean* fixed)
{
    assert(fixed);
    Mutex::Autolock lock(GetSelfLock());
    *fixed = mFixedRate;
    return NOERROR;
}

ECode MyTimerTask::SetFixedRate(
        /* [in] */ Boolean fixed)
{
    Mutex::Autolock lock(mLock);
    mFixedRate = fixed;
    return NOERROR;
}

ECode MyTimerTask::Lock()
{
    mLock.Lock();
    return NOERROR;
}

ECode MyTimerTask::Unlock()
{
    mLock.Unlock();
    return NOERROR;
}

ECode MyTimerTask::SetScheduledTime(
            /* [in] */ Int64 time)
{
    Mutex::Autolock lock(GetSelfLock());
    mScheduledTime = time;
    return NOERROR;
}