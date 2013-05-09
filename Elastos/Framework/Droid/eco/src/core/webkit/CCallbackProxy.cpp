
#include "webkit/CCallbackProxy.h"
#include "webkit/Network.h"
#include "webkit/WebBackForwardList.h"
#include "webkit/CWebView.h"
#include "webkit/CWebHistoryItem.h"

const CString CCallbackProxy::LOGTAG("CallbackProxy");

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

#if 0
ECode CCallbackProxy::HandleMessage(
    /* [in] */ IMessage* msg)
{
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
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}
#endif

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

    ECode (STDCALL CCallbackProxy::*pHandlerFunc)(IBitmap*, String&);

    pHandlerFunc = &CCallbackProxy::HandlePageStarted;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr(favicon);
    params->WriteString(url);

    SendMessage(*(Handle32*)&pHandlerFunc, params);

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

    ECode (STDCALL CCallbackProxy::*pHandlerFunc)(String&);

    pHandlerFunc = &CCallbackProxy::HandlePageFinished;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteString(url);

    SendMessage(*(Handle32*)&pHandlerFunc, params);

    return NOERROR;
}

ECode CCallbackProxy::OnTooManyRedirects(
    /* [in] */ IApartment* cancelHandle,
    /* [in] */ Int32 cancelMessage,
    /* [in] */ IParcel* cancelParams,
    /* [in] */ IApartment* continueHandle,
    /* [in] */ Int32 continueMessage,
    /* [in] */ IParcel* continueParams)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnReceivedError(
    /* [in] */ Int32 errorCode,
    /* [in] */ const String& description,
    /* [in] */ const String& failingUrl)
{
    // Do an unsynchronized quick check to avoid posting if no callback has
    // been set.
    if (mWebViewClient == NULL) {
        return E_NOT_IMPLEMENTED;
    }

    ECode (STDCALL CCallbackProxy::*pHandlerFunc)(Int32, String&, String&);

    pHandlerFunc = &CCallbackProxy::HandleReportError;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32(errorCode);
    params->WriteString(description);
    params->WriteString(failingUrl);

    SendMessage(*(Handle32*)&pHandlerFunc, params);

    return NOERROR;
}

ECode CCallbackProxy::OnFormResubmission(
    /* [in] */ IApartment* resendHandle,
    /* [in] */ Int32 resendMessage,
    /* [in] */ IParcel* resendParams,
    /* [in] */ IApartment* dontResendHandle,
    /* [in] */ Int32 dontResendMessage,
    /* [in] */ IParcel* dontResendParams)
{
    VALIDATE_NOT_NULL(dontResendHandle);

    // Do an unsynchronized quick check to avoid posting if no callback has
    // been set.
    if (mWebViewClient == NULL) {
        dontResendHandle->SendMessage(dontResendMessage, dontResendParams);
        return E_NOT_IMPLEMENTED;
    }

    ECode (STDCALL CCallbackProxy::*pHandlerFunc)(IApartment*,
                   Int32, IParcel*, IApartment*, Int32, IParcel*);

    pHandlerFunc = &CCallbackProxy::HandleResendPostData;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr(resendHandle);
    params->WriteInt32(resendMessage);
    params->WriteInterfacePtr(resendParams);
    params->WriteInterfacePtr(dontResendHandle);
    params->WriteInt32(dontResendMessage);
    params->WriteInterfacePtr(dontResendParams);

    SendMessage(*(Handle32*)&pHandlerFunc, params);

    return NOERROR;
}

ECode CCallbackProxy::ShouldOverrideUrlLoading(
    /* [in] */ const String& url,
    /* [out] */ Boolean* flag)
{
    // We have a default behavior if no client exists so always send the
    // message.
    //ResultTransport<Boolean>* res = new ResultTransport<Boolean>(FALSE);
    ResultTransport<Boolean> res(FALSE);

    //synchronized (this)
    {
        Mutex::Autolock lock(_m_syncLock);

        ECode (STDCALL CCallbackProxy::*pHandlerFunc)(String&, Handle32);

        pHandlerFunc = &CCallbackProxy::HandleOverrideUrl;

        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        params->WriteString(String("url"));
        params->WriteInt32((Handle32)&res);

        SendMessage(*(Handle32*)&pHandlerFunc, params);

        //try {
//            wait();
        //} catch (InterruptedException e) {
        //    Log.e(LOGTAG, "Caught exception while waiting for overrideUrl");
        //    Log.e(LOGTAG, Log.getStackTraceString(e));
        //}
    }

    if (flag) {
        *flag = *(res.GetResult());
    }

    return NOERROR;
}

ECode CCallbackProxy::OnReceivedHttpAuthRequest(
    /* [in] */ IHttpAuthHandler* handler,
    /* [in] */ const String& hostName,
    /* [in] */ const String& realmName)
{
    VALIDATE_NOT_NULL(handler);

    // Do an unsynchronized quick check to avoid posting if no callback has
    // been set.
    if (mWebViewClient == NULL) {
        handler->Cancel();
        return E_NOT_IMPLEMENTED;
    }

    ECode (STDCALL CCallbackProxy::*pHandlerFunc)(IHttpAuthHandler*, 
                             String&, String&);

    pHandlerFunc = &CCallbackProxy::HandleAuthRequest;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr(handler);
    params->WriteString(hostName);
    params->WriteString(realmName);

    SendMessage(*(Handle32*)&pHandlerFunc, params);

    return NOERROR;
}

ECode CCallbackProxy::OnReceivedSslError(
    /* [in] */ ISslErrorHandler* handler,
    /* [in] */ ISslError* error)
{
    VALIDATE_NOT_NULL(handler);
    VALIDATE_NOT_NULL(error);

    // Do an unsynchronized quick check to avoid posting if no callback has
    // been set.
    if (mWebViewClient == NULL) {
        handler->Cancel();
        return E_NOT_IMPLEMENTED;
    }

    HashMap<String, IInterface*>* map = new HashMap<String, IInterface*>();

    ECode (STDCALL CCallbackProxy::*pHandlerFunc)(Handle32,
                           ISslErrorHandler*, ISslError*);

    pHandlerFunc = &CCallbackProxy::HandleSslError;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32((Handle32)map);
    params->WriteInterfacePtr(handler);
    params->WriteInterfacePtr(error);

    SendMessage(*(Handle32*)&pHandlerFunc, params);

    return NOERROR;
}

ECode CCallbackProxy::OnReceivedCertificate(
    /* [in] */ ISslCertificate* certificate)
{
    // Do an unsynchronized quick check to avoid posting if no callback has
    // been set.
    if (mWebViewClient == NULL) {
        return E_NOT_IMPLEMENTED;
    }

    ECode (STDCALL CCallbackProxy::*pHandlerFunc)(ISslCertificate*);

    pHandlerFunc = &CCallbackProxy::HandleReceivedCertificate;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr(certificate);

    // here, certificate can be null (if the site is not secure)
    SendMessage(*(Handle32*)&pHandlerFunc, params);

    return NOERROR;
}

ECode CCallbackProxy::DoUpdateVisitedHistory(
    /* [in] */ const String& url,
    /* [in] */ Boolean isReload)
{
    // Do an unsynchronized quick check to avoid posting if no callback has
    // been set.
    if (mWebViewClient == NULL) {
        return E_NOT_IMPLEMENTED;
    }

    ECode (STDCALL CCallbackProxy::*pHandlerFunc)(String&, Int32);

    pHandlerFunc = &CCallbackProxy::HandleUpdateVisited;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteString(url);
    params->WriteInt32(isReload ? 1 : 0);

    SendMessage(*(Handle32*)&pHandlerFunc, params);

    return NOERROR;
}

ECode CCallbackProxy::OnLoadResource(
    /* [in] */ const String& url)
{
    // Do an unsynchronized quick check to avoid posting if no callback has
    // been set.
    if (mWebViewClient == NULL) {
        return E_NOT_IMPLEMENTED;
    }

    ECode (STDCALL CCallbackProxy::*pHandlerFunc)(String&);

    pHandlerFunc = &CCallbackProxy::HandleLoadResource;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteString(url);

    SendMessage(*(Handle32*)&pHandlerFunc, params);

    return NOERROR;
}

ECode CCallbackProxy::OnUnhandledKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    // Do an unsynchronized quick check to avoid posting if no callback has
    // been set.
    if (mWebViewClient == NULL) {
        return E_NOT_IMPLEMENTED;
    }

    ECode (STDCALL CCallbackProxy::*pHandlerFunc)(IKeyEvent*);

    pHandlerFunc = &CCallbackProxy::HandleAsyncKeyEvents;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr(event);

    SendMessage(*(Handle32*)&pHandlerFunc, params);

    return NOERROR;
}

ECode CCallbackProxy::OnScaleChanged(
    /* [in] */ Float oldScale,
    /* [in] */ Float newScale)
{
    // Do an unsynchronized quick check to avoid posting if no callback has
    // been set.
    if (mWebViewClient == NULL) {
        return E_NOT_IMPLEMENTED;
    }

    ECode (STDCALL CCallbackProxy::*pHandlerFunc)(Float, Float);

    pHandlerFunc = &CCallbackProxy::HandleScaleChanged;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteFloat(oldScale);
    params->WriteFloat(newScale);

    SendMessage(*(Handle32*)&pHandlerFunc, params);

    return NOERROR;
}

ECode CCallbackProxy::OnDownloadStart(
    /* [in] */ const String& url,
    /* [in] */ const String& userAgent,
    /* [in] */ const String& contentDisposition,
    /* [in] */ const String& mimetype,
    /* [in] */ Int64 contentLength,
    /* [out] */ Boolean* flag)
{
    // Do an unsynchronized quick check to avoid posting if no callback has
    // been set.
    if (mDownloadListener == NULL) {
        // Cancel the download if there is no browser client.
        if (flag) *flag = FALSE;

        return E_NOT_IMPLEMENTED;
    }

    ECode (STDCALL CCallbackProxy::*pHandlerFunc)(String&,
                        String&, String&, Int64, String&);

    pHandlerFunc = &CCallbackProxy::HandleDownloadFile;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteString(url);
    params->WriteString(userAgent);
    params->WriteString(mimetype);
    params->WriteInt64(contentLength);
    params->WriteString(contentDisposition);

    SendMessage(*(Handle32*)&pHandlerFunc, params);

    if (flag) {
        *flag = TRUE;
    }

    return NOERROR;
}

ECode CCallbackProxy::OnSavePassword(
    /* [in] */ const String& schemePlusHost,
    /* [in] */ const String& username,
    /* [in] */ const String& password,
    /* [in] */ IApartment* resumeMsgHandle,
    /* [in] */ Int32 resumeMsgId,
    /* [in] */ IParcel* resumeMsgParams,
    /* [out] */ Boolean* flag)
{
    // resumeMsg should be null at this point because we want to create it
    // within the CallbackProxy.
//    if (DebugFlags.CALLBACK_PROXY) {
//        junit.framework.Assert.assertNull(resumeMsg);
//    }

    resumeMsgId = NOTIFY;

    ECode (STDCALL CCallbackProxy::*pHandlerFunc)(String&, String&,
                String&, IApartment*, Int32, IParcel*);

    pHandlerFunc = &CCallbackProxy::HandleSavePassword;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteString(String("host"));
    params->WriteString(String("username"));
    params->WriteString(String("params"));
    params->WriteInterfacePtr((IApartment*)this);
    params->WriteInt32(NOTIFY);
    params->WriteInterfacePtr(NULL);

    //synchronized (this)
    {
        Mutex::Autolock lock(_m_syncLock);
        SendMessage(*(Handle32*)&pHandlerFunc, params);
#if 0
        try {
            wait();
        } catch (InterruptedException e) {
            Log.e(LOGTAG,
                    "Caught exception while waiting for onSavePassword");
            Log.e(LOGTAG, Log.getStackTraceString(e));
        }
#endif
    }

    // Doesn't matter here
    if (flag) {
        *flag = FALSE;
    }

    return NOERROR;
}

ECode CCallbackProxy::OnReceivedHttpAuthCredentials(
    /* [in] */ const String& host,
    /* [in] */ const String& realm,
    /* [in] */ const String& username,
    /* [in] */ const String& password)
{
    ECode (STDCALL CCallbackProxy::*pHandlerFunc)(String&,
                     String&, String&, String&);

    pHandlerFunc = &CCallbackProxy::HandleAuthCredentials;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteString(host);
    params->WriteString(realm);
    params->WriteString(username);
    params->WriteString(password);

    SendMessage(*(Handle32*)&pHandlerFunc, params);

    return NOERROR;
}

ECode CCallbackProxy::OnProgressChanged(
    /* [in] */ Int32 newProgress)
{
    // Synchronize so that mLatestProgress is up-to-date.
    
    Mutex::Autolock lock(_m_syncLock);

    if (mWebChromeClient == NULL || mLatestProgress == newProgress) {
        return E_NOT_IMPLEMENTED;
    }

    mLatestProgress = newProgress;
    if (!mProgressUpdatePending) {

        ECode (STDCALL CCallbackProxy::*pHandlerFunc)();

        pHandlerFunc = &CCallbackProxy::HandleProgress;

        SendMessage(*(Handle32*)&pHandlerFunc, NULL);
        mProgressUpdatePending = true;
    }

    return NOERROR;
}

ECode CCallbackProxy::CreateWindow(
    /* [in] */ Boolean dialog,
    /* [in] */ Boolean userGesture,
    /* [out] */ IBrowserFrame** browseFrame)
{
    VALIDATE_NOT_NULL(browseFrame);

    // Do an unsynchronized quick check to avoid posting if no callback has
    // been set.
    if (mWebChromeClient == NULL) {
        *browseFrame = NULL;
        return E_NOT_IMPLEMENTED;
    }

    AutoPtr<IWebViewTransport> transport;// = mWebView.new WebViewTransport();

    AutoPtr<IParcel> _params;
    CCallbackParcel::New((IParcel**)&_params);
    _params->WriteInterfacePtr(transport.Get());

    //synchronized (this) {
    {
        Mutex::Autolock lock(_m_syncLock);

        ECode (STDCALL CCallbackProxy::*pHandlerFunc)(Int32, Int32,
                                 Int32, IParcel*);

        pHandlerFunc = &CCallbackProxy::HandleCreateWindow;

        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        params->WriteInt32(dialog ? 1 : 0);
        params->WriteInt32(userGesture ? 1 : 0);
        params->WriteInt32(NOTIFY);
        params->WriteInterfacePtr(_params.Get());

        SendMessage(*(Handle32*)&pHandlerFunc, params);

#if 0
        try {
            wait();
        } catch (InterruptedException e) {
            Log.e(LOGTAG,
                    "Caught exception while waiting for createWindow");
            Log.e(LOGTAG, Log.getStackTraceString(e));
        }
#endif
    }

    AutoPtr<IWebView> w;// = transport.getWebView();
    if (w != NULL) {
        AutoPtr<WebViewCore> core = ((CWebView*)w.Get())->GetWebViewCore();
        // If WebView.destroy() has been called, core may be null.  Skip
        // initialization in that case and return null.
        if (core != NULL) {
            core->InitializeSubwindow();
            *browseFrame = core->GetBrowserFrame();
            return NOERROR;
        }
    }

    *browseFrame = NULL;

    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::OnRequestFocus()
{
    // Do an unsynchronized quick check to avoid posting if no callback has
    // been set.
    if (mWebChromeClient == NULL) {
        return E_NOT_IMPLEMENTED;
    }

    ECode (STDCALL CCallbackProxy::*pHandlerFunc)();

    pHandlerFunc = &CCallbackProxy::HandleRequestFocus;

    SendMessage(*(Handle32*)&pHandlerFunc, NULL);

    return NOERROR;
}

ECode CCallbackProxy::OnCloseWindow(
    /* [in] */ IWebView* window)
{
    // Do an unsynchronized quick check to avoid posting if no callback has
    // been set.
    if (mWebChromeClient == NULL) {
        return E_NOT_IMPLEMENTED;
    }

    ECode (STDCALL CCallbackProxy::*pHandlerFunc)(IWebView*);

    pHandlerFunc = &CCallbackProxy::HandleCloseWindow;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr(window);

    SendMessage(*(Handle32*)&pHandlerFunc, params);

    return NOERROR;
}

ECode CCallbackProxy::OnReceivedIcon(
    /* [in] */ IBitmap* icon)
{
    // The current item might be null if the icon was already stored in the
    // database and this is a new WebView.
    AutoPtr<IWebHistoryItem> i;
    mBackForwardList->GetCurrentItem((IWebHistoryItem**)&i);
    if (i != NULL) {
        ((CWebHistoryItem*)i.Get())->SetFavicon(icon);
    }
    // Do an unsynchronized quick check to avoid posting if no callback has
    // been set.
    if (mWebChromeClient == NULL) {
        return E_NOT_IMPLEMENTED;
    }

    ECode (STDCALL CCallbackProxy::*pHandlerFunc)(IBitmap*);

    pHandlerFunc = &CCallbackProxy::HandleReceivedIcon;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr(icon);

    SendMessage(*(Handle32*)&pHandlerFunc, params);

    return NOERROR;
}

ECode CCallbackProxy::OnReceivedTitle(
    /* [in] */ const String& title)
{
    // Do an unsynchronized quick check to avoid posting if no callback has
    // been set.
    if (mWebChromeClient == NULL) {
        return E_NOT_IMPLEMENTED;
    }

    ECode (STDCALL CCallbackProxy::*pHandlerFunc)(String&);

    pHandlerFunc = &CCallbackProxy::HandleReceivedTitle;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteString(title);

    SendMessage(*(Handle32*)&pHandlerFunc, params);

    return NOERROR;
}

ECode CCallbackProxy::OnJsAlert(
    /* [in] */ const String& url,
    /* [in] */ const String& message)
{
    // Do an unsynchronized quick check to avoid posting if no callback has
    // been set.
    if (mWebChromeClient == NULL) {
        return E_NOT_IMPLEMENTED;
    }

    JsResult* result = new JsResult(this, false);

    ECode (STDCALL CCallbackProxy::*pHandlerFunc)(JsResult*, String&, String&);

    pHandlerFunc = &CCallbackProxy::HandleJsAlert;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr((IInterface*)result);
    params->WriteString(message);
    params->WriteString(url);

#if 0
    synchronized (this) {
        sendMessage(alert);
        try {
            wait();
        } catch (InterruptedException e) {
            Log.e(LOGTAG, "Caught exception while waiting for jsAlert");
            Log.e(LOGTAG, Log.getStackTraceString(e));
        }
    }
#endif

    return NOERROR;
}

ECode CCallbackProxy::OnJsConfirm(
    /* [in] */ const String& url,
    /* [in] */ const String& message,
    /* [out] */ Boolean* flag)
{
    // Do an unsynchronized quick check to avoid posting if no callback has
    // been set.
    if (mWebChromeClient == NULL) {
        if (flag) {
            *flag = FALSE;
        }

        return E_NOT_IMPLEMENTED;
    }

    JsResult* result = new JsResult(this, FALSE);

    ECode (STDCALL CCallbackProxy::*pHandlerFunc)(JsResult*,
                              String&, String&);

    pHandlerFunc = &CCallbackProxy::HandleJsConfirm;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr((IInterface*)result);
    params->WriteString(message);
    params->WriteString(url);

    //synchronized (this) {
    {
        SendMessage(*(Handle32*)&pHandlerFunc, params);
#if 0
        try {
            wait();
        } catch (InterruptedException e) {
            Log.e(LOGTAG, "Caught exception while waiting for jsConfirm");
            Log.e(LOGTAG, Log.getStackTraceString(e));
        }
#endif
    }

    if (flag) {
        *flag = result->GetResult();
    }

    return NOERROR;
}

ECode CCallbackProxy::OnJsPrompt(
    /* [in] */ const String& url,
    /* [in] */ const String& message,
    /* [in] */ const String& defaultValue,
    /* [out] */ String* resultOut)
{
    // Do an unsynchronized quick check to avoid posting if no callback has
    // been set.
    if (mWebChromeClient == NULL) {
        if (resultOut) {
            *resultOut = NULL;
        }

        return E_NOT_IMPLEMENTED;
    }

    JsPromptResult* result = new JsPromptResult(this);
    
    ECode (STDCALL CCallbackProxy::*pHandlerFunc)(JsPromptResult*,
                          String&, String&, String&);

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr((IInterface*)result);
    params->WriteString(message);
    params->WriteString(defaultValue);
    params->WriteString(url);

    pHandlerFunc = &CCallbackProxy::HandleJsPrompt;

    //synchronized (this) {
    {
        SendMessage(*(Handle32*)&pHandlerFunc, params);
#if 0
        try {
            wait();
        } catch (InterruptedException e) {
            Log.e(LOGTAG, "Caught exception while waiting for jsPrompt");
            Log.e(LOGTAG, Log.getStackTraceString(e));
        }
#endif
    }

    if (resultOut) {
        result->GetStringResult(resultOut);
    }

    return NOERROR;
}

ECode CCallbackProxy::OnJsBeforeUnload(
    /* [in] */ const String& url,
    /* [in] */ const String& message,
    /* [out] */ Boolean* flag)
{
    // Do an unsynchronized quick check to avoid posting if no callback has
    // been set.
    if (mWebChromeClient == NULL) {
        if (flag) {
            *flag = TRUE;
        }

        return E_NOT_IMPLEMENTED;
    }

    JsResult* result = new JsResult(this, TRUE);

    ECode (STDCALL CCallbackProxy::*pHandlerFunc)(JsResult*,
                               String&, String&);

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr((IInterface*)result);
    params->WriteString(message);
    params->WriteString(url);

    //synchronized (this) {
    {
        SendMessage(*(Handle32*)&pHandlerFunc, params);
#if 0     
        try {
            wait();
        } catch (InterruptedException e) {
            Log.e(LOGTAG, "Caught exception while waiting for jsUnload");
            Log.e(LOGTAG, Log.getStackTraceString(e));
        }
#endif
    }

    if (flag) {
        *flag = result->GetResult();
    }

    return NOERROR;
}

ECode CCallbackProxy::OnExceededDatabaseQuota(
    /* [in] */ const String& url,
    /* [in] */ const String& databaseIdentifier,
    /* [in] */ Int64 currentQuota,
    /* [in] */ Int64 estimatedSize,
    /* [in] */ Int64 totalUsedQuota,
    /* [in] */ IWebStorageQuotaUpdater* quotaUpdater)
{
    VALIDATE_NOT_NULL(quotaUpdater);

    if (mWebChromeClient == NULL) {
        quotaUpdater->UpdateQuota(currentQuota);
        return E_NOT_IMPLEMENTED;
    }

    HashMap<String, IInterface*>* map = new HashMap<String, IInterface*>();
#if 0
    map.put("databaseIdentifier", databaseIdentifier);
    map.put("url", url);
    map.put("currentQuota", currentQuota);
    map.put("estimatedSize", estimatedSize);
    map.put("totalUsedQuota", totalUsedQuota);
    map.put("quotaUpdater", quotaUpdater);
    exceededQuota.obj = map;
    sendMessage(exceededQuota);
#endif

    ECode (STDCALL CCallbackProxy::*pHandlerFunc)(HashMap<String, IInterface*>*);

    pHandlerFunc = &CCallbackProxy::HandleExceededDatabaseQuota;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr((IInterface*)map);

    return NOERROR;
}

ECode CCallbackProxy::OnReachedMaxAppCacheSize(
    /* [in] */ Int64 spaceNeeded,
    /* [in] */ Int64 totalUsedQuota,
    /* [in] */ IWebStorageQuotaUpdater* quotaUpdater)
{
    VALIDATE_NOT_NULL(quotaUpdater);

    if (mWebChromeClient == NULL) {
        quotaUpdater->UpdateQuota(0);
        return E_NOT_IMPLEMENTED;
    }

    HashMap<String, IInterface*>* map = new HashMap<String, IInterface*>();
   
#if 0
    map.put("spaceNeeded", spaceNeeded);
    map.put("totalUsedQuota", totalUsedQuota);
    map.put("quotaUpdater", quotaUpdater);
    msg.obj = map;
#endif

    ECode (STDCALL CCallbackProxy::*pHandlerFunc)(HashMap<String, IInterface*>*);

    pHandlerFunc = &CCallbackProxy::HandleReachedAppcacheMaxsize;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr((IInterface*)map);

    SendMessage(*(Handle32*)&pHandlerFunc, params);

    return NOERROR;
}

ECode CCallbackProxy::OnGeolocationPermissionsShowPrompt(
    /* [in] */ const String& origin,
    /* [in] */ IGeolocationPermissionsCallback* callBack)
{
    VALIDATE_NOT_NULL(callBack);

    if (mWebChromeClient == NULL) {
        return E_NOT_IMPLEMENTED;
    }

    HashMap<String, IInterface*>* map = new HashMap<String, IInterface*>();
#if 0
    map.put("origin", origin);
    map.put("callback", callback);
    showMessage.obj = map;
#endif

    ECode (STDCALL CCallbackProxy::*pHandlerFunc)(HashMap<String, IInterface*>*);

    pHandlerFunc = &CCallbackProxy::HandleGeolocationPermissionsShowPrompt;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr((IInterface*)map);

    SendMessage(*(Handle32*)&pHandlerFunc, params);

    return NOERROR;
}

ECode CCallbackProxy::OnGeolocationPermissionsHidePrompt()
{
    if (mWebChromeClient == NULL) {
        return E_NOT_IMPLEMENTED;
    }

    ECode (STDCALL CCallbackProxy::*pHandlerFunc)();

    SendMessage(*(Handle32*)&pHandlerFunc, NULL);

    return NOERROR;
}

ECode CCallbackProxy::AddMessageToConsole(
    /* [in] */ const String& message,
    /* [in] */ Int32 lineNumber,
    /* [in] */ const String& sourceID,
    /* [in] */ Int32 msgLevel)
{
    if (mWebChromeClient == NULL) {
        return E_NOT_IMPLEMENTED;
    }

    ECode (STDCALL CCallbackProxy::*pHandlerFunc)(String&, String&, Int32, Int32);

    pHandlerFunc = &CCallbackProxy::HandleAddMessageToConsole;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteString(message);
    params->WriteString(sourceID);
    params->WriteInt32(lineNumber);
    params->WriteInt32(msgLevel);

    SendMessage(*(Handle32*)&pHandlerFunc, params);

    return NOERROR;
}

ECode CCallbackProxy::OnJsTimeout(
    /* [out] */ Boolean* flag)
{
    //always interrupt timedout JS by default
    if (mWebChromeClient == NULL) {
        if (flag) {
            *flag = TRUE;
        }
        return E_NOT_IMPLEMENTED;
    }

    JsResult* result = new JsResult(this, true);
    
    ECode (STDCALL CCallbackProxy::*pHandlerFunc)(JsResult*);

    pHandlerFunc = &CCallbackProxy::HandleJsTimeout;

    AutoPtr<IParcel> params;
    params->WriteInterfacePtr((IInterface*)result);

    //synchronized (this) {
    {
        Mutex::Autolock lock (_m_syncLock);
        SendMessage(*(Handle32*)&pHandlerFunc, params);
#if 0
        try {
            wait();
        } catch (InterruptedException e) {
            Log.e(LOGTAG, "Caught exception while waiting for jsUnload");
            Log.e(LOGTAG, Log.getStackTraceString(e));
        }
#endif
    }

    if (flag) {
        *flag = result->GetResult();
    }

    return NOERROR;
}

ECode CCallbackProxy::GetVisitedHistory(
    /* [in] */ IValueCallback* callBack)
{
    if (mWebChromeClient == NULL) {
        return E_NOT_IMPLEMENTED;
    }

    ECode (STDCALL CCallbackProxy::*pHandlerFunc)(IValueCallback*);

    pHandlerFunc = &CCallbackProxy::HandleGetVisitedHistory;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr(callBack);

    return NOERROR;
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
{
    ECode (STDCALL CCallbackProxy::*pHandlerFunc)();

    pHandlerFunc = &CCallbackProxy::HandleSwitchOutHistory;

    SendMessage(*(Handle32*)&pHandlerFunc, NULL);
}

/* package */
CARAPI_(void) CCallbackProxy::OnReceivedTouchIconUrl(
    /* [in] */ const String& url,
    /* [in] */ Boolean precomposed)
{
    // We should have a current item but we do not want to crash so check
    // for null.
    AutoPtr<IWebHistoryItem> i;
    mBackForwardList->GetCurrentItem((IWebHistoryItem**)&i);
    if (i != NULL) {
        String str;
        i->GetTouchIconUrl(&str);
        if (precomposed || str.GetLength() == 0) {
            ((CWebHistoryItem*)i.Get())->SetTouchIconUrl(url);
        }
    }

    // Do an unsynchronized quick check to avoid posting if no callback has
    // been set.
    if (mWebChromeClient == NULL) {
        return;
    }

    ECode (STDCALL CCallbackProxy::*pHandlerFunc)(String&, Int32);

    pHandlerFunc = &CCallbackProxy::HandleReceivedTouchIconUrl;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteString(url);
    params->WriteInt32(precomposed ? 1 : 0);

    SendMessage(*(Handle32*)&pHandlerFunc, params);
}

/**
 * Called by WebViewCore to open a file chooser.
 */
/* package */
CARAPI_(AutoPtr<IUri>) CCallbackProxy::OpenFileChooser()
{
    if (mWebChromeClient == NULL) {
        return NULL;
    }

    UploadFile* uploadFile = new UploadFile();

    ECode (STDCALL CCallbackProxy::*pHandlerFunc)(UploadFile*);

    pHandlerFunc = &HandleOpenFileChooser;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr((IInterface*)uploadFile);

    //synchronized (this) {
    {
        Mutex::Autolock lock(_m_syncLock);

        SendMessage(*(Handle32*)&pHandlerFunc, params);
#if 0        
        try {
            wait();
        } catch (InterruptedException e) {
            Log.e(LOGTAG,
                    "Caught exception while waiting for openFileChooser");
            Log.e(LOGTAG, Log.getStackTraceString(e));
        }
#endif
    }

    return uploadFile->GetResult();
}

CARAPI_(void) CCallbackProxy::OnNewHistoryItem(
    /* [in] */ WebHistoryItem* item)
{
    if (mWebBackForwardListClient == NULL) {
        return;
    }

    ECode (STDCALL CCallbackProxy::*pHandlerFunc)(WebHistoryItem*);

    pHandlerFunc = &CCallbackProxy::HandleAddHistoryItem;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr((IInterface*)item);

    SendMessage(*(Handle32*)&pHandlerFunc, params);
}

CARAPI_(void) CCallbackProxy::OnIndexChanged(
    /* [in] */ WebHistoryItem* item,
    /* [in] */ Int32 index)
{
    if (mWebBackForwardListClient == NULL) {
        return;
    }

    ECode (STDCALL CCallbackProxy::*pHandlerFunc)(WebHistoryItem*, Int32);

    pHandlerFunc = &CCallbackProxy::HandleHistoryIndexChanged;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr((IInterface*)item);
    params->WriteInt32(index);

    SendMessage(*(Handle32*)&pHandlerFunc, params);
}

/********************************ResultTransport********************************/

template <typename E>
inline CCallbackProxy::ResultTransport<E>::ResultTransport(
    /* [in] */ E* defaultResult)
{
    assert(defaultResult != NULL);
    mResult = *defaultResult;
}

template <typename E>
/*synchronized*/
inline CARAPI_(void) CCallbackProxy::ResultTransport<E>::SetResult(
    /* [in] */ E* result)
{
    Mutex::Autolock lock(mLock);
    mResult = *result;
}

template <typename E>
/*synchronized*/
inline CARAPI_(E*) CCallbackProxy::ResultTransport<E>::GetResult()// const
{
    Mutex::Autolock lock(mLock);
    return &mResult;
}

/***********************************UploadFile************************************/

CARAPI_(void) CCallbackProxy::UploadFile::OnReceiveValue(
    /* [in] */ IUri* value)
{}

CARAPI_(AutoPtr<IUri>) CCallbackProxy::UploadFile::GetResult() const
{}



ECode CCallbackProxy::Start(
    /* [in] */ ApartmentAttr attr)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::Finish()
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackProxy::PostCppCallback(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallback(target, func, params, id);
}

ECode CCallbackProxy::PostCppCallbackAtTime(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id,
    /* [in] */ Millisecond64 uptimeMillis)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallbackAtTime(target, func, params, id, uptimeMillis);
}

ECode CCallbackProxy::PostCppCallbackDelayed(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id,
    /* [in] */ Millisecond64 delayMillis)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallbackDelayed(target, func, params, id, delayMillis);
}

ECode CCallbackProxy::PostCppCallbackAtFrontOfQueue(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallbackAtFrontOfQueue(target, func, params, id);
}

ECode CCallbackProxy::RemoveCppCallbacks(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func)
{
    assert(mApartment != NULL);
    return mApartment->RemoveCppCallbacks(target, func);
}

ECode CCallbackProxy::RemoveCppCallbacksEx(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ Int32 id)
{
    assert(mApartment != NULL);
    return mApartment->RemoveCppCallbacksEx(target, func, id);
}

ECode CCallbackProxy::HasCppCallbacks(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [out] */ Boolean* result)
{
    assert(mApartment != NULL);
    return mApartment->HasCppCallbacks(target, func, result);
}

ECode CCallbackProxy::HasCppCallbacksEx(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ Int32 id,
    /* [out] */ Boolean* result)
{
    assert(mApartment != NULL);
    return mApartment->HasCppCallbacksEx(target, func, id, result);
}

ECode CCallbackProxy::SendMessage(
    /* [in] */ Int32 message,
    /* [in] */ IParcel* params)
{
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode CCallbackProxy::SendMessage(
    /* [in] */ Handle32 pvFunc,
    /* [in] */ IParcel* params)
{
    return mApartment->PostCppCallback((Handle32)this, pvFunc, params, 0);
}

ECode CCallbackProxy::SendMessageAtTime(
    /* [in] */ Handle32 pvFunc,
    /* [in] */ IParcel* params,
    /* [in] */ Millisecond64 uptimeMillis)
{
    return mApartment->PostCppCallbackAtTime(
        (Handle32)this, pvFunc, params, 0, uptimeMillis);
}

ECode CCallbackProxy::RemoveMessage(
    /* [in] */ Handle32 func)
{
    return mApartment->RemoveCppCallbacks((Handle32)this, func);
}

ECode CCallbackProxy::HandlePageStarted(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ String& url)
{
    // every time we start a new page, we want to reset the
    // WebView certificate:
    // if the new site is secure, we will reload it and get a
    // new certificate set;
    // if the new site is not secure, the certificate must be
    // null, and that will be the case
    mWebView->SetCertificate(NULL);
    if (mWebViewClient != NULL) {
        mWebViewClient->OnPageStarted(mWebView, url, bitmap);
    }

    return NOERROR;
}

ECode CCallbackProxy::HandlePageFinished(
    /* [in] */ String& url)
{
    ((CWebView*)mWebView.Get())->OnPageFinished(url);
    if (mWebViewClient != NULL) {
        mWebViewClient->OnPageFinished(mWebView, url);
    }

    return NOERROR;
}

ECode CCallbackProxy::HandleReceivedIcon(
    /* [in] */ IBitmap* bitmap)
{
    if (mWebChromeClient != NULL) {
        mWebChromeClient->OnReceivedIcon(mWebView, bitmap);
    }

    return NOERROR;
}

ECode CCallbackProxy::HandleReceivedTouchIconUrl(
    /* [in] */ String& obj,
    /* [in] */ Int32 arg)
{
    if (mWebChromeClient != NULL) {
        mWebChromeClient->OnReceivedTouchIconUrl(mWebView,
                obj, arg == 1);
    }

    return NOERROR;
}

ECode CCallbackProxy::HandleReceivedTitle(
    /* [in] */ String& title)
{
    if (mWebChromeClient != NULL) {
        mWebChromeClient->OnReceivedTitle(mWebView, title);
    }

    return NOERROR;
}

ECode CCallbackProxy::HandleReportError(
    /* [in] */ Int32 reasonCode,
    /* [in] */ String& description,
    /* [in] */ String& failUrl)
{
    if (mWebViewClient != NULL) {
        mWebViewClient->OnReceivedError(mWebView, reasonCode,
                description, failUrl);
    }

    return NOERROR;
}

ECode CCallbackProxy::HandleResendPostData(
    /* [in] */ IApartment* resendHandle,
    /* [in] */ Int32 resendMessage,
    /* [in] */ IParcel* resendParams,
    /* [in] */ IApartment* dontResendHandle,
    /* [in] */ Int32 dontResendMessage,
    /* [in] */ IParcel* dontResendParams)
{
    VALIDATE_NOT_NULL(dontResendHandle);

    if (mWebViewClient != NULL) {
//        mWebViewClient.onFormResubmission(mWebView, dontResend, resend);
    } else {
        dontResendHandle->SendMessage(dontResendMessage, dontResendParams);
    }
}

ECode CCallbackProxy::HandleOverrideUrl(
    /* [in] */ String& url,
    /* [in] */ Handle32 _result)
{
    Boolean override = FALSE;
    UiOverrideUrlLoading(url, &override);
    ResultTransport<Boolean>* result = (ResultTransport<Boolean>*)_result;
    //synchronized (this) {
    {
        Mutex::Autolock lock(_m_syncLock);
        result->SetResult(&override);
//        notify();
    }
}

ECode CCallbackProxy::HandleAuthRequest(
    /* [in] */ IHttpAuthHandler* handler,
    /* [in] */ String& host,
    /* [in] */ String& realm)
{
    VALIDATE_NOT_NULL(handler);
/*
    if (mWebViewClient != null) {
        HttpAuthHandler handler = (HttpAuthHandler) msg.obj;
        String host = msg.getData().getString("host");
        String realm = msg.getData().getString("realm");
        mWebViewClient.onReceivedHttpAuthRequest(mWebView, handler,
                host, realm);
    }
*/
}

ECode CCallbackProxy::HandleSslError(
    /* [in] */ Handle32 _map,
    /* [in] */ ISslErrorHandler* handler,
    /* [in] */ ISslError* error)
{
    VALIDATE_NOT_NULL(_map);

    HashMap<String, IInterface*>* map = (HashMap<String, IInterface*>*)_map;

    if (mWebViewClient != NULL) {
        mWebViewClient->OnReceivedSslError(mWebView, handler, error);
    }
}

ECode CCallbackProxy::HandleProgress()
{
    // Synchronize to ensure mLatestProgress is not modified after
    // setProgress is called and before mProgressUpdatePending is
    // changed.

    Mutex::Autolock lock(_m_syncLock);

    if (mWebChromeClient != NULL) {
        mWebChromeClient->OnProgressChanged(mWebView, mLatestProgress);
    }

    mProgressUpdatePending = FALSE;
}

ECode CCallbackProxy::HandleUpdateVisited(
    /* [in] */ String& obj,
    /* [in] */ Int32 arg)
{
    if (mWebViewClient != NULL) {
        mWebViewClient->DoUpdateVisitedHistory(mWebView, obj, arg != 0);
    }
}

ECode CCallbackProxy::HandleLoadResource(
    /* [in] */ String& obj)
{
    if (mWebViewClient != NULL) {
        mWebViewClient->OnLoadResource(mWebView, obj);
    }
}

ECode CCallbackProxy::HandleDownloadFile(
    /* [in] */ String& url,
    /* [in] */ String& userAgent,
    /* [in] */ String& mimetype,
    /* [in] */ Int64 contentLength,
    /* [in] */ String& contentDisposition)
{
    if (mDownloadListener != NULL) {

        mDownloadListener->OnDownloadStart(url, userAgent,
                contentDisposition, mimetype, contentLength);
    }
}

ECode CCallbackProxy::HandleCreateWindow(
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2,
    /* [in] */ Int32 msgId,
    /* [in] */ IParcel* msgParams)
{
    if (mWebChromeClient != NULL) {
#if 0        
        if (!mWebChromeClient->OnCreateWindow(mWebView,
                 msg.arg1 == 1, msg.arg2 == 1,
                    (Message) msg.obj)) {
            synchronized (this) {
                notify();
            }
        }
#endif
        ((CWebView*)mWebView.Get())->DismissZoomControl();
    }
}

ECode CCallbackProxy::HandleRequestFocus()
{
    if (mWebChromeClient != NULL) {
        mWebChromeClient->OnRequestFocus(mWebView);
    }
}

ECode CCallbackProxy::HandleCloseWindow(
    /* [in] */ IWebView* webView)
{
    if (mWebChromeClient != NULL) {
        mWebChromeClient->OnCloseWindow(webView);
    }
}

ECode CCallbackProxy::HandleSavePassword(
    /* [in] */ String& host,
    /* [in] */ String& username,
    /* [in] */ String& password,
    /* [in] */ IApartment* msgHandle,
    /* [in] */ Int32 msgId,
    /* [in] */ IParcel* msgParams)
{
    // If the client returned false it means that the notify message
    // will not be sent and we should notify WebCore ourselves.
    if (!((CWebView*)mWebView.Get())->OnSavePassword(host, username, password,
                msgHandle, msgId, msgParams)) {
#if 0
        synchronized (this) {
            notify();
        }
#endif
    }
}

ECode CCallbackProxy::HandleAsyncKeyEvents(
    /* [in] */ IKeyEvent* keyEvent)
{
    if (mWebViewClient != NULL) {
        mWebViewClient->OnUnhandledKeyEvent(mWebView, keyEvent);
    }
}

ECode CCallbackProxy::HandleExceededDatabaseQuota(
    /* [in] */ HashMap<String, IInterface*>* map)
{
    VALIDATE_NOT_NULL(map);

    if (mWebChromeClient != NULL) {
        String databaseIdentifier;// = (String) map.get("databaseIdentifier");
        String url;// = (String) map.get("url");
        
        Int64 currentQuota;// = ((Long) map.get("currentQuota")).longValue();
        Int64 totalUsedQuota;// = ((Long) map.get("totalUsedQuota")).longValue();
        Int64 estimatedSize;// = ((Long) map.get("estimatedSize")).longValue();

        AutoPtr<IWebStorageQuotaUpdater> quotaUpdater;// = (WebStorage.QuotaUpdater) map.get("quotaUpdater");

        mWebChromeClient->OnExceededDatabaseQuota(url,
                databaseIdentifier, currentQuota, estimatedSize,
                totalUsedQuota, quotaUpdater);
    }
}

ECode CCallbackProxy::HandleReachedAppcacheMaxsize(
    /* [in] */ HashMap<String, IInterface*>* map)
{
    VALIDATE_NOT_NULL(map);

    if (mWebChromeClient != NULL) {
        Int64 spaceNeeded;// = ((Long) map.get("spaceNeeded")).longValue();
        Int64 totalUsedQuota;// = ((Long) map.get("totalUsedQuota")).longValue();
        AutoPtr<IWebStorageQuotaUpdater> quotaUpdater;// = (WebStorage.QuotaUpdater) map.get("quotaUpdater");

        mWebChromeClient->OnReachedMaxAppCacheSize(spaceNeeded,
                totalUsedQuota, quotaUpdater);
    }
}

ECode CCallbackProxy::HandleGeolocationPermissionsShowPrompt(
    /* [in] */ HashMap<String, IInterface*>* map)
{
    VALIDATE_NOT_NULL(map);

    if (mWebChromeClient != NULL) {
        String origin;// = (String) map.get("origin");
        AutoPtr<IGeolocationPermissionsCallback> callback;// = (GeolocationPermissions.Callback)map.get("callback");
        mWebChromeClient->OnGeolocationPermissionsShowPrompt(origin, callback);
    }
}

ECode CCallbackProxy::HandleGeolocationPermissionsHidePrompt()
{
    if (mWebChromeClient != NULL) {
        mWebChromeClient->OnGeolocationPermissionsHidePrompt();
    }
}

ECode CCallbackProxy::HandleJsAlert(
    /* [in] */ JsResult* res,
    /* [in] */ String& message,
    /* [in] */ String& url)
{
    VALIDATE_NOT_NULL(res);

    if (mWebChromeClient != NULL) {
        //final JsResult res = (JsResult) msg.obj;
        //String message = msg.getData().getString("message");
        //String url = msg.getData().getString("url");
        Boolean flag = FALSE;
//        mWebChromeClient->OnJsAlert(mWebView, url, message, res, &flag);
        if (!flag) {
#if 0
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
#endif
        }
        res->SetReady();
    }
}

ECode CCallbackProxy::HandleJsConfirm(
    /* [in] */ JsResult* res,
    /* [in] */ String& message,
    /* [in] */ String& url)
{
    VALIDATE_NOT_NULL(res);

    if (mWebChromeClient != NULL) {

        Boolean bFlag = TRUE;
#if 0
        mWebChromeClient->OnJsConfirm(mWebView, url, message, res, &bFlag);
        if (!bFlag) {
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
        res->SetReady();
#endif
    }
}

ECode CCallbackProxy::HandleJsPrompt(
    /* [in] */ JsPromptResult* res,
    /* [in] */ String& message,
    /* [in] */ String& defaultVal,
    /* [in] */ String& url)
{
#if 0
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

#endif

    return NOERROR;
}

ECode CCallbackProxy::HandleJsUnload(
    /* [in] */ JsResult* res,
    /* [in] */ String& message,
    /* [in] */ String& url)
{
#if 0
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
#endif 

    return NOERROR;
}

ECode CCallbackProxy::HandleJsTimeout(
    /* [in] */ JsResult* res)
{
    VALIDATE_NOT_NULL(res);

    if(mWebChromeClient != NULL) {
        Boolean bFlag = FALSE;
        mWebChromeClient->OnJsTimeout(&bFlag);
        if(bFlag) {
            res->Confirm();
        } else {
            res->Cancel();
        }

        res->SetReady();
    }
}

ECode CCallbackProxy::HandleReceivedCertificate(
    /* [in] */ ISslCertificate* ssl)
{
    mWebView->SetCertificate(ssl);
}

ECode CCallbackProxy::HandleNotify()
{
    //synchronized (this) {
    {
        Mutex::Autolock lock(_m_syncLock);
//        notify();
    }
}

ECode CCallbackProxy::HandleScaleChanged(
    /* [in] */ Float oldScale,
    /* [in] */ Float newScale)
{
    if (mWebViewClient != NULL) {
        mWebViewClient->OnScaleChanged(mWebView, oldScale, newScale);
    }
}

ECode CCallbackProxy::HandleSwitchOutHistory()
{
    ((CWebView*)mWebView.Get())->SwitchOutDrawHistory();
}

ECode CCallbackProxy::HandleAddMessageToConsole(
    /* [in] */ String& message,
    /* [in] */ String& sourceID,
    /* [in] */ Int32 lineNumber,
    /* [in] */ Int32 msgLevel)
{
    Int32 numberOfMessageLevels;// = ConsoleMessage.MessageLevel.values().length;
    // Sanity bounds check as we'll index an array with msgLevel
    if (msgLevel < 0 || msgLevel >= numberOfMessageLevels) {
        msgLevel = 0;
    }
#if 0
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
#endif

    return NOERROR;
}

ECode CCallbackProxy::HandleGetVisitedHistory(
    /* [in] */ IValueCallback* callback)
{
    if (mWebChromeClient != NULL) {
        mWebChromeClient->GetVisitedHistory(callback);
    }

    return NOERROR;
}

ECode CCallbackProxy::HandleOpenFileChooser(
    /* [in] */ UploadFile* uploadFile)
{
    if (mWebChromeClient != NULL) {
        mWebChromeClient->OpenFileChooser((IValueCallback*)uploadFile);
    }
}

ECode CCallbackProxy::HandleAddHistoryItem(
    /* [in] */ WebHistoryItem* item)
{
    if (mWebBackForwardListClient != NULL) {
//        mWebBackForwardListClient->OnNewHistoryItem(item);
    }

    return NOERROR;
}

ECode CCallbackProxy::HandleHistoryIndexChanged(
    /* [in] */ WebHistoryItem* item,
    /* [in] */ Int32 index)
{
    if (mWebBackForwardListClient != NULL) {
//        mWebBackForwardListClient->OnIndexChanged(item, index);
    }
}

ECode CCallbackProxy::HandleAuthCredentials(
    /* [in] */ String& host,
    /* [in] */ String& realm,
    /* [in] */ String& username,
    /* [in] */ String& password)
{
    mWebView->SetHttpAuthUsernamePassword(
            host, realm, username, password);
}