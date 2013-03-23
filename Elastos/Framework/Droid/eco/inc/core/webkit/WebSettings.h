#ifndef __WEBSETTING_H__
#define __WEBSETTING_H__

#include "ext/frameworkext.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>

using namespace Core;
using namespace Threading;

/**
 * Manages settings state for a WebView. When a WebView is first created, it
 * obtains a set of default settings. These default settings will be returned
 * from any getter call. A WebSettings object obtained from
 * WebView.getSettings() is tied to the life of the WebView. If a WebView has
 * been destroyed, any method call on WebSettings will throw an
 * IllegalStateException.
 */
class WebSettings : public ElRefBase
{    
public:
    /**
     * Enum for controlling the layout of html.
     * NORMAL means no rendering changes.
     * SINGLE_COLUMN moves all content into one column that is the width of the
     * view.
     * NARROW_COLUMNS makes all columns no wider than the screen if possible.
     */
    // XXX: These must match LayoutAlgorithm in Settings.h in WebCore.
    enum LayoutAlgorithm 
    {
        NORMAL,
        SINGLE_COLUMN,
        NARROW_COLUMNS
    };

    /**
     * Enum for specifying the text size.
     * SMALLEST is 50%
     * SMALLER is 75%
     * NORMAL is 100%
     * LARGER is 150%
     * LARGEST is 200%
     */
    struct TextSize
    {
        enum 
        {
            SMALLEST = 50,
            SMALLER  = 75,
            NORMAL   = 100,
            LARGER   = 150,
            LARGEST  = 200
        };
        TextSize(Int32 size) {
            value = size;
        }
        Int32 value;
    };

    /**
     * Enum for specifying the WebView's desired density.
     * FAR makes 100% looking like in 240dpi
     * MEDIUM makes 100% looking like in 160dpi
     * CLOSE makes 100% looking like in 120dpi
     */
    struct ZoomDensity
    {
        enum 
        {
            FAR    = 150,  // 240dpi
            MEDIUM = 100,  // 160dpi
            CLOSE  = 75    // 120dpi
        };
        ZoomDensity(Int32 size) {
            value = size;
        }
        Int32 value;
    };

    enum RenderPriority 
    {
        RP_NORMAL,
        RP_HIGH,
        RP_LOW
    };

    /**
     * The plugin state effects how plugins are treated on a page. ON means
     * that any object will be loaded even if a plugin does not exist to handle
     * the content. ON_DEMAND means that if there is a plugin installed that
     * can handle the content, a placeholder is shown until the user clicks on
     * the placeholder. Once clicked, the plugin will be enabled on the page.
     * OFF means that all plugins will be turned off and any fallback content
     * will be used.
     */
    enum PluginState 
    {
        ON,
        ON_DEMAND,
        OFF
    };

private:
    // Class to handle messages before WebCore is ready.
    class WsEventHandler {
    private:
        class WsEhHandler: public ElRefBase,public IHandler
        {
        public:
            CARAPI_(PInterface) Probe(
                /* [in] */ REIID riid);

            CARAPI_(UInt32) AddRef();

            CARAPI_(UInt32) Release();

            CARAPI GetInterfaceID(
                /* [in] */ IInterface* Object,
                /* [out] */ InterfaceID* iID);

            WsEhHandler(
                /* [in] */ WsEventHandler* eventHandler,
                /* [in] */ WebSettings* webSettings);
        public:
            //@Override
            CARAPI_(void) HandleMessage(
                /* [in] */ IMessage* msg);
        private:
            WsEventHandler* mEventHandler;
            WebSettings* mWebSettings;
        };
    public:
        WsEventHandler(
            /* [in] */ WebSettings* webSettings);
    private:
        CARAPI_(void) CreateHandler();

        CARAPI_(void) SetRenderPriority();

        /**
         * Send a message to the private queue or handler.
         */
        CARAPI_(Boolean) SendMessage(
            /* [in] */ IMessage* msg);

    public:
        // Message id for syncing
        static const Int32 SYNC = 0;
        // Message id for setting priority
        static const Int32 PRIORITY = 1;
        // Message id for writing double-tap toast count
        static const Int32 SET_DOUBLE_TAP_TOAST_COUNT = 2;
    private:
        // Actual WebCore thread handler
        AutoPtr<IHandler> mHandler;
    private:
        WebSettings* mWebSettings;
        Core::Threading::Mutex mMutexWsEhThis;
    };

public:
    friend class WsEhHandler;

    WebSettings();

    /**
     * Package constructor to prevent clients from creating a new settings
     * instance.
     */
    WebSettings(
    	/* [in] */ IContext* context, 
    	/* [in] */ IWebView* webview);

public:
    /**
     * Enables dumping the pages navigation cache to a text file.
     */
	virtual CARAPI_(void) SetNavDump(
		/* [in] */ Boolean enabled);

    /**
     * Returns true if dumping the navigation cache is enabled.
     */
	virtual CARAPI_(Boolean) GetNavDump();

    /**
     * Set whether the WebView supports zoom
     */
	virtual CARAPI_(void) SetSupportZoom(
		/* [in] */ Boolean support);

    /**
     * Returns whether the WebView supports zoom
     */
	virtual CARAPI_(Boolean) SupportZoom();

    /**
     * Sets whether the zoom mechanism built into WebView is used.
     */
	virtual CARAPI_(void) SetBuiltInZoomControls(
		/* [in] */ Boolean enabled);
    
    /**
     * Returns true if the zoom mechanism built into WebView is being used.
     */
	virtual CARAPI_(Boolean) GetBuiltInZoomControls();
    
    /**
     * Enable or disable file access within WebView. File access is enabled by
     * default. Note that this enables or disables file system access only.
     * Assets and resources are still accessible using file:///android_asset and
     * file:///android_res.
     */
	virtual CARAPI_(void) SetAllowFileAccess(
		/* [in] */ Boolean allow);

    /**
     * Returns true if this WebView supports file access.
     */
	virtual CARAPI_(Boolean) GetAllowFileAccess();

    /**
     * Enable or disable content url access within WebView.  Content url access
     * allows WebView to load content from a content provider installed in the
     * system.  The default is enabled.
     * @hide
     */
	virtual CARAPI_(void) SetAllowContentAccess(
		/* [in] */ Boolean allow);

    /**
     * Returns true if this WebView supports content url access.
     * @hide
     */
	virtual CARAPI_(Boolean) GetAllowContentAccess();

    /**
     * Set whether the WebView loads a page with overview mode.
     */
	virtual CARAPI_(void) SetLoadWithOverviewMode(
		/* [in] */ Boolean overview);

    /**
     * Returns true if this WebView loads page with overview mode
     */
	virtual CARAPI_(Boolean) GetLoadWithOverviewMode();

    /**
     * Set whether the WebView uses its background for over scroll background.
     * If true, it will use the WebView's background. If false, it will use an
     * internal pattern. Default is true.
     */
	virtual CARAPI_(void) SetUseWebViewBackgroundForOverscrollBackground(
		/* [in] */ Boolean view);

    /**
     * Returns true if this WebView uses WebView's background instead of
     * internal pattern for over scroll background.
     */
	virtual CARAPI_(Boolean) GetUseWebViewBackgroundForOverscrollBackground();

    /**
     * Store whether the WebView is saving form data.
     */
	virtual CARAPI_(void) SetSaveFormData(
		/* [in] */ Boolean save);

    /**
     *  Return whether the WebView is saving form data.
     */
	virtual CARAPI_(Boolean) GetSaveFormData();

    /**
     *  Store whether the WebView is saving password.
     */
	virtual CARAPI_(void) SetSavePassword(
		/* [in] */ Boolean save);

    /**
     *  Return whether the WebView is saving password.
     */
	virtual CARAPI_(Boolean) GetSavePassword();

    /**
     * Set the text size of the page.
     * @param t A TextSize value for increasing or decreasing the text.
     * @see WebSettings.TextSize
     */
	virtual CARAPI_(void) SetTextSize(
		/* [in] */ TextSize* t);

    /**
     * Get the text size of the page.
     * @return A TextSize enum value describing the text size.
     * @see WebSettings.TextSize
     */
	virtual CARAPI_(TextSize*) GetTextSize();

    /**
     * Set the default zoom density of the page. This should be called from UI
     * thread.
     * @param zoom A ZoomDensity value
     * @see WebSettings.ZoomDensity
     */
	virtual CARAPI_(void) SetDefaultZoom(
		/* [in] */ ZoomDensity* zoom);

    /**
     * Get the default zoom density of the page. This should be called from UI
     * thread.
     * @return A ZoomDensity value
     * @see WebSettings.ZoomDensity
     */
	virtual CARAPI_(ZoomDensity*) GetDefaultZoom();

    /**
     * Enables using light touches to make a selection and activate mouseovers.
     */
	virtual CARAPI_(void) SetLightTouchEnabled(
		/* [in] */ Boolean enabled);

    /**
     * Returns true if light touches are enabled.
     */
	virtual CARAPI_(Boolean) GetLightTouchEnabled();

    /**
     * @deprecated This setting controlled a rendering optimization
     * that is no longer present. Setting it now has no effect.
     */
	virtual CARAPI_(void) SetUseDoubleTree(
		/* [in] */ Boolean use);

    /**
     * @deprecated This setting controlled a rendering optimization
     * that is no longer present. Setting it now has no effect.
     */
	virtual CARAPI_(Boolean) GetUseDoubleTree();

    /**
     * Tell the WebView about user-agent string.
     * @param ua 0 if the WebView should use an Android user-agent string,
     *           1 if the WebView should use a desktop user-agent string.
     *
     * @deprecated Please use setUserAgentString instead.
     */
	virtual CARAPI_(void) SetUserAgent(
		/* [in] */ Int32 ua);

    /**
     * Return user-agent as int
     * @return int  0 if the WebView is using an Android user-agent string.
     *              1 if the WebView is using a desktop user-agent string.
     *             -1 if the WebView is using user defined user-agent string.
     *
     * @deprecated Please use getUserAgentString instead.
     */
	virtual CARAPI_(Int32) GetUserAgent();

    /**
     * Tell the WebView to use the wide viewport
     */
	virtual CARAPI_(void) SetUseWideViewPort(
		/* [in] */ Boolean use);

    /**
     * @return True if the WebView is using a wide viewport
     */
	virtual CARAPI_(Boolean) GetUseWideViewPort();

    /**
     * Tell the WebView whether it supports multiple windows. TRUE means
     *         that {@link WebChromeClient#onCreateWindow(WebView, boolean,
     *         boolean, Message)} is implemented by the host application.
     */
	virtual CARAPI_(void) SetSupportMultipleWindows(
		/* [in] */ Boolean support);

    /**
     * @return True if the WebView is supporting multiple windows. This means
     *         that {@link WebChromeClient#onCreateWindow(WebView, boolean,
     *         boolean, Message)} is implemented by the host application.
     */
	virtual CARAPI_(Boolean) SupportMultipleWindows();

    /**
     * Set the underlying layout algorithm. This will cause a relayout of the
     * WebView.
     * @param l A LayoutAlgorithm enum specifying the algorithm to use.
     * @see WebSettings.LayoutAlgorithm
     */
	virtual CARAPI_(void) SetLayoutAlgorithm(
		/* [in] */ LayoutAlgorithm l);

    /**
     * Return the current layout algorithm. The default is NARROW_COLUMNS.
     * @return LayoutAlgorithm enum value describing the layout algorithm
     *         being used.
     * @see WebSettings.LayoutAlgorithm
     */
	virtual CARAPI_(LayoutAlgorithm) GetLayoutAlgorithm();

    /**
     * Set the standard font family name.
     * @param font A font family name.
     */
	virtual CARAPI_(void) SetStandardFontFamily(
		/* [in] */ const String& font);

    /**
     * Get the standard font family name. The default is "sans-serif".
     * @return The standard font family name as a string.
     */
	virtual CARAPI_(CString) GetStandardFontFamily();

    /**
     * Set the fixed font family name.
     * @param font A font family name.
     */
	virtual CARAPI_(void) SetFixedFontFamily(
		/* [in] */ const String& font);

    /**
     * Get the fixed font family name. The default is "monospace".
     * @return The fixed font family name as a string.
     */
	virtual CARAPI_(CString) GetFixedFontFamily();

    /**
     * Set the sans-serif font family name.
     * @param font A font family name.
     */
	virtual CARAPI_(void) SetSansSerifFontFamily(
		/* [in] */ const String& font);

    /**
     * Get the sans-serif font family name.
     * @return The sans-serif font family name as a string.
     */
	virtual CARAPI_(CString) GetSansSerifFontFamily();

    /**
     * Set the serif font family name. The default is "sans-serif".
     * @param font A font family name.
     */
	virtual CARAPI_(void) SetSerifFontFamily(
		/* [in] */ const String& font);

    /**
     * Get the serif font family name. The default is "serif".
     * @return The serif font family name as a string.
     */
	virtual CARAPI_(String) GetSerifFontFamily();

    /**
     * Set the cursive font family name.
     * @param font A font family name.
     */
	virtual CARAPI_(void) SetCursiveFontFamily(
		/* [in] */ const String& font);

    /**
     * Get the cursive font family name. The default is "cursive".
     * @return The cursive font family name as a string.
     */
	virtual CARAPI_(String) GetCursiveFontFamily();

    /**
     * Set the fantasy font family name.
     * @param font A font family name.
     */
	virtual CARAPI_(void) SetFantasyFontFamily(
		/* [in] */ const String& font);

    /**
     * Get the fantasy font family name. The default is "fantasy".
     * @return The fantasy font family name as a string.
     */
	virtual CARAPI_(String) GetFantasyFontFamily();

    /**
     * Set the minimum font size.
     * @param size A non-negative integer between 1 and 72.
     * Any number outside the specified range will be pinned.
     */
	virtual CARAPI_(void) SetMinimumFontSize(
		/* [in] */ Int32 size);

    /**
     * Get the minimum font size. The default is 8.
     * @return A non-negative integer between 1 and 72.
     */
	virtual CARAPI_(Int32) GetMinimumFontSize();

    /**
     * Set the minimum logical font size.
     * @param size A non-negative integer between 1 and 72.
     * Any number outside the specified range will be pinned.
     */
	virtual CARAPI_(void) SetMinimumLogicalFontSize(
		/* [in] */ Int32 size);

    /**
     * Get the minimum logical font size. The default is 8.
     * @return A non-negative integer between 1 and 72.
     */
	virtual CARAPI_(Int32) GetMinimumLogicalFontSize();

    /**
     * Set the default font size.
     * @param size A non-negative integer between 1 and 72.
     * Any number outside the specified range will be pinned.
     */
	virtual CARAPI_(void) SetDefaultFontSize(
		/* [in] */ Int32 size);

    /**
     * Get the default font size. The default is 16.
     * @return A non-negative integer between 1 and 72.
     */
	virtual CARAPI_(Int32) GetDefaultFontSize();

    /**
     * Set the default fixed font size.
     * @param size A non-negative integer between 1 and 72.
     * Any number outside the specified range will be pinned.
     */
	virtual CARAPI_(void) SetDefaultFixedFontSize(
		/* [in] */ Int32 size);

    /**
     * Get the default fixed font size. The default is 16.
     * @return A non-negative integer between 1 and 72.
     */
	virtual CARAPI_(Int32) GetDefaultFixedFontSize();

    /**
     * Set the number of pages cached by the WebKit for the history navigation.
     * @param size A non-negative integer between 0 (no cache) and 20 (max).
     * @hide
     */
	virtual CARAPI_(void) SetPageCacheCapacity(
		/* [in] */ Int32 size);

    /**
     * Tell the WebView to load image resources automatically.
     * @param flag True if the WebView should load images automatically.
     */
	virtual CARAPI_(void) SetLoadsImagesAutomatically(
		/* [in] */ Boolean flag);

    /**
     * Return true if the WebView will load image resources automatically.
     * The default is true.
     * @return True if the WebView loads images automatically.
     */
	virtual CARAPI_(Boolean) GetLoadsImagesAutomatically();

    /**
     * Tell the WebView to block network images. This is only checked when
     * {@link #getLoadsImagesAutomatically} is true. If you set the value to
     * false, images will automatically be loaded. Use this api to reduce
     * bandwidth only. Use {@link #setBlockNetworkLoads} if possible.
     * @param flag True if the WebView should block network images.
     * @see #setBlockNetworkLoads
     */
	virtual CARAPI_(void) SetBlockNetworkImage(
		/* [in] */ Boolean flag);

    /**
     * Return true if the WebView will block network images. The default is
     * false.
     * @return True if the WebView blocks network images.
     */
	virtual CARAPI_(Boolean) GetBlockNetworkImage();

    /**
     * Tell the WebView to block all network load requests. If you set the
     * value to false, you must call {@link android.webkit.WebView#reload} to
     * fetch remote resources. This flag supercedes the value passed to
     * {@link #setBlockNetworkImage}.
     * @param flag True if the WebView should block all network loads.
     * @see android.webkit.WebView#reload
     */
	virtual CARAPI_(void) SetBlockNetworkLoads(
		/* [in] */ Boolean flag);

    /**
     * Return true if the WebView will block all network loads. The default is
     * false.
     * @return True if the WebView blocks all network loads.
     */
	virtual CARAPI_(Boolean) GetBlockNetworkLoads();

        /**
     * Tell the WebView to enable javascript execution.
     * @param flag True if the WebView should execute javascript.
     */
	virtual CARAPI_(void) SetJavaScriptEnabled(
		/* [in] */ Boolean flag);

    /**
     * Tell the WebView to enable plugins.
     * @param flag True if the WebView should load plugins.
     * @deprecated This method has been deprecated in favor of
     *             {@link #setPluginState}
     */
	virtual CARAPI_(void) SetPluginsEnabled(
		/* [in] */ Boolean flag);

    /**
     * Tell the WebView to enable, disable, or have plugins on demand. On
     * demand mode means that if a plugin exists that can handle the embedded
     * content, a placeholder icon will be shown instead of the plugin. When
     * the placeholder is clicked, the plugin will be enabled.
     * @param state One of the PluginState values.
     */
	virtual CARAPI_(void) SetPluginState(
		/* [in] */ PluginState state);

    /**
     * Set a custom path to plugins used by the WebView. This method is
     * obsolete since each plugin is now loaded from its own package.
     * @param pluginsPath String path to the directory containing plugins.
     * @deprecated This method is no longer used as plugins are loaded from
     * their own APK via the system's package manager.
     */
	virtual CARAPI_(void) SetPluginsPath(
		/* [in] */ const String& pluginsPath);

    /**
     * Set the path to where database storage API databases should be saved.
     * Nota that the WebCore Database Tracker only allows the path to be set once.
     * This will update WebCore when the Sync runs in the C++ side.
     * @param databasePath String path to the directory where databases should
     *     be saved. May be the empty string but should never be null.
     */
	virtual CARAPI_(void) SetDatabasePath(
		/* [in] */ const String& databasePath);

    /**
     * Set the path where the Geolocation permissions database should be saved.
     * This will update WebCore when the Sync runs in the C++ side.
     * @param databasePath String path to the directory where the Geolocation
     *     permissions database should be saved. May be the empty string but
     *     should never be null.
     */
	virtual CARAPI_(void) SetGeolocationDatabasePath(
		/* [in] */ const String& databasePath);

    /**
     * Tell the WebView to enable Application Caches API.
     * @param flag True if the WebView should enable Application Caches.
     */
	virtual CARAPI_(void) SetAppCacheEnabled(
		/* [in] */ Boolean flag);

    /**
     * Set a custom path to the Application Caches files. The client
     * must ensure it exists before this call.
     * @param appCachePath String path to the directory containing Application
     * Caches files. The appCache path can be the empty string but should not
     * be null. Passing null for this parameter will result in a no-op.
     */
	virtual CARAPI_(void) SetAppCachePath(
		/* [in] */ const String& appCachePath);

    /**
     * Set the maximum size for the Application Caches content.
     * @param appCacheMaxSize the maximum size in bytes.
     */
	virtual CARAPI_(void) SetAppCacheMaxSize(
		/* [in] */ Int64 appCacheMaxSize);

    /**
     * Set whether the database storage API is enabled.
     * @param flag boolean True if the WebView should use the database storage
     *     API.
     */
	virtual CARAPI_(void) SetDatabaseEnabled(
		/* [in] */ Boolean flag);

    /**
     * Set whether the DOM storage API is enabled.
     * @param flag boolean True if the WebView should use the DOM storage
     *     API.
     */
	virtual CARAPI_(void) SetDomStorageEnabled(
		/* [in] */ Boolean flag);

    /**
     * Returns true if the DOM Storage API's are enabled.
     * @return True if the DOM Storage API's are enabled.
     */
	virtual CARAPI_(Boolean) GetDomStorageEnabled();

    /**
     * Return the path to where database storage API databases are saved for
     * the current WebView.
     * @return the String path to the database storage API databases.
     */
	virtual CARAPI_(String) GetDatabasePath();

    /**
     * Returns true if database storage API is enabled.
     * @return True if the database storage API is enabled.
     */
	virtual CARAPI_(Boolean) GetDatabaseEnabled();

    /**
     * Tell the WebView to enable WebWorkers API.
     * @param flag True if the WebView should enable WebWorkers.
     * Note that this flag only affects V8. JSC does not have
     * an equivalent setting.
     * @hide pending api council approval
     */
	virtual CARAPI_(void) SetWorkersEnabled(
		/* [in] */ Boolean flag);

    /**
     * Sets whether Geolocation is enabled.
     * @param flag Whether Geolocation should be enabled.
     */
	virtual CARAPI_(void) SetGeolocationEnabled(
		/* [in] */ Boolean flag);

    /**
     * Return true if javascript is enabled. <b>Note: The default is false.</b>
     * @return True if javascript is enabled.
     */
	virtual CARAPI_(Boolean) GetJavaScriptEnabled();

    /**
     * Return true if plugins are enabled.
     * @return True if plugins are enabled.
     * @deprecated This method has been replaced by {@link #getPluginState}
     */
	virtual CARAPI_(Boolean) GetPluginsEnabled();

    /**
     * Return the current plugin state.
     * @return A value corresponding to the enum PluginState.
     */
	virtual CARAPI_(PluginState) GetPluginState();

    /**
     * Returns the directory that contains the plugin libraries. This method is
     * obsolete since each plugin is now loaded from its own package.
     * @return An empty string.
     * @deprecated This method is no longer used as plugins are loaded from
     * their own APK via the system's package manager.
     */
	virtual CARAPI_(String) GetPluginsPath();

    /**
     * Tell javascript to open windows automatically. This applies to the
     * javascript function window.open().
     * @param flag True if javascript can open windows automatically.
     */
	virtual CARAPI_(void) SetJavaScriptCanOpenWindowsAutomatically(
		/* [in] */ Boolean flag);

    /**
     * Return true if javascript can open windows automatically. The default
     * is false.
     * @return True if javascript can open windows automatically during
     *         window.open().
     */
	virtual CARAPI_(Boolean) GetJavaScriptCanOpenWindowsAutomatically();

    /**
     * Set the default text encoding name to use when decoding html pages.
     * @param encoding The text encoding name.
     */
	virtual CARAPI_(void) SetDefaultTextEncodingName(
		/* [in] */ const String& encoding);

    /**
     * Get the default text encoding name. The default is "Latin-1".
     * @return The default text encoding name as a string.
     */
	virtual CARAPI_(String) GetDefaultTextEncodingName();

    /**
     * Set the WebView's user-agent string. If the string "ua" is null or empty,
     * it will use the system default user-agent string.
     */
	virtual CARAPI_(void) SetUserAgentString(
		/* [in] */ String ua);

    /**
     * Return the WebView's user-agent string.
     */
	virtual CARAPI_(String*) GetUserAgentString();

    /* package api to grab the Accept Language string. */
    /*package*/ 
	virtual CARAPI_(String*) GetAcceptLanguage();
    
    /**
     * Tell the WebView whether it needs to set a node to have focus when
     * {@link WebView#requestFocus(int, android.graphics.Rect)} is called.
     * 
     * @param flag
     */
	virtual CARAPI_(void) SetNeedInitialFocus(
		/* [in] */ Boolean flag);

    /* Package api to get the choice whether it needs to set initial focus. */
    /* package */ 
	virtual CARAPI_(Boolean) GetNeedInitialFocus();

    /**
     * Set the priority of the Render thread. Unlike the other settings, this
     * one only needs to be called once per process. The default is NORMAL.
     *
     * @param priority RenderPriority, can be normal, high or low.
     */
	virtual CARAPI_(void) SetRenderPriority(
		/* [in] */ RenderPriority priority);
    
    /**
     * Override the way the cache is used. The way the cache is used is based
     * on the navigation option. For a normal page load, the cache is checked
     * and content is re-validated as needed. When navigating back, content is
     * not revalidated, instead the content is just pulled from the cache.
     * This function allows the client to override this behavior.
     * @param mode One of the LOAD_ values.
     */
	virtual CARAPI_(void) SetCacheMode(
		/* [in] */ Int32 mode);
    
    /**
     * Return the current setting for overriding the cache mode. For a full
     * description, see the {@link #setCacheMode(int)} function.
     */
	virtual CARAPI_(Int32) GetCacheMode();
    
    /**
     * If set, webkit alternately shrinks and expands images viewed outside
     * of an HTML page to fit the screen. This conflicts with attempts by
     * the UI to zoom in and out of an image, so it is set false by default.
     * @param shrink Set true to let webkit shrink the standalone image to fit.
     * {@hide}
     */
	virtual CARAPI_(void) SetShrinksStandaloneImagesToFit(
		/* [in] */ Boolean shrink);

    virtual CARAPI_(Int32) GetDoubleTapToastCount();

    virtual CARAPI_(void) SetDoubleTapToastCount(
    	/* [in] */ Int32 count);

    /**
     * Transfer messages from the queue to the new WebCoreThread. Called from
     * WebCore thread.
     */
    /*package*/
	virtual CARAPI_(void) SyncSettingsAndCreateHandler(
		/* [in] */ IBrowserFrame* frame);

    /**
     * Let the Settings object know that our owner is being destroyed.
     */
    /*package*/
	virtual CARAPI_(void) OnDestroyed();

private:
    /**
     * Looks at sLocale and returns current AcceptLanguage String.
     * @return Current AcceptLanguage String.
     */
    CARAPI_(String) GetCurrentAcceptLanguage();
    
    /**
     * Looks at sLocale and mContext and returns current UserAgent String.
     * @return Current UserAgent String.
     */
    CARAPI_(String) GetCurrentUserAgent();

    CARAPI_(void) VerifyNetworkAccess();

    CARAPI_(Int32) Pin(
        /* [in] */ Int32 size);

    /* Post a SYNC message to handle syncing the native settings. */
    CARAPI_(void) PostSync();

    // Synchronize the native and java settings.
    CARAPI_(void) NativeSync(
        /* [in] */ Int32 nativeFrame);

    //Own added     For Init
    CARAPI_(void) InitPara();

public:
    /**
     * Default cache usage pattern  Use with {@link #setCacheMode}.
     */
    static const Int32 WS_LOAD_DEFAULT = -1;

    /**
     * Normal cache usage pattern  Use with {@link #setCacheMode}.
     */
    static const Int32 WS_LOAD_NORMAL = 0;

    /**
     * Use cache if content is there, even if expired (eg, history nav)
     * If it is not in the cache, load from network.
     * Use with {@link #setCacheMode}.
     */
    static const Int32 WS_LOAD_CACHE_ELSE_NETWORK = 1;

    /**
     * Don't use the cache, load from network
     * Use with {@link #setCacheMode}.
     */
    static const Int32 WS_LOAD_NO_CACHE = 2;
    
    /**
     * Don't use the network, load from cache only.
     * Use with {@link #setCacheMode}.
     */
    static const Int32 WS_LOAD_CACHE_ONLY = 3;


private:
    // User agent strings.
	static const CString DESKTOP_USERAGENT;/* =
            "Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10_5_7; en-us)"
            + " AppleWebKit/530.17 (KHTML, like Gecko) Version/4.0"
            + " Safari/530.17";*/
	static const CString IPHONE_USERAGENT;/* = 
            "Mozilla/5.0 (iPhone; U; CPU iPhone OS 3_0 like Mac OS X; en-us)"
            + " AppleWebKit/528.18 (KHTML, like Gecko) Version/4.0"
            + " Mobile/7A341 Safari/528.16";*/
	static AutoPtr<ILocale> sLocale;
	//static AutoPtr<IInterface> sLockForLocaleSettings;
    static Core::Threading::Mutex sLockForLocaleSettings;

private:

    // WebView associated with this WebSettings.
    AutoPtr<IWebView> mWebView;
    // BrowserFrame used to access the native frame pointer.
    AutoPtr<IBrowserFrame> mBrowserFrame;
    // Flag to prevent multiple SYNC messages at one time.
    Boolean mSyncPending;
    // Custom handler that queues messages until the WebCore thread is active.
    /*const*/ EventHandler* mEventHandler;

    // Private settings so we don't have to go into native code to
    // retrieve the values. After setXXX, postSync() needs to be called.
    //
    // The default values need to match those in WebSettings.cpp
    // If the defaults change, please also update the JavaDocs so developers
    // know what they are.
    LayoutAlgorithm mLayoutAlgorithm;
    AutoPtr<IContext> mContext;
    TextSize*       mTextSize;
    String          mStandardFontFamily;
    String          mFixedFontFamily;
    String          mSansSerifFontFamily;
    String          mSerifFontFamily;
    String          mCursiveFontFamily;
    String          mFantasyFontFamily;
    String          mDefaultTextEncoding;
    String          mUserAgent;
    Boolean         mUseDefaultUserAgent;
    String          mAcceptLanguage;
    Int32           mMinimumFontSize;
    Int32           mMinimumLogicalFontSize;
    Int32           mDefaultFontSize;
    Int32           mDefaultFixedFontSize;
    Int32           mPageCacheCapacity;
    Boolean         mLoadsImagesAutomatically;
    Boolean         mBlockNetworkImage;
    Boolean         mBlockNetworkLoads;
    Boolean         mJavaScriptEnabled;
    PluginState     mPluginState;
    Boolean         mJavaScriptCanOpenWindowsAutomatically;
    Boolean         mUseDoubleTree;
    Boolean         mUseWideViewport;
    Boolean         mSupportMultipleWindows;
    Boolean         mShrinksStandaloneImagesToFit;
    // HTML5 API flags
    Boolean         mAppCacheEnabled;
    Boolean         mDatabaseEnabled;
    Boolean         mDomStorageEnabled;
    Boolean         mWorkersEnabled;  // only affects V8.
    Boolean         mGeolocationEnabled;
    // HTML5 configuration parameters
    Int64           mAppCacheMaxSize;
    String          mAppCachePath;
    String          mDatabasePath;
    // The WebCore DatabaseTracker only allows the database path to be set
    // once. Keep track of when the path has been set.
    Boolean         mDatabasePathHasBeenSet;
    String          mGeolocationDatabasePath;
    // Don't need to synchronize the get/set methods as they
    // are basic types, also none of these values are used in
    // native WebCore code.
    ZoomDensity*    mDefaultZoom;
    RenderPriority  mRenderPriority;
    Int32           mOverrideCacheMode;
    Boolean         mSaveFormData;
    Boolean         mSavePassword;
    Boolean         mLightTouchEnabled;
    Boolean         mNeedInitialFocus;
    Boolean         mNavDump;
    Boolean         mSupportZoom;
    Boolean         mBuiltInZoomControls;
    Boolean         mAllowFileAccess;
    Boolean         mAllowContentAccess;
    Boolean         mLoadWithOverviewMode;
    Boolean         mUseWebViewBackgroundOverscrollBackground;

    // private WebSettings, not accessible by the host activity
    static Int32      mDoubleTapToastCount;

    static const CString PREF_FILE;// = "WebViewSettings";
    static const CString DOUBLE_TAP_TOAST_COUNT;// = "double_tap_toast_count";
 
protected:
    Core::Threading::Mutex mMutex;
};

#endif //__WEBSETTING_H__