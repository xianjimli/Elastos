#include "Timer.h"
#include <elastos/System.h>
#include <StringBuffer.h>

Timer::TimerImpl::TimerHeap::TimerHeap()
{
    mTimers = ArrayOf<TimerTask*>::Alloc(256);
//    TimerTask a;
}

ECode Timer::TimerImpl::TimerHeap::UpHeap()
{
    return NOERROR;
}

ECode Timer::TimerImpl::TimerHeap::DownHeap(
    /* [in] */ Int32 pos)
{
    return NOERROR;
}

TimerTask* Timer::TimerImpl::TimerHeap::Minimum()
{
    return NULL;
}

Boolean Timer::TimerImpl::TimerHeap::IsEmpty()
{
    return TRUE;
}

ECode Timer::TimerImpl::TimerHeap::Insert(
    /* [in] */ TimerTask* task)
{
    return NOERROR;
}

ECode Timer::TimerImpl::TimerHeap::Delete(
    /* [in] */ Int32 pos)
{
    return NOERROR;
}

ECode Timer::TimerImpl::TimerHeap::Reset()
{
    return NOERROR;
}

ECode Timer::TimerImpl::TimerHeap::AdjustMinimum()
{
    return NOERROR;
}


ECode Timer::TimerImpl::TimerHeap::DeleteIfCancelled()
{
    return NOERROR;
}

ECode Timer::TimerImpl::TimerHeap::GetTask(
    /* [in] */ TimerTask* task)
{
    return NOERROR;
}


Timer::TimerImpl::TimerImpl(
    /* [in] */ String name,
    /* [in] */ Boolean isDaemon)
{
//    SetName(name);
//    SetDaemon(isDaemon);
//    Start();
}

ECode Timer::TimerImpl::Run()
{
    return NOERROR;
}

ECode Timer::TimerImpl::Cancel()
{
    return NOERROR;
}

Int32 Timer::TimerImpl::Purge()
{
    return NOERROR;
}

Int32 Timer::TimerImpl::InsertTask(
    /* [in] */ TimerTask* newTask)
{
    return NOERROR;
}

Timer::FinalizerHelper::FinalizerHelper(
    /* [in] */ TimerImpl* impl)
{
    mImpl = impl;
}

ECode Timer::FinalizerHelper::Finalize()
{
    Mutex::Autolock lock(&mImpl->mLock);
    mImpl->mFinished = TRUE;
}

Int64 Timer::mTimerId = 0;

Int64 Timer::NextId()
{
    return mTimerId++;
}

Timer::Timer(
    /* [in] */ String name,
    /* [in] */ Boolean isDaemon)
{
    if (name.IsNull()){
        return;
    }

    this->mImpl = new TimerImpl(name, isDaemon);
    this->mFinalizer = new FinalizerHelper(mImpl);
}

Timer::Timer(
    /* [in] */ String name)
{
    Timer(name, FALSE);
}

Timer::Timer(
    /* [in] */Boolean isDaemon)
{
    StringBuffer str;
    str += String("Timer-");
    str += Timer::NextId();
    Timer(str.Substring(0, str.GetLength()), isDaemon);
}

Timer::Timer()
{
    Timer(FALSE);
}

ECode Timer::Cancel()
{
    return mImpl->Cancel();
}

Int32 Timer::Purge()
{
    Mutex::Autolock lock(&mImplLock);
    return mImpl->Purge();
}

ECode Timer::Schedule(
    /* [in] */ TimerTask* task,
    /* [in] */ IDate* when)
{
    Int64 wTime;
    when->GetTime(&wTime);

    if (wTime < 0) {
        return E_INVALID_ARGUMENT;
    }
    Int64 delay = wTime - System::GetCurrentTimeMillis();
    return ScheduleImpl(task, delay < 0 ? 0 : delay, -1, FALSE);
}



ECode Timer::Schedule(
    /* [in] */ TimerTask* task,
    /* [in] */ Int64 delay)
{
    if (delay < 0) {
        return E_INVALID_ARGUMENT;
    }
    return ScheduleImpl(task, delay, -1, FALSE);
}


ECode Timer::Schedule(
    /* [in] */ TimerTask* task,
    /* [in] */ Int64 delay,
    /* [in] */ Int64 period)
{
    if (delay < 0 || period <= 0) {
        return E_INVALID_ARGUMENT;
    }
    return ScheduleImpl(task, delay, period, FALSE);
}

ECode Timer::Schedule(
        /* [in] */ TimerTask* task,
        /* [in] */ IDate* when,
        /* [in] */ Int64 period)
{
    Int64 wTime;
    when->GetTime(&wTime);

    if (period <= 0 || wTime < 0) {
        return E_INVALID_ARGUMENT;
    }
    Int64 delay = wTime - System::GetCurrentTimeMillis();
    return ScheduleImpl(task, delay < 0 ? 0 : delay, period, FALSE);
}

ECode Timer::ScheduleAtFixedRate(
    /* [in] */ TimerTask* task,
    /* [in] */ Int64 delay,
    /* [in] */ Int64 period)
{
    if (delay < 0 || period <= 0) {
        return E_INVALID_ARGUMENT;
    }
    return ScheduleImpl(task, delay, period, TRUE);
}

ECode Timer::ScheduleAtFixedRate(
    /* [in] */ TimerTask* task,
    /* [in] */ IDate* when,
    /* [in] */ Int64 period)
{
    Int64 wTime;
    when->GetTime(&wTime);

    if (period <= 0 || wTime < 0) {
        return E_INVALID_ARGUMENT;
    }

    Int64 delay = wTime - System::GetCurrentTimeMillis();
    return ScheduleImpl(task, delay, period, TRUE);
}

ECode Timer::ScheduleImpl(
    /* [in] */ TimerTask* task,
    /* [in] */ Int64 delay,
    /* [in] */ Int64 period,
    /* [in] */ Boolean fixed)
{
    Mutex::Autolock lock(&mImplLock);

    if (mImpl->mCancelled) {
        //return E_TIMER_CANCEL;
    }

    Int64 when = delay + System::GetCurrentTimeMillis();

    if (when < 0) {
        // return E_TIMER_DELAY;
    }

    Mutex::Autolock lock1(&task->mLock);
    {
        if (task->IsScheduled()) {
            //return E_TIMER_SCHEDULED_ALREADY;
        }

        if (task->mCancelled) {
            //return E_TIMER_CANCEL;
        }

        task->mWhen = when;
        task->mPeriod = period;
        task->mFixedRate = fixed;
    }

    // insert the newTask into queue
    mImpl->InsertTask(task);
    return NOERROR;
}