
#include "server/CBatteryStatsService.h"

ECode CBatteryStatsService::constructor(
    /* [in] */ const String& fileName)
{
    return E_NOT_IMPLEMENTED;
}

BatteryStatsImpl* CBatteryStatsService::GetActiveStatistics()
{
    return mStats;
}
