
#include "webkit/LoadListener.h"
#include "webkit/CCookieManager.h"
#include "webkit/CCacheManager.h"
#include "webkit/CacheLoader.h"
#include "webkit/Network.h"
#include "webkit/WebViewWorker.h"
#include "webkit/PerfChecker.h"
#include "webkit/CURLUtil.h"
#include "webkit/CBrowserFrame.h"
#include "webkit/CertTool.h"
#include "webkit/CByteArrayBuilder.h"

#include <elastos/HashMap.h>
#include <elastos/AutoFree.h>

// This is the same regex that DOMImplementation uses to check for xml
// content. Use this to check if another Activity wants to handle the
// content before giving it to webkit.
const CString XML_MIME_TYPE = "^[\\w_\\-+~!$\\^{}|.%'`#&*]+/[\\w_\\-+~!$\\^{}|.%'`#&*]+\\+xml$";

const CString LoadListener::LOGTAG = "webkit";

CARAPI_(LoadListener*) LoadListener::GetLoadListener(
	/* [in] */ IContext* context,
	/* [in] */ IBrowserFrame* frame, 
	/* [in] */ const String& url, 
	/* [in] */ Int32 nativeLoader,
	/* [in] */ Boolean synchronous, 
	/* [in] */ Boolean isMainPageLoader,
	/* [in] */ Boolean isMainResource, 
	/* [in] */ Boolean userGesture, 
	/* [in] */ Int64 postIdentifier,
	/* [in] */ const String& username, 
	/* [in] */ const String& password)
{
	assert(context != NULL);
	assert(frame != NULL);

	sNativeLoaderCount += 1;

    return new LoadListener(context, frame, url, nativeLoader, synchronous,
            isMainPageLoader, isMainResource, userGesture, postIdentifier,
            username, password);
}

CARAPI_(Int32) LoadListener::GetNativeLoaderCount()
{
	return sNativeLoaderCount;
}

LoadListener::LoadListener(
	/* [in] */ IContext* context, 
	/* [in] */ IBrowserFrame* frame, 
	/* [in] */ const String& url,
	/* [in] */ Int32 nativeLoader, 
	/* [in] */ Boolean synchronous, 
	/* [in] */ Boolean isMainPageLoader,
	/* [in] */ Boolean isMainResource, 
	/* [in] */ Boolean userGesture, 
	/* [in] */ Int64 postIdentifier,
	/* [in] */ const String& username, 
	/* [in] */ const String& password) : mContext(context), mBrowserFrame(frame),
	                                     mNativeLoader(nativeLoader), mSynchronous(synchronous),
	                                     mIsMainPageLoader(isMainPageLoader), mIsMainResourceLoader(isMainResource),
	                                     mUserGesture(userGesture), mPostIdentifier(postIdentifier),
	                                     mUsername(username), mPassword(password)
{
//	if (DebugFlags.LOAD_LISTENER) {
//        Log.v(LOGTAG, "LoadListener constructor url=" + url);
//    }    
    
    SetUrl(url);
       
    if (synchronous) {
//        mMessageQueue = new Vector<Message>();
    }    
}

/*
 * This message handler is to facilitate communication between the network
 * thread and the browser thread.
 */
CARAPI_(void) LoadListener::HandleMessage(
	/* [in] */ IMessage* msg)
{
	assert(msg != NULL);

	switch (0/*msg.what*/) {
        case MSG_CONTENT_HEADERS:
            /*
             * This message is sent when the LoadListener has headers
             * available. The headers are sent onto WebCore to see what we
             * should do with them.
             */
//            HandleHeaders((Headers) msg.obj);
            break;

        case MSG_CONTENT_DATA:
            /*
             * This message is sent when the LoadListener has data available
             * in it's data buffer. This data buffer could be filled from a
             * file (this thread) or from http (Network thread).
             */
            if (mNativeLoader != 0 && !IgnoreCallbacks()) {
                CommitLoad();
            }
            break;

        case MSG_CONTENT_FINISHED:
            /*
             * This message is sent when the LoadListener knows that the
             * load is finished. This message is not sent in the case of an
             * error.
             *
             */
            HandleEndData();
            break;

        case MSG_CONTENT_ERROR:
            /*
             * This message is sent when a load error has occured. The
             * LoadListener will clean itself up.
             */
//            HandleError(msg.arg1, (String) msg.obj);
            break;

        case MSG_LOCATION_CHANGED:
            /*
             * This message is sent from LoadListener.endData to inform the
             * browser activity that the location of the top level page
             * changed.
             */
            DoRedirect();
            break;

        case MSG_LOCATION_CHANGED_REQUEST:
            /*
             * This message is sent from endData on receipt of a 307
             * Temporary Redirect in response to a POST -- the user must
             * confirm whether to continue loading. If the user says Yes,
             * we simply call MSG_LOCATION_CHANGED. If the user says No,
             * we call MSG_CONTENT_FINISHED.
             */
//            Message contMsg = obtainMessage(MSG_LOCATION_CHANGED);
//            Message stopMsg = obtainMessage(MSG_CONTENT_FINISHED);
//            mBrowserFrame.getCallbackProxy().onFormResubmission(stopMsg, contMsg);
            break;

        case MSG_STATUS:
            /*
             * This message is sent from the network thread when the http
             * stack has received the status response from the server.
             */
//            HashMap status = (HashMap) msg.obj;
//            handleStatus(((Integer) status.get("major")).intValue(),
//                    ((Integer) status.get("minor")).intValue(),
//                    ((Integer) status.get("code")).intValue(),
//                    (String) status.get("reason"));
            break;

        case MSG_SSL_CERTIFICATE:
            /*
             * This message is sent when the network thread receives a ssl
             * certificate.
             */
//            handleCertificate((SslCertificate) msg.obj);
            break;

        case MSG_SSL_ERROR:
            /*
             * This message is sent when the network thread encounters a
             * ssl error.
             */
//            handleSslError((SslError) msg.obj);
            break;
    }
}

/**
 * @return The loader's BrowserFrame.
 */
CARAPI_(AutoPtr<IBrowserFrame>) LoadListener::GetFrame() const
{
	return mBrowserFrame;
}

CARAPI_(AutoPtr<IContext>) LoadListener::GetContext() const
{
	return mContext;
}

/* package */ 
CARAPI_(Boolean) LoadListener::IsSynchronous()
{
	return mSynchronous;
}

/**
 * @return True iff the load has been cancelled
 */
CARAPI_(Boolean) LoadListener::Cancelled()
{
	return mCancelled;
}

/**
 * Parse the headers sent from the server.
 * @param headers gives up the HeaderGroup
 * IMPORTANT: as this is called from network thread, can't call native
 * directly
 */
CARAPI_(void) LoadListener::Headers(
	/* [in] */ IHeaders* headers)
{
	assert(headers != NULL);

	//if (DebugFlags.LOAD_LISTENER) Log.v(LOGTAG, "LoadListener.headers");
    // call db (setCookie) in the non-WebCore thread
    if (mCancelled) {
    	return;
    }

    Vector<String> cookies;// = headers.getSetCookie();
    Int32 size = cookies.GetSize();
    ICookieManager* pCookieManager = NULL;
    CCookieManager::AcquireSingleton(&pCookieManager);
    for (Int32 i = 0; i < size; ++i) {
        pCookieManager->SetCookieEx(mUri, cookies[i]);
    }
//    SendMessageInternal(obtainMessage(MSG_CONTENT_HEADERS, headers));
}

/**
 * @return True iff this loader is in the proxy-authenticate state.
 */
CARAPI_(Boolean) LoadListener::ProxyAuthenticate()
{
//	if (mAuthHeader != NULL) {
//        return mAuthHeader->IsProxy();
//    }

    return FALSE;
}

/**
 * Report the status of the response.
 * TODO: Comments about each parameter.
 * IMPORTANT: as this is called from network thread, can't call native
 * directly
 */
CARAPI_(void) LoadListener::Status(
	/* [in] */ Int32 majorVersion, 
	/* [in] */ Int32 minorVersion,
	/* [in] */ Int32 code, /* Status-Code value */ 
	/* [in] */ const String& reasonPhrase)
{
//	if (DebugFlags.LOAD_LISTENER) {
//        Log.v(LOGTAG, "LoadListener: from: " + mUrl
//                + " major: " + majorVersion
//                + " minor: " + minorVersion
//                + " code: " + code
//                + " reason: " + reasonPhrase);
//    }

	IObjectStringMap* status = NULL;
	CObjectStringMap::New(&status);
	assert(status);

	Int32* _majorVersion  = new Int32(majorVersion);
	Int32* _minorVersion  = new Int32(minorVersion);
	Int32* _code          = new Int32(code);
	String* _reasonPhrase = new String("reason");

	status->Put((String)"major",  (IInterface*)_majorVersion);
	status->Put((String)"minor",  (IInterface*)_minorVersion);
	status->Put((String)"code",   (IInterface*)_code);
	status->Put((String)"reason", (IInterface*)_reasonPhrase);

    // New status means new data. Clear the old.
    mDataBuilder->Clear();
    mMimeType = "";
    mEncoding = "";
    mTransferEncoding = "";

//    sendMessageInternal(obtainMessage(MSG_STATUS, status));
}

/**
 * Implementation of certificate handler for EventHandler. Called
 * before a resource is requested. In this context, can be called
 * multiple times if we have redirects
 *
 * IMPORTANT: as this is called from network thread, can't call
 * native directly
 *
 * @param certificate The SSL certifcate or null if the request
 * was not secure
 */
CARAPI_(void) LoadListener::Certificate(
	/* [in] */ ISslCertificate* certificate)
{}

/**
 * Implementation of error handler for EventHandler.
 * Subclasses should call this method to have error fields set.
 * @param id The error id described by EventHandler.
 * @param description A string description of the error.
 * IMPORTANT: as this is called from network thread, can't call native
 * directly
 */
CARAPI_(void) LoadListener::Error(
	/* [in] */ Int32 id, 
	/* [in] */ const String& description)
{}

/**
 * Add data to the internal collection of data. This function is used by
 * the data: scheme, about: scheme and http/https schemes.
 * @param data A byte array containing the content.
 * @param length The length of data.
 * IMPORTANT: as this is called from network thread, can't call native
 * directly
 * XXX: Unlike the other network thread methods, this method can do the
 * work of decoding the data and appending it to the data builder.
 */
CARAPI_(void) LoadListener::Data(
	/* [in] */ ArrayOf<Byte>& data, 
	/* [in] */ Int32 length)
{
//	if (DebugFlags.LOAD_LISTENER) {
//        Log.v(LOGTAG, "LoadListener.data(): url: " + url());
//    }

    // The reason isEmpty() and append() need to synchronized together is
    // because it is possible for getFirstChunk() to be called multiple
    // times between isEmpty() and append(). This could cause commitLoad()
    // to finish before processing the newly appended data and no message
    // will be sent.
    Boolean sendMessage = FALSE;

    mDataBuilderMutex.Lock();
    {
        mDataBuilder->IsEmpty(&sendMessage);
        mDataBuilder->Append(data, 0, length);
    }
    mDataBuilderMutex.Unlock();

    if (sendMessage) {
        // Send a message whenever data comes in after a write to WebCore
///        sendMessageInternal(obtainMessage(MSG_CONTENT_DATA));
    }
}

/**
 * Event handler's endData call. Send a message to the handler notifying
 * them that the data has finished.
 * IMPORTANT: as this is called from network thread, can't call native
 * directly
 */
CARAPI_(void) LoadListener::EndData()
{}

/* This method is called from CacheLoader when the initial request is
 * serviced by the Cache. */
/* package */ 
CARAPI_(void) LoadListener::SetCacheLoader(
	/* [in] */ CacheLoader* c)
{
	assert(c != NULL);

	mCacheLoader = c;
	mFromCache = TRUE;
}

/**
 * Check the cache for the current URL, and load it if it is valid.
 *
 * @param headers for the request
 * @return true if cached response is used.
 */
CARAPI_(Boolean) LoadListener::CheckCache(
	/* [in] */ IObjectStringMap* headers)
{
	assert(headers != NULL);

	// Get the cache file name for the current URL
	String strUrl;
	Url(strUrl);

    AutoPtr<ICacheManagerCacheResult> result = NULL;
    result = CCacheManager::GetCacheFile(strUrl, mPostIdentifier, headers);

    // Go ahead and set the cache loader to null in case the result is
    // null.
    mCacheLoader = NULL;
    // reset the flag
    mFromCache = FALSE;

    if (result != NULL) {
        // The contents of the cache may need to be revalidated so just
        // remember the cache loader in the case that the server responds
        // positively to the cached content. This is also used to detect if
        // a redirect came from the cache.
        mCacheLoader = new CacheLoader(this, (CCacheManager::CacheResult*)result.Get());

        // If I got a cachedUrl and the revalidation header was not
        // added, then the cached content valid, we should use it.
        IInterface* value1 = NULL;
        IInterface* value2 = NULL;
        headers->Get((String)CCacheManager::HEADER_KEY_IFNONEMATCH, &value1);
        headers->Get((String)CCacheManager::HEADER_KEY_IFMODIFIEDSINCE, &value2);
        if (value1 == NULL && value2 == NULL) {
            
//            if (DebugFlags.LOAD_LISTENER) {
//                Log.v(LOGTAG, "FrameLoader: HTTP URL in cache " +
//                        "and usable: " + url());
//            }

            if (IsSynchronous()) {
                mCacheLoader->Load();
            } else {
                // Load the cached file in a separate thread
//                WebViewWorker.GetHandler().obtainMessage(
//                        WebViewWorker.MSG_ADD_STREAMLOADER, mCacheLoader).sendToTarget();
            }
            mFromCache = TRUE;
            return TRUE;
        }
    }

    return FALSE;
}

/**
 * SSL certificate error callback. Handles SSL error(s) on the way up
 * to the user.
 * IMPORTANT: as this is called from network thread, can't call native
 * directly
 */
CARAPI_(Boolean) LoadListener::HandleSslErrorRequest(
	/* [in] */ ISslError* error)
{
	assert(error != NULL);

//	if (DebugFlags.LOAD_LISTENER) {
//        Log.v(LOGTAG,
//                "LoadListener.handleSslErrorRequest(): url:" + url() +
//                " primary error: " + error.getPrimaryError() +
//                " certificate: " + error.getCertificate());
//    }
    // Check the cached preference table before sending a message. This
    // will prevent waiting for an already available answer.
    if (Network::GetInstance(mContext)->CheckSslPrefTable(this, error)) {
        return TRUE;
    }
    // Do not post a message for a synchronous request. This will cause a
    // deadlock. Just bail on the request.
    if (IsSynchronous()) {
//        mRequestHandle->HandleSslErrorResponse(false);
        return TRUE;
    }
//    sendMessageInternal(obtainMessage(MSG_SSL_ERROR, error));
    // if it has been canceled, return false so that the network thread
    // won't be blocked. If it is not canceled, save the mRequestHandle
    // so that if it is canceled when MSG_SSL_ERROR is handled, we can
    // still call handleSslErrorResponse which will call restartConnection
    // to unblock the network thread.
    if (!mCancelled) {
//        mSslErrorRequestHandle = mRequestHandle;
    }

    return !mCancelled;
}

/**
 * @return HTTP authentication realm or null if none.
 */
CARAPI_(void) LoadListener::Realm(
    /* [out] */ String& strOut)
{}

/**
 * Returns true iff an HTTP authentication problem has
 * occured (credentials invalid).
 */
CARAPI_(Boolean) LoadListener::AuthCredentialsInvalid()
{}

/**
 * @return The last SSL error or null if there is none
 */
CARAPI_(AutoPtr<ISslError>) LoadListener::SslError()
{
	return mSslError;
}

/**
 * Handles SSL error(s) on the way down from the user
 * (the user has already provided their feedback).
 */
CARAPI_(void) LoadListener::HandleSslErrorResponse(
	/* [in] */ Boolean proceed)
{
//	if (mRequestHandle != null) {
//        mRequestHandle.handleSslErrorResponse(proceed);
//    }
  
    if (!proceed) {
        // Commit whatever data we have and tear down the loader.
        CommitLoad();
        TearDown();
    }
}

/**
 * Uses user-supplied credentials to restart a request. If the credentials
 * are null, cancel the request.
 */
CARAPI_(void) LoadListener::HandleAuthResponse(
	/* [in] */ const String& username, 
	/* [in] */ const String& password)
{
//	if (DebugFlags.LOAD_LISTENER) {
//        Log.v(LOGTAG, "LoadListener.handleAuthResponse: url: " + mUrl
//                + " username: " + username
//                + " password: " + password);
//    }
    if (username.GetLength() != 0 && password.GetLength() != 0) {
        MakeAuthResponse(username, password);
    } else {
        // Commit whatever data we have and tear down the loader.
        CommitLoad();
        TearDown();
    }
}

CARAPI_(void) LoadListener::MakeAuthResponse(
	/* [in] */ const String& username, 
	/* [in] */ const String& password)
{}

/**
 * This is called when a request can be satisfied by the cache, however,
 * the cache result could be a redirect. In this case we need to issue
 * the network request.
 * @param method
 * @param headers
 * @param postData
 */
CARAPI_(void) LoadListener::SetRequestData(
	/* [in] */ const String& method, 
	/* [in] */ IObjectStringMap* headers, 
	/* [in] */ ArrayOf<Byte>& postData)
{
	assert(headers != NULL);

	mMethod = method;
    mRequestHeaders = headers;
    mPostData = &postData;
}

/**
 * @return The current URL associated with this load.
 */
CARAPI_(void) LoadListener::Url(
    /* [out] */ String& strOut)
{
	strOut = mUrl;
}

/**
 * @return The current WebAddress associated with this load.
 */
CARAPI_(AutoPtr<IWebAddress>) LoadListener::GetWebAddress()
{
	return mUri;
}

/**
 * @return URL hostname (current URL).
 */
CARAPI_(void) LoadListener::Host(
    /* [out] */ String& strOut)
{
	if (mUri != NULL) {
//        strOut = mUri->mHost;
    }
}

/**
 * @return The original URL associated with this load.
 */
CARAPI_(void) LoadListener::OriginalUrl(
    /* [out] */ String& strOut)
{
	if (mOriginalUrl != NULL) {
        strOut = mOriginalUrl;
    } else {
        strOut = mUrl;
    }
}

CARAPI_(Int64) LoadListener::PostIdentifier()
{
	return mPostIdentifier;
}

CARAPI_(void) LoadListener::AttachRequestHandle(
	/* [in] */ /*RequestHandle* requestHandle*/)
{}

/*
 * This function is called from native WebCore code to
 * notify this LoadListener that the content it is currently
 * downloading should be saved to a file and not sent to
 * WebCore.
 */
CARAPI_(void) LoadListener::DownloadFile()
{}

/*
 * This function is called from native WebCore code to
 * find out if the given URL is in the cache, and if it can
 * be used. This is just for forward/back navigation to a POST
 * URL.
 */
CARAPI_(Boolean) LoadListener::WillLoadFromCache(
	/* [in] */ const String& url, 
	/* [in] */ Int64 identifier)
{
	Boolean inCache =
            CCacheManager::GetCacheFile(url, identifier, NULL) != NULL;
    
//    if (DebugFlags.LOAD_LISTENER) {
//        Log.v(LOGTAG, "willLoadFromCache: " + url + " in cache: " + 
//                inCache);
//    }
    
    return inCache;
}

/*
 * Reset the cancel flag. This is used when we are resuming a stopped
 * download. To suspend a download, we cancel it. It can also be cancelled
 * when it has run out of disk space. In this situation, the download
 * can be resumed.
 */
CARAPI_(void) LoadListener::ResetCancel()
{
	mCancelled = FALSE;
}

CARAPI_(void) LoadListener::MimeType(
    /* [out] */ String& strOut)
{
	strOut = mMimeType;
}

CARAPI_(void) LoadListener::TransferEncoding(
    /* [out] */ String& strOut)
{
	strOut = mTransferEncoding;
}

/*
 * Return the size of the content being downloaded. This represents the
 * full content size, even under the situation where the download has been
 * resumed after interruption.
 *
 * @ return full content size
 */
CARAPI_(Int64) LoadListener::ContentLength() const
{
	return mContentLength;
}

/**
 * Tear down the load. Subclasses should clean up any mess because of
 * cancellation or errors during the load.
 */
CARAPI_(void) LoadListener::TearDown()
{
	if (GetErrorID() == /*OK*/0) {
        WebViewWorker::CacheSaveData* data = new WebViewWorker::CacheSaveData();
        data->mListener = this;
        data->mUrl = mUrl;
        data->mPostId = mPostIdentifier;
//        WebViewWorker.getHandler().obtainMessage(
//                WebViewWorker.MSG_SAVE_CACHE, data).sendToTarget();
    } else {
//        WebViewWorker.getHandler().obtainMessage(
//                WebViewWorker.MSG_REMOVE_CACHE, this).sendToTarget();
    }

    if (mNativeLoader != 0) {
        PerfChecker* checker = new PerfChecker();
        NativeFinished();
        checker->ResponseAlert("res nativeFinished");
        ClearNativeLoader();
    }
}

/**
 * Notify the loader we encountered an error.
 */
CARAPI_(void) LoadListener::NotifyError()
{
    if (mNativeLoader != 0) {
        String description;
        GetErrorDescription(description);

        if (description.GetLength() == 0) {
            description = "";
        }

        String strOut;
        Url(strOut);
        NativeError(GetErrorID(), description, strOut);
        ClearNativeLoader();
    }
}

/**
 * Pause the load. For example, if a plugin is unable to accept more data,
 * we pause reading from the request. Called directly from the WebCore thread.
 */
CARAPI_(void) LoadListener::PauseLoad(
	/* [in] */ Boolean pause)
{}


/**
 * Sets the current URL associated with this load.
 */
CARAPI_(void) LoadListener::SetUrl(
	/* [in] */ const String& url)
{
    if (url.GetLength() != 0) {
        IURLUtil* URL = NULL;
        CURLUtil::AcquireSingleton(&URL);

        assert(URL != NULL);

        mUri = NULL;
        Boolean bFlag = FALSE;
        URL->IsNetworkUrl(url, &bFlag);
        if (bFlag) {
            URL->StripAnchor(url, &mUrl);
//            mUri = new WebAddress(mUrl);
        } else {
            mUrl = url;
        }
    }
}

/**
 * Cycle through our messages for synchronous loads.
 */
/* package */ 
CARAPI_(void) LoadListener::LoadSynchronousMessages()
{}

/* package */ 
CARAPI_(void) LoadListener::ParseContentTypeHeader(
	/* [in] */ const String& contentType)
{
//    if (DebugFlags.LOAD_LISTENER) {
//        Log.v(LOGTAG, "LoadListener.parseContentTypeHeader: " +
//                "contentType: " + contentType);
//    }

    if (contentType.GetLength() != 0) {
        Int32 i = contentType.IndexOf(';');
        if (i >= 0) {
            mMimeType = contentType.Substring(0, i);

            Int32 j = contentType.IndexOf('=', i);
            if (j > 0) {
                i = contentType.IndexOf(';', j);
                if (i < j) {
                    i = contentType.GetLength();
                }
                mEncoding = contentType.Substring(j + 1, i);
            } else {
                mEncoding = contentType.Substring(i + 1);
            }
            // Trim excess whitespace.
            /*mEncoding = */mEncoding.Trim().ToLowerCase();

            if (i < contentType.GetLength() - 1) {
                // for data: uri the mimeType and encoding have
                // the form image/jpeg;base64 or text/plain;charset=utf-8
                // or text/html;charset=utf-8;base64
                contentType.Substring(i + 1).Trim().ToLowerCase();
                mTransferEncoding = contentType;
            }
        } else {
            mMimeType = contentType;
        }

        // Trim leading and trailing whitespace
        mMimeType = mMimeType.Trim();
#if 0
        try {
            Matcher m = CONTENT_TYPE_PATTERN.matcher(mMimeType);
            if (m.find()) {
                mMimeType = m.group(1);
            } else {
                guessMimeType();
            }
        } catch (IllegalStateException ex) {
            guessMimeType();
        }
#endif        
    }
    // Ensure mMimeType is lower case.
    /*mMimeType = */mMimeType.ToLowerCase();
}

// Handle the status callback on the WebCore thread.
CARAPI_(void) LoadListener::HandleStatus(
	/* [in] */ Int32 major, 
	/* [in] */ Int32 minor, 
	/* [in] */ Int32 code, 
	/* [in] */ const String& reason)
{
    if (mCancelled) {
        return;
    }

    mStatusCode = code;
    mStatusText = reason;
    mPermanent = FALSE;
}

// Handle the certificate on the WebCore thread.
CARAPI_(void) LoadListener::HandleCertificate(
	/* [in] */ ISslCertificate* certificate)
{
    assert(certificate != NULL);

    if (mIsMainPageLoader && mIsMainResourceLoader) {
        // update the browser frame with certificate
        ((CBrowserFrame*)mBrowserFrame.Get())->Certificate(certificate);
    }
}

// Handle the error on the WebCore thread.
CARAPI_(void) LoadListener::HandleError(
	/* [in] */ Int32 id, 
	/* [in] */ const String& description)
{
    mErrorID = id;
    mErrorDescription = description;
//    DetachRequestHandle();
    NotifyError();
    TearDown();
}

// Handle the end of data.
CARAPI_(void) LoadListener::HandleEndData()
{}

// Handle the ssl error on the WebCore thread.
CARAPI_(void) LoadListener::HandleSslError(
	/* [in] */ ISslError* error)
{
    assert(error != NULL);

    if (!mCancelled) {
        mSslError = error;
        Network::GetInstance(mContext)->HandleSslErrorRequest(this);
    }
//    else if (mSslErrorRequestHandle != NULL)
//    {
//        mSslErrorRequestHandle.handleSslErrorResponse(true);
//    }
//    mSslErrorRequestHandle = null;
}

// Does the header parsing work on the WebCore thread.
CARAPI_(void) LoadListener::HandleHeaders(
	/* [in] */ IHeaders* headers)
{}

// Commit the headers if the status code is not a redirect.
CARAPI_(void) LoadListener::CommitHeadersCheckRedirect()
{
    if (mCancelled) return;

    // do not call webcore if it is redirect. According to the code in
    // InspectorController::willSendRequest(), the response is only updated
    // when it is not redirect. If we received a not-modified response from
    // the server and mCacheLoader is not null, do not send the response to
    // webkit. This is just a validation response for loading from the
    // cache.
    if ((mStatusCode >= 301 && mStatusCode <= 303) || mStatusCode == 307 ||
            (mStatusCode == 304 && mCacheLoader != NULL)) {
        return;
    }

    CommitHeaders();
}

// This commits the headers without checking the response status code.
CARAPI_(void) LoadListener::CommitHeaders()
{
//    if (mIsMainPageLoader && sCertificateTypeMap.containsKey(mMimeType))
    {
        // In the case of downloading certificate, we will save it to the
        // KeyStore in commitLoad. Do not call webcore.
        return;
    }

    // If the response is an authentication and we've resent the
    // request with some credentials then don't commit the headers
    // of this response; wait for the response to the request with the
    // credentials.
//    if (mAuthHeader != null)
//        return;

    // Commit the headers to WebCore
    int nativeResponse = CreateNativeResponse();
    // The native code deletes the native response object.
    NativeReceivedResponse(nativeResponse);
}

/**
 * Create a WebCore response object so that it can be used by
 * nativeReceivedResponse or nativeRedirectedToUrl
 * @return native response pointer
 */
CARAPI_(Int32) LoadListener::CreateNativeResponse()
{
    // If WebCore sends if-modified-since, mCacheLoader is null. If 
    // CacheManager sends it, mCacheLoader is not null. In this case, if the
    // server responds with a 304, then we treat it like it was a 200 code 
    // and proceed with loading the file from the cache.
    Int32 statusCode = (mStatusCode == HTTP_NOT_MODIFIED &&
            mCacheLoader != NULL) ? HTTP_OK : mStatusCode;
    // pass content-type content-length and content-encoding
    String strUrl;
    OriginalUrl(strUrl);
    const Int32 nativeResponse = NativeCreateResponse(
            strUrl, statusCode, mStatusText,
            mMimeType, mContentLength, mEncoding);
    
//    if (mHeaders != null) {
//        mHeaders.getHeaders(new Headers.HeaderCallback() {
//                public void header(String name, String value) {
//                    nativeSetResponseHeader(nativeResponse, name, value);
//                }
//            });
//    }

    return nativeResponse;
}

/**
 * Commit the load.  It should be ok to call repeatedly but only before
 * tearDown is called.
 */
CARAPI_(void) LoadListener::CommitLoad()
{
    if (mCancelled) return;

    if (mIsMainPageLoader) {
        String* type = NULL;
        sCertificateTypeMap->Get(mMimeType, (IInterface**)&type);
        if (type != NULL) {
            // This must be synchronized so that no more data can be added
            // after getByteSize returns.
            
            Core::Threading::Mutex::Autolock lock(mDataBuilderMutex);

            // In the case of downloading certificate, we will save it
            // to the KeyStore and stop the current loading so that it
            // will not generate a new history page
            Int32 byteSize = 0;
            mDataBuilder->GetByteSize(&byteSize);
            AutoFree<ArrayOf<Byte> > cert = ArrayOf<Byte>::Alloc(byteSize);
            Int32 offset = 0;
            while (TRUE) {
                //ByteArrayBuilder.Chunk c = mDataBuilder.getFirstChunk();
                IByteArrayBuilderChunk* c = NULL;
                mDataBuilder->GetFirstChunk(&c);
                if (c == NULL) break;

//                    if (c.mLength != 0) {
//                        System.arraycopy(c.mArray, 0, cert, offset, c.mLength);
//                        offset += c.mLength;
//                    }
                c->ChunkRelease();
            }
//                CertTool::AddCertificate(mContext.Get(), (const String)(*type), *cert.Get());
            mBrowserFrame->StopLoading();
            return;
        }
    }

    // Give the data to WebKit now. We don't have to synchronize on
    // mDataBuilder here because pulling each chunk removes it from the
    // internal list so it cannot be modified.
    PerfChecker* checker = new PerfChecker();
    AutoPtr<IByteArrayBuilderChunk> ic;
    CByteArrayBuilder::Chunk* c = NULL;
    while (TRUE) {
        mDataBuilder->GetFirstChunk((IByteArrayBuilderChunk**)&ic);
        if (ic == NULL) break;

        c = (CByteArrayBuilder::Chunk*)(ic.Get());

        if (c->mLength != 0) {
            NativeAddData(*c->mArray.Get(), c->mLength);
            WebViewWorker::CacheData* data = new WebViewWorker::CacheData();
            data->mListener = this;
            data->mChunk = c;
//            WebViewWorker.getHandler().obtainMessage(
//                    WebViewWorker.MSG_APPEND_CACHE, data).sendToTarget();
        } else {
            c->ChunkRelease();
        }
        checker->ResponseAlert("res nativeAddData");
    }
}

/**
 * Helper for getting the error ID.
 * @return errorID.
 */
CARAPI_(Int32) LoadListener::GetErrorID() const
{
    return mErrorID;
}

/**
 * Return the error description.
 * @return errorDescription.
 */
CARAPI_(void) LoadListener::GetErrorDescription(
    /* [out] */ String& strOut)
{
    strOut = mErrorDescription;
}

/**
 * Cancel a request.
 * FIXME: This will only work if the request has yet to be handled. This
 * is in no way guarenteed if requests are served in a separate thread.
 * It also causes major problems if cancel is called during an
 * EventHandler's method call.
 */
CARAPI_(void) LoadListener::Cancel()
{}

/*
 * Perform the actual redirection. This involves setting up the new URL,
 * informing WebCore and then telling the Network to start loading again.
 */
CARAPI_(void) LoadListener::DoRedirect()
{
    // as cancel() can cancel the load before doRedirect() is
    // called through handleMessage, needs to check to see if we
    // are canceled before proceed
    if (mCancelled) {
        return;
    }

    // Do the same check for a redirect loop that
    // RequestHandle.setupRedirect does.
//    if (mCacheRedirectCount >= RequestHandle.MAX_REDIRECT_COUNT) {
//        handleError(EventHandler.ERROR_REDIRECT_LOOP, mContext.getString(
//                R.string.httpErrorRedirectLoop));
//        return;
//    }

    String redirectTo;// = mHeaders.getLocation();
    if (redirectTo.GetLength() != 0) {
        Int32 nativeResponse = CreateNativeResponse();
        NativeRedirectedToUrl(mUrl, redirectTo, nativeResponse, redirectTo);
        // nativeRedirectedToUrl() may call cancel(), e.g. when redirect
        // from a https site to a http site, check mCancelled again
        if (mCancelled) {
            return;
        }

        IURLUtil* URL = NULL;
        CURLUtil::AcquireSingleton(&URL);
        Boolean bFlag = FALSE;

        if (redirectTo.GetLength() == 0) {
//            Log.d(LOGTAG, "Redirection failed for "
//                    + mHeaders.getLocation());
            Cancel();
            return;
        } else if (URL->IsNetworkUrl(redirectTo, &bFlag), !bFlag) {
            const String text;// = mContext->GetString(R.string.open_permission_deny) + "\n" + redirectTo;
            ArrayOf<Byte>* textArray = ArrayOf<Byte>::Alloc((Byte*)(const char*)text, text.GetLength());
            NativeAddData(*textArray, text.GetLength());
            NativeFinished();
            ClearNativeLoader();
            return;
        }

        // Cache the redirect response
        if (GetErrorID() == /*OK*/0) {
            WebViewWorker::CacheSaveData* data = new WebViewWorker::CacheSaveData();
            data->mListener = this;
            data->mUrl = mUrl;
            data->mPostId = mPostIdentifier;
//            WebViewWorker.getHandler().obtainMessage(
//                    WebViewWorker.MSG_SAVE_CACHE, data).sendToTarget();
        } else {
//            WebViewWorker.getHandler().obtainMessage(
//                    WebViewWorker.MSG_REMOVE_CACHE, this).sendToTarget();
        }

        // Saving a copy of the unstripped url for the response
        mOriginalUrl = redirectTo;
        // This will strip the anchor
        SetUrl(redirectTo);

        // Redirect may be in the cache
        if (mRequestHeaders == NULL) {
            CObjectStringMap::New((IObjectStringMap**)&mRequestHeaders);
        }
        Boolean fromCache = FALSE;
        if (mCacheLoader != NULL) {
            // This is a redirect from the cache loader. Increment the
            // redirect count to avoid redirect loops.
            mCacheRedirectCount++;
            fromCache = TRUE;
        }
        if (!CheckCache(mRequestHeaders)) {
            // mRequestHandle can be null when the request was satisfied
            // by the cache, and the cache returned a redirect
            if (/*mRequestHandle != null*/1) {
//                mRequestHandle.setupRedirect(mUrl, mStatusCode,
//                        mRequestHeaders);
            } else {
                // If the original request came from the cache, there is no
                // RequestHandle, we have to create a new one through
                // Network.requestURL.
                Network* network = Network::GetInstance(GetContext());
                if (!network->RequestURL(mMethod, mRequestHeaders, *mPostData.Get(), this)) {
                    // Signal a bad url error if we could not load the
                    // redirection.
//                    HandleError(EventHandler.ERROR_BAD_URL, mContext.getString(R.string.httpErrorBadUrl));
                    return;
                }
            }
            if (fromCache) {
                // If we are coming from a cache load, we need to transfer
                // the redirect count to the new (or old) RequestHandle to
                // keep the redirect count in sync.
//                mRequestHandle.setRedirectCount(mCacheRedirectCount);
            }
        } else if (!fromCache) {
            // Switching from network to cache means we need to grab the
            // redirect count from the RequestHandle to keep the count in
            // sync. Add 1 to account for the current redirect.
//            mCacheRedirectCount = mRequestHandle.getRedirectCount() + 1;
        }
    } else {
        CommitHeaders();
        CommitLoad();
        TearDown();
    }

//    if (DebugFlags.LOAD_LISTENER) {
//        Log.v(LOGTAG, "LoadListener.onRedirect(): redirect to: " +
//                redirectTo);
//    }
}

/**
 * @return The HTTP-authentication object or null if there
 * is no supported scheme in the header.
 * If there are several valid schemes present, we pick the
 * strongest one. If there are several schemes of the same
 * strength, we pick the one that comes first.
 */
//    CARAPI_(HttpAuthHeader) ParseAuthHeader(
//		/* [in] */ CString header);

/**
 * If the content is a redirect or not modified we should not send
 * any data into WebCore as that will cause it create a document with
 * the data, then when we try to provide the real content, it will assert.
 *
 * @return True iff the callback should be ignored.
 */
CARAPI_(Boolean) LoadListener::IgnoreCallbacks()
{
    return (mCancelled || /*mAuthHeader != NULL ||*/
                // Allow 305 (Use Proxy) to call through.
                (mStatusCode > 300 && mStatusCode < 400 && mStatusCode != 305));
}

/**
 * We keep a count of refs to the nativeLoader so we do not create
 * so many LoadListeners that the GREFs blow up
 */
CARAPI_(void) LoadListener::ClearNativeLoader()
{
    sNativeLoaderCount -= 1;
    mNativeLoader = 0;
}

/**
 * Guesses MIME type if one was not specified. Defaults to 'text/html'. In
 * addition, tries to guess the MIME type based on the extension.
 *
 */
CARAPI_(void) LoadListener::GuessMimeType()
{
    // Data urls must have a valid mime type or a blank string for the mime
    // type (implying text/plain).
    IURLUtil* URL = NULL;
    CURLUtil::AcquireSingleton(&URL);
    Boolean bFlag = FALSE;
    URL->IsDataUrl(mUrl, &bFlag);
    if (bFlag && mMimeType.GetLength() != 0) {
        Cancel();
        const String text;
//        mContext->GetString(R.string.httpErrorBadUrl, text);
//        HandleError(EventHandler::ERROR_BAD_URL, text);
    } else {
        // Note: This is ok because this is used only for the main content
        // of frames. If no content-type was specified, it is fine to
        // default to text/html.
        mMimeType = "text/html";
        String newMimeType;
        GuessMimeTypeFromExtension(mUrl, newMimeType);
        if (newMimeType.GetLength() != 0) {
            mMimeType = newMimeType;
        }
    }
}

/**
 * guess MIME type based on the file extension.
 */
CARAPI_(void) LoadListener::GuessMimeTypeFromExtension(
	/* [in] */ const String& url,
    /* [out] */ String& strOut)
{}

/**
 * Either send a message to ourselves or queue the message if this is a
 * synchronous load.
 */
CARAPI_(void) LoadListener::SendMessageInternal(
	/* [in] */ IMessage* msg)
{}

//=========================================================================
// native functions
//=========================================================================

/**
 * Create a new native response object.
 * @param url The url of the resource.
 * @param statusCode The HTTP status code.
 * @param statusText The HTTP status text.
 * @param mimeType HTTP content-type.
 * @param expectedLength An estimate of the content length or the length
 *                       given by the server.
 * @param encoding HTTP encoding.
 * @return The native response pointer.
 */
CARAPI_(Int32) LoadListener::NativeCreateResponse(
	/* [in] */ const String& url, 
	/* [in] */ Int32 statusCode,
	/* [in] */ const String& statusText, 
	/* [in] */ const String& mimeType, 
	/* [in] */ Int64 expectedLength,
	/* [in] */ const String& encoding)
{}

/**
 * Add a response header to the native object.
 * @param nativeResponse The native pointer.
 * @param key String key.
 * @param val String value.
 */
CARAPI_(void) LoadListener::NativeSetResponseHeader(
	/* [in] */ Int32 nativeResponse, 
	/* [in] */ const String& key,
	/* [in] */ const String& val)
{}

/**
 * Dispatch the response.
 * @param nativeResponse The native pointer.
 */
CARAPI_(void) LoadListener::NativeReceivedResponse(
	/* [in] */ Int32 nativeResponse)
{}

/**
 * Add data to the loader.
 * @param data Byte array of data.
 * @param length Number of objects in data.
 */
CARAPI_(void) LoadListener::NativeAddData(
	/* [in] */ const ArrayOf<Byte>& data, 
	/* [in] */ Int32 length)
{}

/**
 * Tell the loader it has finished.
 */
CARAPI_(void) LoadListener::NativeFinished()
{}

/**
 * tell the loader to redirect
 * @param baseUrl The base url.
 * @param redirectTo The url to redirect to.
 * @param nativeResponse The native pointer.
 * @return The new url that the resource redirected to.
 */
CARAPI_(void) LoadListener::NativeRedirectedToUrl(
	/* [in] */ const String& baseUrl,
	/* [in] */ const String& redirectTo, 
	/* [in] */ Int32 nativeResponse,
    /* [out] */ String& str)
{}

/**
 * Tell the loader there is error
 * @param id
 * @param desc
 * @param failingUrl The url that failed.
 */
CARAPI_(void) LoadListener::NativeError(
	/* [in] */ Int32 id, 
	/* [in] */ const String& desc, 
	/* [in] */ const String& failingUrl)
{}