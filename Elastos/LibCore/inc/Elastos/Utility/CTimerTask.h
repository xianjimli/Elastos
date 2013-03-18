
#ifndef __CTIMERTASK_H__
#define __CTIMERTASK_H__

#include "_CTimerTask.h"
#include "TimerTask.h"

using namespace Elastos;

CarClass(CTimerTask), public TimerTask
{
public:
    CARAPI Run();

    CARAPI Cancel(
        /* [out] */ Boolean * pValue);

    CARAPI ScheduledExecutionTime(
        /* [out] */ Int64 * pValue);

    CARAPI constructor();

private:
    // TODO: Add your private member variables here.
};

#endif // __CTIMERTASK_H__
