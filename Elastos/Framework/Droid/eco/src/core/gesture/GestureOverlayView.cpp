
#include <gesture/GestureOverlayView.h>
#include <gesture/CGestureStroke.h>
#include <gesture/CGesture.h>
#include <gesture/CGesturePoint.h>
#include <gesture/CGestureUtils.h>
#include "ext/frameworkext.h"
#include <os/Runnable.h>
#include <graphics/CPaint.h>
#include <graphics/CPath.h>
#include <view/CMotionEvent.h>
#include <view/animation/AnimationUtils.h>
#include <view/animation/CAccelerateDecelerateInterpolator.h>
#include <os/SystemClock.h>
#include <elastos.h>
#include <elastos/List.h>
#include <elastos/Math.h>
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


//0/*R.styleable.GestureOverlayView_orientation*/
//gestureStrokeWidth=0x01010274; 1/*R.styleable.GestureOverlayView_gestureStrokeWidth*/
//gestureColor=0x01010275;       2/*R.styleable.GestureOverlayView_gestureColor*/
//3/*R.styleable.GestureOverlayView_uncertainGestureColor*/
//4/*R.styleable.GestureOverlayView_fadeOffset*/
//5/*R.styleable.GestureOverlayView_fadeDuration*/
//gestureStrokeType=0x01010279;  6/*R.styleable.GestureOverlayView_gestureStrokeType*/
//gestureStrokeLengthThreshold=0x0101027a; 7/*R.styleable.GestureOverlayView_gestureStrokeLengthThreshold*/
//gestureStrokeSquarenessThreshold=0x0101027b; 8/*R.styleable.GestureOverlayView_gestureStrokeSquarenessThreshold*/
//gestureStrokeAngleThreshold=0x0101027c; 9/*R.styleable.GestureOverlayView_gestureStrokeAngleThreshold*/
//10/*R.styleable.GestureOverlayView_eventsInterceptionEnabled*/
//11/*R.styleable.GestureOverlayView_fadeEnabled*/

Int32 R_Styleable_GestureOverlayView[] = {
    0x01010273, 0x01010274, 0x01010275, 0x01010276,
    0x01010277, 0x01010278, 0x01010279, 0x0101027a,
    0x0101027b, 0x0101027c, 0x0101027d, 0x0101027e
};

const Int32 GestureOverlayView::FADE_ANIMATION_RATE = 16;
const Boolean GestureOverlayView::GESTURE_RENDERING_ANTIALIAS = TRUE;
const Boolean GestureOverlayView::DITHER_FLAG = TRUE;

GestureOverlayView::GestureOverlayView()
{
    CPaint::New(&mGesturePaint);// =new Paint();

    mFadeDuration = 150;
    mFadeOffset = 420;
    mFadeEnabled = TRUE;

    mCertainGestureColor = 0xFFFFFF00;
    mUncertainGestureColor = 0x48FFFF00;
    mGestureStrokeWidth = 12.0f;
    mInvalidateExtraBorder = 10;

    mGestureStrokeType = GestureOverlayView_GESTURE_STROKE_TYPE_SINGLE;
    mGestureStrokeLengthThreshold = 50.0f;
    mGestureStrokeSquarenessTreshold = 0.275f;
    mGestureStrokeAngleThreshold = 40.0f;

    mOrientation = GestureOverlayView_ORIENTATION_VERTICAL;

    CRect::New(&mInvalidRect);// = new Rect();
    CPath::New(&mPath);// = new Path();
    mGestureVisible = TRUE;

    mIsGesturing = FALSE;
    mPreviousWasGesturing = FALSE;
    mInterceptEvents = TRUE;

    mStrokeBuffer = new List<IGesturePoint*>();

    // TODO: Make (IGestureOverlayView*) this a list of WeakReferences
    mOnGestureListeners = new List<IOnGestureListener*>();

    // TODO: Make (IGestureOverlayView*) this a list of WeakReferences
    mOnGesturePerformedListeners = new List<IOnGesturePerformedListener*>();

    // TODO: Make (IGestureOverlayView*) this a list of WeakReferences
    mOnGesturingListeners = new List<IOnGesturingListener*>();

    // fading out effect
    mIsFadingOut = FALSE;
    mFadingAlpha = 1.0f;

    //AccelerateDecelerateInterpolator mInterpolator = new AccelerateDecelerateInterpolator();
    //IInterpolator *mInterpolator;// =new AccelerateDecelerateInterpolator();
    CAccelerateDecelerateInterpolator::New(&mInterpolator);
    mFadingOut = new FadeOutRunnable(this);

    mCurrentGesture = NULL;
}

GestureOverlayView::~GestureOverlayView()
{
    mGesturePaint->Release();
    mInvalidRect->Release();
    mPath->Release();

    //List<IGesturePoint*>*
    delete mStrokeBuffer;

    //List<IOnGestureListener*>*
    delete mOnGestureListeners;

    //List<IOnGesturePerformedListener*>*
    delete mOnGesturePerformedListeners;

    //List<IOnGesturingListener*>*
    delete mOnGesturingListeners;

    mInterpolator->Release();
    delete mFadingOut;

    if (mCurrentGesture != NULL) {
        mCurrentGesture->Release();
        mCurrentGesture = NULL;
    }
}

ECode GestureOverlayView::Init(
    /* [in] */ IContext * context)
{
    ECode ec = FrameLayout::Init(context);
    Init();
    return ec;
}

ECode GestureOverlayView::Init(
    /* [in] */ IContext * context,
    /* [in] */ IAttributeSet * attrs)
{
    return Init(context, attrs,
         0x010102ce /*com.android.internal.R.attr.gestureOverlayViewStyle*/);
}

ECode GestureOverlayView::Init(
    /* [in] */ IContext * context,
    /* [in] */ IAttributeSet * attrs,
    /* [in] */ Int32 defStyle)
{
    ECode ec = FrameLayout::Init(context, attrs, defStyle);

    ITypedArray *a = NULL;
    context->ObtainStyledAttributesEx3(
        attrs,
        ArrayOf<Int32>(R_Styleable_GestureOverlayView,/*com.android.internal.R.styleable.GestureOverlayView*/
                sizeof(R_Styleable_GestureOverlayView) / sizeof(Int32)),
        defStyle, 0, &a);

    a->GetFloat(
        1/*R.styleable.GestureOverlayView_gestureStrokeWidth*/,
        mGestureStrokeWidth,
        &mGestureStrokeWidth);

    mInvalidateExtraBorder = Math::Max(1, ((Int32) mGestureStrokeWidth) - 1);
    a->GetColor(
        2/*R.styleable.GestureOverlayView_gestureColor*/,
        mCertainGestureColor,
        &mCertainGestureColor);
    a->GetColor(
        3/*R.styleable.GestureOverlayView_uncertainGestureColor*/,
        mUncertainGestureColor,
        &mUncertainGestureColor);
    a->GetInt32(
        5/*R.styleable.GestureOverlayView_fadeDuration*/,
        (Int32) mFadeDuration,
        (Int32 *)&mFadeDuration);
    a->GetInt32(
        4/*R.styleable.GestureOverlayView_fadeOffset*/,
        (Int32) mFadeOffset,
        (Int32 *)&mFadeOffset);
    a->GetInt32(
        6/*R.styleable.GestureOverlayView_gestureStrokeType*/,
        mGestureStrokeType,
        &mGestureStrokeType);
    a->GetFloat(
        7/*R.styleable.GestureOverlayView_gestureStrokeLengthThreshold*/,
        mGestureStrokeLengthThreshold,
        &mGestureStrokeLengthThreshold);
    a->GetFloat(
        9/*R.styleable.GestureOverlayView_gestureStrokeAngleThreshold*/,
        mGestureStrokeAngleThreshold,
        &mGestureStrokeAngleThreshold);
    a->GetFloat(
        8/*R.styleable.GestureOverlayView_gestureStrokeSquarenessThreshold*/,
        mGestureStrokeSquarenessTreshold,
        &mGestureStrokeSquarenessTreshold);
    a->GetBoolean(
        10/*R.styleable.GestureOverlayView_eventsInterceptionEnabled*/,
        mInterceptEvents,
        &mInterceptEvents);
    a->GetBoolean(
        11/*R.styleable.GestureOverlayView_fadeEnabled*/,
        mFadeEnabled,
        &mFadeEnabled);
    a->GetInt32(
        0/*R.styleable.GestureOverlayView_orientation*/,
        mOrientation,
        &mOrientation);

    a->Recycle();

    Init();
    return ec;
}

void GestureOverlayView::Init()
{
    View::SetWillNotDraw(FALSE);

    IPaint * gesturePaint = mGesturePaint;
    gesturePaint->SetAntiAlias(GESTURE_RENDERING_ANTIALIAS);
    gesturePaint->SetColor(mCertainGestureColor);
    gesturePaint->SetStyle(PaintStyle_STROKE/*Paint.Style.STROKE*/);
    gesturePaint->SetStrokeJoin(PaintJoin_ROUND);
    gesturePaint->SetStrokeCap(PaintCap_ROUND);
    gesturePaint->SetStrokeWidth(mGestureStrokeWidth);
    gesturePaint->SetDither(DITHER_FLAG);

    mCurrentColor = mCertainGestureColor;
    SetPaintAlpha(255);
}

//public ArrayList<GesturePoint> getCurrentStroke()
ECode GestureOverlayView::GetCurrentStroke(
    /* [out] */ IObjectContainer ** stroke)
{
    IObjectContainer * ret = NULL;
    CObjectContainer::New(&ret);
    Int32 count=mStrokeBuffer->GetSize();
    for (Int32 i=0; i<count; i++) {
        IGesturePoint * p = (*mStrokeBuffer)[i];
        ret->Add(p);
    }
    *stroke = ret;
    return NOERROR;
}

ECode GestureOverlayView::GetOrientation(
    /* [out] */ Int32 * orientation)
{
    VALIDATE_NOT_NULL(orientation);
    *orientation = mOrientation;
    return NOERROR;
}

ECode GestureOverlayView::SetOrientation(
    /* [in] */ Int32 orientation)
{
    mOrientation = orientation;
    return NOERROR;
}

ECode GestureOverlayView::SetGestureColor(
    /* [in] */ Int32 color)
{
    mCertainGestureColor = color;
    return NOERROR;
}

ECode GestureOverlayView::SetUncertainGestureColor(
    /* [in] */ Int32 color)
{
    mUncertainGestureColor = color;
    return NOERROR;
}

ECode GestureOverlayView::GetUncertainGestureColor(
    /* [out] */ Int32 * uncertainGestureColor)
{
    VALIDATE_NOT_NULL(uncertainGestureColor);
    *uncertainGestureColor = mUncertainGestureColor;
    return NOERROR;
}

ECode GestureOverlayView::GetGestureColor(
    /* [out] */ Int32 * gestureColor)
{
    VALIDATE_NOT_NULL(gestureColor);
    *gestureColor = mCertainGestureColor;
    return NOERROR;
}

ECode GestureOverlayView::GetGestureStrokeWidth(
    /* [out] */ Float * gestureStrokeWidth)
{
    VALIDATE_NOT_NULL(gestureStrokeWidth);
    *gestureStrokeWidth = mGestureStrokeWidth;
    return NOERROR;
}

ECode GestureOverlayView::SetGestureStrokeWidth(
    /* [in] */ Float gestureStrokeWidth)
{
    mGestureStrokeWidth = gestureStrokeWidth;
    mInvalidateExtraBorder = Math::Max(1, ((Int32) gestureStrokeWidth) - 1);
    mGesturePaint->SetStrokeWidth(gestureStrokeWidth);
    return NOERROR;
}

ECode GestureOverlayView::GetGestureStrokeType(
    /* [out] */ Int32 * gestureStrokeType)
{
    VALIDATE_NOT_NULL(gestureStrokeType);
    *gestureStrokeType = mGestureStrokeType;
    return NOERROR;
}

ECode GestureOverlayView::SetGestureStrokeType(
    /* [in] */ Int32 gestureStrokeType)
{
    mGestureStrokeType = gestureStrokeType;
    return NOERROR;
}

ECode GestureOverlayView::GetGestureStrokeLengthThreshold(
    /* [out] */ Float * gestureStrokeLengthThreshold)
{
    VALIDATE_NOT_NULL(gestureStrokeLengthThreshold);
    *gestureStrokeLengthThreshold = mGestureStrokeLengthThreshold;
    return NOERROR;
}

ECode GestureOverlayView::SetGestureStrokeLengthThreshold(
    /* [in] */ Float gestureStrokeLengthThreshold)
{
    mGestureStrokeLengthThreshold = gestureStrokeLengthThreshold;
    return NOERROR;
}

ECode GestureOverlayView::GetGestureStrokeSquarenessTreshold(
    /* [out] */ Float * gestureStrokeSquarenessTreshold)
{
    VALIDATE_NOT_NULL(gestureStrokeSquarenessTreshold);
    *gestureStrokeSquarenessTreshold = mGestureStrokeSquarenessTreshold;
    return NOERROR;
}

ECode GestureOverlayView::SetGestureStrokeSquarenessTreshold(
    /* [in] */ Float gestureStrokeSquarenessTreshold)
{
    mGestureStrokeSquarenessTreshold = gestureStrokeSquarenessTreshold;
    return NOERROR;
}

ECode GestureOverlayView::GetGestureStrokeAngleThreshold(
    /* [out] */ Float * gestureStrokeAngleThreshold)
{
    VALIDATE_NOT_NULL(gestureStrokeAngleThreshold);
    *gestureStrokeAngleThreshold = mGestureStrokeAngleThreshold;
    return NOERROR;
}

ECode GestureOverlayView::SetGestureStrokeAngleThreshold(
    /* [in] */ Float gestureStrokeAngleThreshold)
{
    mGestureStrokeAngleThreshold = gestureStrokeAngleThreshold;
    return NOERROR;
}

ECode GestureOverlayView::IsEventsInterceptionEnabled(
    /* [out] */ Boolean * eventsInterceptionEnabled)
{
    VALIDATE_NOT_NULL(eventsInterceptionEnabled);
    *eventsInterceptionEnabled = mInterceptEvents;
    return NOERROR;
}

ECode GestureOverlayView::SetEventsInterceptionEnabled(
    /* [in] */ Boolean enabled)
{
    mInterceptEvents = enabled;
    return NOERROR;
}

ECode GestureOverlayView::IsFadeEnabled(
    /* [out] */ Boolean * fadeEnabled)
{
    VALIDATE_NOT_NULL(fadeEnabled);
    *fadeEnabled = mFadeEnabled;
    return NOERROR;
}

ECode GestureOverlayView::SetFadeEnabled(
    /* [in] */ Boolean fadeEnabled)
{
    mFadeEnabled = fadeEnabled;
    return NOERROR;
}

ECode GestureOverlayView::GetGesture(
    /* [out] */ IGesture ** gesture)
{
    mCurrentGesture->Clone(gesture);
    return NOERROR;
}

ECode GestureOverlayView::SetGesture(
    /* [in] */ IGesture * gesture)
{
    if (mCurrentGesture != NULL) {
        Clear(FALSE);
    }

    SetCurrentColor(mCertainGestureColor);
    mCurrentGesture->Release();
    gesture->Clone(&mCurrentGesture);

    IPath *path = NULL;
    mCurrentGesture->ToPath(&path);
    IRectF *bounds = NULL;
    CRectF::New(&bounds);
    path->ComputeBounds(bounds, TRUE);

    // TODO: The path should also be scaled to fit inside (IGestureOverlayView*) this view
    mPath->Rewind();
    Float boundsLeft = ((CRectF *)bounds)->mLeft;
    Float boundsTop = ((CRectF *)bounds)->mTop;
    Float boundsWidth;
    bounds->Width(&boundsWidth);
    Float boundsHeight;
    bounds->Height(&boundsHeight);
    mPath->AddPath(path, -boundsLeft + (View::GetWidth() - boundsWidth) / 2.0f,
            -boundsTop + (View::GetHeight() - boundsHeight) / 2.0f);

    mResetGesture = TRUE;

    return View::Invalidate();
    //return NOERROR;
}

ECode GestureOverlayView::GetGesturePath(
    /* [out] */ IPath ** path)
{
    (*path) = mPath;
    return NOERROR;
}

ECode GestureOverlayView::GetGesturePathEx(
    /* [in] */ IPath * path,
    /* [out] */ IPath ** pathret)
{
    VALIDATE_NOT_NULL(path);
    path->Set(mPath);
    *pathret = path;
    return NOERROR;
}

ECode GestureOverlayView::IsGestureVisible(
    /* [out] */ Boolean * gestureVisible)
{
    *gestureVisible = mGestureVisible;
    return NOERROR;
}

ECode GestureOverlayView::SetGestureVisible(
    /* [in] */ Boolean visible)
{
    mGestureVisible = visible;
    return NOERROR;
}

ECode GestureOverlayView::GetFadeOffset(
    /* [out] */ Int64 * fadeOffset)
{
    VALIDATE_NOT_NULL(fadeOffset);
    *fadeOffset = mFadeOffset;
    return NOERROR;
}

ECode GestureOverlayView::SetFadeOffset(
    /* [in] */ Int64 fadeOffset)
{
    mFadeOffset = fadeOffset;
    return NOERROR;
}

ECode GestureOverlayView::AddOnGestureListener(
    /* [in] */ IOnGestureListener * listener)
{
    VALIDATE_NOT_NULL(listener);
    mOnGestureListeners->PushBack(listener);
    return NOERROR;
}

ECode GestureOverlayView::RemoveOnGestureListener(
    /* [in] */ IOnGestureListener * listener)
{
    mOnGestureListeners->Remove(listener);
    return NOERROR;
}

ECode GestureOverlayView::RemoveAllOnGestureListeners()
{
    mOnGestureListeners->Clear();
    return NOERROR;
}

ECode GestureOverlayView::AddOnGesturePerformedListener(
    /* [in] */ IOnGesturePerformedListener * listener)
{
    VALIDATE_NOT_NULL(listener);
    mOnGesturePerformedListeners->PushBack(listener);
    Int32 count =  mOnGesturePerformedListeners->GetSize();
    if ( count > 0) {
        mHandleGestureActions = TRUE;
    }
    return NOERROR;
}

ECode GestureOverlayView::RemoveOnGesturePerformedListener(
    /* [in] */ IOnGesturePerformedListener * listener)
{
    mOnGesturePerformedListeners->Remove(listener);
    Int32 count = mOnGesturePerformedListeners->GetSize();
    if ( count <= 0) {
        mHandleGestureActions = FALSE;
    }
    return NOERROR;
}

ECode GestureOverlayView::RemoveAllOnGesturePerformedListeners()
{
    mOnGesturePerformedListeners->Clear();
    mHandleGestureActions = FALSE;
    return NOERROR;
}

ECode GestureOverlayView::AddOnGesturingListener(
    /* [in] */ IOnGesturingListener * listener)
{
    VALIDATE_NOT_NULL(listener);
    mOnGesturingListeners->PushBack(listener);
    return NOERROR;
}

ECode GestureOverlayView::RemoveOnGesturingListener(
    /* [in] */ IOnGesturingListener * listener)
{
    mOnGesturingListeners->Remove(listener);
    return NOERROR;
}

ECode GestureOverlayView::RemoveAllOnGesturingListeners()
{
    mOnGesturingListeners->Clear();
    return NOERROR;
}

ECode GestureOverlayView::IsGesturing(
    /* [out] */ Boolean * gesturing)
{
    VALIDATE_NOT_NULL(gesturing);
    *gesturing = mIsGesturing;
    return NOERROR;
}

//private
void GestureOverlayView::SetCurrentColor(
    /* [in] */ Int32 color)
{
    mCurrentColor = color;
    if (mFadingHasStarted) {
        SetPaintAlpha((Int32) (255 * mFadingAlpha));
    } else {
        SetPaintAlpha(255);
    }
    View::Invalidate();
}

/**
 * @hide
 */
ECode GestureOverlayView::GetGesturePaint(
    /* [out] */ IPaint ** paint)
{
    *paint = mGesturePaint;
    return NOERROR;
}

//@Override
ECode GestureOverlayView::Draw(
    /* [in] */ ICanvas * canvas)
{
    FrameLayout::Draw(canvas);

    if (mCurrentGesture != NULL && mGestureVisible) {
        canvas->DrawPath(mPath, mGesturePaint);
    }
    return NOERROR;
}

//private
void GestureOverlayView::SetPaintAlpha(
    /* [in] */ Int32 alpha)
{
    alpha += alpha >> 7;
    Int32 baseAlpha = (mCurrentColor >> 24) & 0xFF; //mCurrentColor >>> 24
    Int32 useAlpha = baseAlpha * alpha >> 8;
    //mGesturePaint->SetColor((mCurrentColor << 8 >>> 8) | (useAlpha << 24));
    mGesturePaint->SetColor((((mCurrentColor << 8) >> 8) & 0xFF) |
                            (useAlpha << 24));
}

ECode GestureOverlayView::Clear(
    /* [in] */ Boolean animated)
{
    Clear(animated, FALSE, TRUE);
    return NOERROR;
}

//private
void GestureOverlayView::Clear(
    /* [in] */ Boolean animated,
    /* [in] */ Boolean fireActionPerformed,
    /* [in] */ Boolean immediate)
{
    SetPaintAlpha(255);
    RemoveCallbacks(mFadingOut);
    mResetGesture = FALSE;
    mFadingOut->fireActionPerformed = fireActionPerformed;
    mFadingOut->resetMultipleStrokes = FALSE;

    if (animated && mCurrentGesture != NULL) {
        mFadingAlpha = 1.0f;
        mIsFadingOut = TRUE;
        mFadingHasStarted = FALSE;
        mFadingStart = AnimationUtils::CurrentAnimationTimeMillis() + mFadeOffset;

        PostDelayed(mFadingOut, mFadeOffset);
    } else {
        mFadingAlpha = 1.0f;
        mIsFadingOut = FALSE;
        mFadingHasStarted = FALSE;

        if (immediate) {
            if (mCurrentGesture != NULL) {
                mCurrentGesture->Release();
                mCurrentGesture = NULL;
            }
            mPath->Rewind();
            View::Invalidate();
        } else if (fireActionPerformed) {
            PostDelayed(mFadingOut, mFadeOffset);
        } else if (mGestureStrokeType == GestureOverlayView_GESTURE_STROKE_TYPE_MULTIPLE) {
            mFadingOut->resetMultipleStrokes = TRUE;
            PostDelayed(mFadingOut, mFadeOffset);
        } else {
            if (mCurrentGesture != NULL) {
                mCurrentGesture->Release();
                mCurrentGesture = NULL;
            }
            mPath->Rewind();
            View::Invalidate();
        }
    }
}

ECode GestureOverlayView::CancelClearAnimation()
{
    SetPaintAlpha(255);
    mIsFadingOut = FALSE;
    mFadingHasStarted = FALSE;
    RemoveCallbacks(mFadingOut);
    mPath->Rewind();
    if (mCurrentGesture != NULL) {
        mCurrentGesture->Release();
        mCurrentGesture = NULL;
    }
    return NOERROR;
}

ECode GestureOverlayView::CancelGesture() {
    mIsListeningForGestures = FALSE;

    // add the stroke to the current gesture
    IGestureStroke *stroke = NULL;
    IObjectContainer* points = NULL;
    GetCurrentStroke(&points);
    CGestureStroke::New(points, &stroke);

    mCurrentGesture->AddStroke(stroke);

    // pass the event to handlers
    Int64 now = SystemClock::GetUptimeMillis();
    IMotionEvent* event = NULL;
    CMotionEvent* ev = NULL;
    CMotionEvent::Obtain(now, now,
            MotionEvent_ACTION_CANCEL, 0.0f, 0.0f, 0, &ev);
    event =(IMotionEvent*) ev;

    List<IOnGestureListener*> *listeners = mOnGestureListeners;
    Int32 count = listeners->GetSize();
    for (Int32 i = 0; i < count; i++) {
        (*listeners)[i]->OnGestureCancelled((IGestureOverlayView *)(IGestureOverlayView*) this, event);
    }

    event->Recycle();

    Clear(FALSE);
    mIsGesturing = FALSE;
    mPreviousWasGesturing = FALSE;
    mStrokeBuffer->Clear();

    List<IOnGesturingListener*>* otherListeners = mOnGesturingListeners;
    count = otherListeners->GetSize();
    for (Int32 i = 0; i < count; i++) {
        (*otherListeners)[i]->OnGesturingEnded((IGestureOverlayView *)(IGestureOverlayView*) this);
    }
    return NOERROR;
}

//@Override
//protected
void /*ECode*/ GestureOverlayView::OnDetachedFromWindow() {
    CancelClearAnimation();
    //return NOERROR;
}

//@Override
Boolean GestureOverlayView::DispatchTouchEvent(
    /* [in] */ IMotionEvent * event)
//    /* [out] */ Boolean * ret)
{
    if (View::IsEnabled()) {
        Int32 strokesCount = 0;
        mCurrentGesture->GetStrokesCount(&strokesCount);
        Boolean cancelDispatch = (mIsGesturing || (mCurrentGesture != NULL &&
                strokesCount > 0 && mPreviousWasGesturing)) &&
                mInterceptEvents;

        ProcessEvent(event);

        if (cancelDispatch) {
            event->SetAction(MotionEvent_ACTION_CANCEL);
        }
        FrameLayout::DispatchTouchEvent(event);

        return TRUE;
    }

    return FrameLayout::DispatchTouchEvent(event);
    //return NOERROR;
}

//private
Boolean GestureOverlayView::ProcessEvent(
    /* [in] */ IMotionEvent * event)
{
    Int32 action;
    event->GetAction(&action);

    switch (action) {
        case MotionEvent_ACTION_DOWN:
            TouchDown(event);
            View::Invalidate();
            return TRUE;
        case MotionEvent_ACTION_MOVE:
            if (mIsListeningForGestures) {
                IRect * rect = TouchMove(event);
                if (rect != NULL) {
                    View::Invalidate(rect);
                }
                return TRUE;
            }
            break;
        case MotionEvent_ACTION_UP:
            if (mIsListeningForGestures) {
                TouchUp(event, FALSE);
                View::Invalidate();
                return TRUE;
            }
            break;
        case MotionEvent_ACTION_CANCEL:
            if (mIsListeningForGestures) {
                TouchUp(event, TRUE);
                View::Invalidate();
                return TRUE;
            }
    }

    return FALSE;
}

//private
void GestureOverlayView::TouchDown(
    /* [in] */ IMotionEvent * event)
{
    mIsListeningForGestures = TRUE;

    Float x;
    event->GetX(&x);
    Float y;
    event->GetY(&y);

    mX = x;
    mY = y;

    mTotalLength = 0;
    mIsGesturing = FALSE;

    Int32 strokesCount = 0;
    mCurrentGesture->GetStrokesCount(&strokesCount);

    if (mGestureStrokeType == GestureOverlayView_GESTURE_STROKE_TYPE_SINGLE || mResetGesture) {
        if (mHandleGestureActions) {
            SetCurrentColor(mUncertainGestureColor);
        }
        mResetGesture = FALSE;
        if (mCurrentGesture != NULL) {
            mCurrentGesture->Release();
            mCurrentGesture = NULL;
        }
        mPath->Rewind();
    } else if (mCurrentGesture == NULL || strokesCount == 0) {
        if (mHandleGestureActions) {
            SetCurrentColor(mUncertainGestureColor);
        }
    }

    // if there is fading out going on, stop it.
    if (mFadingHasStarted) {
        CancelClearAnimation();
    } else if (mIsFadingOut) {
        SetPaintAlpha(255);
        mIsFadingOut = FALSE;
        mFadingHasStarted = FALSE;
        RemoveCallbacks(mFadingOut);
    }

    if (mCurrentGesture == NULL) {
        CGesture::New(&mCurrentGesture);
    }
    IGesturePoint* p = NULL;
    Int64 eventTime = 0;
    event->GetEventTime(&eventTime);
    CGesturePoint::New(x, y, eventTime,&p);
    mStrokeBuffer->PushBack(p);
    mPath->MoveTo(x, y);

    Int32 border = mInvalidateExtraBorder;
    mInvalidRect->Set((Int32) x - border,
                      (Int32) y - border,
                      (Int32) x + border,
                      (Int32) y + border);

    mCurveEndX = x;
    mCurveEndY = y;

    // pass the event to handlers
    List<IOnGestureListener*>* listeners = mOnGestureListeners;
    Int32 count = listeners->GetSize();
    for (int i = 0; i < count; i++) {
        (*listeners)[i]->OnGestureStarted((IGestureOverlayView*) this, event);
    }
}

//private
IRect * GestureOverlayView::TouchMove(
    /* [in] */ IMotionEvent * event)
{
    IRect * areaToRefresh = NULL;

    Float x;
    event->GetX(&x);
    Float y;
    event->GetY(&y);

    Float previousX = mX;
    Float previousY = mY;

    Float dx = Math::Abs(x - previousX);
    Float dy = Math::Abs(y - previousY);

    if (dx >= GestureStroke_TOUCH_TOLERANCE || dy >= GestureStroke_TOUCH_TOLERANCE) {
        areaToRefresh = mInvalidRect;

        // start with the curve end
        Int32 border = mInvalidateExtraBorder;
        areaToRefresh->Set(
                (Int32) mCurveEndX - border,
                (Int32) mCurveEndY - border,
                (Int32) mCurveEndX + border,
                (Int32) mCurveEndY + border);

        Float cX = mCurveEndX = (x + previousX) / 2;
        Float cY = mCurveEndY = (y + previousY) / 2;

        mPath->QuadTo(previousX, previousY, cX, cY);

        // union with the control point of the new curve
        areaToRefresh->Union(
                (Int32) previousX - border, (Int32) previousY - border,
                (Int32) previousX + border, (Int32) previousY + border);

        // union with the end point of the new curve
        areaToRefresh->Union(
                (Int32) cX - border, (Int32) cY - border,
                (Int32) cX + border, (Int32) cY + border);

        mX = x;
        mY = y;

        IGesturePoint * p = NULL;
        Int64 eventTime = 0;
        event->GetEventTime(&eventTime);
        CGesturePoint::New(x, y, eventTime, &p);
        mStrokeBuffer->PushBack(p);

        if (mHandleGestureActions && !mIsGesturing) {
            mTotalLength += (Float) Math::Sqrt(dx * dx + dy * dy);

            if (mTotalLength > mGestureStrokeLengthThreshold) {
                IObjectContainer* originalPoints = NULL;
                GetCurrentStroke(&originalPoints);
                IOrientedBoundingBox * box =
                        CGestureUtils::ComputeOrientedBoundingBox(originalPoints);
                Float boxOrientation;
                box->GetOrientation(&boxOrientation);
                Float angle = Math::Abs(boxOrientation);
                if (angle > 90) {
                    angle = 180 - angle;
                }
                Float boxSquareness;
                box->GetSquareness(&boxSquareness);
                if (boxSquareness > mGestureStrokeSquarenessTreshold ||
                        (mOrientation == GestureOverlayView_ORIENTATION_VERTICAL ?
                                angle < mGestureStrokeAngleThreshold :
                                angle > mGestureStrokeAngleThreshold)) {

                    mIsGesturing = TRUE;
                    SetCurrentColor(mCertainGestureColor);

                    List<IOnGesturingListener*>* listeners = mOnGesturingListeners;
                    Int32 count = listeners->GetSize();
                    for (Int32 i = 0; i < count; i++) {
                        (*listeners)[i]->OnGesturingStarted((IGestureOverlayView*) this);
                    }
                }
            }
        }

        // pass the event to handlers
        List<IOnGestureListener *>* listeners = mOnGestureListeners;
        Int32 count = listeners->GetSize();
        for (Int32 i = 0; i < count; i++) {
            (*listeners)[i]->OnGesture((IGestureOverlayView*) this, event);
        }
    }

    return areaToRefresh;
}

//private
void GestureOverlayView::TouchUp(
    /* [in] */ IMotionEvent * event,
    /* [in] */ Boolean cancel)
{
    mIsListeningForGestures = FALSE;

    // A gesture wasn't started or was cancelled
    if (mCurrentGesture != NULL) {
        // add the stroke to the current gesture
        IObjectContainer* originalPoints = NULL;
        GetCurrentStroke(&originalPoints);
        IGestureStroke * gs = NULL;
        CGestureStroke::New(originalPoints, &gs);
        mCurrentGesture->AddStroke(gs);

        if (!cancel) {
            // pass the event to handlers
            List<IOnGestureListener*> * listeners = mOnGestureListeners;
            Int32 count = listeners->GetSize();
            for (Int32 i = 0; i < count; i++) {
                (*listeners)[i]->OnGestureEnded((IGestureOverlayView*) this, event);
            }

            Clear(mHandleGestureActions && mFadeEnabled,
                  mHandleGestureActions && mIsGesturing,
                  FALSE);
        } else {
            CancelGesture(event);

        }
    } else {
        CancelGesture(event);
    }

    mStrokeBuffer->Clear();
    mPreviousWasGesturing = mIsGesturing;
    mIsGesturing = FALSE;

    List<IOnGesturingListener*> * listeners = mOnGesturingListeners;
    Int32 count = listeners->GetSize();
    for (Int32 i = 0; i < count; i++) {
        (*listeners)[i]->OnGesturingEnded((IGestureOverlayView*) this);
    }
}

//private
void GestureOverlayView::CancelGesture(
    /* [in] */ IMotionEvent * event)
{
    // pass the event to handlers
    List<IOnGestureListener*> * listeners = mOnGestureListeners;
    Int32 count = listeners->GetSize();
    for (Int32 i = 0; i < count; i++) {
        (*listeners)[i]->OnGestureCancelled((IGestureOverlayView*) this, event);
    }

    Clear(FALSE);
}

//private
void GestureOverlayView::FireOnGesturePerformed() {
    List<IOnGesturePerformedListener*>* actionListeners = mOnGesturePerformedListeners;
    Int32 count = actionListeners->GetSize();
    for (Int32 i = 0; i < count; i++) {
        (*actionListeners)[i]->OnGesturePerformed((IGestureOverlayView*) this, mCurrentGesture);
    }
}

ECode GestureOverlayView::FadeOutRunnableRun(
    /* [in] */ Boolean fireActionPerformed,
    /* [in] */ Boolean resetMultipleStrokes)
{
    if (mIsFadingOut) {
        Int64 now = AnimationUtils::CurrentAnimationTimeMillis();
        Int64 duration = now - mFadingStart;

        if (duration > mFadeDuration) {
            if (fireActionPerformed) {
                FireOnGesturePerformed();
            }

            mPreviousWasGesturing = FALSE;
            mIsFadingOut = FALSE;
            mFadingHasStarted = FALSE;
            mPath->Rewind();
            if (mCurrentGesture != NULL) {
                mCurrentGesture->Release();
                mCurrentGesture = NULL;
            }
            SetPaintAlpha(255);
        } else {
            mFadingHasStarted = TRUE;
            Float interpolatedTime = Math::Max(0.0f,
                    Math::Min(1.0f, duration / (Float) mFadeDuration));
            Float ret = 0;
            mInterpolator->GetInterpolation(interpolatedTime, &ret);
            mFadingAlpha = 1.0f - ret;
            SetPaintAlpha((int) (255 * mFadingAlpha));
            PostDelayed(mFadingOut, FADE_ANIMATION_RATE);
        }
    } else if (resetMultipleStrokes) {
        mResetGesture = TRUE;
    } else {
        FireOnGesturePerformed();

        mFadingHasStarted = FALSE;
        mPath->Rewind();
        if (mCurrentGesture != NULL) {
            mCurrentGesture->Release();
            mCurrentGesture = NULL;
        }
        mPreviousWasGesturing = FALSE;
        SetPaintAlpha(255);
    }

    View::Invalidate();
    return NOERROR;
}

GestureOverlayView::FadeOutRunnable::FadeOutRunnable(
    /* [in] */ GestureOverlayView* obj)
{
    gestureOverlayView = obj;
}

//private
ECode GestureOverlayView::FadeOutRunnable::Run()
{
    return gestureOverlayView->FadeOutRunnableRun(
            fireActionPerformed,
            resetMultipleStrokes);
}
