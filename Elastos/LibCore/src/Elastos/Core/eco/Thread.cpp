
#include "cmdef.h"
#include "Thread.h"
#include "NativeThread.h"
#include "ThreadGroup.h"
#include <elastos/Math.h>
#include <elastos/System.h>
#include "stdio.h"

// {069757A0-1B05-4c16-BA3E-3848DAC80DA8}
extern "C" const InterfaceID EIID_Thread =
        { 0x69757a0, 0x1b05, 0x4c16, { 0xba, 0x3e, 0x38, 0x48, 0xda, 0xc8, 0xd, 0xa8 } };

const Int32 Thread::NANOS_PER_MILLI = 1000000;

const ThreadState Thread::STATE_MAP[] = {
    ThreadState_TERMINATED,     // ZOMBIE
    ThreadState_RUNNABLE,       // RUNNING
    ThreadState_TIMED_WAITING,  // TIMED_WAIT
    ThreadState_BLOCKED,        // MONITOR
    ThreadState_WAITING,        // WAIT
    ThreadState_NEW,            // INITIALIZING
    ThreadState_NEW,            // STARTING
    ThreadState_RUNNABLE,       // NATIVE
    ThreadState_WAITING,        // VMWAIT
    ThreadState_RUNNABLE        // SUSPENDED
};

Thread::Thread()
    : mDaemon(FALSE)
    , mHasBeenStarted(FALSE)
{}

Thread::~Thread()
{}

/**
 * Constructs a new {@code Thread} with no {@code Runnable} object and a
 * newly generated name. The new {@code Thread} will belong to the same
 * {@code ThreadGroup} as the {@code Thread} calling this constructor.
 *
 * @see java.lang.ThreadGroup
 * @see java.lang.Runnable
 */
ECode Thread::Init()
{
    return Create(NULL, NULL, String(NULL), 0);
}

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
ECode Thread::Init(
    /* [in] */ IRunnable* runnable)
{
    return Create(NULL, runnable, String(NULL), 0);
}

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
ECode Thread::Init(
    /* [in] */ IRunnable* runnable,
    /* [in] */ const String& threadName)
{
    if (threadName.IsNull()) {
        // throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }

    return Create(NULL, runnable, threadName, 0);
}

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
ECode Thread::Init(
    /* [in] */ const String& threadName)
{
    if (threadName.IsNull()) {
        // throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }

    return Create(NULL, NULL, threadName, 0);
}

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
ECode Thread::Init(
    /* [in] */ IThreadGroup* group,
    /* [in] */ IRunnable* runnable)
{
    return Create(group, runnable, String(NULL), 0);
}

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
ECode Thread::Init(
    /* [in] */ IThreadGroup* group,
    /* [in] */ IRunnable* runnable,
    /* [in] */ const String& threadName)
{
    if (threadName.IsNull()) {
        // throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }

    return Create(group, runnable, threadName, 0);
}

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
ECode Thread::Init(
    /* [in] */ IThreadGroup* group,
    /* [in] */ const String& threadName)
{
    if (threadName.IsNull()) {
        // throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }

    return Create(group, NULL, threadName, 0);
}

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
ECode Thread::Init(
    /* [in] */ IThreadGroup* group,
    /* [in] */ IRunnable* runnable,
    /* [in] */ const String& threadName,
    /* [in] */ Int64 stackSize)
{
    if (threadName.IsNull()) {
        // throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    return Create(group, runnable, threadName, stackSize);
}

/**
 * Package-scope method invoked by Dalvik VM to create "internal"
 * threads or attach threads created externally.
 *
 * Don't call Thread.currentThread(), since there may not be such
 * a thing (e.g. for Main).
 */
ECode Thread::Init(
    /* [in] */ IThreadGroup* group,
    /* [in] */ const String& name,
    /* [in] */ Int32 priority,
    /* [in] */ Boolean daemon)
{
    // synchronized (Thread.class) {
    //     id = ++Thread.count;
    // }

    // if (name == null) {
    //     this.name = "Thread-" + id;
    // } else
    //     this.name = name;

    // if (group == null) {
    //     throw new InternalError("group not specified");
    // }

    // this.group = group;

    // this.target = null;
    // this.stackSize = 0;
    // this.priority = priority;
    // this.daemon = daemon;

    // /* add ourselves to our ThreadGroup of choice */
    // this.group.addThread(this);
    return NOERROR;
}

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
ECode Thread::Create(
    /* [in] */ IThreadGroup* group,
    /* [in] */ IRunnable* runnable,
    /* [in] */ const String& threadName,
    /* [in] */ Int64 stackSize)
{
    // SecurityManager smgr = System.getSecurityManager();
    // if (smgr != null) {
    //     if (group == null) {
    //         group = smgr.getThreadGroup();
    //     }

    //     /*
    //      * Freaky security requirement: If the Thread's class is actually
    //      * a subclass of Thread and it tries to override either
    //      * getContextClassLoader() or setContextClassLoader(), the
    //      * SecurityManager has to allow this.
    //      */
    //     if (getClass() != Thread.class) {
    //         Class[] signature = new Class[] { ClassLoader.class };

    //         try {
    //             getClass().getDeclaredMethod("getContextClassLoader", signature);
    //             smgr.checkPermission(new RuntimePermission("enableContextClassLoaderOverride"));
    //         } catch (NoSuchMethodException ex) {
    //             // Ignore. Just interested in the method's existence.
    //         }

    //         try {
    //             getClass().getDeclaredMethod("setContextClassLoader", signature);
    //             smgr.checkPermission(new RuntimePermission("enableContextClassLoaderOverride"));
    //         } catch (NoSuchMethodException ex) {
    //             // Ignore. Just interested in the method's existence.
    //         }
    //     }
    // }

    // AutoPtr<IThread> currentThread = Thread::GetCurrentThread();
    // if (group == NULL) {
    //     currentThread->GetThreadGroup(&group);
    // }

    // FAIL_RETURN(group->CheckAccess());
    // Boolean isDestroyed;
    // if (group->IsDestroyed(&isDestroyed), isDestroyed) {
    //     // throw new IllegalThreadStateException("Group already destroyed");
    //     group->Release();
    //     return E_ILLEGAL_THREAD_STATE_EXCEPTION;
    // }
    //mGroup = (ThreadGroup*)group->Probe(EIID_ThreadGroup);

    mId = NativeGetCount();

    if (threadName.IsNull()) {
        mName = String("Thread-" + mId);
    }
    else {
        mName = threadName;
    }

    mTarget = runnable;
    mStackSize = stackSize;

    //currentThread->GetPriority(&mPriority);
    // this.contextClassLoader = currentThread.contextClassLoader;

    // Transfer over InheritableThreadLocals.
    // if (currentThread.inheritableValues != null) {
    //     inheritableValues
    //             = new ThreadLocal.Values(currentThread.inheritableValues);
    // }

    // store current AccessControlContext as inherited context for this thread
    // SecurityUtils.putContext(this, AccessController.getContext());

    // add ourselves to our ThreadGroup of choice
    //mGroup->AddThread((IThread*)this->Probe(EIID_IThread));
    //group->Release();
    return NOERROR;
}

Int32 Thread::GetActiveCount()
{
    AutoPtr<IThreadGroup> group;
    GetCurrentThread()->GetThreadGroup((IThreadGroup**)&group);
    Int32 num;
    group->ActiveCount(&num);
    return num;
}

ECode Thread::CheckAccess()
{
    // Forwards the message to the SecurityManager (if there's one) passing
    // the receiver as parameter

    // SecurityManager currentManager = System.getSecurityManager();
    // if (currentManager != null) {
    //     currentManager.checkAccess(this);
    // }
    return NOERROR;
}

/**
 * Returns the number of stack frames in this thread.
 *
 * @return Number of stack frames
 * @deprecated The results of this call were never well defined. To make
 *             things worse, it would depend on whether the Thread was
 *             suspended or not, and suspend was deprecated too.
 */
//@Deprecated
ECode Thread::CountStackFrames(
    /* [out] */ Int32* number)
{
//    VALIDATE_NOT_NULL(number);
//    return getStackTrace().length;

    return E_NOT_IMPLEMENTED;
}

/**
 * Returns the Thread of the caller, that is, the current Thread.
 *
 * @return the current Thread.
 */
AutoPtr<IThread> Thread::GetCurrentThread()
{
    return NativeGetCurrentThread();
}

AutoPtr<IThread> Thread::NativeGetCurrentThread()
{
    NativeThread* self = NativeThreadSelf();
    assert(self != NULL);
    return (IThread*)self->mThreadObj->Probe(EIID_IThread);
}

/**
 * Destroys the receiver without any monitor cleanup.
 *
 * @deprecated Not implemented.
 */
//@Deprecated
ECode Thread::Destroy()
{
    return E_NO_SUCH_METHOD_EXCEPTION;
//    throw new NoSuchMethodError("Thread.destroy()"); // TODO Externalize???
}

void Thread::DumpStack()
{}

/**
 * Copies an array with all Threads which are in the same ThreadGroup as the
 * receiver - and subgroups - into the array <code>threads</code> passed as
 * parameter. If the array passed as parameter is too small no exception is
 * thrown - the extra elements are simply not copied.
 *
 * @param threads
 *            array into which the Threads will be copied
 * @return How many Threads were copied over
 * @throws SecurityException
 *             if the installed SecurityManager fails
 *             {@link SecurityManager#checkAccess(Thread)}
 * @see java.lang.SecurityException
 * @see java.lang.SecurityManager
 */
ECode Thread::Enumerate(
    /* [out] */ ArrayOf<IThread*>* threads,
    /* [out] */ Int32* number)
{
    AutoPtr<IThread> currentThread = Thread::GetCurrentThread();
    FAIL_RETURN(currentThread->CheckAccess());
    AutoPtr<IThreadGroup> group;
    currentThread->GetThreadGroup((IThreadGroup**)&group);

    return group->EnumerateThread(threads, number);
}

/**
 * <p>
 * Returns the stack traces of all the currently live threads and puts them
 * into the given map.
 * </p>
 *
 * @return A Map of current Threads to StackTraceElement arrays.
 * @throws SecurityException
 *             if the current SecurityManager fails the
 *             {@link SecurityManager#checkPermission(java.security.Permission)}
 *             call.
 */
//    static CARAPI_(HashMap<AutoPtr<IThread>, StackTraceElement[]>) GetAllStackTraces();

/**
 * Returns the context ClassLoader for this Thread.
 * <p>
 * If the conditions
 * <ol>
 * <li>there is a security manager
 * <li>the caller's class loader is not null
 * <li>the caller's class loader is not the same as the requested
 * context class loader and not an ancestor thereof
 * </ol>
 * are satisfied, a security check for
 * <code>RuntimePermission("getClassLoader")</code> is performed first.
 *
 * @return ClassLoader The context ClassLoader
 * @see java.lang.ClassLoader
 * @see #getContextClassLoader()
 *
 * @throws SecurityException
 *             if the aforementioned security check fails.
 */
//        public ClassLoader getContextClassLoader()


/**
 * Returns the default exception handler that's executed when uncaught
 * exception terminates a thread.
 *
 * @return an {@link UncaughtExceptionHandler} or <code>null</code> if
 *         none exists.
 */
//    static UncaughtExceptionHandler getDefaultUncaughtExceptionHandler();

/**
 * Returns the thread's identifier. The ID is a positive <code>long</code>
 * generated on thread creation, is unique to the thread, and doesn't change
 * during the lifetime of the thread; the ID may be reused after the thread
 * has been terminated.
 *
 * @return the thread's ID.
 */
ECode Thread::GetId(
    /* [out] */ Int64* id)
{
    VALIDATE_NOT_NULL(id);
    *id = mId;
    return NOERROR;
}

/**
 * Returns the name of the Thread.
 *
 * @return the Thread's name
 */
ECode Thread::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mName;
    return NOERROR;
}

/**
 * Returns the priority of the Thread.
 *
 * @return the Thread's priority
 * @see Thread#setPriority
 */
ECode Thread::GetPriority(
    /* [out] */ Int32* priority)
{
    VALIDATE_NOT_NULL(priority);
    *priority = mPriority;
    return NOERROR;
}

/**
 * Returns the a stack trace representing the current execution state of
 * this Thread.
 * <p>
 * The <code>RuntimePermission("getStackTrace")</code> is checked before
 * returning a result.
 * </p>
 *
 * @return an array of StackTraceElements.
 * @throws SecurityException
 *             if the current SecurityManager fails the
 *             {@link SecurityManager#checkPermission(java.security.Permission)}
 *             call.
 */
//        public StackTraceElement[] getStackTrace()

/**
 * Returns the current state of the Thread. This method is useful for
 * monitoring purposes.
 *
 * @return a {@link State} value.
 */
ECode Thread::GetState(
    /* [out] */ ThreadState* state)
{
    VALIDATE_NOT_NULL(state);
    // // TODO This is ugly and should be implemented better.
    // VMThread vmt = this.vmThread;

    // // Make sure we have a valid reference to an object. If native code
    // // deletes the reference we won't run into a null reference later.
    // VMThread thread = vmThread;
    // if (thread != null) {
    //     // If the Thread Object became invalid or was not yet started,
    //     // getStatus() will return -1.
    //     int state = thread.getStatus();
    //     if(state != -1) {
    //         return VMThread.STATE_MAP[state];
    //     }
    // }

    Int32 ts = NativeGetState();
    if (ts != -1) {
        *state = STATE_MAP[ts];
        return NOERROR;
    }

    *state = mHasBeenStarted ? ThreadState_TERMINATED : ThreadState_NEW;
    return NOERROR;
}

Int32 Thread::NativeGetState()
{
    NativeThread* thread;
    Int32 result;

    NativeLockThreadList(NULL);
    thread = NativeGetThreadFromThreadObject(this);
    if (thread != NULL) {
        result = thread->mStatus;
    }
    else {
        result = NTHREAD_ZOMBIE;// assume it used to exist and is now gone
    }
    NativeUnlockThreadList();

    return result;
}

/**
 * Returns the ThreadGroup to which this Thread belongs.
 *
 * @return the Thread's ThreadGroup
 */
ECode Thread::GetThreadGroup(
    /* [out] */ IThreadGroup** group)
{
    VALIDATE_NOT_NULL(group);
    // TODO This should actually be done at native termination.
    ThreadState state;
    GetState(&state);
    if (state == ThreadState_TERMINATED) {
        *group = NULL;
        return NOERROR;
    }
    else {
        *group = (IThreadGroup*)mGroup->Probe(EIID_IThreadGroup);
        return NOERROR;
    }
}

/**
 * Returns the thread's uncaught exception handler. If not explicitly set,
 * then the ThreadGroup's handler is returned. If the thread is terminated,
 * then <code>null</code> is returned.
 *
 * @return an {@link UncaughtExceptionHandler} instance or {@code null}.
 */
//        public UncaughtExceptionHandler getUncaughtExceptionHandler()

/**
 * Posts an interrupt request to this {@code Thread}. Unless the caller is
 * the {@link #currentThread()}, the method {@code checkAccess()} is called
 * for the installed {@code SecurityManager}, if any. This may result in a
 * {@code SecurityException} being thrown. The further behavior depends on
 * the state of this {@code Thread}:
 * <ul>
 * <li>
 * {@code Thread}s blocked in one of {@code Object}'s {@code wait()} methods
 * or one of {@code Thread}'s {@code join()} or {@code sleep()} methods will
 * be woken up, their interrupt status will be cleared, and they receive an
 * {@link InterruptedException}.
 * <li>
 * {@code Thread}s blocked in an I/O operation of an
 * {@link java.nio.channels.InterruptibleChannel} will have their interrupt
 * status set and receive an
 * {@link java.nio.channels.ClosedByInterruptException}. Also, the channel
 * will be closed.
 * <li>
 * {@code Thread}s blocked in a {@link java.nio.channels.Selector} will have
 * their interrupt status set and return immediately. They don't receive an
 * exception in this case.
 * <ul>
 *
 * @throws SecurityException
 *             if <code>checkAccess()</code> fails with a SecurityException
 * @see java.lang.SecurityException
 * @see java.lang.SecurityManager
 * @see Thread#interrupted
 * @see Thread#isInterrupted
 */
ECode Thread::Interrupt()
{
    FAIL_RETURN(CheckAccess());

    if (mInterruptAction != NULL) {
        mInterruptAction->Run();
    }
    // VMThread vmt = this.vmThread;
    // if (vmt != null) {
    //     vmt.interrupt();
    // }
    NativeInterrupt();

    return NOERROR;
}

/*
 * void interrupt()
 *
 * Interrupt a thread that is waiting (or is about to wait) on a monitor.
 */
void Thread::NativeInterrupt()
{
    NativeThread* thread;

    NativeLockThreadList(NULL);
    thread = NativeGetThreadFromThreadObject(this);
    if (thread != NULL) {
        NativeThreadInterrupt(thread);
    }
    NativeUnlockThreadList();
}

/**
 * Returns a <code>boolean</code> indicating whether the current Thread (
 * <code>currentThread()</code>) has a pending interrupt request (<code>
 * true</code>) or not (<code>false</code>). It also has the side-effect of
 * clearing the flag.
 *
 * @return a <code>boolean</code> indicating the interrupt status
 * @see Thread#currentThread
 * @see Thread#interrupt
 * @see Thread#isInterrupted
 */
Boolean Thread::Interrupted()
{
    NativeThread* self = NativeThreadSelf();
    Boolean interrupted = self->mInterrupted;
    self->mInterrupted = FALSE;
    return interrupted;
}

/**
 * Returns <code>true</code> if the receiver has already been started and
 * still runs code (hasn't died yet). Returns <code>false</code> either if
 * the receiver hasn't been started yet or if it has already started and run
 * to completion and died.
 *
 * @return a <code>boolean</code> indicating the liveness of the Thread
 * @see Thread#start
 */
ECode Thread::IsAlive(
    /* [out] */ Boolean* isAlive)
{
    VALIDATE_NOT_NULL(isAlive);
    *isAlive = TRUE;
//    return (vmThread != null);
    return NOERROR;
}

/**
 * Returns a <code>boolean</code> indicating whether the receiver is a
 * daemon Thread (<code>true</code>) or not (<code>false</code>) A
 * daemon Thread only runs as long as there are non-daemon Threads running.
 * When the last non-daemon Thread ends, the whole program ends no matter if
 * it had daemon Threads still running or not.
 *
 * @return a <code>boolean</code> indicating whether the Thread is a daemon
 * @see Thread#setDaemon
 */
ECode Thread::IsDaemon(
    /* [out] */ Boolean* isDaemon)
{
    VALIDATE_NOT_NULL(isDaemon);
    *isDaemon = mDaemon;
    return NOERROR;
}

/**
 * Returns a <code>boolean</code> indicating whether the receiver has a
 * pending interrupt request (<code>true</code>) or not (
 * <code>false</code>)
 *
 * @return a <code>boolean</code> indicating the interrupt status
 * @see Thread#interrupt
 * @see Thread#interrupted
 */
ECode Thread::IsInterrupted(
    /* [out] */ Boolean* isInterrupted)
{
    VALIDATE_NOT_NULL(isInterrupted);
    // VMThread vmt = this.vmThread;
    // if (vmt != null) {
    //     return vmt.isInterrupted();
    // }
    *isInterrupted = NativeIsInterrupted();
    return NOERROR;
}

/*
 * static boolean interrupted()
 *
 * Determine if the current thread has been interrupted.  Clears the flag.
 */
Boolean Thread::NativeIsInterrupted()
{
    Boolean interrupted;

    NativeLockThreadList(NULL);
    NativeThread* thread = NativeGetThreadFromThreadObject(this);
    if (thread != NULL)
        interrupted = thread->mInterrupted;
    else
        interrupted = FALSE;
    NativeUnlockThreadList();

    return interrupted;
}

/**
 * Blocks the current Thread (<code>Thread.currentThread()</code>) until
 * the receiver finishes its execution and dies.
 *
 * @throws InterruptedException if <code>interrupt()</code> was called for
 *         the receiver while it was in the <code>join()</code> call
 * @see Object#notifyAll
 * @see java.lang.ThreadDeath
 */
ECode Thread::Join()
{
    // VMThread t = vmThread;
    // if (t == null) {
    //     return;
    // }

    Mutex::Autolock lock(GetSelfLock());
    Boolean isAlive;
    IsAlive(&isAlive);
    while (isAlive) {
        Wait(0, 0);
    }

    return NOERROR;
}

/**
 * Blocks the current Thread (<code>Thread.currentThread()</code>) until
 * the receiver finishes its execution and dies or the specified timeout
 * expires, whatever happens first.
 *
 * @param millis The maximum time to wait (in milliseconds).
 * @throws InterruptedException if <code>interrupt()</code> was called for
 *         the receiver while it was in the <code>join()</code> call
 * @see Object#notifyAll
 * @see java.lang.ThreadDeath
 */
ECode Thread::JoinEx(
    /* [in] */ Int64 millis)
{
    return JoinEx2(millis, 0);
}

/**
 * Blocks the current Thread (<code>Thread.currentThread()</code>) until
 * the receiver finishes its execution and dies or the specified timeout
 * expires, whatever happens first.
 *
 * @param millis The maximum time to wait (in milliseconds).
 * @param nanos Extra nanosecond precision
 * @throws InterruptedException if <code>interrupt()</code> was called for
 *         the receiver while it was in the <code>join()</code> call
 * @see Object#notifyAll
 * @see java.lang.ThreadDeath
 */
ECode Thread::JoinEx2(
    /* [in] */ Int64 millis,
    /* [in] */ Int32 nanos)
{
    if (millis < 0 || nanos < 0 || nanos >= NANOS_PER_MILLI) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException();
    }

    // avoid overflow: if total > 292,277 years, just wait forever
    Boolean overflow = millis >= (Math::INT64_MAX_VALUE - nanos) / NANOS_PER_MILLI;
    Boolean forever = (millis | nanos) == 0;
    if (forever | overflow) {
        return Join();
    }

    // VMThread t = vmThread;
    // if (t == null) {
    //     return;
    // }

    Mutex::Autolock lock(GetSelfLock());
    Boolean isAlive;
    IsAlive(&isAlive);
    if (!isAlive) {
        return NOERROR;
    }

    // guaranteed not to overflow
    Int64 nanosToWait = millis * NANOS_PER_MILLI + nanos;

    // wait until this thread completes or the timeout has elapsed
    Int64 start = System::GetNanoTime();
    while (TRUE) {
        //t.wait(millis, nanos);//todo: must call
        Boolean isAlive;
        IsAlive(&isAlive);
        if (!isAlive) {
            break;
        }
        Int64 nanosElapsed = System::GetNanoTime() - start;
        Int64 nanosRemaining = nanosToWait - nanosElapsed;
        if (nanosRemaining <= 0) {
            break;
        }
        millis = nanosRemaining / NANOS_PER_MILLI;
        nanos = (Int32) (nanosRemaining - millis * NANOS_PER_MILLI);
    }

    return NOERROR;
}

/**
 * Resumes a suspended Thread. This is a no-op if the receiver was never
 * suspended, or suspended and already resumed. If the receiver is
 * suspended, however, makes it resume to the point where it was when it was
 * suspended.
 *
 * @throws SecurityException
 *             if <code>checkAccess()</code> fails with a SecurityException
 * @see Thread#suspend()
 * @deprecated Used with deprecated method {@link Thread#suspend}
 */
//@Deprecated
ECode Thread::Resume()
{
    FAIL_RETURN(CheckAccess());

    // VMThread vmt = this.vmThread;
    // if (vmt != null) {
    //     vmt.resume();
    // }
    // Logger.global.log(Level.SEVERE, UNSUPPORTED_THREAD_METHOD,
    //         new UnsupportedOperationException());

    return NOERROR;
}


/**
 * Calls the <code>run()</code> method of the Runnable object the receiver
 * holds. If no Runnable is set, does nothing.
 *
 * @see Thread#start
 */
ECode Thread::Run()
{
    ECode ec = NOERROR;
    if (mTarget != NULL) {
        ec = mTarget->Run();
    }
    return ec;
}

/**
 * Set the context ClassLoader for the receiver.
 * <p>
 * The <code>RuntimePermission("setContextClassLoader")</code>
 * is checked prior to setting the handler.
 * </p>
 *
 * @param cl The context ClassLoader
 * @throws SecurityException if the current SecurityManager fails the
 *         checkPermission call.
 * @see java.lang.ClassLoader
 * @see #getContextClassLoader()
 */
//        public void setContextClassLoader(ClassLoader cl)

/**
 * Set if the receiver is a daemon Thread or not. This can only be done
 * before the Thread starts running.
 *
 * @param isDaemon
 *            indicates whether the Thread should be daemon or not
 * @throws SecurityException
 *             if <code>checkAccess()</code> fails with a SecurityException
 * @see Thread#isDaemon
 */
ECode Thread::SetDaemon(
    /* [in] */ Boolean isDaemon)
{
    FAIL_RETURN(CheckAccess());

    if (mHasBeenStarted) {
        return E_ILLEGAL_THREAD_STATE_EXCEPTION;
//        throw new IllegalThreadStateException("Thread already started."); // TODO Externalize?
    }

    // if (vmThread == null) {
    //     daemon = isDaemon;
    // }
    mDaemon = isDaemon;
    return NOERROR;
}

/**
 * <p>
 * Sets the default uncaught exception handler. This handler is invoked in
 * case any Thread dies due to an unhandled exception.
 * </p>
 * <p>
 * The <code>RuntimePermission("setDefaultUncaughtExceptionHandler")</code>
 * is checked prior to setting the handler.
 * </p>
 *
 * @param handler
 *            The handler to set or <code>null</code>.
 * @throws SecurityException
 *             if the current SecurityManager fails the checkPermission
 *             call.
 */
// static CARAPI_(void) SetDefaultUncaughtExceptionHandler(
//     /* [in] */ UncaughtExceptionHandler handler);

/**
 * Set the action to be executed when interruption, which is probably be
 * used to implement the interruptible channel. The action is null by
 * default. And if this method is invoked by passing in a non-null value,
 * this action's run() method will be invoked in <code>interrupt()</code>.
 * <p>
 * This is required internally by NIO, so even if it looks like it's
 * useless, don't delete it!
 *
 * @param action the action to be executed when interruption
 */
//@SuppressWarnings("unused")
void Thread::SetInterruptAction(
    /* [in] */ IRunnable* action)
{
    mInterruptAction = action;
}

/**
 * Sets the name of the Thread.
 *
 * @param threadName the new name for the Thread
 * @throws SecurityException if <code>checkAccess()</code> fails with a
 *         SecurityException
 * @see Thread#getName
 */
ECode Thread::SetName(
    /* [in] */ const String& threadName)
{
    if (threadName.IsNull()) {
        return E_NULL_POINTER_EXCEPTION;
//        throw new NullPointerException();
    }

    FAIL_RETURN(CheckAccess());

    mName = threadName;
    //VMThread vmt = this.vmThread;
    //if (vmt != null) {
    //    /* notify the VM that the thread name has changed */
    //    vmt.nameChanged(threadName);
    //}
    NativeNameChanged(threadName);

    return NOERROR;
}

void Thread::NativeNameChanged(
    /* [in] */ const String& threadName)
{
    Int32 threadId = -1;

    /* get the thread's ID */
    NativeLockThreadList(NULL);
    NativeThread* thread = NativeGetThreadFromThreadObject(this);
    if (thread != NULL)
        threadId = thread->mThreadId;
    NativeUnlockThreadList();

    //vmDdmSendThreadNameChange(threadId, nameStr);
    //char* str = dvmCreateCstrFromString(nameStr);
    //LOGI("UPDATE: threadid=%d now '%s'\n", threadId, str);
    //free(str);
}

/**
 * Sets the priority of the Thread. Note that the final priority set may not
 * be the parameter that was passed - it will depend on the receiver's
 * ThreadGroup. The priority cannot be set to be higher than the receiver's
 * ThreadGroup's maxPriority().
 *
 * @param priority
 *            new priority for the Thread
 * @throws SecurityException
 *             if <code>checkAccess()</code> fails with a SecurityException
 * @throws IllegalArgumentException
 *             if the new priority is greater than Thread.MAX_PRIORITY or
 *             less than Thread.MIN_PRIORITY
 * @see Thread#getPriority
 */
ECode Thread::SetPriority(
    /* [in] */ Int32 priority)
{
    FAIL_RETURN(CheckAccess());

    if (priority < Thread_MIN_PRIORITY || priority > Thread_MAX_PRIORITY) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("Priority out of range"); // TODO Externalize?
    }

    Int32 groupPriority;
    AutoPtr<IThreadGroup> group = (IThreadGroup*)mGroup->Probe(EIID_IThreadGroup);
    group->GetMaxPriority(&groupPriority);
    if (priority > groupPriority) {
        priority = groupPriority;
    }

    mPriority = priority;

    // VMThread vmt = this.vmThread;
    // if (vmt != null) {
    //     vmt.setPriority(priority);
    // }
    NativeSetPriority(priority);

    return NOERROR;
}

/*
 * void setPriority(int newPriority)
 *
 * Alter the priority of the specified thread.  "newPriority" will range
 * from Thread.MIN_PRIORITY to Thread.MAX_PRIORITY (1-10), with "normal"
 * threads at Thread.NORM_PRIORITY (5).
 */
void Thread::NativeSetPriority(
    /* [in] */ Int32 priority)
{
    NativeLockThreadList(NULL);
    NativeThread* thread = NativeGetThreadFromThreadObject(this);
    if (thread != NULL)
        NativeChangeThreadPriority(thread, priority);
    //dvmDumpAllThreads(false);
    NativeUnlockThreadList();
}

/**
 * Causes the thread which sent this message to sleep for the given interval
 * of time (given in milliseconds). The precision is not guaranteed - the
 * Thread may sleep more or less than requested.
 *
 * @param time
 *            The time to sleep in milliseconds.
 * @throws InterruptedException
 *             if <code>interrupt()</code> was called for this Thread while
 *             it was sleeping
 * @see Thread#interrupt()
 */
ECode Thread::Sleep(
    /* [in] */ Int64 time)
{
    return Thread::Sleep(time, 0);
}

/**
 * Causes the thread which sent this message to sleep for the given interval
 * of time (given in milliseconds and nanoseconds). The precision is not
 * guaranteed - the Thread may sleep more or less than requested.
 *
 * @param millis
 *            The time to sleep in milliseconds.
 * @param nanos
 *            Extra nanosecond precision
 * @throws InterruptedException
 *             if <code>interrupt()</code> was called for this Thread while
 *             it was sleeping
 * @see Thread#interrupt()
 */
ECode Thread::Sleep(
    /* [in] */ Int64 millis,
    /* [in] */ Int32 nanos)
{
    return NativeThreadSleep(millis, nanos);
}

/**
 * <p>
 * Sets the uncaught exception handler. This handler is invoked in case this
 * Thread dies due to an unhandled exception.
 * </p>
 *
 * @param handler
 *            The handler to set or <code>null</code>.
 * @throws SecurityException
 *             if the current SecurityManager fails the checkAccess call.
 */
//        public void setUncaughtExceptionHandler(UncaughtExceptionHandler handler)

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
ECode Thread::Start()
{
    Mutex::Autolock lock(GetSelfLock());

    if (mHasBeenStarted) {
        // throw new IllegalThreadStateException("Thread already started."); // TODO Externalize?
        return E_ILLEGAL_THREAD_STATE_EXCEPTION;
    }

    mHasBeenStarted = TRUE;

    /* copying collector will pin threadObj for us since it was an argument */
    return NativeCreateThread(this, (Int32)mStackSize) ? NOERROR : E_RUNTIME_EXCEPTION;
}

/**
 * Requests the receiver Thread to stop and throw ThreadDeath. The Thread is
 * resumed if it was suspended and awakened if it was sleeping, so that it
 * can proceed to throw ThreadDeath.
 *
 * @throws SecurityException if <code>checkAccess()</code> fails with a
 *         SecurityException
 * @deprecated because stopping a thread in this manner is unsafe and can
 * leave your application and the VM in an unpredictable state.
 */
//@Deprecated
ECode Thread::Stop()
{
    // SecurityManager securityManager = System.getSecurityManager();
    // if (securityManager != null) {
    //     securityManager.checkAccess(this);
    //     if (Thread.currentThread() != this) {
    //         securityManager.checkPermission(new RuntimePermission("stopThread"));
    //     }
    // }

    // if (throwable == null) {
    //     throw new NullPointerException();
    // }

    // VMThread vmt = this.vmThread;
    // if (vmt != null) {
    //     vmt.stop(throwable);
    // }
    NativeStop();

    return NOERROR;
}

/**
 * Stops the Thread, passing it a Throwable (which might be ThreadDeath).
 */
//@SuppressWarnings("ThrowableInstanceNeverThrown")
void Thread::NativeStop()
{
    // Logger.global.log(Level.SEVERE, UNSUPPORTED_THREAD_METHOD,
    //         new UnsupportedOperationException());
}

/**
 * Requests the receiver Thread to stop and throw the
 * <code>throwable()</code>. The Thread is resumed if it was suspended
 * and awakened if it was sleeping, so that it can proceed to throw the
 * <code>throwable()</code>.
 *
 * @param throwable Throwable object to be thrown by the Thread
 * @throws SecurityException if <code>checkAccess()</code> fails with a
 *         SecurityException
 * @throws NullPointerException if <code>throwable()</code> is
 *         <code>null</code>
 * @deprecated because stopping a thread in this manner is unsafe and can
 * leave your application and the VM in an unpredictable state.
 */
//@Deprecated
//        public final synchronized void stop(Throwable throwable)

/**
 * Suspends this Thread. This is a no-op if the receiver is suspended. If
 * the receiver <code>isAlive()</code> however, suspended it until <code>
 * resume()</code> is sent to it. Suspend requests are not queued, which
 * means that N requests are equivalent to just one - only one resume
 * request is needed in this case.
 *
 * @throws SecurityException
 *             if <code>checkAccess()</code> fails with a SecurityException
 * @see Thread#resume()
 * @deprecated May cause deadlocks.
 */
//@Deprecated
ECode Thread::Suspend()
{
    FAIL_RETURN(CheckAccess());

    // VMThread vmt = this.vmThread;
    // if (vmt != null) {
    //     vmt.suspend();
    // }
    NativeSuspend();

    return NOERROR;
}

/**
 * Suspends the Thread.
 */
//@SuppressWarnings("ThrowableInstanceNeverThrown")
void Thread::NativeSuspend()
{
    // Logger.global.log(Level.SEVERE, UNSUPPORTED_THREAD_METHOD,
    //         new UnsupportedOperationException());
}

/**
 * Causes the calling Thread to yield execution time to another Thread that
 * is ready to run. The actual scheduling is implementation-dependent.
 */
void Thread::Yield()
{
}

/**
 * Indicates whether the current Thread has a monitor lock on the specified
 * object.
 *
 * @param object the object to test for the monitor lock
 * @return true if the current thread has a monitor lock on the specified
 *         object; false otherwise
 */
//static CARAPI_(Boolean) HoldsLock(Object object);

ECode Thread::Attach(
    /* [out] */ IThread** thread)
{
    return Thread::Attach(String(NULL), thread);
}

ECode Thread::Attach(
    /* [in] */ const String& name,
    /* [out] */ IThread** thread)
{
    VALIDATE_NOT_NULL(thread);

    // JavaVMAttachArgs* args = (JavaVMAttachArgs*) thr_args;
    NativeThread* self;
    Boolean result = FALSE;

    /*
     * Return immediately if we're already one with the VM.
     */
    self = NativeThreadSelf();
    if (self != NULL) {
        // *p_env = self->jniEnv;
        // return JNI_OK;
        *thread = (IThread*)self->mThreadObj->Probe(EIID_IThread);
        (*thread)->AddRef();
        return NOERROR;
    }

    /*
     * No threads allowed in zygote mode.
     */
    // if (gDvm.zygote) {
    //     return JNI_ERR;
    // }

    /* increment the count to keep the VM from bailing while we run */
    // dvmLockThreadList(NULL);
    // if (gDvm.nonDaemonThreadCount == 0) {
    //     // dead or dying
    //     LOGV("Refusing to attach thread '%s' -- VM is shutting down\n",
    //         (thr_args == NULL) ? "(unknown)" : args->name);
    //     dvmUnlockThreadList();
    //     return JNI_ERR;
    // }
    // gDvm.nonDaemonThreadCount++;
    // dvmUnlockThreadList();

    /* tweak the JavaVMAttachArgs as needed */
    NativeAttachArgs argsCopy;
    // if (args == NULL) {
    //     /* allow the v1.1 calling convention */
    //     argsCopy.version = JNI_VERSION_1_2;
    //     argsCopy.name = NULL;
    //     argsCopy.group = dvmGetMainThreadGroup();
    // } else {
    //     assert(args->version >= JNI_VERSION_1_2);

    //     argsCopy.version = args->version;
    //     argsCopy.name = args->name;
    //     if (args->group != NULL)
    //         argsCopy.group = args->group;
    //     else
    //         argsCopy.group = dvmGetMainThreadGroup();
    // }
    argsCopy.mName = name;
    ECode ec = NativeAttachCurrentThread(&argsCopy, FALSE, thread);

    /* restore the count */
    // dvmLockThreadList(NULL);
    // gDvm.nonDaemonThreadCount--;
    // dvmUnlockThreadList();

    /*
     * Change the status to indicate that we're out in native code.  This
     * call is not guarded with state-change macros, so we have to do it
     * by hand.
     */
    // if (result) {
    //     self = dvmThreadSelf();
    //     assert(self != NULL);
    //     dvmChangeStatus(self, THREAD_NATIVE);
    //     *p_env = self->jniEnv;
    //     return JNI_OK;
    // } else {
    //     return JNI_ERR;
    // }
    return ec;
}

ECode Thread::Detach()
{
    NativeThread* self = NativeThreadSelf();

    if (self == NULL || mNativeThread != self) {              /* not attached, can't do anything */
        return E_ILLEGAL_THREAD_STATE_EXCEPTION;
    }

    /* switch to "running" to check for suspension */
    // NativeChangeStatus(self, NTHREAD_RUNNING);
    NativeChangeStatus(self, NTHREAD_NATIVE);

    /* detach the thread */
    NativeDetachCurrentThread();

    /* (no need to change status back -- we have no status) */
    return NOERROR;
}
