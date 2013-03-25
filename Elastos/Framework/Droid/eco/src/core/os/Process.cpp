
#include "os/Process.h"
#include <elastos/Thread.h>
#include <elastos/Vector.h>
#include <Elastos.IO.h>
#include <Elastos.Core.h>
#include <unistd.h>
#include <sys/resource.h>
#include <pwd.h>
#include <grp.h>
#include <pthread.h>
#include <utils/Errors.h>
#include "utils/threads.h"
#include <errno.h>
#include <binder/ProcessState.h>
#include <fcntl.h>
#include <utils/String8.h>
#include <dirent.h>

#ifdef _FRAMEWORK_CORE
#include "net/CLocalSocket.h"
#include "net/CLocalSocketAddress.h"
#endif

#define GUARD_THREAD_PRIORITY 0

#if GUARD_THREAD_PRIORITY
Mutex gKeyCreateMutex;
static pthread_key_t gBgKey = -1;
#endif

AutoPtr<ILocalSocket> Process::sZygoteSocket;
AutoPtr<IDataInputStream> Process::sZygoteInputStream;
AutoPtr<IBufferedWriter> Process::sZygoteWriter;

/** true if previous zygote open failed */
Boolean Process::sPreviousZygoteOpenFailed;

Mutex Process::mProcessClassLock;

Process::ProcessRunnable::ProcessRunnable(
    /* [in] */ String processClass)
{
    mProcessClass = processClass;
}

UInt32 Process::ProcessRunnable::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 Process::ProcessRunnable::Release()
{
    return ElRefBase::Release();
}

PInterface Process::ProcessRunnable::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IRunnable*)this;
    }
    else if (riid == EIID_IRunnable) {
        return (IRunnable*)this;
    }

    return NULL;
}

CARAPI Process::ProcessRunnable::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [in] */ InterfaceID* pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode Process::ProcessRunnable::Run()
{
    Process::InvokeStaticMain(mProcessClass);
    return NOERROR;
}


Int32 Process::GetCallingPid()
{
    //not implemented;
    return -1;
}

Int32 Process::GetCallingUid()
{
    //not implemented;
    return -1;
}
/**
* Start a new process.
*
* <p>If processes are enabled, a new process is created and the
* static main() function of a <var>processClass</var> is executed there.
* The process will continue running after this function returns.
*
* <p>If processes are not enabled, a new thread in the caller's
* process is created and main() of <var>processClass</var> called there.
*
* <p>The niceName parameter, if not an empty string, is a custom name to
* give to the process instead of using processClass.  This allows you to
* make easily identifyable processes even if you are using the same base
* <var>processClass</var> to start them.
*
* @param processClass The class to use as the process's main entry
*                     point.
* @param niceName A more readable name to use for the process.
* @param uid The user-id under which the process will run.
* @param gid The group-id under which the process will run.
* @param gids Additional group-ids associated with the process.
* @param enableDebugger True if debugging should be enabled for this process.
* @param zygoteArgs Additional arguments to supply to the zygote process.
*
* @return int If > 0 the pid of the new process; if 0 the process is
*         being emulated by a thread
* @throws RuntimeException on fatal start failure
*
* {@hide}
*/

ECode Process::Start(
    /* [in] */ const String processClass,
    /* [in] */ const String niceName,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 gid,
    /* [in] */ const ArrayOf<Int32> & gids,
    /* [in] */ Int32 debugFlags,
    /* [in] */ const ArrayOf<String> & zygoteArgs,
    /* [out] */ Int32* pid)
{
    if (SupportsProcesses()) {
        ECode ec = StartViaZygote(
                    processClass,
                    niceName,
                    uid,
                    gid,
                    gids,
                    debugFlags,
                    zygoteArgs,
                    pid);

        if (FAILED(ec))
        {
            // Log.e(LOG_TAG,
            //         "Starting VM process through Zygote failed");
            // throw new RuntimeException(
            //         "Starting VM process through Zygote failed", ex);
            return E_RUNTIME_EXCEPTION;
        }
    }
    else {
        //Running in single-process mode
        ProcessRunnable* runnable = new ProcessRunnable(processClass);

        // Thread constructors must not be called with null names (see spec).
        IThread* thread;
        if (niceName.GetCharCount() != 0) {
            CThread::New(runnable, niceName, &thread);
            thread->Start();
        } else {
            CThread::New(runnable, &thread);
            thread->Start();
        }

        *pid = 0;
        delete runnable;
    }

    return NOERROR;
}

/**
* Start a new process.  Don't supply a custom nice name.
* {@hide}
*/
ECode Process::Start(
    /* [in] */ String processClass,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 gid,
    /* [in] */ const ArrayOf<Int32> & gids,
    /* [in] */ Int32 debugFlags,
    /* [in] */ const ArrayOf<String> & zygoteArgs,
    /* [in] */ Int32* pid)
{
    return Start(
            processClass,
            String(""),
            uid,
            gid,
            gids,
            debugFlags,
            zygoteArgs,
            pid);
}

ECode Process::InvokeStaticMain(
    /* [in] */ String className)
{
    //TODO:
    // Class cl;
    // Object args[] = new Object[1];

    // args[0] = new String[0];     //this is argv

    // try {
    //     cl = Class.forName(className);
    //     cl.getMethod("main", new Class[] { String[].class })
    //             .invoke(null, args);
    // } catch (Exception ex) {
    //     // can be: ClassNotFoundException,
    //     // NoSuchMethodException, SecurityException,
    //     // IllegalAccessException, IllegalArgumentException
    //     // InvocationTargetException
    //     // or uncaught exception from main()

    //     Log.e(LOG_TAG, "Exception invoking static main on "
    //             + className, ex);

    //     throw new RuntimeException(ex);
    // }

    return E_NOT_IMPLEMENTED;
}

/**
 * Tries to open socket to Zygote process if not already open. If
 * already open, does nothing.  May block and retry.
 */
ECode Process::OpenZygoteSocketIfNeeded()
{
    Int32 retryCount;

    if (sPreviousZygoteOpenFailed) {
        /*
         * If we've failed before, expect that we'll fail again and
         * don't pause for retries.
         */
        retryCount = 0;
    } else {
        retryCount = 10;
    }

    /*
     * See bug #811181: Sometimes runtime can make it up before zygote.
     * Really, we'd like to do something better to avoid this condition,
     * but for now just wait a bit...
     */
    for (Int32 retry = 0
            ; (sZygoteSocket == NULL) && (retry < (retryCount + 1))
            ; retry++ ) {

        if (retry > 0) {
            // try {
            //     Log.i("Zygote", "Zygote not up yet, sleeping...");
                Thread::Sleep(ZYGOTE_RETRY_MILLIS);
            // } catch (InterruptedException ex) {
            //     // should never happen
            // }
        }

    // try {
        ECode ec = CLocalSocket::New((ILocalSocket**)&sZygoteSocket);
        if (FAILED(ec)) {
            return ec;
        }

        ILocalSocketAddress* socketAddress;
        ec = CLocalSocketAddress::New(String("zygote"), LocalSocketAddressNamespace_RESERVED, &socketAddress);
        if (FAILED(ec)) {
            return ec;
        }

        sZygoteSocket->Connect(socketAddress);

        IInputStream* inputStream;
        sZygoteSocket->GetInputStream(&inputStream);

        ec = CDataInputStream::New(inputStream, (IDataInputStream**)&sZygoteInputStream);
        if (FAILED(ec)) {
            return ec;
        }

        IOutputStream* outputStream;
        sZygoteSocket->GetOutputStream(&outputStream);

        IOutputStreamWriter* streamWrite;
        ec = COutputStreamWriter::New(outputStream, &streamWrite);

        if (FAILED(ec)) {
            return ec;
        }

        ec = CBufferedWriter::New(streamWrite, 256, (IBufferedWriter**)&sZygoteWriter);

        if (FAILED(ec)) {
            return ec;
        }

        //Log.i("Zygote", "Process: zygote socket opened");
        sPreviousZygoteOpenFailed = FALSE;
        break;
    // } catch (IOException ex) {
    //     if (sZygoteSocket != null) {
    //         try {
    //             sZygoteSocket.close();
    //         } catch (IOException ex2) {
    //             Log.e(LOG_TAG,"I/O exception on close after exception",
    //                     ex2);
    //         }
    //     }

    //     sZygoteSocket = null;
    // }
    }

    if (sZygoteSocket == NULL) {
        sPreviousZygoteOpenFailed = TRUE;
        //throw new ZygoteStartFailedEx("connect failed");
        return E_FAIL;
    }
    return NOERROR;
}

/**
 * Sends an argument list to the zygote process, which starts a new child
 * and returns the child's pid. Please note: the present implementation
 * replaces newlines in the argument list with spaces.
 * @param args argument list
 * @return PID of new child process
 * @throws ZygoteStartFailedEx if process start failed for any reason
 */
Int32 Process::ZygoteSendArgsAndGetPid(
    /* [in] */ List<String>* args)
{
    Int32 pid;

    OpenZygoteSocketIfNeeded();

    // try {
        /**
         * See com.android.internal.os.ZygoteInit.readArgumentList()
         * Presently the wire format to the zygote process is:
         * a) a count of arguments (argc, in essence)
         * b) a number of newline-separated argument strings equal to count
         *
         * After the zygote process reads these it will write the pid of
         * the child or -1 on failure.
         */
        Int32 sz = args->GetSize();
        sZygoteWriter->Write(sz/*String::FromInt32(sz)*/);
        sZygoteWriter->NewLine();

        List<String>::Iterator it;
        for (it = args->Begin(); it != args->End(); it++) {
            String arg = *it;
            const char argChar = '\n';
            if (arg.Find(String(&argChar).string()) >= 0) {
                // throw new ZygoteStartFailedEx(
                //         "embedded newlines not allowed");
                return 0;
            }
            sZygoteWriter->Write(arg.ToInt32());
            sZygoteWriter->NewLine();
        }

        sZygoteWriter->Flush();

        // Should there be a timeout on this?
        ((IInputStream*)sZygoteInputStream)->Read(&pid);

        if (pid < 0) {
            //throw new ZygoteStartFailedEx("fork() failed");
            return 0;
        }
    // } catch (IOException ex) {
    //     try {
    //         if (sZygoteSocket != NULL) {
    //             sZygoteSocket->Close();
    //         }
    //     } catch (IOException ex2) {
    //         // we're going to fail anyway
    //         Log.e(LOG_TAG,"I/O exception on routine close", ex2);
    //     }

    //     sZygoteSocket = NULL;

    //     throw new ZygoteStartFailedEx(ex);
    // }

    return pid;
}

/**
 * Starts a new process via the zygote mechanism.
 *
 * @param processClass Class name whose static main() to run
 * @param niceName 'nice' process name to appear in ps
 * @param uid a POSIX uid that the new process should setuid() to
 * @param gid a POSIX gid that the new process shuold setgid() to
 * @param gids null-ok; a list of supplementary group IDs that the
 * new process should setgroup() to.
 * @param enableDebugger True if debugging should be enabled for this process.
 * @param extraArgs Additional arguments to supply to the zygote process.
 * @return PID
 * @throws ZygoteStartFailedEx if process start failed for any reason
 */
ECode Process::StartViaZygote(
    /* [in] */ const String processClass,
    /* [in] */ const String niceName,
    /* [in] */ const Int32 uid,
    /* [in] */ const Int32 gid,
    /* [in] */ const ArrayOf<Int32> & gids,
    /* [in] */ Int32 debugFlags,
    /* [in] */ const ArrayOf<String> & extraArgs,
    /* [out] */ Int32* pid)
{
    Mutex::Autolock lock(mProcessClassLock);
    List<String>* argsForZygote = new List<String>();
    // --runtime-init, --setuid=, --setgid=,
    // and --setgroups= must go first
    argsForZygote->PushBack(String("--runtime-init"));
    argsForZygote->PushBack(String("--setuid=" + uid));
    argsForZygote->PushBack(String("--setgid=" + gid));
    if ((debugFlags & (1)/*Zygote.DEBUG_ENABLE_SAFEMODE*/) != 0) {
        argsForZygote->PushBack(String("--enable-safemode"));
    }
    if ((debugFlags & (1 << 1)/*Zygote.DEBUG_ENABLE_DEBUGGER*/) != 0) {
        argsForZygote->PushBack(String("--enable-debugger"));
    }
    if ((debugFlags & (1 << 2)/*Zygote.DEBUG_ENABLE_CHECKJNI*/) != 0) {
        argsForZygote->PushBack(String("--enable-checkjni"));
    }
    if ((debugFlags & (1 << 3)/*Zygote.DEBUG_ENABLE_ASSERT*/) != 0) {
        argsForZygote->PushBack(String("--enable-assert"));
    }

    //TODO optionally enable debuger
    //argsForZygote.add("--enable-debugger");

    // --setgroups is a comma-separated list
    Int32 sz = gids.GetLength();
    if (sz > 0) {
        StringBuf* sb = StringBuf::Alloc(0);
        sb->Append("--setgroups=");

        for (Int32 i = 0; i < sz; i++) {
            if (i != 0) {
                sb->Append(',');
            }
            sb->Append(gids[i]);
        }

        argsForZygote->PushBack(String(sb->GetPayload()));
        StringBuf::Free(sb);
    }

    if (niceName != NULL) {
        argsForZygote->PushBack(String("--nice-name=") + niceName);
    }

    argsForZygote->PushBack(processClass);

    sz = extraArgs.GetLength();
    for (Int32 i = 0; i < sz; i++) {
        String arg = extraArgs[i];
        argsForZygote->PushBack(arg);
    }

    *pid = ZygoteSendArgsAndGetPid(argsForZygote);
    delete argsForZygote;

    if (*pid <= 0) {
        //throw new ZygoteStartFailedEx("zygote start failed:" + pid);
        return E_FAIL;
    }

    return NOERROR;
}

/**
 * Returns elapsed milliseconds of the time this process has run.
 * @return  Returns the number of milliseconds this process has return.
 */
Int64 Process::GetElapsedCpuTime()
{
    struct timespec ts;
    Int32 res = clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);
    if (res != 0) {
        return (Int64) 0;
    }

    nsecs_t when = seconds_to_nanoseconds(ts.tv_sec) + ts.tv_nsec;
    return (Int64) nanoseconds_to_milliseconds(when);
}

/**
 * Returns the identifier of this process, which can be used with
 * {@link #killProcess} and {@link #sendSignal}.
 */
Int32 Process::MyPid()
{
    return getpid();
}

/**
 * Returns the identifier of the calling thread, which be used with
 * {@link #setThreadPriority(int, int)}.
 */
Int32 Process::MyTid()
{
    //TODO:
    return androidGetTid();
}

/**
 * Returns the identifier of this process's user.
 */
Int32 Process::MyUid()
{
    return getuid();
}

/**
 * Returns the UID assigned to a particular user name, or -1 if there is
 * none.  If the given string consists of only numbers, it is converted
 * directly to a uid.
 */
Int32 Process::GetUidForName(
    /* [in] */ String name)
{
    if (name == NULL) {
        //jniThrowException(env, "java/lang/NullPointerException", NULL);
        return E_NULL_POINTER_EXCEPTION;
    }

    //const jchar* str16 = env->GetStringCritical(name, 0);
    android::String8 name8(name.string());
    // if (str16) {
    //     name8 = String8(str16, env->GetStringLength(name));
    //     env->ReleaseStringCritical(name, str16);
    // }

    const size_t N = name8.size();
    if (N > 0) {
        const char* str = name8.string();
        for (size_t i = 0; i < N; i++) {
            if (str[i] < '0' || str[i] > '9') {
                struct passwd* pwd = getpwnam(str);
                if (pwd == NULL) {
                    return -1;
                }
                return pwd->pw_uid;
            }
        }
        return atoi(str);
    }
    return -1;

}

/**
 * Returns the GID assigned to a particular user name, or -1 if there is
 * none.  If the given string consists of only numbers, it is converted
 * directly to a gid.
 */
Int32 Process::GetGidForName(
    /* [in] */ String name)
{
    if (name == NULL) {
        //jniThrowException(env, "java/lang/NullPointerException", NULL);
        return E_NULL_POINTER_EXCEPTION;
    }

    // const jchar* str16 = env->GetStringCritical(name, 0);
    android::String8 name8(name.string());
    // if (str16) {
    //     name8 = String8(str16, env->GetStringLength(name));
    //     env->ReleaseStringCritical(name, str16);
    // }

    const size_t N = name8.size();
    if (N > 0) {
        const char* str = name8.string();
        for (size_t i = 0; i < N; i++) {
            if (str[i] < '0' || str[i] > '9') {
                struct group* grp = getgrnam(str);
                if (grp == NULL) {
                    return -1;
                }
                return grp->gr_gid;
            }
        }
        return atoi(str);
    }
    return -1;
}

/**
 * Returns a uid for a currently running process.
 * @param pid the process id
 * @return the uid of the process, or -1 if the process is not running.
 * @hide pending API council review
 */
Int32 Process::GetUidForPid(
    /* [in] */ Int32 pid)
{
    ArrayOf<String>* procStatusLabels = ArrayOf<String>::Alloc(1);
    procStatusLabels->GetPayload()[0] = "Uid";
    ArrayOf<Int64>* procStatusValues = ArrayOf<Int64>::Alloc(1);
    procStatusValues->GetPayload()[0] = -1;

    StringBuf* sbuf = StringBuf::Alloc(0);
    sbuf->Append("/proc/");
    sbuf->Append(pid);
    sbuf->Append("/status");
    ReadProcLines(String(sbuf->GetPayload()), *procStatusLabels, *procStatusValues);

    Int32 value = procStatusValues->GetPayload()[0];
    StringBuf::Free(sbuf);
    ArrayOf<String>::Free(procStatusLabels);
    ArrayOf<Int64>::Free(procStatusValues);
    return value;
}

static ECode SignalExceptionForPriorityError(Int32 err)
{
    switch (err) {
        case EINVAL:
            //jniThrowException(env, "java/lang/IllegalArgumentException", NULL);
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        case ESRCH:
            //jniThrowException(env, "java/lang/IllegalArgumentException", "Given thread does not exist");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        case EPERM:
            //jniThrowException(env, "java/lang/SecurityException", "No permission to modify given thread");
            return E_SECURITY_EXCEPTION;
        case EACCES:
            //jniThrowException(env, "java/lang/SecurityException", "No permission to set to given priority");
            return E_SECURITY_EXCEPTION;
        default:
            //jniThrowException(env, "java/lang/RuntimeException", "Unknown error");
            return E_RUNTIME_EXCEPTION;
    }
}

static ECode SignalExceptionForGroupError(Int32 err)
{
    switch (err) {
        case EINVAL:
            //jniThrowException(env, "java/lang/IllegalArgumentException", NULL);
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        case ESRCH:
            //jniThrowException(env, "java/lang/IllegalArgumentException", "Given thread does not exist");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        case EPERM:
            //jniThrowException(env, "java/lang/SecurityException", "No permission to modify given thread");
            return E_SECURITY_EXCEPTION;
        case EACCES:
            //jniThrowException(env, "java/lang/SecurityException", "No permission to set to given group");
            return E_SECURITY_EXCEPTION;
        default:
            //jniThrowException(env, "java/lang/RuntimeException", "Unknown error");
            return E_RUNTIME_EXCEPTION;
    }
}

/**
 * Set the priority of a thread, based on Linux priorities.
 *
 * @param tid The identifier of the thread/process to change.
 * @param priority A Linux priority level, from -20 for highest scheduling
 * priority to 19 for lowest scheduling priority.
 *
 * @throws IllegalArgumentException Throws IllegalArgumentException if
 * <var>tid</var> does not exist.
 * @throws SecurityException Throws SecurityException if your process does
 * not have permission to modify the given thread, or to use the given
 * priority.
 */
ECode Process::SetThreadPriority(
    /* [in] */ Int32 tid,
    /* [in] */ Int32 priority)
{
#if GUARD_THREAD_PRIORITY
    // if we're putting the current thread into the background, check the TLS
    // to make sure this thread isn't guarded.  If it is, raise an exception.
    if (priority >= ANDROID_PRIORITY_BACKGROUND) {
        if (tid == androidGetTid()) {
            void* bgOk = pthread_getspecific(gBgKey);
            if (bgOk == ((void*)0xbaad)) {
                //LOGE("Thread marked fg-only put self in background!");
                //jniThrowException(env, "java/lang/SecurityException", "May not put this thread into background");
                return E_SECURITY_EXCEPTION;
            }
        }
    }
#endif

    Int32 rc = androidSetThreadPriority(tid, priority);
    if (rc != 0) {
        if (rc == android::INVALID_OPERATION) {
            return SignalExceptionForPriorityError(errno);
        } else {
            return SignalExceptionForGroupError(errno);
        }
    }
    //LOGI("Setting priority of %d: %d, getpriority returns %d\n",
    //     pid, pri, getpriority(PRIO_PROCESS, pid));
    return NOERROR;
}
/**
 * Call with 'false' to cause future calls to {@link #setThreadPriority(int)} to
 * throw an exception if passed a background-level thread priority.  This is only
 * effective if the JNI layer is built with GUARD_THREAD_PRIORITY defined to 1.
 *
 * @hide
 */
ECode Process::SetCanSelfBackground(
    /* [in] */ Boolean backgroundOk)
{
#if GUARD_THREAD_PRIORITY
    //LOGV("Process.setCanSelfBackground(%d) : tid=%d", bgOk, androidGetTid());
    {
        Mutex::Autolock _l(gKeyCreateMutex);
        if (gBgKey == -1) {
            pthread_key_create(&gBgKey, NULL);
        }
    }

    // inverted:  not-okay, we set a sentinel value
    pthread_setspecific(gBgKey, (void*)(backgroundOk ? 0 : 0xbaad));
#endif
    return NOERROR;
}

ECode Process::SetThreadPriority(
    /* [in] */ Int32 priority)
{
    Int32 tid = MyTid();
    return SetThreadPriority(tid, priority);
}

ECode Process::SetProcessGroup(
    /* [in] */ Int32 pid,
    /* [in] */ Int32 group)
{
    Int32 res = androidSetThreadSchedulingGroup(pid, group);
    if (res != android::NO_ERROR) {
        return SignalExceptionForGroupError(res == android::BAD_VALUE ? EINVAL : errno);
    }

    return NOERROR;
}

/**
 * Return the current priority of a thread, based on Linux priorities.
 *
 * @param tid The identifier of the thread/process to change.
 *
 * @return Returns the current priority, as a Linux priority level,
 * from -20 for highest scheduling priority to 19 for lowest scheduling
 * priority.
 *
 * @throws IllegalArgumentException Throws IllegalArgumentException if
 * <var>tid</var> does not exist.
 */
Int32 Process::GetThreadPriority(
    /* [in] */ Int32 tid)
{
    errno = 0;
    Int32 pri = getpriority(PRIO_PROCESS, tid);
    if (errno != 0) {
        SignalExceptionForPriorityError(errno);
    }
    //LOGI("Returning priority of %d: %d\n", pid, pri);
    return pri;
}

Boolean Process::SupportsProcesses()
{
    return android::ProcessState::self()->supportsProcesses();
}

Boolean Process::SetOomAdj(
    /* [in] */ Int32 pid,
    /* [in] */ Int32 amt)
{
#ifdef HAVE_OOM_ADJ
    if (SupportsProcesses()) {
        char text[64];
        sprintf(text, "/proc/%d/oom_adj", pid);
        Int32 fd = open(text, O_WRONLY);
        if (fd >= 0) {
            sprintf(text, "%d", amt);
            write(fd, text, strlen(text));
            close(fd);
        }
        return TRUE;
    }
#endif
    return FALSE;
}

/**
 * Change this process's argv[0] parameter.  This can be useful to show
 * more descriptive information in things like the 'ps' command.
 *
 * @param text The new name of this process.
 *
 * {@hide}
 */
ECode Process::SetArgV0(
    /* [in] */ String name)
{
    if (name == NULL) {
        //jniThrowException(env, "java/lang/NullPointerException", NULL);
        return E_NULL_POINTER_EXCEPTION;
    }

    // const jchar* str = env->GetStringCritical(name, 0);
    android::String8 name8;
    // if (str) {
    //     name8 = String8(str, env->GetStringLength(name));
    //     env->ReleaseStringCritical(name, str);
    // }

    if (name8.size() > 0) {
        android::ProcessState::self()->setArgV0(name8.string());
    }

    return NOERROR;
}

ECode Process::KillProcess(
    /* [in] */ Int32 pid)
{
    return SendSignal(pid, SIGNAL_KILL);
}

/** @hide */
Int32 Process::SetUid(
    /* [in] */ Int32 uid)
{
#if HAVE_ANDROID_OS
    return setuid(uid) == 0 ? 0 : errno;
#else
    return ENOSYS;
#endif
}

/** @hide */
Int32 Process::SetGid(
    /* [in] */ Int32 uid)
{
#if HAVE_ANDROID_OS
    return setgid(uid) == 0 ? 0 : errno;
#else
    return ENOSYS;
#endif
}

/**
 * Send a signal to the given process.
 *
 * @param pid The pid of the target process.
 * @param signal The signal to send.
 */
ECode Process::SendSignal(
    /* [in] */ Int32 pid,
    /* [in] */ Int32 signal)
{
    if (pid > 0) {
        //LOGI("Sending signal. PID: %d SIG: %d", pid, sig);
        kill(pid, signal);
        return NOERROR;
    }

    return E_FAIL;
}

ECode Process::KillProcessQuiet(
    /* [in] */ Int32 pid)
{
    return SendSignalQuiet(pid, SIGNAL_KILL);;
}

/**
 * @hide
 * Private impl for avoiding a log message...  DO NOT USE without doing
 * your own log, or the Android Illuminati will find you some night and
 * beat you up.
 */
ECode Process::SendSignalQuiet(
    /* [in] */ Int32 pid,
    /* [in] */ Int32 signal)
{
    if (pid > 0) {
        kill(pid, signal);
        return NOERROR;
    }

    return E_FAIL;
}

/** @hide */
Int64 Process::GetFreeMemory()
{
    Int32 fd = open("/proc/meminfo", O_RDONLY);
    if (fd < 0) {
        //LOGW("Unable to open /proc/meminfo");
        return -1;
    }

    char buffer[256];
    const Int32 len = read(fd, buffer, sizeof(buffer)-1);
    close(fd);

    if (len < 0) {
        //LOGW("Unable to read /proc/meminfo");
        return -1;
    }
    buffer[len] = 0;

    Int32 numFound = 0;
    Int64 mem = 0;

    static const char* const sums[] = { "MemFree:", "Cached:", NULL };
    static const Int32 sumsLen[] = { strlen("MemFree:"), strlen("Cached:"), NULL };

    char* p = buffer;
    while (*p && numFound < 2) {
        Int32 i = 0;
        while (sums[i]) {
            if (strncmp(p, sums[i], sumsLen[i]) == 0) {
                p += sumsLen[i];
                while (*p == ' ') p++;
                char* num = p;
                while (*p >= '0' && *p <= '9') p++;
                if (*p != 0) {
                    *p = 0;
                    p++;
                    if (*p == 0) p--;
                }
                mem += atoll(num) * 1024;
                numFound++;
                break;
            }
            i++;
        }
        p++;
    }

    return numFound > 0 ? mem : -1;
}

/** @hide */
ECode Process::ReadProcLines(
    /* [in] */ String path,
    /* [in] */ const ArrayOf<String> & reqFields,
    /* [in] */ const ArrayOf<Int64> & outSizes)
{
    //LOGI("getMemInfo: %p %p", reqFields, outFields);

    if (path == NULL) {// || reqFields == NULL || outFields == NULL) {
        //jniThrowException(env, "java/lang/NullPointerException", NULL);
        return E_NULL_POINTER_EXCEPTION;
    }

    const char* file8 = (const char *)path;
    android::String8 file(file8);

    Int32 count = reqFields.GetLength();
    if (count > outSizes.GetLength()) {
        //jniThrowException(env, "java/lang/IllegalArgumentException", "Array lengths differ");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    android::Vector<android::String8> fields;
    Int32 i;
    for (i = 0; i < count; i++) {
        String obj = reqFields.GetPayload()[i];
        if (obj != NULL) {
            const char* str8 = (const char*)obj;
            //LOGI("String at %d: %p = %s", i, obj, str8);
            if (str8 == NULL) {
                //jniThrowException(env, "java/lang/NullPointerException", "Element in reqFields");
                return E_NULL_POINTER_EXCEPTION;
            }
            fields.add(android::String8(str8));
        } else {
            //jniThrowException(env, "java/lang/NullPointerException", "Element in reqFields");
            return E_NULL_POINTER_EXCEPTION;
        }
    }

    Int64* sizesArray = outSizes.GetPayload();
    if (sizesArray == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }

    //LOGI("Clearing %d sizes", count);
    for (i = 0; i < count; i++) {
        sizesArray[i] = 0;
    }

    Int32 fd = open(file.string(), O_RDONLY);

    if (fd >= 0) {
        const size_t BUFFER_SIZE = 2048;
        char* buffer = (char*)malloc(BUFFER_SIZE);
        Int32 len = read(fd, buffer, BUFFER_SIZE-1);
        close(fd);

        if (len < 0) {
            //LOGW("Unable to read %s", file.string());
            len = 0;
        }
        buffer[len] = 0;

        Int32 foundCount = 0;

        char* p = buffer;
        while (*p && foundCount < count) {
            Boolean skipToEol = TRUE;
            //LOGI("Parsing at: %s", p);
            for (i = 0; i < count; i++) {
                const android::String8& field = fields[i];
                if (strncmp(p, field.string(), field.length()) == 0) {
                    p += field.length();
                    while (*p == ' ' || *p == '\t') p++;
                    char* num = p;
                    while (*p >= '0' && *p <= '9') p++;
                    skipToEol = *p != '\n';
                    if (*p != 0) {
                        *p = 0;
                        p++;
                    }
                    char* end;
                    sizesArray[i] = strtoll(num, &end, 10);
                    //LOGI("Field %s = %d", field.string(), sizesArray[i]);
                    foundCount++;
                    break;
                }
            }
            if (skipToEol) {
                while (*p && *p != '\n') {
                    p++;
                }
                if (*p == '\n') {
                    p++;
                }
            }
        }
        free(buffer);
    } else {
        //LOGW("Unable to open %s", file.string());
        return E_FAIL;
    }
    //LOGI("Done!");
    //env->ReleaseLongArrayElements(outFields, sizesArray, 0);

    return NOERROR;
}

static Int32 pid_compare(const void* v1, const void* v2)
{
    //LOGI("Compare %d vs %d\n", *((const jint*)v1), *((const jint*)v2));
    return *((const Int32*)v1) - *((const Int32*)v2);
}

/** @hide */
ECode Process::GetPids(
    /* [in] */ String path,
    /* [in] */ const ArrayOf<Int32> & lastArray,
    /* [out] */ ArrayOf<Int32>** newArray)
{
    if (path == NULL) {
        //jniThrowException(env, "java/lang/NullPointerException", NULL);
        return E_NULL_POINTER_EXCEPTION;
    }

    const char* file8 = (const char*)path;
    if (file8 == NULL) {
        //jniThrowException(env, "java/lang/OutOfMemoryError", NULL);
        return E_NULL_POINTER_EXCEPTION;
    }

    DIR* dirp = opendir(file8);

    if(dirp == NULL) {
        return E_FAIL;
    }

    Int32 curCount = 0;
    Int32* curData = NULL;
    curCount = lastArray.GetLength();
    curData = lastArray.GetPayload();
    Int32 curPos = 0;
    struct dirent* entry;
    while ((entry = readdir(dirp)) != NULL) {
        const char* p = entry->d_name;
        while (*p) {
            if (*p < '0' || *p > '9') {
                break;
            }
            p++;
        }

        if (*p != 0) {
            continue;
        }

        char* end;
        Int32 pid = strtol(entry->d_name, &end, 10);
        //LOGI("File %s pid=%d\n", entry->d_name, pid);
        if (curPos >= curCount) {
            Int32 newCount = (curCount == 0) ? 10 : (curCount*2);
            *newArray = ArrayOf<Int32>::Alloc(newCount);
            if (*newArray == NULL) {
                closedir(dirp);
                //jniThrowException(env, "java/lang/OutOfMemoryError", NULL);
                return E_OUT_OF_MEMORY_ERROR;
            }
            Int32* newData = (*newArray)->GetPayload();
            if (curData != NULL) {
                memcpy(newData, curData, sizeof(Int32) * curCount);
                //ArrayOf<Int32>::Free(lastArray);
            }
            //lastArray = newArray;
            curCount = newCount;
            curData = newData;
        }

        curData[curPos] = pid;
        curPos++;
    }

    closedir(dirp);

    if (curData != NULL && curPos > 0) {
        qsort(curData, curPos, sizeof(Int32), pid_compare);
    }

    while (curPos < curCount) {
        curData[curPos] = -1;
        curPos++;
    }

    return NOERROR;
}

enum {
    PROC_TERM_MASK = 0xff,
    PROC_ZERO_TERM = 0,
    PROC_SPACE_TERM = ' ',
    PROC_COMBINE = 0x100,
    PROC_PARENS = 0x200,
    PROC_OUT_STRING = 0x1000,
    PROC_OUT_LONG = 0x2000,
    PROC_OUT_FLOAT = 0x4000,
};

Boolean ParseProcLineArray(
    /* [in] */ char* buffer,
    /* [in] */ Int32 startIndex,
    /* [in] */ Int32 endIndex,
    /* [in] */ const ArrayOf<Int32> & format,
    /* [in] */ const ArrayOf<String> & outStrings,
    /* [in] */ const ArrayOf<Int64> & outLongs,
    /* [in] */ const ArrayOf<Float> & outFloats)
{

    const Int32 NF = format.GetLength();
    const Int32 NS = outStrings.GetLength();
    const Int32 NL = outLongs.GetLength();
    const Int32 NR = outFloats.GetLength();

    Int32* formatData = format.GetPayload();
    Int64* longsData = outLongs.GetPayload();
    Float* floatsData = outFloats.GetPayload();
    if (formatData == NULL || (NL > 0 && longsData == NULL)
            || (NR > 0 && floatsData == NULL)) {
        //jniThrowException(env, "java/lang/OutOfMemoryError", NULL);
        return FALSE;
    }

    Int32 i = startIndex;
    Int32 di = 0;
    Boolean res = TRUE;
    for (Int32 fi = 0; fi < NF; fi++) {
        const Int32 mode = formatData[fi];
        if ((mode&PROC_PARENS) != 0) {
            i++;
        }
        const char term = (char)(mode & PROC_TERM_MASK);
        const Int32 start = i;
        if (i >= endIndex) {
            res = FALSE;
            break;
        }

        Int32 end = -1;
        if ((mode & PROC_PARENS) != 0) {
            while (buffer[i] != ')' && i < endIndex) {
                i++;
            }
            end = i;
            i++;
        }
        while (buffer[i] != term && i < endIndex) {
            i++;
        }
        if (end < 0) {
            end = i;
        }

        if (i < endIndex) {
            i++;
            if ((mode&PROC_COMBINE) != 0) {
                while (buffer[i] == term && i < endIndex) {
                    i++;
                }
            }
        }

        //LOGI("Field %d: %d-%d dest=%d mode=0x%x\n", i, start, end, di, mode);
        if ((mode & (PROC_OUT_FLOAT|PROC_OUT_LONG | PROC_OUT_STRING)) != 0) {
            char c = buffer[end];
            buffer[end] = 0;
            if ((mode & PROC_OUT_FLOAT) != 0 && di < NR) {
                char* end;
                floatsData[di] = strtof(buffer + start, &end);
            }
            if ((mode & PROC_OUT_LONG) != 0 && di < NL) {
                char* end;
                longsData[di] = strtoll(buffer + start, &end, 10);
            }
            if ((mode & PROC_OUT_STRING) != 0 && di < NS) {
                String str(buffer + start);
                outStrings.GetPayload()[di] = str;
            }
            buffer[end] = c;
            di++;
        }
    }

    return res;
}

/** @hide */
ECode Process::ReadProcFile(
    /* [in] */ String file,
    /* [in] */ const ArrayOf<Int32> & format,
    /* [in] */ const ArrayOf<String> & outStrings,
    /* [in] */ const ArrayOf<Int64> & outLongs,
    /* [in] */ const ArrayOf<Float> outFloats,
    /* [out] */ Boolean* result)
{
    if (file == NULL) {
        //jniThrowException(env, "java/lang/NullPointerException", NULL);
        *result = FALSE;
        return E_NULL_POINTER_EXCEPTION;
    }

    const char* file8 = (const char*)file;
    if (file8 == NULL) {
        //jniThrowException(env, "java/lang/OutOfMemoryError", NULL);
        *result = FALSE;
        return E_OUT_OF_MEMORY_ERROR;
    }

    Int32 fd = open(file8, O_RDONLY);

    if (fd < 0) {
        //LOGW("Unable to open process file: %s\n", file8);
        *result = FALSE;
        return E_FAIL;
    }

    char buffer[256];
    const Int32 len = read(fd, buffer, sizeof(buffer)-1);
    close(fd);

    if (len < 0) {
        //LOGW("Unable to open process file: %s fd=%d\n", file8, fd);
        *result = FALSE;
        return E_FAIL;
    }
    buffer[len] = 0;

    *result = ParseProcLineArray(
                buffer,
                0,
                len,
                format,
                outStrings,
                outLongs,
                outFloats);

    return NOERROR;
}

/** @hide */
Boolean Process::ParseProcLine(
    /* [in] */ const ArrayOf<Byte> & buffer,
    /* [in] */ Int32 startIndex,
    /* [in] */ Int32 endIndex,
    /* [in] */ const ArrayOf<Int32> & format,
    /* [in] */ const ArrayOf<String> & outStrings,
    /* [in] */ const ArrayOf<Int64> & outLongs,
    /* [in] */ const ArrayOf<Float> & outFloats)
{
    Byte* bufferArray = buffer.GetPayload();

    return ParseProcLineArray(
            (char*)bufferArray,
            startIndex,
            endIndex,
            format,
            outStrings,
            outLongs,
            outFloats);
}

/**
 * Gets the total Pss value for a given process, in bytes.
 *
 * @param pid the process to the Pss for
 * @return the total Pss value for the given process in bytes,
 *  or -1 if the value cannot be determined
 * @hide
 */
Int64 Process::GetPss(
    /* [in] */ Int32 pid)
{
    char filename[64];
    snprintf(filename, sizeof(filename), "/proc/%d/smaps", pid);

    FILE * file = fopen(filename, "r");
    if (!file) {
        return (Int64) -1;
    }

    // Tally up all of the Pss from the various maps
    char line[256];
    Int64 pss = 0;
    while (fgets(line, sizeof(line), file)) {
        Int64 v;
        if (sscanf(line, "Pss: %lld kB", &v) == 1) {
            pss += v;
        }
    }

    fclose(file);

    // Return the Pss value in bytes, not kilobytes
    return pss * 1024;
}

