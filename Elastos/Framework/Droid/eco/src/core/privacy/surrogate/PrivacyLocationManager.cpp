
#include "privacy/surrogate/PrivacyLocationManager.h"
#include "privacy/PrivacySettingsManager.h"
#include "os/CServiceManager.h"


const CString PrivacyLocationManager::TAG = "PrivacyLocationManager";
const Int32 PrivacyLocationManager::CUSTOM_LOCATION_UPDATE_COUNT;


PrivacyLocationManager::PrivacyLocationManager(
    /* [in] */ ILocationManager* service,
    /* [in] */ IContext* context)
{
    LocationManager::Init(service);
    mContext = context;
    // pSetMan = (PrivacySettingsManager) context.getSystemService("privacy");
    AutoPtr<IServiceManager> sm;
    CServiceManager::AcquireSingleton((IServiceManager**)&sm);
    AutoPtr<IInterface> psm;
    sm->GetService(String("privacy"), (IInterface**)&psm);
    mSetMan = new PrivacySettingsManager(context, IPrivacySettingsManager::Probe(psm));
}

PInterface PrivacyLocationManager::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(ILocalLocationManager*)this;
    }
    else if (riid == EIID_ILocalLocationManager) {
        return (ILocalLocationManager*)this;
    }

    return NULL;
}

UInt32 PrivacyLocationManager::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 PrivacyLocationManager::Release()
{
    return ElRefBase::Release();
}

ECode PrivacyLocationManager::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(ILocalLocationManager*)this) {
        *pIID = EIID_ILocalLocationManager;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode PrivacyLocationManager::GetAllProviders(
    /* [out] */ IObjectContainer** allProviders)
{
    VALIDATE_NOT_NULL(allProviders);

    return LocationManager::GetAllProviders(allProviders);
}

ECode PrivacyLocationManager::GetProviders(
    /* [in] */ Boolean enabledOnly,
    /* [out] */ IObjectContainer** providers)
{
    VALIDATE_NOT_NULL(providers);

    return LocationManager::GetProviders(enabledOnly, providers);
}

ECode PrivacyLocationManager::GetProvider(
    /* [in] */ const String& name,
    /* [out] */ ILocalLocationProvider** provider)
{
    VALIDATE_NOT_NULL(provider);

    // if (name == null) return super.getProvider(name);

    // PrivacySettings pSet = pSetMan.getSettings(context.getPackageName(), Binder.getCallingUid());
    // LocationProvider output = null;

    // if (pSet != null) {
    //     if (name.equals(LocationManager.GPS_PROVIDER)) {
    //         switch (pSet.getLocationGpsSetting()) {
    //             case PrivacySettings.REAL:
    //             case PrivacySettings.CUSTOM:
    //             case PrivacySettings.RANDOM:
    //                 output = super.getProvider(name);
    //                 break;
    //             case PrivacySettings.EMPTY:
    //                 break;
    //         }
    //     } else if (name.equals(LocationManager.NETWORK_PROVIDER)) {
    //         switch (pSet.getLocationNetworkSetting()) {
    //             case PrivacySettings.REAL:
    //             case PrivacySettings.CUSTOM:
    //             case PrivacySettings.RANDOM:
    //                 output = super.getProvider(name);
    //                 break;
    //             case PrivacySettings.EMPTY:
    //                 break;
    //         }
    //     } else if (name.equals(LocationManager.PASSIVE_PROVIDER)) { // could get location from any of above
    //         if (pSet.getLocationGpsSetting() == PrivacySettings.REAL ||
    //                 pSet.getLocationNetworkSetting() == PrivacySettings.REAL) {
    //             output = super.getProvider(name);
    //         }
    //     }
    // } else {
    //     output = super.getProvider(name);
    // }

    // Log.d(TAG, "getProvider - " + context.getPackageName() + " (" + Binder.getCallingUid() + ") output: " +
    //         (output != null ? "[real value]" : "[null]"));
    // return output;
    return E_NOT_IMPLEMENTED;
}

ECode PrivacyLocationManager::GetProvidersEx(
    /* [in] */ ICriteria* criteria,
    /* [in] */ Boolean enabledOnly,
    /* [out] */ IObjectContainer** providers)
{
    VALIDATE_NOT_NULL(providers);

    return LocationManager::GetProviders(criteria, enabledOnly, providers);
}

ECode PrivacyLocationManager::GetBestProvider(
    /* [in] */ ICriteria* criteria,
    /* [in] */ Boolean enabledOnly,
    /* [out] */ String* provider)
{
    VALIDATE_NOT_NULL(provider);

    return LocationManager::GetBestProvider(criteria, enabledOnly, provider);
}

ECode PrivacyLocationManager::RequestLocationUpdates(
    /* [in] */ const String& provider,
    /* [in] */ Int64 minTime,
    /* [in] */ Float minDistance,
    /* [in] */ ILocalLocationListener* listener)
{
    // if (provider == null || listener == null) {
    //     super.requestLocationUpdates(provider, minTime, minDistance, listener);
    //     return;
    // }
    // if (requestLocationUpdates(provider, listener, null)) return;
    // super.requestLocationUpdates(provider, minTime, minDistance, listener);
    return E_NOT_IMPLEMENTED;
}

ECode PrivacyLocationManager::RequestLocationUpdatesEx(
    /* [in] */ const String& provider,
    /* [in] */ Int64 minTime,
    /* [in] */ Float minDistance,
    /* [in] */ ILocalLocationListener* listener,
    /* [in] */ IApartment* apartment)
{
    // if (provider == null || listener == null) {
    //     super.requestLocationUpdates(provider, minTime, minDistance, listener, looper);
    //     return;
    // }
    // if (requestLocationUpdates(provider, listener, null)) return;
    // super.requestLocationUpdates(provider, minTime, minDistance, listener, looper);
    return E_NOT_IMPLEMENTED;
}

ECode PrivacyLocationManager::RequestLocationUpdatesEx2(
    /* [in] */ Int64 minTime,
    /* [in] */ Float minDistance,
    /* [in] */ ICriteria* criteria,
    /* [in] */ ILocalLocationListener* listener,
    /* [in] */ IApartment* apartment)
{
    // if (criteria == null || listener == null) {
    //     super.requestLocationUpdates(minTime, minDistance, criteria, listener, looper);
    //     return;
    // }
    // if (requestLocationUpdates(criteria, listener, null)) return;
    // super.requestLocationUpdates(minTime, minDistance, criteria, listener, looper);
    return E_NOT_IMPLEMENTED;
}

ECode PrivacyLocationManager::RequestLocationUpdatesPI(
    /* [in] */ const String& provider,
    /* [in] */ Int64 minTime,
    /* [in] */ Float minDistance,
    /* [in] */ IPendingIntent* intent)
{
    // if (provider == null || intent == null) {
    //     super.requestLocationUpdates(provider, minTime, minDistance, intent);
    //     return;
    // }
    // if (requestLocationUpdates(provider, null, intent)) return;
    // super.requestLocationUpdates(provider, minTime, minDistance, intent);
    return E_NOT_IMPLEMENTED;
}

ECode PrivacyLocationManager::RequestLocationUpdatesPIEx(
    /* [in] */ Int64 minTime,
    /* [in] */ Float minDistance,
    /* [in] */ ICriteria* criteria,
    /* [in] */ IPendingIntent* intent)
{
    // if (criteria == null || intent == null) {
    //     super.requestLocationUpdates(minTime, minDistance, criteria, intent);
    //     return;
    // }
    // if (requestLocationUpdates(criteria, null, intent)) return;
    // super.requestLocationUpdates(minTime, minDistance, criteria, intent);
    return E_NOT_IMPLEMENTED;
}

ECode PrivacyLocationManager::RequestSingleUpdate(
    /* [in] */ const String& provider,
    /* [in] */ ILocalLocationListener* listener,
    /* [in] */ IApartment* apartment)
{
    // if (provider == null || listener == null) {
    //     super.requestSingleUpdate(provider, listener, looper);
    //     return;
    // }
    // if (requestLocationUpdates(provider, listener, null)) return;
    // super.requestSingleUpdate(provider, listener, looper);
    return E_NOT_IMPLEMENTED;
}

ECode PrivacyLocationManager::RequestSingleUpdateEx(
    /* [in] */ ICriteria* criteria,
    /* [in] */ ILocalLocationListener* listener,
    /* [in] */ IApartment* apartment)
{
    // if (criteria == null || listener == null) {
    //     super.requestSingleUpdate(criteria, listener, looper);
    //     return;
    // }
    // if (requestLocationUpdates(criteria, listener, null)) return;
    // super.requestSingleUpdate(criteria, listener, looper);
    return E_NOT_IMPLEMENTED;
}

ECode PrivacyLocationManager::RequestSingleUpdatePI(
    /* [in] */ const String& provider,
    /* [in] */ IPendingIntent* intent)
{
    // if (provider == null || intent == null) {
    //     super.requestSingleUpdate(provider, intent);
    //     return;
    // }
    // if (requestLocationUpdates(provider, null, intent)) return;
    // super.requestSingleUpdate(provider, intent);
    return E_NOT_IMPLEMENTED;
}

ECode PrivacyLocationManager::RequestSingleUpdatePIEx(
    /* [in] */ ICriteria* criteria,
    /* [in] */ IPendingIntent* intent)
{
    // if (criteria == null || intent == null) {
    //     super.requestSingleUpdate(criteria, intent);
    //     return;
    // }
    // if (requestLocationUpdates(criteria, null, intent)) return;
    // super.requestSingleUpdate(criteria, intent);
    return E_NOT_IMPLEMENTED;
}

ECode PrivacyLocationManager::RemoveUpdates(
    /* [in] */ ILocalLocationListener* listener)
{
    return LocationManager::RemoveUpdates(listener);
}

ECode PrivacyLocationManager::RemoveUpdatesPI(
    /* [in] */ IPendingIntent* intent)
{
    return LocationManager::RemoveUpdates(intent);
}

ECode PrivacyLocationManager::AddProximityAlert(
    /* [in] */ Double latitude,
    /* [in] */ Double longitude,
    /* [in] */ Float distance,
    /* [in] */ Int64 expiration,
    /* [in] */ IPendingIntent* intent)
{
    return LocationManager::AddProximityAlert(latitude, longitude,
            distance, expiration, intent);
}

ECode PrivacyLocationManager::RemoveProximityAlert(
    /* [in] */ IPendingIntent* intent)
{
    return LocationManager::RemoveProximityAlert(intent);
}

ECode PrivacyLocationManager::IsProviderEnabled(
    /* [in] */ const String& provider,
    /* [out] */ Boolean* isEnabled)
{
    VALIDATE_NOT_NULL(isEnabled);

    // if (provider == null) return super.isProviderEnabled(provider);

    // PrivacySettings pSet = pSetMan.getSettings(context.getPackageName(), Binder.getCallingUid());
    // boolean output = false;

    // if (pSet != null) {
    //     if (provider.equals(LocationManager.GPS_PROVIDER)) {
    //         switch (pSet.getLocationGpsSetting()) {
    //             case PrivacySettings.REAL:
    //                 output = super.isProviderEnabled(provider);
    //                 break;
    //             case PrivacySettings.EMPTY:
    //                 break;
    //             case PrivacySettings.CUSTOM:
    //             case PrivacySettings.RANDOM:
    //                 output = true;
    //                 break;
    //         }
    //     } else if (provider.equals(LocationManager.NETWORK_PROVIDER)) {
    //         switch (pSet.getLocationNetworkSetting()) {
    //             case PrivacySettings.REAL:
    //                 output = super.isProviderEnabled(provider);
    //                 break;
    //             case PrivacySettings.EMPTY:
    //                 break;
    //             case PrivacySettings.CUSTOM:
    //             case PrivacySettings.RANDOM:
    //                 output = true;
    //                 break;
    //         }
    //     } else if (provider.equals(LocationManager.PASSIVE_PROVIDER)) { // could get location from any of above
    //         if (pSet.getLocationGpsSetting() == PrivacySettings.REAL ||
    //                 pSet.getLocationNetworkSetting() == PrivacySettings.REAL) {
    //             output = super.isProviderEnabled(provider);
    //         } else {
    //             output = false;
    //         }
    //     }
    // } else { // if querying unknown provider
    //     output = super.isProviderEnabled(provider);
    // }

    // Log.d(TAG, "isProviderEnabled - " + context.getPackageName() + " (" + Binder.getCallingUid() + ") provider: "
    //         + provider + "output: " + output);
    // return output;
    return E_NOT_IMPLEMENTED;
}

ECode PrivacyLocationManager::GetLastKnownLocation(
    /* [in] */ const String& provider,
    /* [out] */ ILocation** location)
{
    VALIDATE_NOT_NULL(location);

    // if (provider == null) return super.getLastKnownLocation(provider);

    // String packageName = context.getPackageName();
    // int uid = Binder.getCallingUid();
    // PrivacySettings pSet = pSetMan.getSettings(packageName, uid);
    // Location output = null;

    // if (pSet != null) {
    //     if (provider.equals(LocationManager.GPS_PROVIDER)) {
    //         switch (pSet.getLocationGpsSetting()) {
    //             case PrivacySettings.REAL:
    //                 output = super.getLastKnownLocation(provider);
    //                 pSetMan.notification(packageName, uid, PrivacySettings.REAL, PrivacySettings.DATA_LOCATION_GPS,
    //                         (output != null ? "Lat: " + output.getLatitude() + " Lon: " + output.getLongitude() : null), pSet);
    //                 break;
    //             case PrivacySettings.EMPTY:
    //                 pSetMan.notification(packageName, uid, PrivacySettings.EMPTY, PrivacySettings.DATA_LOCATION_GPS, null, pSet);
    //                 break;
    //             case PrivacySettings.CUSTOM:
    //                 output = new Location(provider);
    //                 output.setLatitude(Double.parseDouble(pSet.getLocationGpsLat()));
    //                 output.setLongitude(Double.parseDouble(pSet.getLocationGpsLon()));
    //                 pSetMan.notification(packageName, uid, PrivacySettings.CUSTOM, PrivacySettings.DATA_LOCATION_GPS,
    //                         "Lat: " + output.getLatitude() + " Lon: " + output.getLongitude(), pSet);
    //                 break;
    //             case PrivacySettings.RANDOM:
    //                 output = new Location(provider);
    //                 output.setLatitude(Double.parseDouble(pSet.getLocationGpsLat()));
    //                 output.setLongitude(Double.parseDouble(pSet.getLocationGpsLon()));
    //                 pSetMan.notification(packageName, uid, PrivacySettings.RANDOM, PrivacySettings.DATA_LOCATION_GPS,
    //                         "Lat: " + output.getLatitude() + " Lon: " + output.getLongitude(), pSet);
    //                 break;
    //         }
    //     } else if (provider.equals(LocationManager.NETWORK_PROVIDER)) {
    //         switch (pSet.getLocationNetworkSetting()) {
    //             case PrivacySettings.REAL:
    //                 output = super.getLastKnownLocation(provider);
    //                 pSetMan.notification(packageName, uid, PrivacySettings.REAL, PrivacySettings.DATA_LOCATION_NETWORK,
    //                         output != null ? "Lat: " + output.getLatitude() + " Lon: " + output.getLongitude() : null, pSet);
    //                 break;
    //             case PrivacySettings.EMPTY:
    //                 pSetMan.notification(packageName, uid, PrivacySettings.EMPTY, PrivacySettings.DATA_LOCATION_NETWORK, null, pSet);
    //                 break;
    //             case PrivacySettings.CUSTOM:
    //                 output = new Location(provider);
    //                 output.setLatitude(Double.parseDouble(pSet.getLocationNetworkLat()));
    //                 output.setLongitude(Double.parseDouble(pSet.getLocationNetworkLon()));
    //                 pSetMan.notification(packageName, uid, PrivacySettings.CUSTOM, PrivacySettings.DATA_LOCATION_NETWORK,
    //                         "Lat: " + output.getLatitude() + " Lon: " + output.getLongitude(), pSet);
    //                 break;
    //             case PrivacySettings.RANDOM:
    //                 output = new Location(provider);
    //                 output.setLatitude(Double.parseDouble(pSet.getLocationNetworkLat()));
    //                 output.setLongitude(Double.parseDouble(pSet.getLocationNetworkLon()));
    //                 pSetMan.notification(packageName, uid, PrivacySettings.RANDOM, PrivacySettings.DATA_LOCATION_NETWORK,
    //                         "Lat: " + output.getLatitude() + " Lon: " + output.getLongitude(), pSet);
    //                 break;
    //         }
    //     } else if (provider.equals(LocationManager.PASSIVE_PROVIDER) &&
    //             pSet.getLocationGpsSetting() == PrivacySettings.REAL &&
    //                     pSet.getLocationNetworkSetting() == PrivacySettings.REAL) {
    //         // only output real location if both gps and network are allowed
    //         output = super.getLastKnownLocation(provider);
    //         pSetMan.notification(packageName, uid, PrivacySettings.REAL, PrivacySettings.DATA_LOCATION_GPS,
    //                 output != null ? "Lat: " + output.getLatitude() + " Lon: " + output.getLongitude() : null, pSet);
    //     }
    // } else {
    //     output = super.getLastKnownLocation(provider);
    //     if (provider.equals(LocationManager.NETWORK_PROVIDER)) {
    //         pSetMan.notification(packageName, uid, PrivacySettings.REAL, PrivacySettings.DATA_LOCATION_NETWORK,
    //             output != null ? "Lat: " + output.getLatitude() + " Lon: " + output.getLongitude() : null, pSet);
    //     } else { // including GPS and passive providers
    //         pSetMan.notification(packageName, uid, PrivacySettings.REAL, PrivacySettings.DATA_LOCATION_GPS,
    //                 output != null ? "Lat: " + output.getLatitude() + " Lon: " + output.getLongitude() : null, pSet);
    //     }
    // }

    // Log.d(TAG, "getLastKnownLocation - " + context.getPackageName() + " (" + Binder.getCallingUid() +
    //         ") output: " + output);
    // return output;
    return E_NOT_IMPLEMENTED;
}

ECode PrivacyLocationManager::AddTestProvider(
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

ECode PrivacyLocationManager::RemoveTestProvider(
    /* [in] */ const String& provider)
{
    return LocationManager::RemoveTestProvider(provider);
}

ECode PrivacyLocationManager::SetTestProviderLocation(
    /* [in] */ const String& provider,
    /* [in] */ ILocation* loc)
{
    return LocationManager::SetTestProviderLocation(provider, loc);
}

ECode PrivacyLocationManager::ClearTestProviderLocation(
    /* [in] */ const String& provider)
{
    return LocationManager::ClearTestProviderLocation(provider);
}

ECode PrivacyLocationManager::SetTestProviderEnabled(
    /* [in] */ const String& provider,
    /* [in] */ Boolean enabled)
{
    return LocationManager::SetTestProviderEnabled(provider, enabled);
}

ECode PrivacyLocationManager::ClearTestProviderEnabled(
    /* [in] */ const String& provider)
{
    return LocationManager::ClearTestProviderEnabled(provider);
}

ECode PrivacyLocationManager::SetTestProviderStatus(
    /* [in] */ const String& provider,
    /* [in] */ Int32 status,
    /* [in] */ IBundle* extras,
    /* [in] */ Int64 updateTime)
{
    return LocationManager::SetTestProviderStatus(provider, status, extras, updateTime);
}

ECode PrivacyLocationManager::ClearTestProviderStatus(
    /* [in] */ const String& provider)
{
    return LocationManager::ClearTestProviderStatus(provider);
}

ECode PrivacyLocationManager::AddGpsStatusListener(
    /* [in] */ ILocalGpsStatusListener* listener,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    return LocationManager::AddGpsStatusListener(listener, result);
}

ECode PrivacyLocationManager::RemoveGpsStatusListener(
    /* [in] */ ILocalGpsStatusListener* listener)
{
    return LocationManager::RemoveGpsStatusListener(listener);
}

ECode PrivacyLocationManager::AddNmeaListener(
    /* [in] */ ILocalGpsStatusNmeaListener* listener,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    // // only blocks if access is not allowed
    // // custom and random values not implemented due to Decimal Degrees->NMEA conversion complexity
    // String packageName = context.getPackageName();
    // int uid = Binder.getCallingUid();
    // PrivacySettings pSet = pSetMan.getSettings(packageName, uid);

    // if (pSet != null && pSet.getLocationGpsSetting() != PrivacySettings.REAL) {
    //  pSetMan.notification(packageName, uid, PrivacySettings.EMPTY, PrivacySettings.DATA_LOCATION_GPS, null, pSet);
    //  return false;
    // } else {
    //  pSetMan.notification(packageName, uid, PrivacySettings.REAL, PrivacySettings.DATA_LOCATION_GPS, null, pSet);
    // }
    // Log.d(TAG, "addNmeaListener - " + context.getPackageName() + " (" + Binder.getCallingUid() + ") output: [real value]");
    // return super.addNmeaListener(listener);
    return E_NOT_IMPLEMENTED;
}

ECode PrivacyLocationManager::RemoveNmeaListener(
    /* [in] */ ILocalGpsStatusNmeaListener* listener)
{
    return LocationManager::RemoveNmeaListener(listener);
}

ECode PrivacyLocationManager::GetGpsStatus(
    /* [in] */ IGpsStatus* inStatus,
    /* [out] */ IGpsStatus** outStatus)
{
    VALIDATE_NOT_NULL(outStatus);

    return LocationManager::GetGpsStatus(inStatus, outStatus);
}

ECode PrivacyLocationManager::SendExtraCommand(
    /* [in] */ const String& provider,
    /* [in] */ const String& command,
    /* [in] */ IBundle* inExtras, //inout
    /* [out] */ IBundle** outExtras,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    return LocationManager::SendExtraCommand(provider, command, inExtras, outExtras, result);
}

ECode PrivacyLocationManager::SendNiResponse(
    /* [in] */ Int32 notifId,
    /* [in] */ Int32 userResponse,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    return LocationManager::SendNiResponse(notifId, userResponse, result);
}

Boolean PrivacyLocationManager::RequestLocationUpdates(
    /* [in] */ const String& provider,
    /* [in] */ ILocationListener* listener,
    /* [in] */ IPendingIntent* intent)
{
    // synchronized (lock) { // custom listener should only return a value after this method has returned

    //     String packageName = context.getPackageName();
    //     int uid = Binder.getCallingUid();
    //     PrivacySettings pSet = pSetMan.getSettings(packageName, uid);
    //     boolean output = false;

    //     if (pSet != null) {
    //         if (provider.equals(LocationManager.GPS_PROVIDER)) {
    //             switch (pSet.getLocationGpsSetting()) {
    //                 case PrivacySettings.REAL:
    //                     pSetMan.notification(packageName, uid, PrivacySettings.REAL, PrivacySettings.DATA_LOCATION_GPS, null, pSet);
    //                     break;
    //                 case PrivacySettings.EMPTY:
    //                     if (intent != null) intent.cancel();
    //                     output = true;
    //                     pSetMan.notification(packageName, uid, PrivacySettings.EMPTY, PrivacySettings.DATA_LOCATION_GPS, null, pSet);
    //                     break;
    //                 case PrivacySettings.CUSTOM:
    //                     try {
    //                         new PrivacyLocationUpdater(provider, listener, intent,
    //                                 Double.parseDouble(pSet.getLocationGpsLat()),
    //                                 Double.parseDouble(pSet.getLocationGpsLon())).start();
    //                         output = true;
    //                     } catch (NumberFormatException e) {
    //                         Log.e(TAG, "requestLocationUpdates: invalid coordinates");
    //                         output = true;
    //                     }
    //                     pSetMan.notification(packageName, uid, PrivacySettings.CUSTOM, PrivacySettings.DATA_LOCATION_GPS,
    //                             "Lat: " + pSet.getLocationGpsLat() + " Lon: " + pSet.getLocationGpsLon(), pSet);
    //                     break;
    //                 case PrivacySettings.RANDOM:
    //                     try {
    //                         new PrivacyLocationUpdater(provider, listener, intent,
    //                                 Double.parseDouble(pSet.getLocationGpsLat()),
    //                                 Double.parseDouble(pSet.getLocationGpsLon())).start();
    //                         output = true;
    //                     } catch (NumberFormatException e) {
    //                         Log.e(TAG, "requestLocationUpdates: invalid coordinates");
    //                         output = true;
    //                     }
    //                     pSetMan.notification(packageName, uid, PrivacySettings.RANDOM, PrivacySettings.DATA_LOCATION_GPS,
    //                             "Lat: " + pSet.getLocationGpsLat() + " Lon: " + pSet.getLocationGpsLon(), pSet);
    //                     break;
    //             }
    //         } else if (provider.equals(LocationManager.NETWORK_PROVIDER)) {
    //             switch (pSet.getLocationNetworkSetting()) {
    //                 case PrivacySettings.REAL:
    //                     pSetMan.notification(packageName, uid, PrivacySettings.REAL, PrivacySettings.DATA_LOCATION_NETWORK, null, pSet);
    //                     break;
    //                 case PrivacySettings.EMPTY:
    //                     if (intent != null) intent.cancel();
    //                     output = true;
    //                     pSetMan.notification(packageName, uid, PrivacySettings.EMPTY, PrivacySettings.DATA_LOCATION_NETWORK, null, pSet);
    //                     break;
    //                 case PrivacySettings.CUSTOM:
    //                     try {
    //                         new PrivacyLocationUpdater(provider, listener, intent,
    //                                 Double.parseDouble(pSet.getLocationNetworkLat()),
    //                                 Double.parseDouble(pSet.getLocationNetworkLon())).start();
    //                         output = true;
    //                     } catch (NumberFormatException e) {
    //                         Log.e(TAG, "requestLocationUpdates: invalid coordinates");
    //                         output = true;
    //                     }
    //                     pSetMan.notification(packageName, uid, PrivacySettings.CUSTOM, PrivacySettings.DATA_LOCATION_NETWORK,
    //                             "Lat: " + pSet.getLocationNetworkLat() + " Lon: " + pSet.getLocationNetworkLon(), pSet);
    //                     break;
    //                 case PrivacySettings.RANDOM:
    //                     try {
    //                         new PrivacyLocationUpdater(provider, listener, intent,
    //                                 Double.parseDouble(pSet.getLocationNetworkLat()),
    //                                 Double.parseDouble(pSet.getLocationNetworkLon())).start();
    //                         output = true;
    //                     } catch (NumberFormatException e) {
    //                         Log.e(TAG, "requestLocationUpdates: invalid coordinates");
    //                         output = true;
    //                     }
    //                     pSetMan.notification(packageName, uid, PrivacySettings.RANDOM, PrivacySettings.DATA_LOCATION_NETWORK,
    //                             "Lat: " + pSet.getLocationNetworkLat() + " Lon: " + pSet.getLocationNetworkLon(), pSet);
    //                     break;
    //             }
    //         } else if (provider.equals(LocationManager.PASSIVE_PROVIDER)) { // could get location from any of above
    //             if (pSet.getLocationGpsSetting() == PrivacySettings.REAL &&
    //                     pSet.getLocationNetworkSetting() == PrivacySettings.REAL) {
    //                 output = false;
    //                 pSetMan.notification(packageName, uid, PrivacySettings.REAL, PrivacySettings.DATA_LOCATION_GPS, null, pSet);
    //             } else {
    //                 output = true;
    //                 pSetMan.notification(packageName, uid, PrivacySettings.EMPTY, PrivacySettings.DATA_LOCATION_GPS, null, pSet);
    //             }
    //         }
    //     } else {
    //         if (provider.equals(LocationManager.NETWORK_PROVIDER)) {
    //             pSetMan.notification(packageName, uid, PrivacySettings.REAL, PrivacySettings.DATA_LOCATION_NETWORK, null, pSet);
    //         } else { // including GPS and passive providers
    //             pSetMan.notification(packageName, uid, PrivacySettings.REAL, PrivacySettings.DATA_LOCATION_GPS, null, pSet);
    //         }
    //     }

    //     Log.d(TAG, "requestLocationUpdates - " + context.getPackageName() + " (" + Binder.getCallingUid() +
    //             ") output: " + (output == true ? "[custom location]" : "[real value]"));
    //     return output;
    // }
    return FALSE;
}

Boolean PrivacyLocationManager::RequestLocationUpdates(
    /* [in] */ ICriteria* criteria,
    /* [in] */ ILocationListener* listener,
    /* [in] */ IPendingIntent* intent)
{
    // if (criteria == null) return false;
    //     // treat providers with high accuracy as GPS providers
    // else if (criteria.getAccuracy() == Criteria.ACCURACY_FINE ||
    //         criteria.getBearingAccuracy() == Criteria.ACCURACY_HIGH ||
    //         criteria.getHorizontalAccuracy() == Criteria.ACCURACY_HIGH ||
    //         criteria.getVerticalAccuracy() == Criteria.ACCURACY_HIGH ||
    //         criteria.getSpeedAccuracy() == Criteria.ACCURACY_HIGH) {
    //     return requestLocationUpdates(LocationManager.GPS_PROVIDER, listener, intent);
    // } else { // treat all others as network providers
    //     return requestLocationUpdates(LocationManager.NETWORK_PROVIDER, listener, intent);
    // }
    return FALSE;
}
