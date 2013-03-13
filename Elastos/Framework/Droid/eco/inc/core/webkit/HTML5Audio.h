#ifndef __HTML5AUDIO_H__
#define __HTML5AUDIO_H__

#include "ext/frameworkext.h"

#include <utils/Timers.h>

class MediaPlayer;

/**
 * <p>HTML5 support class for Audio.
 */
class HTML5Audio //: public IHandler,
                 //         IOnBufferingUpdateListener,
                 //         IOnCompletionListener,
                 //         IOnErrorListener,
                 //         IOnPreparedListener,
                 //         IOnSeekCompleteListener
{
public:

    /**
     * @param nativePtr is the C++ pointer to the MediaPlayerPrivate object.
     */
	HTML5Audio(
		/* [in] */ Int32 nativePtr);

public:
    //@Override
	virtual CARAPI_(void) HandleMessage(
		/* [in] */ IMessage* msg);

    // event listeners for MediaPlayer
    // Those are called from the same thread we created the MediaPlayer
    // (i.e. the webviewcore thread here)

    // MediaPlayer.OnBufferingUpdateListener
	virtual CARAPI_(void) OnBufferingUpdate(
		/* [in] */ MediaPlayer* mp, 
		/* [in] */ Int32 percent);

    // MediaPlayer.OnCompletionListener;
	virtual CARAPI_(void) OnCompletion(
		/* [in] */ MediaPlayer* mp);

    // MediaPlayer.OnErrorListener
	virtual CARAPI_(Boolean) OnError(
		/* [in] */ MediaPlayer* mp, 
		/* [in] */ Int32 what, 
		/* [in] */ Int32 extra);

    // MediaPlayer.OnPreparedListener
	virtual CARAPI_(void) OnPrepared(
		/* [in] */ MediaPlayer* mp);

    // MediaPlayer.OnSeekCompleteListener
	virtual CARAPI_(void) OnSeekComplete(
		/* [in] */ MediaPlayer* mp);

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
    // Logging tag.
	static const CString LOGTAG;// = "HTML5Audio";

	MediaPlayer* mMediaPlayer;

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
};

#endif //__HTML5AUDIO_H__