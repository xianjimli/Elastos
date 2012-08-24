
#include "server/location/LocationProviderProxy.h"
#include "content/CIntent.h"

PInterface LocationProviderProxy::Connection::Probe(
    /* [in]  */ REIID riid)
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
    return E_NOT_IMPLEMENTED;
}

ECode LocationProviderProxy::Connection::OnServiceDisconnected(
    /* [in] */ IComponentName* name)
{
    return E_NOT_IMPLEMENTED;
}

ECode LocationProviderProxy::Connection::Run()
{
    return E_NOT_IMPLEMENTED;
}

LocationProviderProxy::LocationProviderProxy(
    /* [in] */ IContext* context,
    /* [in] */ String name,
    /* [in] */ String serviceName,
    /* [in] */ IApartment* handler)
    : mContext(context),
      mName(name),
      mHandler(handler)
{
    mServiceConnection = new Connection();
    ASSERT_SUCCEEDED(CIntent::New(serviceName, (IIntent**)&mIntent));
    Boolean result;
    ASSERT_SUCCEEDED(mContext->BindService(mIntent,
            (IServiceConnection*)mServiceConnection, Context_BIND_AUTO_CREATE, &result));
}

String LocationProviderProxy::GetName()
{
    return String(NULL);
}

Boolean LocationProviderProxy::RequiresNetwork()
{
    return FALSE;
}

Boolean LocationProviderProxy::RequiresSatellite()
{
    return FALSE;
}

Boolean LocationProviderProxy::RequiresCell()
{
    return FALSE;
}

Boolean LocationProviderProxy::HasMonetaryCost()
{
    return FALSE;
}

Boolean LocationProviderProxy::SupportsAltitude()
{
    return FALSE;
}

Boolean LocationProviderProxy::SupportsSpeed()
{
    return FALSE;
}

Boolean LocationProviderProxy::SupportsBearing()
{
    return FALSE;
}

Int32 LocationProviderProxy::GetPowerRequirement()
{
    return 0;
}

Boolean LocationProviderProxy::MeetsCriteria(
    /* [in] */ ICriteria* criteria)
{
    return FALSE;
}

Int32 LocationProviderProxy::GetAccuracy()
{
    return 0;
}

Boolean LocationProviderProxy::IsEnabled()
{
    return FALSE;
}

void LocationProviderProxy::Enable()
{
}

void LocationProviderProxy::Disable()
{
}

Int32 LocationProviderProxy::GetStatus(
    /* [in] */ IBundle* extras)
{
    return 0;
}

Int64 LocationProviderProxy::GetStatusUpdateTime()
{
    return 0;
}

void LocationProviderProxy::EnableLocationTracking(
    /* [in] */ Boolean enable)
{
}

Boolean LocationProviderProxy::RequestSingleShotFix()
{
    return FALSE;
}

String LocationProviderProxy::GetInternalState()
{
    return String(NULL);
}

//    void LocationProviderProxy::SetMinTime(
//        /* [in] */ Int64 minTime,
//        /* [in] */ IWorkSource* ws)

//    void LocationProviderProxy::UpdateNetworkState(
//        /* [in] */ Int32 state,
//        /* [in] */ INetworkInfo* info)

void LocationProviderProxy::UpdateLocation(
    /* [in] */ ILocation* location)
{
}

Boolean LocationProviderProxy::SendExtraCommand(
    /* [in] */ String command,
    /* [in] */ IBundle* extras,
    /* [out] */ IBundle** outExtras)
{
    return FALSE;
}

void LocationProviderProxy::AddListener(
    /* [in] */ Int32 uid)
{
}

void LocationProviderProxy::RemoveListener(
    /* [in] */ Int32 uid)
{
}
