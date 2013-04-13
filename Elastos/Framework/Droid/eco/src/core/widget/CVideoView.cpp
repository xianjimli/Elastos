
#include "widget/CVideoView.h"


IVIEW_METHODS_IMPL(CVideoView, VideoView, VideoView);

IDrawableCallback_METHODS_IMPL(CVideoView, VideoView, VideoView);

IKeyEventCallback_METHODS_IMPL(CVideoView, VideoView, VideoView);

IAccessibilityEventSource_METHODS_IMPL(CVideoView, VideoView, VideoView);

ECode CVideoView::constructor(
    /* [in] */ IContext* context)
{
    return VideoView::Init(context);
}

ECode CVideoView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return VideoView::Init(context, attrs);
}

ECode CVideoView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return VideoView::Init(context, attrs, defStyle);
}

PInterface CVideoView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CVideoView::Probe(riid);
}

ECode CVideoView::ResolveAdjustedSize(
    /* [in] */ Int32 desiredSize,
    /* [in] */ Int32 measureSpec,
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);

    *size = VideoView::ResolveAdjustedSize(desiredSize, measureSpec);
    return NOERROR;
}

ECode CVideoView::SetVideoPath(
    /* [in] */ const String& path)
{
    return VideoView::SetVideoPath(path);
}

ECode CVideoView::SetVideoURI(
    /* [in] */ IUri* uri)
{
    return VideoView::SetVideoURI(uri);
}

ECode CVideoView::StopPlayback()
{
    return VideoView::StopPlayback();
}

ECode CVideoView::SetMediaController(
        /* [in] */ IMediaController* controller)
{
    return VideoView::SetMediaController(controller);
}

ECode CVideoView::SetOnPreparedListener(
    /* [in] */ IMediaPlayerOnPreparedListener* l)
{
    return VideoView::SetOnPreparedListener(l);
}

ECode CVideoView::SetOnCompletionListener(
    /* [in] */ IMediaPlayerOnCompletionListener* l)
{
    return VideoView::SetOnCompletionListener(l);
}

ECode CVideoView::SetOnErrorListener(
    /* [in] */ IMediaPlayerOnErrorListener* l)
{
    return VideoView::SetOnErrorListener(l);
}

ECode CVideoView::Suspend()
{
    return VideoView::Suspend();
}

ECode CVideoView::Resume()
{
    return VideoView::Resume();
}

ECode CVideoView::Start()
{
    return VideoView::Start();
}

ECode CVideoView::Pause()
{
    return VideoView::Pause();
}

ECode CVideoView::GetDuration(
    /* [out] */ Int32* duration)
{
    VALIDATE_NOT_NULL(duration);

    *duration = VideoView::GetDuration();
    return NOERROR;
}

ECode CVideoView::GetCurrentPosition(
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);

    *position = VideoView::GetCurrentPosition();
    return NOERROR;
}

ECode CVideoView::SeekTo(
    /* [in] */ Int32 position)
{
    return VideoView::SeekTo(position);
}

ECode CVideoView::IsPlaying(
    /* [out] */ Boolean* isPlaying)
{
    VALIDATE_NOT_NULL(isPlaying);

    *isPlaying = VideoView::IsPlaying();
    return NOERROR;
}

ECode CVideoView::GetBufferPercentage(
    /* [out] */ Int32* percentage)
{
    VALIDATE_NOT_NULL(percentage);

    *percentage = VideoView::GetBufferPercentage();
    return NOERROR;
}

ECode CVideoView::CanPause(
    /* [out] */ Boolean* canPause)
{
    VALIDATE_NOT_NULL(canPause);

    *canPause = VideoView::CanPause();
    return NOERROR;
}

ECode CVideoView::CanSeekBackward(
    /* [out] */ Boolean* canSeekBackward)
{
    VALIDATE_NOT_NULL(canSeekBackward);

    *canSeekBackward = VideoView::CanSeekBackward();
    return NOERROR;
}

ECode CVideoView::CanSeekForward(
    /* [out] */ Boolean* canSeekForward)
{
    VALIDATE_NOT_NULL(canSeekForward);

    *canSeekForward = VideoView::CanSeekForward();
    return NOERROR;
}

//ISurfaceView
ECode CVideoView::GetHolder(
    /* [out] */ ISurfaceHolder** holder)
{
    VALIDATE_NOT_NULL(holder);

    AutoPtr<ISurfaceHolder> _holder = SurfaceView::GetHolder();
    *holder = _holder;
    if (*holder) (*holder)->AddRef();
    return NOERROR;
}

ECode CVideoView::SetZOrderMediaOverlay(
    /* [in] */ Boolean isMediaOverlay)
{
    return SurfaceView::SetZOrderMediaOverlay(isMediaOverlay);
}

ECode CVideoView::SetZOrderOnTop(
    /* [in] */ Boolean onTop)
{
    return SurfaceView::SetZOrderOnTop(onTop);
}

ECode CVideoView::SetWindowType(
    /* [in] */ Int32 type)
{
    return SurfaceView::SetWindowType(type);
}

ECode CVideoView::IsFixedSize(
    /* [out] */ Boolean* size)
{
    VALIDATE_NOT_NULL(size);

    *size = SurfaceView::IsFixedSize();
    return NOERROR;
}
