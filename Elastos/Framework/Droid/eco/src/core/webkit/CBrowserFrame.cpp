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

const CString CBrowserFrame::LOGTAG = "webkit";

AutoPtr<JWebCoreJavaBridge> CBrowserFrame::sJavaBridge;

AutoPtr<CBrowserFrame::ConfigCallback> CBrowserFrame::sConfigCallback;

ECode CBrowserFrame::LoadUrl(
    /* [in] */ const String& url,
    /* [in] */ IObjectStringMap* extraHeaders)
{
    VALIDATE_NOT_NULL(extraHeaders);

    mLoadInitFromJava = TRUE;
    
    AutoPtr<IURLUtil> urlUtil;
    CURLUtil::AcquireSingleton((IURLUtil**)&urlUtil);
    Boolean flag = FALSE;
    urlUtil->IsJavaScriptUrl(url, &flag);

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

ECode CBrowserFrame::HandleMessage(
    /* [in] */ IMessage* msg)
{
    VALIDATE_NOT_NULL(msg);

    if (mBlockMessages) {
        return NOERROR;
    }

    switch (/*pMsg.what*/1) {
        case FRAME_COMPLETED:
        {
            if (mSettings->GetSavePassword() && HasPasswordField()) {
                WebHistoryItem* item = NULL;
                AutoPtr<IWebBackForwardList> wfl = NULL;                
                
                mCallbackProxy->GetBackForwardList((IWebBackForwardList**)&wfl);
               // item = pWFL->GetCurrentItem();
                if (item != NULL) {
                 //   WebAddress uri = new WebAddress(item.getUrl());
                 //   String schemePlusHost = uri.mScheme + uri.mHost;
                 //   ArrayOf<String> up = mDatabase->GetUsernamePassword(schemePlusHost);
                 //   if (/*up != NULL &&*/ up[0] != NULL)
                    {
                 //       SetUsernamePassword((const char*)up[0], (const char*)up[1]);
                    }
                }
            }
         //   WebViewWorker::GetHandler()->SendEmptyMessage(WebViewWorker.MSG_TRIM_CACHE);
            break;
        }

        case POLICY_FUNCTION:
        {
        //    NativeCallPolicyFunction(pMsg->arg1, pMsg->arg2);
            break;
        }

        case ORIENTATION_CHANGED:
        {
           /* if (mOrientation != pMsg->arg1)
            {
                mOrientation = pMsg->arg1;
                NativeOrientationChanged(pMsg->arg1);
            }*/
            break;
        }

        default:
            break;
    }

    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBrowserFrame::ExternalRepresentation(
    /* [in] */ IMessage* callBack)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBrowserFrame::DocumentAsText(
    /* [in] */ IMessage * pCallBack)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBrowserFrame::HandleUrl(
    /* [in] */ const String& url,
    /* [out] */ Boolean* outFlag)
{
    VALIDATE_NOT_NULL(outFlag);

    if (mLoadInitFromJava == TRUE) {
        *outFlag = FALSE;
        return NOERROR;
    }

    Boolean flag = FALSE;
    mCallbackProxy->ShouldOverrideUrlLoading(url, &flag);
    if (flag) {
        // if the url is hijacked, reset the state of the BrowserFrame
        DidFirstLayout();

        *outFlag = TRUE;
        return NOERROR;
    } else {        
        *outFlag = FALSE;
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
    return FALSE;
}

/* package */
CARAPI_(Boolean) CBrowserFrame::FirstLayoutDone()
{
    return FALSE;
}

/* package */
CARAPI_(Int32) CBrowserFrame::LoadType()
{
    return 0;
}

/* package */
CARAPI_(void) CBrowserFrame::DidFirstLayout()
{

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
    return NULL;
}


/**
 * Returns the User Agent used by this frame
 */
CARAPI_(void) CBrowserFrame::GetUserAgentString(
    /* [out] */ String& str) const
{

}

CARAPI_(void) CBrowserFrame::GetRawResFilename(
    /* [in] */ Int32 id,
    /* [out] */ String& filename) const
{

}



/******************************ConfigCallback**********************************/

CBrowserFrame::ConfigCallback::ConfigCallback(
    /* [in] */ ILocalWindowManager* wm)
{
    assert(wm != NULL);
    mWindowManager = wm;
}

CARAPI_(void) CBrowserFrame::ConfigCallback::AddHandler(
    /* [in] */ IHandler* h)
{
}

ECode CBrowserFrame::ConfigCallback::OnConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    return E_NOT_IMPLEMENTED;
}

ECode CBrowserFrame::ConfigCallback::OnLowMemory()
{
    return E_NOT_IMPLEMENTED;
}
