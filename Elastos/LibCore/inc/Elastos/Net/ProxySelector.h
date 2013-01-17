
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

    /**
     * Gets all applicable proxies based on the accessing protocol of {@code
     * uri}. The format of URI is defined as below:
     * <p>
     * <li>http URI stands for http connection.</li>
     * <li>https URI stands for https connection.</li>
     * <li>ftp URI stands for ftp connection.</li>
     * <li>socket:://ip:port URI stands for tcp client sockets connection.</li>
     *
     * @param uri
     *            the target URI object.
     * @return a list containing all applicable proxies. If no proxy is
     *         available, the list contains only the {@code Proxy.NO_PROXY}
     *         element.
     * @throws IllegalArgumentException
     *             if {@code uri} is {@code null}.
     */
    virtual CARAPI Select(
        /* [in] */ IURI* uri,
        /* [out] */ IObjectContainer** container) = 0;

    /**
     * Notifies the {@code ProxySelector} that a connection to the proxy server
     * could not be established. A concrete implementation should upon this
     * notification maintain the list of available proxies, since an updated
     * version should be provided by {@code select()}.
     *
     * @param uri
     *            the URI to which the connection could not be established.
     * @param sa
     *            the address of the proxy.
     * @param ioe
     *            the exception which was thrown during connection
     *            establishment.
     * @throws IllegalArgumentException
     *             if any argument is {@code null}.
     * @see #select(URI)
     */
    virtual CARAPI ConnectFailed(
        /* [in] */ IURI* uri,
        /* [in] */ ISocketAddress* sa,
        /* [in] */ ECode ec) = 0;

private:
    static AutoPtr<IProxySelector> sDefaultSelector;

    /*
     * "getProxySelector" permission. getDefault method requires this
     * permission.
     */
    static AutoPtr<INetPermission> sGetProxySelectorPermission;

    /*
     * "setProxySelector" permission. setDefault method requires this
     * permission.
     */
    static AutoPtr<INetPermission> sSetProxySelectorPermission;
};

#endif //__ProxySelector_H__
