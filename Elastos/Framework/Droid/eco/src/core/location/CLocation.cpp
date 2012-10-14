
#include "location/CLocation.h"



/**
 * Sets the contents of the location to the values from the given location.
 */
ECode CLocation::Set(
    /* [in] */ ILocation* l)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Clears the contents of the location.
 */
ECode CLocation::Reset()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Returns the approximate distance in meters between this
 * location and the given location.  Distance is defined using
 * the WGS84 ellipsoid.
 *
 * @param dest the destination location
 * @return the approximate distance in meters
 */
ECode CLocation::DistanceTo(
    /* [in] */ ILocation* dest,
    /* [out] */ Float* distance)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Returns the approximate initial bearing in degrees East of true
 * North when traveling along the shortest path between this
 * location and the given location.  The shortest path is defined
 * using the WGS84 ellipsoid.  Locations that are (nearly)
 * antipodal may produce meaningless results.
 *
 * @param dest the destination location
 * @return the initial bearing in degrees
 */
ECode CLocation::BearingTo(
    /* [in] */ ILocation* dest,
    /* [out] */ Float* bearing)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLocation::GetProvider(
    /* [out] */ String* provider)
{
    return E_NOT_IMPLEMENTED;
}

/**
 * Sets the name of the provider that generated this fix.
 */
ECode CLocation::SetProvider(
    /* [in] */ const String& provider)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Returns the UTC time of this fix, in milliseconds since January 1,
 * 1970.
 */
ECode CLocation::GetTime(
    /* [out] */ Int64* time)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Sets the UTC time of this fix, in milliseconds since January 1,
 * 1970.
 */
ECode CLocation::SetTime(
    /* [in] */ Int64 time)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLocation::GetLatitude(
    /* [out] */ Double* latitude)
{
    return E_NOT_IMPLEMENTED;
}

ECode CLocation::SetLatitude(
    /* [in] */ Double latitude)
{
    return E_NOT_IMPLEMENTED;
}

ECode CLocation::GetLongitude(
    /* [out] */ Double* longitude)
{
    return E_NOT_IMPLEMENTED;
}

ECode CLocation::SetLongitude(
    /* [in] */ Double longitude)
{
    return E_NOT_IMPLEMENTED;
}

/**
 * Returns true if this fix contains altitude information, false
 * otherwise.
 */
ECode CLocation::HasAltitude(
    /* [out] */ Boolean* hasAltitude)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Returns the altitude of this fix.  If {@link #hasAltitude} is false,
 * 0.0f is returned.
 */
ECode CLocation::GetAltitude(
    /* [out] */ Double* altitude)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Sets the altitude of this fix.  Following this call,
 * hasAltitude() will return true.
 */
ECode CLocation::SetAltitude(
    /* [in] */ Double altitude)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Clears the altitude of this fix.  Following this call,
 * hasAltitude() will return false.
 */
ECode CLocation::RemoveAltitude()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Returns true if this fix contains speed information, false
 * otherwise.  The default implementation returns false.
 */
ECode CLocation::HasSpeed(
    /* [out] */ Boolean* hasSpeed)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Returns the speed of the device over ground in meters/second.
 * If hasSpeed() is false, 0.0f is returned.
 */
ECode CLocation::GetSpeed(
    /* [out] */ Float* speed)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Sets the speed of this fix, in meters/second.  Following this
 * call, hasSpeed() will return true.
 */
ECode CLocation::SetSpeed(
    /* [in] */ Float speed)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Clears the speed of this fix.  Following this call, hasSpeed()
 * will return false.
 */
ECode CLocation::RemoveSpeed()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Returns true if the provider is able to report bearing information,
 * false otherwise.  The default implementation returns false.
 */
ECode CLocation::HasBearing(
    /* [out] */ Boolean* hasBearing)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Returns the direction of travel in degrees East of true
 * North. If hasBearing() is false, 0.0 is returned.
 */
ECode CLocation::GetBearing(
    /* [out] */ Float* bearing)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Sets the bearing of this fix.  Following this call, hasBearing()
 * will return true.
 */
ECode CLocation::SetBearing(
    /* [in] */ Float bearing)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Clears the bearing of this fix.  Following this call, hasBearing()
 * will return false.
 */
ECode CLocation::RemoveBearing()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Returns true if the provider is able to report accuracy information,
 * false otherwise.  The default implementation returns false.
 */
ECode CLocation::HasAccuracy(
    /* [out] */ Boolean* hasAccuracy)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Returns the accuracy of the fix in meters. If hasAccuracy() is false,
 * 0.0 is returned.
 */
ECode CLocation::GetAccuracy(
    /* [out] */ Float* accuracy)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Sets the accuracy of this fix.  Following this call, hasAccuracy()
 * will return true.
 */
ECode CLocation::SetAccuracy(
    /* [in] */ Float accuracy)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Clears the accuracy of this fix.  Following this call, hasAccuracy()
 * will return false.
 */
ECode CLocation::RemoveAccuracy()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Returns additional provider-specific information about the
 * location fix as a Bundle.  The keys and values are determined
 * by the provider.  If no additional information is available,
 * null is returned.
 *
 * <p> A number of common key/value pairs are listed
 * below. Providers that use any of the keys on this list must
 * provide the corresponding value as described below.
 *
 * <ul>
 * <li> satellites - the number of satellites used to derive the fix
 * </ul>
 */
ECode CLocation::GetExtras(
    /* [out] */ IBundle** extras)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Sets the extra information associated with this fix to the
 * given Bundle.
 */
ECode CLocation::SetExtras(
    /* [in] */ IBundle* extras)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLocation::GetDescription(
    /* [out] */ String* str)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLocation::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return E_NOT_IMPLEMENTED;
}

ECode CLocation::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return E_NOT_IMPLEMENTED;
}

ECode CLocation::constructor(
    /* [in] */ const String& providers)
{
    return E_NOT_IMPLEMENTED;
}

ECode CLocation::constructor(
    /* [in] */ ILocation* l)
{
    return E_NOT_IMPLEMENTED;
}

