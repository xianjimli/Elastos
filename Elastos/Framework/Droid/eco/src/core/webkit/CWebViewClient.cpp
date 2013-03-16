
#include "webkit/CWebViewClient.h"

ECode CWebViewClient::ShouldOverrideUrlLoading(
    /* [in] */ IWebView* view,
    /* [in] */ const String& url,
    /* [out] */ Boolean* flag)
{
    VALIDATE_NOT_NULL(flag);

    *flag = FALSE;

    return NOERROR;
}

ECode CWebViewClient::OnPageStarted(
    /* [in] */ IWebView* view,
    /* [in] */ const String& url,
    /* [in] */ IBitmap* favicon)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebViewClient::OnPageFinished(
    /* [in] */ IWebView* view,
    /* [in] */ const String& url)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebViewClient::OnLoadResource(
    /* [in] */ IWebView* view,
    /* [in] */ const String& url)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebViewClient::OnTooManyRedirects(
    /* [in] */ IWebView* view,
    /* [in] */ IMessage* cancelMsg,
    /* [in] */ IMessage* continueMsg)
{
    VALIDATE_NOT_NULL(cancelMsg);

//    pCancelMsg->SendToTarget();

    return NOERROR;
}

ECode CWebViewClient::OnReceivedError(
    /* [in] */ IWebView* view,
    /* [in] */ Int32 errorCode,
    /* [in] */ const String& description,
    /* [in] */ const String& failingUrl)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebViewClient::OnFormResubmission(
    /* [in] */ IWebView* view,
    /* [in] */ IMessage* dontResend,
    /* [in] */ IMessage* resend)
{
    VALIDATE_NOT_NULL(dontResend);

//    pDontResend->SendToTarget();

    return NOERROR;
}

ECode CWebViewClient::DoUpdateVisitedHistory(
    /* [in] */ IWebView* view,
    /* [in] */ const String& url,
    /* [in] */ Boolean isReload)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebViewClient::OnReceivedSslError(
    /* [in] */ IWebView* view,
    /* [in] */ ISslErrorHandler* handler,
    /* [in] */ ISslError* error)
{
    VALIDATE_NOT_NULL(handler);

    handler->Cancel();

    return NOERROR;
}

ECode CWebViewClient::OnReceivedHttpAuthRequest(
    /* [in] */ IWebView* view,
    /* [in] */ IHttpAuthHandler* handler,
    /* [in] */ const String& host,
    /* [in] */ const String& realm)
{
    VALIDATE_NOT_NULL(handler);

    handler->Cancel();

    return NOERROR;
}

ECode CWebViewClient::ShouldOverrideKeyEvent(
    /* [in] */ IWebView* view,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* flag)
{
    VALIDATE_NOT_NULL(flag);

    *flag = FALSE;

    return NOERROR;
}

ECode CWebViewClient::OnUnhandledKeyEvent(
    /* [in] */ IWebView* view,
    /* [in] */ IKeyEvent* event)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebViewClient::OnScaleChanged(
    /* [in] */ IWebView* View,
    /* [in] */ Float oldScale,
    /* [in] */ Float newScale)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

