
#include "webkit/WebViewCore.h"
#include "os/Process.h"

const CString WebViewCore::THREAD_NAME = "WebViewCoreThread";
const CString WebViewCore::LOGTAG = "webcore";

const Int32 WebViewCore::ZOOM_BITS = 0;/* = Paint.FILTER_BITMAP_FLAG |
                                 Paint.DITHER_FLAG |
                                 Paint.SUBPIXEL_TEXT_FLAG;*/
const Int32 WebViewCore::SCROLL_BITS = 0;/* = Paint.FILTER_BITMAP_FLAG |
                                   Paint.DITHER_FLAG;*/

const CString WebViewCore::HandlerDebugString[] = {
    "REQUEST_LABEL", // 97
    "UPDATE_FRAME_CACHE_IF_LOADING", // = 98
    "SCROLL_TEXT_INPUT", // = 99
    "LOAD_URL", // = 100;
    "STOP_LOADING", // = 101;
    "RELOAD", // = 102;
    "KEY_DOWN", // = 103;
    "KEY_UP", // = 104;
    "VIEW_SIZE_CHANGED", // = 105;
    "GO_BACK_FORWARD", // = 106;
    "SET_SCROLL_OFFSET", // = 107;
    "RESTORE_STATE", // = 108;
    "PAUSE_TIMERS", // = 109;
    "RESUME_TIMERS", // = 110;
    "CLEAR_CACHE", // = 111;
    "CLEAR_HISTORY", // = 112;
    "SET_SELECTION", // = 113;
    "REPLACE_TEXT", // = 114;
    "PASS_TO_JS", // = 115;
    "SET_GLOBAL_BOUNDS", // = 116;
    "UPDATE_CACHE_AND_TEXT_ENTRY", // = 117;
    "CLICK", // = 118;
    "SET_NETWORK_STATE", // = 119;
    "DOC_HAS_IMAGES", // = 120;
    "121", // = 121;
    "DELETE_SELECTION", // = 122;
    "LISTBOX_CHOICES", // = 123;
    "SINGLE_LISTBOX_CHOICE", // = 124;
    "MESSAGE_RELAY", // = 125;
    "SET_BACKGROUND_COLOR", // = 126;
    "SET_MOVE_FOCUS", // = 127
    "SAVE_DOCUMENT_STATE", // = 128;
    "129", // = 129;
    "WEBKIT_DRAW", // = 130;
    "SYNC_SCROLL", // = 131;
    "POST_URL", // = 132;
    "SPLIT_PICTURE_SET", // = 133;
    "CLEAR_CONTENT", // = 134;
    "SET_MOVE_MOUSE", // = 135;
    "SET_MOVE_MOUSE_IF_LATEST", // = 136;
    "REQUEST_CURSOR_HREF", // = 137;
    "ADD_JS_INTERFACE", // = 138;
    "LOAD_DATA", // = 139;
    "TOUCH_UP", // = 140;
    "TOUCH_EVENT", // = 141;
    "SET_ACTIVE", // = 142;
    "ON_PAUSE",     // = 143
    "ON_RESUME",    // = 144
    "FREE_MEMORY",  // = 145
    "VALID_NODE_BOUNDS", // = 146
};

Core::Threading::Mutex WebViewCore::mutexThis;


/**
 * Transfer all messages to the newly created webcore thread handler.
 */
CARAPI_(void) WebViewCore::EventHub::TransferMessages()
{
#if 0
	mTid = Process::MyTid();
    mSavedPriority = Process::GetThreadPriority(mTid);

    mHandler = new Handler() {
        //@Override
        public:
        	CARAPI_(void) HandleMessage(
        		/* [in] */ IMessage* msg) {
#if 0   
            if (DebugFlags.WEB_VIEW_CORE) {
                Log.v(LOGTAG, (msg.what < REQUEST_LABEL
                        || msg.what
                        > VALID_NODE_BOUNDS ? Integer.toString(msg.what)
                        : HandlerDebugString[msg.what
                                - REQUEST_LABEL])
                        + " arg1=" + msg.arg1 + " arg2=" + msg.arg2
                        + " obj=" + msg.obj);
            }
#endif                
            switch (/*msg.what*/) {
                case WEBKIT_DRAW:
                    WebkitDraw();
                    break;

                case DESTROY:
                    // Time to take down the world. Cancel all pending
                    // loads and destroy the native view and frame.
                    WebViewCore::mutexThis.Lock();
                    {
                        mBrowserFrame->Destroy();
                        mBrowserFrame = NULL;
                        mSettings->OnDestroyed();
                        mNativeClass = 0;
                        mWebView = NULL;
                    }
                    WebViewCore::mutexThis.Lock();
                    break;

                case REQUEST_LABEL:
                    if (mWebView != NULL) {
                        Int32 nodePointer = msg.arg2;
                        String label;// = NativeRequestLabel(msg.arg1, nodePointer);
                        if (label.GetLength() > 0) {
//                                Message.obtain(mWebView.mPrivateHandler, WebView.RETURN_LABEL, nodePointer,
//                                        0, label).sendToTarget();
                        }
                    }
                    break;

                case UPDATE_FRAME_CACHE_IF_LOADING:
                    NativeUpdateFrameCacheIfLoading();
                    break;

                case SCROLL_TEXT_INPUT:
//                        NativeScrollFocusedTextInput(((Float) msg.obj).floatValue(), msg.arg1);
                    break;

                case LOAD_URL: {
                    GetUrlData param;// = (GetUrlData) msg.obj;
                    LoadUrl(param.mUrl, param.mExtraHeaders);
                    break;
                }

                case POST_URL: {
                    PostUrlData param;// = (PostUrlData) msg.obj;
                    mBrowserFrame->PostUrl(param.mUrl, param.mPostData);
                    break;
                }
                case LOAD_DATA:
                    BaseUrlData loadParams;// = (BaseUrlData) msg.obj;
                    String baseUrl = loadParams.mBaseUrl;
                    if (baseUrl.GetLength != 0) {
                        Int32 i = baseUrl.indexOf(':');
                        if (i > 0) {
                            /*
                             * In 1.0, {@link
                             * WebView#loadDataWithBaseURL} can access
                             * local asset files as long as the data is
                             * valid. In the new WebKit, the restriction
                             * is tightened. To be compatible with 1.0,
                             * we automatically add the scheme of the
                             * baseUrl for local access as long as it is
                             * not http(s)/ftp(s)/about/javascript
                             */
                            String scheme = baseUrl.Substring(0, i);
                            if (!scheme.StartWith("http") &&
                                    !scheme.StartWith("ftp") &&
                                    !scheme.StartWith("about") &&
                                    !scheme.StartWith("javascript")) {
                                NativeRegisterURLSchemeAsLocal(scheme);
                            }
                        }
                    }
                    mBrowserFrame->LoadData(baseUrl,
                            loadParams.mData,
                            loadParams.mMimeType,
                            loadParams.mEncoding,
                            loadParams.mHistoryUrl);
                    break;

                case STOP_LOADING:
                    // If the WebCore has committed the load, but not
                    // finished the first layout yet, we need to set
                    // first layout done to trigger the interpreted side sync
                    // up with native side
                    if (((CBrowserFrame*)mBrowserFrame.Get())->Committed()
                            && !((CBrowserFrame*)mBrowserFrame.Get())->FirstLayoutDone()) {
                        ((CBrowserFrame*)mBrowserFrame.Get())->DidFirstLayout();
                    }
                    // Do this after syncing up the layout state.
                    StopLoading();
                    break;

                case RELOAD:
                    mBrowserFrame->Reload(FALSE);
                    break;

                case KEY_DOWN:
//                        Key((KeyEvent) msg.obj, TRUE);
                    break;

                case KEY_UP:
//                        Key((KeyEvent) msg.obj, FALSE);
                    break;

                case CLICK:
//                        NativeClick(msg.arg1, msg.arg2);
                    break;

                case VIEW_SIZE_CHANGED: {
                    WebView.ViewSizeData data;// = WebView.ViewSizeData) msg.obj;
                    ViewSizeChanged(data.mWidth, data.mHeight,
                            data.mTextWrapWidth, data.mScale,
                            data.mAnchorX, data.mAnchorY,
                            data.mIgnoreHeight);
                    break;
                }

                case SET_SCROLL_OFFSET:
                    // note: these are in document coordinates
                    // (inv-zoom)
//                        Point pt = (Point) msg.obj;
//                        NativeSetScrollOffset(msg.arg1, pt.x, pt.y);
                    break;

                case SET_GLOBAL_BOUNDS:
                    AutoPtr<IRect> r;// = (Rect) msg.obj;
                    Int32 left, top, width, height;
                    r->GetLeft(&left);
                    r->GetTop(&top);
                    r->GetWidth(&width);
                    r->GetHeight(&height);
                    NativeSetGlobalBounds(left, top, width, height);
                    break;

                case GO_BACK_FORWARD:
                    // If it is a standard load and the load is not
                    // committed yet, we interpret BACK as RELOAD
                    if (!((CBrowserFrame*)mBrowserFrame.Get())->Committed() 
//                        	&& msg.arg1 == -1 
                    	&& (((CBrowserFrame*)mBrowserFrame.Get())->LoadType() == CBrowserFrame::FRAME_LOADTYPE_STANDARD)) {
                        mBrowserFrame->Reload(TRUE);
                    } else {
//                            mBrowserFrame->GoBackOrForward(msg.arg1);
                    }
                    break;

                case RESTORE_STATE:
                    StopLoading();
//                        RestoreState(msg.arg1);
                    break;

                case PAUSE_TIMERS:
                    mSavedPriority = Process::GetThreadPriority(mTid);
                    Process::SetThreadPriority(mTid, Process::THREAD_PRIORITY_BACKGROUND);
                    PauseTimers();
//                    WebViewWorker.getHandler().sendEmptyMessage(WebViewWorker.MSG_PAUSE_CACHE_TRANSACTION);
                    break;

                case RESUME_TIMERS:
                    Process::SetThreadPriority(mTid, mSavedPriority);
                    ResumeTimers();
//                    WebViewWorker.getHandler().sendEmptyMessage(WebViewWorker.MSG_RESUME_CACHE_TRANSACTION);
                    break;

                case ON_PAUSE:
                    NativePause();
                    break;

                case ON_RESUME:
                    NativeResume();
                    break;

                case FREE_MEMORY:
                    ClearCache(false);
                    NativeFreeMemory();
                    break;

                case SET_NETWORK_STATE:
                #if 0
                    if (BrowserFrame.sJavaBridge == null) {
                        throw new IllegalStateException("No WebView " +
                                "has been created in this process!");
                    }
                #endif 
//                    CBrowserFrame::sJavaBridge->SetNetworkOnLine(msg.arg1 == 1);
                    break;

                case SET_NETWORK_TYPE:
                #if 0
                    if (BrowserFrame.sJavaBridge == null) {
                        throw new IllegalStateException("No WebView " +
                                "has been created in this process!");
                    }
                #endif
//                    Map<String, String> map = (Map<String, String>) msg.obj;
                    AutoPtr<IObjectStringMap> map;
                    String* type = NULL;
                    String* subtype = NULL;
                    map->Get("type", (IInterface**)&type);
                    map->Get("subtype", (IInterface**)&subtype);
                    assert(type);
                    assert(subtype);
                    CBrowserFrame::sJavaBridge->SetNetworkType(*type, *subtype);
                    break;

                case CLEAR_CACHE:
//                    ClearCache(msg.arg1 == 1);
                    break;

                case CLEAR_HISTORY: {
                		AutoPtr<IWebBackForwardList> list;
                    	mCallbackProxy->GetBackForwardList((IWebBackForwardList**)&list);
                    	list->Close(((CBrowserFrame*)mBrowserFrame.Get())->mNativeFrame);
                	}
                    break;

                case REPLACE_TEXT:
//                    ReplaceTextData rep = (ReplaceTextData) msg.obj;
//                    NativeReplaceTextfieldText(msg.arg1, msg.arg2, rep.mReplace, rep.mNewStart, rep.mNewEnd, rep.mTextGeneration);
                    break;

                case PASS_TO_JS: {
                    JSKeyData* jsData;// = (JSKeyData) msg.obj;
                    AutoPtr<IKeyEvent> evt = jsData->mEvent;
                    Int32 keyCode, keyValue;
                    Int32 generation;// = msg.arg1;

                    evt->GetKeyCode(&keyCode);
                    evt->GetKeyCode(&keyValue);

                    Boolean down, shiftPressed, altPressed, symPressed;

                    evt->IsDown(&down);
					evt->IsShiftPressed(&shiftPressed);
					evt->IsAltPressed(&altPressed);
					evt->IsSymPressed(&symPressed)

                    PassToJs(generation,
                            jsData.mCurrentText,
                            keyCode,
                            keyValue,
                            down,
                            shiftPressed,
                            altPressed,
                            symPressed);
                    break;
                }

                case SAVE_DOCUMENT_STATE: {
                    CursorData* cDat;// = (CursorData) msg.obj;
                    NativeSaveDocumentState(cDat->mFrame);
                    break;
                }

                case CLEAR_SSL_PREF_TABLE:
                    Network::GetInstance(mContext)->ClearUserSslPrefTable();
                    break;

                case TOUCH_UP:
                    TouchUpData* touchUpData;// = (TouchUpData) msg.obj;
                    NativeTouchUp(touchUpData->mMoveGeneration,
                            touchUpData->mFrame, touchUpData->mNode,
                            touchUpData->mX, touchUpData->mY);
                    break;

                case TOUCH_EVENT: {
                #if 0
                    TouchEventData ted = (TouchEventData) msg.obj;
                    Message.obtain(
                            mWebView.mPrivateHandler,
                            WebView.PREVENT_TOUCH_ID,
                            ted.mAction,
                            nativeHandleTouchEvent(ted.mAction, ted.mX,
                                    ted.mY, ted.mMetaState) ? 1 : 0,
                            ted.mReprocess ? ted : null).sendToTarget();
                #endif
                    break;
                }

                case SET_ACTIVE:
                    NativeSetFocusControllerActive(/*msg.arg1 == 1*/FALSE);
                    break;

                case ADD_JS_INTERFACE:
                    JSInterfaceData* jsData;// = (JSInterfaceData) msg.obj;
                    mBrowserFrame->AddJavascriptInterface(jsData->mObject,
                            jsData->mInterfaceName);
                    break;

                case REQUEST_EXT_REPRESENTATION:
//                    mBrowserFrame->ExternalRepresentation((Message) msg.obj);
                    break;

                case REQUEST_DOC_AS_TEXT:
//                    mBrowserFrame.documentAsText((Message) msg.obj);
                    break;

                case SET_MOVE_FOCUS:
                    CursorData* focusData;// = (CursorData) msg.obj;
                    NativeMoveFocus(focusData->mFrame, focusData->mNode);
                    break;

                case SET_MOVE_MOUSE:
                    CursorData* cursorData;// = (CursorData) msg.obj;
                    NativeMoveMouse(cursorData->mFrame, cursorData->mX, cursorData->mY);
                    break;

                case SET_MOVE_MOUSE_IF_LATEST:
                    CursorData* cData;// = (CursorData) msg.obj;
                    NativeMoveMouseIfLatest(cData->mMoveGeneration, cData->mFrame,
                            cData->mX, cData->mY);
                    break;

                case REQUEST_CURSOR_HREF: {
                #if 0
                    Message hrefMsg = (Message) msg.obj;
                    hrefMsg.getData().putString("url",
                            nativeRetrieveHref(msg.arg1, msg.arg2));
                    hrefMsg.getData().putString("title",
                            nativeRetrieveAnchorText(msg.arg1, msg.arg2));
                    hrefMsg.sendToTarget();
                #endif
                    break;
                }

                case UPDATE_CACHE_AND_TEXT_ENTRY:
                    NativeUpdateFrameCache();
                    // FIXME: this should provide a minimal rectangle
                    if (mWebView != NULL) {
                        mWebView->PostInvalidate();
                    }
                    SendUpdateTextEntry();
                    break;

                case DOC_HAS_IMAGES:
                #if 0
                    Message imageResult = (Message) msg.obj;
                    imageResult.arg1 =
                            mBrowserFrame.documentHasImages() ? 1 : 0;
                    imageResult.sendToTarget();
                #endif
                    break;

                case DELETE_SELECTION:
                #if 0
                    TextSelectionData deleteSelectionData
                            = (TextSelectionData) msg.obj;
                    nativeDeleteSelection(deleteSelectionData.mStart,
                            deleteSelectionData.mEnd, msg.arg1);
                #endif
                    break;

                case SET_SELECTION:
//                    NativeSetSelection(msg.arg1, msg.arg2);
                    break;

                case LISTBOX_CHOICES:
//                    SparseBooleanArray choices = (SparseBooleanArray)msg.obj;
                    Int32 choicesSize;// = msg.arg1;
                    ArrayOf<Boolean>* choicesArray = ArrayOf<Boolean>::Alloc(choicesSize);
                    for (Int32 c = 0; c < choicesSize; c++) {
//                        choicesArray[c] = choices.get(c);
                    }
                    NativeSendListBoxChoices(choicesArray, choicesSize);
                    break;

                case SINGLE_LISTBOX_CHOICE:
//                    NativeSendListBoxChoice(msg.arg1);
                    break;

                case SET_BACKGROUND_COLOR:
//                    NativeSetBackgroundColor(msg.arg1);
                    break;

                case DUMP_DOMTREE:
                    NativeDumpDomTree(/*msg.arg1 == 1*/FALSE);
                    break;

                case DUMP_RENDERTREE:
                    NativeDumpRenderTree(/*msg.arg1 == 1*/FALSE);
                    break;

                case DUMP_NAVTREE:
                    NativeDumpNavTree();
                    break;

                case DUMP_V8COUNTERS:
                    NativeDumpV8Counters();
                    break;

                case SET_JS_FLAGS:
//                    NativeSetJsFlags((String)msg.obj);
                    break;

                case GEOLOCATION_PERMISSIONS_PROVIDE:
                    GeolocationPermissionsData* data;// = (GeolocationPermissionsData) msg.obj;
                    nativeGeolocationPermissionsProvide(data->mOrigin,
                            data->mAllow, data->mRemember);
                    break;

                case SYNC_SCROLL:
//                    mWebkitScrollX = msg.arg1;
//                    mWebkitScrollY = msg.arg2;
                    break;

                case SPLIT_PICTURE_SET:
                    NativeSplitContent();
                    mSplitPictureIsScheduled = FALSE;
                    break;

                case CLEAR_CONTENT:
                    // Clear the view so that onDraw() will draw nothing
                    // but white background
                    // (See public method WebView.clearView)
                    NativeClearContent();
                    break;

                case MESSAGE_RELAY:
                #if 0
                    if (msg.obj instanceof Message) {
                        ((Message) msg.obj).sendToTarget();
                    }
                #endif
                    break;

                case POPULATE_VISITED_LINKS:
//                    NativeProvideVisitedHistory((String[])msg.obj);
                    break;

                case VALID_NODE_BOUNDS: {
                    MotionUpData* motionUpData;// = (MotionUpData) msg.obj;
                    if (!NativeValidNodeAndBounds(
                            motionUpData->mFrame, motionUpData->mNode,
                            motionUpData->mBounds)) {
                        NativeUpdateFrameCache();
                    }
                    #if 0
                    Message message = mWebView.mPrivateHandler
                            .obtainMessage(WebView.DO_MOTION_UP,
                            motionUpData.mX, motionUpData.mY);
                    mWebView.mPrivateHandler.sendMessageAtFrontOfQueue(
                            message);
                    #endif
                    break;
                }

                case HIDE_FULLSCREEN:
//                    NativeFullScreenPluginHidden(msg.arg1);
                    break;
#if 0
                case ADD_PACKAGE_NAMES:
                    if (BrowserFrame.sJavaBridge == null) {
                        throw new IllegalStateException("No WebView " +
                                "has been created in this process!");
                    }
                    BrowserFrame.sJavaBridge.addPackageNames(
                            (Set<String>) msg.obj);
                    break;

                case ADD_PACKAGE_NAME:
                    if (BrowserFrame.sJavaBridge == null) {
                        throw new IllegalStateException("No WebView " +
                                "has been created in this process!");
                    }
                    BrowserFrame.sJavaBridge.addPackageName(
                            (String) msg.obj);
                    break;

                case REMOVE_PACKAGE_NAME:
                    if (BrowserFrame.sJavaBridge == null) {
                        throw new IllegalStateException("No WebView " +
                                "has been created in this process!");
                    }
                    BrowserFrame.sJavaBridge.removePackageName(
                            (String) msg.obj);
                    break;
#endif           
            }
        }
    };
    // Take all queued messages and resend them to the new handler.
    //synchronized (this) 
    mutexThis.Lock();
    {
        Int32 size;// = mMessages.size();
        for (Int32 i = 0; i < size; i++) {
//            mHandler.sendMessage(mMessages.get(i));
        }
        mMessages = NULL;
    }
    mutexThis.Unlock();
#endif
}

/**
 * Send a message internally to the queue or to the handler
 *
 * synchronized
 */
CARAPI_(void) WebViewCore::EventHub::SendMessage(
	/* [in] */ IMessage* msg)
{}

/* synchronized */
CARAPI_(void) WebViewCore::EventHub::RemoveMessages(
	/* [in] */ Int32 what)
{
}

/* synchronized */
CARAPI_(Boolean) WebViewCore::EventHub::HasMessages(
	/* [in] */ Int32 what)
{}

/* synchronized */
CARAPI_(void) WebViewCore::EventHub::SendMessageDelayed(
	/* [in] */ IMessage* msg, 
	/* [in] */ Int64 delay)
{}

/**
 * Send a message internally to the front of the queue.
 * 
 * synchronized
 */
CARAPI_(void) WebViewCore::EventHub::SendMessageAtFrontOfQueue(
	/* [in] */ IMessage* msg)
{}

/**
 * Remove all the messages.
 *
 * synchronized
 */
CARAPI_(void) WebViewCore::EventHub::RemoveMessages()
{}

/**
 * Block sending messages to the EventHub.
 *
 * synchronized
 */
CARAPI_(void) WebViewCore::EventHub::blockMessages()
{}

WebViewCore::WebViewCore(
	/* [in] */ IContext* context, 
	/* [in] */ IWebView* w, 
	/* [in] */ ICallbackProxy* proxy,
	/* [in] */ IObjectStringMap* javascriptInterfaces)
{}

/* Initialize private data within the WebCore thread.
*/
CARAPI_(void) WebViewCore::Initialize()
{}

/* Handle the initialization of WebViewCore during subwindow creation. This
* method is called from the WebCore thread but it is called before the
* INITIALIZE message can be handled.
*/
/* package */
CARAPI_(void) WebViewCore::InitializeSubwindow()
{}

/* Get the BrowserFrame component. This is used for subwindow creation and
* is called only from BrowserFrame in the WebCore thread. */
/* package */ 
CARAPI_(AutoPtr<IBrowserFrame>) WebViewCore::GetBrowserFrame()
{}

//-------------------------------------------------------------------------
// Common methods
//-------------------------------------------------------------------------

/**
* Causes all timers to pause. This applies to all WebViews in the current
* app process.
*/
CARAPI_(void) WebViewCore::PauseTimers()
{}

/**
* Resume all timers. This applies to all WebViews in the current process.
*/
CARAPI_(void) WebViewCore::ResumeTimers()
{}

CARAPI_(WebSettings*) WebViewCore::GetSettings()
{}

/*
* Given mimeType, check whether it's supported in Android media framework.
* mimeType could be such as "audio/ogg" and "video/mp4".
*/
/* package */
CARAPI_(Boolean) WebViewCore::SupportsMimeType(
	/* [in] */ const String& mimeType)
{}


//-------------------------------------------------------------------------
// JNI methods
//-------------------------------------------------------------------------

/* native*/
CARAPI_(CString) WebViewCore::NativeFindAddress(
	/* [in] */ const String& addr, 
	/* [in] */ Boolean caseInsensitive)
{}

//-------------------------------------------------------------------------
// Methods called by host activity (in the same thread)
//-------------------------------------------------------------------------

CARAPI_(void) WebViewCore::StopLoading()
{}

//-------------------------------------------------------------------------
// Methods called by WebView
// If it refers to local variable, it needs synchronized().
// If it needs WebCore, it has to send message.
//-------------------------------------------------------------------------

CARAPI_(void) WebViewCore::SendMessage(
	/* [in] */ IMessage* msg)
{}

CARAPI_(void) WebViewCore::SendMessage(
	/* [in] */ Int32 what)
{}

CARAPI_(void) WebViewCore::SendMessage(
	/* [in] */ Int32 what, 
	/* [in] */ IInterface* obj)
{}

CARAPI_(void) WebViewCore::SendMessage(
	/* [in] */ Int32 what, 
	/* [in] */ Int32 arg1)
{}

CARAPI_(void) WebViewCore::SendMessage(
	/* [in] */ Int32 what, 
	/* [in] */ Int32 arg1, 
	/* [in] */ Int32 arg2)
{}

CARAPI_(void) WebViewCore::SendMessage(
	/* [in] */ Int32 what, 
	/* [in] */ Int32 arg1, 
	/* [in] */ IInterface* obj)
{}

CARAPI_(void) WebViewCore::SendMessage(
	/* [in] */ Int32 what, 
	/* [in] */ Int32 arg1, 
	/* [in] */ Int32 arg2, 
	/* [in] */ IInterface* obj)
{}

CARAPI_(void) WebViewCore::SendMessageAtFrontOfQueue(
	/* [in] */ Int32 what, 
	/* [in] */ IInterface* obj)
{}

CARAPI_(void) WebViewCore::SendMessageDelayed(
	/* [in] */ Int32 what, 
	/* [in] */ IInterface* obj, 
	/* [in] */ Int64 delay)
{}

CARAPI_(void) WebViewCore::RemoveMessages(
	/* [in] */ Int32 what)
{}

CARAPI_(void) WebViewCore::RemoveMessages()
{}

/**
* Removes pending messages and trigger a DESTROY message to send to
* WebCore.
* Called from UI thread.
*/
CARAPI_(void) WebViewCore::Destroy()
{}


///////////////////////////////////////////////////////////////////////////
// These are called from the UI thread, not our thread

/* package */ 
CARAPI_(void) WebViewCore::DrawContentPicture(
	/* [in] */ ICanvas* canvas, 
	/* [in] */ Int32 color,
	/* [in] */ Boolean animatingZoom,
	/* [in] */ Boolean animatingScroll)
{}

/* package */ 
/*synchronized*/
CARAPI_(Boolean) WebViewCore::PictureReady()
{}

/*package*/ 
/*synchronized*/
CARAPI_(AutoPtr<IPicture>) WebViewCore::CopyContentPicture()
{}

CARAPI_(void) WebViewCore::ReducePriority()
{}

CARAPI_(void) WebViewCore::ResumePriority()
{}

CARAPI_(void) WebViewCore::PauseUpdatePicture(
	/* [in] */ WebViewCore* core)
{}

CARAPI_(void) WebViewCore::ResumeUpdatePicture(
	/* [in] */ WebViewCore* core)
{}

//-------------------------------------------------------------------------
// Implement abstract methods in WebViewCore, native WebKit callback part
//-------------------------------------------------------------------------

// called from JNI or WebView thread
/* package */
CARAPI_(void) WebViewCore::ContentDraw()
{}

/*
* Called by the WebView thread
*/
/* package */
CARAPI_(void) WebViewCore::SignalRepaintDone()
{}

/* package */
CARAPI_(AutoPtr<IWebView>) WebViewCore::GetWebView()
{}

/**
* Add an error message to the client's console.
* @param message The message to add
* @param lineNumber the line on which the error occurred
* @param sourceID the filename of the source that caused the error.
* @param msgLevel the log level of this message. This is a value casted to int
*     from WebCore::MessageLevel in WebCore/page/Console.h.
*/
CARAPI_(void) WebViewCore::AddMessageToConsole(
	/* [in] */ const String& message, 
	/* [in] */ Int32 lineNumber, 
	/* [in] */ const String& sourceID,
	/* [in] */ Int32 msgLevel)
{}

/**
* Invoke a javascript alert.
* @param message The message displayed in the alert.
*/
CARAPI_(void) WebViewCore::JsAlert(
	/* [in] */ const String& url, 
	/* [in] */ const String& message)
{}

/**
* Notify the browser that the origin has exceeded it's database quota.
* @param url The URL that caused the overflow.
* @param databaseIdentifier The identifier of the database.
* @param currentQuota The current quota for the origin.
* @param estimatedSize The estimated size of the database.
*/
CARAPI_(void) WebViewCore::ExceededDatabaseQuota(
	/* [in] */ const String& url, 
	/* [in] */ const String& databaseIdentifier,
	/* [in] */ Int64 currentQuota,
	/* [in] */ Int64 estimatedSize)
{}

/**
* Notify the browser that the appcache has exceeded its max size.
* @param spaceNeeded is the amount of disk space that would be needed
* in order for the last appcache operation to succeed.
*/
CARAPI_(void) WebViewCore::ReachedMaxAppCacheSize(
	/* [in] */ Int64 spaceNeeded)
{}

CARAPI_(void) WebViewCore::PopulateVisitedLinks()
{}

/**
* Shows a prompt to ask the user to set the Geolocation permission state
* for the given origin.
* @param origin The origin for which Geolocation permissions are
*     requested.
*/
CARAPI_(void) WebViewCore::GeolocationPermissionsShowPrompt(
	/* [in] */ const String& origin)
{}

/**
* Hides the Geolocation permissions prompt.
*/
CARAPI_(void) WebViewCore::GeolocationPermissionsHidePrompt()
{}

/**
* Invoke a javascript confirm dialog.
* @param message The message displayed in the dialog.
* @return True if the user confirmed or false if the user cancelled.
*/
CARAPI_(Boolean) WebViewCore::JsConfirm(
	/* [in] */ const String& url, 
	/* [in] */ const String& message)
{}

/**
* Invoke a javascript prompt dialog.
* @param message The message to be displayed in the dialog.
* @param defaultValue The default value in the prompt input.
* @return The input from the user or null to indicate the user cancelled
*         the dialog.
*/
CARAPI_(CString) WebViewCore::JsPrompt(
	/* [in] */ const String& url, 
	/* [in] */ const String& message, 
	/* [in] */ const String& defaultValue)
{}

/**
* Invoke a javascript before unload dialog.
* @param url The url that is requesting the dialog.
* @param message The message displayed in the dialog.
* @return True if the user confirmed or false if the user cancelled. False
*         will cancel the navigation.
*/
CARAPI_(Boolean) WebViewCore::JsUnload(
	/* [in] */ const String& url, 
	/* [in] */ const String& message)
{}

/**
*
* Callback to notify that a JavaScript execution timeout has occured.
* @return True if the JavaScript execution should be interrupted. False
*         will continue the execution.
*/
CARAPI_(Boolean) WebViewCore::JsInterrupt()
{}

//-------------------------------------------------------------------------
// WebViewCore private methods
//-------------------------------------------------------------------------

CARAPI_(void) WebViewCore::ClearCache(
	/* [in] */ Boolean includeDiskFiles)
{}

CARAPI_(void) WebViewCore::LoadUrl(
	/* [in] */ const String& url, 
	/* [in] */ IObjectStringMap* extraHeaders)
{}

CARAPI_(void) WebViewCore::Key(
	/* [in] */ IKeyEvent* evt, 
	/* [in] */ Boolean isDown)
{}

// notify webkit that our virtual view size changed size (after inv-zoom)
CARAPI_(void) WebViewCore::ViewSizeChanged(
	/* [in] */ Int32 w, 
	/* [in] */ Int32 h, 
	/* [in] */ Int32 textwrapWidth, 
	/* [in] */ Float scale,
	/* [in] */ Int32 anchorX, 
	/* [in] */ Int32 anchorY, 
	/* [in] */ Boolean ignoreHeight)
{}

CARAPI_(void) WebViewCore::SendUpdateTextEntry()
{}

// Utility method for exceededDatabaseQuota and reachedMaxAppCacheSize
// callbacks. Computes the sum of database quota for all origins.
CARAPI_(Int64) WebViewCore::GetUsedQuota()
{}

CARAPI_(void) WebViewCore::WebkitDraw()
{}

/**
* Empty the picture set.
*/
/*native*/
CARAPI_(void) WebViewCore::NativeClearContent()
{}

/**
* Create a flat picture from the set of pictures.
*/
/*native*/
CARAPI_(void) WebViewCore::NativeCopyContentToPicture(
	/* [in] */ IPicture* picture)
{}

/**
* Draw the picture set with a background color. Returns true
* if some individual picture took too long to draw and can be
* split into parts. Called from the UI thread.
*/
/*native*/
CARAPI_(Boolean) WebViewCore::NativeDrawContent(
	/* [in] */ ICanvas* canvas, 
	/* [in] */ Int32 color)
{}

/**
* check to see if picture is blank and in progress
*/
/*native*/
CARAPI_(Boolean) WebViewCore::NativePictureReady()
{}

/**
* Redraw a portion of the picture set. The Point wh returns the
* width and height of the overall picture.
*/
/*native*/
CARAPI_(Boolean) WebViewCore::NativeRecordContent(
	/* [in] */ IRegion* invalRegion, 
	/* [in] */ IPoint* wh)
{}

/*native*/
CARAPI_(Boolean) WebViewCore::NativeFocusBoundsChanged()
{}

/**
* Splits slow parts of the picture set. Called from the webkit
* thread after nativeDrawContent returns true.
*/
/*native*/ 
CARAPI_(void) WebViewCore::NativeSplitContent()
{}

/*native*/
CARAPI_(Boolean) WebViewCore::NativeKey(
	/* [in] */ Int32 keyCode, 
	/* [in] */ Int32 unichar,
	/* [in] */ Int32 repeatCount,
	/* [in] */ Boolean isShift, 
	/* [in] */ Boolean isAlt,
	/* [in] */ Boolean isSym,
	/* [in] */ Boolean isDown)
{}

/*native*/
CARAPI_(void) WebViewCore::NativeClick(
	/* [in] */ Int32 framePtr, 
	/* [in] */ Int32 nodePtr)
{}

/*native*/
CARAPI_(void) WebViewCore::NativeSendListBoxChoices(
	/* [in] */ ArrayOf<Boolean> choices, 
	/* [in] */ Int32 size)
{}

/*native*/
CARAPI_(void) WebViewCore::NativeSendListBoxChoice(
/* [in] */ Int32 choice)
{}

/*  Tell webkit what its width and height are, for the purposes
of layout/line-breaking. These coordinates are in document space,
which is the same as View coords unless we have zoomed the document
(see nativeSetZoom).
screenWidth is used by layout to wrap column around. If viewport uses
fixed size, screenWidth can be different from width with zooming.
should this be called nativeSetViewPortSize?
*/
/*native*/
CARAPI_(void) WebViewCore::NativeSetSize(
	/* [in] */ Int32 width, 
	/* [in] */ Int32 height, 
	/* [in] */ Int32 screenWidth,
	/* [in] */ Float scale, 
	/* [in] */ Int32 realScreenWidth, 
	/* [in] */ Int32 screenHeight, 
	/* [in] */ Int32 anchorX,
	/* [in] */ Int32 anchorY, 
	/* [in] */ Boolean ignoreHeight)
{}

/*native*/
CARAPI_(Int32) WebViewCore::NativeGetContentMinPrefWidth()
{}

// Start: functions that deal with text editing
/*native*/
CARAPI_(void) WebViewCore::NativeReplaceTextfieldText(
	/* [in] */ Int32 oldStart, 
	/* [in] */ Int32 oldEnd, 
	/* [in] */ const String& replace, 
	/* [in] */ Int32 newStart, 
	/* [in] */ Int32 newEnd,
	/* [in] */ Int32 textGeneration)
{}

/*native*/
CARAPI_(void) WebViewCore::PassToJs(
	/* [in] */ Int32 gen,
	/* [in] */ const String& currentText, 
	/* [in] */ Int32 keyCode, 
	/* [in] */ Int32 keyValue, 
	/* [in] */ Boolean down,
	/* [in] */ Boolean cap, 
	/* [in] */ Boolean fn, 
	/* [in] */ Boolean sym)
{}

/*native*/
CARAPI_(void) WebViewCore::NativeSetFocusControllerActive(
	/* [in] */ Boolean active)
{}

/*native*/ 
CARAPI_(void) WebViewCore::NativeSaveDocumentState(
	/* [in] */ Int32 frame)
{}

/*native*/
CARAPI_(void) WebViewCore::NativeMoveFocus(
	/* [in] */ Int32 framePtr, 
	/* [in] */ Int32 nodePointer)
{}

/*native*/
CARAPI_(void) WebViewCore::NativeMoveMouse(
	/* [in] */ Int32 framePtr, 
	/* [in] */ Int32 x, 
	/* [in] */ Int32 y)
{}

/*native*/
CARAPI_(void) WebViewCore::NativeMoveMouseIfLatest(
	/* [in] */ Int32 moveGeneration,
	/* [in] */ Int32 framePtr, 
	/* [in] */ Int32 x, 
	/* [in] */ Int32 y)
{}

/*native*/
CARAPI_(CString) WebViewCore::NativeRetrieveHref(
	/* [in] */ Int32 framePtr, 
	/* [in] */ Int32 nodePtr)
{}

/*native*/
CARAPI_(CString) WebViewCore::NativeRetrieveAnchorText(
	/* [in] */ Int32 framePtr, 
	/* [in] */ Int32 nodePtr)
{}

/*native*/
CARAPI_(void) WebViewCore::NativeTouchUp(
	/* [in] */ Int32 touchGeneration,
	/* [in] */ Int32 framePtr, 
	/* [in] */ Int32 nodePtr, 
	/* [in] */ Int32 x, 
	/* [in] */ Int32 y)
{}

/*native*/
CARAPI_(Boolean) WebViewCore::NativeHandleTouchEvent(
	/* [in] */ Int32 action, 
	/* [in] */ Int32 x, 
	/* [in] */ Int32 y,
	/* [in] */ Int32 metaState)
{}

/*native*/
CARAPI_(void) WebViewCore::NativeUpdateFrameCache()
{}

/*native*/
CARAPI_(void) WebViewCore::NativeSetBackgroundColor(
	/* [in] */ Int32 color)
{}

/*native*/
CARAPI_(void) WebViewCore::WebViewCore::NativeDumpDomTree(
	/* [in] */ Boolean useFile)
{}

/*native*/
CARAPI_(void) WebViewCore::NativeDumpRenderTree(
	/* [in] */ Boolean useFile)
{}

/*native*/
CARAPI_(void) WebViewCore::NativeDumpNavTree()
{}

/*native*/
CARAPI_(void) WebViewCore::NativeDumpV8Counters()
{}

/*native*/
CARAPI_(void) WebViewCore::NativeSetJsFlags(
	/* [in] */ const String& flags)
{}

/**
*  Delete text from start to end in the focused textfield. If there is no
*  focus, or if start == end, silently fail.  If start and end are out of
*  order, swap them.
*  @param  start   Beginning of selection to delete.
*  @param  end     End of selection to delete.
*  @param  textGeneration Text generation number when delete was pressed.
*/
/*native*/
CARAPI_(void) WebViewCore::NativeDeleteSelection(
	/* [in] */ Int32 start, 
	/* [in] */ Int32 end,
	/* [in] */ Int32 textGeneration)
{}

/**
*  Set the selection to (start, end) in the focused textfield. If start and
*  end are out of order, swap them.
*  @param  start   Beginning of selection.
*  @param  end     End of selection.
*/
/*native*/
CARAPI_(void) WebViewCore::NativeSetSelection(
	/* [in] */ Int32 start, 
	/* [in] */ Int32 end)
{}

// Register a scheme to be treated as local scheme so that it can access
// local asset files for resources
/*native*/
CARAPI_(void) WebViewCore::NativeRegisterURLSchemeAsLocal(
	/* [in] */ const String& scheme)
{}

/*
* Inform webcore that the user has decided whether to allow or deny new
* quota for the current origin or more space for the app cache, and that
* the main thread should wake up now.
* @param limit Is the new quota for an origin or new app cache max size.
*/
/*native*/
CARAPI_(void) WebViewCore::NativeSetNewStorageLimit(
	/* [in] */ Int64 limit)
{}

/**
* Provide WebCore with a Geolocation permission state for the specified
* origin.
* @param origin The origin for which Geolocation permissions are provided.
* @param allow Whether Geolocation permissions are allowed.
* @param remember Whether this decision should be remembered beyond the
*     life of the current page.
*/
/*native*/
CARAPI_(void) WebViewCore::NativeGeolocationPermissionsProvide(
	/* [in] */ const String& origin, 
	/* [in] */ Boolean allow, 
	/* [in] */ Boolean remember)
{}

/**
* Provide WebCore with the previously visted links from the history database
*/
/* native */
CARAPI_(void) WebViewCore::NativeProvideVisitedHistory(
	/* [in] */ ArrayOf<String>& history)
{}

CARAPI_(void) WebViewCore::WebCoreThread::Run()
{}

/**
* Called by JNI.  Open a file chooser to upload a file.
* @return String version of the URI plus the name of the file.
* FIXME: Just return the URI here, and in FileSystem::pathGetFileName, call
* into Java to get the filename.
*/
CARAPI_(String) WebViewCore::OpenFileChooser()
{}

//////////////////////////////////////////////////////////////////////////

CARAPI_(void) WebViewCore::_RestoreState(
	/* [in] */ Int32 index)
{}

// called by JNI
CARAPI_(void) WebViewCore::ContentScrollBy(
	/* [in] */ Int32 dx, 
	/* [in] */ Int32 dy, 
	/* [in] */ Boolean animate)
{}

// called by JNI
CARAPI_(void) WebViewCore::ContentScrollTo(
	/* [in] */ Int32 x,
	/* [in] */ Int32 y)
{}

// called by JNI
CARAPI_(void) WebViewCore::ContentSpawnScrollTo(
	/* [in] */ Int32 x, 
	/* [in] */ Int32 y)
{}

// called by JNI
CARAPI_(void) WebViewCore::SendNotifyProgressFinished()
{}

/*  Called by JNI. The coordinates are in doc coordinates, so they need to
be scaled before they can be used by the view system, which happens
in WebView since it (and its thread) know the current scale factor.
*/
CARAPI_(void) WebViewCore::SendViewInvalidate(
	/* [in] */ Int32 left, 
	/* [in] */ Int32 top, 
	/* [in] */ Int32 right, 
	/* [in] */ Int32 bottom)
{}

// called by JNI
CARAPI_(void) WebViewCore::SendImmediateRepaint()
{}

// called by JNI
CARAPI_(void) WebViewCore::SetRootLayer(
	/* [in] */ Int32 layer)
{}

/*native*/
CARAPI_(void) WebViewCore::SetViewportSettingsFromNative()
{}

// called by JNI
CARAPI_(void) WebViewCore::DidFirstLayout(
	/* [in] */ Boolean standardLoad)
{}

// called by JNI
CARAPI_(void) WebViewCore::UpdateViewport()
{}

CARAPI_(void) WebViewCore::SetupViewport(
	/* [in] */ Boolean updateRestoreState)
{}

// called by JNI
CARAPI_(void) WebViewCore::RestoreScale(
	/* [in] */ Int32 scale)
{}

// called by JNI
CARAPI_(void) WebViewCore::RestoreScreenWidthScale(
	/* [in] */ Int32 scale)
{}

// called by JNI
CARAPI_(void) WebViewCore::NeedTouchEvents(
	/* [in] */ Boolean need)
{}

// called by JNI
CARAPI_(void) WebViewCore::UpdateTextfield(
	/* [in] */ Int32 ptr, 
	/* [in] */ Boolean changeToPassword,
	/* [in] */ CString text, 
	/* [in] */ Int32 textGeneration)
{}

// called by JNI
CARAPI_(void) WebViewCore::UpdateTextSelection(
	/* [in] */ Int32 pointer,
	/* [in] */ Int32 start, 
	/* [in] */ Int32 end,
	/* [in] */ Int32 textGeneration)
{}

// called by JNI
CARAPI_(void) WebViewCore::ClearTextEntry()
{}

// called by JNI
CARAPI_(void) WebViewCore::SendFindAgain()
{}

/*native*/
CARAPI_(void) WebViewCore::NativeUpdateFrameCacheIfLoading()
{}

/*native*/
CARAPI_(String&) WebViewCore::NativeRequestLabel(
	/* [in] */ Int32 framePtr, 
	/* [in] */ Int32 nodePtr)
{}

/**
* Scroll the focused textfield to (xPercent, y) in document space
*/
/*native*/ 
CARAPI_(void) WebViewCore::NativeScrollFocusedTextInput(
	/* [in] */ Float xPercent, 
	/* [in] */ Int32 y)
{}

// these must be in document space (i.e. not scaled/zoomed).
/*native*/ 
CARAPI_(void) WebViewCore::NativeSetScrollOffset(
	/* [in] */ Int32 gen, 
	/* [in] */ Int32 dx, 
	/* [in] */ Int32 dy)
{}

/*native*/ 
CARAPI_(void) WebViewCore::NativeSetGlobalBounds(
	/* [in] */ Int32 x, 
	/* [in] */ Int32 y, 
	/* [in] */ Int32 w, 
	/* [in] */ Int32 h)
{}

// called by JNI
CARAPI_(void) WebViewCore::RequestListBox(
	/* [in] */ ArrayOf<String>& array, 
	/* [in] */ ArrayOf<Int32> enabledArray,
	/* [in] */ ArrayOf<Int32> selectedArray)
{}

// called by JNI
CARAPI_(void) WebViewCore::RequestListBox(
	/* [in] */ ArrayOf<String> array, 
	/* [in] */ ArrayOf<Int32> enabledArray,
	/* [in] */ Int32 selection)
{}

// called by JNI
CARAPI_(void) WebViewCore::RequestKeyboardWithSelection(
	/* [in] */ Int32 pointer, 
	/* [in] */ Int32 selStart,
	/* [in] */ Int32 selEnd, 
	/* [in] */ Int32 textGeneration)
{}

// called by JNI
CARAPI_(void) WebViewCore::RequestKeyboard(
	/* [in] */ Boolean showKeyboard)
{}

// called by JNI
CARAPI_(AutoPtr<IContext>) WebViewCore::GetContext()
{}

// called by JNI
CARAPI_(IInterface*) WebViewCore::GetPluginClass(
	/* [in] */ const String& libName, 
	/* [in] */ const String& clsName)
{}

// called by JNI. PluginWidget function to launch a full-screen view using a
// View object provided by the plugin class.
CARAPI_(void) WebViewCore::ShowFullScreenPlugin(
	/* [in] */ ViewManager::ChildView* childView, 
	/* [in] */ Int32 npp)
{}

// called by JNI
CARAPI_(void) WebViewCore::HideFullScreenPlugin()
{}

// called by JNI.  PluginWidget functions for creating an embedded View for
// the surface drawing model.
CARAPI_(ViewManager::ChildView*) WebViewCore::AddSurface(
	/* [in] */ IView* pluginView, 
	/* [in] */ Int32 x, 
	/* [in] */ Int32 y,
	/* [in] */ Int32 width, 
	/* [in] */ Int32 height)
{}

CARAPI_(void) WebViewCore::UpdateSurface(
	/* [in] */ ViewManager::ChildView* childView, 
	/* [in] */ Int32 x, 
	/* [in] */ Int32 y,
	/* [in] */ Int32 width, 
	/* [in] */ Int32 height)
{}

CARAPI_(void) WebViewCore::DestroySurface(
	/* [in] */ ViewManager::ChildView childView)
{}

CARAPI_(void) WebViewCore::ShowRect(
	/* [in] */ Int32 left,
	/* [in] */ Int32 top, 
	/* [in] */ Int32 width, 
	/* [in] */ Int32 height,
	/* [in] */ Int32 contentWidth, 
	/* [in] */ Int32 contentHeight, 
	/* [in] */ Float xPercentInDoc,
	/* [in] */ Float xPercentInView, 
	/* [in] */ Float yPercentInDoc, 
	/* [in] */ Float yPercentInView)
{}

// called by JNI
CARAPI_(void) WebViewCore::CenterFitRect(
	/* [in] */ Int32 x, 
	/* [in] */ Int32 y, 
	/* [in] */ Int32 width, 
	/* [in] */ Int32 height)
{}

// called by JNI
CARAPI_(void) WebViewCore::SetScrollbarModes(
	/* [in] */ Int32 hMode, 
	/* [in] */ Int32 vMode)
{}

/*native*/
CARAPI_(void) WebViewCore::NativePause()
{}

/*native*/
CARAPI_(void) WebViewCore::NativeResume()
{}

/*native*/
CARAPI_(void) WebViewCore::NativeFreeMemory()
{}

/*native*/
CARAPI_(void) WebViewCore::NativeFullScreenPluginHidden(
	/* [in] */ Int32 npp)
{}

/*native*/
CARAPI_(Boolean) WebViewCore::NativeValidNodeAndBounds(
	/* [in] */ Int32 frame, 
	/* [in] */ Int32 node,
	/* [in] */ IRect* bounds)
{}