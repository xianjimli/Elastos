
#include "ext/frameworkext.h"
#include "net/CConnectivityManagerHelper.h"
#include "net/CConnectivityManager.h"

ECode CConnectivityManagerHelper::IsNetworkTypeValid(
    /* [in] */ Int32 networkType,
    /* [out] */ Boolean* isNetworkTypeValid)
{
    VALIDATE_NOT_NULL(isNetworkTypeValid);
    *isNetworkTypeValid = CConnectivityManager::IsNetworkTypeValid(networkType);

    return NOERROR;
}

