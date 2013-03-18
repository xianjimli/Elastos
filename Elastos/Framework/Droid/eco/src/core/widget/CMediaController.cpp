
#include "widget/CMediaController.h"
#include "widget/FrameLayout.h"

IVIEW_METHODS_IMPL(CMediaController, MediaController, MediaController);

IVIEWGROUP_METHODS_IMPL(CMediaController, MediaController, MediaController);

IVIEWPARENT_METHODS_IMPL(CMediaController, MediaController, MediaController);

IVIEWMANAGER_METHODS_IMPL(CMediaController, MediaController, MediaController);

IDrawableCallback_METHODS_IMPL(CMediaController, MediaController, MediaController);

IKeyEventCallback_METHODS_IMPL(CMediaController, MediaController, MediaController);

IAccessibilityEventSource_METHODS_IMPL(CMediaController, MediaController, MediaController);

PInterface CMediaController::Probe(
        /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CMediaController::Probe(riid);
}

ECode CMediaController::SetMediaPlayer(
    /* [in] */ IMediaPlayerControl* player)
{
    return MediaController::SetMediaPlayer(player);
}

ECode CMediaController::SetAnchorView(
    /* [in] */ IView* view)
{
    return MediaController::SetAnchorView(view);
}

ECode CMediaController::Show()
{
    return MediaController::Show();
}

ECode CMediaController::ShowEx(
    /* [in] */ Int32 timeout)
{
    return MediaController::Show(timeout);
}

ECode CMediaController::IsShowing(
    /* [out] */ Boolean* isShowing)
{
    VALIDATE_NOT_NULL(isShowing);

    return MediaController::IsShowing(isShowing);
}

ECode CMediaController::Hide()
{
    return MediaController::Hide();
}

ECode CMediaController::SetPrevNextListeners(
    /* [in] */ IViewOnClickListener* next,
    /* [in] */ IViewOnClickListener* prev)
{
    return MediaController::SetPrevNextListeners(next, prev);
}


ECode CMediaController::constructor(
    /* [in] */ IContext* context)
{
    return MediaController::Init(context);
}

ECode CMediaController::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return MediaController::Init(context, attrs);
}

ECode CMediaController::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Boolean useFastForward)
{
    return MediaController::Init(context, useFastForward);
}

ECode CMediaController::SetForegroundGravity(
    /* [in] */ Int32 foregroundGravity)
{
    return FrameLayout::SetForegroundGravity(foregroundGravity);
}

ECode CMediaController::SetForeground(
    /* [in] */ IDrawable* drawable)
{
    return FrameLayout::SetForeground(drawable);
}

ECode CMediaController::GetForeground(
    /* [out] */ IDrawable** foreground)
{
    VALIDATE_NOT_NULL(foreground);
    AutoPtr<IDrawable> d = FrameLayout::GetForeground();
    *foreground = d.Get();
    if (*foreground) {
        (*foreground)->AddRef();
    }

    return NOERROR;
}

ECode CMediaController::SetMeasureAllChildren(
    /* [in] */ Boolean measureAll)
{
    return FrameLayout::SetMeasureAllChildren(measureAll);
}

ECode CMediaController::GetConsiderGoneChildrenWhenMeasuring(
    /* [out] */ Boolean* measureAll)
{
    VALIDATE_NOT_NULL(measureAll);
    *measureAll = FrameLayout::GetConsiderGoneChildrenWhenMeasuring();

    return NOERROR;
}
