
#include "webkit/FrameLoader.h"
#include "webkit/CURLUtil.h"
#include "webkit/LoadListener.h"
#include "webkit/Network.h"
#include "webkit/FileLoader.h"
#include "webkit/ContentLoader.h"
#include "webkit/DataLoader.h"
#include "webkit/CCacheManager.h"
#include "webkit/CacheLoader.h"
#include "webkit/CCookieManager.h"

#include <elastos/Thread.h>

const CString FrameLoader::HEADER_STR = "text/xml, text/html, application/xhtml+xml, image/png, text/plain, */*;q=0.8";
const CString FrameLoader::CONTENT_TYPE = "content-type";

// Contents of an about:blank page
const CString FrameLoader::mAboutBlank =
        "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EB\"><html><head><title>about:blank</title></head><body></body></html>";

const CString FrameLoader::LOGTAG = "webkit";


FrameLoader::FrameLoader(
	/* [in] */ LoadListener* listener, 
	/* [in] */ WebSettings* settings,
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
	Boolean bFlag = FALSE;
	CURLUtil::AcquireSingleton(&pURL);
	pURL->IsNetworkUrl(ref, &bFlag);

	if (bFlag) {
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
	String url;
    mListener->Url(url);

	IURLUtil* pURL = NULL;
	Boolean bFlag = FALSE;
	CURLUtil::AcquireSingleton(&pURL);
	pURL->IsNetworkUrl(url, &bFlag);

    if (bFlag) {
        if (mSettings->GetBlockNetworkLoads()) {
//            mListener->Error(EventHandler::ERROR_BAD_URL,
//                    mListener.getContext().getString(com.android.internal.R.string.httpErrorBadUrl));
            return false;
        }
        // Make sure the host part of the url is correctly
        // encoded before sending the request
//        if (!pURL->VerifyURLEncoding(mListener->Host())) {
//            mListener->Error(EventHandler.ERROR_BAD_URL,
//                    mListener.getContext().getString(
//                    com.android.internal.R.string.httpErrorBadUrl));
//            return false;
//        }
        mNetwork = Network::GetInstance(mListener->GetContext());
        if (mListener->IsSynchronous()) {
            return HandleHTTPLoad();
        }

//        WebViewWorker.getHandler().obtainMessage(
//                WebViewWorker.MSG_ADD_HTTPLOADER, this).sendToTarget();

        return TRUE;
    } else if (HandleLocalFile(url, mListener, mSettings)) {
        return TRUE;
    }

//    if (DebugFlags.FRAME_LOADER) {
//        Log.v(LOGTAG, "FrameLoader.executeLoad: url protocol not supported:"
//                + mListener.url());
//    }

//    mListener.error(EventHandler.ERROR_UNSUPPORTED_SCHEME,
//            mListener.getContext().getText(
//                    com.android.internal.R.string.httpErrorUnsupportedScheme).toString());

    return FALSE;
}

/* package */
CARAPI_(Boolean) FrameLoader::HandleLocalFile(
	/* [in] */ const String& _url, 
	/* [in] */ LoadListener* loadListener,
	/* [in] */ WebSettings* settings)
{

    // Attempt to decode the percent-encoded url before passing to the
    // local loaders.
    String url/*(URLUtil.decode(url.getBytes()))*/;
    AutoPtr<IURLUtil> URL;
    
    CURLUtil::AcquireSingleton((IURLUtil**)&URL);
    
    Boolean bFlag = FALSE;
    URL->IsAssetUrl(url, &bFlag);
    if (bFlag) {
        if (loadListener->IsSynchronous()) {
            FileLoader fileLoader(url, loadListener, FileLoader::TYPE_ASSET, TRUE);
            fileLoader.Load();
        } else {
            // load asset in a separate thread as it involves IO
//            WebViewWorker.getHandler().obtainMessage(
//                    WebViewWorker.MSG_ADD_STREAMLOADER,
//                    new FileLoader(url, loadListener, FileLoader.TYPE_ASSET,
//                            true)).sendToTarget();
        }
        return TRUE;
    } else if (URL->IsResourceUrl(url, &bFlag), bFlag) {
        if (loadListener->IsSynchronous()) {
            FileLoader fileLoader(url, loadListener, FileLoader::TYPE_RES, TRUE);
            fileLoader.Load();
        } else {
            // load resource in a separate thread as it involves IO
//            WebViewWorker.getHandler().obtainMessage(
//                    WebViewWorker.MSG_ADD_STREAMLOADER,
//                    new FileLoader(url, loadListener, FileLoader.TYPE_RES,
//                            true)).sendToTarget();
        }
        return TRUE;
    } else if (URL->IsFileUrl(url, &bFlag), bFlag) {
        if (loadListener->IsSynchronous()) {
            FileLoader fileLoader(url, loadListener, FileLoader::TYPE_FILE,
                    settings->GetAllowFileAccess());
            fileLoader.Load();
        } else {
            // load file in a separate thread as it involves IO
//            WebViewWorker.getHandler().obtainMessage(
//                    WebViewWorker.MSG_ADD_STREAMLOADER,
//                    new FileLoader(url, loadListener, FileLoader.TYPE_FILE,
//                            settings.getAllowFileAccess())).sendToTarget();
        }
        return TRUE;
    } else if (settings->GetAllowContentAccess() &&
               (URL->IsContentUrl(url, &bFlag), &bFlag) ) {
        // Send the raw url to the ContentLoader because it will do a
        // permission check and the url has to match.
        if (loadListener->IsSynchronous()) {
            String str;
            loadListener->Url(str);
            ContentLoader contentLoader(str, loadListener);
            contentLoader.Load();
        } else {
            // load content in a separate thread as it involves IO
//            WebViewWorker.getHandler().obtainMessage(
//                    WebViewWorker.MSG_ADD_STREAMLOADER,
//                    new ContentLoader(loadListener.url(), loadListener))
//                    .sendToTarget();
        }
        return TRUE;
    } else if (URL->IsDataUrl(url, &bFlag), bFlag) {
        // load data in the current thread to reduce the latency
        DataLoader dataLoader(url, loadListener);
        dataLoader.Load();

        return TRUE;
    } else if (URL->IsAboutUrl(url, &bFlag), bFlag) {
//        loadListener.data(mAboutBlank.getBytes(), mAboutBlank.length());
        loadListener->EndData();
        return TRUE;
    }

    return FALSE;
}

CARAPI_(Boolean) FrameLoader::HandleHTTPLoad()
{
    if (mHeaders == NULL) {
        CObjectStringMap::New((IObjectStringMap**)&mHeaders);
    }

    PopulateStaticHeaders();
    PopulateHeaders();

    // response was handled by Cache, don't issue HTTP request
    if (HandleCache()) {
        // push the request data down to the LoadListener
        // as response from the cache could be a redirect
        // and we may need to initiate a network request if the cache
        // can't satisfy redirect URL
        mListener->SetRequestData(mMethod, mHeaders, mPostData);
        return TRUE;
    }

//    if (DebugFlags.FRAME_LOADER) {
//        Log.v(LOGTAG, "FrameLoader: http " + mMethod + " load for: "
//                + mListener.url());
//    }

    Boolean ret = FALSE;
    Int32 error;// = EventHandler.ERROR_UNSUPPORTED_SCHEME;
    
    ret = mNetwork->RequestURL(mMethod, mHeaders, mPostData, mListener);

    if (!ret) {
//        mListener.error(error, mListener.getContext().getText(
//                EventHandler.errorStringResources[Math.abs(error)]).toString());
        return FALSE;
    }

    return TRUE;
}

/*
 * This function is used by handleCache to
 * setup a load from the byte stream in a CacheResult.
 */
CARAPI_(void) FrameLoader::StartCacheLoad(
	/* [in] */ const ICacheManagerCacheResult* result)
{
//    if (DebugFlags.FRAME_LOADER) {
//        Log.v(LOGTAG, "FrameLoader: loading from cache: "
//              + mListener.url());
//    }
    // Tell the Listener respond with the cache file
    CacheLoader* cacheLoader = new CacheLoader(mListener, (CCacheManager::CacheResult*)result);
    mListener->SetCacheLoader(cacheLoader);
    if (mListener->IsSynchronous()) {
        cacheLoader->Load();
    } else {
        // Load the cached file in a separate thread
//        WebViewWorker.getHandler().obtainMessage(
//                WebViewWorker.MSG_ADD_STREAMLOADER, cacheLoader).sendToTarget();
    }
}

/*
 * This function is used by the handleHTTPLoad to setup the cache headers
 * correctly.
 * Returns true if the response was handled from the cache
 */
CARAPI_(Boolean) FrameLoader::HandleCache()
{
    switch (mCacheMode)
    {
        // This mode is normally used for a reload, it instructs the http
        // loader to not use the cached content.
        case WebSettings::WS_LOAD_NO_CACHE:
            break;            
            
        // This mode is used when the content should only be loaded from
        // the cache. If it is not there, then fail the load. This is used
        // to load POST content in a history navigation.
        case WebSettings::WS_LOAD_CACHE_ONLY:
        {
            String strUrl;
            mListener->Url(strUrl);
            AutoPtr<ICacheManagerCacheResult> result = CCacheManager::GetCacheFile(strUrl, mListener->PostIdentifier(), NULL);
            if (result != NULL)
            {
                StartCacheLoad(result);
            }
            else
            {
                // This happens if WebCore was first told that the POST
                // response was in the cache, then when we try to use it
                // it has gone.
                // Generate a file not found error
//                int err = EventHandler.FILE_NOT_FOUND_ERROR;
//                mListener.error(err, mListener.getContext().getText(
//                        EventHandler.errorStringResources[Math.abs(err)])
//                        .toString());
            }
            return TRUE;
        }

        // This mode is for when the user is doing a history navigation
        // in the browser and should returned cached content regardless
        // of it's state. If it is not in the cache, then go to the 
        // network.
        case WebSettings::WS_LOAD_CACHE_ELSE_NETWORK:
        {
//            if (DebugFlags.FRAME_LOADER) {
//                Log.v(LOGTAG, "FrameLoader: checking cache: "
//                        + mListener.url());
//            }
            // Get the cache file name for the current URL, passing null for
            // the validation headers causes no validation to occur
            String strUrl;
            mListener->Url(strUrl);
            AutoPtr<ICacheManagerCacheResult> result = CCacheManager::GetCacheFile(strUrl,
                    mListener->PostIdentifier(), NULL);
            if (result != NULL)
            {
                StartCacheLoad(result);
                return TRUE;
            }
            break;
        }

        // This is the default case, which is to check to see if the
        // content in the cache can be used. If it can be used, then
        // use it. If it needs revalidation then the relevant headers
        // are added to the request.
        default:
        case WebSettings::WS_LOAD_NORMAL:
            return mListener->CheckCache(mHeaders);
    }// end of switch

    return FALSE;
}

/**
 * Add the static headers that don't change with each request.
 */
CARAPI_(void) FrameLoader::PopulateStaticHeaders()
{
    // Accept header should already be there as they are built by WebCore,
    // but in the case they are missing, add some.
    String* accept = NULL;
    mHeaders->Get((String)"Accept", (IInterface**)&accept);
    if (accept == NULL || accept->GetLength() == 0) {
        mHeaders->Put((String)"Accept", (IInterface*)&HEADER_STR);
    }

    mHeaders->Put((String)"Accept-Charset", (IInterface*)"utf-8, iso-8859-1, utf-16, *;q=0.7");

    String* acceptLanguage = mSettings->GetAcceptLanguage();

    if (acceptLanguage->GetLength() > 0) {
        mHeaders->Put((String)"Accept-Language", (IInterface*)acceptLanguage);
    }
    
    mHeaders->Put((String)"User-Agent", (IInterface*)(mSettings->GetUserAgentString()));
}

/**
 * Add the content related headers. These headers contain user private data
 * and is not used when we are proxying an untrusted request.
 */
CARAPI_(void) FrameLoader::PopulateHeaders()
{
    if (mReferrer.GetLength() != 0) {
        mHeaders->Put((String)"Referer", (IInterface*)&mReferrer);
    }

    if (mContentType.GetLength() != 0) {
        mHeaders->Put((String)CONTENT_TYPE, (IInterface*)&mContentType);
    }

    // if we have an active proxy and have proxy credentials, do pre-emptive
    // authentication to avoid an extra round-trip:
    if (mNetwork->IsValidProxySet()) {
        const String* username;
        const String* password;
        /* The proxy credentials can be set in the Network thread */
        {
            Mutex mutex;
            Mutex::Autolock lock(mutex);
            username = mNetwork->GetProxyUsername();
            password = mNetwork->GetProxyPassword();
        }

        if (username != NULL && password != NULL) {
            // we collect credentials ONLY if the proxy scheme is BASIC!!!
//            String proxyHeader = RequestHandle.authorizationHeader(true);
//            mHeaders.put(proxyHeader,
//                    "Basic " + RequestHandle.computeBasicAuthResponse(
//                            username, password));
        }
    }

    // Set cookie header
    ICookieManager* pCookieManager = NULL;
    CCookieManager::AcquireSingleton(&pCookieManager);
//    String cookie;
//    pCookieManager->GetCookieEx(mListener->GetWebAddress());
//    if (cookie != null && cookie.length() > 0)
//    {
//        mHeaders->Put("Cookie", cookie);
//    }
}