
#include "ProcessManager.h"
#include <sys/resource.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>

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
