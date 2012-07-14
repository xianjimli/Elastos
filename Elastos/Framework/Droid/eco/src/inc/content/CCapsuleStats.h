
#ifndef __CCAPSULESTATS_H__
#define __CCAPSULESTATS_H__

#include "_CCapsuleStats.h"

CarClass(CCapsuleStats)
{
public:
    CARAPI GetDescription(
        /* [out] */ String* str);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor(
        /* [in] */ const String& capName);

    CARAPI constructor(
        /* [in] */ IParcel* source);

    CARAPI constructor(
        /* [in] */ ICapsuleStats* stats);

private:
    // TODO: Add your private member variables here.
};

#endif // __CCAPSULESTATS_H__

