#ifndef __HTML5VIDEOVIEWPROXY_H__
#define __HTML5VIDEOVIEWPROXY_H__

#include "ext/frameworkext.h"
#include "media/media/CMediaPlayer.h"

#include <elastos/AutoPtr.h>
#include <elastos/Map.h>

class MediaPlayer;
class WebViewCore;

/**
 * <p>Proxy for HTML5 video views.
 */
class HTML5VideoViewProxy : public IApartment,
                            public IMediaPlayerOnPreparedListener,
                            public IMediaPlayerOnCompletionListener,
                            public IMediaPlayerOnErrorListener
                         //   public MediaPlayer.OnPreparedListener,
                         //   public MediaPlayer.OnCompletionListener,
                         //   public MediaPlayer.OnErrorListener 
{

public:
    // A bunch event listeners for our VideoView
    // MediaPlayer.OnPreparedListener
	CARAPI OnPrepared(
		/* [in] */ IMediaPlayer* mp);

    // MediaPlayer.OnCompletionListener;
	CARAPI OnCompletion(
		/* [in] */ IMediaPlayer* mp);

    // MediaPlayer.OnErrorListener
	CARAPI OnError(
		/* [in] */ IMediaPlayer* mp, 
		/* [in] */ Int32 what, 
		/* [in] */ Int32 extra,
		/* [out] */ Boolean* result);

	virtual CARAPI_(void) DispatchOnEnded();

	virtual CARAPI_(void) DispatchOnPaused();

	virtual CARAPI_(void) OnTimeupdate();

	virtual CARAPI_(AutoPtr<IContext>) GetContext();

    // The public methods below are all called from WebKit only.
    /**
     * Play a video stream.
     * @param url is the URL of the video stream.
     */
	virtual CARAPI_(void) Play(
		/* [in] */ CString url);

    /**
     * Seek into the video stream.
     * @param  time is the position in the video stream.
     */
	virtual CARAPI_(void) Seek(
		/* [in] */ Int32 time);

    /**
     * Pause the playback.
     */
	virtual CARAPI_(void) Pause();

    /**
     * Tear down this proxy object.
     */
	virtual CARAPI_(void) Teardown();

    /**
     * Load the poster image.
     * @param url is the URL of the poster image.
     */
	virtual CARAPI_(void) LoadPoster(
		/* [in] */ CString url);

    /**
     * The factory for HTML5VideoViewProxy instances.
     * @param webViewCore is the WebViewCore that is requesting the proxy.
     *
     * @return a new HTML5VideoViewProxy object.
     */
	static CARAPI_(HTML5VideoViewProxy*) GetInstance(
		/* [in] */ WebViewCore* webViewCore, 
		/* [in] */ Int32 nativePtr);

    /* package */ 
	virtual CARAPI_(AutoPtr<IWebView>) GetWebView();

public:

	CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

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
	// The C++ MediaPlayerPrivateAndroid object.
    Int32 mNativePointer;

private:
	/**
     * Private constructor.
     * @param webView is the WebView that hosts the video.
     * @param nativePtr is the C++ pointer to the MediaPlayerPrivate object.
     */
	HTML5VideoViewProxy(
		/* [in] */ IWebView* webView, 
		/* [in] */ Int32 nativePtr);

private:

	// Everything below this comment executes on the WebCore thread, except for
    // the EventHandler methods, which are called on the network thread.

    // A helper class that knows how to download posters
	class PosterDownloader //: public EventHandler 
	{
	public:
		PosterDownloader(
			/* [in] */ CString url, 
			/* [in] */ HTML5VideoViewProxy* proxy);

        // Start the download. Called on WebCore thread.
		virtual CARAPI_(void) Start();

        // Cancel the download if active and release the queue. Called on WebCore thread.
		virtual CARAPI_(void) CancelAndReleaseQueue();

        // EventHandler methods. Executed on the network thread.
		virtual CARAPI_(void) Status(
			/* [in] */ Int32 major_version,
			/* [in] */ Int32 minor_version,
			/* [in] */ Int32 code,
			/* [in] */ CString reason_phrase);

		virtual CARAPI_(void) Headers(
			/* [in] */ IHeaders* headers);

		virtual CARAPI_(void) Data(
			/* [in] */ ArrayOf<Byte>* data, 
			/* [in] */ Int32 len);

		virtual CARAPI_(void) EndData();

		virtual CARAPI_(void) Certificate(
			/* [in] */ ISslCertificate* certificate);

		virtual CARAPI_(void) Error(
			/* [in] */ Int32 id, 
			/* [in] */ CString description);

		virtual CARAPI_(Boolean) HandleSslErrorRequest(
			/* [in] */ ISslError* error);

	private:
        // Tears down the poster bytes stream. Called on network thread.
		CARAPI_(void) Cleanup();

        // Queue management methods. Called on WebCore thread.
		CARAPI_(void) RetainQueue();

		CARAPI_(void) ReleaseQueue();

	private:
        // The request queue. This is static as we have one queue for all posters.
//		static RequestQueue mRequestQueue;
		static Int32 mQueueRefCount;
        // The poster URL
		CString mUrl;
        // The proxy we're doing this for.
		/*const*/ HTML5VideoViewProxy* mProxy;
        // The poster bytes. We only touch this on the network thread.
		AutoPtr<IByteArrayOutputStream> mPosterBytes;
        // The request handle. We only touch this on the WebCore thread.
//		RequestHandle mRequestHandle;
        // The response status code.
		Int32 mStatusCode;
        // The response headers.
		AutoPtr<IHeaders> mHeaders;
        
        // The handler to handle messages on the WebCore thread.
		//IHandler* mHandler;
		AutoPtr<IApartment> mHandler;
    };

	class VideoPlayer 
	{
	public:
		static Boolean isVideoSelfEnded;

		static CARAPI_(void) Play(
			/* [in] */ CString url, 
			/* [in] */ Int32 time, 
			/* [in] */ HTML5VideoViewProxy* proxy,
			/* [in] */ IWebChromeClient* client);

		static CARAPI_(Boolean) IsPlaying(
			/* [in] */ HTML5VideoViewProxy* proxy);

		static CARAPI_(Int32) GetCurrentPosition();

		static CARAPI_(void) Seek(
			/* [in] */ Int32 time, 
			/* [in] */ HTML5VideoViewProxy* proxy);

		static CARAPI_(void) Pause(
			/* [in] */ HTML5VideoViewProxy* proxy);

		static CARAPI_(void) OnPrepared();

	private:
        // The proxy that is currently playing (if any).
		static HTML5VideoViewProxy* mCurrentProxy;
        // The VideoView instance. This is a singleton for now, at least until
        // http://b/issue?id=1973663 is fixed.
		static AutoPtr<IVideoView> mVideoView;
        // The progress view.
		static AutoPtr<IView> mProgressView;
        // The container for the progress view and video view
		static AutoPtr<IFrameLayout> mLayout;
        // The timer for timeupate events.
        // See http://www.whatwg.org/specs/web-apps/current-work/#event-media-timeupdate
//		static Timer mTimer;

	private:
		class TimeupdateTask //: public TimerTask 
		{
		public:
			TimeupdateTask(
				/* [in] */ HTML5VideoViewProxy* proxy);

			virtual CARAPI_(void) Run();

		private:
			HTML5VideoViewProxy* mProxy;
        };

        // The spec says the timer should fire every 250 ms or less.
		static const Int32 TIMEUPDATE_PERIOD = 250;  // ms

		static /*const*/ IWebChromeClientCustomViewCallback* mCallback;
    };

private:
	CARAPI SendMessage(
        /* [in] */ Handle32 pvFunc,
        /* [in] */ IParcel* params);

    CARAPI SendMessageAtTime(
        /* [in] */ Handle32 pvFunc,
        /* [in] */ IParcel* params,
        /* [in] */ Millisecond64 uptimeMillis);

    CARAPI RemoveMessage(
        /* [in] */ Handle32 func);

    CARAPI HandleEnded(
		/* [in] */ Int32 arg);

	CARAPI HandleERROR();

	CARAPI HandleTimeUpdate();

	CARAPI HandlePlay(
		/* [in] */ String& url);

	CARAPI HandleSeek(
		/* [in] */ Int32 time);

	CARAPI HandlePause();

	CARAPI HandleLoadDefaultPoster();

private:

	CARAPI_(void) CreateWebCoreHandler();

	CARAPI_(void) DoSetPoster(
		/* [in] */ IBitmap* poster);

	CARAPI_(void) SendTimeupdate();

	CARAPI_(void) NativeOnPrepared(
		/* [in] */ Int32 duration, 
		/* [in] */ Int32 width, 
		/* [in] */ Int32 height, 
		/* [in] */ Int32 nativePointer);

	CARAPI_(void) NativeOnEnded(
		/* [in] */ Int32 nativePointer);

	CARAPI_(void) NativeOnPaused(
		/* [in] */ Int32 nativePointer);

	CARAPI_(void) NativeOnPosterFetched(
		/* [in] */ IBitmap* poster, 
		/* [in] */ Int32 nativePointer);

	CARAPI_(void) NativeOnTimeupdate(
		/* [in] */ Int32 position, 
		/* [in] */ Int32 nativePointer);

private:
	CARAPI WebCoreHandlerSendMessage(
        /* [in] */ Handle32 pvFunc,
        /* [in] */ IParcel* params);

    CARAPI WebCoreHandlerSendMessageAtTime(
        /* [in] */ Handle32 pvFunc,
        /* [in] */ IParcel* params,
        /* [in] */ Millisecond64 uptimeMillis);

    CARAPI WebCoreHandlerRemoveMessage(
        /* [in] */ Handle32 func);

    CARAPI HandlePrepared(
    	/* [in] */ Map<String, Int32>* map);

    CARAPI WebCoreHandleEnded();

    CARAPI HandlePaused();

    CARAPI HandlePosterFetched(
		/* [in] */ IBitmap* poster);

	CARAPI WebCoreHandleTimeUpdate(
		/* [in] */ Int32 arg);

private:
	// Logging tag.
    static const CString LOGTAG;// = "HTML5VideoViewProxy";

    // Message Ids for WebCore thread -> UI thread communication.
	static const Int32 PLAY                = 100;
	static const Int32 SEEK                = 101;
	static const Int32 PAUSE               = 102;
	static const Int32 ERROR               = 103;
	static const Int32 LOAD_DEFAULT_POSTER = 104;

    // Message Ids to be handled on the WebCore thread
	static const Int32 PREPARED          = 200;
	static const Int32 ENDED             = 201;
	static const Int32 POSTER_FETCHED    = 202;
	static const Int32 PAUSED            = 203;

	static const CString COOKIE;// = "Cookie";

    // Timer thread -> UI thread
	static const Int32 TIMEUPDATE = 300;

private:
    // The handler for WebCore thread messages;
	//IHandler* mWebCoreHandler;
	AutoPtr<IApartment> mWebCoreHandler;

    // The WebView instance that created this view.
	AutoPtr<IWebView> mWebView;

    // The poster image to be shown when the video is not playing.
    // This ref prevents the bitmap from being GC'ed.
	AutoPtr<IBitmap> mPoster;

    // The poster downloader.
	PosterDownloader* mPosterDownloader;
    // The seek position.
	Int32 mSeekPosition;
    // A helper class to control the playback. This executes on the UI thread!

    AutoPtr<IApartment> mApartment;
};

#endif //__HTML5VIDEOVIEWPROXY_H__