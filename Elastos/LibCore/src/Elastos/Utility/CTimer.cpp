
#include "CTimer.h"
ECode CTimer::Cancel()
{
    // TODO: Add your code here
    return m_Timer->Cancel();
}

ECode CTimer::Purge(
    /* [out] */ Int32 * pValue)
{
    // TODO: Add your code here
    *pValue = m_Timer->Purge();
    return NOERROR;
}

ECode CTimer::Schedule(
    /* [in] */ ITimerTask * pTask,
    /* [in] */ IDate * pWhen)
{
    // TODO: Add your code here
    return m_Timer->Schedule(pTask, pWhen);
}

ECode CTimer::ScheduleEx(
    /* [in] */ ITimerTask * pTask,
    /* [in] */ Int64 delay)
{
    // TODO: Add your code here
    return m_Timer->Schedule(pTask, delay);
}

ECode CTimer::ScheduleEx1(
    /* [in] */ ITimerTask * pTask,
    /* [in] */ Int64 delay,
    /* [in] */ Int64 period)
{
    // TODO: Add your code here
    return m_Timer->Schedule(pTask, delay, period);
}

ECode CTimer::ScheduleEx2(
    /* [in] */ ITimerTask * pTask,
    /* [in] */ IDate * pWhen,
    /* [in] */ Int64 period)
{
    // TODO: Add your code here
    return m_Timer->Schedule(pTask, pWhen, period);
}

ECode CTimer::ScheduleAtFixedRate(
    /* [in] */ ITimerTask * pTask,
    /* [in] */ Int64 delay,
    /* [in] */ Int64 period)
{
    // TODO: Add your code here
    return m_Timer->ScheduleAtFixedRate(pTask, delay, period);
}

ECode CTimer::scheduleAtFixedRateEx(
    /* [in] */ ITimerTask * pTask,
    /* [in] */ IDate * pWhen,
    /* [in] */ Int64 period)
{
    // TODO: Add your code here
    return m_Timer->ScheduleAtFixedRate(pTask, pWhen, period);
}

ECode CTimer::constructor(
    /* [in] */ const String& name,
    /* [in] */ Boolean isDaemon)
{
    // TODO: Add your code here
    m_Timer = new Timer(name, isDaemon);
}

ECode CTimer::constructor(
    /* [in] */ const String& name)
{
    // TODO: Add your code here
    m_Timer = new Timer(name);
}

ECode CTimer::constructor(
    /* [in] */ Boolean isDaemon)
{
    // TODO: Add your code here
    m_Timer = new Timer(isDaemon);
}

ECode CTimer::constructor()
{
    // TODO: Add your code here
    m_Timer = new Timer();
}

