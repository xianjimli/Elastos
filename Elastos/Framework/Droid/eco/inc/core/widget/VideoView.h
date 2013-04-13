
#ifndef __VideoView_h__
#define __VideoView_h__

#include "ext/frameworkext.h"
#include "view/SurfaceView.h"
#include "media/media/CMediaPlayer.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>
#include <elastos/HashMap.h>


class VideoView : public SurfaceView
{
private:
    class VVOnVideoSizeChangedListener
        : public IMediaPlayerOnVideoSizeChangedListener
        , public ElRefBase
    {
    public:
        VVOnVideoSizeChangedListener(
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

        CARAPI OnVideoSizeChanged(
            /* [in] */ IMediaPlayer* mp,
            /* [in] */ Int32 width,
            /* [in] */ Int32 height);

    private:
        VideoView* mHost;
    };

    class VVOnPreparedListener
      : public IMediaPlayerOnPreparedListener
      , public ElRefBase
    {
    public:
        VVOnPreparedListener(
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

        CARAPI OnPrepared(
            /* [in] */ IMediaPlayer* mp);

    private:
        VideoView* mHost;
    };

    class VVOnCompletionListener
        : public IMediaPlayerOnCompletionListener
        , public ElRefBase
    {
    public:
        VVOnCompletionListener(
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

        CARAPI OnCompletion(
            /* [in] */ IMediaPlayer* mp);

    private:
        VideoView* mHost;
    };

    class VVOnErrorListener
        : public IMediaPlayerOnErrorListener
        , public ElRefBase
    {
    public:
        VVOnErrorListener(
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

        CARAPI OnError(
            /* [in] */ IMediaPlayer* mp,
            /* [in] */ Int32 what,
            /* [in] */ Int32 extra,
            /* [out] */ Boolean* res);

    private:
        VideoView* mHost;
    };

    class VVDialogOnClickListener
        : public ElRefBase
        , public IDialogInterfaceOnClickListener
    {
    public:
        VVDialogOnClickListener(
            /* [in] */ VideoView* host)
            : mHost(host)
        {}

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface* pObject,
            /* [in] */ InterfaceID* pIID);

        CARAPI OnClick(
            /* [in] */ IDialogInterface* dialog,
            /* [in] */ Int32 which);

    private:
        VideoView* mHost;
    };

    class VVOnBufferingUpdateListener
        : public IMediaPlayerOnBufferingUpdateListener
        , public ElRefBase
    {
    public:
        VVOnBufferingUpdateListener(
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

        CARAPI OnBufferingUpdate(
            /* [in] */ IMediaPlayer* mp,
            /* [in] */ Int32 percent);

    private:
        VideoView* mHost;
    };

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

public:
    VideoView();

    virtual ~VideoView() {}

    virtual CARAPI_(Int32) ResolveAdjustedSize(
        /* [in] */ Int32 desiredSize,
        /* [in] */ Int32 measureSpec);

    virtual CARAPI SetVideoPath(
        /* [in] */ const String& path);

    virtual CARAPI SetVideoURI(
        /* [in] */ IUri* uri);

    /**
     * @hide
     */
    virtual CARAPI SetVideoURI(
        /* [in] */ IUri* uri,
        /* [in] */ HashMap<String, String>* headers);

    virtual CARAPI StopPlayback();

    virtual CARAPI SetMediaController(
        /* [in] */ IMediaController* controller);

    virtual CARAPI SetOnPreparedListener(
        /* [in] */ IMediaPlayerOnPreparedListener* l);

    virtual CARAPI SetOnCompletionListener(
        /* [in] */ IMediaPlayerOnCompletionListener* l);

    virtual CARAPI SetOnErrorListener(
        /* [in] */ IMediaPlayerOnErrorListener* l);

    //@Override
    CARAPI_(Boolean) OnTouchEvent(
        /* [in] */ IMotionEvent* event);

    //@Override
    CARAPI_(Boolean) OnTrackballEvent(
        /* [in] */ IMotionEvent* event);

    //@Override
    CARAPI_(Boolean) OnKeyDown(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    virtual CARAPI Start();

    virtual CARAPI Pause();

    virtual CARAPI Suspend();

    virtual CARAPI Resume();

    virtual CARAPI_(Int32) GetDuration();

    virtual CARAPI_(Int32) GetCurrentPosition();

    virtual CARAPI SeekTo(
        /* [in] */ Int32 msec);

    virtual CARAPI_(Boolean) IsPlaying();

    virtual CARAPI_(Int32) GetBufferPercentage();

    virtual CARAPI_(Boolean) CanPause();

    virtual CARAPI_(Boolean) CanSeekBackward();

    virtual CARAPI_(Boolean) CanSeekForward();

protected:
    CARAPI Init(
        /* [in] */ IContext* context);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle = 0);

    //@Override
    CARAPI_(void) OnMeasure(
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
    static const CString TAG;
    // settable by the client
    AutoPtr<IUri> mUri;
    HashMap<String, String> mHeaders;
    Int32 mDuration;

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
    AutoPtr<IMediaPlayer> mMediaPlayer;
    Int32 mVideoWidth;
    Int32 mVideoHeight;
    Int32 mSurfaceWidth;
    Int32 mSurfaceHeight;
    AutoPtr<IMediaController> mMediaController;
    AutoPtr<IMediaPlayerOnCompletionListener> mOnCompletionListener;
    AutoPtr<IMediaPlayerOnPreparedListener> mOnPreparedListener;
    Int32 mCurrentBufferPercentage;
    AutoPtr<IMediaPlayerOnErrorListener> mOnErrorListener;
    Int32 mSeekWhenPrepared; // recording the seek position while preparing
    Boolean mCanPause;
    Boolean mCanSeekBack;
    Boolean mCanSeekForward;
    Int32 mStateWhenSuspended;  //state before calling suspend()

    AutoPtr<IMediaPlayerOnVideoSizeChangedListener> mSizeChangedListener;
    AutoPtr<IMediaPlayerOnPreparedListener> mPreparedListener;
    AutoPtr<IMediaPlayerOnCompletionListener> mCompletionListener;
    AutoPtr<IMediaPlayerOnErrorListener> mErrorListener;
    AutoPtr<IMediaPlayerOnBufferingUpdateListener> mBufferingUpdateListener;
    AutoPtr<ISurfaceHolderCallback> mSHCallback;
};

#endif //__VideoView_h__
