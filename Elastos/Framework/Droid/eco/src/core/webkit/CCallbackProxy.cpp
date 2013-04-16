
#include "webkit/CCallbackProxy.h"
#include "webkit/Network.h"
#include "webkit/WebBackForwardList.h"

const CString CCallbackProxy::LOGTAG = "CallbackProxy";

ECode CCallbackProxy::SetWebViewClient(
    /* [in] */ IWebViewClient* client)
{
    VALIDATE_NOT_NULL(client);
    mWebViewClient = client;

    return NOERROR;
}

ECode CCallbackProxy::GetWebViewClient(
    /* [out] */ IWebViewClient** client)
{
    VALIDATE_NOT_NULL(client);

    *client = (IWebViewClient*)mWebViewClient;    
    
    return NOERROR;
}

ECode CCallbackProxy::SetWebChromeClient(
    /* [in] */ IWebChromeClient* client)
{
    VALIDATE_NOT_NULL(client);

    mWebChromeClient = client;

    return NOERROR;
}

ECode CCallbackProxy::GetWebChromeClient(
    /* [out] */ IWebChromeClient** client)
{
    VALIDATE_NOT_NULL(client);

    *client = (IWebChromeClient*)mWebChromeClient;
    
    return NOERROR;    
}

ECode CCallbackProxy::SetDownloadListener(
    /* [in] */ IDownloadListener* client)
{
    VALIDATE_NOT_NULL(client);

    mDownloadListener = client;

    return NOERROR;
}

ECode CCallbackProxy::GetBackForwardList(
    /* [out] */ IWebBackForwardList** list)
{
    VALIDATE_NOT_NULL(list);

    *list = /*const_cast<IWebBackForwardList*>*/(mBackForwardList);
   
    return NOERROR;
}

ECode CCallbackProxy::UiOverrideUrlLoading(
    /* [in] */ const String& overrideUrl,
    /* [out] */ Boolean* flag)
{
    VALIDATE_NOT_NULL(flag);

    if (overrideUrl.GetLength() == 0) {
        return FALSE;
    }

    Boolean override = FALSE;
    if (mWebViewClient != NULL) {
        ((IWebViewClient*)mWebViewClient)->ShouldOverrideUrlLoading((IWebView*)mWebView,
                overrideUrl, &override);
    } else {
        IIntent* intent = NULL;
        IUri* pUrl = NULL;
        Uri::Parse(overrideUrl, &pUrl);
//        CIntent::New(IIntent::ACTION_VIEW, pUrl, &intent);
//        intent->AddCategory(IIntent::CATEGORY_BROWSABLE);
        // If another application is running a WebView and launches the
        // Browser through this Intent, we want to reuse the same window if
        // possible.
//        intent->PutExtra(Browser.EXTRA_APPLICATION_ID,
//                mContext->GetPackageName());
        //try {
            (/*const_cast<IContext*>*/(mContext))->StartActivity(intent);
            override = TRUE;
        //} catch (ActivityNotFoundException ex) {
            // If no application can handle the URL, assume that the
            // browser can handle it.
        //}
    }

    *flag = override;

    return NOERROR;
}

ECode CCallbackProxy::UiOverrideKeyEvent(
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* flag)
{
    VALIDATE_NOT_NULL(event);
    VALIDATE_NOT_NULL(flag);

    if (mWebViewClient != NULL) {
        ((IWebViewClient*)mWebViewClient)->ShouldOverrideKeyEvent((IWebView*)mWebView, event, flag);
    } else {
        *flag = false;
    }

    return NOERROR;
}

ECode CCallbackProxy::HandleMessage(
    /* [in] */ IMessage* msg)
{
#if 0
    // We don't have to do synchronization because this function operates
        // in the UI thread. The WebViewClient and WebChromeClient functions
        // that check for a non-null callback are ok because java ensures atomic
        // 32-bit reads and writes.
        switch (msg.what) {
        case PAGE_STARTED:
            // every time we start a new page, we want to reset the
            // WebView certificate:
            // if the new site is secure, we will reload it and get a
            // new certificate set;
            // if the new site is not secure, the certificate must be
            // null, and that will be the case
            mWebView.setCertificate(null);
            if (mWebViewClient != null) {
                mWebViewClient.onPageStarted(mWebView,
                        msg.getData().getString("url"),
                        (Bitmap) msg.obj);
            }
            break;

        case PAGE_FINISHED:
            String finishedUrl = (String) msg.obj;
            mWebView.onPageFinished(finishedUrl);
            if (mWebViewClient != null) {
                mWebViewClient.onPageFinished(mWebView, finishedUrl);
            }
            break;
            
        case RECEIVED_ICON:
            if (mWebChromeClient != null) {
                mWebChromeClient.onReceivedIcon(mWebView, (Bitmap) msg.obj);
            }
            break;

        case RECEIVED_TOUCH_ICON_URL:
            if (mWebChromeClient != null) {
                mWebChromeClient.onReceivedTouchIconUrl(mWebView,
                        (String) msg.obj, msg.arg1 == 1);
            }
            break;

        case RECEIVED_TITLE:
            if (mWebChromeClient != null) {
                mWebChromeClient.onReceivedTitle(mWebView,
                        (String) msg.obj);
            }
            break;

        case REPORT_ERROR:
            if (mWebViewClient != null) {
                int reasonCode = msg.arg1;
                final String description  = msg.getData().getString("description");
                final String failUrl  = msg.getData().getString("failingUrl");
                mWebViewClient.onReceivedError(mWebView, reasonCode,
                        description, failUrl);
            }
            break;

        case RESEND_POST_DATA:
            Message resend =
                    (Message) msg.getData().getParcelable("resend");
            Message dontResend =
                    (Message) msg.getData().getParcelable("dontResend");
            if (mWebViewClient != null) {
                mWebViewClient.onFormResubmission(mWebView, dontResend,
                        resend);
            } else {
                dontResend.sendToTarget();
            }
            break;

        case OVERRIDE_URL:
            String overrideUrl = msg.getData().getString("url");
            boolean override = uiOverrideUrlLoading(overrideUrl);
            ResultTransport<Boolean> result =
                    (ResultTransport<Boolean>) msg.obj;
            synchronized (this) {
                result.setResult(override);
                notify();
            }
            break;

        case AUTH_REQUEST:
            if (mWebViewClient != null) {
                HttpAuthHandler handler = (HttpAuthHandler) msg.obj;
                String host = msg.getData().getString("host");
                String realm = msg.getData().getString("realm");
                mWebViewClient.onReceivedHttpAuthRequest(mWebView, handler,
                        host, realm);
            }
            break;

        case SSL_ERROR:
            if (mWebViewClient != null) {
                HashMap<String, Object> map = 
                    (HashMap<String, Object>) msg.obj;
                mWebViewClient.onReceivedSslError(mWebView,
                        (SslErrorHandler) map.get("handler"),
                        (SslError) map.get("error"));
            }
            break;

        case PROGRESS:
            // Synchronize to ensure mLatestProgress is not modified after
            // setProgress is called and before mProgressUpdatePending is
            // changed.
            synchronized (this) {
                if (mWebChromeClient != null) {
                    mWebChromeClient.onProgressChanged(mWebView,
                            mLatestProgress);
                }
                mProgressUpdatePending = false;
            }
            break;

        case UPDATE_VISITED:
            if (mWebViewClient != null) {
                mWebViewClient.doUpdateVisitedHistory(mWebView,
                        (String) msg.obj, msg.arg1 != 0);
            }
            break;

        case LOAD_RESOURCE:
            if (mWebViewClient != null) {
                mWebViewClient.onLoadResource(mWebView, (String) msg.obj);
            }
            break;

        case DOWNLOAD_FILE:
            if (mDownloadListener != null) {
                String url = msg.getData().getString("url");
                String userAgent = msg.getData().getString("userAgent");
                String contentDisposition =
                    msg.getData().getString("contentDisposition");
                String mimetype = msg.getData().getString("mimetype");
                Long contentLength = msg.getData().getLong("contentLength");

                mDownloadListener.onDownloadStart(url, userAgent,
                        contentDisposition, mimetype, contentLength);
            }
            break;

        case CREATE_WINDOW:
            if (mWebChromeClient != null) {
                if (!mWebChromeClient.onCreateWindow(mWebView,
                            msg.arg1 == 1, msg.arg2 == 1,
                            (Message) msg.obj)) {
                    synchronized (this) {
                        notify();
                    }
                }
                mWebView.dismissZoomControl();
            }
            break;

        case REQUEST_FOCUS:
            if (mWebChromeClient != null) {
                mWebChromeClient.onRequestFocus(mWebView);
            }
            break;

        case CLOSE_WINDOW:
            if (mWebChromeClient != null) {
                mWebChromeClient.onCloseWindow((WebView) msg.obj);
            }
            break;

        case SAVE_PASSWORD:
            Bundle bundle = msg.getData();
            String schemePlusHost = bundle.getString("host");
            String username = bundle.getString("username");
            String password = bundle.getString("password");
            // If the client returned false it means that the notify message
            // will not be sent and we should notify WebCore ourselves.
            if (!mWebView.onSavePassword(schemePlusHost, username, password,
                        (Message) msg.obj)) {
                synchronized (this) {
                    notify();
                }
            }
            break;

        case ASYNC_KEYEVENTS:
            if (mWebViewClient != null) {
                mWebViewClient.onUnhandledKeyEvent(mWebView,
                        (KeyEvent) msg.obj);
            }
            break;

        case EXCEEDED_DATABASE_QUOTA:
            if (mWebChromeClient != null) {
                HashMap<String, Object> map =
                        (HashMap<String, Object>) msg.obj;
                String databaseIdentifier =
                        (String) map.get("databaseIdentifier");
                String url = (String) map.get("url");
                long currentQuota =
                        ((Long) map.get("currentQuota")).longValue();
                long totalUsedQuota =
                        ((Long) map.get("totalUsedQuota")).longValue();
                long estimatedSize =
                        ((Long) map.get("estimatedSize")).longValue();
                WebStorage.QuotaUpdater quotaUpdater =
                    (WebStorage.QuotaUpdater) map.get("quotaUpdater");

                mWebChromeClient.onExceededDatabaseQuota(url,
                        databaseIdentifier, currentQuota, estimatedSize,
                        totalUsedQuota, quotaUpdater);
            }
            break;

        case REACHED_APPCACHE_MAXSIZE:
            if (mWebChromeClient != null) {
                HashMap<String, Object> map =
                        (HashMap<String, Object>) msg.obj;
                long spaceNeeded =
                        ((Long) map.get("spaceNeeded")).longValue();
                long totalUsedQuota =
                    ((Long) map.get("totalUsedQuota")).longValue();
                WebStorage.QuotaUpdater quotaUpdater =
                    (WebStorage.QuotaUpdater) map.get("quotaUpdater");

                mWebChromeClient.onReachedMaxAppCacheSize(spaceNeeded,
                        totalUsedQuota, quotaUpdater);
            }
            break;

        case GEOLOCATION_PERMISSIONS_SHOW_PROMPT:
            if (mWebChromeClient != null) {
                HashMap<String, Object> map =
                        (HashMap<String, Object>) msg.obj;
                String origin = (String) map.get("origin");
                GeolocationPermissions.Callback callback =
                        (GeolocationPermissions.Callback)
                        map.get("callback");
                mWebChromeClient.onGeolocationPermissionsShowPrompt(origin,
                        callback);
            }
            break;

        case GEOLOCATION_PERMISSIONS_HIDE_PROMPT:
            if (mWebChromeClient != null) {
                mWebChromeClient.onGeolocationPermissionsHidePrompt();
            }
            break;

        case JS_ALERT:
            if (mWebChromeClient != null) {
                final JsResult res = (JsResult) msg.obj;
                String message = msg.getData().getString("message");
                String url = msg.getData().getString("url");
                if (!mWebChromeClient.onJsAlert(mWebView, url, message,
                        res)) {
                    new AlertDialog.Builder(mContext)
                            .setTitle(getJsDialogTitle(url))
                            .setMessage(message)
                            .setPositiveButton(R.string.ok,
                                    new AlertDialog.OnClickListener() {
                                        public void onClick(
                                                DialogInterface dialog,
                                                int which) {
                                            res.confirm();
                                        }
                                    })
                            .setCancelable(false)
                            .show();
                }
                res.setReady();
            }
            break;

        case JS_CONFIRM:
            if (mWebChromeClient != null) {
                final JsResult res = (JsResult) msg.obj;
                String message = msg.getData().getString("message");
                String url = msg.getData().getString("url");
                if (!mWebChromeClient.onJsConfirm(mWebView, url, message,
                        res)) {
                    new AlertDialog.Builder(mContext)
                            .setTitle(getJsDialogTitle(url))
                            .setMessage(message)
                            .setPositiveButton(R.string.ok, 
                                    new DialogInterface.OnClickListener() {
                                        public void onClick(
                                                DialogInterface dialog,
                                                int which) {
                                            res.confirm();
                                        }})
                            .setNegativeButton(R.string.cancel, 
                                    new DialogInterface.OnClickListener() {
                                        public void onClick(
                                                DialogInterface dialog,
                                                int which) {
                                            res.cancel();
                                        }})
                            .setOnCancelListener(
                                    new DialogInterface.OnCancelListener() {
                                        public void onCancel(
                                                DialogInterface dialog) {
                                            res.cancel();
                                        }
                                    })
                            .show();
                }
                // Tell the JsResult that it is ready for client
                // interaction.
                res.setReady();
            }
            break;

        case JS_PROMPT:
            if (mWebChromeClient != null) {
                final JsPromptResult res = (JsPromptResult) msg.obj;
                String message = msg.getData().getString("message");
                String defaultVal = msg.getData().getString("default");
                String url = msg.getData().getString("url");
                if (!mWebChromeClient.onJsPrompt(mWebView, url, message,
                            defaultVal, res)) {
                    final LayoutInflater factory = LayoutInflater
                            .from(mContext);
                    final View view = factory.inflate(R.layout.js_prompt,
                            null);
                    final EditText v = (EditText) view
                            .findViewById(R.id.value);
                    v.setText(defaultVal);
                    ((TextView) view.findViewById(R.id.message))
                            .setText(message);
                    new AlertDialog.Builder(mContext)
                            .setTitle(getJsDialogTitle(url))
                            .setView(view)
                            .setPositiveButton(R.string.ok,
                                    new DialogInterface.OnClickListener() {
                                        public void onClick(
                                                DialogInterface dialog,
                                                int whichButton) {
                                            res.confirm(v.getText()
                                                    .toString());
                                        }
                                    })
                            .setNegativeButton(R.string.cancel,
                                    new DialogInterface.OnClickListener() {
                                        public void onClick(
                                                DialogInterface dialog,
                                                int whichButton) {
                                            res.cancel();
                                        }
                                    })
                            .setOnCancelListener(
                                    new DialogInterface.OnCancelListener() {
                                        public void onCancel(
                                                DialogInterface dialog) {
                                            res.cancel();
                                        }
                                    })
                            .show();
                }
                // Tell the JsResult that it is ready for client
                // interaction.
                res.setReady();
            }
            break;

        case JS_UNLOAD:
            if (mWebChromeClient != null) {
                final JsResult res = (JsResult) msg.obj;
                String message = msg.getData().getString("message");
                String url = msg.getData().getString("url");
                if (!mWebChromeClient.onJsBeforeUnload(mWebView, url,
                        message, res)) {
                    final String m = mContext.getString(
                            R.string.js_dialog_before_unload, message);
                    new AlertDialog.Builder(mContext)
                            .setMessage(m)
                            .setPositiveButton(R.string.ok,
                                    new DialogInterface.OnClickListener() {
                                        public void onClick(
                                                DialogInterface dialog,
                                                int which) {
                                            res.confirm();
                                        }
                                    })
                            .setNegativeButton(R.string.cancel,
                                    new DialogInterface.OnClickListener() {
                                        public void onClick(
                                                DialogInterface dialog,
                                                int which) {
                                            res.cancel();
                                        }
                                    })
                            .show();
                }
                res.setReady();
            }
            break;

        case JS_TIMEOUT:
            if(mWebChromeClient != null) {
                final JsResult res = (JsResult) msg.obj;
                if(mWebChromeClient.onJsTimeout()) {
                    res.confirm();
                } else {
                    res.cancel();
                }
                res.setReady();
            }
            break;

        case RECEIVED_CERTIFICATE:
            mWebView.setCertificate((SslCertificate) msg.obj);
            break;

        case NOTIFY:
            synchronized (this) {
                notify();
            }
            break;

        case SCALE_CHANGED:
            if (mWebViewClient != null) {
                mWebViewClient.onScaleChanged(mWebView, msg.getData()
                        .getFloat("old"), msg.getData().getFloat("new"));
            }
            break;

        case SWITCH_OUT_HISTORY:
            mWebView.switchOutDrawHistory();
            break;

        case ADD_MESSAGE_TO_CONSOLE:
            String message = msg.getData().getString("message");
            String sourceID = msg.getData().getString("sourceID");
            int lineNumber = msg.getData().getInt("lineNumber");
            int msgLevel = msg.getData().getInt("msgLevel");
            int numberOfMessageLevels = ConsoleMessage.MessageLevel.values().length;
            // Sanity bounds check as we'll index an array with msgLevel
            if (msgLevel < 0 || msgLevel >= numberOfMessageLevels) {
                msgLevel = 0;
            }

            ConsoleMessage.MessageLevel messageLevel =
                    ConsoleMessage.MessageLevel.values()[msgLevel];

            if (!mWebChromeClient.onConsoleMessage(new ConsoleMessage(message, sourceID,
                    lineNumber, messageLevel))) {
                // If false was returned the user did not provide their own console function so
                //  we should output some default messages to the system log.
                String logTag = "Web Console";
                String logMessage = message + " at " + sourceID + ":" + lineNumber;

                switch (messageLevel) {
                    case TIP:
                        Log.v(logTag, logMessage);
                        break;
                    case LOG:
                        Log.i(logTag, logMessage);
                        break;
                    case WARNING:
                        Log.w(logTag, logMessage);
                        break;
                    case ERROR:
                        Log.e(logTag, logMessage);
                        break;
                    case DEBUG:
                        Log.d(logTag, logMessage);
                        break;
                }
            }

            break;

        case GET_VISITED_HISTORY:
            if (mWebChromeClient != null) {
                mWebChromeClient.getVisitedHistory((ValueCallback<String[]>)msg.obj);
            }
            break;

        case OPEN_FILE_CHOOSER:
            if (mWebChromeClient != null) {
                mWebChromeClient.openFileChooser((UploadFile) msg.obj);
            }
            break;

        case ADD_HISTORY_ITEM:
            if (mWebBackForwardListClient != null) {
                mWebBackForwardListClient.onNewHistoryItem(
                        (WebHistoryItem) msg.obj);
            }
            break;

        case HISTORY_INDEX_CHANGED:
            if (mWebBackForwardListClient != null) {
                mWebBackForwardListClient.onIndexChanged(
                        (WebHistoryItem) msg.obj, msg.arg1);
            }
            break;
        case AUTH_CREDENTIALS:
            String host = msg.getData().getString("host");
            String realm = msg.getData().getString("realm");
            username = msg.getData().getString("username");
            password = msg.getData().getString("password");
            mWebView.setHttpAuthUsernamePassword(
                    host, realm, username, password);
            break;
    }
#endif
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::GetProgress(
    /* [out] */ Int32* progress)
{
    VALIDATE_NOT_NULL(progress);

    *progress = mLatestProgress;
    
    return NOERROR;
}

ECode CCallbackProxy::OnPageStarted(
    /* [in] */ const String& url,
    /* [in] */ IBitmap* favicon)
{
    VALIDATE_NOT_NULL(favicon);

    // Do an unsynchronized quick check to avoid posting if no callback has
    // been set.
    if (mWebViewClient == NULL) {
        return E_NOT_IMPLEMENTED;
    }

    // Performance probe
    if (PERF_PROBE) {
        mWebCoreThreadTime = 0;//SystemClock.currentThreadTimeMillis();
        mWebCoreIdleTime = 0;
        Network::GetInstance((IContext*)mContext)->StartTiming();
    }
//    Message msg = ObtainMessage(PAGE_STARTED);
//    msg.obj = favicon;
//    msg.getData().putString("url", url);
//    sendMessage(msg);

    return NOERROR;
}

ECode CCallbackProxy::OnPageFinished(
    /* [in] */ const String& url)
{
    // Performance probe
    if (PERF_PROBE) {
        // un-comment this if PERF_PROBE is true
//        Log.d("WebCore", "WebCore thread used " + 
//                (SystemClock.currentThreadTimeMillis() - mWebCoreThreadTime)
//                + " ms and idled " + mWebCoreIdleTime + " ms");
        Network::GetInstance((IContext*)mContext)->StopTiming();
    }
//    Message msg = obtainMessage(PAGE_FINISHED, url);
//    sendMessage(msg);

    return NOERROR;
}

ECode CCallbackProxy::OnTooManyRedirects(
    /* [in] */ IMessage* cancelMsg,
    /* [in] */ IMessage* continueMsg)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnReceivedError(
    /* [in] */ Int32 errorCode,
    /* [in] */ const String& description,
    /* [in] */ const String& failingUrl)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnFormResubmission(
    /* [in] */ IMessage* dontResend,
    /* [in] */ IMessage* resend)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::ShouldOverrideUrlLoading(
    /* [in] */ const String& url,
    /* [out] */ Boolean* flag)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnReceivedHttpAuthRequest(
    /* [in] */ IHttpAuthHandler* handler,
    /* [in] */ const String& hostName,
    /* [in] */ const String& realmName)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnReceivedSslError(
    /* [in] */ ISslErrorHandler* handler,
    /* [in] */ ISslError* error)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnReceivedCertificate(
    /* [in] */ ISslCertificate* certificate)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::DoUpdateVisitedHistory(
    /* [in] */ const String& url,
    /* [in] */ Boolean isReload)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnLoadResource(
    /* [in] */ const String& url)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnUnhandledKeyEvent(
    /* [in] */ IKeyEvent* event)
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
    /* [in] */ const String& url,
    /* [in] */ const String& userAgent,
    /* [in] */ const String& contentDisposition,
    /* [in] */ const String& mimetype,
    /* [in] */ Int64 contentLength,
    /* [out] */ Boolean* flag)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnSavePassword(
    /* [in] */ const String& schemePlusHost,
    /* [in] */ const String& username,
    /* [in] */ const String& password,
    /* [in] */ IMessage* resumeMsg,
    /* [out] */ Boolean* flag)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnReceivedHttpAuthCredentials(
    /* [in] */ const String& host,
    /* [in] */ const String& realm,
    /* [in] */ const String& username,
    /* [in] */ const String& password)
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
    /* [out] */ IBrowserFrame** browseFrame)
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
    /* [in] */ IWebView* window)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnReceivedIcon(
    /* [in] */ IBitmap* icon)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnReceivedTitle(
    /* [in] */ const String& title)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnJsAlert(
    /* [in] */ const String& url,
    /* [in] */ const String& message)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnJsConfirm(
    /* [in] */ const String& url,
    /* [in] */ const String& message,
    /* [out] */ Boolean* flag)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnJsPrompt(
    /* [in] */ const String& url,
    /* [in] */ const String& message,
    /* [in] */ const String& defaultValue,
    /* [out] */ String* result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnJsBeforeUnload(
    /* [in] */ const String& url,
    /* [in] */ const String& message,
    /* [out] */ Boolean* result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnExceededDatabaseQuota(
    /* [in] */ const String& url,
    /* [in] */ const String& databaseIdentifier,
    /* [in] */ Int64 currentQuota,
    /* [in] */ Int64 estimatedSize,
    /* [in] */ Int64 totalUsedQuota,
    /* [in] */ IWebStorageQuotaUpdater* quotaUpdater)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnReachedMaxAppCacheSize(
    /* [in] */ Int64 spaceNeeded,
    /* [in] */ Int64 totalUsedQuota,
    /* [in] */ IWebStorageQuotaUpdater* quotaUpdater)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnGeolocationPermissionsShowPrompt(
    /* [in] */ const String& origin,
    /* [in] */ IGeolocationPermissionsCallback* callBack)
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
    /* [in] */ const String& message,
    /* [in] */ Int32 lineNumber,
    /* [in] */ const String& sourceID,
    /* [in] */ Int32 msgLevel)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnJsTimeout(
    /* [out] */ Boolean* flag)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::GetVisitedHistory(
    /* [in] */ IValueCallback* callBack)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnNewHistoryItem(
    /* [in] */ IWebHistoryItem* item)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnIndexChanged(
    /* [in] */ IWebHistoryItem* item,
    /* [in] */ Int32 index)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IWebView* w)
{
    VALIDATE_NOT_NULL(context);
    VALIDATE_NOT_NULL(w);

    // Used to start a default activity.
    mContext = context;
    mWebView = w;
//    mBackForwardList = new WebBackForwardList(this);

    return NOERROR;
}


CARAPI_(void) CCallbackProxy::SetWebBackForwardListClient(
    /* [in] */ IWebBackForwardListClient* client)
{
    mWebBackForwardListClient = client;
}

CARAPI_(AutoPtr<IWebBackForwardListClient>) CCallbackProxy::GetWebBackForwardListClient() const
{
    return mWebBackForwardListClient;
}

/**
 * Called by WebCore side to switch out of history Picture drawing mode
 */
CARAPI_(void) CCallbackProxy::SwitchOutDrawHistory()
{}

/* package */
CARAPI_(void) CCallbackProxy::OnReceivedTouchIconUrl(
    /* [in] */ const String& url,
    /* [in] */ Boolean precomposed)
{}

/**
 * Called by WebViewCore to open a file chooser.
 */
/* package */
CARAPI_(AutoPtr<IUri>) CCallbackProxy::OpenFileChooser()
{}

CARAPI_(void) CCallbackProxy::OnNewHistoryItem(
    /* [in] */ WebHistoryItem* item)
{}

CARAPI_(void) CCallbackProxy::OnIndexChanged(
    /* [in] */ WebHistoryItem* item,
    /* [in] */ Int32 index)
{}

/********************************ResultTransport********************************/

CCallbackProxy::ResultTransport::ResultTransport(
    /* [in] */ IInterface* defaultResult)
{}

/*synchronized*/
CARAPI_(void) CCallbackProxy::ResultTransport::SetResult(
    /* [in] */ const IInterface* result)
{
    Mutex::Autolock lock(mLock);
}

/*synchronized*/
CARAPI_(AutoPtr<IInterface>) CCallbackProxy::ResultTransport::GetResult() const
{
    Mutex::Autolock lock(mLock);
}

/***********************************UploadFile************************************/

CARAPI_(void) CCallbackProxy::UploadFile::OnReceiveValue(
    /* [in] */ const IUri* value)
{}

CARAPI_(AutoPtr<IUri>) CCallbackProxy::UploadFile::GetResult() const
{}
