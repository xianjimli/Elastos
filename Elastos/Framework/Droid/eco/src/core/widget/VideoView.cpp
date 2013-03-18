
#include "widget/VideoView.h"
#include <elastos/Math.h>
#include "net/Uri.h"
#include "content/CIntent.h"

PInterface VideoView::VVSurfaceHodlerCallback::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IInterface*)this;
    }
    else if (riid == EIID_ISurfaceHolderCallback) {
        return (ISurfaceHolderCallback*)this;
    }

    return NULL;
}

UInt32 VideoView::VVSurfaceHodlerCallback::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 VideoView::VVSurfaceHodlerCallback::Release()
{
    return ElRefBase::Release();
}

ECode VideoView::VVSurfaceHodlerCallback::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(ISurfaceHolderCallback*)this) {
        *pIID = EIID_ISurfaceHolderCallback;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
}

//In SurfaceCreated function, you could create a thread to draw your image.
ECode VideoView::VVSurfaceHodlerCallback::SurfaceCreated(
    /* [in] */ ISurfaceHolder* holder)
{
    mHost->mSurfaceHolder = holder;
    //resume() was called before surfaceCreated()
    // if (mHost->mMediaPlayer != NULL && mHost->mCurrentState == STATE_SUSPEND
    //        && mHost->mTargetState == STATE_RESUME) {
    //     mHost->mMediaPlayer->SetDisplay(mHost->mSurfaceHolder);
    //     mHost->Resume();
    // }
    // else {
    //     mHost->OpenVideo();
    // }

    return NOERROR;
}

ECode VideoView::VVSurfaceHodlerCallback::SurfaceChanged(
    /* [in] */ ISurfaceHolder* holder,
    /* [in] */ Int32 format,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    mHost->mSurfaceWidth = width;
    mHost->mSurfaceHeight = height;
    Boolean isValidState =  (mHost->mTargetState == STATE_PLAYING);
    Boolean hasValidSize = (mHost->mVideoWidth == width &&
                            mHost->mVideoHeight == height);
    // if (mHost->mMediaPlayer != NULL && isValidState && hasValidSize) {
        if (mHost->mSeekWhenPrepared != 0) {
            mHost->SeekTo(mHost->mSeekWhenPrepared);
        }
        mHost->Start();
        if (mHost->mMediaController != NULL) {
            Boolean isShowing;
            mHost->mMediaController->IsShowing(&isShowing);
            if (isShowing) {
                // ensure the controller will get repositioned later
                mHost->mMediaController->Hide();
            }
            mHost->mMediaController->Show();
        }
    // }

    return NOERROR;
}

ECode VideoView::VVSurfaceHodlerCallback::SurfaceDestroyed(
    /* [in] */ ISurfaceHolder* holder)
{
    // after we return from this we can't use the surface any more
    mHost->mSurfaceHolder.Clear();
    if (mHost->mMediaController != NULL) mHost->mMediaController->Hide();
    if (mHost->mCurrentState != STATE_SUSPEND) {
        mHost->Release(TRUE);
    }

    return NOERROR;
}

// PInterface VideoView::VVOnPreparedListener::Probe(
//     /* [in]  */ REIID riid)
// {
//     if (riid == EIID_IInterface) {
//         return (PInterface)(IInterface*)this;
//     }
//     else if (riid == EIID_IOnPreparedListener) {
//         return (IOnPreparedListener*)this;
//     }

//     return NULL;
// }

// UInt32 VideoView::VVOnPreparedListener::AddRef()
// {
//     return ElRefBase::AddRef();
// }

// UInt32 VideoView::VVOnPreparedListener::Release()
// {
//     return ElRefBase::Release();
// }

// ECode VideoView::VVOnPreparedListener::GetInterfaceID(
//     /* [in] */ IInterface *pObject,
//     /* [out] */ InterfaceID *pIID)
// {
//     VALIDATE_NOT_NULL(pIID);

//     if (pObject == (IInterface*)(IOnPreparedListener*)this) {
//         *pIID = EIID_IOnPreparedListener;
//     }
//     else {
//         return E_INVALID_ARGUMENT;
//     }
// }

// ECode VideoView::VVOnPreparedListener::OnPrepared(
//     /* [in] */ IMediaPlayer* mp)
// {
//     mHost->mCurrentState = STATE_PREPARED;

//     // Get the capabilities of the player for this stream
//     AutoPtr<IMetadata> data;
//     mp->GetMetadata(MediaPlayer::METADATA_ALL,
//                     MediaPlayer::BYPASS_METADATA_FILTER, (IMetadata**)&data);

//     if (data != NULL) {
//         Boolean hasRes, boolRes;
//         data->Has(Metadata::PAUSE_AVAILABLE, &hasRes);
//         data->GetBoolean(Metadata::PAUSE_AVAILABLE, &boolRes);
//         mHost->mCanPause = !hasRes || boolRes;

//         data->Has(Metadata::SEEK_BACKWARD_AVAILABLE, &hasRes);
//         data->GetBoolean(Metadata::SEEK_BACKWARD_AVAILABLE, &boolRes);
//         mHost->mCanSeekBack = !hasRes || boolRes;

//         data->Has(Metadata::SEEK_FORWARD_AVAILABLE, &hasRes);
//         data->GetBoolean(Metadata::SEEK_FORWARD_AVAILABLE, &boolRes);
//         mHost->mCanSeekForward = !hasRes || boolRes;
//     }
//     else {
//         mHost->mCanPause = mHost->mCanSeekBack = mHost->mCanSeekForward = TRUE;
//     }

//     if (mHost->mOnPreparedListener != NULL) {
//         mHost->mOnPreparedListener->OnPrepared(mHost->mMediaPlayer);
//     }
//     if (mHost->mMediaController != NULL) {
//         mHost->mMediaController->SetEnabled(TRUE);
//     }
//     mHost->mVideoWidth = mp->GetVideoWidth();
//     mHost->mVideoHeight = mp->GetVideoHeight();

//     Int32 seekToPosition = mHost->mSeekWhenPrepared;  // mSeekWhenPrepared may be changed after seekTo() call
//     if (seekToPosition != 0) {
//         mHost->SeekTo(seekToPosition);
//     }
//     if (mHost->mVideoWidth != 0 && mHost->mVideoHeight != 0) {
//         //Log.i("@@@@", "video size: " + mVideoWidth +"/"+ mVideoHeight);
//         mHost->GetHolder()->SetFixedSize(mHost->mVideoWidth, mHost->mVideoHeight);
//         if (mHost->mSurfaceWidth == mHost->mVideoWidth &&
//             mHost->mSurfaceHeight == mHost->mVideoHeight) {
//             // We didn't actually change the size (it was already at the size
//             // we need), so we won't get a "surface changed" callback, so
//             // start the video here instead of in the callback.
//             if (mHost->mTargetState == STATE_PLAYING) {
//                 mHost->Start();
//                 if (mHost->mMediaController != NULL) {
//                     mHost->mMediaController->Show();
//                 }
//             }
//             else if (!mHost->IsPlaying() &&
//                     (seekToPosition != 0 || GetCurrentPosition() > 0)) {
//                if (mHost->mMediaController != NULL) {
//                    // Show the media controls when we're paused into a video and make 'em stick.
//                    mHost->mMediaController->ShowEx(0);
//                }
//            }
//         }
//     }
//     else {
//         // We don't know the video size yet, but should start anyway.
//         // The video size might be reported to us later.
//         if (mHost->mTargetState == STATE_PLAYING) {
//             mHost->Start();
//         }
//     }

//     return NOERROR;
// }

// PInterface VideoView::VVOnVideoSizeChangedListener::Probe(
//     /* [in]  */ REIID riid)
// {
//     if (riid == EIID_IInterface) {
//         return (PInterface)(IInterface*)this;
//     }
//     else if (riid == EIID_IOnVideoSizeChangedListener) {
//         return (IOnVideoSizeChangedListener*)this;
//     }

//     return NULL;
// }

// UInt32 VideoView::VVOnVideoSizeChangedListener::AddRef()
// {
//     return ElRefBase::AddRef();
// }

// UInt32 VideoView::VVOnVideoSizeChangedListener::Release()
// {
//     return ElRefBase::Release();
// }

// ECode VideoView::VVOnVideoSizeChangedListener::GetInterfaceID(
//     /* [in] */ IInterface *pObject,
//     /* [out] */ InterfaceID *pIID)
// {
//     VALIDATE_NOT_NULL(pIID);

//     if (pObject == (IInterface*)(IOnVideoSizeChangedListener*)this) {
//         *pIID = EIID_IOnVideoSizeChangedListener;
//     }
//     else {
//         return E_INVALID_ARGUMENT;
//     }
// }

// ECode VideoView::VVOnVideoSizeChangedListener::OnVideoSizeChanged(
//     /* [in] */ IMediaPlayer* mp,
//     /* [in] */ Int32 width,
//     /* [in] */ Int32 height)
// {
//     mHost->mVideoWidth = mp->GetVideoWidth();
//     mHost->mVideoHeight = mp->GetVideoHeight();
//     if (mHost->mVideoWidth != 0 &&mHost-> mVideoHeight != 0) {
//         mHost->GetHolder()->SetFixedSize(
//                     mHost->mVideoWidth, mHost->mVideoHeight);
//     }

//     return NOERROR;
// }

// PInterface VideoView::VVOnCompletionListener::Probe(
//     /* [in]  */ REIID riid)
// {
//     if (riid == EIID_IInterface) {
//         return (PInterface)(IInterface*)this;
//     }
//     else if (riid == EIID_IOnCompletionListener) {
//         return (IOnCompletionListener*)this;
//     }

//     return NULL;
// }

// UInt32 VideoView::VVOnCompletionListener::AddRef()
// {
//     return ElRefBase::AddRef();
// }

// UInt32 VideoView::VVOnCompletionListener::Release()
// {
//     return ElRefBase::Release();
// }

// ECode VideoView::VVOnCompletionListener::GetInterfaceID(
//     /* [in] */ IInterface *pObject,
//     /* [out] */ InterfaceID *pIID)
// {
//     VALIDATE_NOT_NULL(pIID);

//     if (pObject == (IInterface*)(IOnCompletionListener*)this) {
//         *pIID = EIID_IOnCompletionListener;
//     }
//     else {
//         return E_INVALID_ARGUMENT;
//     }
// }

// ECode VideoView::VVOnCompletionListener::OnCompletion(
//     /* [in] */ IMediaPlayer* mp)
// {
//     mHost->mCurrentState = STATE_PLAYBACK_COMPLETED;
//     mHost->mTargetState = STATE_PLAYBACK_COMPLETED;
//     if (mHost->mMediaController != NULL) {
//         mHost->mMediaController->Hide();
//     }
//     if (mHost->mOnCompletionListener != NULL) {
//         mHost->mOnCompletionListener->OnCompletion(mHost->mMediaPlayer);
//     }

//     return NOERROR;
// }

// PInterface VideoView::VVOnErrorListener::Probe(
//     /* [in]  */ REIID riid)
// {
//     if (riid == EIID_IInterface) {
//         return (PInterface)(IInterface*)this;
//     }
//     else if (riid == EIID_IOnErrorListener) {
//         return (IOnErrorListener*)this;
//     }

//     return NULL;
// }

// UInt32 VideoView::VVOnErrorListener::AddRef()
// {
//     return ElRefBase::AddRef();
// }

// UInt32 VideoView::VVOnErrorListener::Release()
// {
//     return ElRefBase::Release();
// }

// ECode VideoView::VVOnErrorListener::GetInterfaceID(
//     /* [in] */ IInterface *pObject,
//     /* [out] */ InterfaceID *pIID)
// {
//     VALIDATE_NOT_NULL(pIID);

//     if (pObject == (IInterface*)(IOnErrorListener*)this) {
//         *pIID = EIID_IOnErrorListener;
//     }
//     else {
//         return E_INVALID_ARGUMENT;
//     }
// }

// ECode VideoView::VVOnErrorListener::OnError(
//     /* [in] */ IMediaPlayer* mp,
//     /* [in] */ Int32 what,
//     /* [in] */ Int32 extra,
//     /* [out] */ Boolean* res)
// {
//     printf("VideoView Error: %d, %d\n", what, extra);
//     mHost->mCurrentState = STATE_ERROR;
//     mHost->mTargetState = STATE_ERROR;
//     if (mHost->mMediaController != NULL) {
//         mHost->mMediaController->Hide();
//     }

//     /* If an error handler has been supplied, use it and finish. */
//     if (mHost->mOnErrorListener != NULL) {
//         Boolean result;
//         mHost->mOnErrorListener->OnError(mHost->mMediaPlayer, what, extra, result);
//         if (result) {
//             *res = TRUE;
//             return NOERROR;
//         }
//     }

//     /* Otherwise, pop up an error dialog so the user knows that
//      * something bad has happened. Only try and pop up the dialog
//      * if we're attached to a window. When we're going away and no
//      * longer have a window, don't bother showing the user an error.
//      */
//     if (mHost->GetWindowToken() != NULL) {
//         AutoPtr<IResources> r;
//         ASSERT_SUCCEEDED(mHost->mContext->GetResources((IResources**)&r));
//         Int32 messageId;

//         if (what == MediaPlayer::MEDIA_ERROR_NOT_VALID_FOR_PROGRESSIVE_PLAYBACK) {
//             messageId = 0x01040015;//com.android.internal.R.string.VideoView_error_text_invalid_progressive_playback;
//         } else {
//             messageId = 0x01040011;//com.android.internal.R.string.VideoView_error_text_unknown;
//         }


//         AutoPtr<IAlertDialogBuilder> builder;
//         ASSERT_SUCCEEDED(CAlertDialogBuilder::New(
//                 mHost->mContext, (IAlertDialogBuilder**)&builder));
//         builder->SetTitle(0x01040012); //com.android.internal.R.string.VideoView_error_title
//         builder->SetMessageEx(messageId);
//         builder->SetCancelable(FALSE);

//         AutoPtr<VVDialogOnClickListener> listener = new VVDialogOnClickListener(mHost);
//         builder->SetPositiveButton(0x01040010, //com.android.internal.R.string.VideoView_error_button
//                                   listener);

//         AutoPtr<IAlertDialog> dialog;
//         builder->Show((IAlertDialog**)&dialog);
//     }

//     *res = TRUE;
//     return NOERROR;
// }

// PInterface VideoView::VVOnBufferingUpdateListener::Probe(
//     /* [in]  */ REIID riid)
// {
//     if (riid == EIID_IInterface) {
//         return (PInterface)(IInterface*)this;
//     }
//     else if (riid == EIID_IOnBufferingUpdateListener) {
//         return (IOnBufferingUpdateListener*)this;
//     }

//     return NULL;
// }

// UInt32 VideoView::VVOnBufferingUpdateListener::AddRef()
// {
//     return ElRefBase::AddRef();
// }

// UInt32 VideoView::VVOnBufferingUpdateListener::Release()
// {
//     return ElRefBase::Release();
// }

// ECode VideoView::VVOnBufferingUpdateListener::GetInterfaceID(
//     /* [in] */ IInterface *pObject,
//     /* [out] */ InterfaceID *pIID)
// {
//     VALIDATE_NOT_NULL(pIID);

//     if (pObject == (IInterface*)(IOnBufferingUpdateListener*)this) {
//         *pIID = EIID_IOnBufferingUpdateListener;
//     }
//     else {
//         return E_INVALID_ARGUMENT;
//     }
// }

// ECode VideoView::VVOnBufferingUpdateListener::OnBufferingUpdate(
//     /* [in] */ IMediaPlayer* mp,
//     /* [in] */ Int32 percent)
// {
//     mHost->mCurrentBufferPercentage = percent;

//     return NOERROR;
// }

// PInterface VideoView::VVDialogOnClickListener::Probe(
//     /* [in]  */ REIID riid)
// {
//     if (riid == EIID_IInterface) {
//         return (PInterface)(IInterface*)this;
//     }
//     else if (riid == EIID_IDialogInterfaceOnClickListener) {
//         return (IDialogInterfaceOnClickListener*)this;
//     }

//     return NULL;
// }

// UInt32 VideoView::VVDialogOnClickListener::AddRef()
// {
//     return ElRefBase::AddRef();
// }

// UInt32 VideoView::VVDialogOnClickListener::Release()
// {
//     return ElRefBase::Release();
// }

// ECode VideoView::VVDialogOnClickListener::GetInterfaceID(
//     /* [in] */ IInterface *pObject,
//     /* [out] */ InterfaceID *pIID)
// {
//     VALIDATE_NOT_NULL(pIID);

//     if (pObject == (IInterface*)(IDialogInterfaceOnClickListener*)this) {
//         *pIID = EIID_IDialogInterfaceOnClickListener;
//     }
//     else {
//         return E_INVALID_ARGUMENT;
//     }
// }

// ECode VideoView::VVDialogOnClickListener::OnClick(
//     /* [in] */ IDialogInterface* dialog,
//     /* [in] */ Int32 which)
// {
//     /* If we get here, there is no onError listener, so
//      * at least inform them that the video is over.
//      */
//     if (mHost->mOnCompletionListener != NULL) {
//         mHost->mOnCompletionListener->OnCompletion(mHost->mMediaPlayer);
//     }

//     return NOERROR;
// }

VideoView::VideoView()
    : mCurrentState(STATE_IDLE)
    , mTargetState(STATE_IDLE)
{
    mSHCallback = new VVSurfaceHodlerCallback(this);
    assert(mSHCallback.Get());

    // mSizeChangedListener = new VVOnVideoSizeChangedListener(this);
    // assert(mSizeChangedListener.Get());

    // mCompletionListener = new VVOnCompletionListener(this);
    // assert(mCompletionListener.Get());

    // mErrorListener = new VVOnErrorListener(this);
    // assert(mErrorListener.Get());

    // mBufferingUpdateListener = new VVOnBufferingUpdateListener(this);
    // assert(mBufferingUpdateListener.Get());
}

ECode VideoView::ResolveAdjustedSize(
    /* [in] */ Int32 desiredSize,
    /* [in] */ Int32 measureSpec,
    /* [out] */ Int32* size)
{
    Int32 result = desiredSize;
    Int32 specMode = MeasureSpec::GetMode(measureSpec);
    Int32 specSize =  MeasureSpec::GetSize(measureSpec);

    switch (specMode) {
        case MeasureSpec::UNSPECIFIED:
            /* Parent says we can be as big as we want. Just don't be larger
             * than max size imposed on ourselves.
             */
            result = desiredSize;
            break;

        case MeasureSpec::AT_MOST:
            /* Parent says we can be as big as we want, up to specSize.
             * Don't be larger than specSize, and don't be larger than
             * the max size imposed on ourselves.
             */
            result = Math::Min(desiredSize, specSize);
            break;

        case MeasureSpec::EXACTLY:
            // No choice. Do what we are told.
            result = specSize;
            break;
    }
    *size = result;

    return NOERROR;
}

ECode VideoView::SetVideoPath(
    /* [in] */ const String& path)
{
    AutoPtr<IUri> uri;
    FAIL_RETURN(Uri::Parse(path, (IUri**)&uri));

    return SetVideoURI(uri);
}

ECode VideoView::SetVideoURI(
    /* [in] */ IUri* uri)
{
    return SetVideoURI(uri, NULL);
}

ECode VideoView::StopPlayback()
{
    // if (mMediaPlayer != NULL) {
    //     mMediaPlayer->Stop();
    //     mMediaPlayer->ReleaseEx();
    //     mMediaPlayer.Clear();
    //     mCurrentState = STATE_IDLE;
    //     mTargetState  = STATE_IDLE;
    // }

    return NOERROR;
}

ECode VideoView::SetMediaController(
        /* [in] */ IMediaController* controller)
{
    if (mMediaController != NULL) {
        mMediaController->Hide();
        mMediaController.Clear();
    }
    mMediaController = controller;
    AttachMediaController();

    return NOERROR;
}

// ECode VideoView::SetOnPreparedListener(
//     /* [in] */ IOnPreparedListener* l)
// {
//     return E_NOT_IMPLEMENTED;
// }

// ECode VideoView::SetOnCompletionListener(
//     /* [in] */ IOnCompletionListener* l)
// {
//     return E_NOT_IMPLEMENTED;
// }

// ECode VideoView::SetOnErrorListener(
//     /* [in] */ IOnErrorListener* l)
// {
//     return E_NOT_IMPLEMENTED;
// }

ECode VideoView::Suspend()
{
     if (IsInPlaybackState()) {
        Boolean result;
        // mMediaPlayer->Suspend(&result)
        if (result) {
            mStateWhenSuspended = mCurrentState;
            mCurrentState = STATE_SUSPEND;
            mTargetState = STATE_SUSPEND;
        }
        else {
            Release(FALSE);
            mCurrentState = STATE_SUSPEND_UNSUPPORTED;
            printf("VideoView Unable to suspend video. Release MediaPlayer.\n");
        }
    }

    return NOERROR;
}

ECode VideoView::Resume()
{
    if (mSurfaceHolder == NULL && mCurrentState == STATE_SUSPEND){
        mTargetState = STATE_RESUME;
        return NOERROR;
    }

    // if (mMediaPlayer != NULL && mCurrentState == STATE_SUSPEND) {
    //     Boolean result;
    //     // FAIL_RETURN(mMediaPlayer->Resume(&result));
    //     if (result) {
    //         mCurrentState = mStateWhenSuspended;
    //         mTargetState = mStateWhenSuspended;
    //     }
    //     else {
    //         printf("VideoView Unable to resume video\n");
    //     }
    //     return NOERROR;
    // }

    if (mCurrentState == STATE_SUSPEND_UNSUPPORTED) {
        OpenVideo();
    }

    return E_NOT_IMPLEMENTED;
}

ECode VideoView::Start()
{
    if (IsInPlaybackState()) {
        // FAIL_RETURN(mMediaPlayer->Start());
        mCurrentState = STATE_PLAYING;
    }
    mTargetState = STATE_PLAYING;

    return NOERROR;
}

ECode VideoView::Pause()
{
    if (IsInPlaybackState()) {
        // if (mMediaPlayer->IsPlaying()) {
        //     FAIL_RETURN(mMediaPlayer->Pause());
        //     mCurrentState = STATE_PAUSED;
        // }
    }
    mTargetState = STATE_PAUSED;

    return NOERROR;
}

ECode VideoView::GetDuration(
    /* [out] */ Int32* duration)
{
    VALIDATE_NOT_NULL(duration);
    if (IsInPlaybackState()) {
        if (mDuration > 0) {
            *duration = mDuration;
            return NOERROR;
        }
        // mDuration = mMediaPlayer->GetDuration();
        *duration = mDuration;
        return NOERROR;
    }

    mDuration = -1;
    *duration = mDuration;

    return NOERROR;
}

ECode VideoView::GetCurrentPosition(
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);
    if (IsInPlaybackState()) {
        // *position = mMediaPlayer->GetCurrentPosition();
        return NOERROR;
    }

    *position = 0;
    return NOERROR;
}

ECode VideoView::SeekTo(
    /* [in] */ Int32 msec)
{
    if (IsInPlaybackState()) {
        // FAIL_RETURN(mMediaPlayer->SeekTo(msec));
        mSeekWhenPrepared = 0;
    }
    else {
        mSeekWhenPrepared = msec;
    }

    return NOERROR;
}

ECode VideoView::IsPlaying(
    /* [out] */ Boolean* isPlaying)
{
    VALIDATE_NOT_NULL(isPlaying);
    // *isPlaying = IsInPlaybackState() && mMediaPlayer->IsPlaying();

    return NOERROR;
}

ECode VideoView::GetBufferPercentage(
    /* [out] */ Int32* percentage)
{
    VALIDATE_NOT_NULL(percentage);
    // if (mMediaPlayer != NULL) {
    //     *percentage =  mCurrentBufferPercentage;
    //     return NOERROR;
    // }
    *percentage = 0;

    return NOERROR;
}

ECode VideoView::CanPause(
    /* [out] */ Boolean* canPause)
{
    VALIDATE_NOT_NULL(canPause);
    *canPause = mCanPause;

    return NOERROR;
}

ECode VideoView::CanSeekBackward(
    /* [out] */ Boolean* canSeekBackward)
{
    VALIDATE_NOT_NULL(canSeekBackward);
    *canSeekBackward = mCanSeekBack;

    return NOERROR;
}

ECode VideoView::CanSeekForward(
    /* [out] */ Boolean* canSeekForward)
{
    VALIDATE_NOT_NULL(canSeekForward);
    *canSeekForward = mCanSeekForward;

    return NOERROR;
}

ECode VideoView::Init(
    /* [in] */ IContext* context)
{
    ASSERT_SUCCEEDED(View::Init(context));
    InitVideoView();

    return NOERROR;
}

ECode VideoView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    ASSERT_SUCCEEDED(Init(context, attrs, 0));
    InitVideoView();

    return NOERROR;
}

ECode VideoView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    ASSERT_SUCCEEDED(View::Init(context, attrs, defStyle));
    InitVideoView();

    return NOERROR;
}

ECode VideoView::SetVideoURI(
    /* [in] */ IUri* uri,
    /* [in] */  Map<String, String>* headers)
{
    mUri = uri;
    mHeaders = *headers;
    mSeekWhenPrepared = 0;
    OpenVideo();
    FAIL_RETURN(RequestLayout());

    return Invalidate();
}

Boolean VideoView::OnTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    if (IsInPlaybackState() && mMediaController != NULL) {
        ToggleMediaControlsVisiblity();
    }

    return FALSE;
}

Boolean VideoView::OnTrackballEvent(
    /* [in] */ IMotionEvent* event)
{
    if (IsInPlaybackState() && mMediaController != NULL) {
        ToggleMediaControlsVisiblity();
    }

    return FALSE;
}

Boolean VideoView::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    Boolean isKeyCodeSupported = keyCode != KeyEvent_KEYCODE_BACK &&
                                 keyCode != KeyEvent_KEYCODE_VOLUME_UP &&
                                 keyCode != KeyEvent_KEYCODE_VOLUME_DOWN &&
                                 keyCode != KeyEvent_KEYCODE_MENU &&
                                 keyCode != KeyEvent_KEYCODE_CALL &&
                                 keyCode != KeyEvent_KEYCODE_ENDCALL;
    // if (IsInPlaybackState() && isKeyCodeSupported && mMediaController != NULL) {
    //     if (keyCode == KeyEvent_KEYCODE_HEADSETHOOK ||
    //         keyCode == KeyEvent_KEYCODE_MEDIA_PLAY_PAUSE) {
    //         if (mMediaPlayer.isPlaying()) {
    //             Pause();
    //             mMediaController->Show();
    //         }
    //         else {
    //             Start();
    //             mMediaController->Hide();
    //         }
    //         return true;
    //     }
    //     else if (keyCode == KeyEvent_KEYCODE_MEDIA_STOP
    //             && mMediaPlayer.isPlaying()) {
    //         Pause();
    //         mMediaController->Show();
    //     }
    //     else {
    //         ToggleMediaControlsVisiblity();
    //     }
    // }

    return SurfaceView::OnKeyDown(keyCode, event);
}

void VideoView::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    //Log.i("@@@@", "onMeasure");
    Int32 width = GetDefaultSize(mVideoWidth, widthMeasureSpec);
    Int32 height = GetDefaultSize(mVideoHeight, heightMeasureSpec);

    if (mVideoWidth > 0 && mVideoHeight > 0) {
        if ( mVideoWidth * height  > width * mVideoHeight ) {
            //Log.i("@@@", "image too tall, correcting");
            height = width * mVideoHeight / mVideoWidth;
        } else if ( mVideoWidth * height  < width * mVideoHeight ) {
            //Log.i("@@@", "image too wide, correcting");
            width = height * mVideoWidth / mVideoHeight;
        } else {
            //Log.i("@@@", "aspect ratio is correct: " +
                    //width+"/"+height+"="+
                    //mVideoWidth+"/"+mVideoHeight);
        }
    }
    //Log.i("@@@@@@@@@@", "setting size: " + width + 'x' + height);
    SetMeasuredDimension(width, height);
}

void VideoView::InitVideoView()
{
    mVideoWidth = 0;
    mVideoHeight = 0;
    AutoPtr<ISurfaceHolder> holder = GetHolder();
    holder->AddCallback(mSHCallback);
    holder->SetType(SurfaceHolder_SURFACE_TYPE_PUSH_BUFFERS);
    SetFocusable(TRUE);
    SetFocusableInTouchMode(TRUE);
    RequestFocus();
    mCurrentState = STATE_IDLE;
    mTargetState  = STATE_IDLE;
}

void VideoView::OpenVideo()
{
    if (mUri == NULL || mSurfaceHolder == NULL) {
        // not ready for playback just yet, will try again later
        return;
    }
    // Tell the music playback service to pause
    // TODO: these constants need to be published somewhere in the framework.
    AutoPtr<IIntent> i;
    ASSERT_SUCCEEDED(CIntent::New(
        String("com.android.music.musicservicecommand"), (IIntent**)&i));
    i->PutStringExtra(String("command"), String("pause"));
    mContext->SendBroadcast(i);

    // we shouldn't clear the target state, because somebody might have
    // called start() previously
    Release(FALSE);
    //try {
//         ECode ec = CMediaPlayer::New((IMediaPlayer**)&mMediaPlayer);
//         if (FAILED(ec)) goto exit;

//         ec = mMediaPlayer->SetOnPreparedListener(mPreparedListener);
//         if (FAILED(ec)) goto exit;

//         ec = mMediaPlayer->SetOnVideoSizeChangedListener(mSizeChangedListener);
//         if (FAILED(ec)) goto exit;

//         mDuration = -1;
//         ec = mMediaPlayer->SetOnCompletionListener(mCompletionListener);
//         if (FAILED(ec)) goto exit;

//         ec = mMediaPlayer->SetOnErrorListener(mErrorListener);
//         if (FAILED(ec)) goto exit;

//         ec = mMediaPlayer->SetOnBufferingUpdateListener(mBufferingUpdateListener);
//         if (FAILED(ec)) goto exit;

//         mCurrentBufferPercentage = 0;
//         ec = mMediaPlayer->SetDataSource(mContext, mUri, mHeaders);
//         if (FAILED(ec)) goto exit;

//         ec = mMediaPlayer->SetDisplay(mSurfaceHolder);
//         if (FAILED(ec)) goto exit;

//         ec = mMediaPlayer->SetAudioStreamType(AudioManager.STREAM_MUSIC);
//         if (FAILED(ec)) goto exit;

//         ec = mMediaPlayer->SetScreenOnWhilePlaying(TRUE);
//         if (FAILED(ec)) goto exit;

//         ec = mMediaPlayer->PrepareAsync();
//         if (FAILED(ec)) goto exit;
//         // we don't set the target state here either, but preserve the
//         // target state that was there before.
//         mCurrentState = STATE_PREPARING;
//         ec = AttachMediaController();
//         if (FAILED(ec)) goto exit;
// exit:
//     if (ec == E_IOException) {
//         mCurrentState = STATE_ERROR;
//         mTargetState = STATE_ERROR;
//         //mErrorListener.onError(mMediaPlayer, MediaPlayer.MEDIA_ERROR_UNKNOWN, 0);
//     }
//     else if (ec = E_IllegalArgumentException) {
//         mCurrentState = STATE_ERROR;
//         mTargetState = STATE_ERROR;
//         //mErrorListener.onError(mMediaPlayer, MediaPlayer.MEDIA_ERROR_UNKNOWN, 0);
//     }
    // } catch (IOException ex) {
    //     Log.w(TAG, "Unable to open content: " + mUri, ex);
    //     mCurrentState = STATE_ERROR;
    //     mTargetState = STATE_ERROR;
    //     mErrorListener.onError(mMediaPlayer, MediaPlayer.MEDIA_ERROR_UNKNOWN, 0);
    //     return;
    // } catch (IllegalArgumentException ex) {
    //     Log.w(TAG, "Unable to open content: " + mUri, ex);
    //     mCurrentState = STATE_ERROR;
    //     mTargetState = STATE_ERROR;
    //     mErrorListener.onError(mMediaPlayer, MediaPlayer.MEDIA_ERROR_UNKNOWN, 0);
    //     return;
    // }
}

void VideoView::AttachMediaController()
{
    // if (mMediaPlayer != NULL && mMediaController != NULL) {
    //     IMediaPlayerControl* pControl =
    //         (IMediaPlayerControl*)Probe(EIID_IMediaPlayerControl);
    //     assert(pControl);
    //     mMediaController->SetMediaPlayer(pControl);

    //     AutoPtr<IViewParent> parent;
    //     ASSERT_SUCCEEDED(GetParent((IViewParent**)&parent));
    //     IView* anchorView = (IView*)parent->Probe(EIID_View);
    //     if (!anchorView) {
    //         anchorView = (IView*)Probe(EIID_View);
    //     }

    //     mMediaController->SetAnchorView(anchorView);
    //     mMediaController->SetEnabled(IsInPlaybackState());
    // }
}

void VideoView::Release(
    /* [in] */ Boolean cleartargetstate)
{
    // if (mMediaPlayer != NULL) {
    //     mMediaPlayer->Reset();
    //     mMediaPlayer->ReleaseEx();
    //     mMediaPlayer.Clear();
    //     mCurrentState = STATE_IDLE;
    //     if (cleartargetstate) {
    //         mTargetState  = STATE_IDLE;
    //     }
    // }
}

void VideoView::ToggleMediaControlsVisiblity()
{
    Boolean isShowing;
    mMediaController->IsShowing(&isShowing);
    if (isShowing) {
        mMediaController->Hide();
    }
    else {
        mMediaController->Show();
    }
}

Boolean VideoView::IsInPlaybackState()
{
    // return (mMediaPlayer != NULL &&
    //             mCurrentState != STATE_ERROR &&
    //             mCurrentState != STATE_IDLE &&
    //             mCurrentState != STATE_PREPARING);
    return FALSE;
}
