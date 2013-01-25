
#ifndef __CWEBVIEWCLIENT_H__
#define __CWEBVIEWCLIENT_H__

#include "_CWebViewClient.h"

CarClass(CWebViewClient)
{
public:
    CARAPI ShouldOverrideUrlLoading(
        /* [out] */ Boolean * pFlag,
        /* [in] */ IWebView * pView,
        /* [in] */ CString url);

    CARAPI OnPageStarted(
        /* [in] */ IWebView * pView,
        /* [in] */ const String& url,
        /* [in] */ IBitmap * pFavicon);

    CARAPI OnPageFinished(
        /* [in] */ IWebView * pView,
        /* [in] */ CString url);

    CARAPI OnLoadResource(
        /* [in] */ IWebView * pView,
        /* [in] */ CString url);

    CARAPI OnTooManyRedirects(
        /* [in] */ IWebView * pView,
        /* [in] */ IMessage * pCancelMsg,
        /* [in] */ IMessage * pContinueMsg);

    CARAPI OnReceivedError(
        /* [in] */ IWebView * pView,
        /* [in] */ Int32 errorCode,
        /* [in] */ CString description,
        /* [in] */ CString failingUrl);

    CARAPI OnFormResubmission(
        /* [in] */ IWebView * pView,
        /* [in] */ IMessage * pDontResend,
        /* [in] */ IMessage * pResend);

    CARAPI DoUpdateVisitedHistory(
        /* [in] */ IWebView * pView,
        /* [in] */ CString url,
        /* [in] */ Boolean isReload);

    CARAPI OnReceivedSslError(
        /* [in] */ IWebView * pView,
        /* [in] */ ISslErrorHandler * pHandler,
        /* [in] */ ISslError * pError);

    CARAPI OnReceivedHttpAuthRequest(
        /* [in] */ IWebView * pView,
        /* [in] */ IHttpAuthHandler * pHandler,
        /* [in] */ CString host,
        /* [in] */ CString realm);

    CARAPI ShouldOverrideKeyEvent(
        /* [out] */ Boolean * pFlag,
        /* [in] */ IWebView * pView,
        /* [in] */ IKeyEvent * pEvent);

    CARAPI OnUnhandledKeyEvent(
        /* [in] */ IWebView * pView,
        /* [in] */ IKeyEvent * pEvent);

    CARAPI OnScaleChanged(
        /* [in] */ IWebView * pView,
        /* [in] */ Float oldScale,
        /* [in] */ Float newScale);

private:
    // TODO: Add your private member variables here.
};

#endif // __CWEBVIEWCLIENT_H__
