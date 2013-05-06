
#include "location/CLocationManager.h"
#include "location/CLocation.h"
#include "location/CGpsStatus.h"
#include "location/DummyLocationProvider.h"
#include "os/CApartment.h"
#include "os/CBundle.h"

const CString CLocationManager::TAG = "LocationManager";

CLocationManager::ListenerTransport::ListenerTransport(
    /* [in] */ ILocalLocationListener* listener,
    /* [in] */ IApartment* apartment)
    : mLocalListener(listener)
{
    if (apartment == NULL) {
        assert(SUCCEEDED(CApartment::GetMainApartment((IApartment**)&mListenerHandler))
        && (mListenerHandler != NULL));
    }
    else {
        mListenerHandler = apartment;
    }
}

PInterface CLocationManager::ListenerTransport::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }
    else if (riid == EIID_ILocationListener) {
        return (ILocationListener*)this;
    }

    return NULL;
}

UInt32 CLocationManager::ListenerTransport::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CLocationManager::ListenerTransport::Release()
{
    return ElRefBase::Release();
}

ECode CLocationManager::ListenerTransport::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManager::ListenerTransport::OnLocationChanged(
    /* [in] */ ILocation* location)
{
    void (STDCALL CLocationManager::ListenerTransport::*pHandlerFunc)(ILocation*);
    pHandlerFunc = &CLocationManager::ListenerTransport::HandleLocationChanged;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr((IInterface*)location);

    return mListenerHandler->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

void CLocationManager::ListenerTransport::HandleLocationChanged(
    /* [in] */ ILocation* location)
{
    AutoPtr<ILocation> nativeLocation;
    CLocation::New(location, (ILocation**)&nativeLocation);
    mLocalListener->OnLocationChanged(nativeLocation);
}

ECode CLocationManager::ListenerTransport::OnStatusChanged(
    /* [in] */ const String& provider,
    /* [in] */ Int32 status,
    /* [in] */ IBundle* extras)
{
    void (STDCALL CLocationManager::ListenerTransport::*pHandlerFunc)(const String&, Int32, IBundle*);
    pHandlerFunc = &CLocationManager::ListenerTransport::HandleStatusChanged;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteString(provider);
    params->WriteInt32(status);
    if (extras != NULL) {
        params->WriteInterfacePtr((IInterface*)extras);
    }

    return mListenerHandler->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

void CLocationManager::ListenerTransport::HandleStatusChanged(
    /* [in] */ const String& provider,
    /* [in] */ Int32 status,
    /* [in] */ IBundle* extras)
{
    mLocalListener->OnStatusChanged(provider, status, extras);
}

ECode CLocationManager::ListenerTransport::OnProviderEnabled(
    /* [in] */ const String& provider)
{
    void (STDCALL CLocationManager::ListenerTransport::*pHandlerFunc)(const String&);
    pHandlerFunc = &CLocationManager::ListenerTransport::HandleProviderEnabled;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteString(provider);

    return mListenerHandler->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

void CLocationManager::ListenerTransport::HandleProviderEnabled(
    /* [in] */ const String& provider)
{
    mLocalListener->OnProviderEnabled(provider);
}

ECode CLocationManager::ListenerTransport::OnProviderDisabled(
    /* [in] */ const String& provider)
{
    void (STDCALL CLocationManager::ListenerTransport::*pHandlerFunc)(const String&);
    pHandlerFunc = &CLocationManager::ListenerTransport::HandleProviderDisabled;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteString(provider);

    return mListenerHandler->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

void CLocationManager::ListenerTransport::HandleProviderDisabled(
    /* [in] */ const String& provider)
{
    mLocalListener->OnProviderDisabled(provider);
}

AutoPtr<ILocalLocationProvider> CLocationManager::CreateProvider(
    /* [in] */ const String& name,
    /* [in] */ IBundle* info)
{
    DummyLocationProvider* provider =
        new DummyLocationProvider(name, mService);

    Boolean value;
    info->GetBoolean(String("network"), &value);
    provider->SetRequiresNetwork(value);

    info->GetBoolean(String("satellite"), &value);
    provider->SetRequiresSatellite(value);

    info->GetBoolean(String("cell"), &value);
    provider->SetRequiresCell(value);

    info->GetBoolean(String("cost"), &value);
    provider->SetHasMonetaryCost(value);

    info->GetBoolean(String("altitude"), &value);
    provider->SetSupportsAltitude(value);

    info->GetBoolean(String("speed"), &value);
    provider->SetSupportsSpeed(value);

    info->GetBoolean(String("bearing"), &value);
    provider->SetSupportsBearing(value);

    info->GetBoolean(String("power"), &value);
    provider->SetPowerRequirement(value);

    info->GetBoolean(String("accuracy"), &value);
    provider->SetAccuracy(value);

    return AutoPtr<ILocalLocationProvider>((ILocalLocationProvider*)provider);
}

CLocationManager::CLocationManager()
{
    ASSERT_SUCCEEDED(CGpsStatus::New((IGpsStatus**)&mGpsStatus));
}

ECode CLocationManager::GetAllProviders(
    /* [out] */ IObjectContainer** allProviders)
{
//    if (FALSE) {
//        Log.d(TAG, "getAllProviders");
//    }
//    try {
    VALIDATE_NOT_NULL(allProviders);

    ECode ec = mService->GetAllProviders(allProviders);
    if (FAILED(ec)) {
//        Log.e(TAG, "getAllProviders: RemoteException", ex);
        *allProviders = NULL;
    }
//    } catch (RemoteException ex) {
//        Log.e(TAG, "getAllProviders: RemoteException", ex);
//    }
    return ec;
}

ECode CLocationManager::GetProviders(
    /* [in] */ Boolean enabledOnly,
    /* [out] */ IObjectContainer** providers)
{
//    try {
    VALIDATE_NOT_NULL(providers);

    ECode ec = mService->GetProviders(NULL, enabledOnly, providers);
    if (FAILED(ec)) {
//        Log.e(TAG, "getProviders: RemoteException", ex);
        *providers = NULL;
    }
//    } catch (RemoteException ex) {
//        Log.e(TAG, "getProviders: RemoteException", ex);
//    }
    return ec;
}

ECode CLocationManager::GetProvider(
    /* [in] */ const String& name,
    /* [out] */ ILocalLocationProvider** provider)
{
    VALIDATE_NOT_NULL(provider);

    if (name.IsNull()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("name==null");
    }
//    try {
    AutoPtr<IBundle> info;
    ECode ec = mService->GetProviderInfo(name, (IBundle**)&info);
    if (FAILED(ec)){
//        Log.e(TAG, "getProvider: RemoteException", ex);
        *provider = NULL;
        return ec;
    }
    if (info == NULL) {
        *provider = NULL;
        return NOERROR;
    }

    *provider =  CreateProvider(name, info);
//    } catch (RemoteException ex) {
//        Log.e(TAG, "getProvider: RemoteException", ex);
//    }

    return NOERROR;
}

ECode CLocationManager::GetProvidersEx(
    /* [in] */ ICriteria* criteria,
    /* [in] */ Boolean enabledOnly,
    /* [out] */ IObjectContainer** providers)
{
    VALIDATE_NOT_NULL(providers);

    if (criteria == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("criteria==null");
    }
//    try {
    ECode ec = mService->GetProviders(criteria, enabledOnly, providers);
    if (FAILED(ec)) {
//        Log.e(TAG, "getProviders: RemoteException", ex);
        *providers = NULL;
    }
//    } catch (RemoteException ex) {
//        Log.e(TAG, "getProviders: RemoteException", ex);
//    }
    return ec;
}

ECode CLocationManager::GetBestProvider(
    /* [in] */ ICriteria* criteria,
    /* [in] */ Boolean enabledOnly,
    /* [out] */ String* provider)
{
    VALIDATE_NOT_NULL(provider);

    if (criteria == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("criteria==null");
    }
//    try {
    ECode ec = mService->GetBestProvider(criteria, enabledOnly, provider);
    if (FAILED(ec)) {
//        Log.e(TAG, "getBestProvider: RemoteException", ex);
        *provider = String(NULL);
    }
//    } catch (RemoteException ex) {
//        Log.e(TAG, "getBestProvider: RemoteException", ex);
//    }
    return ec;
}

ECode CLocationManager::RequestLocationUpdates(
    /* [in] */ const String& provider,
    /* [in] */ Int64 minTime,
    /* [in] */ Float minDistance,
    /* [in] */ ILocalLocationListener* listener)
{
    if (provider.IsNull()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("provider==null");
    }
    if (listener == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("listener==null");
    }
    _RequestLocationUpdates(provider, NULL, minTime, minDistance, FALSE, listener, NULL);

    return NOERROR;
}

ECode CLocationManager::RequestLocationUpdatesEx(
    /* [in] */ const String& provider,
    /* [in] */ Int64 minTime,
    /* [in] */ Float minDistance,
    /* [in] */ ILocalLocationListener* listener,
    /* [in] */ IApartment* apartment)
{
    if (provider.IsNull()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("provider==null");
    }
    if (listener == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("listener==null");
    }
    _RequestLocationUpdates(provider, NULL, minTime, minDistance, FALSE, listener, apartment);

    return NOERROR;
}

ECode CLocationManager::RequestLocationUpdatesEx2(
    /* [in] */ Int64 minTime,
    /* [in] */ Float minDistance,
    /* [in] */ ICriteria* criteria,
    /* [in] */ ILocalLocationListener* listener,
    /* [in] */ IApartment* apartment)
{
    if (criteria == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("criteria==null");
    }
    if (listener == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("listener==null");
    }
    _RequestLocationUpdates(String(NULL), criteria, minTime, minDistance, FALSE, listener, apartment);

    return NOERROR;
}

void CLocationManager::_RequestLocationUpdates(
    /* [in] */ const String& provider,
    /* [in] */ ICriteria* criteria,
    /* [in] */ Int64 minTime,
    /* [in] */ Float minDistance,
    /* [in] */ Boolean singleShot,
    /* [in] */ ILocalLocationListener* listener,
    /* [in] */ IApartment* apartment)
{
    if (minTime < 0L) {
        minTime = 0L;
    }
    if (minDistance < 0.0f) {
        minDistance = 0.0f;
    }

//    try {
    Mutex::Autolock lock(mListenersLock);

    AutoPtr<ListenerTransport> transport;
    HashMap<AutoPtr<ILocalLocationListener>, AutoPtr<ListenerTransport> >::Iterator it
            = mListeners.Find(listener);
    if (it != mListeners.End()) {
        transport = it->mSecond;
    }

    if (transport == NULL) {
        transport = new ListenerTransport(listener, apartment);
    }
    mListeners[listener] = transport;
    ECode ec = mService->RequestLocationUpdates(provider, criteria, minTime, minDistance,
            singleShot, (ILocationListener*)transport);
    if (FAILED(ec)) {
//        Log.e(TAG, "requestLocationUpdates: DeadObjectException", ex);
    }
//    } catch (RemoteException ex) {
//        Log.e(TAG, "requestLocationUpdates: DeadObjectException", ex);
//    }
}

ECode CLocationManager::RequestLocationUpdatesPI(
    /* [in] */ const String& provider,
    /* [in] */ Int64 minTime,
    /* [in] */ Float minDistance,
    /* [in] */ IPendingIntent* intent)
{
    if (provider.IsNull()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("provider==null");
    }
    if (intent == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("intent==null");
    }
    _RequestLocationUpdatesPI(provider, NULL, minTime, minDistance, FALSE, intent);

    return NOERROR;
}

ECode CLocationManager::RequestLocationUpdatesPIEx(
    /* [in] */ Int64 minTime,
    /* [in] */ Float minDistance,
    /* [in] */ ICriteria* criteria,
    /* [in] */ IPendingIntent* intent)
{
    if (criteria == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("criteria==null");
    }
    if (intent == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("intent==null");
    }
    _RequestLocationUpdatesPI(String(NULL), criteria, minTime, minDistance, FALSE, intent);

    return NOERROR;
}

void CLocationManager::_RequestLocationUpdatesPI(
    /* [in] */ const String& provider,
    /* [in] */ ICriteria* criteria,
    /* [in] */ Int64 minTime,
    /* [in] */ Float minDistance,
    /* [in] */ Boolean singleShot,
    /* [in] */ IPendingIntent* intent)
{
    if (minTime < 0L) {
        minTime = 0L;
    }
    if (minDistance < 0.0f) {
        minDistance = 0.0f;
    }

//    try {
    ECode ec = mService->RequestLocationUpdatesPI(provider, criteria, minTime, minDistance, singleShot, intent);
    if (FAILED(ec)) {
//        Log.e(TAG, "requestLocationUpdates: RemoteException", ex);
    }
//    } catch (RemoteException ex) {
//        Log.e(TAG, "requestLocationUpdates: RemoteException", ex);
//    }
}

ECode CLocationManager::RequestSingleUpdate(
    /* [in] */ const String& provider,
    /* [in] */ ILocalLocationListener* listener,
    /* [in] */ IApartment* apartment)
{
    if (provider.IsNull()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("provider==null");
    }
    if (listener == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("listener==null");
    }
    _RequestLocationUpdates(provider, NULL, 0L, 0.0f, TRUE, listener, apartment);

    return NOERROR;
}

ECode CLocationManager::RequestSingleUpdateEx(
    /* [in] */ ICriteria* criteria,
    /* [in] */ ILocalLocationListener* listener,
    /* [in] */ IApartment* apartment)
{
    if (criteria == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("criteria==null");
    }
    if (listener == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("listener==null");
    }
    _RequestLocationUpdates(String(NULL), criteria, 0L, 0.0f, TRUE, listener, apartment);

    return NOERROR;
}

ECode CLocationManager::RequestSingleUpdatePI(
    /* [in] */ const String& provider,
    /* [in] */ IPendingIntent* intent)
{
    if (provider.IsNull()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("provider==null");
    }
    if (intent == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("intent==null");
    }
    _RequestLocationUpdatesPI(provider, NULL, 0L, 0.0f, TRUE, intent);

    return NOERROR;
}

ECode CLocationManager::RequestSingleUpdatePIEx(
    /* [in] */ ICriteria* criteria,
    /* [in] */ IPendingIntent* intent)
{
    if (criteria == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("criteria==null");
    }
    if (intent == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("intent==null");
    }
    _RequestLocationUpdatesPI(String(NULL), criteria, 0L, 0.0f, TRUE, intent);

    return NOERROR;
}

ECode CLocationManager::RemoveUpdates(
    /* [in] */ ILocalLocationListener* listener)
{
    if (listener == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("listener==null");
    }
//    if (FALSE) {
//        Log.d(TAG, "removeUpdates: listener = " + listener);
//    }
//    try {
    AutoPtr<ListenerTransport> transport;
    HashMap<AutoPtr<ILocalLocationListener>, AutoPtr<ListenerTransport> >::Iterator it
             = mListeners.Find(listener);
    if (it != mListeners.End()) {
        transport = it->mSecond;
        mListeners.Erase(it);
    }

    ECode ec = NOERROR;
    if (transport != NULL) {
        ec = mService->RemoveUpdates((ILocationListener*)transport.Get());
        if (FAILED(ec)) {
//            Log.e(TAG, "removeUpdates: DeadObjectException", ex);
        }
    }

    return ec;
//    } catch (RemoteException ex) {
//        Log.e(TAG, "removeUpdates: DeadObjectException", ex);
//    }
}

ECode CLocationManager::RemoveUpdatesPI(
    /* [in] */ IPendingIntent* intent)
{
    if (intent == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("intent==null");
    }
    if (FALSE) {
//        Log.d(TAG, "removeUpdates: intent = " + intent);
    }
//    try {
    ECode ec = mService->RemoveUpdatesPI(intent);
    if (FAILED(ec)) {
//        Log.e(TAG, "removeUpdates: RemoteException", ex);
    }

    return ec;
//    } catch (RemoteException ex) {
//        Log.e(TAG, "removeUpdates: RemoteException", ex);
//    }
}

ECode CLocationManager::AddProximityAlert(
    /* [in] */ Double latitude,
    /* [in] */ Double longitude,
    /* [in] */ Float distance,
    /* [in] */ Int64 expiration,
    /* [in] */ IPendingIntent* intent)
{
    if (FALSE) {
//        Log.d(TAG, "addProximityAlert: latitude = " + latitude +
//            ", longitude = " + longitude + ", radius = " + radius +
//            ", expiration = " + expiration +
//            ", intent = " + intent);
    }
//    try {
    ECode ec = mService->AddProximityAlert(latitude, longitude, distance,
                                   expiration, intent);
    if (FAILED(ec)) {
//        Log.e(TAG, "addProximityAlert: RemoteException", ex);
    }

    return ec;
//    } catch (RemoteException ex) {
//        Log.e(TAG, "addProximityAlert: RemoteException", ex);
//    }
}

ECode CLocationManager::RemoveProximityAlert(
    /* [in] */ IPendingIntent* intent)
{
    if (FALSE) {
//        Log.d(TAG, "removeProximityAlert: intent = " + intent);
    }
//    try {
    ECode ec = mService->RemoveProximityAlert(intent);
    if (FAILED(ec)) {
//        Log.e(TAG, "removeProximityAlert: RemoteException", ex);
    }

    return ec;
//    } catch (RemoteException ex) {
//        Log.e(TAG, "removeProximityAlert: RemoteException", ex);
//  }
}

ECode CLocationManager::IsProviderEnabled(
    /* [in] */ const String& provider,
    /* [out] */ Boolean* isEnabled)
{
    VALIDATE_NOT_NULL(isEnabled);

    if (provider.IsNull()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("provider==null");
    }
//    try {
    ECode ec = mService->IsProviderEnabled(provider, isEnabled);
    if (FAILED(ec)) {
        *isEnabled = FALSE;
//        Log.e(TAG, "isProviderEnabled: RemoteException", ex);
    }

    return ec;
//    } catch (RemoteException ex) {
//        Log.e(TAG, "isProviderEnabled: RemoteException", ex);
//        return false;
//    }
}

ECode CLocationManager::GetLastKnownLocation(
    /* [in] */ const String& provider,
    /* [out] */ ILocation** location)
{
    VALIDATE_NOT_NULL(location);

    if (provider.IsNull()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("provider==null");
    }
//    try {
    ECode ec = mService->GetLastKnownLocation(provider, location);
    if (FAILED(ec)) {
        *location = NULL;
//        Log.e(TAG, "getLastKnowLocation: RemoteException", ex);
    }

    return ec;
//    } catch (RemoteException ex) {
//        Log.e(TAG, "getLastKnowLocation: RemoteException", ex);
//        return null;
//    }
}

ECode CLocationManager::AddTestProvider(
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
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManager::RemoveTestProvider(
    /* [in] */ const String& provider)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManager::SetTestProviderLocation(
    /* [in] */ const String& provider,
    /* [in] */ ILocation* loc)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManager::ClearTestProviderLocation(
    /* [in] */ const String& provider)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManager::SetTestProviderEnabled(
    /* [in] */ const String& provider,
    /* [in] */ Boolean enabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManager::ClearTestProviderEnabled(
    /* [in] */ const String& provider)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManager::SetTestProviderStatus(
    /* [in] */ const String& provider,
    /* [in] */ Int32 status,
    /* [in] */ IBundle* extras,
    /* [in] */ Int64 updateTime)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManager::ClearTestProviderStatus(
    /* [in] */ const String& provider)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

CLocationManager::GpsStatusListenerTransport::Nmea::Nmea(
    /* [in] */ Int64 timestamp,
    /* [in] */ const String& nmea)
    : mTimestamp(timestamp)
    , mNmea(nmea)
{
}

CLocationManager::GpsStatusListenerTransport::GpsStatusListenerTransport(
    /* [in] */ CLocationManager* locManager,
    /* [in] */ ILocalGpsStatusListener* listener)
    : mLocationManager(locManager)
    , mListener(listener)
{
}

CLocationManager::GpsStatusListenerTransport::GpsStatusListenerTransport(
    /* [in] */ CLocationManager* locManager,
    /* [in] */ ILocalGpsStatusNmeaListener* listener)
    : mLocationManager(locManager)
    , mNmeaListener(listener)
{
}

PInterface CLocationManager::GpsStatusListenerTransport::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }
    else if (riid == EIID_IGpsStatusListener) {
        return (IGpsStatusListener*)this;
    }

    return NULL;
}

UInt32 CLocationManager::GpsStatusListenerTransport::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CLocationManager::GpsStatusListenerTransport::Release()
{
    return ElRefBase::Release();
}

ECode CLocationManager::GpsStatusListenerTransport::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CLocationManager::GpsStatusListenerTransport::OnGpsStarted()
{
    if (mListener != NULL) {
        void (STDCALL CLocationManager::GpsStatusListenerTransport::*pHandlerFunc)(Int32);
        pHandlerFunc = &CLocationManager::GpsStatusListenerTransport::HandleGpsChanged;

        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        params->WriteInt32(GpsStatus_GPS_EVENT_STARTED);

        return mGpsHandler->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
    }

    return NOERROR;
}

ECode CLocationManager::GpsStatusListenerTransport::OnGpsStopped()
{
    if (mListener != NULL) {
        void (STDCALL CLocationManager::GpsStatusListenerTransport::*pHandlerFunc)(Int32);
        pHandlerFunc = &CLocationManager::GpsStatusListenerTransport::HandleGpsChanged;

        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        params->WriteInt32(GpsStatus_GPS_EVENT_STOPPED);

        return mGpsHandler->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
    }

    return NOERROR;
}

ECode CLocationManager::GpsStatusListenerTransport::OnFirstFix(
    /* [in] */ Int32 ttff)
{
    if (mListener != NULL) {
        mLocationManager->mGpsStatus->SetTimeToFirstFix(ttff);

        void (STDCALL CLocationManager::GpsStatusListenerTransport::*pHandlerFunc)(Int32);
        pHandlerFunc = &CLocationManager::GpsStatusListenerTransport::HandleGpsChanged;

        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        params->WriteInt32(GpsStatus_GPS_EVENT_FIRST_FIX);

        return mGpsHandler->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
    }

    return NOERROR;
}

ECode CLocationManager::GpsStatusListenerTransport::OnSvStatusChanged(
    /* [in] */ Int32 svCount,
    /* [in] */ const ArrayOf<Int32>& prns,
    /* [in] */ const ArrayOf<Float>& snrs,
    /* [in] */ const ArrayOf<Float>& elevations,
    /* [in] */ const ArrayOf<Float>& azimuths,
    /* [in] */ Int32 ephemerisMask,
    /* [in] */ Int32 almanacMask,
    /* [in] */ Int32 usedInFixMask)
{
    if (mListener != NULL) {
        mLocationManager->mGpsStatus->SetStatus(svCount, prns, snrs, elevations, azimuths,
                ephemerisMask, almanacMask, usedInFixMask);

        void (STDCALL CLocationManager::GpsStatusListenerTransport::*pHandlerFunc)(Int32);
        pHandlerFunc = &CLocationManager::GpsStatusListenerTransport::HandleGpsChanged;

        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        params->WriteInt32(GpsStatus_GPS_EVENT_SATELLITE_STATUS);

        mGpsHandler->RemoveCppCallbacks((Handle32)this, *(Handle32*)&pHandlerFunc);
        return mGpsHandler->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
    }

    return NOERROR;
}

void CLocationManager::GpsStatusListenerTransport::HandleGpsChanged(
    /* [in] */ Int32 event)
{
    // synchronize on mGpsStatus to ensure the data is copied atomically.
    Mutex::Autolock lock(mLocationManager->mGpsStatusLock);

    mListener->OnGpsStatusChanged(event);
}

ECode CLocationManager::GpsStatusListenerTransport::OnNmeaReceived(
    /* [in] */ Int64 timestamp,
    /* [in] */ const String& nmea)
{
    if (mNmeaListener != NULL) {
        {
            Mutex::Autolock lock(mNmeaBufferLock);
            mNmeaBuffer.PushBack(new Nmea(timestamp, nmea));
        }

        void (STDCALL CLocationManager::GpsStatusListenerTransport::*pHandlerFunc)();
        pHandlerFunc = &CLocationManager::GpsStatusListenerTransport::HandleNmeaReceived;

        mGpsHandler->RemoveCppCallbacks((Handle32)this, *(Handle32*)&pHandlerFunc);
        return mGpsHandler->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, NULL, 0);
    }

    return NOERROR;
}

void CLocationManager::GpsStatusListenerTransport::HandleNmeaReceived()
{
    Mutex::Autolock lock(mNmeaBufferLock);

    List<Nmea*>::Iterator it;
    for (it = mNmeaBuffer.Begin(); it != mNmeaBuffer.End(); ++it) {
        Nmea* nmea = *it;
        mNmeaListener->OnNmeaReceived(nmea->mTimestamp, nmea->mNmea);
    }
    mNmeaBuffer.Clear();
}

ECode CLocationManager::AddGpsStatusListener(
    /* [in] */ ILocalGpsStatusListener* listener,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    HashMap<AutoPtr<ILocalGpsStatusListener>, AutoPtr<GpsStatusListenerTransport> >::Iterator
            it = mGpsStatusListeners.Find(listener);
    if (it != mGpsStatusListeners.End()) {
        // listener is already registered
        *result = TRUE;
        return NOERROR;
    }
//    try {
    AutoPtr<GpsStatusListenerTransport> transport = new GpsStatusListenerTransport(this, listener);
    ECode ec = mService->AddGpsStatusListener((IGpsStatusListener*)transport, result);
    if (FAILED(ec)) {
//        Log.e(TAG, "RemoteException in registerGpsStatusListener: ", e);
        *result  = FALSE;
    }
    if (*result) {
        mGpsStatusListeners[listener] = transport;
    }

    return ec;
//    } catch (RemoteException e) {
//        Log.e(TAG, "RemoteException in registerGpsStatusListener: ", e);
//        result = false;
//    }
}

ECode CLocationManager::RemoveGpsStatusListener(
    /* [in] */ ILocalGpsStatusListener* listener)
{
//    try {
    AutoPtr<GpsStatusListenerTransport> transport;
    HashMap<AutoPtr<ILocalGpsStatusListener>, AutoPtr<GpsStatusListenerTransport> >::Iterator
            it = mGpsStatusListeners.Find(listener);
    if (it != mGpsStatusListeners.End()) {
        transport = it->mSecond;
        mGpsStatusListeners.Erase(it);
    }

    ECode ec = NOERROR;
    if (transport != NULL) {
        ec = mService->RemoveGpsStatusListener((IGpsStatusListener*)transport);
        if (FAILED(ec)) {
//            Log.e(TAG, "RemoteException in unregisterGpsStatusListener: ", e);
        }
    }

    return ec;
//    } catch (RemoteException e) {
//        Log.e(TAG, "RemoteException in unregisterGpsStatusListener: ", e);
//    }
}

ECode CLocationManager::AddNmeaListener(
    /* [in] */ ILocalGpsStatusNmeaListener* listener,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    HashMap<AutoPtr<ILocalGpsStatusNmeaListener>, AutoPtr<GpsStatusListenerTransport> >::Iterator
            it = mNmeaListeners.Find(listener);
    if (it != mNmeaListeners.End()) {
        // listener is already registered
        *result = TRUE;
        return NOERROR;
    }
//    try {
    AutoPtr<GpsStatusListenerTransport> transport = new GpsStatusListenerTransport(this, listener);
    ECode ec = mService->AddGpsStatusListener((IGpsStatusListener*)transport, result);
    if (FAILED(ec)) {
//        Log.e(TAG, "RemoteException in registerGpsStatusListener: ", e);
        *result = FALSE;
        return ec;
    }
    if (*result) {
        mNmeaListeners[listener] = transport;
    }
//    } catch (RemoteException e) {
//        Log.e(TAG, "RemoteException in registerGpsStatusListener: ", e);
//        result = false;
//    }

    return NOERROR;
}

ECode CLocationManager::RemoveNmeaListener(
    /* [in] */ ILocalGpsStatusNmeaListener* listener)
{
//    try {
    AutoPtr<GpsStatusListenerTransport> transport;
    HashMap<AutoPtr<ILocalGpsStatusNmeaListener>, AutoPtr<GpsStatusListenerTransport> >::Iterator
            it = mNmeaListeners.Find(listener);
    if (it != mNmeaListeners.End()) {
        transport = it->mSecond;
        mNmeaListeners.Erase(it);
    }

    ECode ec = NOERROR;
    if (transport != NULL) {
        ec = mService->RemoveGpsStatusListener((IGpsStatusListener*)transport);
        if (FAILED(ec)) {
//            Log.e(TAG, "RemoteException in unregisterGpsStatusListener: ", e);
        }
    }

    return ec;
//    } catch (RemoteException e) {
//        Log.e(TAG, "RemoteException in unregisterGpsStatusListener: ", e);
//    }
}

ECode CLocationManager::GetGpsStatus(
    /* [in] */ IGpsStatus* inStatus,
    /* [out] */ IGpsStatus** outStatus)
{
    VALIDATE_NOT_NULL(outStatus);
    if (inStatus == NULL) {
        ASSERT_SUCCEEDED(CGpsStatus::New(outStatus));
    }
    else {
        *outStatus = inStatus;
    }
    (*outStatus)->SetStatusEx(mGpsStatus);

    return NOERROR;
}

ECode CLocationManager::SendExtraCommand(
    /* [in] */ const String& provider,
    /* [in] */ const String& command,
    /* [in] */ IBundle* inExtras, //inout
    /* [out] */ IBundle** outExtras,
    /* [out] */ Boolean* result)
{
//    try {
    VALIDATE_NOT_NULL(result);
    return mService->SendExtraCommand(provider, command, inExtras, outExtras, result);
//    } catch (RemoteException e) {
//        Log.e(TAG, "RemoteException in sendExtraCommand: ", e);
//        return false;
//    }
}

ECode CLocationManager::SendNiResponse(
    /* [in] */ Int32 notifId,
    /* [in] */ Int32 userResponse,
    /* [out] */ Boolean* result)
{
//    try {
    VALIDATE_NOT_NULL(result);
    return mService->SendNiResponse(notifId, userResponse, result);
//    } catch (RemoteException e) {
//        Log.e(TAG, "RemoteException in sendNiResponse: ", e);
//        return false;
//    }
}

ECode CLocationManager::constructor(
    /* [in] */ ILocationManager* service)
{
    mService = service;

    return NOERROR;
}
