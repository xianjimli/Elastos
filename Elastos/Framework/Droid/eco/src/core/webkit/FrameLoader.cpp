
#include "webkit/FrameLoader.h"
#include "webkit/CURLUtil.h"

const char* FrameLoader::HEADER_STR = "text/xml, text/html, application/xhtml+xml, image/png, text/plain, */*;q=0.8";
const char* FrameLoader::CONTENT_TYPE = "content-type";

// Contents of an about:blank page
const char* FrameLoader::mAboutBlank =
        "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EB\"><html><head><title>about:blank</title></head><body></body></html>";

const char* FrameLoader::LOGTAG = "webkit";


FrameLoader::FrameLoader(
	/* [in] */ LoadListener* listener, 
	/* [in] */ const WebSettings* settings,
	/* [in] */ const String& method)
{
	mListener = listener;
    mHeaders = NULL;
    mMethod = method;
    mCacheMode = WebSettings::WS_LOAD_NORMAL;
    mSettings = settings;
}

CARAPI_(void) FrameLoader::SetReferrer(
	/* [in] */ const String& ref)
{
	// only set referrer for http or https
	IURLUtil* pURL = NULL;
	Boolean bFlag = false;
	CURLUtil::AcquireSingleton(&pURL);
	pURL->IsNetworkUrl(ref, &bFlag);

	if (bFlag)
	{
		mReferrer = ref;
	}
}

CARAPI_(void) FrameLoader::SetPostData(
	/* [in] */ Vector<Byte>& postData)
{
	mPostData = postData;
}

CARAPI_(void) FrameLoader::SetContentTypeForPost(
	/* [in] */ const String& postContentType)
{
	mContentType = postContentType;
}

CARAPI_(void) FrameLoader::SetCacheMode(
	/* [in] */ Int32 cacheMode)
{
	mCacheMode = cacheMode;
}

CARAPI_(void) FrameLoader::SetHeaders(
	/* [in] */ /*IHashMap*/IObjectStringMap* headers)
{
	mHeaders = headers;
}

CARAPI_(LoadListener*) FrameLoader::GetLoadListener() const
{
	return mListener;
}

/**
 * Issues the load request.
 *
 * Return value does not indicate if the load was successful or not. It
 * simply indicates that the load request is reasonable.
 *
 * @return true if the load is reasonable.
 */
CARAPI_(Boolean) FrameLoader::ExecuteLoad()
{
#if 0
	String url = mListener->Url();

	IURLUtil* pURL = NULL;
	Boolean bFlag = false;
	CURLUtil::AcquireSingleton(&pURL);
	pURL->IsNetworkUrl(url, &bFlag);

    if (bFlag)
    {
        if (mSettings->GetBlockNetworkLoads())
        {
//            mListener->Error(EventHandler::ERROR_BAD_URL,
//                    mListener.getContext().getString(com.android.internal.R.string.httpErrorBadUrl));
            return false;
        }
        // Make sure the host part of the url is correctly
        // encoded before sending the request
        if (!URLUtil->VerifyURLEncoding(mListener->Host())) {
            mListener->Error(EventHandler.ERROR_BAD_URL,
                    mListener.getContext().getString(
                    com.android.internal.R.string.httpErrorBadUrl));
            return false;
        }
        mNetwork = Network.getInstance(mListener.getContext());
        if (mListener.isSynchronous()) {
            return handleHTTPLoad();
        }
        WebViewWorker.getHandler().obtainMessage(
                WebViewWorker.MSG_ADD_HTTPLOADER, this).sendToTarget();
        return true;
    }
    else if (handleLocalFile(url, mListener, mSettings))
    {
        return true;
    }

    if (DebugFlags.FRAME_LOADER) {
        Log.v(LOGTAG, "FrameLoader.executeLoad: url protocol not supported:"
                + mListener.url());
    }

    mListener.error(EventHandler.ERROR_UNSUPPORTED_SCHEME,
            mListener.getContext().getText(
                    com.android.internal.R.string.httpErrorUnsupportedScheme).toString());
#endif

    return false;
}

/* package */
CARAPI_(Boolean) FrameLoader::HandleLocalFile(
	/* [in] */ const String& url, 
	/* [in] */ const LoadListener* loadListener,
	/* [in] */ const WebSettings* settings)
{
#if 0
    // Attempt to decode the percent-encoded url before passing to the
    // local loaders.
    url = new String(URLUtil.decode(url.getBytes()));
    
    if (URLUtil.isAssetUrl(url)) {
        if (loadListener.isSynchronous()) {
            new FileLoader(url, loadListener, FileLoader.TYPE_ASSET,
                    true).load();
        } else {
            // load asset in a separate thread as it involves IO
            WebViewWorker.getHandler().obtainMessage(
                    WebViewWorker.MSG_ADD_STREAMLOADER,
                    new FileLoader(url, loadListener, FileLoader.TYPE_ASSET,
                            true)).sendToTarget();
        }
        return true;
    } else if (URLUtil.isResourceUrl(url)) {
        if (loadListener.isSynchronous()) {
            new FileLoader(url, loadListener, FileLoader.TYPE_RES,
                    true).load();
        } else {
            // load resource in a separate thread as it involves IO
            WebViewWorker.getHandler().obtainMessage(
                    WebViewWorker.MSG_ADD_STREAMLOADER,
                    new FileLoader(url, loadListener, FileLoader.TYPE_RES,
                            true)).sendToTarget();
        }
        return true;
    } else if (URLUtil.isFileUrl(url)) {
        if (loadListener.isSynchronous()) {
            new FileLoader(url, loadListener, FileLoader.TYPE_FILE,
                    settings.getAllowFileAccess()).load();
        } else {
            // load file in a separate thread as it involves IO
            WebViewWorker.getHandler().obtainMessage(
                    WebViewWorker.MSG_ADD_STREAMLOADER,
                    new FileLoader(url, loadListener, FileLoader.TYPE_FILE,
                            settings.getAllowFileAccess())).sendToTarget();
        }
        return true;
    } else if (settings.getAllowContentAccess() &&
               URLUtil.isContentUrl(url)) {
        // Send the raw url to the ContentLoader because it will do a
        // permission check and the url has to match.
        if (loadListener.isSynchronous()) {
            new ContentLoader(loadListener.url(), loadListener).load();
        } else {
            // load content in a separate thread as it involves IO
            WebViewWorker.getHandler().obtainMessage(
                    WebViewWorker.MSG_ADD_STREAMLOADER,
                    new ContentLoader(loadListener.url(), loadListener))
                    .sendToTarget();
        }
        return true;
    } else if (URLUtil.isDataUrl(url)) {
        // load data in the current thread to reduce the latency
        new DataLoader(url, loadListener).load();
        return true;
    } else if (URLUtil.isAboutUrl(url)) {
        loadListener.data(mAboutBlank.getBytes(), mAboutBlank.length());
        loadListener.endData();
        return true;
    }
#endif
    return false;
}

CARAPI_(Boolean) FrameLoader::HandleHTTPLoad()
{}

/*
 * This function is used by handleCache to
 * setup a load from the byte stream in a CacheResult.
 */
CARAPI_(void) FrameLoader::StartCacheLoad(
	/* [in] */ const ICacheManagerCacheResult* result)
{}

/*
 * This function is used by the handleHTTPLoad to setup the cache headers
 * correctly.
 * Returns true if the response was handled from the cache
 */
CARAPI_(Boolean) FrameLoader::HandleCache()
{}

/**
 * Add the static headers that don't change with each request.
 */
CARAPI_(void) FrameLoader::PopulateStaticHeaders()
{}

/**
 * Add the content related headers. These headers contain user private data
 * and is not used when we are proxying an untrusted request.
 */
CARAPI_(void) FrameLoader::PopulateHeaders()
{}