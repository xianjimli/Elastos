
#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <elastos.h>

using namespace Elastos;

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
     * Sets the scheduling group for a process and all child threads
     * @hide
     * @param pid The indentifier of the process to change.
     * @param group The target group for this process.
     *
     * @throws IllegalArgumentException Throws IllegalArgumentException if
     * <var>tid</var> does not exist.
     * @throws SecurityException Throws SecurityException if your process does
     * not have permission to modify the given thread, or to use the given
     * priority.
     */
    static CARAPI SetProcessGroup(
        /* [in] */ Int32 pid,
        /* [in] */ Int32 group);

    /**
     * Set the priority of the calling thread, based on Linux priorities.  See
     * {@link #setThreadPriority(int, int)} for more information.
     *
     * @param priority A Linux priority level, from -20 for highest scheduling
     * priority to 19 for lowest scheduling priority.
     *
     * @throws IllegalArgumentException Throws IllegalArgumentException if
     * <var>tid</var> does not exist.
     * @throws SecurityException Throws SecurityException if your process does
     * not have permission to modify the given thread, or to use the given
     * priority.
     *
     * @see #setThreadPriority(int, int)
     */
    static CARAPI_(void) SetThreadPriority(
        /* [in] */ Int32 priority);

    /**
     * Determine whether the current environment supports multiple processes.
     *
     * @return Returns true if the system can run in multiple processes, else
     * false if everything is running in a single process.
     */
    static CARAPI_(Boolean) SupportsProcesses();

    /**
     * Set the out-of-memory badness adjustment for a process.
     *
     * @param pid The process identifier to set.
     * @param amt Adjustment value -- linux allows -16 to +15.
     *
     * @return Returns true if the underlying system supports this
     *         feature, else false.
     *
     * {@hide}
     */
    static CARAPI_(Boolean) SetOomAdj(
        /* [in] */ Int32 pid,
        /* [in] */ Int32 amt);

    static CARAPI_(void) KillProcess(
        /* [in] */ Int32 pid);

    /**
     * @hide
     * Private impl for avoiding a log message...  DO NOT USE without doing
     * your own log, or the Android Illuminati will find you some night and
     * beat you up.
     */
    static CARAPI_(void) KillProcessQuiet(
        /* [in] */ Int32 pid);

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
        /* [in] */ const String& name);

    /**
     * Returns the GID assigned to a particular user name, or -1 if there is
     * none.  If the given string consists of only numbers, it is converted
     * directly to a gid.
     */
    static CARAPI_(Int32) GetGidForName(
        /* [in] */ const String& name);
};

#endif // __PROCESS_H__
