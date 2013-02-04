
#include "Globals.h"
#include "NativeThread.h"

/* global state */
struct DvmGlobals gDvm;

/*
 * Set defaults for fields altered or modified by arguments.
 *
 * Globals are initialized to 0 (a/k/a NULL or false).
 */
static void SetCommandLineDefaults()
{
    // const char* envStr;

    // envStr = getenv("CLASSPATH");
    // if (envStr != NULL)
    //     gDvm.classPathStr = strdup(envStr);
    // else
    //     gDvm.classPathStr = strdup(".");
    // envStr = getenv("BOOTCLASSPATH");
    // if (envStr != NULL)
    //     gDvm.bootClassPathStr = strdup(envStr);
    // else
    //     gDvm.bootClassPathStr = strdup(".");

    /* Defaults overridden by -Xms and -Xmx.
     * TODO: base these on a system or application-specific default
     */
    gDvm.mHeapSizeStart = 2 * 1024 * 1024;   // Spec says 16MB; too big for us.
    gDvm.mHeapSizeMax = 16 * 1024 * 1024;    // Spec says 75% physical mem
    gDvm.mStackSize = kDefaultStackSize;

    //gDvm.concurrentMarkSweep = true;

    /* gDvm.jdwpSuspend = true; */

    /* allowed unless zygote config doesn't allow it */
    //gDvm.jdwpAllowed = true;

    /* default verification and optimization modes */
    // gDvm.classVerifyMode = VERIFY_MODE_ALL;
    // gDvm.dexOptMode = OPTIMIZE_MODE_VERIFIED;

    /*
     * Default execution mode.
     *
     * This should probably interact with the mterp code somehow, e.g. if
     * we know we're using the "desktop" build we should probably be
     * using "portable" rather than "fast".
     */
// #if defined(WITH_JIT)
//     gDvm.executionMode = kExecutionModeJit;
// #else
//     gDvm.executionMode = kExecutionModeInterpFast;
// #endif

    /*
     * SMP support is a compile-time define, but we may want to have
     * dexopt target a differently-configured device.
     */
    //gDvm.dexOptForSmp = (ANDROID_SMP != 0);
}

/*
 * All threads have stopped.  Finish the shutdown procedure.
 *
 * We can also be called if startup fails partway through, so be prepared
 * to deal with partially initialized data.
 *
 * Free any storage allocated in gGlobals.
 *
 * We can't dlclose() shared libs we've loaded, because it's possible a
 * thread not associated with the VM is running code in one.
 *
 * This is called from the JNI DestroyJavaVM function, which can be
 * called from any thread.  (In practice, this will usually run in the
 * same thread that started the VM, a/k/a the main thread, but we don't
 * want to assume that.)
 */
void NativeShutdown()
{}

/*
 * VM initialization.  Pass in any options provided on the command line.
 * Do not pass in the class name or the options for the class.
 *
 * Returns 0 on success.
 */
Int32 NativeStartup()
{
    //LOGV("VM init args (%d):\n", argc);
    // for (i = 0; i < argc; i++)
    //     LOGV("  %d: '%s'\n", i, argv[i]);

    SetCommandLineDefaults();

    /*
     * Initialize components.
     */
    if (!NativeThreadStartup())
        goto fail;

    /*
     * At this point, the VM is in a pretty good state.  Finish prep on
     * the main thread (specifically, create a java.lang.Thread object to go
     * along with our Thread struct).  Note we will probably be executing
     * some interpreted class initializer code in here.
     */
    if (!NativePrepMainThread())
        goto fail;

    /*
     * Init for either zygote mode or non-zygote mode.  The key difference
     * is that we don't start any additional threads in Zygote mode.
     */
    // if (gDvm.zygote) {
    //     if (!dvmInitZygote())
    //         goto fail;
    // } else {
    //     if (!dvmInitAfterZygote())
    //         goto fail;
    // }

    // assert(!dvmCheckException(dvmThreadSelf()));
    // gDvm.initExceptionCount = 0;

    return 0;

fail:
    NativeShutdown();
    return 1;
}
