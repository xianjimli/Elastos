
#ifndef __CPROXY_H__
#define __CPROXY_H__

#include "_CProxy.h"
#include <elastos/AutoPtr.h>

CarClass(CProxy)
{
public:
    CARAPI GetType(
        /* [out] */ ProxyType* type);

    CARAPI GetAddress(
        /* [out] */ ISocketAddress** address);

    CARAPI constructor(
        /* [in] */ ProxyType type,
        /* [in] */ ISocketAddress* sa);

private:
    CARAPI_(void) Init();

    static CARAPI_(AutoPtr<IProxy>) InitProxy();

public:
    /**
     * Represents the proxy type setting {@code Proxy.Type.DIRECT}. It tells
     * protocol handlers that there is no proxy to be used. The address is set
     * to {@code null}.
     */
    static AutoPtr<IProxy> NO_PROXY;// = new Proxy();

private:
    ProxyType mType;

    AutoPtr<ISocketAddress> mAddress;
};

#endif //__CPROXY_H__
