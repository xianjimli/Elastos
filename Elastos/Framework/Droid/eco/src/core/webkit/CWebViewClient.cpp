
#include "webkit/CWebViewClient.h"
ECode CWebViewClient::ShouldOverrideUrlLoading(
    /* [in] */ IWebView * pView,
    /* [in] */ CString url,
    /* [out] */ Boolean * pFlag)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebViewClient::OnPageStarted(
    /* [in] */ IWebView * pView,
    /* [in] */ const String& url,
    /* [in] */ IBitmap * pFavicon)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebViewClient::OnPageFinished(
    /* [in] */ IWebView * pView,
    /* [in] */ CString url)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebViewClient::OnLoadResource(
    /* [in] */ IWebView * pView,
    /* [in] */ CString url)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebViewClient::OnTooManyRedirects(
    /* [in] */ IWebView * pView,
    /* [in] */ IMessage * pCancelMsg,
    /* [in] */ IMessage * pContinueMsg)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebViewClient::OnReceivedError(
    /* [in] */ IWebView * pView,
    /* [in] */ Int32 errorCode,
    /* [in] */ CString description,
    /* [in] */ CString failingUrl)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebViewClient::OnFormResubmission(
    /* [in] */ IWebView * pView,
    /* [in] */ IMessage * pDontResend,
    /* [in] */ IMessage * pResend)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebViewClient::DoUpdateVisitedHistory(
    /* [in] */ IWebView * pView,
    /* [in] */ CString url,
    /* [in] */ Boolean isReload)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebViewClient::OnReceivedSslError(
    /* [in] */ IWebView * pView,
    /* [in] */ ISslErrorHandler * pHandler,
    /* [in] */ ISslError * pError)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebViewClient::OnReceivedHttpAuthRequest(
    /* [in] */ IWebView * pView,
    /* [in] */ IHttpAuthHandler * pHandler,
    /* [in] */ CString host,
    /* [in] */ CString realm)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebViewClient::ShouldOverrideKeyEvent(
    /* [in] */ IWebView * pView,
    /* [in] */ IKeyEvent * pEvent,
    /* [out] */ Boolean * pFlag)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebViewClient::OnUnhandledKeyEvent(
    /* [in] */ IWebView * pView,
    /* [in] */ IKeyEvent * pEvent)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebViewClient::OnScaleChanged(
    /* [in] */ IWebView * pView,
    /* [in] */ Float oldScale,
    /* [in] */ Float newScale)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

