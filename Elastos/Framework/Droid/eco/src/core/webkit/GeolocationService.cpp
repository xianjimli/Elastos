
#include "webkit/GeolocationService.h"
#include "location/CLocationManager.h"
#include "location/LocationProvider.h"

const CString GeolocationService::TAG("geolocationService");

GeolocationService::GeolocationService(
	/* [in] */ Int64 nativeObject)
{
#if 0
	mNativeObject = nativeObject;
    // Register newLocationAvailable with platform service.
    ActivityThread thread = ActivityThread.systemMain();
    Context context = thread.getApplication();
    mLocationManager = (LocationManager) context.getSystemService(Context.LOCATION_SERVICE);
    if (mLocationManager == null) {
        Log.e(TAG, "Could not get location manager.");
    }
#endif
}

/**
 * Start listening for location updates.
 */
CARAPI_(void) GeolocationService::Start()
{
	RegisterForLocationUpdates();
    mIsRunning = TRUE;
}

/**
 * Stop listening for location updates.
 */
CARAPI_(void) GeolocationService::Stop()
{
	UnregisterFromLocationUpdates();
    mIsRunning = FALSE;
}

/**
 * Sets whether to use the GPS.
 * @param enable Whether to use the GPS.
 */
CARAPI_(void) GeolocationService::SetEnableGps(
	/* [in] */ Boolean enable)
{
	if (mIsGpsEnabled != enable) {
        mIsGpsEnabled = enable;
        if (mIsRunning) {
            // There's no way to unregister from a single provider, so we can
            // only unregister from all, then reregister with all but the GPS.
            UnregisterFromLocationUpdates();
            RegisterForLocationUpdates();
        }
    }
}

/**
 * LocationListener implementation.
 * Called when the location has changed.
 * @param location The new location, as a Location object.
 */
ECode GeolocationService::OnLocationChanged(
	/* [in] */ ILocation* location)
{
    VALIDATE_NOT_NULL(location);

	// Callbacks from the system location sevice are queued to this thread, so it's possible
    // that we receive callbacks after unregistering. At this point, the native object will no
    // longer exist.
    if (mIsRunning) {
        NativeNewLocationAvailable(mNativeObject, location);
    }

    return NOERROR;
}

/**
 * LocationListener implementation.
 * Called when the provider status changes.
 * @param provider The name of the provider.
 * @param status The new status of the provider.
 * @param extras an optional Bundle with provider specific data.
 */
ECode GeolocationService::OnStatusChanged(
	/* [in] */ const String& providerName, 
	/* [in] */ Int32 status, 
	/* [in] */ IBundle* extras)
{
    VALIDATE_NOT_NULL(extras);

	Boolean isAvailable = (status == LocationProvider_AVAILABLE);
    if (providerName.Equals(LocationManager_NETWORK_PROVIDER)) {
        mIsNetworkProviderAvailable = isAvailable;
    } else if (providerName.Equals(LocationManager_GPS_PROVIDER)) {
        mIsGpsProviderAvailable = isAvailable;
    }

    MaybeReportError((String)"The last location provider is no longer available");

    return NOERROR;
}

/**
 * LocationListener implementation.
 * Called when the provider is enabled.
 * @param provider The name of the location provider that is now enabled.
 */
ECode GeolocationService::OnProviderEnabled(
	/* [in] */ const String& providerName)
{
	// No need to notify the native side. It's enough to start sending
    // valid position fixes again.
    if (providerName.Equals(LocationManager_NETWORK_PROVIDER)) {
        mIsNetworkProviderAvailable = TRUE;
    } else if (providerName.Equals(LocationManager_GPS_PROVIDER)) {
        mIsGpsProviderAvailable = TRUE;
    }

    return NOERROR;
}

/**
 * LocationListener implementation.
 * Called when the provider is disabled.
 * @param provider The name of the location provider that is now disabled.
 */
ECode GeolocationService::OnProviderDisabled(
	/* [in] */ const String& providerName)
{
	if (providerName.Equals(LocationManager_NETWORK_PROVIDER)) {
        mIsNetworkProviderAvailable = FALSE;
    } else if (providerName.Equals(LocationManager_GPS_PROVIDER)) {
        mIsGpsProviderAvailable = FALSE;
    }

    MaybeReportError((String)"The last location provider was disabled");

    return NOERROR;
}

/**
 * Registers this object with the location service.
 */
CARAPI_(void) GeolocationService::RegisterForLocationUpdates()
{
    mLocationManager->RequestLocationUpdates((String)LocationManager_NETWORK_PROVIDER, (Int64)0, (Float)0, this);
    mIsNetworkProviderAvailable = TRUE;
    if (mIsGpsEnabled) {
        mLocationManager->RequestLocationUpdates((String)LocationManager_GPS_PROVIDER, 0, 0, this);
        mIsGpsProviderAvailable = TRUE;
    }
}

/**
 * Unregisters this object from the location service.
 */
CARAPI_(void) GeolocationService::UnregisterFromLocationUpdates()
{
	mLocationManager->RemoveUpdates(this);
}

/**
 * Reports an error if neither the network nor the GPS provider is available.
 */
CARAPI_(void) GeolocationService::MaybeReportError(
	/* [in] */ const String& message)
{
	// Callbacks from the system location sevice are queued to this thread, so it's possible
    // that we receive callbacks after unregistering. At this point, the native object will no
    // longer exist.
    if (mIsRunning && !mIsNetworkProviderAvailable && !mIsGpsProviderAvailable) {
        NativeNewErrorAvailable(mNativeObject, message);
    }
}

// Native functions
CARAPI_(void) GeolocationService::NativeNewLocationAvailable(
	/* [in] */ Int64 nativeObject, 
	/* [in] */ ILocation* location)
{}

CARAPI_(void) GeolocationService::NativeNewErrorAvailable(
	/* [in] */ Int64 nativeObject, 
	/* [in] */ const String& message)
{}