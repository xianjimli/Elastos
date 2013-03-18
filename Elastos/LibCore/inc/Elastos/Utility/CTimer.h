
#ifndef __CTIMER_H__
#define __CTIMER_H__

#include "_CTimer.h"
#include "Timer.h"

CarClass(CTimer), public Timer
{
public:
    CARAPI Cancel();

    CARAPI Purge(
        /* [out] */ Int32 * pValue);

    CARAPI Schedule(
        /* [in] */ ITimerTask * pTask,
        /* [in] */ IDate * pWhen);

    CARAPI ScheduleEx(
        /* [in] */ ITimerTask * pTask,
        /* [in] */ Int64 delay);

    CARAPI ScheduleEx1(
        /* [in] */ ITimerTask * pTask,
        /* [in] */ Int64 delay,
        /* [in] */ Int64 period);

    CARAPI ScheduleEx2(
        /* [in] */ ITimerTask * pTask,
        /* [in] */ IDate * pWhen,
        /* [in] */ Int64 period);

    CARAPI ScheduleAtFixedRate(
        /* [in] */ ITimerTask * pTask,
        /* [in] */ Int64 delay,
        /* [in] */ Int64 period);

    CARAPI scheduleAtFixedRateEx(
        /* [in] */ ITimerTask * pTask,
        /* [in] */ IDate * pWhen,
        /* [in] */ Int64 period);

    CARAPI constructor(
        /* [in] */ const String& name,
        /* [in] */ Boolean isDaemon);

    CARAPI constructor(
        /* [in] */ const String& name);

    CARAPI constructor(
        /* [in] */ Boolean isDaemon);

    CARAPI constructor();

private:
    Timer* m_Timer;
    // TODO: Add your private member variables here.
};

#endif // __CTIMER_H__
