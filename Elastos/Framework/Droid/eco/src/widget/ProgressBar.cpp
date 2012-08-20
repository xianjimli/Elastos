
#include "widget/ProgressBar.h"
#include "view/animation/AnimationUtils.h"
#include "view/animation/CLinearInterpolator.h"
#include "view/animation/CTransformation.h"
#include "view/animation/CAlphaAnimation.h"
#include "widget/CProgressBarSavedState.h"
#include "os/SystemClock.h"
#include <elastos/Math.h>

ProgressBar::RefreshProgressRunnable::RefreshProgressRunnable(
    /* [in] */ Int32 id,
    /* [in] */ Int32 progress,
    /* [in] */ Boolean fromUser,
    /* [in] */ ProgressBar* owner)
    : mId(id)
    , mProgress(progress)
    , mFromUser(fromUser)
    , mOwner(owner)
{}

ECode ProgressBar::RefreshProgressRunnable::Run()
{
    mOwner->DoRefreshProgress(mId, mProgress, mFromUser);
    // Put ourselves back in the cache when we are done
    mOwner->mRefreshProgressRunnable = this;

    return NOERROR;
}

void ProgressBar::RefreshProgressRunnable::Setup(
    /* [in] */ Int32 id,
    /* [in] */ Int32 progress,
    /* [in] */ Boolean fromUser)
{
    mId = id;
    mProgress = progress;
    mFromUser = fromUser;
}

const Int32 ProgressBar::MAX_LEVEL;
const Int32 ProgressBar::ANIMATION_RESOLUTION;

ProgressBar::ProgressBar()
{}

ProgressBar::ProgressBar(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : View(context, attrs, defStyle)
{
    ASSERT_SUCCEEDED(InitFromAttributes(context, attrs, defStyle));
}

ECode ProgressBar::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    FAIL_RETURN(View::Init(context, attrs, defStyle));
    return InitFromAttributes(context, attrs, defStyle);
}

static Int32 R_Styleable_ProgressBar[] = {
    0x0101011f, 0x01010120, 0x01010136, 0x01010137,
    0x01010138, 0x01010139, 0x0101013a, 0x0101013b,
    0x0101013c, 0x0101013d, 0x0101013e, 0x0101013f,
    0x01010140, 0x01010141
};

ECode ProgressBar::InitFromAttributes(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    mUiThreadId = pthread_self();
    InitProgressBar();

    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributesEx3(
        attrs, ArrayOf<Int32>(R_Styleable_ProgressBar, sizeof(R_Styleable_ProgressBar) / sizeof(Int32)),
        defStyle, 0, (ITypedArray**)&a);

    mNoInvalidate = TRUE;

    AutoPtr<IDrawable> drawable;
    a->GetDrawable(8/*R.styleable.ProgressBar_progressDrawable*/, (IDrawable**)&drawable);
    if (drawable != NULL) {
        drawable = Tileify(drawable, FALSE);
        // Calling this method can set mMaxHeight, make sure the corresponding
        // XML attribute for mMaxHeight is read after calling this method
        SetProgressDrawable(drawable);
    }


    a->GetInt32(9/*R.styleable.ProgressBar_indeterminateDuration*/, mDuration, &mDuration);

    a->GetDimensionPixelSize(11/*R.styleable.ProgressBar_minWidth*/, mMinWidth, &mMinWidth);
    a->GetDimensionPixelSize(0/*R.styleable.ProgressBar_maxWidth*/, mMaxWidth, &mMaxWidth);
    a->GetDimensionPixelSize(12/*R.styleable.ProgressBar_minHeight*/, mMinHeight, &mMinHeight);
    a->GetDimensionPixelSize(1/*R.styleable.ProgressBar_maxHeight*/, mMaxHeight, &mMaxHeight);

    a->GetInt32(10/*R.styleable.ProgressBar_indeterminateBehavior*/, mBehavior, &mBehavior);

    Int32 resID;
    a->GetResourceId(
        13/*com.android.internal.R.styleable.ProgressBar_interpolator*/,
        0x010a000b/*R.anim.linear_interpolator*/, &resID); // default to linear interpolator
    if (resID > 0) {
        SetInterpolator(context, resID);
    }

    Int32 result;
    a->GetInt32(2/*R.styleable.ProgressBar_max*/, mMax, &result);
    SetMax(result);

    a->GetInt32(3/*R.styleable.ProgressBar_progress*/, mProgress, &result);
    SetProgress(result);

    a->GetInt32(4/*R.styleable.ProgressBar_secondaryProgress*/, mSecondaryProgress, &result);
    SetSecondaryProgress(result);

    drawable = NULL;
    a->GetDrawable(7/*R.Styleable.ProgressBar_indeterminateDrawable*/, (IDrawable**)&drawable);
    if (drawable != NULL) {
        drawable = TileifyIndeterminate(drawable);
        SetIndeterminateDrawable(drawable);
    }

    a->GetBoolean(
        6/*R.styleable.ProgressBar_indeterminateOnly*/, mOnlyIndeterminate, &mOnlyIndeterminate);

    mNoInvalidate = FALSE;

    Boolean b;
    a->GetBoolean(5/*R.styleable.ProgressBar_indeterminate*/, mIndeterminate, &b);
    SetIndeterminate(mOnlyIndeterminate || b);

    a->Recycle();
    return NOERROR;
}

/**
 * Converts a drawable to a tiled version of itself. It will recursively
 * traverse layer and state list drawables.
 */
AutoPtr<IDrawable> ProgressBar::Tileify(
    /* [in] */ IDrawable* drawable,
    /* [in] */ Boolean clip)
{
//    if (drawable->Probe(EIID_ILayerDrawable)) {
//        ILayerDrawable* background = (ILayerDrawable*) drawable;
//        Int32 N;
//        background->GetNumberOfLayers(&N);
//        ArrayOf<AutoPtr<IDrawable> >* outDrawables = ArrayOf<AutoPtr<IDrawable> >::Alloc(N);
//
//        for (Int32 i = 0; i < N; i++) {
//            Int32 id;
//            background->GetId(i, &id);
//            (*outDrawables)[i] = Tileify(background.getDrawable(i),
//                    (id == R.id.progress || id == R.id.secondaryProgress));
//        }
//
//        LayerDrawable newBg = new LayerDrawable(outDrawables);
//
//        for (Int32 i = 0; i < N; i++) {
//            newBg.setId(i, background.getId(i));
//        }
//
//        return newBg;
//
//    } else if (drawable->Probe(EIID_IStateListDrawable)) {
//        StateListDrawable in = (StateListDrawable) drawable;
//        StateListDrawable out = new StateListDrawable();
//        Int32 numStates = in.getStateCount();
//        for (Int32 i = 0; i < numStates; i++) {
//            out.addState(in.getStateSet(i), tileify(in.getStateDrawable(i), clip));
//        }
//        return out;
//
//    } else if (drawable instanceof BitmapDrawable) {
//        final Bitmap tileBitmap = ((BitmapDrawable) drawable).getBitmap();
//        if (mSampleTile == NULL) {
//            mSampleTile = tileBitmap;
//        }
//
//        final ShapeDrawable shapeDrawable = new ShapeDrawable(getDrawableShape());
//
//        final BitmapShader bitmapShader = new BitmapShader(tileBitmap,
//                Shader.TileMode.REPEAT, Shader.TileMode.CLAMP);
//        shapeDrawable.getPaint().setShader(bitmapShader);
//
//        return (clip) ? new ClipDrawable(shapeDrawable, Gravity.LEFT,
//                ClipDrawable.HORIZONTAL) : shapeDrawable;
//    }

    return drawable;
}

//AutoPtr<IShape> ProgressBar::GetDrawableShape()
//{
//    /*final Float[] roundedCorners = new Float[] { 5, 5, 5, 5, 5, 5, 5, 5
//    };
//    return new RoundRectShape(roundedCorners, NULL, NULL);*/
//
//    return NULL;
//}

/**
 * Convert a AnimationDrawable for use as a barberpole animation.
 * Each frame of the animation is wrapped in a ClipDrawable and
 * given a tiling BitmapShader.
 */
AutoPtr<IDrawable> ProgressBar::TileifyIndeterminate(
    /* [in] */ IDrawable* drawable)
{
//    if (drawable instanceof AnimationDrawable) {
//        AnimationDrawable background = (AnimationDrawable) drawable;
//        final Int32 N = background.getNumberOfFrames();
//        AnimationDrawable newBg = new AnimationDrawable();
//        newBg.setOneShot(background.isOneShot());
//
//        for (Int32 i = 0; i < N; i++) {
//            Drawable frame = tileify(background.getFrame(i), TRUE);
//            frame.setLevel(10000);
//            newBg.addFrame(frame, background.getDuration(i));
//        }
//        newBg.setLevel(10000);
//        drawable = newBg;
//    }
    return drawable;
}

/**
 * <p>
 * Initialize the progress bar's default values:
 * </p>
 * <ul>
 * <li>progress = 0</li>
 * <li>max = 100</li>
 * <li>animation duration = 4000 ms</li>
 * <li>indeterminate = FALSE</li>
 * <li>behavior = repeat</li>
 * </ul>
 */
void ProgressBar::InitProgressBar()
{
    mMax = 100;
    mProgress = 0;
    mSecondaryProgress = 0;
    mIndeterminate = FALSE;
    mOnlyIndeterminate = FALSE;
    mDuration = 4000;
//    mBehavior = AlphaAnimation.RESTART;
    mMinWidth = 24;
    mMaxWidth = 48;
    mMinHeight = 24;
    mMaxHeight = 48;
}

/**
 * <p>Indicate whether this progress bar is in indeterminate mode.</p>
 *
 * @return TRUE if the progress bar is in indeterminate mode
 */
//synchronized
//
Boolean ProgressBar::IsIndeterminate()
{
    Mutex::Autolock lock(*GetSelfLock());

    return mIndeterminate;
}

/**
 * <p>Change the indeterminate mode for this progress bar. In indeterminate
 * mode, the progress is ignored and the progress bar shows an infinite
 * animation instead.</p>
 *
 * If this progress bar's style only supports indeterminate mode (such as the circular
 * progress bars), then this will be ignored.
 *
 * @param indeterminate TRUE to enable the indeterminate mode
 */
//synchronized
//
ECode ProgressBar::SetIndeterminate(
    /* [in] */ Boolean indeterminate)
{
    Mutex::Autolock lock(*GetSelfLock());

    if ((!mOnlyIndeterminate || !mIndeterminate) && indeterminate != mIndeterminate) {
        mIndeterminate = indeterminate;

        if (indeterminate) {
            // swap between indeterminate and regular backgrounds
            mCurrentDrawable = mIndeterminateDrawable;
            StartAnimation();
        }
        else {
            mCurrentDrawable = mProgressDrawable;
            StopAnimation();
        }
    }

    return NOERROR;
}

/**
 * <p>Get the drawable used to draw the progress bar in
 * indeterminate mode.</p>
 *
 * @return a {@link android.graphics.drawable.Drawable} instance
 *
 * @see #setIndeterminateDrawable(android.graphics.drawable.Drawable)
 * @see #setIndeterminate(Boolean)
 */
AutoPtr<IDrawable> ProgressBar::GetIndeterminateDrawable()
{
    return mIndeterminateDrawable;
}

/**
 * <p>Define the drawable used to draw the progress bar in
 * indeterminate mode.</p>
 *
 * @param d the new drawable
 *
 * @see #getIndeterminateDrawable()
 * @see #setIndeterminate(Boolean)
 */
ECode ProgressBar::SetIndeterminateDrawable(
    /* [in] */ IDrawable* d)
{
    if (d != NULL) {
        d->SetCallback((IDrawableCallback*)this->Probe(EIID_IDrawableCallback));
    }
    mIndeterminateDrawable = d;
    if (mIndeterminate) {
        mCurrentDrawable = d;
        PostInvalidate();
    }

    return NOERROR;
}

/**
 * <p>Get the drawable used to draw the progress bar in
 * progress mode.</p>
 *
 * @return a {@link android.graphics.drawable.Drawable} instance
 *
 * @see #setProgressDrawable(android.graphics.drawable.Drawable)
 * @see #setIndeterminate(Boolean)
 */
AutoPtr<IDrawable> ProgressBar::GetProgressDrawable()
{
    return mProgressDrawable;
}

/**
 * <p>Define the drawable used to draw the progress bar in
 * progress mode.</p>
 *
 * @param d the new drawable
 *
 * @see #getProgressDrawable()
 * @see #setIndeterminate(Boolean)
 */
ECode ProgressBar::SetProgressDrawable(
    /* [in] */ IDrawable* d)
{
    if (d != NULL) {
        d->SetCallback((IDrawableCallback*)this->Probe(EIID_IDrawableCallback));

        // Make sure the ProgressBar is always tall enough
        Int32 drawableHeight;
        d->GetMinimumHeight(&drawableHeight);
        if (mMaxHeight < drawableHeight) {
            mMaxHeight = drawableHeight;
            RequestLayout();
        }
    }
    mProgressDrawable = d;
    if (!mIndeterminate) {
        mCurrentDrawable = d;
        PostInvalidate();
    }

    return NOERROR;
}

/**
 * @return The drawable currently used to draw the progress bar
 */
AutoPtr<IDrawable> ProgressBar::GetCurrentDrawable()
{
    return mCurrentDrawable;
}

Boolean ProgressBar::VerifyDrawable(
    /* [in] */ IDrawable* who)
{
    return who == mProgressDrawable || who == mIndeterminateDrawable
        || View::VerifyDrawable(who);
}

ECode ProgressBar::PostInvalidate()
{
    if (!mNoInvalidate) {
        View::PostInvalidate();
    }

    return NOERROR;
}

//synchronized
//
void ProgressBar::DoRefreshProgress(
    /* [in] */ Int32 id,
    /* [in] */ Int32 progress,
    /* [in] */ Boolean fromUser)
{
    Mutex::Autolock lock(*GetSelfLock());

    Float scale = mMax > 0 ? (Float)progress / (Float)mMax : 0;
    AutoPtr<IDrawable> d = mCurrentDrawable;
    if (d != NULL) {
        AutoPtr<IDrawable> progressDrawable;

//        if (d instanceof LayerDrawable) {
//            progressDrawable = ((LayerDrawable) d).findDrawableByLayerId(id);
//        }

        Int32 level = (Int32)(scale * MAX_LEVEL);

        Boolean tmp;
        (progressDrawable != NULL ? progressDrawable : d)->SetLevel(level, &tmp);
    }
    else {
        Invalidate();
    }

    if (id == 0x0102000d/*R.id.progress*/) {
        OnProgressRefresh(scale, fromUser);
    }
}

void ProgressBar::OnProgressRefresh(
    /* [in] */ Float scale,
    /* [in] */ Boolean fromUser)
{
}

//the caller of RefreshProgress already locks
//so, rename RefreshProgress to RefreshProgressLocked
void ProgressBar::RefreshProgressLocked(
    /* [in] */ Int32 id,
    /* [in] */ Int32 progress,
    /* [in] */ Boolean fromUser)
{
    if (mUiThreadId == pthread_self()) {
        DoRefreshProgress(id, progress, fromUser);
    }
    else {
        AutoPtr<RefreshProgressRunnable> r;
        if (mRefreshProgressRunnable != NULL) {
            // Use cached RefreshProgressRunnable if available
            r = mRefreshProgressRunnable;
            // Uncache it
            mRefreshProgressRunnable = NULL;
            r->Setup(id, progress, fromUser);
        }
        else {
            // Make a new one
            r = new RefreshProgressRunnable(id, progress, fromUser, this);
        }
        Post((IRunnable*)r);
    }
}

/**
 * <p>Set the current progress to the specified value. Does not do anything
 * if the progress bar is in indeterminate mode.</p>
 *
 * @param progress the new progress, between 0 and {@link #getMax()}
 *
 * @see #setIndeterminate(Boolean)
 * @see #isIndeterminate()
 * @see #getProgress()
 * @see #incrementProgressBy(Int32)
 */
//synchronized
//
ECode ProgressBar::SetProgress(
    /* [in] */ Int32 progress)
{
    Mutex::Autolock lock(*GetSelfLock());

    SetProgressLocked(progress, FALSE);

    return NOERROR;
}

//synchronized
//
void ProgressBar::SetProgress(
    /* [in] */ Int32 progress,
    /* [in] */ Boolean fromUser)
{
    Mutex::Autolock lock(*GetSelfLock());

    SetProgressLocked(progress, fromUser);
}

void ProgressBar::SetProgressLocked(
    /* [in] */ Int32 progress,
    /* [in] */ Boolean fromUser)
{
    if (mIndeterminate) {
        return;
    }

    if (progress < 0) {
        progress = 0;
    }

    if (progress > mMax) {
        progress = mMax;
    }

    if (progress != mProgress) {
        mProgress = progress;
        RefreshProgressLocked(0x0102000d/*R.id.progress*/, mProgress, fromUser);
    }
}

/**
 * <p>
 * Set the current secondary progress to the specified value. Does not do
 * anything if the progress bar is in indeterminate mode.
 * </p>
 *
 * @param secondaryProgress the new secondary progress, between 0 and {@link #getMax()}
 * @see #setIndeterminate(Boolean)
 * @see #isIndeterminate()
 * @see #getSecondaryProgress()
 * @see #incrementSecondaryProgressBy(Int32)
 */
//synchronized
//
ECode ProgressBar::SetSecondaryProgress(
    /* [in] */ Int32 secondaryProgress)
{
    Mutex::Autolock lock(*GetSelfLock());

    SetSecondaryProgressLocked(secondaryProgress);

    return NOERROR;
}

void ProgressBar::SetSecondaryProgressLocked(
    /* [in] */ Int32 secondaryProgress)
{
    if (mIndeterminate) {
        return;
    }

    if (secondaryProgress < 0) {
        secondaryProgress = 0;
    }

    if (secondaryProgress > mMax) {
        secondaryProgress = mMax;
    }

    if (secondaryProgress != mSecondaryProgress) {
        mSecondaryProgress = secondaryProgress;
        RefreshProgressLocked(0x0102000f/*R.id.secondaryProgress*/, mSecondaryProgress, FALSE);
    }
}

/**
 * <p>Get the progress bar's current level of progress. Return 0 when the
 * progress bar is in indeterminate mode.</p>
 *
 * @return the current progress, between 0 and {@link #getMax()}
 *
 * @see #setIndeterminate(Boolean)
 * @see #isIndeterminate()
 * @see #setProgress(Int32)
 * @see #setMax(Int32)
 * @see #getMax()
 */
//synchronized
//
Int32 ProgressBar::GetProgress()
{
    Mutex::Autolock lock(*GetSelfLock());

    return mIndeterminate ? 0 : mProgress;
}

/**
 * <p>Get the progress bar's current level of secondary progress. Return 0 when the
 * progress bar is in indeterminate mode.</p>
 *
 * @return the current secondary progress, between 0 and {@link #getMax()}
 *
 * @see #setIndeterminate(Boolean)
 * @see #isIndeterminate()
 * @see #setSecondaryProgress(Int32)
 * @see #setMax(Int32)
 * @see #getMax()
 */
//synchronized
//
Int32 ProgressBar::GetSecondaryProgress()
{
    Mutex::Autolock lock(*GetSelfLock());

    return mIndeterminate ? 0 : mSecondaryProgress;
}

/**
 * <p>Return the upper limit of this progress bar's range.</p>
 *
 * @return a positive integer
 *
 * @see #setMax(Int32)
 * @see #getProgress()
 * @see #getSecondaryProgress()
 */
//synchronized
//
Int32 ProgressBar::GetMax()
{
    Mutex::Autolock lock(*GetSelfLock());

    return mMax;
}

/**
 * <p>Set the range of the progress bar to 0...<tt>max</tt>.</p>
 *
 * @param max the upper range of this progress bar
 *
 * @see #getMax()
 * @see #setProgress(Int32)
 * @see #setSecondaryProgress(Int32)
 */
//synchronized
//
ECode ProgressBar::SetMax(
    /* [in] */ Int32 max)
{
    Mutex::Autolock lock(*GetSelfLock());

    if (max < 0) {
        max = 0;
    }
    if (max != mMax) {
        mMax = max;
        PostInvalidate();

        if (mProgress > max) {
            mProgress = max;
            RefreshProgressLocked(0x0102000d/*R.id.progress*/, mProgress, FALSE);
        }
    }

    return NOERROR;
}

/**
 * <p>Increase the progress bar's progress by the specified amount.</p>
 *
 * @param diff the amount by which the progress must be increased
 *
 * @see #setProgress(Int32)
 */
//synchronized
//
ECode ProgressBar::IncrementProgressBy(
    /* [in] */ Int32 diff)
{
    Mutex::Autolock lock(*GetSelfLock());

    SetProgressLocked(mProgress + diff, FALSE);

    return NOERROR;
}

/**
 * <p>Increase the progress bar's secondary progress by the specified amount.</p>
 *
 * @param diff the amount by which the secondary progress must be increased
 *
 * @see #setSecondaryProgress(Int32)
 */
//synchronized
//
ECode ProgressBar::IncrementSecondaryProgressBy(
    /* [in] */ Int32 diff)
{
    Mutex::Autolock lock(*GetSelfLock());

    SetSecondaryProgressLocked(mSecondaryProgress + diff);

    return NOERROR;
}

/**
 * <p>Start the indeterminate progress animation.</p>
 */
void ProgressBar::StartAnimation()
{
    if (GetVisibility() != VISIBLE) {
        return;
    }

    if (IAnimatable::Probe(mIndeterminateDrawable.Get())) {
        mShouldStartAnimationDrawable = TRUE;
        mAnimation = NULL;
    }
    else {
        if (mInterpolator == NULL) {
            ASSERT_SUCCEEDED(CLinearInterpolator::New((IInterpolator**)&mInterpolator));
        }

        ASSERT_SUCCEEDED(CTransformation::New((ITransformation**)&mTransformation));
        ASSERT_SUCCEEDED(CAlphaAnimation::New(0.0f, 1.0f, (IAnimation**)&mAnimation));
        mAnimation->SetRepeatMode(mBehavior);
        mAnimation->SetRepeatCount(Animation_INFINITE);
        mAnimation->SetDuration(mDuration);
        mAnimation->SetInterpolatorEx(mInterpolator);
        mAnimation->SetStartTime(Animation_START_ON_FIRST_FRAME);
        PostInvalidate();
    }
}

/**
 * <p>Stop the indeterminate progress animation.</p>
 */
void ProgressBar::StopAnimation()
{
    mAnimation = NULL;
    mTransformation = NULL;
    if (IAnimatable::Probe(mIndeterminateDrawable.Get())) {
        IAnimatable::Probe(mIndeterminateDrawable.Get())->Stop();
        mShouldStartAnimationDrawable = FALSE;
    }
}

/**
 * Sets the acceleration curve for the indeterminate animation.
 * The interpolator is loaded as a resource from the specified context.
 *
 * @param context The application environment
 * @param resID The resource identifier of the interpolator to load
 */
ECode ProgressBar::SetInterpolator(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resID)
{
    AutoPtr<IInterpolator> p;
    AnimationUtils::LoadInterpolator(context, resID, (IInterpolator**)&p);

    SetInterpolator(p);

    return NOERROR;
}

/**
 * Sets the acceleration curve for the indeterminate animation.
 * Defaults to a linear interpolation.
 *
 * @param interpolator The interpolator which defines the acceleration curve
 */
ECode ProgressBar::SetInterpolator(
    /* [in] */ IInterpolator* interpolator)
{
    mInterpolator = interpolator;

    return NOERROR;
}

/**
 * Gets the acceleration curve type for the indeterminate animation.
 *
 * @return the {@link Interpolator} associated to this animation
 */
AutoPtr<IInterpolator> ProgressBar::GetInterpolator()
{
    return mInterpolator;
}

ECode ProgressBar::SetVisibility(
    /* [in] */ Int32 v)
{
    if (GetVisibility() != v) {
        View::SetVisibility(v);

        if (mIndeterminate) {
            // let's be nice with the UI thread
            if (v == GONE || v == INVISIBLE) {
                StopAnimation();
            }
            else {
                StartAnimation();
            }
        }
    }

    return NOERROR;
}

void ProgressBar::OnVisibilityChanged(
    /* [in] */ IView* changedView,
    /* [in] */ Int32 visibility)
{
    View::OnVisibilityChanged(changedView, visibility);

    if (mIndeterminate) {
        // let's be nice with the UI thread
        if (visibility == GONE || visibility == INVISIBLE) {
            StopAnimation();
        }
        else {
            StartAnimation();
        }
    }
}

ECode ProgressBar::InvalidateDrawable(
    /* [in] */ IDrawable* dr)
{
    if (!mInDrawing) {
        if (VerifyDrawable(dr)) {
            AutoPtr<IRect> dirty;
            dr->GetBounds((IRect**)&dirty);
            Int32 scrollX = mScrollX + mPaddingLeft;
            Int32 scrollY = mScrollY + mPaddingTop;

            CRect* _dirty = (CRect*)dirty.Get();
            Invalidate(_dirty->mLeft + scrollX, _dirty->mTop + scrollY,
                    _dirty->mRight + scrollX, _dirty->mBottom + scrollY);
        }
        else {
            View::InvalidateDrawable(dr);
        }
    }

    return NOERROR;
}

void ProgressBar::OnSizeChanged(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 oldw,
    /* [in] */ Int32 oldh)
{
    // onDraw will translate the canvas so we draw starting at 0,0
    Int32 right = w - mPaddingRight - mPaddingLeft;
    Int32 bottom = h - mPaddingBottom - mPaddingTop;

    if (mIndeterminateDrawable != NULL) {
        mIndeterminateDrawable->SetBounds(0, 0, right, bottom);
    }

    if (mProgressDrawable != NULL) {
        mProgressDrawable->SetBounds(0, 0, right, bottom);
    }
}

//synchronized
//
void ProgressBar::OnDraw(
    /* [in] */ ICanvas* canvas)
{
    Mutex::Autolock lock(*GetSelfLock());

    View::OnDraw(canvas);

    AutoPtr<IDrawable> d = mCurrentDrawable;
    if (d.Get() != NULL) {
        // Translate canvas so a indeterminate circular progress bar with padding
        // rotates properly in its animation

        Int32 s;
        canvas->Save(&s);
        canvas->Translate(mPaddingLeft, mPaddingTop);
        Int64 time = GetDrawingTime();
        if (mAnimation != NULL) {
            Boolean res;
            mAnimation->GetTransformation(time, mTransformation, &res);
            Float scale;
            mTransformation->GetAlpha(&scale);
            mInDrawing = TRUE;
            d->SetLevel((Int32)(scale * MAX_LEVEL), &res);
            mInDrawing = FALSE;
            if (SystemClock::GetUptimeMillis() - mLastDrawTime >= ANIMATION_RESOLUTION) {
                mLastDrawTime = SystemClock::GetUptimeMillis();
                PostInvalidateDelayed(ANIMATION_RESOLUTION);
            }
        }
        d->Draw(canvas);
        canvas->Restore();
        if (mShouldStartAnimationDrawable && IAnimatable::Probe(d.Get())) {
            IAnimatable::Probe(d.Get())->Start();
            mShouldStartAnimationDrawable = FALSE;
        }
    }
}

//synchronized
//
void ProgressBar::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    Mutex::Autolock lock(*GetSelfLock());

    AutoPtr<IDrawable> d = mCurrentDrawable;

    Int32 dw = 0;
    Int32 dh = 0;
    if (d != NULL) {
        d->GetIntrinsicWidth(&dw);
        d->GetIntrinsicHeight(&dh);
        dw = Math::Max(mMinWidth, Math::Min(mMaxWidth, dw));
        dh = Math::Max(mMinHeight, Math::Min(mMaxHeight, dh));
    }
    dw += mPaddingLeft + mPaddingRight;
    dh += mPaddingTop + mPaddingBottom;

    SetMeasuredDimension(ResolveSize(dw, widthMeasureSpec),
            ResolveSize(dh, heightMeasureSpec));
}

ECode ProgressBar::DrawableStateChanged()
{
    View::DrawableStateChanged();

    ArrayOf<Int32>* state = GetDrawableState();

    Boolean stateful;
    if (mProgressDrawable != NULL && (mProgressDrawable->IsStateful(&stateful), stateful)) {
        mProgressDrawable->SetState(state, &stateful);
    }

    if (mIndeterminateDrawable != NULL && (mIndeterminateDrawable->IsStateful(&stateful), stateful)) {
        mIndeterminateDrawable->SetState(state, &stateful);
    }

    return NOERROR;
}

AutoPtr<IParcelable> ProgressBar::OnSaveInstanceState()
{
    // Force our ancestor class to save its state
    AutoPtr<IParcelable> superState = View::OnSaveInstanceState();
    AutoPtr<CProgressBarSavedState> ss;
    CProgressBarSavedState::NewByFriend(superState, (CProgressBarSavedState**)&ss);

    ss->mProgress = mProgress;
    ss->mSecondaryProgress = mSecondaryProgress;

    return (IParcelable*)ss->Probe(EIID_IParcelable);
}

void ProgressBar::OnRestoreInstanceState(
    /* [in] */ IParcelable* state)
{
    IProgressBarSavedState* ss = IProgressBarSavedState::Probe(state);

    AutoPtr<IParcelable> p;
    ss->GetSuperState((IParcelable**)&p);
    View::OnRestoreInstanceState(p);

    SetProgress(((CProgressBarSavedState*)ss)->mProgress);
    SetSecondaryProgress(((CProgressBarSavedState*)ss)->mSecondaryProgress);
}

void ProgressBar::OnAttachedToWindow()
{
    View::OnAttachedToWindow();
    if (mIndeterminate) {
        StartAnimation();
    }
}

void ProgressBar::OnDetachedFromWindow()
{
    View::OnDetachedFromWindow();
    if (mIndeterminate) {
        StopAnimation();
    }
}

