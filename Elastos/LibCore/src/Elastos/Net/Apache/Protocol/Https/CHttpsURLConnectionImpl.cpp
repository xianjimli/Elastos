
#include "CHttpsURLConnectionImpl.h"
#include "StringBuffer.h"

ECode CHttpsURLConnectionImpl::Connect()
{
    // TODO: Add your code here
    //mConnected = TRUE;
    return mHttpsEngine->Connect();
}

ECode CHttpsURLConnectionImpl::GetAllowUserInteraction(
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return mHttpsEngine->GetAllowUserInteraction(pValue);
}

ECode CHttpsURLConnectionImpl::GetContent(
    /* [out] */ IInterface ** ppContent)
{
    // TODO: Add your code here
    return mHttpsEngine->GetContent(ppContent);
}

ECode CHttpsURLConnectionImpl::GetContentEncoding(
    /* [out] */ String * pEncoding)
{
    // TODO: Add your code here
    return mHttpsEngine->GetContentEncoding(pEncoding);
}

ECode CHttpsURLConnectionImpl::GetContentLength(
    /* [out] */ Int32 * pLength)
{
    // TODO: Add your code here
    return mHttpsEngine->GetContentLength(pLength);
}

ECode CHttpsURLConnectionImpl::GetContentType(
    /* [out] */ String * pType)
{
    // TODO: Add your code here
    return mHttpsEngine->GetContentType(pType);
}

ECode CHttpsURLConnectionImpl::GetDate(
    /* [out] */ Int64 * pDate)
{
    // TODO: Add your code here
    return mHttpsEngine->GetDate(pDate);
}

ECode CHttpsURLConnectionImpl::GetDefaultUseCaches(
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return mHttpsEngine->GetDefaultUseCaches(pValue);
}

ECode CHttpsURLConnectionImpl::GetDoInput(
    /* [out] */ Boolean * pAllowed)
{
    // TODO: Add your code here
    return mHttpsEngine->GetDoInput(pAllowed);
}

ECode CHttpsURLConnectionImpl::GetDoOutput(
    /* [out] */ Boolean * pAllowed)
{
    // TODO: Add your code here
    return mHttpsEngine->GetDoOutput(pAllowed);
}

ECode CHttpsURLConnectionImpl::GetExpiration(
    /* [out] */ Int64 * pExpiration)
{
    // TODO: Add your code here
    return mHttpsEngine->GetExpiration(pExpiration);
}

ECode CHttpsURLConnectionImpl::GetHeaderFieldByPosition(
    /* [in] */ Int32 pos,
    /* [out] */ String * pValue)
{
    // TODO: Add your code here
    return mHttpsEngine->GetHeaderFieldByPosition(pos, pValue);
}

ECode CHttpsURLConnectionImpl::AddRequestProperty(
    /* [in] */ const String& field,
    /* [in] */ const String& newValue)
{
    // TODO: Add your code here
    return mHttpsEngine->AddRequestProperty(field, newValue);
}

ECode CHttpsURLConnectionImpl::GetHeaderFieldByKey(
    /* [in] */ const String& key,
    /* [out] */ String * pValue)
{
    // TODO: Add your code here
    return mHttpsEngine->GetHeaderFieldByKey(key, pValue);
}

ECode CHttpsURLConnectionImpl::GetHeaderFieldDate(
    /* [in] */ const String& field,
    /* [in] */ Int64 defaultValue,
    /* [out] */ Int64 * pValue)
{
    // TODO: Add your code here
    return mHttpsEngine->GetHeaderFieldDate(field, defaultValue, pValue);
}

ECode CHttpsURLConnectionImpl::GetHeaderFieldInt32(
    /* [in] */ const String& field,
    /* [in] */ Int32 defaultValue,
    /* [out] */ Int32 * pValue)
{
    // TODO: Add your code here
    return mHttpsEngine->GetHeaderFieldInt32(field, defaultValue, pValue);
}

ECode CHttpsURLConnectionImpl::GetHeaderFieldKey(
    /* [in] */ Int32 posn,
    /* [out] */ String * pKey)
{
    // TODO: Add your code here
    return mHttpsEngine->GetHeaderFieldKey(posn, pKey);
}

ECode CHttpsURLConnectionImpl::GetIfModifiedSince(
    /* [out] */ Int64 * pTime)
{
    // TODO: Add your code here
    return mHttpsEngine->GetIfModifiedSince(pTime);
}

ECode CHttpsURLConnectionImpl::GetInputStream(
    /* [out] */ IInputStream ** ppIs)
{
    // TODO: Add your code here
    return mHttpsEngine->GetInputStream(ppIs);
}

ECode CHttpsURLConnectionImpl::GetLastModified(
    /* [out] */ Int64 * pValue)
{
    // TODO: Add your code here
    return mHttpsEngine->GetLastModified(pValue);
}

ECode CHttpsURLConnectionImpl::GetOutputStream(
    /* [out] */ IOutputStream ** ppOs)
{
    // TODO: Add your code here
    return mHttpsEngine->GetOutputStream(ppOs);
}

ECode CHttpsURLConnectionImpl::GetRequestProperty(
    /* [in] */ const String& field,
    /* [out] */ String * pProperty)
{
    // TODO: Add your code here
    return mHttpsEngine->GetRequestProperty(field, pProperty);
}

ECode CHttpsURLConnectionImpl::GetURL(
    /* [out] */ IURL ** ppUrl)
{
    // TODO: Add your code here
    return mHttpsEngine->GetURL(ppUrl);
}

ECode CHttpsURLConnectionImpl::GetUseCaches(
    /* [out] */ Boolean * pAllowed)
{
    // TODO: Add your code here
    return mHttpsEngine->GetUseCaches(pAllowed);
}

ECode CHttpsURLConnectionImpl::SetAllowUserInteraction(
    /* [in] */ Boolean newValue)
{
    // TODO: Add your code here
    return mHttpsEngine->SetAllowUserInteraction(newValue);
}

ECode CHttpsURLConnectionImpl::SetDefaultUseCaches(
    /* [in] */ Boolean newValue)
{
    // TODO: Add your code here
    return mHttpsEngine->SetDefaultUseCaches(newValue);
}

ECode CHttpsURLConnectionImpl::SetDoInput(
    /* [in] */ Boolean newValue)
{
    // TODO: Add your code here
    return mHttpsEngine->SetDoInput(newValue);
}

ECode CHttpsURLConnectionImpl::SetDoOutput(
    /* [in] */ Boolean newValue)
{
    // TODO: Add your code here
    return mHttpsEngine->SetDoOutput(newValue);
}

ECode CHttpsURLConnectionImpl::SetIfModifiedSince(
    /* [in] */ Int64 newValue)
{
    // TODO: Add your code here
    return mHttpsEngine->SetIfModifiedSince(newValue);
}

ECode CHttpsURLConnectionImpl::SetRequestProperty(
    /* [in] */ const String& field,
    /* [in] */ const String& newValue)
{
    // TODO: Add your code here
    return mHttpsEngine->SetRequestProperty(field, newValue);
}

ECode CHttpsURLConnectionImpl::SetUseCaches(
    /* [in] */ Boolean newValue)
{
    // TODO: Add your code here
    return mHttpsEngine->SetUseCaches(newValue);
}

ECode CHttpsURLConnectionImpl::SetConnectTimeout(
    /* [in] */ Int32 timeout)
{
    // TODO: Add your code here
    return mHttpsEngine->SetConnectTimeout(timeout);
}

ECode CHttpsURLConnectionImpl::GetConnectTimeout(
    /* [out] */ Int32 * pTimeout)
{
    // TODO: Add your code here
    return mHttpsEngine->GetConnectTimeout(pTimeout);
}

ECode CHttpsURLConnectionImpl::SetReadTimeout(
    /* [in] */ Int32 timeout)
{
    // TODO: Add your code here
    return mHttpsEngine->SetReadTimeout(timeout);;
}

ECode CHttpsURLConnectionImpl::GetReadTimeout(
    /* [out] */ Int32 * pTimeout)
{
    // TODO: Add your code here
    return mHttpsEngine->GetReadTimeout(pTimeout);
}

ECode CHttpsURLConnectionImpl::Disconnect()
{
    // TODO: Add your code here
    return mHttpsEngine->Disconnect();
}

ECode CHttpsURLConnectionImpl::GetErrorStream(
    /* [out] */ IInputStream ** ppStream)
{
    // TODO: Add your code here
    return mHttpsEngine->GetErrorStream(ppStream);
}

ECode CHttpsURLConnectionImpl::GetRequestMethod(
    /* [out] */ String * pMethod)
{
    // TODO: Add your code here
    return mHttpsEngine->GetRequestMethod(pMethod);
}

ECode CHttpsURLConnectionImpl::GetResponseCode(
    /* [out] */ Int32 * pResponseCode)
{
    // TODO: Add your code here
    return mHttpsEngine->GetResponseCode(pResponseCode);
}

ECode CHttpsURLConnectionImpl::GetResponseMessage(
    /* [out] */ String * pMessage)
{
    // TODO: Add your code here
    return mHttpsEngine->GetResponseMessage(pMessage);
}

ECode CHttpsURLConnectionImpl::SetRequestMethod(
    /* [in] */ const String& method)
{
    // TODO: Add your code here
    return mHttpsEngine->SetRequestMethod(method);
}

ECode CHttpsURLConnectionImpl::UsingProxy(
    /* [out] */ Boolean * pUsingProxy)
{
    // TODO: Add your code here
    return mHttpsEngine->UsingProxy(pUsingProxy);
}

ECode CHttpsURLConnectionImpl::GetInstanceFollowRedirects(
    /* [out] */ Boolean * pFollowRedirects)
{
    // TODO: Add your code here
    return mHttpsEngine->GetInstanceFollowRedirects(pFollowRedirects);
}

ECode CHttpsURLConnectionImpl::SetInstanceFollowRedirects(
    /* [in] */ Boolean followRedirects)
{
    // TODO: Add your code here
    return mHttpsEngine->SetInstanceFollowRedirects(followRedirects);
}

ECode CHttpsURLConnectionImpl::SetFixedLengthStreamingMode(
    /* [in] */ Int32 contentLength)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpsURLConnectionImpl::SetChunkedStreamingMode(
    /* [in] */ Int32 chunkLength)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpsURLConnectionImpl::GetCipherSuite(
    /* [out] */ String * pCipherSuite)
{
    // TODO: Add your code here
	ECode ec = NOERROR;
	ec = CheckConnected();
	if (FAILED(ec)) {
		return ec;
	}
	
	AutoPtr<ISSLSession> session;
	ec = mSslSocket->GetSession((ISSLSession**)&session);
	if (FAILED(ec)) {
		return ec;
	}
	
	ec = session->GetCipherSuite(pCipherSuite);
	return ec;;
}

ECode CHttpsURLConnectionImpl::GetLocalCertificates(
    /* [out, callee] */ ArrayOf<ICertificate *> ** ppCertificates)
{
    // TODO: Add your code here
    ECode ec = NOERROR;
    CheckConnected();
    
    AutoPtr<ISSLSession> session;
	ec = mSslSocket->GetSession((ISSLSession**)&session);
	if (FAILED(ec)) {
		return ec;
	}
    
    return session->GetLocalCertificates(ppCertificates);
}

ECode CHttpsURLConnectionImpl::GetServerCertificates(
    /* [out, callee] */ ArrayOf<ICertificate *> ** ppPpCertificates)
{
    // TODO: Add your code here
    ECode ec = NOERROR;
    CheckConnected();
    
    AutoPtr<ISSLSession> session;
	ec = mSslSocket->GetSession((ISSLSession**)&session);
	if (FAILED(ec)) {
		return ec;
	}
    
    return session->GetPeerCertificates(ppPpCertificates);
}

ECode CHttpsURLConnectionImpl::GetPeerPrincipal(
    /* [out] */ IPrincipal ** ppPrincipal)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpsURLConnectionImpl::GetLocalPrincipal(
    /* [out] */ IPrincipal ** ppPrincipal)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpsURLConnectionImpl::SetHostnameVerifier(
    /* [in] */ IHostnameVerifier * pV)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpsURLConnectionImpl::GetHostnameVerifier(
    /* [out] */ IHostnameVerifier ** ppVerifier)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpsURLConnectionImpl::SetSSLSocketFactory(
    /* [in] */ ISSLSocketFactory * pSf)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpsURLConnectionImpl::GetSSLSocketFactory(
    /* [out] */ ISSLSocketFactory ** ppSslSocketFactory)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpsURLConnectionImpl::constructor(
    /* [in] */ IURL * pUrl,
    /* [in] */ Int32 port)
{
    // TODO: Add your code here
    HttpsEngine::NewByFriend(pUrl, port, (CHttpURLConnectionImpl**)&mHttpsEngine);
    return NOERROR;
}

ECode CHttpsURLConnectionImpl::constructor(
    /* [in] */ IURL * pUrl,
    /* [in] */ Int32 port,
    /* [in] */ IProxy * pProxy)
{
    // TODO: Add your code here
    HttpsEngine::NewByFriend(pUrl, port, pProxy, (CHttpURLConnectionImpl**)&mHttpsEngine);
    return NOERROR;
}

ECode CHttpsURLConnectionImpl::CheckConnected()
{
	if (mSslSocket == NULL) {
		return E_ILLEGAL_STATE_EXCEPTION;
	}
	return NOERROR;
}

CHttpsURLConnectionImpl::HttpsEngine::HttpsEngine(
    /* [in] */ IURL* url,
    /* [in] */ Int32 port,
    /* [in] */ CHttpsURLConnectionImpl* impl)
{
    CHttpURLConnectionImpl::constructor(url, port);
    mImpl = impl;
}

CHttpsURLConnectionImpl::HttpsEngine::HttpsEngine(
	/* [in] */ IURL* url, 
	/* [in] */ Int32 port, 
	/* [in] */ IProxy* proxy,
	/* [in] */ CHttpsURLConnectionImpl* impl)
{
	CHttpURLConnectionImpl::constructor(url, port, proxy);
	mImpl = impl;
}


ECode CHttpsURLConnectionImpl::HttpsEngine::MakeConnection()
{
    /*
     * Short-circuit a reentrant call. The first step in doing SSL with
     * an HTTP proxy requires calling retrieveResponse() which calls
     * back into makeConnection(). We can return immediately because the
     * unencrypted connection is already valid.
     */
    if (mMethod == CONNECT) {
        return NOERROR;
    }

    Boolean connectionReused;
    // first try an SSL connection with compression and
    // various TLS extensions enabled, if it fails (and its
    // not unheard of that it will) fallback to a more
    // barebones connections
    ECode ec = NOERROR;
    ec = MakeSslConnection(TRUE, &connectionReused);
    if (FAILED(ec)) {
		// If the problem was a CertificateException from the X509TrustManager,
        // do not retry, we didn't have an abrupt server initiated exception.
        ReleaseSocket(FALSE);
        MakeSslConnection(FALSE, &connectionReused);    	
    }

    if (!connectionReused) {
    	AutoPtr<IHostnameVerifier> verifier;
    	mImpl->GetHostnameVerifier((IHostnameVerifier**) &verifier);
        mImpl->mSslSocket = mConnection->VerifySecureSocketHostname(verifier);
    }
    
    SetUpTransportIO(mConnection);
	return NOERROR;	
}

ECode CHttpsURLConnectionImpl::HttpsEngine::SetUpTransportIO(HttpConnection* connection)
{
	Boolean value;
	RequiresTunnel(&value);
    if (!value && mImpl->mSslSocket == NULL) {
        return E_ILLEGAL_STATE_EXCEPTION; // don't initialize streams that won't be used
    }
    return CHttpURLConnectionImpl::SetUpTransportIO(connection);
}


ECode CHttpsURLConnectionImpl::HttpsEngine::MakeSslConnection(
	/* [in] */ Boolean tlsTolerant,
	/* [out] */ Boolean* pValue)
{
	ECode ec = NOERROR;
    CHttpURLConnectionImpl::MakeConnection();

    // if super.makeConnection returned a connection from the
    // pool, sslSocket needs to be initialized here. If it is
    // a new connection, it will be initialized by
    // getSecureSocket below.
    mImpl->mSslSocket = mConnection->GetSecureSocketIfConnected();

    // we already have an SSL connection,
    if (mImpl->mSslSocket != NULL) {
		*pValue = TRUE;
		return NOERROR;
    }

    // make SSL Tunnel
    Boolean value;
    RequiresTunnel(&value);
    if (value) {
        String originalMethod = mMethod;
        mMethod = CONNECT;
        mIntermediateResponse = TRUE;
        
        ec = RetrieveResponse();
        if (FAILED(ec)) {
        	return ec;
        }
        
        DiscardIntermediateResponse();
        
        mMethod = originalMethod;
        mIntermediateResponse = FALSE;
    }

	AutoPtr<ISSLSocketFactory> factory;
	mImpl->GetSSLSocketFactory((ISSLSocketFactory**) &factory);
    mConnection->SetupSecureSocket(factory, tlsTolerant);
    *pValue = FALSE;
    return ec;
}

ECode CHttpsURLConnectionImpl::HttpsEngine::RequiresTunnel(
	/* [out] */ Boolean* pValue)
{
	return UsingProxy(pValue);
}


ECode CHttpsURLConnectionImpl::HttpsEngine::RequestString(
	/* [out] */ String* pStr)
{
	Boolean useing;
	UsingProxy(&useing);
	if (!useing) {
		*pStr = CHttpURLConnectionImpl::RequestString();
	    return NOERROR;
	} else if (mMethod == CONNECT) {
	    // SSL tunnels require host:port for the origin server
		StringBuffer buf;
		String host;
		mUrl->GetHost(&host);
		buf += host;
		buf += String(":");
		Int32 port;
		mUrl->GetEffectivePort(&port);
		buf += port;
		*pStr = buf.ToString();
		return NOERROR;
	
	} else {
	    // we has made SSL Tunneling, return /requested.data
	    String file;
	    mUrl->GetFile(&file);
	    if (file.IsNull() || file.GetLength() == 0) {
	    	file = String("/");
	    }
		*pStr = file;
	};
	return NOERROR;
}