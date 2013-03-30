
#include "CThread.h"


/**
 * Constructs a new {@code Thread} with no {@code Runnable} object and a
 * newly generated name. The new {@code Thread} will belong to the same
 * {@code ThreadGroup} as the {@code Thread} calling this constructor.
 *
 * @see java.lang.ThreadGroup
 * @see java.lang.Runnable
 */
ECode CThread::constructor()
{
    return Thread::Init();
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
ECode CThread::constructor(
    /* [in] */ IRunnable* runnable)
{
    return Thread::Init(runnable);
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
ECode CThread::constructor(
    /* [in] */ IRunnable* runnable,
    /* [in] */ const String& threadName)
{
    return Thread::Init(runnable, threadName);
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
ECode CThread::constructor(
    /* [in] */ const String& threadName)
{
    return Thread::Init(threadName);
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
ECode CThread::constructor(
    /* [in] */ IThreadGroup* group,
    /* [in] */ IRunnable* runnable)
{
    return Thread::Init(group, runnable);
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
ECode CThread::constructor(
    /* [in] */ IThreadGroup* group,
    /* [in] */ IRunnable* runnable,
    /* [in] */ const String& threadName)
{
    return Thread::Init(group, runnable, threadName);
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
ECode CThread::constructor(
    /* [in] */ IThreadGroup* group,
    /* [in] */ const String& threadName)
{
    return Thread::Init(group, threadName);
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
ECode CThread::constructor(
    /* [in] */ IThreadGroup* group,
    /* [in] */ IRunnable* runnable,
    /* [in] */ const String& threadName,
    /* [in] */ Int64 stackSize)
{
    return Thread::Init(group, runnable, threadName, stackSize);
}

ECode CThread::constructor(
    /* [in] */ IThreadGroup* group,
    /* [in] */ const String& name,
    /* [in] */ Int32 priority,
    /* [in] */ Boolean daemon)
{
    return Thread::Init(group, name, priority, daemon);
}

PInterface CThread::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_Thread) {
        return reinterpret_cast<PInterface>((Thread*)this);
    }
    else
        return _CThread::Probe(riid);
}

/**
 * Is used for operations that require approval from a SecurityManager. If
 * there's none installed, this method is a no-op. If there's a
 * SecurityManager installed, {@link SecurityManager#checkAccess(Thread)} is
 * called for that SecurityManager.
 *
 * @throws SecurityException
 *             if a SecurityManager is installed and it does not allow
 *             access to the Thread.
 *
 * @see java.lang.SecurityException
 * @see java.lang.SecurityManager
 */
ECode CThread::CheckAccess()
{
    return Thread::CheckAccess();
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
ECode CThread::CountStackFrames(
    /* [out] */ Int32* number)
{
    return Thread::CountStackFrames(number);
}

/**
 * Destroys the receiver without any monitor cleanup.
 *
 * @deprecated Not implemented.
 */
//@Deprecated
ECode CThread::Destroy()
{
    return Thread::Destroy();
}

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
 * Returns the thread's identifier. The ID is a positive <code>long</code>
 * generated on thread creation, is unique to the thread, and doesn't change
 * during the lifetime of the thread; the ID may be reused after the thread
 * has been terminated.
 *
 * @return the thread's ID.
 */
ECode CThread::GetId(
    /* [out] */ Int64* id)
{
    return Thread::GetId(id);
}

/**
 * Returns the name of the Thread.
 *
 * @return the Thread's name
 */
ECode CThread::GetName(
    /* [out] */ String* name)
{
    return Thread::GetName(name);
}

/**
 * Returns the priority of the Thread.
 *
 * @return the Thread's priority
 * @see Thread#setPriority
 */
ECode CThread::GetPriority(
    /* [out] */ Int32* priority)
{
    return Thread::GetPriority(priority);
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
ECode CThread::GetState(
    /* [out] */ ThreadState* state)
{
    return Thread::GetState(state);
}

/**
 * Returns the ThreadGroup to which this Thread belongs.
 *
 * @return the Thread's ThreadGroup
 */
ECode CThread::GetThreadGroup(
    /* [out] */ IThreadGroup** group)
{
    return Thread::GetThreadGroup(group);
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
ECode CThread::Interrupt()
{
    return Thread::Interrupt();
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
ECode CThread::IsAlive(
    /* [out] */ Boolean* isAlive)
{
    return Thread::IsAlive(isAlive);
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
ECode CThread::IsDaemon(
    /* [out] */ Boolean* isDaemon)
{
    return Thread::IsDaemon(isDaemon);
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
ECode CThread::IsInterrupted(
    /* [out] */ Boolean* isInterrupted)
{
    return Thread::IsInterrupted(isInterrupted);
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
ECode CThread::Join()
{
    return Thread::Join();
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
ECode CThread::JoinEx(
    /* [in] */ Int64 millis)
{
    return Thread::JoinEx(millis);
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
ECode CThread::JoinEx2(
    /* [in] */ Int64 millis,
    /* [in] */ Int32 nanos)
{
    return Thread::JoinEx2(millis, nanos);
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
ECode CThread::Resume()
{
    return Thread::Resume();
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
ECode CThread::SetDaemon(
    /* [in] */ Boolean isDaemon)
{
    return Thread::SetDaemon(isDaemon);
}

/**
 * Sets the name of the Thread.
 *
 * @param threadName the new name for the Thread
 * @throws SecurityException if <code>checkAccess()</code> fails with a
 *         SecurityException
 * @see Thread#getName
 */
ECode CThread::SetName(
    /* [in] */ const String& threadName)
{
    return Thread::SetName(threadName);
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
ECode CThread::SetPriority(
    /* [in] */ Int32 priority)
{
    return Thread::SetPriority(priority);
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
ECode CThread::Start()
{
    return Thread::Start();
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
ECode CThread::Stop()
{
    return Thread::Stop();
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
ECode CThread::Suspend()
{
    return Thread::Suspend();
}

ECode CThread::Run()
{
    return Thread::Run();
}

ECode CThread::Detach()
{
    return Thread::Detach();
}

ECode CThread::Lock()
{
    return Thread::Lock();
}

ECode CThread::Unlock()
{
    return Thread::Unlock();
}

ECode CThread::Notify()
{
    return Thread::Notify();
}

ECode CThread::NotifyAll()
{
    return Thread::NotifyAll();
}

ECode CThread::Wait()
{
    return Thread::Wait();
}

ECode CThread::WaitEx(
    /* [in] */ Int64 millis)
{
    return Thread::Wait(millis);
}

ECode CThread::WaitEx2(
    /* [in] */ Int64 millis,
    /* [in] */ Int32 nanos)
{
    return Thread::Wait(millis, nanos);
}

Mutex* CThread::GetSelfLock()
{
    return &_m_syncLock;
}
