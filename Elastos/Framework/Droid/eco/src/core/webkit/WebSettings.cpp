
#include "webkit/WebSettings.h"

#define AUTOLOCK() do {\
		Core::Threading::Mutex mutex;\
		Core::Threading::Mutex::Autolock lock(mutex);\
	}while(0)

// User agent strings.
const CString WebSettings::DESKTOP_USERAGENT =
        "Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10_5_7; en-us) AppleWebKit/530.17 (KHTML, like Gecko) Version/4.0 Safari/530.17";
const CString WebSettings::IPHONE_USERAGENT = 
        "Mozilla/5.0 (iPhone; U; CPU iPhone OS 3_0 like Mac OS X; en-us) AppleWebKit/528.18 (KHTML, like Gecko) Version/4.0 Mobile/7A341 Safari/528.16";

const CString WebSettings::PREF_FILE = "WebViewSettings";
const CString WebSettings::DOUBLE_TAP_TOAST_COUNT = "double_tap_toast_count";

Int32 WebSettings::mDoubleTapToastCount;

AutoPtr<ILocale> WebSettings::sLocale;
Core::Threading::Mutex WebSettings::sLockForLocaleSettings;

WebSettings::WebSettings()
{}

/**
 * Package constructor to prevent clients from creating a new settings
 * instance.
 */
WebSettings::WebSettings(
	/* [in] */ IContext* context, 
	/* [in] */ IWebView* webview)
{
	mEventHandler = new EventHandler();
    mContext = context;
    mWebView = webview;
//    mDefaultTextEncoding = context->GetString(com.android.internal. R.string.default_text_encoding);

//    if (sLockForLocaleSettings == NULL) {
//        sLockForLocaleSettings = new Object();
//        sLocale = Locale.getDefault();
//    }
    mAcceptLanguage = GetCurrentAcceptLanguage();
    mUserAgent = GetCurrentUserAgent();
    mUseDefaultUserAgent = TRUE;

//    mBlockNetworkLoads = mContext.checkPermission("android.permission.INTERNET", android.os.Process.myPid(),
//            android.os.Process.myUid()) != PackageManager.PERMISSION_GRANTED;
}

/**
 * Enables dumping the pages navigation cache to a text file.
 */
CARAPI_(void) WebSettings::SetNavDump(
	/* [in] */ Boolean enabled)
{
	mNavDump = enabled;
}

/**
 * Returns true if dumping the navigation cache is enabled.
 */
CARAPI_(Boolean) WebSettings::GetNavDump()
{
	return mNavDump;
}

/**
 * Set whether the WebView supports zoom
 */
CARAPI_(void) WebSettings::SetSupportZoom(
	/* [in] */ Boolean support)
{
	mSupportZoom = support;
	mWebView->UpdateMultiTouchSupport(mContext);
}

/**
 * Returns whether the WebView supports zoom
 */
CARAPI_(Boolean) WebSettings::SupportZoom()
{
	return mSupportZoom;
}

/**
 * Sets whether the zoom mechanism built into WebView is used.
 */
CARAPI_(void) WebSettings::SetBuiltInZoomControls(
	/* [in] */ Boolean enabled)
{
	mBuiltInZoomControls = enabled;
	mWebView->UpdateMultiTouchSupport(mContext);
}

/**
 * Returns true if the zoom mechanism built into WebView is being used.
 */
CARAPI_(Boolean) WebSettings::GetBuiltInZoomControls()
{
	return mBuiltInZoomControls;
}

/**
 * Enable or disable file access within WebView. File access is enabled by
 * default. Note that this enables or disables file system access only.
 * Assets and resources are still accessible using file:///android_asset and
 * file:///android_res.
 */
CARAPI_(void) WebSettings::SetAllowFileAccess(
	/* [in] */ Boolean allow)
{
	mAllowFileAccess = allow;
}

/**
 * Returns true if this WebView supports file access.
 */
CARAPI_(Boolean) WebSettings::GetAllowFileAccess()
{
	return mAllowFileAccess;
}

/**
 * Enable or disable content url access within WebView.  Content url access
 * allows WebView to load content from a content provider installed in the
 * system.  The default is enabled.
 * @hide
 */
CARAPI_(void) WebSettings::SetAllowContentAccess(
	/* [in] */ Boolean allow)
{
	mAllowContentAccess = allow;
}

/**
 * Returns true if this WebView supports content url access.
 * @hide
 */
CARAPI_(Boolean) WebSettings::GetAllowContentAccess()
{
	return mAllowContentAccess;
}

/**
 * Set whether the WebView loads a page with overview mode.
 */
CARAPI_(void) WebSettings::SetLoadWithOverviewMode(
	/* [in] */ Boolean overview)
{
	mLoadWithOverviewMode = overview;
}

/**
 * Returns true if this WebView loads page with overview mode
 */
CARAPI_(Boolean) WebSettings::GetLoadWithOverviewMode()
{
	return mLoadWithOverviewMode;
}

/**
 * Set whether the WebView uses its background for over scroll background.
 * If true, it will use the WebView's background. If false, it will use an
 * internal pattern. Default is true.
 */
CARAPI_(void) WebSettings::SetUseWebViewBackgroundForOverscrollBackground(
	/* [in] */ Boolean view)
{
	mUseWebViewBackgroundOverscrollBackground = view;
}

/**
 * Returns true if this WebView uses WebView's background instead of
 * internal pattern for over scroll background.
 */
CARAPI_(Boolean) WebSettings::GetUseWebViewBackgroundForOverscrollBackground()
{
	return mUseWebViewBackgroundOverscrollBackground;
}

/**
 * Store whether the WebView is saving form data.
 */
CARAPI_(void) WebSettings::SetSaveFormData(
	/* [in] */ Boolean save)
{
	mSaveFormData = save;
}

/**
 *  Return whether the WebView is saving form data.
 */
CARAPI_(Boolean) WebSettings::GetSaveFormData()
{
	return mSaveFormData;
}

/**
 *  Store whether the WebView is saving password.
 */
CARAPI_(void) WebSettings::SetSavePassword(
	/* [in] */ Boolean save)
{
	mSavePassword = save;
}

/**
 *  Return whether the WebView is saving password.
 */
CARAPI_(Boolean) WebSettings::GetSavePassword()
{
	return mSavePassword;
}

/**
 * Set the text size of the page.
 * @param t A TextSize value for increasing or decreasing the text.
 * @see WebSettings.TextSize
 */
CARAPI_(void) WebSettings::SetTextSize(
	/* [in] */ TextSize* t)
{
	assert(t != NULL);
#if 0
	if (WebView.mLogEvent && mTextSize != t ) {
        EventLog.WriteEvent(EventLogTags::BROWSER_TEXT_SIZE_CHANGE,
                mTextSize->value, t->value);
    }
#endif
    mTextSize = t;
    PostSync();
}

/**
 * Get the text size of the page.
 * @return A TextSize enum value describing the text size.
 * @see WebSettings.TextSize
 */
CARAPI_(WebSettings::TextSize*) WebSettings::GetTextSize()
{
	return mTextSize;
}

/**
 * Set the default zoom density of the page. This should be called from UI
 * thread.
 * @param zoom A ZoomDensity value
 * @see WebSettings.ZoomDensity
 */
CARAPI_(void) WebSettings::SetDefaultZoom(
	/* [in] */ WebSettings::ZoomDensity* zoom)
{
	assert(zoom != NULL);

	if (mDefaultZoom != zoom) {
        mDefaultZoom = zoom;
//        mWebView->UpdateDefaultZoomDensity(zoom->value);
    }
}

/**
 * Get the default zoom density of the page. This should be called from UI
 * thread.
 * @return A ZoomDensity value
 * @see WebSettings.ZoomDensity
 */
CARAPI_(WebSettings::ZoomDensity*) WebSettings::GetDefaultZoom()
{
	return mDefaultZoom;
}

/**
 * Enables using light touches to make a selection and activate mouseovers.
 */
CARAPI_(void) WebSettings::SetLightTouchEnabled(
	/* [in] */ Boolean enabled)
{
	mLightTouchEnabled = enabled;
}

/**
 * Returns true if light touches are enabled.
 */
CARAPI_(Boolean) WebSettings::GetLightTouchEnabled()
{
	return mLightTouchEnabled;
}

/**
 * @deprecated This setting controlled a rendering optimization
 * that is no longer present. Setting it now has no effect.
 */
CARAPI_(void) WebSettings::SetUseDoubleTree(
	/* [in] */ Boolean use)
{
	return;
}

/**
 * @deprecated This setting controlled a rendering optimization
 * that is no longer present. Setting it now has no effect.
 */
CARAPI_(Boolean) WebSettings::GetUseDoubleTree()
{
	return FALSE;
}

/**
 * Tell the WebView about user-agent string.
 * @param ua 0 if the WebView should use an Android user-agent string,
 *           1 if the WebView should use a desktop user-agent string.
 *
 * @deprecated Please use setUserAgentString instead.
 */
CARAPI_(void) WebSettings::SetUserAgent(
	/* [in] */ Int32 ua)
{
	String uaString;
    if (ua == 1) {
        if (mUserAgent.Equals(DESKTOP_USERAGENT)) {
            return; // do nothing
        } else {
            uaString = DESKTOP_USERAGENT;
        }
    } else if (ua == 2) {
        if (mUserAgent.Equals(IPHONE_USERAGENT)) {
            return; // do nothing
        } else {
            uaString = IPHONE_USERAGENT;
        }
    } else if (ua != 0) {
        return; // do nothing
    }

    SetUserAgentString(uaString);
}

/**
 * Return user-agent as int
 * @return int  0 if the WebView is using an Android user-agent string.
 *              1 if the WebView is using a desktop user-agent string.
 *             -1 if the WebView is using user defined user-agent string.
 *
 * @deprecated Please use getUserAgentString instead.
 */
CARAPI_(Int32) WebSettings::GetUserAgent()
{
	AUTOLOCK();

	if (mUserAgent.Equals(DESKTOP_USERAGENT)) {
        return 1;
    } else if (mUserAgent.Equals(IPHONE_USERAGENT)) {
        return 2;
    } else if (mUseDefaultUserAgent) {
        return 0;
    }

    return -1;
}

/**
 * Tell the WebView to use the wide viewport
 */
CARAPI_(void) WebSettings::SetUseWideViewPort(
	/* [in] */ Boolean use)
{
	AUTOLOCK();

	if (mUseWideViewport != use) {
        mUseWideViewport = use;
        PostSync();
    }
}

/**
 * @return True if the WebView is using a wide viewport
 */
CARAPI_(Boolean) WebSettings::GetUseWideViewPort()
{
	AUTOLOCK();

	return mUseWideViewport;
}

/**
 * Tell the WebView whether it supports multiple windows. TRUE means
 *         that {@link WebChromeClient#onCreateWindow(WebView, boolean,
 *         boolean, Message)} is implemented by the host application.
 */
CARAPI_(void) WebSettings::SetSupportMultipleWindows(
	/* [in] */ Boolean support)
{
	AUTOLOCK();

	if (mSupportMultipleWindows != support) {
        mSupportMultipleWindows = support;
        PostSync();
    }
}

/**
 * @return True if the WebView is supporting multiple windows. This means
 *         that {@link WebChromeClient#onCreateWindow(WebView, boolean,
 *         boolean, Message)} is implemented by the host application.
 */
CARAPI_(Boolean) WebSettings::SupportMultipleWindows()
{
	AUTOLOCK();

	return mSupportMultipleWindows;
}

/**
 * Set the underlying layout algorithm. This will cause a relayout of the
 * WebView.
 * @param l A LayoutAlgorithm enum specifying the algorithm to use.
 * @see WebSettings.LayoutAlgorithm
 */
CARAPI_(void) WebSettings::SetLayoutAlgorithm(
	/* [in] */ WebSettings::LayoutAlgorithm l)
{
	AUTOLOCK();

	// XXX: This will only be affective if libwebcore was built with
    // ANDROID_LAYOUT defined.
    if (mLayoutAlgorithm != l) {
        mLayoutAlgorithm = l;
        PostSync();
    }
}

/**
 * Return the current layout algorithm. The default is NARROW_COLUMNS.
 * @return LayoutAlgorithm enum value describing the layout algorithm
 *         being used.
 * @see WebSettings.LayoutAlgorithm
 */
CARAPI_(WebSettings::LayoutAlgorithm) WebSettings::GetLayoutAlgorithm()
{
	AUTOLOCK();

	return mLayoutAlgorithm;
}

/**
 * Set the standard font family name.
 * @param font A font family name.
 */
CARAPI_(void) WebSettings::SetStandardFontFamily(
	/* [in] */ const String& font)
{
	AUTOLOCK();

	if (font.GetLength() != 0 && !font.Equals(mStandardFontFamily)) {
        mStandardFontFamily = font;
        PostSync();
    }
}

/**
 * Get the standard font family name. The default is "sans-serif".
 * @return The standard font family name as a string.
 */
CARAPI_(CString) WebSettings::GetStandardFontFamily()
{
	AUTOLOCK();

	return mStandardFontFamily;
}

/**
 * Set the fixed font family name.
 * @param font A font family name.
 */
CARAPI_(void) WebSettings::SetFixedFontFamily(
	/* [in] */ const String& font)
{
	AUTOLOCK();

	if (font.GetLength() != 0 && !font.Equals(mFixedFontFamily)) {
        mFixedFontFamily = font;
        PostSync();
    }
}

/**
 * Get the fixed font family name. The default is "monospace".
 * @return The fixed font family name as a string.
 */
CARAPI_(CString) WebSettings::GetFixedFontFamily()
{
	AUTOLOCK();

	return mFixedFontFamily;
}

/**
 * Set the sans-serif font family name.
 * @param font A font family name.
 */
CARAPI_(void) WebSettings::SetSansSerifFontFamily(
	/* [in] */ const String& font)
{
	AUTOLOCK();

	if (font.GetLength() != 0 && !font.Equals(mSansSerifFontFamily)) {
        mSansSerifFontFamily = font;
        PostSync();
    }
}

/**
 * Get the sans-serif font family name.
 * @return The sans-serif font family name as a string.
 */
CARAPI_(CString) WebSettings::GetSansSerifFontFamily()
{
	AUTOLOCK();

	return mSansSerifFontFamily;
}

/**
 * Set the serif font family name. The default is "sans-serif".
 * @param font A font family name.
 */
CARAPI_(void) WebSettings::SetSerifFontFamily(
	/* [in] */ const String& font)
{
	AUTOLOCK();

	if (font.GetLength() != 0 && !font.Equals(mSerifFontFamily)) {
        mSerifFontFamily = font;
        PostSync();
    }
}

/**
 * Get the serif font family name. The default is "serif".
 * @return The serif font family name as a string.
 */
CARAPI_(String) WebSettings::GetSerifFontFamily()
{
	AUTOLOCK();

	return mSerifFontFamily;
}

/**
 * Set the cursive font family name.
 * @param font A font family name.
 */
CARAPI_(void) WebSettings::SetCursiveFontFamily(
	/* [in] */ const String& font)
{
	AUTOLOCK();

	if (font.GetLength() != 0 && !font.Equals(mCursiveFontFamily)) {
        mCursiveFontFamily = font;
        PostSync();
    }
}

/**
 * Get the cursive font family name. The default is "cursive".
 * @return The cursive font family name as a string.
 */
CARAPI_(String) WebSettings::GetCursiveFontFamily()
{
	AUTOLOCK();

	return mCursiveFontFamily;
}

/**
 * Set the fantasy font family name.
 * @param font A font family name.
 */
CARAPI_(void) WebSettings::SetFantasyFontFamily(
	/* [in] */ const String& font)
{
	AUTOLOCK();

	if (font.GetLength() != 0 && !font.Equals(mFantasyFontFamily)) {
        mFantasyFontFamily = font;
        PostSync();
    }
}

/**
 * Get the fantasy font family name. The default is "fantasy".
 * @return The fantasy font family name as a string.
 */
CARAPI_(String) WebSettings::GetFantasyFontFamily()
{
	AUTOLOCK();

	return mFantasyFontFamily;
}

/**
 * Set the minimum font size.
 * @param size A non-negative integer between 1 and 72.
 * Any number outside the specified range will be pinned.
 */
CARAPI_(void) WebSettings::SetMinimumFontSize(
	/* [in] */ Int32 size)
{
	AUTOLOCK();

	size = Pin(size);
    if (mMinimumFontSize != size) {
        mMinimumFontSize = size;
        PostSync();
    }
}

/**
 * Get the minimum font size. The default is 8.
 * @return A non-negative integer between 1 and 72.
 */
CARAPI_(Int32) WebSettings::GetMinimumFontSize()
{
	AUTOLOCK();

	return mMinimumFontSize;
}

/**
 * Set the minimum logical font size.
 * @param size A non-negative integer between 1 and 72.
 * Any number outside the specified range will be pinned.
 */
CARAPI_(void) WebSettings::SetMinimumLogicalFontSize(
	/* [in] */ Int32 size)
{
	AUTOLOCK();

	size = Pin(size);
    if (mMinimumLogicalFontSize != size) {
        mMinimumLogicalFontSize = size;
        PostSync();
    }
}

/**
 * Get the minimum logical font size. The default is 8.
 * @return A non-negative integer between 1 and 72.
 */
CARAPI_(Int32) WebSettings::GetMinimumLogicalFontSize()
{
	AUTOLOCK();

	return mMinimumLogicalFontSize;
}

/**
 * Set the default font size.
 * @param size A non-negative integer between 1 and 72.
 * Any number outside the specified range will be pinned.
 */
CARAPI_(void) WebSettings::SetDefaultFontSize(
	/* [in] */ Int32 size)
{
	AUTOLOCK();

	size = Pin(size);
    if (mDefaultFontSize != size) {
        mDefaultFontSize = size;
        PostSync();
    }
}

/**
 * Get the default font size. The default is 16.
 * @return A non-negative integer between 1 and 72.
 */
CARAPI_(Int32) WebSettings::GetDefaultFontSize()
{
	AUTOLOCK();

	return mDefaultFontSize;
}

/**
 * Set the default fixed font size.
 * @param size A non-negative integer between 1 and 72.
 * Any number outside the specified range will be pinned.
 */
CARAPI_(void) WebSettings::SetDefaultFixedFontSize(
	/* [in] */ Int32 size)
{
	AUTOLOCK();

	size = Pin(size);
    if (mDefaultFixedFontSize != size) {
        mDefaultFixedFontSize = size;
        PostSync();
    }
}

/**
 * Get the default fixed font size. The default is 16.
 * @return A non-negative integer between 1 and 72.
 */
CARAPI_(Int32) WebSettings::GetDefaultFixedFontSize()
{
	AUTOLOCK();

	return mDefaultFixedFontSize;
}

/**
 * Set the number of pages cached by the WebKit for the history navigation.
 * @param size A non-negative integer between 0 (no cache) and 20 (max).
 * @hide
 */
CARAPI_(void) WebSettings::SetPageCacheCapacity(
	/* [in] */ Int32 size)
{
	AUTOLOCK();

	if (size < 0) size = 0;
    if (size > 20) size = 20;
    if (mPageCacheCapacity != size) {
        mPageCacheCapacity = size;
        PostSync();
    }
}

/**
 * Tell the WebView to load image resources automatically.
 * @param flag True if the WebView should load images automatically.
 */
CARAPI_(void) WebSettings::SetLoadsImagesAutomatically(
	/* [in] */ Boolean flag)
{
	AUTOLOCK();

	if (mLoadsImagesAutomatically != flag) {
        mLoadsImagesAutomatically = flag;
        PostSync();
    }
}

/**
 * Return true if the WebView will load image resources automatically.
 * The default is true.
 * @return True if the WebView loads images automatically.
 */
CARAPI_(Boolean) WebSettings::GetLoadsImagesAutomatically()
{
	AUTOLOCK();

	return mLoadsImagesAutomatically;
}

/**
 * Tell the WebView to block network images. This is only checked when
 * {@link #getLoadsImagesAutomatically} is true. If you set the value to
 * false, images will automatically be loaded. Use this api to reduce
 * bandwidth only. Use {@link #setBlockNetworkLoads} if possible.
 * @param flag True if the WebView should block network images.
 * @see #setBlockNetworkLoads
 */
CARAPI_(void) WebSettings::SetBlockNetworkImage(
	/* [in] */ Boolean flag)
{
	AUTOLOCK();

	if (mBlockNetworkImage != flag) {
        mBlockNetworkImage = flag;
        PostSync();
    }
}

/**
 * Return true if the WebView will block network images. The default is
 * false.
 * @return True if the WebView blocks network images.
 */
CARAPI_(Boolean) WebSettings::GetBlockNetworkImage()
{
	AUTOLOCK();

	return mBlockNetworkImage;;
}

/**
 * Tell the WebView to block all network load requests. If you set the
 * value to false, you must call {@link android.webkit.WebView#reload} to
 * fetch remote resources. This flag supercedes the value passed to
 * {@link #setBlockNetworkImage}.
 * @param flag True if the WebView should block all network loads.
 * @see android.webkit.WebView#reload
 */
CARAPI_(void) WebSettings::SetBlockNetworkLoads(
	/* [in] */ Boolean flag)
{
	AUTOLOCK();

	if (mBlockNetworkImage != flag) {
        mBlockNetworkImage = flag;
        PostSync();
    }
}

/**
 * Return true if the WebView will block all network loads. The default is
 * false.
 * @return True if the WebView blocks all network loads.
 */
CARAPI_(Boolean) WebSettings::GetBlockNetworkLoads()
{
	AUTOLOCK();

	return mBlockNetworkLoads;
}

    /**
 * Tell the WebView to enable javascript execution.
 * @param flag True if the WebView should execute javascript.
 */
CARAPI_(void) WebSettings::SetJavaScriptEnabled(
	/* [in] */ Boolean flag)
{
	AUTOLOCK();

	if (mJavaScriptEnabled != flag) {
        mJavaScriptEnabled = flag;
        PostSync();
    }
}

/**
 * Tell the WebView to enable plugins.
 * @param flag True if the WebView should load plugins.
 * @deprecated This method has been deprecated in favor of
 *             {@link #setPluginState}
 */
CARAPI_(void) WebSettings::SetPluginsEnabled(
	/* [in] */ Boolean flag)
{
	AUTOLOCK();

	SetPluginState(ON);
}

/**
 * Tell the WebView to enable, disable, or have plugins on demand. On
 * demand mode means that if a plugin exists that can handle the embedded
 * content, a placeholder icon will be shown instead of the plugin. When
 * the placeholder is clicked, the plugin will be enabled.
 * @param state One of the PluginState values.
 */
CARAPI_(void) WebSettings::SetPluginState(
	/* [in] */ PluginState state)
{
	AUTOLOCK();

	if (mPluginState != state) {
        mPluginState = state;
        PostSync();
    }
}

/**
 * Set a custom path to plugins used by the WebView. This method is
 * obsolete since each plugin is now loaded from its own package.
 * @param pluginsPath String path to the directory containing plugins.
 * @deprecated This method is no longer used as plugins are loaded from
 * their own APK via the system's package manager.
 */
CARAPI_(void) WebSettings::SetPluginsPath(
	/* [in] */ const String& pluginsPath)
{
	AUTOLOCK();
}

/**
 * Set the path to where database storage API databases should be saved.
 * Nota that the WebCore Database Tracker only allows the path to be set once.
 * This will update WebCore when the Sync runs in the C++ side.
 * @param databasePath String path to the directory where databases should
 *     be saved. May be the empty string but should never be null.
 */
CARAPI_(void) WebSettings::SetDatabasePath(
	/* [in] */ const String& databasePath)
{
	AUTOLOCK();

	if (databasePath.GetLength() != 0 && !mDatabasePathHasBeenSet) {
        mDatabasePath = databasePath;
        mDatabasePathHasBeenSet = TRUE;
        PostSync();
    }
}

/**
 * Set the path where the Geolocation permissions database should be saved.
 * This will update WebCore when the Sync runs in the C++ side.
 * @param databasePath String path to the directory where the Geolocation
 *     permissions database should be saved. May be the empty string but
 *     should never be null.
 */
CARAPI_(void) WebSettings::SetGeolocationDatabasePath(
	/* [in] */ const String& databasePath)
{
	AUTOLOCK();

	if (databasePath.GetLength() != 0
            && !databasePath.Equals(mGeolocationDatabasePath)) {
        mGeolocationDatabasePath = databasePath;
        PostSync();
    }
}

/**
 * Tell the WebView to enable Application Caches API.
 * @param flag True if the WebView should enable Application Caches.
 */
CARAPI_(void) WebSettings::SetAppCacheEnabled(
	/* [in] */ Boolean flag)
{
	AUTOLOCK();

	if (mAppCacheEnabled != flag) {
        mAppCacheEnabled = flag;
        PostSync();
    }
}

/**
 * Set a custom path to the Application Caches files. The client
 * must ensure it exists before this call.
 * @param appCachePath String path to the directory containing Application
 * Caches files. The appCache path can be the empty string but should not
 * be null. Passing null for this parameter will result in a no-op.
 */
CARAPI_(void) WebSettings::SetAppCachePath(
	/* [in] */ const String& appCachePath)
{
	AUTOLOCK();

	if (appCachePath.GetLength() != 0 && !appCachePath.Equals(mAppCachePath)) {
        mAppCachePath = appCachePath;
        PostSync();
    }
}

/**
 * Set the maximum size for the Application Caches content.
 * @param appCacheMaxSize the maximum size in bytes.
 */
CARAPI_(void) WebSettings::SetAppCacheMaxSize(
	/* [in] */ Int64 appCacheMaxSize)
{
	AUTOLOCK();

	if (appCacheMaxSize != mAppCacheMaxSize) {
        mAppCacheMaxSize = appCacheMaxSize;
        PostSync();
    }
}

/**
 * Set whether the database storage API is enabled.
 * @param flag boolean True if the WebView should use the database storage
 *     API.
 */
CARAPI_(void) WebSettings::SetDatabaseEnabled(
	/* [in] */ Boolean flag)
{
	AUTOLOCK();

	if (mDatabaseEnabled != flag) {
       mDatabaseEnabled = flag;
       PostSync();
   }
}

/**
 * Set whether the DOM storage API is enabled.
 * @param flag boolean True if the WebView should use the DOM storage
 *     API.
 */
CARAPI_(void) WebSettings::SetDomStorageEnabled(
	/* [in] */ Boolean flag)
{
	AUTOLOCK();

	if (mDomStorageEnabled != flag) {
       mDomStorageEnabled = flag;
       PostSync();
   }
}

/**
 * Returns true if the DOM Storage API's are enabled.
 * @return True if the DOM Storage API's are enabled.
 */
CARAPI_(Boolean) WebSettings::GetDomStorageEnabled()
{
	AUTOLOCK();

	return mDomStorageEnabled;
}

/**
 * Return the path to where database storage API databases are saved for
 * the current WebView.
 * @return the String path to the database storage API databases.
 */
CARAPI_(String) WebSettings::GetDatabasePath()
{
	AUTOLOCK();

	return mDatabasePath;
}

/**
 * Returns true if database storage API is enabled.
 * @return True if the database storage API is enabled.
 */
CARAPI_(Boolean) WebSettings::GetDatabaseEnabled()
{
	AUTOLOCK();

	return mDatabaseEnabled;
}

/**
 * Tell the WebView to enable WebWorkers API.
 * @param flag True if the WebView should enable WebWorkers.
 * Note that this flag only affects V8. JSC does not have
 * an equivalent setting.
 * @hide pending api council approval
 */
CARAPI_(void) WebSettings::SetWorkersEnabled(
	/* [in] */ Boolean flag)
{
	AUTOLOCK();

	if (mWorkersEnabled != flag) {
        mWorkersEnabled = flag;
        PostSync();
    }
}

/**
 * Sets whether Geolocation is enabled.
 * @param flag Whether Geolocation should be enabled.
 */
CARAPI_(void) WebSettings::SetGeolocationEnabled(
	/* [in] */ Boolean flag)
{
	AUTOLOCK();

	if (mGeolocationEnabled != flag) {
        mGeolocationEnabled = flag;
        PostSync();
    }
}

/**
 * Return true if javascript is enabled. <b>Note: The default is false.</b>
 * @return True if javascript is enabled.
 */
CARAPI_(Boolean) WebSettings::GetJavaScriptEnabled()
{
	AUTOLOCK();

	return mJavaScriptEnabled;
}

/**
 * Return true if plugins are enabled.
 * @return True if plugins are enabled.
 * @deprecated This method has been replaced by {@link #getPluginState}
 */
CARAPI_(Boolean) WebSettings::GetPluginsEnabled()
{
	AUTOLOCK();

	return mPluginState == ON;
}

/**
 * Return the current plugin state.
 * @return A value corresponding to the enum PluginState.
 */
CARAPI_(WebSettings::PluginState) WebSettings::GetPluginState()
{
	AUTOLOCK();

	return mPluginState;
}

/**
 * Returns the directory that contains the plugin libraries. This method is
 * obsolete since each plugin is now loaded from its own package.
 * @return An empty string.
 * @deprecated This method is no longer used as plugins are loaded from
 * their own APK via the system's package manager.
 */
CARAPI_(String) WebSettings::GetPluginsPath()
{
	AUTOLOCK();

	return (String)"";
}

/**
 * Tell javascript to open windows automatically. This applies to the
 * javascript function window.open().
 * @param flag True if javascript can open windows automatically.
 */
CARAPI_(void) WebSettings::SetJavaScriptCanOpenWindowsAutomatically(
	/* [in] */ Boolean flag)
{
	AUTOLOCK();

	if (mJavaScriptCanOpenWindowsAutomatically != flag) {
        mJavaScriptCanOpenWindowsAutomatically = flag;
        PostSync();
    }
}

/**
 * Return true if javascript can open windows automatically. The default
 * is false.
 * @return True if javascript can open windows automatically during
 *         window.open().
 */
CARAPI_(Boolean) WebSettings::GetJavaScriptCanOpenWindowsAutomatically()
{
	AUTOLOCK();

	return mJavaScriptCanOpenWindowsAutomatically;
}

/**
 * Set the default text encoding name to use when decoding html pages.
 * @param encoding The text encoding name.
 */
CARAPI_(void) WebSettings::SetDefaultTextEncodingName(
	/* [in] */ const String& encoding)
{
	AUTOLOCK();

	if (encoding.GetLength() != 0 && !encoding.Equals(mDefaultTextEncoding)) {
        mDefaultTextEncoding = encoding;
        PostSync();
    }
}

/**
 * Get the default text encoding name. The default is "Latin-1".
 * @return The default text encoding name as a string.
 */
CARAPI_(String) WebSettings::GetDefaultTextEncodingName()
{
	AUTOLOCK();

	return mDefaultTextEncoding;
}

/**
 * Set the WebView's user-agent string. If the string "ua" is null or empty,
 * it will use the system default user-agent string.
 */
CARAPI_(void) WebSettings::SetUserAgentString(
	/* [in] */ String ua)
{
	if (/*ua == null || */ua.GetLength() == 0) {
    	sLockForLocaleSettings.Lock();
		{
            AutoPtr<ILocale> currentLocale;// = Locale.GetDefault(); 
            if (/*!sLocale.Equals(currentLocale)*/FALSE) {
                sLocale = currentLocale;
                mAcceptLanguage = GetCurrentAcceptLanguage();
            }
        }
        sLockForLocaleSettings.Unlock();

        ua = GetCurrentUserAgent();
        mUseDefaultUserAgent = TRUE;
    } else  {
        mUseDefaultUserAgent = FALSE;
    }

    if (!ua.Equals(mUserAgent)) {
        mUserAgent = ua;
        PostSync();
    }
}

/**
 * Return the WebView's user-agent string.
 */
CARAPI_(String*) WebSettings::GetUserAgentString()
{
	AUTOLOCK();

	if (mUserAgent.Equals(DESKTOP_USERAGENT) ||
            mUserAgent.Equals(IPHONE_USERAGENT) ||
            !mUseDefaultUserAgent) {
        return &mUserAgent;
    }

    Boolean doPostSync = FALSE;
    
    sLockForLocaleSettings.Lock();
    {
        AutoPtr<ILocale> currentLocale;// = Locale.GetDefault();
        if (/*!sLocale.Equals(currentLocale)*/FALSE) {
            sLocale = currentLocale;
            mUserAgent = GetCurrentUserAgent();
            mAcceptLanguage = GetCurrentAcceptLanguage();
            doPostSync = TRUE;
        }
    }
    sLockForLocaleSettings.Unlock();

    if (doPostSync) {
        PostSync();
    }

    return &mUserAgent;
}

/* package api to grab the Accept Language string. */
/*package*/ 
CARAPI_(String*) WebSettings::GetAcceptLanguage()
{
	AUTOLOCK();

	sLockForLocaleSettings.Lock();
	{
        AutoPtr<ILocale> currentLocale;// = Locale.getDefault();
        if (/*!sLocale.equals(currentLocale)*/FALSE) {
            sLocale = currentLocale;
            mAcceptLanguage = GetCurrentAcceptLanguage();
        }
    }
    sLockForLocaleSettings.Unlock();

    return &mAcceptLanguage;
}

/**
 * Tell the WebView whether it needs to set a node to have focus when
 * {@link WebView#requestFocus(int, android.graphics.Rect)} is called.
 * 
 * @param flag
 */
CARAPI_(void) WebSettings::SetNeedInitialFocus(
	/* [in] */ Boolean flag)
{
	if (mNeedInitialFocus != flag) {
        mNeedInitialFocus = flag;
    }
}

/* Package api to get the choice whether it needs to set initial focus. */
/* package */ 
CARAPI_(Boolean) WebSettings::GetNeedInitialFocus()
{
	return mNeedInitialFocus;
}

/**
 * Set the priority of the Render thread. Unlike the other settings, this
 * one only needs to be called once per process. The default is NORMAL.
 *
 * @param priority RenderPriority, can be normal, high or low.
 */
CARAPI_(void) WebSettings::SetRenderPriority(
	/* [in] */ RenderPriority priority)
{
	AUTOLOCK();

	if (mRenderPriority != priority) {
        mRenderPriority = priority;
//        mEventHandler.sendMessage(Message.obtain(null, EventHandler.PRIORITY));
    }
}

/**
 * Override the way the cache is used. The way the cache is used is based
 * on the navigation option. For a normal page load, the cache is checked
 * and content is re-validated as needed. When navigating back, content is
 * not revalidated, instead the content is just pulled from the cache.
 * This function allows the client to override this behavior.
 * @param mode One of the LOAD_ values.
 */
CARAPI_(void) WebSettings::SetCacheMode(
	/* [in] */ Int32 mode)
{
	if (mode != mOverrideCacheMode) {
        mOverrideCacheMode = mode;
    }
}

/**
 * Return the current setting for overriding the cache mode. For a full
 * description, see the {@link #setCacheMode(int)} function.
 */
CARAPI_(Int32) WebSettings::GetCacheMode()
{
	return mOverrideCacheMode;
}

/**
 * If set, webkit alternately shrinks and expands images viewed outside
 * of an HTML page to fit the screen. This conflicts with attempts by
 * the UI to zoom in and out of an image, so it is set false by default.
 * @param shrink Set true to let webkit shrink the standalone image to fit.
 * {@hide}
 */
CARAPI_(void) WebSettings::SetShrinksStandaloneImagesToFit(
	/* [in] */ Boolean shrink)
{
	if (mShrinksStandaloneImagesToFit != shrink) {
        mShrinksStandaloneImagesToFit = shrink;
        PostSync();
    }
}

CARAPI_(Int32) WebSettings::GetDoubleTapToastCount()
{
	return mDoubleTapToastCount;
}

CARAPI_(void) WebSettings::SetDoubleTapToastCount(
	/* [in] */ Int32 count)
{
	if (mDoubleTapToastCount != count) {
        mDoubleTapToastCount = count;
        // write the settings in the non-UI thread
//        mEventHandler.sendMessage(Message.obtain(null, EventHandler.SET_DOUBLE_TAP_TOAST_COUNT));
    }
}

/**
 * Transfer messages from the queue to the new WebCoreThread. Called from
 * WebCore thread.
 */
/*package*/
CARAPI_(void) WebSettings::SyncSettingsAndCreateHandler(
	/* [in] */ IBrowserFrame* frame)
{
#if 0
	mBrowserFrame = frame;
    if (DebugFlags.WEB_SETTINGS) {
        junit.framework.Assert.assertTrue(frame.mNativeFrame != 0);
    }

    SharedPreferences sp = mContext.getSharedPreferences(PREF_FILE,
            Context.MODE_PRIVATE);
    if (mDoubleTapToastCount > 0) {
        mDoubleTapToastCount = sp.getInt(DOUBLE_TAP_TOAST_COUNT,
                mDoubleTapToastCount);
    }
    nativeSync(frame.mNativeFrame);
    mSyncPending = false;
    mEventHandler.createHandler();
#endif
}

/**
 * Let the Settings object know that our owner is being destroyed.
 */
/*package*/
CARAPI_(void) WebSettings::OnDestroyed()
{
	AUTOLOCK();
}

/**
 * Looks at sLocale and returns current AcceptLanguage String.
 * @return Current AcceptLanguage String.
 */
CARAPI_(String) WebSettings::GetCurrentAcceptLanguage()
{}

/**
 * Looks at sLocale and mContext and returns current UserAgent String.
 * @return Current UserAgent String.
 */
CARAPI_(String) WebSettings::GetCurrentUserAgent()
{}

CARAPI_(void) WebSettings::VerifyNetworkAccess()
{}

CARAPI_(Int32) WebSettings::Pin(
	/* [in] */ Int32 size)
{
	// FIXME: 72 is just an arbitrary max text size value.
    if (size < 1) {
        return 1;
    } else if (size > 72) {
        return 72;
    }
    return size;
}

/* Post a SYNC message to handle syncing the native settings. */
CARAPI_(void) WebSettings::PostSync()
{}

// Synchronize the native and java settings.
CARAPI_(void) WebSettings::NativeSync(
	/* [in] */ Int32 nativeFrame)
{
	AUTOLOCK();

	if (!mSyncPending) {
//        mSyncPending = mEventHandler.sendMessage(Message.obtain(null, EventHandler.SYNC));
    }
}

/*****************************EventHandler*****************************/
CARAPI_(void) WebSettings::EventHandler::CreateHandler()
{}

CARAPI_(void) WebSettings::EventHandler::SetRenderPriority()
{}

/**
 * Send a message to the private queue or handler.
 */
CARAPI_(Boolean) WebSettings::EventHandler::SendMessage(
	/* [in] */ IMessage* msg)
{}