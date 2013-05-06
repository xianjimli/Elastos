
#ifndef __CLOCATIONMANAGERSERVICE_H__
#define __CLOCATIONMANAGERSERVICE_H__

#include "ext/frameworkdef.h"
#include "_CLocationManagerService.h"
#include "server/location/MockProvider.h"
#include "server/location/LocationProviderProxy.h"
#include "server/location/GeocoderProxy.h"
#include "server/CProximityListener.h"
#include <elastos/AutoPtr.h>
#include <elastos/List.h>
#include <elastos/HashMap.h>
#include <elastos/HashSet.h>
#include <elastos/Mutex.h>
#include <elastos/ElRefBase.h>

_ELASTOS_NAMESPACE_BEGIN

template<> struct Hash<AutoPtr<IPendingIntent> >
{
    size_t operator()(AutoPtr<IPendingIntent> s) const
    {
        assert(s != NULL);
        return (size_t)s.Get();
    }
};

template<> struct Hash<AutoPtr<IInterface> >
{
    size_t operator()(AutoPtr<IInterface> s) const
    {
        assert(s != NULL);
        return (size_t)s.Get();
    }
};

_ELASTOS_NAMESPACE_END

CarClass(CLocationManagerService)
{
    friend class Receiver;
    friend class CProximityListener;

public:
    class ProximityAlert : public ElRefBase
    {
    public:
        struct HashKey
        {
            size_t operator()(const ProximityAlert* s) const
            {
                return (size_t)s->GetHashCode();
            }
        };

    public:
        ProximityAlert(
            /* [in] */ Int32 uid,
            /* [in] */ Double latitude,
            /* [in] */ Double longitude,
            /* [in] */ Float radius,
            /* [in] */ Int64 expiration,
            /* [in] */ IPendingIntent* intent);

        CARAPI_(Int64) GetExpiration();

        CARAPI_(AutoPtr<IPendingIntent>) GetIntent();

        CARAPI_(Boolean) IsInProximity(
            /* [in] */ Double latitude,
            /* [in] */ Double longitude,
            /* [in] */ Float accuracy);

        CARAPI_(String) ToString();

        CARAPI_(Int32) GetHashCode() const;

    public:
        Int32  mUid;
        Double mLatitude;
        Double mLongitude;
        Float mRadius;
        Int64 mExpiration;
        AutoPtr<IPendingIntent> mIntent;
        AutoPtr<ILocation> mLocation;
    };

private:
    class UpdateRecord;

    class Receiver : public ElRefBase
    {
    public:
        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        Receiver(
            /* [in] */ CLocationManagerService* host,
            /* [in] */ ILocationListener* listener);

        Receiver(
            /* [in] */ CLocationManagerService* host,
            /* [in] */ IPendingIntent* intent);

        CARAPI_(String) ToString();

        CARAPI_(Boolean) IsListener();

        CARAPI_(Boolean) IsPendingIntent();

        CARAPI GetListener(
            /* [out] */ ILocationListener** locationListener);

        CARAPI GetPendingIntent(
            /* [out] */ IPendingIntent** pendingIntent);

        CARAPI_(Boolean) CallStatusChangedLocked(
            /* [in] */ const String& provider,
            /* [in] */ Int32 status,
            /* [in] */ IBundle* extras);

        CARAPI_(Boolean) CallLocationChangedLocked(
            /* [in] */ ILocation* location);

        CARAPI_(Boolean) CallProviderEnabledLocked(
            /* [in] */ const String& provider,
            /* [in] */ Boolean enabled);

        CARAPI_(void) BinderDied();

        CARAPI_(void) OnSendFinished(
            /* [in] */ IPendingIntent* pendingIntent,
            /* [in] */ IIntent* intent,
            /* [in] */ Int32 resultCode,
            /* [in] */ const String& resultData,
            /* [in] */ IBundle* resultExtras);

        CARAPI_(void) IncrementPendingBroadcastsLocked();

        CARAPI_(void) DecrementPendingBroadcastsLocked();

    public:
        CLocationManagerService* mHost;
        AutoPtr<ILocationListener> mListener;
        AutoPtr<IPendingIntent> mPendingIntent;
        AutoPtr<IInterface> mKey;
        HashMap<String, AutoPtr<UpdateRecord> > mUpdateRecords;
        Int32 mPendingBroadcasts;
        Mutex mLock;
    };

//    class SettingsObserver//implements Observer
//    {
//    };

    class LpPowerComparator
        : public ElRefBase
        , public IComparator
    {
    public:
        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI Compare(
            /* [in] */ IInterface* object1,
            /* [in] */ IInterface* object2,
            /* [out] */ Int32* result);

        CARAPI Equals(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* isEqual);
    };

    class LpAccuracyComparator
        : public ElRefBase
        , public IComparator
    {
    public:
        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI Compare(
            /* [in] */ IInterface* object1,
            /* [in] */ IInterface* object2,
            /* [out] */ Int32* result);

        CARAPI Equals(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* isEqual);
    };

    class LpCapabilityComparator
        : public ElRefBase
        , public IComparator
    {
    private:
        static const Int32 ALTITUDE_SCORE = 4;
        static const Int32 BEARING_SCORE = 4;
        static const Int32 SPEED_SCORE = 4;

    public:
        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI Compare(
            /* [in] */ IInterface* object1,
            /* [in] */ IInterface* object2,
            /* [out] */ Int32* result);

        CARAPI Equals(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* isEqual);

    private:
        CARAPI_(Int32) Score(
            /* [in] */ ILocationProviderInterface* p);
    };

    /**
     * Note: must be constructed with lock held.
     */
    class UpdateRecord : public ElRefBase
    {
    public:
         /**
         * Note: must be constructed with lock held.
         */
        UpdateRecord(
            /* [in] */ CLocationManagerService* lms,
            /* [in] */ const String& provider,
            /* [in] */ Int64 minTime,
            /* [in] */ Float minDistance,
            /* [in] */ Boolean singleShot,
            /* [in] */ Receiver* receiver,
            /* [in] */ Int32 uid);

        /**
         * Method to be called when a record will no longer be used.  Calling this multiple times
         * must have the same effect as calling it once.
         */
        CARAPI_(void) DisposeLocked();

        CARAPI ToString(
            /* [out] */ String* str);

    public:
        CLocationManagerService* mHost;
        String mProvider;
        AutoPtr<Receiver> mReceiver;
        Int64 mMinTime;
        Float mMinDistance;
        Boolean mSingleShot;
        Int32 mUid;
        AutoPtr<ILocation> mLastFixBroadcast;
        Int64 mLastStatusBroadcast;
    };

public:
    CLocationManagerService();

    CARAPI constructor(
        /* [in] */ IContext* ctx);

    CARAPI LocationCallbackFinished(
        /* [in] */ ILocationListener* listener);

    /* package */
    CARAPI_(void) SystemReady();

    CARAPI Run();

    CARAPI GetAllProviders(
        /* [out] */ IObjectContainer** allProviders);

    CARAPI GetProviders(
        /* [in] */ ICriteria* criteria,
        /* [in] */ Boolean enabledOnly,
        /* [out] */ IObjectContainer** providers);

    CARAPI GetBestProvider(
        /* [in] */ ICriteria* criteria,
        /* [in] */ Boolean enabledOnly,
        /* [out] */ String* provider);

    CARAPI ProviderMeetsCriteria(
        /* [in] */ const String& provider,
        /* [in] */ ICriteria* criteria,
        /* [out] */ Boolean* result);

    CARAPI RequestLocationUpdates(
        /* [in] */ const String& provider,
        /* [in] */ ICriteria* criteria,
        /* [in] */ Int64 minTime,
        /* [in] */ Float minDistance,
        /* [in] */ Boolean singleShot,
        /* [in] */ ILocationListener* listener);

    CARAPI RequestLocationUpdatesPI(
        /* [in] */ const String& provider,
        /* [in] */ ICriteria* criteria,
        /* [in] */ Int64 minTime,
        /* [in] */ Float minDistance,
        /* [in] */ Boolean singleShot,
        /* [in] */ IPendingIntent* intent);

    CARAPI RemoveUpdates(
        /* [in] */ ILocationListener* listener);

    CARAPI RemoveUpdatesPI(
        /* [in] */ IPendingIntent* intent);

    CARAPI AddGpsStatusListener(
        /* [in] */ IGpsStatusListener* listener,
        /* [out] */ Boolean* result);

    CARAPI RemoveGpsStatusListener(
        /* [in] */ IGpsStatusListener* listener);

    CARAPI SendExtraCommand(
        /* [in] */ const String& provider,
        /* [in] */ const String& command,
        /* [in] */ IBundle* inExtras, //inout
        /* [out] */ IBundle** outExtras,
        /* [out] */ Boolean* result);

    // for NI support
    CARAPI SendNiResponse(
        /* [in] */ Int32 notifId,
        /* [in] */ Int32 userResponse,
        /* [out] */ Boolean* result);

    CARAPI AddProximityAlert(
        /* [in] */ Double latitude,
        /* [in] */ Double longitude,
        /* [in] */ Float radius,
        /* [in] */ Int64 expiration,
        /* [in] */ IPendingIntent* intent);

    CARAPI RemoveProximityAlert(
        /* [in] */ IPendingIntent* intent);

    CARAPI GetProviderInfo(
        /* [in] */ const String& provider,
        /* [out] */ IBundle** info);

    CARAPI IsProviderEnabled(
        /* [in] */ const String& provider,
        /* [out] */ Boolean* isEnabled);

    // Used by location providers to tell the location manager when it has a new location.
    // Passive is true if the location is coming from the passive provider, in which case
    // it need not be shared with other providers.
    CARAPI ReportLocation(
        /* [in] */ ILocation* location,
        /* [in] */ Boolean passive);

    CARAPI GetLastKnownLocation(
        /* [in] */ const String& provider,
        /* [out] */ ILocation** location);

    CARAPI GeocoderIsPresent(
        /* [out] */ Boolean* result);

    CARAPI GetFromLocation(
        /* [in] */ Double latitude,
        /* [in] */ Double longitude,
        /* [in] */ Int32 maxResults,
        /* [in] */ IGeocoderParams* params,
        /* [out] */ IObjectContainer** addrs,
        /* [out] */ String* result);

    CARAPI GetFromLocationName(
        /* [in] */ const String& locationName,
        /* [in] */ Double lowerLeftLatitude,
        /* [in] */ Double lowerLeftLongitude,
        /* [in] */ Double upperRightLatitude,
        /* [in] */ Double upperRightLongitude,
        /* [in] */ Int32 maxResults,
        /* [in] */ IGeocoderParams* params,
        /* [out] */ IObjectContainer** addrs,
        /* [out] */ String* result);

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

private:
    CARAPI_(void) AddProvider(
        /* [in] */ ILocationProviderInterface* provider);

    CARAPI_(void) RemoveProvider(
        /* [in] */ ILocationProviderInterface* provider);

    CARAPI_(void) LoadProviders();

    CARAPI_(void) LoadProvidersLocked();

    CARAPI_(void) _LoadProvidersLocked();

    CARAPI_(void) Initialize();

    CARAPI_(Boolean) IsAllowedBySettingsLocked(
        /* [in] */ const String& provider);

    CARAPI CheckPermissionsSafe(
        /* [in] */ const String& provider);

    CARAPI_(Boolean) IsAllowedProviderSafe(
        /* [in] */ const String& provider);

    CARAPI _GetAllProvidersLocked(
        /* [out] */ IObjectContainer** allProviders);

    CARAPI _GetProvidersLocked(
        /* [in] */ ICriteria* criteria,
        /* [in] */ Boolean enabledOnly,
        /* [out] */ IObjectContainer** providers);

    /**
     * Returns the next looser power requirement, in the sequence:
     *
     * POWER_LOW -> POWER_MEDIUM -> POWER_HIGH -> NO_REQUIREMENT
     */
    CARAPI_(Int32) NextPower(
        /* [in] */ Int32 power);

    /**
     * Returns the next looser accuracy requirement, in the sequence:
     *
     * ACCURACY_FINE -> ACCURACY_APPROXIMATE-> NO_REQUIREMENT
     */
    CARAPI_(Int32) NextAccuracy(
        /* [in] */ Int32 accuracy);

    CARAPI_(AutoPtr<ILocationProviderInterface>) Best(
        /* [in] */ IObjectContainer* providerNames);

    CARAPI_(void) UpdateProvidersLocked();

    CARAPI_(void) UpdateProviderListenersLocked(
        /* [in] */ const String& provider,
        /* [in] */ Boolean enabled);

    CARAPI_(Int64) GetMinTimeLocked(
        /* [in] */ const String& provider);

    CARAPI_(AutoPtr<Receiver>) GetReceiver(
        /* [in] */ ILocationListener* listener);

    CARAPI_(AutoPtr<Receiver>) GetReceiver(
        /* [in] */ IPendingIntent* intent);

    CARAPI_(Boolean) ProviderHasListener(
        /* [in] */ const String& provider,
        /* [in] */ Int32 uid,
        /* [in] */ Receiver* excludedReceiver);

    CARAPI RequestLocationUpdatesLocked(
        /* [in] */ const String& provider,
        /* [in] */ Int64 minTime,
        /* [in] */ Float minDistance,
        /* [in] */ Boolean singleShot,
        /* [in] */ Receiver* receiver);

    CARAPI_(void) RemoveUpdatesLocked(
        /* [in] */ Receiver* receiver);

    CARAPI AddProximityAlertLocked(
        /* [in] */ Double latitude,
        /* [in] */ Double longitude,
        /* [in] */ Float radius,
        /* [in] */ Int64 expiration,
        /* [in] */ IPendingIntent* intent);

    CARAPI RemoveProximityAlertLocked(
        /* [in] */ IPendingIntent* intent);

    CARAPI _GetProviderInfoLocked(
        /* [in] */ const String& provider,
        /* [out] */ IBundle** info);

    CARAPI _IsProviderEnabledLocked(
        /* [in] */ const String& provider,
        /* [out] */ Boolean* isEnabled);

    CARAPI _GetLastKnownLocationLocked(
        /* [in] */ const String& provider,
        /* [out] */ ILocation** location);

    static CARAPI_(Boolean) ShouldBroadcastSafe(
        /* [in] */ ILocation* loc,
        /* [in] */ ILocation* lastLoc,
        /* [in] */ UpdateRecord* record);

    CARAPI_(void) HandleLocationChangedLocked(
        /* [in] */ ILocation* location,
        /* [in] */ Boolean passive);

    CARAPI_(void) IncrementPendingBroadcasts();

    CARAPI_(void) DecrementPendingBroadcasts();

    // Mock Providers

    CARAPI CheckMockPermissionsSafe();

    CARAPI_(void) HandleLocationChanged(
        /* [in] */ ILocation* location,
        /* [in] */ Boolean passive);

private:
    static const CString TAG;
    static const Boolean LOCAL_LOGV = FALSE;
    static const CString ACCESS_FINE_LOCATION;
    static const CString ACCESS_COARSE_LOCATION;
    static const CString ACCESS_MOCK_LOCATION;
    static const CString ACCESS_LOCATION_EXTRA_COMMANDS;
    static const CString INSTALL_LOCATION_PROVIDER;

    // Handler messages
    static const Int32 MESSAGE_LOCATION_CHANGED = 1;
    static const Int32 MESSAGE_PACKAGE_UPDATED = 2;

    // The last time a location was written, by provider name.
    HashMap<String, Int64> mLastWriteTime;

    // Set of providers that are explicitly enabled
    HashSet<String> mEnabledProviders;

    // Set of providers that are explicitly disabled
    HashSet<String> mDisabledProviders;

    // Locations, status values, and extras for mock providers
    HashMap<String, AutoPtr<MockProvider> > mMockProviders;

    static Boolean sProvidersLoaded;

    AutoPtr<IContext> mContext;
    String mNetworkLocationProviderPackageName;
    String mGeocodeProviderPackageName;
    AutoPtr<GeocoderProxy> mGeocodeProvider;
    AutoPtr<IGpsStatusProvider> mGpsStatusProvider;
    AutoPtr<INetInitiatedListener> mNetInitiatedListener;
    AutoPtr<IApartment> mLocationHandler;

    // Cache the real providers for use in addTestProvider() and removeTestProvider()
    AutoPtr<LocationProviderProxy> mNetworkLocationProvider;
    AutoPtr<ILocationProviderInterface> mGpsLocationProvider;

    // wakelock variables
    static const CString WAKELOCK_KEY;
//    PowerManager.WakeLock mWakeLock = null;
    Int32 mPendingBroadcasts;

    /**
     * List of all receivers.
     */
    HashMap<AutoPtr<IInterface>, AutoPtr<Receiver> > mReceivers;


    /**
     * List of location providers.
     */
    List<AutoPtr<ILocationProviderInterface> > mProviders;
    HashMap<String, AutoPtr<ILocationProviderInterface> > mProvidersByName;

    /**
     * Object used internally for synchronization
     */
    Mutex mLock;

    /**
     * Mapping from provider name to all its UpdateRecords
     */
    HashMap<String, List< AutoPtr<UpdateRecord> >* > mRecordsByProvider;

    /**
     * Temporary filled in when computing min time for a provider.  Access is
     * protected by global lock mLock.
     */
    AutoPtr<IWorkSource> mTmpWorkSource;

    // Proximity listeners
    AutoPtr<Receiver> mProximityReceiver;
    AutoPtr<ILocationListener> mProximityListener;
    HashMap<AutoPtr<IPendingIntent>, AutoPtr<ProximityAlert> > mProximityAlerts;
    HashSet<AutoPtr<ProximityAlert>, ProximityAlert::HashKey> mProximitiesEntered;

    // Last known location for each provider
    HashMap<String, AutoPtr<ILocation> > mLastKnownLocation;

    Int32 mNetworkState;

    // for Settings change notification
//    ContentQueryMap mSettings;
//    PackageMonitor mPackageMonitor = new PackageMonitor();

    AutoPtr<IThread> mThread;
};

#endif //__CLOCATIONMANAGERSERVICE_H__
