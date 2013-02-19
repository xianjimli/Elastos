
#ifndef __CBROWSERFRAME_H__
#define __CBROWSERFRAME_H__

#include "_CBrowserFrame.h"

CarClass(CBrowserFrame)
{
public:
    CARAPI LoadUrl(
        /* [in] */ CString url,
        /* [in] */ IObjectStringMap * pExtraHeaders);

    CARAPI PostUrl(
        /* [in] */ const String& url,
        /* [in] */ const ArrayOf<Byte> & data);

    CARAPI LoadData(
        /* [in] */ CString baseUrl,
        /* [in] */ CString data,
        /* [in] */ CString mimeType,
        /* [in] */ CString encoding,
        /* [in] */ CString historyUrl);

    CARAPI GoBackOrForward(
        /* [in] */ Int32 steps);

    CARAPI Destroy();

    CARAPI HandleMessage(
        /* [in] */ IMessage * pMsg);

    CARAPI ExternalRepresentation(
        /* [in] */ IMessage * pCallBack);

    CARAPI DocumentAsText(
        /* [in] */ IMessage * pCallBack);

    CARAPI HandleUrl(
        /* [in] */ CString url,
        /* [out] */ Boolean * pFlag);

    CARAPI AddJavascriptInterface(
        /* [in] */ IInterface * pObj,
        /* [in] */ CString interfaceName);

    CARAPI NativeDestroyFrame();

    CARAPI Reload(
        /* [in] */ Boolean allowStale);

    CARAPI StringByEvaluatingJavaScriptFromString(
        /* [in] */ CString script,
        /* [out] */ String * pScriptString);

    CARAPI CacheDisabled(
        /* [out] */ Boolean * pFlag);

    CARAPI ClearCache();

    CARAPI StopLoading();

    CARAPI DocumentHasImages(
        /* [out] */ Boolean * pFlag);

    CARAPI constructor(
        /* [in] */ IContext * pContext,
        /* [in] */ Handle32 webViewCore,
        /* [in] */ ICallbackProxy * pProxy,
        /* [in] */ Handle32 settings,
        /* [in] */ IObjectStringMap * pJavascriptInterfaces);

private:
    // TODO: Add your private member variables here.
};

#endif // __CBROWSERFRAME_H__
