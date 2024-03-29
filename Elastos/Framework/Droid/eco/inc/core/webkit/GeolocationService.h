#ifndef __GEOLOCATIONSERVICE_H__
#define __GEOLOCATIONSERVICE_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>

/**
 * Implements the Java side of GeolocationServiceAndroid.
 */
class GeolocationService : public ILocalLocationListener 
{

public:
    /**
     * Constructor
     * @param nativeObject The native object to which this object will report position updates and
     *     errors.
     */
	GeolocationService(
		/* [in] */ Int64 nativeObject);

    /**
     * Start listening for location updates.
     */
	virtual CARAPI_(void) Start();

    /**
     * Stop listening for location updates.
     */
	virtual CARAPI_(void) Stop();

    /**
     * Sets whether to use the GPS.
     * @param enable Whether to use the GPS.
     */
	virtual CARAPI_(void) SetEnableGps(
		/* [in] */ Boolean enable);

    /**
     * LocationListener implementation.
     * Called when the location has changed.
     * @param location The new location, as a Location object.
     */
	virtual CARAPI OnLocationChanged(
		/* [in] */ ILocation* location);

    /**
     * LocationListener implementation.
     * Called when the provider status changes.
     * @param provider The name of the provider.
     * @param status The new status of the provider.
     * @param extras an optional Bundle with provider specific data.
     */
	virtual CARAPI OnStatusChanged(
		/* [in] */ const String& providerName, 
		/* [in] */ Int32 status, 
		/* [in] */ IBundle* extras);

    /**
     * LocationListener implementation.
     * Called when the provider is enabled.
     * @param provider The name of the location provider that is now enabled.
     */
	virtual CARAPI OnProviderEnabled(
		/* [in] */ const String& providerName);

    /**
     * LocationListener implementation.
     * Called when the provider is disabled.
     * @param provider The name of the location provider that is now disabled.
     */
	virtual CARAPI OnProviderDisabled(
		/* [in] */ const String& providerName);

private:
    /**
     * Registers this object with the location service.
     */
	CARAPI_(void) RegisterForLocationUpdates();

    /**
     * Unregisters this object from the location service.
     */
	CARAPI_(void) UnregisterFromLocationUpdates();

    /**
     * Reports an error if neither the network nor the GPS provider is available.
     */
	CARAPI_(void) MaybeReportError(
		/* [in] */ const String& message);

    // Native functions
	static CARAPI_(void) NativeNewLocationAvailable(
		/* [in] */ Int64 nativeObject, 
		/* [in] */ ILocation* location);

	static CARAPI_(void) NativeNewErrorAvailable(
		/* [in] */ Int64 nativeObject, 
		/* [in] */ const String& message);

private:
	// Log tag
    static const CString TAG;// = "geolocationService";

	Int64 mNativeObject;
	AutoPtr<ILocalLocationManager> mLocationManager;
	Boolean mIsGpsEnabled;
	Boolean mIsRunning;
	Boolean mIsNetworkProviderAvailable;
	Boolean mIsGpsProviderAvailable;

};

#endif //__GEOLOCATIONSERVICE_H__