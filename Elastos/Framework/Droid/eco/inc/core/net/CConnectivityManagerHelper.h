
#ifndef __CCONNECTIVITYMANAGERHELPER_H__
#define __CCONNECTIVITYMANAGERHELPER_H__

#include "_CConnectivityManagerHelper.h"

CarClass(CConnectivityManagerHelper)
{
public:
    CARAPI IsNetworkTypeValid(
        /* [in] */ Int32 networkType,
        /* [out] */ Boolean* isNetworkTypeValid);

private:
    // TODO: Add your private member variables here.
};

#endif // __CCONNECTIVITYMANAGERHELPER_H__
