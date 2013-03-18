#ifndef __TIMER_H__
#define __TIMER_H__

#include "CTimerTask.h"
#include "CDate.h"
#include <elastos/Mutex.h>
#include "Elastos.Core.h"
#include <elastos/AutoPtr.h>
//#include <elastos.h>
//
using namespace Elastos;
using namespace Elastos::Core;
using namespace Elastos::Core::Threading;

//class TimerImplA : public CThread
//{
//public:
//    void SetX( const String& name) { SetName(name);}
//};

class Timer
{
    class TimerImpl : public IRunnable
    {
            class TimerHeap {
            private:
                CARAPI UpHeap();

                CARAPI DownHeap(
                    /* [in] */ Int32 pos);



            private:
                Int32 DEFAULT_HEAP_SIZE;
                ArrayOf<ITimerTask*>* mTimers;
                Int32 mSize;
                Int32 mDeletedCancelledNumber;

            public:
                TimerHeap();

                ITimerTask* Minimum();

                Boolean IsEmpty();

                CARAPI Insert(
                    /* [in] */ ITimerTask* task);

                CARAPI Delete(
                    /* [in] */ Int32 pos);

                CARAPI Reset();

                CARAPI AdjustMinimum();

                CARAPI DeleteIfCancelled();

                CARAPI GetTask(
                    /* [in] */ ITimerTask* task);

        };

        public:
            Boolean mCancelled;

            Mutex mLock;

        /**
         * True if the Timer has become garbage
         */
            Boolean mFinished;

        /**
         * Vector consists of scheduled events, sorted according to
         * {@code when} field of TaskScheduled object.
         */
            TimerHeap* mTasks;

            AutoPtr<IThread> mThread;

        public:
            TimerImpl(
                /* [in] */ String name,
                /* [in] */ Boolean isDaemon);

            CARAPI_(PInterface) Probe(
                /* [in] */ REIID riid);

            CARAPI_(UInt32) AddRef();

            CARAPI_(UInt32) Release();

            CARAPI GetInterfaceID(
                /* [in] */ IInterface *pObject,
                /* [out] */ InterfaceID *pIID);

            CARAPI Run();

            CARAPI Cancel();

            CARAPI_(Int32) Purge();

            CARAPI InsertTask(
                /* [in] */ ITimerTask* newTask);

//            virtual IInterface* Probe(
//                /* [in] */ const InterfaceID & id) {};
//
//            virtual Mutex* GetSelfLock() {};
    };

    class FinalizerHelper
    {
    public:
        FinalizerHelper(
            /* [in] */ TimerImpl* impl);

    private:
        TimerImpl* mImpl;
        Mutex mLock;

    protected:
         CARAPI Finalize();
    };

private:
    static Int64 mTimerId;

    static Int64 NextId();

    /* This object will be used in synchronization purposes */
    TimerImpl* mImpl;

    Mutex mImplLock;

    // Used to finalize thread
    FinalizerHelper* mFinalizer;

    /**
     * Creates a new named {@code Timer} which may be specified to be run as a
     * daemon thread.
     *
     * @param name the name of the {@code Timer}.
     * @param isDaemon true if {@code Timer}'s thread should be a daemon thread.
     * @throws NullPointerException is {@code name} is {@code null}
     */
public:
     Timer(
        /* [in] */ String name,
        /* [in] */ Boolean isDaemon);

    /**
     * Creates a new named {@code Timer} which does not run as a daemon thread.
     *
     * @param name the name of the Timer.
     * @throws NullPointerException is {@code name} is {@code null}
     */
    Timer(
        /* [in] */ String name);

    /**
     * Creates a new {@code Timer} which may be specified to be run as a daemon thread.
     *
     * @param isDaemon {@code true} if the {@code Timer}'s thread should be a daemon thread.
     */
    Timer(
        /* [in] */ Boolean isDaemon);

    /**
     * Creates a new non-daemon {@code Timer}.
     */
    Timer();
    /**
     * Cancels the {@code Timer} and all scheduled tasks. If there is a
     * currently running task it is not affected. No more tasks may be scheduled
     * on this {@code Timer}. Subsequent calls do nothing.
     */
    CARAPI Cancel();

    /**
     * Removes all canceled tasks from the task queue. If there are no
     * other references on the tasks, then after this call they are free
     * to be garbage collected.
     *
     * @return the number of canceled tasks that were removed from the task
     *         queue.
     */
    CARAPI_(Int32) Purge();

    /**
     * Schedule a task for single execution. If {@code when} is less than the
     * current time, it will be scheduled to be executed as soon as possible.
     *
     * @param task
     *            the task to schedule.
     * @param when
     *            time of execution.
     * @throws IllegalArgumentException
     *                if {@code when.getTime() < 0}.
     * @throws IllegalStateException
     *                if the {@code Timer} has been canceled, or if the task has been
     *                scheduled or canceled.
     */
    CARAPI Schedule(
        /* [in] */ ITimerTask* task,
        /* [in] */ IDate* when);

    /**
     * Schedule a task for single execution after a specified delay.
     *
     * @param task
     *            the task to schedule.
     * @param delay
     *            amount of time in milliseconds before execution.
     * @throws IllegalArgumentException
     *                if {@code delay < 0}.
     * @throws IllegalStateException
     *                if the {@code Timer} has been canceled, or if the task has been
     *                scheduled or canceled.
     */
    CARAPI Schedule(
        /* [in] */ ITimerTask* task,
        /* [in] */ Int64 delay);

    /**
     * Schedule a task for repeated fixed-delay execution after a specific delay.
     *
     * @param task
     *            the task to schedule.
     * @param delay
     *            amount of time in milliseconds before first execution.
     * @param period
     *            amount of time in milliseconds between subsequent executions.
     * @throws IllegalArgumentException
     *                if {@code delay < 0} or {@code period < 0}.
     * @throws IllegalStateException
     *                if the {@code Timer} has been canceled, or if the task has been
     *                scheduled or canceled.
     */
    CARAPI Schedule(
        /* [in] */ ITimerTask* task,
        /* [in] */ Int64 delay,
        /* [in] */ Int64 period);

    /**
     * Schedule a task for repeated fixed-delay execution after a specific time
     * has been reached.
     *
     * @param task
     *            the task to schedule.
     * @param when
     *            time of first execution.
     * @param period
     *            amount of time in milliseconds between subsequent executions.
     * @throws IllegalArgumentException
     *                if {@code when.getTime() < 0} or {@code period < 0}.
     * @throws IllegalStateException
     *                if the {@code Timer} has been canceled, or if the task has been
     *                scheduled or canceled.
     */
    CARAPI Schedule(
        /* [in] */ ITimerTask* task,
        /* [in] */ IDate* when,
        /* [in] */ Int64 period);

    /**
     * Schedule a task for repeated fixed-rate execution after a specific delay
     * has passed.
     *
     * @param task
     *            the task to schedule.
     * @param delay
     *            amount of time in milliseconds before first execution.
     * @param period
     *            amount of time in milliseconds between subsequent executions.
     * @throws IllegalArgumentException
     *                if {@code delay < 0} or {@code period < 0}.
     * @throws IllegalStateException
     *                if the {@code Timer} has been canceled, or if the task has been
     *                scheduled or canceled.
     */
    CARAPI ScheduleAtFixedRate(
        /* [in] */ ITimerTask* task,
        /* [in] */ Int64 delay,
        /* [in] */ Int64 period);

    /**
     * Schedule a task for repeated fixed-rate execution after a specific time
     * has been reached.
     *
     * @param task
     *            the task to schedule.
     * @param when
     *            time of first execution.
     * @param period
     *            amount of time in milliseconds between subsequent executions.
     * @throws IllegalArgumentException
     *                if {@code when.getTime() < 0} or {@code period < 0}.
     * @throws IllegalStateException
     *                if the {@code Timer} has been canceled, or if the task has been
     *                scheduled or canceled.
     */
    CARAPI ScheduleAtFixedRate(
        /* [in] */ ITimerTask* task,
        /* [in] */ IDate* when,
        /* [in] */ Int64 period);

    /*
     * Schedule a task.
     */
    CARAPI ScheduleImpl(
        /* [in] */ ITimerTask* task,
        /* [in] */ Int64 delay,
        /* [in] */ Int64 period,
        /* [in] */ Boolean fixed);
};
#endif //__TIMER_H__
