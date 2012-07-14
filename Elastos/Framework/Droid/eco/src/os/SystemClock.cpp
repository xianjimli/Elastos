
#include "os/SystemClock.h"
#include <utils/SystemClock.h>
#include <utils/Timers.h>

#define MillisecPerSec 1000
#define MicrosecPerSec 1000000

//extern Millisecond64 elapsedRealtime();

//temporary implementation, should link to android-linux toolbox.so
Millisecond64 elapsedRealtime()
{
    return 0;
}

Millisecond64 SystemClock::GetUptimeMillis()
{
    Int64 when = systemTime(SYSTEM_TIME_MONOTONIC);
    return (Millisecond64)nanoseconds_to_milliseconds(when);
}

Millisecond64 SystemClock::GetElapsedRealtime()
{
    //call android-linux toolbox api
    return elapsedRealtime();
}
