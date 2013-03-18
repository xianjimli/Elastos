
#include "ext/frameworkdef.h"
#include "provider/CSettingsSecure.h"


ECode CSettingsSecure::GetString(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ const String& name,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value);
    return Settings::Secure::GetString(resolver, name, value);
}

ECode CSettingsSecure::PutString(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ const String& name,
    /* [in] */ const String& value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return Settings::Secure::PutString(resolver, name, value, result);
}


ECode CSettingsSecure::GetUriFor(
    /* [in] */ const String& name,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);
    return Settings::Secure::GetUriFor(name, uri);
}

ECode CSettingsSecure::GetInt32(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int32 def,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    return Settings::Secure::GetInt32(cr, name, def, value);
}

ECode CSettingsSecure::GetInt32Ex(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    return Settings::Secure::GetInt32Ex(cr, name, value);
}

ECode CSettingsSecure::PutInt32(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int32 value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return Settings::Secure::PutInt32(cr, name, value, result);
}

ECode CSettingsSecure::GetInt64(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int64 def,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);
    return Settings::Secure::GetInt64(cr, name, def, value);
}

ECode CSettingsSecure::GetInt64Ex(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);
    return Settings::Secure::GetInt64Ex(cr, name, value);
}

ECode CSettingsSecure::PutInt64(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int64 value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return Settings::Secure::PutInt64(cr, name, value, result);
}

ECode CSettingsSecure::GetFloat(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Float def,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);
    return Settings::Secure::GetFloat(cr, name, def, value);
}

ECode CSettingsSecure::GetFloatEx(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);
    return Settings::Secure::GetFloatEx(cr, name, value);
}

ECode CSettingsSecure::PutFloat(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Float value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return Settings::Secure::PutFloat(cr, name, value, result);
}

ECode CSettingsSecure::GetBluetoothHeadsetPriorityKey(
    /* [in] */ const String& address,
    /* [out] */ String* key)
{
    VALIDATE_NOT_NULL(key);
    return Settings::Secure::GetBluetoothHeadsetPriorityKey(address, key);
}

ECode CSettingsSecure::GetBluetoothA2dpSinkPriorityKey(
    /* [in] */ const String& address,
    /* [out] */ String* key)
{
    VALIDATE_NOT_NULL(key);
    return Settings::Secure::GetBluetoothA2dpSinkPriorityKey(address, key);
}

ECode CSettingsSecure::IsLocationProviderEnabled(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& provider,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return Settings::Secure::IsLocationProviderEnabled(cr, provider, result);
}

ECode CSettingsSecure::SetLocationProviderEnabled(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& provider,
    /* [in] */ Boolean enabled)
{
    return Settings::Secure::SetLocationProviderEnabled(cr, provider, enabled);
}
