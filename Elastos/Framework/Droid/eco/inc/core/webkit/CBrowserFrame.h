
#ifndef __CBROWSERFRAME_H__
#define __CBROWSERFRAME_H__

#include "_CBrowserFrame.h"
#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/Vector.h>

#include "WebSettings.h"
#include "WebViewCore.h"

class JWebCoreJavaBridge;
class LoadListener;
class WebViewCore;
class WebBackForwardList;

CarClass(CBrowserFrame)
{
public:
    CARAPI LoadUrl(
        /* [in] */ const String& url,
        /* [in] */ IObjectStringMap * extraHeaders);

    CARAPI PostUrl(
        /* [in] */ const String& url,
        /* [in] */ const ArrayOf<Byte>& data);

    CARAPI LoadData(
        /* [in] */ const String& baseUrl,
        /* [in] */ const String& data,
        /* [in] */ const String& mimeType,
        /* [in] */ const String& encoding,
        /* [in] */ const String& historyUrl);

    CARAPI GoBackOrForward(
        /* [in] */ Int32 steps);

    CARAPI Destroy();

    CARAPI HandleMessage(
        /* [in] */ IMessage* msg);

    CARAPI ExternalRepresentation(
        /* [in] */ IMessage* callBack);

    CARAPI DocumentAsText(
        /* [in] */ IMessage* callBack);

    CARAPI HandleUrl(
        /* [in] */ const String& url,
        /* [out] */ Boolean* flag);

    CARAPI AddJavascriptInterface(
        /* [in] */ IInterface* obj,
        /* [in] */ const String& interfaceName);

    CARAPI NativeDestroyFrame();

    CARAPI Reload(
        /* [in] */ Boolean allowStale);

    CARAPI StringByEvaluatingJavaScriptFromString(
        /* [in] */ const String& script,
        /* [out] */ String* scriptString);

    CARAPI CacheDisabled(
        /* [out] */ Boolean* flag);

    CARAPI ClearCache();

    CARAPI StopLoading();

    CARAPI DocumentHasImages(
        /* [out] */ Boolean* flag);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Handle32 webViewCore,
        /* [in] */ ICallbackProxy* proxy,
        /* [in] */ Handle32 settings,
        /* [in] */ IObjectStringMap* javascriptInterfaces);

public:
    // message ids
    // a message posted when a frame loading is completed
    static const Int32 FRAME_COMPLETED = 1001;
    // orientation change message
    static const Int32 ORIENTATION_CHANGED = 1002;
    // a message posted when the user decides the policy
    static const Int32 POLICY_FUNCTION = 1003;

    // Note: need to keep these in sync with FrameLoaderTypes.h in native
    static const Int32 FRAME_LOADTYPE_STANDARD = 0;
    static const Int32 FRAME_LOADTYPE_BACK = 1;
    static const Int32 FRAME_LOADTYPE_FORWARD = 2;
    static const Int32 FRAME_LOADTYPE_INDEXEDBACKFORWARD = 3;
    static const Int32 FRAME_LOADTYPE_RELOAD = 4;
    static const Int32 FRAME_LOADTYPE_RELOADALLOWINGSTALEDATA = 5;
    static const Int32 FRAME_LOADTYPE_SAME = 6;
    static const Int32 FRAME_LOADTYPE_REDIRECT = 7;
    static const Int32 FRAME_LOADTYPE_REPLACE = 8;

    // XXX: Must match PolicyAction in FrameLoaderTypes.h in webcore
    static const Int32 POLICY_USE = 0;
    static const Int32 POLICY_IGNORE = 2;

public:
    /* package */
    CARAPI_(Boolean) Committed();

    /* package */
    CARAPI_(Boolean) FirstLayoutDone();

    /* package */
    CARAPI_(Int32) LoadType();

    /* package */
    CARAPI_(void) DidFirstLayout();

    /**
     * We have received an SSL certificate for the main top-level page.
     *
     * !!!Called from the network thread!!!
     */
    CARAPI_(void) Certificate(
        /* [in] */ ISslCertificate* certificate);

    /**
     * Get the CallbackProxy for sending messages to the UI thread.
     */
    /* package */
    CARAPI_(AutoPtr<ICallbackProxy>) GetCallbackProxy() const;

    /**
     * Returns the User Agent used by this frame
     */
    CARAPI_(void) GetUserAgentString(
        /* [out] */ String& str) const;

    CARAPI_(void) GetRawResFilename(
        /* [in] */ Int32 id,
        /* [out] */ String& filename) const;

private:
    class ConfigCallback : public IComponentCallbacks
    {
    public:
        ConfigCallback(
            /* [in] */ ILocalWindowManager* wm);

        CARAPI_(void) AddHandler(
            /* [in] */ IHandler* h);

        CARAPI OnConfigurationChanged(
            /* [in] */ IConfiguration* newConfig);

        CARAPI OnLowMemory();

    private:
        Vector<AutoPtr<IHandler> > mHandlers;
        AutoPtr<ILocalWindowManager> mWindowManager;
    };

public:
    // This is a field accessed by native code as well as package classes.
    /*package*/
    Int32 mNativeFrame;
    /* package */
    Boolean mLoadInitFromJava;

    // Static instance of a JWebCoreJavaBridge to handle timer and cookie
    // requests from WebCore.
    static AutoPtr<JWebCoreJavaBridge> sJavaBridge;

    static AutoPtr<ConfigCallback> sConfigCallback;

private:
    static const CString LOGTAG;// = "webkit";

    /**
     * Cap the number of LoadListeners that will be instantiated, so
     * we don't blow the GREF count.  Attempting to queue more than
     * this many requests will prompt an error() callback on the
     * request's LoadListener
     */
    const static Int32 MAX_OUTSTANDING_REQUESTS = 300;

    // These ids need to be in sync with enum rawResId in PlatformBridge.h
    static const Int32 NODOMAIN = 1;
    static const Int32 LOADERROR = 2;
    static const Int32 DRAWABLEDIR = 3;
    static const Int32 FILE_UPLOAD_LABEL = 4;
    static const Int32 RESET_LABEL = 5;
    static const Int32 SUBMIT_LABEL = 6;

    // A progress threshold to switch from history Picture to live Picture
    static const Int32 TRANSITION_SWITCH_THRESHOLD = 75;

private:
    /**
     * native callback
     * Report an error to an activity.
     * @param errorCode The HTTP error code.
     * @param description A String description.
     * TODO: Report all errors including resource errors but include some kind
     * of domain identifier. Change errorCode to an enum for a cleaner
     * interface.
     */
    CARAPI_(void) ReportError(
        /* [in] */ const Int32 errorCode,
        /* [in] */ const String description,
        /* [in] */ const String failingUrl);

    CARAPI_(void) ResetLoadingStates();

    /**
     * native callback
     * Indicates the beginning of a new load.
     * This method will be called once for the main frame.
     */
    CARAPI_(void) LoadStarted(
        /* [in] */ const String& url,
        /* [in] */ IBitmap* favicon,
        /* [in] */ Int32 loadType,
        /* [in] */ Boolean isMainFrame);

    /**
     * native callback
     * Indicates the WebKit has committed to the new load
     */
    CARAPI_(void) TransitionToCommitted(
        /* [in] */ Int32 loadType,
        /* [in] */ Boolean isMainFrame);

    /**
     * native callback
     * <p>
     * Indicates the end of a new load.
     * This method will be called once for the main frame.
     */
    CARAPI_(void) LoadFinished(
        /* [in] */ const String& url,
        /* [in] */ Int32 loadType,
        /* [in] */ Boolean isMainFrame);

    /**
     * Punch-through for WebCore to set the document
     * title. Inform the Activity of the new title.
     * @param title The new title of the document.
     */
    CARAPI_(void) SetTitle(
        /* [in] */ const String& title);

    /**
     * Return the render tree as a string
     */
    CARAPI_(void) ExternalRepresentation(
        /* [out] */ String& str);

    /**
     * Return the text drawn on the screen as a string
     */
    CARAPI_(void) DocumentAsText(
        /* [out] */ String& str);

    /*
     * This method is called by WebCore to inform the frame that
     * the Javascript window object has been cleared.
     * We should re-attach any attached js interfaces.
     */
    CARAPI_(void) WindowObjectCleared(
        /* [in] */ Int32 nativeFramePointer);

    /**
     * Called by JNI.  Given a URI, find the associated file and return its size
     * @param uri A String representing the URI of the desired file.
     * @return int The size of the given file.
     */
    CARAPI_(Int32) GetFileSize(
        /* [in] */ const String& uri) const;

    /**
     * Called by JNI.  Given a URI, a buffer, and an offset into the buffer,
     * copy the resource into buffer.
     * @param uri A String representing the URI of the desired file.
     * @param buffer The byte array to copy the data into.
     * @param offset The offet into buffer to place the data.
     * @param expectedSize The size that the buffer has allocated for this file.
     * @return int The size of the given file, or zero if it fails.
     */
    CARAPI_(Int32) GetFile(
        /* [in] */ const String& uri,
        /* [in] */ Byte buffer[],
        /* [in] */ Int32 offset,
        /* [in] */ Int32 expectedSize) const;

    /**
     * Start loading a resource.
     * @param loaderHandle The native ResourceLoader that is the target of the
     *                     data.
     * @param url The url to load.
     * @param method The http method.
     * @param headers The http headers.
     * @param postData If the method is "POST" postData is sent as the request
     *                 body. Is null when empty.
     * @param postDataIdentifier If the post data contained form this is the form identifier, otherwise it is 0.
     * @param cacheMode The cache mode to use when loading this resource. See WebSettings.setCacheMode
     * @param mainResource True if the this resource is the main request, not a supporting resource
     * @param userGesture
     * @param synchronous True if the load is synchronous.
     * @return A newly created LoadListener object.
     */
    CARAPI_(AutoPtr<LoadListener>) StartLoadingResource(
        /* [in] */ Int32 loaderHandle,
        /* [in] */ const String& url,
        /* [in] */ const String& method,
        /* [in] */ IHashMap* headers,
        /* [in] */ Byte postData[],
        /* [in] */ Int64 postDataIdentifier,
        /* [in] */ Int32 cacheMode,
        /* [in] */ Boolean mainResource,
        /* [in] */ Boolean userGesture,
        /* [in] */ Boolean synchronous,
        /* [in] */ const String& username,
        /* [in] */ const String& password);

    /**
     * Set the progress for the browser activity.  Called by native code.
     * Uses a delay so it does not happen too often.
     * @param newProgress An int between zero and one hundred representing
     *                    the current progress percentage of loading the page.
     */
    CARAPI_(void) SetProgress(
        /* [in] */ Int32 newProgress);

    /**
     * Send the icon to the activity for display.
     * @param icon A Bitmap representing a page's favicon.
     */
    CARAPI_(void) DidReceiveIcon(
        /* [in] */ IBitmap* icon);

    // Called by JNI when an apple-touch-icon attribute was found.
    CARAPI_(void) DidReceiveTouchIconUrl(
        /* [in] */ const String& url,
        /* [in] */ Boolean precomposed);

    /**
     * Request a new window from the client.
     * @return The BrowserFrame object stored in the new WebView.
     */
    CARAPI_(AutoPtr<IBrowserFrame>) CreateWindow(
        /* [in] */ Boolean dialog,
        /* [in] */ Boolean userGesture);

    /**
     * Try to focus this WebView.
     */
    CARAPI_(void) RequestFocus();

    /**
     * Close this frame and window.
     */
    CARAPI_(void) CloseWindow(
        /* [in] */ WebViewCore* w);

    CARAPI_(void) DecidePolicyForFormResubmission(
        /* [in] */ Int32 policyFunction);

    /**
     * Tell the activity to update its global history.
     */
    CARAPI_(void) UpdateVisitedHistory(
        /* [in] */ const String& url,
        /* [in] */ Boolean isReload);

    CARAPI_(Float) Density();

    //==========================================================================
    // native functions
    //==========================================================================

    /**
     * Create a new native frame for a given WebView
     * @param w     A WebView that the frame draws into.
     * @param am    AssetManager to use to get assets.
     * @param list  The native side will add and remove items from this list as
     *              the native list changes.
     */
    CARAPI_(void) NativeCreateFrame(
        /* [in] */ WebViewCore* w,
        /* [in] */ IAssetManager* am,
        /* [in] */ IWebBackForwardList* list)
    {

    }

    CARAPI_(void) NativeCallPolicyFunction(
        /* [in] */ Int32 policyFunction,
        /* [in] */ Int32 decision)
    {

    }

    /**
     * Go back or forward the number of steps given.
     * @param steps A negative or positive number indicating the direction
     *              and number of steps to move.
     */
    CARAPI_(void) NativeGoBackOrForward(
        /* [in] */ Int32 steps)
    {

    }

    /**
     * Add a javascript interface to the main frame.
     */
    CARAPI_(void) NativeAddJavascriptInterface(
        /* [in] */ Int32 nativeFramePointer,
        /* [in] */ IInterface* obj,
        /* [in] */ const String& interfaceName)
    {

    }

    /**
     * Enable or disable the native cache.
     */
    /* FIXME: The native cache is always on for now until we have a better
     * solution for our 2 caches. */
    CARAPI_(void) SetCacheDisabled(
        /* [in] */ Boolean disabled)
    {

    }

    /**
     * Returns false if the url is bad.
     */
    CARAPI_(void) NativeLoadUrl(
        /* [in] */ CString url,
        /* [in] */ IObjectStringMap* headers)
    {

    }

    CARAPI_(void) NativePostUrl(
        /* [in] */ CString url,
        /* [in] */ const ArrayOf<Byte>& postData)
    {

    }

    CARAPI_(void) NativeLoadData(
        /* [in] */ CString baseUrl,
        /* [in] */ CString data,
        /* [in] */ CString mimeType,
        /* [in] */ CString encoding,
        /* [in] */ CString historyUrl)
    {

    }


    CARAPI_(void) NativeStopLoading()
    {

    }

    /**
     * @return TRUE if there is a password field in the current frame
     */
    CARAPI_(Boolean) HasPasswordField()
    {

    }

    /**
     * Get username and password in the current frame. If found, String[0] is
     * username and String[1] is password. Otherwise return NULL.
     * @return String[]
     */
    CARAPI_(void) GetUsernamePassword(
        /* [in] */ String& str)
    {

    }

    /**
     * Set username and password to the proper fields in the current frame
     * @param username
     * @param password
     */
    CARAPI_(void) SetUsernamePassword(
        /* [in] */ CString username,
        /* [in] */ CString password)
    {

    }

    /**
     * Get form's "text" type data associated with the current frame.
     * @return HashMap If succeed, returns a list of name/value pair. Otherwise
     *         returns null.
     */
    CARAPI_(AutoPtr<IHashMap>) GetFormTextData() const;

    CARAPI_(void) NativeOrientationChanged(
        /* [in] */ Int32 orientation)
    {

    }

private:
    AutoPtr<ICallbackProxy> mCallbackProxy;
    AutoPtr<WebSettings> mSettings;
    AutoPtr<IContext> mContext;
    AutoPtr<IWebViewDatabase> mDatabase;
    AutoPtr<WebViewCore> mWebViewCore;

    Int32 mLoadType;
    Boolean mFirstLayoutDone;
    Boolean mCommitted;
    // Flag for blocking messages. This is used during destroy() so
    // that if the UI thread posts any messages after the message
    // queue has been cleared,they are ignored.
    Boolean mBlockMessages;
    Int32 mOrientation;

    // Is this frame the main frame?
    Boolean mIsMainFrame;

    // Attached Javascript interfaces
    AutoPtr<IObjectStringMap> mJSInterfaceMap;
};

#endif // __CBROWSERFRAME_H__
