
#ifndef __CLOCATIONMANAGERSERVICE_H__
#define __CLOCATIONMANAGERSERVICE_H__

#include "ext/frameworkdef.h"
#include <elastos/ElRefBase.h>
#include "_CLocationManagerService.h"
#include "server/location/MockProvider.h"
#include "server/location/LocationProviderProxy.h"
#include "server/location/GeocoderProxy.h"
#include <elastos/AutoPtr.h>
#include <elastos/List.h>
#include <elastos/HashMap.h>
#include <elastos/HashSet.h>
#include <elastos/Mutex.h>

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

    // wakelock variables
    static const CString WAKELOCK_KEY;

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
            /* [in] */ CLocationManagerService* locationMS,
            /* [in] */ ILocationListener* listener);

        Receiver(
            /* [in] */ CLocationManagerService* locationMS,
            /* [in] */ IPendingIntent* intent);

        CARAPI_(String) ToString();

        CARAPI_(Boolean) IsListener();

        CARAPI_(Boolean) IsPendingIntent();

        CARAPI GetListener(
            /* [out] */ ILocationListener** locationListener);

        CARAPI GetPendingIntent(
            /* [out] */ IPendingIntent** pendingIntent);

        CARAPI_(Boolean) CallStatusChangedLocked(
            /* [in] */ String provider,
            /* [in] */ Int32 status,
            /* [in] */ IBundle* extras);

        CARAPI_(Boolean) CallLocationChangedLocked(
            /* [in] */ ILocation* location);

        CARAPI_(Boolean) CallProviderEnabledLocked(
            /* [in] */ String provider,
            /* [in] */ Boolean enabled);

        CARAPI_(void) BinderDied();

        CARAPI_(void) OnSendFinished(
            /* [in] */ IPendingIntent* pendingIntent,
            /* [in] */ IIntent* intent,
            /* [in] */ Int32 resultCode,
            /* [in] */ String resultData,
            /* [in] */ IBundle* resultExtras);

        CARAPI_(void) IncrementPendingBroadcastsLocked();

        CARAPI_(void) DecrementPendingBroadcastsLocked();

    public:
        AutoPtr<CLocationManagerService> mLocationMS;
        AutoPtr<ILocationListener> mListener;
        AutoPtr<IPendingIntent> mPendingIntent;
//        Object mKey;
        HashMap<String, UpdateRecord*> mUpdateRecords;
        Int32 mPendingBroadcasts;
        Mutex mSync;
    };

    /**
     * Note: must be constructed with lock held.
     */
    class UpdateRecord
    {
    public:
         /**
         * Note: must be constructed with lock held.
         */
        UpdateRecord(
            /* [in] */ CLocationManagerService* lms,
            /* [in] */ String provider,
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
        CLocationManagerService* mLocationManagerService;
        String mProvider;
        AutoPtr<Receiver> mReceiver;
        Int64 mMinTime;
        Float mMinDistance;
        Boolean mSingleShot;
        Int32 mUid;
        AutoPtr<ILocation> mLastFixBroadcast;
        Int64 mLastStatusBroadcast;
    };

    class LpPowerComparator : public ElRefBase,  IComparator
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

    class LpAccuracyComparator : public ElRefBase,  IComparator
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

    class LpCapabilityComparator : public ElRefBase,  IComparator
    {
    private:
        static const Int32 ALTITUDE_SCORE = 4;
        static const Int32 BEARING_SCORE = 4;
        static const Int32 SPEED_SCORE = 4;

    private:
        CARAPI_(Int32) Score(
            /* [in] */ LocationProviderInterface* p);

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

//    class SettingsObserver//implements Observer
//    {
//    };

    class LocationWorkerHandler : public ElRefBase, IApartment
    {
    public:
        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI Start(
            /* [in] */ ApartmentAttr attr);

        CARAPI Finish();

        CARAPI PostCppCallback(
            /* [in] */ Handle32 target,
            /* [in] */ Handle32 func,
            /* [in] */ IParcel* params,
            /* [in] */ Int32 id);

        CARAPI PostCppCallbackAtTime(
            /* [in] */ Handle32 target,
            /* [in] */ Handle32 func,
            /* [in] */ IParcel* params,
            /* [in] */ Int32 id,
            /* [in] */ Millisecond64 uptimeMillis);

        CARAPI PostCppCallbackDelayed(
            /* [in] */ Handle32 target,
            /* [in] */ Handle32 func,
            /* [in] */ IParcel* params,
            /* [in] */ Int32 id,
            /* [in] */ Millisecond64 delayMillis);

        CARAPI PostCppCallbackAtFrontOfQueue(
            /* [in] */ Handle32 target,
            /* [in] */ Handle32 func,
            /* [in] */ IParcel* params,
            /* [in] */ Int32 id);

        CARAPI RemoveCppCallbacks(
            /* [in] */ Handle32 target,
            /* [in] */ Handle32 func);

        CARAPI RemoveCppCallbacksEx(
            /* [in] */ Handle32 target,
            /* [in] */ Handle32 func,
            /* [in] */ Int32 id);

        CARAPI HasCppCallbacks(
            /* [in] */ Handle32 target,
            /* [in] */ Handle32 func,
            /* [out] */ Boolean* result);

        CARAPI HasCppCallbacksEx(
            /* [in] */ Handle32 target,
            /* [in] */ Handle32 func,
            /* [in] */ Int32 id,
            /* [out] */ Boolean* result);
    };

public:
    class ProximityAlert : public ElRefBase
    {
    public:
        ProximityAlert(
            /* [in] */ Int32 uid,
            /* [in] */ Double latitude,
            /* [in] */ Double longitude,
            /* [in] */ Float radius,
            /* [in] */ Int64 expiration,
            /* [in] */ IPendingIntent* intent);

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI_(Int64) GetExpiration();

        CARAPI_(AutoPtr<IPendingIntent>) GetIntent();

        CARAPI_(Boolean) IsInProximity(
        	/* [in] */ Double latitude,
        	/* [in] */ Double longitude,
        	/* [in] */ Float accuracy);

        CARAPI_(String) ToString();

        struct HashKey
        {
            size_t operator()(const ProximityAlert* s) const
            {
                return (size_t)s->GetHashCode();
            }
        };

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

    // Listener for receiving locations to trigger proximity alerts
    class ProximityListener : public ElRefBase, ILocationListener
	{
	public:
    	ProximityListener(
    	    /* [in] */ CLocationManagerService* locMS);

    	CARAPI_(PInterface) Probe(
			/* [in]  */ REIID riid);

		CARAPI_(UInt32) AddRef();

		CARAPI_(UInt32) Release();

		CARAPI GetInterfaceID(
			/* [in] */ IInterface *pObject,
			/* [out] */ InterfaceID *pIID);

		CARAPI OnLocationChanged(
			/* [in] */ ILocation* loc);

		CARAPI OnProviderEnabled(
			/* [in] */ const String& provider);

		CARAPI OnProviderDisabled(
			/* [in] */ const String& provider);

		CARAPI OnStatusChanged(
			/* [in] */ const String& provider,
			/* [in] */ Int32 status,
			/* [in] */ IBundle* extras);

//		CARAPI OnSendFinished(
//			/* [in] */ IPendingIntent* pendingIntent,
//			/* [in] */ IIntent* intent,
//		    /* [in] */ Int32 resultCode,
//		    /* [in] */ String resultData,
//		    /* [in] */ IBundle* resultExtras);

	public:
		CLocationManagerService* mLocationManagerService;
		Boolean mIsGpsAvailable;
		Mutex mSync;
	};

public:
    CLocationManagerService();

    CARAPI constructor(
        /* [in] */ IContext* ctx);

    CARAPI_(void) SystemReady();

    CARAPI Run();

    CARAPI GetAllProviders(
        /* [out] */ ArrayOf<String>* allProviders);

    CARAPI GetProviders(
        /* [in] */ ICriteria* criteria,
        /* [in] */ Boolean enabledOnly,
        /* [out] */ ArrayOf<String>* providers);

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

    // for reporting callback completion
    CARAPI LocationCallbackFinished(
        /* [in] */ ILocationListener* listener);

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
    static void* EntryRoutine(void *arg);

    static CARAPI_(Boolean) ShouldBroadcastSafe(
    	/* [in] */ ILocation* loc,
    	/* [in] */ ILocation* lastLoc,
    	/* [in] */ UpdateRecord* record);

private:
    CARAPI_(void) AddProvider(
        /* [in] */ LocationProviderInterface* provider);

    CARAPI_(void) RemoveProvider(
        /* [in] */ LocationProviderInterface* provider);

    CARAPI_(void) LoadProviders();

    CARAPI_(void) LoadProvidersLocked();

    CARAPI_(void) _LoadProvidersLocked();

    CARAPI_(void) Initialize();

    CARAPI_(Boolean) IsAllowedBySettingsLocked(
        /* [in] */ String provider);

    CARAPI CheckPermissionsSafe(
        /* [in] */ String provider);

    CARAPI_(Boolean) IsAllowedProviderSafe(
        /* [in] */ String provider);

    CARAPI_(ArrayOf<String>*) _GetAllProvidersLocked();

    CARAPI_(ArrayOf<String>*) _GetProvidersLocked(
        /* [in] */ ICriteria* criteria,
        /* [in] */ Boolean enabledOnly);

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

    CARAPI_(AutoPtr<LocationProviderInterface>) Best(
        /* [in] */ const ArrayOf<String>& providerNames);

    CARAPI_(void) UpdateProvidersLocked();

    CARAPI_(void) UpdateProviderListenersLocked(
        /* [in] */ String provider,
        /* [in] */ Boolean enabled);

    CARAPI_(Int64) GetMinTimeLocked(
        /* [in] */ String provider);

    CARAPI_(AutoPtr<Receiver>) GetReceiver(
        /* [in] */ ILocationListener* listener);

    CARAPI_(AutoPtr<Receiver>) GetReceiver(
        /* [in] */ IPendingIntent* intent);

    CARAPI_(Boolean) ProviderHasListener(
        /* [in] */ String provider,
        /* [in] */ Int32 uid,
        /* [in] */ Receiver* excludedReceiver);

    CARAPI RequestLocationUpdatesLocked(
    	/* [in] */ String provider,
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
    	/* [in] */ String provider,
    	/* [out] */ IBundle** info);

    CARAPI _IsProviderEnabledLocked(
    	/* [in] */ String provider,
    	/* [out] */ Boolean* isEnabled);

    CARAPI _GetLastKnownLocationLocked(
    	/* [in] */ String provider,
    	/* [out] */ ILocation** location);

    CARAPI_(void) HandleLocationChangedLocked(
    	/* [in] */ ILocation* location,
    	/* [in] */ Boolean passive);

    CARAPI_(void) IncrementPendingBroadcasts();

    CARAPI_(void) DecrementPendingBroadcasts();
    
    // Mock Providers

    CARAPI CheckMockPermissionsSafe();

private:
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
    GeocoderProxy* mGeocodeProvider;
    AutoPtr<IGpsStatusProvider> mGpsStatusProvider;
    AutoPtr<INetInitiatedListener> mNetInitiatedListener;
    LocationWorkerHandler* mLocationHandler;

    // Cache the real providers for use in addTestProvider() and removeTestProvider()
    AutoPtr<LocationProviderProxy> mNetworkLocationProvider;
    AutoPtr<LocationProviderInterface> mGpsLocationProvider;

    // wakelock variables
//    PowerManager.WakeLock mWakeLock = null;
    Int32 mPendingBroadcasts;

    /**
     * List of all receivers.
     */
    HashMap<AutoPtr<IInterface>, AutoPtr<Receiver> > mReceivers;


    /**
     * List of location providers.
     */
    List<AutoPtr<LocationProviderInterface> > mProviders;
    HashMap<String, AutoPtr<LocationProviderInterface> > mProvidersByName;

    /**
     * Object used internally for synchronization
     */
//    Object mLock = new Object();

    /**
     * Mapping from provider name to all its UpdateRecords
     */
    HashMap<String, List<UpdateRecord*>* > mRecordsByProvider;

    /**
     * Temporary filled in when computing min time for a provider.  Access is
     * protected by global lock mLock.
     */
//    WorkSource mTmpWorkSource = new WorkSource();

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
};

#endif //__CLOCATIONMANAGERSERVICE_H__
