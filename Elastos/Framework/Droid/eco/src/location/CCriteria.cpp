
#include "location/CCriteria.h"


/**
 * Indicates the desired horizontal accuracy (latitude and longitude).
 * Accuracy may be {@link #ACCURACY_LOW}, {@link #ACCURACY_MEDIUM},
 * {@link #ACCURACY_HIGH} or {@link #NO_REQUIREMENT}.
 * More accurate location may consume more power and may take longer.
 *
 * @throws IllegalArgumentException if accuracy is not one of the supported constants
 */
ECode CCriteria::SetHorizontalAccuracy(
    /* [in] */ Int32 accuracy)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Returns a constant indicating the desired horizontal accuracy (latitude and longitude).
 * Accuracy may be {@link #ACCURACY_LOW}, {@link #ACCURACY_MEDIUM},
 * {@link #ACCURACY_HIGH} or {@link #NO_REQUIREMENT}.
 */
ECode CCriteria::GetHorizontalAccuracy(
    /* [out] */ Int32* accuracy)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Indicates the desired vertical accuracy (altitude).
 * Accuracy may be {@link #ACCURACY_LOW}, {@link #ACCURACY_MEDIUM},
 * {@link #ACCURACY_HIGH} or {@link #NO_REQUIREMENT}.
 * More accurate location may consume more power and may take longer.
 *
 * @throws IllegalArgumentException if accuracy is not one of the supported constants
 */
ECode CCriteria::SetVerticalAccuracy(
    /* [in] */ Int32 accuracy)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Returns a constant indicating the desired vertical accuracy (altitude).
 * Accuracy may be {@link #ACCURACY_LOW}, {@link #ACCURACY_HIGH},
 * or {@link #NO_REQUIREMENT}.
 */
ECode CCriteria::GetVerticalAccuracy(
    /* [out] */ Int32* accuracy)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCriteria::SetSpeedAccuracy(
    /* [in] */ Int32 accuracy)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCriteria::GetSpeedAccuracy(
    /* [out] */ Int32* accuracy)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCriteria::SetBearingAccuracy(
    /* [in] */ Int32 accuracy)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCriteria::GetBearingAccuracy(
    /* [out] */ Int32* accuracy)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCriteria::SetAccuracy(
    /* [in] */ Int32 accuracy)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCriteria::GetAccuracy(
    /* [out] */ Int32* accuracy)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCriteria::SetPowerRequirement(
    /* [in] */ Int32 level)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCriteria::GetPowerRequirement(
    /* [out] */ Int32* requirement)
{
    return E_NOT_IMPLEMENTED;
}

/**
 * Indicates whether the provider is allowed to incur monetary cost.
 */
ECode CCriteria::SetCostAllowed(
    /* [in] */ Boolean costAllowed)
{
    return E_NOT_IMPLEMENTED;
}

/**
 * Returns whether the provider is allowed to incur monetary cost.
 */
ECode CCriteria::IsCostAllowed(
    /* [out] */ Boolean* costAllowed)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCriteria::SetAltitudeRequired(
    /* [in] */ Boolean altitudeRequired)
{
    return E_NOT_IMPLEMENTED;
}

/**
 * Returns whether the provider must provide altitude information.
 * Not all fixes are guaranteed to contain such information.
 */
ECode CCriteria::IsAltitudeRequired(
    /* [out] */ Boolean* altitudeRequired)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCriteria::SetSpeedRequired(
    /* [in] */ Boolean speedRequired)
{
    return E_NOT_IMPLEMENTED;
}

/**
 * Returns whether the provider must provide speed information.
 * Not all fixes are guaranteed to contain such information.
 */
ECode CCriteria::IsSpeedRequired(
    /* [out] */ Boolean* speedRequired)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCriteria::SetBearingRequired(
    /* [in] */ Boolean bearingRequired)
{
    return E_NOT_IMPLEMENTED;
}

/**
 * Returns whether the provider must provide bearing information.
 * Not all fixes are guaranteed to contain such information.
 */
ECode CCriteria::IsBearingRequired(
    /* [out] */ Boolean* bearingRequired)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCriteria::GetDescription(
    /* [out] */ String* str)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCriteria::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCriteria::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCriteria::constructor()
{
    return E_NOT_IMPLEMENTED;
}

ECode CCriteria::constructor(
    /* [in] */ ICriteria* criteria)
{
    return E_NOT_IMPLEMENTED;
}


