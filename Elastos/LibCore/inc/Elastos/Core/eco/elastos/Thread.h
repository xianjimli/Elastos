
#ifndef __THREAD_H__
#define __THREAD_H__

//#include "NativeThread.h"
//#include "ThreadGroup.h"
#ifdef ELASTOS_CORE_ECO
#include "Elastos.Core_server.h"
#else
#include <Elastos.Core.h>
#endif
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>

using namespace Elastos;
using namespace Elastos::Core::Threading;

enum {
    /*
     * ***********************************************
     * ** Keep in sync with android.os.Process.java **
     * ***********************************************
     *
     * This maps directly to the "nice" priorites we use in Android.
     * A thread priority should be chosen inverse-proportinally to
     * the amount of work the thread is expected to do. The more work
     * a thread will do, the less favorable priority it should get so that
     * it doesn't starve the system. Threads not behaving properly might
     * be "punished" by the kernel.
     * Use the levels below when appropriate. Intermediate values are
     * acceptable, preferably use the {MORE|LESS}_FAVORABLE constants below.
     */
    ELASTOS_PRIORITY_LOWEST         =  19,

    /* use for background tasks */
    ELASTOS_PRIORITY_BACKGROUND     =  10,

    /* most threads run at normal priority */
    ELASTOS_PRIORITY_NORMAL         =   0,

    /* threads currently running a UI that the user is interacting with */
    ELASTOS_PRIORITY_FOREGROUND     =  -2,

    /* the main UI thread has a slightly more favorable priority */
    ELASTOS_PRIORITY_DISPLAY        =  -4,

    /* ui service treads might want to run at a urgent display (uncommon) */
    ELASTOS_PRIORITY_URGENT_DISPLAY =  -8,

    /* all normal audio threads */
    ELASTOS_PRIORITY_AUDIO          = -16,

    /* service audio threads (uncommon) */
    ELASTOS_PRIORITY_URGENT_AUDIO   = -19,

    /* should never be used in practice. regular process might not
     * be allowed to use this level */
    ELASTOS_PRIORITY_HIGHEST        = -20,

    ELASTOS_PRIORITY_DEFAULT        = ELASTOS_PRIORITY_NORMAL,
    ELASTOS_PRIORITY_MORE_FAVORABLE = -1,
    ELASTOS_PRIORITY_LESS_FAVORABLE = +1,
};

struct NativeThread;
class ThreadGroup;

class Thread
{
public:
    /**
     * Returns the Thread of the caller, that is, the current Thread.
     *
     * @return the current Thread.
     */
    static CARAPI_(AutoPtr<IThread>) GetCurrentThread();

    /**
     * Calls the <code>run()</code> method of the Runnable object the receiver
     * holds. If no Runnable is set, does nothing.
     *
     * @see Thread#start
     */
    virtual CARAPI Run();

    /**
     * Starts the new Thread of execution. The <code>run()</code> method of
     * the receiver will be called by the receiver Thread itself (and not the
     * Thread calling <code>start()</code>).
     *
     * @throws IllegalThreadStateException if the Thread has been started before
     *
     * @see Thread#run
     */
    //synchronized
    virtual CARAPI Start();

protected:
    Thread();

    virtual ~Thread();

    virtual CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid) = 0;

    /**
     * Constructs a new {@code Thread} with no {@code Runnable} object and a
     * newly generated name. The new {@code Thread} will belong to the same
     * {@code ThreadGroup} as the {@code Thread} calling this constructor.
     *
     * @see java.lang.ThreadGroup
     * @see java.lang.Runnable
     */
    CARAPI Init();

    /**
     * Constructs a new {@code Thread} with a {@code Runnable} object and a
     * newly generated name. The new {@code Thread} will belong to the same
     * {@code ThreadGroup} as the {@code Thread} calling this constructor.
     *
     * @param runnable
     *            a {@code Runnable} whose method <code>run</code> will be
     *            executed by the new {@code Thread}
     *
     * @see java.lang.ThreadGroup
     * @see java.lang.Runnable
     */
    CARAPI Init(
        /* [in] */ IRunnable* runnable);

    /**
     * Constructs a new {@code Thread} with a {@code Runnable} object and name
     * provided. The new {@code Thread} will belong to the same {@code
     * ThreadGroup} as the {@code Thread} calling this constructor.
     *
     * @param runnable
     *            a {@code Runnable} whose method <code>run</code> will be
     *            executed by the new {@code Thread}
     * @param threadName
     *            the name for the {@code Thread} being created
     *
     * @see java.lang.ThreadGroup
     * @see java.lang.Runnable
     */
    CARAPI Init(
        /* [in] */ IRunnable* runnable,
        /* [in] */ const String& threadName);

    /**
     * Constructs a new {@code Thread} with no {@code Runnable} object and the
     * name provided. The new {@code Thread} will belong to the same {@code
     * ThreadGroup} as the {@code Thread} calling this constructor.
     *
     * @param threadName
     *            the name for the {@code Thread} being created
     *
     * @see java.lang.ThreadGroup
     * @see java.lang.Runnable
     *
     */
    CARAPI Init(
        /* [in] */ const String& threadName);

    /**
     * Constructs a new {@code Thread} with a {@code Runnable} object and a
     * newly generated name. The new {@code Thread} will belong to the {@code
     * ThreadGroup} passed as parameter.
     *
     * @param group
     *            {@code ThreadGroup} to which the new {@code Thread} will
     *            belong
     * @param runnable
     *            a {@code Runnable} whose method <code>run</code> will be
     *            executed by the new {@code Thread}
     * @throws SecurityException
     *             if <code>group.checkAccess()</code> fails with a
     *             SecurityException
     * @throws IllegalThreadStateException
     *             if <code>group.destroy()</code> has already been done
     * @see java.lang.ThreadGroup
     * @see java.lang.Runnable
     * @see java.lang.SecurityException
     * @see java.lang.SecurityManager
     */
    CARAPI Init(
        /* [in] */ IThreadGroup* group,
        /* [in] */ IRunnable* runnable);

    /**
     * Constructs a new {@code Thread} with a {@code Runnable} object, the given
     * name and belonging to the {@code ThreadGroup} passed as parameter.
     *
     * @param group
     *            ThreadGroup to which the new {@code Thread} will belong
     * @param runnable
     *            a {@code Runnable} whose method <code>run</code> will be
     *            executed by the new {@code Thread}
     * @param threadName
     *            the name for the {@code Thread} being created
     * @throws SecurityException
     *             if <code>group.checkAccess()</code> fails with a
     *             SecurityException
     * @throws IllegalThreadStateException
     *             if <code>group.destroy()</code> has already been done
     * @see java.lang.ThreadGroup
     * @see java.lang.Runnable
     * @see java.lang.SecurityException
     * @see java.lang.SecurityManager
     */
    CARAPI Init(
        /* [in] */ IThreadGroup* group,
        /* [in] */ IRunnable* runnable,
        /* [in] */ const String& threadName);

    /**
     * Constructs a new {@code Thread} with no {@code Runnable} object, the
     * given name and belonging to the {@code ThreadGroup} passed as parameter.
     *
     * @param group
     *            {@code ThreadGroup} to which the new {@code Thread} will belong
     * @param threadName
     *            the name for the {@code Thread} being created
     * @throws SecurityException
     *             if <code>group.checkAccess()</code> fails with a
     *             SecurityException
     * @throws IllegalThreadStateException
     *             if <code>group.destroy()</code> has already been done
     * @see java.lang.ThreadGroup
     * @see java.lang.Runnable
     * @see java.lang.SecurityException
     * @see java.lang.SecurityManager
     */
    CARAPI Init(
        /* [in] */ IThreadGroup* group,
        /* [in] */ const String& threadName);

    /**
     * Constructs a new {@code Thread} with a {@code Runnable} object, the given
     * name and belonging to the {@code ThreadGroup} passed as parameter.
     *
     * @param group
     *            {@code ThreadGroup} to which the new {@code Thread} will
     *            belong
     * @param runnable
     *            a {@code Runnable} whose method <code>run</code> will be
     *            executed by the new {@code Thread}
     * @param threadName
     *            the name for the {@code Thread} being created
     * @param stackSize
     *            a stack size for the new {@code Thread}. This has a highly
     *            platform-dependent interpretation. It may even be ignored
     *            completely.
     * @throws SecurityException
     *             if <code>group.checkAccess()</code> fails with a
     *             SecurityException
     * @throws IllegalThreadStateException
     *             if <code>group.destroy()</code> has already been done
     * @see java.lang.ThreadGroup
     * @see java.lang.Runnable
     * @see java.lang.SecurityException
     * @see java.lang.SecurityManager
     */
    CARAPI Init(
        /* [in] */ IThreadGroup* group,
        /* [in] */ IRunnable* runnable,
        /* [in] */ const String& threadName,
        /* [in] */ Int64 stackSize);

    virtual CARAPI_(Mutex*) GetSelfLock() = 0;

private:
    /**
     * Initializes a new, existing Thread object with a runnable object,
     * the given name and belonging to the ThreadGroup passed as parameter.
     * This is the method that the several public constructors delegate their
     * work to.
     *
     * @param group ThreadGroup to which the new Thread will belong
     * @param runnable a java.lang.Runnable whose method <code>run</code> will
     *        be executed by the new Thread
     * @param threadName Name for the Thread being created
     * @param stackSize Platform dependent stack size
     * @throws SecurityException if <code>group.checkAccess()</code> fails
     *         with a SecurityException
     * @throws IllegalThreadStateException if <code>group.destroy()</code> has
     *         already been done
     * @see java.lang.ThreadGroup
     * @see java.lang.Runnable
     * @see java.lang.SecurityException
     * @see java.lang.SecurityManager
     */
    CARAPI Create(
        /* [in] */ IThreadGroup* group,
        /* [in] */ IRunnable* runnable,
        /* [in] */ const String& threadName,
        /* [in] */ Int64 stackSize);

    static CARAPI_(AutoPtr<IThread>) NativeGetCurrentThread();

public:
    /*volatile*/ String mName;
    /*volatile*/ Int32 mPriority;

protected:
    /* some of these are accessed directly by the VM; do not rename them */
    // volatile VMThread vmThread;
    /* volatile */ AutoPtr<ThreadGroup> mGroup;
    /*volatile*/ Boolean mDaemon;
//    /*volatile*/ String mName;
//    /*volatile*/ Int32 mPriority;
    /*volatile*/ Int64 mStackSize;
    AutoPtr<IRunnable> mTarget;

    /**
     * Normal thread local values.
     */
    // ThreadLocal.Values localValues;

    /**
     * Inheritable thread local values.
     */
    // ThreadLocal.Values inheritableValues;

    /**
     * Reflects whether this Thread has already been started. A Thread
     * can only be started once (no recycling). Also, we need it to deduce
     * the proper Thread status.
     */
    Boolean mHasBeenStarted;

private:
    static const Int32 NANOS_PER_MILLI;

    static Int32 sCount;

    /**
     * Holds the thread's ID. We simply count upwards, so
     * each Thread has a unique ID.
     */
    Int64 mId;

    /**
     * Holds the interrupt action for this Thread, if any.
     * <p>
     * This is required internally by NIO, so even if it looks like it's
     * useless, don't delete it!
     */
    AutoPtr<IRunnable> mInterruptAction;

    /**
     * Holds the class loader for this Thread, in case there is one.
     */
    // private ClassLoader contextClassLoader;

    /**
     * Holds the handler for uncaught exceptions in this Thread,
     * in case there is one.
     */
    // private UncaughtExceptionHandler uncaughtHandler;

    /**
     * Holds the default handler for uncaught exceptions, in case there is one.
     */
    // private static UncaughtExceptionHandler defaultUncaughtHandler;

    /** the park state of the thread */
    // private int parkState = ParkState.UNPARKED;

    /** The synchronization object responsible for this thread parking. */
    // private Object parkBlocker;
};

#endif //__THREAD_H__
