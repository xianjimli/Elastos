#ifndef __LOADLISTENER_H__
#define __LOADLISTENER_H__

class LoadListener extends Handler implements EventHandler {


public long mContentLength; // Content length of the incoming data

public:
    // =========================================================================
    // Public functions
    // =========================================================================

	static CARAPI_(LoadListener) GetLoadListener(
		/* [in] */ Context context,
		/* [in] */ BrowserFrame frame, 
		/* [in] */ CString url, 
		/* [in] */ Int32 nativeLoader,
		/* [in] */ Boolean synchronous, 
		/* [in] */ Boolean isMainPageLoader,
		/* [in] */ Boolean isMainResource, 
		/* [in] */ Boolean userGesture, 
		/* [in] */ Int64 postIdentifier,
		/* [in] */ String username, 
		/* [in] */ String password);

	static CARAPI_(Int32) GetNativeLoaderCount();

    LoadListener(
    	/* [in] */ Context context, 
    	/* [in] */ BrowserFrame frame, 
    	/* [in] */ CString url,
    	/* [in] */ Int32 nativeLoader, 
    	/* [in] */ Boolean synchronous, 
    	/* [in] */ Boolean isMainPageLoader,
    	/* [in] */ Boolean isMainResource, 
    	/* [in] */ Boolean userGesture, 
    	/* [in] */ Int64 postIdentifier,
    	/* [in] */ CString username, 
    	/* [in] */ String password);


    /*
     * This message handler is to facilitate communication between the network
     * thread and the browser thread.
     */
	virtual CARAPI_(void) HandleMessage(
		/* [in] */ Message msg);

    /**
     * @return The loader's BrowserFrame.
     */
    virtual CARAPI_(BrowserFrame) GetFrame();

    virtual CARAPI_(Context) GetContext();

    /* package */ 
    virtual CARAPI_(Boolean) IsSynchronous();

    /**
     * @return True iff the load has been cancelled
     */
	virtual CARAPI_(Boolean) Cancelled();

    /**
     * Parse the headers sent from the server.
     * @param headers gives up the HeaderGroup
     * IMPORTANT: as this is called from network thread, can't call native
     * directly
     */
	virtual CARAPI_(void) Headers(
		/* [in] */ Headers headers);



    /**
     * @return True iff this loader is in the proxy-authenticate state.
     */
    virtual CARAPI_(Boolean) ProxyAuthenticate();

    /**
     * Report the status of the response.
     * TODO: Comments about each parameter.
     * IMPORTANT: as this is called from network thread, can't call native
     * directly
     */
	virtual CARAPI_(void) Status(
		/* [in] */ Int32 majorVersion, 
		/* [in] */ Int32 minorVersion,
		/* [in] */ Int32 code, /* Status-Code value */ 
		/* [in] */ CString reasonPhrase);



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
	virtual CARAPI_(void) Certificate(
		/* [in] */ SslCertificate certificate);

    /**
     * Implementation of error handler for EventHandler.
     * Subclasses should call this method to have error fields set.
     * @param id The error id described by EventHandler.
     * @param description A string description of the error.
     * IMPORTANT: as this is called from network thread, can't call native
     * directly
     */
	virtual CARAPI_(void) Error(
		/* [in] */ int id, 
		/* [in] */ CString description);

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
	virtual CARAPI_(void) Data(
		/* [in] */ ArrayOf<Byte> data, 
		/* [in] */ Int32 length);

    /**
     * Event handler's endData call. Send a message to the handler notifying
     * them that the data has finished.
     * IMPORTANT: as this is called from network thread, can't call native
     * directly
     */
	virtual CARAPI_(void) EndData();

    /* This method is called from CacheLoader when the initial request is
     * serviced by the Cache. */
    /* package */ 
    virtual CARAPI_(void) SetCacheLoader(
    	/* [in] */ CacheLoader c);

    /**
     * Check the cache for the current URL, and load it if it is valid.
     *
     * @param headers for the request
     * @return true if cached response is used.
     */
    virtual CARAPI_(Boolean) CheckCache(
    	/* [in] */ Map<String, String> headers);

    /**
     * SSL certificate error callback. Handles SSL error(s) on the way up
     * to the user.
     * IMPORTANT: as this is called from network thread, can't call native
     * directly
     */
	virtual CARAPI_(Boolean) HandleSslErrorRequest(
		/* [in] */ SslError error);

    /**
     * @return HTTP authentication realm or null if none.
     */
    virtual CARAPI_(CString) Realm();

    /**
     * Returns true iff an HTTP authentication problem has
     * occured (credentials invalid).
     */
    virtual CARAPI_(Boolean) AuthCredentialsInvalid();

    /**
     * @return The last SSL error or null if there is none
     */
    virtual CARAPI_(SslError) SslError();

    /**
     * Handles SSL error(s) on the way down from the user
     * (the user has already provided their feedback).
     */
    virtual CARAPI_(void) HandleSslErrorResponse(
    	/* [in] */ Boolean proceed);

    /**
     * Uses user-supplied credentials to restart a request. If the credentials
     * are null, cancel the request.
     */
    virtual CARAPI_(void) HandleAuthResponse(
    	/* [in] */ CString username, 
    	/* [in] */ CString password);

    virtual CARAPI_(void) MakeAuthResponse(
    	/* [in] */ CString username, 
    	/* [in] */ CString password);

    /**
     * This is called when a request can be satisfied by the cache, however,
     * the cache result could be a redirect. In this case we need to issue
     * the network request.
     * @param method
     * @param headers
     * @param postData
     */
    virtual CARAPI_(void) SetRequestData(
    	/* [in] */ String method, 
    	/* [in] */ Map<String, String> headers, 
    	/* [in] */ ArrayOf<Byte> postData);

    /**
     * @return The current URL associated with this load.
     */
    virtual CARAPI_(CString) Url();

    /**
     * @return The current WebAddress associated with this load.
     */
    virtual CARAPI_(WebAddress) GetWebAddress();

    /**
     * @return URL hostname (current URL).
     */
    virtual CARAPI_(String) Host();

    /**
     * @return The original URL associated with this load.
     */
    virtual CARAPI_(String) OriginalUrl();

    virtual CARAPI_(Int64) PostIdentifier();

    virtual CARAPI_(void) AttachRequestHandle(
    	/* [in] */ RequestHandle requestHandle);

    /*
     * This function is called from native WebCore code to
     * notify this LoadListener that the content it is currently
     * downloading should be saved to a file and not sent to
     * WebCore.
     */
    virtual CARAPI_(void) DownloadFile();
    
    /*
     * This function is called from native WebCore code to
     * find out if the given URL is in the cache, and if it can
     * be used. This is just for forward/back navigation to a POST
     * URL.
     */
    static CARAPI_(Boolean) WillLoadFromCache(
    	/* [in] */ String url, 
    	/* [in] */ Int64 identifier);

    /*
     * Reset the cancel flag. This is used when we are resuming a stopped
     * download. To suspend a download, we cancel it. It can also be cancelled
     * when it has run out of disk space. In this situation, the download
     * can be resumed.
     */
    virtual CARAPI_(void) ResetCancel();

    virtual CARAPI_(String) MimeType();

    virtual CARAPI_(String) TransferEncoding();

    /*
     * Return the size of the content being downloaded. This represents the
     * full content size, even under the situation where the download has been
     * resumed after interruption.
     *
     * @ return full content size
     */
    virtual CARAPI_(Int64) ContentLength();

        /**
     * Tear down the load. Subclasses should clean up any mess because of
     * cancellation or errors during the load.
     */
    virtual CARAPI_(void) TearDown();

        /**
     * Notify the loader we encountered an error.
     */
    virtual CARAPI_(void) NotifyError();

    /**
     * Pause the load. For example, if a plugin is unable to accept more data,
     * we pause reading from the request. Called directly from the WebCore thread.
     */
    virtual CARAPI_(void) PauseLoad(
    	/* [in] */ Boolean pause);

    
    /**
     * Sets the current URL associated with this load.
     */
    virtual CARAPI_(void) SetUrl(
    	/* [in] */ CString url);

   /**
     * Cycle through our messages for synchronous loads.
     */
    /* package */ 
	virtual CARAPI_(void) LoadSynchronousMessages();

    /* package */ 
    virtual CARAPI_(void) ParseContentTypeHeader(
    	/* [in] */ CString contentType);

private:

	// Handle the status callback on the WebCore thread.
    CARAPI_(void) HandleStatus(
		/* [in] */ Int32 major, 
		/* [in] */ Int32 minor, 
		/* [in] */ Int32 code, 
		/* [in] */ CString reason);

    // Handle the certificate on the WebCore thread.
    CARAPI_(void) HandleCertificate(
		/* [in] */ SslCertificate certificate);

    // Handle the error on the WebCore thread.
    CARAPI_(void) HandleError(
		/* [in] */ Int32 id, 
		/* [in] */ CString description);

    // Handle the end of data.
    CARAPI_(void) HandleEndData();

    // Handle the ssl error on the WebCore thread.
    CARAPI_(void) HandleSslError(
		/* [in] */ SslError error);

	// Does the header parsing work on the WebCore thread.
    CARAPI_(void) HandleHeaders(
		/* [in] */ Headers headers);

    // Commit the headers if the status code is not a redirect.
    CARAPI_(void) CommitHeadersCheckRedirect();

    // This commits the headers without checking the response status code.
    CARAPI_(void) CommitHeaders();

    /**
     * Create a WebCore response object so that it can be used by
     * nativeReceivedResponse or nativeRedirectedToUrl
     * @return native response pointer
     */
    CARAPI_(Int32) CreateNativeResponse();

    /**
     * Commit the load.  It should be ok to call repeatedly but only before
     * tearDown is called.
     */
    CARAPI_(void) CommitLoad();

    /**
     * Helper for getting the error ID.
     * @return errorID.
     */
    CARAPI_(Int32) GetErrorID();

    /**
     * Return the error description.
     * @return errorDescription.
     */
    CARAPI_(CString) GetErrorDescription();

    /**
     * Cancel a request.
     * FIXME: This will only work if the request has yet to be handled. This
     * is in no way guarenteed if requests are served in a separate thread.
     * It also causes major problems if cancel is called during an
     * EventHandler's method call.
     */
    CARAPI_(void) Cancel();

    /*
     * Perform the actual redirection. This involves setting up the new URL,
     * informing WebCore and then telling the Network to start loading again.
     */
    CARAPI_(void) DoRedirect();

    /**
     * @return The HTTP-authentication object or null if there
     * is no supported scheme in the header.
     * If there are several valid schemes present, we pick the
     * strongest one. If there are several schemes of the same
     * strength, we pick the one that comes first.
     */
    CARAPI_(HttpAuthHeader) ParseAuthHeader(
		/* [in] */ CString header);

    /**
     * If the content is a redirect or not modified we should not send
     * any data into WebCore as that will cause it create a document with
     * the data, then when we try to provide the real content, it will assert.
     *
     * @return True iff the callback should be ignored.
     */
    CARAPI_(Boolean) IgnoreCallbacks();


	/**
     * We keep a count of refs to the nativeLoader so we do not create
     * so many LoadListeners that the GREFs blow up
     */
    CARAPI_(void) ClearNativeLoader();


    /**
     * Guesses MIME type if one was not specified. Defaults to 'text/html'. In
     * addition, tries to guess the MIME type based on the extension.
     *
     */
    CARAPI_(void) GuessMimeType();

    /**
     * guess MIME type based on the file extension.
     */
    CARAPI_(CString) GuessMimeTypeFromExtension(
		/* [in] */ CString url);

    /**
     * Either send a message to ourselves or queue the message if this is a
     * synchronous load.
     */
    CARAPI_(void) SendMessageInternal(
		/* [in] */ Message msg);

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
    CARAPI_(Int32) NativeCreateResponse(
    	/* [in] */ CString url, 
    	/* [in] */ Int32 statusCode,
    	/* [in] */ CString statusText, 
    	/* [in] */ String mimeType, 
    	/* [in] */ Int64 expectedLength,
    	/* [in] */ CString encoding);

    /**
     * Add a response header to the native object.
     * @param nativeResponse The native pointer.
     * @param key String key.
     * @param val String value.
     */
    CARAPI_(void) NativeSetResponseHeader(
		/* [in] */ Int32 nativeResponse, 
		/* [in] */ CString key,
		/* [in] */ CString val);

    /**
     * Dispatch the response.
     * @param nativeResponse The native pointer.
     */
    CARAPI_(void) NativeReceivedResponse(
		/* [in] */ Int32 nativeResponse);

    /**
     * Add data to the loader.
     * @param data Byte array of data.
     * @param length Number of objects in data.
     */
    CARAPI_(void) NativeAddData(
		/* [in] */ ArrayOf<Byte> data, 
		/* [in] */ Int32 length);

    /**
     * Tell the loader it has finished.
     */
    CARAPI_(void) NativeFinished();

    /**
     * tell the loader to redirect
     * @param baseUrl The base url.
     * @param redirectTo The url to redirect to.
     * @param nativeResponse The native pointer.
     * @return The new url that the resource redirected to.
     */
    CARAPI_(CString) NativeRedirectedToUrl(
		/* [in] */ CString baseUrl,
		/* [in] */ CString redirectTo, 
		/* [in] */ Int32 nativeResponse);

    /**
     * Tell the loader there is error
     * @param id
     * @param desc
     * @param failingUrl The url that failed.
     */
    CARAPI_(void) NativeError(
		/* [in] */ Int32 id, 
		/* [in] */ CString desc, 
		/* [in] */ String failingUrl);

private:

	// This is the same regex that DOMImplementation uses to check for xml
    // content. Use this to check if another Activity wants to handle the
    // content before giving it to webkit.
	static final String XML_MIME_TYPE =
            "^[\\w_\\-+~!$\\^{}|.%'`#&*]+/" +
            "[\\w_\\-+~!$\\^{}|.%'`#&*]+\\+xml$";

	/**
     * Parses the content-type header.
     * The first part only allows '-' if it follows x or X.
     */
	static const Pattern CONTENT_TYPE_PATTERN =
            Pattern.compile("^((?:[xX]-)?[a-zA-Z\\*]+/[\\w\\+\\*-]+[\\.[\\w\\+-]+]*)$");

    // This count is transferred from RequestHandle to LoadListener when
    // loading from the cache so that we can detect redirect loops that switch
    // between the network and the cache.
	Int32 mCacheRedirectCount;

	static const CString LOGTAG = "webkit";

    // Messages used internally to communicate state between the
    // Network thread and the WebCore thread.
	static const Int32 MSG_CONTENT_HEADERS = 100;
	static const Int32 MSG_CONTENT_DATA = 110;
	static const Int32 MSG_CONTENT_FINISHED = 120;
	static const Int32 MSG_CONTENT_ERROR = 130;
	static const Int32 MSG_LOCATION_CHANGED = 140;
	static const Int32 MSG_LOCATION_CHANGED_REQUEST = 150;
	static const Int32 MSG_STATUS = 160;
	static const Int32 MSG_SSL_CERTIFICATE = 170;
	static const Int32 MSG_SSL_ERROR = 180;

    // Standard HTTP status codes in a more representative format
	static const Int32 HTTP_OK = 200;
	static const Int32 HTTP_PARTIAL_CONTENT = 206;
	static const Int32 HTTP_MOVED_PERMANENTLY = 301;
	static const Int32 HTTP_FOUND = 302;
	static const Int32 HTTP_SEE_OTHER = 303;
	static const Int32 HTTP_NOT_MODIFIED = 304;
	static const Int32 HTTP_TEMPORARY_REDIRECT = 307;
	static const Int32 HTTP_AUTH = 401;
	static const Int32 HTTP_NOT_FOUND = 404;
	static const Int32 HTTP_PROXY_AUTH = 407;

	static HashMap<String, String> sCertificateTypeMap;
    /*static {
        sCertificateTypeMap = new HashMap<String, String>();
        sCertificateTypeMap.put("application/x-x509-ca-cert", CertTool.CERT);
        sCertificateTypeMap.put("application/x-x509-user-cert", CertTool.CERT);
        sCertificateTypeMap.put("application/x-pkcs12", CertTool.PKCS12);
    }*/

	static Int32 sNativeLoaderCount;

	const ByteArrayBuilder mDataBuilder;

	CString   mUrl;
	WebAddress mUri;
	Boolean  mPermanent;
	CString   mOriginalUrl;
	Context  mContext;
	BrowserFrame mBrowserFrame;
	Int32      mNativeLoader;
	CString   mMimeType;
	CString   mEncoding;
	CString   mTransferEncoding;
	Int32      mStatusCode;
	CString   mStatusText;

	Boolean  mCancelled;  // The request has been cancelled.
	Boolean  mAuthFailed;  // indicates that the prev. auth failed
	CacheLoader mCacheLoader;
	Boolean  mFromCache = false;
	HttpAuthHeader mAuthHeader;
	Int32      mErrorID = OK;
	CString   mErrorDescription;
	SslError mSslError;
	RequestHandle mRequestHandle;
	RequestHandle mSslErrorRequestHandle;
	Int64     mPostIdentifier;

    // Request data. It is only valid when we are doing a load from the
    // cache. It is needed if the cache returns a redirect
	CString mMethod;
	Map<String, String> mRequestHeaders;
	ArrayOf<Byte> mPostData;
    // Flag to indicate that this load is synchronous.
	Boolean mSynchronous;
	Vector<Message> mMessageQueue;

    // Does this loader correspond to the main-frame top-level page?
	Boolean mIsMainPageLoader;
    // Does this loader correspond to the main content (as opposed to a supporting resource)
	const Boolean mIsMainResourceLoader;
	const Boolean mUserGesture;

	Headers mHeaders;

	const CString mUsername;
	const CString mPassword;

};

#endif //__LOADLISTENER_H__