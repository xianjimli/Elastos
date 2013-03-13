
#include "webkit/HTML5Audio.h"

/**
 * @param nativePtr is the C++ pointer to the MediaPlayerPrivate object.
 */
HTML5Audio::HTML5Audio(
	/* [in] */ Int32 nativePtr)
{
	// Save the native ptr
    mNativePointer = nativePtr;
    ResetMediaPlayer();
}

//@Override
CARAPI_(void) HTML5Audio::HandleMessage(
	/* [in] */ IMessage* msg)
{
	assert(msg != NULL);

	switch (0/*msg->what*/)
	{
        case TIMEUPDATE:
        {
//            if (mState != ERROR && mMediaPlayer->IsPlaying())
            {
                Int32 position;// = mMediaPlayer.getCurrentPosition();
                NativeOnTimeupdate(position, mNativePointer);
            }
        }
    }
}

// MediaPlayer.OnBufferingUpdateListener
CARAPI_(void) HTML5Audio::OnBufferingUpdate(
	/* [in] */ MediaPlayer* mp, 
	/* [in] */ Int32 percent)
{
	assert(mp != NULL);

	NativeOnBuffering(percent, mNativePointer);
}

// MediaPlayer.OnCompletionListener;
CARAPI_(void) HTML5Audio::OnCompletion(
	/* [in] */ MediaPlayer* mp)
{
	assert(mp != NULL);

	ResetMediaPlayer();
    mState = IDLE;
    NativeOnEnded(mNativePointer);
}

// MediaPlayer.OnErrorListener
CARAPI_(Boolean) HTML5Audio::OnError(
	/* [in] */ MediaPlayer* mp, 
	/* [in] */ Int32 what, 
	/* [in] */ Int32 extra)
{
	assert(mp != NULL);

	mState = ERROR;
    ResetMediaPlayer();
    mState = IDLE;

    return false;
}

// MediaPlayer.OnPreparedListener
CARAPI_(void) HTML5Audio::OnPrepared(
	/* [in] */ MediaPlayer* mp)
{
}

// MediaPlayer.OnSeekCompleteListener
CARAPI_(void) HTML5Audio::OnSeekComplete(
	/* [in] */ MediaPlayer* mp)
{}

CARAPI_(void) HTML5Audio::ResetMediaPlayer()
{}

CARAPI_(void) HTML5Audio::SetDataSource(
	/* [in] */ CString url)
{}

CARAPI_(void) HTML5Audio::Play()
{}

CARAPI_(void) HTML5Audio::Pause()
{}

CARAPI_(void) HTML5Audio::Seek(
	/* [in] */ Int32 msec)
{}

CARAPI_(void) HTML5Audio::Teardown()
{}

CARAPI_(Float) HTML5Audio::GetMaxTimeSeekable()
{}

CARAPI_(void) HTML5Audio::NativeOnBuffering(
	/* [in] */ Int32 percent, 
	/* [in] */ Int32 nativePointer)
{}

CARAPI_(void) HTML5Audio::NativeOnEnded(
	/* [in] */ Int32 nativePointer)
{}

CARAPI_(void) HTML5Audio::NativeOnPrepared(
	/* [in] */ Int32 duration, 
	/* [in] */ Int32 width, 
	/* [in] */ Int32 height, 
	/* [in] */ Int32 nativePointer)
{}

CARAPI_(void) HTML5Audio::NativeOnTimeupdate(
	/* [in] */ Int32 position, 
	/* [in] */ Int32 nativePointer)
{}


/****************************TimeupdateTask*****************************/

CARAPI_(void) HTML5Audio::TimeupdateTask::Run()
{}