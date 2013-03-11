
#include "webkit/JWebCoreJavaBridge.h"

const char* JWebCoreJavaBridge::LOGTAG = "webkit-timers";

JWebCoreJavaBridge::JWebCoreJavaBridge()
{	
	
}

/* synchronized */
CARAPI_(void) JWebCoreJavaBridge::SetActiveWebView(
	/* [in] */ IWebView* webview)
{}

/* synchronized */
CARAPI_(void) JWebCoreJavaBridge::RemoveActiveWebView(
	/* [in] */ IWebView* webview)
{}

/**
 * handleMessage
 * @param msg The dispatched message.
 *
 * The only accepted message currently is TIMER_MESSAGE
 */
//@Override
CARAPI_(void) JWebCoreJavaBridge::HandleMessage(
	/* [in] */ IMessage* msg)
{}

/**
 * Pause all timers.
 */
CARAPI_(void) JWebCoreJavaBridge::Pause()
{}

/**
 * Resume all timers.
 */
CARAPI_(void) JWebCoreJavaBridge::Resume()
{}

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
	/* [in] */ /*Set<String> packageNames*/)
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
{}

/**
 * Call native timer callbacks.
 */
CARAPI_(void) JWebCoreJavaBridge::FireSharedTimer()
{}

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
{}

/**
 * Retrieve the cookie string for the given url.
 * @param url The resource's url.
 * @return A String representing the cookies for the given resource url.
 */
CARAPI_(CString) JWebCoreJavaBridge::Cookies(
	/* [in] */ const String& url)
{}

/**
 * Returns whether cookies are enabled or not.
 */
CARAPI_(Boolean) JWebCoreJavaBridge::CookiesEnabled()
{}

/**
 * Returns an array of plugin directoies
 */
CARAPI_(void) JWebCoreJavaBridge::GetPluginDirectories(
    /* [out] */ String& str) const
{}

/**
 * Returns the path of the plugin data directory
 */
CARAPI_(void) JWebCoreJavaBridge::GetPluginSharedDataDirectory(
    /* [out] */ String& str) const
{}

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
{}

CARAPI_(void) JWebCoreJavaBridge::GetKeyStrengthList(
    /* [out] */ String& str) const
{}

/*synchronized*/ 
CARAPI_(void) JWebCoreJavaBridge::GetSignedPublicKey(
	/* [in] */ Int32 index, 
	/* [in] */ const String& challenge,
	/* [in] */ const String& url,
    /* [out] */ String& str)
{}

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