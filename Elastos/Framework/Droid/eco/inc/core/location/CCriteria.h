
#ifndef __CCRITERIA_H__
#define __CCRITERIA_H__

#include "ext/frameworkdef.h"
#include "_CCriteria.h"

using namespace Elastos;

CarClass(CCriteria)
{
public:
    CCriteria();

    /**
     * Indicates the desired horizontal accuracy (latitude and longitude).
     * Accuracy may be {@link #ACCURACY_LOW}, {@link #ACCURACY_MEDIUM},
     * {@link #ACCURACY_HIGH} or {@link #NO_REQUIREMENT}.
     * More accurate location may consume more power and may take longer.
     *
     * @throws IllegalArgumentException if accuracy is not one of the supported constants
     */
    CARAPI SetHorizontalAccuracy(
        /* [in] */ Int32 accuracy);

    /**
     * Returns a constant indicating the desired horizontal accuracy (latitude and longitude).
     * Accuracy may be {@link #ACCURACY_LOW}, {@link #ACCURACY_MEDIUM},
     * {@link #ACCURACY_HIGH} or {@link #NO_REQUIREMENT}.
     */
    CARAPI GetHorizontalAccuracy(
        /* [out] */ Int32* accuracy);

    /**
     * Indicates the desired vertical accuracy (altitude).
     * Accuracy may be {@link #ACCURACY_LOW}, {@link #ACCURACY_MEDIUM},
     * {@link #ACCURACY_HIGH} or {@link #NO_REQUIREMENT}.
     * More accurate location may consume more power and may take longer.
     *
     * @throws IllegalArgumentException if accuracy is not one of the supported constants
     */
    CARAPI SetVerticalAccuracy(
        /* [in] */ Int32 accuracy);

    /**
     * Returns a constant indicating the desired vertical accuracy (altitude).
     * Accuracy may be {@link #ACCURACY_LOW}, {@link #ACCURACY_HIGH},
     * or {@link #NO_REQUIREMENT}.
     */
    CARAPI GetVerticalAccuracy(
        /* [out] */ Int32* accuracy);

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

    /**
     * Indicates whether the provider is allowed to incur monetary cost.
     */
    CARAPI SetCostAllowed(
        /* [in] */ Boolean costAllowed);

    /**
     * Returns whether the provider is allowed to incur monetary cost.
     */
    CARAPI IsCostAllowed(
        /* [out] */ Boolean* costAllowed);

    CARAPI SetAltitudeRequired(
        /* [in] */ Boolean altitudeRequired);

    /**
     * Returns whether the provider must provide altitude information.
     * Not all fixes are guaranteed to contain such information.
     */
    CARAPI IsAltitudeRequired(
        /* [out] */ Boolean* altitudeRequired);

    CARAPI SetSpeedRequired(
        /* [in] */ Boolean speedRequired);

    /**
     * Returns whether the provider must provide speed information.
     * Not all fixes are guaranteed to contain such information.
     */
    CARAPI IsSpeedRequired(
        /* [out] */ Boolean* speedRequired);

    CARAPI SetBearingRequired(
        /* [in] */ Boolean bearingRequired);

    /**
     * Returns whether the provider must provide bearing information.
     * Not all fixes are guaranteed to contain such information.
     */
    CARAPI IsBearingRequired(
        /* [out] */ Boolean* bearingRequired);

    CARAPI GetDescription(
        /* [out] */ String* str);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ ICriteria* criteria);

private:
    Int32 mHorizontalAccuracy;
    Int32 mVerticalAccuracy;
    Int32 mSpeedAccuracy;
    Int32 mBearingAccuracy;
    Int32 mPowerRequirement;
    Boolean mAltitudeRequired;
    Boolean mBearingRequired;
    Boolean mSpeedRequired;
    Boolean mCostAllowed;
};

#endif // __CCRITERIA_H__

