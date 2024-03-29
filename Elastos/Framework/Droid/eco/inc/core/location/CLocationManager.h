
#ifndef __CLOCATIONMANAGER_H__
#define __CLOCATIONMANAGER_H__

#include "_CLocationManager.h"
#include "location/LocationManager.h"

CarClass(CLocationManager), public LocationManager
{
public:
    CARAPI constructor(
        /* [in] */ ILocationManager* service);

    CARAPI GetAllProviders(
        /* [out] */ IObjectContainer** allProviders);

    CARAPI GetProviders(
        /* [in] */ Boolean enabledOnly,
        /* [out] */ IObjectContainer** providers);

    CARAPI GetProvider(
        /* [in] */ const String& name,
        /* [out] */ ILocalLocationProvider** provider);

    CARAPI GetProvidersEx(
        /* [in] */ ICriteria* criteria,
        /* [in] */ Boolean enabledOnly,
        /* [out] */ IObjectContainer** providers);

    CARAPI GetBestProvider(
        /* [in] */ ICriteria* criteria,
        /* [in] */ Boolean enabledOnly,
        /* [out] */ String* provider);

    CARAPI RequestLocationUpdates(
        /* [in] */ const String& provider,
        /* [in] */ Int64 minTime,
        /* [in] */ Float minDistance,
        /* [in] */ ILocalLocationListener* listener);

    CARAPI RequestLocationUpdatesEx(
        /* [in] */ const String& provider,
        /* [in] */ Int64 minTime,
        /* [in] */ Float minDistance,
        /* [in] */ ILocalLocationListener* listener,
        /* [in] */ IApartment* apartment);

    CARAPI RequestLocationUpdatesEx2(
        /* [in] */ Int64 minTime,
        /* [in] */ Float minDistance,
        /* [in] */ ICriteria* criteria,
        /* [in] */ ILocalLocationListener* listener,
        /* [in] */ IApartment* apartment);

    CARAPI RequestLocationUpdatesPI(
        /* [in] */ const String& provider,
        /* [in] */ Int64 minTime,
        /* [in] */ Float minDistance,
        /* [in] */ IPendingIntent* intent);

    CARAPI RequestLocationUpdatesPIEx(
        /* [in] */ Int64 minTime,
        /* [in] */ Float minDistance,
        /* [in] */ ICriteria* criteria,
        /* [in] */ IPendingIntent* intent);

    CARAPI RequestSingleUpdate(
        /* [in] */ const String& provider,
        /* [in] */ ILocalLocationListener* listener,
        /* [in] */ IApartment* apartment);

    CARAPI RequestSingleUpdateEx(
        /* [in] */ ICriteria* criteria,
        /* [in] */ ILocalLocationListener* listener,
        /* [in] */ IApartment* apartment);

    CARAPI RequestSingleUpdatePI(
        /* [in] */ const String& provider,
        /* [in] */ IPendingIntent* intent);

    CARAPI RequestSingleUpdatePIEx(
        /* [in] */ ICriteria* criteria,
        /* [in] */ IPendingIntent* intent);

    CARAPI RemoveUpdates(
        /* [in] */ ILocalLocationListener* listener);

    CARAPI RemoveUpdatesPI(
        /* [in] */ IPendingIntent* intent);

    CARAPI AddProximityAlert(
        /* [in] */ Double latitude,
        /* [in] */ Double longitude,
        /* [in] */ Float distance,
        /* [in] */ Int64 expiration,
        /* [in] */ IPendingIntent* intent);

    CARAPI RemoveProximityAlert(
        /* [in] */ IPendingIntent* intent);

    CARAPI IsProviderEnabled(
        /* [in] */ const String& provider,
        /* [out] */ Boolean* isEnabled);

    CARAPI GetLastKnownLocation(
        /* [in] */ const String& provider,
        /* [out] */ ILocation** location);

    CARAPI AddTestProvider(
        /* [in] */ const String& name,
        /* [in] */ Boolean requiresNetwork,
        /* [in] */ Boolean requiresSatellite,
        /* [in] */ Boolean requiresCell,
        /* [in] */ Boolean hasMonetaryCost,
        /* [in] */ Boolean supportsAltitude,
        /* [in] */ Boolean supportsSpeed,
        /* [in] */ Boolean supportsBearing,
        /* [in] */ Int32 powerRequirement,
        /* [in] */ Int32 accuracy);

    CARAPI RemoveTestProvider(
        /* [in] */ const String& provider);

    CARAPI SetTestProviderLocation(
        /* [in] */ const String& provider,
        /* [in] */ ILocation* loc);

    CARAPI ClearTestProviderLocation(
        /* [in] */ const String& provider);

    CARAPI SetTestProviderEnabled(
        /* [in] */ const String& provider,
        /* [in] */ Boolean enabled);

    CARAPI ClearTestProviderEnabled(
        /* [in] */ const String& provider);

    CARAPI SetTestProviderStatus(
        /* [in] */ const String& provider,
        /* [in] */ Int32 status,
        /* [in] */ IBundle* extras,
        /* [in] */ Int64 updateTime);

    CARAPI ClearTestProviderStatus(
        /* [in] */ const String& provider);

    CARAPI AddGpsStatusListener(
        /* [in] */ ILocalGpsStatusListener* listener,
        /* [out] */ Boolean* result);

    CARAPI RemoveGpsStatusListener(
        /* [in] */ ILocalGpsStatusListener* listener);

    CARAPI AddNmeaListener(
        /* [in] */ ILocalGpsStatusNmeaListener* listener,
        /* [out] */ Boolean* result);

    CARAPI RemoveNmeaListener(
        /* [in] */ ILocalGpsStatusNmeaListener* listener);

    CARAPI GetGpsStatus(
        /* [in] */ IGpsStatus* inStatus,
        /* [out] */ IGpsStatus** outStatus);

    CARAPI SendExtraCommand(
        /* [in] */ const String& provider,
        /* [in] */ const String& command,
        /* [in] */ IBundle* inExtras, //inout
        /* [out] */ IBundle** outExtras,
        /* [out] */ Boolean* result);

    CARAPI SendNiResponse(
        /* [in] */ Int32 notifId,
        /* [in] */ Int32 userResponse,
        /* [out] */ Boolean* result);
};

#endif //__CLOCATIONMANAGER_H__
