
#include "server/CNetStatService.h"

#include <elastos/AutoPtr.h>
static AutoPtr<ITrafficStats> sTrafficStats;

CNetStatService::CNetStatService()
{}

CNetStatService::~CNetStatService()
{}

ECode CNetStatService::constructor(
    /* [in] */ IContext* context)
{
    CTrafficStats::AcquireSingleton((ITrafficStats**)&sTrafficStats);

    mContext = context;
    return NOERROR;
}

ECode CNetStatService::GetMobileTxPackets(
    /* [out] */ Int64* result)
{
//    VALIDATE_NOT_NULL(result);
    sTrafficStats->TrafficStatsGetMobileTxPackets(result);
    return NOERROR;
}

ECode CNetStatService::GetMobileRxPackets(
    /* [out] */ Int64* result)
{
    //*result = TrafficStats.getMobileRxPackets();
    sTrafficStats->TrafficStatsGetMobileRxPackets(result);
    return NOERROR;
}

ECode CNetStatService::GetMobileTxBytes(
    /* [out] */ Int64* result)
{
    //*result = TrafficStats.getMobileTxBytes();
    sTrafficStats->TrafficStatsGetMobileTxBytes(result);
    return NOERROR;
}

ECode CNetStatService::GetMobileRxBytes(
    /* [out] */ Int64* result)
{
    //*result = TrafficStats.getMobileRxBytes();
    sTrafficStats->TrafficStatsGetMobileRxBytes(result);
    return NOERROR;
}

ECode CNetStatService::GetTotalTxPackets(
    /* [out] */ Int64* result)
{
    //*result = TrafficStats.getTotalTxPackets();
    sTrafficStats->TrafficStatsGetTotalTxPackets(result);
    return NOERROR;
}

ECode CNetStatService::GetTotalRxPackets(
    /* [out] */ Int64* result)
{
    //*result = TrafficStats.getTotalRxPackets();
    sTrafficStats->TrafficStatsGetTotalRxPackets(result);
    return NOERROR;
}

ECode CNetStatService::GetTotalTxBytes(
    /* [out] */ Int64* result)
{
    //*result = TrafficStats.getTotalTxBytes();
    sTrafficStats->TrafficStatsGetTotalTxBytes(result);
    return NOERROR;
}

ECode CNetStatService::GetTotalRxBytes(
    /* [out] */ Int64* result)
{
    //*result = TrafficStats.getTotalRxBytes();
    sTrafficStats->TrafficStatsGetTotalRxBytes(result);
    return NOERROR;
}


