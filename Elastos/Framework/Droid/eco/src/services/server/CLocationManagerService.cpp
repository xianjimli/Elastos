
#include "server/CLocationManagerService.h"
#include "server/location/GpsLocationProvider.h"
#include "server/location/PassiveProvider.h"
#include "os/Binder.h"
#include "os/Process.h"
#include <StringBuffer.h>
#include <elastos/Math.h>
#include <elastos/System.h>


CLocationManagerService::Receiver::Receiver(
    /* [in] */ CLocationManagerService* host,
    /* [in] */ ILocationListener* listener)
    : mHost(host)
    , mListener(listener)
    , mKey((IInterface*)listener)
    , mUpdateRecords(5)
    , mPendingBroadcasts(0)
{}

CLocationManagerService::Receiver::Receiver(
    /* [in] */ CLocationManagerService* host,
    /* [in] */ IPendingIntent* intent)
    : mHost(host)
    , mPendingIntent(intent)
    , mKey((IInterface*)intent)
    , mUpdateRecords(5)
    , mPendingBroadcasts(0)
{}

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
    VALIDATE_NOT_NULL(locationListener);

    if (mListener != NULL) {
        *locationListener = mListener;
        (*locationListener)->AddRef();
        return NOERROR;
    }

//    throw new IllegalStateException("Request for non-existent listener");
    return E_ILLEGAL_STATE_EXCEPTION;
}

ECode CLocationManagerService::Receiver::GetPendingIntent(
    /* [out] */ IPendingIntent** pendingIntent)
{
    VALIDATE_NOT_NULL(pendingIntent);

    if (mPendingIntent != NULL) {
        *pendingIntent = mPendingIntent;
        (*pendingIntent)->AddRef();
        return NOERROR;
    }

//    throw new IllegalStateException("Request for non-existent intent");
    return E_ILLEGAL_STATE_EXCEPTION;
}

Boolean CLocationManagerService::Receiver::CallStatusChangedLocked(
    /* [in] */ const String& provider,
    /* [in] */ Int32 status,
    /* [in] */ IBundle* extras)
{
    if (mListener != NULL) {
//        try {
        Mutex::Autolock lock(mLock);

        // synchronize to ensure incrementPendingBroadcastsLocked()
        // is called before decrementPendingBroadcasts()
        ECode ec = mListener->OnStatusChanged(provider, status, extras);
        if (FAILED(ec)) {
            return FALSE;
        }

        if (mListener != mHost->mProximityListener) {
            // call this after broadcasting so we do not increment
            // if we throw an exeption.
            IncrementPendingBroadcastsLocked();
        }
//        } catch (RemoteException e) {
//            return false;
//        }
    }
    else {
        AutoPtr<IIntent> statusChanged;
        CIntent::New((IIntent**)&statusChanged);
        statusChanged->PutExtrasEx(extras);
        statusChanged->PutBooleanExtra(String(LocationManager_KEY_STATUS_CHANGED), status);
//        try {
        Mutex::Autolock lock(mLock);
        // synchronize to ensure incrementPendingBroadcastsLocked()
        // is called before decrementPendingBroadcasts()
//        mPendingIntent->Send(mContext, 0, statusChanged, this, mLocationHandler);
        // call this after broadcasting so we do not increment
        // if we throw an exeption.
        IncrementPendingBroadcastsLocked();
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
        Mutex::Autolock lock(mLock);
        // synchronize to ensure incrementPendingBroadcastsLocked()
        // is called before decrementPendingBroadcasts()
        ECode ec = mListener->OnLocationChanged(location);
        if (FAILED(ec)) {
            return FALSE;
        }

        if (mListener != mHost->mProximityListener) {
            // call this after broadcasting so we do not increment
            // if we throw an exeption.
            IncrementPendingBroadcastsLocked();
        }
//        } catch (RemoteException e) {
//            return false;
//        }
    }
    else {
        AutoPtr<IIntent> locationChanged;
        CIntent::New((IIntent**)&locationChanged);
        locationChanged->PutParcelableExtra(String(LocationManager_KEY_LOCATION_CHANGED), IParcelable::Probe(location));
//        try {
        Mutex::Autolock lock(mLock);
        // synchronize to ensure incrementPendingBroadcastsLocked()
        // is called before decrementPendingBroadcasts()
//        mPendingIntent.send(mContext, 0, locationChanged, this, mLocationHandler);
        // call this after broadcasting so we do not increment
        // if we throw an exeption.
        IncrementPendingBroadcastsLocked();
//        } catch (PendingIntent.CanceledException e) {
//            return false;
//        }
    }
    return TRUE;
}

Boolean CLocationManagerService::Receiver::CallProviderEnabledLocked(
    /* [in] */ const String& provider,
    /* [in] */ Boolean enabled)
{
    if (mListener != NULL) {
//        try {
        Mutex::Autolock lock(mLock);
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

        if (mListener != mHost->mProximityListener) {
            // call this after broadcasting so we do not increment
            // if we throw an exeption.
            IncrementPendingBroadcastsLocked();
        }
//        } catch (RemoteException e) {
//            return false;
//        }
    }
    else {
        AutoPtr<IIntent> providerIntent;
        CIntent::New((IIntent**)&providerIntent);
        providerIntent->PutBooleanExtra(String(LocationManager_KEY_PROVIDER_ENABLED), enabled);
//        try {
        Mutex::Autolock lock(mLock);
        // synchronize to ensure incrementPendingBroadcastsLocked()
        // is called before decrementPendingBroadcasts()
//        mPendingIntent.send(mContext, 0, providerIntent, this, mLocationHandler);
        // call this after broadcasting so we do not increment
        // if we throw an exeption.
        IncrementPendingBroadcastsLocked();
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
    {
        Mutex::Autolock lock(mHost->mLock);
        mHost->RemoveUpdatesLocked(this);
    }

    Mutex::Autolock lock(mLock);
    if (mPendingBroadcasts > 0) {
        mHost->DecrementPendingBroadcasts();
        mPendingBroadcasts = 0;
    }
}

void CLocationManagerService::Receiver::OnSendFinished(
    /* [in] */ IPendingIntent* pendingIntent,
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 resultCode,
    /* [in] */ const String& resultData,
    /* [in] */ IBundle* resultExtras)
{
    Mutex::Autolock lock(mLock);
    DecrementPendingBroadcastsLocked();
}

// this must be called while synchronized by caller in a synchronized block
// containing the sending of the broadcaset
void CLocationManagerService::Receiver::IncrementPendingBroadcastsLocked()
{
    if (mPendingBroadcasts++ == 0) {
        mHost->IncrementPendingBroadcasts();
    }
}

void CLocationManagerService::Receiver::DecrementPendingBroadcastsLocked()
{
    if (--mPendingBroadcasts == 0) {
        mHost->DecrementPendingBroadcasts();
    }
}


// private final class SettingsObserver implements Observer {
//     public void update(Observable o, Object arg) {
//         synchronized (mLock) {
//             updateProvidersLocked();
//         }
//     }
// }


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

    assert(object1 != NULL && object2 != NULL);
    ILocationProviderInterface* firstObj = ILocationProviderInterface::Probe(object1);
    ILocationProviderInterface* secondObj = ILocationProviderInterface::Probe(object2);

    assert(firstObj != NULL && secondObj != NULL);
    Int32 r1, r2;
    firstObj->GetPowerRequirement(&r1);
    secondObj->GetPowerRequirement(&r2);
    *result = r1 - r2;
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

    assert(object1 != NULL && object2 != NULL);
    ILocationProviderInterface* firstObj = ILocationProviderInterface::Probe(object1);
    ILocationProviderInterface* secondObj = ILocationProviderInterface::Probe(object2);

    assert(firstObj != NULL && secondObj != NULL);
    Int32 a1, a2;
    firstObj->GetAccuracy(&a1);
    secondObj->GetAccuracy(&a2);
    *result = a1 - a2;
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
    /* [in] */ ILocationProviderInterface* p)
{
    Int32 score;
    Boolean supported;
    p->SupportsAltitude(&supported);
    score = supported ? ALTITUDE_SCORE : 0;
    p->SupportsBearing(&supported);
    score += supported ? BEARING_SCORE : 0;
    p->SupportsSpeed(&supported);
    score += supported ? SPEED_SCORE : 0;
    return score;
}

ECode CLocationManagerService::LpCapabilityComparator::Compare(
    /* [in] */ IInterface* object1,
    /* [in] */ IInterface* object2,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    assert(object1 != NULL && object2 != NULL);
    ILocationProviderInterface* firstObj = ILocationProviderInterface::Probe(object1);
    ILocationProviderInterface* secondObj = ILocationProviderInterface::Probe(object2);

    assert(firstObj != NULL && secondObj != NULL);
    *result = Score(firstObj) - Score(secondObj);// Bigger is better
    return NOERROR;
}

ECode CLocationManagerService::LpCapabilityComparator::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* isEqual)
{
//    return (score(l1) == score(l2));
    return E_NOT_IMPLEMENTED;
}


CLocationManagerService::UpdateRecord::UpdateRecord(
    /* [in] */ CLocationManagerService* lms,
    /* [in] */ const String& provider,
    /* [in] */ Int64 minTime,
    /* [in] */ Float minDistance,
    /* [in] */ Boolean singleShot,
    /* [in] */ Receiver* receiver,
    /* [in] */ Int32 uid)
    : mHost(lms)
    , mProvider(provider)
    , mReceiver(receiver)
    , mMinTime(minTime)
    , mMinDistance(minDistance)
    , mSingleShot(singleShot)
    , mUid(uid)
{
    List< AutoPtr<UpdateRecord> >* records = NULL;
    HashMap<String, List< AutoPtr<UpdateRecord> >* >::Iterator hit =
            mHost->mRecordsByProvider.Find(provider);
    if (hit != mHost->mRecordsByProvider.End()) {
        records = hit->mSecond;
    }
    if (records == NULL) {
        records = new List< AutoPtr<UpdateRecord> >();
        mHost->mRecordsByProvider[provider] = records;
    }

    List< AutoPtr<UpdateRecord> >::Iterator lit = Find(records->Begin(),
            records->End(), AutoPtr<UpdateRecord>(this));
    if (lit == records->End()) {
        records->PushBack(AutoPtr<UpdateRecord>(this));
    }
}

void CLocationManagerService::UpdateRecord::DisposeLocked()
{
    List< AutoPtr<UpdateRecord> >* records = NULL;
    HashMap<String, List< AutoPtr<UpdateRecord> >* >::Iterator it =
            mHost->mRecordsByProvider.Find(mProvider);
    if (it != mHost->mRecordsByProvider.End()) {
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


CLocationManagerService::ProximityAlert::ProximityAlert(
    /* [in] */ Int32 uid,
    /* [in] */ Double latitude,
    /* [in] */ Double longitude,
    /* [in] */ Float radius,
    /* [in] */ Int64 expiration,
    /* [in] */ IPendingIntent* intent)
    : mUid(uid)
    , mLatitude(latitude)
    , mLongitude(longitude)
    , mRadius(radius)
    , mExpiration(expiration)
    , mIntent(intent)
{
    ASSERT_SUCCEEDED(CLocation::New(String(""), (ILocation**)&mLocation));
    mLocation->SetLatitude(latitude);
    mLocation->SetLongitude(longitude);
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
    ASSERT_SUCCEEDED(CLocation::New(String(""), (ILocation**)&loc));
    loc->SetLatitude(latitude);
    loc->SetLongitude(longitude);

    Double radius;
    loc->DistanceTo(mLocation, (Float*)&radius);
    return radius <= Math::Max(mRadius, accuracy);
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

CLocationManagerService::CLocationManagerService()
    : mLastWriteTime(7)
    , mEnabledProviders(7)
    , mDisabledProviders(7)
    , mMockProviders(7)
    , mPendingBroadcasts(0)
    , mReceivers(7)
    , mProvidersByName(7)
    , mRecordsByProvider(7)
    , mProximityAlerts(7)
    , mProximitiesEntered(7)
    , mLastKnownLocation(7)
    , mNetworkState(LocationProvider_TEMPORARILY_UNAVAILABLE)
{
    ASSERT_SUCCEEDED(CWorkSource::New((IWorkSource**)&mTmpWorkSource));
}

ECode CLocationManagerService::constructor(
    /* [in] */ IContext* ctx)
{
    mContext = ctx;
    AutoPtr<IResources> resources;
    ctx->GetResources((IResources**)&resources);
    resources->GetString(0x01040019, /*com.android.internal.R.string.config_networkLocationProvider*/
            &mNetworkLocationProviderPackageName);
    resources->GetString(0x0104001a, /*com.android.internal.R.string.config_geocodeProvider*/
            &mGeocodeProviderPackageName);
//    mPackageMonitor.register(context, TRUE);

//    if (LOCAL_LOGV) {
//        Slog.v(TAG, "Constructed LocationManager Service");
//    }
    return NOERROR;
}

ECode CLocationManagerService::LocationCallbackFinished(
    /* [in] */ ILocationListener* listener)
{
    //Do not use getReceiver here as that will add the ILocationListener to
    //the receiver list if it is not found.  If it is not found then the
    //LocationListener was removed when it had a pending broadcast and should
    //not be added back.
    Receiver* receiver = NULL;
    HashMap<AutoPtr<IInterface>, AutoPtr<Receiver> >::Iterator it =
            mReceivers.Find((IInterface*)listener);
    if (it != mReceivers.End()) {
        receiver = it->mSecond;
    }
    if (receiver != NULL) {
        Mutex::Autolock lock(receiver->mLock);
            // so wakelock calls will succeed
        Int64 identity = Binder::ClearCallingIdentity();
        receiver->DecrementPendingBroadcastsLocked();
        Binder::RestoreCallingIdentity(identity);
    }
    return NOERROR;
}

void CLocationManagerService::AddProvider(
    /* [in] */ ILocationProviderInterface* provider)
{
    mProviders.PushBack(provider);
    String name;
    provider->GetName(&name);
    mProvidersByName[name] = provider;
}

void CLocationManagerService::RemoveProvider(
    /* [in] */ ILocationProviderInterface* provider)
{
    List< AutoPtr<ILocationProviderInterface> >::Iterator it =
            Find(mProviders.Begin(), mProviders.End(), provider);
    mProviders.Erase(it);
    String name;
    provider->GetName(&name);
    mProvidersByName.Erase(name);
}

void CLocationManagerService::LoadProviders()
{
    Mutex::Autolock lock(&mLock);

    if (sProvidersLoaded) {
        return;
    }

    // Load providers
    LoadProvidersLocked();
    sProvidersLoaded = TRUE;
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
        GpsLocationProvider* gpsProvider = new GpsLocationProvider(mContext, (ILocationManager*)this);
        mGpsStatusProvider = gpsProvider->GetGpsStatusProvider();
//        mNetInitiatedListener = gpsProvider.getNetInitiatedListener();
        AddProvider((ILocationProviderInterface*)gpsProvider);
        mGpsLocationProvider = (ILocationProviderInterface*)gpsProvider;
    }

    // create a passive location provider, which is always enabled
    PassiveProvider* passiveProvider = new PassiveProvider((ILocationManager*)this);
    AddProvider((ILocationProviderInterface*)passiveProvider);
    String name;
    passiveProvider->GetName(&name);
    mEnabledProviders.Insert(name);

    // initialize external network location and geocoder services
    if (!mNetworkLocationProviderPackageName.IsNull()) {
        mNetworkLocationProvider =
            new LocationProviderProxy(mContext, String(LocationManager_NETWORK_PROVIDER),
                    mNetworkLocationProviderPackageName, mLocationHandler);
        AddProvider((ILocationProviderInterface*)mNetworkLocationProvider.Get());
    }

    if (!mGeocodeProviderPackageName.IsNull()) {
        mGeocodeProvider = new GeocoderProxy(mContext, mGeocodeProviderPackageName);
    }

    UpdateProvidersLocked();
}

void CLocationManagerService::SystemReady()
{
    // we defer starting up the service until the system is ready
    CThread::New(NULL, (IRunnable*)this, String("CLocationManagerService"), (IThread**)&mThread);
    mThread->Start();
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
    intentFilter->AddAction(String(ConnectivityManager_CONNECTIVITY_ACTION));
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
    // Process.setThreadPriority(Process.THREAD_PRIORITY_BACKGROUND);
    CApartment::New(FALSE, (IApartment**)(&mLocationHandler));
    Initialize();
    return mLocationHandler->Start(ApartmentAttr_Current);
}

Boolean CLocationManagerService::IsAllowedBySettingsLocked(
    /* [in] */ const String& provider)
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
    return FALSE;
}

ECode CLocationManagerService::CheckPermissionsSafe(
    /* [in] */ const String& provider)
{
    if (CString(LocationManager_GPS_PROVIDER).Equals(provider)
             || CString(LocationManager_PASSIVE_PROVIDER).Equals(provider)) {
        Int32 perm;
        mContext->CheckCallingOrSelfPermission(String(ACCESS_FINE_LOCATION), &perm);
        if (perm != CapsuleManager_PERMISSION_GRANTED) {
//        throw new SecurityException("Provider " + provider
//                + " requires ACCESS_FINE_LOCATION permission");
            return E_SECURITY_EXCEPTION;
        }
    }
    if (CString(LocationManager_NETWORK_PROVIDER).Equals(provider)) {
        Int32 perm;
        mContext->CheckCallingOrSelfPermission(String(ACCESS_FINE_LOCATION), &perm);
        if (perm == CapsuleManager_PERMISSION_GRANTED) {
            return NOERROR;
        }
        mContext->CheckCallingOrSelfPermission(String(ACCESS_COARSE_LOCATION), &perm);
        if (perm != CapsuleManager_PERMISSION_GRANTED) {
//        throw new SecurityException("Provider " + provider
//                + " requires ACCESS_FINE_LOCATION or ACCESS_COARSE_LOCATION permission");
            return E_SECURITY_EXCEPTION;
        }
    }
    return NOERROR;
}

Boolean CLocationManagerService::IsAllowedProviderSafe(
    /* [in] */ const String& provider)
{
    if (CString(LocationManager_GPS_PROVIDER).Equals(provider)
            || CString(LocationManager_PASSIVE_PROVIDER).Equals(provider)) {
        Int32 perm;
        mContext->CheckCallingOrSelfPermission(String(ACCESS_FINE_LOCATION), &perm);
        if (perm != CapsuleManager_PERMISSION_GRANTED ) {
            return FALSE;
        }
    }
    if (CString(LocationManager_NETWORK_PROVIDER).Equals(provider)) {
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
    /* [out] */ IObjectContainer** allProviders)
{
    VALIDATE_NOT_NULL(allProviders);
//    try {
    Mutex::Autolock lock(mLock);

    return _GetAllProvidersLocked(allProviders);
//    } catch (SecurityException se) {
//        throw se;
//    } catch (Exception e) {
//        Slog.e(TAG, "getAllProviders got exception:", e);
//        return null;
//    }
}

ECode CLocationManagerService::_GetAllProvidersLocked(
    /* [out] */ IObjectContainer** allProviders)
{
//    if (LOCAL_LOGV) {
//        Slog.v(TAG, "getAllProviders");
//    }
    CParcelableObjectContainer::New(allProviders);
    List< AutoPtr<ILocationProviderInterface> >::ReverseIterator rit;
    for (rit = mProviders.RBegin(); rit != mProviders.REnd(); ++rit) {
        String providers;
        (*rit)->GetName(&providers);
        AutoPtr<ICharSequence> wrapper;
        CParcelableStringWrapper::New(providers, (ICharSequence**)&wrapper);
        (*allProviders)->Add(wrapper.Get());
    }
    return NOERROR;
}

ECode CLocationManagerService::GetProviders(
    /* [in] */ ICriteria* criteria,
    /* [in] */ Boolean enabledOnly,
    /* [out] */ IObjectContainer** providers)
{
    VALIDATE_NOT_NULL(providers);
//    try {
    Mutex::Autolock lock(mLock);

    return _GetProvidersLocked(criteria, enabledOnly, providers);
//    } catch (SecurityException se) {
//        throw se;
//    } catch (Exception e) {
//        Slog.e(TAG, "getProviders got exception:", e);
//        return null;
//    }
}

ECode CLocationManagerService::_GetProvidersLocked(
    /* [in] */ ICriteria* criteria,
    /* [in] */ Boolean enabledOnly,
    /* [out] */ IObjectContainer** providers)
{
//    if (LOCAL_LOGV) {
//        Slog.v(TAG, "getProviders");
//    }
    CParcelableObjectContainer::New(providers);
    List< AutoPtr<ILocationProviderInterface> >::ReverseIterator rit;
    for (rit = mProviders.RBegin(); rit != mProviders.REnd(); ++rit) {
        ILocationProviderInterface* p = *rit;
        String provider;
        p->GetName(&provider);
        if (IsAllowedProviderSafe(provider)) {
            if (enabledOnly && !IsAllowedBySettingsLocked(provider)) {
                continue;
            }
            Boolean result;
            if (criteria != NULL && (p->MeetsCriteria(criteria, &result), !result)) {
                continue;
            }
            AutoPtr<ICharSequence> wrapper;
            CParcelableStringWrapper::New(provider, (ICharSequence**)&wrapper);
            (*providers)->Add(wrapper.Get());
        }
    }
    return NOERROR;
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

AutoPtr<ILocationProviderInterface> CLocationManagerService::Best(
    /* [in] */ IObjectContainer* providerNames)
{
    List< AutoPtr<ILocationProviderInterface> > providers;
    {
        Mutex::Autolock lock(mLock);

        AutoPtr<IObjectEnumerator> eit;
        providerNames->GetObjectEnumerator((IObjectEnumerator**)&eit);
        Boolean succeed;
        while (eit->MoveNext(&succeed), succeed) {
            AutoPtr<ICharSequence> obj;
            eit->Current((IInterface**)&obj);
            String provider;
            obj->ToString(&provider);
            HashMap<String, AutoPtr<ILocationProviderInterface> >::Iterator it =
                    mProvidersByName.Find(provider);
            if (it != mProvidersByName.End()) {
                providers.PushBack(it->mSecond);
            }
        }
    }

    if (providers.GetSize() < 2) {
        return providers[0];
    }

    // First, sort by power requirement
//    Collections.sort(providers, new LpPowerComparator());
    Int32 power0, power1;
    providers[0]->GetPowerRequirement(&power0);
    providers[1]->GetPowerRequirement(&power1);
    if (power0 < power1) {
        return providers[0];
    }

    List< AutoPtr<ILocationProviderInterface> > tmp;
    List< AutoPtr<ILocationProviderInterface> >::Iterator pit = providers.Begin();
    Int32 power;
    while ((pit != providers.End()) && ((*pit)->GetPowerRequirement(&power), power == power0)) {
        tmp.PushBack(*pit);
        ++pit;
    }

    // Next, sort by accuracy
//    Collections.sort(tmp, new LpAccuracyComparator());
    Int32 acc0, acc1;
    tmp[0]->GetAccuracy(&acc0);
    tmp[1]->GetAccuracy(&acc1);
    if (acc0 < acc1) {
        return tmp[0];
    }

    List< AutoPtr<ILocationProviderInterface> > tmp2;
    List< AutoPtr<ILocationProviderInterface> >::Iterator tit = tmp.Begin();
    Int32 acc;
    while ((tit != tmp.End()) && ((*tit)->GetAccuracy(&acc), acc == acc0)) {
        tmp2.PushBack(*tit);
        ++tit;
    }

    // Finally, sort by capability "score"
//    Collections.sort(tmp2, new LpCapabilityComparator());
    return tmp2[0];
}

ECode CLocationManagerService::GetBestProvider(
    /* [in] */ ICriteria* criteria,
    /* [in] */ Boolean enabledOnly,
    /* [out] */ String* provider)
{
    VALIDATE_NOT_NULL(provider);

    AutoPtr<IObjectContainer> goodProviders;
    GetProviders(criteria, enabledOnly, (IObjectContainer**)&goodProviders);
    Int32 count;
    if (goodProviders->GetObjectCount(&count), count > 0) {
        return Best(goodProviders)->GetName(provider);
    }

    // Make a copy of the criteria that we can modify
    AutoPtr<ICriteria> criteriaCopy;
    CCriteria::New(criteria, (ICriteria**)&criteriaCopy);

    // Loosen power requirement
    Int32 power;
    criteriaCopy->GetPowerRequirement(&power);
    while ((goodProviders->GetObjectCount(&count), count == 0) && (power != Criteria_NO_REQUIREMENT)) {
        power = NextPower(power);
        criteriaCopy->SetPowerRequirement(power);
        goodProviders = NULL;
        GetProviders(criteriaCopy, enabledOnly, (IObjectContainer**)&goodProviders);
    }
    if (goodProviders->GetObjectCount(&count), count > 0) {
        return Best(goodProviders)->GetName(provider);
    }

    // Loosen accuracy requirement
    Int32 accuracy;
    criteriaCopy->GetAccuracy(&accuracy);
    while ((goodProviders->GetObjectCount(&count), count == 0) && (accuracy != Criteria_NO_REQUIREMENT)) {
        accuracy = NextAccuracy(accuracy);
        criteriaCopy->SetAccuracy(accuracy);
        goodProviders = NULL;
        GetProviders(criteriaCopy, enabledOnly, (IObjectContainer**)&goodProviders);
    }
    if (goodProviders->GetObjectCount(&count), count > 0) {
        return Best(goodProviders)->GetName(provider);
    }

    // Remove bearing requirement
    criteriaCopy->SetBearingRequired(FALSE);
    goodProviders = NULL;
    GetProviders(criteriaCopy, enabledOnly, (IObjectContainer**)&goodProviders);
    if (goodProviders->GetObjectCount(&count), count > 0) {
        return Best(goodProviders)->GetName(provider);
    }

    // Remove speed requirement
    criteriaCopy->SetSpeedRequired(FALSE);
    goodProviders = NULL;
    GetProviders(criteriaCopy, enabledOnly, (IObjectContainer**)&goodProviders);
    if (goodProviders->GetObjectCount(&count), count > 0) {
        return Best(goodProviders)->GetName(provider);
    }

    // Remove altitude requirement
    criteriaCopy->SetAltitudeRequired(FALSE);
    goodProviders = NULL;
    GetProviders(criteriaCopy, enabledOnly, (IObjectContainer**)&goodProviders);
    if (goodProviders->GetObjectCount(&count), count > 0) {
        return Best(goodProviders)->GetName(provider);
    }

    *provider = NULL;
    return NOERROR;
}

ECode CLocationManagerService::ProviderMeetsCriteria(
    /* [in] */ const String& provider,
    /* [in] */ ICriteria* criteria,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    ILocationProviderInterface* p = NULL;
    HashMap<String, AutoPtr<ILocationProviderInterface> >::Iterator it = mProvidersByName.Find(provider);
    if (it != mProvidersByName.End()) {
        p = it->mSecond;
    }
    if (p == NULL) {
//        throw new IllegalArgumentException("provider=" + provider);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return p->MeetsCriteria(criteria, result);
}

void CLocationManagerService::UpdateProvidersLocked()
{
    Boolean changesMade = FALSE;
    List< AutoPtr<ILocationProviderInterface> >::ReverseIterator rit;
    for (rit = mProviders.RBegin(); rit != mProviders.REnd(); ++rit) {
        ILocationProviderInterface* p = *rit;
        Boolean isEnabled;
        p->IsEnabled(&isEnabled);
        String name;
        p->GetName(&name);
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
    /* [in] */ const String& provider,
    /* [in] */ Boolean enabled)
{
    Int32 listeners = 0;

    ILocationProviderInterface* p = NULL;
    HashMap<String, AutoPtr<ILocationProviderInterface> >::Iterator it =
            mProvidersByName.Find(provider);
    if (it != mProvidersByName.End()) {
        p = it->mSecond;
    }
    if (p == NULL) {
        return;
    }

    List< AutoPtr<Receiver> >* deadReceivers = NULL;

    List< AutoPtr<UpdateRecord> >* records = NULL;
    HashMap<String, List< AutoPtr<UpdateRecord> >* >::Iterator hmIt =
            mRecordsByProvider.Find(provider);
    if (hmIt != mRecordsByProvider.End()) {
        records = hmIt->mSecond;
    }
    if (records != NULL) {
        List< AutoPtr<UpdateRecord> >::Iterator lit;
        for (lit = records->Begin(); lit != records->End(); ++lit) {
            UpdateRecord* record = *lit;
            // Sends a notification message to the receiver
            if (!record->mReceiver->CallProviderEnabledLocked(provider, enabled)) {
                if (deadReceivers == NULL) {
                    deadReceivers = new List< AutoPtr<Receiver> >();
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
            p->SetMinTime(GetMinTimeLocked(provider), mTmpWorkSource);
            p->EnableLocationTracking(TRUE);
        }
    }
    else {
        p->EnableLocationTracking(FALSE);
        p->Disable();
    }

    if (deadReceivers != NULL) {
        deadReceivers->Clear();
        delete deadReceivers;
    }
}

Int64 CLocationManagerService::GetMinTimeLocked(
    /* [in] */ const String& provider)
{
    Int64 minTime = Math::INT64_MAX_VALUE;
    List< AutoPtr<UpdateRecord> >* records = NULL;
    HashMap<String, List< AutoPtr<UpdateRecord> >* >::Iterator hmit =
            mRecordsByProvider.Find(provider);
    if (hmit != mRecordsByProvider.End()) {
        records = hmit->mSecond;
    }
    mTmpWorkSource->Clear();
    if (records != NULL) {
        List< AutoPtr<UpdateRecord> >::ReverseIterator rit;
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
                Boolean result;
                mTmpWorkSource->AddEx(ur->mUid, &result);
            }
        }
    }
    return minTime;
}

AutoPtr<CLocationManagerService::Receiver> CLocationManagerService::GetReceiver(
    /* [in] */ ILocationListener* listener)
{
//    IBinder binder = listener.asBinder();
    AutoPtr<Receiver> receiver;
    HashMap<AutoPtr<IInterface>, AutoPtr<Receiver> >::Iterator it =
            mReceivers.Find((IInterface*)listener);
    if (it != mReceivers.End()) {
        receiver = it->mSecond;
    }
    if (receiver == NULL) {
        receiver = new Receiver(this, listener);
        mReceivers[(IInterface*)listener] = receiver;

//        try {
        if (receiver->IsListener()) {
//            receiver.getListener().asBinder().linkToDeath(receiver, 0);
        }
//        } catch (RemoteException e) {
//            Slog.e(TAG, "linkToDeath failed:", e);
//            return null;
//        }
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
        mReceivers[(IInterface*)intent]= receiver;
    }
    return receiver;
}

Boolean CLocationManagerService::ProviderHasListener(
    /* [in] */ const String& provider,
    /* [in] */ Int32 uid,
    /* [in] */ Receiver* excludedReceiver)
{
    List< AutoPtr<UpdateRecord> >* records = NULL;
    HashMap<String, List< AutoPtr<UpdateRecord> >* >::Iterator it =
            mRecordsByProvider.Find(provider);
    if (it != mRecordsByProvider.End()) {
        records = it->mSecond;
    }
    if (records != NULL) {
        List< AutoPtr<UpdateRecord> >::ReverseIterator rit;
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
    /* [in] */ const String& _provider,
    /* [in] */ ICriteria* criteria,
    /* [in] */ Int64 minTime,
    /* [in] */ Float minDistance,
    /* [in] */ Boolean singleShot,
    /* [in] */ ILocationListener* listener)
{
    String provider = _provider;
    if (criteria != NULL) {
        // FIXME - should we consider using multiple providers simultaneously
        // rather than only the best one?
        // Should we do anything different for single shot fixes?
        GetBestProvider(criteria, TRUE, &provider);
        if (provider.IsNull()) {
//            throw new IllegalArgumentException("no providers found for criteria");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
    }
//    try {
    Mutex::Autolock lock(&mLock);

    return RequestLocationUpdatesLocked(provider, minTime, minDistance, singleShot,
            GetReceiver(listener));
//    } catch (SecurityException se) {
//        throw se;
//    } catch (IllegalArgumentException iae) {
//        throw iae;
//    } catch (Exception e) {
//        Slog.e(TAG, "requestUpdates got exception:", e);
//    }
}

ECode CLocationManagerService::RequestLocationUpdatesPI(
    /* [in] */ const String& _provider,
    /* [in] */ ICriteria* criteria,
    /* [in] */ Int64 minTime,
    /* [in] */ Float minDistance,
    /* [in] */ Boolean singleShot,
    /* [in] */ IPendingIntent* intent)
{
    String provider = _provider;
    if (criteria != NULL) {
        // FIXME - should we consider using multiple providers simultaneously
        // rather than only the best one?
        // Should we do anything different for single shot fixes?
        GetBestProvider(criteria, TRUE, &provider);
        if (provider.IsNull()) {
//            throw new IllegalArgumentException("no providers found for criteria");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
    }
//    try {
    Mutex::Autolock lock(&mLock);

    return RequestLocationUpdatesLocked(provider, minTime, minDistance, singleShot,
                    GetReceiver(intent));
//    } catch (SecurityException se) {
//        throw se;
//    } catch (IllegalArgumentException iae) {
//        throw iae;
//    } catch (Exception e) {
//        Slog.e(TAG, "requestUpdates got exception:", e);
//    }
}

ECode CLocationManagerService::RequestLocationUpdatesLocked(
    /* [in] */ const String& provider,
    /* [in] */ Int64 minTime,
    /* [in] */ Float minDistance,
    /* [in] */ Boolean singleShot,
    /* [in] */ Receiver* receiver)
{
    AutoPtr<ILocationProviderInterface> p;
    HashMap<String, AutoPtr<ILocationProviderInterface> >::Iterator it =
            mProvidersByName.Find(provider);
    if (it != mProvidersByName.End()) {
        p = it->mSecond;
    }
    if (p == NULL) {
//        throw new IllegalArgumentException("provider=" + provider);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    FAIL_RETURN(CheckPermissionsSafe(provider));

    // so wakelock calls will succeed32
    Int32 callingUid = Binder::GetCallingUid();
    Boolean newUid = !ProviderHasListener(provider, callingUid, NULL);
    Int64 identity = Binder::ClearCallingIdentity();
//    try {
    AutoPtr<UpdateRecord> r = new UpdateRecord(this, provider, minTime, minDistance, singleShot,
            receiver, callingUid);
    UpdateRecord* oldRecord = NULL;
    HashMap<String, AutoPtr<UpdateRecord> >::Iterator oldit =
            receiver->mUpdateRecords.Find(provider);
    if (oldit != receiver->mUpdateRecords.End()) {
        oldRecord = oldit->mSecond;
    }
    if (oldRecord != NULL) {
        oldRecord->DisposeLocked();
    }
    receiver->mUpdateRecords[provider] = r;

    if (newUid) {
        p->AddListener(callingUid);
    }

    Boolean isProviderEnabled = IsAllowedBySettingsLocked(provider);
    if (isProviderEnabled) {
        Int64 minTimeForProvider = GetMinTimeLocked(provider);
        p->SetMinTime(minTimeForProvider, mTmpWorkSource);
        // try requesting single shot if singleShot is true, and fall back to
        // regular location tracking if requestSingleShotFix() is not supported
        Boolean result;
        if (!singleShot || (p->RequestSingleShotFix(&result), !result)) {
            p->EnableLocationTracking(TRUE);
        }
    }
    else {
        // Notify the listener that updates are currently disabled
        receiver->CallProviderEnabledLocked(provider, FALSE);
    }
//        if (LOCAL_LOGV) {
//            Slog.v(TAG, "_requestLocationUpdates: provider = " + provider + " listener = " + receiver);
//        }
//    } finally {
    Binder::RestoreCallingIdentity(identity);
//    }
    return NOERROR;
}

ECode CLocationManagerService::RemoveUpdates(
    /* [in] */ ILocationListener* listener)
{
//    try {
    Mutex::Autolock lock(&mLock);

    RemoveUpdatesLocked(GetReceiver(listener));
//    } catch (SecurityException se) {
//        throw se;
//    } catch (IllegalArgumentException iae) {
//        throw iae;
//    } catch (Exception e) {
//        Slog.e(TAG, "removeUpdates got exception:", e);
//    }
    return NOERROR;
}

ECode CLocationManagerService::RemoveUpdatesPI(
    /* [in] */ IPendingIntent* intent)
{
//    try {
    Mutex::Autolock lock(&mLock);

    RemoveUpdatesLocked(GetReceiver(intent));
//    } catch (SecurityException se) {
//        throw se;
//    } catch (IllegalArgumentException iae) {
//        throw iae;
//    } catch (Exception e) {
//        Slog.e(TAG, "removeUpdates got exception:", e);
//    }
    return NOERROR;
}

void CLocationManagerService::RemoveUpdatesLocked(
    /* [in] */ Receiver* receiver)
{
//    if (LOCAL_LOGV) {
//        Slog.v(TAG, "_removeUpdates: listener = " + receiver);
//    }

    // so wakelock calls will succeed
    Int32 callingUid = Binder::GetCallingUid();
    Int64 identity = Binder::ClearCallingIdentity();
//    try {
    HashMap<AutoPtr<IInterface>, AutoPtr<Receiver> >::Iterator it = mReceivers.Find(receiver->mKey);
    if (it != mReceivers.End() && receiver->IsListener()) {
        mReceivers.Erase(it);
//        receiver->GetListener().asBinder().unlinkToDeath(receiver, 0);
        Mutex::Autolock lock(receiver->mLock);

        if(receiver->mPendingBroadcasts > 0) {
            DecrementPendingBroadcasts();
            receiver->mPendingBroadcasts = 0;
        }
    }

    // Record which providers were associated with this listener
    HashSet<String> providers;
    HashMap<String, AutoPtr<UpdateRecord> >& oldRecords = receiver->mUpdateRecords;
    // Call dispose() on the obsolete update records.
    HashMap<String, AutoPtr<UpdateRecord> >::Iterator oldit;
    for (oldit = oldRecords.Begin(); oldit != oldRecords.End(); ++oldit) {
        UpdateRecord* record = oldit->mSecond;
        if (!ProviderHasListener(record->mProvider, callingUid, receiver)) {
            AutoPtr<ILocationProviderInterface> p;
            HashMap<String, AutoPtr<ILocationProviderInterface> >::Iterator lpit =
                    mProvidersByName.Find(record->mProvider);
            if (lpit != mProvidersByName.End()) {
                p = lpit->mSecond;
            }
            if (p != NULL) {
                p->RemoveListener(callingUid);
            }
        }
        record->DisposeLocked();

        // Accumulate providers
        providers.Insert(oldit->mFirst);
    }

    // See if the providers associated with this listener have any
    // other listeners; if one does, inform it of the new smallest minTime
    // value; if one does not, disable location tracking for it
    HashSet<String>::Iterator providersIt;
    for (providersIt = providers.Begin(); providersIt != providers.End(); ++providersIt) {
        String provider = *providersIt;
        // If provider is already disabled, don't need to do anything
        if (!IsAllowedBySettingsLocked(provider)) {
            continue;
        }

        Boolean hasOtherListener = FALSE;
        List< AutoPtr<UpdateRecord> >* recordsForProvider = NULL;
        HashMap<String, List< AutoPtr<UpdateRecord> >* >::Iterator recordsForProviderIt =
                mRecordsByProvider.Find(provider);
        if (recordsForProviderIt != mRecordsByProvider.End()) {
            recordsForProvider = recordsForProviderIt->mSecond;
        }
        if (recordsForProvider != NULL && recordsForProvider->GetSize() > 0) {
            hasOtherListener = TRUE;
        }

        ILocationProviderInterface* p = NULL;
        HashMap<String, AutoPtr<ILocationProviderInterface> >::Iterator lpit =
                mProvidersByName.Find(provider);
        if (lpit != mProvidersByName.End()) {
            p = lpit->mSecond;
        }
        if (p != NULL) {
            if (hasOtherListener) {
                p->SetMinTime(GetMinTimeLocked(provider), mTmpWorkSource);
            }
            else {
                p->EnableLocationTracking(FALSE);
            }
        }
    }
//    } finally {
    Binder::RestoreCallingIdentity(identity);
//    }
}

ECode CLocationManagerService::AddGpsStatusListener(
    /* [in] */ IGpsStatusListener* listener,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (mGpsStatusProvider == NULL) {
        *result = FALSE;
        return NOERROR;
    }
    Int32 perm;
    mContext->CheckCallingOrSelfPermission(String(ACCESS_FINE_LOCATION), &perm);
    if (perm != CapsuleManager_PERMISSION_GRANTED) {
//        throw new SecurityException("Requires ACCESS_FINE_LOCATION permission");
        return E_SECURITY_EXCEPTION;
    }

//    try {
    ECode ec = mGpsStatusProvider->AddGpsStatusListener(listener);
    if (ec == E_REMOTE_EXCEPTION) {
        *result = FALSE;
        return NOERROR;
    }
//    } catch (RemoteException e) {
//        Slog.e(TAG, "mGpsStatusProvider.addGpsStatusListener failed", e);
//        return false;
//    }
    *result = TRUE;
    return NOERROR;
}

ECode CLocationManagerService::RemoveGpsStatusListener(
    /* [in] */ IGpsStatusListener* listener)
{
    Mutex::Autolock lock(&mLock);
//        try {
    return mGpsStatusProvider->RemoveGpsStatusListener(listener);
//        } catch (Exception e) {
//            Slog.e(TAG, "mGpsStatusProvider.removeGpsStatusListener failed", e);
//        }
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
//        throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }

    // first check for permission to the provider
    FAIL_RETURN(CheckPermissionsSafe(provider));
    // and check for ACCESS_LOCATION_EXTRA_COMMANDS
    Int32 perm;
    mContext->CheckCallingOrSelfPermission(String(ACCESS_LOCATION_EXTRA_COMMANDS), &perm);
    if (perm != CapsuleManager_PERMISSION_GRANTED) {
//        throw new SecurityException("Requires ACCESS_LOCATION_EXTRA_COMMANDS permission");
        return E_SECURITY_EXCEPTION;
    }

    Mutex::Autolock lock(mLock);

    ILocationProviderInterface* p = NULL;
    HashMap<String, AutoPtr<ILocationProviderInterface> >::Iterator it =
            mProvidersByName.Find(provider);
    if (it != mProvidersByName.End()) {
        p = it->mSecond;
    }
    if (p == NULL) {
        *outExtras = inExtras;
        if (*outExtras != NULL) (*outExtras)->AddRef();
        *result = FALSE;
        return NOERROR;
    }

    return p->SendExtraCommand(command, inExtras, outExtras, result);
}

// for NI support
ECode CLocationManagerService::SendNiResponse(
    /* [in] */ Int32 notifId,
    /* [in] */ Int32 userResponse,
    /* [out] */ Boolean* result)
{
    if (Binder::GetCallingUid() != Process::MyUid()) {
        // throw new SecurityException(
        //        "calling sendNiResponse from outside of the system is not allowed");
        return E_SECURITY_EXCEPTION;
    }
//    try {
    ECode ec = mNetInitiatedListener->SendNiResponse(notifId, userResponse, result);
    if (FAILED(ec)) {
        *result = FALSE;
    }
    return ec;
//    }
//    catch (RemoteException e)
//    {
//        Slog.e(TAG, "RemoteException in LocationManagerService.sendNiResponse");
//        return false;
//    }
}

ECode CLocationManagerService::AddProximityAlert(
    /* [in] */ Double latitude,
    /* [in] */ Double longitude,
    /* [in] */ Float radius,
    /* [in] */ Int64 expiration,
    /* [in] */ IPendingIntent* intent)
{
//    try {
    Mutex::Autolock lock(&mLock);

    return AddProximityAlertLocked(latitude, longitude, radius, expiration, intent);
//    } catch (SecurityException se) {
//        throw se;
//    } catch (IllegalArgumentException iae) {
//        throw iae;
//    } catch (Exception e) {
//        Slog.e(TAG, "addProximityAlert got exception:", e);
//    }
}

ECode CLocationManagerService::AddProximityAlertLocked(
    /* [in] */ Double latitude,
    /* [in] */ Double longitude,
    /* [in] */ Float radius,
    /* [in] */ Int64 expiration,
    /* [in] */ IPendingIntent* intent)
{
//    if (LOCAL_LOGV) {
//        Slog.v(TAG, "addProximityAlert: latitude = " + latitude +
//                ", longitude = " + longitude +
//                ", expiration = " + expiration +
//                ", intent = " + intent);
//    }

    // Require ability to access all providers for now
    if (!IsAllowedProviderSafe(String(LocationManager_GPS_PROVIDER)) ||
        !IsAllowedProviderSafe(String(LocationManager_NETWORK_PROVIDER))) {
//        throw new SecurityException("Requires ACCESS_FINE_LOCATION permission");
        return E_SECURITY_EXCEPTION;
    }

    if (expiration != -1) {
        expiration += System::GetCurrentTimeMillis();
    }
    AutoPtr<ProximityAlert> alert = new ProximityAlert(Binder::GetCallingUid(),
            latitude, longitude, radius, expiration, intent);
    mProximityAlerts[intent] = alert;

    if (mProximityReceiver == NULL) {
        CProximityListener::New((Handle32)this, (ILocationListener**)&mProximityListener);
        mProximityReceiver = new Receiver(this, mProximityListener);

        List<AutoPtr<ILocationProviderInterface> >::ReverseIterator rit;
        for (rit = mProviders.RBegin(); rit != mProviders.REnd(); ++rit) {
            ILocationProviderInterface* provider = *rit;
            String name;
            provider->GetName(&name);
            RequestLocationUpdatesLocked(name, 1000, 1.0f,
                    FALSE, mProximityReceiver);
        }
    }
    return NOERROR;
}

ECode CLocationManagerService::RemoveProximityAlert(
    /* [in] */ IPendingIntent* intent)
{
//    try {
    Mutex::Autolock lock(&mLock);

    return RemoveProximityAlertLocked(intent);
//    } catch (SecurityException se) {
//        throw se;
//    } catch (IllegalArgumentException iae) {
//        throw iae;
//    } catch (Exception e) {
//        Slog.e(TAG, "removeProximityAlert got exception:", e);
//    }
}

ECode CLocationManagerService::RemoveProximityAlertLocked(
    /* [in] */ IPendingIntent* intent)
{
//    if (LOCAL_LOGV) {
//        Slog.v(TAG, "removeProximityAlert: intent = " + intent);
//    }

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
    VALIDATE_NOT_NULL(info);
//    try {
    Mutex::Autolock lock(&mLock);

    return _GetProviderInfoLocked(provider, info);
//        }
//    } catch (SecurityException se) {
//        throw se;
//    } catch (IllegalArgumentException iae) {
//        throw iae;
//    } catch (Exception e) {
//        Slog.e(TAG, "_getProviderInfo got exception:", e);
//        return null;
//    }
}

ECode CLocationManagerService::_GetProviderInfoLocked(
    /* [in] */ const String& provider,
    /* [out] */ IBundle** info)
{
    ILocationProviderInterface* p = NULL;
    HashMap<String, AutoPtr<ILocationProviderInterface> >::Iterator it =
            mProvidersByName.Find(provider);
    if (it != mProvidersByName.End()) {
        p = it->mSecond;
    }
    if (p == NULL) {
        *info = NULL;
        return NOERROR;
    }

    FAIL_RETURN(CheckPermissionsSafe(provider));

    AutoPtr<IBundle> b;
    CBundle::New((IBundle**)&b);
    Boolean bv;
    p->RequiresNetwork(&bv);
    b->PutBoolean(String("network"), bv);
    p->RequiresSatellite(&bv);
    b->PutBoolean(String("satellite"), bv);
    p->RequiresCell(&bv);
    b->PutBoolean(String("cell"), bv);
    p->HasMonetaryCost(&bv);
    b->PutBoolean(String("cost"), bv);
    p->SupportsAltitude(&bv);
    b->PutBoolean(String("altitude"), bv);
    p->SupportsSpeed(&bv);
    b->PutBoolean(String("speed"), bv);
    p->SupportsBearing(&bv);
    b->PutBoolean(String("bearing"), bv);
    Int32 iv;
    p->GetPowerRequirement(&iv);
    b->PutInt32(String("power"), iv);
    p->GetAccuracy(&iv);
    b->PutInt32(String("accuracy"), iv);

    *info = b;
    (*info)->AddRef();
    return NOERROR;
}

ECode CLocationManagerService::IsProviderEnabled(
    /* [in] */ const String& provider,
    /* [out] */ Boolean* isEnabled)
{
    VALIDATE_NOT_NULL(isEnabled);
//    try {
    Mutex::Autolock lock(&mLock);

    return _IsProviderEnabledLocked(provider, isEnabled);
//    } catch (SecurityException se) {
//        throw se;
//    } catch (Exception e) {
//        Slog.e(TAG, "isProviderEnabled got exception:", e);
//        return false;
//    }
}

// Used by location providers to tell the location manager when it has a new location.
// Passive is true if the location is coming from the passive provider, in which case
// it need not be shared with other providers.
ECode CLocationManagerService::ReportLocation(
    /* [in] */ ILocation* location,
    /* [in] */ Boolean passive)
{
    Int32 perm;
    mContext->CheckCallingOrSelfPermission(String(INSTALL_LOCATION_PROVIDER), &perm);
    if (perm != CapsuleManager_PERMISSION_GRANTED) {
//        throw new SecurityException("Requires INSTALL_LOCATION_PROVIDER permission");
        return E_SECURITY_EXCEPTION;
    }

    void (STDCALL CLocationManagerService::*pHandlerFunc)(ILocation*, Boolean);
    pHandlerFunc = &CLocationManagerService::HandleLocationChanged;
    mLocationHandler->RemoveCppCallbacks((Handle32)this, *(Handle32*)&pHandlerFunc);
    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr((IInterface*)location);
    params->WriteBoolean(passive);
    return mLocationHandler->PostCppCallbackAtFrontOfQueue((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

ECode CLocationManagerService::_IsProviderEnabledLocked(
    /* [in] */ const String& provider,
    /* [out] */ Boolean* isEnabled)
{
    FAIL_RETURN(CheckPermissionsSafe(provider));

    ILocationProviderInterface* p = NULL;
    HashMap<String, AutoPtr<ILocationProviderInterface> >::Iterator it =
            mProvidersByName.Find(provider);
    if (it != mProvidersByName.End()) {
        p = it->mSecond;
    }
    if (p == NULL) {
        *isEnabled = FALSE;
        return NOERROR;
    }

    *isEnabled = IsAllowedBySettingsLocked(provider);
    return NOERROR;
}

ECode CLocationManagerService::GetLastKnownLocation(
    /* [in] */ const String& provider,
    /* [out] */ ILocation** location)
{
    VALIDATE_NOT_NULL(location);
//    if (LOCAL_LOGV) {
//        Slog.v(TAG, "getLastKnownLocation: " + provider);
//    }
//    try {
    Mutex::Autolock lock(mLock);

    return _GetLastKnownLocationLocked(provider, location);
//    } catch (SecurityException se) {
//        throw se;
//    } catch (Exception e) {
//        Slog.e(TAG, "getLastKnownLocation got exception:", e);
//        return null;
//    }
}

ECode CLocationManagerService::_GetLastKnownLocationLocked(
    /* [in] */ const String& provider,
    /* [out] */ ILocation** location)
{
    FAIL_RETURN(CheckPermissionsSafe(provider));

    ILocationProviderInterface* p = NULL;
    HashMap<String, AutoPtr<ILocationProviderInterface> >::Iterator it =
            mProvidersByName.Find(provider);
    if (it != mProvidersByName.End()) {
        p = it->mSecond;
    }
    if (p == NULL) {
        *location = NULL;
        return NOERROR;
    }

    if (!IsAllowedBySettingsLocked(provider)) {
        *location = NULL;
        return NOERROR;
    }

    HashMap<String, AutoPtr<ILocation> >::Iterator lastIt = mLastKnownLocation.Find(provider);
    *location = lastIt != mLastKnownLocation.End() ? lastIt->mSecond : NULL;
    if (*location != NULL) (*location)->AddRef();
    return NOERROR;
}

Boolean CLocationManagerService::ShouldBroadcastSafe(
    /* [in] */ ILocation* loc,
    /* [in] */ ILocation* lastLoc,
    /* [in] */ UpdateRecord* record)
{
    // Always broadcast the first update
    if (lastLoc == NULL) {
        return TRUE;
    }

    // Don't broadcast same location again regardless of condition
    // TODO - we should probably still rebroadcast if user explicitly sets a minTime > 0
    Int64 time, lastTime;
    loc->GetTime(&time);
    lastLoc->GetTime(&lastTime);
    if (time == lastTime) {
        return FALSE;
    }

    // Check whether sufficient distance has been traveled
    Double minDistance = record->mMinDistance;
    if (minDistance > 0.0) {
        Int32 dis;
        loc->DistanceTo(lastLoc, (Float*)&dis);
        if (dis <= minDistance) {
            return FALSE;
        }
    }

    return TRUE;
}

void CLocationManagerService::HandleLocationChangedLocked(
    /* [in] */ ILocation* location,
    /* [in] */ Boolean passive)
{
    String provider;
    if(passive) {
        provider = LocationManager_PASSIVE_PROVIDER;
    }
    else {
        location->GetProvider(&provider);
    }
    List< AutoPtr<UpdateRecord> >* records = NULL;
    HashMap<String, List< AutoPtr<UpdateRecord> >* >::Iterator it =
            mRecordsByProvider.Find(provider);
    if (it != mRecordsByProvider.End()) {
        records = it->mSecond;
    }
    if (records == NULL || records->Begin() == records->End()) {
        return;
    }

    ILocationProviderInterface* p;
    HashMap<String, AutoPtr<ILocationProviderInterface> >::Iterator providersIt =
            mProvidersByName.Find(provider);
    if (providersIt != mProvidersByName.End()) {
        p = providersIt->mSecond;
    }
    if (p == NULL) {
        return;
    }

    // Update last known location for provider
    AutoPtr<ILocation> lastLocation;
    HashMap<String, AutoPtr<ILocation> >::Iterator locIt = mLastKnownLocation.Find(provider);
    if (locIt != mLastKnownLocation.End()) {
        lastLocation = locIt->mSecond;
    }
    if (lastLocation == NULL) {
        AutoPtr<ILocation> loc;
        CLocation::New(location, (ILocation**)&loc);
        mLastKnownLocation[provider] = loc;
    }
    else {
        lastLocation->Set(location);
    }

    // Fetch latest status update time
    Int64 newStatusUpdateTime;
    p->GetStatusUpdateTime(&newStatusUpdateTime);

    // Get latest status
    AutoPtr<IBundle> extras;
    CBundle::New((IBundle**)&extras);
    Int32 status;
    p->GetStatus(extras, &status);

    List< AutoPtr<Receiver> >* deadReceivers = NULL;

    // Broadcast location or status to all listeners
    List< AutoPtr<UpdateRecord> >::Iterator recordIt;
    for (recordIt = records->Begin(); recordIt != records->End(); ++recordIt) {
        UpdateRecord* r = *recordIt;
        AutoPtr<Receiver> receiver = r->mReceiver;
        Boolean receiverDead = FALSE;

        AutoPtr<ILocation> lastLoc = r->mLastFixBroadcast;
        if ((lastLoc == NULL) || ShouldBroadcastSafe(location, lastLoc, r)) {
            if (lastLoc == NULL) {
                CLocation::New(location, (ILocation**)&lastLoc);
                r->mLastFixBroadcast = lastLoc;
            }
            else {
                lastLoc->Set(location);
            }
            if (!receiver->CallLocationChangedLocked(location)) {
//                Slog.w(TAG, "RemoteException calling onLocationChanged on " + receiver);
                receiverDead = TRUE;
            }
        }

        Int64 prevStatusUpdateTime = r->mLastStatusBroadcast;
        if ((newStatusUpdateTime > prevStatusUpdateTime) &&
            (prevStatusUpdateTime != 0 || status != LocationProvider_AVAILABLE)) {

            r->mLastStatusBroadcast = newStatusUpdateTime;
            if (!receiver->CallStatusChangedLocked(provider, status, extras)) {
                receiverDead = TRUE;
//                Slog.w(TAG, "RemoteException calling onStatusChanged on " + receiver);
            }
        }

        // remove receiver if it is dead or we just processed a single shot request
        if (receiverDead || r->mSingleShot) {
            if (deadReceivers == NULL) {
                deadReceivers = new List<AutoPtr<Receiver> >();
            }
            if (Find(deadReceivers->Begin(), deadReceivers->End(), receiver) ==
                    deadReceivers->End()) {
                deadReceivers->PushBack(receiver);
            }
        }
    }

    if (deadReceivers != NULL) {
        List<AutoPtr<Receiver> >::ReverseIterator rit;
        for (rit = deadReceivers->RBegin(); rit != deadReceivers->REnd(); ++rit) {
            RemoveUpdatesLocked(*rit);
        }
        deadReceivers->Clear();
        delete deadReceivers;
    }
}

void CLocationManagerService::IncrementPendingBroadcasts()
{
//    synchronized (mWakeLock) {
//    if (mPendingBroadcasts++ == 0) {
//        try {
//        mWakeLock->Acquire();
//        log("Acquired wakelock");
//        } catch (Exception e) {
//            // This is to catch a runtime exception thrown when we try to release an
//            // already released lock.
//            Slog.e(TAG, "exception in acquireWakeLock()", e);
//        }
//    }
//}
}

void CLocationManagerService::DecrementPendingBroadcasts()
{
//    synchronized (mWakeLock) {
//    if (--mPendingBroadcasts == 0) {
//        try {
        // Release wake lock
//        if (mWakeLock->IsHeld()) {
//            mWakeLock->Release();
//            log("Released wakelock");
//        } else {
//            log("Can't release wakelock again!");
//        }
//        } catch (Exception e) {
//            // This is to catch a runtime exception thrown when we try to release an
//            // already released lock.
//            Slog.e(TAG, "exception in releaseWakeLock()", e);
//        }
//    }
//    }
}

ECode CLocationManagerService::GeocoderIsPresent(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mGeocodeProvider != NULL;
    return NOERROR;
}

ECode CLocationManagerService::GetFromLocation(
    /* [in] */ Double latitude,
    /* [in] */ Double longitude,
    /* [in] */ Int32 maxResults,
    /* [in] */ IGeocoderParams* params,
    /* [out] */ IObjectContainer** addrs,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(addrs);
    VALIDATE_NOT_NULL(result);

    if (mGeocodeProvider != NULL) {
        *result = mGeocodeProvider->GetFromLocation(latitude, longitude, maxResults,
                params, addrs);
       return NOERROR;
    }
    *result = NULL;
    return NOERROR;
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
    VALIDATE_NOT_NULL(addrs);
    VALIDATE_NOT_NULL(result);

    if (mGeocodeProvider != NULL) {
        *result = mGeocodeProvider->GetFromLocationName(locationName, lowerLeftLatitude,
                lowerLeftLongitude, upperRightLatitude, upperRightLongitude,
                maxResults, params, addrs);
        return NOERROR;
    }
    *result = NULL;
    return NOERROR;
}

ECode CLocationManagerService::CheckMockPermissionsSafe()
{
//    Boolean allowMocks = Settings.Secure.getInt(mContext.getContentResolver(),
//            Settings.Secure.ALLOW_MOCK_LOCATION, 0) == 1;
//    if (!allowMocks) {
//        throw new SecurityException("Requires ACCESS_MOCK_LOCATION secure setting");
//    }

    Int32 value;
    mContext->CheckCallingPermission(String(ACCESS_MOCK_LOCATION), &value);
    if (value != CapsuleManager_PERMISSION_GRANTED) {
//        throw new SecurityException("Requires ACCESS_MOCK_LOCATION permission");
        return E_SECURITY_EXCEPTION;
    }
    return NOERROR;
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
    // checkMockPermissionsSafe();

    // if (LocationManager.PASSIVE_PROVIDER.equals(name)) {
    //     throw new IllegalArgumentException("Cannot mock the passive location provider");
    // }

    // long identity = Binder.clearCallingIdentity();
    // synchronized (mLock) {
    //     MockProvider provider = new MockProvider(name, this,
    //         requiresNetwork, requiresSatellite,
    //         requiresCell, hasMonetaryCost, supportsAltitude,
    //         supportsSpeed, supportsBearing, powerRequirement, accuracy);
    //     // remove the real provider if we are replacing GPS or network provider
    //     if (LocationManager.GPS_PROVIDER.equals(name)
    //             || LocationManager.NETWORK_PROVIDER.equals(name)) {
    //         LocationProviderInterface p = mProvidersByName.get(name);
    //         if (p != null) {
    //             p.enableLocationTracking(false);
    //             removeProvider(p);
    //         }
    //     }
    //     if (mProvidersByName.get(name) != null) {
    //         throw new IllegalArgumentException("Provider \"" + name + "\" already exists");
    //     }
    //     addProvider(provider);
    //     mMockProviders.put(name, provider);
    //     mLastKnownLocation.put(name, null);
    //     updateProvidersLocked();
    // }
    // Binder.restoreCallingIdentity(identity);
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManagerService::RemoveTestProvider(
    /* [in] */ const String& provider)
{
    // checkMockPermissionsSafe();
    // synchronized (mLock) {
    //     MockProvider mockProvider = mMockProviders.get(provider);
    //     if (mockProvider == null) {
    //         throw new IllegalArgumentException("Provider \"" + provider + "\" unknown");
    //     }
    //     long identity = Binder.clearCallingIdentity();
    //     removeProvider(mProvidersByName.get(provider));
    //     mMockProviders.remove(mockProvider);
    //     // reinstall real provider if we were mocking GPS or network provider
    //     if (LocationManager.GPS_PROVIDER.equals(provider) &&
    //             mGpsLocationProvider != null) {
    //         addProvider(mGpsLocationProvider);
    //     } else if (LocationManager.NETWORK_PROVIDER.equals(provider) &&
    //             mNetworkLocationProvider != null) {
    //         addProvider(mNetworkLocationProvider);
    //     }
    //     mLastKnownLocation.put(provider, null);
    //     updateProvidersLocked();
    //     Binder.restoreCallingIdentity(identity);
    // }
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManagerService::SetTestProviderLocation(
    /* [in] */ const String& provider,
    /* [in] */ ILocation* loc)
{
    // checkMockPermissionsSafe();
    // synchronized (mLock) {
    //     MockProvider mockProvider = mMockProviders.get(provider);
    //     if (mockProvider == null) {
    //         throw new IllegalArgumentException("Provider \"" + provider + "\" unknown");
    //     }
    //     // clear calling identity so INSTALL_LOCATION_PROVIDER permission is not required
    //     long identity = Binder.clearCallingIdentity();
    //     mockProvider.setLocation(loc);
    //     Binder.restoreCallingIdentity(identity);
    // }
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManagerService::ClearTestProviderLocation(
    /* [in] */ const String& provider)
{
    // checkMockPermissionsSafe();
    // synchronized (mLock) {
    //     MockProvider mockProvider = mMockProviders.get(provider);
    //     if (mockProvider == null) {
    //         throw new IllegalArgumentException("Provider \"" + provider + "\" unknown");
    //     }
    //     mockProvider.clearLocation();
    // }
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManagerService::SetTestProviderEnabled(
    /* [in] */ const String& provider,
    /* [in] */ Boolean enabled)
{
    // checkMockPermissionsSafe();
    // synchronized (mLock) {
    //     MockProvider mockProvider = mMockProviders.get(provider);
    //     if (mockProvider == null) {
    //         throw new IllegalArgumentException("Provider \"" + provider + "\" unknown");
    //     }
    //     long identity = Binder.clearCallingIdentity();
    //     if (enabled) {
    //         mockProvider.enable();
    //         mEnabledProviders.add(provider);
    //         mDisabledProviders.remove(provider);
    //     } else {
    //         mockProvider.disable();
    //         mEnabledProviders.remove(provider);
    //         mDisabledProviders.add(provider);
    //     }
    //     updateProvidersLocked();
    //     Binder.restoreCallingIdentity(identity);
    // }
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManagerService::ClearTestProviderEnabled(
    /* [in] */ const String& provider)
{
    // checkMockPermissionsSafe();
    // synchronized (mLock) {
    //     MockProvider mockProvider = mMockProviders.get(provider);
    //     if (mockProvider == null) {
    //         throw new IllegalArgumentException("Provider \"" + provider + "\" unknown");
    //     }
    //     long identity = Binder.clearCallingIdentity();
    //     mEnabledProviders.remove(provider);
    //     mDisabledProviders.remove(provider);
    //     updateProvidersLocked();
    //     Binder.restoreCallingIdentity(identity);
    // }
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManagerService::SetTestProviderStatus(
    /* [in] */ const String& provider,
    /* [in] */ Int32 status,
    /* [in] */ IBundle* extras,
    /* [in] */ Int64 updateTime)
{
    // checkMockPermissionsSafe();
    // synchronized (mLock) {
    //     MockProvider mockProvider = mMockProviders.get(provider);
    //     if (mockProvider == null) {
    //         throw new IllegalArgumentException("Provider \"" + provider + "\" unknown");
    //     }
    //     mockProvider.setStatus(status, extras, updateTime);
    // }
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManagerService::ClearTestProviderStatus(
    /* [in] */ const String& provider)
{
    // checkMockPermissionsSafe();
    // synchronized (mLock) {
    //     MockProvider mockProvider = mMockProviders.get(provider);
    //     if (mockProvider == null) {
    //         throw new IllegalArgumentException("Provider \"" + provider + "\" unknown");
    //     }
    //     mockProvider.clearStatus();
    // }
    return E_NOT_IMPLEMENTED;
}

void CLocationManagerService::HandleLocationChanged(
    /* [in] */ ILocation* location,
    /* [in] */ Boolean passive)
{
    // log("LocationWorkerHandler: MESSAGE_LOCATION_CHANGED!");

    Mutex::Autolock lock(&mLock);

    String provider;
    location->GetProvider(&provider);

    if (!passive) {
        // notify other providers of the new location
        List< AutoPtr<ILocationProviderInterface> >::ReverseIterator rit;
        for (rit = mProviders.RBegin(); rit != mProviders.REnd(); ++rit) {
            ILocationProviderInterface* p = *rit;
            String name;
            p->GetName(&name);
            if (!provider.Equals(name)) {
                p->UpdateLocation(location);
            }
        }
    }

    if (IsAllowedBySettingsLocked(provider)) {
        HandleLocationChangedLocked(location, passive);
    }
}
