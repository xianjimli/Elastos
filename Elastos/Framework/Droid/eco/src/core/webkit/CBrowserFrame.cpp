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

const char* CBrowserFrame::LOGTAG = "webkit";

AutoPtr<JWebCoreJavaBridge> CBrowserFrame::sJavaBridge = NULL;

AutoPtr<CBrowserFrame::ConfigCallback> CBrowserFrame::sConfigCallback = NULL;

ECode CBrowserFrame::LoadUrl(
    /* [in] */ const String& url,
    /* [in] */ IObjectStringMap * pExtraHeaders)
{
    if (pExtraHeaders == NULL)
    {
        return E_INVALID_ARGUMENT;
    }

    mLoadInitFromJava = true;
    
    IURLUtil* pURLUtil = NULL;
    CURLUtil::AcquireSingleton(&pURLUtil);
    Boolean flag = false;
    pURLUtil->IsJavaScriptUrl(url, &flag);

    if (flag) 
    {
        // strip off the scheme and evaluate the string
        StringByEvaluatingJavaScriptFromString(
                url.Substring(strlen("javascript:")), NULL);
    }
    else
    {
        NativeLoadUrl((const char*)url, pExtraHeaders);
    }
    mLoadInitFromJava = false;

    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBrowserFrame::PostUrl(
    /* [in] */ const String& url,
    /* [in] */ const ArrayOf<Byte> & data)
{
    mLoadInitFromJava = true;
    NativePostUrl((const char*)url, data);
    mLoadInitFromJava = false;

    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
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

    mLoadInitFromJava = true;
    if (_historyUrl == NULL || _historyUrl.GetLength() == 0)
    {
        _historyUrl = "about:blank";
    }
    if (_data == NULL)
    {
        _data = "";
    }
    
    // Setup defaults for missing values. These defaults where taken from
    // WebKit's WebFrame.mm
    if (_baseUrl == NULL || _baseUrl.GetLength() == 0)
    {
        _baseUrl = "about:blank";
    }
    if (_mimeType == NULL || _mimeType.GetLength() == 0)
    {
        _mimeType = "text/html";
    }
    NativeLoadData(_baseUrl, _data, _mimeType, _encoding, _historyUrl);
    mLoadInitFromJava = false;

    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBrowserFrame::GoBackOrForward(
    /* [in] */ Int32 steps)
{
    mLoadInitFromJava = true;
    NativeGoBackOrForward(steps);
    mLoadInitFromJava = false;

    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBrowserFrame::Destroy()
{
    NativeDestroyFrame();
    mBlockMessages = true;
//    RemoveCallbacksAndMessages(NULL);

    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBrowserFrame::HandleMessage(
    /* [in] */ IMessage * pMsg)
{
    if (pMsg == NULL)
    {
        return E_INVALID_ARGUMENT;
    }

    if (mBlockMessages) 
    {
            return E_NOT_IMPLEMENTED;
    }
    switch (/*pMsg.what*/1) 
    {
        case FRAME_COMPLETED: 
        {
            if (mSettings->GetSavePassword() && HasPasswordField()) 
            {
                WebHistoryItem* item = NULL;
                IWebBackForwardList* pWFL = NULL;                
                
                mCallbackProxy->GetBackForwardList(&pWFL);
               // item = pWFL->GetCurrentItem();
                if (item != NULL) 
                {
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
    /* [in] */ IMessage * pCallBack)
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
    /* [out] */ Boolean * pFlag)
{
    if (pFlag == NULL)
    {
        return E_INVALID_ARGUMENT;
    }

    if (mLoadInitFromJava == true) 
    {
        *pFlag = false;
        return E_NOT_IMPLEMENTED;
    }

    Boolean flag = false;
    mCallbackProxy->ShouldOverrideUrlLoading(url, &flag);
    if (flag)
    {
        // if the url is hijacked, reset the state of the BrowserFrame
        DidFirstLayout();

        *pFlag = true;
        return E_NOT_IMPLEMENTED;
    }
    else
    {        
        *pFlag = false;
        return E_NOT_IMPLEMENTED;
    }

    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBrowserFrame::AddJavascriptInterface(
    /* [in] */ IInterface * pObj,
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
    /* [out] */ String * pScriptString)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBrowserFrame::CacheDisabled(
    /* [out] */ Boolean * pFlag)
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
    /* [out] */ Boolean * pFlag)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBrowserFrame::constructor(
    /* [in] */ IContext * pContext,
    /* [in] */ Handle32 webViewCore,
    /* [in] */ ICallbackProxy * pProxy,
    /* [in] */ Handle32 settings,
    /* [in] */ IObjectStringMap * pJavascriptInterfaces)
{
    if (pContext == NULL || webViewCore == 0 
        || pProxy == NULL || settings == 0 || pJavascriptInterfaces == NULL)
    {
        return E_INVALID_ARGUMENT;
    }

    IContext* appContext;
    pContext->GetApplicationContext((IContext**)&appContext);

    // Create a global JWebCoreJavaBridge to handle timers and
    // cookies in the WebCore thread.
    if (sJavaBridge == NULL) 
    {
        sJavaBridge = new JWebCoreJavaBridge();
        // set WebCore native cache size
        IActivityManager* am;
        pContext->GetSystemService(Context_ACTIVITY_SERVICE, (IInterface**)&am);

     /*   if (am.getMemoryClass() > 16) {
            sJavaBridge.setCacheSize(8 * 1024 * 1024);
        } else {
            sJavaBridge.setCacheSize(4 * 1024 * 1024);
        }*/

        // initialize CacheManager
        CCacheManager* pCacheManager;
        ECode ec = CCacheManager::AcquireSingletonByFriend(&pCacheManager);
        assert(!FAILED(ec));

        pCacheManager->Init(appContext);
        // create CookieSyncManager with current Context
        CookieSyncManager::CreateInstance(appContext);
        // create PluginManager with current Context
//        CPluginManager::CreateInstance(appContext, NULL);
    }

    if (sConfigCallback == NULL)
    {
        IWindowManager* pWindowManager;
        pContext->GetSystemService(Context_WINDOW_SERVICE, (IInterface**)&pWindowManager);

//        sConfigCallback = new ConfigCallback(pWindowManager);
        ViewRoot::AddConfigCallback(sConfigCallback);
    }
    sConfigCallback->AddHandler(this);

    mJSInterfaceMap = pJavascriptInterfaces;

    mSettings = (WebSettings*)settings;
    mContext = pContext;
    mCallbackProxy = pProxy;

    //CWebViewDatabase::CreateInstance((IContext*)appContext, (IWebViewDatabase**)&mDatabase);
    IWebViewDatabase* pDatabase = NULL;
    CWebViewDatabase::AcquireSingleton((IWebViewDatabase**)&pDatabase);
    pDatabase->GetInstance((IContext*)appContext, (IWebViewDatabase**)&mDatabase);

    mWebViewCore = (WebViewCore*)webViewCore;

    IAssetManager* am = NULL;
    pContext->GetAssets(&am);
    IWebBackForwardList* pList = NULL;
    pProxy->GetBackForwardList(&pList);
    NativeCreateFrame((WebViewCore*)webViewCore, am, pList);

/*    if (DebugFlags.BROWSER_FRAME) {
        Log.v(LOGTAG, "BrowserFrame constructor: this=" + this);
    }*/

    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/*********************************Packet*************************************/
/* package */
CARAPI_(Boolean) CBrowserFrame::Committed()
{
    return false;
}

/* package */
CARAPI_(Boolean) CBrowserFrame::FirstLayoutDone()
{
    return false;
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
    /* [in] */ IWindowManager* wm)
{
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
