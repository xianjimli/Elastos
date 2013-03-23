
#include "webkit/WebSettings.h"

#include <elastos/Math.h>
#include "webkit/CBrowserFrame.h"
#include "os/Process.h"
#include "../../../res/gen/R.h"
#include <StringBuffer.h>
//#include <elastos/CLocale.h>
#include "os/Build.h"
#include "webkit/CWebView.h"
#include "webkit/DebugFlags.h" 
#include <Logger.h>

// User agent strings.
const CString WebSettings::DESKTOP_USERAGENT =
        "Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10_5_7; en-us) AppleWebKit/530.17 (KHTML, like Gecko) Version/4.0 Safari/530.17";
const CString WebSettings::IPHONE_USERAGENT = 
        "Mozilla/5.0 (iPhone; U; CPU iPhone OS 3_0 like Mac OS X; en-us) AppleWebKit/528.18 (KHTML, like Gecko) Version/4.0 Mobile/7A341 Safari/528.16";

// private WebSettings, not accessible by the host activity
Int32 WebSettings::mDoubleTapToastCount = 3;

const CString WebSettings::PREF_FILE = "WebViewSettings";
const CString WebSettings::DOUBLE_TAP_TOAST_COUNT = "double_tap_toast_count";

AutoPtr<ILocale> WebSettings::sLocale;
Mutex WebSettings::sLockForLocaleSettings;

/*****************************WebSettings::WsEventHandler::WsEhHandler*****************************/
PInterface WebSettings::WsEventHandler::WsEhHandler::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IHandler*)this;
    }
    else if (riid == EIID_IHandler) {
        return (IHandler*)this;
    }
    return NULL;
}

UInt32 WebSettings::WsEventHandler::WsEhHandler::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 WebSettings::WsEventHandler::WsEhHandler::Release()
{
    return ElRefBase::Release();
}

ECode WebSettings::WsEventHandler::WsEhHandler::GetInterfaceID(
    /* [in] */ IInterface* Object,
    /* [out] */ InterfaceID* iID)
{
    VALIDATE_NOT_NULL(iID);
    if (iID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (Object == (IInterface*)(IHandler*)this) {
        *iID = EIID_IHandler;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

WebSettings::WsEventHandler::WsEhHandler::WsEhHandler(
    /* [in] */ WsEventHandler* eventHandler,
    /* [in] */ WebSettings* webSettings)
{
    mEventHandler = eventHandler;
    mWebSettings = webSettings;
}
void WebSettings::WsEventHandler::WsEhHandler::HandleMessage(
    /* [in] */IMessage* msg)
{
    switch (/*msg -> what*/0) {
        case (WsEventHandler::SYNC):
            if(TRUE){
                Core::Threading::Mutex::Autolock lock(mWebSettings -> mMutex);
                Int32 nativeFrameT = ( (CBrowserFrame*)((mWebSettings -> mBrowserFrame).Get()) ) -> mNativeFrame;
                if (nativeFrameT != 0) {
                    mWebSettings -> NativeSync(nativeFrameT);
                }
                (mWebSettings -> mSyncPending) = FALSE;
            }
            break;
        case (WsEventHandler::PRIORITY):{
            mEventHandler -> SetRenderPriority();
            break;
        }
        case (WsEventHandler::SET_DOUBLE_TAP_TOAST_COUNT): {
            /*            
            AutoPtr<ISharedPreferences> sharedPreferencesT;    //JAVA:    content/SharedPreferences(Interface)
            mContext -> GetSharedPreferences(PREF_FILE,Context_MODE_PRIVATE,(ISharedPreferences**)&sharedPreferencesT);
            AutoPtr<ISharedPreferencesEditor> editor;        //JAVA:    content/SharedPreferences(Interface)::Editor(Interface)
            sharedPreferencesT -> Edit((ISharedPreferencesEditor**)&editor);
            editor -> PutInt(DOUBLE_TAP_TOAST_COUNT, mDoubleTapToastCount);    //JAVA:    Editor putInt(String key,int value);
            Boolean bCommit;
            editor -> Commit(&bCommit);    //JAVA:    boolean commit();
            */
            break;
        }
    }
}

/*****************************WebSettings::EventHandler*****************************/
WebSettings::WsEventHandler::WsEventHandler(
            /* [in] */ WebSettings* webSettings)
{
    mWebSettings = webSettings;
}

void WebSettings::WsEventHandler::CreateHandler()
{
    Core::Threading::Mutex::Autolock lock(mMutexWsEhThis);
    // as mRenderPriority can be set before thread is running, sync up
    SetRenderPriority();

    // create a new handler
    mHandler = new WsEhHandler(this,mWebSettings);

}

void WebSettings::WsEventHandler::SetRenderPriority()
{
    Core::Threading::Mutex::Autolock lock(mWebSettings -> mMutex);
    if ((mWebSettings -> mRenderPriority) == RP_NORMAL) {
        Process::SetThreadPriority(Process::THREAD_PRIORITY_DEFAULT);    //JAVA:    android.os.Process.setThreadPriority(android.os.Process.THREAD_PRIORITY_DEFAULT);
    } 
    else if ((mWebSettings -> mRenderPriority) == RP_HIGH) {
        Process::SetThreadPriority(Process::THREAD_PRIORITY_FOREGROUND + Process::THREAD_PRIORITY_LESS_FAVORABLE);    //JAVA:    android.os.Process.setThreadPriority(android.os.Process.THREAD_PRIORITY_FOREGROUND + android.os.Process.THREAD_PRIORITY_LESS_FAVORABLE);
    } 
    else if ((mWebSettings -> mRenderPriority) == RP_LOW) {
        Process::SetThreadPriority(Process::THREAD_PRIORITY_BACKGROUND);
    }
}

/**
 * Send a message to the private queue or handler.
 */
Boolean WebSettings::WsEventHandler::SendMessage(
    /* [in] */ IMessage* msg)
{
    Core::Threading::Mutex::Autolock lock(mMutexWsEhThis);
    if(mHandler != NULL) {
        //Boolean bSdMsg;
        //mHandler -> SendMessage(msg,&bSdMsg);
        return TRUE;
    } 
    else {
        return FALSE;
    }
}

/*****************************WebSettings*****************************/
void WebSettings::InitPara()
{
    // Flag to prevent multiple SYNC messages at one time.
    mSyncPending = FALSE;

    mLayoutAlgorithm = NARROW_COLUMNS;//LayoutAlgorithm.NARROW_COLUMNS;

    mTextSize = new WebSettings::TextSize(WebSettings::TextSize::NORMAL);

    mStandardFontFamily = String("sans-serif");
    mFixedFontFamily = String("monospace");
    mSansSerifFontFamily = String("sans-serif");
    mSerifFontFamily = String("serif");
    mCursiveFontFamily = String("cursive");
    mFantasyFontFamily = String("fantasy");

    mMinimumFontSize = 8;
    mMinimumLogicalFontSize = 8;
    mDefaultFontSize = 16;
    mDefaultFixedFontSize = 13;
    mPageCacheCapacity = 0;
    mLoadsImagesAutomatically = TRUE;
    mBlockNetworkImage = FALSE;

    mJavaScriptEnabled = FALSE;
    mPluginState = OFF;//PluginState.OFF;
    mJavaScriptCanOpenWindowsAutomatically = FALSE;
    mUseDoubleTree = FALSE;
    mUseWideViewport = FALSE;
    mSupportMultipleWindows = FALSE;
    mShrinksStandaloneImagesToFit = FALSE;
    // HTML5 API flags
    mAppCacheEnabled = FALSE;
    mDatabaseEnabled = FALSE;
    mDomStorageEnabled = FALSE;
    mWorkersEnabled = FALSE;  // only affects V8.
    mGeolocationEnabled = TRUE;
    // HTML5 configuration parameters
    mAppCacheMaxSize = Elastos::Core::Math::INT64_MAX_VALUE;//Long.MAX_VALUE;
    mAppCachePath = "";
    mDatabasePath = "";
    // The WebCore DatabaseTracker only allows the database path to be set
    // once. Keep track of when the path has been set.
    mDatabasePathHasBeenSet = FALSE;
    mGeolocationDatabasePath = "";
    // Don't need to synchronize the get/set methods as they
    // are basic types, also none of these values are used in
    // native WebCore code.
    mDefaultZoom = new WebSettings::ZoomDensity(WebSettings::ZoomDensity::MEDIUM);
    mRenderPriority = RP_NORMAL;//RenderPriority.NORMAL;
    mOverrideCacheMode = WS_LOAD_DEFAULT;//LOAD_DEFAULT;
    mSaveFormData = TRUE;
    mSavePassword = TRUE;
    mLightTouchEnabled = FALSE;
    mNeedInitialFocus = TRUE;
    mNavDump = FALSE;
    mSupportZoom = TRUE;
    mBuiltInZoomControls = FALSE;
    mAllowFileAccess = TRUE;
    mAllowContentAccess = TRUE;
    mLoadWithOverviewMode = FALSE;
    mUseWebViewBackgroundOverscrollBackground = TRUE;
}

WebSettings::WebSettings()
{
    InitPara();
}

/**
 * Package constructor to prevent clients from creating a new settings
 * instance.
 */
WebSettings::WebSettings(
    /* [in] */ IContext* context, 
    /* [in] */ IWebView* webview)
{
    InitPara();
    mEventHandler = new WebSettings::WsEventHandler::WsEventHandler(this);
    mContext = context;
    mWebView = webview;
    context -> GetString(R::string::default_text_encoding,&mDefaultTextEncoding);

    if(sLocale.Get() == NULL){        
        AutoPtr<ILocaleHelper> localeHelper;
        CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
        localeHelper->GetDefault((ILocale**)&sLocale);
    }

    mAcceptLanguage = GetCurrentAcceptLanguage();
    mUserAgent = GetCurrentUserAgent();
    mUseDefaultUserAgent = TRUE;
    Int32 nPermission = 0;
    //mContext -> CheckPermission(String"android.permission.INTERNET",Process::MyPid(),Process::MyUid(),&nPermission);
    mBlockNetworkLoads = /*(nPermission != PackageManager_PERMISSION_GRANTED)*/FALSE;
}

/**
 * Looks at sLocale and returns current AcceptLanguage String.
 * @return Current AcceptLanguage String.
 */
String WebSettings::GetCurrentAcceptLanguage()
{
    AutoPtr<ILocale> locale;
    sLockForLocaleSettings.Lock();
    locale = sLocale;
    sLockForLocaleSettings.Unlock();
    StringBuffer buffer;
    /*const*/ String language;
    /*const*/ String country;
    locale -> GetLanguage(&language);
    if(language.IsNullOrEmpty()){
        buffer += language;        
        locale -> GetCountry(&country);
        if (country.IsNullOrEmpty()) {
            buffer += String("-");
            buffer += country;
        }
    }
    if( !( (language == String("en")) && (country == String("US")) ) )
    {
        buffer += String(", ");
        buffer += String("en");
        buffer += String("-");
        buffer += String("US");
    }
    return String(buffer);
}

/**
 * Looks at sLocale and mContext and returns current UserAgent String.
 * @return Current UserAgent String.
 */
String WebSettings::GetCurrentUserAgent()
{
    Core::Threading::Mutex::Autolock lock(mMutex);
    AutoPtr<ILocale> locale;
    sLockForLocaleSettings.Lock();
    locale = sLocale;
    sLockForLocaleSettings.Unlock();

    StringBuffer buffer;
    // Add version
    /*const*/ String version;// = Build::VERSION::RELEASE;    //JAVA:    Build.VERSION.RELEASE;
    if (version.GetLength() > 0) {
        buffer += version;
    } 
    else { // default to "1.0"
        buffer += String("1.0");
    }  
    buffer += String("; ");
    /*const*/ String language;
    locale -> GetLanguage(&language);
    if (language.IsNullOrEmpty()) {
        language.ToLowerCase();
        buffer += language;
        /*const*/ String country;
        locale -> GetCountry(&country);
        if (country.IsNullOrEmpty()) {
            buffer += String("-");
            country.ToLowerCase();
            buffer += country;
        }
    }
    else {
        // default to "en"
        buffer += String("en");
    }
    // add the model for the release build
    if (String("REL").Equals(Build::VERSION::CODENAME)) {
        /*const*/ String model = Build::MODEL;
        if (model.GetLength() > 0) {
            buffer += String("; ");
            buffer += model;
        }
    }
    /*const*/ String id = Build::ID;
    if (id.GetLength() > 0) {
        buffer += String(" Build/");
        buffer += String(id);
    }
    AutoPtr<IResources> resT;
    mContext -> GetResources((IResources**)&resT);
    AutoPtr<ICharSequence> base;
    resT -> GetText(R::string::web_user_agent,(ICharSequence**)&base);
    String retVal;
    retVal.AppendFormat((char*)(base.Get()),buffer);
    return retVal;
}

/**
 * Enables dumping the pages navigation cache to a text file.
 */
void WebSettings::SetNavDump(
    /* [in] */ Boolean enabled)
{
    mNavDump = enabled;
}

/**
 * Returns true if dumping the navigation cache is enabled.
 */
Boolean WebSettings::GetNavDump()
{
    return mNavDump;
}

/**
 * Set whether the WebView supports zoom
 */
void WebSettings::SetSupportZoom(
    /* [in] */ Boolean support)
{
    mSupportZoom = support;
    //((CWebView*)(mWebView.Get()))-> UpdateMultiTouchSupport(mContext.Get());
}

/**
 * Returns whether the WebView supports zoom
 */
Boolean WebSettings::SupportZoom()
{
    return mSupportZoom;
}

/**
 * Sets whether the zoom mechanism built into WebView is used.
 */
void WebSettings::SetBuiltInZoomControls(
    /* [in] */ Boolean enabled)
{
    mBuiltInZoomControls = enabled;
    //((CWebView*)(mWebView.Get()))-> UpdateMultiTouchSupport(mContext.Get());
}

/**
 * Returns true if the zoom mechanism built into WebView is being used.
 */
Boolean WebSettings::GetBuiltInZoomControls()
{
    return mBuiltInZoomControls;
}

/**
 * Enable or disable file access within WebView. File access is enabled by
 * default. Note that this enables or disables file system access only.
 * Assets and resources are still accessible using file:///android_asset and
 * file:///android_res.
 */
void WebSettings::SetAllowFileAccess(
    /* [in] */ Boolean allow)
{
    mAllowFileAccess = allow;
}

/**
 * Returns true if this WebView supports file access.
 */
Boolean WebSettings::GetAllowFileAccess()
{
    return mAllowFileAccess;
}

/**
 * Enable or disable content url access within WebView.  Content url access
 * allows WebView to load content from a content provider installed in the
 * system.  The default is enabled.
 * @hide
 */
void WebSettings::SetAllowContentAccess(
    /* [in] */ Boolean allow)
{
    mAllowContentAccess = allow;
}

/**
 * Returns true if this WebView supports content url access.
 * @hide
 */
Boolean WebSettings::GetAllowContentAccess()
{
    return mAllowContentAccess;
}

/**
 * Set whether the WebView loads a page with overview mode.
 */
void WebSettings::SetLoadWithOverviewMode(
    /* [in] */ Boolean overview)
{
    mLoadWithOverviewMode = overview;
}

/**
 * Returns true if this WebView loads page with overview mode
 */
Boolean WebSettings::GetLoadWithOverviewMode()
{
    return mLoadWithOverviewMode;
}

/**
 * Set whether the WebView uses its background for over scroll background.
 * If true, it will use the WebView's background. If false, it will use an
 * internal pattern. Default is true.
 */
void WebSettings::SetUseWebViewBackgroundForOverscrollBackground(
    /* [in] */ Boolean view)
{
    mUseWebViewBackgroundOverscrollBackground = view;
}

/**
 * Returns true if this WebView uses WebView's background instead of
 * internal pattern for over scroll background.
 */
Boolean WebSettings::GetUseWebViewBackgroundForOverscrollBackground()
{
    return mUseWebViewBackgroundOverscrollBackground;
}

/**
 * Store whether the WebView is saving form data.
 */
void WebSettings::SetSaveFormData(
    /* [in] */ Boolean save)
{
    mSaveFormData = save;
}

/**
 *  Return whether the WebView is saving form data.
 */
Boolean WebSettings::GetSaveFormData()
{
    return mSaveFormData;
}

/**
 *  Store whether the WebView is saving password.
 */
void WebSettings::SetSavePassword(
    /* [in] */ Boolean save)
{
    mSavePassword = save;
}

/**
 *  Return whether the WebView is saving password.
 */
Boolean WebSettings::GetSavePassword()
{
    return mSavePassword;
}

/**
 * Set the text size of the page.
 * @param t A TextSize value for increasing or decreasing the text.
 * @see WebSettings.TextSize
 */
void WebSettings::SetTextSize(
    /* [in] */ TextSize* t)
{
    Core::Threading::Mutex::Autolock lock(mMutex);
    if (/*CWebView::mLogEvent &&*/ mTextSize != t ) {
        //JAVA:    EventLog.WriteEvent(EventLogTags::BROWSER_TEXT_SIZE_CHANGE,mTextSize.value, t.value);
    }
    mTextSize = t;
    PostSync();
}

/**
 * Get the text size of the page.
 * @return A TextSize enum value describing the text size.
 * @see WebSettings.TextSize
 */
WebSettings::TextSize* WebSettings::GetTextSize()
{
    Core::Threading::Mutex::Autolock lock(mMutex);
    return mTextSize;
}

/**
 * Set the default zoom density of the page. This should be called from UI
 * thread.
 * @param zoom A ZoomDensity value
 * @see WebSettings.ZoomDensity
 */
void WebSettings::SetDefaultZoom(
    /* [in] */ WebSettings::ZoomDensity* zoom)
{
    assert(zoom != NULL);
    if (mDefaultZoom != zoom) {
        mDefaultZoom = zoom;
        //((CWebView*)(mWebView.Get()))->UpdateDefaultZoomDensity(zoom->value);
    }
}

/**
 * Get the default zoom density of the page. This should be called from UI
 * thread.
 * @return A ZoomDensity value
 * @see WebSettings.ZoomDensity
 */
WebSettings::ZoomDensity* WebSettings::GetDefaultZoom()
{
    return mDefaultZoom;
}

/**
 * Enables using light touches to make a selection and activate mouseovers.
 */
void WebSettings::SetLightTouchEnabled(
    /* [in] */ Boolean enabled)
{
    mLightTouchEnabled = enabled;
}

/**
 * Returns true if light touches are enabled.
 */
Boolean WebSettings::GetLightTouchEnabled()
{
    return mLightTouchEnabled;
}

/**
 * @deprecated This setting controlled a rendering optimization
 * that is no longer present. Setting it now has no effect.
 */
void WebSettings::SetUseDoubleTree(
    /* [in] */ Boolean use)
{
    Core::Threading::Mutex::Autolock lock(mMutex);
    return;
}

/**
 * @deprecated This setting controlled a rendering optimization
 * that is no longer present. Setting it now has no effect.
 */
Boolean WebSettings::GetUseDoubleTree()
{
    Core::Threading::Mutex::Autolock lock(mMutex);
    return FALSE;
}

/**
 * Tell the WebView about user-agent string.
 * @param ua 0 if the WebView should use an Android user-agent string,
 *           1 if the WebView should use a desktop user-agent string.
 *
 * @deprecated Please use setUserAgentString instead.
 */
void WebSettings::SetUserAgent(
    /* [in] */ Int32 ua)
{
    Core::Threading::Mutex::Autolock lock(mMutex);
    String uaString;
    if (ua == 1) {
        if (mUserAgent.Equals(DESKTOP_USERAGENT)) {
            return; // do nothing
        } 
        else {
            uaString = DESKTOP_USERAGENT;
        }
    } 
    else if (ua == 2) {
        if (mUserAgent.Equals(IPHONE_USERAGENT)) {
            return; // do nothing
        } 
        else {
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
Int32 WebSettings::GetUserAgent()
{
    Mutex::Autolock lock(mMutex);

    if (mUserAgent.Equals(DESKTOP_USERAGENT)) {
        return 1;
    } 
    else if (mUserAgent.Equals(IPHONE_USERAGENT)) {
        return 2;
    } 
    else if (mUseDefaultUserAgent) {
        return 0;
    }
    return -1;
}

/**
 * Tell the WebView to use the wide viewport
 */
void WebSettings::SetUseWideViewPort(
    /* [in] */ Boolean use)
{
    Mutex::Autolock lock(mMutex);

    if (mUseWideViewport != use) {
        mUseWideViewport = use;
        PostSync();
    }
}

/**
 * @return True if the WebView is using a wide viewport
 */
Boolean WebSettings::GetUseWideViewPort()
{
    Mutex::Autolock lock(mMutex);

    return mUseWideViewport;
}

/**
 * Tell the WebView whether it supports multiple windows. TRUE means
 *         that {@link WebChromeClient#onCreateWindow(WebView, boolean,
 *         boolean, Message)} is implemented by the host application.
 */
void WebSettings::SetSupportMultipleWindows(
    /* [in] */ Boolean support)
{
    Mutex::Autolock lock(mMutex);

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
Boolean WebSettings::SupportMultipleWindows()
{
    Mutex::Autolock lock(mMutex);

    return mSupportMultipleWindows;
}

/**
 * Set the underlying layout algorithm. This will cause a relayout of the
 * WebView.
 * @param l A LayoutAlgorithm enum specifying the algorithm to use.
 * @see WebSettings.LayoutAlgorithm
 */
void WebSettings::SetLayoutAlgorithm(
    /* [in] */ WebSettings::LayoutAlgorithm l)
{
    Mutex::Autolock lock(mMutex);

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
WebSettings::LayoutAlgorithm WebSettings::GetLayoutAlgorithm()
{
    Mutex::Autolock lock(mMutex);

    return mLayoutAlgorithm;
}

/**
 * Set the standard font family name.
 * @param font A font family name.
 */
void WebSettings::SetStandardFontFamily(
    /* [in] */ const String& font)
{
    Mutex::Autolock lock(mMutex);

    if (font.GetLength() != 0 && !font.Equals(mStandardFontFamily)) {
        mStandardFontFamily = font;
        PostSync();
    }
}

/**
 * Get the standard font family name. The default is "sans-serif".
 * @return The standard font family name as a string.
 */
String WebSettings::GetStandardFontFamily()
{
    Mutex::Autolock lock(mMutex);

    return mStandardFontFamily;
}

/**
 * Set the fixed font family name.
 * @param font A font family name.
 */
void WebSettings::SetFixedFontFamily(
    /* [in] */ const String& font)
{
    Mutex::Autolock lock(mMutex);

    if (font.GetLength() != 0 && !font.Equals(mFixedFontFamily)) {
        mFixedFontFamily = font;
        PostSync();
    }
}

/**
 * Get the fixed font family name. The default is "monospace".
 * @return The fixed font family name as a string.
 */
String WebSettings::GetFixedFontFamily()
{
    Mutex::Autolock lock(mMutex);

    return mFixedFontFamily;
}

/**
 * Set the sans-serif font family name.
 * @param font A font family name.
 */
void WebSettings::SetSansSerifFontFamily(
    /* [in] */ const String& font)
{
    Mutex::Autolock lock(mMutex);

    if (font.GetLength() != 0 && !font.Equals(mSansSerifFontFamily)) {
        mSansSerifFontFamily = font;
        PostSync();
    }
}

/**
 * Get the sans-serif font family name.
 * @return The sans-serif font family name as a string.
 */
String WebSettings::GetSansSerifFontFamily()
{
    Mutex::Autolock lock(mMutex);

    return mSansSerifFontFamily;
}

/**
 * Set the serif font family name. The default is "sans-serif".
 * @param font A font family name.
 */
void WebSettings::SetSerifFontFamily(
    /* [in] */ const String& font)
{
    Mutex::Autolock lock(mMutex);

    if (font.GetLength() != 0 && !font.Equals(mSerifFontFamily)) {
        mSerifFontFamily = font;
        PostSync();
    }
}

/**
 * Get the serif font family name. The default is "serif".
 * @return The serif font family name as a string.
 */
String WebSettings::GetSerifFontFamily()
{
    Mutex::Autolock lock(mMutex);

    return mSerifFontFamily;
}

/**
 * Set the cursive font family name.
 * @param font A font family name.
 */
void WebSettings::SetCursiveFontFamily(
    /* [in] */ const String& font)
{
    Mutex::Autolock lock(mMutex);

    if (font.GetLength() != 0 && !font.Equals(mCursiveFontFamily)) {
        mCursiveFontFamily = font;
        PostSync();
    }
}

/**
 * Get the cursive font family name. The default is "cursive".
 * @return The cursive font family name as a string.
 */
String WebSettings::GetCursiveFontFamily()
{
    Mutex::Autolock lock(mMutex);

    return mCursiveFontFamily;
}

/**
 * Set the fantasy font family name.
 * @param font A font family name.
 */
void WebSettings::SetFantasyFontFamily(
    /* [in] */ const String& font)
{
    Mutex::Autolock lock(mMutex);

    if (font.GetLength() != 0 && !font.Equals(mFantasyFontFamily)) {
        mFantasyFontFamily = font;
        PostSync();
    }
}

/**
 * Get the fantasy font family name. The default is "fantasy".
 * @return The fantasy font family name as a string.
 */
 String WebSettings::GetFantasyFontFamily()
{
    Mutex::Autolock lock(mMutex);

    return mFantasyFontFamily;
}

/**
 * Set the minimum font size.
 * @param size A non-negative integer between 1 and 72.
 * Any number outside the specified range will be pinned.
 */
void WebSettings::SetMinimumFontSize(
    /* [in] */ Int32 size)
{
    Mutex::Autolock lock(mMutex);

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
Int32 WebSettings::GetMinimumFontSize()
{
    Mutex::Autolock lock(mMutex);

    return mMinimumFontSize;
}

/**
 * Set the minimum logical font size.
 * @param size A non-negative integer between 1 and 72.
 * Any number outside the specified range will be pinned.
 */
void WebSettings::SetMinimumLogicalFontSize(
    /* [in] */ Int32 size)
{
    Mutex::Autolock lock(mMutex);

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
Int32 WebSettings::GetMinimumLogicalFontSize()
{
    Mutex::Autolock lock(mMutex);

    return mMinimumLogicalFontSize;
}

/**
 * Set the default font size.
 * @param size A non-negative integer between 1 and 72.
 * Any number outside the specified range will be pinned.
 */
void WebSettings::SetDefaultFontSize(
    /* [in] */ Int32 size)
{
    Mutex::Autolock lock(mMutex);

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
Int32 WebSettings::GetDefaultFontSize()
{
    Mutex::Autolock lock(mMutex);

    return mDefaultFontSize;
}

/**
 * Set the default fixed font size.
 * @param size A non-negative integer between 1 and 72.
 * Any number outside the specified range will be pinned.
 */
void WebSettings::SetDefaultFixedFontSize(
    /* [in] */ Int32 size)
{
    Mutex::Autolock lock(mMutex);

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
Int32 WebSettings::GetDefaultFixedFontSize()
{
    Mutex::Autolock lock(mMutex);

    return mDefaultFixedFontSize;
}

/**
 * Set the number of pages cached by the WebKit for the history navigation.
 * @param size A non-negative integer between 0 (no cache) and 20 (max).
 * @hide
 */
void WebSettings::SetPageCacheCapacity(
    /* [in] */ Int32 size)
{
    Mutex::Autolock lock(mMutex);

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
void WebSettings::SetLoadsImagesAutomatically(
    /* [in] */ Boolean flag)
{
    Mutex::Autolock lock(mMutex);

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
Boolean WebSettings::GetLoadsImagesAutomatically()
{
    Mutex::Autolock lock(mMutex);

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
void WebSettings::SetBlockNetworkImage(
    /* [in] */ Boolean flag)
{
    Mutex::Autolock lock(mMutex);

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
Boolean WebSettings::GetBlockNetworkImage()
{
    Mutex::Autolock lock(mMutex);

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
void WebSettings::SetBlockNetworkLoads(
    /* [in] */ Boolean flag)
{
    Mutex::Autolock lock(mMutex);

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
Boolean WebSettings::GetBlockNetworkLoads()
{
    Mutex::Autolock lock(mMutex);

    return mBlockNetworkLoads;
}

void WebSettings::VerifyNetworkAccess()
{
	if (!mBlockNetworkLoads) {
		Int32 nCheckPermission = 0;
		//mContext -> CheckPermission(String("android.permission.INTERNET"),Process::MyPid(),Process::MyUid(),&nCheckPermission);
		if(/*nCheckPermission != CPackageManager_PERMISSION_GRANTED*/FALSE) {
			Utility::Logging::Logger::W(String("WebSettings"), String("Permission denied - application missing INTERNET permission\n") );
			//return E_SECURITY_EXCEPTION;
        }
    }
}

/**
 * Tell the WebView to enable javascript execution.
 * @param flag True if the WebView should execute javascript.
 */
void WebSettings::SetJavaScriptEnabled(
    /* [in] */ Boolean flag)
{
    Mutex::Autolock lock(mMutex);

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
void WebSettings::SetPluginsEnabled(
    /* [in] */ Boolean flag)
{
    Mutex::Autolock lock(mMutex);

    SetPluginState(ON);
}

/**
 * Tell the WebView to enable, disable, or have plugins on demand. On
 * demand mode means that if a plugin exists that can handle the embedded
 * content, a placeholder icon will be shown instead of the plugin. When
 * the placeholder is clicked, the plugin will be enabled.
 * @param state One of the PluginState values.
 */
void WebSettings::SetPluginState(
    /* [in] */ PluginState state)
{
    Mutex::Autolock lock(mMutex);

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
void WebSettings::SetPluginsPath(
    /* [in] */ const String& pluginsPath)
{
    Mutex::Autolock lock(mMutex);
}

/**
 * Set the path to where database storage API databases should be saved.
 * Nota that the WebCore Database Tracker only allows the path to be set once.
 * This will update WebCore when the Sync runs in the C++ side.
 * @param databasePath String path to the directory where databases should
 *     be saved. May be the empty string but should never be null.
 */
void WebSettings::SetDatabasePath(
    /* [in] */ const String& databasePath)
{
    Mutex::Autolock lock(mMutex);

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
void WebSettings::SetGeolocationDatabasePath(
    /* [in] */ const String& databasePath)
{
    Mutex::Autolock lock(mMutex);

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
void WebSettings::SetAppCacheEnabled(
    /* [in] */ Boolean flag)
{
    Mutex::Autolock lock(mMutex);

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
void WebSettings::SetAppCachePath(
    /* [in] */ const String& appCachePath)
{
    Mutex::Autolock lock(mMutex);

    if (appCachePath.GetLength() != 0 && !appCachePath.Equals(mAppCachePath)) {
        mAppCachePath = appCachePath;
        PostSync();
    }
}

/**
 * Set the maximum size for the Application Caches content.
 * @param appCacheMaxSize the maximum size in bytes.
 */
void WebSettings::SetAppCacheMaxSize(
    /* [in] */ Int64 appCacheMaxSize)
{
    Mutex::Autolock lock(mMutex);

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
void WebSettings::SetDatabaseEnabled(
    /* [in] */ Boolean flag)
{
    Mutex::Autolock lock(mMutex);

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
void WebSettings::SetDomStorageEnabled(
    /* [in] */ Boolean flag)
{
    Mutex::Autolock lock(mMutex);

    if (mDomStorageEnabled != flag) {
       mDomStorageEnabled = flag;
       PostSync();
   }
}

/**
 * Returns true if the DOM Storage API's are enabled.
 * @return True if the DOM Storage API's are enabled.
 */
Boolean WebSettings::GetDomStorageEnabled()
{
    Mutex::Autolock lock(mMutex);

    return mDomStorageEnabled;
}

/**
 * Return the path to where database storage API databases are saved for
 * the current WebView.
 * @return the String path to the database storage API databases.
 */
String WebSettings::GetDatabasePath()
{
    Mutex::Autolock lock(mMutex);

    return mDatabasePath;
}

/**
 * Returns true if database storage API is enabled.
 * @return True if the database storage API is enabled.
 */
Boolean WebSettings::GetDatabaseEnabled()
{
    Mutex::Autolock lock(mMutex);

    return mDatabaseEnabled;
}

/**
 * Tell the WebView to enable WebWorkers API.
 * @param flag True if the WebView should enable WebWorkers.
 * Note that this flag only affects V8. JSC does not have
 * an equivalent setting.
 * @hide pending api council approval
 */
void WebSettings::SetWorkersEnabled(
    /* [in] */ Boolean flag)
{
    Mutex::Autolock lock(mMutex);

    if (mWorkersEnabled != flag) {
        mWorkersEnabled = flag;
        PostSync();
    }
}

/**
 * Sets whether Geolocation is enabled.
 * @param flag Whether Geolocation should be enabled.
 */
void WebSettings::SetGeolocationEnabled(
    /* [in] */ Boolean flag)
{
    Mutex::Autolock lock(mMutex);

    if (mGeolocationEnabled != flag) {
        mGeolocationEnabled = flag;
        PostSync();
    }
}

/**
 * Return true if javascript is enabled. <b>Note: The default is false.</b>
 * @return True if javascript is enabled.
 */
Boolean WebSettings::GetJavaScriptEnabled()
{
    Mutex::Autolock lock(mMutex);

    return mJavaScriptEnabled;
}

/**
 * Return true if plugins are enabled.
 * @return True if plugins are enabled.
 * @deprecated This method has been replaced by {@link #getPluginState}
 */
Boolean WebSettings::GetPluginsEnabled()
{
    Mutex::Autolock lock(mMutex);

    return mPluginState == ON;
}

/**
 * Return the current plugin state.
 * @return A value corresponding to the enum PluginState.
 */
WebSettings::PluginState WebSettings::GetPluginState()
{
    Mutex::Autolock lock(mMutex);

    return mPluginState;
}

/**
 * Returns the directory that contains the plugin libraries. This method is
 * obsolete since each plugin is now loaded from its own package.
 * @return An empty string.
 * @deprecated This method is no longer used as plugins are loaded from
 * their own APK via the system's package manager.
 */
String WebSettings::GetPluginsPath()
{
    Mutex::Autolock lock(mMutex);

    return (String)"";
}

/**
 * Tell javascript to open windows automatically. This applies to the
 * javascript function window.open().
 * @param flag True if javascript can open windows automatically.
 */
void WebSettings::SetJavaScriptCanOpenWindowsAutomatically(
    /* [in] */ Boolean flag)
{
    Mutex::Autolock lock(mMutex);

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
Boolean WebSettings::GetJavaScriptCanOpenWindowsAutomatically()
{
    Mutex::Autolock lock(mMutex);

    return mJavaScriptCanOpenWindowsAutomatically;
}

/**
 * Set the default text encoding name to use when decoding html pages.
 * @param encoding The text encoding name.
 */
void WebSettings::SetDefaultTextEncodingName(
    /* [in] */ const String& encoding)
{
    Mutex::Autolock lock(mMutex);

    if (encoding.GetLength() != 0 && !encoding.Equals(mDefaultTextEncoding)) {
        mDefaultTextEncoding = encoding;
        PostSync();
    }
}

/**
 * Get the default text encoding name. The default is "Latin-1".
 * @return The default text encoding name as a string.
 */
String WebSettings::GetDefaultTextEncodingName()
{
    Mutex::Autolock lock(mMutex);

    return mDefaultTextEncoding;
}

/**
 * Set the WebView's user-agent string. If the string "ua" is null or empty,
 * it will use the system default user-agent string.
 */
void WebSettings::SetUserAgentString(
    /* [in] */ String ua)
{
    if (ua.IsNullOrEmpty()) {
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
String* WebSettings::GetUserAgentString()
{
    Mutex::Autolock lock(mMutex);

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
String* WebSettings::GetAcceptLanguage()
{
    Mutex::Autolock lock(mMutex);

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
void WebSettings::SetNeedInitialFocus(
    /* [in] */ Boolean flag)
{
    if (mNeedInitialFocus != flag) {
        mNeedInitialFocus = flag;
    }
}

/* Package api to get the choice whether it needs to set initial focus. */
/* package */ 
Boolean WebSettings::GetNeedInitialFocus()
{
    return mNeedInitialFocus;
}

/**
 * Set the priority of the Render thread. Unlike the other settings, this
 * one only needs to be called once per process. The default is NORMAL.
 *
 * @param priority RenderPriority, can be normal, high or low.
 */
void WebSettings::SetRenderPriority(
    /* [in] */ RenderPriority priority)
{
    Mutex::Autolock lock(mMutex);

    if (mRenderPriority != priority) {
        mRenderPriority = priority;
        AutoPtr<IMessage> messageT;
        //CMessage::Obtain(NULL, WsEventHandler::PRIORITY,(IMessage**)&messageT);
        mEventHandler -> SendMessage(messageT.Get());
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
void WebSettings::SetCacheMode(
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
Int32 WebSettings::GetCacheMode()
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
void WebSettings::SetShrinksStandaloneImagesToFit(
    /* [in] */ Boolean shrink)
{
    if (mShrinksStandaloneImagesToFit != shrink) {
        mShrinksStandaloneImagesToFit = shrink;
        PostSync();
    }
}

Int32 WebSettings::GetDoubleTapToastCount()
{
    return mDoubleTapToastCount;
}

void WebSettings::SetDoubleTapToastCount(
    /* [in] */ Int32 count)
{
    if (mDoubleTapToastCount != count) {
        mDoubleTapToastCount = count;
        // write the settings in the non-UI thread
        //JAVA:    mEventHandler.sendMessage(Message.obtain(null, EventHandler.SET_DOUBLE_TAP_TOAST_COUNT));
        AutoPtr<IMessage> messageT;
        //CMessage::Obtain(NULL, WsEventHandler::SET_DOUBLE_TAP_TOAST_COUNT,(IMessage**)&messageT);
        mEventHandler -> SendMessage(messageT.Get());
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
	Mutex::Autolock lock(mMutex);
    mBrowserFrame = frame;
    if (DebugFlags::sWEB_SETTINGS) {
        //JAVA:    junit.framework.Assert.assertTrue(frame.mNativeFrame != 0);
        assert( (((CBrowserFrame*)frame) -> mNativeFrame) != 0 );
    }

    /*
    AutoPtr<ISharedPreferences> sp ;	//SharedPreferences sp = mContext.getSharedPreferences(PREF_FILE, Context.MODE_PRIVATE);
    mContext -> GetSharedPreferences(PREF_FILE, Context.MODE_PRIVATE, (ISharedPreferences**)&sp);
    if (mDoubleTapToastCount > 0) {
        sp -> GetInt(DOUBLE_TAP_TOAST_COUNT, mDoubleTapToastCount ,&mDoubleTapToastCount);
    }
    */
    NativeSync( (((CBrowserFrame*)frame) -> mNativeFrame) );
    mSyncPending = false;
    mEventHandler -> CreateHandler();
}

/**
 * Let the Settings object know that our owner is being destroyed.
 */
/*package*/
void WebSettings::OnDestroyed()
{
    Mutex::Autolock lock(mMutex);
}

Int32 WebSettings::Pin(
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
{
	Mutex::Autolock lock(mMutex);

    if (!mSyncPending) {
    	//JAVA:    mSyncPending = mEventHandler.sendMessage(Message.obtain(null, EventHandler.SYNC));
    	AutoPtr<IMessage> messageT;
    	//CMessage::Obtain(NULL, WsEventHandler::SYNC,(IMessage**)&messageT);
    	mSyncPending = mEventHandler -> SendMessage(messageT.Get());		
    }
}

// Synchronize the native and java settings.
CARAPI_(void) WebSettings::NativeSync(
    /* [in] */ Int32 nativeFrame)
{
    
}

