
#include <elastos/TimerTask.h>

TimerTask::TimerTask()
    : mCancelled(FALSE)
    , mWhen(0)
    , mPeriod(0)
    , mFixedRate(FALSE)
    , mScheduledTime(0)
{}

ECode TimerTask::GetWhen(
    /* [out] */ Int64* when)
{
    assert(when != NULL);
    Mutex::Autolock lock(GetSelfLock());
    *when = mWhen;
    return NOERROR;
}

ECode TimerTask::SetScheduledTime(
    /* [in] */ Int64 time)
{
    Mutex::Autolock lock(GetSelfLock());
    mScheduledTime = time;
    return NOERROR;
}

ECode TimerTask::IsScheduled(
    /* [out] */ Boolean* scheduled)
{
    assert(scheduled != NULL);
    Mutex::Autolock lock(GetSelfLock());
    *scheduled = mWhen > 0 || mScheduledTime > 0;
    return NOERROR;
}

ECode TimerTask::Cancel(
    /* [out] */ Boolean* cancelled)
{
    assert(cancelled != NULL);
    Mutex::Autolock lock(GetSelfLock());
    Boolean willRun = !mCancelled && mWhen > 0;
    mCancelled = TRUE;
    *cancelled = willRun;
    return NOERROR;
}

ECode TimerTask::ScheduledExecutionTime(
    /* [out] */ Int64* time)
{
    assert(time != NULL);
    Mutex::Autolock lock(GetSelfLock());
    *time = mScheduledTime;
    return NOERROR;
}

ECode TimerTask::SetWhen(
    /* [in] */ Int64 when)
{
    Mutex::Autolock lock(GetSelfLock());
    mWhen = when;
    return NOERROR;
}

ECode TimerTask::IsCancelled(
    /* [out] */ Boolean* cancelled)
{
    assert(cancelled != NULL);
    Mutex::Autolock lock(GetSelfLock());
    *cancelled = mCancelled;
    return NOERROR;
}

ECode TimerTask::GetPeriod(
    /* [out] */ Int64* period)
{
    assert(period != NULL);
    Mutex::Autolock lock(GetSelfLock());
    *period = mPeriod;
    return NOERROR;
}

ECode TimerTask::SetPeriod(
    /* [in] */ Int64 period)
{
    Mutex::Autolock lock(GetSelfLock());
    mPeriod = period;
    return NOERROR;
}

ECode TimerTask::IsFixedRate(
    /* [out] */ Boolean* fixed)
{
    assert(fixed);
    Mutex::Autolock lock(GetSelfLock());
    *fixed = mFixedRate;
    return NOERROR;
}

ECode TimerTask::SetFixedRate(
    /* [in] */ Boolean fixed)
{
    Mutex::Autolock lock(GetSelfLock());
    mFixedRate = fixed;
    return NOERROR;
}
