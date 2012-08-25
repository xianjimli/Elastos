
#ifndef __GEOCODERPROXY_H__
#define __GEOCODERPROXY_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>

class GeocoderProxy
{
private:
    class Connection : public ElRefBase, IServiceConnection
    {
    public:
        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI OnServiceConnected(
            /* [in] */ IComponentName* name,
            /* [in] */ IBinder* service);

        CARAPI OnServiceDisconnected(
            /* [in] */ IComponentName* name);
    };

public:
    GeocoderProxy(
        /* [in] */ IContext* context,
        /* [in] */ String serviceName);

private:
    AutoPtr<IContext> mContext;
    AutoPtr<IIntent> mIntent;
    AutoPtr<Connection> mServiceConnection;  // never null
};

#endif //__GEOCODERPROXY_H__
