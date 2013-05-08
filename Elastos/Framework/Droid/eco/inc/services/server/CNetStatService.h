
#ifndef __CNETSTATSERVICE_H__
#define __CNETSTATSERVICE_H__

#include "_CNetStatService.h"


CarClass(CNetStatService)//,public CTrafficStats
{
public:
    CNetStatService();

    ~CNetStatService();
    
    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI GetMobileTxPackets(
        /* [out] */ Int64* result);

    CARAPI GetMobileRxPackets(
        /* [out] */ Int64* result);

    CARAPI GetMobileTxBytes(
        /* [out] */ Int64* result);

    CARAPI GetMobileRxBytes(
        /* [out] */ Int64* result);

    CARAPI GetTotalTxPackets(
        /* [out] */ Int64* result);

    CARAPI GetTotalRxPackets(
        /* [out] */ Int64* result);

    CARAPI GetTotalTxBytes(
        /* [out] */ Int64* result);

    CARAPI GetTotalRxBytes(
        /* [out] */ Int64* result);

private:
    IContext* mContext;
};

#endif // __CNETSTATSERVICE_H__
