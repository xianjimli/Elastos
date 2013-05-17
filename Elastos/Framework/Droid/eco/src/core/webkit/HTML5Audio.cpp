
#include "webkit/HTML5Audio.h"
#include "media/media/CMediaPlayer.h"

const CString HTML5Audio::LOGTAG("HTML5Audio");

Int32 HTML5Audio::IDLE        =  0;
Int32 HTML5Audio::INITIALIZED =  1;
Int32 HTML5Audio::PREPARED    =  2;
Int32 HTML5Audio::STARTED     =  4;
Int32 HTML5Audio::COMPLETE    =  5;
Int32 HTML5Audio::PAUSED      =  6;
Int32 HTML5Audio::STOPPED     = -2;
Int32 HTML5Audio::ERROR       = -1;

// Timer thread -> UI thread
const Int32 HTML5Audio::TIMEUPDATE;

// The spec says the timer should fire every 250 ms or less.
const Int32 HTML5Audio::TIMEUPDATE_PERIOD;

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

PInterface HTML5Audio::Probe(
    /* [in] */ REIID riid)
{
    return NULL;
}

UInt32 HTML5Audio::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 HTML5Audio::Release()
{
    return ElRefBase::Release();
}

ECode HTML5Audio::GetInterfaceID(
    /* [in] */ IInterface *object,
    /* [out] */ InterfaceID *IID)
{
    VALIDATE_NOT_NULL(IID);

    return NOERROR;
}

#if 0
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
#endif

// MediaPlayer.OnBufferingUpdateListener
ECode HTML5Audio::OnBufferingUpdate(
	/* [in] */ IMediaPlayer* mp, 
	/* [in] */ Int32 percent)
{
	VALIDATE_NOT_NULL(mp);

	NativeOnBuffering(percent, mNativePointer);
}

// MediaPlayer.OnCompletionListener;
ECode HTML5Audio::OnCompletion(
	/* [in] */ IMediaPlayer* mp)
{
	VALIDATE_NOT_NULL(mp);

	ResetMediaPlayer();
    mState = IDLE;
    NativeOnEnded(mNativePointer);
}

// MediaPlayer.OnErrorListener
ECode HTML5Audio::OnError(
	/* [in] */ IMediaPlayer* mp, 
	/* [in] */ Int32 what, 
	/* [in] */ Int32 extra,
	/* [out] */ Boolean* result)
{
	VALIDATE_NOT_NULL(mp);

	mState = ERROR;
    ResetMediaPlayer();
    mState = IDLE;

    if (result) {
    	*result = FALSE;
    }

    return NOERROR;
}

// MediaPlayer.OnPreparedListener
ECode HTML5Audio::OnPrepared(
	/* [in] */ IMediaPlayer* mp)
{
}

// MediaPlayer.OnSeekCompleteListener
ECode HTML5Audio::OnSeekComplete(
	/* [in] */ IMediaPlayer* mp)
{
	VALIDATE_NOT_NULL(mp);

	Int32 position;
	mp->GetCurrentPosition(&position);
	NativeOnTimeupdate(position, mNativePointer);

	return NOERROR;
}

ECode HTML5Audio::Start(
    /* [in] */ ApartmentAttr attr)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode HTML5Audio::Finish()
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode HTML5Audio::PostCppCallback(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallback(target, func, params, id);
}

ECode HTML5Audio::PostCppCallbackAtTime(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id,
    /* [in] */ Millisecond64 uptimeMillis)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallbackAtTime(target, func, params, id, uptimeMillis);
}

ECode HTML5Audio::PostCppCallbackDelayed(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id,
    /* [in] */ Millisecond64 delayMillis)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallbackDelayed(target, func, params, id, delayMillis);
}

ECode HTML5Audio::PostCppCallbackAtFrontOfQueue(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallbackAtFrontOfQueue(target, func, params, id);
}

ECode HTML5Audio::RemoveCppCallbacks(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func)
{
    assert(mApartment != NULL);
    return mApartment->RemoveCppCallbacks(target, func);
}

ECode HTML5Audio::RemoveCppCallbacksEx(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ Int32 id)
{
    assert(mApartment != NULL);
    return mApartment->RemoveCppCallbacksEx(target, func, id);
}

ECode HTML5Audio::HasCppCallbacks(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [out] */ Boolean* result)
{
    assert(mApartment != NULL);
    return mApartment->HasCppCallbacks(target, func, result);
}

ECode HTML5Audio::HasCppCallbacksEx(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ Int32 id,
    /* [out] */ Boolean* result)
{
    assert(mApartment != NULL);
    return mApartment->HasCppCallbacksEx(target, func, id, result);
}

ECode HTML5Audio::SendMessage(
    /* [in] */ Int32 message,
    /* [in] */ IParcel* params)
{
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

CARAPI_(void) HTML5Audio::ResetMediaPlayer()
{
	if (mMediaPlayer == NULL) {
		CMediaPlayer::New((IMediaPlayer**)&mMediaPlayer);
    } else {
        mMediaPlayer->Reset();
    }
    mMediaPlayer->SetOnBufferingUpdateListener(this);
    mMediaPlayer->SetOnCompletionListener(this);
    mMediaPlayer->SetOnErrorListener(this);
    mMediaPlayer->SetOnPreparedListener(this);
    mMediaPlayer->SetOnSeekCompleteListener(this);
#if 0
    if (mTimer != null) {
        mTimer.cancel();
    }
    mTimer = new Timer();
#endif
    mState = IDLE;
}

CARAPI_(void) HTML5Audio::SetDataSource(
	/* [in] */ CString url)
{
	mUrl = url;
    //try {
        if (mState != IDLE) {
            ResetMediaPlayer();
        }
        mMediaPlayer->SetDataSourceEx2((String)(const char*)url);
        mState = INITIALIZED;
        mMediaPlayer->PrepareAsync();
    //} catch (IOException e) {
    //    Log.e(LOGTAG, "couldn't load the resource: " + url + " exc: " + e);
    //    resetMediaPlayer();
    //}
}

CARAPI_(void) HTML5Audio::Play()
{
	if ((mState == ERROR || mState == IDLE) && (const char*)mUrl != NULL) {
        ResetMediaPlayer();
        SetDataSource(mUrl);
        mAskToPlay = TRUE;
    }

    if (mState >= PREPARED) {
        mMediaPlayer->Start();
        mState = STARTED;
    }
}

CARAPI_(void) HTML5Audio::Pause()
{
	if (mState == STARTED) {
#if 0
        if (mTimer != null) {
            mTimer.purge();
        }
#endif
        mMediaPlayer->Pause();
        mState = PAUSED;
    }
}

CARAPI_(void) HTML5Audio::Seek(
	/* [in] */ Int32 msec)
{
	if (mState >= PREPARED) {
        mMediaPlayer->SeekTo(msec);
    }
}

CARAPI_(void) HTML5Audio::Teardown()
{
	mMediaPlayer->Release();
    mState = ERROR;
    mNativePointer = 0;
}

CARAPI_(Float) HTML5Audio::GetMaxTimeSeekable()
{
	Int32 duration;
	mMediaPlayer->GetDuration(&duration);
	return duration / 1000.0f;
}

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

ECode HTML5Audio::SendMessage(
    /* [in] */ Handle32 pvFunc,
    /* [in] */ IParcel* params)
{
    return mApartment->PostCppCallback((Handle32)this, pvFunc, params, 0);
}

ECode HTML5Audio::SendMessageAtTime(
    /* [in] */ Handle32 pvFunc,
    /* [in] */ IParcel* params,
    /* [in] */ Millisecond64 uptimeMillis)
{
    return mApartment->PostCppCallbackAtTime(
        (Handle32)this, pvFunc, params, 0, uptimeMillis);
}

ECode HTML5Audio::RemoveMessage(
    /* [in] */ Handle32 func)
{
    return mApartment->RemoveCppCallbacks((Handle32)this, func);
}

ECode HTML5Audio::HandleTimeUpdate()
{
	//try {
		Boolean isPlaying = FALSE;
		mMediaPlayer->IsPlaying(&isPlaying);
        if (mState != ERROR && isPlaying) {
            Int32 position;
            mMediaPlayer->GetCurrentPosition(&position);
            NativeOnTimeupdate(position, mNativePointer);
        }
    //} catch (IllegalStateException e) {
    //    mState = ERROR;
    //}
}


/****************************TimeupdateTask*****************************/

CARAPI_(void) HTML5Audio::TimeupdateTask::Run()
{
	//HTML5Audio.this.obtainMessage(TIMEUPDATE).sendToTarget();

	ECode (HTML5Audio::*pHandlerFunc)();

	pHandlerFunc = &HTML5Audio::HandleTimeUpdate;

//	SendMessage(*(Handle32*)&pHandlerFunc, NULL);
}