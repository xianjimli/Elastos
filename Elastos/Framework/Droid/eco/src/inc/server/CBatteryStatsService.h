
#ifndef __CBATTERYSTATSSERVICE_H__
#define __CBATTERYSTATSSERVICE_H__

#include "_CBatteryStatsService.h"
#include <elastos/AutoPtr.h>
#include "os/BatteryStatsImpl.h"

using namespace Elastos;

CarClass(CBatteryStatsService)
{
public:
    CARAPI constructor(
        /* [in] */ const String& fileName);

public:
    /**
     * @return the current statistics object, which may be modified
     * to reflect events that affect battery usage.  You must lock the
     * stats object before doing anything with it.
     */
    CARAPI_(BatteryStatsImpl*) GetActiveStatistics();

private:
    AutoPtr<BatteryStatsImpl> mStats;
};

#endif //__CBATTERYSTATSSERVICE_H__
