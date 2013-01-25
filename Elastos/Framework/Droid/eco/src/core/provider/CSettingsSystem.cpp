#include "provider/CSettingsSystem.h"

using namespace Elastos::Core;

HashSet<String>* const CSettingsSystem::MOVED_TO_SECURE = new HashSet<String>(30);

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

ECode CSettingsSystem::GetString(
    /*[in]*/ IContentResolver* resolver,
    /*[in]*/ const String& name,
    /*[out]*/ String* outString)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSettingsSystem::PutString(
    /*[in]*/ IContentResolver* resolver,
    /*[in]*/ const String& name,
    /*[in]*/ const String& value,
    /*[out]*/ Boolean* outBoolean)
{
    return E_NOT_IMPLEMENTED;
}


ECode CSettingsSystem::GetUriFor(
    /*[in]*/ const String& name,
    /*[out]*/ IUri** outUri)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSettingsSystem::GetInt(
    /*[in]*/ IContentResolver* cr,
    /*[in]*/ const String& name,
    /*[in]*/ Int32 def,
    /*[out]*/ Int32* outInt)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSettingsSystem::GetIntExtra(
    /*[in]*/ IContentResolver* cr,
    /*[in]*/ const String& name,
    /*[out]*/ Int32* outInt)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSettingsSystem::PutInt(
    /*[in]*/ IContentResolver* cr,
    /*[in]*/ const String& name,
    /*[in]*/ Int32 value,
    /*[out]*/ Boolean* outBoolean)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSettingsSystem::GetLong(
    /*[in]*/ IContentResolver* cr,
    /*[in]*/ const String& name,
    /*[in]*/ Int64 def,
    /*[out]*/ Int64* outLong)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSettingsSystem::GetLongExtra(
    /*[in]*/ IContentResolver* cr,
    /*[in]*/ const String& name,
    /*[out]*/ Int64* outLong)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSettingsSystem::PutLong(
    /*[in]*/ IContentResolver* cr,
    /*[in]*/ const String& name,
    /*[in]*/ Int64 value,
    /*[out]*/ Boolean* outBoolean)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSettingsSystem::GetFloat(
    /*[in]*/ IContentResolver* cr,
    /*[in]*/ const String& name,
    /*[in]*/ Float def,
    /*[out]*/ Float* outFloat)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSettingsSystem::GetFloatExtra(
    /*[in]*/ IContentResolver* cr,
    /*[in]*/ const String& name,
    /*[out]*/ Float* outFloat)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSettingsSystem::PutFloat(
    /*[in]*/ IContentResolver* cr,
    /*[in]*/ const String& name,
    /*[in]*/ Float value,
    /*[out]*/ Boolean* outBoolean)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSettingsSystem::GetConfiguration(
    /*[in]*/ IContentResolver* cr,
    /*[in]*/ IConfiguration* outConfig)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSettingsSystem::PutConfiguration(
    /*[in]*/ IContentResolver* cr,
    /*[in]*/ IConfiguration* config,
    /*[out]*/ Boolean* outBoolean)
{
    return E_NOT_IMPLEMENTED;

}

    /** @hide */
ECode CSettingsSystem::HasInterestingConfigurationChanges(
    /*[in]*/ Int32 changes,
    /*[out]*/ Boolean* outBoolean)
 {
     return E_NOT_IMPLEMENTED;
 }

ECode CSettingsSystem::GetShowGTalkServiceStatus(
    /*[in]*/ IContentResolver* cr,
    /*[out]*/ Boolean* outBoolean)
{
     return E_NOT_IMPLEMENTED;
}

ECode CSettingsSystem::SetShowGTalkServiceStatus(
    /*[in]*/ IContentResolver* cr,
    /*[in]*/ Boolean flag)
{
     return E_NOT_IMPLEMENTED;
}