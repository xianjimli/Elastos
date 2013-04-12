
#include "webkit/JWebCoreJavaBridge.h"
#include "webkit/PerfChecker.h"
#include "webkit/CCookieManager.h"
#include "webkit/CPluginManager.h"
#include "webkit/CertTool.h"
#include "content/Context.h"

const CString JWebCoreJavaBridge::LOGTAG = "webkit-timers";
AutoPtr<IWebView> JWebCoreJavaBridge::sCurrentMainWebView = NULL;

JWebCoreJavaBridge::JWebCoreJavaBridge()
{
	NativeConstructor();
}

/* synchronized */
CARAPI_(void) JWebCoreJavaBridge::SetActiveWebView(
	/* [in] */ IWebView* webview)
{
	assert(webview != NULL);

	if (sCurrentMainWebView.Get() != NULL) {
        // it is possible if there is a sub-WebView. Do nothing.
        return;
    }
    //sCurrentMainWebView = new WeakReference<WebView>(webview);
    sCurrentMainWebView = webview;
}

/* synchronized */
CARAPI_(void) JWebCoreJavaBridge::RemoveActiveWebView(
	/* [in] */ IWebView* webview)
{
	assert(webview != NULL);

	if (sCurrentMainWebView.Get() != webview) {
        // it is possible if there is a sub-WebView. Do nothing.
        return;
    }
    sCurrentMainWebView.Clear();
}

/**
 * handleMessage
 * @param msg The dispatched message.
 *
 * The only accepted message currently is TIMER_MESSAGE
 */
//@Override
CARAPI_(void) JWebCoreJavaBridge::HandleMessage(
	/* [in] */ IMessage* msg)
{
	assert(msg != NULL);

	switch (0/*msg.what*/)
	{
        case TIMER_MESSAGE:
	        {
	            if (mTimerPaused) {
	                mHasDeferredTimers = TRUE;
	            } else {
	                FireSharedTimer();
	            }
	            break;
	        }

        case FUNCPTR_MESSAGE:
            NativeServiceFuncPtrQueue();
            break;

        case REFRESH_PLUGINS:
//            nativeUpdatePluginDirectories(PluginManager.getInstance(null)
//                    .getPluginDirectories(), ((Boolean) msg.obj)
//                    .booleanValue());
            break;
    }
}

/**
 * Pause all timers.
 */
CARAPI_(void) JWebCoreJavaBridge::Pause()
{
	if (--mPauseTimerRefCount == 0)	{
        mTimerPaused = TRUE;
        mHasDeferredTimers = FALSE;
    }
}

/**
 * Resume all timers.
 */
CARAPI_(void) JWebCoreJavaBridge::Resume()
{
	if (++mPauseTimerRefCount == 1)	{
       mTimerPaused = FALSE;
       if (mHasDeferredTimers) {
           mHasDeferredTimers = FALSE;
           FireSharedTimer();
       }
    }
}

/**
 * Set WebCore cache size.
 * @param bytes The cache size in bytes.
 *
 * native
 */
CARAPI_(void) JWebCoreJavaBridge::SetCacheSize(
	/* [in] */ Int32 bytes)
{}

/*native*/
CARAPI_(void) JWebCoreJavaBridge::SetNetworkOnLine(
	/* [in] */ Boolean online)
{}

/* native */	
CARAPI_(void) JWebCoreJavaBridge::SetNetworkType(
	/* [in] */ const String& type, 
	/* [in] */ const String& subtype)
{}

/* native */
CARAPI_(void) JWebCoreJavaBridge::AddPackageNames(
	/* [in] */ Set<String>* packageNames)
{}

/* native */
CARAPI_(void) JWebCoreJavaBridge::AddPackageName(
	/* [in] */ const String& packageName)
{}

CARAPI_(void) JWebCoreJavaBridge::RemovePackageName(
	/* [in] */ const String& packageName)
{}

//@Override
CARAPI_(void) JWebCoreJavaBridge::Finalize()
{
	NativeFinalize();
}

/**
 * Call native timer callbacks.
 */
CARAPI_(void) JWebCoreJavaBridge::FireSharedTimer()
{
	PerfChecker checker;// = new PerfChecker();
    // clear the flag so that sharedTimerFired() can set a new timer
    mHasInstantTimer = FALSE;
    SharedTimerFired();
    checker.ResponseAlert("sharedTimer");
}

// called from JNI side
CARAPI_(void) JWebCoreJavaBridge::SignalServiceFuncPtrQueue()
{}

CARAPI_(void) JWebCoreJavaBridge::NativeServiceFuncPtrQueue()
{}

/**
 * Store a cookie string associated with a url.
 * @param url The url to be used as a key for the cookie.
 * @param value The cookie string to be stored.
 */
CARAPI_(void) JWebCoreJavaBridge::SetCookies(
	/* [in] */ const String& url, 
	/* [in] */ const String& value)
{
	String buffer;

	if (value.Contains("\r") || value.Contains("\n")) {
        // for security reason, filter out '\r' and '\n' from the cookie
        Int32 size = value.GetLength();
        //StringBuilder buffer = new StringBuilder(size);
        
        Int32 i = 0;
        while (i != -1 && i < size) {
            Int32 ir = value.IndexOf('\r', i);
            Int32 in = value.IndexOf('\n', i);
            Int32 newi = (ir == -1) ? in : (in == -1 ? ir : (ir < in ? ir
                    : in));
            if (newi > i) {
                buffer.Append(value.Substring(i, newi));
            } else if (newi == -1) {
                buffer.Append(value.Substring(i, size));
                break;
            }
            i = newi + 1;
        }
        //value = buffer.toString();
    }

    ICookieManager* pCookieManager = NULL;
    CCookieManager::AcquireSingleton(&pCookieManager);
    pCookieManager->SetCookie(url, /*value*/buffer);
}

/**
 * Retrieve the cookie string for the given url.
 * @param url The resource's url.
 * @return A String representing the cookies for the given resource url.
 */
CARAPI_(void) JWebCoreJavaBridge::Cookies(
	/* [in] */ const String& url,
	/* [out] */ String& str)
{
	ICookieManager* pCookieManager = NULL;
	CCookieManager::AcquireSingleton(&pCookieManager);
	pCookieManager->GetCookie(url, &str);
}

/**
 * Returns whether cookies are enabled or not.
 */
CARAPI_(Boolean) JWebCoreJavaBridge::CookiesEnabled()
{
	ICookieManager* pCookieManager = NULL;
	CCookieManager::AcquireSingleton(&pCookieManager);
	Boolean bRet = false;
	pCookieManager->AcceptCookie(&bRet);
}

/**
 * Returns an array of plugin directoies
 */
CARAPI_(void) JWebCoreJavaBridge::GetPluginDirectories(
    /* [out] */ ArrayOf<String>* list) const
{
	IPluginManager* p = NULL;
	CPluginManager::AcquireSingleton(&p);
	p->GetPluginDirectories(list);
	//return PluginManager.getInstance(null).getPluginDirectories();
}

/**
 * Returns the path of the plugin data directory
 */
CARAPI_(void) JWebCoreJavaBridge::GetPluginSharedDataDirectory(
    /* [out] */ String* str) const
{
	CPluginManager* p = NULL;
	CPluginManager::AcquireSingletonByFriend(&p);
	p->GetPluginSharedDataDirectory(str);
}

/**
 * setSharedTimer
 * @param timemillis The relative time when the timer should fire
 */
CARAPI_(void) JWebCoreJavaBridge::SetSharedTimer(
	/* [in] */ Int64 timemillis)
{}

/**
 * Stop the shared timer.
 */
CARAPI_(void) JWebCoreJavaBridge::StopSharedTimer()
{
//	if (DebugFlags.J_WEB_CORE_JAVA_BRIDGE) {
//        Log.v(LOGTAG, "stopSharedTimer removing all timers");
//    }
//    RemoveMessages(TIMER_MESSAGE);
    mHasInstantTimer = FALSE;
    mHasDeferredTimers = FALSE;
}

CARAPI_(void) JWebCoreJavaBridge::GetKeyStrengthList(
    /* [out] */ Vector<String>& list) const
{
	CertTool::GetKeyStrengthList(list);
}

/*synchronized*/ 
CARAPI_(void) JWebCoreJavaBridge::GetSignedPublicKey(
	/* [in] */ Int32 index, 
	/* [in] */ const String& challenge,
	/* [in] */ const String& url,
    /* [out] */ String& strOut)
{
	//WebView current = sCurrentMainWebView.get();
    if (sCurrentMainWebView != NULL) {
        // generateKeyPair expects organizations which we don't have. Ignore
        // url.
        IContext* pContext = NULL;
        sCurrentMainWebView->GetContext(&pContext);
        assert(pContext != NULL);
        CertTool::GetSignedPublicKey(pContext, index, challenge, strOut);
    } else {
//        Log.e(LOGTAG, "There is no active WebView for getSignedPublicKey");
        strOut = "";
    }
}

/*native*/
CARAPI_(void) JWebCoreJavaBridge::NativeConstructor()
{}

/*native*/
CARAPI_(void) JWebCoreJavaBridge::NativeFinalize()
{}

/*native*/
CARAPI_(void) JWebCoreJavaBridge::SharedTimerFired()
{}

/*native*/
CARAPI_(void) JWebCoreJavaBridge::NativeUpdatePluginDirectories(
	/* [in] */ Vector<String>& directories, 
	/* [in] */ Boolean reload)
{}