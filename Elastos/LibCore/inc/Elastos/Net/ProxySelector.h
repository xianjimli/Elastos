
#ifndef __PROXYSELECTOR_H__
#define __PROXYSELECTOR_H__

#include "Elastos.Net_server.h"
#include <elastos/AutoPtr.h>

/**
 * Selects an applicable proxy server when connecting to a resource specified by
 * a URL. Proxy selectors are concrete subclasses of {@code ProxySelector} and
 * can be set as default by calling the {@code setDefault()} method. If a
 * connection can't be established, the caller should notify the proxy selector
 * by invoking the {@code connectFailed()} method.
 */
class ProxySelector
{
public:
    ProxySelector();

    /**
     * Gets the default {@code ProxySelector} of the system.
     *
     * @return the currently set default {@code ProxySelector}.
     * @throws SecurityException
     *             if a security manager is installed but it doesn't have the
     *             NetPermission("getProxySelector").
     */
    static CARAPI GetDefault(
        /* [in] */ IProxySelector** defaultSelector);

    /**
     * Sets the default {@code ProxySelector} of the system. Removes the system
     * default {@code ProxySelector} if the parameter {@code selector} is set to
     * {@code null}.
     *
     * @param selector
     *            the {@code ProxySelector} instance to set as default or
     *            {@code null} to remove the current default {@code
     *            ProxySelector}.
     * @throws SecurityException
     *             if a security manager is installed but it doesn't have the
     *             NetPermission("setProxySelector").
     */
    static CARAPI SetDefault(
        /* [in] */ IProxySelector* selector);

    virtual CARAPI Select(
        /* [in] */ IURI* uri,
        /* [out] */ IObjectContainer** container) = 0;

    virtual CARAPI ConnectFailed(
        /* [in] */ IURI* uri,
        /* [in] */ ISocketAddress* sa
        /*[in] IOException ioe*/) = 0;

private:
    static AutoPtr<IProxySelector> mDefaultSelector;

    /*
     * "getProxySelector" permission. getDefault method requires this
     * permission.
     */
    static AutoPtr<INetPermission> mGetProxySelectorPermission;

    /*
     * "setProxySelector" permission. setDefault method requires this
     * permission.
     */
    static AutoPtr<INetPermission> mSetProxySelectorPermission;
};

#endif //__ProxySelector_H__
