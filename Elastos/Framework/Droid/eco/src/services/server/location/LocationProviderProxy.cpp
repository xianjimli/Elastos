
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

/**
 * When unbundled NetworkLocationService package is updated, we
 * need to unbind from the old version and re-bind to the new one.
 */
void LocationProviderProxy::Reconnect()
{
    Mutex::Autolock lock(mMutex);
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
    Mutex::Autolock lock(mLPProxy->mMutex);

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


String LocationProviderProxy::GetName()
{
    return mName;
}

DummyLocationProvider* LocationProviderProxy::GetCachedAttributes()
{
    Mutex::Autolock lock(mMutex);

    return mServiceConnection->GetCachedAttributes();
}

Boolean LocationProviderProxy::RequiresNetwork()
{
    DummyLocationProvider* cachedAttributes = GetCachedAttributes();
    if (cachedAttributes != NULL) {
        Boolean result;
        cachedAttributes->RequiresNetwork(&result);
        return result;
    }
    else {
        return FALSE;
    }
}

Boolean LocationProviderProxy::RequiresSatellite()
{
    DummyLocationProvider* cachedAttributes = GetCachedAttributes();
    if (cachedAttributes != NULL) {
        Boolean result;
        cachedAttributes->RequiresSatellite(&result);
        return result;
    }
    else {
        return FALSE;
    }
}

Boolean LocationProviderProxy::RequiresCell()
{
    DummyLocationProvider* cachedAttributes = GetCachedAttributes();
    if (cachedAttributes != NULL) {
        Boolean result;
        cachedAttributes->RequiresCell(&result);
        return result;
    }
    else {
        return FALSE;
    }
}

Boolean LocationProviderProxy::HasMonetaryCost()
{
    DummyLocationProvider* cachedAttributes = GetCachedAttributes();
    if (cachedAttributes != NULL) {
        Boolean result;
        cachedAttributes->HasMonetaryCost(&result);
        return result;
    }
    else {
        return FALSE;
    }
}

Boolean LocationProviderProxy::SupportsAltitude()
{
    DummyLocationProvider* cachedAttributes = GetCachedAttributes();
    if (cachedAttributes != NULL) {
        Boolean result;
        cachedAttributes->SupportsAltitude(&result);
        return result;
    }
    else {
        return FALSE;
    }
}

Boolean LocationProviderProxy::SupportsSpeed()
{
    DummyLocationProvider* cachedAttributes = GetCachedAttributes();
    if (cachedAttributes != NULL) {
        Boolean result;
        cachedAttributes->SupportsSpeed(&result);
        return result;
    }
    else {
        return FALSE;
    }
}

Boolean LocationProviderProxy::SupportsBearing()
{
    DummyLocationProvider* cachedAttributes = GetCachedAttributes();
    if (cachedAttributes != NULL) {
        Boolean result;
        cachedAttributes->SupportsBearing(&result);
        return result;
    }
    else {
        return FALSE;
    }
}

Int32 LocationProviderProxy::GetPowerRequirement()
{
    DummyLocationProvider* cachedAttributes = GetCachedAttributes();
    if (cachedAttributes != NULL) {
        Int32 requirement;
        cachedAttributes->GetPowerRequirement(&requirement);
        return requirement;
    }
    else {
        return -1;
    }
}

Int32 LocationProviderProxy::GetAccuracy()
{
    DummyLocationProvider* cachedAttributes = GetCachedAttributes();
    if (cachedAttributes != NULL) {
        Int32 accuracy;
        cachedAttributes->GetAccuracy(&accuracy);
        return accuracy;
    }
    else {
        return -1;
    }
}

Boolean LocationProviderProxy::MeetsCriteria(
    /* [in] */ ICriteria* criteria)
{
    {
        Mutex::Autolock lock(mMutex);

        AutoPtr<ILocationProvider> provider = mServiceConnection->GetProvider();
        if (provider != NULL) {
//            try {
            Boolean result;
            provider->MeetsCriteria(criteria, &result);
            return result;
//            } catch (RemoteException e) {
//            }
        }
    }

    // default implementation if we lost connection to the provider
    Int32 accuracy;
    criteria->GetAccuracy(&accuracy);
    if ((accuracy != Criteria_NO_REQUIREMENT) &&
        (accuracy < GetAccuracy())) {
        return FALSE;
    }

    Int32 criteriaPower;
    criteria->GetPowerRequirement(&criteriaPower);
    if ((criteriaPower != Criteria_NO_REQUIREMENT) &&
        (criteriaPower < GetPowerRequirement())) {
        return FALSE;
    }

    Boolean result;
    criteria->IsAltitudeRequired(&result);
    if (result && !SupportsAltitude()) {
        return FALSE;
    }

    criteria->IsSpeedRequired(&result);
    if (result && !SupportsSpeed()) {
        return FALSE;
    }

    criteria->IsBearingRequired(&result);
    if (result && !SupportsBearing()) {
        return FALSE;
    }

    return TRUE;
}

void LocationProviderProxy::Enable()
{
    Mutex::Autolock lock(mMutex);

    mEnabled = TRUE;
    AutoPtr<ILocationProvider> provider = mServiceConnection->GetProvider();
    if (provider != NULL) {
//        try {
        provider->Enable();
//        } catch (RemoteException e) {
//        }
    }
}

void LocationProviderProxy::Disable()
{
    Mutex::Autolock lock(mMutex);

    mEnabled = FALSE;
    AutoPtr<ILocationProvider> provider = mServiceConnection->GetProvider();
    if (provider != NULL) {
//        try {
        provider->Disable();
//        } catch (RemoteException e) {
//        }
    }
}

Boolean LocationProviderProxy::IsEnabled()
{
    Mutex::Autolock lock(mMutex);
    return mEnabled;
}

Int32 LocationProviderProxy::GetStatus(
    /* [in] */ IBundle* extras)
{
    AutoPtr<ILocationProvider> provider;
    {
        Mutex::Autolock lock(mMutex);
        provider = mServiceConnection->GetProvider();
    }

    if (provider != NULL) {
//        try {
        Int32 status;
        AutoPtr<IBundle> temp;
        provider->GetStatus((IBundle**)&temp, &status);
        extras->PutAll(temp);
        return status;
//        } catch (RemoteException e) {
//        }
    }
    return 0;
}

Int64 LocationProviderProxy::GetStatusUpdateTime()
{
    AutoPtr<ILocationProvider> provider;
    {
        Mutex::Autolock lock(mMutex);
        provider = mServiceConnection->GetProvider();
    }

    if (provider != NULL) {
//        try {
        Int64 updateTime;
        provider->GetStatusUpdateTime(&updateTime);
        return updateTime;
//        } catch (RemoteException e) {
//        }
    }
    return 0;
}

String LocationProviderProxy::GetInternalState()
{
    AutoPtr<ILocationProvider> provider;
    {
        Mutex::Autolock lock(mMutex);
        provider = mServiceConnection->GetProvider();
    }

    if (provider != NULL) {
//        try {
        String internalState;
        provider->GetInternalState(&internalState);
        return internalState;
//        } catch (RemoteException e) {
//        }
    }

    return String(NULL);
}

Boolean LocationProviderProxy::IsLocationTracking()
{
    Mutex::Autolock lock(mMutex);
    return mLocationTracking;
}

void LocationProviderProxy::EnableLocationTracking(
    /* [in] */ Boolean enable)
{
    Mutex::Autolock lock(mMutex);

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
}

Boolean LocationProviderProxy::RequestSingleShotFix()
{
    return FALSE;
}

void LocationProviderProxy::SetMinTime(
    /* [in] */ Int64 minTime,
    /* [in] */ IWorkSource* ws)
{
    Mutex::Autolock lock(mMutex);
    mMinTime = minTime;
    mMinTimeSource->Set(ws);
    AutoPtr<ILocationProvider> provider= mServiceConnection->GetProvider();
    if (provider != NULL) {
//        try {
        provider->SetMinTime(minTime, ws);
//        } catch (RemoteException e) {
//        }
    }
}

void LocationProviderProxy::UpdateNetworkState(
    /* [in] */ Int32 state,
    /* [in] */ INetworkInfo* info)
{
    Mutex::Autolock lock(mMutex);
    mNetworkState = state;
    mNetworkInfo = info;
    AutoPtr<ILocationProvider> provider = mServiceConnection->GetProvider();
    if (provider != NULL) {
//        try {
        provider->UpdateNetworkState(state, info);
//        } catch (RemoteException e) {
//        }
    }
}

void LocationProviderProxy::UpdateLocation(
    /* [in] */ ILocation* location)
{
    Mutex::Autolock lock(mMutex);

    AutoPtr<ILocationProvider> provider = mServiceConnection->GetProvider();
    if (provider != NULL) {
//        try {
        provider->UpdateLocation(location);
//        } catch (RemoteException e) {
//        }
    }
}

Boolean LocationProviderProxy::SendExtraCommand(
    /* [in] */ String command,
    /* [in] */ IBundle* extras,
    /* [out] */ IBundle** outExtras)
{
    {
        Mutex::Autolock lock(mMutex);

        AutoPtr<ILocationProvider> provider = mServiceConnection->GetProvider();
        if (provider != NULL) {
//            try {
            Boolean result;
            provider->SendExtraCommand(command, extras, outExtras, &result);
            return result;
//            } catch (RemoteException e) {
//            }
        }
    }

    return FALSE;
}

void LocationProviderProxy::AddListener(
    /* [in] */ Int32 uid)
{
    Mutex::Autolock lock(mMutex);

    AutoPtr<ILocationProvider> provider = mServiceConnection->GetProvider();
    if (provider != NULL) {
//        try {
        provider->AddListener(uid);
//        } catch (RemoteException e) {
//        }
    }
}

void LocationProviderProxy::RemoveListener(
    /* [in] */ Int32 uid)
{
    Mutex::Autolock lock(mMutex);

    AutoPtr<ILocationProvider> provider = mServiceConnection->GetProvider();
    if (provider != NULL) {
//        try {
        provider->RemoveListener(uid);
//        } catch (RemoteException e) {
//        }
    }
}
