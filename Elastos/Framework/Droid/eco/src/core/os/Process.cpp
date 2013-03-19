
#include "os/Process.h"


const Int32 Process::SYSTEM_UID;
const Int32 Process::PHONE_UID;
const Int32 Process::SHELL_UID;
const Int32 Process::LOG_UID;
const Int32 Process::WIFI_UID;
const Int32 Process::NFC_UID;
const Int32 Process::FIRST_APPLICATION_UID;
const Int32 Process::LAST_APPLICATION_UID;
const Int32 Process::BLUETOOTH_GID;
const Int32 Process::THREAD_PRIORITY_DEFAULT;
const Int32 Process::THREAD_PRIORITY_LOWEST;
const Int32 Process::THREAD_PRIORITY_BACKGROUND;
const Int32 Process::THREAD_PRIORITY_FOREGROUND;
const Int32 Process::THREAD_PRIORITY_DISPLAY;
const Int32 Process::THREAD_PRIORITY_URGENT_DISPLAY;
const Int32 Process::THREAD_PRIORITY_AUDIO;
const Int32 Process::THREAD_PRIORITY_URGENT_AUDIO;
const Int32 Process::THREAD_PRIORITY_MORE_FAVORABLE;
const Int32 Process::THREAD_PRIORITY_LESS_FAVORABLE;
const Int32 Process::THREAD_GROUP_DEFAULT;
const Int32 Process::THREAD_GROUP_BG_NONINTERACTIVE;
const Int32 Process::THREAD_GROUP_FG_BOOST;
const Int32 Process::SIGNAL_QUIT;
const Int32 Process::SIGNAL_KILL;
const Int32 Process::SIGNAL_USR1;
const Int32 Process::ZYGOTE_RETRY_MILLIS;
const Int32 Process::PROC_TERM_MASK;
const Int32 Process::PROC_ZERO_TERM;
const Int32 Process::PROC_SPACE_TERM;
const Int32 Process::PROC_TAB_TERM;
const Int32 Process::PROC_COMBINE;
const Int32 Process::PROC_PARENS;
const Int32 Process::PROC_OUT_STRING;
const Int32 Process::PROC_OUT_LONG;
const Int32 Process::PROC_OUT_FLOAT;

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

Int32 Process::Start(
    /* [in] */ const String processClass,
    /* [in] */ const String niceName,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 gid,
    /* [in] */ const ArrayOf<Int32> & gids,
    /* [in] */ Int32 debugFlags,
    /* [in] */ const ArrayOf<String> & zygoteArgs)
{
    // if (supportsProcesses()) {
    //     try {
    //         return startViaZygote(processClass, niceName, uid, gid, gids,
    //                 debugFlags, zygoteArgs);
    //     } catch (ZygoteStartFailedEx ex) {
    //         Log.e(LOG_TAG,
    //                 "Starting VM process through Zygote failed");
    //         throw new RuntimeException(
    //                 "Starting VM process through Zygote failed", ex);
    //     }
    // } else {
    //     // Running in single-process mode

    //     Runnable runnable = new Runnable() {
    //                 public void run() {
    //                     Process.invokeStaticMain(processClass);
    //                 }
    //     };

    //     // Thread constructors must not be called with null names (see spec).
    //     if (niceName != null) {
    //         new Thread(runnable, niceName).start();
    //     } else {
    //         new Thread(runnable).start();
    //     }

    //     return 0;
    // }
    return -1;
}

/**
* Start a new process.  Don't supply a custom nice name.
* {@hide}
*/
Int32 Process::Start(
    /* [in] */ String processClass,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 gid,
    /* [in] */ const ArrayOf<Int32> & gids,
    /* [in] */ Int32 debugFlags,
    /* [in] */ const ArrayOf<String> & zygoteArgs)
{
    // return start(processClass, "", uid, gid, gids,
    //     debugFlags, zygoteArgs);
    return -1;
}

ECode Process::InvokeStaticMain(
    /* [in] */ String className)
{
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
//     int retryCount;

//     if (sPreviousZygoteOpenFailed) {
//         /*
//          * If we've failed before, expect that we'll fail again and
//          * don't pause for retries.
//          */
//         retryCount = 0;
//     } else {
//         retryCount = 10;
//     }

//     /*
//      * See bug #811181: Sometimes runtime can make it up before zygote.
//      * Really, we'd like to do something better to avoid this condition,
//      * but for now just wait a bit...
//      */
//     for (int retry = 0
//             ; (sZygoteSocket == null) && (retry < (retryCount + 1))
//             ; retry++ ) {

//         if (retry > 0) {
//             try {
//                 Log.i("Zygote", "Zygote not up yet, sleeping...");
//                 Thread.sleep(ZYGOTE_RETRY_MILLIS);
//             } catch (InterruptedException ex) {
//                 // should never happen
//             }
//         }

//         try {
//             sZygoteSocket = new LocalSocket();

//             sZygoteSocket.connect(new LocalSocketAddress(ZYGOTE_SOCKET,
//                     LocalSocketAddress.Namespace.RESERVED));

//             sZygoteInputStream
//                     = new DataInputStream(sZygoteSocket.getInputStream());

//             sZygoteWriter =
//                 new BufferedWriter(
//                         new OutputStreamWriter(
//                                 sZygoteSocket.getOutputStream()),
//                         256);

//             Log.i("Zygote", "Process: zygote socket opened");

//             sPreviousZygoteOpenFailed = false;
//             break;
//         } catch (IOException ex) {
//             if (sZygoteSocket != null) {
//                 try {
//                     sZygoteSocket.close();
//                 } catch (IOException ex2) {
//                     Log.e(LOG_TAG,"I/O exception on close after exception",
//                             ex2);
//                 }
//             }

//             sZygoteSocket = null;
//         }
//     }

//     if (sZygoteSocket == null) {
//         sPreviousZygoteOpenFailed = true;
//         throw new ZygoteStartFailedEx("connect failed");
//     }
    return E_NOT_IMPLEMENTED;
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
    /* [in] */ List<String> args)
{
    // int pid;

    // openZygoteSocketIfNeeded();

    // try {
    //     /**
    //      * See com.android.internal.os.ZygoteInit.readArgumentList()
    //      * Presently the wire format to the zygote process is:
    //      * a) a count of arguments (argc, in essence)
    //      * b) a number of newline-separated argument strings equal to count
    //      *
    //      * After the zygote process reads these it will write the pid of
    //      * the child or -1 on failure.
    //      */

    //     sZygoteWriter.write(Integer.toString(args.size()));
    //     sZygoteWriter.newLine();

    //     int sz = args.size();
    //     for (int i = 0; i < sz; i++) {
    //         String arg = args.get(i);
    //         if (arg.indexOf('\n') >= 0) {
    //             throw new ZygoteStartFailedEx(
    //                     "embedded newlines not allowed");
    //         }
    //         sZygoteWriter.write(arg);
    //         sZygoteWriter.newLine();
    //     }

    //     sZygoteWriter.flush();

    //     // Should there be a timeout on this?
    //     pid = sZygoteInputStream.readInt();

    //     if (pid < 0) {
    //         throw new ZygoteStartFailedEx("fork() failed");
    //     }
    // } catch (IOException ex) {
    //     try {
    //         if (sZygoteSocket != null) {
    //             sZygoteSocket.close();
    //         }
    //     } catch (IOException ex2) {
    //         // we're going to fail anyway
    //         Log.e(LOG_TAG,"I/O exception on routine close", ex2);
    //     }

    //     sZygoteSocket = null;

    //     throw new ZygoteStartFailedEx(ex);
    // }

    // return pid;
    return -1;
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
Int32 Process::StartViaZygote(
    /* [in] */ const String processClass,
    /* [in] */ const String niceName,
    /* [in] */ const Int32 uid,
    /* [in] */ const Int32 gid,
    /* [in] */ const ArrayOf<Int32> & gids,
    /* [in] */ Int32 debugFlags,
    /* [in] */ const ArrayOf<String> & extraArgs)
{
    // int pid;

    // synchronized(Process.class) {
    //     ArrayList<String> argsForZygote = new ArrayList<String>();

    //     // --runtime-init, --setuid=, --setgid=,
    //     // and --setgroups= must go first
    //     argsForZygote.add("--runtime-init");
    //     argsForZygote.add("--setuid=" + uid);
    //     argsForZygote.add("--setgid=" + gid);
    //     if ((debugFlags & Zygote.DEBUG_ENABLE_SAFEMODE) != 0) {
    //         argsForZygote.add("--enable-safemode");
    //     }
    //     if ((debugFlags & Zygote.DEBUG_ENABLE_DEBUGGER) != 0) {
    //         argsForZygote.add("--enable-debugger");
    //     }
    //     if ((debugFlags & Zygote.DEBUG_ENABLE_CHECKJNI) != 0) {
    //         argsForZygote.add("--enable-checkjni");
    //     }
    //     if ((debugFlags & Zygote.DEBUG_ENABLE_ASSERT) != 0) {
    //         argsForZygote.add("--enable-assert");
    //     }

    //     //TODO optionally enable debuger
    //     //argsForZygote.add("--enable-debugger");

    //     // --setgroups is a comma-separated list
    //     if (gids != null && gids.length > 0) {
    //         StringBuilder sb = new StringBuilder();
    //         sb.append("--setgroups=");

    //         int sz = gids.length;
    //         for (int i = 0; i < sz; i++) {
    //             if (i != 0) {
    //                 sb.append(',');
    //             }
    //             sb.append(gids[i]);
    //         }

    //         argsForZygote.add(sb.toString());
    //     }

    //     if (niceName != null) {
    //         argsForZygote.add("--nice-name=" + niceName);
    //     }

    //     argsForZygote.add(processClass);

    //     if (extraArgs != null) {
    //         for (String arg : extraArgs) {
    //             argsForZygote.add(arg);
    //         }
    //     }

    //     pid = zygoteSendArgsAndGetPid(argsForZygote);
    // }

    // if (pid <= 0) {
    //     throw new ZygoteStartFailedEx("zygote start failed:" + pid);
    // }

    // return pid;
    return -1;
}

/**
 * Returns elapsed milliseconds of the time this process has run.
 * @return  Returns the number of milliseconds this process has return.
 */
Int64 Process::GetElapsedCpuTime()
{
    return -1;
}

/**
 * Returns the identifier of this process, which can be used with
 * {@link #killProcess} and {@link #sendSignal}.
 */
Int32 Process::MyPid()
{
    return -1;
}

/**
 * Returns the identifier of the calling thread, which be used with
 * {@link #setThreadPriority(int, int)}.
 */
Int32 Process::MyTid()
{
    return -1;
}

/**
 * Returns the identifier of this process's user.
 */
Int32 Process::MyUid()
{
    return -1;
}

/**
 * Returns the UID assigned to a particular user name, or -1 if there is
 * none.  If the given string consists of only numbers, it is converted
 * directly to a uid.
 */
Int32 Process::GetUidForName(
    /* [in] */ String name)
{
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
    // String[] procStatusLabels = { "Uid:" };
    // long[] procStatusValues = new long[1];
    // procStatusValues[0] = -1;
    // Process.readProcLines("/proc/" + pid + "/status", procStatusLabels, procStatusValues);
    // return (int) procStatusValues[0];

    return -1;
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
    return E_NOT_IMPLEMENTED;
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
    return E_NOT_IMPLEMENTED;
}

ECode Process::SetThreadPriority(
    /* [in] */ Int32 priority)
{
    return E_NOT_IMPLEMENTED;
}

ECode Process::SetProcessGroup(
    /* [in] */ Int32 pid,
    /* [in] */ Int32 group)
{
    return E_NOT_IMPLEMENTED;
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
    return -1;
}

Boolean Process::SupportsProcesses()
{
    //not implemented;
    return TRUE;
}

Boolean Process::SetOomAdj(
    /* [in] */ Int32 pid,
    /* [in] */ Int32 amt)
{
    //not implemented;
    return TRUE;
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
    /* [in] */ String text)
{
    return E_NOT_IMPLEMENTED;
}

ECode Process::KillProcess(
    /* [in] */ Int32 pid)
{
    return E_NOT_IMPLEMENTED;
}

/** @hide */
Int32 Process::SetUid(
    /* [in] */ Int32 uid)
{
    return -1;
}

/** @hide */
Int32 Process::SetGid(
    /* [in] */ Int32 uid)
{
    return -1;
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
    return E_NOT_IMPLEMENTED;
}

ECode Process::KillProcessQuiet(
    /* [in] */ Int32 pid)
{
    return E_NOT_IMPLEMENTED;
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
    return E_NOT_IMPLEMENTED;
}

/** @hide */
Int64 Process::GetFreeMemory()
{
    return -1;
}

/** @hide */
ECode Process::ReadProcLines(
    /* [in] */ String path,
    /* [in] */ const ArrayOf<String> & reqFields,
    /* [in] */ const ArrayOf<Int64> & outSizes)
{
    return E_NOT_IMPLEMENTED;
}

/** @hide */
ArrayOf<Int32>* Process::GetPids(
    /* [in] */ String path,
    /* [in] */ const ArrayOf<Int32> & lastArray)
{
    return NULL;
}

/** @hide */
Boolean Process::ReadProcFile(
    /* [in] */ String file,
    /* [in] */ const ArrayOf<Int32> & format,
    /* [in] */ const ArrayOf<String> & outStrings,
    /* [in] */ const ArrayOf<Int64> & outLongs,
    /* [in] */ const ArrayOf<Float> outFloats)
{
    return FALSE;
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
    return FALSE;
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
    return -1;
}

