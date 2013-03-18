
#include "CTimerTask.h"
ECode CTimerTask::Run()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTimerTask::Cancel(
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    *pValue = TimerTask::Cancel();
    return NOERROR;
}

ECode CTimerTask::ScheduledExecutionTime(
    /* [out] */ Int64 * pValue)
{
    // TODO: Add your code here
    *pValue = TimerTask::ScheduledExecutionTime();
    return NOERROR;
}

ECode CTimerTask::constructor()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

