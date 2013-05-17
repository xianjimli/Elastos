
#include "webkit/HTML5VideoViewProxy.h"
#include "webkit/WebViewCore.h"
#include "graphics/CBitmapFactory.h"
#include "widget/CVideoView.h"
#include "widget/CFrameLayout.h"
#include "widget/CFrameLayoutLayoutParams.h"
#include "widget/MediaController.h"
#include "webkit/CCookieManager.h"
#include "net/Uri.h"
#include "webkit/CWebView.h"

// Logging tag.
const CString HTML5VideoViewProxy::LOGTAG("HTML5VideoViewProxy");

// Message Ids for WebCore thread -> UI thread communication.
const Int32 HTML5VideoViewProxy::PLAY;
const Int32 HTML5VideoViewProxy::SEEK;
const Int32 HTML5VideoViewProxy::PAUSE;
const Int32 HTML5VideoViewProxy::ERROR;
const Int32 HTML5VideoViewProxy::LOAD_DEFAULT_POSTER;

// Message Ids to be handled on the WebCore thread
const Int32 HTML5VideoViewProxy::PREPARED;
const Int32 HTML5VideoViewProxy::ENDED;
const Int32 HTML5VideoViewProxy::POSTER_FETCHED;
const Int32 HTML5VideoViewProxy::PAUSED;

const CString HTML5VideoViewProxy::COOKIE("Cookie");

// Timer thread -> UI thread
const Int32 HTML5VideoViewProxy::TIMEUPDATE;

// A bunch event listeners for our VideoView
// MediaPlayer.OnPreparedListener
ECode HTML5VideoViewProxy::OnPrepared(
	/* [in] */ IMediaPlayer* mp)
{
	VALIDATE_NOT_NULL(mp);

	VideoPlayer::OnPrepared();

    Int32 dur, width, height;
    mp->GetDuration(&dur);
    mp->GetVideoWidth(&width);
    mp->GetVideoHeight(&height);

    Map<String, Int32>* map = new Map<String, Int32>();
    (*map)[String("dur")]    = dur;
    (*map)[String("width")]  = width;
    (*map)[String("height")] = height;

    ECode (STDCALL HTML5VideoViewProxy::*pHandlerFunc)(Map<String, Int32>*);

    pHandlerFunc = &HTML5VideoViewProxy::HandlePrepared;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr((IInterface*)map);

    WebCoreHandlerSendMessage(*(Handle32*)&pHandlerFunc, params);
}

// MediaPlayer.OnCompletionListener;
ECode HTML5VideoViewProxy::OnCompletion(
	/* [in] */ IMediaPlayer* mp)
{
	// The video ended by itself, so we need to
    // send a message to the UI thread to dismiss
    // the video view and to return to the WebView.
    // arg1 == 1 means the video ends by itself.

	ECode (STDCALL HTML5VideoViewProxy::*pHandlerFunc)(Int32);

	pHandlerFunc = &HTML5VideoViewProxy::HandleEnded;

	AutoPtr<IParcel> params;
	CCallbackParcel::New((IParcel**)&params);
	params->WriteInt32(1);

	SendMessage(*(Handle32*)&pHandlerFunc, params);
}

// MediaPlayer.OnErrorListener
ECode HTML5VideoViewProxy::OnError(
	/* [in] */ IMediaPlayer* mp, 
	/* [in] */ Int32 what, 
	/* [in] */ Int32 extra,
	/* [out] */ Boolean* result)
{
	ECode (STDCALL HTML5VideoViewProxy::*pHandlerFunc)();

	pHandlerFunc = &HTML5VideoViewProxy::HandleERROR;

	SendMessage(*(Handle32*)&pHandlerFunc, NULL);

	if (result) {
		*result = false;
	}
}

CARAPI_(void) HTML5VideoViewProxy::DispatchOnEnded()
{
	ECode (STDCALL HTML5VideoViewProxy::*pHandlerFunc)();

	pHandlerFunc = &HTML5VideoViewProxy::WebCoreHandleEnded;

	WebCoreHandlerSendMessage(*(Handle32*)&pHandlerFunc, NULL);
}

CARAPI_(void) HTML5VideoViewProxy::DispatchOnPaused()
{
    ECode (STDCALL HTML5VideoViewProxy::*pHandlerFunc)();

    pHandlerFunc = &HTML5VideoViewProxy::HandlePaused;

    WebCoreHandlerSendMessage(*(Handle32*)&pHandlerFunc, NULL);
}

CARAPI_(void) HTML5VideoViewProxy::OnTimeupdate()
{
	ECode (STDCALL HTML5VideoViewProxy::*pHandlerFunc)();

	pHandlerFunc = &HTML5VideoViewProxy::HandleTimeUpdate;

	SendMessage(*(Handle32*)&pHandlerFunc, NULL);
}

CARAPI_(AutoPtr<IContext>) HTML5VideoViewProxy::GetContext()
{
	AutoPtr<IContext> context;
	mWebView->GetContext((IContext**)&context);

	return context;
}

// The public methods below are all called from WebKit only.
/**
 * Play a video stream.
 * @param url is the URL of the video stream.
 */
CARAPI_(void) HTML5VideoViewProxy::Play(
	/* [in] */ CString url)
{
	if (url.GetLength() == 0) {
        return;
    }

    ECode (STDCALL HTML5VideoViewProxy::*pHandlerFunc)(String&);

    pHandlerFunc = &HTML5VideoViewProxy::HandlePlay;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteString((String)(const char*)url);

    SendMessage(*(Handle32*)&pHandlerFunc, params);
}

/**
 * Seek into the video stream.
 * @param  time is the position in the video stream.
 */
CARAPI_(void) HTML5VideoViewProxy::Seek(
	/* [in] */ Int32 time)
{
    ECode (STDCALL HTML5VideoViewProxy::*pHandlerFunc)(Int32);

    pHandlerFunc = &HTML5VideoViewProxy::HandleSeek;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32(time);

    SendMessage(*(Handle32*)&pHandlerFunc, params);
}

/**
 * Pause the playback.
 */
CARAPI_(void) HTML5VideoViewProxy::Pause()
{
	ECode (STDCALL HTML5VideoViewProxy::*pHandlerFunc)();

	SendMessage(*(Handle32*)&pHandlerFunc, NULL);
}

/**
 * Tear down this proxy object.
 */
CARAPI_(void) HTML5VideoViewProxy::Teardown()
{
	// This is called by the C++ MediaPlayerPrivate dtor.
    // Cancel any active poster download.
    if (mPosterDownloader != NULL) {
        mPosterDownloader->CancelAndReleaseQueue();
    }
    mNativePointer = 0;
}

/**
 * Load the poster image.
 * @param url is the URL of the poster image.
 */
CARAPI_(void) HTML5VideoViewProxy::LoadPoster(
	/* [in] */ CString url)
{
	if (url.GetLength() == 0) {

        ECode (STDCALL HTML5VideoViewProxy::*pHandlerFunc)();

        pHandlerFunc = &HTML5VideoViewProxy::HandleLoadDefaultPoster;

        SendMessage(*(Handle32*)&pHandlerFunc, NULL);

        return;
    }

    // Cancel any active poster download.
    if (mPosterDownloader != NULL) {
        mPosterDownloader->CancelAndReleaseQueue();
    }

    // Load the poster asynchronously
    mPosterDownloader = new PosterDownloader(url, this);
    mPosterDownloader->Start();
}

/**
 * The factory for HTML5VideoViewProxy instances.
 * @param webViewCore is the WebViewCore that is requesting the proxy.
 *
 * @return a new HTML5VideoViewProxy object.
 */
CARAPI_(HTML5VideoViewProxy*) HTML5VideoViewProxy::GetInstance(
	/* [in] */ WebViewCore* webViewCore, 
	/* [in] */ Int32 nativePtr)
{
	if (webViewCore == NULL) {
		return NULL;
	}

	return new HTML5VideoViewProxy(webViewCore->GetWebView(), nativePtr);
}

/* package */ 
CARAPI_(AutoPtr<IWebView>) HTML5VideoViewProxy::GetWebView()
{
	return mWebView;
}


CARAPI_(PInterface) HTML5VideoViewProxy::Probe(
    /* [in] */ REIID riid)
{}

CARAPI_(UInt32) HTML5VideoViewProxy::AddRef()
{}

CARAPI_(UInt32) HTML5VideoViewProxy::Release()
{}

CARAPI HTML5VideoViewProxy::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{}

CARAPI HTML5VideoViewProxy::Start(
    /* [in] */ ApartmentAttr attr)
{
	assert(0);
    return E_NOT_IMPLEMENTED;
}

CARAPI HTML5VideoViewProxy::Finish()
{
	assert(0);
    return E_NOT_IMPLEMENTED;
}

CARAPI HTML5VideoViewProxy::PostCppCallback(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id)
{
	assert(mApartment != NULL);
    return mApartment->PostCppCallback(target, func, params, id);
}

CARAPI HTML5VideoViewProxy::PostCppCallbackAtTime(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id,
    /* [in] */ Millisecond64 uptimeMillis)
{
	assert(mApartment != NULL);
    return mApartment->PostCppCallbackAtTime(target, func, params, id, uptimeMillis);
}

CARAPI HTML5VideoViewProxy::PostCppCallbackDelayed(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id,
    /* [in] */ Millisecond64 delayMillis)
{
	assert(mApartment != NULL);
    return mApartment->PostCppCallbackDelayed(target, func, params, id, delayMillis);
}

CARAPI HTML5VideoViewProxy::PostCppCallbackAtFrontOfQueue(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id)
{
	assert(mApartment != NULL);
    return mApartment->PostCppCallbackAtFrontOfQueue(target, func, params, id);
}

CARAPI HTML5VideoViewProxy::RemoveCppCallbacks(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func)
{
	assert(mApartment != NULL);
    return mApartment->RemoveCppCallbacks(target, func);
}

CARAPI HTML5VideoViewProxy::RemoveCppCallbacksEx(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ Int32 id)
{
	assert(mApartment != NULL);
    return mApartment->RemoveCppCallbacksEx(target, func, id);
}

CARAPI HTML5VideoViewProxy::HasCppCallbacks(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [out] */ Boolean* result)
{
	assert(mApartment != NULL);
    return mApartment->HasCppCallbacks(target, func, result);
}

CARAPI HTML5VideoViewProxy::HasCppCallbacksEx(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ Int32 id,
    /* [out] */ Boolean* result)
{
	assert(mApartment != NULL);
    return mApartment->HasCppCallbacksEx(target, func, id, result);
}

CARAPI HTML5VideoViewProxy::SendMessage(
    /* [in] */ Int32 message,
    /* [in] */ IParcel* params)
{}

/**
 * Private constructor.
 * @param webView is the WebView that hosts the video.
 * @param nativePtr is the C++ pointer to the MediaPlayerPrivate object.
 */
HTML5VideoViewProxy::HTML5VideoViewProxy(
	/* [in] */ IWebView* webView, 
	/* [in] */ Int32 nativePtr)
{
	assert(webView);

	// This handler is for the main (UI) thread.
//    super(Looper.getMainLooper());
    // Save the WebView object.
    mWebView = webView;
    // Save the native ptr
    mNativePointer = nativePtr;
    // create the message handler for this thread
    CreateWebCoreHandler();
}

// Everything below this comment executes on the WebCore thread, except for
// the EventHandler methods, which are called on the network thread.

// A helper class that knows how to download posters

HTML5VideoViewProxy::PosterDownloader::PosterDownloader(
	/* [in] */ CString url, 
	/* [in] */ HTML5VideoViewProxy* proxy)
{
	mUrl = url;
    mProxy = proxy;
//    mHandler = new Handler();
}

// Start the download. Called on WebCore thread.
CARAPI_(void) HTML5VideoViewProxy::PosterDownloader::Start()
{
	RetainQueue();
//	mRequestHandle = mRequestQueue.queueRequest(mUrl, "GET", null, this, null, 0);
}

// Cancel the download if active and release the queue. Called on WebCore thread.
CARAPI_(void) HTML5VideoViewProxy::PosterDownloader::CancelAndReleaseQueue()
{
#if 0
	if (mRequestHandle != NULL) {
        mRequestHandle.cancel();
        mRequestHandle = null;
    }
#endif
    ReleaseQueue();
}

// EventHandler methods. Executed on the network thread.
CARAPI_(void) HTML5VideoViewProxy::PosterDownloader::Status(
	/* [in] */ Int32 major_version,
	/* [in] */ Int32 minor_version,
	/* [in] */ Int32 code,
	/* [in] */ CString reason_phrase)
{
	mStatusCode = code;
}

CARAPI_(void) HTML5VideoViewProxy::PosterDownloader::Headers(
	/* [in] */ IHeaders* headers)
{
	mHeaders = headers;
}

CARAPI_(void) HTML5VideoViewProxy::PosterDownloader::Data(
	/* [in] */ ArrayOf<Byte>* data, 
	/* [in] */ Int32 len)
{
	assert(data != NULL);

	if (mPosterBytes == NULL) {
 		CByteArrayOutputStream::New((IByteArrayOutputStream**)&mPosterBytes);
    }
    mPosterBytes->WriteBufferEx(0, len, *data);
}

CARAPI_(void) HTML5VideoViewProxy::PosterDownloader::EndData()
{
	if (mStatusCode == 200) {
		Int32 size = 0;
		mPosterBytes->GetSize(&size);
        if (size > 0) {
        	AutoPtr<IBitmap> poster;
        	AutoPtr<IBitmapFactory> bitmapFactory;

        	CBitmapFactory::AcquireSingleton((IBitmapFactory**)&bitmapFactory);

        	ArrayOf<Byte>* byteArray = ArrayOf<Byte>::Alloc(size);

        	bitmapFactory->DecodeByteArrayEx(*byteArray, 0, size, (IBitmap**)&poster);

            mProxy->DoSetPoster(poster);
        }

        Cleanup();
    } else if (mStatusCode >= 300 && mStatusCode < 400) {
#if 0
        // We have a redirect.
        mUrl = mHeaders.getLocation();
        if (mUrl != null) {
            mHandler.post(new Runnable() {
               public void run() {
                   if (mRequestHandle != null) {
                       mRequestHandle.setupRedirect(mUrl, mStatusCode,
                               new HashMap<String, String>());
                   }
               }
            });
        }
#endif
    }
}

CARAPI_(void) HTML5VideoViewProxy::PosterDownloader::Certificate(
	/* [in] */ ISslCertificate* certificate)
{
	// Don't care.
}

CARAPI_(void) HTML5VideoViewProxy::PosterDownloader::Error(
	/* [in] */ Int32 id, 
	/* [in] */ CString description)
{
	Cleanup();
}

CARAPI_(Boolean) HTML5VideoViewProxy::PosterDownloader::HandleSslErrorRequest(
	/* [in] */ ISslError* error)
{
	// Don't care. If this happens, data() will never be called so
    // mPosterBytes will never be created, so no need to call cleanup.
    return FALSE;
}

// Tears down the poster bytes stream. Called on network thread.
CARAPI_(void) HTML5VideoViewProxy::PosterDownloader::Cleanup()
{
	if (mPosterBytes != NULL) {
     //   try {
            mPosterBytes->Close();
     //   } catch (IOException ignored) {
            // Ignored.
     //   } finally {
     //       mPosterBytes = null;
     //   }
    }
}

// Queue management methods. Called on WebCore thread.
CARAPI_(void) HTML5VideoViewProxy::PosterDownloader::RetainQueue()
{}

CARAPI_(void) HTML5VideoViewProxy::PosterDownloader::ReleaseQueue()
{}

/*******************************VideoPlayer******************************/ 

Boolean HTML5VideoViewProxy::VideoPlayer::isVideoSelfEnded;

// The proxy that is currently playing (if any).
HTML5VideoViewProxy* HTML5VideoViewProxy::VideoPlayer::mCurrentProxy;

// The VideoView instance. This is a singleton for now, at least until
// http://b/issue?id=1973663 is fixed.
//		static VideoView mVideoView;
// The progress view.
AutoPtr<IView> HTML5VideoViewProxy::VideoPlayer::mProgressView;

// The container for the progress view and video view
AutoPtr<IFrameLayout> HTML5VideoViewProxy::VideoPlayer::mLayout;

/*const*/ IWebChromeClientCustomViewCallback* HTML5VideoViewProxy::VideoPlayer::mCallback;

CARAPI_(void) HTML5VideoViewProxy::VideoPlayer::Play(
	/* [in] */ CString url, 
	/* [in] */ Int32 time, 
	/* [in] */ HTML5VideoViewProxy* proxy,
	/* [in] */ IWebChromeClient* client)
{
	assert(proxy != NULL);
	assert(client != NULL);

	if (mCurrentProxy == proxy) {
		Boolean bFlag = FALSE;
		((CVideoView*)mVideoView.Get())->IsPlaying(&bFlag);
        if (!bFlag) {
            ((CVideoView*)mVideoView.Get())->Start();
        }
        return;
    }

    if (mCurrentProxy != NULL) {
        // Some other video is already playing. Notify the caller that its playback ended.
        proxy->DispatchOnEnded();
        return;
    }

    mCurrentProxy = proxy;
    // Create a FrameLayout that will contain the VideoView and the
    // progress view (if any).
    CFrameLayout::New(proxy->GetContext(), (IFrameLayout**)&mLayout);

    AutoPtr<IFrameLayoutLayoutParams> layoutParams;
    CFrameLayoutLayoutParams::New(ViewGroupLayoutParams_WRAP_CONTENT,
    	ViewGroupLayoutParams_WRAP_CONTENT, Gravity_CENTER,
    	(IFrameLayoutLayoutParams**)&layoutParams);

    CVideoView::New(proxy->GetContext(), (IVideoView**)&mVideoView);
    mVideoView->SetWillNotDraw(FALSE);
//    mVideoView->SetMediaController(new MediaController(proxy->GetContext()));
    AutoPtr<ICookieManager> cookieManager;
    CCookieManager::AcquireSingleton((ICookieManager**)&cookieManager);
    String cookieValue;// = CookieManager.getInstance().getCookie(url);
    cookieManager->GetCookie(url, &cookieValue);
    //Map<String, String> headers = null;
    HashMap<String, String>* headers = NULL;
    if (cookieValue.GetLength() != 0) {
        headers = new HashMap<String, String>();
        (*headers)[String(COOKIE)] = cookieValue;
    }

    AutoPtr<IUri> uri;
    Uri::Parse((String)(const char*)url, (IUri**)&uri);
//    mVideoView->SetVideoURI(uri, headers);
    mVideoView->SetOnCompletionListener(proxy);
    mVideoView->SetOnPreparedListener(proxy);
    mVideoView->SetOnErrorListener(proxy);
    ((CVideoView*)mVideoView.Get())->SeekTo(time);
    mLayout->AddViewEx3(mVideoView, layoutParams);
//    mProgressView = client.getVideoLoadingProgressView();
    if (mProgressView != NULL) {
        mLayout->AddViewEx3(mProgressView, layoutParams);
        mProgressView->SetVisibility(View_VISIBLE);
    }
    mLayout->SetVisibility(View_VISIBLE);

//    mTimer = new Timer();
    ((CVideoView*)mVideoView.Get())->Start();
    client->OnShowCustomView(mLayout, mCallback);
    // Plugins like Flash will draw over the video so hide
    // them while we're playing.
    ((CWebView*)(mCurrentProxy->GetWebView().Get()))->GetViewManager()->HideAll();
}

CARAPI_(Boolean) HTML5VideoViewProxy::VideoPlayer::IsPlaying(
	/* [in] */ HTML5VideoViewProxy* proxy)
{
	Boolean bFlag = FALSE;
	((CVideoView*)mVideoView.Get())->IsPlaying(&bFlag);
	return (mCurrentProxy == proxy && mVideoView != NULL && bFlag);
}

CARAPI_(Int32) HTML5VideoViewProxy::VideoPlayer::GetCurrentPosition()
{
	Int32 currentPosMs = 0;
    if (mVideoView != NULL) {
        ((CVideoView*)mVideoView.Get())->GetCurrentPosition(&currentPosMs);
    }
    return currentPosMs;
}

CARAPI_(void) HTML5VideoViewProxy::VideoPlayer::Seek(
	/* [in] */ Int32 time, 
	/* [in] */ HTML5VideoViewProxy* proxy)
{
	if (mCurrentProxy == proxy && time >= 0 && mVideoView != NULL) {
        ((CVideoView*)mVideoView.Get())->SeekTo(time);
    }
}

CARAPI_(void) HTML5VideoViewProxy::VideoPlayer::Pause(
	/* [in] */ HTML5VideoViewProxy* proxy)
{
	if (mCurrentProxy == proxy && mVideoView != NULL) {
        ((CVideoView*)mVideoView.Get())->Pause();
//        mTimer.purge();
    }
}

CARAPI_(void) HTML5VideoViewProxy::VideoPlayer::OnPrepared()
{
	if (mProgressView == NULL || mLayout == NULL) {
        return;
    }
//    mTimer.schedule(new TimeupdateTask(mCurrentProxy), TIMEUPDATE_PERIOD, TIMEUPDATE_PERIOD);
    mProgressView->SetVisibility(View_GONE);
    ((CFrameLayout*)mLayout.Get())->RemoveView(mProgressView);
    mProgressView = NULL;
}

HTML5VideoViewProxy::VideoPlayer::TimeupdateTask::TimeupdateTask(
	/* [in] */ HTML5VideoViewProxy* proxy)
{
	mProxy = proxy;
}

CARAPI_(void) HTML5VideoViewProxy::VideoPlayer::TimeupdateTask::Run()
{
	mProxy->OnTimeupdate();
}


CARAPI_(void) HTML5VideoViewProxy::CreateWebCoreHandler()
{}

CARAPI_(void) HTML5VideoViewProxy::DoSetPoster(
	/* [in] */ IBitmap* poster)
{
	if (poster == NULL) {
        return;
    }
    // Save a ref to the bitmap and send it over to the WebCore thread.
    mPoster = poster;

    ECode (STDCALL HTML5VideoViewProxy::*pHandlerFunc)(IBitmap*);

    pHandlerFunc = &HTML5VideoViewProxy::HandlePosterFetched;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr(poster);
    WebCoreHandlerSendMessage(*(Handle32*)&pHandlerFunc, params);
}

CARAPI_(void) HTML5VideoViewProxy::SendTimeupdate()
{
    ECode (STDCALL HTML5VideoViewProxy::*pHandlerFunc)(Int32);

    pHandlerFunc = &HTML5VideoViewProxy::WebCoreHandleTimeUpdate;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32(VideoPlayer::GetCurrentPosition());

    WebCoreHandlerSendMessage(*(Handle32*)&pHandlerFunc, params);
}

CARAPI_(void) HTML5VideoViewProxy::NativeOnPrepared(
	/* [in] */ Int32 duration, 
	/* [in] */ Int32 width, 
	/* [in] */ Int32 height, 
	/* [in] */ Int32 nativePointer)
{}

CARAPI_(void) HTML5VideoViewProxy::NativeOnEnded(
	/* [in] */ Int32 nativePointer)
{}

CARAPI_(void) HTML5VideoViewProxy::NativeOnPaused(
	/* [in] */ Int32 nativePointer)
{}

CARAPI_(void) HTML5VideoViewProxy::NativeOnPosterFetched(
	/* [in] */ IBitmap* poster, 
	/* [in] */ Int32 nativePointer)
{}

CARAPI_(void) HTML5VideoViewProxy::NativeOnTimeupdate(
	/* [in] */ Int32 position, 
	/* [in] */ Int32 nativePointer)
{}


ECode HTML5VideoViewProxy::WebCoreHandlerSendMessage(
    /* [in] */ Handle32 pvFunc,
    /* [in] */ IParcel* params)
{
    return mWebCoreHandler->PostCppCallback((Handle32)this, pvFunc, params, 0);
}

ECode HTML5VideoViewProxy::WebCoreHandlerSendMessageAtTime(
    /* [in] */ Handle32 pvFunc,
    /* [in] */ IParcel* params,
    /* [in] */ Millisecond64 uptimeMillis)
{
    return mWebCoreHandler->PostCppCallbackAtTime(
        (Handle32)this, pvFunc, params, 0, uptimeMillis);
}

ECode HTML5VideoViewProxy::WebCoreHandlerRemoveMessage(
    /* [in] */ Handle32 func)
{
    return mWebCoreHandler->RemoveCppCallbacks((Handle32)this, func);
}

ECode HTML5VideoViewProxy::HandlePrepared(
	/* [in] */ Map<String, Int32>* map)
{
	VALIDATE_NOT_NULL(map);

    Int32 duration = (*map)[String("dur")];
    Int32 width    = (*map)[String("width")];
    Int32 height   = (*map)[String("height")];

    NativeOnPrepared(duration, width, height, mNativePointer);
}

ECode HTML5VideoViewProxy::WebCoreHandleEnded()
{
	NativeOnEnded(mNativePointer);
}

ECode HTML5VideoViewProxy::HandlePaused()
{
	NativeOnPaused(mNativePointer);
}

ECode HTML5VideoViewProxy::HandlePosterFetched(
	/* [in] */ IBitmap* poster)
{
	NativeOnPosterFetched(poster, mNativePointer);
}

ECode HTML5VideoViewProxy::WebCoreHandleTimeUpdate(
	/* [in] */ Int32 arg)
{
	NativeOnTimeupdate(arg, mNativePointer);
}


ECode HTML5VideoViewProxy::SendMessage(
    /* [in] */ Handle32 pvFunc,
    /* [in] */ IParcel* params)
{
    return mApartment->PostCppCallback((Handle32)this, pvFunc, params, 0);
}

ECode HTML5VideoViewProxy::SendMessageAtTime(
    /* [in] */ Handle32 pvFunc,
    /* [in] */ IParcel* params,
    /* [in] */ Millisecond64 uptimeMillis)
{
    return mApartment->PostCppCallbackAtTime(
        (Handle32)this, pvFunc, params, 0, uptimeMillis);
}

ECode HTML5VideoViewProxy::RemoveMessage(
    /* [in] */ Handle32 func)
{
    return mApartment->RemoveCppCallbacks((Handle32)this, func);
}

ECode HTML5VideoViewProxy::HandleEnded(
	/* [in] */ Int32 arg)
{
	if (arg == 1) {
		VideoPlayer::isVideoSelfEnded = TRUE;
	}
}

ECode HTML5VideoViewProxy::HandleERROR()
{
	AutoPtr<IWebChromeClient> client;
	mWebView->GetWebChromeClient((IWebChromeClient**)&client);
    if (client != NULL) {
        client->OnHideCustomView();
    }
}

ECode HTML5VideoViewProxy::HandleTimeUpdate()
{
	if (VideoPlayer::IsPlaying(this)) {
        SendTimeupdate();
    }
}

ECode HTML5VideoViewProxy::HandlePlay(
	/* [in] */ String& url)
{
    AutoPtr<IWebChromeClient> client;
    mWebView->GetWebChromeClient((IWebChromeClient**)&client);
    if (client != NULL) {
        VideoPlayer::Play(url, mSeekPosition, this, client);
    }
}

ECode HTML5VideoViewProxy::HandleSeek(
	/* [in] */ Int32 time)
{
    mSeekPosition = time;
    VideoPlayer::Seek(mSeekPosition, this);
}

ECode HTML5VideoViewProxy::HandlePause()
{
	VideoPlayer::Pause(this);
}

ECode HTML5VideoViewProxy::HandleLoadDefaultPoster()
{
	AutoPtr<IWebChromeClient> client;
	mWebView->GetWebChromeClient((IWebChromeClient**)&client);
    if (client != NULL) {
    	AutoPtr<IBitmap> bitmap;
    	client->GetDefaultVideoPoster((IBitmap**)&bitmap);
        DoSetPoster(bitmap);
    }
}