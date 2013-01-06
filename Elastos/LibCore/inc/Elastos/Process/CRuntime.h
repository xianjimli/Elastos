
#ifndef __CRUNTIME_H__
#define __CRUNTIME_H__

#include "_CRuntime.h"

CarClass(CRuntime)
{
public:
    /**
     * Executes the specified command and its arguments in a separate native
     * process. The new process inherits the environment of the caller. Calling
     * this method is equivalent to calling {@code exec(progArray, null, null)}.
     *
     * @param progArray
     *            the array containing the program to execute as well as any
     *            arguments to the program.
     * @return the new {@code Process} object that represents the native
     *         process.
     * @throws IOException
     *             if the requested program can not be executed.
     * @throws SecurityException
     *             if the current {@code SecurityManager} disallows program
     *             execution.
     * @see SecurityManager#checkExec
     */
    CARAPI Exec(
        /* [in] */ const ArrayOf<String>& progArray,
        /* [out] */ IProcess** proc);

    /**
     * Executes the specified command and its arguments in a separate native
     * process. The new process uses the environment provided in {@code envp}.
     * Calling this method is equivalent to calling
     * {@code exec(progArray, envp, null)}.
     *
     * @param progArray
     *            the array containing the program to execute as well as any
     *            arguments to the program.
     * @param envp
     *            the array containing the environment to start the new process
     *            in.
     * @return the new {@code Process} object that represents the native
     *         process.
     * @throws IOException
     *             if the requested program can not be executed.
     * @throws SecurityException
     *             if the current {@code SecurityManager} disallows program
     *             execution.
     * @see SecurityManager#checkExec
     */
    CARAPI ExecEx(
        /* [in] */ const ArrayOf<String>& progArray,
        /* [in] */ const ArrayOf<String>& envp,
        /* [out] */ IProcess** proc);

    /**
     * Executes the specified command and its arguments in a separate native
     * process. The new process uses the environment provided in {@code envp}
     * and the working directory specified by {@code directory}.
     *
     * @param progArray
     *            the array containing the program to execute as well as any
     *            arguments to the program.
     * @param envp
     *            the array containing the environment to start the new process
     *            in.
     * @param directory
     *            the directory in which to execute the program. If {@code null},
     *            execute if in the same directory as the parent process.
     * @return the new {@code Process} object that represents the native
     *         process.
     * @throws IOException
     *             if the requested program can not be executed.
     * @throws SecurityException
     *             if the current {@code SecurityManager} disallows program
     *             execution.
     * @see SecurityManager#checkExec
     */
    CARAPI ExecEx2(
        /* [in] */ const ArrayOf<String>& progArray,
        /* [in] */ ArrayOf<String>* envp,
        /* [in] */ IFile* directory,
        /* [out] */ IProcess** proc);

    /**
     * Executes the specified program in a separate native process. The new
     * process inherits the environment of the caller. Calling this method is
     * equivalent to calling {@code exec(prog, null, null)}.
     *
     * @param prog
     *            the name of the program to execute.
     * @return the new {@code Process} object that represents the native
     *         process.
     * @throws IOException
     *             if the requested program can not be executed.
     * @throws SecurityException
     *             if the current {@code SecurityManager} disallows program
     *             execution.
     * @see SecurityManager#checkExec
     */
    CARAPI ExecEx3(
        /* [in] */ const String& prog,
        /* [out] */ IProcess** proc);

    /**
     * Executes the specified program in a separate native process. The new
     * process uses the environment provided in {@code envp}. Calling this
     * method is equivalent to calling {@code exec(prog, envp, null)}.
     *
     * @param prog
     *            the name of the program to execute.
     * @param envp
     *            the array containing the environment to start the new process
     *            in.
     * @return the new {@code Process} object that represents the native
     *         process.
     * @throws IOException
     *             if the requested program can not be executed.
     * @throws SecurityException
     *             if the current {@code SecurityManager} disallows program
     *             execution.
     * @see SecurityManager#checkExec
     */
    CARAPI ExecEx4(
        /* [in] */ const String& prog,
        /* [in] */ const ArrayOf<String>& envp,
        /* [out] */ IProcess** proc);

    /**
     * Executes the specified program in a separate native process. The new
     * process uses the environment provided in {@code envp} and the working
     * directory specified by {@code directory}.
     *
     * @param prog
     *            the name of the program to execute.
     * @param envp
     *            the array containing the environment to start the new process
     *            in.
     * @param directory
     *            the directory in which to execute the program. If {@code null},
     *            execute if in the same directory as the parent process.
     * @return the new {@code Process} object that represents the native
     *         process.
     * @throws IOException
     *             if the requested program can not be executed.
     * @throws SecurityException
     *             if the current {@code SecurityManager} disallows program
     *             execution.
     * @see SecurityManager#checkExec
     */
    CARAPI ExecEx5(
        /* [in] */ const String& prog,
        /* [in] */ const ArrayOf<String>& envp,
        /* [in] */ IFile* directory,
        /* [out] */ IProcess** proc);

    /**
     * Causes the virtual machine to stop running and the program to exit. If
     * {@link #runFinalizersOnExit(boolean)} has been previously invoked with a
     * {@code true} argument, then all objects will be properly
     * garbage-collected and finalized first.
     *
     * @param code
     *            the return code. By convention, non-zero return codes indicate
     *            abnormal terminations.
     * @throws SecurityException
     *             if the current {@code SecurityManager} does not allow the
     *             running thread to terminate the virtual machine.
     * @see SecurityManager#checkExit
     */
    CARAPI Exit(
        /* [in] */ Int32 code);

    /**
     * Loads and links the dynamic library that is identified through the
     * specified path. This method is similar to {@link #loadLibrary(String)},
     * but it accepts a full path specification whereas {@code loadLibrary} just
     * accepts the name of the library to load.
     *
     * @param pathName
     *            the absolute (platform dependent) path to the library to load.
     * @throws UnsatisfiedLinkError
     *             if the library can not be loaded.
     * @throws SecurityException
     *             if the current {@code SecurityManager} does not allow to load
     *             the library.
     * @see SecurityManager#checkLink
     */
    CARAPI Load(
        /* [in] */ const String& pathName);

    /**
     * Loads and links the library with the specified name. The mapping of the
     * specified library name to the full path for loading the library is
     * implementation-dependent.
     *
     * @param libName
     *            the name of the library to load.
     * @throws UnsatisfiedLinkError
     *             if the library can not be loaded.
     * @throws SecurityException
     *             if the current {@code SecurityManager} does not allow to load
     *             the library.
     * @see SecurityManager#checkLink
     */
    CARAPI LoadLibrary(
        /* [in] */ const String& libName);
};

#endif //__CRUNTIME_H__
