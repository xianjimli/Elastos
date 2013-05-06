
#include "server/location/LocationProviderProxy.h"

const CString LocationProviderProxy::TAG = "LocationProviderProxy";

LocationProviderProxy::LocationProviderProxy(
    /* [in] */ IContext* context,
    /* [in] */ String name,
    /* [in] */ String serviceName,
    /* [in] */ IApartment* handler)
    : mContext(context),
      mName(name),
      mHandler(handler),
      mLocationTracking(FALSE),
      mEnabled(FALSE),
      mMinTime(-1)
{
    mServiceConnection = new Connection(this);
    ASSERT_SUCCEEDED(CIntent::New(serviceName, (IIntent**)&mIntent));
    ASSERT_SUCCEEDED(CWorkSource::New((IWorkSource**)&mMinTimeSource));

    Boolean result;
    ASSERT_SUCCEEDED(mContext->BindService(mIntent,
            (IServiceConnection*)mServiceConnection, Context_BIND_AUTO_CREATE, &result));
}

PInterface LocationProviderProxy::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(ILocationProviderInterface*)this;
    }
    else if (riid == EIID_ILocationProviderInterface) {
        return (ILocationProviderInterface*)this;
    }

    return NULL;
}

UInt32 LocationProviderProxy::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 LocationProviderProxy::Release()
{
    return ElRefBase::Release();
}

ECode LocationProviderProxy::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

/**
 * When unbundled NetworkLocationService package is updated, we
 * need to unbind from the old version and re-bind to the new one.
 */
void LocationProviderProxy::Reconnect()
{
    Mutex::Autolock lock(&mLock);
    mContext->UnbindService((IServiceConnection*)mServiceConnection);
    mServiceConnection = new Connection(this);
    Boolean result;
    mContext->BindService(mIntent, (IServiceConnection*)mServiceConnection,
            Context_BIND_AUTO_CREATE, &result);
}

LocationProviderProxy::Connection::Connection(
    /* [in] */ LocationProviderProxy* lpProxy)
    : mLPProxy(lpProxy)
{
}

PInterface LocationProviderProxy::Connection::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IServiceConnection*)this;
    }
    else if (riid == EIID_IServiceConnection) {
        return (IServiceConnection*)this;
    }
    else if (riid == EIID_IRunnable) {
        return (IRunnable*)this;
    }

    return NULL;
}

UInt32 LocationProviderProxy::Connection::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 LocationProviderProxy::Connection::Release()
{
    return ElRefBase::Release();
}

ECode LocationProviderProxy::Connection::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode LocationProviderProxy::Connection::OnServiceConnected(
    /* [in] */ IComponentName* name,
    /* [in] */ IBinder* service)
{
//   Log.d(TAG, "LocationProviderProxy.onServiceConnected " + className);
    {
        Mutex::Autolock lock(mSync);

        //todo service should be IInterface
        if (service && service->Probe(EIID_ILocationProvider)) {
            mProvider = (ILocationProvider*)service->Probe(EIID_ILocationProvider);
        }
        if (mProvider != NULL) {
            return Post((IRunnable*)this);
        }
    }

    return NOERROR;
}

ECode LocationProviderProxy::Connection::Post(
    /* [in] */ IRunnable* action)
{
    ECode (STDCALL IRunnable::*pHandlerFunc)();
    pHandlerFunc = &IRunnable::Run;

    return mLPProxy->mHandler->PostCppCallback(
        (Handle32)action, *(Handle32*)&pHandlerFunc, NULL, 0);
}

ECode LocationProviderProxy::Connection::OnServiceDisconnected(
    /* [in] */ IComponentName* name)
{
//    Log.d(TAG, "LocationProviderProxy.onServiceDisconnected " + className);
    {
        Mutex::Autolock lock(mSync);
        mProvider = NULL;
    }

    return NOERROR;
}

AutoPtr<ILocationProvider> LocationProviderProxy::Connection::GetProvider()
{
    return mProvider;
}

DummyLocationProvider* LocationProviderProxy::Connection::GetCachedAttributes()
{
    return mCachedAttributes;
}

ECode LocationProviderProxy::Connection::Run()
{
    Mutex::Autolock lock(&mLPProxy->mLock);

    if (mLPProxy->mServiceConnection.Get() != this) {
        // This ServiceConnection no longer the one we want to bind to.
        return NOERROR;
    }
    AutoPtr<ILocationProvider> provider = GetProvider();
    if (provider == NULL) {
        return NOERROR;
    }

    // resend previous values from the location manager if the service has restarted
//    try {
    if (mLPProxy->mEnabled) {
        provider->Enable();
    }
    if (mLPProxy->mLocationTracking) {
        provider->EnableLocationTracking(TRUE);
    }
//    if (mLPProxy->mMinTime >= 0) {
//        provider->SetMinTime(mLPProxy->mMinTime, mLPProxy->mMinTimeSource);
//    }
//    if (mLPProxy->mNetworkInfo != NULL) {
//        provider->UpdateNetworkState(mLPProxy->mNetworkState, mLPProxy->mNetworkInfo);
//    }
//    } catch (RemoteException e) {
//    }

    // init cache of parameters
    if (mCachedAttributes == NULL) {
//        try {
        mCachedAttributes = new DummyLocationProvider(mLPProxy->mName, NULL);

        Boolean val;
        provider->RequiresNetwork(&val);
        mCachedAttributes->SetRequiresNetwork(val);

        provider->RequiresSatellite(&val);
        mCachedAttributes->SetRequiresSatellite(val);

        provider->RequiresCell(&val);
        mCachedAttributes->SetRequiresCell(val);

        provider->HasMonetaryCost(&val);
        mCachedAttributes->SetHasMonetaryCost(val);

        provider->SupportsAltitude(&val);
        mCachedAttributes->SetSupportsAltitude(val);

        provider->SupportsSpeed(&val);
        mCachedAttributes->SetSupportsSpeed(val);

        provider->SupportsBearing(&val);
        mCachedAttributes->SetSupportsBearing(val);

        Int32 intVal;
        provider->GetPowerRequirement(&intVal);
        mCachedAttributes->SetPowerRequirement(intVal);

        provider->GetAccuracy(&intVal);
        mCachedAttributes->SetAccuracy(intVal);
//        } catch (RemoteException e) {
//            mCachedAttributes = null;
//        }
    }

    return NOERROR;
}

ECode LocationProviderProxy::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);

    *name = mName;
    return NOERROR;
}

DummyLocationProvider* LocationProviderProxy::GetCachedAttributes()
{
    Mutex::Autolock lock(&mLock);

    return mServiceConnection->GetCachedAttributes();
}

ECode LocationProviderProxy::RequiresNetwork(
    /* [out] */ Boolean* required)
{
    VALIDATE_NOT_NULL(required);

    DummyLocationProvider* cachedAttributes = GetCachedAttributes();
    if (cachedAttributes != NULL) {
        return cachedAttributes->RequiresNetwork(required);
    }
    else {
        *required = FALSE;
        return NOERROR;
    }
}

ECode LocationProviderProxy::RequiresSatellite(
    /* [out] */ Boolean* required)
{
    VALIDATE_NOT_NULL(required);

    DummyLocationProvider* cachedAttributes = GetCachedAttributes();
    if (cachedAttributes != NULL) {
        return cachedAttributes->RequiresSatellite(required);
    }
    else {
        *required = FALSE;
        return NOERROR;
    }
}

ECode LocationProviderProxy::RequiresCell(
    /* [out] */ Boolean* required)
{
    VALIDATE_NOT_NULL(required);

    DummyLocationProvider* cachedAttributes = GetCachedAttributes();
    if (cachedAttributes != NULL) {
        return cachedAttributes->RequiresCell(required);
    }
    else {
        *required = FALSE;
        return NOERROR;
    }
}

ECode LocationProviderProxy::HasMonetaryCost(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    DummyLocationProvider* cachedAttributes = GetCachedAttributes();
    if (cachedAttributes != NULL) {
        return cachedAttributes->HasMonetaryCost(result);
    }
    else {
        *result = FALSE;
        return NOERROR;
    }
}

ECode LocationProviderProxy::SupportsAltitude(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);

    DummyLocationProvider* cachedAttributes = GetCachedAttributes();
    if (cachedAttributes != NULL) {
        return cachedAttributes->SupportsAltitude(supported);
    }
    else {
        *supported = FALSE;
        return NOERROR;
    }
}

ECode LocationProviderProxy::SupportsSpeed(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);

    DummyLocationProvider* cachedAttributes = GetCachedAttributes();
    if (cachedAttributes != NULL) {
        return cachedAttributes->SupportsSpeed(supported);
    }
    else {
        *supported = FALSE;
        return NOERROR;
    }
}

ECode LocationProviderProxy::SupportsBearing(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);

    DummyLocationProvider* cachedAttributes = GetCachedAttributes();
    if (cachedAttributes != NULL) {
        return cachedAttributes->SupportsBearing(supported);
    }
    else {
        *supported = FALSE;
        return NOERROR;
    }
}

ECode LocationProviderProxy::GetPowerRequirement(
    /* [out] */ Int32* requirement)
{
    VALIDATE_NOT_NULL(requirement);

    DummyLocationProvider* cachedAttributes = GetCachedAttributes();
    if (cachedAttributes != NULL) {
        return cachedAttributes->GetPowerRequirement(requirement);
    }
    else {
        *requirement = -1;
        return NOERROR;
    }
}

ECode LocationProviderProxy::GetAccuracy(
    /* [out] */ Int32* accuracy)
{
    VALIDATE_NOT_NULL(accuracy);

    DummyLocationProvider* cachedAttributes = GetCachedAttributes();
    if (cachedAttributes != NULL) {
        return cachedAttributes->GetAccuracy(accuracy);
    }
    else {
        *accuracy = -1;
        return NOERROR;
    }
}

ECode LocationProviderProxy::MeetsCriteria(
    /* [in] */ ICriteria* criteria,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    {
        Mutex::Autolock lock(&mLock);

        AutoPtr<ILocationProvider> provider = mServiceConnection->GetProvider();
        if (provider != NULL) {
//            try {
            return provider->MeetsCriteria(criteria, result);
//            } catch (RemoteException e) {
//            }
        }
    }

    // default implementation if we lost connection to the provider
    Int32 accuracy, thisAcc;
    criteria->GetAccuracy(&accuracy);
    GetAccuracy(&thisAcc);
    if ((accuracy != Criteria_NO_REQUIREMENT) &&
        (accuracy < thisAcc)) {
        *result = FALSE;
        return NOERROR;
    }

    Int32 criteriaPower, thisPow;
    criteria->GetPowerRequirement(&criteriaPower);
    GetPowerRequirement(&thisPow);
    if ((criteriaPower != Criteria_NO_REQUIREMENT) &&
        (criteriaPower < thisPow)) {
        *result = FALSE;
        return NOERROR;
    }

    Boolean _result;
    criteria->IsAltitudeRequired(&_result);
    if (_result && (SupportsAltitude(&_result), !_result)) {
        *result = FALSE;
        return NOERROR;
    }

    criteria->IsSpeedRequired(&_result);
    if (_result && (SupportsSpeed(&_result), !_result)) {
        *result = FALSE;
        return NOERROR;
    }

    criteria->IsBearingRequired(&_result);
    if (_result && (SupportsBearing(&_result), !_result)) {
        *result = FALSE;
        return NOERROR;
    }

    *result = TRUE;
    return NOERROR;
}

ECode LocationProviderProxy::Enable()
{
    Mutex::Autolock lock(&mLock);

    mEnabled = TRUE;
    AutoPtr<ILocationProvider> provider = mServiceConnection->GetProvider();
    if (provider != NULL) {
//        try {
        provider->Enable();
//        } catch (RemoteException e) {
//        }
    }
    return NOERROR;
}

ECode LocationProviderProxy::Disable()
{
    Mutex::Autolock lock(&mLock);

    mEnabled = FALSE;
    AutoPtr<ILocationProvider> provider = mServiceConnection->GetProvider();
    if (provider != NULL) {
//        try {
        provider->Disable();
//        } catch (RemoteException e) {
//        }
    }
    return NOERROR;
}

ECode LocationProviderProxy::IsEnabled(
    /* [out] */ Boolean* isEnabled)
{
    VALIDATE_NOT_NULL(isEnabled);

    Mutex::Autolock lock(&mLock);

    *isEnabled = mEnabled;
    return NOERROR;
}

ECode LocationProviderProxy::GetStatus(
    /* [in] */ IBundle* extras,
    /* [out] */ Int32* status)
{
    VALIDATE_NOT_NULL(status);

    AutoPtr<ILocationProvider> provider;
    {
        Mutex::Autolock lock(&mLock);
        provider = mServiceConnection->GetProvider();
    }

    if (provider != NULL) {
//        try {
        // return provider->GetStatus(extras, status);
//        } catch (RemoteException e) {
//        }
    }
    *status = 0;
    return NOERROR;
}

ECode LocationProviderProxy::GetStatusUpdateTime(
    /* [out] */ Int64* time)
{
    VALIDATE_NOT_NULL(time);

    AutoPtr<ILocationProvider> provider;
    {
        Mutex::Autolock lock(&mLock);
        provider = mServiceConnection->GetProvider();
    }

    if (provider != NULL) {
//        try {
        return provider->GetStatusUpdateTime(time);
//        } catch (RemoteException e) {
//        }
    }
    *time = 0;
    return NOERROR;
}

ECode LocationProviderProxy::GetInternalState(
    /* [out] */ String* state)
{
    VALIDATE_NOT_NULL(state);

    AutoPtr<ILocationProvider> provider;
    {
        Mutex::Autolock lock(&mLock);
        provider = mServiceConnection->GetProvider();
    }

    if (provider != NULL) {
//        try {
        return provider->GetInternalState(state);
//        } catch (RemoteException e) {
//        }
    }

    *state = NULL;
    return NOERROR;
}

Boolean LocationProviderProxy::IsLocationTracking()
{
    Mutex::Autolock lock(&mLock);
    return mLocationTracking;
}

ECode LocationProviderProxy::EnableLocationTracking(
    /* [in] */ Boolean enable)
{
    Mutex::Autolock lock(&mLock);

    mLocationTracking = enable;
    if (!enable) {
        mMinTime = -1;
//        mMinTimeSource.clear();
    }
    AutoPtr<ILocationProvider> provider = mServiceConnection->GetProvider();
    if (provider != NULL) {
//        try {
        provider->EnableLocationTracking(enable);
//        } catch (RemoteException e) {
//        }
    }
    return NOERROR;
}

ECode LocationProviderProxy::RequestSingleShotFix(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = FALSE;
    return NOERROR;
}

ECode LocationProviderProxy::SetMinTime(
    /* [in] */ Int64 minTime,
    /* [in] */ IWorkSource* ws)
{
    Mutex::Autolock lock(&mLock);
    mMinTime = minTime;
    mMinTimeSource->Set(ws);
    AutoPtr<ILocationProvider> provider= mServiceConnection->GetProvider();
    if (provider != NULL) {
//        try {
        provider->SetMinTime(minTime, ws);
//        } catch (RemoteException e) {
//        }
    }
    return NOERROR;
}

ECode LocationProviderProxy::UpdateNetworkState(
    /* [in] */ Int32 state,
    /* [in] */ INetworkInfo* info)
{
    Mutex::Autolock lock(&mLock);
    mNetworkState = state;
    mNetworkInfo = info;
    AutoPtr<ILocationProvider> provider = mServiceConnection->GetProvider();
    if (provider != NULL) {
//        try {
        provider->UpdateNetworkState(state, info);
//        } catch (RemoteException e) {
//        }
    }
    return NOERROR;
}

ECode LocationProviderProxy::UpdateLocation(
    /* [in] */ ILocation* location)
{
    Mutex::Autolock lock(&mLock);

    AutoPtr<ILocationProvider> provider = mServiceConnection->GetProvider();
    if (provider != NULL) {
//        try {
        provider->UpdateLocation(location);
//        } catch (RemoteException e) {
//        }
    }
    return NOERROR;
}

ECode LocationProviderProxy::SendExtraCommand(
    /* [in] */ const String& command,
    /* [in] */ IBundle* extras,
    /* [out] */ IBundle** outExtras,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    {
        Mutex::Autolock lock(&mLock);

        AutoPtr<ILocationProvider> provider = mServiceConnection->GetProvider();
        if (provider != NULL) {
//            try {
            return provider->SendExtraCommand(command, extras, outExtras, result);
//            } catch (RemoteException e) {
//            }
        }
    }
    *result = FALSE;
    return NOERROR;
}

ECode LocationProviderProxy::AddListener(
    /* [in] */ Int32 uid)
{
    Mutex::Autolock lock(&mLock);

    AutoPtr<ILocationProvider> provider = mServiceConnection->GetProvider();
    if (provider != NULL) {
//        try {
        provider->AddListener(uid);
//        } catch (RemoteException e) {
//        }
    }
    return NOERROR;
}

ECode LocationProviderProxy::RemoveListener(
    /* [in] */ Int32 uid)
{
    Mutex::Autolock lock(&mLock);

    AutoPtr<ILocationProvider> provider = mServiceConnection->GetProvider();
    if (provider != NULL) {
//        try {
        provider->RemoveListener(uid);
//        } catch (RemoteException e) {
//        }
    }
    return NOERROR;
}
