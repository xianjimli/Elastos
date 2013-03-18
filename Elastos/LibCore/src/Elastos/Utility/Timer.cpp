#include "Timer.h"
#include <elastos/System.h>
#include <StringBuffer.h>
#include <elastos/Autolock.h>


#define E_TIMER_SCHEDULED_ALREADY 0x80160001

Timer::TimerImpl::TimerHeap::TimerHeap()
{
    mTimers = ArrayOf<ITimerTask*>::Alloc(256);
//    TimerTask a;
}

ECode Timer::TimerImpl::TimerHeap::UpHeap()
{
    Int32 current = mSize - 1;
    Int32 parent = (current - 1) / 2;

    while (((CTimerTask *)(*mTimers)[current])->mWhen < ((CTimerTask *)(*mTimers)[parent])->mWhen) {
        // swap the two
        ITimerTask* tmp = (*mTimers)[current];
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
                && ((CTimerTask *)(*mTimers)[child + 1])->mWhen < ((CTimerTask *)(*mTimers)[child])->mWhen) {
            child++;
        }

        // compare selected child with parent
        if (((CTimerTask *)(*mTimers)[current])->mWhen < ((CTimerTask *)(*mTimers)[child])->mWhen) {
            break;
        }

        // swap the two
        ITimerTask* tmp = (*mTimers)[current];
        (*mTimers)[current] = (*mTimers)[child];
        (*mTimers)[child] = tmp;

        // update pos and current
        current = child;
        child = 2 * current + 1;
    }

    return NOERROR;
}

ITimerTask* Timer::TimerImpl::TimerHeap::Minimum()
{
    return (*mTimers)[0];
}

Boolean Timer::TimerImpl::TimerHeap::IsEmpty()
{
    return mSize == 0;;
}

ECode Timer::TimerImpl::TimerHeap::Insert(
    /* [in] */ ITimerTask* task)
{
    if (mTimers->GetLength() == mSize) {
        ArrayOf<ITimerTask*> *appendedTimers = ArrayOf<ITimerTask*>::Alloc(mSize * 2);
        //System.arraycopy(timers, 0, appendedTimers, 0, size);
        Int32 count = 0;
        for(; count < mSize; count++) {
            (*appendedTimers)[count] = (*mTimers)[count];
        }
        ArrayOf<ITimerTask*>::Free(mTimers);
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
    ArrayOf<ITimerTask*>::Free(mTimers);
    mTimers = ArrayOf<ITimerTask*>::Alloc(256);;
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
        if (((CTimerTask *)(*mTimers)[i])->mCancelled) {
            mDeletedCancelledNumber++;
            Delete(i);
            // re-try this point
            i--;
        }
    }
    return NOERROR;
}

ECode Timer::TimerImpl::TimerHeap::GetTask(
    /* [in] */ ITimerTask* task)
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
//    CThread::New((IRunnable*)this, (IThread**)&mThread);
    mThread->SetName(name);
    mThread->SetDaemon(isDaemon);
    mThread->Start();
}

PInterface Timer::TimerImpl::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IRunnable) {
        return (IRunnable*)this;
    }

    return NULL;
}

UInt32 Timer::TimerImpl::AddRef()
{
//    return ElRefBase::AddRef();
}

UInt32 Timer::TimerImpl::Release()
{
//    return ElRefBase::Release();
}

ECode Timer::TimerImpl::GetInterfaceID(
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

ECode Timer::TimerImpl::Run()
{
    while (TRUE) {
        ITimerTask* task;
        Autolock lock(mThread);
        {
            // need to check cancelled inside the synchronized block
            if (mCancelled) {
                //return E_TIMER_CANCELLED;
            }
            if (mTasks->IsEmpty()) {
                if (mFinished) {
                    //return E_TIMER_FINISHED;;
                }
                // no tasks scheduled -- sleep until any task appear
                mThread->WaitEx2(0, 0);
                continue;
            }

            Int64 currentTime = System::GetCurrentTimeMillis();

            task = mTasks->Minimum();
            Int64 timeToSleep;

            Mutex::Autolock lock(&((CTimerTask *)task)->mLock);
            {
                if (((CTimerTask *)task)->mCancelled) {
                    mTasks->Delete(0);
                    continue;
                }

                // check the time to sleep for the first task scheduled
                timeToSleep = ((CTimerTask *)task)->mWhen - currentTime;
            }

            if (timeToSleep > 0) {
                // sleep!
                mThread->WaitEx2(timeToSleep, 0);
                continue;
            }

            // no sleep is necessary before launching the task

            //synchronized (task.lock)
            {
                Int32 pos = 0;
                if (((CTimerTask *)mTasks->Minimum())->mWhen != ((CTimerTask *)task)->mWhen) {
                    pos = mTasks->GetTask(task);
                }
                if (((CTimerTask *)task)->mCancelled) {
                    mTasks->Delete(mTasks->GetTask(task));
                    continue;
                }

                // set time to schedule
                ((CTimerTask *)task)->SetScheduledTime(((CTimerTask *)task)->mWhen);

                // remove task from queue
                mTasks->Delete(pos);

                // set when the next task should be launched
                if (((CTimerTask *)task)->mPeriod >= 0) {
                    // this is a repeating task,
                    if (((CTimerTask *)task)->mFixedRate) {
                        // task is scheduled at fixed rate
                        ((CTimerTask *)task)->mWhen = ((CTimerTask *)task)->mWhen + ((CTimerTask *)task)->mPeriod;
                    } else {
                        // task is scheduled at fixed delay
                        ((CTimerTask *)task)->mWhen = System::GetCurrentTimeMillis()
                                + ((CTimerTask *)task)->mPeriod;
                    }

                    // insert this task into queue
                    InsertTask(task);
                } else {
                    ((CTimerTask *)task)->mWhen = 0;
                }
            }
        }

        Boolean taskCompletedNormally = FALSE;
        ECode ec = task->Run();
        if (ec == NOERROR) {
            taskCompletedNormally = TRUE;
        }
        if (!taskCompletedNormally) {
            //synchronized (this) {
                mCancelled = TRUE;
            //}
        }
    }
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
    /* [in] */ ITimerTask* newTask)
{
    mTasks->Insert(newTask);
    mThread->Notify();
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
    /* [in] */ ITimerTask* task,
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
    /* [in] */ ITimerTask* task,
    /* [in] */ Int64 delay)
{
    if (delay < 0) {
        return E_INVALID_ARGUMENT;
    }
    return ScheduleImpl(task, delay, -1, FALSE);
}


ECode Timer::Schedule(
    /* [in] */ ITimerTask* task,
    /* [in] */ Int64 delay,
    /* [in] */ Int64 period)
{
    if (delay < 0 || period <= 0) {
        return E_INVALID_ARGUMENT;
    }
    return ScheduleImpl(task, delay, period, FALSE);
}

ECode Timer::Schedule(
        /* [in] */ ITimerTask* task,
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
    /* [in] */ ITimerTask* task,
    /* [in] */ Int64 delay,
    /* [in] */ Int64 period)
{
    if (delay < 0 || period <= 0) {
        return E_INVALID_ARGUMENT;
    }
    return ScheduleImpl(task, delay, period, TRUE);
}

ECode Timer::ScheduleAtFixedRate(
    /* [in] */ ITimerTask* task,
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
    /* [in] */ ITimerTask* task,
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

    Mutex::Autolock lock1(&((CTimerTask *)task)->mLock);
    {
        if (((CTimerTask *)task)->IsScheduled()) {
            //return E_TIMER_SCHEDULED_ALREADY;
        }

        if (((CTimerTask *)task)->mCancelled) {
            //return E_TIMER_CANCEL;
        }

        ((CTimerTask *)task)->mWhen = when;
        ((CTimerTask *)task)->mPeriod = period;
        ((CTimerTask *)task)->mFixedRate = fixed;
    }

    // insert the newTask into queue
    mImpl->InsertTask(task);
    return NOERROR;
}
