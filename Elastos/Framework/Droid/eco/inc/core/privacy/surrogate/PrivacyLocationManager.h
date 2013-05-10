
#ifndef __PRIVACYLOCATIONMANAGER_H__
#define __PRIVACYLOCATIONMANAGER_H__

#include "location/LocationManager.h"
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>


using namespace Elastos::Core::Threading;

class PrivacyLocationManager
    : public ElRefBase
    , public LocationManager
    , public ILocalLocationManager
{
public:
    PrivacyLocationManager(
        /* [in] */ ILocationManager* service,
        /* [in] */ IContext* context);

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI GetAllProviders(
        /* [out] */ IObjectContainer** allProviders);

    CARAPI GetProviders(
        /* [in] */ Boolean enabledOnly,
        /* [out] */ IObjectContainer** providers);

    //@Override
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

    //@Override
    CARAPI RequestLocationUpdates(
        /* [in] */ const String& provider,
        /* [in] */ Int64 minTime,
        /* [in] */ Float minDistance,
        /* [in] */ ILocalLocationListener* listener);

    //@Override
    CARAPI RequestLocationUpdatesEx(
        /* [in] */ const String& provider,
        /* [in] */ Int64 minTime,
        /* [in] */ Float minDistance,
        /* [in] */ ILocalLocationListener* listener,
        /* [in] */ IApartment* apartment);

    //@Override
    CARAPI RequestLocationUpdatesEx2(
        /* [in] */ Int64 minTime,
        /* [in] */ Float minDistance,
        /* [in] */ ICriteria* criteria,
        /* [in] */ ILocalLocationListener* listener,
        /* [in] */ IApartment* apartment);

    //@Override
    CARAPI RequestLocationUpdatesPI(
        /* [in] */ const String& provider,
        /* [in] */ Int64 minTime,
        /* [in] */ Float minDistance,
        /* [in] */ IPendingIntent* intent);

    //@Override
    CARAPI RequestLocationUpdatesPIEx(
        /* [in] */ Int64 minTime,
        /* [in] */ Float minDistance,
        /* [in] */ ICriteria* criteria,
        /* [in] */ IPendingIntent* intent);

    //@Override
    CARAPI RequestSingleUpdate(
        /* [in] */ const String& provider,
        /* [in] */ ILocalLocationListener* listener,
        /* [in] */ IApartment* apartment);

    //@Override
    CARAPI RequestSingleUpdateEx(
        /* [in] */ ICriteria* criteria,
        /* [in] */ ILocalLocationListener* listener,
        /* [in] */ IApartment* apartment);

    //@Override
    CARAPI RequestSingleUpdatePI(
        /* [in] */ const String& provider,
        /* [in] */ IPendingIntent* intent);

    //@Override
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

    //@Override
    CARAPI IsProviderEnabled(
        /* [in] */ const String& provider,
        /* [out] */ Boolean* isEnabled);

    //@Override
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

    //@Override
    CARAPI AddNmeaListener(
        /* [in] */ ILocalGpsStatusNmeaListener* listener,
        /* [out] */ Boolean* result);

    CARAPI RemoveNmeaListener(
        /* [in] */ ILocalGpsStatusNmeaListener* listener);

    CARAPI GetGpsStatus(
        /* [in] */ IGpsStatus* inStatus,
        /* [out] */ IGpsStatus** outStatus);

    //@Override
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

private:
    /**
     * Handles calls to requestLocationUpdates and requestSingleUpdate methods
     * @return true, if action has been taken
     *         false, if the processing needs to be passed to the default method
     */
    CARAPI_(Boolean) RequestLocationUpdates(
        /* [in] */ const String& provider,
        /* [in] */ ILocationListener* listener,
        /* [in] */ IPendingIntent* intent);

    CARAPI_(Boolean) RequestLocationUpdates(
        /* [in] */ ICriteria* criteria,
        /* [in] */ ILocationListener* listener,
        /* [in] */ IPendingIntent* intent);

private:
    static const CString TAG;
    static const Int32 CUSTOM_LOCATION_UPDATE_COUNT = 5;

    AutoPtr<IContext> mContext;
    AutoPtr<IPrivacySettingsManager> mPSetMan;
    Mutex mLock;
};

#endif //__PRIVACYLOCATIONMANAGER_H__
