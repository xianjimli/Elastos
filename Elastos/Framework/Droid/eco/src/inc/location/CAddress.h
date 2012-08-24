
#ifndef __CADDRESS_H__
#define __CADDRESS_H__

#include "_CAddress.h"

using namespace Elastos;

CarClass(CAddress)
{
public:
    CARAPI GetDescription(
        /* [out] */ String* str);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor(
        /* [in] */ ILocation* location);

private:
    // TODO: Add your private member variables here.
};

#endif // __CADDRESS_H__

