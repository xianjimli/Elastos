
#ifndef __CGESTUREOVERLAYVIEW_H__
#define __CGESTUREOVERLAYVIEW_H__

#include "_CGestureOverlayView.h"
#include "ext/frameworkext.h"
#include <gesture/GestureOverlayView.h>
#include <view/ViewMacro.h>
#include <elastos.h>
#include <elastos/List.h>
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
//public class GestureOverlayView extends FrameLayout {
CarClass(CGestureOverlayView)
, public GestureOverlayView
{
public:
    IVIEW_METHODS_DECL();

    IVIEWGROUP_METHODS_DECL();

    IVIEWPARENT_METHODS_DECL();

    IVIEWMANAGER_METHODS_DECL();

    IDrawableCallback_METHODS_DECL();

    IKeyEventCallback_METHODS_DECL();

    IAccessibilityEventSource_METHODS_DECL();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI SetForegroundGravity(
        /* [in] */ Int32 foregroundGravity);

    CARAPI SetForeground(
        /* [in] */ IDrawable* drawable);

    CARAPI GetForeground(
        /* [out] */ IDrawable** foreground);

    CARAPI SetMeasureAllChildren(
        /* [in] */ Boolean measureAll);

    CARAPI GetConsiderGoneChildrenWhenMeasuring(
        /* [out] */ Boolean* measureAll);

    CARAPI constructor(
        /* [in] */ IContext * context);

    CARAPI constructor(
        /* [in] */ IContext * context,
        /* [in] */ IAttributeSet * attrs);

    CARAPI constructor(
        /* [in] */ IContext * context,
        /* [in] */ IAttributeSet * attrs,
        /* [in] */ Int32 defStyle);

    //ArrayList<GesturePoint> getCurrentStroke()
    CARAPI GetCurrentStroke(
        /* [out] */ IObjectContainer ** stroke);

    CARAPI GetOrientation(
        /* [out] */ Int32 * orientation);

    CARAPI SetOrientation(
        /* [in] */ Int32 orientation);

    CARAPI SetGestureColor(
        /* [in] */ Int32 color);

    CARAPI SetUncertainGestureColor(
        /* [in] */ Int32 color);

    CARAPI GetUncertainGestureColor(
        /* [out] */ Int32 * uncertainGestureColor);

    CARAPI GetGestureColor(
        /* [out] */ Int32 * gestureColor);

    CARAPI GetGestureStrokeWidth(
        /* [out] */ Float * gestureStrokeWidth);

    CARAPI SetGestureStrokeWidth(
        /* [in] */ Float gestureStrokeWidth);

    CARAPI GetGestureStrokeType(
        /* [out] */ Int32 * gestureStrokeType);

    CARAPI SetGestureStrokeType(
        /* [in] */ Int32 gestureStrokeType);

    CARAPI GetGestureStrokeLengthThreshold(
        /* [out] */ Float * gestureStrokeLengthThreshold);

    CARAPI SetGestureStrokeLengthThreshold(
        /* [in] */ Float gestureStrokeLengthThreshold);

    CARAPI GetGestureStrokeSquarenessTreshold(
        /* [out] */ Float * gestureStrokeSquarenessTreshold);

    CARAPI SetGestureStrokeSquarenessTreshold(
        /* [in] */ Float gestureStrokeSquarenessTreshold);

    CARAPI GetGestureStrokeAngleThreshold(
        /* [out] */ Float * gestureStrokeAngleThreshold);

    CARAPI SetGestureStrokeAngleThreshold(
        /* [in] */ Float gestureStrokeAngleThreshold);

    CARAPI IsEventsInterceptionEnabled(
        /* [out] */ Boolean * eventsInterceptionEnabled);

    CARAPI SetEventsInterceptionEnabled(
        /* [in] */ Boolean enabled);

    CARAPI IsFadeEnabled(
        /* [out] */ Boolean * fadeEnabled);

    CARAPI SetFadeEnabled(
        /* [in] */ Boolean fadeEnabled);

    CARAPI GetGesture(
        /* [out] */ IGesture ** gesture);

    CARAPI SetGesture(
        /* [in] */ IGesture * gesture);

    CARAPI GetGesturePath(
        /* [out] */ IPath ** path);

    CARAPI GetGesturePathEx(
        /* [in] */ IPath * path,
        /* [out] */ IPath ** pathret);

    CARAPI IsGestureVisible(
        /* [out] */ Boolean * gestureVisible);

    CARAPI SetGestureVisible(
        /* [in] */ Boolean visible);

    CARAPI GetFadeOffset(
        /* [out] */ Int64 * fadeOffset);

    CARAPI SetFadeOffset(
        /* [in] */ Int64 fadeOffset);

    CARAPI AddOnGestureListener(
        /* [in] */ IOnGestureListener * listener);

    CARAPI RemoveOnGestureListener(
        /* [in] */ IOnGestureListener * listener);

    CARAPI RemoveAllOnGestureListeners();

    CARAPI AddOnGesturePerformedListener(
        /* [in] */ IOnGesturePerformedListener * listener);

    CARAPI RemoveOnGesturePerformedListener(
        /* [in] */ IOnGesturePerformedListener * listener);

    CARAPI RemoveAllOnGesturePerformedListeners();

    CARAPI AddOnGesturingListener(
        /* [in] */ IOnGesturingListener * listener);

    CARAPI RemoveOnGesturingListener(
        /* [in] */ IOnGesturingListener * listener);

    CARAPI RemoveAllOnGesturingListeners();

    CARAPI IsGesturing(
        /* [out] */ Boolean * gesturing);

    /**
     * @hide
     */
    CARAPI GetGesturePaint(
        /* [out] */ IPaint ** paint);

    //@Override
//    virtual CARAPI Draw(
//        /* [in] */ ICanvas * canvas);

    CARAPI Clear(
        /* [in] */ Boolean animated);

    CARAPI CancelClearAnimation();

    CARAPI CancelGesture();

    //@Override
//    virtual CARAPI_(Boolean) DispatchTouchEvent(
//        /* [in] */ IMotionEvent * event);
//        /* [out] */ Boolean * ret);

protected:
    //    @Override
    //    protected void onDetachedFromWindow() {
    CARAPI_(void) OnDetachedFromWindow();
};

#endif // __CGESTUREOVERLAYVIEW_H_
