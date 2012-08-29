
#include "location/CAddress.h"


/**
 * Returns the Locale associated with this address.
 */
ECode CAddress::GetLocale(
    /* [out] */ ILocale** locale)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Returns the largest index currently in use to specify an address line.
 * If no address lines are specified, -1 is returned.
 */
ECode CAddress::GetMaxAddressLineIndex(
    /* [out] */ Int32* index)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Returns a line of the address numbered by the given index
 * (starting at 0), or null if no such line is present.
 *
 * @throws IllegalArgumentException if index < 0
 */
ECode CAddress::GetAddressLine(
    /* [in] */ Int32 index,
    /* [out] */ String* line)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Sets the line of the address numbered by index (starting at 0) to the
 * given String, which may be null.
 *
 * @throws IllegalArgumentException if index < 0
 */
ECode CAddress::SetAddressLine(
    /* [in] */ Int32 index,
    /* [in] */ const String& line)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Returns the feature name of the address, for example, "Golden Gate Bridge", or null
 * if it is unknown
 */
ECode CAddress::GetFeatureName(
    /* [out] */ String* featureName)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Sets the feature name of the address to the given String, which may be null
 */
ECode CAddress::SetFeatureName(
    /* [in] */ const String& featureName)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Returns the administrative area name of the address, for example, "CA", or null if
 * it is unknown
 */
ECode CAddress::GetAdminArea(
    /* [out] */ String* adminArea)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Sets the administrative area name of the address to the given String, which may be null
 */
ECode CAddress::SetAdminArea(
    /* [in] */ const String& adminArea)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Returns the sub-administrative area name of the address, for example, "Santa Clara County",
 * or null if it is unknown
 */
ECode CAddress::GetSubAdminArea(
    /* [out] */ String* subAdminArea)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Sets the sub-administrative area name of the address to the given String, which may be null
 */
ECode CAddress::SetSubAdminArea(
    /* [in] */ const String& subAdminArea)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Returns the locality of the address, for example "Mountain View", or null if it is unknown.
 */
ECode CAddress::GetLocality(
    /* [out] */ String* locality)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Sets the locality of the address to the given String, which may be null.
 */
ECode CAddress::SetLocality(
    /* [in] */ const String& locality)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Returns the sub-locality of the address, or null if it is unknown.
 * For example, this may correspond to the neighborhood of the locality.
 */
ECode CAddress::GetSubLocality(
    /* [out] */ String* sublocality)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Sets the sub-locality of the address to the given String, which may be null.
 */
ECode CAddress::SetSubLocality(
    /* [in] */ const String& sublocality)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Returns the thoroughfare name of the address, for example, "1600 Ampitheater Parkway",
 * which may be null
 */
ECode CAddress::GetThoroughfare(
    /* [out] */ String* thoroughfare)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Sets the thoroughfare name of the address, which may be null.
 */
ECode CAddress::SetThoroughfare(
    /* [in] */ const String& thoroughfare)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Returns the sub-thoroughfare name of the address, which may be null.
 * This may correspond to the street number of the address.
 */
ECode CAddress::GetSubThoroughfare(
    /* [out] */ String* subthoroughfare)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Sets the sub-thoroughfare name of the address, which may be null.
 */
ECode CAddress::SetSubThoroughfare(
    /* [in] */ const String& subthoroughfare)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Returns the premises of the address, or null if it is unknown.
 */
ECode CAddress::GetPremises(
    /* [out] */ String* premises)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Sets the premises of the address to the given String, which may be null.
 */
ECode CAddress::SetPremises(
    /* [in] */ const String& premises)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Returns the postal code of the address, for example "94110",
 * or null if it is unknown.
 */
ECode CAddress::GetPostalCode(
    /* [out] */ String* postalCode)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Sets the postal code of the address to the given String, which may
 * be null.
 */
ECode CAddress::SetPostalCode(
    /* [in] */ const String& postalCode)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Returns the country code of the address, for example "US",
 * or null if it is unknown.
 */
ECode CAddress::GetCountryCode(
    /* [out] */ String* countryCode)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Sets the country code of the address to the given String, which may
 * be null.
 */
ECode CAddress::SetCountryCode(
    /* [in] */ const String& countryCode)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Returns the localized country name of the address, for example "Iceland",
 * or null if it is unknown.
 */
ECode CAddress::GetCountryName(
    /* [out] */ String* countryName)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Sets the country name of the address to the given String, which may
 * be null.
 */
ECode CAddress::SetCountryName(
    /* [in] */ const String& countryName)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Returns true if a latitude has been assigned to this Address,
 * false otherwise.
 */
ECode CAddress::HasLatitude(
    /* [out] */ Boolean* hasLatitude)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Returns the latitude of the address if known.
 *
 * @throws IllegalStateException if this Address has not been assigned
 * a latitude.
 */
ECode CAddress::GetLatitude(
    /* [out] */ Double* latitude)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Sets the latitude associated with this address.
 */
ECode CAddress::SetLatitude(
    /* [in] */ Double latitude)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Removes any latitude associated with this address.
 */
ECode CAddress::ClearLatitude()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Returns true if a longitude has been assigned to this Address,
 * false otherwise.
 */
ECode CAddress::HasLongitude(
    /* [out] */ Boolean* hasLongitude)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Returns the longitude of the address if known.
 *
 * @throws IllegalStateException if this Address has not been assigned
 * a longitude.
 */
ECode CAddress::GetLongitude(
    /* [out] */ Double* longitude)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Sets the longitude associated with this address.
 */
ECode CAddress::SetLongitude(
    /* [in] */ Double longitude)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Removes any longitude associated with this address.
 */
ECode CAddress::ClearLongitude()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Returns the phone number of the address if known,
 * or null if it is unknown.
 *
 * @throws IllegalStateException if this Address has not been assigned
 * a latitude.
 */
ECode CAddress::GetPhone(
    /* [out] */ String* phone)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Sets the phone number associated with this address.
 */
ECode CAddress::SetPhone(
    /* [in] */ const String& phone)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Returns the public URL for the address if known,
 * or null if it is unknown.
 */
ECode CAddress::GetUrl(
    /* [out] */ String* url)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Sets the public URL associated with this address.
 */
ECode CAddress::SetUrl(
    /* [in] */ const String& Url)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Returns additional provider-specific information about the
 * address as a Bundle.  The keys and values are determined
 * by the provider.  If no additional information is available,
 * null is returned.
 *
 * <!--
 * <p> A number of common key/value pairs are listed
 * below. Providers that use any of the keys on this list must
 * provide the corresponding value as described below.
 *
 * <ul>
 * </ul>
 * -->
 */
ECode CAddress::GetExtras(
    /* [out] */ IBundle** extras)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Sets the extra information associated with this fix to the
 * given Bundle.
 */
ECode CAddress::SetExtras(
    /* [in] */ IBundle* extras)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAddress::GetDescription(
    /* [out] */ String* str)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAddress::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAddress::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAddress::constructor(
    /* [in] */ ILocale* locale)
{
    return E_NOT_IMPLEMENTED;
}
