
#ifndef __CSYNCSTATS_H__
#define __CSYNCSTATS_H__

#include "_CSyncStats.h"
#include <ext/frameworkdef.h>

CarClass(CSyncStats)
{
public:
    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI Clear();

    CARAPI DescribeContents(
        /* [out] */ Int32* result);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IParcel* parcel);

private:
    // TODO: Add your private member variables here.
};

#endif // __CSYNCSTATS_H__
