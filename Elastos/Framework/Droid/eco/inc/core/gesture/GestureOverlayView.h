
#ifndef __GESTUREOVERLAYVIEW_H__
#define __GESTUREOVERLAYVIEW_H__

//#include "_CGestureOverlayView.h"
#include "ext/frameworkext.h"
#include <os/Runnable.h>
#include <widget/FrameLayout.h>

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
class GestureOverlayView : public FrameLayout
{
public:
    class FadeOutRunnable : public Runnable
    {
    public:
        Boolean fireActionPerformed;

        Boolean resetMultipleStrokes;

        FadeOutRunnable(
            /* [in] */ GestureOverlayView* obj);

        CARAPI Run();

    protected:
        GestureOverlayView* gestureOverlayView;
    };

    GestureOverlayView();

    virtual ~GestureOverlayView();

    CARAPI Init(
        /* [in] */ IContext * context);

    CARAPI Init(
        /* [in] */ IContext * context,
        /* [in] */ IAttributeSet * attrs);

    CARAPI Init(
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
    CARAPI /*_(void)*/ Draw(
        /* [in] */ ICanvas * canvas);

    CARAPI Clear(
        /* [in] */ Boolean animated);

    CARAPI CancelClearAnimation();

    CARAPI CancelGesture();

    //@Override
    //protected void onDetachedFromWindow() {
    CARAPI_(void) OnDetachedFromWindow();

    //@Override
    CARAPI_(Boolean) DispatchTouchEvent(
        /* [in] */ IMotionEvent * event);
//        /* [out] */ Boolean * ret);

//    CARAPI_(void) SetIGestureOverlayView(
//        /* [in] */ IGestureOverlayView * obj);

protected:
    CARAPI FadeOutRunnableRun(
        /* [in] */ Boolean fireActionPerformed,
        /* [in] */ Boolean resetMultipleStrokes);

private:
    static const Int32 FADE_ANIMATION_RATE;// = 16;
    static const Boolean GESTURE_RENDERING_ANTIALIAS;// = TRUE;
    static const Boolean DITHER_FLAG;//= TRUE;

    IPaint * mGesturePaint;// =new Paint();

    Int64 mFadeDuration;// = 150;
    Int64 mFadeOffset;// = 420;
    Int64 mFadingStart;
    Boolean mFadingHasStarted;
    Boolean mFadeEnabled;// = TRUE;

    Int32 mCurrentColor;
    Int32 mCertainGestureColor;// = 0xFFFFFF00;
    Int32 mUncertainGestureColor;// = 0x48FFFF00;
    Float mGestureStrokeWidth;// = 12.0f;
    Int32 mInvalidateExtraBorder;// = 10;

    Int32 mGestureStrokeType;// = GestureOverlayView_GESTURE_STROKE_TYPE_SINGLE;
    Float mGestureStrokeLengthThreshold;// = 50.0f;
    Float mGestureStrokeSquarenessTreshold;// = 0.275f;
    Float mGestureStrokeAngleThreshold;// = 40.0f;

    Int32 mOrientation;// = GestureOverlayView_ORIENTATION_VERTICAL;

    IRect* mInvalidRect;// = new Rect();
    IPath* mPath;// = new Path();
    Boolean mGestureVisible;// = TRUE;

    Float mX;
    Float mY;

    Float mCurveEndX;
    Float mCurveEndY;

    Float mTotalLength;
    Boolean mIsGesturing;// = FALSE;
    Boolean mPreviousWasGesturing;// = FALSE;
    Boolean mInterceptEvents;// = TRUE;
    Boolean mIsListeningForGestures;
    Boolean mResetGesture;

    // current gesture
    IGesture * mCurrentGesture;
    List<IGesturePoint*>* mStrokeBuffer;// = new ArrayList<GesturePoint>(100);

// TODO: Make this a list of WeakReferences
    List<IOnGestureListener*>* mOnGestureListeners;// =new ArrayList<OnGestureListener>();

// TODO: Make this a list of WeakReferences
    List<IOnGesturePerformedListener*>* mOnGesturePerformedListeners;// =new ArrayList<OnGesturePerformedListener>();

// TODO: Make this a list of WeakReferences
    List<IOnGesturingListener*>* mOnGesturingListeners;// =new ArrayList<OnGesturingListener>();

    Boolean mHandleGestureActions;

    // fading out effect
    Boolean mIsFadingOut;// = FALSE;
    Float mFadingAlpha;// = 1.0f;

    //AccelerateDecelerateInterpolator mInterpolator = new AccelerateDecelerateInterpolator();
    IInterpolator *mInterpolator;// =new AccelerateDecelerateInterpolator();

    FadeOutRunnable *mFadingOut;// = new FadeOutRunnable();

    void Init();

    void SetCurrentColor(
        /* [in] */ Int32 color);

    void SetPaintAlpha(
        /* [in] */ Int32 alpha);

    void Clear(
        /* [in] */ Boolean animated,
        /* [in] */ Boolean fireActionPerformed,
        /* [in] */ Boolean immediate);

    Boolean ProcessEvent(
        /* [in] */ IMotionEvent * event);

    void TouchDown(
        /* [in] */ IMotionEvent * event);

    IRect * TouchMove(
        /* [in] */ IMotionEvent * event);

    void TouchUp(
        /* [in] */ IMotionEvent * event,
        /* [in] */ Boolean cancel);

    void CancelGesture(
        /* [in] */ IMotionEvent * event);

    void FireOnGesturePerformed();

};

#endif // __GESTUREOVERLAYVIEW_H__
