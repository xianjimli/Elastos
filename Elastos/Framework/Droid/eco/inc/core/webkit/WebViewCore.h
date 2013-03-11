#ifndef __WEBVIEWCORE_H__
#define __WEBVIEWCORE_H__

#include <elastos/ElRefBase.h>

#include "ViewManager.h"
#include "WebSettings.h"

class WebViewCore : public ElRefBase
{
public:
	struct BaseUrlData 
	{
        CString mBaseUrl;
        CString mData;
        CString mMimeType;
        CString mEncoding;
        CString mHistoryUrl;
    };

	struct CursorData 
	{
        CursorData() {}
        CursorData(
        	/* [in] */ Int32 frame, 
        	/* [in] */ Int32 node, 
        	/* [in] */ Int32 x, 
        	/* [in] */ Int32 y);

        Int32 mMoveGeneration;
        Int32 mFrame;
        Int32 mNode;
        Int32 mX;
        Int32 mY;
    };

	struct JSInterfaceData 
	{
        IInterface* mObject;
        CString mInterfaceName;
    };

	struct JSKeyData 
	{
        CString mCurrentText;
        IKeyEvent* mEvent;
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
        CString mUrl;
        IObjectStringMap* mExtraHeaders;
    };

	struct PostUrlData 
	{
        CString mUrl;
        byte mPostData[];
    };

	struct ReplaceTextData 
	{
        CString mReplace;
        Int32 mNewStart;
        Int32 mNewEnd;
        Int32 mTextGeneration;
    };

	struct TextSelectionData 
	{
        TextSelectionData(
        	/* [in] */ Int32 start, 
        	/* [in] */ Int32 end);

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
        CString mOrigin;
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

        Region mInvalRegion;
        Point mViewPoint;
        Point mWidthHeight;
        Int32 mMinPrefWidth;
        RestoreState mRestoreState; // only non-null if it is for the first
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

    class EventHub 
    {
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
		static const Int32 DESTROY =     200;

        // Private handler for WebCore messages.
		IHandler* mHandler;
        // Message queue for containing messages before the WebCore thread is
        // ready.
//		ArrayList<Message> mMessages;
        // Flag for blocking messages. This is used during DESTROY to avoid
        // posting more messages to the EventHub or to WebView's event handler.
		Boolean mBlockMessages;

		Int32 mTid;
		Int32 mSavedPriority;

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
			/* [in] */ IMessage* msg);

		/* synchronized */
        CARAPI_(void) RemoveMessages(
        	/* [in] */ Int32 what);

        /* synchronized */
		CARAPI_(Boolean) HasMessages(
			/* [in] */ Int32 what);

        /* synchronized */
		CARAPI_(void) SendMessageDelayed(
			/* [in] */ IMessage* msg, 
			/* [in] */ Int64 delay);

        /**
         * Send a message internally to the front of the queue.
         * 
         * synchronized
         */
		CARAPI_(void) SendMessageAtFrontOfQueue(
			/* [in] */ IMessage* msg);

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
		CARAPI_(void) blockMessages();
    };

public:
    /*static {
        // Load libwebcore during static initialization. This happens in the
        // zygote process so it will be shared read-only across all app
        // processes.
        System.loadLibrary("webcore");
    }*/

    // The thread name used to identify the WebCore thread and for use in
    // debugging other classes that require operation within the WebCore thread.
    /* package */ 
	static const char* THREAD_NAME;// = "WebViewCoreThread";

	WebViewCore(
		/* [in] */ IContext* context, 
		/* [in] */ IWebView* w, 
		/* [in] */ ICallbackProxy* proxy,
		/* [in] */ IObjectStringMap* javascriptInterfaces);

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
    virtual CARAPI_(IBrowserFrame*) GetBrowserFrame();

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
		/* [in] */ CString mimeType);



    //-------------------------------------------------------------------------
    // JNI methods
    //-------------------------------------------------------------------------

	/* native*/
    static CARAPI_(CString) NativeFindAddress(
    	/* [in] */ CString addr, 
    	/* [in] */ Boolean caseInsensitive);





    // mAction of TouchEventData can be MotionEvent.getAction() which uses the
    // last two bytes or one of the following values
    static const Int32 ACTION_LONGPRESS = 0x100;
    static const Int32 ACTION_DOUBLETAP = 0x200;

    static const String HandlerDebugString[];/* = {
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
    	/* [in] */ IMessage* msg);

    virtual CARAPI_(void) SendMessage(
    	/* [in] */ Int32 what);

    virtual CARAPI_(void) SendMessage(
    	/* [in] */ Int32 what, 
    	/* [in] */ IInterface* obj);

    virtual CARAPI_(void) SendMessage(
    	/* [in] */ Int32 what, 
    	/* [in] */ Int32 arg1);

    virtual CARAPI_(void) SendMessage(
    	/* [in] */ Int32 what, 
    	/* [in] */ Int32 arg1, 
    	/* [in] */ Int32 arg2);

    virtual CARAPI_(void) SendMessage(
    	/* [in] */ Int32 what, 
    	/* [in] */ Int32 arg1, 
    	/* [in] */ IInterface* obj);

    virtual CARAPI_(void) SendMessage(
    	/* [in] */ Int32 what, 
    	/* [in] */ Int32 arg1, 
    	/* [in] */ Int32 arg2, 
    	/* [in] */ IInterface* obj);

    virtual CARAPI_(void) SendMessageAtFrontOfQueue(
    	/* [in] */ Int32 what, 
    	/* [in] */ IInterface* obj);

    virtual CARAPI_(void) SendMessageDelayed(
    	/* [in] */ Int32 what, 
    	/* [in] */ IInterface* obj, 
    	/* [in] */ Int64 delay);

    virtual CARAPI_(void) RemoveMessages(
    	/* [in] */ Int32 what);

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

    const IDrawFilter* mZoomFilter;
    // If we need to trade better quality for speed, set mScrollFilter to null
    const IDrawFilter* mScrollFilter;

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
	virtual CARAPI_(IPicture*) CopyContentPicture();

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
	virtual CARAPI_(IWebView*) GetWebView();


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
		/* [in] */ CString message, 
		/* [in] */ Int32 lineNumber, 
		/* [in] */ CString sourceID,
		/* [in] */ Int32 msgLevel);

    /**
     * Invoke a javascript alert.
     * @param message The message displayed in the alert.
     */
	virtual CARAPI_(void) JsAlert(
		/* [in] */ CString url, 
		/* [in] */ CString message);

    /**
     * Notify the browser that the origin has exceeded it's database quota.
     * @param url The URL that caused the overflow.
     * @param databaseIdentifier The identifier of the database.
     * @param currentQuota The current quota for the origin.
     * @param estimatedSize The estimated size of the database.
     */
	virtual CARAPI_(void) ExceededDatabaseQuota(
		/* [in] */ CString url, 
		/* [in] */ CString databaseIdentifier,
		/* [in] */ Int64 currentQuota,
		/* [in] */ Int64 estimatedSize);

    /**
     * Notify the browser that the appcache has exceeded its max size.
     * @param spaceNeeded is the amount of disk space that would be needed
     * in order for the last appcache operation to succeed.
     */
	virtual CARAPI_(void) ReachedMaxAppCacheSize(
		/* [in] */ long spaceNeeded);

	virtual CARAPI_(void) PopulateVisitedLinks();

    /**
     * Shows a prompt to ask the user to set the Geolocation permission state
     * for the given origin.
     * @param origin The origin for which Geolocation permissions are
     *     requested.
     */
	virtual CARAPI_(void) GeolocationPermissionsShowPrompt(
		/* [in] */ CString origin);

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
		/* [in] */ CString url, 
		/* [in] */ CString message);

    /**
     * Invoke a javascript prompt dialog.
     * @param message The message to be displayed in the dialog.
     * @param defaultValue The default value in the prompt input.
     * @return The input from the user or null to indicate the user cancelled
     *         the dialog.
     */
	virtual CARAPI_(CString) JsPrompt(
		/* [in] */ CString url, 
		/* [in] */ String message, 
		/* [in] */ String defaultValue);

    /**
     * Invoke a javascript before unload dialog.
     * @param url The url that is requesting the dialog.
     * @param message The message displayed in the dialog.
     * @return True if the user confirmed or false if the user cancelled. False
     *         will cancel the navigation.
     */
	virtual CARAPI_(Boolean) JsUnload(
		/* [in] */ CString url, 
		/* [in] */ CString message);

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
		/* [in] */ CString url, 
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
	RestoreState mRestoreState;

    
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
		/* [in] */ Boolean choices[], 
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
		/* [in] */ CString replace, 
		/* [in] */ Int32 newStart, 
		/* [in] */ Int32 newEnd,
		/* [in] */ Int32 textGeneration);

    /*native*/
	CARAPI_(void) PassToJs(
		/* [in] */ Int32 gen,
		/* [in] */ CString currentText, 
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
	CARAPI_(void) nativeMoveFocus(
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
	CARAPI_(CString) NativeRetrieveHref(
		/* [in] */ Int32 framePtr, 
		/* [in] */ Int32 nodePtr);

	/*native*/
	CARAPI_(CString) NativeRetrieveAnchorText(
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
		/* [in] */ CString flags);

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
		/* [in] */ CString scheme);

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
		/* [in] */ CString origin, 
		/* [in] */ Boolean allow, 
		/* [in] */ Boolean remember);

    /**
     * Provide WebCore with the previously visted links from the history database
     */
    /* native */
	CARAPI_(void) NativeProvideVisitedHistory(
		/* [in] */ CString history[]);


    // EventHub for processing messages
	const EventHub mEventHub;
    // WebCore thread handler
	static IHandler* sWebCoreHandler;
    // Class for providing Handler creation inside the WebCore thread.
    class WebCoreThread// : public Runnable 
    {
	public:
		virtual CARAPI_(void) Run();

	private:
        // Message id for initializing a new WebViewCore.
		static const Int32 INITIALIZE = 0;
		static const Int32 REDUCE_PRIORITY = 1;
		static const Int32 RESUME_PRIORITY = 2;
    };

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
	CARAPI_(CString) NativeRequestLabel(
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
		/* [in] */ CString array[], 
		/* [in] */ int enabledArray[],
		/* [in] */ int selectedArray[]);

    // called by JNI
	CARAPI_(void) RequestListBox(
		/* [in] */ CString array[], 
		/* [in] */ int enabledArray[],
		/* [in] */ int selection);

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
	CARAPI_(IContext*) GetContext();

    // called by JNI
	CARAPI_(IInterface*) GetPluginClass(
		/* [in] */ CString libName, 
		/* [in] */ CString clsName);

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
		/* [in] */ ViewManager::ChildView childView);



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

private:

	static Boolean mRepaintScheduled;

	static const CString LOGTAG;// = "webcore";

    /*
     * WebViewCore always executes in the same thread as the native webkit.
     */

    // The WebView that corresponds to this WebViewCore.
	IWebView* mWebView;
    // Proxy for handling callbacks from native code
	const ICallbackProxy* mCallbackProxy;
    // Settings object for maintaining all settings
	const WebSettings mSettings;
    // Context for initializing the BrowserFrame with the proper assets.
	const IContext* mContext;
    // The pointer to a native view object.
	Int32 mNativeClass;
    // The BrowserFrame is an interface to the native Frame component.
	IBrowserFrame* mBrowserFrame;
    // Custom JS interfaces to add during the initialization.
	IObjectStringMap* mJavascriptInterfaces;
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
};

#endif //__WEBVIEWCORE_H__