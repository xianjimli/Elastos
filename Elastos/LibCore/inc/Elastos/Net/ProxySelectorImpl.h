
#ifndef __PROXYSELECTORIMPL_H__
#define __PROXYSELECTORIMPL_H__

#include "ProxySelector.h"
#include <elastos/ElRefBase.h>

//extern "C" const InterfaceID EIID_ProxySelectorImpl;

/**
 * Default implementation for {@code ProxySelector}.
 */
class ProxySelectorImpl : public ElRefBase, IProxySelector, ProxySelector
{
public:
    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI ConnectFailed(
        /* [in] */ IURI* uri,
        /* [in] */ ISocketAddress* sa
        /*[in] IOException ioe*/);

    CARAPI Select(
        /* [in] */ IURI* uri,
        /* [out] */ IObjectContainer** container);

    // read net.properties file
    // static {
    //     AccessController.doPrivileged(new java.security.PrivilegedAction() {
    //         public Object run() {
    //             File f = new File(System.getProperty("java.home")
    //                     + File.separator + "lib" + File.separator
    //                     + "net.properties");

    //             if (f.exists()) {
    //                 try {
    //                     FileInputStream fis = new FileInputStream(f);
    //                     InputStream is = new BufferedInputStream(fis);
    //                     netProps = new Properties();
    //                     netProps.load(is);
    //                     is.close();
    //                 } catch (IOException e) {
    //                 }
    //             }
    //             return null;
    //         }
    //     });
    // }

private:
    /*
     * Gets proxy for http request. 1. gets from "http.proxyHost", then gets
     * port from "http.proxyPort", or from "proxyPort" if "http.proxyPort" is
     * unavailable. 2. gets from "proxyHost" if 1 is unavailable,then get port
     * from "proxyPort", or from "http.proxyPort" if "proxyPort" is unavailable.
     * 3. gets from "socksProxyHost" if 2 is unavailable.
     */
    CARAPI_(AutoPtr<IProxy>) SelectHttpProxy(
        /* [in] */ const String& uriHost);

    /*
     * Gets proxy for https request.
     */
    CARAPI_(AutoPtr<IProxy>) SelectHttpsProxy();

    /*
     * Gets proxy for ftp request.
     */
    CARAPI_(AutoPtr<IProxy>) SelectFtpProxy(
        /* [in] */ const String& uriHost);

    /*
     * Gets proxy for socks request.
     */
    CARAPI_(AutoPtr<IProxy>) SelectSocksProxy();

    /*
     * checks whether the host needs proxy. return true if it doesn't need a
     * proxy.
     */
    CARAPI_(Boolean) IsNonProxyHost(
        /* [in] */ const String& host,
        /* [in] */ const String& nonProxyHosts);

    /*
     * Create Proxy by "type","host" and "port".
     */
    CARAPI_(AutoPtr<IProxy>) CreateProxy(
        /* [in] */ ProxyType type,
        /* [in] */ const String& host,
        /* [in] */ const String& port,
        /* [in] */ Int32 defaultPort);

    /*
     * gets system property, privileged operation. If the value of the property
     * is null or empty String, it returns defaultValue.
     */
    CARAPI_(String) GetSystemProperty(
        /* [in] */ const String& property);

    /*
     * gets system property, privileged operation. If the value of the property
     * is null or empty String, it returns defaultValue.
     */
    CARAPI_(String) GetSystemPropertyEx(
        /* [in] */ const String& property,
        /* [in] */ const String& defaultValue);

    /*
     * gets system property, privileged operation. If the value of "key"
     * property is null, then retrieve value from "alternative" property.
     * Finally, if the value is null or empty String, it returns defaultValue.
     */
    CARAPI_(String) GetSystemPropertyOrAlternative(
        /* [in] */ const String& key,
        /* [in] */ const String& alternativeKey,
        /* [in] */ const String& defaultValue);

private:
    static const Int32 HTTP_PROXY_PORT = 80;

    static const Int32 HTTPS_PROXY_PORT = 443;

    static const Int32 FTP_PROXY_PORT = 80;

    static const Int32 SOCKS_PROXY_PORT = 1080;

    // Net properties read from net.properties file.
//    static AutoPtr<IProperties> mNetProps;// = null;
};

#endif //__PROXYSELECTORIMPL_H__
