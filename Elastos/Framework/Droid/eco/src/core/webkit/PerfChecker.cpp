#include <Logger.h>
#include <StringBuffer.h>

#include "webkit/PerfChecker.h"
#include "os/SystemClock.h"

using namespace Elastos::Utility::Logging;

const Int64 PerfChecker::mResponseThreshold;

PerfChecker::PerfChecker()
{
	if (FALSE) {
        //mTime = SystemClock.uptimeMillis();
        mTime = SystemClock::GetUptimeMillis();
    }
}

/**
 * @param what log string
 * Logs given string if mResponseThreshold time passed between either
 * instantiation or previous responseAlert call
 */
CARAPI_(void) PerfChecker::ResponseAlert(
	/* [in] */ CString what)
{
	if (FALSE) {
        Int64 upTime = SystemClock::GetUptimeMillis();// = SystemClock.uptimeMillis();
        Int64 time =  upTime - mTime;
        if (time > mResponseThreshold) {
            Logger::W("webkit", StringBuffer(what) + " used %d ms" + time);
        }
        // Reset mTime, to permit reuse
        mTime = upTime;
    }
}