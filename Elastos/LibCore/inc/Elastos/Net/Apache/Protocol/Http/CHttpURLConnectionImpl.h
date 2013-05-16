
#ifndef __CHTTPURLCONNECTIONIMPL_H__
#define __CHTTPURLCONNECTIONIMPL_H__

#include "_CHttpURLConnectionImpl.h"
#include "Elastos.IO.h"
#include "CProxy.h"
#include "CURI.h"
#include "HttpConnection.h"
//#include "CResponseCache.h"
//#include "CCacheResponse.h"
//#include "CCacheRequest.h"

CarClass(CHttpURLConnectionImpl)
{
public:
    CHttpURLConnectionImpl();

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

    CARAPI constructor(
        /* [in] */ IURL * pUrl,
        /* [in] */ Int32 port);

    CARAPI constructor(
        /* [in] */ IURL * pUrl,
        /* [in] */ Int32 port,
        /* [in] */ IProxy * pProxy);

public:
    static String OPTIONS;
    static String GET;
    static String HEAD;
    static String POST;
    static String PUT;
    static String DELETE;
    static String TRACE;
    static String CONNECT;

    static Int32 HTTP_CONTINUE;
    static Int32 MAX_REDIRECTS;
    static Int32 DEFAULT_CHUNK_LENGTH;

    static String PERMITTED_USER_METHODS[];

protected:
    HttpConnection* mConnection;
    Boolean mIntermediateResponse;

private:
    // TODO: Add your private member variables here.
    Int32 mDefaultPort;
    Int32 mHttpVersion; // Assume HTTP/1.1
    AutoPtr<IInputStream> mSocketIn;
    AutoPtr<IOutputStream> mSocketOut;
    AutoPtr<IInputStream> mResponseBodyIn;

//    IAbstractHttpOutputStream* mRequestBodyOut;
//    AutoPtr<IResponseCache> mResponseCache;
//    AutoPtr<ICacheResponse> mCacheResponse;
//    AutoPtr<ICacheRequest> mCacheRequest;

    Boolean mHasTriedCache;
    Boolean mSentRequestHeaders;
    Boolean mTransparentGzip;
    Boolean mSendChunked;

    AutoPtr<IProxy> mProxy;

    // the destination URI
    AutoPtr<IURI> mUri;

//    static Header* mDefaultRequestHeader;
//    Header* mRequestHeader;

    /** Null until a response is received from the network or the cache */
//    Header* mResponseHeader;

    Int32 mRedirectionCount;

    /**
     * Intermediate responses are always followed by another request for the
     * same content, possibly from a different URL or with different headers.
     */
};

#endif // __CHTTPURLCONNECTIONIMPL_H__
