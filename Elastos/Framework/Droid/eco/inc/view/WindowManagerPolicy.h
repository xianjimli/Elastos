/**
 * This interface supplies all UI-specific behavior of the window manager.  An
 * instance of it is created by the window manager when it starts up, and allows
 * customization of window layering, special window types, key dispatching, and
 * layout.
 *
 * <p>Because this provides deep interaction with the system window manager,
 * specific methods on this interface can be called from a variety of contexts
 * with various restrictions on what they can do.  These are encoded through
 * a suffixes at the end of a method encoding the thread the method is called
 * from and any locks that are held when it is being called; if no suffix
 * is attached to a method, then it is not called with any locks and may be
 * called from the main window manager thread or another thread calling into
 * the window manager.
 *
 * <p>The current suffixes are:
 *
 * <dl>
 * <dt> Ti <dd> Called from the input thread.  This is the thread that
 * collects pending input events and dispatches them to the appropriate window.
 * It may block waiting for events to be processed, so that the input stream is
 * properly serialized.
 * <dt> Tq <dd> Called from the low-level input queue thread.  This is the
 * thread that reads events out of the raw input devices and places them
 * into the global input queue that is read by the <var>Ti</var> thread.
 * This thread should not block for a long period of time on anything but the
 * key driver.
 * <dt> Lw <dd> Called with the main window manager lock held.  Because the
 * window manager is a very low-level system service, there are few other
 * system services you can call with this lock held.  It is explicitly okay to
 * make calls into the package manager and power manager; it is explicitly not
 * okay to make calls into the activity manager or most other services.  Note that
 * {@link android.content.Context#checkPermission(String, Int32, Int32)} and
 * variations require calling into the activity manager.
 * <dt> Li <dd> Called with the input thread lock held.  This lock can be
 * acquired by the window manager while it holds the window lock, so this is
 * even more restrictive than <var>Lw</var>.
 * </dl>
 *
 * @hide
 */

#ifndef __WINDOWMANAGERPOLICY_H__
#define __WINDOWMANAGERPOLICY_H__

#include <elastos.h>

using namespace Elastos;

class WindowManagerPolicy
{
public:
    // Policy flags.  These flags are also defined in frameworks/base/include/ui/Input.h.
    static const Int32 FLAG_WAKE = 0x00000001;
    static const Int32 FLAG_WAKE_DROPPED = 0x00000002;
    static const Int32 FLAG_SHIFT = 0x00000004;
    static const Int32 FLAG_CAPS_LOCK = 0x00000008;
    static const Int32 FLAG_ALT = 0x00000010;
    static const Int32 FLAG_ALT_GR = 0x00000020;
    static const Int32 FLAG_MENU = 0x00000040;
    static const Int32 FLAG_LAUNCHER = 0x00000080;
    static const Int32 FLAG_VIRTUAL = 0x00000100;

    static const Int32 FLAG_INJECTED = 0x01000000;
    static const Int32 FLAG_TRUSTED = 0x02000000;

    static const Int32 FLAG_WOKE_HERE = 0x10000000;
    static const Int32 FLAG_BRIGHT_HERE = 0x20000000;
    static const Int32 FLAG_PASS_TO_USER = 0x40000000;

    static const Boolean WATCH_pointER = FALSE;

    // flags for interceptKeyTq
    /**
     * Pass this event to the user / app.  To be returned from {@link #interceptKeyTq}.
     */
    static const Int32 ACTION_PASS_TO_USER = 0x00000001;

    /**
     * This key event should extend the user activity timeout and turn the lights on.
     * To be returned from {@link #interceptKeyTq}. Do not return this and
     * {@link #ACTION_GO_TO_SLEEP} or {@link #ACTION_PASS_TO_USER}.
     */
    static const Int32 ACTION_POKE_USER_ACTIVITY = 0x00000002;

    /**
     * This key event should put the device to sleep (and engage keyguard if necessary)
     * To be returned from {@link #interceptKeyTq}.  Do not return this and
     * {@link #ACTION_POKE_USER_ACTIVITY} or {@link #ACTION_PASS_TO_USER}.
     */
    static const Int32 ACTION_GO_TO_SLEEP = 0x00000004;

    /**
     * Bit mask that is set for all enter transition.
     */
    static const Int32 TRANSIT_ENTER_MASK = 0x1000;

    /**
     * Bit mask that is set for all exit transitions.
     */
    static const Int32 TRANSIT_EXIT_MASK = 0x2000;

    /** Not set up for a transition. */
    static const Int32 TRANSIT_UNSET = -1;
    /** No animation for transition. */
    static const Int32 TRANSIT_NONE = 0;
    /** Window has been added to the screen. */
    static const Int32 TRANSIT_ENTER = 1 | TRANSIT_ENTER_MASK;
    /** Window has been removed from the screen. */
    static const Int32 TRANSIT_EXIT = 2 | TRANSIT_EXIT_MASK;
    /** Window has been made visible. */
    static const Int32 TRANSIT_SHOW = 3 | TRANSIT_ENTER_MASK;
    /** Window has been made invisible. */
    static const Int32 TRANSIT_HIDE = 4 | TRANSIT_EXIT_MASK;
    /** The "application starting" preview window is no longer needed, and will
     * animate away to show the real window. */
    static const Int32 TRANSIT_PREVIEW_DONE = 5;
    /** A window in a new activity is being opened on top of an existing one
     * in the same task. */
    static const Int32 TRANSIT_ACTIVITY_OPEN = 6 | TRANSIT_ENTER_MASK;
    /** The window in the top-most activity is being closed to reveal the
     * previous activity in the same task. */
    static const Int32 TRANSIT_ACTIVITY_CLOSE = 7 | TRANSIT_EXIT_MASK;
    /** A window in a new task is being opened on top of an existing one
     * in another activity's task. */
    static const Int32 TRANSIT_TASK_OPEN = 8 | TRANSIT_ENTER_MASK;
    /** A window in the top-most activity is being closed to reveal the
     * previous activity in a different task. */
    static const Int32 TRANSIT_TASK_CLOSE = 9 | TRANSIT_EXIT_MASK;
    /** A window in an existing task is being displayed on top of an existing one
     * in another activity's task. */
    static const Int32 TRANSIT_TASK_TO_FRONT = 10 | TRANSIT_ENTER_MASK;
    /** A window in an existing task is being put below all other tasks. */
    static const Int32 TRANSIT_TASK_TO_BACK = 11 | TRANSIT_EXIT_MASK;
    /** A window in a new activity that doesn't have a wallpaper is being
     * opened on top of one that does, effectively closing the wallpaper. */
    static const Int32 TRANSIT_WALLPAPER_CLOSE = 12 | TRANSIT_EXIT_MASK;
    /** A window in a new activity that does have a wallpaper is being
     * opened on one that didn't, effectively opening the wallpaper. */
    static const Int32 TRANSIT_WALLPAPER_OPEN = 13 | TRANSIT_ENTER_MASK;
    /** A window in a new activity is being opened on top of an existing one,
     * and both are on top of the wallpaper. */
    static const Int32 TRANSIT_WALLPAPER_Int32RA_OPEN = 14 | TRANSIT_ENTER_MASK;
    /** The window in the top-most activity is being closed to reveal the
     * previous activity, and both are on top of he wallpaper. */
    static const Int32 TRANSIT_WALLPAPER_Int32RA_CLOSE = 15 | TRANSIT_EXIT_MASK;

    // NOTE: screen off reasons are in order of significance, with more
    // important ones lower than less important ones.

    /** Screen turned off because of a device admin */
    static const Int32 OFF_BECAUSE_OF_ADMIN = 1;
    /** Screen turned off because of power button */
    static const Int32 OFF_BECAUSE_OF_USER = 2;
    /** Screen turned off because of timeout */
    static const Int32 OFF_BECAUSE_OF_TIMEOUT = 3;
    /** Screen turned off because of proximity sensor */
    static const Int32 OFF_BECAUSE_OF_PROX_SENSOR = 4;

    /**
     * Magic constant to {@link IWindowManager#setRotation} to not actually
     * modify the rotation.
     */
    static const Int32 USE_LAST_ROTATION = -1000;

    /** Layout state may have changed (so another layout will be performed) */
    static const Int32 FINISH_LAYOUT_REDO_LAYOUT = 0x0001;
    /** Configuration state may have changed */
    static const Int32 FINISH_LAYOUT_REDO_CONFIG = 0x0002;
    /** Wallpaper may need to move */
    static const Int32 FINISH_LAYOUT_REDO_WALLPAPER = 0x0004;
    /** Need to recompute animations */
    static const Int32 FINISH_LAYOUT_REDO_ANIM = 0x0008;
};

#endif //__WINDOWMANAGERPOLICY_H__
