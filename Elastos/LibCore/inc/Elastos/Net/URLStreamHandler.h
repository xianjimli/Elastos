
#ifndef __URLSTREAMHANDLER_H__
#define __URLSTREAMHANDLER_H__

#include "Elastos.Net_server.h"

extern "C" const InterfaceID EIID_URLStreamHandler;

//using namespace Elastos;

/**
 * The abstract class {@code URLStreamHandler} is the base for all classes which
 * can handle the communication with a URL object over a particular protocol
 * type.
 */
class URLStreamHandler
{
public:
    virtual CARAPI OpenConnection(
        /* [in] */ IURL* u,
        /* [out] */ IURLConnection** urlConnection) = 0;

    virtual CARAPI OpenConnectionEx(
        /* [in] */ IURL* u,
        /* [in] */ IProxy* proxy,
        /* [out] */ IURLConnection** urlConnection);

    virtual CARAPI ParseURL(
        /* [in] */ IURL* u,
        /* [in] */ String str,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    virtual CARAPI SetURL(
        /* [in] */ IURL* u,
        /* [in] */ String protocol,
        /* [in] */ String host,
        /* [in] */ Int32 port,
        /* [in] */ String file,
        /* [in] */ String ref);

    virtual CARAPI SetURLEx(
        /* [in] */ IURL* u,
        /* [in] */ String protocol,
        /* [in] */ String host,
        /* [in] */ Int32 port,
        /* [in] */ String authority,
        /* [in] */ String userInfo,
        /* [in] */ String file,
        /* [in] */ String query,
        /* [in] */ String ref);

    virtual CARAPI ToExternalForm(
        /* [in] */ IURL* url,
        /* [out] */ String* s);

    virtual CARAPI Equals(
        /* [in] */ IURL* url1,
        /* [in] */ IURL* url2,
        /* [out] */ Boolean* isEquals);

    virtual CARAPI GetDefaultPort(
        /* [out] */ Int32* portNum);

    virtual CARAPI GetHostAddress(
        /* [in] */ IURL* url,
        /* [out] */ IInetAddress** hostAddress);

    virtual CARAPI HashCode(
        /* [in] */ IURL* url,
        /* [out] */ Int32* code);

    virtual CARAPI HostsEqual(
        /* [in] */ IURL* a,
        /* [in] */ IURL* b,
        /* [out] */ Boolean* isEqual);

    virtual CARAPI SameFile(
        /* [in] */ IURL* url1,
        /* [in] */ IURL* url2,
        /* [out] */ Boolean* isSame);

private:
    /*
     * If the URL host is empty while protocal is file, the host is regarded as
     * localhost.
     */
    static CARAPI_(String) GetHost(
        /* [in] */ IURL* url);
};

#endif //__URLSTREAMHANDLER_H__
