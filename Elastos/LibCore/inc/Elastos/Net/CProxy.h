
#ifndef __CPROXY_H__
#define __CPROXY_H__

#include "_CProxy.h"
#include <elastos/AutoPtr.h>

/**
 * This class represents proxy server settings. A created instance of {@code
 * Proxy} stores a type and an address and is immutable. There are three types
 * of proxies:
 * <ul>
 * <li>DIRECT</li>
 * <li>HTTP</li>
 * <li>SOCKS</li></ul
 */
CarClass(CProxy)
{
public:
    /*
     * Constructs a Proxy instance, which is Proxy.DIRECT type with null
     * SocketAddress. This constructor is used for NO_PROXY.
     */
    CARAPI constructor();

    /**
     * Creates a new {@code Proxy} instance. {@code SocketAddress} must NOT be
     * {@code null} when {@code type} is either {@code Proxy.Type.HTTP} or
     * {@code Proxy.Type.SOCKS}. To create a {@code Proxy} instance representing
     * the proxy type {@code Proxy.Type.DIRECT}, use {@code Proxy.NO_PROXY}
     * instead of this constructor.
     *
     * @param type
     *            the proxy type of this instance.
     * @param sa
     *            the proxy address of this instance.
     * @throws IllegalArgumentException
     *             if the parameter {@code type} is set to {@code
     *             Proxy.Type.DIRECT} or the value for {@code SocketAddress} is
     *             {@code null}.
     */
    CARAPI constructor(
        /* [in] */ ProxyType type,
        /* [in] */ ISocketAddress* sa);

    CARAPI GetType(
        /* [out] */ ProxyType* type);

    CARAPI GetAddress(
        /* [out] */ ISocketAddress** address);

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
