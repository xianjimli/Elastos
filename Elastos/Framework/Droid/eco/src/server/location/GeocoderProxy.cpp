
#include "server/location/GeocoderProxy.h"

PInterface GeocoderProxy::Connection::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }
    else if (riid == EIID_IServiceConnection) {
        return (IServiceConnection*)this;
    }

    return NULL;
}

UInt32 GeocoderProxy::Connection::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 GeocoderProxy::Connection::Release()
{
    return ElRefBase::Release();
}

ECode GeocoderProxy::Connection::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode GeocoderProxy::Connection::OnServiceConnected(
    /* [in] */ IComponentName* name,
    /* [in] */ IBinder* service)
{
    return E_NOT_IMPLEMENTED;
}

ECode GeocoderProxy::Connection::OnServiceDisconnected(
    /* [in] */ IComponentName* name)
{
    return E_NOT_IMPLEMENTED;
}

GeocoderProxy::GeocoderProxy(
    /* [in] */ IContext* context,
    /* [in] */ String serviceName)
    : mContext(context)
{
    mServiceConnection = new Connection();
    Boolean result;
    ASSERT_SUCCEEDED(mContext->BindService(mIntent, (IServiceConnection*)mServiceConnection.Get(), Context_BIND_AUTO_CREATE, &result));
}

String GeocoderProxy::GetFromLocation(
	/* [in] */ Double latitude,
	/* [in] */ Double longitude,
	/* [in] */ Int32 maxResults,
	/* [in] */ IGeocoderParams* params,
	/* [in] */ IObjectContainer** addrs)
{
	return String(NULL);
}

String GeocoderProxy::GetFromLocationName(
	/* [in] */ String locationName,
	/* [in] */ Double lowerLeftLatitude,
	/* [in] */ Double lowerLeftLongitude,
	/* [in] */ Double upperRightLatitude,
	/* [in] */ Double upperRightLongitude,
	/* [in] */ Int32 maxResults,
	/* [in] */ IGeocoderParams* params,
	/* [in] */ IObjectContainer** addrs)
{
	return String(NULL);
}

