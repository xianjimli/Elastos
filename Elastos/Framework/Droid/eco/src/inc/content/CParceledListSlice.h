
#ifndef __CPARCELEDLISTSLICE_H__
#define __CPARCELEDLISTSLICE_H__

#include "_CParceledListSlice.h"

CarClass(CParceledListSlice)
{
public:
    CARAPI Append(
        /* [in] */ IParcelable* item,
        /* [out] */ Boolean* isFull);

    CARAPI SetLastSlice(
        /* [in] */ Boolean lastSlice);

    CARAPI IsLastSlice(
        /* [out] */ Boolean* isLast);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IParcel* p,
        /* [in] */ Int32 numItems,
        /* [in] */ Boolean lastSlice);

private:
    // TODO: Add your private member variables here.
};

#endif // __CPARCELEDLISTSLICE_H__

