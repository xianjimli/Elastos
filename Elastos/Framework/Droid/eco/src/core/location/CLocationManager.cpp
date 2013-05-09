
#include "ext/frameworkdef.h"
#include "location/CLocationManager.h"


ECode CLocationManager::constructor(
    /* [in] */ ILocationManager* service)
{
    return LocationManager::Init(service);
}

ECode CLocationManager::GetAllProviders(
    /* [out] */ IObjectContainer** allProviders)
{
    VALIDATE_NOT_NULL(allProviders);

    return LocationManager::GetAllProviders(allProviders);
}

ECode CLocationManager::GetProviders(
    /* [in] */ Boolean enabledOnly,
    /* [out] */ IObjectContainer** providers)
{
    VALIDATE_NOT_NULL(providers);

    return LocationManager::GetProviders(enabledOnly, providers);
}

ECode CLocationManager::GetProvider(
    /* [in] */ const String& name,
    /* [out] */ ILocalLocationProvider** provider)
{
    VALIDATE_NOT_NULL(provider);

    return LocationManager::GetProvider(name, provider);
}

ECode CLocationManager::GetProvidersEx(
    /* [in] */ ICriteria* criteria,
    /* [in] */ Boolean enabledOnly,
    /* [out] */ IObjectContainer** providers)
{
    VALIDATE_NOT_NULL(providers);

    return LocationManager::GetProviders(criteria, enabledOnly, providers);
}

ECode CLocationManager::GetBestProvider(
    /* [in] */ ICriteria* criteria,
    /* [in] */ Boolean enabledOnly,
    /* [out] */ String* provider)
{
    VALIDATE_NOT_NULL(provider);

    return LocationManager::GetBestProvider(criteria, enabledOnly, provider);
}

ECode CLocationManager::RequestLocationUpdates(
    /* [in] */ const String& provider,
    /* [in] */ Int64 minTime,
    /* [in] */ Float minDistance,
    /* [in] */ ILocalLocationListener* listener)
{
    return LocationManager::RequestLocationUpdates(provider, minTime,
            minDistance, listener);
}

ECode CLocationManager::RequestLocationUpdatesEx(
    /* [in] */ const String& provider,
    /* [in] */ Int64 minTime,
    /* [in] */ Float minDistance,
    /* [in] */ ILocalLocationListener* listener,
    /* [in] */ IApartment* apartment)
{
    return LocationManager::RequestLocationUpdates(provider, minTime,
            minDistance, listener, apartment);
}

ECode CLocationManager::RequestLocationUpdatesEx2(
    /* [in] */ Int64 minTime,
    /* [in] */ Float minDistance,
    /* [in] */ ICriteria* criteria,
    /* [in] */ ILocalLocationListener* listener,
    /* [in] */ IApartment* apartment)
{
    return LocationManager::RequestLocationUpdates(minTime, minDistance,
            criteria, listener, apartment);
}

ECode CLocationManager::RequestLocationUpdatesPI(
    /* [in] */ const String& provider,
    /* [in] */ Int64 minTime,
    /* [in] */ Float minDistance,
    /* [in] */ IPendingIntent* intent)
{
    return LocationManager::RequestLocationUpdates(provider, minTime, minDistance, intent);
}

ECode CLocationManager::RequestLocationUpdatesPIEx(
    /* [in] */ Int64 minTime,
    /* [in] */ Float minDistance,
    /* [in] */ ICriteria* criteria,
    /* [in] */ IPendingIntent* intent)
{
    return LocationManager::RequestLocationUpdates(minTime, minDistance, criteria, intent);
}

ECode CLocationManager::RequestSingleUpdate(
    /* [in] */ const String& provider,
    /* [in] */ ILocalLocationListener* listener,
    /* [in] */ IApartment* apartment)
{
    return LocationManager::RequestSingleUpdate(provider, listener, apartment);
}

ECode CLocationManager::RequestSingleUpdateEx(
    /* [in] */ ICriteria* criteria,
    /* [in] */ ILocalLocationListener* listener,
    /* [in] */ IApartment* apartment)
{
    return LocationManager::RequestSingleUpdate(criteria, listener, apartment);
}

ECode CLocationManager::RequestSingleUpdatePI(
    /* [in] */ const String& provider,
    /* [in] */ IPendingIntent* intent)
{
    return LocationManager::RequestSingleUpdate(provider, intent);
}

ECode CLocationManager::RequestSingleUpdatePIEx(
    /* [in] */ ICriteria* criteria,
    /* [in] */ IPendingIntent* intent)
{
    return LocationManager::RequestSingleUpdate(criteria, intent);
}

ECode CLocationManager::RemoveUpdates(
    /* [in] */ ILocalLocationListener* listener)
{
    return LocationManager::RemoveUpdates(listener);
}

ECode CLocationManager::RemoveUpdatesPI(
    /* [in] */ IPendingIntent* intent)
{
    return LocationManager::RemoveUpdates(intent);
}

ECode CLocationManager::AddProximityAlert(
    /* [in] */ Double latitude,
    /* [in] */ Double longitude,
    /* [in] */ Float distance,
    /* [in] */ Int64 expiration,
    /* [in] */ IPendingIntent* intent)
{
    return LocationManager::AddProximityAlert(latitude, longitude,
            distance, expiration, intent);
}

ECode CLocationManager::RemoveProximityAlert(
    /* [in] */ IPendingIntent* intent)
{
    return LocationManager::RemoveProximityAlert(intent);
}

ECode CLocationManager::IsProviderEnabled(
    /* [in] */ const String& provider,
    /* [out] */ Boolean* isEnabled)
{
    VALIDATE_NOT_NULL(isEnabled);

    return LocationManager::IsProviderEnabled(provider, isEnabled);
}

ECode CLocationManager::GetLastKnownLocation(
    /* [in] */ const String& provider,
    /* [out] */ ILocation** location)
{
    VALIDATE_NOT_NULL(location);

    return LocationManager::GetLastKnownLocation(provider, location);
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
    return LocationManager::AddTestProvider(name, requiresNetwork, requiresSatellite, requiresCell,
            hasMonetaryCost, supportsAltitude, supportsSpeed, supportsBearing, powerRequirement, accuracy);
}

ECode CLocationManager::RemoveTestProvider(
    /* [in] */ const String& provider)
{
    return LocationManager::RemoveTestProvider(provider);
}

ECode CLocationManager::SetTestProviderLocation(
    /* [in] */ const String& provider,
    /* [in] */ ILocation* loc)
{
    return LocationManager::SetTestProviderLocation(provider, loc);
}

ECode CLocationManager::ClearTestProviderLocation(
    /* [in] */ const String& provider)
{
    return LocationManager::ClearTestProviderLocation(provider);
}

ECode CLocationManager::SetTestProviderEnabled(
    /* [in] */ const String& provider,
    /* [in] */ Boolean enabled)
{
    return LocationManager::SetTestProviderEnabled(provider, enabled);
}

ECode CLocationManager::ClearTestProviderEnabled(
    /* [in] */ const String& provider)
{
    return LocationManager::ClearTestProviderEnabled(provider);
}

ECode CLocationManager::SetTestProviderStatus(
    /* [in] */ const String& provider,
    /* [in] */ Int32 status,
    /* [in] */ IBundle* extras,
    /* [in] */ Int64 updateTime)
{
    return LocationManager::SetTestProviderStatus(provider, status, extras, updateTime);
}

ECode CLocationManager::ClearTestProviderStatus(
    /* [in] */ const String& provider)
{
    return LocationManager::ClearTestProviderStatus(provider);
}

ECode CLocationManager::AddGpsStatusListener(
    /* [in] */ ILocalGpsStatusListener* listener,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    return LocationManager::AddGpsStatusListener(listener, result);
}

ECode CLocationManager::RemoveGpsStatusListener(
    /* [in] */ ILocalGpsStatusListener* listener)
{
    return LocationManager::RemoveGpsStatusListener(listener);
}

ECode CLocationManager::AddNmeaListener(
    /* [in] */ ILocalGpsStatusNmeaListener* listener,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    return LocationManager::AddNmeaListener(listener, result);
}

ECode CLocationManager::RemoveNmeaListener(
    /* [in] */ ILocalGpsStatusNmeaListener* listener)
{
    return LocationManager::RemoveNmeaListener(listener);
}

ECode CLocationManager::GetGpsStatus(
    /* [in] */ IGpsStatus* inStatus,
    /* [out] */ IGpsStatus** outStatus)
{
    VALIDATE_NOT_NULL(outStatus);

    return LocationManager::GetGpsStatus(inStatus, outStatus);
}

ECode CLocationManager::SendExtraCommand(
    /* [in] */ const String& provider,
    /* [in] */ const String& command,
    /* [in] */ IBundle* inExtras, //inout
    /* [out] */ IBundle** outExtras,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    return LocationManager::SendExtraCommand(provider, command, inExtras, outExtras, result);
}

ECode CLocationManager::SendNiResponse(
    /* [in] */ Int32 notifId,
    /* [in] */ Int32 userResponse,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    return LocationManager::SendNiResponse(notifId, userResponse, result);
}
