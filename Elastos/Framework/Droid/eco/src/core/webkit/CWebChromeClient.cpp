
#include "webkit/CWebChromeClient.h"

ECode CWebChromeClient::OnProgressChanged(
    /* [in] */ IWebView* view,
    /* [in] */ Int32 newProgress)
{
    return NOERROR;
}

ECode CWebChromeClient::OnReceivedTitle(
    /* [in] */ IWebView* view,
    /* [in] */ CString title)
{
    return NOERROR;
}

ECode CWebChromeClient::OnReceivedIcon(
    /* [in] */ IWebView* view,
    /* [in] */ IBitmap* icon)
{
    return NOERROR;
}

ECode CWebChromeClient::OnReceivedTouchIconUrl(
    /* [in] */ IWebView* View,
    /* [in] */ CString url,
    /* [in] */ Boolean precomposed)
{
    return NOERROR;
}

ECode CWebChromeClient::OnShowCustomView(
    /* [in] */ IView* view,
    /* [in] */ IWebChromeClientCustomViewCallback* callBack)
{
    return NOERROR;
}

ECode CWebChromeClient::OnHideCustomView()
{
    return NOERROR;
}

ECode CWebChromeClient::OnCreateWindow(
    /* [in] */ IWebView* view,
    /* [in] */ Boolean dialog,
    /* [in] */ Boolean userGesture,
    /* [in] */ IMessage* resultMsg,
    /* [out] */ Boolean* flag)
{
    VALIDATE_NOT_NULL(flag);

    *flag = FALSE;

    return NOERROR;
}

ECode CWebChromeClient::OnRequestFocus(
    /* [in] */ IWebView* view)
{
    return NOERROR;
}

ECode CWebChromeClient::OnCloseWindow(
    /* [in] */ IWebView* window)
{
    return NOERROR;
}

ECode CWebChromeClient::OnJsAlert(
    /* [in] */ IWebView* view,
    /* [in] */ CString url,
    /* [in] */ CString message,
    /* [in] */ IJsResult* result,
    /* [out] */ Boolean* flag)
{
    VALIDATE_NOT_NULL(flag);

    *flag = FALSE;

    return NOERROR;
}

ECode CWebChromeClient::OnJsConfirm(
    /* [in] */ IWebView* view,
    /* [in] */ CString url,
    /* [in] */ const String& message,
    /* [in] */ IJsResult* result,
    /* [out] */ Boolean* flag)
{
    VALIDATE_NOT_NULL(flag);

    *flag = FALSE;

    return NOERROR;
}

ECode CWebChromeClient::OnJsPrompt(
    /* [in] */ IWebView* view,
    /* [in] */ CString url,
    /* [in] */ CString message,
    /* [in] */ CString defaultValue,
    /* [in] */ IJsPromptResult* result,
    /* [out] */ Boolean* flag)
{
    VALIDATE_NOT_NULL(flag);

    *flag = FALSE;

    return NOERROR;
}

ECode CWebChromeClient::OnJsBeforeUnload(
    /* [in] */ IWebView* view,
    /* [in] */ CString url,
    /* [in] */ CString message,
    /* [in] */ IJsResult* result,
    /* [out] */ Boolean* flag)
{
    VALIDATE_NOT_NULL(flag);

    *flag = FALSE;

    return NOERROR;
}

ECode CWebChromeClient::OnExceededDatabaseQuota(
    /* [in] */ CString url,
    /* [in] */ CString databaseIdentifier,
    /* [in] */ Int64 currentQuota,
    /* [in] */ Int64 estimatedSize,
    /* [in] */ Int64 totalUsedQuota,
    /* [in] */ IWebStorageQuotaUpdater* quotaUpdater)
{
    VALIDATE_NOT_NULL(quotaUpdater);

    quotaUpdater->UpdateQuota(currentQuota);

    return NOERROR;
}

ECode CWebChromeClient::OnReachedMaxAppCacheSize(
    /* [in] */ Int64 spaceNeeded,
    /* [in] */ Int64 totalUsedQuota,
    /* [in] */ IWebStorageQuotaUpdater* quotaUpdater)
{
    VALIDATE_NOT_NULL(quotaUpdater);

  //  QuotaUpdater->UpdateQuota(0);

    return NOERROR;
}

ECode CWebChromeClient::OnGeolocationPermissionsShowPrompt(
    /* [in] */ CString origin,
    /* [in] */ IGeolocationPermissionsCallback* callBack)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebChromeClient::OnGeolocationPermissionsHidePrompt()
{
    return NOERROR;
}

ECode CWebChromeClient::OnJsTimeout(
    /* [out] */ Boolean* flag)
{
    VALIDATE_NOT_NULL(flag);

    *flag = FALSE;

    return NOERROR;
}

ECode CWebChromeClient::OnConsoleMessage(
    /* [in] */ CString message,
    /* [in] */ Int32 lineNumber,
    /* [in] */ CString sourceID)
{
    return NOERROR;
}

ECode CWebChromeClient::OnConsoleMessageEx(
    /* [in] */ IConsoleMessage* consoleMessage,
    /* [out] */ Boolean* flag)
{
    VALIDATE_NOT_NULL(consoleMessage);
    VALIDATE_NOT_NULL(flag);

    // Call the old version of this function for backwards compatability.
    String message;
    Int32 lineNumber;
    String sourceId;

    consoleMessage->Message(&message);
    consoleMessage->LineNumber(&lineNumber);
    consoleMessage->SourceId(&sourceId);

    OnConsoleMessage(message, lineNumber, sourceId);
    
    *flag = FALSE;

    return NOERROR;
}

ECode CWebChromeClient::GetDefaultVideoPoster(
    /* [out] */ IBitmap** bitmap)
{
    VALIDATE_NOT_NULL(bitmap);

    *bitmap = NULL;

    return NOERROR;
}

ECode CWebChromeClient::GetVideoLoadingProgressView(
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);

    *view = NULL;

    return NOERROR;
}

ECode CWebChromeClient::GetVisitedHistory(
    /* [in] */ IValueCallback* callBack)
{
    return NOERROR;
}

ECode CWebChromeClient::OpenFileChooser(
    /* [in] */ IValueCallback* uploadFile)
{
    VALIDATE_NOT_NULL(uploadFile);

    uploadFile->OnReceiveValue(NULL);

    return NOERROR;
}

ECode CWebChromeClient::OnSelectionStart(
    /* [in] */ IWebView* view)
{
    VALIDATE_NOT_NULL(view);

    view->NotifySelectDialogDismissed();

    return NOERROR;
}

ECode CWebChromeClient::OnSelectionDone(
    /* [in] */ IWebView* view)
{
    return NOERROR;
}

