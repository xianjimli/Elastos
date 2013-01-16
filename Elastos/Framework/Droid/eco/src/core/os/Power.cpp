
#include "os/Power.h"
#include <hardware_legacy/power.h>
#include <sys/reboot.h>

const Int32 Power::PARTIAL_WAKE_LOCK;
const Int32 Power::FULL_WAKE_LOCK;
const Int32 Power::BRIGHTNESS_OFF;
const Int32 Power::BRIGHTNESS_DIM;
const Int32 Power::BRIGHTNESS_ON;
const Int32 Power::BRIGHTNESS_LOW_BATTERY;
const Int32 Power::LOW_BATTERY_THRESHOLD;

void Power::AcquireWakeLock(
    /* [in] */ Int32 lock,
    /* [in] */ const String& id)
{
    if (id.IsNull()) {
        // throw_NullPointerException(env, "id is null");
        return E_NULL_POINTER_EXCEPTION;
    }

    acquire_wake_lock(lock, id.string());
}

void Power::ReleaseWakeLock(
    /* [in] */ const String& id)
{
    if (id.IsNull()) {
        // throw_NullPointerException(env, "id is null");
        return E_NULL_POINTER_EXCEPTION;
    }

    release_wake_lock(id.string());
}

Int32 Power::SetScreenState(
    /* [in] */ Boolean on)
{
    return set_screen_state(on);
}

Int32 Power::SetLastUserActivityTimeout(
    /* [in] */ Int64 ms)
{
    return set_last_user_activity_timeout(timeMS/1000);
}

void Power::Shutdown()
{
    sync();
#ifdef HAVE_ANDROID_OS
    reboot(RB_POWER_OFF);
#endif
}

ECode Power::Reboot(
    /* [in] */ const String& reason)
{
    return NativeReboot(reason);
}

ECode Power::NativeReboot(
    /* [in] */ const String& reason)
{
    sync();
#ifdef HAVE_ANDROID_OS
    if (reason.IsNull()) {
        reboot(RB_AUTOBOOT);
    }
    else {
        const char *chars = reason.string();
        __reboot(LINUX_REBOOT_MAGIC1, LINUX_REBOOT_MAGIC2,
                 LINUX_REBOOT_CMD_RESTART2, (char*) chars);
    }
    // jniThrowIOException(env, errno);
    return E_IO_EXCEPTION;
#endif
}
