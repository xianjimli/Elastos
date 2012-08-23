
#ifndef __CCRITERIA_H__
#define __CCRITERIA_H__

#include "_CCriteria.h"

using namespace Elastos;

CarClass(CCriteria)
{
public:
    CARAPI GetDescription(
        /* [out] */ String* str);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ ICriteria* criteria);

    CARAPI SetSpeedAccuracy(
        /* [in] */ Int32 accuracy);

    CARAPI GetSpeedAccuracy(
        /* [out] */ Int32* accuracy);

    CARAPI SetBearingAccuracy(
        /* [in] */ Int32 accuracy);

    CARAPI GetBearingAccuracy(
        /* [out] */ Int32* accuracy);

    CARAPI SetAccuracy(
        /* [in] */ Int32 accuracy);

    CARAPI GetAccuracy(
        /* [out] */ Int32* accuracy);

    CARAPI SetPowerRequirement(
        /* [in] */ Int32 level);

    CARAPI GetPowerRequirement(
        /* [out] */ Int32* requirement);

    CARAPI SetAltitudeRequired(
        /* [in] */ Boolean altitudeRequired);

    CARAPI SetSpeedRequired(
        /* [in] */ Boolean speedRequired);

    CARAPI SetBearingRequired(
        /* [in] */ Boolean bearingRequired);

private:
    // TODO: Add your private member variables here.
};

#endif // __CCRITERIA_H__

