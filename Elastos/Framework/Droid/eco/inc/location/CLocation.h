
#ifndef __CLOCATION_H__
#define __CLOCATION_H__

#include "_CLocation.h"

using namespace Elastos;

CarClass(CLocation)
{
public:
    /**
     * Sets the contents of the location to the values from the given location.
     */
    CARAPI Set(
        /* [in] */ ILocation* l);

    /**
     * Clears the contents of the location.
     */
    CARAPI Reset();

    /**
     * Returns the approximate distance in meters between this
     * location and the given location.  Distance is defined using
     * the WGS84 ellipsoid.
     *
     * @param dest the destination location
     * @return the approximate distance in meters
     */
    CARAPI DistanceTo(
        /* [in] */ ILocation* dest,
        /* [out] */ Float* distance);

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
    CARAPI BearingTo(
        /* [in] */ ILocation* dest,
        /* [out] */ Float* bearing);

    CARAPI GetProvider(
        /* [out] */ String* provider);

    /**
     * Sets the name of the provider that generated this fix.
     */
    CARAPI SetProvider(
        /* [in] */ const String& provider);

    /**
     * Returns the UTC time of this fix, in milliseconds since January 1,
     * 1970.
     */
    CARAPI GetTime(
        /* [out] */ Int64* time);

    /**
     * Sets the UTC time of this fix, in milliseconds since January 1,
     * 1970.
     */
    CARAPI SetTime(
        /* [in] */ Int64 time);

    CARAPI GetLatitude(
        /* [out] */ Double* latitude);

    CARAPI SetLatitude(
        /* [in] */ Double latitude);

    CARAPI GetLongitude(
        /* [out] */ Double* longitude);

    CARAPI SetLongitude(
        /* [in] */ Double longitude);

    /**
     * Returns true if this fix contains altitude information, false
     * otherwise.
     */
    CARAPI HasAltitude(
        /* [out] */ Boolean* hasAltitude);

    /**
     * Returns the altitude of this fix.  If {@link #hasAltitude} is false,
     * 0.0f is returned.
     */
    CARAPI GetAltitude(
        /* [out] */ Double* altitude);

    /**
     * Sets the altitude of this fix.  Following this call,
     * hasAltitude() will return true.
     */
    CARAPI SetAltitude(
        /* [in] */ Double altitude);

    /**
     * Clears the altitude of this fix.  Following this call,
     * hasAltitude() will return false.
     */
    CARAPI RemoveAltitude();

    /**
     * Returns true if this fix contains speed information, false
     * otherwise.  The default implementation returns false.
     */
    CARAPI HasSpeed(
        /* [out] */ Boolean* hasSpeed);

    /**
     * Returns the speed of the device over ground in meters/second.
     * If hasSpeed() is false, 0.0f is returned.
     */
    CARAPI GetSpeed(
        /* [out] */ Float* speed);

    /**
     * Sets the speed of this fix, in meters/second.  Following this
     * call, hasSpeed() will return true.
     */
    CARAPI SetSpeed(
        /* [in] */ Float speed);

    /**
     * Clears the speed of this fix.  Following this call, hasSpeed()
     * will return false.
     */
    CARAPI RemoveSpeed();

    /**
     * Returns true if the provider is able to report bearing information,
     * false otherwise.  The default implementation returns false.
     */
    CARAPI HasBearing(
        /* [out] */ Boolean* hasBearing);

    /**
     * Returns the direction of travel in degrees East of true
     * North. If hasBearing() is false, 0.0 is returned.
     */
    CARAPI GetBearing(
        /* [out] */ Float* bearing);

    /**
     * Sets the bearing of this fix.  Following this call, hasBearing()
     * will return true.
     */
    CARAPI SetBearing(
        /* [in] */ Float bearing);

    /**
     * Clears the bearing of this fix.  Following this call, hasBearing()
     * will return false.
     */
    CARAPI RemoveBearing();

    /**
     * Returns true if the provider is able to report accuracy information,
     * false otherwise.  The default implementation returns false.
     */
    CARAPI HasAccuracy(
        /* [out] */ Boolean* hasAccuracy);

    /**
     * Returns the accuracy of the fix in meters. If hasAccuracy() is false,
     * 0.0 is returned.
     */
    CARAPI GetAccuracy(
        /* [out] */ Float* accuracy);

    /**
     * Sets the accuracy of this fix.  Following this call, hasAccuracy()
     * will return true.
     */
    CARAPI SetAccuracy(
        /* [in] */ Float accuracy);

    /**
     * Clears the accuracy of this fix.  Following this call, hasAccuracy()
     * will return false.
     */
    CARAPI RemoveAccuracy();

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
    CARAPI GetExtras(
        /* [out] */ IBundle** extras);

    /**
     * Sets the extra information associated with this fix to the
     * given Bundle.
     */
    CARAPI SetExtras(
        /* [in] */ IBundle* extras);

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

private:
    // TODO: Add your private member variables here.
};

#endif // __CLOCATION_H__

