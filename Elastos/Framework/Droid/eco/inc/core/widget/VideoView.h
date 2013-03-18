
#ifndef __VideoView_h__
#define __VideoView_h__

#include "view/SurfaceView.h"
#include <elastos/AutoPtr.h>
#include "ext/frameworkdef.h"

using namespace Elastos;

class VideoView : public SurfaceView
{
private:
    class VVSurfaceHodlerCallback
        : public ISurfaceHolderCallback
        , public ElRefBase
    {
    public:
        VVSurfaceHodlerCallback(
            /* [in] */ VideoView* host)
            : mHost(host)
        {}

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI SurfaceCreated(
            /* [in] */ ISurfaceHolder* holder);

        CARAPI SurfaceChanged(
            /* [in] */ ISurfaceHolder* holder,
            /* [in] */ Int32 format,
            /* [in] */ Int32 width,
            /* [in] */ Int32 height);

        CARAPI SurfaceDestroyed(
            /* [in] */ ISurfaceHolder* holder);

    private:
        VideoView* mHost;
    };

    // class VVOnPreparedListener
    //   : public IOnPreparedListener
    //   , public ElRefBase
    // {
    // public:
    //     VVOnPreparedListener(
    //         /* [in] */ VideoView* host)
    //         : mHost(host)
    //     {}

    //     CARAPI_(PInterface) Probe(
    //         /* [in]  */ REIID riid);

    //     CARAPI_(UInt32) AddRef();

    //     CARAPI_(UInt32) Release();

    //     CARAPI GetInterfaceID(
    //         /* [in] */ IInterface *pObject,
    //         /* [out] */ InterfaceID *pIID);

    //     CARAPI OnPrepared(
    //         /* [in] */ IMediaPlayer* mp);

    // private:
    //     VideoView* mHost;
    // };

    // class VVOnVideoSizeChangedListener
    //     : public IOnVideoSizeChangedListener
    //     , public ElRefBase
    // {
    // public:
    //     VVOnVideoSizeChangedListener(
    //         /* [in] */ VideoView* host)
    //         : mHost(host)
    //     {}

    //     CARAPI_(PInterface) Probe(
    //         /* [in]  */ REIID riid);

    //     CARAPI_(UInt32) AddRef();

    //     CARAPI_(UInt32) Release();

    //     CARAPI GetInterfaceID(
    //         /* [in] */ IInterface *pObject,
    //         /* [out] */ InterfaceID *pIID);

    //     CARAPI OnVideoSizeChanged(
    //         /* [in] */ IMediaPlayer* mp,
    //         /* [in] */ Int32 width,
    //         /* [in] */ Int32 height);

    // private:
    //     VideoView* mHost;
    // };

    // class VVOnCompletionListener
    //     : public IOnCompletionListener
    //     , public ElRefBase
    // {
    // public:
    //     VVOnCompletionListener(
    //         /* [in] */ VideoView* host)
    //         : mHost(host)
    //     {}

    //     CARAPI_(PInterface) Probe(
    //         /* [in]  */ REIID riid);

    //     CARAPI_(UInt32) AddRef();

    //     CARAPI_(UInt32) Release();

    //     CARAPI GetInterfaceID(
    //         /* [in] */ IInterface *pObject,
    //         /* [out] */ InterfaceID *pIID);

    //     CARAPI OnCompletion(
    //         /* [in] */ IMediaPlayer* mp);

    // private:
    //     VideoView* mHost;
    // };

    // class VVOnErrorListener
    //     : public IOnErrorListener
    //     , public ElRefBase
    // {
    // public:
    //     VVOnErrorListener(
    //         /* [in] */ VideoView* host)
    //         : mHost(host)
    //     {}

    //     CARAPI_(PInterface) Probe(
    //         /* [in]  */ REIID riid);

    //     CARAPI_(UInt32) AddRef();

    //     CARAPI_(UInt32) Release();

    //     CARAPI GetInterfaceID(
    //         /* [in] */ IInterface *pObject,
    //         /* [out] */ InterfaceID *pIID);

    //     CARAPI OnError(
    //         /* [in] */ IMediaPlayer* mp,
    //         /* [in] */ Int32 what,
    //         /* [in] */ Int32 extra,
    //         /* [out] */ Boolean* res);

    // private:
    //     VideoView* mHost;
    // };

    // class VVOnBufferingUpdateListener
    //     : public IOnBufferingUpdateListener
    //     , public ElRefBase
    // {
    // public:
    //     VVOnBufferingUpdateListener(
    //         /* [in] */ VideoView* host)
    //         : mHost(host)
    //     {}

    //     CARAPI_(PInterface) Probe(
    //         /* [in]  */ REIID riid);

    //     CARAPI_(UInt32) AddRef();

    //     CARAPI_(UInt32) Release();

    //     CARAPI GetInterfaceID(
    //         /* [in] */ IInterface *pObject,
    //         /* [out] */ InterfaceID *pIID);

    //     CARAPI OnBufferingUpdate(
    //         /* [in] */ IMediaPlayer* mp,
    //         /* [in] */ Int32 percent);

    // private:
    //     VideoView* mHost;
    // };

    // class VVDialogOnClickListener
    //     : public ElRefBase
    //     , public IDialogInterfaceOnClickListener
    // {
    // public:
    //     VVDialogOnClickListener(
    //         /* [in] */ GeckoApp* host);

    //     CARAPI_(PInterface) Probe(
    //         /* [in] */ REIID riid);

    //     CARAPI_(UInt32) AddRef();

    //     CARAPI_(UInt32) Release();

    //     CARAPI GetInterfaceID(
    //         /* [in] */ IInterface* pObject,
    //         /* [in] */ InterfaceID* pIID);

    //     CARAPI OnClick(
    //         /* [in] */ IDialogInterface* dialog,
    //         /* [in] */ Int32 which);

    // private:
    //     VVDialogOnClickListener* mHost;
    // };

public:
    VideoView();

    CARAPI ResolveAdjustedSize(
        /* [in] */ Int32 desiredSize,
        /* [in] */ Int32 measureSpec,
        /* [out] */ Int32* size);

    CARAPI SetVideoPath(
        /* [in] */ const String& path);

    CARAPI SetVideoURI(
        /* [in] */ IUri* uri);

    CARAPI StopPlayback();

    CARAPI SetMediaController(
        /* [in] */ IMediaController* controller);

    // CARAPI SetOnPreparedListener(
    //     /* [in] */ IOnPreparedListener* l);

    // CARAPI SetOnCompletionListener(
    //     /* [in] */ IOnCompletionListener* l);

    // CARAPI SetOnErrorListener(
    //     /* [in] */ IOnErrorListener* l);

    CARAPI Suspend();

    CARAPI Resume();

    CARAPI Start();

    CARAPI Pause();

    CARAPI GetDuration(
        /* [out] */ Int32* duration);

    CARAPI GetCurrentPosition(
        /* [out] */ Int32* position);

    CARAPI SeekTo(
        /* [in] */ Int32 msec);

    CARAPI IsPlaying(
        /* [out] */ Boolean* isPlaying);

    CARAPI GetBufferPercentage(
        /* [out] */ Int32* percentage);

    CARAPI CanPause(
        /* [out] */ Boolean* canPause);

    CARAPI CanSeekBackward(
        /* [out] */ Boolean* canSeekBackward);

    CARAPI CanSeekForward(
        /* [out] */ Boolean* canSeekForward);

    CARAPI Init(
        /* [in] */ IContext* context);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI SetVideoURI(
        /* [in] */ IUri* uri,
        /* [in] */  Map<String, String>* headers);

    virtual CARAPI_(Boolean) OnTouchEvent(
        /* [in] */ IMotionEvent* event);

    virtual CARAPI_(Boolean) OnTrackballEvent(
        /* [in] */ IMotionEvent* event);

    virtual CARAPI_(Boolean) OnKeyDown(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

protected:
    virtual CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

private:
    CARAPI_(void) InitVideoView();

    CARAPI_(void) OpenVideo();

    CARAPI_(void) AttachMediaController();

    CARAPI_(void) Release(
        /* [in] */ Boolean cleartargetstate);

    CARAPI_(void) ToggleMediaControlsVisiblity();

    CARAPI_(Boolean) IsInPlaybackState();

private:
    // settable by the client
    AutoPtr<IUri> mUri;
    Map<String, String> mHeaders;
    Int32         mDuration;

    // all possible internal states
    static const Int32 STATE_ERROR              = -1;
    static const Int32 STATE_IDLE               = 0;
    static const Int32 STATE_PREPARING          = 1;
    static const Int32 STATE_PREPARED           = 2;
    static const Int32 STATE_PLAYING            = 3;
    static const Int32 STATE_PAUSED             = 4;
    static const Int32 STATE_PLAYBACK_COMPLETED = 5;
    static const Int32 STATE_SUSPEND            = 6;
    static const Int32 STATE_RESUME             = 7;
    static const Int32 STATE_SUSPEND_UNSUPPORTED = 8;

    // mCurrentState is a VideoView object's current state.
    // mTargetState is the state that a method caller intends to reach.
    // For instance, regardless the VideoView object's current state,
    // calling pause() intends to bring the object to a target state
    // of STATE_PAUSED.
    Int32 mCurrentState;
    Int32 mTargetState;

    // All the stuff we need for playing and showing a video
    AutoPtr<ISurfaceHolder> mSurfaceHolder;
    //AutoPtr<IMediaPlayer> mMediaPlayer;
    Int32 mVideoWidth;
    Int32 mVideoHeight;
    Int32 mSurfaceWidth;
    Int32 mSurfaceHeight;
    AutoPtr<IMediaController> mMediaController;
    // AutoPtr<IOnCompletionListener> mOnCompletionListener;
    // AutoPtr<IOnPreparedListener> mOnPreparedListener;
    Int32 mCurrentBufferPercentage;
    // AutoPtr<IOnErrorListener> mOnErrorListener;
    Int32 mSeekWhenPrepared; // recording the seek position while preparing
    Boolean     mCanPause;
    Boolean     mCanSeekBack;
    Boolean     mCanSeekForward;
    Int32       mStateWhenSuspended;  //state before calling suspend()

    AutoPtr<ISurfaceHolderCallback> mSHCallback;
    // AutoPtr<IOnVideoSizeChangedListener> mSizeChangedListener
    // AutoPtr<IOnCompletionListener> mCompletionListener
    // AutoPtr<IOnErrorListener> mErrorListener;
    // AutoPtr<IOnBufferingUpdateListener> mBufferingUpdateListener;

};

#endif //__VideoView_h__
