
#ifndef __CLOCATIONMANAGER_H__
#define __CLOCATIONMANAGER_H__

#include "ext/frameworkdef.h"
#include "_CLocationManager.h"
#include "location/LocationProvider.h"
#include <elastos/AutoPtr.h>
#include <elastos/HashMap.h>
#include <elastos/List.h>
#include <elastos/ElRefBase.h>
#include <elastos/Mutex.h>

_ELASTOS_NAMESPACE_BEGIN

template<> struct Hash<AutoPtr<ILocalGpsStatusListener> >
{
    size_t operator()(AutoPtr<ILocalGpsStatusListener> s) const
    {
        assert(s != NULL);
        return (size_t)s.Get();
    }
};

template<> struct Hash<AutoPtr<ILocalGpsStatusNmeaListener> >
{
    size_t operator()(AutoPtr<ILocalGpsStatusNmeaListener> s) const
    {
        assert(s != NULL);
        return (size_t)s.Get();
    }
};

template<> struct Hash<AutoPtr<ILocalLocationListener> >
{
    size_t operator()(AutoPtr<ILocalLocationListener> s) const
    {
        assert(s != NULL);
        return (size_t)s.Get();
    }
};

_ELASTOS_NAMESPACE_END

using namespace Elastos::Core::Threading;

CarClass(CLocationManager)
{
private:
    static const CString TAG;

private:
    class ListenerTransport : public ElRefBase, ILocationListener
    {
    public:
        ListenerTransport(
            /* [in] */ ILocalLocationListener* listener,
            /* [in] */ IApartment* apartment);

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI OnLocationChanged(
            /* [in] */ ILocation* location);

        CARAPI_(void) HandleLocationChanged(
            /* [in] */ ILocation* location);

        CARAPI OnStatusChanged(
            /* [in] */ const String& provider,
            /* [in] */ Int32 status,
            /* [in] */ IBundle* extras);

        CARAPI_(void) HandleStatusChanged(
            /* [in] */ const String& provider,
            /* [in] */ Int32 status,
            /* [in] */ IBundle* extras);

        CARAPI OnProviderEnabled(
            /* [in] */ const String& provider);

        CARAPI_(void) HandleProviderEnabled(
            /* [in] */ const String& provider);

        CARAPI OnProviderDisabled(
            /* [in] */ const String& provider);

        CARAPI_(void) HandleProviderDisabled(
            /* [in] */ const String& provider);

    private:
        AutoPtr<ILocalLocationListener> mLocalListener;
        AutoPtr<IApartment> mListenerHandler;
    };

    // GPS-specific support

    // This class is used to send GPS status events to the client's main thread.
    class GpsStatusListenerTransport : public ElRefBase, IGpsStatusListener
    {
    private:
        class Nmea
        {
        public:
            Nmea(
                /* [in] */ Int64 timestamp,
                /* [in] */ const String& nmea);

        public:
            Int64 mTimestamp;
            String mNmea;
        };

    public:
        GpsStatusListenerTransport(
            /* [in] */ CLocationManager* locManager,
            /* [in] */ ILocalGpsStatusListener* listener);

        GpsStatusListenerTransport(
            /* [in] */ CLocationManager* locManager,
            /* [in] */ ILocalGpsStatusNmeaListener* listener);

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI OnGpsStarted();

        CARAPI OnGpsStopped();

        CARAPI OnFirstFix(
            /* [in] */ Int32 ttff);

        CARAPI OnSvStatusChanged(
            /* [in] */ Int32 svCount,
            /* [in] */ const ArrayOf<Int32>& prns,
            /* [in] */ const ArrayOf<Float>& snrs,
            /* [in] */ const ArrayOf<Float>& elevations,
            /* [in] */ const ArrayOf<Float>& azimuths,
            /* [in] */ Int32 ephemerisMask,
            /* [in] */ Int32 almanacMask,
            /* [in] */ Int32 usedInFixMask);

        CARAPI_(void) HandleGpsChanged(
            /* [in] */ Int32 event);

        CARAPI OnNmeaReceived(
            /* [in] */ Int64 timestamp,
            /* [in] */ const String& nmea);

        CARAPI_(void) HandleNmeaReceived();

    private:
        AutoPtr<CLocationManager> mLocationManager;
        AutoPtr<ILocalGpsStatusListener> mListener;
        AutoPtr<ILocalGpsStatusNmeaListener> mNmeaListener;
        List<Nmea*> mNmeaBuffer;
        Mutex mNmeaBufferLock;
        AutoPtr<IApartment> mGpsHandler;
    };

public:
    CLocationManager();

	/**
     * Returns a list of the names of all known location providers.  All
     * providers are returned, including ones that are not permitted to be
     * accessed by the calling activity or are currently disabled.
     *
     * @return list of Strings containing names of the providers
     */
    CARAPI GetAllProviders(
        /* [out] */ IObjectContainer** allProviders);

    /**
     * Returns a list of the names of location providers.  Only providers that
     * are permitted to be accessed by the calling activity will be returned.
     *
     * @param enabledOnly if true then only the providers which are currently
     * enabled are returned.
     * @return list of Strings containing names of the providers
     */
    CARAPI GetProviders(
        /* [in] */ Boolean enabledOnly,
        /* [out] */ IObjectContainer** providers);

    /**
     * Returns the information associated with the location provider of the
     * given name, or null if no provider exists by that name.
     *
     * @param name the provider name
     * @return a LocationProvider, or null
     *
     * @throws IllegalArgumentException if name is null
     * @throws SecurityException if the caller is not permitted to access the
     * given provider.
     */
    CARAPI GetProvider(
        /* [in] */ const String& name,
        /* [out] */ ILocalLocationProvider** provider);

    /**
     * Returns a list of the names of LocationProviders that satisfy the given
     * criteria, or null if none do.  Only providers that are permitted to be
     * accessed by the calling activity will be returned.
     *
     * @param criteria the criteria that the returned providers must match
     * @param enabledOnly if true then only the providers which are currently
     * enabled are returned.
     * @return list of Strings containing names of the providers
     */
    CARAPI GetProvidersEx(
        /* [in] */ ICriteria* criteria,
        /* [in] */ Boolean enabledOnly,
        /* [out] */ IObjectContainer** providers);

    /**
     * Returns the name of the provider that best meets the given criteria. Only providers
     * that are permitted to be accessed by the calling activity will be
     * returned.  If several providers meet the criteria, the one with the best
     * accuracy is returned.  If no provider meets the criteria,
     * the criteria are loosened in the following sequence:
     *
     * <ul>
     * <li> power requirement
     * <li> accuracy
     * <li> bearing
     * <li> speed
     * <li> altitude
     * </ul>
     *
     * <p> Note that the requirement on monetary cost is not removed
     * in this process.
     *
     * @param criteria the criteria that need to be matched
     * @param enabledOnly if true then only a provider that is currently enabled is returned
     * @return name of the provider that best matches the requirements
     */
    CARAPI GetBestProvider(
        /* [in] */ ICriteria* criteria,
        /* [in] */ Boolean enabledOnly,
        /* [out] */ String* provider);

    /**
     * Registers the current activity to be notified periodically by
     * the named provider.  Periodically, the supplied LocationListener will
     * be called with the current Location or with status updates.
     *
     * <p> It may take a while to receive the most recent location. If
     * an immediate location is required, applications may use the
     * {@link #getLastKnownLocation(String)} method.
     *
     * <p> In case the provider is disabled by the user, updates will stop,
     * and the {@link LocationListener#onProviderDisabled(String)}
     * method will be called. As soon as the provider is enabled again,
     * the {@link LocationListener#onProviderEnabled(String)} method will
     * be called and location updates will start again.
     *
     * <p> The frequency of notification may be controlled using the
     * minTime and minDistance parameters. If minTime is greater than 0,
     * the LocationManager could potentially rest for minTime milliseconds
     * between location updates to conserve power. If minDistance is greater than 0,
     * a location will only be broadcasted if the device moves by minDistance meters.
     * To obtain notifications as frequently as possible, set both parameters to 0.
     *
     * <p> Background services should be careful about setting a sufficiently high
     * minTime so that the device doesn't consume too much power by keeping the
     * GPS or wireless radios on all the time. In particular, values under 60000ms
     * are not recommended.
     *
     * <p> The calling thread must be a {@link android.os.Looper} thread such as
     * the main thread of the calling Activity.
     *
     * @param provider the name of the provider with which to register
     * @param minTime the minimum time interval for notifications, in
     * milliseconds. This field is only used as a hint to conserve power, and actual
     * time between location updates may be greater or lesser than this value.
     * @param minDistance the minimum distance interval for notifications,
     * in meters
     * @param listener a {#link LocationListener} whose
     * {@link LocationListener#onLocationChanged} method will be called for
     * each location update
     *
     * @throws IllegalArgumentException if provider or listener is null
     * @throws RuntimeException if the calling thread has no Looper
     * @throws SecurityException if no suitable permission is present for the provider.
     */
    CARAPI RequestLocationUpdates(
        /* [in] */ const String& provider,
        /* [in] */ Int64 minTime,
        /* [in] */ Float minDistance,
        /* [in] */ ILocalLocationListener* listener);

    /**
     * Registers the current activity to be notified periodically by
     * the named provider.  Periodically, the supplied LocationListener will
     * be called with the current Location or with status updates.
     *
     * <p> It may take a while to receive the most recent location. If
     * an immediate location is required, applications may use the
     * {@link #getLastKnownLocation(String)} method.
     *
     * <p> In case the provider is disabled by the user, updates will stop,
     * and the {@link LocationListener#onProviderDisabled(String)}
     * method will be called. As soon as the provider is enabled again,
     * the {@link LocationListener#onProviderEnabled(String)} method will
     * be called and location updates will start again.
     *
     * <p> The frequency of notification may be controlled using the
     * minTime and minDistance parameters. If minTime is greater than 0,
     * the LocationManager could potentially rest for minTime milliseconds
     * between location updates to conserve power. If minDistance is greater than 0,
     * a location will only be broadcasted if the device moves by minDistance meters.
     * To obtain notifications as frequently as possible, set both parameters to 0.
     *
     * <p> Background services should be careful about setting a sufficiently high
     * minTime so that the device doesn't consume too much power by keeping the
     * GPS or wireless radios on all the time. In particular, values under 60000ms
     * are not recommended.
     *
     * <p> The supplied Looper is used to implement the callback mechanism.
     *
     * @param provider the name of the provider with which to register
     * @param minTime the minimum time interval for notifications, in
     * milliseconds. This field is only used as a hint to conserve power, and actual
     * time between location updates may be greater or lesser than this value.
     * @param minDistance the minimum distance interval for notifications,
     * in meters
     * @param listener a {#link LocationListener} whose
     * {@link LocationListener#onLocationChanged} method will be called for
     * each location update
     * @param looper a Looper object whose message queue will be used to
     * implement the callback mechanism.
     * If looper is null then the callbacks will be called on the main thread.
     *
     * @throws IllegalArgumentException if provider is null or doesn't exist
     * @throws IllegalArgumentException if listener is null
     * @throws SecurityException if no suitable permission is present for the provider.
     */
    CARAPI RequestLocationUpdatesEx(
        /* [in] */ const String& provider,
        /* [in] */ Int64 minTime,
        /* [in] */ Float minDistance,
        /* [in] */ ILocalLocationListener* listener,
        /* [in] */ IApartment* apartment);

    /**
     * Registers the current activity to be notified periodically based on
     * the specified criteria.  Periodically, the supplied LocationListener will
     * be called with the current Location or with status updates.
     *
     * <p> It may take a while to receive the most recent location. If
     * an immediate location is required, applications may use the
     * {@link #getLastKnownLocation(String)} method.
     *
     * <p> In case the provider is disabled by the user, updates will stop,
     * and the {@link LocationListener#onProviderDisabled(String)}
     * method will be called. As soon as the provider is enabled again,
     * the {@link LocationListener#onProviderEnabled(String)} method will
     * be called and location updates will start again.
     *
     * <p> The frequency of notification may be controlled using the
     * minTime and minDistance parameters. If minTime is greater than 0,
     * the LocationManager could potentially rest for minTime milliseconds
     * between location updates to conserve power. If minDistance is greater than 0,
     * a location will only be broadcasted if the device moves by minDistance meters.
     * To obtain notifications as frequently as possible, set both parameters to 0.
     *
     * <p> Background services should be careful about setting a sufficiently high
     * minTime so that the device doesn't consume too much power by keeping the
     * GPS or wireless radios on all the time. In particular, values under 60000ms
     * are not recommended.
     *
     * <p> The supplied Looper is used to implement the callback mechanism.
     *
     * @param minTime the minimum time interval for notifications, in
     * milliseconds. This field is only used as a hint to conserve power, and actual
     * time between location updates may be greater or lesser than this value.
     * @param minDistance the minimum distance interval for notifications,
     * in meters
     * @param criteria contains parameters for the location manager to choose the
     * appropriate provider and parameters to compute the location
     * @param listener a {#link LocationListener} whose
     * {@link LocationListener#onLocationChanged} method will be called for
     * each location update
     * @param looper a Looper object whose message queue will be used to
     * implement the callback mechanism.
     * If looper is null then the callbacks will be called on the main thread.
     *
     * @throws IllegalArgumentException if criteria is null
     * @throws IllegalArgumentException if listener is null
     * @throws SecurityException if no suitable permission is present to access
     * the location services.
     */
    CARAPI RequestLocationUpdatesEx2(
        /* [in] */ Int64 minTime,
        /* [in] */ Float minDistance,
        /* [in] */ ICriteria* criteria,
        /* [in] */ ILocalLocationListener* listener,
        /* [in] */ IApartment* apartment);

    /**
     * Registers the current activity to be notified periodically by
     * the named provider.  Periodically, the supplied PendingIntent will
     * be broadcast with the current Location or with status updates.
     *
     * <p> Location updates are sent with a key of KEY_LOCATION_CHANGED and a Location value.
     *
     * <p> It may take a while to receive the most recent location. If
     * an immediate location is required, applications may use the
     * {@link #getLastKnownLocation(String)} method.
     *
     * <p> The frequency of notification or new locations may be controlled using the
     * minTime and minDistance parameters. If minTime is greater than 0,
     * the LocationManager could potentially rest for minTime milliseconds
     * between location updates to conserve power. If minDistance is greater than 0,
     * a location will only be broadcast if the device moves by minDistance meters.
     * To obtain notifications as frequently as possible, set both parameters to 0.
     *
     * <p> Background services should be careful about setting a sufficiently high
     * minTime so that the device doesn't consume too much power by keeping the
     * GPS or wireless radios on all the time. In particular, values under 60000ms
     * are not recommended.
     *
     * <p> In case the provider is disabled by the user, updates will stop,
     * and an intent will be sent with an extra with key KEY_PROVIDER_ENABLED and a boolean value
     * of false.  If the provider is re-enabled, an intent will be sent with an
     * extra with key KEY_PROVIDER_ENABLED and a boolean value of true and location updates will
     * start again.
     *
     * <p> If the provider's status changes, an intent will be sent with an extra with key
     * KEY_STATUS_CHANGED and an integer value indicating the new status.  Any extras associated
     * with the status update will be sent as well.
     *
     * @param provider the name of the provider with which to register
     * @param minTime the minimum time interval for notifications, in
     * milliseconds. This field is only used as a hint to conserve power, and actual
     * time between location updates may be greater or lesser than this value.
     * @param minDistance the minimum distance interval for notifications,
     * in meters
     * @param intent a {#link PendingIntent} to be sent for each location update
     *
     * @throws IllegalArgumentException if provider is null or doesn't exist
     * @throws IllegalArgumentException if intent is null
     * @throws SecurityException if no suitable permission is present for the provider.
     */
    CARAPI RequestLocationUpdatesPI(
        /* [in] */ const String& provider,
        /* [in] */ Int64 minTime,
        /* [in] */ Float minDistance,
        /* [in] */ IPendingIntent* intent);

    /**
     * Registers the current activity to be notified periodically based on
     * the specified criteria.  Periodically, the supplied PendingIntent will
     * be broadcast with the current Location or with status updates.
     *
     * <p> Location updates are sent with a key of KEY_LOCATION_CHANGED and a Location value.
     *
     * <p> It may take a while to receive the most recent location. If
     * an immediate location is required, applications may use the
     * {@link #getLastKnownLocation(String)} method.
     *
     * <p> The frequency of notification or new locations may be controlled using the
     * minTime and minDistance parameters. If minTime is greater than 0,
     * the LocationManager could potentially rest for minTime milliseconds
     * between location updates to conserve power. If minDistance is greater than 0,
     * a location will only be broadcast if the device moves by minDistance meters.
     * To obtain notifications as frequently as possible, set both parameters to 0.
     *
     * <p> Background services should be careful about setting a sufficiently high
     * minTime so that the device doesn't consume too much power by keeping the
     * GPS or wireless radios on all the time. In particular, values under 60000ms
     * are not recommended.
     *
     * <p> In case the provider is disabled by the user, updates will stop,
     * and an intent will be sent with an extra with key KEY_PROVIDER_ENABLED and a boolean value
     * of false.  If the provider is re-enabled, an intent will be sent with an
     * extra with key KEY_PROVIDER_ENABLED and a boolean value of true and location updates will
     * start again.
     *
     * <p> If the provider's status changes, an intent will be sent with an extra with key
     * KEY_STATUS_CHANGED and an integer value indicating the new status.  Any extras associated
     * with the status update will be sent as well.
     *
     * @param minTime the minimum time interval for notifications, in
     * milliseconds. This field is only used as a hint to conserve power, and actual
     * time between location updates may be greater or lesser than this value.
     * @param minDistance the minimum distance interval for notifications,
     * in meters
     * @param criteria contains parameters for the location manager to choose the
     * appropriate provider and parameters to compute the location
     * @param intent a {#link PendingIntent} to be sent for each location update
     *
     * @throws IllegalArgumentException if provider is null or doesn't exist
     * @throws IllegalArgumentException if intent is null
     * @throws SecurityException if no suitable permission is present for the provider.
     */
    CARAPI RequestLocationUpdatesPIEx(
        /* [in] */ Int64 minTime,
        /* [in] */ Float minDistance,
        /* [in] */ ICriteria* criteria,
        /* [in] */ IPendingIntent* intent);

    /**
     * Registers the current activity to be notified periodically by
     * the named provider.  Periodically, the supplied LocationListener will
     * be called with the current Location or with status updates.
     *
     * <p> It may take a while to receive the most recent location. If
     * an immediate location is required, applications may use the
     * {@link #getLastKnownLocation(String)} method.
     *
     * <p> In case the provider is disabled by the user, updates will stop,
     * and the {@link LocationListener#onProviderDisabled(String)}
     * method will be called. As soon as the provider is enabled again,
     * the {@link LocationListener#onProviderEnabled(String)} method will
     * be called and location updates will start again.
     *
     * <p> The supplied Looper is used to implement the callback mechanism.
     *
     * @param provider the name of the provider with which to register
     * @param listener a {#link LocationListener} whose
     * {@link LocationListener#onLocationChanged} method will be called for
     * each location update
     * @param looper a Looper object whose message queue will be used to
     * implement the callback mechanism.
     * If looper is null then the callbacks will be called on the main thread.
     *
     * @throws IllegalArgumentException if provider is null or doesn't exist
     * @throws IllegalArgumentException if listener is null
     * @throws SecurityException if no suitable permission is present for the provider.
     */
    CARAPI RequestSingleUpdate(
        /* [in] */ const String& provider,
        /* [in] */ ILocalLocationListener* listener,
        /* [in] */ IApartment* apartment);

    /**
     * Registers the current activity to be notified periodically based on
     * the specified criteria.  Periodically, the supplied LocationListener will
     * be called with the current Location or with status updates.
     *
     * <p> It may take a while to receive the most recent location. If
     * an immediate location is required, applications may use the
     * {@link #getLastKnownLocation(String)} method.
     *
     * <p> In case the provider is disabled by the user, updates will stop,
     * and the {@link LocationListener#onProviderDisabled(String)}
     * method will be called. As soon as the provider is enabled again,
     * the {@link LocationListener#onProviderEnabled(String)} method will
     * be called and location updates will start again.
     *
     * <p> The supplied Looper is used to implement the callback mechanism.
     *
     * @param criteria contains parameters for the location manager to choose the
     * appropriate provider and parameters to compute the location
     * @param listener a {#link LocationListener} whose
     * {@link LocationListener#onLocationChanged} method will be called for
     * each location update
     * @param looper a Looper object whose message queue will be used to
     * implement the callback mechanism.
     * If looper is null then the callbacks will be called on the current thread.
     *
     * @throws IllegalArgumentException if criteria is null
     * @throws IllegalArgumentException if listener is null
     * @throws SecurityException if no suitable permission is present to access
     * the location services.
     */
    CARAPI RequestSingleUpdateEx(
        /* [in] */ ICriteria* criteria,
        /* [in] */ ILocalLocationListener* listener,
        /* [in] */ IApartment* apartment);

    /**
     * Registers the current activity to be notified periodically by
     * the named provider.  Periodically, the supplied PendingIntent will
     * be broadcast with the current Location or with status updates.
     *
     * <p> Location updates are sent with a key of KEY_LOCATION_CHANGED and a Location value.
     *
     * <p> It may take a while to receive the most recent location. If
     * an immediate location is required, applications may use the
     * {@link #getLastKnownLocation(String)} method.
     *
     * @param provider the name of the provider with which to register
     * @param intent a {#link PendingIntent} to be sent for the location update
     *
     * @throws IllegalArgumentException if provider is null or doesn't exist
     * @throws IllegalArgumentException if intent is null
     * @throws SecurityException if no suitable permission is present for the provider.
     */
    CARAPI RequestSingleUpdatePI(
        /* [in] */ const String& provider,
        /* [in] */ IPendingIntent* intent);

    /**
     * Registers the current activity to be notified periodically based on
     * the specified criteria.  Periodically, the supplied PendingIntent will
     * be broadcast with the current Location or with status updates.
     *
     * <p> Location updates are sent with a key of KEY_LOCATION_CHANGED and a Location value.
     *
     * <p> It may take a while to receive the most recent location. If
     * an immediate location is required, applications may use the
     * {@link #getLastKnownLocation(String)} method.
     *
     * @param criteria contains parameters for the location manager to choose the
     * appropriate provider and parameters to compute the location
     * @param intent a {#link PendingIntent} to be sent for the location update
     *
     * @throws IllegalArgumentException if provider is null or doesn't exist
     * @throws IllegalArgumentException if intent is null
     * @throws SecurityException if no suitable permission is present for the provider.
     */
    CARAPI RequestSingleUpdatePIEx(
        /* [in] */ ICriteria* criteria,
        /* [in] */ IPendingIntent* intent);

    /**
     * Removes any current registration for location updates of the current activity
     * with the given LocationListener.  Following this call, updates will no longer
     * occur for this listener.
     *
     * @param listener {#link LocationListener} object that no longer needs location updates
     * @throws IllegalArgumentException if listener is null
     */
    CARAPI RemoveUpdates(
        /* [in] */ ILocalLocationListener* listener);

    /**
     * Removes any current registration for location updates of the current activity
     * with the given PendingIntent.  Following this call, updates will no longer
     * occur for this intent.
     *
     * @param intent {#link PendingIntent} object that no longer needs location updates
     * @throws IllegalArgumentException if intent is null
     */
    CARAPI RemoveUpdatesPI(
        /* [in] */ IPendingIntent* intent);

    /**
     * Sets a proximity alert for the location given by the position
     * (latitude, longitude) and the given radius.  When the device
     * detects that it has entered or exited the area surrounding the
     * location, the given PendingIntent will be used to create an Intent
     * to be fired.
     *
     * <p> The fired Intent will have a boolean extra added with key
     * {@link #KEY_PROXIMITY_ENTERING}. If the value is true, the device is
     * entering the proximity region; if false, it is exiting.
     *
     * <p> Due to the approximate nature of position estimation, if the
     * device passes through the given area briefly, it is possible
     * that no Intent will be fired.  Similarly, an Intent could be
     * fired if the device passes very close to the given area but
     * does not actually enter it.
     *
     * <p> After the number of milliseconds given by the expiration
     * parameter, the location manager will delete this proximity
     * alert and no longer monitor it.  A value of -1 indicates that
     * there should be no expiration time.
     *
     * <p> In case the screen goes to sleep, checks for proximity alerts
     * happen only once every 4 minutes. This conserves battery life by
     * ensuring that the device isn't perpetually awake.
     *
     * <p> Internally, this method uses both {@link #NETWORK_PROVIDER}
     * and {@link #GPS_PROVIDER}.
     *
     * @param latitude the latitude of the central point of the
     * alert region
     * @param longitude the longitude of the central point of the
     * alert region
     * @param radius the radius of the central point of the
     * alert region, in meters
     * @param expiration time for this proximity alert, in milliseconds,
     * or -1 to indicate no expiration
     * @param intent a PendingIntent that will be used to generate an Intent to
     * fire when entry to or exit from the alert region is detected
     *
     * @throws SecurityException if no permission exists for the required
     * providers.
     */
    CARAPI AddProximityAlert(
        /* [in] */ Double latitude,
        /* [in] */ Double longitude,
        /* [in] */ Float distance,
        /* [in] */ Int64 expiration,
        /* [in] */ IPendingIntent* intent);

    /**
     * Removes the proximity alert with the given PendingIntent.
     *
     * @param intent the PendingIntent that no longer needs to be notified of
     * proximity alerts
     */
    CARAPI RemoveProximityAlert(
        /* [in] */ IPendingIntent* intent);

    /**
     * Returns the current enabled/disabled status of the given provider. If the
     * user has enabled this provider in the Settings menu, true is returned
     * otherwise false is returned
     *
     * @param provider the name of the provider
     * @return true if the provider is enabled
     *
     * @throws SecurityException if no suitable permission is present for the provider.
     * @throws IllegalArgumentException if provider is null
     */
    CARAPI IsProviderEnabled(
        /* [in] */ const String& provider,
        /* [out] */ Boolean* isEnabled);

    /**
     * Returns a Location indicating the data from the last known
     * location fix obtained from the given provider.  This can be done
     * without starting the provider.  Note that this location could
     * be out-of-date, for example if the device was turned off and
     * moved to another location.
     *
     * <p> If the provider is currently disabled, null is returned.
     *
     * @param provider the name of the provider
     * @return the last known location for the provider, or null
     *
     * @throws SecurityException if no suitable permission is present for the provider.
     * @throws IllegalArgumentException if provider is null or doesn't exist
     */
    CARAPI GetLastKnownLocation(
        /* [in] */ const String& provider,
        /* [out] */ ILocation** location);

    /**
     * Creates a mock location provider and adds it to the set of active providers.
     *
     * @param name the provider name
     * @param requiresNetwork
     * @param requiresSatellite
     * @param requiresCell
     * @param hasMonetaryCost
     * @param supportsAltitude
     * @param supportsSpeed
     * @param supportsBearing
     * @param powerRequirement
     * @param accuracy
     *
     * @throws SecurityException if the ACCESS_MOCK_LOCATION permission is not present
     * or the {@link android.provider.Settings.Secure#ALLOW_MOCK_LOCATION
     * Settings.Secure.ALLOW_MOCK_LOCATION} system setting is not enabled
     * @throws IllegalArgumentException if a provider with the given name already exists
     */
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

    /**
     * Removes the mock location provider with the given name.
     *
     * @param provider the provider name
     *
     * @throws SecurityException if the ACCESS_MOCK_LOCATION permission is not present
     * or the {@link android.provider.Settings.Secure#ALLOW_MOCK_LOCATION
     * Settings.Secure.ALLOW_MOCK_LOCATION}} system setting is not enabled
     * @throws IllegalArgumentException if no provider with the given name exists
     */
    CARAPI RemoveTestProvider(
        /* [in] */ const String& provider);

    /**
     * Sets a mock location for the given provider.  This location will be used in place
     * of any actual location from the provider.
     *
     * @param provider the provider name
     * @param loc the mock location
     *
     * @throws SecurityException if the ACCESS_MOCK_LOCATION permission is not present
     * or the {@link android.provider.Settings.Secure#ALLOW_MOCK_LOCATION
     * Settings.Secure.ALLOW_MOCK_LOCATION}} system setting is not enabled
     * @throws IllegalArgumentException if no provider with the given name exists
     */
    CARAPI SetTestProviderLocation(
        /* [in] */ const String& provider,
        /* [in] */ ILocation* loc);

    /**
     * Removes any mock location associated with the given provider.
     *
     * @param provider the provider name
     *
     * @throws SecurityException if the ACCESS_MOCK_LOCATION permission is not present
     * or the {@link android.provider.Settings.Secure#ALLOW_MOCK_LOCATION
     * Settings.Secure.ALLOW_MOCK_LOCATION}} system setting is not enabled
     * @throws IllegalArgumentException if no provider with the given name exists
     */
    CARAPI ClearTestProviderLocation(
        /* [in] */ const String& provider);

    /**
     * Sets a mock enabled value for the given provider.  This value will be used in place
     * of any actual value from the provider.
     *
     * @param provider the provider name
     * @param enabled the mock enabled value
     *
     * @throws SecurityException if the ACCESS_MOCK_LOCATION permission is not present
     * or the {@link android.provider.Settings.Secure#ALLOW_MOCK_LOCATION
     * Settings.Secure.ALLOW_MOCK_LOCATION}} system setting is not enabled
     * @throws IllegalArgumentException if no provider with the given name exists
     */
    CARAPI SetTestProviderEnabled(
        /* [in] */ const String& provider,
        /* [in] */ Boolean enabled);

    /**
     * Removes any mock enabled value associated with the given provider.
     *
     * @param provider the provider name
     *
     * @throws SecurityException if the ACCESS_MOCK_LOCATION permission is not present
     * or the {@link android.provider.Settings.Secure#ALLOW_MOCK_LOCATION
     * Settings.Secure.ALLOW_MOCK_LOCATION}} system setting is not enabled
     * @throws IllegalArgumentException if no provider with the given name exists
     */
    CARAPI ClearTestProviderEnabled(
        /* [in] */ const String& provider);

    /**
     * Sets mock status values for the given provider.  These values will be used in place
     * of any actual values from the provider.
     *
     * @param provider the provider name
     * @param status the mock status
     * @param extras a Bundle containing mock extras
     * @param updateTime the mock update time
     *
     * @throws SecurityException if the ACCESS_MOCK_LOCATION permission is not present
     * or the {@link android.provider.Settings.Secure#ALLOW_MOCK_LOCATION
     * Settings.Secure.ALLOW_MOCK_LOCATION}} system setting is not enabled
     * @throws IllegalArgumentException if no provider with the given name exists
     */
    CARAPI SetTestProviderStatus(
        /* [in] */ const String& provider,
        /* [in] */ Int32 status,
        /* [in] */ IBundle* extras,
        /* [in] */ Int64 updateTime);

    /**
     * Removes any mock status values associated with the given provider.
     *
     * @param provider the provider name
     *
     * @throws SecurityException if the ACCESS_MOCK_LOCATION permission is not present
     * or the {@link android.provider.Settings.Secure#ALLOW_MOCK_LOCATION
     * Settings.Secure.ALLOW_MOCK_LOCATION}} system setting is not enabled
     * @throws IllegalArgumentException if no provider with the given name exists
     */
    CARAPI ClearTestProviderStatus(
        /* [in] */ const String& provider);

    /**
     * Adds a GPS status listener.
     *
     * @param listener GPS status listener object to register
     *
     * @return true if the listener was successfully added
     *
     * @throws SecurityException if the ACCESS_FINE_LOCATION permission is not present
     */
    CARAPI AddGpsStatusListener(
        /* [in] */ ILocalGpsStatusListener* listener,
        /* [out] */ Boolean* result);

    /**
     * Removes a GPS status listener.
     *
     * @param listener GPS status listener object to remove
     */
    CARAPI RemoveGpsStatusListener(
        /* [in] */ ILocalGpsStatusListener* listener);

    /**
     * Adds an NMEA listener.
     *
     * @param listener a {#link GpsStatus.NmeaListener} object to register
     *
     * @return true if the listener was successfully added
     *
     * @throws SecurityException if the ACCESS_FINE_LOCATION permission is not present
     */
    CARAPI AddNmeaListener(
        /* [in] */ ILocalGpsStatusNmeaListener* listener,
        /* [out] */ Boolean* result);

    /**
     * Removes an NMEA listener.
     *
     * @param listener a {#link GpsStatus.NmeaListener} object to remove
     */
    CARAPI RemoveNmeaListener(
        /* [in] */ ILocalGpsStatusNmeaListener* listener);

    /**
     * Retrieves information about the current status of the GPS engine.
     * This should only be called from the {@link GpsStatus.Listener#onGpsStatusChanged}
     * callback to ensure that the data is copied atomically.
     *
     * The caller may either pass in a {@link GpsStatus} object to set with the latest
     * status information, or pass null to create a new {@link GpsStatus} object.
     *
     * @param status object containing GPS status details, or null.
     * @return status object containing updated GPS status.
     */
    CARAPI GetGpsStatus(
        /* [in] */ IGpsStatus* inStatus,
        /* [out] */ IGpsStatus** outStatus);

    /**
     * Sends additional commands to a location provider.
     * Can be used to support provider specific extensions to the Location Manager API
     *
     * @param provider name of the location provider.
     * @param command name of the command to send to the provider.
     * @param extras optional arguments for the command (or null).
     * The provider may optionally fill the extras Bundle with results from the command.
     *
     * @return true if the command succeeds.
     */
    CARAPI SendExtraCommand(
        /* [in] */ const String& provider,
        /* [in] */ const String& command,
        /* [in] */ IBundle* inExtras, //inout
        /* [out] */ IBundle** outExtras,
        /* [out] */ Boolean* result);

    /**
     * Used by NetInitiatedActivity to report user response
     * for network initiated GPS fix requests.
     *
     * {@hide}
     */
    CARAPI SendNiResponse(
        /* [in] */ Int32 notifId,
        /* [in] */ Int32 userResponse,
        /* [out] */ Boolean* result);

    CARAPI GetDescription(
        /* [out] */ String* str);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor(
        /* [in] */ ILocationManager* service);

private:
    CARAPI_(AutoPtr<ILocalLocationProvider>) CreateProvider(
        /* [in] */ const String& name,
        /* [in] */ IBundle* info);

    CARAPI_(void) _RequestLocationUpdates(
        /* [in] */ const String& provider,
        /* [in] */ ICriteria* criteria,
        /* [in] */ Int64 minTime,
        /* [in] */ Float minDistance,
        /* [in] */ Boolean singleShot,
        /* [in] */ ILocalLocationListener* listener,
        /* [in] */ IApartment* apartment);

    CARAPI_(void) _RequestLocationUpdatesPI(
        /* [in] */ const String& provider,
        /* [in] */ ICriteria* criteria,
        /* [in] */ Int64 minTime,
        /* [in] */ Float minDistance,
        /* [in] */ Boolean singleShot,
        /* [in] */ IPendingIntent* intent);

private:
    AutoPtr<ILocationManager> mService;
    HashMap<AutoPtr<ILocalGpsStatusListener>, AutoPtr<GpsStatusListenerTransport> > mGpsStatusListeners;
    HashMap<AutoPtr<ILocalGpsStatusNmeaListener>, AutoPtr<GpsStatusListenerTransport> > mNmeaListeners;
    AutoPtr<IGpsStatus> mGpsStatus;;
    Mutex mGpsStatusLock;

    // Map from LocationListeners to their associated ListenerTransport objects
    HashMap<AutoPtr<ILocalLocationListener>, AutoPtr<ListenerTransport> > mListeners;
    Mutex mListenersLock;
};

#endif // __CLOCATIONMANAGER_H__

