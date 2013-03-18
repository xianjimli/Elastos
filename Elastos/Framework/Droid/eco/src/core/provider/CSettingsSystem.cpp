
#include "ext/frameworkdef.h"
#include "provider/CSettingsSystem.h"


ECode CSettingsSystem::GetString(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ const String& name,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value);
    return Settings::System::GetString(resolver, name, value);
}

ECode CSettingsSystem::PutString(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ const String& name,
    /* [in] */ const String& value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return Settings::System::PutString(resolver, name, value, result);
}

ECode CSettingsSystem::GetUriFor(
    /* [in] */ const String& name,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);
    return Settings::System::GetUriFor(name, uri);
}

ECode CSettingsSystem::GetInt32(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int32 def,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    return Settings::System::GetInt32(cr, name, def, value);
}

ECode CSettingsSystem::GetInt32Ex(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    return Settings::System::GetInt32Ex(cr, name, value);
}

ECode CSettingsSystem::PutInt32(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int32 value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return Settings::System::PutInt32(cr, name, value, result);
}

ECode CSettingsSystem::GetInt64(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int64 def,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);
    return Settings::System::GetInt64(cr, name, def, value);
}

ECode CSettingsSystem::GetInt64Ex(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);
    return Settings::System::GetInt64Ex(cr, name, value);
}

ECode CSettingsSystem::PutInt64(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int64 value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return Settings::System::PutInt64(cr, name, value, result);
}

ECode CSettingsSystem::GetFloat(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Float def,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);
    return Settings::System::GetFloat(cr, name, def, value);
}

ECode CSettingsSystem::GetFloatEx(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);
    return Settings::System::GetFloatEx(cr, name, value);
}

ECode CSettingsSystem::PutFloat(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Float value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return Settings::System::PutFloat(cr, name, value, result);
}

ECode CSettingsSystem::GetConfiguration(
    /* [in] */ IContentResolver* cr,
    /* [in] */ IConfiguration* outConfig)
{
    return Settings::System::GetConfiguration(cr, outConfig);
}

ECode CSettingsSystem::PutConfiguration(
    /* [in] */ IContentResolver* cr,
    /* [in] */ IConfiguration* config,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return Settings::System::PutConfiguration(cr, config, result);
}

/** @hide */
ECode CSettingsSystem::HasInterestingConfigurationChanges(
    /* [in] */ Int32 changes,
    /* [out] */ Boolean* result)
 {
    VALIDATE_NOT_NULL(result);
    return Settings::System::HasInterestingConfigurationChanges(changes, result);
 }

ECode CSettingsSystem::GetShowGTalkServiceStatus(
    /* [in] */ IContentResolver* cr,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return Settings::System::GetShowGTalkServiceStatus(cr, result);
}

ECode CSettingsSystem::SetShowGTalkServiceStatus(
    /* [in] */ IContentResolver* cr,
    /* [in] */ Boolean flag)
{
    return Settings::System::SetShowGTalkServiceStatus(cr, flag);
}
