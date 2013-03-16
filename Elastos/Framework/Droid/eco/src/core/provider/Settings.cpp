
#include "provider/CSettings.h"
#include "utils/Config.h"
#include "net/Uri.h"
#include "content/CContentValues.h"
#include <Slogger.h>
#include <elastos/AutoPtr.h>
#include <StringBuffer.h>

using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

const CString Settings::JID_RESOURCE_PREFIX = "android";
const CString Settings::TAG = "CSettings";
const Boolean Settings::LOCAL_LOGV = Config::LOGV || FALSE;


//const String CSettings::NameValueCache::SELECT_VALUE[] = { CSettingsNameValueTable_VALUE };
//const String CSettings::NameValueCache::NAME_EQ_PLACEHOLDER = String("name=?");

Boolean Settings::NameValueTable::PutString(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ IUri* uri,
    /* [in] */ const String& name,
    /* [in] */ const String& value)
{
    // The database will take care of replacing duplicates.
    //try {
    AutoPtr<IContentValues> values;
    CContentValues::New((IContentValues**)&values);
    values->PutString(String(SettingsNameValueTable_NAME), name);
    values->PutString(String(SettingsNameValueTable_VALUE), value);
    AutoPtr<IUri> outUri;
    resolver->Insert(uri, values,(IUri**)&outUri);
    return TRUE;
    //} catch (/*SQLException e*/...) {
    //    Slogger::W(CSettings::TAG, StringBuffer("Can't set key ") + name + StringBuffer(" in ") + uri);
    //    return FALSE;
    //  }

}

ECode Settings::NameValueTable::GetUriFor(
    /* [in] */ IUri* uri,
    /* [in] */ const String& name,
    /* [out] */ IUri** value)
{
    VALIDATE_NOT_NULL(value);
    return Uri::WithAppendedPath(uri, name,value);
}


Settings::NameValueCache::NameValueCache(
    /* [in] */ const String& versionSystemProperty,
    /* [in] */ IUri* uri,
    /* [in] */ const String& callCommand)
    :mVersionSystemProperty(versionSystemProperty)
    ,mUri(uri)
    ,mCallCommand(callCommand)
    ,mValuesVersion(0)
    ,mContentProvider(NULL)
    ,mValues(10)
{}

ECode Settings::NameValueCache::GetString(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [out] */ String* string)
{
/*            long newValuesVersion = SystemProperties.getLong(mVersionSystemProperty, 0);

            synchronized (this) {
                if (mValuesVersion != newValuesVersion) {
                    if (LOCAL_LOGV) {
                        Log.v(TAG, "invalidate [" + mUri.getLastPathSegment() + "]: current " +
                                newValuesVersion + " != cached " + mValuesVersion);
                    }

                    mValues.clear();
                    mValuesVersion = newValuesVersion;
                }

                if (mValues.containsKey(name)) {
                    return mValues.get(name);  // Could be null, that's OK -- negative caching
                }
            }

            IContentProvider cp = null;
            synchronized (this) {
                cp = mContentProvider;
                if (cp == null) {
                    cp = mContentProvider = cr.acquireProvider(mUri.getAuthority());
                }
            }

            // Try the fast path first, not using query().  If this
            // fails (alternate Settings provider that doesn't support
            // this interface?) then we fall back to the query/table
            // interface.
            if (mCallCommand != null) {
                try {
                    Bundle b = cp.call(mCallCommand, name, null);
                    if (b != null) {
                        String value = b.getPairValue();
                        synchronized (this) {
                            mValues.put(name, value);
                        }
                        return value;
                    }
                    // If the response Bundle is null, we fall through
                    // to the query interface below.
                } catch (RemoteException e) {
                    // Not supported by the remote side?  Fall through
                    // to query().
                }
            }

            Cursor c = null;
            try {
                c = cp.query(mUri, SELECT_VALUE, NAME_EQ_PLACEHOLDER,
                             new String[]{name}, null);
                if (c == null) {
                    Log.w(TAG, "Can't get key " + name + " from " + mUri);
                    return null;
                }

                String value = c.moveToNext() ? c.getString(0) : null;
                synchronized (this) {
                    mValues.put(name, value);
                }
                if (LOCAL_LOGV) {
                    Log.v(TAG, "cache miss [" + mUri.getLastPathSegment() + "]: " +
                            name + " = " + (value == null ? "(null)" : value));
                }
                return value;
            } catch (RemoteException e) {
                Log.w(TAG, "Can't get key " + name + " from " + mUri, e);
                return null;  // Return null, but don't cache it.
            } finally {
                if (c != null) c.close();
            }*/
    return E_NOT_IMPLEMENTED;
}

HashSet<String> const Settings::System::MOVED_TO_SECURE(30);

//CSettings::System::MOVED_TO_SECURE.Insert(CSettings::Secure::ADB_ENABLED);
/*CSettings::System::MOVED_TO_SECURE.add(Secure.ANDROID_ID);
CSettings::System::MOVED_TO_SECURE.add(Secure.BLUETOOTH_ON);
CSettings::System::MOVED_TO_SECURE.add(Secure.DATA_ROAMING);
CSettings::System::MOVED_TO_SECURE.add(Secure.HTTP_PROXY);
CSettings::System::MOVED_TO_SECURE.add(Secure.INSTALL_NON_MARKET_APPS);
CSettings::System::MOVED_TO_SECURE.add(Secure.LOCATION_PROVIDERS_ALLOWED);
CSettings::System::MOVED_TO_SECURE.add(Secure.LOCK_PATTERN_ENABLED);
CSettings::System::MOVED_TO_SECURE.add(Secure.LOCK_PATTERN_VISIBLE);
CSettings::System::MOVED_TO_SECURE.add(Secure.LOCK_PATTERN_TACTILE_FEEDBACK_ENABLED);
CSettings::System::MOVED_TO_SECURE.add(Secure.LOGGING_ID);
CSettings::System::MOVED_TO_SECURE.add(Secure.PARENTAL_CONTROL_ENABLED);
CSettings::System::MOVED_TO_SECURE.add(Secure.PARENTAL_CONTROL_LAST_UPDATE);
CSettings::System::MOVED_TO_SECURE.add(Secure.PARENTAL_CONTROL_REDIRECT_URL);
CSettings::System::MOVED_TO_SECURE.add(Secure.SETTINGS_CLASSNAME);
CSettings::System::MOVED_TO_SECURE.add(Secure.USB_MASS_STORAGE_ENABLED);
CSettings::System::MOVED_TO_SECURE.add(Secure.USE_GOOGLE_MAIL);
CSettings::System::MOVED_TO_SECURE.add(Secure.WIFI_NETWORKS_AVAILABLE_NOTIFICATION_ON);
CSettings::System::MOVED_TO_SECURE.add(Secure.WIFI_NETWORKS_AVAILABLE_REPEAT_DELAY);
CSettings::System::MOVED_TO_SECURE.add(Secure.WIFI_NUM_OPEN_NETWORKS_KEPT);
CSettings::System::MOVED_TO_SECURE.add(Secure.WIFI_ON);
CSettings::System::MOVED_TO_SECURE.add(Secure.WIFI_WATCHDOG_ACCEPTABLE_PACKET_LOSS_PERCENTAGE);
CSettings::System::MOVED_TO_SECURE.add(Secure.WIFI_WATCHDOG_AP_COUNT);
CSettings::System::MOVED_TO_SECURE.add(Secure.WIFI_WATCHDOG_BACKGROUND_CHECK_DELAY_MS);
CSettings::System::MOVED_TO_SECURE.add(Secure.WIFI_WATCHDOG_BACKGROUND_CHECK_ENABLED);
CSettings::System::MOVED_TO_SECURE.add(Secure.WIFI_WATCHDOG_BACKGROUND_CHECK_TIMEOUT_MS);
CSettings::System::MOVED_TO_SECURE.add(Secure.WIFI_WATCHDOG_INITIAL_IGNORED_PING_COUNT);
CSettings::System::MOVED_TO_SECURE.add(Secure.WIFI_WATCHDOG_MAX_AP_CHECKS);
CSettings::System::MOVED_TO_SECURE.add(Secure.WIFI_WATCHDOG_ON);
CSettings::System::MOVED_TO_SECURE.add(Secure.WIFI_WATCHDOG_PING_COUNT);
CSettings::System::MOVED_TO_SECURE.add(Secure.WIFI_WATCHDOG_PING_DELAY_MS);
CSettings::System::MOVED_TO_SECURE.add(Secure.WIFI_WATCHDOG_PING_TIMEOUT_MS);
*/

ECode Settings::System::GetString(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ const String& name,
    /* [out] */ String* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode Settings::System::PutString(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ const String& name,
    /* [in] */ const String& value,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}


ECode Settings::System::GetUriFor(
    /* [in] */ const String& name,
    /* [out] */ IUri** uri)
{
    return E_NOT_IMPLEMENTED;
}

ECode Settings::System::GetInt32(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int32 def,
    /* [out] */ Int32* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode Settings::System::GetInt32Ex(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [out] */ Int32* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode Settings::System::PutInt32(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int32 value,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode Settings::System::GetInt64(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int64 def,
    /* [out] */ Int64* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode Settings::System::GetInt64Ex(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [out] */ Int64* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode Settings::System::PutInt64(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int64 value,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode Settings::System::GetFloat(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Float def,
    /* [out] */ Float* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode Settings::System::GetFloatEx(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [out] */ Float* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode Settings::System::PutFloat(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Float value,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode Settings::System::GetConfiguration(
    /* [in] */ IContentResolver* cr,
    /* [in] */ IConfiguration* outConfig)
{
    return E_NOT_IMPLEMENTED;
}

ECode Settings::System::PutConfiguration(
    /* [in] */ IContentResolver* cr,
    /* [in] */ IConfiguration* config,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;

}

/** @hide */
ECode Settings::System::HasInterestingConfigurationChanges(
    /* [in] */ Int32 changes,
    /* [out] */ Boolean* result)
 {
     return E_NOT_IMPLEMENTED;
 }

ECode Settings::System::GetShowGTalkServiceStatus(
    /* [in] */ IContentResolver* cr,
    /* [out] */ Boolean* result)
{
     return E_NOT_IMPLEMENTED;
}

ECode Settings::System::SetShowGTalkServiceStatus(
    /* [in] */ IContentResolver* cr,
    /* [in] */ Boolean flag)
{
     return E_NOT_IMPLEMENTED;
}

ECode Settings::Secure::GetString(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ const String& name,
    /* [out] */ String* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode Settings::Secure::PutString(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ const String& name,
    /* [in] */ const String& value,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode Settings::Secure::GetUriFor(
    /* [in] */ const String& name,
    /* [out] */ IUri** uri)
{
    return E_NOT_IMPLEMENTED;
}

ECode Settings::Secure::GetInt32(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int32 def,
    /* [out] */ Int32* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode Settings::Secure::GetInt32Ex(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [out] */ Int32* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode Settings::Secure::PutInt32(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int32 value,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode Settings::Secure::GetInt64(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int64 def,
    /* [out] */ Int64* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode Settings::Secure::GetInt64Ex(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [out] */ Int64* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode Settings::Secure::PutInt64(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int64 value,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode Settings::Secure::GetFloat(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Float def,
    /* [out] */ Float* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode Settings::Secure::GetFloatEx(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [out] */ Float* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode Settings::Secure::PutFloat(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Float value,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode Settings::Secure::GetBluetoothHeadsetPriorityKey(
    /* [in] */ const String& address,
    /* [out] */ String* key)
{
    return E_NOT_IMPLEMENTED;
}

ECode Settings::Secure::GetBluetoothA2dpSinkPriorityKey(
    /* [in] */ const String& address,
    /* [out] */ String* key)
{
    return E_NOT_IMPLEMENTED;
}

ECode Settings::Secure::IsLocationProviderEnabled(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& provider,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode Settings::Secure::SetLocationProviderEnabled(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& provider,
    /* [in] */ Boolean enabled)
{
    return E_NOT_IMPLEMENTED;
}

ECode Settings::Bookmarks::GetIntentForShortcut(
    /* [in] */ IContentResolver* cr,
    /* [in] */ Char32 shortcut,
    /* [out] */ IIntent** intent)
{
    return E_NOT_IMPLEMENTED;
}

ECode Settings::Bookmarks::Add(
    /* [in] */ IContentResolver* cr,
    /* [in] */ IIntent* intent,
    /* [in] */ const String& title,
    /* [in] */ const String& folder,
    /* [in] */ Char32 shortcut,
    /* [in] */ Int32 ordering,
    /* [out] */ IUri** uri)
{
    return E_NOT_IMPLEMENTED;
}

ECode Settings::Bookmarks::GetLabelForFolder(
    /* [in] */ IResources* r,
    /* [in] */ const String& folder,
    /* [out] */ ICharSequence** fName)
{
    return E_NOT_IMPLEMENTED;
}

ECode Settings::Bookmarks::GetTitle(
    /* [in] */ IContext* context,
    /* [in] */ ICursor* cursor,
    /* [out] */ ICharSequence** title)
{
    return E_NOT_IMPLEMENTED;
}

ECode Settings::GetGTalkDeviceId(
    /* [in] */ Int64 androidId,
    /* [out] */ String* deviceId)
{
//    return "android-" + Long.toHexString(androidId);
    return E_NOT_IMPLEMENTED;
}
