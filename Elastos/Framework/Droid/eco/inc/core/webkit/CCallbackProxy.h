
#ifndef __CCALLBACKPROXY_H__
#define __CCALLBACKPROXY_H__

#include "_CCallbackProxy.h"

CarClass(CCallbackProxy)
{
public:
    CARAPI SetWebViewClient(
        /* [in] */ IWebViewClient * pClient);

    CARAPI GetWebViewClient(
        /* [out] */ IWebViewClient ** ppClient);

    CARAPI SetWebChromeClient(
        /* [in] */ IWebChromeClient * pClient);

    CARAPI GetWebChromeClient(
        /* [out] */ IWebChromeClient ** ppClient);

    CARAPI SetDownloadListener(
        /* [in] */ IDownloadListener * pClient);

    CARAPI GetBackForwardList(
        /* [out] */ IWebBackForwardList ** ppList);

    CARAPI UiOverrideUrlLoading(
        /* [out] */ Boolean * pFlag,
        /* [in] */ CString overrideUrl);

    CARAPI UiOverrideKeyEvent(
        /* [out] */ Boolean * pFlag,
        /* [in] */ IKeyEvent * pEvent);

    CARAPI HandleMessage(
        /* [in] */ IMessage * pMsg);

    CARAPI GetProgress(
        /* [out] */ Int32 * pProgress);

    CARAPI OnPageStarted(
        /* [in] */ CString url,
        /* [in] */ IBitmap * pFavicon);

    CARAPI OnPageFinished(
        /* [in] */ CString url);

    CARAPI OnTooManyRedirects(
        /* [in] */ IMessage * pCancelMsg,
        /* [in] */ IMessage * pContinueMsg);

    CARAPI OnReceivedError(
        /* [in] */ Int32 errorCode,
        /* [in] */ CString description,
        /* [in] */ CString failingUrl);

    CARAPI OnFormResubmission(
        /* [in] */ IMessage * pDontResend,
        /* [in] */ IMessage * pResend);

    CARAPI ShouldOverrideUrlLoading(
        /* [out] */ Boolean * pFlag,
        /* [in] */ CString url);

    CARAPI OnReceivedHttpAuthRequest(
        /* [in] */ IHttpAuthHandler * pHandler,
        /* [in] */ CString hostName,
        /* [in] */ CString realmName);

    CARAPI OnReceivedSslError(
        /* [in] */ ISslErrorHandler * pHandler,
        /* [in] */ ISslError * pError);

    CARAPI OnReceivedCertificate(
        /* [in] */ ISslCertificate * pCertificate);

    CARAPI DoUpdateVisitedHistory(
        /* [in] */ CString url,
        /* [in] */ Boolean isReload);

    CARAPI OnLoadResource(
        /* [in] */ CString url);

    CARAPI OnUnhandledKeyEvent(
        /* [in] */ IKeyEvent * pEvent);

    CARAPI OnScaleChanged(
        /* [in] */ Float oldScale,
        /* [in] */ Float newScale);

    CARAPI OnDownloadStart(
        /* [out] */ Boolean * pFlag,
        /* [in] */ CString url,
        /* [in] */ CString userAgent,
        /* [in] */ CString contentDisposition,
        /* [in] */ CString mimetype,
        /* [in] */ Int64 contentLength);

    CARAPI OnSavePassword(
        /* [out] */ Boolean * pFlag,
        /* [in] */ CString schemePlusHost,
        /* [in] */ CString username,
        /* [in] */ const String& password,
        /* [in] */ IMessage * pResumeMsg);

    CARAPI OnReceivedHttpAuthCredentials(
        /* [in] */ CString host,
        /* [in] */ CString realm,
        /* [in] */ CString username,
        /* [in] */ CString password);

    CARAPI OnProgressChanged(
        /* [in] */ Int32 newProgress);

    CARAPI CreateWindow(
        /* [out] */ IBrowserFrame ** ppBrowseFrame,
        /* [in] */ Boolean dialog,
        /* [in] */ Boolean userGesture);

    CARAPI OnRequestFocus();

    CARAPI OnCloseWindow(
        /* [in] */ IWebView * pWindow);

    CARAPI OnReceivedIcon(
        /* [in] */ IBitmap * pIcon);

    CARAPI OnReceivedTitle(
        /* [in] */ CString title);

    CARAPI OnJsAlert(
        /* [in] */ CString url,
        /* [in] */ CString message);

    CARAPI OnJsConfirm(
        /* [out] */ Boolean * pFlag,
        /* [in] */ CString url,
        /* [in] */ CString message);

    CARAPI OnJsPrompt(
        /* [out] */ String * pResult,
        /* [in] */ CString url,
        /* [in] */ CString message,
        /* [in] */ CString defaultValue);

    CARAPI OnJsBeforeUnload(
        /* [out] */ Boolean * pResult,
        /* [in] */ CString url,
        /* [in] */ CString message);

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

    CARAPI AddMessageToConsole(
        /* [in] */ CString message,
        /* [in] */ Int32 lineNumber,
        /* [in] */ CString sourceID,
        /* [in] */ Int32 msgLevel);

    CARAPI OnJsTimeout(
        /* [out] */ Boolean * pFlag);

    CARAPI GetVisitedHistory(
        /* [in] */ IValueCallback * pCallBack);

    CARAPI OpenFileChooser();

    CARAPI OnNewHistoryItem(
        /* [in] */ IWebHistoryItem * pItem);

    CARAPI OnIndexChanged(
        /* [in] */ IWebHistoryItem * pItem,
        /* [in] */ Int32 index);

    CARAPI constructor(
        /* [in] */ IContext * pContext,
        /* [in] */ IWebView * pW);

private:
    // TODO: Add your private member variables here.
};

#endif // __CCALLBACKPROXY_H__
