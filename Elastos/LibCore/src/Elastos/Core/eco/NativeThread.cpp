
#include "NativeThread.h"
#include "Globals.h"
#include "CThread.h"
#include <sys/resource.h>
#include <cutils/sched_policy.h>
#include <cutils/atomic.h>
#include <unistd.h>
#include "stdio.h"

static NativeThread* AllocThread(Int32 stackSize);
static void SetThreadSelf(NativeThread* thread);
static void AssignThreadId(NativeThread* thread);
static void* ThreadEntry(void* arg);
static void ThreadExitCheck(void* arg);
static Boolean PrepareThread(NativeThread* thread);

/*
 * Initialize thread list and main thread's environment.  We need to set
 * up some basic stuff so that dvmThreadSelf() will work when we start
 * loading classes (e.g. to check for exceptions).
 */
Boolean NativeThreadStartup()
{
    NativeThread* thread = NULL;

    /* allocate a TLS slot */
    if (pthread_key_create(&gDvm.mPthreadKeySelf, ThreadExitCheck) != 0) {
        //LOGE("ERROR: pthread_key_create failed\n");
        return FALSE;
    }

    /* test our pthread lib */
    if (pthread_getspecific(gDvm.mPthreadKeySelf) != NULL)
        //LOGW("WARNING: newly-created pthread TLS slot is not NULL\n");

    /* prep thread-related locks and conditions */
    NativeInitMutex(&gDvm.mThreadListLock);
    pthread_cond_init(&gDvm.mThreadStartCond, NULL);
    //dvmInitMutex(&gDvm.vmExitLock);
    //pthread_cond_init(&gDvm.vmExitCond, NULL);
    NativeInitMutex(&gDvm.mThreadSuspendLock);
    NativeInitMutex(&gDvm.mThreadSuspendCountLock);
    pthread_cond_init(&gDvm.mThreadSuspendCountCond, NULL);
// #ifdef WITH_DEADLOCK_PREDICTION
//     dvmInitMutex(&gDvm.deadlockHistoryLock);
// #endif

    /*
     * Dedicated monitor for Thread.sleep().
     * TODO: change this to an Object* so we don't have to expose this
     * call, and we interact better with JDWP monitor calls.  Requires
     * deferring the object creation to much later (e.g. final "main"
     * thread prep) or until first use.
     */
    // gDvm.threadSleepMon = dvmCreateMonitor(NULL);

    // gDvm.threadIdMap = dvmAllocBitVector(kMaxThreadId, false);

    thread = AllocThread(gDvm.mStackSize);
    if (thread == NULL)
        return FALSE;

    /* switch mode for when we run initializers */
    thread->mStatus = NTHREAD_RUNNING;

    /*
     * We need to assign the threadId early so we can lock/notify
     * object monitors.  We'll set the "threadObj" field later.
     */
    PrepareThread(thread);
    gDvm.mThreadList = thread;

// #ifdef COUNT_PRECISE_METHODS
//     gDvm.preciseMethods = dvmPointerSetAlloc(200);
// #endif

    return TRUE;
}

/*
 * Grab the thread list global lock.
 *
 * This is held while "suspend all" is trying to make everybody stop.  If
 * the shutdown is in progress, and somebody tries to grab the lock, they'll
 * have to wait for the GC to finish.  Therefore it's important that the
 * thread not be in RUNNING mode.
 *
 * We don't have to check to see if we should be suspended once we have
 * the lock.  Nobody can suspend all threads without holding the thread list
 * lock while they do it, so by definition there isn't a GC in progress.
 *
 * This function deliberately avoids the use of dvmChangeStatus(),
 * which could grab threadSuspendCountLock.  To avoid deadlock, threads
 * are required to grab the thread list lock before the thread suspend
 * count lock.  (See comment in DvmGlobals.)
 *
 * TODO: consider checking for suspend after acquiring the lock, and
 * backing off if set.  As stated above, it can't happen during normal
 * execution, but it *can* happen during shutdown when daemon threads
 * are being suspended.
 */
void NativeLockThreadList(
    /* [in] */ NativeThread* self)
{
    NativeThreadStatus oldStatus;

    if (self == NULL) {
        /* try to get it from TLS */
        self = NativeThreadSelf();
    }

    if (self != NULL) {
        oldStatus = self->mStatus;
        self->mStatus = NTHREAD_VMWAIT;
    }
    else {
        /* happens during VM shutdown */
        //LOGW("NULL self in dvmLockThreadList\n");
        // oldStatus = -1;         // shut up gcc
    }

    NativeLockMutex(&gDvm.mThreadListLock);

    if (self != NULL) {
        self->mStatus = oldStatus;
    }
}

/*
 * Release the thread list global lock.
 */
void NativeUnlockThreadList()
{
    NativeUnlockMutex(&gDvm.mThreadListLock);
}

/*
 * Finish preparing the main thread, allocating some objects to represent
 * it.  As part of doing so, we finish initializing Thread and ThreadGroup.
 * This will execute some interpreted code (e.g. class initializers).
 */
Boolean NativePrepMainThread()
{
    NativeThread* thread;
    AutoPtr<IThreadGroup> groupObj;
    // Object* threadObj;
    // Object* vmThreadObj;
    // StringObject* threadNameStr;
    // Method* init;
    // JValue unused;

    //LOGV("+++ finishing prep on main VM thread\n");

    /* main thread is always first in list at this point */
    thread = gDvm.mThreadList;
    assert(thread->mThreadId == kMainThreadId);

    /*
     * Make sure the classes are initialized.  We have to do this before
     * we create an instance of them.
     */
    // if (!dvmInitClass(gDvm.classJavaLangClass)) {
    //     LOGE("'Class' class failed to initialize\n");
    //     return false;
    // }
    // if (!dvmInitClass(gDvm.classJavaLangThreadGroup) ||
    //     !dvmInitClass(gDvm.classJavaLangThread) ||
    //     !dvmInitClass(gDvm.classJavaLangVMThread))
    // {
    //     LOGE("thread classes failed to initialize\n");
    //     return false;
    // }

    // groupObj = dvmGetMainThreadGroup();
    // if (groupObj == NULL)
    //     return false;

    /*
     * Allocate and construct a Thread with the internal-creation
     * constructor.
     */
    // threadObj = dvmAllocObject(gDvm.classJavaLangThread, ALLOC_DEFAULT);
    // if (threadObj == NULL) {
    //     LOGE("unable to allocate main thread object\n");
    //     return false;
    // }
    // dvmReleaseTrackedAlloc(threadObj, NULL);

    // threadNameStr = dvmCreateStringFromCstr("main");
    // if (threadNameStr == NULL)
    //     return false;
    // dvmReleaseTrackedAlloc((Object*)threadNameStr, NULL);

    // init = dvmFindDirectMethodByDescriptor(gDvm.classJavaLangThread, "<init>",
    //         "(Ljava/lang/ThreadGroup;Ljava/lang/String;IZ)V");
    // assert(init != NULL);
    // dvmCallMethod(thread, init, threadObj, &unused, groupObj, threadNameStr,
    //     THREAD_NORM_PRIORITY, false);
    // if (dvmCheckException(thread)) {
    //     LOGE("exception thrown while constructing main thread object\n");
    //     return false;
    // }
    AutoPtr<IThread> mainThread;
    const String threadName = String("main");
    CThread::New(groupObj, threadName, (IThread**)&mainThread);
    thread->mThreadObj = (Thread*)mainThread->Probe(EIID_Thread);

    /*
     * Allocate and construct a VMThread.
     */
    // vmThreadObj = dvmAllocObject(gDvm.classJavaLangVMThread, ALLOC_DEFAULT);
    // if (vmThreadObj == NULL) {
    //     LOGE("unable to allocate main vmthread object\n");
    //     return false;
    // }
    // dvmReleaseTrackedAlloc(vmThreadObj, NULL);

    // init = dvmFindDirectMethodByDescriptor(gDvm.classJavaLangVMThread, "<init>",
    //         "(Ljava/lang/Thread;)V");
    // dvmCallMethod(thread, init, vmThreadObj, &unused, threadObj);
    // if (dvmCheckException(thread)) {
    //     LOGE("exception thrown while constructing main vmthread object\n");
    //     return false;
    // }

    // /* set the VMThread.vmData field to our Thread struct */
    // assert(gDvm.offJavaLangVMThread_vmData != 0);
    // dvmSetFieldInt(vmThreadObj, gDvm.offJavaLangVMThread_vmData, (u4)thread);

    /*
     * Stuff the VMThread back into the Thread.  From this point on, other
     * Threads will see that this Thread is running (at least, they would,
     * if there were any).
     */
    // dvmSetFieldObject(threadObj, gDvm.offJavaLangThread_vmThread,
    //     vmThreadObj);

    // thread->threadObj = threadObj;

    /*
     * Set the context class loader.  This invokes a ClassLoader method,
     * which could conceivably call Thread.currentThread(), so we want the
     * Thread to be fully configured before we do this.
     */
    // Object* systemLoader = dvmGetSystemClassLoader();
    // if (systemLoader == NULL) {
    //     LOGW("WARNING: system class loader is NULL (setting main ctxt)\n");
    //     /* keep going */
    // }
    // int ctxtClassLoaderOffset = dvmFindFieldOffset(gDvm.classJavaLangThread,
    //     "contextClassLoader", "Ljava/lang/ClassLoader;");
    // if (ctxtClassLoaderOffset < 0) {
    //     LOGE("Unable to find contextClassLoader field in Thread\n");
    //     return false;
    // }
    // dvmSetFieldObject(threadObj, ctxtClassLoaderOffset, systemLoader);

    /*
     * Finish our thread prep.
     */

    /* include self in non-daemon threads (mainly for AttachCurrentThread) */
    gDvm.mNonDaemonThreadCount++;

    return TRUE;
}

/*
 * Alloc and initialize a Thread struct.
 *
 * Does not create any objects, just stuff on the system (malloc) heap.
 */
static NativeThread* AllocThread(
    /* [in] */ Int32 stackSize)
{
    NativeThread* thread;
    // u1* stackBottom;

    thread = (NativeThread*)calloc(1, sizeof(Thread));
    if (thread == NULL) {
        return NULL;
    }

//#if defined(WITH_SELF_VERIFICATION)
//    if (dvmSelfVerificationShadowSpaceAlloc(thread) == NULL)
//        return NULL;
//#endif

    assert(stackSize >= kMinStackSize && stackSize <=kMaxStackSize);

    thread->mStatus = NTHREAD_INITIALIZING;
    thread->mSuspendCount = 0;

#ifdef WITH_ALLOC_LIMITS
    thread->mAllocLimit = -1;
#endif

//    /*
//     * Allocate and initialize the interpreted code stack.  We essentially
//     * "lose" the alloc pointer, which points at the bottom of the stack,
//     * but we can get it back later because we know how big the stack is.
//     *
//     * The stack must be aligned on a 4-byte boundary.
//     */
//#ifdef MALLOC_INTERP_STACK
//    stackBottom = (u1*) malloc(interpStackSize);
//    if (stackBottom == NULL) {
//#if defined(WITH_SELF_VERIFICATION)
//        dvmSelfVerificationShadowSpaceFree(thread);
//#endif
//        free(thread);
//        return NULL;
//    }
//    memset(stackBottom, 0xc5, interpStackSize);     // stop valgrind complaints
//#else
//    stackBottom = mmap(NULL, interpStackSize, PROT_READ | PROT_WRITE,
//        MAP_PRIVATE | MAP_ANON, -1, 0);
//    if (stackBottom == MAP_FAILED) {
//#if defined(WITH_SELF_VERIFICATION)
//        dvmSelfVerificationShadowSpaceFree(thread);
//#endif
//        free(thread);
//        return NULL;
//    }
//#endif

//    assert(((u4)stackBottom & 0x03) == 0); // looks like our malloc ensures this
    thread->mStackSize = stackSize;
//    thread->interpStackStart = stackBottom + interpStackSize;
//    thread->interpStackEnd = stackBottom + STACK_OVERFLOW_RESERVE;
//
//    /* give the thread code a chance to set things up */
//    dvmInitInterpStack(thread, interpStackSize);

    return thread;
}

/*
 * Get a meaningful thread ID.  At present this only has meaning under Linux,
 * where getpid() and gettid() sometimes agree and sometimes don't depending
 * on your thread model (try "export LD_ASSUME_KERNEL=2.4.19").
 */
pid_t NativeGetSysThreadId()
{
#ifdef HAVE_GETTID
    return gettid();
#else
    return getpid();
#endif
}

/*
 * Finish initialization of a Thread struct.
 *
 * This must be called while executing in the new thread, but before the
 * thread is added to the thread list.
 *
 * NOTE: The threadListLock must be held by the caller (needed for
 * assignThreadId()).
 */
static Boolean PrepareThread(
    /* [in] */ NativeThread* thread)
{
    thread->mSystemTid = NativeGetSysThreadId();
    AssignThreadId(thread);
    thread->mHandle = pthread_self();
    //thread->mSystemTid = NativeGetSysThreadId();

    //LOGI("SYSTEM TID IS %d (pid is %d)\n", (int) thread->systemTid,
    //    (int) getpid());
    /*
     * If we were called by dvmAttachCurrentThread, the self value is
     * already correctly established as "thread".
     */
    SetThreadSelf(thread);

    // LOGV("threadid=%d: interp stack at %p\n",
    //     thread->threadId, thread->interpStackStart - thread->interpStackSize);

    /*
     * Initialize invokeReq.
     */
//    DvmInitMutex(&thread->invokeReq.lock);
//    pthread_cond_init(&thread->invokeReq.cv, NULL);

//    /*
//     * Initialize our reference tracking tables.
//     *
//     * Most threads won't use jniMonitorRefTable, so we clear out the
//     * structure but don't call the init function (which allocs storage).
//     */
//#ifdef USE_INDIRECT_REF
//    if (!dvmInitIndirectRefTable(&thread->jniLocalRefTable,
//            kJniLocalRefMin, kJniLocalRefMax, kIndirectKindLocal))
//        return false;
//#else
//    /*
//     * The JNI local ref table *must* be fixed-size because we keep pointers
//     * into the table in our stack frames.
//     */
//    if (!dvmInitReferenceTable(&thread->jniLocalRefTable,
//            kJniLocalRefMax, kJniLocalRefMax))
//        return false;
//#endif
//    if (!dvmInitReferenceTable(&thread->internalLocalRefTable,
//            kInternalRefDefault, kInternalRefMax))
//        return false;

    // memset(&thread->jniMonitorRefTable, 0, sizeof(thread->jniMonitorRefTable));

    pthread_cond_init(&thread->mWaitCond, NULL);
    NativeInitMutex(&thread->mWaitMutex);

    return TRUE;
}

/*
 * Remove a thread from the internal list.
 * Clear out the links to make it obvious that the thread is
 * no longer on the list.  Caller must hold gDvm.threadListLock.
 */
static void UnlinkThread(
    /* [in] */ NativeThread* thread)
{
    // LOG_THREAD("threadid=%d: removing from list\n", thread->threadId);
    if (thread == gDvm.mThreadList) {
        assert(thread->mPrev == NULL);
        gDvm.mThreadList = thread->mNext;
    }
    else {
        assert(thread->mPrev != NULL);
        thread->mPrev->mNext = thread->mNext;
    }
    if (thread->mNext != NULL) {
        thread->mNext->mPrev = thread->mPrev;
    }
    thread->mPrev = thread->mNext = NULL;
}

/*
 * Free a Thread struct, and all the stuff allocated within.
 */
static void FreeThread(NativeThread* thread)
{
    if (thread == NULL) {
        return;
    }

    /* thread->threadId is zero at this point */
    // LOGVV("threadid=%d: freeing\n", thread->threadId);

//     if (thread->interpStackStart != NULL) {
//         u1* interpStackBottom;

//         interpStackBottom = thread->interpStackStart;
//         interpStackBottom -= thread->interpStackSize;
// #ifdef MALLOC_INTERP_STACK
//         free(interpStackBottom);
// #else
//         if (munmap(interpStackBottom, thread->interpStackSize) != 0)
//             LOGW("munmap(thread stack) failed\n");
// #endif
//     }

// #ifdef USE_INDIRECT_REF
//     dvmClearIndirectRefTable(&thread->jniLocalRefTable);
// #else
//     dvmClearReferenceTable(&thread->jniLocalRefTable);
// #endif
//     dvmClearReferenceTable(&thread->internalLocalRefTable);
//     if (&thread->jniMonitorRefTable.table != NULL)
//         dvmClearReferenceTable(&thread->jniMonitorRefTable);

// #if defined(WITH_SELF_VERIFICATION)
//     dvmSelfVerificationShadowSpaceFree(thread);
// #endif
    free(thread);
}


/*
 * Like pthread_self(), but on a Thread*.
 */
NativeThread* NativeThreadSelf()
{
    return (NativeThread*)pthread_getspecific(gDvm.mPthreadKeySelf);
}

/*
 * Explore our sense of self.  Stuffs the thread pointer into TLS.
 */
static void SetThreadSelf(
    /* [in] */ NativeThread* thread)
{
    Int32 cc;

    cc = pthread_setspecific(gDvm.mPthreadKeySelf, thread);
    if (cc != 0) {
        /*
         * Sometimes this fails under Bionic with EINVAL during shutdown.
         * This can happen if the timing is just right, e.g. a thread
         * fails to attach during shutdown, but the "fail" path calls
         * here to ensure we clean up after ourselves.
         */
        if (thread != NULL) {
            // LOGE("pthread_setspecific(%p) failed, err=%d\n", thread, cc);
            // dvmAbort();     /* the world is fundamentally hosed */
        }
    }
}

/*
 * This is associated with the pthreadKeySelf key.  It's called by the
 * pthread library when a thread is exiting and the "self" pointer in TLS
 * is non-NULL, meaning the VM hasn't had a chance to clean up.  In normal
 * operation this will not be called.
 *
 * This is mainly of use to ensure that we don't leak resources if, for
 * example, a thread attaches itself to us with AttachCurrentThread and
 * then exits without notifying the VM.
 *
 * We could do the detach here instead of aborting, but this will lead to
 * portability problems.  Other implementations do not do this check and
 * will simply be unaware that the thread has exited, leading to resource
 * leaks (and, if this is a non-daemon thread, an infinite hang when the
 * VM tries to shut down).
 *
 * Because some implementations may want to use the pthread destructor
 * to initiate the detach, and the ordering of destructors is not defined,
 * we want to iterate a couple of times to give those a chance to run.
 */
static void ThreadExitCheck(void* arg)
{
    const Int32 kMaxCount = 2;

    NativeThread* self = (NativeThread*)arg;
    assert(self != NULL);

    // LOGV("threadid=%d: threadExitCheck(%p) count=%d\n",
    //     self->threadId, arg, self->threadExitCheckCount);

    if (self->mStatus == NTHREAD_ZOMBIE) {
        // LOGW("threadid=%d: Weird -- shouldn't be in threadExitCheck\n",
        //     self->threadId);
        return;
    }

    if (self->mThreadExitCheckCount < kMaxCount) {
        /*
         * Spin a couple of times to let other destructors fire.
         */
        // LOGD("threadid=%d: thread exiting, not yet detached (count=%d)\n",
        //     self->threadId, self->threadExitCheckCount);
        self->mThreadExitCheckCount++;
        Int32 cc = pthread_setspecific(gDvm.mPthreadKeySelf, self);
        if (cc != 0) {
            // LOGE("threadid=%d: unable to re-add thread to TLS\n",
            //     self->threadId);
            //NativeAbort();
        }
    }
    else {
        // LOGE("threadid=%d: native thread exited without detaching\n",
        //     self->threadId);
        //NativeAbort();
    }
}

/*
 * Assign the threadId.  This needs to be a small integer so that our
 * "thin" locks fit in a small number of bits.
 *
 * We reserve zero for use as an invalid ID.
 *
 * This must be called with threadListLock held.
 */
static void AssignThreadId(
    /* [in] */ NativeThread* thread)
{
    /*
     * Find a small unique integer.  threadIdMap is a vector of
     * kMaxThreadId bits;  dvmAllocBit() returns the index of a
     * bit, meaning that it will always be < kMaxThreadId.
     */
    // int num = dvmAllocBit(gDvm.threadIdMap);
    // if (num < 0) {
    //     LOGE("Ran out of thread IDs\n");
    //     dvmAbort();     // TODO: make this a non-fatal error result
    // }

    // thread->mThreadId = num + 1;
    thread->mThreadId = (UInt32)thread->mSystemTid;

    assert(thread->mThreadId != 0);
    // assert(thread->mThreadId != DVM_LOCK_INITIAL_THIN_VALUE);
}

/*
 * Give back the thread ID.
 */
static void ReleaseThreadId(
    /* [in] */ NativeThread* thread)
{
    assert(thread->mThreadId > 0);
    // dvmClearBit(gDvm.threadIdMap, thread->threadId - 1);
    thread->mThreadId = 0;
}

/*
 * Helper function to set the name of the current thread
 */
static void SetThreadName(
    /* [in] */ const char *threadName)
{
    Int32 hasAt = 0;
    Int32 hasDot = 0;
    const char *s = threadName;
    while (*s) {
        if (*s == '.') hasDot = 1;
        else if (*s == '@') hasAt = 1;
        s++;
    }
    Int32 len = s - threadName;
    if (len < 15 || hasAt || !hasDot) {
        s = threadName;
    }
    else {
        s = threadName + len - 15;
    }
#if defined(HAVE_ANDROID_PTHREAD_SETNAME_NP)
    /* pthread_setname_np fails rather than truncating long strings */
    char buf[16];       // MAX_TASK_COMM_LEN=16 is hard-coded into bionic
    strncpy(buf, s, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';
    Int32 err = pthread_setname_np(pthread_self(), buf);
    if (err != 0) {
        // LOGW("Unable to set the name of current thread to '%s': %s\n",
        //     buf, strerror(err));
    }
#elif defined(HAVE_PRCTL)
    prctl(PR_SET_NAME, (unsigned long) s, 0, 0, 0);
#else
    // LOGD("No way to set current thread's name (%s)\n", s);
#endif
}

/*
 * Create a thread as a result of java.lang.Thread.start().
 *
 * We do have to worry about some concurrency problems, e.g. programs
 * that try to call Thread.start() on the same object from multiple threads.
 * (This will fail for all but one, but we have to make sure that it succeeds
 * for exactly one.)
 *
 * Some of the complexity here arises from our desire to mimic the
 * Thread vs. VMThread class decomposition we inherited.  We've been given
 * a Thread, and now we need to create a VMThread and then populate both
 * objects.  We also need to create one of our internal Thread objects.
 *
 * Pass in a stack size of 0 to get the default.
 *
 * The "threadObj" reference must be pinned by the caller to prevent the GC
 * from moving it around (e.g. added to the tracked allocation list).
 */
Boolean NativeCreateThread(
    /* [in] */ Thread* threadObj,
    /* [in] */ Int32 reqStackSize)
{
    pthread_attr_t threadAttr;
    pthread_t threadHandle;
    NativeThread* self = NULL;
    NativeThread* newThread = NULL;
    Int32 stackSize;
    NativeThreadStatus oldStatus;
    Int32 cc;

    assert(threadObj != NULL);
    //self = NativeThreadSelf();
    if (reqStackSize == 0) {
        stackSize = kDefaultStackSize;//gDvm.mStackSize;
    }
    else if (reqStackSize < kMinStackSize) {
        stackSize = kMinStackSize;
    }
    else if (reqStackSize > kMaxStackSize) {
        stackSize = kMaxStackSize;
    }
    else {
        stackSize = reqStackSize;
    }

    pthread_attr_init(&threadAttr);
    pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_DETACHED);

    printf("======function: %s======, ======stackSize: %d======\n", __FUNCTION__, stackSize);
    newThread = AllocThread(stackSize);
    if (newThread == NULL) {
        goto fail;
    }
    pthread_attr_setstacksize (&threadAttr, stackSize);
    newThread->mThreadObj = threadObj;

    assert(newThread->mStatus == NTHREAD_INITIALIZING);

    /*
     * We need to lock out other threads while we test and set the
     * "vmThread" field in java.lang.Thread, because we use that to determine
     * if this thread has been started before.  We use the thread list lock
     * because it's handy and we're going to need to grab it again soon
     * anyway.
     */
    NativeLockThreadList(self);

    // if (dvmGetFieldObject(threadObj, gDvm.offJavaLangThread_vmThread) != NULL) {
    //     dvmUnlockThreadList();
    //     dvmThrowException("Ljava/lang/IllegalThreadStateException;",
    //         "thread has already been started");
    //     goto fail;
    // }

    /*
     * There are actually three data structures: Thread (object), VMThread
     * (object), and Thread (C struct).  All of them point to at least one
     * other.
     *
     * As soon as "VMThread.vmData" is assigned, other threads can start
     * making calls into us (e.g. setPriority).
     */
    // dvmSetFieldInt(vmThreadObj, gDvm.offJavaLangVMThread_vmData, (u4)newThread);
    // dvmSetFieldObject(threadObj, gDvm.offJavaLangThread_vmThread, vmThreadObj);
    threadObj->mNativeThread = newThread;

    /*
     * Thread creation might take a while, so release the lock.
     */
    NativeUnlockThreadList();
    // oldStatus = NativeChangeStatus(self, NTHREAD_VMWAIT);
    cc = pthread_create(&threadHandle, &threadAttr, ThreadEntry, newThread);
    // oldStatus = NativeChangeStatus(self, oldStatus);
    if (cc != 0) {
        /*
         * Failure generally indicates that we have exceeded system
         * resource limits.  VirtualMachineError is probably too severe,
         * so use OutOfMemoryError.
         */
        // LOGE("Thread creation failed (err=%s)\n", strerror(errno));

        // dvmSetFieldObject(threadObj, gDvm.offJavaLangThread_vmThread, NULL);

        // dvmThrowException("Ljava/lang/OutOfMemoryError;",
        //     "thread creation failed");
        goto fail;
    }

    /*
     * We need to wait for the thread to start.  Otherwise, depending on
     * the whims of the OS scheduler, we could return and the code in our
     * thread could try to do operations on the new thread before it had
     * finished starting.
     *
     * The new thread will lock the thread list, change its state to
     * THREAD_STARTING, broadcast to gDvm.threadStartCond, and then sleep
     * on gDvm.threadStartCond (which uses the thread list lock).  This
     * thread (the parent) will either see that the thread is already ready
     * after we grab the thread list lock, or will be awakened from the
     * condition variable on the broadcast.
     *
     * We don't want to stall the rest of the VM while the new thread
     * starts, which can happen if the GC wakes up at the wrong moment.
     * So, we change our own status to VMWAIT, and self-suspend if
     * necessary after we finish adding the new thread.
     *
     *
     * We have to deal with an odd race with the GC/debugger suspension
     * mechanism when creating a new thread.  The information about whether
     * or not a thread should be suspended is contained entirely within
     * the Thread struct; this is usually cleaner to deal with than having
     * one or more globally-visible suspension flags.  The trouble is that
     * we could create the thread while the VM is trying to suspend all
     * threads.  The suspend-count won't be nonzero for the new thread,
     * so dvmChangeStatus(THREAD_RUNNING) won't cause a suspension.
     *
     * The easiest way to deal with this is to prevent the new thread from
     * running until the parent says it's okay.  This results in the
     * following (correct) sequence of events for a "badly timed" GC
     * (where '-' is us, 'o' is the child, and '+' is some other thread):
     *
     *  - call pthread_create()
     *  - lock thread list
     *  - put self into THREAD_VMWAIT so GC doesn't wait for us
     *  - sleep on condition var (mutex = thread list lock) until child starts
     *  + GC triggered by another thread
     *  + thread list locked; suspend counts updated; thread list unlocked
     *  + loop waiting for all runnable threads to suspend
     *  + success, start GC
     *  o child thread wakes, signals condition var to wake parent
     *  o child waits for parent ack on condition variable
     *  - we wake up, locking thread list
     *  - add child to thread list
     *  - unlock thread list
     *  - change our state back to THREAD_RUNNING; GC causes us to suspend
     *  + GC finishes; all threads in thread list are resumed
     *  - lock thread list
     *  - set child to THREAD_VMWAIT, and signal it to start
     *  - unlock thread list
     *  o child resumes
     *  o child changes state to THREAD_RUNNING
     *
     * The above shows the GC starting up during thread creation, but if
     * it starts anywhere after VMThread.create() is called it will
     * produce the same series of events.
     *
     * Once the child is in the thread list, it will be suspended and
     * resumed like any other thread.  In the above scenario the resume-all
     * code will try to resume the new thread, which was never actually
     * suspended, and try to decrement the child's thread suspend count to -1.
     * We can catch this in the resume-all code.
     *
     * Bouncing back and forth between threads like this adds a small amount
     * of scheduler overhead to thread startup.
     *
     * One alternative to having the child wait for the parent would be
     * to have the child inherit the parents' suspension count.  This
     * would work for a GC, since we can safely assume that the parent
     * thread didn't cause it, but we must only do so if the parent suspension
     * was caused by a suspend-all.  If the parent was being asked to
     * suspend singly by the debugger, the child should not inherit the value.
     *
     * We could also have a global "new thread suspend count" that gets
     * picked up by new threads before changing state to THREAD_RUNNING.
     * This would be protected by the thread list lock and set by a
     * suspend-all.
     */
    NativeLockThreadList(self);
    // assert(self->mStatus == NTHREAD_RUNNING);
    // self->mStatus = NTHREAD_VMWAIT;
    while (newThread->mStatus != NTHREAD_STARTING) {
        pthread_cond_wait(&gDvm.mThreadStartCond, &gDvm.mThreadListLock);
    }
printf("======function: %s======, ======line: %d======\n", __FUNCTION__, __LINE__);
    // LOG_THREAD("threadid=%d: adding to list\n", newThread->threadId);
    if (gDvm.mThreadList != NULL) {
        newThread->mNext = gDvm.mThreadList->mNext;
        newThread->mPrev = gDvm.mThreadList;
        gDvm.mThreadList->mNext = newThread;
    }
    else {
        gDvm.mThreadList = newThread;
    }
    //newThread->mNext = gDvm.mThreadList->mNext;
    if (newThread->mNext != NULL) {
        newThread->mNext->mPrev = newThread;
    }
    // newThread->mPrev = gDvm.mThreadList;
    // gDvm.mThreadList->mNext = newThread;
printf("======function: %s======, ======line: %d======\n", __FUNCTION__, __LINE__);
    // if (!dvmGetFieldBoolean(threadObj, gDvm.offJavaLangThread_daemon)) {
    //     gDvm.nonDaemonThreadCount++;        // guarded by thread list lock
    // }

    NativeUnlockThreadList();

    /* change status back to RUNNING, self-suspending if necessary */
    //NativeChangeStatus(self, NTHREAD_RUNNING);

    /*
     * Tell the new thread to start.
     *
     * We must hold the thread list lock before messing with another thread.
     * In the general case we would also need to verify that newThread was
     * still in the thread list, but in our case the thread has not started
     * executing user code and therefore has not had a chance to exit.
     *
     * We move it to VMWAIT, and it then shifts itself to RUNNING, which
     * comes with a suspend-pending check.
     */
    NativeLockThreadList(self);

    assert(newThread->mStatus == NTHREAD_STARTING);
    newThread->mStatus = NTHREAD_VMWAIT;
    pthread_cond_broadcast(&gDvm.mThreadStartCond);
printf("======function: %s======, ======line: %d======\n", __FUNCTION__, __LINE__);
    NativeUnlockThreadList();

    // DvmReleaseTrackedAlloc(vmThreadObj, NULL);
    return TRUE;

fail:
printf("======function: %s======, ======line: %d======\n", __FUNCTION__, __LINE__);
    FreeThread(newThread);
    // dvmReleaseTrackedAlloc(vmThreadObj, NULL);
    return FALSE;
}

/*
 * pthread entry function for threads started from interpreted code.
 */
static void* ThreadEntry(void* arg)
{
    NativeThread* self = (NativeThread*)arg;

    const char* threadName = self->mThreadObj->mName.string();
    SetThreadName(threadName);

    /*
     * Finish initializing the Thread struct.
     */
    NativeLockThreadList(self);
    PrepareThread(self);

    // LOG_THREAD("threadid=%d: created from interp\n", self->threadId);

    /*
     * Change our status and wake our parent, who will add us to the
     * thread list and advance our state to VMWAIT.
     */
    self->mStatus = NTHREAD_STARTING;
    pthread_cond_broadcast(&gDvm.mThreadStartCond);
printf("======function: %s======, ======line: %d======\n", __FUNCTION__, __LINE__);
    /*
     * Wait until the parent says we can go.  Assuming there wasn't a
     * suspend pending, this will happen immediately.  When it completes,
     * we're full-fledged citizens of the VM.
     *
     * We have to use THREAD_VMWAIT here rather than THREAD_RUNNING
     * because the pthread_cond_wait below needs to reacquire a lock that
     * suspend-all is also interested in.  If we get unlucky, the parent could
     * change us to THREAD_RUNNING, then a GC could start before we get
     * signaled, and suspend-all will grab the thread list lock and then
     * wait for us to suspend.  We'll be in the tail end of pthread_cond_wait
     * trying to get the lock.
     */
    while (self->mStatus != NTHREAD_VMWAIT) {
        pthread_cond_wait(&gDvm.mThreadStartCond, &gDvm.mThreadListLock);
    }
printf("======function: %s======, ======line: %d======\n", __FUNCTION__, __LINE__);
    NativeUnlockThreadList();

    /*
     * Add a JNI context.
     */
    // self->jniEnv = dvmCreateJNIEnv(self);

    /*
     * Change our state so the GC will wait for us from now on.  If a GC is
     * in progress this call will suspend us.
     */
    NativeChangeStatus(self, NTHREAD_RUNNING);
printf("======function: %s======, ======line: %d======\n", __FUNCTION__, __LINE__);
    /*
     * Notify the debugger & DDM.  The debugger notification may cause
     * us to suspend ourselves (and others).
     */
    // if (gDvm.debuggerConnected) {
    //     dvmDbgPostThreadStart(self);
    // }

    /*
     * Set the system thread priority according to the Thread object's
     * priority level.  We don't usually need to do this, because both the
     * Thread object and system thread priorities inherit from parents.  The
     * tricky case is when somebody creates a Thread object, calls
     * setPriority(), and then starts the thread.  We could manage this with
     * a "needs priority update" flag to avoid the redundant call.
     */
    Int32 priority = self->mThreadObj->mPriority;
    NativeChangeThreadPriority(self, priority);
printf("======function: %s======, ======line: %d======\n", __FUNCTION__, __LINE__);
    /*
     * Execute the "run" method.
     *
     * At this point our stack is empty, so somebody who comes looking for
     * stack traces right now won't have much to look at.  This is normal.
     */
    // Method* run = self->threadObj->clazz->vtable[gDvm.voffJavaLangThread_run];
    // JValue unused;

    // LOGV("threadid=%d: calling run()\n", self->threadId);
    // assert(strcmp(run->name, "run") == 0);
    // dvmCallMethod(self, run, self->threadObj, &unused);
    ECode ec = self->mThreadObj->Run();
    // LOGV("threadid=%d: exiting\n", self->threadId);

    /*
     * Remove the thread from various lists, report its death, and free
     * its resources.
     */
    // NativeDetachCurrentThread();

    return NULL;
}

/*
 * Detach the thread from the various data structures, notify other threads
 * that are waiting to "join" it, and free up all heap-allocated storage.
 *
 * Used for all threads.
 *
 * When we get here the interpreted stack should be empty.  The JNI 1.6 spec
 * requires us to enforce this for the DetachCurrentThread call, probably
 * because it also says that DetachCurrentThread causes all monitors
 * associated with the thread to be released.  (Because the stack is empty,
 * we only have to worry about explicit JNI calls to MonitorEnter.)
 *
 * THOUGHT:
 * We might want to avoid freeing our internal Thread structure until the
 * associated Thread/VMThread objects get GCed.  Our Thread is impossible to
 * get to once the thread shuts down, but there is a small possibility of
 * an operation starting in another thread before this thread halts, and
 * finishing much later (perhaps the thread got stalled by a weird OS bug).
 * We don't want something like Thread.isInterrupted() crawling through
 * freed storage.  Can do with a Thread finalizer, or by creating a
 * dedicated ThreadObject class for java/lang/Thread and moving all of our
 * state into that.
 */
void NativeDetachCurrentThread()
{
    NativeThread* self = NativeThreadSelf();
    // Object* vmThread;
    // Object* group;

    /*
     * Make sure we're not detaching a thread that's still running.  (This
     * could happen with an explicit JNI detach call.)
     *
     * A thread created by interpreted code will finish with a depth of
     * zero, while a JNI-attached thread will have the synthetic "stack
     * starter" native method at the top.
     */
    // int curDepth = dvmComputeExactFrameDepth(self->curFrame);
    // if (curDepth != 0) {
    //     bool topIsNative = false;

    //     if (curDepth == 1) {
    //         /* not expecting a lingering break frame; just look at curFrame */
    //         assert(!dvmIsBreakFrame(self->curFrame));
    //         StackSaveArea* ssa = SAVEAREA_FROM_FP(self->curFrame);
    //         if (dvmIsNativeMethod(ssa->method))
    //             topIsNative = true;
    //     }

    //     if (!topIsNative) {
    //         LOGE("ERROR: detaching thread with interp frames (count=%d)\n",
    //             curDepth);
    //         dvmDumpThread(self, false);
    //         dvmAbort();
    //     }
    // }

    // group = dvmGetFieldObject(self->threadObj, gDvm.offJavaLangThread_group);
    // LOG_THREAD("threadid=%d: detach (group=%p)\n", self->threadId, group);

    /*
     * Release any held monitors.  Since there are no interpreted stack
     * frames, the only thing left are the monitors held by JNI MonitorEnter
     * calls.
     */
    // dvmReleaseJniMonitors(self);

    /*
     * Do some thread-exit uncaught exception processing if necessary.
     */
    // if (dvmCheckException(self))
    //     threadExitUncaughtException(self, group);

    /*
     * Remove the thread from the thread group.
     */
    // if (group != NULL) {
    //     Method* removeThread =
    //         group->clazz->vtable[gDvm.voffJavaLangThreadGroup_removeThread];
    //     JValue unused;
    //     dvmCallMethod(self, removeThread, group, &unused, self->threadObj);
    // }

    /*
     * Clear the vmThread reference in the Thread object.  Interpreted code
     * will now see that this Thread is not running.  As this may be the
     * only reference to the VMThread object that the VM knows about, we
     * have to create an internal reference to it first.
     */
    // vmThread = dvmGetFieldObject(self->threadObj,
    //                 gDvm.offJavaLangThread_vmThread);
    // dvmAddTrackedAlloc(vmThread, self);
    // dvmSetFieldObject(self->threadObj, gDvm.offJavaLangThread_vmThread, NULL);

    /* clear out our struct Thread pointer, since it's going away */
    // dvmSetFieldObject(vmThread, gDvm.offJavaLangVMThread_vmData, NULL);

    /*
     * Tell the debugger & DDM.  This may cause the current thread or all
     * threads to suspend.
     *
     * The JDWP spec is somewhat vague about when this happens, other than
     * that it's issued by the dying thread, which may still appear in
     * an "all threads" listing.
     */
    // if (gDvm.debuggerConnected)
    //     dvmDbgPostThreadDeath(self);

    /*
     * Thread.join() is implemented as an Object.wait() on the VMThread
     * object.  Signal anyone who is waiting.
     */
    // dvmLockObject(self, vmThread);
    // dvmObjectNotifyAll(self, vmThread);
    // dvmUnlockObject(self, vmThread);

    // dvmReleaseTrackedAlloc(vmThread, self);
    // vmThread = NULL;

    /*
     * We're done manipulating objects, so it's okay if the GC runs in
     * parallel with us from here out.  It's important to do this if
     * profiling is enabled, since we can wait indefinitely.
     */
    android_atomic_release_store(NTHREAD_VMWAIT, (int32_t*)&self->mStatus);

    /*
     * If we're doing method trace profiling, we don't want threads to exit,
     * because if they do we'll end up reusing thread IDs.  This complicates
     * analysis and makes it impossible to have reasonable output in the
     * "threads" section of the "key" file.
     *
     * We need to do this after Thread.join() completes, or other threads
     * could get wedged.  Since self->threadObj is still valid, the Thread
     * object will not get GCed even though we're no longer in the ThreadGroup
     * list (which is important since the profiling thread needs to get
     * the thread's name).
     */
//    MethodTraceState* traceState = &gDvm.methodTrace;
//
//    dvmLockMutex(&traceState->startStopLock);
//    if (traceState->traceEnabled) {
//        LOGI("threadid=%d: waiting for method trace to finish\n",
//            self->threadId);
//        while (traceState->traceEnabled) {
//            dvmWaitCond(&traceState->threadExitCond,
//                        &traceState->startStopLock);
//        }
//    }
//    dvmUnlockMutex(&traceState->startStopLock);

    NativeLockThreadList(self);

    /*
     * Lose the JNI context.
     */
    // dvmDestroyJNIEnv(self->jniEnv);
    // self->jniEnv = NULL;

    self->mStatus = NTHREAD_ZOMBIE;

    /*
     * Remove ourselves from the internal thread list.
     */
    UnlinkThread(self);

    /*
     * If we're the last one standing, signal anybody waiting in
     * DestroyJavaVM that it's okay to exit.
     */
//    if (!dvmGetFieldBoolean(self->threadObj, gDvm.offJavaLangThread_daemon)) {
//        gDvm.nonDaemonThreadCount--;        // guarded by thread list lock
//
//        if (gDvm.nonDaemonThreadCount == 0) {
//            int cc;
//
//            LOGV("threadid=%d: last non-daemon thread\n", self->threadId);
//            //dvmDumpAllThreads(false);
//            // cond var guarded by threadListLock, which we already hold
//            cc = pthread_cond_signal(&gDvm.vmExitCond);
//            assert(cc == 0);
//        }
//    }

    // LOGV("threadid=%d: bye!\n", self->threadId);
    ReleaseThreadId(self);
    NativeUnlockThreadList();

    SetThreadSelf(NULL);

    FreeThread(self);
}

/*
 * Check to see if we need to suspend ourselves.  If so, go to sleep on
 * a condition variable.
 *
 * Returns "true" if we suspended ourselves.
 */
static Boolean FullSuspendCheck(
    /* [in] */ NativeThread* self)
{
    assert(self != NULL);
    assert(self->mSuspendCount >= 0);

    /*
     * Grab gDvm.threadSuspendCountLock.  This gives us exclusive write
     * access to self->suspendCount.
     */
//    lockThreadSuspendCount();   /* grab gDvm.threadSuspendCountLock */

    Boolean needSuspend = (self->mSuspendCount != 0);
    if (needSuspend) {
//        LOG_THREAD("threadid=%d: self-suspending\n", self->threadId);
//        ThreadStatus oldStatus = self->status;      /* should be RUNNING */
//        self->status = THREAD_SUSPENDED;
//
//        while (self->suspendCount != 0) {
//            /*
//             * Wait for wakeup signal, releasing lock.  The act of releasing
//             * and re-acquiring the lock provides the memory barriers we
//             * need for correct behavior on SMP.
//             */
//            dvmWaitCond(&gDvm.threadSuspendCountCond,
//                    &gDvm.threadSuspendCountLock);
//        }
//        assert(self->suspendCount == 0 && self->dbgSuspendCount == 0);
//        self->status = oldStatus;
//        LOG_THREAD("threadid=%d: self-reviving, status=%d\n",
//            self->threadId, self->status);
    }

//    unlockThreadSuspendCount();

    return needSuspend;
}

/*
 * Update our status.
 *
 * The "self" argument, which may be NULL, is accepted as an optimization.
 *
 * Returns the old status.
 */
NativeThreadStatus NativeChangeStatus(
    /* [in] */ NativeThread* self,
    /* [in] */ NativeThreadStatus newStatus)
{
    NativeThreadStatus oldStatus;

    if (self == NULL) {
        self = NativeThreadSelf();
    }

    // LOGVV("threadid=%d: (status %d -> %d)\n",
    //     self->mThreadId, self->mStatus, newStatus);

    oldStatus = self->mStatus;

    if (newStatus == NTHREAD_RUNNING) {
        /*
         * Change our status to THREAD_RUNNING.  The transition requires
         * that we check for pending suspension, because the VM considers
         * us to be "asleep" in all other states, and another thread could
         * be performing a GC now.
         *
         * The order of operations is very significant here.  One way to
         * do this wrong is:
         *
         *   GCing thread                   Our thread (in NATIVE)
         *   ------------                   ----------------------
         *                                  check suspend count (== 0)
         *   dvmSuspendAllThreads()
         *   grab suspend-count lock
         *   increment all suspend counts
         *   release suspend-count lock
         *   check thread state (== NATIVE)
         *   all are suspended, begin GC
         *                                  set state to RUNNING
         *                                  (continue executing)
         *
         * We can correct this by grabbing the suspend-count lock and
         * performing both of our operations (check suspend count, set
         * state) while holding it, now we need to grab a mutex on every
         * transition to RUNNING.
         *
         * What we do instead is change the order of operations so that
         * the transition to RUNNING happens first.  If we then detect
         * that the suspend count is nonzero, we switch to SUSPENDED.
         *
         * Appropriate compiler and memory barriers are required to ensure
         * that the operations are observed in the expected order.
         *
         * This does create a small window of opportunity where a GC in
         * progress could observe what appears to be a running thread (if
         * it happens to look between when we set to RUNNING and when we
         * switch to SUSPENDED).  At worst this only affects assertions
         * and thread logging.  (We could work around it with some sort
         * of intermediate "pre-running" state that is generally treated
         * as equivalent to running, but that doesn't seem worthwhile.)
         *
         * We can also solve this by combining the "status" and "suspend
         * count" fields into a single 32-bit value.  This trades the
         * store/load barrier on transition to RUNNING for an atomic RMW
         * op on all transitions and all suspend count updates (also, all
         * accesses to status or the thread count require bit-fiddling).
         * It also eliminates the brief transition through RUNNING when
         * the thread is supposed to be suspended.  This is possibly faster
         * on SMP and slightly more correct, but less convenient.
         */
        assert(oldStatus != NTHREAD_RUNNING);
        android_atomic_acquire_store(newStatus, (int32_t*)&self->mStatus);
        if (self->mSuspendCount != 0) {
            FullSuspendCheck(self);
        }
    }
    else {
        /*
         * Not changing to THREAD_RUNNING.  No additional work required.
         *
         * We use a releasing store to ensure that, if we were RUNNING,
         * any updates we previously made to objects on the managed heap
         * will be observed before the state change.
         */
        assert(newStatus != NTHREAD_SUSPENDED);
        android_atomic_release_store(newStatus, (int32_t*)&self->mStatus);
    }

    return oldStatus;
}

/*
 * Given a VMThread object, return the associated Thread*.
 *
 * NOTE: if the thread detaches, the struct Thread will disappear, and
 * we will be touching invalid data.  For safety, lock the thread list
 * before calling this.
 */
NativeThread* NativeGetThreadFromThreadObject(
    /* [in] */ Thread* threadObj)
{
    // Int32 vmData;

    // vmData = dvmGetFieldInt(vmThreadObj, gDvm.offJavaLangVMThread_vmData);
    assert(threadObj != NULL);
    NativeThread* thread = threadObj->mNativeThread;
    if (FALSE) {
        thread = gDvm.mThreadList;
        while (thread != NULL) {
            if (threadObj == thread->mThreadObj)
                break;

            thread = thread->mNext;
        }
        // if (thread == NULL) {
        //     //LOGW("WARNING: vmThreadObj=%p has thread=%p, not in thread list\n",
        //     //    vmThreadObj, (Thread*)vmData);
        //     vmData = 0;
        // }
    }

    return thread;
}

/*
 * Conversion map for "nice" values.
 *
 * We use Android thread priority constants to be consistent with the rest
 * of the system.  In some cases adjacent entries may overlap.
 */
static const Int32 kNiceValues[10] = {
    ELASTOS_PRIORITY_LOWEST,                /* 1 (MIN_PRIORITY) */
    ELASTOS_PRIORITY_BACKGROUND + 6,
    ELASTOS_PRIORITY_BACKGROUND + 3,
    ELASTOS_PRIORITY_BACKGROUND,
    ELASTOS_PRIORITY_NORMAL,                /* 5 (NORM_PRIORITY) */
    ELASTOS_PRIORITY_NORMAL - 2,
    ELASTOS_PRIORITY_NORMAL - 4,
    ELASTOS_PRIORITY_URGENT_DISPLAY + 3,
    ELASTOS_PRIORITY_URGENT_DISPLAY + 2,
    ELASTOS_PRIORITY_URGENT_DISPLAY         /* 10 (MAX_PRIORITY) */
};

/*
 * Change the priority of a system thread to match that of the Thread object.
 *
 * We map a priority value from 1-10 to Linux "nice" values, where lower
 * numbers indicate higher priority.
 */
void NativeChangeThreadPriority(
    /* [in] */ NativeThread* thread,
    /* [in] */ Int32 newPriority)
{
    pid_t pid = thread->mSystemTid;
    Int32 newNice;

    if (newPriority < 1 || newPriority > 10) {
        // LOGW("bad priority %d\n", newPriority);
        newPriority = 5;
    }
    newNice = kNiceValues[newPriority - 1];

    if (newNice >= ELASTOS_PRIORITY_BACKGROUND) {
        set_sched_policy(NativeGetSysThreadId(), SP_BACKGROUND);
    }
    else if (getpriority(PRIO_PROCESS, pid) >= ELASTOS_PRIORITY_BACKGROUND) {
        set_sched_policy(NativeGetSysThreadId(), SP_FOREGROUND);
    }

    if (setpriority(PRIO_PROCESS, pid, newNice) != 0) {
        const char* str = thread->mThreadObj->mName.string();
        // LOGI("setPriority(%d) '%s' to prio=%d(n=%d) failed: %s\n",
        //     pid, str, newPriority, newNice, strerror(errno));
    }
    else {
        // LOGV("setPriority(%d) to prio=%d(n=%d)\n",
        //     pid, newPriority, newNice);
    }
}

/*
 * Implement java.lang.Thread.interrupt().
 */
void NativeThreadInterrupt(
    /* [in] */ NativeThread* thread)
{
    assert(thread != NULL);

    NativeLockMutex(&thread->mWaitMutex);

    /*
     * If the interrupted flag is already set no additional action is
     * required.
     */
    if (thread->mInterrupted == TRUE) {
        NativeUnlockMutex(&thread->mWaitMutex);
        return;
    }

    /*
     * Raise the "interrupted" flag.  This will cause it to bail early out
     * of the next wait() attempt, if it's not currently waiting on
     * something.
     */
    thread->mInterrupted = TRUE;

    /*
     * Is the thread waiting?
     *
     * Note that fat vs. thin doesn't matter here;  waitMonitor
     * is only set when a thread actually waits on a monitor,
     * which implies that the monitor has already been fattened.
     */
    // if (thread->mWaitMonitor != NULL) {
    //     pthread_cond_signal(&thread->mWaitCond);
    // }

    NativeUnlockMutex(&thread->mWaitMutex);
}
