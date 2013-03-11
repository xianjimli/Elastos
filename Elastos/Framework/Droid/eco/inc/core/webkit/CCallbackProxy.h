
#ifndef __CCALLBACKPROXY_H__
#define __CCALLBACKPROXY_H__

#include "_CCallbackProxy.h"
#include "IValueCallback.h"
#include "net/Uri.h"
#include "content/CIntent.h"

class WebBackForwardListClient;
class WebHistoryItem;
class DownloadListener;
class WebBackForwardList;
class WebBackForwardListClient;

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
        /* [in] */ const String& overrideUrl,
        /* [out] */ Boolean * pFlag);

    CARAPI UiOverrideKeyEvent(
        /* [in] */ IKeyEvent * pEvent,
        /* [out] */ Boolean * pFlag);

    CARAPI HandleMessage(
        /* [in] */ IMessage * pMsg);

    CARAPI GetProgress(
        /* [out] */ Int32 * pProgress);

    CARAPI OnPageStarted(
        /* [in] */ const String& url,
        /* [in] */ IBitmap * pFavicon);

    CARAPI OnPageFinished(
        /* [in] */ const String& url);

    CARAPI OnTooManyRedirects(
        /* [in] */ IMessage * pCancelMsg,
        /* [in] */ IMessage * pContinueMsg);

    CARAPI OnReceivedError(
        /* [in] */ Int32 errorCode,
        /* [in] */ const String& description,
        /* [in] */ const String& failingUrl);

    CARAPI OnFormResubmission(
        /* [in] */ IMessage * pDontResend,
        /* [in] */ IMessage * pResend);

    CARAPI ShouldOverrideUrlLoading(
        /* [in] */ const String& url,
        /* [out] */ Boolean * pFlag);

    CARAPI OnReceivedHttpAuthRequest(
        /* [in] */ IHttpAuthHandler * pHandler,
        /* [in] */ const String& hostName,
        /* [in] */ const String& realmName);

    CARAPI OnReceivedSslError(
        /* [in] */ ISslErrorHandler * pHandler,
        /* [in] */ ISslError * pError);

    CARAPI OnReceivedCertificate(
        /* [in] */ ISslCertificate * pCertificate);

    CARAPI DoUpdateVisitedHistory(
        /* [in] */ const String& url,
        /* [in] */ Boolean isReload);

    CARAPI OnLoadResource(
        /* [in] */ const String& url);

    CARAPI OnUnhandledKeyEvent(
        /* [in] */ IKeyEvent * pEvent);

    CARAPI OnScaleChanged(
        /* [in] */ Float oldScale,
        /* [in] */ Float newScale);

    CARAPI OnDownloadStart(
        /* [in] */ const String& url,
        /* [in] */ const String& userAgent,
        /* [in] */ const String& contentDisposition,
        /* [in] */ const String& mimetype,
        /* [in] */ Int64 contentLength,
        /* [out] */ Boolean * pFlag);

    CARAPI OnSavePassword(
        /* [in] */ const String& schemePlusHost,
        /* [in] */ const String& username,
        /* [in] */ const String& password,
        /* [in] */ IMessage * pResumeMsg,
        /* [out] */ Boolean * pFlag);

    CARAPI OnReceivedHttpAuthCredentials(
        /* [in] */ const String& host,
        /* [in] */ const String& realm,
        /* [in] */ const String& username,
        /* [in] */ const String& password);

    CARAPI OnProgressChanged(
        /* [in] */ Int32 newProgress);

    CARAPI CreateWindow(
        /* [in] */ Boolean dialog,
        /* [in] */ Boolean userGesture,
        /* [out] */ IBrowserFrame ** ppBrowseFrame);

    CARAPI OnRequestFocus();

    CARAPI OnCloseWindow(
        /* [in] */ IWebView * pWindow);

    CARAPI OnReceivedIcon(
        /* [in] */ IBitmap * pIcon);

    CARAPI OnReceivedTitle(
        /* [in] */ const String& title);

    CARAPI OnJsAlert(
        /* [in] */ const String& url,
        /* [in] */ const String& message);

    CARAPI OnJsConfirm(
        /* [in] */ const String& url,
        /* [in] */ const String& message,
        /* [out] */ Boolean * pFlag);

    CARAPI OnJsPrompt(
        /* [in] */ const String& url,
        /* [in] */ const String& message,
        /* [in] */ const String& defaultValue,
        /* [out] */ String * pResult);

    CARAPI OnJsBeforeUnload(
        /* [in] */ const String& url,
        /* [in] */ const String& message,
        /* [out] */ Boolean * pResult);

    CARAPI OnExceededDatabaseQuota(
        /* [in] */ const String& url,
        /* [in] */ const String& databaseIdentifier,
        /* [in] */ Int64 currentQuota,
        /* [in] */ Int64 estimatedSize,
        /* [in] */ Int64 totalUsedQuota,
        /* [in] */ IWebStorageQuotaUpdater * pQuotaUpdater);

    CARAPI OnReachedMaxAppCacheSize(
        /* [in] */ Int64 spaceNeeded,
        /* [in] */ Int64 totalUsedQuota,
        /* [in] */ IWebStorageQuotaUpdater * pQuotaUpdater);

    CARAPI OnGeolocationPermissionsShowPrompt(
        /* [in] */ const String& origin,
        /* [in] */ IGeolocationPermissionsCallback * pCallBack);

    CARAPI OnGeolocationPermissionsHidePrompt();

    CARAPI AddMessageToConsole(
        /* [in] */ const String& message,
        /* [in] */ Int32 lineNumber,
        /* [in] */ const String& sourceID,
        /* [in] */ Int32 msgLevel);

    CARAPI OnJsTimeout(
        /* [out] */ Boolean * pFlag);

    CARAPI GetVisitedHistory(
        /* [in] */ IValueCallback * pCallBack);

    CARAPI OnNewHistoryItem(
        /* [in] */ IWebHistoryItem * pItem);

    CARAPI OnIndexChanged(
        /* [in] */ IWebHistoryItem * pItem,
        /* [in] */ Int32 index);

    CARAPI constructor(
        /* [in] */ IContext * pContext,
        /* [in] */ IWebView * pW);

public:
    CARAPI_(void) SetWebBackForwardListClient(
        /* [in] */ WebBackForwardListClient* client);

    CARAPI_(AutoPtr<WebBackForwardListClient>) GetWebBackForwardListClient() const;

    /**
     * Called by WebCore side to switch out of history Picture drawing mode
     */
    CARAPI_(void) SwitchOutDrawHistory();

    /* package */
    CARAPI_(void) OnReceivedTouchIconUrl(
        /* [in] */ const String& url,
        /* [in] */ Boolean precomposed);

    /**
     * Called by WebViewCore to open a file chooser.
     */
    /* package */
    CARAPI_(AutoPtr<IUri>) OpenFileChooser();

    CARAPI_(void) OnNewHistoryItem(
        /* [in] */ WebHistoryItem* item);

    CARAPI_(void) OnIndexChanged(
        /* [in] */ WebHistoryItem* item,
        /* [in] */ Int32 index);

private:
    // Message Ids
    static const Int32 PAGE_STARTED                        = 100;
    static const Int32 RECEIVED_ICON                       = 101;
    static const Int32 RECEIVED_TITLE                      = 102;
    static const Int32 OVERRIDE_URL                        = 103;
    static const Int32 AUTH_REQUEST                        = 104;
    static const Int32 SSL_ERROR                           = 105;
    static const Int32 PROGRESS                            = 106;
    static const Int32 UPDATE_VISITED                      = 107;
    static const Int32 LOAD_RESOURCE                       = 108;
    static const Int32 CREATE_WINDOW                       = 109;
    static const Int32 CLOSE_WINDOW                        = 110;
    static const Int32 SAVE_PASSWORD                       = 111;
    static const Int32 JS_ALERT                            = 112;
    static const Int32 JS_CONFIRM                          = 113;
    static const Int32 JS_PROMPT                           = 114;
    static const Int32 JS_UNLOAD                           = 115;
    static const Int32 ASYNC_KEYEVENTS                     = 116;
    static const Int32 DOWNLOAD_FILE                       = 118;
    static const Int32 REPORT_ERROR                        = 119;
    static const Int32 RESEND_POST_DATA                    = 120;
    static const Int32 PAGE_FINISHED                       = 121;
    static const Int32 REQUEST_FOCUS                       = 122;
    static const Int32 SCALE_CHANGED                       = 123;
    static const Int32 RECEIVED_CERTIFICATE                = 124;
    static const Int32 SWITCH_OUT_HISTORY                  = 125;
    static const Int32 EXCEEDED_DATABASE_QUOTA             = 126;
    static const Int32 REACHED_APPCACHE_MAXSIZE            = 127;
    static const Int32 JS_TIMEOUT                          = 128;
    static const Int32 ADD_MESSAGE_TO_CONSOLE              = 129;
    static const Int32 GEOLOCATION_PERMISSIONS_SHOW_PROMPT = 130;
    static const Int32 GEOLOCATION_PERMISSIONS_HIDE_PROMPT = 131;
    static const Int32 RECEIVED_TOUCH_ICON_URL             = 132;
    static const Int32 GET_VISITED_HISTORY                 = 133;
    static const Int32 OPEN_FILE_CHOOSER                   = 134;
    static const Int32 ADD_HISTORY_ITEM                    = 135;
    static const Int32 HISTORY_INDEX_CHANGED               = 136;
    static const Int32 AUTH_CREDENTIALS                    = 137;

    // Message triggered by the client to resume execution
    static const Int32 NOTIFY                              = 200;

private:
    // Result transportation object for returning results across thread
    // boundaries.
    class ResultTransport
    {
    public:
        ResultTransport(
            /* [in] */ IInterface* defaultResult);

        /*synchronized*/
        CARAPI_(void) SetResult(
            /* [in] */ const IInterface* result);

        /*synchronized*/
        CARAPI_(AutoPtr<IInterface>) GetResult() const;

    private:
        // Private result object
        AutoPtr<IInterface> mResult;
    };

    class UploadFile : public ValueCallback<IUri>
    {   
    public:     
        CARAPI_(void) OnReceiveValue(
            /* [in] */ const IUri* value);

        CARAPI_(AutoPtr<IUri>) GetResult() const;

    private:
        AutoPtr<IUri> mValue;
    };

private:
    CARAPI_(void) GetJsDialogTitle(
        /* [in] */ const String& url,
        /* [out] */ String& str);

private:

    // Logging tag
    static const char* LOGTAG;// = "CallbackProxy";
    // Instance of WebViewClient that is the client callback.
    volatile IWebViewClient* mWebViewClient;
    // Instance of WebChromeClient for handling all chrome functions.
    volatile IWebChromeClient* mWebChromeClient;
    // Instance of WebView for handling UI requests.
    const IWebView* mWebView;
    // Client registered callback listener for download events
    volatile IDownloadListener* mDownloadListener;
    // Keep track of multiple progress updates.
    Boolean mProgressUpdatePending;
    // Keep track of the last progress amount.
    // Start with 100 to indicate it is not in load for the empty page.
    volatile Int32 mLatestProgress;// = 100;
    // Back/Forward list
    const IWebBackForwardList* mBackForwardList;
    // Back/Forward list client
    volatile WebBackForwardListClient* mWebBackForwardListClient;
    // Used to call startActivity during url override.
    const IContext* mContext;

    //--------------------------------------------------------------------------
    // WebViewClient functions.
    // NOTE: shouldOverrideKeyEvent is never called from the WebCore thread so
    // it is not necessary to include it here.
    //--------------------------------------------------------------------------

    // Performance probe
    static const Boolean PERF_PROBE = false;
    Int64 mWebCoreThreadTime;
    Int64 mWebCoreIdleTime;
};

#endif // __CCALLBACKPROXY_H__
