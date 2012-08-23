
#ifndef __CLOCATIONMANAGER_H__
#define __CLOCATIONMANAGER_H__

#include "_CLocationManager.h"

using namespace Elastos;

CarClass(CLocationManager)
{
public:
    CARAPI GetDescription(
        /* [out] */ String* str);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor(
        /* [in] */ ILocationManagerEx* service);

private:
    // TODO: Add your private member variables here.
};

#endif // __CLOCATIONMANAGER_H__

