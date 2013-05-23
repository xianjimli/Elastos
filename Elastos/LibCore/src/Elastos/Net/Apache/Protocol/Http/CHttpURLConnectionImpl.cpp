
#include "CHttpURLConnectionImpl.h"
#include "HttpConnectionPool.h"
#include "ResponseCache.h"
#include "StringBuffer.h"
#include "CURL.h"
#include "CInetSocketAddress.h"

String CHttpURLConnectionImpl::OPTIONS = String("OPTIONS");
String CHttpURLConnectionImpl::GET = String("GET");
String CHttpURLConnectionImpl::HEAD = String("HEAD");
String CHttpURLConnectionImpl::POST = String("POST");
String CHttpURLConnectionImpl::PUT = String("PUT");
String CHttpURLConnectionImpl::DELETE = String("DELETE");
String CHttpURLConnectionImpl::TRACE = String("TRACE");
String CHttpURLConnectionImpl::CONNECT = String("CONNECT");

Int32 CHttpURLConnectionImpl::HTTP_CONTINUE = 100;
Int32 CHttpURLConnectionImpl::MAX_REDIRECTS = 5;
Int32 CHttpURLConnectionImpl::DEFAULT_CHUNK_LENGTH = 1024;

Header* CHttpURLConnectionImpl::mDefaultRequestHeader;

String CHttpURLConnectionImpl::PERMITTED_USER_METHODS[] = {
            OPTIONS,
            GET,
            HEAD,
            POST,
            PUT,
            DELETE,
            TRACE
            // Note: we don't allow users to specify "CONNECT"
};


CHttpURLConnectionImpl::CHttpURLConnectionImpl()
    :mHttpVersion(1), mTransparentGzip(FALSE), mIntermediateResponse(FALSE)
{
}

ECode CHttpURLConnectionImpl::Connect()
{
    // TODO: Add your code here
    if (mConnected) {
        return NOERROR;
    }
    return MakeConnection();
}

ECode CHttpURLConnectionImpl::MakeConnection()
{
    mConnected = TRUE;
    ECode ec = NOERROR;
    if (mConnection != NULL) {
        return NOERROR;
    }

    Boolean is;
    GetFromCache(&is);
    if (is) {
        return NOERROR;
    }

    /*
     * URL.toURI() throws if it has illegal characters. Since we don't mind
     * illegal characters for proxy selection, just create the minimal URI.
     */
    String protocol;
    String host;
    Int32 port;
    String path;
    mUrl->GetProtocol(&protocol);
    mUrl->GetHost(&host);
    mUrl->GetPort(&port);
    mUrl->GetPath(&path);

    ec = CURI::New(protocol, String(""), host, port, path, String(""), String(""), (IURI**)&mUri);
    if (FAILED(ec)) {
        return ec;
    }

    // try to determine: to use the proxy or not
    if (mProxy != NULL) {
        // try to make the connection to the proxy
        // specified in constructor.
        // IOException will be thrown in the case of failure
        ec = GetHttpConnection(mProxy, &mConnection);
        if (FAILED(ec)) {
            return ec;
        }
    } else {
        // Use system-wide ProxySelect to select proxy list,
        // then try to connect via elements in the proxy list.
//        AutoPtr<IProxySelector> selector;
//        ProxySelector::GetDefault((IProxySelector **)&selector);
//
//        List<IProxy*> *proxyList = selector->Select(uri);
//        if (proxyList != null) {
//            for (Proxy selectedProxy : proxyList) {
//                if (selectedProxy.type() == Proxy.Type.DIRECT) {
//                    // the same as NO_PROXY
//                    continue;
//                }
//                try {
//                    connection = getHttpConnection(selectedProxy);
//                    proxy = selectedProxy;
//                    break; // connected
//                } catch (IOException e) {
//                    // failed to connect, tell it to the selector
//                    selector.connectFailed(uri, selectedProxy.address(), e);
//                }
//            }
//        }
//        if (connection == null) {
//            // make direct connection
//            connection = getHttpConnection(null);
//        }
    }
//    connection.setSoTimeout(getReadTimeout());
//    setUpTransportIO(connection);
}

ECode CHttpURLConnectionImpl::GetHttpConnection(
        /* [in] */ IProxy* proxy,
        /* [out] */ HttpConnection** connection)
{
    HttpConnection::Address address((IURI*)mUri.Get());
//    *address = new HttpConnection::Address((IURI*)mUri.Get());
//    if (proxy == null || proxy.type() == Proxy.Type.DIRECT) {
//        this.proxy = null; // not using proxy
//        address = new HttpConnection.Address(uri);
//    } else {
//        address = new HttpConnection.Address(uri, proxy, requiresTunnel());
//    }
    Int32 timeout;
    GetConnectTimeout(&timeout);
    HttpConnectionPool::INSTANCE->Get(&address, timeout, connection);
    return NOERROR;
}

ECode CHttpURLConnectionImpl::GetAllowUserInteraction(
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpURLConnectionImpl::GetContent(
    /* [out] */ IInterface ** ppContent)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpURLConnectionImpl::GetContentEncoding(
    /* [out] */ String * pEncoding)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpURLConnectionImpl::GetContentLength(
    /* [out] */ Int32 * pLength)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpURLConnectionImpl::GetContentType(
    /* [out] */ String * pType)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpURLConnectionImpl::GetDate(
    /* [out] */ Int64 * pDate)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpURLConnectionImpl::GetDefaultUseCaches(
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpURLConnectionImpl::GetDoInput(
    /* [out] */ Boolean * pAllowed)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpURLConnectionImpl::GetDoOutput(
    /* [out] */ Boolean * pAllowed)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpURLConnectionImpl::GetExpiration(
    /* [out] */ Int64 * pExpiration)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpURLConnectionImpl::GetHeaderFieldByPosition(
    /* [in] */ Int32 pos,
    /* [out] */ String * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpURLConnectionImpl::AddRequestProperty(
    /* [in] */ const String& field,
    /* [in] */ const String& newValue)
{
    // TODO: Add your code here
    if (mConnected) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    if (field.IsNull()) {
        return E_INVALID_ARGUMENT;
    }
    mRequestHeader->Add(field, newValue);
    return NOERROR;
}

ECode CHttpURLConnectionImpl::GetHeaderFieldByKey(
    /* [in] */ const String& key,
    /* [out] */ String * pValue)
{
    // TODO: Add your code here
    AutoPtr<IInputStream> in;
    ECode ec = NOERROR;
    ec = GetInputStream((IInputStream**)&in);
    if (FAILED(ec)) {
        return ec;
    }

    if (NULL == mResponseHeader) {
        *pValue = NULL;
        return NOERROR;
    }
    *pValue = mResponseHeader->Get(key);
    return NOERROR;
}

ECode CHttpURLConnectionImpl::GetHeaderFieldDate(
    /* [in] */ const String& field,
    /* [in] */ Int64 defaultValue,
    /* [out] */ Int64 * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpURLConnectionImpl::GetHeaderFieldInt32(
    /* [in] */ const String& field,
    /* [in] */ Int32 defaultValue,
    /* [out] */ Int32 * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpURLConnectionImpl::GetHeaderFieldKey(
    /* [in] */ Int32 posn,
    /* [out] */ String * pKey)
{
    // TODO: Add your code here
    AutoPtr<IInputStream> in;
    ECode ec = NOERROR;
    ec = GetInputStream((IInputStream**)&in);
    if (FAILED(ec)) {
        return ec;
    }

    if (NULL == mResponseHeader) {
        *pKey = NULL;
        return NOERROR;
    }
    *pKey = mResponseHeader->Get(posn);
    return NOERROR;
}

ECode CHttpURLConnectionImpl::GetIfModifiedSince(
    /* [out] */ Int64 * pTime)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}




ECode CHttpURLConnectionImpl::GetInputStream(
    /* [out] */ IInputStream ** ppIs)
{
    // TODO: Add your code here
    if (!mDoInput) {
        return E_PROTOCOL_EXCEPTION;
    }
    ECode ec;
    ec = RetrieveResponse();
    if (FAILED(ec)) return ec;
    /*
     * if the requested file does not exist, throw an exception formerly the
     * Error page from the server was returned if the requested file was
     * text/html this has changed to return FileNotFoundException for all
     * file types
     */
    if (mResponseCode >= HTTP_BAD_REQUEST) {
        return E_FILE_NOT_FOUND_EXCEPTION;
    }

    if (mResponseBodyIn == NULL) {
        return E_IO_EXCEPTION;
    }

    *ppIs = mResponseBodyIn;
    return NOERROR;
}

ECode CHttpURLConnectionImpl::GetLastModified(
    /* [out] */ Int64 * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpURLConnectionImpl::GetOutputStream(
    /* [out] */ IOutputStream ** ppOs)
{
    // TODO: Add your code here
    if (!mDoOutput) {
        return E_PROTOCOL_EXCEPTION;
    }

    // you can't write after you read
    if (mSentRequestHeaders) {
        // TODO: just return 'requestBodyOut' if that's non-null?
        return E_PROTOCOL_EXCEPTION;
    }

    if (mRequestBodyOut != NULL) {
        *ppOs = mRequestBodyOut;
    }

    // they are requesting a stream to write to. This implies a POST method
    if (mMethod == GET) {
        mMethod = POST;
    }

    // If the request method is neither PUT or POST, then you're not writing
    if (mMethod != PUT && mMethod != POST) {
        return E_PROTOCOL_EXCEPTION;
    }

    Int32 contentLength = -1;
    String contentLengthString = mRequestHeader->Get(String("Content-Length"));
    if (!contentLengthString.IsNull()) {
        contentLength = contentLengthString.ToInt32();
    }

    String encoding = mRequestHeader->Get(String("Transfer-Encoding"));
    if (mChunkLength > 0 || String("chunked").EqualsIgnoreCase(encoding)) {
        mSendChunked = TRUE;
        contentLength = -1;
        if (mChunkLength == -1) {
            mChunkLength = DEFAULT_CHUNK_LENGTH;
        }
    }

    Connect();

    if (mSocketOut == NULL) {
        // TODO: what should we do if a cached response exists?
        return E_IO_EXCEPTION;
    }

    if (mHttpVersion == 0) {
        mSendChunked = FALSE;
    }

    if (mFixedContentLength != -1) {
        WriteRequestHeaders(mSocketOut);
        //requestBodyOut = new FixedLengthOutputStream(socketOut, fixedContentLength);
    } else if (mSendChunked) {
        WriteRequestHeaders(mSocketOut);
        //requestBodyOut = new ChunkedOutputStream(socketOut, chunkLength);
    } else if (contentLength != -1) {
        //requestBodyOut = new RetryableOutputStream(contentLength);
    } else {
        //requestBodyOut = new RetryableOutputStream();
    }

    *ppOs = mRequestBodyOut;
    return NOERROR;
}

ECode CHttpURLConnectionImpl::GetRequestProperty(
    /* [in] */ const String& field,
    /* [out] */ String * pProperty)
{
    // TODO: Add your code here
    if (field.IsNull()) {
        *pProperty = NULL;
        return NOERROR;
    }
    *pProperty = mRequestHeader->Get(field);
    return NOERROR;
}

ECode CHttpURLConnectionImpl::GetURL(
    /* [out] */ IURL ** ppUrl)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpURLConnectionImpl::GetUseCaches(
    /* [out] */ Boolean * pAllowed)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpURLConnectionImpl::SetAllowUserInteraction(
    /* [in] */ Boolean newValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpURLConnectionImpl::SetDefaultUseCaches(
    /* [in] */ Boolean newValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpURLConnectionImpl::SetDoInput(
    /* [in] */ Boolean newValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpURLConnectionImpl::SetDoOutput(
    /* [in] */ Boolean newValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpURLConnectionImpl::SetIfModifiedSince(
    /* [in] */ Int64 newValue)
{
    // TODO: Add your code here
    HttpURLConnection::SetIfModifiedSince(newValue);
    // convert from millisecond since epoch to date string
//    SimpleDateFormat sdf = new SimpleDateFormat("E, dd MMM yyyy HH:mm:ss 'GMT'", Locale.US);
//    sdf.setTimeZone(TimeZone.getTimeZone("GMT"));
//    String date = sdf.format(new Date(newValue));
//    requestHeader.add("If-Modified-Since", date);
}

ECode CHttpURLConnectionImpl::SetRequestProperty(
    /* [in] */ const String& field,
    /* [in] */ const String& newValue)
{
    // TODO: Add your code here
    if (mConnected) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    if (field.IsNull()) {
        return E_INVALID_ARGUMENT;
    }

    mRequestHeader->Set(field, newValue);
    return NOERROR;
}

ECode CHttpURLConnectionImpl::SetUseCaches(
    /* [in] */ Boolean newValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpURLConnectionImpl::SetConnectTimeout(
    /* [in] */ Int32 timeout)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpURLConnectionImpl::GetConnectTimeout(
    /* [out] */ Int32 * pTimeout)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpURLConnectionImpl::SetReadTimeout(
    /* [in] */ Int32 timeout)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpURLConnectionImpl::GetReadTimeout(
    /* [out] */ Int32 * pTimeout)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpURLConnectionImpl::Disconnect()
{
    // TODO: Add your code here
    return ReleaseSocket(FALSE);;
}

ECode CHttpURLConnectionImpl::GetErrorStream(
    /* [out] */ IInputStream ** ppStream)
{
    // TODO: Add your code here
    if (mConnected && mMethod != HEAD && mResponseCode >= HTTP_BAD_REQUEST) {
        *ppStream = mResponseBodyIn;
        return NOERROR;
    }
    *ppStream = NULL;
    return NOERROR;
}

ECode CHttpURLConnectionImpl::GetRequestMethod(
    /* [out] */ String * pMethod)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpURLConnectionImpl::GetResponseCode(
    /* [out] */ Int32 * pResponseCode)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpURLConnectionImpl::GetResponseMessage(
    /* [out] */ String * pMessage)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpURLConnectionImpl::SetRequestMethod(
    /* [in] */ const String& method)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpURLConnectionImpl::UsingProxy(
    /* [out] */ Boolean * pUsingProxy)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpURLConnectionImpl::GetInstanceFollowRedirects(
    /* [out] */ Boolean * pFollowRedirects)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpURLConnectionImpl::SetInstanceFollowRedirects(
    /* [in] */ Boolean followRedirects)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpURLConnectionImpl::SetFixedLengthStreamingMode(
    /* [in] */ Int32 contentLength)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpURLConnectionImpl::SetChunkedStreamingMode(
    /* [in] */ Int32 chunkLength)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpURLConnectionImpl::constructor(
    /* [in] */ IURL * pUrl,
    /* [in] */ Int32 port)
{
    // TODO: Add your code here
    HttpURLConnection::Init(pUrl);
    mDefaultPort = port;
    mRequestHeader = mDefaultRequestHeader;
    ECode ec = NOERROR;
    ec = pUrl->ToURI((IURI**)&mUri);
    if (FAILED(ec)){
        return ec;
    }

//    responseCache = AccessController
//            .doPrivileged(new PrivilegedAction<ResponseCache>() {
//                public ResponseCache run() {
//                    return ResponseCache.getDefault();
//                }
//            });
}

ECode CHttpURLConnectionImpl::constructor(
    /* [in] */ IURL * pUrl,
    /* [in] */ Int32 port,
    /* [in] */ IProxy * pProxy)
{
    // TODO: Add your code here
    constructor(pUrl, port);
    mProxy = pProxy;
}


ECode CHttpURLConnectionImpl::GetFromCache(
    /* [out] */ Boolean* is)
{
    if (!mUseCaches || mResponseCache == NULL || mHasTriedCache) {
        *is = (mHasTriedCache && mSocketIn != NULL);
        return NOERROR;
    }

    mHasTriedCache = TRUE;
    ResponseCache::GetDefault((IResponseCache**)&mResponseCache);
    if (mResponseCache == NULL) {
        *is = (mSocketIn != NULL);
        return NOERROR;
    }

    *is = (mSocketIn != NULL);
    return NOERROR;
//    if (cacheResponse == null) {
//        return socketIn != null; // TODO: if this is non-null, why are we calling getFromCache?
//    }
//    Map<String, List<String>> headersMap = cacheResponse.getHeaders();
//    if (headersMap != null) {
//        responseHeader = new Header(headersMap);
//    }
//    socketIn = responseBodyIn = cacheResponse.getBody();
//    return socketIn != null;
}

CARAPI_(PInterface) CHttpURLConnectionImpl::Probe(
    /* [in]  */ REIID riid)
{
}

Boolean CHttpURLConnectionImpl::HasConnectionCloseHeader()
{
    String cl("close");
    return (mResponseHeader != NULL
                && cl.EqualsIgnoreCase(mResponseHeader->Get(String("Connection"))))
                || (mRequestHeader != NULL
                && cl.EqualsIgnoreCase(mRequestHeader->Get(String("Connection"))));
}

ECode CHttpURLConnectionImpl::ReleaseSocket(
    /* [in] */ Boolean reuseSocket)
{
    if (mRequestBodyOut != NULL) {
        reuseSocket = FALSE;
    }

    // if the headers specify that the connection shouldn't be reused, don't reuse it
    if (HasConnectionCloseHeader()) {
        reuseSocket = FALSE;
    }

    /*
     * Don't return the socket to the connection pool if this is an
     * intermediate response; we're going to use it again right away.
     */
    if (mIntermediateResponse && reuseSocket) {
        return NOERROR;
    }

    if (mConnection != NULL) {
        if (reuseSocket) {
            HttpConnectionPool::INSTANCE->Recycle(mConnection);
        } else {
            mConnection->CloseSocketAndStreams();
        }
        mConnection = NULL;
    }

    /*
     * Clear "socketIn" and "socketOut" to ensure that no further I/O
     * attempts from this instance make their way to the underlying
     * connection (which may get recycled).
     */
    mSocketIn = NULL;
    mSocketOut = NULL;
}

ECode CHttpURLConnectionImpl::SetUpTransportIO(
    /* [in] */ HttpConnection* connection)
{
    ECode ec = NOERROR;
    ec = mConnection->GetOutputStream((IOutputStream**)&mSocketOut);
    if (FAILED(ec)) {
        return ec;
    }

    ec = mConnection->GetInputStream((IInputStream**)&mSocketIn);
    if (FAILED(ec)) {
        return ec;
    }
    return ec;
}

ECode CHttpURLConnectionImpl::MaybeCache()
{
    // Are we caching at all?
    if (!mUseCaches || mResponseCache == NULL) {
        return NOERROR;
    }
    // Should we cache this particular response code?
    // TODO: cache response code 300 HTTP_MULT_CHOICE ?
    if (mResponseCode != HTTP_OK && mResponseCode != HTTP_NOT_AUTHORITATIVE &&
            mResponseCode != HTTP_PARTIAL && mResponseCode != HTTP_MOVED_PERM &&
            mResponseCode != HTTP_GONE) {
        return NOERROR;
    }
    // Offer this request to the cache.
    //cacheRequest =
    ECode ec = NOERROR;
    ec = mResponseCache->Put(mUri, (IURLConnection*)this, (ICacheRequest**)&mCacheRequest);
    return ec;
}


void CHttpURLConnectionImpl::DiscardIntermediateResponse()
{
}

ECode CHttpURLConnectionImpl::RetrieveResponse()
{
    if (mResponseHeader != NULL) {
        return NOERROR;
    }

    mRedirectionCount = 0;
    while (TRUE) {
        MakeConnection();

        // if we can get a response from the cache, we're done
        if (mCacheResponse != NULL) {
            // TODO: how does this interact with redirects? Consider processing the headers so
            // that a redirect is never returned.
            return NOERROR;
        }

        if (!mSentRequestHeaders) {
            WriteRequestHeaders(mSocketOut);
        }

        if (mRequestBodyOut != NULL) {
            mRequestBodyOut->Close();
//            if (requestBodyOut instanceof RetryableOutputStream) {
//                ((RetryableOutputStream) requestBodyOut).writeToSocket(socketOut);
//            }
        }

        mSocketOut->Flush();

        ReadResponseHeaders();

        if (HasResponseBody()) {
            MaybeCache(); // reentrant. this calls into user code which may call back into this!
        }

        InitContentStream();

//        Retry retry = ProcessResponseHeaders();
//
//        if (retry == Retry.NONE) {
//            return;
//        }

        /*
         * The first request wasn't sufficient. Prepare for another...
         */

        if (mRequestBodyOut != NULL) {
            return E_HTTP_RETRY_EXCEPTION;
        }

//        if (retry == Retry.SAME_CONNECTION && hasConnectionCloseHeader()) {
//            retry = Retry.NEW_CONNECTION;
//        }

        DiscardIntermediateResponse();

//        if (retry == Retry.NEW_CONNECTION) {
//            releaseSocket(true);
//        }
    }
}


AutoPtr<IInputStream> CHttpURLConnectionImpl::InitContentStream()
{
    return NULL;
}

ECode CHttpURLConnectionImpl::WriteRequestHeaders(
    /* [in] */ IOutputStream* out)
{
    Header* header;
    PrepareRequestHeaders(&header);

    StringBuffer result;
    result += header->GetStatusLine();
    result += String("\r\n");

    for (Int32 i = 0; i < header->Length(); i++) {
        String key = header->GetKey(i);
        String value = header->Get(i);
        if (!key.IsNull()) {
            result += key;
            result += String(": ");
            result += value;
            result += String("\r\n");
        }
    }

    result += String("\r\n");

    ArrayOf<Byte>* bytes;
//    result.ToString().GetBytes(&bytes);
    out->WriteBuffer(*bytes);
    mSentRequestHeaders = TRUE;
    return NOERROR;
}

ECode CHttpURLConnectionImpl::ReadResponseHeaders()
{
    do {
        mResponseCode = -1;
        mResponseMessage = NULL;
        mResponseHeader = new Header();
        mResponseHeader->SetStatusLine(ReadLine(mSocketIn));
        ReadHeaders();
    } while (ParseResponseCode() == HTTP_CONTINUE);
}

Boolean CHttpURLConnectionImpl::HasResponseBody()
{
    if (mMethod != HEAD
            && mMethod != CONNECT
            && (mResponseCode < HTTP_CONTINUE || mResponseCode >= 200)
            && mResponseCode != HTTP_NO_CONTENT
            && mResponseCode != HTTP_NOT_MODIFIED) {
        return TRUE;
    }

    /*
     * If the Content-Length or Transfer-Encoding headers disagree with the
     * response code, the response is malformed. For best compatibility, we
     * honor the headers.
     */
    String contentLength = mResponseHeader->Get(String("Content-Length"));
    if (!contentLength.IsNull() && contentLength.ToInt32() > 0) {
        return TRUE;
    }
    if (String("chunked").EqualsIgnoreCase(mResponseHeader->Get(String("Transfer-Encoding")))) {
        return TRUE;
    }

    return FALSE;
}

String CHttpURLConnectionImpl::ReadLine(
    /* [in] */ IInputStream *is)
{
    StringBuffer result;;
    while (TRUE) {
        Int32 c;
        is->Read(&c);
         if (c == -1 || c == '\n') {
            break;
        }

        result += (char)c;
    }

    Int32 length = result.GetLength();

    if (length > 0 && result.GetChar(length - 1) == '\r') {
        result.SetLength(length - 1);
    }

    return result.ToString();
}

void CHttpURLConnectionImpl::ReadHeaders()
{
    String line;
    while ((line = ReadLine(mSocketIn)).GetLength() > 1) {
        // Header parsing
        Int32 index = line.IndexOf(":");
        if (index == -1) {
            mResponseHeader->Add(String(""), line);
        } else {
            mResponseHeader->Add(line.Substring(0, index), line.Substring(index + 1));
        }
    }

//    CookieHandler cookieHandler = CookieHandler.getDefault();
//    if (cookieHandler != null) {
//        cookieHandler.put(uri, responseHeader.getFieldMap());
//    }
}

Int32 CHttpURLConnectionImpl::ParseResponseCode()
{
    String response = mResponseHeader->GetStatusLine();
    if (response.IsNull() || !response.StartWith(String("HTTP/"))) {
        return -1;
    }

//    response = response.trim();
    Int32 mark = response.IndexOf(" ") + 1;
    if (mark == 0) {
        return -1;
    }
    if (response.GetChar(mark - 2) != '1') {
        mHttpVersion = 0;
    }

    Int32 last = mark + 3;
    if (last > response.GetLength()) {
        last = response.GetLength();
    }
    //responseCode = Integer.parseInt(response.substring(mark, last));
    mResponseCode = response.Substring(mark, last-mark).ToInt32();
    if (last + 1 <= response.GetLength()) {
        mResponseMessage = response.Substring(last + 1);
    }
    return mResponseCode;
}

String CHttpURLConnectionImpl::RequestString()
{
    Boolean useProxy;
    UsingProxy(&useProxy);
    if (useProxy) {
        String urlstr;
        mUrl->ToString(&urlstr);
        return urlstr;
    }

    String file;
    mUrl->GetFile(&file);
    if (file.IsNull() || file.GetLength() == 0) {
        file = String("/");
    }

    return file;
}


ECode CHttpURLConnectionImpl::GetTransferStream(
    /* [out] */ IInputStream** ppIs)
{
//    if (!HasResponseBody()) {
//        return new FixedLengthInputStream(socketIn, cacheRequest, this, 0);
//    }
//
//    if ("chunked".equalsIgnoreCase(responseHeader.get("Transfer-Encoding"))) {
//        return new ChunkedInputStream(socketIn, cacheRequest, this);
//    }
//
//    String contentLength = responseHeader.get("Content-Length");
//    if (contentLength != null) {
//        try {
//            int length = Integer.parseInt(contentLength);
//            return new FixedLengthInputStream(socketIn, cacheRequest, this, length);
//        } catch (NumberFormatException ignored) {
//        }
//    }
//
//    /*
//     * Wrap the input stream from the HttpConnection (rather than
//     * just returning "socketIn" directly here), so that we can control
//     * its use after the reference escapes.
//     */
//    return new UnknownLengthHttpInputStream(socketIn, cacheRequest, this);
}

ECode CHttpURLConnectionImpl::PrepareRequestHeaders(
    /* [out] */ Header** header)
{
    if (mMethod == CONNECT) {
        Header* proxyHeader = new Header();
        proxyHeader->SetStatusLine(GetStatusLine());

    // always set Host and User-Agent
        String host = mRequestHeader->Get(String("Host"));
        if (host.IsNull()) {
            host = GetOriginAddress(mUrl);
        }

        proxyHeader->Set(String("Host"), host);

        String userAgent = mRequestHeader->Get(String("User-Agent"));
        if (userAgent.IsNull()) {
            userAgent = GetDefaultUserAgent();
        }

        proxyHeader->Set(String("User-Agent"), userAgent);

    // copy over the Proxy-Authorization header if it exists
        String proxyAuthorization = mRequestHeader->Get(String("Proxy-Authorization"));
        if (!proxyAuthorization.IsNull()) {
            proxyHeader->Set(String("Proxy-Authorization"), proxyAuthorization);
        }

    // Always set the Proxy-Connection to Keep-Alive for the benefit of
    // HTTP/1.0 proxies like Squid.
        proxyHeader->Set(String("Proxy-Connection"), String("Keep-Alive"));
        *header = proxyHeader;
    }
    return NOERROR;
}

String CHttpURLConnectionImpl::GetStatusLine()
{
    StringBuffer result;
    String protocol;
    if (mHttpVersion == 0) {
        protocol = String("HTTP/1.0");
    } else {
        protocol = String("HTTP/1.1");
    }

    result += mMethod;
    result += String(" ");
    result += RequestString();
    result += String(" ");
    result += protocol;

    return result.ToString();
}

String CHttpURLConnectionImpl::GetOriginAddress(
    /* [in] */ IURL* url)
{
    Int32 port;
    url->GetPort(&port);

    String result;
    url->GetHost(&result);

    StringBuffer buff;
    if (port > 0 && port != mDefaultPort) {
        buff += result;
        buff += String(":");
        buff += port;
    }

    return buff.ToString();
}

String CHttpURLConnectionImpl::GetDefaultUserAgent()
{
    String agent = GetSystemProperty(String("http.agent"));
    StringBuffer buf;
    if (!agent.IsNull()) {
        return agent;
    } else {
        buf += String("Java");
        buf += GetSystemProperty(String("java.version"));
        return buf.ToString();
    }
}

String CHttpURLConnectionImpl::GetSystemProperty(
    /* [in] */ const String& property)
{
    return String("");
}


Int32 CHttpURLConnectionImpl::GetConnectToPort()
{
    Boolean useproxy;
    Int32 port;
    UsingProxy(&useproxy);
    if (useproxy) {
        AutoPtr<IInetSocketAddress> addr;
        mProxy->GetAddress((ISocketAddress**)&addr);
        addr->GetPort(&port);
    } else {
        mUrl->GetPort(&port);
    }

    if (port < 0) {
        return mDefaultPort;
    } else {
        return port;
    }
}

ECode CHttpURLConnectionImpl::GetConnectToInetAddress(
    /* [out] */ IInetAddress** addr)
{
    Boolean useproxy;
    UsingProxy(&useproxy);
    if (useproxy) {
        AutoPtr<IInetSocketAddress> saddr;
        mProxy->GetAddress((ISocketAddress**)&saddr);
        saddr->GetAddress(addr);
    } else {
        assert(0);
    }
    return NOERROR;
}

String CHttpURLConnectionImpl::GetConnectToHost()
{
    Boolean useproxy;
    UsingProxy(&useproxy);
    String hostname;
    if (useproxy) {
        AutoPtr<IInetSocketAddress> saddr;
        mProxy->GetAddress((ISocketAddress**)&saddr);
        saddr->GetHostName(&hostname);
    }
    else {
        mUrl->GetHost(&hostname);
    }
    return hostname;
}

ECode CHttpURLConnectionImpl::ProcessResponseHeaders(
    /* [out] */ Retry* retry)
{
    switch (mResponseCode) {
        case HTTP_PROXY_AUTH: // proxy authorization failed ?
            Boolean us;
            UsingProxy(&us);
            if (!us) {
                return E_IO_EXCEPTION;
            }
            return ProcessAuthHeader(String("Proxy-Authenticate"), String("Proxy-Authorization"), retry);

        case HTTP_UNAUTHORIZED: // HTTP authorization failed ?
            return ProcessAuthHeader(String("WWW-Authenticate"), String("Authorization"), retry);

        case HTTP_MULT_CHOICE:
        case HTTP_MOVED_PERM:
        case HTTP_MOVED_TEMP:
        case HTTP_SEE_OTHER:
        case HTTP_USE_PROXY:
        {
            Boolean isIn;
            GetInstanceFollowRedirects(&isIn);
            if (!isIn) {
                *retry = Retry_NONE;
                return NOERROR;
            }
            if (mRequestBodyOut != NULL) {
                // TODO: follow redirects for retryable output streams...
                *retry = Retry_NONE;
                return NOERROR;
            }
            if (++mRedirectionCount > MAX_REDIRECTS) {
                return E_PROTOCOL_EXCEPTION;
            }
            String location;
            GetHeaderFieldByKey(String("Location"), &location);
            if (location.IsNull()) {
                *retry = Retry_NONE;
                return NOERROR;
            }
            if (mResponseCode == HTTP_USE_PROXY) {
                Int32 start = 0;
                String protocol;
                mUrl->GetProtocol(&protocol);
                StringBuffer re;
                re += protocol;
                re += ':';
                if (location.StartWith(re.ToString())) {
                    start = protocol.GetLength() + 1;
                }
                if (location.StartWith("//")) {
                    start += 2;
                }
                SetProxy(location.Substring(start));
                *retry = Retry_NEW_CONNECTION;
                return NOERROR;
            }
            AutoPtr<IURL> previousUrl = mUrl;
            CURL::New(previousUrl, location, (IURL**)&mUrl);
            String pro;
            previousUrl->GetProtocol(&pro);
            String pro1;
            mUrl->GetProtocol(&pro1);
            if (!pro.Equals(pro1)) {
                *retry = Retry_NONE; // the scheme changed; don't retry.
                return NOERROR;
            }
            String host;
            previousUrl->GetHost(&host);
            String host1;
            mUrl->GetHost(&host1);
            Int32 effport;
            Int32 effport1;
            previousUrl->GetEffectivePort(&effport);
            mUrl->GetEffectivePort(&effport1);
            if (host.Equals(host1) && effport == effport1) {
                *retry = Retry_SAME_CONNECTION;
                return NOERROR;
            } else {
                // TODO: strip cookies?
                mRequestHeader->RemoveAll(String("Host"));
                *retry = Retry_NEW_CONNECTION;
                return NOERROR;
            }
            break;
        }
        default:
        {
            *retry = Retry_NONE;
            return NOERROR;
        }
    }
}

ECode CHttpURLConnectionImpl::ProcessAuthHeader(
    /* [in] */ const String& responseHeader,
    /* [in] */ const String& retryHeader,
    /* [out] */ Retry* retry)
{
    String challenge = mResponseHeader->Get(responseHeader);
    if (challenge.IsNull()) {
        return E_IO_EXCEPTION;
    }

    String credentials;
    ECode ec = NOERROR;
    ec = GetAuthorizationCredentials(challenge, &credentials);
    if (FAILED(ec)) {
        return ec;
    }

    if (credentials.IsNull()) {
        *retry = Retry_NONE;
        return NOERROR; // could not find credentials, end request cycle
    }
    // add authorization credentials, bypassing the already-connected check
    mRequestHeader->Set(retryHeader, credentials);
    *retry = Retry_SAME_CONNECTION;
    return NOERROR;
}

void CHttpURLConnectionImpl::SetProxy(
    /* [in] */ const String& proxy)
{
    Int32 colon = proxy.IndexOf(':');
    String host;
    Int32 port;
    if (colon != -1) {
        host = proxy.Substring(0, colon);
        String strport;
        strport = proxy.Substring(colon + 1);
        port = strport.ToInt32();
    } else {
        host = proxy;
        port = mDefaultPort;
    }
    AutoPtr<IInetSocketAddress> addr;
    CInetSocketAddress::New(host, port, (IInetSocketAddress**)&addr);
    CProxy::New(0, addr, (IProxy**)&mProxy);
}


ECode CHttpURLConnectionImpl::GetAuthorizationCredentials(
        /* [in] */ const String& challenge,
        /* [out] */ String* credential)
{
}