#ifndef __CSETTINGSSECURE_H__
#define __CSETTINGSSECURE_H__

#include "_CSettingsSecure.h"
#include "Settings.h"


CarClass(CSettingsSecure), public Settings::Secure
{
public:
    /**
     * Look up a name in the database.
     * @param resolver to access the database with
     * @param name to look up in the table
     * @return the corresponding value, or null if not present
     */
    CARAPI GetString(
        /* [in] */ IContentResolver* resolver,
        /* [in] */ const String& name,
        /* [out] */ String* value);

    /**
     * Store a name/value pair into the database.
     * @param resolver to access the database with
     * @param name to store
     * @param value to associate with the name
     * @return true if the value was set, false on database errors
     */
    CARAPI PutString(
        /* [in] */ IContentResolver* resolver,
        /* [in] */ const String& name,
        /* [in] */ const String& value,
        /* [out] */ Boolean* result);

    /**
     * Construct the content URI for a particular name/value pair,
     * useful for monitoring changes with a ContentObserver.
     * @param name to look up in the table
     * @return the corresponding content URI, or null if not present
     */
    CARAPI GetUriFor(
        /* [in] */ const String& name,
        /* [out] */ IUri** uri);

    /**
     * Convenience function for retrieving a single secure settings value
     * as an integer.  Note that internally setting values are always
     * stored as strings; this function converts the string to an integer
     * for you.  The default value will be returned if the setting is
     * not defined or not an integer.
     *
     * @param cr The ContentResolver to access.
     * @param name The name of the setting to retrieve.
     * @param def Value to return if the setting is not defined.
     *
     * @return The setting's current value, or 'def' if it is not defined
     * or not a valid integer.
     */
    CARAPI GetInt32(
        /* [in] */ IContentResolver* cr,
        /* [in] */ const String& name,
        /* [in] */ Int32 def,
        /* [out] */ Int32* value);

    /**
     * Convenience function for retrieving a single secure settings value
     * as an integer.  Note that internally setting values are always
     * stored as strings; this function converts the string to an integer
     * for you.
     * <p>
     * This version does not take a default value.  If the setting has not
     * been set, or the string value is not a number,
     * it throws {@link SettingNotFoundException}.
     *
     * @param cr The ContentResolver to access.
     * @param name The name of the setting to retrieve.
     *
     * @throws SettingNotFoundException Thrown if a setting by the given
     * name can't be found or the setting value is not an integer.
     *
     * @return The setting's current value.
     */
    CARAPI GetInt32Ex(
        /* [in] */ IContentResolver* cr,
        /* [in] */ const String& name,
        /* [out] */ Int32* value);

    /**
     * Convenience function for updating a single settings value as an
     * integer. This will either create a new entry in the table if the
     * given name does not exist, or modify the value of the existing row
     * with that name.  Note that internally setting values are always
     * stored as strings, so this function converts the given value to a
     * string before storing it.
     *
     * @param cr The ContentResolver to access.
     * @param name The name of the setting to modify.
     * @param value The new value for the setting.
     * @return true if the value was set, false on database errors
     */
    CARAPI PutInt32(
        /* [in] */ IContentResolver* cr,
        /* [in] */ const String& name,
        /* [in] */ Int32 value,
        /* [out] */ Boolean* result);

    /**
     * Convenience function for retrieving a single secure settings value
     * as a {@code long}.  Note that internally setting values are always
     * stored as strings; this function converts the string to a {@code long}
     * for you.  The default value will be returned if the setting is
     * not defined or not a {@code long}.
     *
     * @param cr The ContentResolver to access.
     * @param name The name of the setting to retrieve.
     * @param def Value to return if the setting is not defined.
     *
     * @return The setting's current value, or 'def' if it is not defined
     * or not a valid {@code long}.
     */
    CARAPI GetInt64(
        /* [in] */ IContentResolver* cr,
        /* [in] */ const String& name,
        /* [in] */ Int64 def,
        /* [out] */ Int64* value);

    /**
     * Convenience function for retrieving a single secure settings value
     * as a {@code long}.  Note that internally setting values are always
     * stored as strings; this function converts the string to a {@code long}
     * for you.
     * <p>
     * This version does not take a default value.  If the setting has not
     * been set, or the string value is not a number,
     * it throws {@link SettingNotFoundException}.
     *
     * @param cr The ContentResolver to access.
     * @param name The name of the setting to retrieve.
     *
     * @return The setting's current value.
     * @throws SettingNotFoundException Thrown if a setting by the given
     * name can't be found or the setting value is not an integer.
     */
    CARAPI GetInt64Ex(
        /* [in] */ IContentResolver* cr,
        /* [in] */ const String& name,
        /* [out] */ Int64* value);

    /**
     * Convenience function for updating a secure settings value as a long
     * integer. This will either create a new entry in the table if the
     * given name does not exist, or modify the value of the existing row
     * with that name.  Note that internally setting values are always
     * stored as strings, so this function converts the given value to a
     * string before storing it.
     *
     * @param cr The ContentResolver to access.
     * @param name The name of the setting to modify.
     * @param value The new value for the setting.
     * @return true if the value was set, false on database errors
     */
    CARAPI PutInt64(
        /* [in] */ IContentResolver* cr,
        /* [in] */ const String& name,
        /* [in] */ Int64 value,
        /* [out] */ Boolean* result);

    /**
     * Convenience function for retrieving a single secure settings value
     * as a floating point number.  Note that internally setting values are
     * always stored as strings; this function converts the string to an
     * float for you. The default value will be returned if the setting
     * is not defined or not a valid float.
     *
     * @param cr The ContentResolver to access.
     * @param name The name of the setting to retrieve.
     * @param def Value to return if the setting is not defined.
     *
     * @return The setting's current value, or 'def' if it is not defined
     * or not a valid float.
     */
    CARAPI GetFloat(
        /* [in] */ IContentResolver* cr,
        /* [in] */ const String& name,
        /* [in] */ Float def,
        /* [out] */ Float* value);

    /**
     * Convenience function for retrieving a single secure settings value
     * as a float.  Note that internally setting values are always
     * stored as strings; this function converts the string to a float
     * for you.
     * <p>
     * This version does not take a default value.  If the setting has not
     * been set, or the string value is not a number,
     * it throws {@link SettingNotFoundException}.
     *
     * @param cr The ContentResolver to access.
     * @param name The name of the setting to retrieve.
     *
     * @throws SettingNotFoundException Thrown if a setting by the given
     * name can't be found or the setting value is not a float.
     *
     * @return The setting's current value.
     */
    CARAPI GetFloatEx(
        /* [in] */ IContentResolver* cr,
        /* [in] */ const String& name,
        /* [out] */ Float* value);

    /**
     * Convenience function for updating a single settings value as a
     * floating point number. This will either create a new entry in the
     * table if the given name does not exist, or modify the value of the
     * existing row with that name.  Note that internally setting values
     * are always stored as strings, so this function converts the given
     * value to a string before storing it.
     *
     * @param cr The ContentResolver to access.
     * @param name The name of the setting to modify.
     * @param value The new value for the setting.
     * @return true if the value was set, false on database errors
     */
    CARAPI PutFloat(
        /* [in] */ IContentResolver* cr,
        /* [in] */ const String& name,
        /* [in] */ Float value,
        /* [out] */ Boolean* result);


    /**
     * Get the key that retrieves a bluetooth headset's priority.
     * @hide
     */
    CARAPI GetBluetoothHeadsetPriorityKey(
        /* [in] */ const String& address,
        /* [out] */ String* key);

    /**
     * Get the key that retrieves a bluetooth a2dp sink's priority.
     * @hide
     */
    CARAPI GetBluetoothA2dpSinkPriorityKey(
        /* [in] */ const String& address,
        /* [out] */ String* key);

    /**
     * Helper method for determining if a location provider is enabled.
     * @param cr the content resolver to use
     * @param provider the location provider to query
     * @return true if the provider is enabled
     */
    CARAPI IsLocationProviderEnabled(
        /* [in] */ IContentResolver* cr,
        /* [in] */ const String& provider,
        /* [out] */ Boolean* result);

    /**
     * Thread-safe method for enabling or disabling a single location provider.
     * @param cr the content resolver to use
     * @param provider the location provider to enable or disable
     * @param enabled true if the provider should be enabled
     */
    CARAPI SetLocationProviderEnabled(
        /* [in] */ IContentResolver* cr,
        /* [in] */ const String& provider,
        /* [in] */ Boolean enabled);
};

#endif //__CSETTINGSSECURE_H__
