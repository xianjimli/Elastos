
#ifndef __CLOCATION_H__
#define __CLOCATION_H__

#include "_CLocation.h"

using namespace Elastos;

CarClass(CLocation)
{
public:
    CARAPI GetDescription(
        /* [out] */ String* str);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor(
        /* [in] */ const String& provider);

    CARAPI constructor(
        /* [in] */ ILocation* l);

    CARAPI GetProvider(
        /* [out] */ String* provider);

    CARAPI DistanceTo(
        /* [in] */ ILocation* dest,
		/* [out] */ Float* distance);

    CARAPI GetLatitude(
    	/* [out] */ Double* latitude);

    CARAPI SetLatitude(
    	/* [in] */ Double latitude);

    CARAPI GetLongitude(
        /* [out] */ Double* longitude);

    CARAPI SetLongitude(
    	/* [in] */ Double longitude);

    CARAPI GetAccuracy(
    	/* [out] */ Float* accuracy);

private:
    // TODO: Add your private member variables here.
};

#endif // __CLOCATION_H__

