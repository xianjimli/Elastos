
#include "location/CLocationManager.h"



ECode CLocationManager::GetAllProviders(
    /* [out, callee] */ ArrayOf<String>** allProviders)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManager::GetProviders(
    /* [in] */ Boolean enabledOnly,
    /* [out, callee] */ ArrayOf<String>** providers)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManager::GetProvider(
    /* [in] */ const String& name,
    /* [out] */ ILocalLocationProvider** provider)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManager::GetProvidersEx(
    /* [in] */ ICriteria* criteria,
    /* [in] */ Boolean enabledOnly,
    /* [out, callee] */ ArrayOf<String>** providers)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManager::GetBestProvider(
    /* [in] */ ICriteria* criteria,
    /* [in] */ Boolean enabledOnly,
    /* [out] */ String* provider)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManager::RequestLocationUpdates(
    /* [in] */ const String& provider,
    /* [in] */ Int64 minTime,
    /* [in] */ Float minDistance,
    /* [in] */ ILocationListener* listener)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManager::RequestLocationUpdatesEx(
    /* [in] */ const String& provider,
    /* [in] */ Int64 minTime,
    /* [in] */ Float minDistance,
    /* [in] */ ILocationListener* listener,
    /* [in] */ IApartment* looper)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManager::RequestLocationUpdatesEx2(
    /* [in] */ Int64 minTime,
    /* [in] */ Float minDistance,
    /* [in] */ ICriteria* criteria,
    /* [in] */ ILocationListener* listener,
    /* [in] */ IApartment* looper)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManager::RequestLocationUpdatesPI(
    /* [in] */ const String& provider,
    /* [in] */ Int64 minTime,
    /* [in] */ Float minDistance,
    /* [in] */ IPendingIntent* intent)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManager::RequestLocationUpdatesPIEx(
    /* [in] */ Int64 minTime,
    /* [in] */ Float minDistance,
    /* [in] */ ICriteria* criteria,
    /* [in] */ IPendingIntent* intent)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManager::RequestSingleUpdate(
    /* [in] */ const String& provider,
    /* [in] */ ILocationListener* listener,
    /* [in] */ IApartment* looper)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManager::RequestSingleUpdateEx(
    /* [in] */ ICriteria* criteria,
    /* [in] */ ILocationListener* listener,
    /* [in] */ IApartment* looper)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManager::RequestSingleUpdatePI(
    /* [in] */ const String& provider,
    /* [in] */ IPendingIntent* intent)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManager::RequestSingleUpdatePIEx(
    /* [in] */ ICriteria* criteria,
    /* [in] */ IPendingIntent* intent)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManager::RemoveUpdates(
    /* [in] */ ILocationListener* listener)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManager::RemoveUpdatesPI(
    /* [in] */ IPendingIntent* intent)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManager::AddProximityAlert(
    /* [in] */ Double latitude,
    /* [in] */ Double longitude,
    /* [in] */ Float distance,
    /* [in] */ Int64 expiration,
    /* [in] */ IPendingIntent* intent)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManager::RemoveProximityAlert(
    /* [in] */ IPendingIntent* intent)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManager::IsProviderEnabled(
    /* [in] */ const String& provider,
    /* [out] */ Boolean* isEnabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManager::GetLastKnownLocation(
    /* [in] */ const String& provider,
    /* [out] */ ILocation** location)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManager::AddTestProvider(
    /* [in] */ const String& name,
    /* [in] */ Boolean requiresNetwork,
    /* [in] */ Boolean requiresSatellite,
    /* [in] */ Boolean requiresCell,
    /* [in] */ Boolean hasMonetaryCost,
    /* [in] */ Boolean supportsAltitude,
    /* [in] */ Boolean supportsSpeed,
    /* [in] */ Boolean supportsBearing,
    /* [in] */ Int32 powerRequirement,
    /* [in] */ Int32 accuracy)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManager::RemoveTestProvider(
    /* [in] */ const String& provider)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManager::SetTestProviderLocation(
    /* [in] */ const String& provider,
    /* [in] */ ILocation* loc)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManager::ClearTestProviderLocation(
    /* [in] */ const String& provider)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManager::SetTestProviderEnabled(
    /* [in] */ const String& provider,
    /* [in] */ Boolean enabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManager::ClearTestProviderEnabled(
    /* [in] */ const String& provider)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManager::SetTestProviderStatus(
    /* [in] */ const String& provider,
    /* [in] */ Int32 status,
    /* [in] */ IBundle* extras,
    /* [in] */ Int64 updateTime)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManager::ClearTestProviderStatus(
    /* [in] */ const String& provider)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManager::AddGpsStatusListener(
    /* [in] */ ILocalGpsStatusListener* listener,
    /* [out] */ Boolean* result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManager::RemoveGpsStatusListener(
    /* [in] */ ILocalGpsStatusListener* listener)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManager::AddNmeaListener(
    /* [in] */ ILocalGpsStatusNmeaListener* listener,
    /* [out] */ Boolean* result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManager::RemoveNmeaListener(
    /* [in] */ ILocalGpsStatusNmeaListener* listener)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManager::GetGpsStatus(
    /* [out] */ IGpsStatus** status)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManager::SendExtraCommand(
    /* [in] */ const String& provider,
    /* [in] */ const String& command,
    /* [in] */ IBundle* inExtras, //inout
    /* [out] */ IBundle** outExtras,
    /* [out] */ Boolean* result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManager::SendNiResponse(
    /* [in] */ Int32 notifId,
    /* [in] */ Int32 userResponse,
    /* [out] */ Boolean* result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManager::GetDescription(
    /* [out] */ String* str)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManager::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManager::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManager::constructor(
    /* [in] */ ILocationManager* service)
{
    return E_NOT_IMPLEMENTED;
}
