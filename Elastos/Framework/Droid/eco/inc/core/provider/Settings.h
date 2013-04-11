#ifndef __SETTINGS_H__
#define __SETTINGS_H_

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/HashMap.h>
#include <elastos/HashSet.h>


class Settings
{
public:
    class NameValueTable
    {
    public:
        CARAPI GetUriFor(
            /* [in] */ IUri* uri,
            /* [in] */ const String& name,
            /* [out] */ IUri** value);

    protected:
        static CARAPI_(Boolean) PutString(
            /* [in] */ IContentResolver* resolver,
            /* [in] */ IUri* uri,
            /* [in] */ const String& name,
            /* [in] */ const String& value);
    };

private:
    // Thread-safe.
    class NameValueCache
    {
    public:
        NameValueCache(
            /* [in] */ const String& versionSystemProperty,
            /* [in] */ IUri* uri,
            /* [in] */ const String& callCommand);

        CARAPI GetString(
            /* [in] */ IContentResolver* cr,
            /* [in] */ const String& name,
            /* [out] */ String* value);

    private:
        const String mVersionSystemProperty;
        const AutoPtr<IUri> mUri;
        static const CString SELECT_VALUE[];
        static const CString NAME_EQ_PLACEHOLDER;
        // Must synchronize on 'this' to access mValues and mValuesVersion.
        const HashMap< String, String > mValues;

        Int64 mValuesVersion;

        AutoPtr<IContentProvider> mContentProvider;

        // The method we'll call (or null, to not use) on the provider
        // for the fast path of retrieving settings.
        const String mCallCommand;

    };

public:
    class System : public NameValueTable
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

        static const AutoPtr<IUri> CONTENT_URI;

    private:
        /*// Populated lazily, guarded by class object:
        static NameValueCache* sNameValueCache = NULL;*/

        static HashSet<String> const MOVED_TO_SECURE;

    };

    class Secure : public NameValueTable
    {
    public:
        /**
         * Look up a name in the database.
         * @param resolver to access the database with
         * @param name to look up in the table
         * @return the corresponding value, or null if not present
         */
        static CARAPI GetString(
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
       static CARAPI PutString(
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
       static CARAPI GetUriFor(
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
       static CARAPI GetInt32(
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
       static CARAPI GetInt32Ex(
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
       static CARAPI PutInt32(
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
       static CARAPI GetInt64(
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
       static CARAPI GetInt64Ex(
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
       static CARAPI PutInt64(
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
       static CARAPI GetFloat(
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
       static CARAPI GetFloatEx(
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
       static CARAPI PutFloat(
           /* [in] */ IContentResolver* cr,
           /* [in] */ const String& name,
           /* [in] */ Float value,
           /* [out] */ Boolean* result);

       /**
        * Get the key that retrieves a bluetooth headset's priority.
        * @hide
        */
       static CARAPI GetBluetoothHeadsetPriorityKey(
           /* [in] */ const String& address,
           /* [out] */ String* key);

       /**
        * Get the key that retrieves a bluetooth a2dp sink's priority.
        * @hide
        */
       static CARAPI GetBluetoothA2dpSinkPriorityKey(
           /* [in] */ const String& address,
           /* [out] */ String* key);

       /**
        * Helper method for determining if a location provider is enabled.
        * @param cr the content resolver to use
        * @param provider the location provider to query
        * @return true if the provider is enabled
        */
       static CARAPI IsLocationProviderEnabled(
           /* [in] */ IContentResolver* cr,
           /* [in] */ const String& provider,
           /* [out] */ Boolean* result);

       /**
        * Thread-safe method for enabling or disabling a single location provider.
        * @param cr the content resolver to use
        * @param provider the location provider to enable or disable
        * @param enabled true if the provider should be enabled
        */
       static CARAPI SetLocationProviderEnabled(
           /* [in] */ IContentResolver* cr,
           /* [in] */ const String& provider,
           /* [in] */ Boolean enabled);

    private:
       /*// Populated lazily, guarded by class object:
       static NameValueCache* sNameValueCache = NULL;*/
    };

    class Bookmarks
    {
    public:
       /**
        * Convenience function to retrieve the bookmarked Intent for a
        * particular shortcut key.
        *
        * @param cr The ContentResolver to query.
        * @param shortcut The shortcut key.
        *
        * @return Intent The bookmarked URL, or null if there is no bookmark
        *         matching the given shortcut.
        */
       CARAPI GetIntentForShortcut(
           /* [in] */ IContentResolver* cr,
           /* [in] */ Char32 shortcut,
           /* [out] */ IIntent** intent);

       /**
        * Add a new bookmark to the system.
        *
        * @param cr The ContentResolver to query.
        * @param intent The desired target of the bookmark.
        * @param title Bookmark title that is shown to the user; null if none
        *            or it should be resolved to the intent's title.
        * @param folder Folder in which to place the bookmark; null if none.
        * @param shortcut Shortcut that will invoke the bookmark; 0 if none. If
        *            this is non-zero and there is an existing bookmark entry
        *            with this same shortcut, then that existing shortcut is
        *            cleared (the bookmark is not removed).
        * @return The unique content URL for the new bookmark entry.
        */
       CARAPI Add(
           /* [in] */ IContentResolver* cr,
           /* [in] */ IIntent* intent,
           /* [in] */ const String& title,
           /* [in] */ const String& folder,
           /* [in] */ Char32 shortcut,
           /* [in] */ Int32 ordering,
           /* [out] */ IUri** uri);

       /**
        * Return the folder name as it should be displayed to the user.  This
        * takes care of localizing special folders.
        *
        * @param r Resources object for current locale; only need access to
        *          system resources.
        * @param folder The value found in the {@link #FOLDER} column.
        *
        * @return CharSequence The label for this folder that should be shown
        *         to the user.
        */
       CARAPI GetLabelForFolder(
           /* [in] */ IResources* r,
           /* [in] */ const String& folder,
           /* [out] */ ICharSequence** fName);

       /**
        * Return the title as it should be displayed to the user. This takes
        * care of localizing bookmarks that point to activities.
        *
        * @param context A context.
        * @param cursor A cursor pointing to the row whose title should be
        *        returned. The cursor must contain at least the {@link #TITLE}
        *        and {@link #INTENT} columns.
        * @return A title that is localized and can be displayed to the user,
        *         or the empty string if one could not be found.
        */
       CARAPI GetTitle(
           /* [in] */ IContext* context,
           /* [in] */ ICursor* cursor,
           /* [out] */ ICharSequence** title);

    private:
       static const CString TAG;

       static const String sIntentProjection[];
       static const String sShortcutProjection[];
       static const String sShortcutSelection;
    };

public:
    /**
     * Returns the device ID that we should use when connecting to the mobile gtalk server.
     * This is a string like "android-0x1242", where the hex string is the Android ID obtained
     * from the GoogleLoginService.
     *
     * @param androidId The Android ID for this device.
     * @return The device ID that should be used when connecting to the mobile gtalk server.
     * @hide
     */
    CARAPI GetGTalkDeviceId(
        /* [in] */ Int64 androidId,
        /* [out] */ String* deviceId);

private:
    static const CString JID_RESOURCE_PREFIX;
    static const CString TAG;
    static const Boolean LOCAL_LOGV;
};

#endif //__SETTINGS_H__
