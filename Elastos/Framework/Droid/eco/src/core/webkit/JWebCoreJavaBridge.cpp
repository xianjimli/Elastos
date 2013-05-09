
#include "webkit/JWebCoreJavaBridge.h"
#include "webkit/PerfChecker.h"
#include "webkit/CCookieManager.h"
#include "webkit/CPluginManager.h"
#include "webkit/CertTool.h"
#include "content/Context.h"

const CString JWebCoreJavaBridge::LOGTAG("webkit-timers");
AutoPtr<IWebView> JWebCoreJavaBridge::sCurrentMainWebView = NULL;
Core::Threading::Mutex JWebCoreJavaBridge::mutexClass;

JWebCoreJavaBridge::JWebCoreJavaBridge()
{
	NativeConstructor();
}

//@Override
CARAPI_(void) JWebCoreJavaBridge::Finalize()
{
    NativeFinalize();
}

/* synchronized */
CARAPI_(void) JWebCoreJavaBridge::SetActiveWebView(
	/* [in] */ IWebView* webview)
{
	//assert(webview != NULL);
    Mutex::Autolock lock(mutexClass);    
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
	//assert(webview != NULL);
    Mutex::Autolock lock(mutexClass);
	if (sCurrentMainWebView.Get() != webview) {
        // it is possible if there is a sub-WebView. Do nothing.
        return;
    }
    sCurrentMainWebView.Clear();
}

/**
 * Call native timer callbacks.
 */
CARAPI_(void) JWebCoreJavaBridge::FireSharedTimer()
{
    PerfChecker* checker = new PerfChecker();
    // clear the flag so that sharedTimerFired() can set a new timer
    mHasInstantTimer = FALSE;
    SharedTimerFired();
    checker -> ResponseAlert("sharedTimer");
    delete checker;
}

// called from JNI side
CARAPI_(void) JWebCoreJavaBridge::SignalServiceFuncPtrQueue()
{
    void (STDCALL JWebCoreJavaBridge::*pHandlerFunc)();

    pHandlerFunc = &JWebCoreJavaBridge::NativeServiceFuncPtrQueue;

    SendMessage(*(Handle32*)&pHandlerFunc, NULL);
}

CARAPI_(void) JWebCoreJavaBridge::NativeServiceFuncPtrQueue()
{}

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
{
//    if (DebugFlags.J_WEB_CORE_JAVA_BRIDGE) Log.v(LOGTAG, "setSharedTimer " + timemillis);

    ECode (STDCALL JWebCoreJavaBridge::*pHandlerFunc)();

    pHandlerFunc = &JWebCoreJavaBridge::HandleTimerMessage;

    if (timemillis <= 0) {
        // we don't accumulate the sharedTimer unless it is a delayed
        // request. This way we won't flood the message queue with
        // WebKit messages. This should improve the browser's
        // responsiveness to key events.
        if (mHasInstantTimer) {
            return;
        } else {
            mHasInstantTimer = TRUE;
            PostCppCallbackDelayed((Handle32)this, *(Handle32*)&pHandlerFunc, NULL, 0, timemillis);
        }
    } else {
        PostCppCallbackDelayed((Handle32)this, *(Handle32*)&pHandlerFunc, NULL, 0, timemillis);
    }
}

/**
 * Stop the shared timer.
 */
CARAPI_(void) JWebCoreJavaBridge::StopSharedTimer()
{
//	if (DebugFlags.J_WEB_CORE_JAVA_BRIDGE) {
//        Log.v(LOGTAG, "stopSharedTimer removing all timers");
//    }

    ECode (STDCALL JWebCoreJavaBridge::*pHandlerFunc)();
    pHandlerFunc = &JWebCoreJavaBridge::HandleTimerMessage;

    RemoveMessage(*(Handle32*)&pHandlerFunc);

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


PInterface JWebCoreJavaBridge::Probe(
    /* [in] */ REIID riid)
{
    return NULL;
}

UInt32 JWebCoreJavaBridge::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 JWebCoreJavaBridge::Release()
{
    return ElRefBase::Release();
}

ECode JWebCoreJavaBridge::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}


ECode JWebCoreJavaBridge::Start(
    /* [in] */ ApartmentAttr attr)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode JWebCoreJavaBridge::Finish()
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode JWebCoreJavaBridge::PostCppCallback(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallback(target, func, params, id);
}

ECode JWebCoreJavaBridge::PostCppCallbackAtTime(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id,
    /* [in] */ Millisecond64 uptimeMillis)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallbackAtTime(target, func, params, id, uptimeMillis);
}

ECode JWebCoreJavaBridge::PostCppCallbackDelayed(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id,
    /* [in] */ Millisecond64 delayMillis)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallbackDelayed(target, func, params, id, delayMillis);
}

ECode JWebCoreJavaBridge::PostCppCallbackAtFrontOfQueue(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallbackAtFrontOfQueue(target, func, params, id);
}

ECode JWebCoreJavaBridge::RemoveCppCallbacks(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func)
{
    assert(mApartment != NULL);
    return mApartment->RemoveCppCallbacks(target, func);
}

ECode JWebCoreJavaBridge::RemoveCppCallbacksEx(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ Int32 id)
{
    assert(mApartment != NULL);
    return mApartment->RemoveCppCallbacksEx(target, func, id);
}

ECode JWebCoreJavaBridge::HasCppCallbacks(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [out] */ Boolean* result)
{
    assert(mApartment != NULL);
    return mApartment->HasCppCallbacks(target, func, result);
}

ECode JWebCoreJavaBridge::HasCppCallbacksEx(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ Int32 id,
    /* [out] */ Boolean* result)
{
    assert(mApartment != NULL);
    return mApartment->HasCppCallbacksEx(target, func, id, result);
}

ECode JWebCoreJavaBridge::SendMessage(
    /* [in] */ Int32 message,
    /* [in] */ IParcel* params)
{
    if (message == REFRESH_PLUGINS) {
//        NativeUpdatePluginDirectories(PluginManager.getInstance(null)
//                        .getPluginDirectories(), ((Boolean) msg.obj)
//                        .booleanValue());

        return NOERROR;
    }

    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode JWebCoreJavaBridge::SendMessage(
    /* [in] */ Handle32 pvFunc,
    /* [in] */ IParcel* params)
{
    return mApartment->PostCppCallback((Handle32)this, pvFunc, params, 0);
}

ECode JWebCoreJavaBridge::SendMessageAtTime(
    /* [in] */ Handle32 pvFunc,
    /* [in] */ IParcel* params,
    /* [in] */ Millisecond64 uptimeMillis)
{
    return mApartment->PostCppCallbackAtTime(
        (Handle32)this, pvFunc, params, 0, uptimeMillis);
}

ECode JWebCoreJavaBridge::RemoveMessage(
    /* [in] */ Handle32 func)
{
    return mApartment->RemoveCppCallbacks((Handle32)this, func);
}

ECode JWebCoreJavaBridge::HandleTimerMessage()
{
    if (mTimerPaused) {
        mHasDeferredTimers = TRUE;
    } else {
        FireSharedTimer();
    }
}