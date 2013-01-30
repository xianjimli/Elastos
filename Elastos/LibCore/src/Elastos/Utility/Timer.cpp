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
    Int32 current = mSize - 1;
    Int32 parent = (current - 1) / 2;

    while ((*mTimers)[current]->mWhen < (*mTimers)[parent]->mWhen) {
        // swap the two
        TimerTask* tmp = (*mTimers)[current];
        (*mTimers)[current] = (*mTimers)[parent];
        (*mTimers)[parent] = tmp;

        // update pos and current
        current = parent;
        parent = (current - 1) / 2;
    }
    return NOERROR;
}

ECode Timer::TimerImpl::TimerHeap::DownHeap(
    /* [in] */ Int32 pos)
{
    Int32 current = pos;
    Int32 child = 2 * current + 1;

    while (child < mSize && mSize > 0) {
        // compare the children if they exist
        if (child + 1 < mSize
                && (*mTimers)[child + 1]->mWhen < (*mTimers)[child]->mWhen) {
            child++;
        }

        // compare selected child with parent
        if ((*mTimers)[current]->mWhen < (*mTimers)[child]->mWhen) {
            break;
        }

        // swap the two
        TimerTask* tmp = (*mTimers)[current];
        (*mTimers)[current] = (*mTimers)[child];
        (*mTimers)[child] = tmp;

        // update pos and current
        current = child;
        child = 2 * current + 1;
    }

    return NOERROR;
}

TimerTask* Timer::TimerImpl::TimerHeap::Minimum()
{
    return (*mTimers)[0];
}

Boolean Timer::TimerImpl::TimerHeap::IsEmpty()
{
    return mSize == 0;;
}

ECode Timer::TimerImpl::TimerHeap::Insert(
    /* [in] */ TimerTask* task)
{
    if (mTimers->GetLength() == mSize) {
        ArrayOf<TimerTask*> *appendedTimers = ArrayOf<TimerTask*>::Alloc(mSize * 2);
        //System.arraycopy(timers, 0, appendedTimers, 0, size);
        Int32 count = 0;
        for(; count < mSize; count++) {
            (*appendedTimers)[count] = (*mTimers)[count];
        }
        ArrayOf<TimerTask*>::Free(mTimers);
        mTimers = appendedTimers;

    }
    (*mTimers)[mSize++] = task;
    UpHeap();
    return NOERROR;
}

ECode Timer::TimerImpl::TimerHeap::Delete(
    /* [in] */ Int32 pos)
{
    if (pos >= 0 && pos < mSize) {
        (*mTimers)[pos] = (*mTimers)[--mSize];
        (*mTimers)[mSize] = NULL;
        DownHeap(pos);
    }
    return NOERROR;
}

ECode Timer::TimerImpl::TimerHeap::Reset()
{
    ArrayOf<TimerTask*>::Free(mTimers);
    mTimers = ArrayOf<TimerTask*>::Alloc(256);;
    mSize = 0;
    return NOERROR;
}

ECode Timer::TimerImpl::TimerHeap::AdjustMinimum()
{
    return DownHeap(0);;
}


ECode Timer::TimerImpl::TimerHeap::DeleteIfCancelled()
{
    for (Int32 i = 0; i < mSize; i++) {
        if ((*mTimers)[i]->mCancelled) {
            mDeletedCancelledNumber++;
            Delete(i);
            // re-try this point
            i--;
        }
    }
    return NOERROR;
}

ECode Timer::TimerImpl::TimerHeap::GetTask(
    /* [in] */ TimerTask* task)
{
    for (Int32 i = 0; i < mTimers->GetLength(); i++) {
        if ((*mTimers)[i] == task) {
            return i;
        }
    }
    return E_FAIL;
//    return NOERROR;
}


Timer::TimerImpl::TimerImpl(
    /* [in] */ String name,
    /* [in] */ Boolean isDaemon)
{
    mThread->SetName(name);
    mThread->SetDaemon(isDaemon);
    mThread->Start();
}

ECode Timer::TimerImpl::Run()
{
//    while (TRUE) {
//        TimerTask* task;
//        Mutex::Autolock lock(&mLock);
//        {
//            // need to check cancelled inside the synchronized block
//            if (mCancelled) {
//                //return E_TIMER_CANCELLED;
//            }
//            if (mTasks->IsEmpty()) {
//                if (mFinished) {
//                    //return E_TIMER_FINISHED;;
//                }
//                // no tasks scheduled -- sleep until any task appear
//                mThread->Wait();
//                continue;
//            }
//
//            Int64 currentTime = System::GetCurrentTimeMillis();
//
//            task = mTasks->Minimum();
//            Int64 timeToSleep;
//
//            Mutex::Autolock lock(&task->mLock); {
//                if (task.cancelled) {
//                    tasks.delete(0);
//                    continue;
//                }
//
//                // check the time to sleep for the first task scheduled
//                timeToSleep = task.when - currentTime;
//            }
//
//            if (timeToSleep > 0) {
//                // sleep!
//                try {
//                    this.wait(timeToSleep);
//                } catch (InterruptedException ignored) {
//                }
//                continue;
//            }
//
//            // no sleep is necessary before launching the task
//
//            synchronized (task.lock) {
//                int pos = 0;
//                if (tasks.minimum().when != task.when) {
//                    pos = tasks.getTask(task);
//                }
//                if (task.cancelled) {
//                    tasks.delete(tasks.getTask(task));
//                    continue;
//                }
//
//                // set time to schedule
//                task.setScheduledTime(task.when);
//
//                // remove task from queue
//                tasks.delete(pos);
//
//                // set when the next task should be launched
//                if (task.period >= 0) {
//                    // this is a repeating task,
//                    if (task.fixedRate) {
//                        // task is scheduled at fixed rate
//                        task.when = task.when + task.period;
//                    } else {
//                        // task is scheduled at fixed delay
//                        task.when = System.currentTimeMillis()
//                                + task.period;
//                    }
//
//                    // insert this task into queue
//                    insertTask(task);
//                } else {
//                    task.when = 0;
//                }
//            }
//        }
//
//        boolean taskCompletedNormally = false;
//        try {
//            task.run();
//            taskCompletedNormally = true;
//        } finally {
//            if (!taskCompletedNormally) {
//                synchronized (this) {
//                    cancelled = true;
//                }
//            }
//        }
//    }
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
    mTasks->Insert(newTask);
    //mThread->Notify();
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