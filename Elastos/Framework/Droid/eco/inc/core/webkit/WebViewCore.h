#ifndef __WEBVIEWCORE_H__
#define __WEBVIEWCORE_H__

#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>

#include "ViewManager.h"
#include "WebSettings.h"
#include <elastos/Mutex.h>
#include "webkit/CWebStorage.h"
#include "os/Runnable.h"
#include <elastos/List.h>
#include "graphics/CPoint.h"
#include "graphics/CRegion.h"

#include <elastos/Set.h>

class WebViewCore : public ElRefBase
{
public:
    class WvcWebStorageQuotaUpdater: public WebStorageQuotaUpdater
    {
    public:
        virtual CARAPI UpdateQuota(
            /* [in] */  Int64 newQuota);

        WvcWebStorageQuotaUpdater(
            /* [in] */  WebViewCore* webViewCore);

    private:
        WebViewCore* mWebViewCore;
    };

private:
    // Class for providing Handler creation inside the WebCore thread.
    /*static*/ 
    class WebCoreThread: public Runnable 
    {
        friend class WebViewCore;
    public:
        class WvcWctHandler:public ElRefBase, public IApartment
        {
        public:
            CARAPI_(PInterface) Probe(
                /* [in] */ REIID riid);

            CARAPI_(UInt32) AddRef();

            CARAPI_(UInt32) Release();

            CARAPI GetInterfaceID(
                /* [in] */ IInterface* Object,
                /* [out] */ InterfaceID* iID);
        public:
            //IApartment
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
            Handle32 GetFunc(
                /* [in] */ Int32 message);
        public:
            void HandleInitialize(
                /* [in] */ WebViewCore* webViewCore);
            void HandleReducePriority();
            void HandleResumePriority();

        public:
            WvcWctHandler();
        private:
            AutoPtr<IApartment> mApartment;
        };
    public:
        virtual CARAPI Run();

    private:
        // Message id for initializing a new WebViewCore.
        static const Int32 INITIALIZE = 0;
        static const Int32 REDUCE_PRIORITY = 1;
        static const Int32 RESUME_PRIORITY = 2;
    };

public:
    friend class WvcWebStorageQuotaUpdater;
    friend class WebCoreThread;

    struct BaseUrlData 
    {
        String mBaseUrl;
        String mData;
        String mMimeType;
        String mEncoding;
        String mHistoryUrl;
    };

    struct CursorData 
    {
        CursorData() {}
        CursorData(
            /* [in] */ Int32 frame, 
            /* [in] */ Int32 node, 
            /* [in] */ Int32 x, 
            /* [in] */ Int32 y) : mFrame(frame), mNode(node),
                        mX(x), mY(y) {}

        Int32 mMoveGeneration;
        Int32 mFrame;
        Int32 mNode;
        Int32 mX;
        Int32 mY;
    };

    struct JSInterfaceData 
    {
        IInterface* mObject;
        String mInterfaceName;
    };

    struct JSKeyData 
    {
        String mCurrentText;
        AutoPtr<IKeyEvent> mEvent;
    };

    struct MotionUpData 
    {
        Int32 mFrame;
        Int32 mNode;
        IRect* mBounds;
        Int32 mX;
        Int32 mY;
    };

    struct GetUrlData 
    {
        String mUrl;
        IObjectStringMap* mExtraHeaders;
    };

    struct PostUrlData 
    {
        String mUrl;
        ArrayOf<Byte>* mPostData;
    };

    struct ReplaceTextData 
    {
        String mReplace;
        Int32 mNewStart;
        Int32 mNewEnd;
        Int32 mTextGeneration;
    };

    struct TextSelectionData 
    {
        TextSelectionData(
            /* [in] */ Int32 start, 
            /* [in] */ Int32 end) : mStart(start), mEnd(end)
        {}

        Int32 mStart;
        Int32 mEnd;
    };

    struct TouchUpData 
    {
        Int32 mMoveGeneration;
        Int32 mFrame;
        Int32 mNode;
        Int32 mX;
        Int32 mY;
    };

    struct TouchEventData 
    {
        Int32 mAction;
        Int32 mX;
        Int32 mY;
        Int32 mMetaState;
        Boolean mReprocess;
        Float mViewX;
        Float mViewY;
    };

    struct GeolocationPermissionsData 
    {
        String mOrigin;
        Boolean mAllow;
        Boolean mRemember;
    };


    struct RestoreState 
    {
        Float mMinScale;
        Float mMaxScale;
        Float mViewScale;
        Float mTextWrapScale;
        Float mDefaultScale;
        Int32 mScrollX;
        Int32 mScrollY;
        Boolean mMobileSite;
    };

    struct DrawData 
    {
        DrawData();
        ~DrawData();
        AutoPtr<IRegion> mInvalRegion;
        AutoPtr<IPoint> mViewPoint;
        AutoPtr<IPoint> mWidthHeight;
        Int32 mMinPrefWidth;
        RestoreState* mRestoreState; // only non-null if it is for the first
                                    // picture set after the first layout
        Boolean mFocusSizeChanged;
    };

    // called by JNI
    struct ShowRectData 
    {
        Int32 mLeft;
        Int32 mTop;
        Int32 mWidth;
        Int32 mHeight;
        Int32 mContentWidth;
        Int32 mContentHeight;
        Float mXPercentInDoc;
        Float mXPercentInView;
        Float mYPercentInDoc;
        Float mYPercentInView;
    };

    class EventHub: public ElRefBase
    {
    public:
        class WvcEhHandler:public ElRefBase, public IApartment
        {
        public:
            CARAPI_(PInterface) Probe(
                /* [in] */ REIID riid);

            CARAPI_(UInt32) AddRef();

            CARAPI_(UInt32) Release();

            CARAPI GetInterfaceID(
                /* [in] */ IInterface* Object,
                /* [out] */ InterfaceID* iID);
        public:
            //IApartment
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
            Handle32 GetFunc(
                /* [in] */ Int32 message);

        public:
            //EventHub::WEBKIT_DRAW
            void HandleWebkitDraw();
            //EventHub::DESTROY
            void HandleDestroy();
            //EventHub::REQUEST_LABEL
            void HandleRequestLabel(
                /* [in] */ Int32 arg1, 
                /* [in] */ Int32 arg2);
            //EventHub::UPDATE_FRAME_CACHE_IF_LOADING
            void HandleUpdateFrameCacheIfLoading();
            //EventHub::SCROLL_TEXT_INPUT
            void HandleScrollTextInput(
                /* [in] */ Int32 arg1,
                /* [in] */ Float obj);
            //EventHub::LOAD_URL
            void HandleLoadUrl(
                /* [in] */ GetUrlData* obj);
            //EventHub::POST_URL
            void HandlePostUrl(
                /* [in] */ PostUrlData* obj);
            //EventHub::LOAD_DATA
            void HandleLoadData(
                /* [in] */ BaseUrlData* obj);
            //EventHub::STOP_LOADING
            void HandleStopLoading();
            //EventHub::RELOAD
            void HandleReload();
            //EventHub::KEY_DOWN
            void HandleKeyDown(
                /* [in] */ IKeyEvent* obj);
            //EventHub::KEY_UP
            void HandleKeyUp(
                /* [in] */ IKeyEvent* obj);
            //EventHub::CLICK:
            void HandleClick(
                /* [in] */ Int32 arg1, 
                /* [in] */ Int32 arg2);
            //EventHub::VIEW_SIZE_CHANGED
            void HandleViewSizeChanged(
                /* [in] */ /*CWebView::ViewSizeData* */Handle32 obj);
            //EventHub::SET_SCROLL_OFFSET
            void HandleSetScrollOffset(
                /* [in] */ Int32 arg1, 
                /* [in] */ CPoint* obj);
            //EventHub::SET_GLOBAL_BOUNDS
            void HandleSetGlobalBounds(
                /* [in] */ IRect* obj);
            //EventHub::GO_BACK_FORWARD
            void HandleGoBackForward(
                /* [in] */ Int32 arg1);
            //EventHub::RESTORE_STATE
            void HandleRestoreState(
                /* [in] */ Int32 arg1);
            //EventHub::PAUSE_TIMERS
            void HandlePauseTimers();
            //EventHub::RESUME_TIMERS
            void HandleResumeTimers();
            //EventHub::ON_PAUSE
            void HandleOnPause();
            //EventHub::ON_RESUME
            void HandleOnResume();
            //EventHub::FREE_MEMORY
            void HandleFreeMemory();
            //EventHub::SET_NETWORK_STATE
            void HandleSetNetworkState(
                /* [in] */ Int32 arg1);
            //EventHub::SET_NETWORK_TYPE
            void HandleSetNetworkType(
                /* [in] */ IObjectStringMap* obj);
            //EventHub::CLEAR_CACHE
            void HandleClearCache(
                /* [in] */ Int32 arg1);
            //EventHub::CLEAR_HISTORY
            void HandleClearHistory();
            //EventHub::REPLACE_TEXT
            void HandleReplaceText(
                /* [in] */ Int32 arg1, 
                /* [in] */ Int32 arg2, 
                /* [in] */ ReplaceTextData* obj);
            //EventHub::PASS_TO_JS
            void HandlePassToJs(
                /* [in] */ Int32 arg1, 
                /* [in] */ JSKeyData* obj);
            //EventHub::SAVE_DOCUMENT_STATE
            void HandleSaveDocumentState(
                /* [in] */ CursorData* obj);
            //EventHub::CLEAR_SSL_PREF_TABLE
            void HandleClearSslPrefTable();
            //EventHub::TOUCH_UP
            void HandleTouchUp(
                /* [in] */ TouchUpData* obj);
            //EventHub::TOUCH_EVENT
            void HandleTouchEvent(
                /* [in] */ TouchEventData* obj);
            //EventHub::SET_ACTIVE
            void HandleSetActive(
                /* [in] */ Int32 arg1);
            //EventHub::ADD_JS_INTERFACE
            void HandleAddJsInterface(
                /* [in] */ JSInterfaceData* obj);
            //EventHub::REQUEST_EXT_REPRESENTATION
            void HandleRequestExtRepresentation(
                /* [in] */ IApartment* apartmentR,
                /* [in] */ Int32 message,
                /* [in] */ IParcel* params);
            //EventHub::REQUEST_DOC_AS_TEXT
            void HandleRequestDocAsText(
                /* [in] */ IApartment* apartmentR,
                /* [in] */ Int32 message,
                /* [in] */ IParcel* params);
            //EventHub::SET_MOVE_FOCUS
            void HandleSetMoveFocus(
                /* [in] */ CursorData* obj);
            //EventHub::SET_MOVE_MOUSE
            void HandleSetMoveMouse(
                /* [in] */ CursorData* obj);
            //EventHub::SET_MOVE_MOUSE_IF_LATEST
            void HandleSetMoveMouseIfLatest(
                /* [in] */ CursorData* obj);
            //EventHub::REQUEST_CURSOR_HREF
            void HandleRequestCursorHref(
                /* [in] */ Int32 arg1,
                /* [in] */ Int32 arg2,    
                /* [in] */ IApartment* apartmentR,
                /* [in] */ Int32 message,
                /* [in] */ IParcel* params);
            //EventHub::UPDATE_CACHE_AND_TEXT_ENTRY
            void HandleUpdateCacheAndTextEntry();
            //EventHub::DOC_HAS_IMAGES
            void HandleDocHasImages(
                /* [in] */ IApartment* apartmentR,
                /* [in] */ Int32 message,
                /* [in] */ IParcel* params);
            //EventHub::DELETE_SELECTION
            void HandleDeleteSelection(
                /* [in] */ Int32 arg1,
                /* [in] */ TextSelectionData* obj);
            //EventHub::SET_SELECTION
            void HandleSetSelection(
                /* [in] */ Int32 arg1,
                /* [in] */ Int32 arg2);
            //EventHub::LISTBOX_CHOICES
            void HandleListboxChoices(
                /* [in] */ Int32 arg1//,
                /* [in] */ /*ISparseBooleanArray* obj*/);
            //EventHub::SINGLE_LISTBOX_CHOICE
            void HandleSingleListboxChoices(
                /* [in] */ Int32 arg1);
            //EventHub::SET_BACKGROUND_COLOR
            void HandleSetBackgroundColor(
                /* [in] */ Int32 arg1);
            //EventHub::DUMP_DOMTREE
            void HandleDumpDomTree(
                /* [in] */ Int32 arg1);
            //EventHub::DUMP_RENDERTREE
            void HandleDumpRenderTree(
                /* [in] */ Int32 arg1);
            //EventHub::DUMP_NAVTREE
            void HandleDumpNavTree();
            //DUMP_V8COUNTERS
            void HandleDumpV8Counters();
            //EventHub::SET_JS_FLAGS
            void HandleSetJsFlags(
                /* [in] */ String obj);
            //EventHub::GEOLOCATION_PERMISSIONS_PROVIDE
            void HandleGeolocationPermissionsProvide(
                /* [in] */ GeolocationPermissionsData* obj);
            //EventHub::SYNC_SCROLL
            void HandleSyncScroll(
                /* [in] */ Int32 arg1,
                /* [in] */ Int32 arg2);
            //EventHub::SPLIT_PICTURE_SET
            void HandleSplitPictureSet();
            //EventHub::CLEAR_CONTENT
            void HandleClearContent();
            //EventHub::MESSAGE_RELAY
            void HandleMessageRelay(
                /* [in] */ IApartment* apartment,
                /* [in] */ Int32 message,
                /* [in] */ IParcel* params);
            //EventHub::POPULATE_VISITED_LINKS
            void HandlePopulateVisitedLinks(
                /* [in] */ ArrayOf<String>* obj);
            //EventHub::VALID_NODE_BOUNDS
            void HandleValidNodeBounds(
                /* [in] */ MotionUpData* obj);
            //EventHub::HIDE_FULLSCREEN
            void HandleHideFullScreen(
                /* [in] */ Int32 arg1);
            //EventHub::ADD_PACKAGE_NAMES
            void HandleAddPackageNames(
                /* [in] */ Set<String>* obj);
            //EventHub::ADD_PACKAGE_NAME
            void HandleAddPackageName(
                /* [in] */ String obj);
            //EventHub::REMOVE_PACKAGE_NAME
            void HandleRemovePackageName(
                /* [in] */ String obj);

        public:
            WvcEhHandler(
                /* [in] */ WebViewCore* webViewCore,
                /* [in] */ EventHub* eventHub);

        private:
            WebViewCore* mWebViewCore;
            EventHub* mEventHub;
            AutoPtr<IApartment> mApartment;
        };

        class Msg : public ElRefBase
        {
        public:
            //AutoPtr<IApartment> apartment;
            Int32 message;
            AutoPtr<IParcel> params;
        }; 
        
    public:
        friend class WebViewCore;
        friend class WvcEhHandler;

        EventHub(
            /* [in] */ WebViewCore* webViewCore);

    public:
        // Message Ids
        static const Int32 REQUEST_LABEL = 97;
        static const Int32 UPDATE_FRAME_CACHE_IF_LOADING = 98;
        static const Int32 SCROLL_TEXT_INPUT = 99;
        static const Int32 LOAD_URL = 100;
        static const Int32 STOP_LOADING = 101;
        static const Int32 RELOAD = 102;
        static const Int32 KEY_DOWN = 103;
        static const Int32 KEY_UP = 104;
        static const Int32 VIEW_SIZE_CHANGED = 105;
        static const Int32 GO_BACK_FORWARD = 106;
        static const Int32 SET_SCROLL_OFFSET = 107;
        static const Int32 RESTORE_STATE = 108;
        static const Int32 PAUSE_TIMERS = 109;
        static const Int32 RESUME_TIMERS = 110;
        static const Int32 CLEAR_CACHE = 111;
        static const Int32 CLEAR_HISTORY = 112;
        static const Int32 SET_SELECTION = 113;
        static const Int32 REPLACE_TEXT = 114;
        static const Int32 PASS_TO_JS = 115;
        static const Int32 SET_GLOBAL_BOUNDS = 116;
        static const Int32 UPDATE_CACHE_AND_TEXT_ENTRY = 117;
        static const Int32 CLICK = 118;
        static const Int32 SET_NETWORK_STATE = 119;
        static const Int32 DOC_HAS_IMAGES = 120;
        static const Int32 DELETE_SELECTION = 122;
        static const Int32 LISTBOX_CHOICES = 123;
        static const Int32 SINGLE_LISTBOX_CHOICE = 124;
        static const Int32 MESSAGE_RELAY = 125;
        static const Int32 SET_BACKGROUND_COLOR = 126;
        static const Int32 SET_MOVE_FOCUS = 127;
        static const Int32 SAVE_DOCUMENT_STATE = 128;

        static const Int32 WEBKIT_DRAW = 130;
        static const Int32 SYNC_SCROLL = 131;
        static const Int32 POST_URL = 132;
        static const Int32 SPLIT_PICTURE_SET = 133;
        static const Int32 CLEAR_CONTENT = 134;

        // UI nav messages
        static const Int32 SET_MOVE_MOUSE = 135;
        static const Int32 SET_MOVE_MOUSE_IF_LATEST = 136;
        static const Int32 REQUEST_CURSOR_HREF = 137;
        static const Int32 ADD_JS_INTERFACE = 138;
        static const Int32 LOAD_DATA = 139;

        // motion
        static const Int32 TOUCH_UP = 140;
        // message used to pass UI touch events to WebCore
        static const Int32 TOUCH_EVENT = 141;

        // Used to tell the focus controller not to draw the blinking cursor,
        // based on whether the WebView has focus and whether the WebView's
        // cursor matches the webpage's focus.
        static const Int32 SET_ACTIVE = 142;

        // lifecycle activities for just this DOM (unlike pauseTimers, which
        // is global)
        static const Int32 ON_PAUSE = 143;
        static const Int32 ON_RESUME = 144;
        static const Int32 FREE_MEMORY = 145;
        static const Int32 VALID_NODE_BOUNDS = 146;

        // Network-based messaging
        static const Int32 CLEAR_SSL_PREF_TABLE = 150;

        // Test harness messages
        static const Int32 REQUEST_EXT_REPRESENTATION = 160;
        static const Int32 REQUEST_DOC_AS_TEXT = 161;

        // debugging
        static const Int32 DUMP_DOMTREE = 170;
        static const Int32 DUMP_RENDERTREE = 171;
        static const Int32 DUMP_NAVTREE = 172;
        static const Int32 DUMP_V8COUNTERS = 173;

        static const Int32 SET_JS_FLAGS = 174;
        // Geolocation
        static const Int32 GEOLOCATION_PERMISSIONS_PROVIDE = 180;

        static const Int32 POPULATE_VISITED_LINKS = 181;

        static const Int32 HIDE_FULLSCREEN = 182;

        static const Int32 SET_NETWORK_TYPE = 183;

        // navigator.isApplicationInstalled()
        static const Int32 ADD_PACKAGE_NAMES = 184;
        static const Int32 ADD_PACKAGE_NAME = 185;
        static const Int32 REMOVE_PACKAGE_NAME = 186;

    private:

        // private message ids
        static const Int32 DESTROY = 200;

        // Private handler for WebCore messages.
        AutoPtr<IApartment> mHandler;

        // Message queue for containing messages before the WebCore thread is
        // ready.        
        List< AutoPtr<Msg> >* mMessages;
        // Flag for blocking messages. This is used during DESTROY to avoid
        // posting more messages to the EventHub or to WebView's event handler.
        Boolean mBlockMessages;

        Int32 mTid;
        Int32 mSavedPriority;

        Core::Threading::Mutex mutexThis;

        WebViewCore* mWebViewCore;

    private:

        /**
         * Prevent other classes from creating an EventHub.
         */
        EventHub() {}

        /**
         * Transfer all messages to the newly created webcore thread handler.
         */
        CARAPI_(void) TransferMessages();

        /**
         * Send a message internally to the queue or to the handler
         *
         * synchronized
         */
        CARAPI_(void) SendMessage(
            /* [in] */ Msg* msg);

        /* synchronized */
        CARAPI_(void) RemoveMessages(
            /* [in] */ Int32 what);

        /* synchronized */
        CARAPI_(Boolean) HasMessages(
            /* [in] */ Int32 what);

        /* synchronized */
        CARAPI_(void) SendMessageDelayed(
            /* [in] */ Msg* msg, 
            /* [in] */ Int64 delay);

        /**
         * Send a message internally to the front of the queue.
         * 
         * synchronized
         */
        CARAPI_(void) SendMessageAtFrontOfQueue(
            /* [in] */ Msg* msg);

        /**
         * Remove all the messages.
         *
         * synchronized
         */
        CARAPI_(void) RemoveMessages();

        /**
         * Block sending messages to the EventHub.
         *
         * synchronized
         */
        CARAPI_(void) BlockMessages();
    };

public:
    friend class EventHub::WvcEhHandler;

public:
    /*static {
        // Load libwebcore during static initialization. This happens in the
        // zygote process so it will be shared read-only across all app
        // processes.
        System.loadLibrary("webcore");
    }*/

    WebViewCore(
        /* [in] */ IContext* context, 
        /* [in] */ IWebView* w, 
        /* [in] */ ICallbackProxy* proxy,
        /* [in] */ IObjectStringMap* javascriptInterfaces);

    ~WebViewCore();

    /* Initialize private data within the WebCore thread.
     */
    virtual CARAPI_(void) Initialize();

    /* Handle the initialization of WebViewCore during subwindow creation. This
     * method is called from the WebCore thread but it is called before the
     * INITIALIZE message can be handled.
     */
    /* package */
    virtual CARAPI_(void) InitializeSubwindow();

    /* Get the BrowserFrame component. This is used for subwindow creation and
     * is called only from BrowserFrame in the WebCore thread. */
    /* package */ 
    virtual CARAPI_(AutoPtr<IBrowserFrame>) GetBrowserFrame();

    //-------------------------------------------------------------------------
    // Common methods
    //-------------------------------------------------------------------------

    /**
     * Causes all timers to pause. This applies to all WebViews in the current
     * app process.
     */
    static CARAPI_(void) PauseTimers();

    /**
     * Resume all timers. This applies to all WebViews in the current process.
     */
    static CARAPI_(void) ResumeTimers();

    virtual CARAPI_(WebSettings*) GetSettings();

    /*
     * Given mimeType, check whether it's supported in Android media framework.
     * mimeType could be such as "audio/ogg" and "video/mp4".
     */
    /* package */
    static CARAPI_(Boolean) SupportsMimeType(
        /* [in] */ const String& mimeType);



    //-------------------------------------------------------------------------
    // JNI methods
    //-------------------------------------------------------------------------

    /* native*/
    static CARAPI_(String) NativeFindAddress(
        /* [in] */ const String& addr, 
        /* [in] */ Boolean caseInsensitive);





    // mAction of TouchEventData can be MotionEvent.getAction() which uses the
    // last two bytes or one of the following values
    static const Int32 ACTION_LONGPRESS = 0x100;
    static const Int32 ACTION_DOUBLETAP = 0x200;

    static const CString HandlerDebugString[];/* = {
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
    };*/

    

    //-------------------------------------------------------------------------
    // Methods called by host activity (in the same thread)
    //-------------------------------------------------------------------------

    virtual CARAPI_(void) StopLoading();

    //-------------------------------------------------------------------------
    // Methods called by WebView
    // If it refers to local variable, it needs synchronized().
    // If it needs WebCore, it has to send message.
    //-------------------------------------------------------------------------

    virtual CARAPI_(void) SendMessage(
        /* [in] */ /*IApartment* apartmentR,*/
        /* [in] */ Int32 message,
        /* [in] */ IParcel* params);

    virtual CARAPI_(void) SendMessage(
        /* [in] */ Int32 message);

    virtual CARAPI_(void) SendMessage(
        /* [in] */ Int32 message, 
        /* [in] */ IInterface* obj);

    virtual CARAPI_(void) SendMessage(
        /* [in] */ Int32 message, 
        /* [in] */ Int32 arg1);

    virtual CARAPI_(void) SendMessage(
        /* [in] */ Int32 message, 
        /* [in] */ Int32 arg1, 
        /* [in] */ Int32 arg2);

    virtual CARAPI_(void) SendMessage(
        /* [in] */ Int32 message, 
        /* [in] */ Int32 arg1, 
        /* [in] */ IInterface* obj);

    virtual CARAPI_(void) SendMessage(
        /* [in] */ Int32 message, 
        /* [in] */ Int32 arg1, 
        /* [in] */ Int32 arg2, 
        /* [in] */ IInterface* obj);

    virtual CARAPI_(void) SendMessageAtFrontOfQueue(
        /* [in] */ Int32 message, 
        /* [in] */ IInterface* obj);

    virtual CARAPI_(void) SendMessageDelayed(
        /* [in] */ Int32 message, 
        /* [in] */ IInterface* obj, 
        /* [in] */ Int64 delay);

    virtual CARAPI_(void) RemoveMessages(
        /* [in] */ Int32 message);

    virtual CARAPI_(void) RemoveMessages();

    /**
     * Removes pending messages and trigger a DESTROY message to send to
     * WebCore.
     * Called from UI thread.
     */
    virtual CARAPI_(void) Destroy();


    ///////////////////////////////////////////////////////////////////////////
    // These are called from the UI thread, not our thread

    static const Int32 ZOOM_BITS;/* = Paint.FILTER_BITMAP_FLAG |
                                         Paint.DITHER_FLAG |
                                         Paint.SUBPIXEL_TEXT_FLAG;*/
    static const Int32 SCROLL_BITS;/* = Paint.FILTER_BITMAP_FLAG |
                                           Paint.DITHER_FLAG;*/

    const AutoPtr<IDrawFilter> mZoomFilter;
    // If we need to trade better quality for speed, set mScrollFilter to null
    const AutoPtr<IDrawFilter> mScrollFilter;

    /* package */ 
    virtual CARAPI_(void) DrawContentPicture(
        /* [in] */ ICanvas* canvas, 
        /* [in] */ Int32 color,
        /* [in] */ Boolean animatingZoom,
        /* [in] */ Boolean animatingScroll);

    /* package */ 
    /*synchronized*/
    virtual CARAPI_(Boolean) PictureReady();

    /*package*/ 
    /*synchronized*/
    virtual CARAPI_(AutoPtr<IPicture>) CopyContentPicture();

    static CARAPI_(void) ReducePriority();

    static CARAPI_(void) ResumePriority();

    static CARAPI_(void) PauseUpdatePicture(
        /* [in] */ WebViewCore* core);

    static CARAPI_(void) ResumeUpdatePicture(
        /* [in] */ WebViewCore* core);

    //-------------------------------------------------------------------------
    // Implement abstract methods in WebViewCore, native WebKit callback part
    //-------------------------------------------------------------------------

    // called from JNI or WebView thread
    /* package */
    virtual CARAPI_(void) ContentDraw();

    /*
     * Called by the WebView thread
     */
    /* package */
    virtual CARAPI_(void) SignalRepaintDone();

    /* package */
    virtual CARAPI_(AutoPtr<IWebView>) GetWebView();

protected:
    /**
     * Add an error message to the client's console.
     * @param message The message to add
     * @param lineNumber the line on which the error occurred
     * @param sourceID the filename of the source that caused the error.
     * @param msgLevel the log level of this message. This is a value casted to int
     *     from WebCore::MessageLevel in WebCore/page/Console.h.
     */
    virtual CARAPI_(void) AddMessageToConsole(
        /* [in] */ const String& message, 
        /* [in] */ Int32 lineNumber, 
        /* [in] */ const String& sourceID,
        /* [in] */ Int32 msgLevel);

    /**
     * Invoke a javascript alert.
     * @param message The message displayed in the alert.
     */
    virtual CARAPI_(void) JsAlert(
        /* [in] */ const String& url, 
        /* [in] */ const String& message);

    /**
     * Notify the browser that the origin has exceeded it's database quota.
     * @param url The URL that caused the overflow.
     * @param databaseIdentifier The identifier of the database.
     * @param currentQuota The current quota for the origin.
     * @param estimatedSize The estimated size of the database.
     */
    virtual CARAPI_(void) ExceededDatabaseQuota(
        /* [in] */ const String& url, 
        /* [in] */ const String& databaseIdentifier,
        /* [in] */ Int64 currentQuota,
        /* [in] */ Int64 estimatedSize);

    /**
     * Notify the browser that the appcache has exceeded its max size.
     * @param spaceNeeded is the amount of disk space that would be needed
     * in order for the last appcache operation to succeed.
     */
    virtual CARAPI_(void) ReachedMaxAppCacheSize(
        /* [in] */ Int64 spaceNeeded);

    virtual CARAPI_(void) PopulateVisitedLinks();

    /**
     * Shows a prompt to ask the user to set the Geolocation permission state
     * for the given origin.
     * @param origin The origin for which Geolocation permissions are
     *     requested.
     */
    virtual CARAPI_(void) GeolocationPermissionsShowPrompt(
        /* [in] */ const String& origin);

    /**
     * Hides the Geolocation permissions prompt.
     */
    virtual CARAPI_(void) GeolocationPermissionsHidePrompt();

    /**
     * Invoke a javascript confirm dialog.
     * @param message The message displayed in the dialog.
     * @return True if the user confirmed or false if the user cancelled.
     */
    virtual CARAPI_(Boolean) JsConfirm(
        /* [in] */ const String& url, 
        /* [in] */ const String& message);

    /**
     * Invoke a javascript prompt dialog.
     * @param message The message to be displayed in the dialog.
     * @param defaultValue The default value in the prompt input.
     * @return The input from the user or null to indicate the user cancelled
     *         the dialog.
     */
    virtual CARAPI_(String) JsPrompt(
        /* [in] */ const String& url, 
        /* [in] */ const String& message, 
        /* [in] */ const String& defaultValue);

    /**
     * Invoke a javascript before unload dialog.
     * @param url The url that is requesting the dialog.
     * @param message The message displayed in the dialog.
     * @return True if the user confirmed or false if the user cancelled. False
     *         will cancel the navigation.
     */
    virtual CARAPI_(Boolean) JsUnload(
        /* [in] */ const String& url, 
        /* [in] */ const String& message);

    /**
     *
     * Callback to notify that a JavaScript execution timeout has occured.
     * @return True if the JavaScript execution should be interrupted. False
     *         will continue the execution.
     */
    virtual CARAPI_(Boolean) JsInterrupt();

private:

    //-------------------------------------------------------------------------
    // WebViewCore private methods
    //-------------------------------------------------------------------------

    CARAPI_(void) ClearCache(
        /* [in] */ Boolean includeDiskFiles);

    CARAPI_(void) LoadUrl(
        /* [in] */ const String& url, 
        /* [in] */ IObjectStringMap* extraHeaders);

    CARAPI_(void) Key(
        /* [in] */ IKeyEvent* evt, 
        /* [in] */ Boolean isDown);

    // These values are used to avoid requesting a layout based on old values
    Int32 mCurrentViewWidth;
    Int32 mCurrentViewHeight;
    Float mCurrentViewScale;

    // notify webkit that our virtual view size changed size (after inv-zoom)
    CARAPI_(void) ViewSizeChanged(
        /* [in] */ Int32 w, 
        /* [in] */ Int32 h, 
        /* [in] */ Int32 textwrapWidth, 
        /* [in] */ Float scale,
        /* [in] */ Int32 anchorX, 
        /* [in] */ Int32 anchorY, 
        /* [in] */ Boolean ignoreHeight) ;

    CARAPI_(void) SendUpdateTextEntry();

    // Utility method for exceededDatabaseQuota and reachedMaxAppCacheSize
    // callbacks. Computes the sum of database quota for all origins.
    Int64 GetUsedQuota();

    // Used to avoid posting more than one draw message.
    Boolean mDrawIsScheduled;

    // Used to avoid posting more than one split picture message.
    Boolean mSplitPictureIsScheduled;

    // Used to suspend drawing.
    Boolean mDrawIsPaused;

    // mRestoreState is set in didFirstLayout(), and reset in the next
    // webkitDraw after passing it to the UI thread.
    RestoreState* mRestoreState;

    
    CARAPI_(void) WebkitDraw();


    /**
     * Empty the picture set.
     */
    /*native*/
    CARAPI_(void) NativeClearContent();

    /**
     * Create a flat picture from the set of pictures.
     */
    /*native*/
    CARAPI_(void) NativeCopyContentToPicture(
        /* [in] */ IPicture* picture);

    /**
     * Draw the picture set with a background color. Returns true
     * if some individual picture took too long to draw and can be
     * split into parts. Called from the UI thread.
     */
    /*native*/
    CARAPI_(Boolean) NativeDrawContent(
        /* [in] */ ICanvas* canvas, 
        /* [in] */ Int32 color);

    /**
     * check to see if picture is blank and in progress
     */
    /*native*/
    CARAPI_(Boolean) NativePictureReady();

    /**
     * Redraw a portion of the picture set. The Point wh returns the
     * width and height of the overall picture.
     */
    /*native*/
    CARAPI_(Boolean) NativeRecordContent(
        /* [in] */ IRegion* invalRegion, 
        /* [in] */ IPoint* wh);

    /*native*/
    CARAPI_(Boolean) NativeFocusBoundsChanged();

    /**
     * Splits slow parts of the picture set. Called from the webkit
     * thread after nativeDrawContent returns true.
     */
    /*native*/ 
    CARAPI_(void) NativeSplitContent();

    /*native*/
    CARAPI_(Boolean) NativeKey(
        /* [in] */ Int32 keyCode, 
        /* [in] */ Int32 unichar,
        /* [in] */ Int32 repeatCount,
        /* [in] */ Boolean isShift, 
        /* [in] */ Boolean isAlt,
        /* [in] */ Boolean isSym,
        /* [in] */ Boolean isDown);

    /*native*/
    CARAPI_(void) NativeClick(
        /* [in] */ Int32 framePtr, 
        /* [in] */ Int32 nodePtr);

    /*native*/
    CARAPI_(void) NativeSendListBoxChoices(
        /* [in] */ ArrayOf<Boolean>* choices, 
        /* [in] */ Int32 size);

    /*native*/
    CARAPI_(void) NativeSendListBoxChoice(
        /* [in] */ Int32 choice);

    /*  Tell webkit what its width and height are, for the purposes
        of layout/line-breaking. These coordinates are in document space,
        which is the same as View coords unless we have zoomed the document
        (see nativeSetZoom).
        screenWidth is used by layout to wrap column around. If viewport uses
        fixed size, screenWidth can be different from width with zooming.
        should this be called nativeSetViewPortSize?
    */
    /*native*/
    CARAPI_(void) NativeSetSize(
        /* [in] */ Int32 width, 
        /* [in] */ Int32 height, 
        /* [in] */ Int32 screenWidth,
        /* [in] */ Float scale, 
        /* [in] */ Int32 realScreenWidth, 
        /* [in] */ Int32 screenHeight, 
        /* [in] */ Int32 anchorX,
        /* [in] */ Int32 anchorY, 
        /* [in] */ Boolean ignoreHeight);

    /*native*/
    CARAPI_(Int32) NativeGetContentMinPrefWidth();

    // Start: functions that deal with text editing
    /*native*/
    CARAPI_(void) NativeReplaceTextfieldText(
        /* [in] */ Int32 oldStart, 
        /* [in] */ Int32 oldEnd, 
        /* [in] */ const String& replace, 
        /* [in] */ Int32 newStart, 
        /* [in] */ Int32 newEnd,
        /* [in] */ Int32 textGeneration);

    /*native*/
    CARAPI_(void) PassToJs(
        /* [in] */ Int32 gen,
        /* [in] */ const String& currentText, 
        /* [in] */ Int32 keyCode, 
        /* [in] */ Int32 keyValue, 
        /* [in] */ Boolean down,
        /* [in] */ Boolean cap, 
        /* [in] */ Boolean fn, 
        /* [in] */ Boolean sym);

    /*native*/
    CARAPI_(void) NativeSetFocusControllerActive(
        /* [in] */ Boolean active);

    /*native*/ 
    CARAPI_(void) NativeSaveDocumentState(
        /* [in] */ Int32 frame);

    /*native*/
    CARAPI_(void) NativeMoveFocus(
        /* [in] */ Int32 framePtr, 
        /* [in] */ Int32 nodePointer);

    /*native*/
    CARAPI_(void) NativeMoveMouse(
        /* [in] */ Int32 framePtr, 
        /* [in] */ Int32 x, 
        /* [in] */ Int32 y);

    /*native*/
    CARAPI_(void) NativeMoveMouseIfLatest(
        /* [in] */ Int32 moveGeneration,
        /* [in] */ Int32 framePtr, 
        /* [in] */ Int32 x, 
        /* [in] */ Int32 y);

    /*native*/
    CARAPI_(String) NativeRetrieveHref(
        /* [in] */ Int32 framePtr, 
        /* [in] */ Int32 nodePtr);

    /*native*/
    CARAPI_(String) NativeRetrieveAnchorText(
        /* [in] */ Int32 framePtr, 
        /* [in] */ Int32 nodePtr);

    /*native*/
    CARAPI_(void) NativeTouchUp(
        /* [in] */ Int32 touchGeneration,
        /* [in] */ Int32 framePtr, 
        /* [in] */ Int32 nodePtr, 
        /* [in] */ Int32 x, 
        /* [in] */ Int32 y);

    /*native*/
    CARAPI_(Boolean) NativeHandleTouchEvent(
        /* [in] */ Int32 action, 
        /* [in] */ Int32 x, 
        /* [in] */ Int32 y,
        /* [in] */ Int32 metaState);

    /*native*/
    CARAPI_(void) NativeUpdateFrameCache();

    /*native*/
    CARAPI_(void) NativeSetBackgroundColor(
        /* [in] */ Int32 color);

    /*native*/
    CARAPI_(void) NativeDumpDomTree(
        /* [in] */ Boolean useFile);

    /*native*/
    CARAPI_(void) NativeDumpRenderTree(
        /* [in] */ Boolean useFile);

    /*native*/
    CARAPI_(void) NativeDumpNavTree();

    /*native*/
    CARAPI_(void) NativeDumpV8Counters();

    /*native*/
    CARAPI_(void) NativeSetJsFlags(
        /* [in] */ const String& flags);

    /**
     *  Delete text from start to end in the focused textfield. If there is no
     *  focus, or if start == end, silently fail.  If start and end are out of
     *  order, swap them.
     *  @param  start   Beginning of selection to delete.
     *  @param  end     End of selection to delete.
     *  @param  textGeneration Text generation number when delete was pressed.
     */
    /*native*/
    CARAPI_(void) NativeDeleteSelection(
        /* [in] */ Int32 start, 
        /* [in] */ Int32 end,
        /* [in] */ Int32 textGeneration);

    /**
     *  Set the selection to (start, end) in the focused textfield. If start and
     *  end are out of order, swap them.
     *  @param  start   Beginning of selection.
     *  @param  end     End of selection.
     */
    /*native*/
    CARAPI_(void) NativeSetSelection(
        /* [in] */ Int32 start, 
        /* [in] */ Int32 end);

    // Register a scheme to be treated as local scheme so that it can access
    // local asset files for resources
    /*native*/
    CARAPI_(void) NativeRegisterURLSchemeAsLocal(
        /* [in] */ const String& scheme);

    /*
     * Inform webcore that the user has decided whether to allow or deny new
     * quota for the current origin or more space for the app cache, and that
     * the main thread should wake up now.
     * @param limit Is the new quota for an origin or new app cache max size.
     */
    /*native*/
    CARAPI_(void) NativeSetNewStorageLimit(
        /* [in] */ Int64 limit);

    /**
     * Provide WebCore with a Geolocation permission state for the specified
     * origin.
     * @param origin The origin for which Geolocation permissions are provided.
     * @param allow Whether Geolocation permissions are allowed.
     * @param remember Whether this decision should be remembered beyond the
     *     life of the current page.
     */
    /*native*/
    CARAPI_(void) NativeGeolocationPermissionsProvide(
        /* [in] */ const String& origin, 
        /* [in] */ Boolean allow, 
        /* [in] */ Boolean remember);

    /**
     * Provide WebCore with the previously visted links from the history database
     */
    /* native */
    CARAPI_(void) NativeProvideVisitedHistory(
        /* [in] */ ArrayOf<String>* history);


    // EventHub for processing messages
    /*const*/ AutoPtr<EventHub> mEventHub;
    // WebCore thread handler
    static AutoPtr<IApartment> sWebCoreHandler;

    /**
     * Called by JNI.  Open a file chooser to upload a file.
     * @return String version of the URI plus the name of the file.
     * FIXME: Just return the URI here, and in FileSystem::pathGetFileName, call
     * into Java to get the filename.
     */
    CARAPI_(String) OpenFileChooser();

    //////////////////////////////////////////////////////////////////////////

    CARAPI_(void) _RestoreState(
        /* [in] */ Int32 index);

    // called by JNI
    CARAPI_(void) ContentScrollBy(
        /* [in] */ Int32 dx, 
        /* [in] */ Int32 dy, 
        /* [in] */ Boolean animate);

    // called by JNI
    CARAPI_(void) ContentScrollTo(
        /* [in] */ Int32 x, 
        /* [in] */ Int32 y);

    // called by JNI
    CARAPI_(void) ContentSpawnScrollTo(
        /* [in] */ Int32 x, 
        /* [in] */ Int32 y);

    // called by JNI
    CARAPI_(void) SendNotifyProgressFinished();

    /*  Called by JNI. The coordinates are in doc coordinates, so they need to
        be scaled before they can be used by the view system, which happens
        in WebView since it (and its thread) know the current scale factor.
     */
    CARAPI_(void) SendViewInvalidate(
        /* [in] */ Int32 left, 
        /* [in] */ Int32 top, 
        /* [in] */ Int32 right, 
        /* [in] */ Int32 bottom);    

    // called by JNI
    CARAPI_(void) SendImmediateRepaint();

    // called by JNI
    CARAPI_(void) SetRootLayer(
        /* [in] */ Int32 layer);



    /*native*/
    CARAPI_(void) SetViewportSettingsFromNative();

    // called by JNI
    CARAPI_(void) DidFirstLayout(
        /* [in] */ Boolean standardLoad);

    // called by JNI
    CARAPI_(void) UpdateViewport();

    CARAPI_(void) SetupViewport(
        /* [in] */ Boolean updateRestoreState);

    // called by JNI
    CARAPI_(void) RestoreScale(
        /* [in] */ Int32 scale);

    // called by JNI
    CARAPI_(void) RestoreScreenWidthScale(
        /* [in] */ Int32 scale);

    // called by JNI
    CARAPI_(void) NeedTouchEvents(
        /* [in] */ Boolean need);

    // called by JNI
    CARAPI_(void) UpdateTextfield(
        /* [in] */ Int32 ptr, 
        /* [in] */ Boolean changeToPassword,
        /* [in] */ CString text, 
        /* [in] */ Int32 textGeneration);

    // called by JNI
    CARAPI_(void) UpdateTextSelection(
        /* [in] */ Int32 pointer, 
        /* [in] */ Int32 start, 
        /* [in] */ Int32 end,
        /* [in] */ Int32 textGeneration);

    // called by JNI
    CARAPI_(void) ClearTextEntry();

    // called by JNI
    CARAPI_(void) SendFindAgain();

    /*native*/
    CARAPI_(void) NativeUpdateFrameCacheIfLoading();

    /*native*/
    CARAPI_(String&) NativeRequestLabel(
        /* [in] */ Int32 framePtr, 
        /* [in] */ Int32 nodePtr);

    /**
     * Scroll the focused textfield to (xPercent, y) in document space
     */
    /*native*/ 
    CARAPI_(void) NativeScrollFocusedTextInput(
        /* [in] */ Float xPercent, 
        /* [in] */ Int32 y);

    // these must be in document space (i.e. not scaled/zoomed).
    /*native*/ 
    CARAPI_(void) NativeSetScrollOffset(
        /* [in] */ Int32 gen, 
        /* [in] */ Int32 dx, 
        /* [in] */ Int32 dy);

    /*native*/ 
    CARAPI_(void) NativeSetGlobalBounds(
        /* [in] */ Int32 x, 
        /* [in] */ Int32 y, 
        /* [in] */ Int32 w, 
        /* [in] */ Int32 h);

    // called by JNI
    CARAPI_(void) RequestListBox(
        /* [in] */ ArrayOf<String>* array, 
        /* [in] */ ArrayOf<Int32>* enabledArray,
        /* [in] */ ArrayOf<Int32>* selectedArray);

    // called by JNI
    CARAPI_(void) RequestListBox(
        /* [in] */ ArrayOf<String>* array, 
        /* [in] */ ArrayOf<Int32>* enabledArray,
        /* [in] */ Int32 selection);

    // called by JNI
    CARAPI_(void) RequestKeyboardWithSelection(
        /* [in] */ Int32 pointer, 
        /* [in] */ Int32 selStart,
        /* [in] */ Int32 selEnd, 
        /* [in] */ Int32 textGeneration);

    // called by JNI
    CARAPI_(void) RequestKeyboard(
        /* [in] */ Boolean showKeyboard);

    // called by JNI
    CARAPI_(AutoPtr<IContext>) GetContext();

    // called by JNI
    CARAPI_(IInterface*) GetPluginClass(
        /* [in] */ const String& libName, 
        /* [in] */ const String& clsName);

    // called by JNI. PluginWidget function to launch a full-screen view using a
    // View object provided by the plugin class.
    CARAPI_(void) ShowFullScreenPlugin(
        /* [in] */ ViewManager::ChildView* childView, 
        /* [in] */ Int32 npp);

    // called by JNI
    CARAPI_(void) HideFullScreenPlugin();

    // called by JNI.  PluginWidget functions for creating an embedded View for
    // the surface drawing model.
    CARAPI_(ViewManager::ChildView*) AddSurface(
        /* [in] */ IView* pluginView, 
        /* [in] */ Int32 x, 
        /* [in] */ Int32 y,
        /* [in] */ Int32 width, 
        /* [in] */ Int32 height);

    CARAPI_(void) UpdateSurface(
        /* [in] */ ViewManager::ChildView* childView, 
        /* [in] */ Int32 x, 
        /* [in] */ Int32 y,
        /* [in] */ Int32 width, 
        /* [in] */ Int32 height);

    CARAPI_(void) DestroySurface(
        /* [in] */ ViewManager::ChildView* childView);



    CARAPI_(void) ShowRect(
        /* [in] */ Int32 left, 
        /* [in] */ Int32 top, 
        /* [in] */ Int32 width, 
        /* [in] */ Int32 height,
        /* [in] */ Int32 contentWidth, 
        /* [in] */ Int32 contentHeight, 
        /* [in] */ Float xPercentInDoc,
        /* [in] */ Float xPercentInView, 
        /* [in] */ Float yPercentInDoc, 
        /* [in] */ Float yPercentInView);

    // called by JNI
    CARAPI_(void) CenterFitRect(
        /* [in] */ Int32 x, 
        /* [in] */ Int32 y, 
        /* [in] */ Int32 width, 
        /* [in] */ Int32 height);

    // called by JNI
    CARAPI_(void) SetScrollbarModes(
        /* [in] */ Int32 hMode, 
        /* [in] */ Int32 vMode);

    /*native*/
    CARAPI_(void) NativePause();

    /*native*/
    CARAPI_(void) NativeResume();

    /*native*/
    CARAPI_(void) NativeFreeMemory();

    /*native*/
    CARAPI_(void) NativeFullScreenPluginHidden(
        /* [in] */ Int32 npp);

    /*native*/
    CARAPI_(Boolean) NativeValidNodeAndBounds(
        /* [in] */ Int32 frame, 
        /* [in] */ Int32 node,
        /* [in] */ IRect* bounds);

public:
    // The thread name used to identify the WebCore thread and for use in
    // debugging other classes that require operation within the WebCore thread.
    /* package */ 
    static const CString THREAD_NAME;// = "WebViewCoreThread";

private:

    static Boolean mRepaintScheduled;

    static const CString LOGTAG;// = "webcore";

    /*
     * WebViewCore always executes in the same thread as the native webkit.
     */

    // The WebView that corresponds to this WebViewCore.
    AutoPtr<IWebView> mWebView;
    // Proxy for handling callbacks from native code
    /*const*/ AutoPtr<ICallbackProxy> mCallbackProxy;
    // Settings object for maintaining all settings
    /*const*/ AutoPtr<WebSettings> mSettings;
    // Context for initializing the BrowserFrame with the proper assets.
    /*const*/ AutoPtr<IContext> mContext;
    // The pointer to a native view object.
    Int32 mNativeClass;
    // The BrowserFrame is an interface to the native Frame component.
    AutoPtr<IBrowserFrame> mBrowserFrame;
    // Custom JS interfaces to add during the initialization.
    AutoPtr<IObjectStringMap> mJavascriptInterfaces;
    /*
     * range is from 200 to 10,000. 0 is a special value means device-width. -1
     * means undefined.
     */
    Int32 mViewportWidth;

    /*
     * range is from 200 to 10,000. 0 is a special value means device-height. -1
     * means undefined.
     */
    Int32 mViewportHeight;

    /*
     * scale in percent, range is from 1 to 1000. 0 means undefined.
     */
    Int32 mViewportInitialScale;

    /*
     * scale in percent, range is from 1 to 1000. 0 means undefined.
     */
    Int32 mViewportMinimumScale;

    /*
     * scale in percent, range is from 1 to 1000. 0 means undefined.
     */
    Int32 mViewportMaximumScale;

    Boolean mViewportUserScalable;

    /*
     * range is from 70 to 400.
     * 0 is a special value means device-dpi. The default scale factor will be
     * always 100.
     * -1 means undefined. The default scale factor will be
     * WebView.DEFAULT_SCALE_PERCENT.
     */
    Int32 mViewportDensityDpi;

    Int32 mRestoredScale;
    Int32 mRestoredScreenWidthScale;
    Int32 mRestoredX;
    Int32 mRestoredY;

    Int32 mWebkitScrollX;
    Int32 mWebkitScrollY;

    Core::Threading::Mutex mEventHubLock;
    Core::Threading::Mutex mutexThis;
    static Core::Threading::Mutex mMutexClass;
};

#endif //__WEBVIEWCORE_H__