
#include "webkit/CWebViewClient.h"

ECode CWebViewClient::ShouldOverrideUrlLoading(
    /* [in] */ IWebView * pView,
    /* [in] */ const String& url,
    /* [out] */ Boolean * pFlag)
{
    if (pFlag == NULL)
    {
        return E_INVALID_ARGUMENT;
    }

    *pFlag = false;

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
    /* [in] */ const String& url)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebViewClient::OnLoadResource(
    /* [in] */ IWebView * pView,
    /* [in] */ const String& url)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebViewClient::OnTooManyRedirects(
    /* [in] */ IWebView * pView,
    /* [in] */ IMessage * pCancelMsg,
    /* [in] */ IMessage * pContinueMsg)
{
    if (pCancelMsg == NULL)
    {
        return E_INVALID_ARGUMENT;
    }

//    pCancelMsg->SendToTarget();

    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebViewClient::OnReceivedError(
    /* [in] */ IWebView * pView,
    /* [in] */ Int32 errorCode,
    /* [in] */ const String& description,
    /* [in] */ const String& failingUrl)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebViewClient::OnFormResubmission(
    /* [in] */ IWebView * pView,
    /* [in] */ IMessage * pDontResend,
    /* [in] */ IMessage * pResend)
{
    if (pDontResend == NULL)
    {
        return E_INVALID_ARGUMENT;
    }

//    pDontResend->SendToTarget();

    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebViewClient::DoUpdateVisitedHistory(
    /* [in] */ IWebView * pView,
    /* [in] */ const String& url,
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
    if (pHandler == NULL)
    {
        return E_INVALID_ARGUMENT;
    }

    pHandler->Cancel();

    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebViewClient::OnReceivedHttpAuthRequest(
    /* [in] */ IWebView * pView,
    /* [in] */ IHttpAuthHandler * pHandler,
    /* [in] */ const String& host,
    /* [in] */ const String& realm)
{
    if (pHandler == NULL)
    {
        return E_INVALID_ARGUMENT;
    }

    pHandler->Cancel();

    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebViewClient::ShouldOverrideKeyEvent(
    /* [in] */ IWebView * pView,
    /* [in] */ IKeyEvent * pEvent,
    /* [out] */ Boolean * pFlag)
{
    if (pFlag == NULL)
    {
        return E_INVALID_ARGUMENT;
    }

    *pFlag = false;

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

