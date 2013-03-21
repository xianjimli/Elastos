
#include "cmdef.h"
#include "Timer.h"
#include <elastos/System.h>
#include <StringBuffer.h>
#include <elastos/Autolock.h>

#include <stdio.h>


Timer::TimerImpl::TimerHeap::TimerHeap()
    : DEFAULT_HEAP_SIZE(256)
    , mTimers(ArrayOf< AutoPtr<ITimerTask> >::Alloc(DEFAULT_HEAP_SIZE))
    , mSize(0)
    , mDeletedCancelledNumber(0)
{}

Timer::TimerImpl::TimerHeap::~TimerHeap()
{
    if (mTimers != NULL) {
        for (Int32 i = 0; i < mTimers->GetLength(); ++i) {
            (*mTimers)[i] = NULL;
        }
        ArrayOf< AutoPtr<ITimerTask> >::Free(mTimers);
    }
}

AutoPtr<ITimerTask> Timer::TimerImpl::TimerHeap::Minimum()
{
    return (*mTimers)[0];
}

Boolean Timer::TimerImpl::TimerHeap::IsEmpty()
{
    return mSize == 0;
}

void Timer::TimerImpl::TimerHeap::Insert(
    /* [in] */ ITimerTask* task)
{
    if (mTimers->GetLength() == mSize) {
        ArrayOf< AutoPtr<ITimerTask> >* appendedTimers =
                ArrayOf< AutoPtr<ITimerTask> >::Alloc(mSize * 2);
        //System.arraycopy(timers, 0, appendedTimers, 0, size);
        memcpy(appendedTimers->GetPayload(), mTimers->GetPayload(),
                mSize * sizeof(AutoPtr<ITimerTask>));
        ArrayOf< AutoPtr<ITimerTask> >::Free(mTimers);
        mTimers = appendedTimers;
    }
    (*mTimers)[mSize++] = task;
    UpHeap();
}

void Timer::TimerImpl::TimerHeap::Delete(
    /* [in] */ Int32 pos)
{
    if (pos >= 0 && pos < mSize) {
        (*mTimers)[pos] = (*mTimers)[--mSize];
        (*mTimers)[mSize] = NULL;
        DownHeap(pos);
    }
}

void Timer::TimerImpl::TimerHeap::UpHeap()
{
    Int32 current = mSize - 1;
    Int32 parent = (current - 1) / 2;

    Int64 curWhen, parWhen;
    while ((*mTimers)[current]->GetWhen(&curWhen),
            (*mTimers)[parent]->GetWhen(&parWhen),
            curWhen < parWhen) {
        // swap the two
        AutoPtr<ITimerTask> tmp = (*mTimers)[current];
        (*mTimers)[current] = (*mTimers)[parent];
        (*mTimers)[parent] = tmp;

        // update pos and current
        current = parent;
        parent = (current - 1) / 2;
    }
}

void Timer::TimerImpl::TimerHeap::DownHeap(
    /* [in] */ Int32 pos)
{
    Int32 current = pos;
    Int32 child = 2 * current + 1;

    Int64 when1, when2;
    while (child < mSize && mSize > 0) {
        // compare the children if they exist
        if (child + 1 < mSize
                && ((*mTimers)[child + 1]->GetWhen(&when1),
                    (*mTimers)[child]->GetWhen(&when2),
                    when1 < when2)) {
            child++;
        }

        // compare selected child with parent
        if ((*mTimers)[current]->GetWhen(&when1),
            (*mTimers)[child]->GetWhen(&when2),
            when1 < when2) {
            break;
        }

        // swap the two
        AutoPtr<ITimerTask> tmp = (*mTimers)[current];
        (*mTimers)[current] = (*mTimers)[child];
        (*mTimers)[child] = tmp;

        // update pos and current
        current = child;
        child = 2 * current + 1;
    }
}

void Timer::TimerImpl::TimerHeap::Reset()
{
    if (mTimers != NULL) {
        for (Int32 i = 0; i < mTimers->GetLength(); ++i) {
            (*mTimers)[i] = NULL;
        }
        ArrayOf< AutoPtr<ITimerTask> >::Free(mTimers);
    }
    mTimers = ArrayOf< AutoPtr<ITimerTask> >::Alloc(DEFAULT_HEAP_SIZE);;
    mSize = 0;
}

void Timer::TimerImpl::TimerHeap::AdjustMinimum()
{
    DownHeap(0);
}

void Timer::TimerImpl::TimerHeap::DeleteIfCancelled()
{
    for (Int32 i = 0; i < mSize; i++) {
        Boolean cancelled;
        if ((*mTimers)[i]->IsCancelled(&cancelled), cancelled) {
            mDeletedCancelledNumber++;
            Delete(i);
            // re-try this point
            i--;
        }
    }
}

ECode Timer::TimerImpl::TimerHeap::GetTask(
    /* [in] */ ITimerTask* task)
{
    for (Int32 i = 0; i < mTimers->GetLength(); i++) {
        if ((*mTimers)[i].Get() == task) {
            return i;
        }
    }
    return -1;
}


Timer::TimerImpl::TimerImpl(
    /* [in] */ String name,
    /* [in] */ Boolean isDaemon)
{
    mTasks = new TimerHeap();
    CThread::New((IRunnable*)this, (IThread**)&mThread);
    mThread->SetName(name);
    mThread->SetDaemon(isDaemon);
    mThread->Start();
}

Timer::TimerImpl::~TimerImpl()
{
    delete mTasks;
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
    return ElRefBase::AddRef();
}

UInt32 Timer::TimerImpl::Release()
{
    return ElRefBase::Release();
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
        AutoPtr<ITimerTask> task;
        {
            Autolock lock(mThread);
            // need to check cancelled inside the synchronized block
            if (mCancelled) {
                return NOERROR;
            }
            if (mTasks->IsEmpty()) {
                if (mFinished) {
                    return NOERROR;
                }
                // no tasks scheduled -- sleep until any task appear
                // try {
                mThread->Wait();
                // } catch (InterruptedException ignored) {
                // }
                continue;
            }

            Int64 currentTime = System::GetCurrentTimeMillis();

            task = mTasks->Minimum();
            Int64 timeToSleep;

            task->Lock();
            {
                Boolean cancelled;
                if (task->IsCancelled(&cancelled), cancelled) {
                    mTasks->Delete(0);
                    task->Unlock();
                    continue;
                }

                // check the time to sleep for the first task scheduled
                Int64 when;
                task->GetWhen(&when);
                timeToSleep = when - currentTime;
            }
            task->Unlock();

            if (timeToSleep > 0) {
                // sleep!
                // try {
                mThread->WaitEx(timeToSleep);
                // } catch (InterruptedException ignored) {
                // }
                continue;
            }

            // no sleep is necessary before launching the task

            //synchronized (task.lock)
            task->Lock();
            {
                Int32 pos = 0;
                Int64 when1, when2;
                mTasks->Minimum()->GetWhen(&when1);
                task->GetWhen(&when2);
                if (when1 != when2) {
                    pos = mTasks->GetTask(task);
                }
                Boolean cancelled;
                task->IsCancelled(&cancelled);
                if (cancelled) {
                    mTasks->Delete(mTasks->GetTask(task));
                    task->Unlock();
                    continue;
                }

                // set time to schedule
                task->SetScheduledTime(when2);

                // remove task from queue
                mTasks->Delete(pos);

                // set when the next task should be launched
                Int64 period;
                if (task->GetPeriod(&period), period >= 0) {
                    // this is a repeating task,
                    Boolean fixed;
                    if (task->IsFixedRate(&fixed), fixed) {
                        // task is scheduled at fixed rate
                        task->SetWhen(when2 + period);
                    }
                    else {
                        // task is scheduled at fixed delay
                        task->SetWhen(System::GetCurrentTimeMillis() + period);
                    }

                    // insert this task into queue
                    InsertTask(task);
                }
                else {
                    task->SetWhen(0);
                }
            }
            task->Unlock();
        }

        Boolean taskCompletedNormally = FALSE;
        if (SUCCEEDED(task->Run())) {
            taskCompletedNormally = TRUE;
        }
        if (!taskCompletedNormally) {
            Autolock lock(mThread);
            mCancelled = TRUE;
        }
    }
    return NOERROR;
}

void Timer::TimerImpl::InsertTask(
    /* [in] */ ITimerTask* newTask)
{
    mTasks->Insert(newTask);
    mThread->Notify();
}

void Timer::TimerImpl::Cancel()
{
    Autolock lock(mThread);
    mCancelled = TRUE;
    mTasks->Reset();
    mThread->Notify();
}

Int32 Timer::TimerImpl::Purge()
{
    if (mTasks->IsEmpty()) {
        return 0;
    }
    // callers are synchronized
    mTasks->mDeletedCancelledNumber = 0;
    mTasks->DeleteIfCancelled();
    return mTasks->mDeletedCancelledNumber;
}


Timer::FinalizerHelper::FinalizerHelper(
    /* [in] */ TimerImpl* impl)
    : mImpl(impl)
{}

Timer::FinalizerHelper::~FinalizerHelper()
{
    Autolock lock(mImpl->mThread);
    mImpl->mFinished = TRUE;
    mImpl->mThread->Notify();
}


Int64 Timer::sTimerId = 0;
Mutex Timer::sTimerIdLock;

Int64 Timer::NextId()
{
    Mutex::Autolock lock(&sTimerIdLock);
    return sTimerId++;
}

Timer::Timer()
    : mFinalizer(0)
{}

Timer::Timer(
    /* [in] */ String name,
    /* [in] */ Boolean isDaemon)
{
    ASSERT_SUCCEEDED(Init(name, isDaemon));
}

Timer::Timer(
    /* [in] */ String name)
{
    ASSERT_SUCCEEDED(Init(name, FALSE));
}

Timer::Timer(
    /* [in] */Boolean isDaemon)
{
    ASSERT_SUCCEEDED(Init(isDaemon));
}

Timer::~Timer()
{
    if (mFinalizer != NULL) {
        delete mFinalizer;
    }
}

ECode Timer::Init(
    /* [in] */ String name,
    /* [in] */ Boolean isDaemon)
{
    if (name.IsNull()){
        // throw new NullPointerException("name is null");
        assert(0);
    }
    mImpl = new TimerImpl(name, isDaemon);
    mFinalizer = new FinalizerHelper(mImpl);
    return NOERROR;
}

ECode Timer::Init(
    /* [in] */Boolean isDaemon)
{
    StringBuffer str;
    str += "Timer-";
    str += Timer::NextId();
    return Init(String(str), isDaemon);
}

ECode Timer::Init()
{
    return Init(FALSE);
}

ECode Timer::Cancel()
{
    mImpl->Cancel();
    return NOERROR;
}

ECode Timer::Purge(
    /* [out] */ Int32* number)
{
    assert(number != NULL);
    Autolock lock(mImpl->mThread);
    *number = mImpl->Purge();
    return NOERROR;
}

ECode Timer::Schedule(
    /* [in] */ ITimerTask* task,
    /* [in] */ IDate* when)
{
    Int64 time;
    when->GetTime(&time);
    if (time < 0) {
        // throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int64 delay = time - System::GetCurrentTimeMillis();
    return ScheduleImpl(task, delay < 0 ? 0 : delay, -1, FALSE);
}

ECode Timer::Schedule(
    /* [in] */ ITimerTask* task,
    /* [in] */ Int64 delay)
{
    if (delay < 0) {
        // throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return ScheduleImpl(task, delay, -1, FALSE);
}

ECode Timer::Schedule(
    /* [in] */ ITimerTask* task,
    /* [in] */ Int64 delay,
    /* [in] */ Int64 period)
{
    if (delay < 0 || period <= 0) {
        // throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return ScheduleImpl(task, delay, period, FALSE);
}

ECode Timer::Schedule(
        /* [in] */ ITimerTask* task,
        /* [in] */ IDate* when,
        /* [in] */ Int64 period)
{
    Int64 time;
    when->GetTime(&time);
    if (period <= 0 || time < 0) {
        // throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int64 delay = time - System::GetCurrentTimeMillis();
    return ScheduleImpl(task, delay < 0 ? 0 : delay, period, FALSE);
}

ECode Timer::ScheduleAtFixedRate(
    /* [in] */ ITimerTask* task,
    /* [in] */ Int64 delay,
    /* [in] */ Int64 period)
{
    if (delay < 0 || period <= 0) {
        // throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return ScheduleImpl(task, delay, period, TRUE);
}

ECode Timer::ScheduleAtFixedRate(
    /* [in] */ ITimerTask* task,
    /* [in] */ IDate* when,
    /* [in] */ Int64 period)
{
    Int64 time;
    when->GetTime(&time);
    if (period <= 0 || time < 0) {
        // throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int64 delay = time - System::GetCurrentTimeMillis();
    return ScheduleImpl(task, delay, period, TRUE);
}

ECode Timer::ScheduleImpl(
    /* [in] */ ITimerTask* task,
    /* [in] */ Int64 delay,
    /* [in] */ Int64 period,
    /* [in] */ Boolean fixed)
{
    Autolock lock(mImpl->mThread);

    if (mImpl->mCancelled) {
        // throw new IllegalStateException("Timer was canceled");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    Int64 when = delay + System::GetCurrentTimeMillis();

    if (when < 0) {
        // throw new IllegalArgumentException("Illegal delay to start the TimerTask: " + when);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    task->Lock();
    {
        Boolean scheduled;
        if (task->IsScheduled(&scheduled), scheduled) {
            // throw new IllegalStateException("TimerTask is scheduled already");
            task->Unlock();
            return E_ILLEGAL_STATE_EXCEPTION;
        }

        Boolean cancelled;
        if (task->IsCancelled(&cancelled), cancelled) {
            // throw new IllegalStateException("TimerTask is canceled");
            task->Unlock();
            return E_ILLEGAL_STATE_EXCEPTION;
        }

        task->SetWhen(when);
        task->SetPeriod(period);
        task->SetFixedRate(fixed);
    }
    task->Unlock();

    // insert the newTask into queue
    mImpl->InsertTask(task);
    return NOERROR;
}
