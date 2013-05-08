
#ifndef __CTRAFFICSTATS_H__
#define __CTRAFFICSTATS_H__

#include "_CTrafficStats.h"

CarClass(CTrafficStats)
{
public:
    CTrafficStats();

    ~CTrafficStats();

    CARAPI TrafficStatsGetMobileTxPackets(
        /* [out] */ Int64* result);

    CARAPI TrafficStatsGetMobileRxPackets(
        /* [out] */ Int64* result);

    CARAPI TrafficStatsGetMobileTxBytes(
        /* [out] */ Int64* result);

    CARAPI TrafficStatsGetMobileRxBytes(
        /* [out] */ Int64* result);

    CARAPI TrafficStatsGetTotalTxPackets(
        /* [out] */ Int64* result);

    CARAPI TrafficStatsGetTotalRxPackets(
        /* [out] */ Int64* result);

    CARAPI TrafficStatsGetTotalTxBytes(
        /* [out] */ Int64* result);

    CARAPI TrafficStatsGetTotalRxBytes(
        /* [out] */ Int64* result);

    CARAPI TrafficStatsGetUidTxBytes(
        /* [in] */ Int32 uid,
        /* [out] */ Int32* result);

    CARAPI TrafficStatsGetUidRxBytes(
        /* [in] */ Int32 uid,
        /* [out] */ Int32* result);

private:
    // TODO: Add your private member variables here.
};

#endif // __CTRAFFICSTATS_H__
