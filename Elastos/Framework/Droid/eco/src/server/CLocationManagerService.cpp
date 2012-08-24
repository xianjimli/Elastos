
#include "server/CLocationManagerService.h"
#include "content/CIntent.h"
#include "content/CIntentFilter.h"
#include "server/location/GpsLocationProvider.h"
#include "server/location/PassiveProvider.h"
#include "location/CCriteria.h"
#include "location/CLocation.h"
#include <StringBuffer.h>
#include <elastos/Math.h>

const CString CLocationManagerService::TAG = "LocationManagerService";
const Boolean CLocationManagerService::LOCAL_LOGV;
const CString CLocationManagerService::ACCESS_FINE_LOCATION =
    "elastos.Manifest.permission.ACCESS_FINE_LOCATION";
const CString CLocationManagerService::ACCESS_COARSE_LOCATION =
    "elastos.Manifest.permission.ACCESS_COARSE_LOCATION";
const CString CLocationManagerService::ACCESS_MOCK_LOCATION =
    "elastos.Manifest.permission.ACCESS_MOCK_LOCATION";
const CString CLocationManagerService::ACCESS_LOCATION_EXTRA_COMMANDS =
    "elastos.Manifest.permission.ACCESS_LOCATION_EXTRA_COMMANDS";
const CString CLocationManagerService::INSTALL_LOCATION_PROVIDER =
    "elastos.Manifest.permission.INSTALL_LOCATION_PROVIDER";
const Int32 CLocationManagerService::MESSAGE_LOCATION_CHANGED;
const Int32 CLocationManagerService::MESSAGE_PACKAGE_UPDATED;
const CString CLocationManagerService::WAKELOCK_KEY = "LocationManagerService";

const Int32 CLocationManagerService::LpCapabilityComparator::ALTITUDE_SCORE;
const Int32 CLocationManagerService::LpCapabilityComparator::BEARING_SCORE;
const Int32 CLocationManagerService::LpCapabilityComparator::SPEED_SCORE;

Boolean CLocationManagerService::sProvidersLoaded = FALSE;

CLocationManagerService::Receiver::Receiver(
    /* [in] */ CLocationManagerService* locationMS,
    /* [in] */ ILocationListener* listener)
    : mLocationMS(locationMS),
      mListener(listener),
      mPendingBroadcasts(0)
{
//    mKey = listener.asBinder();
}

CLocationManagerService::Receiver::Receiver(
    /* [in] */ CLocationManagerService* locationMS,
    /* [in] */ IPendingIntent* intent)
    : mLocationMS(locationMS),
      mPendingIntent(intent),
      mPendingBroadcasts(0)
{
//    mKey = intent;
}

PInterface CLocationManagerService::Receiver::Probe(
    /* [in]  */ REIID riid)
{
    return NULL;
}

UInt32 CLocationManagerService::Receiver::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CLocationManagerService::Receiver::Release()
{
    return ElRefBase::Release();
}

ECode CLocationManagerService::Receiver::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    return E_NOT_IMPLEMENTED;
}

String CLocationManagerService::Receiver::ToString()
{
    String result;
    if (mListener != NULL) {
        result = String((const char*)(StringBuffer("Receiver{")
        + this /*Integer.toHexString(System.identityHashCode(this))*/
        + " Listener " + /*mKey + */ "}"));
    }
    else {
        result = String((const char*)(StringBuffer("Receiver{")
        + this /*Integer.toHexString(System.identityHashCode(this))*/
        + " Intent " + /*mKey + */ "}"));
    }
//    result += "mUpdateRecords: " + mUpdateRecords;

    return result;
}

Boolean CLocationManagerService::Receiver::IsListener()
{
    return mListener != NULL;
}

Boolean CLocationManagerService::Receiver::IsPendingIntent()
{
    return mPendingIntent != NULL;
}

ECode CLocationManagerService::Receiver::GetListener(
    /* [out] */ ILocationListener** locationListener)
{
    assert(locationListener != NULL);

	if (mListener != NULL) {
        *locationListener = mListener;
        return NOERROR;
    }

    return E_ILLEGAL_STATE_EXCEPTION;
//    throw new IllegalStateException("Request for non-existent listener");
}

ECode CLocationManagerService::Receiver::GetPendingIntent(
    /* [out] */ IPendingIntent** pendingIntent)
{
	assert(pendingIntent != NULL);

	if (mPendingIntent != NULL) {
        *pendingIntent = mPendingIntent;
        return NOERROR;
    }

    return E_ILLEGAL_STATE_EXCEPTION;
//    throw new IllegalStateException("Request for non-existent intent");
}

Boolean CLocationManagerService::Receiver::CallStatusChangedLocked(
    /* [in] */ String provider,
    /* [in] */ Int32 status,
    /* [in] */ IBundle* extras)
{
    if (mListener != NULL) {
//        try {
//            synchronized (this) {
        // synchronize to ensure incrementPendingBroadcastsLocked()
        // is called before decrementPendingBroadcasts()
        ECode ec = mListener->OnStatusChanged(provider, status, extras);
        if (FAILED(ec)) {
            return FALSE;
        }

        if (mListener != mLocationMS->mProximityListener) {
            // call this after broadcasting so we do not increment
            // if we throw an exeption.
            IncrementPendingBroadcastsLocked();
        }
//            }
//        } catch (RemoteException e) {
//            return false;
//        }
    }
    else {
        AutoPtr<IIntent> statusChanged;
        CIntent::New((IIntent**)&statusChanged);
        statusChanged->PutBooleanExtra(String(LocationManager_KEY_STATUS_CHANGED), status);
//        try {
//            synchronized (this) {
        // synchronize to ensure incrementPendingBroadcastsLocked()
        // is called before decrementPendingBroadcasts()
//        mPendingIntent->Send(mContext, 0, statusChanged, this, mLocationHandler);
        // call this after broadcasting so we do not increment
        // if we throw an exeption.
        IncrementPendingBroadcastsLocked();
//            }
//        } catch (PendingIntent.CanceledException e) {
//            return false;
//        }
    }
    return TRUE;
}

Boolean CLocationManagerService::Receiver::CallLocationChangedLocked(
    /* [in] */ ILocation* location)
{
    if (mListener != NULL) {
//        try {
//            synchronized (this) {
        // synchronize to ensure incrementPendingBroadcastsLocked()
        // is called before decrementPendingBroadcasts()
        ECode ec = mListener->OnLocationChanged(location);
        if (FAILED(ec)) {
            return FALSE;
        }

        if (mListener != mLocationMS->mProximityListener) {
            // call this after broadcasting so we do not increment
            // if we throw an exeption.
            IncrementPendingBroadcastsLocked();
        }
//            }
//        } catch (RemoteException e) {
//            return false;
//        }
    }
    else {
        AutoPtr<IIntent> locationChanged;
        CIntent::New((IIntent**)&locationChanged);
        AutoPtr<IParcelable> parcelable = (IParcelable*)location->Probe(EIID_IParcelable);
        locationChanged->PutParcelableExtra(String(LocationManager_KEY_LOCATION_CHANGED), parcelable);
//        try {
//            synchronized (this) {
        // synchronize to ensure incrementPendingBroadcastsLocked()
        // is called before decrementPendingBroadcasts()
//        mPendingIntent.send(mContext, 0, locationChanged, this, mLocationHandler);
        // call this after broadcasting so we do not increment
        // if we throw an exeption.
        IncrementPendingBroadcastsLocked();
//            }
//        } catch (PendingIntent.CanceledException e) {
//            return false;
//        }
    }

    return TRUE;
}

Boolean CLocationManagerService::Receiver::CallProviderEnabledLocked(
    /* [in] */ String provider,
    /* [in] */ Boolean enabled)
{
    if (mListener != NULL) {
//        try {
//            synchronized (this) {
        // synchronize to ensure incrementPendingBroadcastsLocked()
        // is called before decrementPendingBroadcasts()
        ECode ec = NOERROR;
        if (enabled) {
            ec = mListener->OnProviderEnabled(provider);
        }
        else {
            ec = mListener->OnProviderDisabled(provider);
        }
        if (FAILED(ec)) {
            return FALSE;
        }

        if (mListener != mLocationMS->mProximityListener) {
            // call this after broadcasting so we do not increment
            // if we throw an exeption.
            IncrementPendingBroadcastsLocked();
        }
//            }
//        } catch (RemoteException e) {
//            return false;
//        }
    }
    else {
        AutoPtr<IIntent> providerIntent;
        CIntent::New((IIntent**)&providerIntent);
        providerIntent->PutBooleanExtra(String(LocationManager_KEY_PROVIDER_ENABLED), enabled);
//        try {
//            synchronized (this) {
        // synchronize to ensure incrementPendingBroadcastsLocked()
        // is called before decrementPendingBroadcasts()
//        mPendingIntent.send(mContext, 0, providerIntent, this, mLocationHandler);
        // call this after broadcasting so we do not increment
        // if we throw an exeption.
        IncrementPendingBroadcastsLocked();
//            }
//        } catch (PendingIntent.CanceledException e) {
//            return false;
//        }
    }
    return TRUE;
}

void CLocationManagerService::Receiver::BinderDied()
{
    if (LOCAL_LOGV) {
//        Slog.v(TAG, "Location listener died");
    }
//    synchronized (mLock) {
    mLocationMS->RemoveUpdatesLocked(this);
//    }
//    synchronized (this) {
    if (mPendingBroadcasts > 0) {
        mLocationMS->DecrementPendingBroadcasts();
        mPendingBroadcasts = 0;
    }
//    }
}

void CLocationManagerService::Receiver::OnSendFinished(
    /* [in] */ IPendingIntent* pendingIntent,
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 resultCode,
    /* [in] */ String resultData,
    /* [in] */ IBundle* resultExtras)
{
//    synchronized (this) {
    DecrementPendingBroadcastsLocked();
//    }
}

// this must be called while synchronized by caller in a synchronized block
// containing the sending of the broadcaset
void CLocationManagerService::Receiver::IncrementPendingBroadcastsLocked()
{
    if (mPendingBroadcasts++ == 0) {
        mLocationMS->IncrementPendingBroadcasts();
    }
}

void CLocationManagerService::Receiver::DecrementPendingBroadcastsLocked()
{
    if (--mPendingBroadcasts == 0) {
        mLocationMS->DecrementPendingBroadcasts();
    }
}

PInterface CLocationManagerService::LocationWorkerHandler::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }
    else if (riid == EIID_IApartment) {
        return (IApartment*)this;
    }

    return NULL;
}

UInt32 CLocationManagerService::LocationWorkerHandler::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CLocationManagerService::LocationWorkerHandler::Release()
{
    return ElRefBase::Release();
}

ECode CLocationManagerService::LocationWorkerHandler::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManagerService::LocationWorkerHandler::Start(
    /* [in] */ ApartmentAttr attr)
{
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManagerService::LocationWorkerHandler::Finish()
{
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManagerService::LocationWorkerHandler::PostCppCallback(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id)
{
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManagerService::LocationWorkerHandler::PostCppCallbackAtTime(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id,
    /* [in] */ Millisecond64 uptimeMillis)
{
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManagerService::LocationWorkerHandler::PostCppCallbackDelayed(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id,
    /* [in] */ Millisecond64 delayMillis)
{
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManagerService::LocationWorkerHandler::PostCppCallbackAtFrontOfQueue(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id)
{
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManagerService::LocationWorkerHandler::RemoveCppCallbacks(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func)
{
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManagerService::LocationWorkerHandler::RemoveCppCallbacksEx(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ Int32 id)
{
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManagerService::LocationWorkerHandler::HasCppCallbacks(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManagerService::LocationWorkerHandler::HasCppCallbacksEx(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ Int32 id,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

CLocationManagerService::CLocationManagerService()
    : mPendingBroadcasts(0),
      mNetworkState(LocationProvider_TEMPORARILY_UNAVAILABLE)
{
}

// for reporting callback completion
ECode CLocationManagerService::LocationCallbackFinished(
    /* [in] */ ILocationListener* listener)
{
    //Do not use getReceiver here as that will add the ILocationListener to
    //the receiver list if it is not found.  If it is not found then the
    //LocationListener was removed when it had a pending broadcast and should
    //not be added back.
    AutoPtr<IInterface> listenerInterface = listener->Probe(EIID_IInterface);
    HashMap<AutoPtr<IInterface>, AutoPtr<Receiver> >::Iterator it =
            mReceivers.Find(listenerInterface);
    AutoPtr<Receiver> receiver;
    if (it != mReceivers.End()) {
        receiver = it->mSecond;
    }
    if (receiver != NULL) {
//        synchronized (receiver) {
            // so wakelock calls will succeed
//        Int64 identity = Binder.clearCallingIdentity();
        receiver->DecrementPendingBroadcastsLocked();
//        Binder.restoreCallingIdentity(identity);
//       }
    }

    return NOERROR;
}

void CLocationManagerService::AddProvider(
    /* [in] */ LocationProviderInterface* provider)
{
    mProviders.PushBack(AutoPtr<LocationProviderInterface>((LocationProviderInterface*)provider));
    mProvidersByName[provider->GetName()] = AutoPtr<LocationProviderInterface>((LocationProviderInterface*)provider);
}

void CLocationManagerService::RemoveProvider(
    /* [in] */ LocationProviderInterface* provider)
{
    List<AutoPtr<LocationProviderInterface> >::Iterator it;
    for (it = mProviders.Begin(); it != mProviders.End(); ++it) {
        AutoPtr<LocationProviderInterface> locationProvider = it->Get();
        if (locationProvider.Get() == provider)
            break;
    }
    mProviders.Erase(it);
    mProvidersByName.Erase(provider->GetName());
}

void CLocationManagerService::LoadProviders()
{
//    synchronized (mLock) {
    if (sProvidersLoaded) {
        return;
    }

    // Load providers
    LoadProvidersLocked();
    sProvidersLoaded = TRUE;
//    }
}

void CLocationManagerService::LoadProvidersLocked()
{
//    try {
    _LoadProvidersLocked();
//    } catch (Exception e) {
//        Slog.e(TAG, "Exception loading providers:", e);
//    }
}

void CLocationManagerService::_LoadProvidersLocked()
{
    // Attempt to load "real" providers first
    if (GpsLocationProvider::IsSupported()) {
        // Create a gps location provider
        AutoPtr<GpsLocationProvider> gpsProvider = new GpsLocationProvider(mContext, (ILocationManagerEx*)this);
        mGpsStatusProvider = gpsProvider->GetGpsStatusProvider();
//        mNetInitiatedListener = gpsProvider.getNetInitiatedListener();
        AddProvider((LocationProviderInterface*)gpsProvider.Get());
        mGpsLocationProvider = (LocationProviderInterface*)gpsProvider;
    }

    // create a passive location provider, which is always enabled
    AutoPtr<PassiveProvider> passiveProvider = new PassiveProvider((ILocationManagerEx*)this);
    AddProvider((LocationProviderInterface*)passiveProvider.Get());
    mEnabledProviders.Insert(passiveProvider->GetName());

    // initialize external network location and geocoder services
    if (mNetworkLocationProviderPackageName.IsNull()) {
        mNetworkLocationProvider =
            new LocationProviderProxy(mContext, String(LocationManager_NETWORK_PROVIDER),
                    mNetworkLocationProviderPackageName, (IApartment*)mLocationHandler);
        AddProvider((LocationProviderInterface*)mNetworkLocationProvider.Get());
    }

    if (mGeocodeProviderPackageName.IsNull()) {
        mGeocodeProvider = new GeocoderProxy(mContext, mGeocodeProviderPackageName);
    }

    UpdateProvidersLocked();
}

ECode CLocationManagerService::constructor(
    /* [in] */ IContext* ctx)
{
    mContext = ctx;
    AutoPtr<IResources> resources;
    ctx->GetResources((IResources**)&resources);
    resources->GetString(0x01040019/*com.android.internal.R.string.config_networkLocationProvider*/
            , &mNetworkLocationProviderPackageName);
    resources->GetString(0x0104001a/*com.android.internal.R.string.config_geocodeProvider*/
            , &mGeocodeProviderPackageName);
//    mPackageMonitor.register(context, TRUE);

//    if (LOCAL_LOGV) {
//        Slog.v(TAG, "Constructed LocationManager Service");
//    }

    return NOERROR;
}

void* CLocationManagerService::EntryRoutine(void *arg)
{
    if (arg == NULL) {
        pthread_exit((void*)E_THREAD_ABORTED);
    }

    CLocationManagerService* lms = (CLocationManagerService*)arg;
    lms->Run();
    return (void*)NOERROR;
}

void CLocationManagerService::SystemReady()
{
    // we defer starting up the service until the system is ready
    pthread_t thread;
    pthread_create(&thread, NULL, EntryRoutine, (void*)this);
//    Thread thread = new Thread(null, this, "LocationManagerService");
//    thread.start();
}

void CLocationManagerService::Initialize()
{
    // Create a wake lock, needs to be done before calling loadProviders() below
//    PowerManager powerManager = (PowerManager) mContext.getSystemService(Context.POWER_SERVICE);
//    mWakeLock = powerManager.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, WAKELOCK_KEY);

    // Load providers
    LoadProviders();

    // Register for Network (Wifi or Mobile) updates
    AutoPtr<IIntentFilter> intentFilter;
    CIntentFilter::New((IIntentFilter**)&intentFilter);
    intentFilter->AddAction(String("elastos.net.conn.CONNECTIVITY_CHANGE")/*ConnectivityManager_CONNECTIVITY_ACTION*/);
    // Register for Package Manager updates
    intentFilter->AddAction(String(Intent_ACTION_CAPSULE_REMOVED));
    intentFilter->AddAction(String(Intent_ACTION_CAPSULE_RESTARTED));
    intentFilter->AddAction(String(Intent_ACTION_QUERY_CAPSULE_RESTART));
//    mContext->RegisterReceiver(mBroadcastReceiver, intentFilter);
    AutoPtr<IIntentFilter> sdFilter;
    CIntentFilter::New(String(Intent_ACTION_EXTERNAL_APPLICATIONS_UNAVAILABLE), (IIntentFilter**)&sdFilter);
//    mContext.registerReceiver(mBroadcastReceiver, sdFilter);

    // listen for settings changes
    AutoPtr<IContentResolver> resolver;
    mContext->GetContentResolver((IContentResolver**)&resolver);
    AutoPtr<ICursor> settingsCursor;
//    resolver->Query(Settings.Secure.CONTENT_URI, NULL,
//            "(" + Settings.System.NAME + "=?)",
//            new String[]{Settings.Secure.LOCATION_PROVIDERS_ALLOWED},
//            null);
//    mSettings = new ContentQueryMap(settingsCursor, Settings.System.NAME, true, mLocationHandler);
//    SettingsObserver* settingsObserver = new SettingsObserver();
//    mSettings.addObserver(settingsObserver);
}

ECode CLocationManagerService::Run()
{
//    Process.setThreadPriority(Process.THREAD_PRIORITY_BACKGROUND);
//    Looper.prepare();
//    mLocationHandler = new LocationWorkerHandler();
//    initialize();
//    Looper.loop();

    return E_NOT_IMPLEMENTED;
}

Boolean CLocationManagerService::IsAllowedBySettingsLocked(
    /* [in] */ String provider)
{
    if (mEnabledProviders.Find(provider) != mEnabledProviders.End()) {
        return TRUE;
    }
    if (mDisabledProviders.Find(provider) != mDisabledProviders.End()) {
        return FALSE;
    }
    // Use system settings
    AutoPtr<IContentResolver> resolver;
    mContext->GetContentResolver((IContentResolver**)&resolver);

//    return Settings.Secure.isLocationProviderEnabled(resolver, provider);
    return TRUE;
}

ECode CLocationManagerService::CheckPermissionsSafe(
    /* [in] */ String provider)
{
    if (LocationManager_GPS_PROVIDER == provider
             || LocationManager_PASSIVE_PROVIDER == provider) {
        Int32 perm;
        mContext->CheckCallingOrSelfPermission(String(ACCESS_FINE_LOCATION), &perm);
        if (perm != CapsuleManager_PERMISSION_GRANTED) {
            return E_SECURITY_EXCEPTION;
        }
//        throw new SecurityException("Provider " + provider
//                + " requires ACCESS_FINE_LOCATION permission");
    }
    if (LocationManager_NETWORK_PROVIDER == provider) {
        Int32 perm;
        mContext->CheckCallingOrSelfPermission(String(ACCESS_FINE_LOCATION), &perm);
        if (perm == CapsuleManager_PERMISSION_GRANTED) {
            return NOERROR;
        }
        mContext->CheckCallingOrSelfPermission(String(ACCESS_COARSE_LOCATION), &perm);
        if (perm != CapsuleManager_PERMISSION_GRANTED) {
            return E_SECURITY_EXCEPTION;
        }
//        throw new SecurityException("Provider " + provider
//                + " requires ACCESS_FINE_LOCATION or ACCESS_COARSE_LOCATION permission");
    }

    return NOERROR;
}

Boolean CLocationManagerService::IsAllowedProviderSafe(
    /* [in] */ String provider)
{
    if (LocationManager_GPS_PROVIDER == provider
            || LocationManager_PASSIVE_PROVIDER == provider) {
        Int32 perm;
        mContext->CheckCallingOrSelfPermission(String(ACCESS_FINE_LOCATION), &perm);
        if (perm != CapsuleManager_PERMISSION_GRANTED ) {
            return FALSE;
        }
    }
    if (LocationManager_NETWORK_PROVIDER == provider) {
        Int32 perm;
        mContext->CheckCallingOrSelfPermission(String(ACCESS_FINE_LOCATION), &perm);
        if (perm == CapsuleManager_PERMISSION_GRANTED) {
            return TRUE;
        }
        mContext->CheckCallingOrSelfPermission(String(ACCESS_COARSE_LOCATION), &perm);
        if (perm != CapsuleManager_PERMISSION_GRANTED) {
            return FALSE;
        }
    }

    return TRUE;
}

ECode CLocationManagerService::GetAllProviders(
    /* [out] */ ArrayOf<String>* allProviders)
{
//    try {
//        synchronized (mLock) {
    VALIDATE_NOT_NULL(allProviders);

    allProviders = _GetAllProvidersLocked();
    return NOERROR;
//        }
//    } catch (SecurityException se) {
//        throw se;
//    } catch (Exception e) {
//        Slog.e(TAG, "getAllProviders got exception:", e);
//        return null;
//    }
}

ArrayOf<String>* CLocationManagerService::_GetAllProvidersLocked()
{
//    if (LOCAL_LOGV) {
//        Slog.v(TAG, "getAllProviders");
//    }
    Int32 size = mProviders.GetSize();
    ArrayOf<String>* out = ArrayOf<String>::Alloc(size);
    List<AutoPtr<LocationProviderInterface> >::ReverseIterator rit;
    Int32 i = 0;
    for (rit = mProviders.RBegin(); rit != mProviders.REnd(); ++rit, ++i) {
        AutoPtr<LocationProviderInterface> p = *(rit);
        (*out)[i] = p->GetName();
    }
    return out;
}

ECode CLocationManagerService::GetProviders(
    /* [in] */ ICriteria* criteria,
    /* [in] */ Boolean enabledOnly,
    /* [out] */ ArrayOf<String>* providers)
{
//    try {
//        synchronized (mLock) {
    VALIDATE_NOT_NULL(providers);

    providers = _GetProvidersLocked(criteria, enabledOnly);
    return NOERROR;
//        }
//    } catch (SecurityException se) {
//        throw se;
//    } catch (Exception e) {
//        Slog.e(TAG, "getProviders got exception:", e);
//        return null;
//    }

    return E_NOT_IMPLEMENTED;
}

ArrayOf<String>* CLocationManagerService::_GetProvidersLocked(
    /* [in] */ ICriteria* criteria,
    /* [in] */ Boolean enabledOnly)
{
//    if (LOCAL_LOGV) {
//        Slog.v(TAG, "getProviders");
//    }
    Int32 size = mProviders.GetSize();
    ArrayOf<String>* out = ArrayOf<String>::Alloc(size);
    List<AutoPtr<LocationProviderInterface> >::ReverseIterator rit;
    Int32 i = 0;
    for (rit = mProviders.RBegin(); rit != mProviders.REnd(); ++rit) {
        AutoPtr<LocationProviderInterface> p = *rit;
        String name = p->GetName();
        if (IsAllowedProviderSafe(name)) {
            if (enabledOnly && !IsAllowedBySettingsLocked(name)) {
                continue;
            }
            if (criteria != NULL && !p->MeetsCriteria(criteria)) {
                continue;
            }
            (*out)[i] = p->GetName();
            ++i;
        }
    }
    return out;
}

Int32 CLocationManagerService::NextPower(
    /* [in] */ Int32 power)
{
    switch (power) {
    case Criteria_POWER_LOW:
        return Criteria_POWER_MEDIUM;
    case Criteria_POWER_MEDIUM:
        return Criteria_POWER_HIGH;
    case Criteria_POWER_HIGH:
        return Criteria_NO_REQUIREMENT;
    case Criteria_NO_REQUIREMENT:
    default:
        return Criteria_NO_REQUIREMENT;
    }
}

Int32 CLocationManagerService::NextAccuracy(
    /* [in] */ Int32 accuracy)
{
    if (accuracy == Criteria_ACCURACY_FINE) {
        return Criteria_ACCURACY_COARSE;
    }
    else {
        return Criteria_NO_REQUIREMENT;
    }
}

PInterface CLocationManagerService::LpPowerComparator::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }
    else if (riid == EIID_IComparator) {
        return (IComparator*)this;
    }

    return NULL;
}

UInt32 CLocationManagerService::LpPowerComparator::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CLocationManagerService::LpPowerComparator::Release()
{
    return ElRefBase::Release();
}

ECode CLocationManagerService::LpPowerComparator::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManagerService::LpPowerComparator::Compare(
    /* [in] */ IInterface* object1,
    /* [in] */ IInterface* object2,
    /* [out] */ Int32* result)
{
    // Smaller is better
    VALIDATE_NOT_NULL(result);

    LocationProviderInterface* first = (LocationProviderInterface*)object1->Probe(EIID_LocationProviderInterface);
    LocationProviderInterface* second = (LocationProviderInterface*)object2->Probe(EIID_LocationProviderInterface);

    VALIDATE_NOT_NULL(first);
    VALIDATE_NOT_NULL(second);
    *result = first->GetPowerRequirement() - second->GetPowerRequirement();

    return NOERROR;
}

ECode CLocationManagerService::LpPowerComparator::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* isEqual)
{
//    *isEqual = l1.getPowerRequirement() == l2.getPowerRequirement();

    return E_NOT_IMPLEMENTED;
}

PInterface CLocationManagerService::LpAccuracyComparator::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }
    else if (riid == EIID_IComparator) {
        return (IComparator*)this;
    }

    return NULL;
}

UInt32 CLocationManagerService::LpAccuracyComparator::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CLocationManagerService::LpAccuracyComparator::Release()
{
    return ElRefBase::Release();
}

ECode CLocationManagerService::LpAccuracyComparator::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManagerService::LpAccuracyComparator::Compare(
    /* [in] */ IInterface* object1,
    /* [in] */ IInterface* object2,
    /* [out] */ Int32* result)
{
    // Smaller is better
    VALIDATE_NOT_NULL(result);

    LocationProviderInterface* first = (LocationProviderInterface*)object1->Probe(EIID_LocationProviderInterface);
    LocationProviderInterface* second = (LocationProviderInterface*)object2->Probe(EIID_LocationProviderInterface);

    VALIDATE_NOT_NULL(first);
    VALIDATE_NOT_NULL(second);
    *result = first->GetAccuracy() - second->GetAccuracy();

    return NOERROR;
}

ECode CLocationManagerService::LpAccuracyComparator::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* isEqual)
{
//    return (l1.getAccuracy() == l2.getAccuracy());

    return E_NOT_IMPLEMENTED;
}

PInterface CLocationManagerService::LpCapabilityComparator::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }
    else if (riid == EIID_IComparator) {
        return (IComparator*)this;
    }

    return NULL;
}

UInt32 CLocationManagerService::LpCapabilityComparator::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CLocationManagerService::LpCapabilityComparator::Release()
{
    return ElRefBase::Release();
}

ECode CLocationManagerService::LpCapabilityComparator::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    return E_NOT_IMPLEMENTED;
}

Int32 CLocationManagerService::LpCapabilityComparator::Score(
    /* [in] */ LocationProviderInterface* p)
{
    return (p->SupportsAltitude() ? ALTITUDE_SCORE : 0) +
                (p->SupportsBearing() ? BEARING_SCORE : 0) +
                (p->SupportsSpeed() ? SPEED_SCORE : 0);
}

ECode CLocationManagerService::LpCapabilityComparator::Compare(
    /* [in] */ IInterface* object1,
    /* [in] */ IInterface* object2,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    LocationProviderInterface* first = (LocationProviderInterface*)object1->Probe(EIID_LocationProviderInterface);
    LocationProviderInterface* second = (LocationProviderInterface*)object2->Probe(EIID_LocationProviderInterface);

    VALIDATE_NOT_NULL(first);
    VALIDATE_NOT_NULL(second);
    *result = Score(first) - Score(second);// Bigger is better

    return NOERROR;
}

ECode CLocationManagerService::LpCapabilityComparator::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* isEqual)
{
//    return (score(l1) == score(l2));

    return E_NOT_IMPLEMENTED;
}

AutoPtr<LocationProviderInterface> CLocationManagerService::Best(
    /* [in] */ const ArrayOf<String>& providerNames)
{
//    synchronized (mLock) {
    Int32 size = providerNames.GetLength();
    List<AutoPtr<LocationProviderInterface> > providers;
    Int32 i = 0;
    for (; i < size ; ++i) {
        HashMap<String, AutoPtr<LocationProviderInterface> >::Iterator it = mProvidersByName.Find(providerNames[i]);
        if (it != mProvidersByName.End() && it->mSecond != NULL) {
            providers.PushBack(it->mSecond);
        }
    }
//    }

    size = providers.GetSize();
    if (size < 2) {
        return *(providers.Begin());
    }

    // First, sort by power requirement
//    Collections.sort(providers, new LpPowerComparator());
    Int32 power = providers[0]->GetPowerRequirement();
    if (power < providers[1]->GetPowerRequirement()) {
        return providers[0];
    }

    List<AutoPtr<LocationProviderInterface> > tmp;
    List<AutoPtr<LocationProviderInterface> >::Iterator providersIt;
    while ((providersIt != providers.End()) && ((*providersIt)->GetPowerRequirement() == power)) {
        tmp.PushBack(*providersIt);
        providersIt++;
    }

    // Next, sort by accuracy
//    Collections.sort(tmp, new LpAccuracyComparator());
    Int32 acc = (*(tmp.Begin()))->GetAccuracy();
    if (acc < (*(++(tmp.Begin())))->GetAccuracy()) {
        return *(tmp.Begin());
    }

    List<AutoPtr<LocationProviderInterface> > tmp2;
    List<AutoPtr<LocationProviderInterface> >::Iterator tmpIt;
    while ((tmpIt != tmp.End()) && ((*tmpIt)->GetAccuracy() == acc)) {
        tmp2.PushBack(*tmpIt);
        tmpIt++;
    }

    // Finally, sort by capability "score"
//    Collections.sort(tmp2, new LpCapabilityComparator());
    return *(tmp2.Begin());
}

ECode CLocationManagerService::GetBestProvider(
    /* [in] */ ICriteria* criteria,
    /* [in] */ Boolean enabledOnly,
    /* [out] */ String* provider)
{
	VALIDATE_NOT_NULL(provider);

	ArrayOf<String>* goodProviders;
    GetProviders(criteria, enabledOnly, goodProviders);
    if (goodProviders->GetLength() > 0) {
        *provider = Best(*goodProviders)->GetName();
        return NOERROR;
    }

    // Make a copy of the criteria that we can modify
    AutoPtr<ICriteria> criteriaCopy;
    CCriteria::New(criteria, (ICriteria**)&criteriaCopy);

    // Loosen power requirement
    Int32 power;
    criteriaCopy->GetPowerRequirement(&power);
    while ((goodProviders->GetLength() == 0) && (power != Criteria_NO_REQUIREMENT)) {
        power = NextPower(power);
        criteriaCopy->SetPowerRequirement(power);
        GetProviders(criteriaCopy, enabledOnly, goodProviders);
    }
    if (goodProviders->GetLength() > 0) {
        *provider = Best(*goodProviders)->GetName();
        return NOERROR;
    }

    // Loosen accuracy requirement
    Int32 accuracy;
    criteriaCopy->GetAccuracy(&accuracy);
    while ((goodProviders->GetLength() == 0) && (accuracy != Criteria_NO_REQUIREMENT)) {
        accuracy = NextAccuracy(accuracy);
        criteriaCopy->SetAccuracy(accuracy);
        GetProviders(criteriaCopy, enabledOnly, goodProviders);
    }
    if (goodProviders->GetLength() > 0) {
        *provider = Best(*goodProviders)->GetName();
        return NOERROR;
    }

    // Remove bearing requirement
    criteriaCopy->SetBearingRequired(FALSE);
    GetProviders(criteriaCopy, enabledOnly, goodProviders);
    if (goodProviders->GetLength() > 0) {
        *provider = Best(*goodProviders)->GetName();
        return NOERROR;
    }

    // Remove speed requirement
    criteriaCopy->SetSpeedRequired(FALSE);
    GetProviders(criteriaCopy, enabledOnly, goodProviders);
    if (goodProviders->GetLength() > 0) {
        *provider = Best(*goodProviders)->GetName();
        return NOERROR;
    }

    // Remove altitude requirement
    criteriaCopy->SetAltitudeRequired(FALSE);
    GetProviders(criteriaCopy, enabledOnly, goodProviders);
    if (goodProviders->GetLength() > 0) {
        *provider = Best(*goodProviders)->GetName();
        return NOERROR;
    }

    *provider = String(NULL);

    return NOERROR;
}

ECode CLocationManagerService::ProviderMeetsCriteria(
    /* [in] */ const String& provider,
    /* [in] */ ICriteria* criteria,
    /* [out] */ Boolean* result)
{
	VALIDATE_NOT_NULL(result);

	AutoPtr<LocationProviderInterface> p;
    HashMap<String, AutoPtr<LocationProviderInterface> >::Iterator it = mProvidersByName.Find(provider);
    if (it != mProvidersByName.End()) {
        p = it->mSecond;
    }
    if (p == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("provider=" + provider);
    }

    *result = p->MeetsCriteria(criteria);

    return NOERROR;
}

void CLocationManagerService::UpdateProvidersLocked()
{
    Boolean changesMade = FALSE;
    List<AutoPtr<LocationProviderInterface> >::ReverseIterator rit;
    for (rit = mProviders.RBegin(); rit != mProviders.REnd(); ++rit) {
        AutoPtr<LocationProviderInterface> p = *rit;
        Boolean isEnabled = p->IsEnabled();
        String name = p->GetName();
        Boolean shouldBeEnabled = IsAllowedBySettingsLocked(name);
        if (isEnabled && !shouldBeEnabled) {
            UpdateProviderListenersLocked(name, FALSE);
            changesMade = TRUE;
        }
        else if (!isEnabled && shouldBeEnabled) {
            UpdateProviderListenersLocked(name, TRUE);
            changesMade = TRUE;
        }
    }
    if (changesMade) {
        AutoPtr<IIntent> intent;
        CIntent::New(String(LocationManager_PROVIDERS_CHANGED_ACTION), (IIntent**)&intent);
        mContext->SendBroadcast(intent);
    }
}

void CLocationManagerService::UpdateProviderListenersLocked(
    /* [in] */ String provider,
    /* [in] */ Boolean enabled)
{
    Int32 listeners = 0;

    AutoPtr<LocationProviderInterface> p;
    HashMap<String, AutoPtr<LocationProviderInterface> >::Iterator it = mProvidersByName.Find(provider);
    if (it != mProvidersByName.End()) {
        p = it->mSecond;
    }
    if (p == NULL) {
        return;
    }

    List<AutoPtr<Receiver> >* deadReceivers = NULL;

    List<UpdateRecord*>* records = NULL;
    HashMap<String, List<UpdateRecord*>* >::Iterator hmIt = mRecordsByProvider.Find(provider);
    if (hmIt != mRecordsByProvider.End()) {
        records = hmIt->mSecond;
    }

    if (records != NULL) {
        List<UpdateRecord*>::Iterator lit;
        for (lit = records->Begin(); lit != records->End(); ++lit) {
            UpdateRecord* record = *lit;
            // Sends a notification message to the receiver
            if (!record->mReceiver->CallProviderEnabledLocked(provider, enabled)) {
                if (deadReceivers == NULL) {
                    deadReceivers = new List<AutoPtr<Receiver> >();
                }
                deadReceivers->PushBack(record->mReceiver);
            }
            listeners++;
        }
    }

    if (deadReceivers != NULL) {
        List<AutoPtr<Receiver> >::ReverseIterator lrit;
        for (lrit = deadReceivers->RBegin(); lrit != deadReceivers->REnd(); ++lrit) {
            RemoveUpdatesLocked(*lrit);
        }
    }

    if (enabled) {
        p->Enable();
        if (listeners > 0) {
//            p->SetMinTime(GetMinTimeLocked(provider), mTmpWorkSource);
            p->EnableLocationTracking(TRUE);
        }
    }
    else {
        p->EnableLocationTracking(FALSE);
        p->Disable();
    }
}

Int64 CLocationManagerService::GetMinTimeLocked(
    /* [in] */ String provider)
{
    Int64 minTime = Math::INT64_MAX_VALUE;
    List<UpdateRecord*>* records = NULL;
    HashMap<String, List<UpdateRecord*>* >::Iterator hmit = mRecordsByProvider.Find(provider);
    if (hmit != mRecordsByProvider.End()) {
        records = hmit->mSecond;
    }
//    mTmpWorkSource.clear();
    if (records != NULL) {
        List<UpdateRecord*>::ReverseIterator rit;
        for (rit = records->RBegin(); rit != records->REnd(); ++rit) {
            UpdateRecord* ur = *rit;
            Int32 curTime = ur->mMinTime;
            if (curTime < minTime) {
                minTime = curTime;
            }
        }
        Int32 inclTime = (minTime * 3) / 2;
        for (rit = records->RBegin(); rit != records->REnd(); ++rit) {
            UpdateRecord* ur = *rit;
            if (ur->mMinTime <= inclTime) {
//                mTmpWorkSource.add(ur.mUid);
            }
        }
    }
    return minTime;
}

CLocationManagerService::UpdateRecord::UpdateRecord(
    /* [in] */ CLocationManagerService* lms,
    /* [in] */ String provider,
    /* [in] */ Int64 minTime,
    /* [in] */ Float minDistance,
    /* [in] */ Boolean singleShot,
    /* [in] */ Receiver* receiver,
    /* [in] */ Int32 uid)
    : mLocationManagerService(lms),
      mProvider(provider),
      mReceiver(receiver),
      mMinTime(minTime),
      mMinDistance(minDistance),
      mSingleShot(singleShot),
      mUid(uid)
{
    List<UpdateRecord*>* records = NULL;
    HashMap<String, List<UpdateRecord*>* >::Iterator hmit= mLocationManagerService->mRecordsByProvider.Find(provider);
    if (hmit != mLocationManagerService->mRecordsByProvider.End()) {
        records = hmit->mSecond;
    }
    if (records == NULL) {
        records = new List<UpdateRecord*>();
        mLocationManagerService->mRecordsByProvider[provider] = records;
    }

    List<UpdateRecord*>::Iterator lit;
    for (lit = records->Begin(); lit != records->End(); ++lit) {
        if (*lit == this){
            break;
        }
    }
    if (lit == records->End()) {
        records->PushBack(this);
    }
}

void CLocationManagerService::UpdateRecord::DisposeLocked()
{
    List<UpdateRecord*>* records = NULL;
    HashMap<String, List<UpdateRecord*>* >::Iterator it = mLocationManagerService->mRecordsByProvider.Find(mProvider);
    if (it != mLocationManagerService->mRecordsByProvider.End()) {
        records = it->mSecond;
    }
    if (records != NULL) {
        records->Remove(this);
    }
}

ECode CLocationManagerService::UpdateRecord::ToString(
    /* [out] */ String* des)
{
    VALIDATE_NOT_NULL(des);

//    return "UpdateRecord{"
//            + Integer.toHexString(System.identityHashCode(this))
//            + " mProvider: " + mProvider + " mUid: " + mUid + "}";
    return E_NOT_IMPLEMENTED;
}

AutoPtr<CLocationManagerService::Receiver> CLocationManagerService::GetReceiver(
    /* [in] */ ILocationListener* listener)
{
//	IBinder binder = listener.asBinder();
	AutoPtr<Receiver> receiver;
	HashMap<AutoPtr<IInterface>, AutoPtr<Receiver> >::Iterator it = mReceivers.Find((IInterface*)listener);
    if (it != mReceivers.End()) {
    	receiver = it->mSecond;
    }
	if (receiver == NULL) {
		receiver = new Receiver(this, listener);
		mReceivers[AutoPtr<IInterface>((IInterface*)listener)]= receiver;

//		try {
		if (receiver->IsListener()) {
//			receiver.getListener().asBinder().linkToDeath(receiver, 0);
		}
//		} catch (RemoteException e) {
//			Slog.e(TAG, "linkToDeath failed:", e);
//			return null;
//		}
	}
	return receiver;
}

AutoPtr<CLocationManagerService::Receiver> CLocationManagerService::GetReceiver(
    /* [in] */ IPendingIntent* intent)
{
	AutoPtr<Receiver> receiver;
	HashMap<AutoPtr<IInterface>, AutoPtr<Receiver> >::Iterator it = mReceivers.Find((IInterface*)intent);
	if (it != mReceivers.End()) {
		receiver = it->mSecond;
	}
	if (receiver == NULL) {
		receiver = new Receiver(this, intent);
		mReceivers[AutoPtr<IInterface>((IInterface*)intent)]= receiver;
	}
	return receiver;
}

Boolean CLocationManagerService::ProviderHasListener(
	/* [in] */ String provider,
	/* [in] */ Int32 uid,
	/* [in] */ Receiver* excludedReceiver)
{
	List<UpdateRecord*>* records = NULL;
	HashMap<String, List<UpdateRecord*>* >::Iterator it = mRecordsByProvider.Find(provider);
	if (it != mRecordsByProvider.End()) {
		records = it->mSecond;
	}
	if (records != NULL) {
        List<UpdateRecord*>::ReverseIterator rit;
		for (rit = records->RBegin(); rit != records->REnd(); ++rit) {
			UpdateRecord* record = *rit;
			if (record->mUid == uid && record->mReceiver.Get() != excludedReceiver) {
				return TRUE;
			}
	   }
	}
	HashMap<AutoPtr<IPendingIntent>, AutoPtr<ProximityAlert> >::Iterator alertIt;
	for (alertIt = mProximityAlerts.Begin(); alertIt != mProximityAlerts.End(); ++alertIt) {
		AutoPtr<ProximityAlert> alert = alertIt->mSecond;
		if (alert->mUid == uid) {
			return TRUE;
		}
	}
	return FALSE;
}

ECode CLocationManagerService::RequestLocationUpdates(
    /* [in] */ const String& provider,
    /* [in] */ ICriteria* criteria,
    /* [in] */ Int64 minTime,
    /* [in] */ Float minDistance,
    /* [in] */ Boolean singleShot,
    /* [in] */ ILocationListener* listener)
{
	if (criteria != NULL) {
		// FIXME - should we consider using multiple providers simultaneously
		// rather than only the best one?
		// Should we do anything different for single shot fixes?
		String bestProvider(NULL);
		GetBestProvider(criteria, TRUE, &bestProvider);
		if (bestProvider.IsNullOrEmpty()) {
			return E_ILLEGAL_ARGUMENT_EXCEPTION;
//			throw new IllegalArgumentException("no providers found for criteria");
		}
	}
//	try {
//		synchronized (mLock) {
	return RequestLocationUpdatesLocked(provider, minTime, minDistance, singleShot,
			GetReceiver(listener));
//		}
//	} catch (SecurityException se) {
//		throw se;
//	} catch (IllegalArgumentException iae) {
//		throw iae;
//	} catch (Exception e) {
//		Slog.e(TAG, "requestUpdates got exception:", e);
//	}
}

ECode CLocationManagerService::RequestLocationUpdatesPI(
    /* [in] */ const String& provider,
    /* [in] */ ICriteria* criteria,
    /* [in] */ Int64 minTime,
    /* [in] */ Float minDistance,
    /* [in] */ Boolean singleShot,
    /* [in] */ IPendingIntent* intent)
{
	if (criteria != NULL) {
		// FIXME - should we consider using multiple providers simultaneously
		// rather than only the best one?
		// Should we do anything different for single shot fixes?
		String bestProvider(NULL);
		GetBestProvider(criteria, TRUE, &bestProvider);
		if (bestProvider.IsNullOrEmpty()) {
			return E_ILLEGAL_ARGUMENT_EXCEPTION;
//			throw new IllegalArgumentException("no providers found for criteria");
		}
	}
//	try {
//		synchronized (mLock) {
	return RequestLocationUpdatesLocked(provider, minTime, minDistance, singleShot,
					GetReceiver(intent));
//		}
//	} catch (SecurityException se) {
//		throw se;
//	} catch (IllegalArgumentException iae) {
//		throw iae;
//	} catch (Exception e) {
//		Slog.e(TAG, "requestUpdates got exception:", e);
//	}
}

ECode CLocationManagerService::RequestLocationUpdatesLocked(
	/* [in] */ String provider,
	/* [in] */ Int64 minTime,
	/* [in] */ Float minDistance,
	/* [in] */ Boolean singleShot,
	/* [in] */ Receiver* receiver)
{
	AutoPtr<LocationProviderInterface> p;
	HashMap<String, AutoPtr<LocationProviderInterface> >::Iterator it = mProvidersByName.Find(provider);
	if (it != mProvidersByName.End()) {
		p = it->mSecond;
	}
	if (p == NULL) {
		return E_ILLEGAL_ARGUMENT_EXCEPTION;
//		throw new IllegalArgumentException("provider=" + provider);
	}

	CheckPermissionsSafe(provider);

	// so wakelock calls will succeed32
//    Int32 callingUid = Binder.getCallingUid();
	Boolean newUid = !ProviderHasListener(provider, 0/*callingUid*/, NULL);
//	Int64 identity = Binder.clearCallingIdentity();
//	try {
	UpdateRecord* r = new UpdateRecord(this, provider, minTime, minDistance, singleShot,
			receiver, 0/*callingUid*/);
	UpdateRecord* oldRecord = NULL;
	HashMap<String, UpdateRecord*>::Iterator oldit = receiver->mUpdateRecords.Find(provider);
	if (oldit != receiver->mUpdateRecords.End()) {
		oldRecord = oldit->mSecond;
	}
	if (oldRecord != NULL) {
		oldRecord->DisposeLocked();
	}
	receiver->mUpdateRecords[provider] = r;

	if (newUid) {
//		p->AddListener(callingUid);
	}

	Boolean isProviderEnabled = IsAllowedBySettingsLocked(provider);
	if (isProviderEnabled) {
//		Int32 minTimeForProvider = GetMinTimeLocked(provider);
//		p->SetMinTime(minTimeForProvider, mTmpWorkSource);
		// try requesting single shot if singleShot is true, and fall back to
		// regular location tracking if requestSingleShotFix() is not supported
		if (!singleShot || !p->RequestSingleShotFix()) {
			p->EnableLocationTracking(TRUE);
		}
	}
	else {
		// Notify the listener that updates are currently disabled
		receiver->CallProviderEnabledLocked(provider, FALSE);
	}
//		if (LOCAL_LOGV) {
//			Slog.v(TAG, "_requestLocationUpdates: provider = " + provider + " listener = " + receiver);
//		}
//	} finally {
//		Binder.restoreCallingIdentity(identity);
//	}
	return NOERROR;
}

ECode CLocationManagerService::RemoveUpdates(
    /* [in] */ ILocationListener* listener)
{
//	try {
//		synchronized (mLock) {
	RemoveUpdatesLocked(GetReceiver(listener));
//		}
//	} catch (SecurityException se) {
//		throw se;
//	} catch (IllegalArgumentException iae) {
//		throw iae;
//	} catch (Exception e) {
//		Slog.e(TAG, "removeUpdates got exception:", e);
//	}

	return E_NOT_IMPLEMENTED;
}

ECode CLocationManagerService::RemoveUpdatesPI(
    /* [in] */ IPendingIntent* intent)
{
//	try {
//		synchronized (mLock) {
	RemoveUpdatesLocked(GetReceiver(intent));
//		}
//	} catch (SecurityException se) {
//		throw se;
//	} catch (IllegalArgumentException iae) {
//		throw iae;
//	} catch (Exception e) {
//		Slog.e(TAG, "removeUpdates got exception:", e);
//	}

	return E_NOT_IMPLEMENTED;
}

void CLocationManagerService::RemoveUpdatesLocked(
    /* [in] */ Receiver* receiver)
{
//	if (LOCAL_LOGV) {
//		Slog.v(TAG, "_removeUpdates: listener = " + receiver);
//	}

	// so wakelock calls will succeed
//    Int32 callingUid = Binder.getCallingUid();
//	Int64 identity = Binder.clearCallingIdentity();
//	try {
//	HashMap<AutoPtr<IInterface>, AutoPtr<Receiver> >::Iterator it = mReceivers.Find(receiver->mKey);
////	if (mReceivers.remove(receiver.mKey) != null && receiver.isListener()) {
//	if (it != mReceivers.End() && receiver->IsListener()) {
//		receiver->GetListener().asBinder().unlinkToDeath(receiver, 0);
//		synchronized(receiver) {
//		Mutex::Autolock lock(receiver->mSync);
//		if(receiver->mPendingBroadcasts > 0) {
//			DecrementPendingBroadcasts();
//			receiver->mPendingBroadcasts = 0;
//		}
//		}
//	}

	// Record which providers were associated with this listener
	HashSet<String>* providers = new HashSet<String>();
	HashMap<String,UpdateRecord*> oldRecords = receiver->mUpdateRecords;
//	if (oldRecords != null) {
		// Call dispose() on the obsolete update records.
	HashMap<String,UpdateRecord*>::Iterator oldit;
	for (oldit = oldRecords.Begin(); oldit != oldRecords.End(); ++oldit) {
		UpdateRecord* record = oldit->mSecond;
		if (!ProviderHasListener(record->mProvider, 0/*callingUid*/, receiver)) {
			AutoPtr<LocationProviderInterface> p;
			HashMap<String, AutoPtr<LocationProviderInterface> >::Iterator lpit = mProvidersByName.Find(record->mProvider);
			if (lpit != mProvidersByName.End()) {
				p = lpit->mSecond;
			}
			if (p != NULL) {
				p->RemoveListener(0/*callingUid*/);
			}
		}
		record->DisposeLocked();

		// Accumulate providers
		providers->Insert(oldit->mFirst);
	}
//	// Accumulate providers
//	providers->AddAll(oldRecords.keySet());
//	}

	// See if the providers associated with this listener have any
	// other listeners; if one does, inform it of the new smallest minTime
	// value; if one does not, disable location tracking for it
	HashSet<String>::Iterator providersIt;
	for (providersIt = providers->Begin(); providersIt != providers->End(); ++providersIt) {
		String provider = *providersIt;
		// If provider is already disabled, don't need to do anything
		if (!IsAllowedBySettingsLocked(provider)) {
			continue;
		}

		Boolean hasOtherListener = FALSE;
		List<UpdateRecord*>* recordsForProvider = NULL;
		HashMap<String, List<UpdateRecord*>* >::Iterator recordsForProviderIt = mRecordsByProvider.Find(provider);
		if (recordsForProviderIt != mRecordsByProvider.End()) {
			recordsForProvider = recordsForProviderIt->mSecond;
		}
		if (recordsForProvider != NULL && recordsForProvider->GetSize() > 0) {
			hasOtherListener = TRUE;
		}

		AutoPtr<LocationProviderInterface> p;
		HashMap<String, AutoPtr<LocationProviderInterface> >::Iterator lpit = mProvidersByName.Find(provider);
		if (lpit != mProvidersByName.End()) {
			p = lpit->mSecond;
		}
		if (p != NULL) {
			if (hasOtherListener) {
//				p->SetMinTime(getMinTimeLocked(provider), mTmpWorkSource);
			} else {
				p->EnableLocationTracking(FALSE);
			}
		}
	}
//	} finally {
//		Binder.restoreCallingIdentity(identity);
//	}
}

ECode CLocationManagerService::AddGpsStatusListener(
    /* [in] */ IGpsStatusListener* listener,
    /* [out] */ Boolean* result)
{
	VALIDATE_NOT_NULL(result);

	if (mGpsStatusProvider == NULL) {
		return FALSE;
	}
	Int32 perm;
	mContext->CheckCallingOrSelfPermission(String(ACCESS_FINE_LOCATION), &perm);
	if (perm != CapsuleManager_PERMISSION_GRANTED) {
		return E_SECURITY_EXCEPTION;
//		throw new SecurityException("Requires ACCESS_FINE_LOCATION permission");
	}

//	try {
	ECode ec = mGpsStatusProvider->AddGpsStatusListener(listener);
	if (ec == E_REMOTE_EXCEPTION) {
		*result = FALSE;
		return NOERROR;
	}
//	} catch (RemoteException e) {
//		Slog.e(TAG, "mGpsStatusProvider.addGpsStatusListener failed", e);
//		return false;
//	}
	*result = TRUE;

	return NOERROR;
}

ECode CLocationManagerService::RemoveGpsStatusListener(
    /* [in] */ IGpsStatusListener* listener)
{
//	synchronized (mLock) {
//		try {
	return mGpsStatusProvider->RemoveGpsStatusListener(listener);
//		} catch (Exception e) {
//			Slog.e(TAG, "mGpsStatusProvider.removeGpsStatusListener failed", e);
//		}
//	}
}

ECode CLocationManagerService::SendExtraCommand(
    /* [in] */ const String& provider,
    /* [in] */ const String& command,
    /* [in] */ IBundle* inExtras, //inout
    /* [out] */ IBundle** outExtras,
    /* [out] */ Boolean* result)
{
	VALIDATE_NOT_NULL(outExtras);
	VALIDATE_NOT_NULL(result);

	if (provider.IsNull()) {
		// throw NullPointerException to remain compatible with previous implementation
		return E_NULL_POINTER_EXCEPTION;
//		throw new NullPointerException();
	}

	// first check for permission to the provider
	CheckPermissionsSafe(provider);
	// and check for ACCESS_LOCATION_EXTRA_COMMANDS
	Int32 perm;
	mContext->CheckCallingOrSelfPermission(String(ACCESS_LOCATION_EXTRA_COMMANDS), &perm);
	if (perm != CapsuleManager_PERMISSION_GRANTED) {
		return E_SECURITY_EXCEPTION;
//		throw new SecurityException("Requires ACCESS_LOCATION_EXTRA_COMMANDS permission");
	}

//	synchronized (mLock) {
	AutoPtr<LocationProviderInterface> p;
	HashMap<String, AutoPtr<LocationProviderInterface> >::Iterator it = mProvidersByName.Find(provider);
	if (it != mProvidersByName.End()) {
		p = it->mSecond;
	}
	if (p == NULL) {
		*outExtras = inExtras;
		*result = FALSE;
	}

    *result = p->SendExtraCommand(command, inExtras, outExtras);
//	}

	return NOERROR;
}

// for NI support
ECode CLocationManagerService::SendNiResponse(
    /* [in] */ Int32 notifId,
    /* [in] */ Int32 userResponse,
    /* [out] */ Boolean* result)
{
//	if (Binder.getCallingUid() != Process.myUid()) {
//		throw new SecurityException(
//				"calling sendNiResponse from outside of the system is not allowed");
//	}
//	try {
//	return mNetInitiatedListener.sendNiResponse(notifId, userResponse);
//	}
//	catch (RemoteException e)
//	{
//		Slog.e(TAG, "RemoteException in LocationManagerService.sendNiResponse");
//		return false;
//	}

	return E_NOT_IMPLEMENTED;
}

CLocationManagerService::ProximityAlert::ProximityAlert(
	/* [in] */ Int32 uid,
	/* [in] */ Double latitude,
	/* [in] */ Double longitude,
	/* [in] */ Float radius,
	/* [in] */ Int64 expiration,
	/* [in] */ IPendingIntent* intent)
    : mUid(uid),
	  mLatitude(latitude),
	  mLongitude(longitude),
	  mRadius(radius),
	  mExpiration(expiration),
	  mIntent(intent)
{
	CLocation::New(String(""), (ILocation**)&mLocation);
	mLocation->SetLatitude(latitude);
	mLocation->SetLongitude(longitude);
}

PInterface CLocationManagerService::ProximityAlert::Probe(
    /* [in]  */ REIID riid)
{
    return NULL;
}

UInt32 CLocationManagerService::ProximityAlert::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CLocationManagerService::ProximityAlert::Release()
{
    return ElRefBase::Release();
}

ECode CLocationManagerService::ProximityAlert::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    return E_NOT_IMPLEMENTED;
}

Int64 CLocationManagerService::ProximityAlert::GetExpiration()
{
	return mExpiration;
}

AutoPtr<IPendingIntent> CLocationManagerService::ProximityAlert::GetIntent()
{
	return mIntent;
}

Boolean CLocationManagerService::ProximityAlert::IsInProximity(
	/* [in] */ Double latitude,
	/* [in] */ Double longitude,
	/* [in] */ Float accuracy)
{
	AutoPtr<ILocation> loc;
	CLocation::New(String(""), (ILocation**)&loc);
	loc->SetLatitude(latitude);
	loc->SetLongitude(longitude);

	Double radius;
	loc->DistanceTo(mLocation, (Float*)&radius);
	return radius <= Math::Max(mRadius,accuracy);
}

String CLocationManagerService::ProximityAlert::ToString()
{
    return String((const char*)(StringBuffer("ProximityAlert{")
        + this /*Integer.toHexString(System.identityHashCode(this))*/
        + " uid " + mUid + "/" + mIntent + "}"));
}

Int32 CLocationManagerService::ProximityAlert::GetHashCode() const
{
    return (Int32)this;
}

CLocationManagerService::ProximityListener::ProximityListener(
    /* [in] */ CLocationManagerService* locMS)
    : mLocationManagerService(locMS)
    , mIsGpsAvailable(FALSE)
{
}

PInterface CLocationManagerService::ProximityListener::Probe(
    /* [in]  */ REIID riid)
{
	if (riid == EIID_IInterface) {
		return (IInterface*)this;
	}
	else if (riid == EIID_ILocationListener) {
		return (ILocationListener*)this;
	}

	return NULL;
}

UInt32 CLocationManagerService::ProximityListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CLocationManagerService::ProximityListener::Release()
{
    return ElRefBase::Release();
}

ECode CLocationManagerService::ProximityListener::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManagerService::ProximityListener::OnLocationChanged(
    /* [in] */ ILocation* loc)
{
	// If Gps is available, then ignore updates from NetworkLocationProvider
	String provider;
	loc->GetProvider(&provider);
	if (provider == String(LocationManager_GPS_PROVIDER)) {
		mIsGpsAvailable = TRUE;
	}
	if (mIsGpsAvailable && provider == String(LocationManager_NETWORK_PROVIDER)) {
		return NOERROR;
	}

	// Process proximity alerts
//	Int64 now = System.currentTimeMillis();
	Double latitude, longitude;
	loc->GetLatitude(&latitude);
	loc->GetLongitude(&longitude);
	Float accuracy;
    loc->GetAccuracy(&accuracy);
	List<AutoPtr<IPendingIntent> >* intentsToRemove = NULL;

	HashMap<AutoPtr<IPendingIntent>, AutoPtr<ProximityAlert> >::Iterator it;
	for (it = mLocationManagerService->mProximityAlerts.Begin(); it != mLocationManagerService->mProximityAlerts.End(); ++it) {
		AutoPtr<ProximityAlert> alert = it->mSecond;
		AutoPtr<IPendingIntent> intent = alert->GetIntent();
		Int64 expiration = alert->GetExpiration();

		if (expiration == -1/* || (now <= expiration)*/) {
			Boolean entered = FALSE;
			HashSet<AutoPtr<ProximityAlert>, ProximityAlert::HashKey>::Iterator hsIt =
					mLocationManagerService->mProximitiesEntered.Find(alert);
	        if (hsIt != mLocationManagerService->mProximitiesEntered.End()) {
	        	entered = TRUE;
	        }
			Boolean inProximity =
				alert->IsInProximity(latitude, longitude, accuracy);
			if (!entered && inProximity) {
//				if (LOCAL_LOGV) {
//					Slog.v(TAG, "Entered alert");
//				}
				mLocationManagerService->mProximitiesEntered.Insert(alert);
				AutoPtr<IIntent> enteredIntent;
				CIntent::New((IIntent**)&enteredIntent);
				enteredIntent->PutBooleanExtra(String(LocationManager_KEY_PROXIMITY_ENTERING), TRUE);
//				try {
				{
					Mutex::Autolock lock(mSync);
					// synchronize to ensure incrementPendingBroadcasts()
					// is called before decrementPendingBroadcasts()
//					ECode ec = intent->Send(mContext, 0, enteredIntent, this, mLocationHandler);
//					if (ec == E_CANCELED_EXCEPTION) {
//						if (LOCAL_LOGV) {
//							Slog.v(TAG, "Canceled proximity alert: " + alert, e);
//						}
//						if (intentsToRemove == NULL) {
//							intentsToRemove = new List<AutoPtr<IPendingIntent> >();
//						}
//						intentsToRemove->Inset(intent);
//					}
					// call this after broadcasting so we do not increment
					// if we throw an exeption.
					mLocationManagerService->IncrementPendingBroadcasts();
				}
//				} catch (PendingIntent.CanceledException e) {
//					if (LOCAL_LOGV) {
//						Slog.v(TAG, "Canceled proximity alert: " + alert, e);
//					}
//					if (intentsToRemove == null) {
//						intentsToRemove = new ArrayList<PendingIntent>();
//					}
//					intentsToRemove.add(intent);
//				}
			}
			else if (entered && !inProximity) {
//				if (LOCAL_LOGV) {
//					Slog.v(TAG, "Exited alert");
//				}
				mLocationManagerService->mProximitiesEntered.Erase(alert);
				AutoPtr<IIntent> exitedIntent;
				CIntent::New((IIntent**)&exitedIntent);
				exitedIntent->PutBooleanExtra(String(LocationManager_KEY_PROXIMITY_ENTERING), FALSE);
//				try {
				{
					Mutex::Autolock lock(mSync);
					// synchronize to ensure incrementPendingBroadcasts()
					// is called before decrementPendingBroadcasts()
//					ECode ec = intent->Send(mContext, 0, exitedIntent, this, mLocationHandler);
//					if (ec == E_CANCELED_EXCEPTION) {
//						if (LOCAL_LOGV) {
//							Slog.v(TAG, "Canceled proximity alert: " + alert, e);
//						}
//						if (intentsToRemove == NULL) {
//							intentsToRemove = intentsToRemove = new List<PendingIntent>();
//						}
//						intentsToRemove->Insert(intent);
//					}
					// call this after broadcasting so we do not increment
					// if we throw an exeption.
					mLocationManagerService->IncrementPendingBroadcasts();
				}
//				} catch (PendingIntent.CanceledException e) {
//					if (LOCAL_LOGV) {
//						Slog.v(TAG, "Canceled proximity alert: " + alert, e);
//					}
//					if (intentsToRemove == null) {
//						intentsToRemove = new ArrayList<PendingIntent>();
//					}
//					intentsToRemove.add(intent);
//				}
			}
		}
		else {
			// Mark alert for expiration
//			if (LOCAL_LOGV) {
//				Slog.v(TAG, "Expiring proximity alert: " + alert);
//			}
			if (intentsToRemove == NULL) {
				intentsToRemove = new List<AutoPtr<IPendingIntent> >();
			}
			intentsToRemove->PushBack(alert->GetIntent());
		}
	}

	// Remove expired alerts
	if (intentsToRemove != NULL) {
		List<AutoPtr<IPendingIntent> >::Iterator intentIt;
		for (intentIt = intentsToRemove->Begin(); intentIt != intentsToRemove->End(); ++intentIt) {
			AutoPtr<IPendingIntent> i = *intentIt;
			AutoPtr<ProximityAlert> alert;
			HashMap<AutoPtr<IPendingIntent>, AutoPtr<ProximityAlert> >::Iterator alertIt = mLocationManagerService->mProximityAlerts.Find(i);
			if (alertIt != mLocationManagerService->mProximityAlerts.End()) {
				alert = alertIt->mSecond;
			}
			mLocationManagerService->mProximitiesEntered.Erase(alert);
			mLocationManagerService->RemoveProximityAlertLocked(i);
		}
	}

	return NOERROR;
}

// Note: this is called with the lock held.
ECode CLocationManagerService::ProximityListener::OnProviderDisabled(
    /* [in] */ const String& provider)
{
	if (provider == String(LocationManager_GPS_PROVIDER)) {
		mIsGpsAvailable = FALSE;
	}

	return NOERROR;
}

// Note: this is called with the lock held.
ECode CLocationManagerService::ProximityListener::OnProviderEnabled(
    /* [in] */ const String& provider)
{
	// ignore
	return NOERROR;
}

// Note: this is called with the lock held.
ECode CLocationManagerService::ProximityListener::OnStatusChanged(
	/* [in] */ const String& provider,
	/* [in] */ Int32 status,
	/* [in] */ IBundle* extras)
{
	if ((provider == String(LocationManager_GPS_PROVIDER)) &&
		(status != LocationProvider_AVAILABLE)) {
		mIsGpsAvailable = FALSE;
	}

	return NOERROR;
}

//ECode CLocationManagerService::ProximityListener::OnSendFinished(
//	/* [in] */ IPendingIntent* pendingIntent,
//	/* [in] */ IIntent* intent,
//	/* [in] */ Int32 resultCode,
//	/* [in] */ String resultData,
//	/* [in] */ IBundle* resultExtras)
//{
//	return E_NOT_IMPLEMENTED;
//}

ECode CLocationManagerService::AddProximityAlert(
    /* [in] */ Double latitude,
    /* [in] */ Double longitude,
    /* [in] */ Float radius,
    /* [in] */ Int64 expiration,
    /* [in] */ IPendingIntent* intent)
{
//	try {
//		synchronized (mLock) {
	return AddProximityAlertLocked(latitude, longitude, radius, expiration, intent);
//		}
//	} catch (SecurityException se) {
//		throw se;
//	} catch (IllegalArgumentException iae) {
//		throw iae;
//	} catch (Exception e) {
//		Slog.e(TAG, "addProximityAlert got exception:", e);
//	}
}

ECode CLocationManagerService::AddProximityAlertLocked(
	/* [in] */ Double latitude,
	/* [in] */ Double longitude,
	/* [in] */ Float radius,
	/* [in] */ Int64 expiration,
	/* [in] */ IPendingIntent* intent)
{
//	if (LOCAL_LOGV) {
//		Slog.v(TAG, "addProximityAlert: latitude = " + latitude +
//				", longitude = " + longitude +
//				", expiration = " + expiration +
//				", intent = " + intent);
//	}

	// Require ability to access all providers for now
	if (!IsAllowedProviderSafe(String(LocationManager_GPS_PROVIDER)) ||
		!IsAllowedProviderSafe(String(LocationManager_NETWORK_PROVIDER))) {
		return E_SECURITY_EXCEPTION;
//		throw new SecurityException("Requires ACCESS_FINE_LOCATION permission");
	}

	if (expiration != -1) {
//		expiration += System.currentTimeMillis();
	}
	AutoPtr<ProximityAlert> alert = new ProximityAlert(0/*Binder.getCallingUid()*/,
			latitude, longitude, radius, expiration, intent);
	mProximityAlerts[intent] = alert;

	if (mProximityReceiver == NULL) {
		ProximityListener* listener = new ProximityListener(this);
		mProximityListener = (ILocationListener*)listener->Probe(EIID_ILocationListener);
		mProximityReceiver = new Receiver(this, mProximityListener);

		List<AutoPtr<LocationProviderInterface> >::ReverseIterator rit;
		for (rit = mProviders.RBegin(); rit != mProviders.REnd(); ++rit) {
			AutoPtr<LocationProviderInterface> provider = *rit;
			RequestLocationUpdatesLocked(provider->GetName(), 1000, 1.0f,
					FALSE, mProximityReceiver);
		}
	}

	return NOERROR;
}

ECode CLocationManagerService::RemoveProximityAlert(
    /* [in] */ IPendingIntent* intent)
{
//	try {
//		synchronized (mLock) {
   return RemoveProximityAlertLocked(intent);
//		}
//	} catch (SecurityException se) {
//		throw se;
//	} catch (IllegalArgumentException iae) {
//		throw iae;
//	} catch (Exception e) {
//		Slog.e(TAG, "removeProximityAlert got exception:", e);
//	}

	return E_NOT_IMPLEMENTED;
}

ECode CLocationManagerService::RemoveProximityAlertLocked(
    /* [in] */ IPendingIntent* intent)
{
//	if (LOCAL_LOGV) {
//		Slog.v(TAG, "removeProximityAlert: intent = " + intent);
//	}

	mProximityAlerts.Erase(intent);
	if (mProximityAlerts.Begin() == mProximityAlerts.End()) {
		RemoveUpdatesLocked(mProximityReceiver);
		mProximityReceiver = NULL;
		mProximityListener = NULL;
	}

	return NOERROR;
}

ECode CLocationManagerService::GetProviderInfo(
    /* [in] */ const String& provider,
    /* [out] */ IBundle** info)
{
//	try {
//		synchronized (mLock) {
	return _GetProviderInfoLocked(provider, info);
//		}
//	} catch (SecurityException se) {
//		throw se;
//	} catch (IllegalArgumentException iae) {
//		throw iae;
//	} catch (Exception e) {
//		Slog.e(TAG, "_getProviderInfo got exception:", e);
//		return null;
//	}
}

ECode CLocationManagerService::_GetProviderInfoLocked(
	/* [in] */ String provider,
	/* [out] */ IBundle** info)
{
	return E_NOT_IMPLEMENTED;
}

ECode CLocationManagerService::IsProviderEnabled(
    /* [in] */ const String& provider,
    /* [out] */ Boolean* isEnabled)
{
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManagerService::GetLastKnownLocation(
    /* [in] */ const String& provider,
    /* [out] */ ILocation** location)
{
    return E_NOT_IMPLEMENTED;
}

// Used by location providers to tell the location manager when it has a new location.
// Passive is true if the location is coming from the passive provider, in which case
// it need not be shared with other providers.
ECode CLocationManagerService::ReportLocation(
    /* [in] */ ILocation* location,
    /* [in] */ Boolean passive)
{
    return E_NOT_IMPLEMENTED;
}

void CLocationManagerService::IncrementPendingBroadcasts()
{
}

void CLocationManagerService::DecrementPendingBroadcasts()
{
}

ECode CLocationManagerService::GeocoderIsPresent()
{
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManagerService::GetFromLocation(
    /* [in] */ Double latitude,
    /* [in] */ Double longitude,
    /* [in] */ Int32 maxResults,
    /* [in] */ IGeocoderParams* params,
    /* [out] */ IObjectContainer** addrs,
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManagerService::GetFromLocationName(
    /* [in] */ const String& locationName,
    /* [in] */ Double lowerLeftLatitude,
    /* [in] */ Double lowerLeftLongitude,
    /* [in] */ Double upperRightLatitude,
    /* [in] */ Double upperRightLongitude,
    /* [in] */ Int32 maxResults,
    /* [in] */ IGeocoderParams* params,
    /* [out] */ IObjectContainer** addrs,
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManagerService::AddTestProvider(
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
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManagerService::RemoveTestProvider(
    /* [in] */ const String& provider)
{
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManagerService::SetTestProviderLocation(
    /* [in] */ const String& provider,
    /* [in] */ ILocation* loc)
{
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManagerService::ClearTestProviderLocation(
    /* [in] */ const String& provider)
{
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManagerService::SetTestProviderEnabled(
    /* [in] */ const String& provider,
    /* [in] */ Boolean enabled)
{
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManagerService::ClearTestProviderEnabled(
    /* [in] */ const String& provider)
{
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManagerService::SetTestProviderStatus(
    /* [in] */ const String& provider,
    /* [in] */ Int32 status,
    /* [in] */ IBundle* extras,
    /* [in] */ Int64 updateTime)
{
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManagerService::ClearTestProviderStatus(
    /* [in] */ const String& provider)
{
    return E_NOT_IMPLEMENTED;
}


