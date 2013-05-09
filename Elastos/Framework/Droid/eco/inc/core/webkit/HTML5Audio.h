#ifndef __HTML5AUDIO_H__
#define __HTML5AUDIO_H__

#include "ext/frameworkext.h"

#include <utils/Timers.h>
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>

class MediaPlayer;

/**
 * <p>HTML5 support class for Audio.
 */
class HTML5Audio : public ElRefBase,
                   public IApartment,
                   public IMediaPlayerOnBufferingUpdateListener,
                   public IMediaPlayerOnCompletionListener,
                   public IMediaPlayerOnErrorListener,
                   public IMediaPlayerOnPreparedListener,
                   public IMediaPlayerOnSeekCompleteListener
{
public:

    /**
     * @param nativePtr is the C++ pointer to the MediaPlayerPrivate object.
     */
	HTML5Audio(
		/* [in] */ Int32 nativePtr);

public:
    //@Override
	//virtual CARAPI_(void) HandleMessage(
	//	/* [in] */ IMessage* msg);

    // event listeners for MediaPlayer
    // Those are called from the same thread we created the MediaPlayer
    // (i.e. the webviewcore thread here)

    // MediaPlayer.OnBufferingUpdateListener
	CARAPI OnBufferingUpdate(
		/* [in] */ IMediaPlayer* mp, 
		/* [in] */ Int32 percent);

    // MediaPlayer.OnCompletionListener;
	CARAPI OnCompletion(
		/* [in] */ IMediaPlayer* mp);

    // MediaPlayer.OnErrorListener
	CARAPI OnError(
		/* [in] */ IMediaPlayer* mp, 
		/* [in] */ Int32 what, 
		/* [in] */ Int32 extra,
		/* [out] */ Boolean* result);

    // MediaPlayer.OnPreparedListener
	CARAPI OnPrepared(
		/* [in] */ IMediaPlayer* mp);

    // MediaPlayer.OnSeekCompleteListener
	CARAPI OnSeekComplete(
		/* [in] */ IMediaPlayer* mp);

public:
	CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

public:
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

private:
	class TimeupdateTask // : public TimerTask 
	{
	public:
		virtual CARAPI_(void) Run();
    };

	CARAPI_(void) ResetMediaPlayer();

	CARAPI_(void) SetDataSource(
		/* [in] */ CString url);

	CARAPI_(void) Play();

	CARAPI_(void) Pause();

	CARAPI_(void) Seek(
		/* [in] */ Int32 msec);

	CARAPI_(void) Teardown();

	CARAPI_(Float) GetMaxTimeSeekable();

	CARAPI_(void) NativeOnBuffering(
		/* [in] */ Int32 percent, 
		/* [in] */ Int32 nativePointer);

	CARAPI_(void) NativeOnEnded(
		/* [in] */ Int32 nativePointer);

	CARAPI_(void) NativeOnPrepared(
		/* [in] */ Int32 duration, 
		/* [in] */ Int32 width, 
		/* [in] */ Int32 height, 
		/* [in] */ Int32 nativePointer);

	CARAPI_(void) NativeOnTimeupdate(
		/* [in] */ Int32 position, 
		/* [in] */ Int32 nativePointer);

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

     CARAPI HandleTimeUpdate();

private:
    // Logging tag.
	static const CString LOGTAG;// = "HTML5Audio";

	AutoPtr<IMediaPlayer> mMediaPlayer;

    // The C++ MediaPlayerPrivateAndroid object.
	Int32 mNativePointer;

	static Int32 IDLE;
	static Int32 INITIALIZED;
	static Int32 PREPARED;
	static Int32 STARTED;
	static Int32 COMPLETE;
	static Int32 PAUSED;
	static Int32 STOPPED;
	static Int32 ERROR;

	Int32 mState;

	CString mUrl;
	Boolean mAskToPlay;

    // Timer thread -> UI thread
	static const Int32 TIMEUPDATE = 100;

    // The spec says the timer should fire every 250 ms or less.
	static const Int32 TIMEUPDATE_PERIOD = 250;  // ms
    // The timer for timeupate events.
    // See http://www.whatwg.org/specs/web-apps/current-work/#event-media-timeupdate
//	Timer mTimer;

	AutoPtr<IApartment> mApartment;
};

#endif //__HTML5AUDIO_H__