
#ifndef __SYSTEMCLOCK_H__
#define __SYSTEMCLOCK_H__

#include <elastos.h>

using namespace Elastos;

class SystemClock
{
public:
    /**
     * Returns milliseconds since boot, not counting time spent in deep sleep.
     * <b>Note:</b> This value may get reset occasionally (before it would
     * otherwise wrap around).
     *
     * @return milliseconds of non-sleep uptime since boot.
     */
    static Millisecond64 GetUptimeMillis();

    /**
     * Returns milliseconds since boot, including time spent in sleep.
     *
     * @return elapsed milliseconds since boot.
     */
    static Millisecond64 GetElapsedRealtime();

    /**
     * Returns milliseconds since boot, not counting time spent in deep sleep.
     * <b>Note:</b> This value may get reset occasionally (before it would
     * otherwise wrap around).
     *
     * @return milliseconds of non-sleep uptime since boot.
     */
    static Int64 UptimeMillis() { return 0; }
};

#endif //__SYSTEMCLOCK_H__
