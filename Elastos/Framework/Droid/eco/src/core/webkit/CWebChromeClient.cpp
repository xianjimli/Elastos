
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
    /* [in] */ IWebView * pView,
    /* [in] */ Boolean dialog,
    /* [in] */ Boolean userGesture,
    /* [in] */ IMessage * pResultMsg,
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
    /* [in] */ IWebView * pView,
    /* [in] */ CString url,
    /* [in] */ CString message,
    /* [in] */ IJsResult * pResult,
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

ECode CWebChromeClient::OnJsConfirm(
    /* [in] */ IWebView * pView,
    /* [in] */ CString url,
    /* [in] */ const String& message,
    /* [in] */ IJsResult * pResult,
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

ECode CWebChromeClient::OnJsPrompt(
    /* [in] */ IWebView * pView,
    /* [in] */ CString url,
    /* [in] */ CString message,
    /* [in] */ CString defaultValue,
    /* [in] */ IJsPromptResult * pResult,
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

ECode CWebChromeClient::OnJsBeforeUnload(
    /* [in] */ IWebView * pView,
    /* [in] */ CString url,
    /* [in] */ CString message,
    /* [in] */ IJsResult * pResult,
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

ECode CWebChromeClient::OnExceededDatabaseQuota(
    /* [in] */ CString url,
    /* [in] */ CString databaseIdentifier,
    /* [in] */ Int64 currentQuota,
    /* [in] */ Int64 estimatedSize,
    /* [in] */ Int64 totalUsedQuota,
    /* [in] */ IWebStorageQuotaUpdater * pQuotaUpdater)
{
    if (pQuotaUpdater == NULL)
    {
        return E_INVALID_ARGUMENT;
    }

    pQuotaUpdater->UpdateQuota(currentQuota);

    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebChromeClient::OnReachedMaxAppCacheSize(
    /* [in] */ Int64 spaceNeeded,
    /* [in] */ Int64 totalUsedQuota,
    /* [in] */ IWebStorageQuotaUpdater * pQuotaUpdater)
{
    if (pQuotaUpdater == NULL)
    {
        return E_INVALID_ARGUMENT;
    }

  //  QuotaUpdater->UpdateQuota(0);

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
    if (pFlag == NULL)
    {
        return E_INVALID_ARGUMENT;
    }

    *pFlag = false;

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
    /* [in] */ IConsoleMessage * pConsoleMessage,
    /* [out] */ Boolean * pFlag)
{
    if (pConsoleMessage == NULL || pFlag == NULL)
    {
        return E_INVALID_ARGUMENT;
    }

    // Call the old version of this function for backwards compatability.
    String message;
    Int32 lineNumber;
    String sourceId;

    pConsoleMessage->Message(&message);
    pConsoleMessage->LineNumber(&lineNumber);
    pConsoleMessage->SourceId(&sourceId);

    OnConsoleMessage(message, lineNumber, sourceId);
    
    *pFlag = false;

    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebChromeClient::GetDefaultVideoPoster(
    /* [out] */ IBitmap ** ppBitmap)
{
    if (ppBitmap == NULL)
    {
        return E_INVALID_ARGUMENT;
    }

    *ppBitmap = NULL;

    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebChromeClient::GetVideoLoadingProgressView(
    /* [out] */ IView** pView)
{
    if (pView == NULL)
    {
        return E_INVALID_ARGUMENT;
    }

    *pView = NULL;

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
    if (pUploadFile == NULL)
    {
        return E_INVALID_ARGUMENT;
    }

    pUploadFile->OnReceiveValue(NULL);

    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebChromeClient::OnSelectionStart(
    /* [in] */ IWebView * pView)
{
    if (pView == NULL)
    {
        return E_INVALID_ARGUMENT;
    }

    pView->NotifySelectDialogDismissed();

    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebChromeClient::OnSelectionDone(
    /* [in] */ IWebView * pView)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

