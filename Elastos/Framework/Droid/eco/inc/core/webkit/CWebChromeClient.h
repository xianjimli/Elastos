
#ifndef __CWEBCHROMECLIENT_H__
#define __CWEBCHROMECLIENT_H__

#include "_CWebChromeClient.h"
#include "ext/frameworkext.h"

CarClass(CWebChromeClient)
{
public:
    CARAPI OnProgressChanged(
        /* [in] */ IWebView* view,
        /* [in] */ Int32 newProgress);

    CARAPI OnReceivedTitle(
        /* [in] */ IWebView* view,
        /* [in] */ CString title);

    CARAPI OnReceivedIcon(
        /* [in] */ IWebView* view,
        /* [in] */ IBitmap* icon);

    CARAPI OnReceivedTouchIconUrl(
        /* [in] */ IWebView* view,
        /* [in] */ CString url,
        /* [in] */ Boolean precomposed);

    CARAPI OnShowCustomView(
        /* [in] */ IView* view,
        /* [in] */ IWebChromeClientCustomViewCallback* callBack);

    CARAPI OnHideCustomView();

    CARAPI OnCreateWindow(
        /* [in] */ IWebView* view,
        /* [in] */ Boolean dialog,
        /* [in] */ Boolean userGesture,
        /* [in] */ IMessage* resultMsg,
        /* [out] */ Boolean* flag);

    CARAPI OnRequestFocus(
        /* [in] */ IWebView* view);

    CARAPI OnCloseWindow(
        /* [in] */ IWebView* window);

    CARAPI OnJsAlert(
        /* [in] */ IWebView* view,
        /* [in] */ CString url,
        /* [in] */ CString message,
        /* [in] */ IJsResult* result,
        /* [out] */ Boolean* flag);

    CARAPI OnJsConfirm(
        /* [in] */ IWebView* view,
        /* [in] */ CString url,
        /* [in] */ const String& message,
        /* [in] */ IJsResult* result,
        /* [out] */ Boolean* flag);

    CARAPI OnJsPrompt(
        /* [in] */ IWebView* view,
        /* [in] */ CString url,
        /* [in] */ CString message,
        /* [in] */ CString defaultValue,
        /* [in] */ IJsPromptResult* result,
        /* [out] */ Boolean* flag);

    CARAPI OnJsBeforeUnload(
        /* [in] */ IWebView* view,
        /* [in] */ CString url,
        /* [in] */ CString message,
        /* [in] */ IJsResult* result,
        /* [out] */ Boolean* flag);

    CARAPI OnExceededDatabaseQuota(
        /* [in] */ CString url,
        /* [in] */ CString databaseIdentifier,
        /* [in] */ Int64 currentQuota,
        /* [in] */ Int64 estimatedSize,
        /* [in] */ Int64 totalUsedQuota,
        /* [in] */ IWebStorageQuotaUpdater* quotaUpdater);

    CARAPI OnReachedMaxAppCacheSize(
        /* [in] */ Int64 spaceNeeded,
        /* [in] */ Int64 totalUsedQuota,
        /* [in] */ IWebStorageQuotaUpdater* quotaUpdater);

    CARAPI OnGeolocationPermissionsShowPrompt(
        /* [in] */ CString origin,
        /* [in] */ IGeolocationPermissionsCallback* callBack);

    CARAPI OnGeolocationPermissionsHidePrompt();

    CARAPI OnJsTimeout(
        /* [out] */ Boolean* flag);

    CARAPI OnConsoleMessage(
        /* [in] */ CString message,
        /* [in] */ Int32 lineNumber,
        /* [in] */ CString sourceID);

    CARAPI OnConsoleMessageEx(
        /* [in] */ IConsoleMessage* consoleMessage,
        /* [out] */ Boolean* flag);

    CARAPI GetDefaultVideoPoster(
        /* [out] */ IBitmap** bitmap);

    CARAPI GetVideoLoadingProgressView(
        /* [out] */ IView** view);

    CARAPI GetVisitedHistory(
        /* [in] */ IValueCallback* callBack);

    CARAPI OpenFileChooser(
        /* [in] */ IValueCallback* uploadFile);

    CARAPI OnSelectionStart(
        /* [in] */ IWebView* view);

    CARAPI OnSelectionDone(
        /* [in] */ IWebView* view);

private:
    // TODO: Add your private member variables here.
};

#endif // __CWEBCHROMECLIENT_H__
