
#include "location/LocationManager.h"
#include "location/CLocation.h"
#include "location/CGpsStatus.h"
#include "location/DummyLocationProvider.h"
#include "os/CApartment.h"
#include "os/CBundle.h"

const CString LocationManager::TAG = "LocationManager";

LocationManager::LocationManager()
    : mGpsStatusListeners(5)
    , mNmeaListeners(5)
    , mListeners(5)
{
    ASSERT_SUCCEEDED(CGpsStatus::New((IGpsStatus**)&mGpsStatus));
}

ECode LocationManager::Init(
    /* [in] */ ILocationManager* service)
{
    mService = service;
    return NOERROR;
}

AutoPtr<ILocalLocationProvider> LocationManager::CreateProvider(
    /* [in] */ const String& name,
    /* [in] */ IBundle* info)
{
    AutoPtr<DummyLocationProvider> provider =
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
    return (ILocalLocationProvider*)provider.Get();
}

ECode LocationManager::GetAllProviders(
    /* [out] */ IObjectContainer** allProviders)
{
    assert(allProviders != NULL);
//    if (FALSE) {
//        Log.d(TAG, "getAllProviders");
//    }
//    try {
    ECode ec = mService->GetAllProviders(allProviders);
    if (FAILED(ec)) {
//        Log.e(TAG, "getAllProviders: RemoteException", ex);
        *allProviders = NULL;
    }
//    } catch (RemoteException ex) {
//        Log.e(TAG, "getAllProviders: RemoteException", ex);
//    }
    return NOERROR;
}

ECode LocationManager::GetProviders(
    /* [in] */ Boolean enabledOnly,
    /* [out] */ IObjectContainer** providers)
{
    assert(providers != NULL);
//    try {
    ECode ec = mService->GetProviders(NULL, enabledOnly, providers);
    if (FAILED(ec)) {
//        Log.e(TAG, "getProviders: RemoteException", ex);
        *providers = NULL;
    }
//    } catch (RemoteException ex) {
//        Log.e(TAG, "getProviders: RemoteException", ex);
//    }
    return NOERROR;
}

ECode LocationManager::GetProvider(
    /* [in] */ const String& name,
    /* [out] */ ILocalLocationProvider** provider)
{
    assert(provider != NULL);

    if (name.IsNull()) {
//        throw new IllegalArgumentException("name==null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
//    try {
    AutoPtr<IBundle> info;
    ECode ec = mService->GetProviderInfo(name, (IBundle**)&info);
    if (FAILED(ec)){
//        Log.e(TAG, "getProvider: RemoteException", ex);
        *provider = NULL;
        return NOERROR;
    }
    if (info == NULL) {
        *provider = NULL;
        return NOERROR;
    }
    AutoPtr<ILocalLocationProvider> _p = CreateProvider(name, info);
    *provider = _p;
    (*provider)->AddRef();
//    } catch (RemoteException ex) {
//        Log.e(TAG, "getProvider: RemoteException", ex);
//    }
    return NOERROR;
}

ECode LocationManager::GetProviders(
    /* [in] */ ICriteria* criteria,
    /* [in] */ Boolean enabledOnly,
    /* [out] */ IObjectContainer** providers)
{
    assert(providers != NULL);

    if (criteria == NULL) {
//        throw new IllegalArgumentException("criteria==null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
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
    return NOERROR;
}

ECode LocationManager::GetBestProvider(
    /* [in] */ ICriteria* criteria,
    /* [in] */ Boolean enabledOnly,
    /* [out] */ String* provider)
{
    assert(provider != NULL);

    if (criteria == NULL) {
//        throw new IllegalArgumentException("criteria==null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
//    try {
    ECode ec = mService->GetBestProvider(criteria, enabledOnly, provider);
    if (FAILED(ec)) {
//        Log.e(TAG, "getBestProvider: RemoteException", ex);
        *provider = NULL;
    }
//    } catch (RemoteException ex) {
//        Log.e(TAG, "getBestProvider: RemoteException", ex);
//    }
    return NOERROR;
}

ECode LocationManager::RequestLocationUpdates(
    /* [in] */ const String& provider,
    /* [in] */ Int64 minTime,
    /* [in] */ Float minDistance,
    /* [in] */ ILocalLocationListener* listener)
{
    if (provider.IsNull()) {
//        throw new IllegalArgumentException("provider==null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (listener == NULL) {
//        throw new IllegalArgumentException("listener==null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _RequestLocationUpdates(provider, NULL, minTime, minDistance, FALSE, listener, NULL);
    return NOERROR;
}

ECode LocationManager::RequestLocationUpdates(
    /* [in] */ const String& provider,
    /* [in] */ Int64 minTime,
    /* [in] */ Float minDistance,
    /* [in] */ ILocalLocationListener* listener,
    /* [in] */ IApartment* apartment)
{
    if (provider.IsNull()) {
//        throw new IllegalArgumentException("provider==null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (listener == NULL) {
//        throw new IllegalArgumentException("listener==null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _RequestLocationUpdates(provider, NULL, minTime, minDistance, FALSE, listener, apartment);
    return NOERROR;
}

ECode LocationManager::RequestLocationUpdates(
    /* [in] */ Int64 minTime,
    /* [in] */ Float minDistance,
    /* [in] */ ICriteria* criteria,
    /* [in] */ ILocalLocationListener* listener,
    /* [in] */ IApartment* apartment)
{
    if (criteria == NULL) {
//        throw new IllegalArgumentException("criteria==null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (listener == NULL) {
//        throw new IllegalArgumentException("listener==null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _RequestLocationUpdates(String(NULL), criteria, minTime, minDistance, FALSE, listener, apartment);
    return NOERROR;
}

void LocationManager::_RequestLocationUpdates(
    /* [in] */ const String& provider,
    /* [in] */ ICriteria* criteria,
    /* [in] */ Int64 minTime,
    /* [in] */ Float minDistance,
    /* [in] */ Boolean singleShot,
    /* [in] */ ILocalLocationListener* listener,
    /* [in] */ IApartment* apartment)
{
    if (minTime < 0ll) {
        minTime = 0ll;
    }
    if (minDistance < 0.0f) {
        minDistance = 0.0f;
    }

//    try {
    Mutex::Autolock lock(&mListenersLock);

    AutoPtr<CListenerTransport> transport;
    HashMap<AutoPtr<ILocalLocationListener>, AutoPtr<CListenerTransport> >::Iterator it
            = mListeners.Find(listener);
    if (it != mListeners.End()) {
        transport = it->mSecond;
    }
    if (transport == NULL) {
        CListenerTransport::NewByFriend((Handle32)this, listener, apartment,
                (CListenerTransport**)&transport);
    }
    mListeners[listener] = transport;
    ECode ec = mService->RequestLocationUpdates(provider, criteria, minTime, minDistance,
            singleShot, (ILocationListener*)transport.Get());
    if (FAILED(ec)) {
//        Log.e(TAG, "requestLocationUpdates: DeadObjectException", ex);
    }
//    } catch (RemoteException ex) {
//        Log.e(TAG, "requestLocationUpdates: DeadObjectException", ex);
//    }
}

ECode LocationManager::RequestLocationUpdates(
    /* [in] */ const String& provider,
    /* [in] */ Int64 minTime,
    /* [in] */ Float minDistance,
    /* [in] */ IPendingIntent* intent)
{
    if (provider.IsNull()) {
//        throw new IllegalArgumentException("provider==null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (intent == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("intent==null");
    }
    _RequestLocationUpdates(provider, NULL, minTime, minDistance, FALSE, intent);
    return NOERROR;
}

ECode LocationManager::RequestLocationUpdates(
    /* [in] */ Int64 minTime,
    /* [in] */ Float minDistance,
    /* [in] */ ICriteria* criteria,
    /* [in] */ IPendingIntent* intent)
{
    if (criteria == NULL) {
//        throw new IllegalArgumentException("criteria==null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (intent == NULL) {
//        throw new IllegalArgumentException("intent==null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _RequestLocationUpdates(String(NULL), criteria, minTime, minDistance, FALSE, intent);
    return NOERROR;
}

void LocationManager::_RequestLocationUpdates(
    /* [in] */ const String& provider,
    /* [in] */ ICriteria* criteria,
    /* [in] */ Int64 minTime,
    /* [in] */ Float minDistance,
    /* [in] */ Boolean singleShot,
    /* [in] */ IPendingIntent* intent)
{
    if (minTime < 0ll) {
        minTime = 0ll;
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

ECode LocationManager::RequestSingleUpdate(
    /* [in] */ const String& provider,
    /* [in] */ ILocalLocationListener* listener,
    /* [in] */ IApartment* apartment)
{
    if (provider.IsNull()) {
//        throw new IllegalArgumentException("provider==null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (listener == NULL) {
//        throw new IllegalArgumentException("listener==null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _RequestLocationUpdates(provider, NULL, 0ll, 0.0f, TRUE, listener, apartment);
    return NOERROR;
}

ECode LocationManager::RequestSingleUpdate(
    /* [in] */ ICriteria* criteria,
    /* [in] */ ILocalLocationListener* listener,
    /* [in] */ IApartment* apartment)
{
    if (criteria == NULL) {
//        throw new IllegalArgumentException("criteria==null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (listener == NULL) {
//        throw new IllegalArgumentException("listener==null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _RequestLocationUpdates(String(NULL), criteria, 0ll, 0.0f, TRUE, listener, apartment);
    return NOERROR;
}

ECode LocationManager::RequestSingleUpdate(
    /* [in] */ const String& provider,
    /* [in] */ IPendingIntent* intent)
{
    if (provider.IsNull()) {
//        throw new IllegalArgumentException("provider==null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (intent == NULL) {
//        throw new IllegalArgumentException("intent==null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _RequestLocationUpdates(provider, NULL, 0ll, 0.0f, TRUE, intent);
    return NOERROR;
}

ECode LocationManager::RequestSingleUpdate(
    /* [in] */ ICriteria* criteria,
    /* [in] */ IPendingIntent* intent)
{
    if (criteria == NULL) {
//        throw new IllegalArgumentException("criteria==null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (intent == NULL) {
//        throw new IllegalArgumentException("intent==null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _RequestLocationUpdates(String(NULL), criteria, 0ll, 0.0f, TRUE, intent);
    return NOERROR;
}

ECode LocationManager::RemoveUpdates(
    /* [in] */ ILocalLocationListener* listener)
{
    if (listener == NULL) {
//        throw new IllegalArgumentException("listener==null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
//    if (FALSE) {
//        Log.d(TAG, "removeUpdates: listener = " + listener);
//    }
//    try {
    AutoPtr<CListenerTransport> transport;
    HashMap<AutoPtr<ILocalLocationListener>, AutoPtr<CListenerTransport> >::Iterator it
             = mListeners.Find(listener);
    if (it != mListeners.End()) {
        transport = it->mSecond;
        mListeners.Erase(it);
    }
    if (transport != NULL) {
        ECode ec = mService->RemoveUpdates((ILocationListener*)transport.Get());
        if (FAILED(ec)) {
//            Log.e(TAG, "removeUpdates: DeadObjectException", ex);
        }
    }
    return NOERROR;
//    } catch (RemoteException ex) {
//        Log.e(TAG, "removeUpdates: DeadObjectException", ex);
//    }
}

ECode LocationManager::RemoveUpdates(
    /* [in] */ IPendingIntent* intent)
{
    if (intent == NULL) {
//        throw new IllegalArgumentException("intent==null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
//     if (FALSE) {
//        Log.d(TAG, "removeUpdates: intent = " + intent);
//     }
//    try {
    ECode ec = mService->RemoveUpdatesPI(intent);
    if (FAILED(ec)) {
//        Log.e(TAG, "removeUpdates: RemoteException", ex);
    }
    return NOERROR;
//    } catch (RemoteException ex) {
//        Log.e(TAG, "removeUpdates: RemoteException", ex);
//    }
}

ECode LocationManager::AddProximityAlert(
    /* [in] */ Double latitude,
    /* [in] */ Double longitude,
    /* [in] */ Float distance,
    /* [in] */ Int64 expiration,
    /* [in] */ IPendingIntent* intent)
{
//    if (FALSE) {
//        Log.d(TAG, "addProximityAlert: latitude = " + latitude +
//            ", longitude = " + longitude + ", radius = " + radius +
//            ", expiration = " + expiration +
//            ", intent = " + intent);
//    }
//    try {
    ECode ec = mService->AddProximityAlert(latitude, longitude, distance,
            expiration, intent);
    if (FAILED(ec)) {
//        Log.e(TAG, "addProximityAlert: RemoteException", ex);
    }
    return NOERROR;
//    } catch (RemoteException ex) {
//        Log.e(TAG, "addProximityAlert: RemoteException", ex);
//    }
}

ECode LocationManager::RemoveProximityAlert(
    /* [in] */ IPendingIntent* intent)
{
//    if (FALSE) {
//        Log.d(TAG, "removeProximityAlert: intent = " + intent);
//    }
//    try {
    ECode ec = mService->RemoveProximityAlert(intent);
    if (FAILED(ec)) {
//        Log.e(TAG, "removeProximityAlert: RemoteException", ex);
    }
    return NOERROR;
//    } catch (RemoteException ex) {
//        Log.e(TAG, "removeProximityAlert: RemoteException", ex);
//  }
}

ECode LocationManager::IsProviderEnabled(
    /* [in] */ const String& provider,
    /* [out] */ Boolean* isEnabled)
{
    assert(isEnabled != NULL);

    if (provider.IsNull()) {
//        throw new IllegalArgumentException("provider==null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
//    try {
    ECode ec = mService->IsProviderEnabled(provider, isEnabled);
    if (FAILED(ec)) {
        *isEnabled = FALSE;
//        Log.e(TAG, "isProviderEnabled: RemoteException", ex);
    }
    return NOERROR;
//    } catch (RemoteException ex) {
//        Log.e(TAG, "isProviderEnabled: RemoteException", ex);
//        return false;
//    }
}

ECode LocationManager::GetLastKnownLocation(
    /* [in] */ const String& provider,
    /* [out] */ ILocation** location)
{
    assert(location != NULL);

    if (provider.IsNull()) {
//        throw new IllegalArgumentException("provider==null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
//    try {
    ECode ec = mService->GetLastKnownLocation(provider, location);
    if (FAILED(ec)) {
        *location = NULL;
//        Log.e(TAG, "getLastKnowLocation: RemoteException", ex);
    }
    return NOERROR;
//    } catch (RemoteException ex) {
//        Log.e(TAG, "getLastKnowLocation: RemoteException", ex);
//        return null;
//    }
}

ECode LocationManager::AddTestProvider(
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
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode LocationManager::RemoveTestProvider(
    /* [in] */ const String& provider)
{
    // TODO: Add your code here
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode LocationManager::SetTestProviderLocation(
    /* [in] */ const String& provider,
    /* [in] */ ILocation* loc)
{
    // TODO: Add your code here
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode LocationManager::ClearTestProviderLocation(
    /* [in] */ const String& provider)
{
    // TODO: Add your code here
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode LocationManager::SetTestProviderEnabled(
    /* [in] */ const String& provider,
    /* [in] */ Boolean enabled)
{
    // TODO: Add your code here
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode LocationManager::ClearTestProviderEnabled(
    /* [in] */ const String& provider)
{
    // TODO: Add your code here
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode LocationManager::SetTestProviderStatus(
    /* [in] */ const String& provider,
    /* [in] */ Int32 status,
    /* [in] */ IBundle* extras,
    /* [in] */ Int64 updateTime)
{
    // TODO: Add your code here
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode LocationManager::ClearTestProviderStatus(
    /* [in] */ const String& provider)
{
    // TODO: Add your code here
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode LocationManager::AddGpsStatusListener(
    /* [in] */ ILocalGpsStatusListener* listener,
    /* [out] */ Boolean* result)
{
    assert(result != NULL);

    if (mGpsStatusListeners.Find(listener) != mGpsStatusListeners.End()) {
        // listener is already registered
        *result = TRUE;
        return NOERROR;
    }
//    try {
    AutoPtr<CGpsStatusListenerTransport> transport;
    CGpsStatusListenerTransport::NewByFriend((Handle32)this, listener,
            (CGpsStatusListenerTransport**)&transport);
    ECode ec = mService->AddGpsStatusListener((IGpsStatusListener*)transport.Get(), result);
    if (FAILED(ec)) {
//        Log.e(TAG, "RemoteException in registerGpsStatusListener: ", e);
        *result  = FALSE;
    }
    if (*result) {
        mGpsStatusListeners[listener] = transport;
    }
    return NOERROR;
//    } catch (RemoteException e) {
//        Log.e(TAG, "RemoteException in registerGpsStatusListener: ", e);
//        result = false;
//    }
}

ECode LocationManager::RemoveGpsStatusListener(
    /* [in] */ ILocalGpsStatusListener* listener)
{
//    try {
    AutoPtr<CGpsStatusListenerTransport> transport;
    HashMap<AutoPtr<ILocalGpsStatusListener>, AutoPtr<CGpsStatusListenerTransport> >::Iterator
            it = mGpsStatusListeners.Find(listener);
    if (it != mGpsStatusListeners.End()) {
        transport = it->mSecond;
        mGpsStatusListeners.Erase(it);
    }
    if (transport != NULL) {
        ECode ec = mService->RemoveGpsStatusListener((IGpsStatusListener*)transport.Get());
        if (FAILED(ec)) {
//            Log.e(TAG, "RemoteException in unregisterGpsStatusListener: ", e);
        }
    }
    return NOERROR;
//    } catch (RemoteException e) {
//        Log.e(TAG, "RemoteException in unregisterGpsStatusListener: ", e);
//    }
}

ECode LocationManager::AddNmeaListener(
    /* [in] */ ILocalGpsStatusNmeaListener* listener,
    /* [out] */ Boolean* result)
{
    assert(result != NULL);

    if (mNmeaListeners.Find(listener) != mNmeaListeners.End()) {
        // listener is already registered
        *result = TRUE;
        return NOERROR;
    }
//    try {
    AutoPtr<CGpsStatusListenerTransport> transport;
    CGpsStatusListenerTransport::NewByFriend((Handle32)this, listener,
            (CGpsStatusListenerTransport**)&transport);
    ECode ec = mService->AddGpsStatusListener((IGpsStatusListener*)transport.Get(), result);
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

ECode LocationManager::RemoveNmeaListener(
    /* [in] */ ILocalGpsStatusNmeaListener* listener)
{
//    try {
    AutoPtr<CGpsStatusListenerTransport> transport;
    HashMap<AutoPtr<ILocalGpsStatusNmeaListener>, AutoPtr<CGpsStatusListenerTransport> >::Iterator
            it = mNmeaListeners.Find(listener);
    if (it != mNmeaListeners.End()) {
        transport = it->mSecond;
        mNmeaListeners.Erase(it);
    }
    if (transport != NULL) {
        ECode ec = mService->RemoveGpsStatusListener((IGpsStatusListener*)transport.Get());
        if (FAILED(ec)) {
//            Log.e(TAG, "RemoteException in unregisterGpsStatusListener: ", e);
        }
    }
    return NOERROR;
//    } catch (RemoteException e) {
//        Log.e(TAG, "RemoteException in unregisterGpsStatusListener: ", e);
//    }
}

ECode LocationManager::GetGpsStatus(
    /* [in] */ IGpsStatus* inStatus,
    /* [out] */ IGpsStatus** outStatus)
{
    assert(outStatus != NULL);

    if (inStatus == NULL) {
        FAIL_RETURN(CGpsStatus::New(outStatus));
    }
    else {
        *outStatus = inStatus;
        (*outStatus)->AddRef();
    }
    (*outStatus)->SetStatusEx(mGpsStatus);
    return NOERROR;
}

ECode LocationManager::SendExtraCommand(
    /* [in] */ const String& provider,
    /* [in] */ const String& command,
    /* [in] */ IBundle* inExtras, //inout
    /* [out] */ IBundle** outExtras,
    /* [out] */ Boolean* result)
{
    assert(result != NULL);

//    try {
    return mService->SendExtraCommand(provider, command, inExtras, outExtras, result);
//    } catch (RemoteException e) {
//        Log.e(TAG, "RemoteException in sendExtraCommand: ", e);
//        return false;
//    }
}

ECode LocationManager::SendNiResponse(
    /* [in] */ Int32 notifId,
    /* [in] */ Int32 userResponse,
    /* [out] */ Boolean* result)
{
    assert(result != NULL);

//    try {
    return mService->SendNiResponse(notifId, userResponse, result);
//    } catch (RemoteException e) {
//        Log.e(TAG, "RemoteException in sendNiResponse: ", e);
//        return false;
//    }
}
