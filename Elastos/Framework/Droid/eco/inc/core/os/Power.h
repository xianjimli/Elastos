
#ifndef __POWER_H__
#define __POWER_H__

#include "ext/frameworkext.h"

using namespace Elastos;

/**
 * Class that provides access to some of the power management functions.
 *
 * {@hide}
 */
class Power
{
public:
    static CARAPI_(void) AcquireWakeLock(
        /* [in] */ Int32 lock,
        /* [in] */ const String& id);

    static CARAPI_(void) ReleaseWakeLock(
        /* [in] */ const String& id);

    /**
     * Turn the screen on or off
     *
     * @param on Whether you want the screen on or off
     */
    static CARAPI_(Int32) SetScreenState(
        /* [in] */ Boolean on);

    static CARAPI_(Int32) SetLastUserActivityTimeout(
        /* [in] */ Int64 ms);

    /**
     * Low-level function turn the device off immediately, without trying
     * to be clean.  Most people should use
     * {@link android.internal.app.ShutdownThread} for a clean shutdown.
     *
     * @deprecated
     * @hide
     */
    static CARAPI_(void) Shutdown();

    /**
     * Reboot the device.
     * @param reason code to pass to the kernel (e.g. "recovery"), or null.
     *
     * @throws IOException if reboot fails for some reason (eg, lack of permission)
     */
    static CARAPI Reboot(
        /* [in] */ const String& reason);

private:
    // can't instantiate this class
    Power() {}

    static CARAPI NativeReboot(
        /* [in] */ const String& reason);

public:
    /**
     * Wake lock that ensures that the CPU is running.  The screen might not be on.
     */
    static const Int32 PARTIAL_WAKE_LOCK = 1;

    /**
     * Wake lock that ensures that the screen is on.
     */
    static const Int32 FULL_WAKE_LOCK = 2;

    /**
     * Brightness value for fully off
     */
    static const Int32 BRIGHTNESS_OFF = 0;

    /**
     * Brightness value for dim backlight
     */
    static const Int32 BRIGHTNESS_DIM = 20;

    /**
     * Brightness value for fully on
     */
    static const Int32 BRIGHTNESS_ON = 255;

    /**
     * Brightness value to use when battery is low
     */
    static const Int32 BRIGHTNESS_LOW_BATTERY = 10;

    /**
     * Threshold for BRIGHTNESS_LOW_BATTERY (percentage)
     * Screen will stay dim if battery level is <= LOW_BATTERY_THRESHOLD
     */
    static const Int32 LOW_BATTERY_THRESHOLD = 10;

};

#endif // __POWERH__
