
#include "ext/frameworkext.h"
#include "utils/CDisplayMetricsHelper.h"
#include "utils/CDisplayMetrics.h"

ECode CDisplayMetricsHelper::GetDensityDevice(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = CDisplayMetrics::GetDeviceDensity();

    return NOERROR;
}

