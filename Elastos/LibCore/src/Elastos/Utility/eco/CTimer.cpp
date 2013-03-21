
#include "cmdef.h"
#include "CTimer.h"


ECode CTimer::constructor(
    /* [in] */ const String& name,
    /* [in] */ Boolean isDaemon)
{
    return Timer::Init(name, isDaemon);
}

ECode CTimer::constructor(
    /* [in] */ const String& name)
{
    return Timer::Init(name);
}

ECode CTimer::constructor(
    /* [in] */ Boolean isDaemon)
{
    return Timer::Init(isDaemon);
}

ECode CTimer::constructor()
{
    return Timer::Init(FALSE);
}

ECode CTimer::Cancel()
{
    return Timer::Cancel();
}

ECode CTimer::Purge(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    return Timer::Purge(value);
}

ECode CTimer::Schedule(
    /* [in] */ ITimerTask* task,
    /* [in] */ IDate* when)
{
    return Timer::Schedule(task, when);
}

ECode CTimer::ScheduleEx(
    /* [in] */ ITimerTask* task,
    /* [in] */ Int64 delay)
{
    return Timer::Schedule(task, delay);
}

ECode CTimer::ScheduleEx2(
    /* [in] */ ITimerTask* task,
    /* [in] */ Int64 delay,
    /* [in] */ Int64 period)
{
    return Timer::Schedule(task, delay, period);
}

ECode CTimer::ScheduleEx3(
    /* [in] */ ITimerTask* task,
    /* [in] */ IDate* when,
    /* [in] */ Int64 period)
{
    return Timer::Schedule(task, when, period);
}

ECode CTimer::ScheduleAtFixedRate(
    /* [in] */ ITimerTask* task,
    /* [in] */ Int64 delay,
    /* [in] */ Int64 period)
{
    return Timer::ScheduleAtFixedRate(task, delay, period);
}

ECode CTimer::scheduleAtFixedRateEx(
    /* [in] */ ITimerTask* task,
    /* [in] */ IDate* when,
    /* [in] */ Int64 period)
{
    return Timer::ScheduleAtFixedRate(task, when, period);
}
