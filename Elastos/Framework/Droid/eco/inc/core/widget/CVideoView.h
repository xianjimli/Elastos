
#ifndef __CVIDEOVIEW_H__
#define __CVIDEOVIEW_H__

#include "_CVideoView.h"
#include "widget/VideoView.h"
#include "view/ViewMacro.h"

CarClass(CVideoView), public VideoView
{
public:
    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    IVIEW_METHODS_DECL();

    IDrawableCallback_METHODS_DECL();

    IKeyEventCallback_METHODS_DECL();

    IAccessibilityEventSource_METHODS_DECL();

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

    CARAPI SetOnPreparedListener(
        /* [in] */ IMediaPlayerOnPreparedListener* l);

    CARAPI SetOnCompletionListener(
        /* [in] */ IMediaPlayerOnCompletionListener* l);

    CARAPI SetOnErrorListener(
        /* [in] */ IMediaPlayerOnErrorListener* l);

    CARAPI Suspend();

    CARAPI Resume();

    CARAPI Start();

    CARAPI Pause();

    CARAPI GetDuration(
        /* [out] */ Int32* duration);

    CARAPI GetCurrentPosition(
        /* [out] */ Int32* position);

    CARAPI SeekTo(
        /* [in] */ Int32 position);

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

    CARAPI GetHolder(
        /* [out] */ ISurfaceHolder** holder);

    CARAPI SetZOrderMediaOverlay(
        /* [in] */ Boolean isMediaOverlay);

    CARAPI SetZOrderOnTop(
        /* [in] */ Boolean onTop);

    CARAPI SetWindowType(
        /* [in] */ Int32 type);

    CARAPI IsFixedSize(
        /* [out] */ Boolean* size);
};

#endif //__CVIDEOVIEW_H__
