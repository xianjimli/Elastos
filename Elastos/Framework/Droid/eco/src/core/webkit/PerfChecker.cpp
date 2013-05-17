
#include "webkit/PerfChecker.h"

const Int64 PerfChecker::mResponseThreshold;

PerfChecker::PerfChecker()
{
	if (FALSE) {
//        mTime = SystemClock.uptimeMillis();
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
        Int64 upTime;// = SystemClock.uptimeMillis();
        Int64 time =  upTime - mTime;
        if (time > mResponseThreshold) {
//            Log.w("webkit", what + " used " + time + " ms");
        }
        // Reset mTime, to permit reuse
        mTime = upTime;
    }
}