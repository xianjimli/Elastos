
#ifndef __CCALLBACKPROXY_H__
#define __CCALLBACKPROXY_H__

#include <elastos/Mutex.h>

#include "_CCallbackProxy.h"
#include "IValueCallback.h"
#include "net/Uri.h"
#include "content/CIntent.h"
#include "WebBackForwardListClient.h"

using namespace Core;
using namespace Core::Threading;

class WebHistoryItem;
class DownloadListener;
class WebBackForwardList;

CarClass(CCallbackProxy)
{
public:
    CARAPI SetWebViewClient(
        /* [in] */ IWebViewClient* client);

    CARAPI GetWebViewClient(
        /* [out] */ IWebViewClient** client);

    CARAPI SetWebChromeClient(
        /* [in] */ IWebChromeClient* client);

    CARAPI GetWebChromeClient(
        /* [out] */ IWebChromeClient** client);

    CARAPI SetDownloadListener(
        /* [in] */ IDownloadListener* client);

    CARAPI GetBackForwardList(
        /* [out] */ IWebBackForwardList** list);

    CARAPI UiOverrideUrlLoading(
        /* [in] */ const String& overrideUrl,
        /* [out] */ Boolean* flag);

    CARAPI UiOverrideKeyEvent(
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* flag);

    CARAPI GetProgress(
        /* [out] */ Int32* progress);

    CARAPI OnPageStarted(
        /* [in] */ const String& url,
        /* [in] */ IBitmap* favicon);

    CARAPI OnPageFinished(
        /* [in] */ const String& url);

    CARAPI OnTooManyRedirects(
        /* [in] */ IMessage* cancelMsg,
        /* [in] */ IMessage* continueMsg);

    CARAPI OnReceivedError(
        /* [in] */ Int32 errorCode,
        /* [in] */ const String& description,
        /* [in] */ const String& failingUrl);

    CARAPI OnFormResubmission(
        /* [in] */ IMessage* dontResend,
        /* [in] */ IMessage* resend);

    CARAPI ShouldOverrideUrlLoading(
        /* [in] */ const String& url,
        /* [out] */ Boolean* flag);

    CARAPI OnReceivedHttpAuthRequest(
        /* [in] */ IHttpAuthHandler* handler,
        /* [in] */ const String& hostName,
        /* [in] */ const String& realmName);

    CARAPI OnReceivedSslError(
        /* [in] */ ISslErrorHandler* handler,
        /* [in] */ ISslError* error);

    CARAPI OnReceivedCertificate(
        /* [in] */ ISslCertificate* certificate);

    CARAPI DoUpdateVisitedHistory(
        /* [in] */ const String& url,
        /* [in] */ Boolean isReload);

    CARAPI OnLoadResource(
        /* [in] */ const String& url);

    CARAPI OnUnhandledKeyEvent(
        /* [in] */ IKeyEvent* event);

    CARAPI OnScaleChanged(
        /* [in] */ Float oldScale,
        /* [in] */ Float newScale);

    CARAPI OnDownloadStart(
        /* [in] */ const String& url,
        /* [in] */ const String& userAgent,
        /* [in] */ const String& contentDisposition,
        /* [in] */ const String& mimetype,
        /* [in] */ Int64 contentLength,
        /* [out] */ Boolean* flag);

    CARAPI OnSavePassword(
        /* [in] */ const String& schemePlusHost,
        /* [in] */ const String& username,
        /* [in] */ const String& password,
        /* [in] */ IMessage* resumeMsg,
        /* [out] */ Boolean* flag);

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
        /* [out] */ IBrowserFrame** browseFrame);

    CARAPI OnRequestFocus();

    CARAPI OnCloseWindow(
        /* [in] */ IWebView* window);

    CARAPI OnReceivedIcon(
        /* [in] */ IBitmap* icon);

    CARAPI OnReceivedTitle(
        /* [in] */ const String& title);

    CARAPI OnJsAlert(
        /* [in] */ const String& url,
        /* [in] */ const String& message);

    CARAPI OnJsConfirm(
        /* [in] */ const String& url,
        /* [in] */ const String& message,
        /* [out] */ Boolean * flag);

    CARAPI OnJsPrompt(
        /* [in] */ const String& url,
        /* [in] */ const String& message,
        /* [in] */ const String& defaultValue,
        /* [out] */ String* result);

    CARAPI OnJsBeforeUnload(
        /* [in] */ const String& url,
        /* [in] */ const String& message,
        /* [out] */ Boolean* result);

    CARAPI OnExceededDatabaseQuota(
        /* [in] */ const String& url,
        /* [in] */ const String& databaseIdentifier,
        /* [in] */ Int64 currentQuota,
        /* [in] */ Int64 estimatedSize,
        /* [in] */ Int64 totalUsedQuota,
        /* [in] */ IWebStorageQuotaUpdater* quotaUpdater);

    CARAPI OnReachedMaxAppCacheSize(
        /* [in] */ Int64 spaceNeeded,
        /* [in] */ Int64 totalUsedQuota,
        /* [in] */ IWebStorageQuotaUpdater* quotaUpdater);

    CARAPI OnGeolocationPermissionsShowPrompt(
        /* [in] */ const String& origin,
        /* [in] */ IGeolocationPermissionsCallback* callBack);

    CARAPI OnGeolocationPermissionsHidePrompt();

    CARAPI AddMessageToConsole(
        /* [in] */ const String& message,
        /* [in] */ Int32 lineNumber,
        /* [in] */ const String& sourceID,
        /* [in] */ Int32 msgLevel);

    CARAPI OnJsTimeout(
        /* [out] */ Boolean* flag);

    CARAPI GetVisitedHistory(
        /* [in] */ IValueCallback* callBack);

    CARAPI OnNewHistoryItem(
        /* [in] */ IWebHistoryItem* item);

    CARAPI OnIndexChanged(
        /* [in] */ IWebHistoryItem* item,
        /* [in] */ Int32 index);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IWebView* w);

        CARAPI Start(
        /* [in] */ ApartmentAttr attr);

    CARAPI Finish();

    CARAPI PostCppCallback(
        /* [in] */ Handle32 target,
        /* [in] */ Handle32 func,
        /* [in] */ IParcel* params,
        /* [in] */ Int32 id);

    CARAPI PostCppCallbackAtTime(
        /* [in] */ Handle32 target,
        /* [in] */ Handle32 func,
        /* [in] */ IParcel* params,
        /* [in] */ Int32 id,
        /* [in] */ Millisecond64 uptimeMillis);

    CARAPI PostCppCallbackDelayed(
        /* [in] */ Handle32 target,
        /* [in] */ Handle32 func,
        /* [in] */ IParcel* params,
        /* [in] */ Int32 id,
        /* [in] */ Millisecond64 delayMillis);

    CARAPI PostCppCallbackAtFrontOfQueue(
        /* [in] */ Handle32 target,
        /* [in] */ Handle32 func,
        /* [in] */ IParcel* params,
        /* [in] */ Int32 id);

    CARAPI RemoveCppCallbacks(
        /* [in] */ Handle32 target,
        /* [in] */ Handle32 func);

    CARAPI RemoveCppCallbacksEx(
        /* [in] */ Handle32 target,
        /* [in] */ Handle32 func,
        /* [in] */ Int32 id);

    CARAPI HasCppCallbacks(
        /* [in] */ Handle32 target,
        /* [in] */ Handle32 func,
        /* [out] */ Boolean* result);

    CARAPI HasCppCallbacksEx(
        /* [in] */ Handle32 target,
        /* [in] */ Handle32 func,
        /* [in] */ Int32 id,
        /* [out] */ Boolean* result);

    CARAPI SendMessage(
        /* [in] */ Int32 message,
        /* [in] */ IParcel* params);

public:
    CARAPI_(void) SetWebBackForwardListClient(
        /* [in] */ IWebBackForwardListClient* client);

    CARAPI_(AutoPtr<IWebBackForwardListClient>) GetWebBackForwardListClient() const;

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

        mutable Mutex mLock;
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
    CARAPI SendMessage(
        /* [in] */ Handle32 pvFunc,
        /* [in] */ IParcel* params);

    CARAPI SendMessageAtTime(
        /* [in] */ Handle32 pvFunc,
        /* [in] */ IParcel* params,
        /* [in] */ Millisecond64 uptimeMillis);

    CARAPI RemoveMessage(
        /* [in] */ Handle32 func);

    CARAPI HandlePageStarted(
        /* [in] */ IBitmap* bitmap,
        /* [in] */ String& url);

    CARAPI HandlePageFinished(
        /* [in] */ String& url);

    CARAPI HandleReceivedIcon(
        /* [in] */ IBitmap* bitmap);

    CARAPI HandleReceivedTouchIconUrl(
        /* [in] */ String& obj,
        /* [in] */ Int32 arg);

    CARAPI HandleReceivedTitle(
        /* [in] */ String& title);

    CARAPI HandleReportError(
        /* [in] */ Int32 reasonCode,
        /* [in] */ String& description,
        /* [in] */ String& failUrl);

private:

    // Logging tag
    static const CString LOGTAG;// = "CallbackProxy";
    // Instance of WebViewClient that is the client callback.
    /*volatile*/ AutoPtr<IWebViewClient> mWebViewClient;
    // Instance of WebChromeClient for handling all chrome functions.
    /*volatile*/ AutoPtr<IWebChromeClient> mWebChromeClient;
    // Instance of WebView for handling UI requests.
    /*const*/ AutoPtr<IWebView> mWebView;
    // Client registered callback listener for download events
    /*volatile*/ AutoPtr<IDownloadListener> mDownloadListener;
    // Keep track of multiple progress updates.
    Boolean mProgressUpdatePending;
    // Keep track of the last progress amount.
    // Start with 100 to indicate it is not in load for the empty page.
    volatile Int32 mLatestProgress;// = 100;
    // Back/Forward list
    /*const*/ AutoPtr<IWebBackForwardList> mBackForwardList;
    // Back/Forward list client
    /*volatile*/ AutoPtr<IWebBackForwardListClient> mWebBackForwardListClient;
    // Used to call startActivity during url override.
    /*const*/ AutoPtr<IContext> mContext;

    //--------------------------------------------------------------------------
    // WebViewClient functions.
    // NOTE: shouldOverrideKeyEvent is never called from the WebCore thread so
    // it is not necessary to include it here.
    //--------------------------------------------------------------------------

    // Performance probe
    static const Boolean PERF_PROBE = FALSE;
    Int64 mWebCoreThreadTime;
    Int64 mWebCoreIdleTime;

    AutoPtr<IApartment> mApartment;
};

#endif // __CCALLBACKPROXY_H__
