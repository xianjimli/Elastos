
#ifndef __CHTTPSURLCONNECTIONIMPL_H__
#define __CHTTPSURLCONNECTIONIMPL_H__

#include "_CHttpsURLConnectionImpl.h"
#include <elastos/AutoPtr.h>
#include "CHttpURLConnectionImpl.h"

CarClass(CHttpsURLConnectionImpl)
{
	class HttpsEngine : public CHttpURLConnectionImpl
	{
	protected:
		HttpsEngine(
		    /* [in] */ IURL* url,
		    /* [in] */ Int32 port,
		    /* [in] */ CHttpsURLConnectionImpl* impl);
		
		HttpsEngine(
			/* [in] */ IURL* url, 
			/* [in] */ Int32 port, 
			/* [in] */ IProxy* proxy,
			/* [in] */ CHttpsURLConnectionImpl* impl);
			
		ECode SetUpTransportIO(HttpConnection* connection);
		
		ECode RequiresTunnel(
			/* [out] */ Boolean* pValue);
			
		ECode RequestString(
			/* [out] */ String* pStr);			
	
	public:
		ECode MakeConnection();
	
	private:
		ECode MakeSslConnection(
			/* [in] */ Boolean tlsTolerant,
			/* [out] */ Boolean* pValue);
	private:
		AutoPtr<CHttpsURLConnectionImpl> mImpl;
	};

	friend class HttpsEngine;	
public:
    CARAPI Connect();

    CARAPI GetAllowUserInteraction(
        /* [out] */ Boolean * pValue);

    CARAPI GetContent(
        /* [out] */ IInterface ** ppContent);

    CARAPI GetContentEncoding(
        /* [out] */ String * pEncoding);

    CARAPI GetContentLength(
        /* [out] */ Int32 * pLength);

    CARAPI GetContentType(
        /* [out] */ String * pType);

    CARAPI GetDate(
        /* [out] */ Int64 * pDate);

    CARAPI GetDefaultUseCaches(
        /* [out] */ Boolean * pValue);

    CARAPI GetDoInput(
        /* [out] */ Boolean * pAllowed);

    CARAPI GetDoOutput(
        /* [out] */ Boolean * pAllowed);

    CARAPI GetExpiration(
        /* [out] */ Int64 * pExpiration);

    CARAPI GetHeaderFieldByPosition(
        /* [in] */ Int32 pos,
        /* [out] */ String * pValue);

    CARAPI AddRequestProperty(
        /* [in] */ const String& field,
        /* [in] */ const String& newValue);

    CARAPI GetHeaderFieldByKey(
        /* [in] */ const String& key,
        /* [out] */ String * pValue);

    CARAPI GetHeaderFieldDate(
        /* [in] */ const String& field,
        /* [in] */ Int64 defaultValue,
        /* [out] */ Int64 * pValue);

    CARAPI GetHeaderFieldInt32(
        /* [in] */ const String& field,
        /* [in] */ Int32 defaultValue,
        /* [out] */ Int32 * pValue);

    CARAPI GetHeaderFieldKey(
        /* [in] */ Int32 posn,
        /* [out] */ String * pKey);

    CARAPI GetIfModifiedSince(
        /* [out] */ Int64 * pTime);

    CARAPI GetInputStream(
        /* [out] */ IInputStream ** ppIs);

    CARAPI GetLastModified(
        /* [out] */ Int64 * pValue);

    CARAPI GetOutputStream(
        /* [out] */ IOutputStream ** ppOs);

    CARAPI GetRequestProperty(
        /* [in] */ const String& field,
        /* [out] */ String * pProperty);

    CARAPI GetURL(
        /* [out] */ IURL ** ppUrl);

    CARAPI GetUseCaches(
        /* [out] */ Boolean * pAllowed);

    CARAPI SetAllowUserInteraction(
        /* [in] */ Boolean newValue);

    CARAPI SetDefaultUseCaches(
        /* [in] */ Boolean newValue);

    CARAPI SetDoInput(
        /* [in] */ Boolean newValue);

    CARAPI SetDoOutput(
        /* [in] */ Boolean newValue);

    CARAPI SetIfModifiedSince(
        /* [in] */ Int64 newValue);

    CARAPI SetRequestProperty(
        /* [in] */ const String& field,
        /* [in] */ const String& newValue);

    CARAPI SetUseCaches(
        /* [in] */ Boolean newValue);

    CARAPI SetConnectTimeout(
        /* [in] */ Int32 timeout);

    CARAPI GetConnectTimeout(
        /* [out] */ Int32 * pTimeout);

    CARAPI SetReadTimeout(
        /* [in] */ Int32 timeout);

    CARAPI GetReadTimeout(
        /* [out] */ Int32 * pTimeout);

    CARAPI Disconnect();

    CARAPI GetErrorStream(
        /* [out] */ IInputStream ** ppStream);

    CARAPI GetRequestMethod(
        /* [out] */ String * pMethod);

    CARAPI GetResponseCode(
        /* [out] */ Int32 * pResponseCode);

    CARAPI GetResponseMessage(
        /* [out] */ String * pMessage);

    CARAPI SetRequestMethod(
        /* [in] */ const String& method);

    CARAPI UsingProxy(
        /* [out] */ Boolean * pUsingProxy);

    CARAPI GetInstanceFollowRedirects(
        /* [out] */ Boolean * pFollowRedirects);

    CARAPI SetInstanceFollowRedirects(
        /* [in] */ Boolean followRedirects);

    CARAPI SetFixedLengthStreamingMode(
        /* [in] */ Int32 contentLength);

    CARAPI SetChunkedStreamingMode(
        /* [in] */ Int32 chunkLength);

    CARAPI GetCipherSuite(
        /* [out] */ String * pCipherSuite);

    CARAPI GetLocalCertificates(
        /* [out, callee] */ ArrayOf<ICertificate *> ** ppCertificates);

    CARAPI GetServerCertificates(
        /* [out, callee] */ ArrayOf<ICertificate *> ** ppPpCertificates);

    CARAPI GetPeerPrincipal(
        /* [out] */ IPrincipal ** ppPrincipal);

    CARAPI GetLocalPrincipal(
        /* [out] */ IPrincipal ** ppPrincipal);

    CARAPI SetHostnameVerifier(
        /* [in] */ IHostnameVerifier * pV);

    CARAPI GetHostnameVerifier(
        /* [out] */ IHostnameVerifier ** ppVerifier);

    CARAPI SetSSLSocketFactory(
        /* [in] */ ISSLSocketFactory * pSf);

    CARAPI GetSSLSocketFactory(
        /* [out] */ ISSLSocketFactory ** ppSslSocketFactory);

    CARAPI constructor(
        /* [in] */ IURL * pUrl,
        /* [in] */ Int32 port);

    CARAPI constructor(
        /* [in] */ IURL * pUrl,
        /* [in] */ Int32 port,
        /* [in] */ IProxy * pProxy);

private:
	ECode CheckConnected();
	
private:
    AutoPtr<HttpsEngine> mHttpsEngine;
    AutoPtr<ISSLSocket> mSslSocket;
	
    // TODO: Add your private member variables here.
};

#endif // __CHTTPSURLCONNECTIONIMPL_H__
