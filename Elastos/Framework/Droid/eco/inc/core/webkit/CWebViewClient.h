
#ifndef __CWEBVIEWCLIENT_H__
#define __CWEBVIEWCLIENT_H__

#include "_CWebViewClient.h"
#include "ext/frameworkext.h"

CarClass(CWebViewClient)
{
public:
    CARAPI ShouldOverrideUrlLoading(
        /* [in] */ IWebView* view,
        /* [in] */ const String& url,
        /* [out] */ Boolean* flag);

    CARAPI OnPageStarted(
        /* [in] */ IWebView* view,
        /* [in] */ const String& url,
        /* [in] */ IBitmap* favicon);

    CARAPI OnPageFinished(
        /* [in] */ IWebView* view,
        /* [in] */ const String& url);

    CARAPI OnLoadResource(
        /* [in] */ IWebView* view,
        /* [in] */ const String& url);

    CARAPI OnTooManyRedirects(
        /* [in] */ IWebView* view,
        /* [in] */ IMessage* cancelMsg,
        /* [in] */ IMessage* continueMsg);

    CARAPI OnReceivedError(
        /* [in] */ IWebView* view,
        /* [in] */ Int32 errorCode,
        /* [in] */ const String& description,
        /* [in] */ const String& failingUrl);

    CARAPI OnFormResubmission(
        /* [in] */ IWebView* view,
        /* [in] */ IMessage* dontResend,
        /* [in] */ IMessage* resend);

    CARAPI DoUpdateVisitedHistory(
        /* [in] */ IWebView* view,
        /* [in] */ const String& url,
        /* [in] */ Boolean isReload);

    CARAPI OnReceivedSslError(
        /* [in] */ IWebView* view,
        /* [in] */ ISslErrorHandler* handler,
        /* [in] */ ISslError* error);

    CARAPI OnReceivedHttpAuthRequest(
        /* [in] */ IWebView* view,
        /* [in] */ IHttpAuthHandler* handler,
        /* [in] */ const String& host,
        /* [in] */ const String& realm);

    CARAPI ShouldOverrideKeyEvent(
        /* [in] */ IWebView* view,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* flag);

    CARAPI OnUnhandledKeyEvent(
        /* [in] */ IWebView* view,
        /* [in] */ IKeyEvent* event);

    CARAPI OnScaleChanged(
        /* [in] */ IWebView* view,
        /* [in] */ Float oldScale,
        /* [in] */ Float newScale);

private:
    // TODO: Add your private member variables here.
};

#endif // __CWEBVIEWCLIENT_H__
