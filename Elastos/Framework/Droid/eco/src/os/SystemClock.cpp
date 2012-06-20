
#include "os/SystemClock.h"
#include <time.h>

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
    struct timeval tp;
    long long now;
    gettimeofday(&tp, NULL);
    now = (long long)tp.tv_sec * MillisecPerSec;
    now += (long long)tp.tv_usec / (MicrosecPerSec / MillisecPerSec);
    return (Millisecond64)now;
}

Millisecond64 SystemClock::GetElapsedRealtime()
{
    //call android-linux toolbox api
    return elapsedRealtime();
}
