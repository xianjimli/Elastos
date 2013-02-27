#include "provider/CSettingsSecure.h"

using namespace Elastos::Core;

ECode CSettingsSecure::GetString(
    /*[in]*/ IContentResolver* resolver,
    /*[in]*/ const String& name,
    /*[out]*/ String* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSettingsSecure::PutString(
    /*[in]*/ IContentResolver* resolver,
    /*[in]*/ const String& name,
    /*[in]*/ const String& value,
    /*[out]*/ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}


ECode CSettingsSecure::GetUriFor(
    /*[in]*/ const String& name,
    /*[out]*/ IUri** uri)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSettingsSecure::GetInt(
    /*[in]*/ IContentResolver* cr,
    /*[in]*/ const String& name,
    /*[in]*/ Int32 def,
    /*[out]*/ Int32* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSettingsSecure::GetIntExtra(
    /*[in]*/ IContentResolver* cr,
    /*[in]*/ const String& name,
    /*[out]*/ Int32* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSettingsSecure::PutInt(
    /*[in]*/ IContentResolver* cr,
    /*[in]*/ const String& name,
    /*[in]*/ Int32 value,
    /*[out]*/ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSettingsSecure::GetLong(
    /*[in]*/ IContentResolver* cr,
    /*[in]*/ const String& name,
    /*[in]*/ Int64 def,
    /*[out]*/ Int64* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSettingsSecure::GetLongExtra(
    /*[in]*/ IContentResolver* cr,
    /*[in]*/ const String& name,
    /*[out]*/ Int64* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSettingsSecure::PutLong(
    /*[in]*/ IContentResolver* cr,
    /*[in]*/ const String& name,
    /*[in]*/ Int64 value,
    /*[out]*/ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSettingsSecure::GetFloat(
    /*[in]*/ IContentResolver* cr,
    /*[in]*/ const String& name,
    /*[in]*/ Float def,
    /*[out]*/ Float* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSettingsSecure::GetFloatExtra(
    /*[in]*/ IContentResolver* cr,
    /*[in]*/ const String& name,
    /*[out]*/ Float* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSettingsSecure::PutFloat(
    /*[in]*/ IContentResolver* cr,
    /*[in]*/ const String& name,
    /*[in]*/ Float value,
    /*[out]*/ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSettingsSecure::GetBluetoothHeadsetPriorityKey(
    /*[in]*/ const String& address,
    /*[out]*/ String* key)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSettingsSecure::GetBluetoothA2dpSinkPriorityKey(
    /*[in]*/ const String& address,
    /*[out]*/ String* key)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSettingsSecure::IsLocationProviderEnabled(
    /*[in]*/ IContentResolver* cr,
    /*[in]*/ const String& provider,
    /*[out]*/ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSettingsSecure::SetLocationProviderEnabled(
    /*[in]*/ IContentResolver* cr,
    /*[in]*/ const String& provider,
    /*[in]*/ Boolean enabled)
{
    return E_NOT_IMPLEMENTED;
}