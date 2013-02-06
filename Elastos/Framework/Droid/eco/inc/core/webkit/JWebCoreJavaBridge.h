#ifndef __JWEBCOREJAVABRIDGE_H_
#define __JWEBCOREJAVABRIDGE_H_

class JWebCoreJavaBridge : public Handler {

public:

    /**
     * Construct a new JWebCoreJavaBridge to interface with
     * WebCore timers and cookies.
     */
	JWebCoreJavaBridge();

public:
    /* package */
    static const Int32 REFRESH_PLUGINS = 100;

	/* synchronized */
    static CARAPI_(void) SetActiveWebView(
    	/* [in] */ WebView webview);

    /* synchronized */
    static  CARAPI_(void) RemoveActiveWebView(
    	/* [in] */ WebView webview);

    /**
     * handleMessage
     * @param msg The dispatched message.
     *
     * The only accepted message currently is TIMER_MESSAGE
     */
    //@Override
	virtual CARAPI_(void) HandleMessage(
		/* [in] */ Message msg);

    /**
     * Pause all timers.
     */
	virtual CARAPI_(void) Pause();

    /**
     * Resume all timers.
     */
	virtual CARAPI_(void) Resume();

    /**
     * Set WebCore cache size.
     * @param bytes The cache size in bytes.
     *
     * native
     */
	virtual CARAPI_(void) SetCacheSize(
		/* [in] */ Int32 bytes);

	/*native*/
	virtual CARAPI_(void) SetNetworkOnLine(
		/* [in] */ Boolean online);

	/* native */	
	virtual CARAPI_(void) SetNetworkType(
		/* [in] */ CString type, 
		/* [in] */ CString subtype);

	/* native */
	virtual CARAPI_(void) AddPackageNames(
		/* [in] */ Set<String> packageNames);

	/* native */
	virtual CARAPI_(void) AddPackageName(
		/* [in] */ CString packageName);

	virtual CARAPI_(void) RemovePackageName(
		/* [in] */ CString packageName);

protected:
	//@Override
	virtual CARAPI_(void) Finalize();

private:
    /**
     * Call native timer callbacks.
     */
	CARAPI_(void) FireSharedTimer();

        // called from JNI side
	CARAPI_(void) SignalServiceFuncPtrQueue();
    
	CARAPI_(void) NativeServiceFuncPtrQueue();

    /**
     * Store a cookie string associated with a url.
     * @param url The url to be used as a key for the cookie.
     * @param value The cookie string to be stored.
     */
	CARAPI_(void) SetCookies(
		/* [in] */ CString url, 
		/* [in] */ String value);

    /**
     * Retrieve the cookie string for the given url.
     * @param url The resource's url.
     * @return A String representing the cookies for the given resource url.
     */
	CARAPI_(CString) Cookies(
		/* [in] */ CString url);

    /**
     * Returns whether cookies are enabled or not.
     */
	CARAPI_(Boolean) CookiesEnabled();

    /**
     * Returns an array of plugin directoies
     */
	CARAPI_(String[]) GetPluginDirectories();

    /**
     * Returns the path of the plugin data directory
     */
	CARAPI_(CString) GetPluginSharedDataDirectory();

    /**
     * setSharedTimer
     * @param timemillis The relative time when the timer should fire
     */
	CARAPI_(void) SetSharedTimer(
		/* [in] */ Int64 timemillis);

    /**
     * Stop the shared timer.
     */
	CARAPI_(void) StopSharedTimer();

	CARAPI_(String[]) GetKeyStrengthList();

    /*synchronized*/ 
    CARAPI_(CString) GetSignedPublicKey(
    	/* [in] */ Int32 index, 
    	/* [in] */ CString challenge,
    	/* [in] */ String url);

	/*native*/
    CARAPI_(void) NativeConstructor();

    /*native*/
    CARAPI_(void) NativeFinalize();

	/*native*/
	CARAPI_(void) SharedTimerFired();

    /*native*/
    CARAPI_(void) NativeUpdatePluginDirectories(
    	/* [in] */ ArrayOf<String> directories, 
    	/* [in] */ Boolean reload);

private:
	// Identifier for the timer message.
	static const Int32 TIMER_MESSAGE = 1;
    // ID for servicing functionptr queue
	static const Int32 FUNCPTR_MESSAGE = 2;
    // Log system identifier.
	static const CString LOGTAG = "webkit-timers";

    // Native object pointer for interacting in native code.
	Int32 mNativeBridge;
    // Instant timer is used to implement a timer that needs to fire almost
    // immediately.
	Boolean mHasInstantTimer;

    // Reference count the pause/resume of timers
	Int32 mPauseTimerRefCount;

	Boolean mTimerPaused;
	Boolean mHasDeferredTimers;

    // keep track of the main WebView attached to the current window so that we
    // can get the proper Context.
	static WeakReference<WebView> sCurrentMainWebView;
};

#endif //__JWEBCOREJAVABRIDGE_H_