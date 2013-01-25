
#ifndef __CWEBCHROMECLIENT_H__
#define __CWEBCHROMECLIENT_H__

#include "_CWebChromeClient.h"

CarClass(CWebChromeClient)
{
public:
    CARAPI OnProgressChanged(
        /* [in] */ IWebView * pView,
        /* [in] */ Int32 newProgress);

    CARAPI OnReceivedTitle(
        /* [in] */ IWebView * pView,
        /* [in] */ CString title);

    CARAPI OnReceivedIcon(
        /* [in] */ IWebView * pView,
        /* [in] */ IBitmap * pIcon);

    CARAPI OnReceivedTouchIconUrl(
        /* [in] */ IWebView * pView,
        /* [in] */ CString url,
        /* [in] */ Boolean precomposed);

    CARAPI OnShowCustomView(
        /* [in] */ IView * pView,
        /* [in] */ IWebChromeClientCustomViewCallback * pCallBack);

    CARAPI OnHideCustomView();

    CARAPI OnCreateWindow(
        /* [out] */ Boolean * pFlag,
        /* [in] */ IWebView * pView,
        /* [in] */ Boolean dialog,
        /* [in] */ Boolean userGesture,
        /* [in] */ IMessage * pResultMsg);

    CARAPI OnRequestFocus(
        /* [in] */ IWebView * pView);

    CARAPI OnCloseWindow(
        /* [in] */ IWebView * pWindow);

    CARAPI OnJsAlert(
        /* [out] */ Boolean * pFlag,
        /* [in] */ IWebView * pView,
        /* [in] */ CString url,
        /* [in] */ CString message,
        /* [in] */ IJsResult * pResult);

    CARAPI OnJsConfirm(
        /* [out] */ Boolean * pFlag,
        /* [in] */ IWebView * pView,
        /* [in] */ CString url,
        /* [in] */ const String& message,
        /* [in] */ IJsResult * pResult);

    CARAPI OnJsPrompt(
        /* [out] */ Boolean * pFlag,
        /* [in] */ IWebView * pView,
        /* [in] */ CString url,
        /* [in] */ CString message,
        /* [in] */ CString defaultValue,
        /* [in] */ IJsPromptResult * pResult);

    CARAPI OnJsBeforeUnload(
        /* [out] */ Boolean * pFlag,
        /* [in] */ IWebView * pView,
        /* [in] */ CString url,
        /* [in] */ CString message,
        /* [in] */ IJsResult * pResult);

    CARAPI OnExceededDatabaseQuota(
        /* [in] */ CString url,
        /* [in] */ CString databaseIdentifier,
        /* [in] */ Int64 currentQuota,
        /* [in] */ Int64 estimatedSize,
        /* [in] */ Int64 totalUsedQuota,
        /* [in] */ IWebStorageQuotaUpdater * pQuotaUpdater);

    CARAPI OnReachedMaxAppCacheSize(
        /* [in] */ Int64 spaceNeeded,
        /* [in] */ Int64 totalUsedQuota,
        /* [in] */ IWebStorageQuotaUpdater * pQuotaUpdater);

    CARAPI OnGeolocationPermissionsShowPrompt(
        /* [in] */ CString origin,
        /* [in] */ IGeolocationPermissionsCallback * pCallBack);

    CARAPI OnGeolocationPermissionsHidePrompt();

    CARAPI OnJsTimeout(
        /* [out] */ Boolean * pFlag);

    CARAPI OnConsoleMessage(
        /* [in] */ CString message,
        /* [in] */ Int32 lineNumber,
        /* [in] */ CString sourceID);

    CARAPI OnConsoleMessageEx(
        /* [out] */ Boolean * pFlag,
        /* [in] */ IConsoleMessage * pConsoleMessage);

    CARAPI GetDefaultVideoPoster(
        /* [out] */ IBitmap ** ppBitmap);

    CARAPI GetVideoLoadingProgressView();

    CARAPI GetVisitedHistory(
        /* [in] */ IValueCallback * pCallBack);

    CARAPI OpenFileChooser(
        /* [in] */ IValueCallback * pUploadFile);

    CARAPI OnSelectionStart(
        /* [in] */ IWebView * pView);

    CARAPI OnSelectionDone(
        /* [in] */ IWebView * pView);

private:
    // TODO: Add your private member variables here.
};

#endif // __CWEBCHROMECLIENT_H__
