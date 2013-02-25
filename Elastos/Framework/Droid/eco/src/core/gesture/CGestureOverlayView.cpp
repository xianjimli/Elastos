
#include <gesture/CGestureOverlayView.h>
using namespace Elastos;
using namespace Elastos::Core;

/**
 * A transparent overlay for gesture input that can be placed on top of other
 * widgets or contain other widgets.
 *
 * @attr ref android.R.styleable#GestureOverlayView_eventsInterceptionEnabled
 * @attr ref android.R.styleable#GestureOverlayView_fadeDuration
 * @attr ref android.R.styleable#GestureOverlayView_fadeOffset
 * @attr ref android.R.styleable#GestureOverlayView_fadeEnabled
 * @attr ref android.R.styleable#GestureOverlayView_gestureStrokeWidth
 * @attr ref android.R.styleable#GestureOverlayView_gestureStrokeAngleThreshold
 * @attr ref android.R.styleable#GestureOverlayView_gestureStrokeLengthThreshold
 * @attr ref android.R.styleable#GestureOverlayView_gestureStrokeSquarenessThreshold
 * @attr ref android.R.styleable#GestureOverlayView_gestureStrokeType
 * @attr ref android.R.styleable#GestureOverlayView_gestureColor
 * @attr ref android.R.styleable#GestureOverlayView_orientation
 * @attr ref android.R.styleable#GestureOverlayView_uncertainGestureColor
 */
//@Widget

IVIEW_METHODS_IMPL(CGestureOverlayView, FrameLayout, GestureOverlayView);

IVIEWGROUP_METHODS_IMPL(CGestureOverlayView, FrameLayout, GestureOverlayView);

IVIEWPARENT_METHODS_IMPL(CGestureOverlayView, FrameLayout, GestureOverlayView);

IVIEWMANAGER_METHODS_IMPL(CGestureOverlayView, FrameLayout, GestureOverlayView);

IDrawableCallback_METHODS_IMPL(CGestureOverlayView, FrameLayout, GestureOverlayView);

IKeyEventCallback_METHODS_IMPL(CGestureOverlayView, FrameLayout, GestureOverlayView);

IAccessibilityEventSource_METHODS_IMPL(CGestureOverlayView, FrameLayout, GestureOverlayView);

PInterface CGestureOverlayView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CGestureOverlayView::Probe(riid);
}

ECode CGestureOverlayView::SetForegroundGravity(
    /* [in] */ Int32 foregroundGravity)
{
    return FrameLayout::SetForegroundGravity(foregroundGravity);
}

ECode CGestureOverlayView::SetForeground(
    /* [in] */ IDrawable* drawable)
{
    return FrameLayout::SetForeground(drawable);
}

ECode CGestureOverlayView::GetForeground(
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

ECode CGestureOverlayView::SetMeasureAllChildren(
    /* [in] */ Boolean measureAll)
{
    return FrameLayout::SetMeasureAllChildren(measureAll);
}

ECode CGestureOverlayView::GetConsiderGoneChildrenWhenMeasuring(
    /* [out] */ Boolean* measureAll)
{
    VALIDATE_NOT_NULL(measureAll);
    *measureAll = FrameLayout::GetConsiderGoneChildrenWhenMeasuring();
    return NOERROR;
}

ECode CGestureOverlayView::constructor(
    /* [in] */ IContext * context)
{
    return GestureOverlayView::Init(context);
}

ECode CGestureOverlayView::constructor(
    /* [in] */ IContext * context,
    /* [in] */ IAttributeSet * attrs)
{
    return GestureOverlayView::Init(context, attrs,
         0x010102ce /*com.android.internal.R.attr.gestureOverlayViewStyle*/);
}

ECode CGestureOverlayView::constructor(
    /* [in] */ IContext * context,
    /* [in] */ IAttributeSet * attrs,
    /* [in] */ Int32 defStyle)
{
    return GestureOverlayView::Init(context, attrs, defStyle);
}

//public ArrayList<GesturePoint> getCurrentStroke()
ECode CGestureOverlayView::GetCurrentStroke(
    /* [out] */ IObjectContainer ** stroke)
{
    return GestureOverlayView::GetCurrentStroke(stroke);
}

ECode CGestureOverlayView::GetOrientation(
    /* [out] */ Int32 * orientation)
{
    return CGestureOverlayView::GetOrientation(orientation);
}

ECode CGestureOverlayView::SetOrientation(
    /* [in] */ Int32 orientation)
{
    return GestureOverlayView::SetOrientation(orientation);
}

ECode CGestureOverlayView::SetGestureColor(
    /* [in] */ Int32 color)
{
    return GestureOverlayView::SetGestureColor(color);
}

ECode CGestureOverlayView::SetUncertainGestureColor(
    /* [in] */ Int32 color)
{
    return GestureOverlayView::SetUncertainGestureColor(color);
}

ECode CGestureOverlayView::GetUncertainGestureColor(
    /* [out] */ Int32 * uncertainGestureColor)
{
    return GestureOverlayView::GetUncertainGestureColor(uncertainGestureColor);
}

ECode CGestureOverlayView::GetGestureColor(
    /* [out] */ Int32 * gestureColor)
{
    return GestureOverlayView::GetGestureColor(gestureColor);
}

ECode CGestureOverlayView::GetGestureStrokeWidth(
    /* [out] */ Float * gestureStrokeWidth)
{
    return GestureOverlayView::GetGestureStrokeWidth(gestureStrokeWidth);
}

ECode CGestureOverlayView::SetGestureStrokeWidth(
    /* [in] */ Float gestureStrokeWidth)
{
    return GestureOverlayView::SetGestureStrokeWidth(gestureStrokeWidth);
}

ECode CGestureOverlayView::GetGestureStrokeType(
    /* [out] */ Int32 * gestureStrokeType)
{
    return GestureOverlayView::GetGestureStrokeType(gestureStrokeType);
}

ECode CGestureOverlayView::SetGestureStrokeType(
    /* [in] */ Int32 gestureStrokeType)
{
    return GestureOverlayView::SetGestureStrokeType(gestureStrokeType);
}

ECode CGestureOverlayView::GetGestureStrokeLengthThreshold(
    /* [out] */ Float * gestureStrokeLengthThreshold)
{
    return GestureOverlayView::GetGestureStrokeLengthThreshold(
        gestureStrokeLengthThreshold);
}

ECode CGestureOverlayView::SetGestureStrokeLengthThreshold(
    /* [in] */ Float gestureStrokeLengthThreshold)
{
    return GestureOverlayView::SetGestureStrokeLengthThreshold(
        gestureStrokeLengthThreshold);
}

ECode CGestureOverlayView::GetGestureStrokeSquarenessTreshold(
    /* [out] */ Float * gestureStrokeSquarenessTreshold)
{
    return GestureOverlayView::GetGestureStrokeSquarenessTreshold(
        gestureStrokeSquarenessTreshold);
}

ECode CGestureOverlayView::SetGestureStrokeSquarenessTreshold(
    /* [in] */ Float gestureStrokeSquarenessTreshold)
{
    return GestureOverlayView::SetGestureStrokeSquarenessTreshold(
        gestureStrokeSquarenessTreshold);
}

ECode CGestureOverlayView::GetGestureStrokeAngleThreshold(
    /* [out] */ Float * gestureStrokeAngleThreshold)
{
    return GestureOverlayView::GetGestureStrokeAngleThreshold(
        gestureStrokeAngleThreshold);
}

ECode CGestureOverlayView::SetGestureStrokeAngleThreshold(
    /* [in] */ Float gestureStrokeAngleThreshold)
{
    return GestureOverlayView::SetGestureStrokeAngleThreshold(
        gestureStrokeAngleThreshold);
}

ECode CGestureOverlayView::IsEventsInterceptionEnabled(
    /* [out] */ Boolean * eventsInterceptionEnabled)
{
    return GestureOverlayView::IsEventsInterceptionEnabled(eventsInterceptionEnabled);
}

ECode CGestureOverlayView::SetEventsInterceptionEnabled(
    /* [in] */ Boolean enabled)
{
    return GestureOverlayView::SetEventsInterceptionEnabled(enabled);
}

ECode CGestureOverlayView::IsFadeEnabled(
    /* [out] */ Boolean * fadeEnabled)
{
    return GestureOverlayView::IsFadeEnabled(fadeEnabled);
}

ECode CGestureOverlayView::SetFadeEnabled(
    /* [in] */ Boolean fadeEnabled)
{
    return GestureOverlayView::SetFadeEnabled(fadeEnabled);
}

ECode CGestureOverlayView::GetGesture(
    /* [out] */ IGesture ** gesture)
{
    return GestureOverlayView::GetGesture(gesture);
}

ECode CGestureOverlayView::SetGesture(
    /* [in] */ IGesture * gesture)
{
    return GestureOverlayView::SetGesture(gesture);
}

ECode CGestureOverlayView::GetGesturePath(
    /* [out] */ IPath ** path)
{
    return GestureOverlayView::GetGesturePath(path);
}

ECode CGestureOverlayView::GetGesturePathEx(
    /* [in] */ IPath * path,
    /* [out] */ IPath ** pathret)
{
    return GestureOverlayView::GetGesturePathEx(path,pathret);
}

ECode CGestureOverlayView::IsGestureVisible(
    /* [out] */ Boolean * gestureVisible)
{
    return GestureOverlayView::IsGestureVisible(gestureVisible);
}

ECode CGestureOverlayView::SetGestureVisible(
    /* [in] */ Boolean visible)
{
    return GestureOverlayView::SetGestureVisible(visible);
}

ECode CGestureOverlayView::GetFadeOffset(
    /* [out] */ Int64 * fadeOffset)
{
    return GestureOverlayView::GetFadeOffset(fadeOffset);
}

ECode CGestureOverlayView::SetFadeOffset(
    /* [in] */ Int64 fadeOffset)
{
    return GestureOverlayView::SetFadeOffset(fadeOffset);
}

ECode CGestureOverlayView::AddOnGestureListener(
    /* [in] */ IOnGestureListener * listener)
{
    return GestureOverlayView::AddOnGestureListener(listener);
}

ECode CGestureOverlayView::RemoveOnGestureListener(
    /* [in] */ IOnGestureListener * listener)
{
    return GestureOverlayView::RemoveOnGestureListener(listener);
}

ECode CGestureOverlayView::RemoveAllOnGestureListeners()
{
    return GestureOverlayView::RemoveAllOnGestureListeners();
}

ECode CGestureOverlayView::AddOnGesturePerformedListener(
    /* [in] */ IOnGesturePerformedListener * listener)
{
    return GestureOverlayView::AddOnGesturePerformedListener(listener);
}

ECode CGestureOverlayView::RemoveOnGesturePerformedListener(
    /* [in] */ IOnGesturePerformedListener * listener)
{
    return GestureOverlayView::RemoveOnGesturePerformedListener(listener);
}

ECode CGestureOverlayView::RemoveAllOnGesturePerformedListeners()
{
    return GestureOverlayView::RemoveAllOnGesturePerformedListeners();
}

ECode CGestureOverlayView::AddOnGesturingListener(
    /* [in] */ IOnGesturingListener * listener)
{
    return GestureOverlayView::AddOnGesturingListener(listener);
}

ECode CGestureOverlayView::RemoveOnGesturingListener(
    /* [in] */ IOnGesturingListener * listener)
{
    return GestureOverlayView::RemoveOnGesturingListener(listener);
}

ECode CGestureOverlayView::RemoveAllOnGesturingListeners()
{
    return GestureOverlayView::RemoveAllOnGesturingListeners();
}

ECode CGestureOverlayView::IsGesturing(
    /* [out] */ Boolean * gesturing)
{
    return GestureOverlayView::IsGesturing(gesturing);
}

/**
 * @hide
 */
ECode CGestureOverlayView::GetGesturePaint(
    /* [out] */ IPaint ** paint)
{
    return GestureOverlayView::GetGesturePaint(paint);
}

ECode CGestureOverlayView::Clear(
    /* [in] */ Boolean animated)
{
    return GestureOverlayView::Clear(animated);
}

ECode CGestureOverlayView::CancelClearAnimation()
{
    return GestureOverlayView::CancelClearAnimation();
}

ECode CGestureOverlayView::CancelGesture()
{
    return GestureOverlayView::CancelGesture();
}

//@Override
//protected
void /*ECode*/ CGestureOverlayView::OnDetachedFromWindow()
{
    return GestureOverlayView::OnDetachedFromWindow();
    //return NOERROR;
}
