#include "TimerTask.h"


TimerTask::TimerTask()
{

}

Int64 TimerTask::GetWhen()
{
    Mutex::Autolock lock(&mLock);
    return mWhen;
}

void TimerTask::SetScheduledTime(
        /* [in] */ Int64 time)
{
    Mutex::Autolock lock(&mLock);
    mScheduledTime = time;
}

Boolean TimerTask::IsScheduled()
{
    Mutex::Autolock lock(&mLock);
    return mWhen > 0 || mScheduledTime > 0;
}

Boolean TimerTask::Cancel()
{
    Mutex::Autolock lock(&mLock);
    Boolean willRun = !mCancelled && mWhen > 0;
    mCancelled = TRUE;
    return willRun;
}

Int64 TimerTask::ScheduledExecutionTime()
{
    Mutex::Autolock lock(&mLock);
    return mScheduledTime;
}