
#include "cmdef.h"
#include "Thread.h"
#include "NativeThread.h"
#include "ThreadGroup.h"

const Int32 Thread::NANOS_PER_MILLI = 1000000;
Int32 Thread::sCount = 0;

Thread::Thread()
    : mDaemon(FALSE)
    , mHasBeenStarted(FALSE)
{

}

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

    AutoPtr<IThread> currentThread = Thread::GetCurrentThread();
    if (group == NULL) {
        currentThread->GetThreadGroup(&group);
    }

    FAIL_RETURN(group->CheckAccess());
    Boolean isDestroyed;
    if (group->IsDestroyed(&isDestroyed), isDestroyed) {
        // throw new IllegalThreadStateException("Group already destroyed");
        group->Release();
        return E_ILLEGAL_THREAD_STATE_EXCEPTION;
    }

    mGroup = (ThreadGroup*)group->Probe(EIID_ThreadGroup);

    // synchronized (Thread.class) {
    //     id = ++Thread.count;
    // }

    if (threadName.IsNull()) {
        // this.name = "Thread-" + id;
    }
    else {
        mName = threadName;
    }

    mTarget = runnable;
    mStackSize = stackSize;

    currentThread->GetPriority(&mPriority);

    // this.contextClassLoader = currentThread.contextClassLoader;

    // Transfer over InheritableThreadLocals.
    // if (currentThread.inheritableValues != null) {
    //     inheritableValues
    //             = new ThreadLocal.Values(currentThread.inheritableValues);
    // }

    // store current AccessControlContext as inherited context for this thread
    // SecurityUtils.putContext(this, AccessController.getContext());

    // add ourselves to our ThreadGroup of choice
    mGroup->AddThread((IThread*)this->Probe(EIID_IThread));
    group->Release();
    return NOERROR;
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

AutoPtr<IThread> Thread::NativeGetCurrentThread()
{
    return AutoPtr<IThread>((IThread*)NativeThreadSelf()->mThreadObj->Probe(EIID_IThread));
}
