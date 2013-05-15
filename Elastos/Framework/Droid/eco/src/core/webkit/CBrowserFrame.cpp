#include <stdio.h>

#include "webkit/CBrowserFrame.h"
#include "webkit/JWebCoreJavaBridge.h"
#include "webkit/CWebViewDatabase.h"
#include "webkit/CCacheManager.h"
#include "webkit/CookieSyncManager.h"
#include "webkit/CPluginManager.h"
#include "webkit/CURLUtil.h"
#include "webkit/WebHistoryItem.h"
#include "webkit/WebBackForwardList.h"
#include "view/ViewRoot.h"
#include "view/CDisplay.h"
#include "webkit/CCallbackProxy.h"
#include "os/CApartment.h"

const CString CBrowserFrame::LOGTAG("webkit");

AutoPtr<JWebCoreJavaBridge> CBrowserFrame::sJavaBridge;

AutoPtr<CBrowserFrame::ConfigCallback> CBrowserFrame::sConfigCallback;

ECode CBrowserFrame::LoadUrl(
    /* [in] */ const String& url,
    /* [in] */ IObjectStringMap* extraHeaders)
{
    VALIDATE_NOT_NULL(extraHeaders);

    mLoadInitFromJava = TRUE;
      
    Boolean flag = FALSE;
    flag = CURLUtil::IsJavaScriptUrl(url);

    if (flag) {
        // strip off the scheme and evaluate the string
        StringByEvaluatingJavaScriptFromString(
                url.Substring(CString("javascript:").GetLength()), NULL);
    }
    else {
        NativeLoadUrl((const char*)url, extraHeaders);
    }
    mLoadInitFromJava = FALSE;

    return NOERROR;
}

ECode CBrowserFrame::PostUrl(
    /* [in] */ const String& url,
    /* [in] */ const ArrayOf<Byte>& data)
{
    mLoadInitFromJava = TRUE;
    NativePostUrl((const char*)url, data);
    mLoadInitFromJava = FALSE;

    return NOERROR;
}

ECode CBrowserFrame::LoadData(
    /* [in] */ const String& baseUrl,
    /* [in] */ const String& data,
    /* [in] */ const String& mimeType,
    /* [in] */ const String& encoding,
    /* [in] */ const String& historyUrl)
{
    String _baseUrl    = const_cast<String&>(baseUrl);
    String _data       = const_cast<String&>(data);
    String _mimeType   = const_cast<String&>(mimeType);
    String _encoding   = const_cast<String&>(encoding);
    String _historyUrl = const_cast<String&>(historyUrl);

    mLoadInitFromJava = TRUE;
    if (_historyUrl == NULL || _historyUrl.GetLength() == 0) {
        _historyUrl = "about:blank";
    }

    if (_data == NULL) {
        _data = "";
    }

    // Setup defaults for missing values. These defaults where taken from
    // WebKit's WebFrame.mm
    if (_baseUrl == NULL || _baseUrl.GetLength() == 0) {
        _baseUrl = "about:blank";
    }

    if (_mimeType == NULL || _mimeType.GetLength() == 0) {
        _mimeType = "text/html";
    }

    NativeLoadData(_baseUrl, _data, _mimeType, _encoding, _historyUrl);
    mLoadInitFromJava = FALSE;

    return NOERROR;
}

ECode CBrowserFrame::GoBackOrForward(
    /* [in] */ Int32 steps)
{
    mLoadInitFromJava = TRUE;
    NativeGoBackOrForward(steps);
    mLoadInitFromJava = FALSE;

    return NOERROR;
}

ECode CBrowserFrame::Destroy()
{
    NativeDestroyFrame();
    mBlockMessages = TRUE;
//    RemoveCallbacksAndMessages(NULL);

    return NOERROR;
}

ECode CBrowserFrame::ExternalRepresentation(
    /* [in] */ IApartment* target,
    /* [in] */ Int32 message,
    /* [in] */ IParcel* param)
{
    VALIDATE_NOT_NULL(target);
//    VALIDATE_NOT_NULL(param);

    String str;
    ExternalRepresentation(str);
    param->WriteString(str);

    target->SendMessage(message, param);

    return NOERROR;
}

ECode CBrowserFrame::DocumentAsText(
    /* [in] */ IApartment* target,
    /* [in] */ Int32 message,
    /* [in] */ IParcel* param)
{
    VALIDATE_NOT_NULL(target);
 //   VALIDATE_NOT_NULL(param);

    String str;
    DocumentAsText(str);
    param->WriteString(str);

    target->SendMessage(message, param);

    return NOERROR;
}

ECode CBrowserFrame::HandleUrl(
    /* [in] */ const String& url,
    /* [out] */ Boolean* outFlag)
{
//    VALIDATE_NOT_NULL(outFlag);

    if (mLoadInitFromJava == TRUE && outFlag) {
        *outFlag = FALSE;
        return NOERROR;
    }

    Boolean flag = FALSE;
    mCallbackProxy->ShouldOverrideUrlLoading(url, &flag);
    if (flag) {
        // if the url is hijacked, reset the state of the BrowserFrame
        DidFirstLayout();

        if (outFlag) {
            *outFlag = TRUE;            
        }
        
        return NOERROR;
    } else {

        if (outFlag) {
            *outFlag = FALSE;
        }
        
        return NOERROR;
    }

    return NOERROR;
}

ECode CBrowserFrame::AddJavascriptInterface(
    /* [in] */ IInterface* obj,
    /* [in] */ const String& interfaceName)
{
/*    if (mJSInterfaceMap == NULL)
    {
        mJSInterfaceMap = new HashMap<String, Object>();
    }

    if (mJSInterfaceMap.containsKey(interfaceName))
    {
        mJSInterfaceMap.remove(interfaceName);
    }

    mJSInterfaceMap.put(interfaceName, obj);
*/
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBrowserFrame::NativeDestroyFrame()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBrowserFrame::Reload(
    /* [in] */ Boolean allowStale)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBrowserFrame::StringByEvaluatingJavaScriptFromString(
    /* [in] */ const String& script,
    /* [out] */ String* scriptString)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBrowserFrame::CacheDisabled(
    /* [out] */ Boolean* outFlag)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBrowserFrame::ClearCache()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBrowserFrame::StopLoading()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBrowserFrame::DocumentHasImages(
    /* [out] */ Boolean* outFlag)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBrowserFrame::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Handle32 webViewCore,
    /* [in] */ ICallbackProxy* proxy,
    /* [in] */ Handle32 settings,
    /* [in] */ IObjectStringMap* javascriptInterfaces)
{
    VALIDATE_NOT_NULL(context);
    VALIDATE_NOT_NULL(webViewCore);
    VALIDATE_NOT_NULL(proxy);
    VALIDATE_NOT_NULL(javascriptInterfaces);

    AutoPtr<IContext> appContext;
    context->GetApplicationContext((IContext**)&appContext);

    // Create a global JWebCoreJavaBridge to handle timers and
    // cookies in the WebCore thread.
    if (sJavaBridge == NULL) {
        sJavaBridge = new JWebCoreJavaBridge();
        // set WebCore native cache size
        AutoPtr<IActivityManager> am;
        context->GetSystemService(Context_ACTIVITY_SERVICE, (IInterface**)&am);

     /*   if (am.getMemoryClass() > 16) {
            sJavaBridge.setCacheSize(8 * 1024 * 1024);
        } else {
            sJavaBridge.setCacheSize(4 * 1024 * 1024);
        }*/

        // initialize CacheManager
        CCacheManager* cacheManager;
        ECode ec = CCacheManager::AcquireSingletonByFriend(&cacheManager);
        assert(!FAILED(ec));

        cacheManager->Init(appContext);
        // create CookieSyncManager with current Context
        CookieSyncManager::CreateInstance(appContext);
        // create PluginManager with current Context
//        CPluginManager::CreateInstance(appContext, NULL);
    }

    if (sConfigCallback == NULL) {
        AutoPtr<IWindowManager> windowManager;
        context->GetSystemService(Context_WINDOW_SERVICE, (IInterface**)&windowManager);

//        sConfigCallback = new ConfigCallback(pWindowManager);
        ViewRoot::AddConfigCallback(sConfigCallback);
    }
    sConfigCallback->AddHandler(this);

    mJSInterfaceMap = javascriptInterfaces;

    mSettings = (WebSettings*)settings;
    mContext = context;
    mCallbackProxy = proxy;

    //CWebViewDatabase::CreateInstance((IContext*)appContext, (IWebViewDatabase**)&mDatabase);
    AutoPtr<IWebViewDatabase> database;
    CWebViewDatabase::AcquireSingleton((IWebViewDatabase**)&database);
    database->GetInstance((IContext*)appContext, (IWebViewDatabase**)&mDatabase);

    mWebViewCore = (WebViewCore*)webViewCore;

    AutoPtr<IAssetManager> am;
    context->GetAssets((IAssetManager**)&am);
    AutoPtr<IWebBackForwardList> list;
    proxy->GetBackForwardList((IWebBackForwardList**)&list);
    NativeCreateFrame((WebViewCore*)webViewCore, am, list);

/*    if (DebugFlags.BROWSER_FRAME) {
        Log.v(LOGTAG, "BrowserFrame constructor: this=" + this);
    }*/

    return NOERROR;
}

/*********************************Packet*************************************/
/* package */
CARAPI_(Boolean) CBrowserFrame::Committed()
{
    return mCommitted;
}

/* package */
CARAPI_(Boolean) CBrowserFrame::FirstLayoutDone()
{
    return mFirstLayoutDone;
}

/* package */
CARAPI_(Int32) CBrowserFrame::LoadType()
{
    return mLoadType;
}

/* package */
CARAPI_(void) CBrowserFrame::DidFirstLayout()
{
    if (!mFirstLayoutDone) {
        mFirstLayoutDone = TRUE;
        // ensure {@link WebViewCore#webkitDraw} is called as we were
        // blocking the update in {@link #loadStarted}
        mWebViewCore->ContentDraw();
    }
}

/**
 * We have received an SSL certificate for the main top-level page.
 *
 * !!!Called from the network thread!!!
 */
CARAPI_(void) CBrowserFrame::Certificate(
    /* [in] */ ISslCertificate* certificate)
{

}

/**
 * Get the CallbackProxy for sending messages to the UI thread.
 */
/* package */
CARAPI_(AutoPtr<ICallbackProxy>) CBrowserFrame::GetCallbackProxy() const
{
    return mCallbackProxy;
}


/**
 * Returns the User Agent used by this frame
 */
CARAPI_(void) CBrowserFrame::GetUserAgentString(
    /* [out] */ String& str) const
{
    str = *(mSettings->GetUserAgentString());
}

CARAPI_(void) CBrowserFrame::GetRawResFilename(
    /* [in] */ Int32 id,
    /* [out] */ String& filename) const
{}



/******************************ConfigCallback**********************************/

CBrowserFrame::ConfigCallback::ConfigCallback(
    /* [in] */ ILocalWindowManager* wm)
{
    assert(wm != NULL);
    mWindowManager = wm;
}

CARAPI_(void) CBrowserFrame::ConfigCallback::AddHandler(
    /* [in] */ IApartment* h)
{
}

ECode CBrowserFrame::ConfigCallback::OnConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    if (mHandlers.GetSize() == 0) {
        return NOERROR;
    }

    AutoPtr<IDisplay> display;
    Int32 orientation = 0;
    mWindowManager->GetDefaultDisplay((IDisplay**)&display);
//    display->GetOrientation(&orientation);
    switch (orientation) {
        case Surface_ROTATION_90:
            orientation = 90;
            break;
        case Surface_ROTATION_180:
            orientation = 180;
            break;
        case Surface_ROTATION_270:
            orientation = -90;
            break;
        case Surface_ROTATION_0:
            orientation = 0;
            break;
        default:
            break;
    }

    {
        Mutex::Autolock lock(mMutex);

        // Create a list of handlers to remove. Go ahead and make it
        // the same size to avoid resizing.
        Vector<AutoPtr<IApartment> > handlersToRemove;// = new ArrayList<WeakReference>(mHandlers.size());
        Int32 size = mHandlers.GetSize();
        for (Int32 i = 0; i < size; i++) {
        //for (AutoPtr<IApartment> wh : mHandlers) {
            //Handler h = wh.get();
            AutoPtr<IApartment> h = mHandlers[i];
            if (h != NULL) {
                
                ECode (STDCALL CBrowserFrame::*pHandlerFunc)(Int32);

                pHandlerFunc = &CBrowserFrame::HandleOrientationChanged;

                AutoPtr<IParcel> params;
                CCallbackParcel::New((IParcel**)&params);
                params->WriteInt32(orientation);
                h->SendMessage(*(Handle32*)&pHandlerFunc, params);
            } else {
         //       handlersToRemove.add(wh);
            }
        }
        // Now remove all the null references.
      //  for (WeakReference weak : handlersToRemove) {
      //      mHandlers.remove(weak);
      //  }
    }

    return NOERROR;
}

ECode CBrowserFrame::ConfigCallback::OnLowMemory()
{
    return E_NOT_IMPLEMENTED;
}

/**
 * native callback
 * Report an error to an activity.
 * @param errorCode The HTTP error code.
 * @param description A String description.
 * TODO: Report all errors including resource errors but include some kind
 * of domain identifier. Change errorCode to an enum for a cleaner
 * interface.
 */
CARAPI_(void) CBrowserFrame::ReportError(
    /* [in] */ const Int32 errorCode,
    /* [in] */ const String description,
    /* [in] */ const String failingUrl)
{
    // As this is called for the main resource and loading will be stopped
    // after, reset the state variables.
    ResetLoadingStates();
    mCallbackProxy->OnReceivedError(errorCode, description, failingUrl);
}

CARAPI_(void) CBrowserFrame::ResetLoadingStates()
{
    mCommitted = TRUE;
    mFirstLayoutDone = TRUE;
}

/**
 * native callback
 * Indicates the beginning of a new load.
 * This method will be called once for the main frame.
 */
CARAPI_(void) CBrowserFrame::LoadStarted(
    /* [in] */ const String& url,
    /* [in] */ IBitmap* favicon,
    /* [in] */ Int32 loadType,
    /* [in] */ Boolean isMainFrame)
{
    assert (favicon != NULL);

    mIsMainFrame = isMainFrame;

    if (isMainFrame || loadType == FRAME_LOADTYPE_STANDARD) {
        mLoadType = loadType;

        if (isMainFrame) {
            // Call onPageStarted for main frames.
            mCallbackProxy->OnPageStarted(url, favicon);
            // as didFirstLayout() is only called for the main frame, reset 
            // mFirstLayoutDone only for the main frames
            mFirstLayoutDone = FALSE;
            mCommitted = FALSE;
            // remove pending draw to block update until mFirstLayoutDone is
            // set to true in didFirstLayout()
            mWebViewCore->RemoveMessages(WebViewCore::EventHub::WEBKIT_DRAW);
        }

        // Note: only saves committed form data in standard load
        if (loadType == FRAME_LOADTYPE_STANDARD
                && mSettings->GetSaveFormData()) {
            AutoPtr<IWebHistoryItem> h;
            AutoPtr<IWebBackForwardList> wfl;
            mCallbackProxy->GetBackForwardList((IWebBackForwardList**)&wfl);
            assert(wfl.Get());
            wfl->GetCurrentItem((IWebHistoryItem**)&h);
            if (h != NULL) {
                String currentUrl;
                h->GetUrl(&currentUrl);
                if (currentUrl.IsNullOrEmpty()) {
//                    ((CWebViewDatabase*)mDatabase.Get())->SetFormData(currentUrl, GetFormTextData());
                }
            }
        }
    }
}

/**
 * native callback
 * Indicates the WebKit has committed to the new load
 */
CARAPI_(void) CBrowserFrame::TransitionToCommitted(
    /* [in] */ Int32 loadType,
    /* [in] */ Boolean isMainFrame)
{
    // loadType is not used yet
    if (isMainFrame) {
        mCommitted = TRUE;
        AutoPtr<IWebView> webView;
        webView = mWebViewCore->GetWebView();
        ((CWebView*)webView.Get())->mViewManager->PostResetStateAll();
    }
}

/**
 * native callback
 * <p>
 * Indicates the end of a new load.
 * This method will be called once for the main frame.
 */
CARAPI_(void) CBrowserFrame::LoadFinished(
    /* [in] */ const String& url,
    /* [in] */ Int32 loadType,
    /* [in] */ Boolean isMainFrame)
{
    // mIsMainFrame and isMainFrame are better be equal!!!

    if (isMainFrame || loadType == FRAME_LOADTYPE_STANDARD) {
        if (isMainFrame) {
            ResetLoadingStates();
            ((CCallbackProxy*)mCallbackProxy.Get())->SwitchOutDrawHistory();
            mCallbackProxy->OnPageFinished(url);
        }
    }
}

/**
 * Punch-through for WebCore to set the document
 * title. Inform the Activity of the new title.
 * @param title The new title of the document.
 */
CARAPI_(void) CBrowserFrame::SetTitle(
    /* [in] */ const String& title)
{
    // FIXME: The activity must call getTitle (a native method) to get the
    // title. We should try and cache the title if we can also keep it in
    // sync with the document.
    mCallbackProxy->OnReceivedTitle(title);
}

/**
 * Return the render tree as a string
 */
CARAPI_(void) CBrowserFrame::ExternalRepresentation(
    /* [out] */ String& str)
{}

/**
 * Return the text drawn on the screen as a string
 */
CARAPI_(void) CBrowserFrame::DocumentAsText(
    /* [out] */ String& str)
{}

/*
 * This method is called by WebCore to inform the frame that
 * the Javascript window object has been cleared.
 * We should re-attach any attached js interfaces.
 */
CARAPI_(void) CBrowserFrame::WindowObjectCleared(
    /* [in] */ Int32 nativeFramePointer)
{
#if 0
    if (mJSInterfaceMap != NULL) {
        //Iterator iter = mJSInterfaceMap.keySet().iterator();
        Int32 size = 0, i;
        mJSInterfaceMap->GetSize(&size);

        ArrayOf<String>* Keys = ArrayOf<String>::Alloc(size*sizeof(String));
        while (iter.hasNext())  {
            String interfaceName = (String) iter.next();
            nativeAddJavascriptInterface(nativeFramePointer,
                    mJSInterfaceMap.get(interfaceName), interfaceName);
        }
    }
#endif
}

/**
 * Called by JNI.  Given a URI, find the associated file and return its size
 * @param uri A String representing the URI of the desired file.
 * @return int The size of the given file.
 */
CARAPI_(Int32) CBrowserFrame::GetFileSize(
    /* [in] */ const String& uri) const
{}

/**
 * Called by JNI.  Given a URI, a buffer, and an offset into the buffer,
 * copy the resource into buffer.
 * @param uri A String representing the URI of the desired file.
 * @param buffer The byte array to copy the data into.
 * @param offset The offet into buffer to place the data.
 * @param expectedSize The size that the buffer has allocated for this file.
 * @return int The size of the given file, or zero if it fails.
 */
CARAPI_(Int32) CBrowserFrame::GetFile(
    /* [in] */ const String& uri,
    /* [in] */ ArrayOf<Byte> buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 expectedSize) const
{}

/**
 * Start loading a resource.
 * @param loaderHandle The native ResourceLoader that is the target of the
 *                     data.
 * @param url The url to load.
 * @param method The http method.
 * @param headers The http headers.
 * @param postData If the method is "POST" postData is sent as the request
 *                 body. Is null when empty.
 * @param postDataIdentifier If the post data contained form this is the form identifier, otherwise it is 0.
 * @param cacheMode The cache mode to use when loading this resource. See WebSettings.setCacheMode
 * @param mainResource True if the this resource is the main request, not a supporting resource
 * @param userGesture
 * @param synchronous True if the load is synchronous.
 * @return A newly created LoadListener object.
 */
CARAPI_(AutoPtr<LoadListener>) CBrowserFrame::StartLoadingResource(
    /* [in] */ Int32 loaderHandle,
    /* [in] */ const String& url,
    /* [in] */ const String& method,
    /* [in] */ IHashMap* headers,
    /* [in] */ ArrayOf<Byte> postData,
    /* [in] */ Int64 postDataIdentifier,
    /* [in] */ Int32 cacheMode,
    /* [in] */ Boolean mainResource,
    /* [in] */ Boolean userGesture,
    /* [in] */ Boolean synchronous,
    /* [in] */ const String& username,
    /* [in] */ const String& password)
{}

/**
 * Set the progress for the browser activity.  Called by native code.
 * Uses a delay so it does not happen too often.
 * @param newProgress An int between zero and one hundred representing
 *                    the current progress percentage of loading the page.
 */
CARAPI_(void) CBrowserFrame::SetProgress(
    /* [in] */ Int32 newProgress)
{
    mCallbackProxy->OnProgressChanged(newProgress);
    if (newProgress == 100) {
        ECode (STDCALL CBrowserFrame::*pHandlerFunc)();
        pHandlerFunc = &CBrowserFrame::HandleFrameCompleted;

        PostCppCallbackDelayed((Handle32)this, *(Handle32*)&pHandlerFunc, NULL, 0, 100);
    }
    // FIXME: Need to figure out a better way to switch out of the history
    // drawing mode. Maybe we can somehow compare the history picture with 
    // the current picture, and switch when it contains more content.
    if (mFirstLayoutDone && newProgress > TRANSITION_SWITCH_THRESHOLD) {
        ((CCallbackProxy*)mCallbackProxy.Get())->SwitchOutDrawHistory();
    }
}

/**
 * Send the icon to the activity for display.
 * @param icon A Bitmap representing a page's favicon.
 */
CARAPI_(void) CBrowserFrame::DidReceiveIcon(
    /* [in] */ IBitmap* icon)
{
    mCallbackProxy->OnReceivedIcon(icon);
}

// Called by JNI when an apple-touch-icon attribute was found.
CARAPI_(void) CBrowserFrame::DidReceiveTouchIconUrl(
    /* [in] */ const String& url,
    /* [in] */ Boolean precomposed)
{
    ((CCallbackProxy*)mCallbackProxy.Get())->OnReceivedTouchIconUrl(url, precomposed);
}

/**
 * Request a new window from the client.
 * @return The BrowserFrame object stored in the new WebView.
 */
CARAPI_(AutoPtr<IBrowserFrame>) CBrowserFrame::CreateWindow(
    /* [in] */ Boolean dialog,
    /* [in] */ Boolean userGesture)
{
    AutoPtr<IBrowserFrame> bf;
    mCallbackProxy->CreateWindow(dialog, userGesture, (IBrowserFrame**)&bf);

    return bf;
}

/**
 * Try to focus this WebView.
 */
CARAPI_(void) CBrowserFrame::RequestFocus()
{
    mCallbackProxy->OnRequestFocus();
}

/**
 * Close this frame and window.
 */
CARAPI_(void) CBrowserFrame::CloseWindow(
    /* [in] */ WebViewCore* w)
{
    assert(w != NULL);
    mCallbackProxy->OnCloseWindow(w->GetWebView());
}

CARAPI_(void) CBrowserFrame::DecidePolicyForFormResubmission(
    /* [in] */ Int32 policyFunction)
{}

/**
 * Tell the activity to update its global history.
 */
CARAPI_(void) CBrowserFrame::UpdateVisitedHistory(
    /* [in] */ const String& url,
    /* [in] */ Boolean isReload)
{
    mCallbackProxy->DoUpdateVisitedHistory(url, isReload);
}

CARAPI_(Float) CBrowserFrame::Density()
{}

//==========================================================================
// native functions
//==========================================================================

/**
 * Create a new native frame for a given WebView
 * @param w     A WebView that the frame draws into.
 * @param am    AssetManager to use to get assets.
 * @param list  The native side will add and remove items from this list as
 *              the native list changes.
 */
CARAPI_(void) CBrowserFrame::NativeCreateFrame(
    /* [in] */ WebViewCore* w,
    /* [in] */ IAssetManager* am,
    /* [in] */ IWebBackForwardList* list)
{}

CARAPI_(void) CBrowserFrame::NativeCallPolicyFunction(
    /* [in] */ Int32 policyFunction,
    /* [in] */ Int32 decision)
{}

/**
 * Go back or forward the number of steps given.
 * @param steps A negative or positive number indicating the direction
 *              and number of steps to move.
 */
CARAPI_(void) CBrowserFrame::NativeGoBackOrForward(
    /* [in] */ Int32 steps)
{}

/**
 * Add a javascript interface to the main frame.
 */
CARAPI_(void) CBrowserFrame::NativeAddJavascriptInterface(
    /* [in] */ Int32 nativeFramePointer,
    /* [in] */ IInterface* obj,
    /* [in] */ const String& interfaceName)
{}

/**
 * Enable or disable the native cache.
 */
/* FIXME: The native cache is always on for now until we have a better
 * solution for our 2 caches. */
CARAPI_(void) CBrowserFrame::SetCacheDisabled(
    /* [in] */ Boolean disabled)
{}

/**
 * Returns false if the url is bad.
 */
CARAPI_(void) CBrowserFrame::NativeLoadUrl(
    /* [in] */ CString url,
    /* [in] */ IObjectStringMap* headers)
{}

CARAPI_(void) CBrowserFrame::NativePostUrl(
    /* [in] */ CString url,
    /* [in] */ const ArrayOf<Byte>& postData)
{}

CARAPI_(void) CBrowserFrame::NativeLoadData(
    /* [in] */ CString baseUrl,
    /* [in] */ CString data,
    /* [in] */ CString mimeType,
    /* [in] */ CString encoding,
    /* [in] */ CString historyUrl)
{}


CARAPI_(void) CBrowserFrame::NativeStopLoading()
{}

/**
 * @return TRUE if there is a password field in the current frame
 */
CARAPI_(Boolean) CBrowserFrame::HasPasswordField()
{}

/**
 * Get username and password in the current frame. If found, String[0] is
 * username and String[1] is password. Otherwise return NULL.
 * @return String[]
 */
CARAPI_(void) CBrowserFrame::GetUsernamePassword(
    /* [in] */ String& str)
{}

/**
 * Set username and password to the proper fields in the current frame
 * @param username
 * @param password
 */
CARAPI_(void) CBrowserFrame::SetUsernamePassword(
    /* [in] */ CString username,
    /* [in] */ CString password)
{}

/**
 * Get form's "text" type data associated with the current frame.
 * @return HashMap If succeed, returns a list of name/value pair. Otherwise
 *         returns null.
 */
CARAPI_(AutoPtr<IHashMap>) CBrowserFrame::GetFormTextData() const
{}

CARAPI_(void) CBrowserFrame::NativeOrientationChanged(
    /* [in] */ Int32 orientation)
{}


CARAPI_(PInterface) CBrowserFrame::Probe(
    /* [in] */ REIID riid)
{}

CARAPI_(UInt32) CBrowserFrame::AddRef()
{}

CARAPI_(UInt32) CBrowserFrame::Release()
{}

ECode CBrowserFrame::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{}

/*******************************Message************************************/

ECode CBrowserFrame::Start(
    /* [in] */ ApartmentAttr attr)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CBrowserFrame::Finish()
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CBrowserFrame::PostCppCallback(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallback(target, func, params, id);
}

ECode CBrowserFrame::PostCppCallbackAtTime(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id,
    /* [in] */ Millisecond64 uptimeMillis)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallbackAtTime(target, func, params, id, uptimeMillis);
}

ECode CBrowserFrame::PostCppCallbackDelayed(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id,
    /* [in] */ Millisecond64 delayMillis)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallbackDelayed(target, func, params, id, delayMillis);
}

ECode CBrowserFrame::PostCppCallbackAtFrontOfQueue(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallbackAtFrontOfQueue(target, func, params, id);
}

ECode CBrowserFrame::RemoveCppCallbacks(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func)
{
    assert(mApartment != NULL);
    return mApartment->RemoveCppCallbacks(target, func);
}

ECode CBrowserFrame::RemoveCppCallbacksEx(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ Int32 id)
{
    assert(mApartment != NULL);
    return mApartment->RemoveCppCallbacksEx(target, func, id);
}

ECode CBrowserFrame::HasCppCallbacks(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [out] */ Boolean* result)
{
    assert(mApartment != NULL);
    return mApartment->HasCppCallbacks(target, func, result);
}

ECode CBrowserFrame::HasCppCallbacksEx(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ Int32 id,
    /* [out] */ Boolean* result)
{
    assert(mApartment != NULL);
    return mApartment->HasCppCallbacksEx(target, func, id, result);
}

ECode CBrowserFrame::SendMessage(
    /* [in] */ Int32 message,
    /* [in] */ IParcel* params)
{
    if (mBlockMessages) {
        return NOERROR;
    }

    if (message == POLICY_FUNCTION) {
        void (STDCALL CBrowserFrame::*pHandlerFunc)(Int32, Int32);
        pHandlerFunc = &CBrowserFrame::NativeCallPolicyFunction;
        return SendMessage(*(Handle32*)&pHandlerFunc, params);
    }

    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode CBrowserFrame::SendMessage(
    /* [in] */ Handle32 pvFunc,
    /* [in] */ IParcel* params)
{
    return mApartment->PostCppCallback((Handle32)this, pvFunc, params, 0);
}

ECode CBrowserFrame::SendMessageAtTime(
    /* [in] */ Handle32 pvFunc,
    /* [in] */ IParcel* params,
    /* [in] */ Millisecond64 uptimeMillis)
{
    return mApartment->PostCppCallbackAtTime(
        (Handle32)this, pvFunc, params, 0, uptimeMillis);
}

ECode CBrowserFrame::RemoveMessage(
    /* [in] */ Handle32 func)
{
    return mApartment->RemoveCppCallbacks((Handle32)this, func);
}

ECode CBrowserFrame::HandleOrientationChanged(
    /* [in] */ Int32 orientation)
{
    if (mBlockMessages) {
        return NOERROR;
    }

    if (mOrientation != orientation) {
        mOrientation = orientation;
        NativeOrientationChanged(orientation);
    }
}

ECode CBrowserFrame::HandleFrameCompleted()
{
    if (mBlockMessages) {
        return NOERROR;
    }

    if (mSettings->GetSavePassword() && HasPasswordField()) {
        
        AutoPtr<IWebBackForwardList> wfl;
        AutoPtr<IWebHistoryItem> item;

        mCallbackProxy->GetBackForwardList((IWebBackForwardList**)&wfl);
        wfl->GetCurrentItem((IWebHistoryItem**)&item);
        if (item != NULL) {
//            WebAddress uri = new WebAddress(item.getUrl());
            String schemePlusHost;// = uri.mScheme + uri.mHost;
            Vector<String> up;
            ((CWebViewDatabase*)mDatabase.Get())->GetUsernamePassword(schemePlusHost, up);
            if (up.GetSize() != 0 && up[0].GetLength() != 0) {
                SetUsernamePassword(up[0], up[1]);
            }
        }
    }

//    WebViewWorker::GetHandler().sendEmptyMessage(WebViewWorker.MSG_TRIM_CACHE);
}