
#include "webkit/CWebChromeClient.h"
ECode CWebChromeClient::OnProgressChanged(
    /* [in] */ IWebView * pView,
    /* [in] */ Int32 newProgress)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebChromeClient::OnReceivedTitle(
    /* [in] */ IWebView * pView,
    /* [in] */ CString title)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebChromeClient::OnReceivedIcon(
    /* [in] */ IWebView * pView,
    /* [in] */ IBitmap * pIcon)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebChromeClient::OnReceivedTouchIconUrl(
    /* [in] */ IWebView * pView,
    /* [in] */ CString url,
    /* [in] */ Boolean precomposed)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebChromeClient::OnShowCustomView(
    /* [in] */ IView * pView,
    /* [in] */ IWebChromeClientCustomViewCallback * pCallBack)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebChromeClient::OnHideCustomView()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebChromeClient::OnCreateWindow(
    /* [out] */ Boolean * pFlag,
    /* [in] */ IWebView * pView,
    /* [in] */ Boolean dialog,
    /* [in] */ Boolean userGesture,
    /* [in] */ IMessage * pResultMsg)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebChromeClient::OnRequestFocus(
    /* [in] */ IWebView * pView)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebChromeClient::OnCloseWindow(
    /* [in] */ IWebView * pWindow)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebChromeClient::OnJsAlert(
    /* [out] */ Boolean * pFlag,
    /* [in] */ IWebView * pView,
    /* [in] */ CString url,
    /* [in] */ CString message,
    /* [in] */ IJsResult * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebChromeClient::OnJsConfirm(
    /* [out] */ Boolean * pFlag,
    /* [in] */ IWebView * pView,
    /* [in] */ CString url,
    /* [in] */ const String& message,
    /* [in] */ IJsResult * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebChromeClient::OnJsPrompt(
    /* [out] */ Boolean * pFlag,
    /* [in] */ IWebView * pView,
    /* [in] */ CString url,
    /* [in] */ CString message,
    /* [in] */ CString defaultValue,
    /* [in] */ IJsPromptResult * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebChromeClient::OnJsBeforeUnload(
    /* [out] */ Boolean * pFlag,
    /* [in] */ IWebView * pView,
    /* [in] */ CString url,
    /* [in] */ CString message,
    /* [in] */ IJsResult * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebChromeClient::OnExceededDatabaseQuota(
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

ECode CWebChromeClient::OnReachedMaxAppCacheSize(
    /* [in] */ Int64 spaceNeeded,
    /* [in] */ Int64 totalUsedQuota,
    /* [in] */ IWebStorageQuotaUpdater * pQuotaUpdater)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebChromeClient::OnGeolocationPermissionsShowPrompt(
    /* [in] */ CString origin,
    /* [in] */ IGeolocationPermissionsCallback * pCallBack)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebChromeClient::OnGeolocationPermissionsHidePrompt()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebChromeClient::OnJsTimeout(
    /* [out] */ Boolean * pFlag)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebChromeClient::OnConsoleMessage(
    /* [in] */ CString message,
    /* [in] */ Int32 lineNumber,
    /* [in] */ CString sourceID)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebChromeClient::OnConsoleMessageEx(
    /* [out] */ Boolean * pFlag,
    /* [in] */ IConsoleMessage * pConsoleMessage)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebChromeClient::GetDefaultVideoPoster(
    /* [out] */ IBitmap ** ppBitmap)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebChromeClient::GetVideoLoadingProgressView()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebChromeClient::GetVisitedHistory(
    /* [in] */ IValueCallback * pCallBack)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebChromeClient::OpenFileChooser(
    /* [in] */ IValueCallback * pUploadFile)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebChromeClient::OnSelectionStart(
    /* [in] */ IWebView * pView)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebChromeClient::OnSelectionDone(
    /* [in] */ IWebView * pView)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

