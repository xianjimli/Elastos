
#include "ProcessManager.h"
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>

/*
 * These are constants shared with the higher level code in
 * ProcessManager.java.
 */
#define WAIT_STATUS_UNKNOWN (-1)       // unknown child status
#define WAIT_STATUS_NO_CHILDREN (-2)   // no children to wait for
#define WAIT_STATUS_STRANGE_ERRNO (-3) // observed an undocumented errno

const ProcessManager* ProcessManager::mInstance = new ProcessManager();

ProcessManager::ProcessThread::ProcessThread(
    /* [in] */ ProcessManager* pMg)
    : mPMg(pMg)
{}

PInterface ProcessManager::ProcessThread::Probe(
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

UInt32 ProcessManager::ProcessThread::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ProcessManager::ProcessThread::Release()
{
    return ElRefBase::Release();
}

ECode ProcessManager::ProcessThread::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode ProcessManager::ProcessThread::Run()
{
    mPMg->WatchChildren();
    return NOERROR;
}

ProcessManager::ProcessManager()
{
    ProcessThread* processThread = new ProcessThread(this);
    AutoPtr<IRunnable> runnable = (IRunnable*)processThread->Probe(EIID_IRunnable);
    AutoPtr<IThread> pThread;
    ASSERT_SUCCEEDED(CThread::New(runnable, String("ProcessManager"), (IThread**)&pThread));
    pThread->SetDaemon(TRUE);
    pThread->Start();
}

/**
 * Kills the process with the given ID.
 *
 * @parm pid ID of process to kill
 */
ECode ProcessManager::Kill(
    /* [in] */ Int32 pid)
{
    Int32 result = kill((pid_t)pid, SIGKILL);
    if (result == -1) {
        // jniThrowIOException(env, errno);
        return E_IO_EXCEPTION;
    }
    return NOERROR;
}

void ProcessManager::CleanUp()
{
    // ProcessReference reference;
    // while ((reference = referenceQueue.poll()) != null) {
    //     synchronized (processReferences) {
    //         processReferences.remove(reference.processId);
    //     }
    // }
}

void ProcessManager::WatchChildren()
{
    // if (onExitMethod == NULL) {
    //     jniThrowException(env, "java/lang/IllegalStateException",
    //             "staticInitialize() must run first.");
    // }

    while (1) {
        Int32 status;

        /* wait for children in our process group */
        pid_t pid = waitpid(0, &status, 0);

        if (pid >= 0) {
            // Extract real status.
            if (WIFEXITED(status)) {
                status = WEXITSTATUS(status);
            }
            else if (WIFSIGNALED(status)) {
                status = WTERMSIG(status);
            }
            else if (WIFSTOPPED(status)) {
                status = WSTOPSIG(status);
            }
            else {
                status = WAIT_STATUS_UNKNOWN;
            }
        }
        else {
            /*
             * The pid should be -1 already, but force it here just in case
             * we somehow end up with some other negative value.
             */
            pid = -1;

            switch (errno) {
                case ECHILD: {
                    /*
                     * Expected errno: There are no children to wait()
                     * for. The callback will sleep until it is
                     * informed of another child coming to life.
                     */
                    status = WAIT_STATUS_NO_CHILDREN;
                    break;
                }
                case EINTR: {
                    /*
                     * An unblocked signal came in while waiting; just
                     * retry the wait().
                     */
                    continue;
                }
                default: {
                    /*
                     * Unexpected errno, so squawk! Note: Per the
                     * Linux docs, there are no errnos defined for
                     * wait() other than the two that are handled
                     * immediately above.
                     */
                    // LOGE("Error %d calling wait(): %s", errno,
                    //         strerror(errno));
                    status = WAIT_STATUS_STRANGE_ERRNO;
                    break;
                }
            }
        }

        OnExit(pid, status);
        //if (env->ExceptionOccurred()) {
            /*
             * The callback threw, so break out of the loop and return,
             * letting the exception percolate up.
             */
        //    break;
       // }
    }
}

ECode ProcessManager::OnExit(
    /* [in] */ Int32 pid,
    /* [in] */ Int32 exitValue)
{
    //ProcessReference processReference = null;
    AutoPtr<IProcess> exitProcess;

    {
        Mutex::Autolock lock(mProcessesLock);
        CleanUp();
        if (pid >= 0) {
            HashMap<Int32, AutoPtr<IProcess> >::Iterator it = mProcesses.Find(pid);
            if (it != mProcesses.End()) {
                exitProcess = it->mSecond;
                mProcesses.Erase(it);
            }
        }
        else if (exitValue == WAIT_STATUS_NO_CHILDREN) {
            if (mProcesses.Begin() == mProcesses.End()) {
                /*
                 * There are no eligible children; wait for one to be
                 * added. The wait() will return due to the
                 * notifyAll() call below.
                 */
                //try {
                //    processReferences.wait();
                //} catch (InterruptedException ex) {
                    // This should never happen.
                //    throw new AssertionError("unexpected interrupt");
                //}
                return E_NOT_IMPLEMENTED;
            }
            else {
                /*
                 * A new child was spawned just before we entered
                 * the synchronized block. We can just fall through
                 * without doing anything special and land back in
                 * the native wait().
                 */
            }
        }
        else {
            // Something weird is happening; abort!
            return E_ASSERTION_ERROR;
            //throw new AssertionError("unexpected wait() behavior");
        }
    }

    if (exitProcess != NULL) {
        ProcessImpl* process = (ProcessImpl*)exitProcess.Get();
        if (process != NULL) {
            process->SetExitValue(exitValue);
        }
    }

    return NOERROR;
}

/** Close all open fds > 2 (i.e. everything but stdin/out/err), != skipFd. */
static void CloseNonStandardFds(Int32 skipFd)
{
    // TODO: rather than close all these non-open files, we could look in /proc/self/fd.
    rlimit rlimit;
    getrlimit(RLIMIT_NOFILE, &rlimit);
    const int max_fd = rlimit.rlim_max;
    for (int fd = 3; fd < max_fd; ++fd) {
        if (fd != skipFd
#ifdef ANDROID
                && fd != androidSystemPropertiesFd
#endif
                ) {
            close(fd);
        }
    }
}

#define PIPE_COUNT (4) // number of pipes used to communicate with child proc

/** Closes all pipes in the given array. */
static void ClosePipes(
    /* [in] */ Int32 pipes[],
    /* [in] */ Int32 skipFd)
{
    Int32 i;
    for (i = 0; i < PIPE_COUNT * 2; i++) {
        Int32 fd = pipes[i];
        if (fd == -1) {
            return;
        }
        if (fd != skipFd) {
            close(pipes[i]);
        }
    }
}

/** Executes a command in a child process. */
static ECode ExecuteProcess(
    /* [in] */ char** commands,
    /* [in] */ char** environment,
    /* [in] */ const char* workingDirectory,
    /* [in] */ IFileDescriptor* inDescriptor,
    /* [in] */ IFileDescriptor* outDescriptor,
    /* [in] */ IFileDescriptor* errDescriptor,
    /* [in] */ Boolean redirectErrorStream,
    /* [out] */ pid_t* pid)
{
    Int32 i, result, error;
    ECode ec = NOERROR;

    // Create 4 pipes: stdin, stdout, stderr, and an exec() status pipe.
    Int32 pipes[PIPE_COUNT * 2] = { -1, -1, -1, -1, -1, -1, -1, -1 };
    for (i = 0; i < PIPE_COUNT; i++) {
        if (pipe(pipes + i * 2) == -1) {
            // jniThrowIOException(env, errno);
            ec = E_IO_EXCEPTION;
            ClosePipes(pipes, -1);
            *pid = -1;
            return ec;
        }
    }
    Int32 stdinIn = pipes[0];
    Int32 stdinOut = pipes[1];
    Int32 stdoutIn = pipes[2];
    Int32 stdoutOut = pipes[3];
    Int32 stderrIn = pipes[4];
    Int32 stderrOut = pipes[5];
    Int32 statusIn = pipes[6];
    Int32 statusOut = pipes[7];

    pid_t childPid = fork();

    // If fork() failed...
    if (childPid == -1) {
        // jniThrowIOException(env, errno);
        ec = E_IO_EXCEPTION;
        ClosePipes(pipes, -1);
        *pid = -1;
        return ec;
    }

    // If this is the child process...
    if (childPid == 0) {
        /*
         * Note: We cannot malloc() or free() after this point!
         * A no-longer-running thread may be holding on to the heap lock, and
         * an attempt to malloc() or free() would result in deadlock.
         */

        // Replace stdin, out, and err with pipes.
        dup2(stdinIn, 0);
        dup2(stdoutOut, 1);
        if (redirectErrorStream) {
            dup2(stdoutOut, 2);
        }
        else {
            dup2(stderrOut, 2);
        }

        // Close all but statusOut. This saves some work in the next step.
        ClosePipes(pipes, statusOut);

        // Make statusOut automatically close if execvp() succeeds.
        fcntl(statusOut, F_SETFD, FD_CLOEXEC);

        // Close remaining open fds with the exception of statusOut.
        CloseNonStandardFds(statusOut);

        // Switch to working directory.
        if (workingDirectory != NULL) {
            if (chdir(workingDirectory) == -1) {
                goto execFailed;
            }
        }

        // Set up environment.
        if (environment != NULL) {
            environ = environment;
        }

        // Execute process. By convention, the first argument in the arg array
        // should be the command itself. In fact, I get segfaults when this
        // isn't the case.
        execvp(commands[0], commands);

        // If we got here, execvp() failed or the working dir was invalid.
        execFailed:
            error = errno;
            write(statusOut, &error, sizeof(int));
            close(statusOut);
            exit(error);
    }

    // This is the parent process.

    // Close child's pipe ends.
    close(stdinIn);
    close(stdoutOut);
    close(stderrOut);
    close(statusOut);

    // Check status pipe for an error code. If execvp() succeeds, the other
    // end of the pipe should automatically close, in which case, we'll read
    // nothing.
    Int32 count = read(statusIn, &result, sizeof(int));
    close(statusIn);
    if (count > 0) {
        // jniThrowIOException(env, result);
        ec = E_IO_EXCEPTION;

        close(stdoutIn);
        close(stdinOut);
        close(stderrIn);

        *pid = -1;
        return ec;
    }

    // Fill in file descriptor wrappers.
    inDescriptor->SetDescriptor(stdoutIn);
    outDescriptor->SetDescriptor(stdinOut);
    errDescriptor->SetDescriptor(stderrIn);

    *pid = childPid;
    return ec;
}

/** Converts a Java String[] to a 0-terminated char**. */
static char** ConvertStrings(const ArrayOf<String>* strArray)
{
    if (strArray == NULL) {
        return NULL;
    }

    Int32 length = strArray->GetLength();
    char** array = new char*[length + 1];
    array[length] = 0;
    for (Int32 index = 0; index < length; index++) {
        // We need to pass these strings to const-unfriendly code.
        char* entry = const_cast<char*>((*strArray)[index].string());
        array[index] = entry;
    }

    return array;
}

/** Frees a char** which was converted from a Java String[]. */
static void FreeStrings(const ArrayOf<String>* strArray, char** array)
{
    if (strArray == NULL) {
        return;
    }

    delete[] array;
}

/**
 * Executes a native process. Fills in in, out, and err and returns the
 * new process ID upon success.
 */
ECode ProcessManager::Exec(
    /* [in] */ const ArrayOf<String>* _commands,
    /* [in] */ const ArrayOf<String>* _environment,
    /* [in] */ const String& _workingDirectory,
    /* [in] */ IFileDescriptor* inDescriptor,
    /* [in] */ IFileDescriptor* outDescriptor,
    /* [in] */ IFileDescriptor* errDescriptor,
    /* [in] */ Boolean redirectErrorStream,
    /* [out] */ Int32* procId)
{
    // Copy commands into char*[].
    char** commands = ConvertStrings(_commands);

    // Extract working directory string.
    const char* workingDirectory = NULL;
    if (!_workingDirectory.IsNull()) {
        workingDirectory = _workingDirectory.string();
    }

    // Convert environment array.
    char** environment = ConvertStrings(_environment);

    pid_t result;
    ECode ec = ExecuteProcess(
            commands, environment, workingDirectory,
            inDescriptor, outDescriptor, errDescriptor, redirectErrorStream,
            &result);

    FreeStrings(_environment, environment);

    FreeStrings(_commands, commands);

    *procId = (Int32)result;
    return ec;
}

ECode ProcessManager::Exec(
    /* [in] */ const ArrayOf<String>* taintedCommand,
    /* [in] */ const ArrayOf<String>* taintedEnvironment,
    /* [in] */ IFile* workingDirectory,
    /* [in] */ Boolean redirectErrorStream,
    /* [out] */ IProcess** proc)
{
    VALIDATE_NOT_NULL(proc);
    // Make sure we throw the same exceptions as the RI.
    if (taintedCommand == NULL) {
        return E_NULL_POINTER_EXCEPTION;
//        throw new NullPointerException();
    }
    if (taintedCommand->GetLength() == 0) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }

    // Handle security and safety by copying mutable inputs and checking them.
    ArrayOf<String>* command = taintedCommand->Clone();
    ArrayOf<String>* environment = taintedEnvironment != NULL ? taintedEnvironment->Clone() : NULL;
    // SecurityManager securityManager = System.getSecurityManager();
    // if (securityManager != null) {
    //     securityManager.checkExec(command[0]);
    // }
    // Check we're not passing null Strings to the native exec.
    String arg(NULL);
    for (Int32 i = 0; i < command->GetLength(); ++i) {
        arg = (*command)[i];
        if (arg.IsNull()) {
            return E_NULL_POINTER_EXCEPTION;
//            throw new NullPointerException();
        }
    }
    // The environment is allowed to be null or empty, but no element may be null.
    if (environment != NULL) {
        String env(NULL);
        for (Int32 j = 0; j < environment->GetLength(); ++j) {
            env = (*environment)[j];
            if (env.IsNull()) {
                return E_NULL_POINTER_EXCEPTION;
    //            throw new NullPointerException();
            }
        }
    }

    AutoPtr<IFileDescriptor> in, out, err;
    ASSERT_SUCCEEDED(CFileDescriptor::New((IFileDescriptor**)&in));
    ASSERT_SUCCEEDED(CFileDescriptor::New((IFileDescriptor**)&out));
    ASSERT_SUCCEEDED(CFileDescriptor::New((IFileDescriptor**)&err));

    String workingPath(NULL);
    if (workingDirectory != NULL) {
        workingDirectory->GetPath(&workingPath);
    }

    // Ensure onExit() doesn't access the process map before we add our
    // entry.
    Mutex::Autolock lock(mProcessesLock);
    Int32 pid;
    //try {
    FAIL_RETURN(Exec(command, environment, workingPath,
           in, out, err, redirectErrorStream, &pid));
    //     } catch (IOException e) {
    //         IOException wrapper = new IOException("Error running exec()."
    //                 + " Command: " + Arrays.toString(command)
    //                 + " Working Directory: " + workingDirectory
    //                 + " Environment: " + Arrays.toString(environment));
    //         wrapper.initCause(e);
    //         throw wrapper;
    //     }
    ProcessImpl* process = new ProcessImpl(pid, in, out, err);
    // ProcessReference processReference
    //         = new ProcessReference(process, referenceQueue);
    mProcesses[pid] = (IProcess*)process->Probe(EIID_IProcess);

    /*
     * This will wake up the child monitor thread in case there
     * weren't previously any children to wait on.
     */
    //processReferences.notifyAll();

    *proc = (IProcess*)process->Probe(EIID_IProcess);
    return NOERROR;
}

PInterface ProcessManager::ProcessImpl::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IProcess) {
        return (IProcess*)this;
    }

    return NULL;
}

UInt32 ProcessManager::ProcessImpl::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ProcessManager::ProcessImpl::Release()
{
    return ElRefBase::Release();
}

ECode ProcessManager::ProcessImpl::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ProcessManager::ProcessImpl::ProcessImpl(
    /* [in] */ Int32 id,
    /* [in] */ IFileDescriptor* in,
    /* [in] */ IFileDescriptor* out,
    /* [in] */ IFileDescriptor* err)
    : mId(id)
    , mExitValue(0)
{
    mErrorStream = (IInputStream*)(new ProcessInputStream(err))->Probe(EIID_IInputStream);
    mInputStream = (IInputStream*)(new ProcessInputStream(in))->Probe(EIID_IInputStream);
    mOutputStream = (IOutputStream*)(new ProcessOutputStream(out))->Probe(EIID_IOutputStream);
}

ECode ProcessManager::ProcessImpl::Destroy()
{
//    try {
    return ProcessManager::Kill(mId);
//    } catch (IOException e) {
//        Logger.getLogger(Runtime.class.getName()).log(Level.FINE,
//                "Failed to destroy process " + id + ".", e);
//    }
}

ECode ProcessManager::ProcessImpl::ExitValue(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    Mutex::Autolock lock(mExitValueLock);
    // if (exitValue == NULL) {
    //     return E_ILLEGAL_THREAD_STATE_EXCEPTION;
    //     throw new IllegalThreadStateException(
    //             "Process has not yet terminated.");
    // }

    *value = mExitValue;
    return NOERROR;
}

ECode ProcessManager::ProcessImpl::GetErrorStream(
    /* [out] */ IInputStream** es)
{
    VALIDATE_NOT_NULL(es);
    *es = mErrorStream;
    return NOERROR;
}

ECode ProcessManager::ProcessImpl::GetInputStream(
    /* [out] */ IInputStream** is)
{
    VALIDATE_NOT_NULL(is);
    *is = mInputStream;
    return NOERROR;
}

ECode ProcessManager::ProcessImpl::GetOutputStream(
    /* [out] */ IOutputStream** os)
{
    VALIDATE_NOT_NULL(os);
    *os = mOutputStream;
    return NOERROR;
}

ECode ProcessManager::ProcessImpl::WaitFor(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    Mutex::Autolock lock(mExitValueLock);
    // while (mExitValue == null) {
    //     mExitValueLock.Wait();
    // }
    *value = mExitValue;

    return NOERROR;
}

void ProcessManager::ProcessImpl::SetExitValue(
    /* [in] */ Int32 exitValue)
{
    Mutex::Autolock lock(mExitValueLock);
    mExitValue = exitValue;
//    mExitValueLock.notifyAll();
}

ProcessManager::ProcessInputStream::ProcessInputStream(
    /* [in]  */ IFileDescriptor* fd)
    : mFd(fd)
{
    ASSERT_SUCCEEDED(CFileInputStream::New(fd, (IFileInputStream**)&mFIn));
}

PInterface ProcessManager::ProcessInputStream::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IFileInputStream) {
        return (IFileInputStream*)this;
    }
    else if (riid == EIID_IInputStream) {
        return (IInputStream*)this;
    }

    return NULL;
}

UInt32 ProcessManager::ProcessInputStream::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ProcessManager::ProcessInputStream::Release()
{
    return ElRefBase::Release();
}

ECode ProcessManager::ProcessInputStream::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode ProcessManager::ProcessInputStream::Available(
    /* [out] */ Int32* number)
{
    return mFIn->Available(number);
}

ECode ProcessManager::ProcessInputStream::Close()
{
    //try {
    mFIn->Close();
    //} finally {
    Mutex::Autolock lock(m_lock);
    Boolean isValid;
    mFd->Valid(&isValid);
    if (mFd != NULL && isValid) {
        //try {
        Int32 nativeFd;
        mFd->GetDescriptor(&nativeFd);
        Int32 rc = TEMP_FAILURE_RETRY(close(nativeFd));
        if (rc == -1) {
    //        jniThrowIOException(env, errno);
            mFd = NULL;
            return E_IO_EXCEPTION;
        }
        mFd->SetDescriptor(-1);
        //} finally {
        mFd = NULL;
        //}
    }
    //}
    return NOERROR;
}

ECode ProcessManager::ProcessInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    return mFIn->Mark(readLimit);
}

ECode ProcessManager::ProcessInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    return mFIn->IsMarkSupported(supported);
}

ECode ProcessManager::ProcessInputStream::Read(
    /* [out] */ Int32* value)
{
    return mFIn->Read(value);
}

ECode ProcessManager::ProcessInputStream::ReadBuffer(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    return mFIn->ReadBuffer(buffer, number);
}

ECode ProcessManager::ProcessInputStream::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    return mFIn->ReadBufferEx(offset, length, buffer, number);
}

ECode ProcessManager::ProcessInputStream::Reset()
{
    return mFIn->Reset();
}

ECode ProcessManager::ProcessInputStream::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    return mFIn->Skip(count, number);
}

ECode ProcessManager::ProcessInputStream::GetChannel(
    /* [out] */ IFileChannel** channel)
{
    return mFIn->GetChannel(channel);
}

ECode ProcessManager::ProcessInputStream::GetFD(
    /* [out] */ IFileDescriptor** fd)
{
    return mFIn->GetFD(fd);
}

ProcessManager::ProcessOutputStream::ProcessOutputStream(
    /* [in]  */ IFileDescriptor* fd)
    : mFd(fd)
{
    ASSERT_SUCCEEDED(CFileOutputStream::New(fd, (IFileOutputStream**)&mFOut));
}

PInterface ProcessManager::ProcessOutputStream::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IFileOutputStream) {
        return (IFileOutputStream*)this;
    }
    else if (riid == EIID_IOutputStream) {
        return (IOutputStream*)this;
    }

    return NULL;
}

UInt32 ProcessManager::ProcessOutputStream::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ProcessManager::ProcessOutputStream::Release()
{
    return ElRefBase::Release();
}

ECode ProcessManager::ProcessOutputStream::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode ProcessManager::ProcessOutputStream::Close()
{
    //try {
    mFOut->Close();
    //} finally {
    Mutex::Autolock lock(m_lock);
    Boolean isValid;
    mFd->Valid(&isValid);
    if (mFd != NULL && isValid) {
        //try {
        Int32 nativeFd;
        mFd->GetDescriptor(&nativeFd);
        Int32 rc = TEMP_FAILURE_RETRY(close(nativeFd));
        if (rc == -1) {
    //        jniThrowIOException(env, errno);
            mFd = NULL;
            return E_IO_EXCEPTION;
        }
        mFd->SetDescriptor(-1);
        //} finally {
        mFd = NULL;
        //}
    }
    //}
    return NOERROR;
}

ECode ProcessManager::ProcessOutputStream::Flush()
{
    return mFOut->Flush();
}

ECode ProcessManager::ProcessOutputStream::Write(
    /* [in] */ Int32 oneByte)
{
    return mFOut->Write(oneByte);
}

ECode ProcessManager::ProcessOutputStream::WriteBuffer(
    /* [in] */ const ArrayOf<Byte>& buffer)
{
    return mFOut->WriteBuffer(buffer);
}

ECode ProcessManager::ProcessOutputStream::WriteBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ const ArrayOf<Byte>& buffer)
{
    return mFOut->WriteBufferEx(offset, count, buffer);
}

ECode ProcessManager::ProcessOutputStream::GetChannel(
    /* [out] */ IFileChannel** channel)
{
    return mFOut->GetChannel(channel);
}

ECode ProcessManager::ProcessOutputStream::GetFD(
    /* [out] */ IFileDescriptor** fd)
{
    return mFOut->GetFD(fd);
}

ECode ProcessManager::ProcessOutputStream::CheckError(
    /* [out] */ Boolean* hasError)
{
    VALIDATE_NOT_NULL(hasError);
    *hasError = FALSE;
    return NOERROR;
}

const ProcessManager* ProcessManager::GetInstance()
{
    return mInstance;
}
