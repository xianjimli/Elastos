
#ifndef __PROCESS_H__
#define __PROCESS_H__

#include "ext/frameworkext.h"
#include <elastos.h>
#include <elastos/List.h>
#include <elastos/ElRefBase.h>
#include <elastos/Mutex.h>

using namespace Elastos;
using namespace Elastos::Core::Threading;

class Process
{
public:
    /**
     * Defines the UID/GID under which system code runs.
     */
    static const Int32 SYSTEM_UID = 1000;

    /**
     * Defines the UID/GID under which the telephony code runs.
     */
    static const Int32 PHONE_UID = 1001;

    /**
     * Defines the UID/GID for the user shell.
     * @hide
     */
    static const Int32 SHELL_UID = 2000;

    /**
     * Defines the UID/GID for the log group.
     * @hide
     */
    static const Int32 LOG_UID = 1007;

    /**
     * Defines the UID/GID for the WIFI supplicant process.
     * @hide
     */
    static const Int32 WIFI_UID = 1010;

    /**
     * Defines the UID/GID for the NFC service process.
     * @hide
     */
    static const Int32 NFC_UID = 1025;

    /**
     * Defines the start of a range of UIDs (and GIDs), going from this
     * number to {@link #LAST_APPLICATION_UID} that are reserved for assigning
     * to applications.
     */
    static const Int32 FIRST_APPLICATION_UID = 10000;
    /**
     * Last of application-specific UIDs starting at
     * {@link #FIRST_APPLICATION_UID}.
     */
    static const Int32 LAST_APPLICATION_UID = 99999;

    /**
     * Defines a secondary group id for access to the bluetooth hardware.
     */
    static const Int32 BLUETOOTH_GID = 2000;

    /**
     * Standard priority of application threads.
     * Use with {@link #setThreadPriority(int)} and
     * {@link #setThreadPriority(int, int)}, <b>not</b> with the normal
     * {@link java.lang.Thread} class.
     */
    static const Int32 THREAD_PRIORITY_DEFAULT = 0;

    /*
     * ***************************************
     * ** Keep in sync with utils/threads.h **
     * ***************************************
     */

    /**
     * Lowest available thread priority.  Only for those who really, really
     * don't want to run if anything else is happening.
     * Use with {@link #setThreadPriority(int)} and
     * {@link #setThreadPriority(int, int)}, <b>not</b> with the normal
     * {@link java.lang.Thread} class.
     */
    static const Int32 THREAD_PRIORITY_LOWEST = 19;

    /**
     * Standard priority background threads.  This gives your thread a slightly
     * lower than normal priority, so that it will have less chance of impacting
     * the responsiveness of the user interface.
     * Use with {@link #setThreadPriority(int)} and
     * {@link #setThreadPriority(int, int)}, <b>not</b> with the normal
     * {@link java.lang.Thread} class.
     */
    static const Int32 THREAD_PRIORITY_BACKGROUND = 10;

    /**
     * Standard priority of threads that are currently running a user interface
     * that the user is interacting with.  Applications can not normally
     * change to this priority; the system will automatically adjust your
     * application threads as the user moves through the UI.
     * Use with {@link #setThreadPriority(int)} and
     * {@link #setThreadPriority(int, int)}, <b>not</b> with the normal
     * {@link java.lang.Thread} class.
     */
    static const Int32 THREAD_PRIORITY_FOREGROUND = -2;

    /**
     * Standard priority of system display threads, involved in updating
     * the user interface.  Applications can not
     * normally change to this priority.
     * Use with {@link #setThreadPriority(int)} and
     * {@link #setThreadPriority(int, int)}, <b>not</b> with the normal
     * {@link java.lang.Thread} class.
     */
    static const Int32 THREAD_PRIORITY_DISPLAY = -4;

    /**
     * Standard priority of the most important display threads, for compositing
     * the screen and retrieving input events.  Applications can not normally
     * change to this priority.
     * Use with {@link #setThreadPriority(int)} and
     * {@link #setThreadPriority(int, int)}, <b>not</b> with the normal
     * {@link java.lang.Thread} class.
     */
    static const Int32 THREAD_PRIORITY_URGENT_DISPLAY = -8;

    /**
     * Standard priority of audio threads.  Applications can not normally
     * change to this priority.
     * Use with {@link #setThreadPriority(int)} and
     * {@link #setThreadPriority(int, int)}, <b>not</b> with the normal
     * {@link java.lang.Thread} class.
     */
    static const Int32 THREAD_PRIORITY_AUDIO = -16;

    /**
     * Standard priority of the most important audio threads.
     * Applications can not normally change to this priority.
     * Use with {@link #setThreadPriority(int)} and
     * {@link #setThreadPriority(int, int)}, <b>not</b> with the normal
     * {@link java.lang.Thread} class.
     */
    static const Int32 THREAD_PRIORITY_URGENT_AUDIO = -19;

    /**
     * Minimum increment to make a priority more favorable.
     */
    static const Int32 THREAD_PRIORITY_MORE_FAVORABLE = -1;

    /**
     * Minimum increment to make a priority less favorable.
     */
    static const Int32 THREAD_PRIORITY_LESS_FAVORABLE = +1;

    /**
     * Default thread group - gets a 'normal' share of the CPU
     * @hide
     */
    static const Int32 THREAD_GROUP_DEFAULT = 0;

    /**
     * Background non-interactive thread group - All threads in
     * this group are scheduled with a reduced share of the CPU.
     * @hide
     */
    static const Int32 THREAD_GROUP_BG_NONINTERACTIVE = 1;

    /**
     * Foreground 'boost' thread group - All threads in
     * this group are scheduled with an increased share of the CPU
     * @hide
     **/
    static const Int32 THREAD_GROUP_FG_BOOST = 2;

    static const Int32 SIGNAL_QUIT = 3;

    static const Int32 SIGNAL_KILL = 9;

    static const Int32 SIGNAL_USR1 = 10;

    /** retry interval for opening a zygote socket */
    static const Int32 ZYGOTE_RETRY_MILLIS = 500;

    /** @hide */
    static const Int32 PROC_TERM_MASK = 0xff;

    /** @hide */
    static const Int32 PROC_ZERO_TERM = 0;

    /** @hide */
    static const Int32 PROC_SPACE_TERM = (Int32)' ';

    /** @hide */
    static const Int32 PROC_TAB_TERM = (Int32)'\t';

    /** @hide */
    static const Int32 PROC_COMBINE = 0x100;

    /** @hide */
    static const Int32 PROC_PARENS = 0x200;

    /** @hide */
    static const Int32 PROC_OUT_STRING = 0x1000;

    /** @hide */
    static const Int32 PROC_OUT_LONG = 0x2000;

    /** @hide */
    static const Int32 PROC_OUT_FLOAT = 0x4000;

public:
    class ProcessRunnable : public ElRefBase, public IRunnable
        {
        public:
            ProcessRunnable(
                /* [in] */ String processClass);

            UInt32 AddRef();

            UInt32 Release();

            PInterface Probe(
                /* [in] */ REIID riid);

            CARAPI GetInterfaceID(
                /* [in] */ IInterface* pObject,
                /* [in] */ InterfaceID* pIID);

            ECode Run();

        protected:
            String mProcessClass;
        };

public:
    /**
     * Return the ID of the process that sent you the current transaction
     * that is being processed.  This pid can be used with higher-level
     * system services to determine its identity and check permissions.
     * If the current thread is not currently executing an incoming transaction,
     * then its own pid is returned.
     */
    static CARAPI_(Int32) GetCallingPid();

    /**
     * Return the ID of the user assigned to the process that sent you the
     * current transaction that is being processed.  This uid can be used with
     * higher-level system services to determine its identity and check
     * permissions.  If the current thread is not currently executing an
     * incoming transaction, then its own uid is returned.
     */
    static CARAPI_(Int32) GetCallingUid();

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

    static CARAPI Start(
        /* [in] */ const String processClass,
        /* [in] */ const String niceName,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 gid,
        /* [in] */ const ArrayOf<Int32> & gids,
        /* [in] */ Int32 debugFlags,
        /* [in] */ const ArrayOf<String> & zygoteArgs,
        /* [out] */ Int32* pid);

    /**
    * Start a new process.  Don't supply a custom nice name.
    * {@hide}
    */
    static CARAPI Start(
        /* [in] */ String processClass,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 gid,
        /* [in] */ const ArrayOf<Int32> & gids,
        /* [in] */ Int32 debugFlags,
        /* [in] */ const ArrayOf<String> & zygoteArgs,
        /* [in] */ Int32* pid);

    /**
     * Returns elapsed milliseconds of the time this process has run.
     * @return  Returns the number of milliseconds this process has return.
     */
    static CARAPI_(Int64) GetElapsedCpuTime();

    /**
     * Returns the identifier of this process, which can be used with
     * {@link #killProcess} and {@link #sendSignal}.
     */
    static CARAPI_(Int32) MyPid();

    /**
     * Returns the identifier of the calling thread, which be used with
     * {@link #setThreadPriority(int, int)}.
     */
    static CARAPI_(Int32) MyTid();

    /**
     * Returns the identifier of this process's user.
     */
    static CARAPI_(Int32) MyUid();

    /**
     * Returns the UID assigned to a particular user name, or -1 if there is
     * none.  If the given string consists of only numbers, it is converted
     * directly to a uid.
     */
    static CARAPI_(Int32) GetUidForName(
        /* [in] */ String name);

    /**
     * Returns the GID assigned to a particular user name, or -1 if there is
     * none.  If the given string consists of only numbers, it is converted
     * directly to a gid.
     */
    static CARAPI_(Int32) GetGidForName(
        /* [in] */ String name);

    /**
     * Returns a uid for a currently running process.
     * @param pid the process id
     * @return the uid of the process, or -1 if the process is not running.
     * @hide pending API council review
     */
    static CARAPI_(Int32) GetUidForPid(
        /* [in] */ Int32 pid);

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
    static CARAPI SetThreadPriority(
        /* [in] */ Int32 tid,
        /* [in] */ Int32 priority);

    /**
     * Call with 'false' to cause future calls to {@link #setThreadPriority(int)} to
     * throw an exception if passed a background-level thread priority.  This is only
     * effective if the JNI layer is built with GUARD_THREAD_PRIORITY defined to 1.
     *
     * @hide
     */
    static CARAPI SetCanSelfBackground(
        /* [in] */ Boolean backgroundOk);

    static CARAPI SetThreadPriority(
        /* [in] */ Int32 priority);

    static CARAPI SetProcessGroup(
        /* [in] */ Int32 pid,
        /* [in] */ Int32 group);

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
    static CARAPI_(Int32) GetThreadPriority(
        /* [in] */ Int32 tid);

    static CARAPI_(Boolean) SupportsProcesses();

    static CARAPI_(Boolean) SetOomAdj(
        /* [in] */ Int32 pid,
        /* [in] */ Int32 amt);

    /**
     * Change this process's argv[0] parameter.  This can be useful to show
     * more descriptive information in things like the 'ps' command.
     *
     * @param text The new name of this process.
     *
     * {@hide}
     */
    static CARAPI SetArgV0(
        /* [in] */ String name);

    static CARAPI KillProcess(
        /* [in] */ Int32 pid);
    /** @hide */
    static CARAPI_(Int32) SetUid(
        /* [in] */ Int32 uid);

    /** @hide */
    static CARAPI_(Int32) SetGid(
        /* [in] */ Int32 uid);
    /**
     * Send a signal to the given process.
     *
     * @param pid The pid of the target process.
     * @param signal The signal to send.
     */
    static CARAPI SendSignal(
        /* [in] */ Int32 pid,
        /* [in] */ Int32 signal);

    static CARAPI KillProcessQuiet(
        /* [in] */ Int32 pid);

    /**
     * @hide
     * Private impl for avoiding a log message...  DO NOT USE without doing
     * your own log, or the Android Illuminati will find you some night and
     * beat you up.
     */
    static CARAPI SendSignalQuiet(
        /* [in] */ Int32 pid,
        /* [in] */ Int32 signal);

    /** @hide */
    static CARAPI_(Int64) GetFreeMemory();

    /** @hide */
    static CARAPI ReadProcLines(
        /* [in] */ String path,
        /* [in] */ const ArrayOf<String> & reqFields,
        /* [in] */ const ArrayOf<Int64> & outSizes);

    /** @hide */
    static CARAPI GetPids(
        /* [in] */ String path,
        /* [in] */ const ArrayOf<Int32> & lastArray,
        /* [out] */ ArrayOf<Int32>** newArray);

    /** @hide */
    static CARAPI ReadProcFile(
        /* [in] */ String file,
        /* [in] */ const ArrayOf<Int32> & format,
        /* [in] */ const ArrayOf<String> & outStrings,
        /* [in] */ const ArrayOf<Int64> & outLongs,
        /* [in] */ const ArrayOf<Float> outFloats,
        /* [out] */ Boolean* result);

    /** @hide */
    static CARAPI_(Boolean) ParseProcLine(
        /* [in] */ const ArrayOf<Byte> & buffer,
        /* [in] */ Int32 startIndex,
        /* [in] */ Int32 endIndex,
        /* [in] */ const ArrayOf<Int32> & format,
        /* [in] */ const ArrayOf<String> & outStrings,
        /* [in] */ const ArrayOf<Int64> & outLongs,
        /* [in] */ const ArrayOf<Float> & outFloats);

    /**
     * Gets the total Pss value for a given process, in bytes.
     *
     * @param pid the process to the Pss for
     * @return the total Pss value for the given process in bytes,
     *  or -1 if the value cannot be determined
     * @hide
     */
    static CARAPI_(Int64) GetPss(
        /* [in] */ Int32 pid);

private:
    static CARAPI InvokeStaticMain(
        /* [in] */ String className);

    /**
     * Tries to open socket to Zygote process if not already open. If
     * already open, does nothing.  May block and retry.
     */
    static CARAPI OpenZygoteSocketIfNeeded();

    /**
     * Sends an argument list to the zygote process, which starts a new child
     * and returns the child's pid. Please note: the present implementation
     * replaces newlines in the argument list with spaces.
     * @param args argument list
     * @return PID of new child process
     * @throws ZygoteStartFailedEx if process start failed for any reason
     */
    static CARAPI_(Int32) ZygoteSendArgsAndGetPid(
        /* [in] */ List<String>* args);

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
    static CARAPI StartViaZygote(
        /* [in] */ const String processClass,
        /* [in] */ const String niceName,
        /* [in] */ const Int32 uid,
        /* [in] */ const Int32 gid,
        /* [in] */ const ArrayOf<Int32> & gids,
        /* [in] */ Int32 debugFlags,
        /* [in] */ const ArrayOf<String> & extraArgs,
        /* [out] */ Int32* pid);

// public:
//     /**
//      * Name of a process for running the platform's media services.
//      * {@hide}
//      */
//     static const String ANDROID_SHARED_MEDIA = "com.android.process.media";

//     /**
//      * Name of the process that Google content providers can share.
//      * {@hide}
//      */
//     static const String GOOGLE_SHARED_APP_CONTENT = "com.google.process.content";

 private:

//     const String LOG_TAG = "Process";


};
#endif // __PROCESS_H__
