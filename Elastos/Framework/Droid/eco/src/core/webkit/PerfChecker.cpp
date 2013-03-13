
#include "webkit/PerfChecker.h"

PerfChecker::PerfChecker()
{}

/**
 * @param what log string
 * Logs given string if mResponseThreshold time passed between either
 * instantiation or previous responseAlert call
 */
CARAPI_(void) PerfChecker::ResponseAlert(
	/* [in] */ CString what)
{}