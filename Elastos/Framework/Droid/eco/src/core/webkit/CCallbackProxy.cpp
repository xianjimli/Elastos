
#include "webkit/CCallbackProxy.h"
ECode CCallbackProxy::SetWebViewClient(
    /* [in] */ IWebViewClient * pClient)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::GetWebViewClient(
    /* [out] */ IWebViewClient ** ppClient)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::SetWebChromeClient(
    /* [in] */ IWebChromeClient * pClient)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::GetWebChromeClient(
    /* [out] */ IWebChromeClient ** ppClient)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::SetDownloadListener(
    /* [in] */ IDownloadListener * pClient)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::GetBackForwardList(
    /* [out] */ IWebBackForwardList ** ppList)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::UiOverrideUrlLoading(
    /* [in] */ CString overrideUrl,
    /* [out] */ Boolean * pFlag)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::UiOverrideKeyEvent(
    /* [in] */ IKeyEvent * pEvent,
    /* [out] */ Boolean * pFlag)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::HandleMessage(
    /* [in] */ IMessage * pMsg)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::GetProgress(
    /* [out] */ Int32 * pProgress)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnPageStarted(
    /* [in] */ CString url,
    /* [in] */ IBitmap * pFavicon)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnPageFinished(
    /* [in] */ CString url)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnTooManyRedirects(
    /* [in] */ IMessage * pCancelMsg,
    /* [in] */ IMessage * pContinueMsg)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnReceivedError(
    /* [in] */ Int32 errorCode,
    /* [in] */ CString description,
    /* [in] */ CString failingUrl)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnFormResubmission(
    /* [in] */ IMessage * pDontResend,
    /* [in] */ IMessage * pResend)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::ShouldOverrideUrlLoading(
    /* [in] */ CString url,
    /* [out] */ Boolean * pFlag)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnReceivedHttpAuthRequest(
    /* [in] */ IHttpAuthHandler * pHandler,
    /* [in] */ CString hostName,
    /* [in] */ CString realmName)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnReceivedSslError(
    /* [in] */ ISslErrorHandler * pHandler,
    /* [in] */ ISslError * pError)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnReceivedCertificate(
    /* [in] */ ISslCertificate * pCertificate)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::DoUpdateVisitedHistory(
    /* [in] */ CString url,
    /* [in] */ Boolean isReload)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnLoadResource(
    /* [in] */ CString url)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnUnhandledKeyEvent(
    /* [in] */ IKeyEvent * pEvent)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnScaleChanged(
    /* [in] */ Float oldScale,
    /* [in] */ Float newScale)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnDownloadStart(
    /* [in] */ CString url,
    /* [in] */ CString userAgent,
    /* [in] */ CString contentDisposition,
    /* [in] */ CString mimetype,
    /* [in] */ Int64 contentLength,
    /* [out] */ Boolean * pFlag)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnSavePassword(
    /* [in] */ CString schemePlusHost,
    /* [in] */ CString username,
    /* [in] */ const String& password,
    /* [in] */ IMessage * pResumeMsg,
    /* [out] */ Boolean * pFlag)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnReceivedHttpAuthCredentials(
    /* [in] */ CString host,
    /* [in] */ CString realm,
    /* [in] */ CString username,
    /* [in] */ CString password)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnProgressChanged(
    /* [in] */ Int32 newProgress)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::CreateWindow(
    /* [in] */ Boolean dialog,
    /* [in] */ Boolean userGesture,
    /* [out] */ IBrowserFrame ** ppBrowseFrame)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnRequestFocus()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnCloseWindow(
    /* [in] */ IWebView * pWindow)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnReceivedIcon(
    /* [in] */ IBitmap * pIcon)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnReceivedTitle(
    /* [in] */ CString title)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnJsAlert(
    /* [in] */ CString url,
    /* [in] */ CString message)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnJsConfirm(
    /* [in] */ CString url,
    /* [in] */ CString message,
    /* [out] */ Boolean * pFlag)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnJsPrompt(
    /* [in] */ CString url,
    /* [in] */ CString message,
    /* [in] */ CString defaultValue,
    /* [out] */ String * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnJsBeforeUnload(
    /* [in] */ CString url,
    /* [in] */ CString message,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnExceededDatabaseQuota(
    /* [in] */ CString url,
    /* [in] */ CString databaseIdentifier,
    /* [in] */ Int64 currentQuota,
    /* [in] */ Int64 estimatedSize,
    /* [in] */ Int64 totalUsedQuota,
    /* [in] */ IWebStorageQuotaUpdater * pQuotaUpdater)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnReachedMaxAppCacheSize(
    /* [in] */ Int64 spaceNeeded,
    /* [in] */ Int64 totalUsedQuota,
    /* [in] */ IWebStorageQuotaUpdater * pQuotaUpdater)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnGeolocationPermissionsShowPrompt(
    /* [in] */ CString origin,
    /* [in] */ IGeolocationPermissionsCallback * pCallBack)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnGeolocationPermissionsHidePrompt()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::AddMessageToConsole(
    /* [in] */ CString message,
    /* [in] */ Int32 lineNumber,
    /* [in] */ CString sourceID,
    /* [in] */ Int32 msgLevel)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnJsTimeout(
    /* [out] */ Boolean * pFlag)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::GetVisitedHistory(
    /* [in] */ IValueCallback * pCallBack)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OpenFileChooser()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnNewHistoryItem(
    /* [in] */ IWebHistoryItem * pItem)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnIndexChanged(
    /* [in] */ IWebHistoryItem * pItem,
    /* [in] */ Int32 index)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::constructor(
    /* [in] */ IContext * pContext,
    /* [in] */ IWebView * pW)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

