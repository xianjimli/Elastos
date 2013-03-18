#ifndef __CSETTINGSSYSTEM_H__
#define __CSETTINGSSYSTEM_H__

#include "_CSettingsSystem.h"
#include "Settings.h"


CarClass(CSettingsSystem), public Settings::System
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
     * Convenience function for retrieving a single system settings value
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
     * Convenience function for retrieving a single system settings value
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
     * Convenience function for retrieving a single system settings value
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
     * Convenience function for retrieving a single system settings value
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
     * Convenience function for updating a single settings value as a long
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
     * Convenience function for retrieving a single system settings value
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
     * Convenience function for retrieving a single system settings value
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
     * Convenience function to read all of the current
     * configuration-related settings into a
     * {@link Configuration} object.
     *
     * @param cr The ContentResolver to access.
     * @param outConfig Where to place the configuration settings.
     */
    CARAPI GetConfiguration(
        /* [in] */ IContentResolver* cr,
        /* [in] */ IConfiguration* outConfig);

    /**
     * Convenience function to write a batch of configuration-related
     * settings from a {@link Configuration} object.
     *
     * @param cr The ContentResolver to access.
     * @param config The settings to write.
     * @return true if the values were set, false on database errors
     */
    CARAPI PutConfiguration(
        /* [in] */ IContentResolver* cr,
        /* [in] */ IConfiguration* config,
        /* [out] */ Boolean* result);

    /** @hide */
    CARAPI HasInterestingConfigurationChanges(
        /* [in] */ Int32 changes,
        /* [out] */ Boolean* result);

    CARAPI GetShowGTalkServiceStatus(
        /* [in] */ IContentResolver* cr,
        /* [out] */ Boolean* result);

    CARAPI SetShowGTalkServiceStatus(
        /* [in] */ IContentResolver* cr,
        /* [in] */ Boolean flag);
};

#endif //__CSETTINGSSYSTEM_H__
