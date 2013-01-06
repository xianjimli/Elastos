
#ifndef __PROCESSMANAGER_H__
#define __PROCESSMANAGER_H__

#include "Elastos.Process_server.h"

class ProcessManager
{
public:
    /**
     * Cleans up after garbage collected processes. Requires the lock on the
     * map.
     */
    /* package */ CARAPI_(void) CleanUp();

    /**
     * Listens for signals from processes and calls back to
     * {@link #onExit(int,int)}.
     */
    /* package */ CARAPI_(void) WatchChildren();

    /**
     * Called by {@link #watchChildren()} when a child process exits.
     *
     * @param pid ID of process that exited
     * @param exitValue value the process returned upon exit
     */
    /* package */ CARAPI_(void) OnExit(
        /* [in] */ Int32 pid,
        /* [in] */ Int32 exitValue);

    /**
     * Executes a native process. Fills in in, out, and err and returns the
     * new process ID upon success.
     */
    /* package */ static CARAPI Exec(
        /* [in] */ const ArrayOf<String>* command,
        /* [in] */ const ArrayOf<String>* environment,
        /* [in] */ const String& workingDirectory,
        /* [in] */ IFileDescriptor* in,
        /* [in] */ IFileDescriptor* out,
        /* [in] */ IFileDescriptor* err,
        /* [in] */ Boolean redirectErrorStream,
        /* [out] */ Int32* procId);

    /**
     * Executes a process and returns an object representing it.
     */
    /* package */ CARAPI Exec(
        /* [in] */ ArrayOf<String> taintedCommand,
        /* [in] */ ArrayOf<String> taintedEnvironment,
        /* [in] */ IFile* workingDirectory,
        /* [in] */ Boolean redirectErrorStream,
        /* [out] */ IProcess** proc);

    /** Gets the process manager. */
    static CARAPI_(ProcessManager*) GetInstance();

private:
    ProcessManager();

    /**
     * Kills the process with the given ID.
     *
     * @parm pid ID of process to kill
     */
    static CARAPI Kill(
        /* [in] */ Int32 pid);
};

#endif //__PROCESSMANAGER_H__
