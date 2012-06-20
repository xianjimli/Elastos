
#include "server/CBatteryStatsService.h"

ECode CBatteryStatsService::constructor(
    /* [in] */ String fileName)
{
    return E_NOT_IMPLEMENTED;
}

BatteryStatsImpl* CBatteryStatsService::GetActiveStatistics()
{
    return mStats;
}
