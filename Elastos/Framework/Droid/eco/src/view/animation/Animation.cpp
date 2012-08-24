
#include "view/animation/Animation.h"
#include "view/animation/AnimationUtils.h"
#include "view/animation/CTransformation.h"
#include "view/animation/CAccelerateDecelerateInterpolator.h"
#include "graphics/CRectF.h"
#include "utils/CTypedValue.h"
#include <elastos/Math.h>
#include <Logger.h>

using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

extern "C" const InterfaceID EIID_Animation =
    {0x483e7556, 0x8aff, 0x4b89, {0x8e, 0xf8, 0x3a, 0x57, 0x39, 0x6f, 0x59, 0xe6}};

/**
 * Size descriptions can appear inthree forms:
 * <ol>
 * <li>An absolute size. This is represented by a number.</li>
 * <li>A size relative to the size of the object being animated. This
 * is represented by a number followed by "%".</li> *
 * <li>A size relative to the size of the parent of object being
 * animated. This is represented by a number followed by "%p".</li>
 * </ol>
 * @param value The typed value to parse
 * @return The parsed version of the description
 */
AutoPtr<Animation::Description> Animation::Description::ParseValue(
    /* [in] */ ITypedValue* value)
{
    AutoPtr<Description> d = new Description();
    if (value == NULL) {
        d->mType = Animation_ABSOLUTE;
        d->mValue = 0;
    }
    else {
        Int32 type = ((CTypedValue*)value)->mType;
        if (type == TypedValue_TYPE_FRACTION) {
            d->mType = (((CTypedValue*)value)->mData & TypedValue_COMPLEX_UNIT_MASK)
                == TypedValue_COMPLEX_UNIT_FRACTION_PARENT ?
                Animation_RELATIVE_TO_PARENT : Animation_RELATIVE_TO_SELF;
            d->mValue = CTypedValue::ComplexToFloat(((CTypedValue*)value)->mData);
            return d;
        }
        else if (type ==TypedValue_TYPE_FLOAT) {
            d->mType = Animation_ABSOLUTE;
            value->GetFloat(&d->mValue);
            return d;
        }
        else if (type >= TypedValue_TYPE_FIRST_INT &&
            type <= TypedValue_TYPE_LAST_INT) {
            d->mType = Animation_ABSOLUTE;
            d->mValue = ((CTypedValue*)value)->mData;
            return d;
        }
    }

    d->mType = Animation_ABSOLUTE;
    d->mValue = 0.0f;

    return d;
}

/**
 * Creates a new animation with a duration of 0ms, the default interpolator, with
 * fillBefore Set to TRUE and fillAfter Set to FALSE
 */
Animation::Animation()
    : mEnded(FALSE)
    , mStarted(FALSE)
    , mCycleFlip(FALSE)
    , mInitialized(FALSE)
    , mFillBefore(TRUE)
    , mFillAfter(FALSE)
    , mFillEnabled(FALSE)
    , mStartTime(-1)
    , mRepeatCount(0)
    , mRepeated(0)
    , mRepeatMode(Animation_RESTART)
    , mDetachWallpaper(FALSE)
    , mMore(TRUE)
    , mOneMoreTime(TRUE)
{
    AutoPtr<CRectF> rectF;
    ASSERT_SUCCEEDED(CRectF::NewByFriend((CRectF**)&rectF));
    mPreviousRegion = (IRectF*)rectF.Get();
    rectF = NULL;
    ASSERT_SUCCEEDED(CRectF::NewByFriend((CRectF**)&rectF));
    mRegion = (IRectF*)rectF.Get();

    AutoPtr<CTransformation> trans;
    ASSERT_SUCCEEDED(CTransformation::NewByFriend((CTransformation**)&trans));
    mTransformation = (ITransformation*)trans.Get();
    trans = NULL;
    ASSERT_SUCCEEDED(CTransformation::NewByFriend((CTransformation**)&trans));
    mPreviousTransformation = (ITransformation*)trans.Get();

    EnsureInterpolator();
}

/**
 * Creates a new animation whose parameters come from the specified context and
 * attributes Set.
 *
 * @param context the application environment
 * @param attrs the Set of attributes holding the animation parameters
 */
Animation::Animation(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : mEnded(FALSE)
    , mStarted(FALSE)
    , mCycleFlip(FALSE)
    , mInitialized(FALSE)
    , mFillBefore(TRUE)
    , mFillAfter(FALSE)
    , mFillEnabled(FALSE)
    , mStartTime(-1)
    , mRepeatCount(0)
    , mRepeated(0)
    , mRepeatMode(Animation_RESTART)
    , mDetachWallpaper(FALSE)
    , mMore(TRUE)
    , mOneMoreTime(TRUE)
{
    ASSERT_SUCCEEDED(CRectF::New((IRectF**)&mPreviousRegion));
    ASSERT_SUCCEEDED(CRectF::New((IRectF**)&mRegion));
    ASSERT_SUCCEEDED(CTransformation::New((ITransformation**)&mTransformation));
    ASSERT_SUCCEEDED(CTransformation::New((ITransformation**)&mPreviousTransformation));

    Init(context, attrs);

    EnsureInterpolator();
}

//@Override
//Animation Clone()
//{
//    final Animation animation = (Animation) super.clone();
//    animation.mPreviousRegion = new RectF();
//    animation.mRegion = new RectF();
//    animation.mTransformation = new Transformation();
//    animation.mPreviousTransformation = new Transformation();
//    return animation;
//}

/**
 * Reset the initialization state of this animation.
 *
 * @see #initialize(Int32, Int32, Int32, Int32)
 */
ECode Animation::Reset()
{
    mPreviousRegion->SetEmpty();
    mPreviousTransformation->Clear();
    mInitialized = FALSE;
    mCycleFlip = FALSE;
    mRepeated = 0;
    mMore = TRUE;
    mOneMoreTime = TRUE;

    return NOERROR;
}

/**
 * Cancel the animation. Cancelling an animation invokes the animation
 * listener, if Set, to notify the end of the animation.
 *
 * If you cancel an animation manually, you must call {@link #reset()}
 * before starting the animation again.
 *
 * @see #reset()
 * @see #start()
 * @see #startNow()
 */
ECode Animation::Cancel()
{
    if (mStarted && !mEnded) {
        if (mListener != NULL) {
            mListener->OnAnimationEnd((IAnimation*)this->Probe(EIID_IAnimation));
        }
        mEnded = TRUE;
    }
    // Make sure we move the animation to the end
    mStartTime = Math::INT64_MIN_VALUE;
    mMore = mOneMoreTime = FALSE;

    return NOERROR;
}

/**
 * @hide
 */
ECode Animation::Detach()
{
    if (mStarted && !mEnded) {
        mEnded = TRUE;
        if (mListener != NULL) {
            mListener->OnAnimationEnd((IAnimation*)this->Probe(EIID_IAnimation));
        }
    }

    return NOERROR;
}

/**
 * Whether or not the animation has been initialized.
 *
 * @return Has this animation been initialized.
 * @see #initialize(Int32, Int32, Int32, Int32)
 */
Boolean Animation::IsInitialized()
{
    return mInitialized;
}

/**
 * Initialize this animation with the dimensions of the object being
 * animated as well as the objects parents. (This is to support animation
 * sizes being specifed relative to these dimensions.)
 *
 * <p>Objects that interpret a Animations should call this method when
 * the sizes of the object being animated and its parent are known, and
 * before calling {@link #getTransformation}.
 *
 *
 * @param width Width of the object being animated
 * @param height Height of the object being animated
 * @param parentWidth Width of the animated object's parent
 * @param parentHeight Height of the animated object's parent
 */
ECode Animation::Initialize(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 parentWidth,
    /* [in] */ Int32 parentHeight)
{
    Reset();
    mInitialized = TRUE;

    return NOERROR;
}

/**
 * Sets the acceleration curve for this animation. The interpolator is loaded as
 * a resource from the specified context.
 *
 * @param context The application environment
 * @param resID The resource identifier of the interpolator to load
 * @attr ref android.R.styleable#Animation_interpolator
 */
ECode Animation::SetInterpolator(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resID)
{
    AutoPtr<IInterpolator> interpolator;
    AnimationUtils::LoadInterpolator(context, resID, (IInterpolator**)&interpolator);
    return SetInterpolator(interpolator);
}

/**
 * Sets the acceleration curve for this animation. Defaults to a linear
 * interpolation.
 *
 * @param i The interpolator which defines the acceleration curve
 * @attr ref android.R.styleable#Animation_interpolator
 */
ECode Animation::SetInterpolator(
    /* [in] */ IInterpolator* i)
{
    mInterpolator = i;
    return NOERROR;
}

/**
 * When this animation should start relative to the start time. This is most
 * useful when composing complex animations using an {@link AnimationSet }
 * where some of the animations components start at different times.
 *
 * @param startOffset When this Animation should start, in milliseconds from
 *                    the start time of the root AnimationSet.
 * @attr ref android.R.styleable#Animation_startOffset
 */
ECode Animation::SetStartOffset(
    /* [in] */ Int64 startOffset)
{
    mStartOffset = startOffset;

    return NOERROR;
}

/**
 * How Int64 this animation should last. The duration cannot be negative.
 *
 * @param durationMillis Duration in milliseconds
 *
 * @throws java.lang.IllegalArgumentException if the duration is < 0
 *
 * @attr ref android.R.styleable#Animation_duration
 */
ECode Animation::SetDuration(
    /* [in] */ Int64 durationMillis)
{
    if (durationMillis < 0) {
        Logger::E("Animation", "Animation duration cannot be negative");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mDuration = durationMillis;

    return NOERROR;
}

/**
 * Ensure that the duration that this animation will run is not longer
 * than <var>durationMillis</var>.  In addition to adjusting the duration
 * itself, this ensures that the repeat count also will not make it run
 * longer than the given time.
 *
 * @param durationMillis The maximum duration the animation is allowed
 * to run.
 */
ECode Animation::RestrictDuration(
    /* [in] */ Int64 durationMillis)
{
    // If we start after the duration, then we just won't run.
    if (mStartOffset > durationMillis) {
        mStartOffset = durationMillis;
        mDuration = 0;
        mRepeatCount = 0;
        return NOERROR;
    }

    Int64 dur = mDuration + mStartOffset;
    if (dur > durationMillis) {
        mDuration = durationMillis - mStartOffset;
        dur = durationMillis;
    }
    // If the duration is 0 or less, then we won't run.
    if (mDuration <= 0) {
        mDuration = 0;
        mRepeatCount = 0;
        return NOERROR;
    }
    // Reduce the number of repeats to keep below the maximum duration.
    // The comparison between mRepeatCount and duration is to catch
    // overflows after multiplying them.
    if (mRepeatCount < 0 || mRepeatCount > durationMillis
            || (dur*mRepeatCount) > durationMillis) {
        // Figure out how many times to do the animation.  Subtract 1 since
        // repeat count is the number of times to repeat so 0 runs once.
        mRepeatCount = (Int32)(durationMillis/dur) - 1;
        if (mRepeatCount < 0) {
            mRepeatCount = 0;
        }
    }

    return NOERROR;
}

/**
 * How much to scale the duration by.
 *
 * @param scale The amount to scale the duration.
 */
ECode Animation::ScaleCurrentDuration(
    /* [in] */ Float scale)
{
    mDuration = (Int64)(mDuration * scale);

    return NOERROR;
}

/**
 * When this animation should start. When the start time is Set to
 * {@link #START_ON_FIRST_FRAME}, the animation will start the first time
 * {@link #getTransformation(Int64, Transformation)} is invoked. The time passed
 * to this method should be obtained by calling
 * {@link AnimationUtils#currentAnimationTimeMillis()} instead of
 * {@link System#currentTimeMillis()}.
 *
 * @param startTimeMillis the start time in milliseconds
 */
ECode Animation::SetStartTime(
    /* [in] */ Int64 startTimeMillis)
{
    mStartTime = startTimeMillis;
    mStarted = mEnded = FALSE;
    mCycleFlip = FALSE;
    mRepeated = 0;
    mMore = TRUE;

    return NOERROR;
}

/**
 * Convenience method to start the animation the first time
 * {@link #getTransformation(Int64, Transformation)} is invoked.
 */
ECode Animation::Start()
{
    return SetStartTime(-1);
}

/**
 * Convenience method to start the animation at the current time in
 * milliseconds.
 */
ECode Animation::StartNow()
{
    return SetStartTime(AnimationUtils::CurrentAnimationTimeMillis());
}

/**
 * Defines what this animation should do when it reaches the end. This
 * Setting is applied only when the repeat count is either greater than
 * 0 or {@link #INFINITE}. Defaults to {@link #Animation_RESTART}.
 *
 * @param repeatMode {@link #Animation_RESTART} or {@link #REVERSE}
 * @attr ref android.R.styleable#Animation_repeatMode
 */
ECode Animation::SetRepeatMode(
    /* [in] */ Int32 repeatMode)
{
    mRepeatMode = repeatMode;
    return NOERROR;
}

/**
 * Sets how many times the animation should be repeated. If the repeat
 * count is 0, the animation is never repeated. If the repeat count is
 * greater than 0 or {@link #INFINITE}, the repeat mode will be taken
 * into account. The repeat count is 0 by default.
 *
 * @param repeatCount the number of times the animation should be repeated
 * @attr ref android.R.styleable#Animation_repeatCount
 */
ECode Animation::SetRepeatCount(
    /* [in] */ Int32 repeatCount)
{
    if (repeatCount < 0) {
        repeatCount = Animation_INFINITE;
    }
    mRepeatCount = repeatCount;

    return NOERROR;
}

/**
 * If fillEnabled is TRUE, this animation will apply fillBefore and fillAfter.
 *
 * @return TRUE if the animation will take fillBefore and fillAfter into account
 * @attr ref android.R.styleable#Animation_fillEnabled
 */
Boolean Animation::IsFillEnabled()
{
    return mFillEnabled;
}

/**
 * If fillEnabled is TRUE, the animation will apply the value of fillBefore and
 * fillAfter. Otherwise, fillBefore and fillAfter are ignored and the animation
 * transformation is always applied.
 *
 * @param fillEnabled TRUE if the animation should take fillBefore and fillAfter into account
 * @attr ref android.R.styleable#Animation_fillEnabled
 *
 * @see #setFillBefore(Boolean)
 * @see #setFillAfter(Boolean)
 */
ECode Animation::SetFillEnabled(
    /* [in] */ Boolean fillEnabled)
{
    mFillEnabled = fillEnabled;
    return NOERROR;
}

/**
 * If fillBefore is TRUE, this animation will apply its transformation
 * before the start time of the animation. Defaults to TRUE if not Set.
 * Note that this applies when using an {@link
 * android.view.animation.AnimationSet AnimationSet} to chain
 * animations. The transformation is not applied before the AnimationSet
 * itself starts.
 *
 * @param fillBefore TRUE if the animation should apply its transformation before it starts
 * @attr ref android.R.styleable#Animation_fillBefore
 *
 * @see #setFillEnabled(Boolean)
 */
ECode Animation::SetFillBefore(
    /* [in] */ Boolean fillBefore)
{
    mFillBefore = fillBefore;
    return NOERROR;
}

/**
 * If fillAfter is TRUE, the transformation that this animation performed
 * will persist when it is finished. Defaults to FALSE if not Set.
 * Note that this applies when using an {@link
 * android.view.animation.AnimationSet AnimationSet} to chain
 * animations. The transformation is not applied before the AnimationSet
 * itself starts.
 *
 * @param fillAfter TRUE if the animation should apply its transformation after it ends
 * @attr ref android.R.styleable#Animation_fillAfter
 *
 * @see #setFillEnabled(Boolean)
 */
ECode Animation::SetFillAfter(
    /* [in] */ Boolean fillAfter)
{
    mFillAfter = fillAfter;
    return NOERROR;
}

/**
 * Set the Z ordering mode to use while running the animation.
 *
 * @param zAdjustment The desired mode, one of {@link #ZORDER_NORMAL},
 * {@link #ZORDER_TOP}, or {@link #ZORDER_BOTTOM}.
 * @attr ref android.R.styleable#Animation_zAdjustment
 */
ECode Animation::SetZAdjustment(
    /* [in] */ Int32 zAdjustment)
{
    mZAdjustment = zAdjustment;
    return NOERROR;
}

/**
 * If detachWallpaper is TRUE, and this is a window animation of a window
 * that has a wallpaper background, then the window will be detached from
 * the wallpaper while it runs.  That is, the animation will only be applied
 * to the window, and the wallpaper behind it will remain static.
 *
 * @param detachWallpaper TRUE if the wallpaper should be detached from the animation
 * @attr ref android.R.styleable#Animation_detachWallpaper
 */
ECode Animation::SetDetachWallpaper(
    /* [in] */ Boolean detachWallpaper)
{
    mDetachWallpaper = detachWallpaper;
    return NOERROR;
}

/**
 * Gets the acceleration curve type for this animation.
 *
 * @return the {@link Interpolator} associated to this animation
 * @attr ref android.R.styleable#Animation_interpolator
 */
AutoPtr<IInterpolator> Animation::GetInterpolator()
{
    return mInterpolator;
}

/**
 * When this animation should start. If the animation has not startet yet,
 * this method might return {@link #START_ON_FIRST_FRAME}.
 *
 * @return the time in milliseconds when the animation should start or
 *         {@link #START_ON_FIRST_FRAME}
 */
Int64 Animation::GetStartTime()
{
    return mStartTime;
}

/**
 * How Int64 this animation should last
 *
 * @return the duration in milliseconds of the animation
 * @attr ref android.R.styleable#Animation_duration
 */
Int64 Animation::GetDuration()
{
    return mDuration;
}

/**
 * When this animation should start, relative to StartTime
 *
 * @return the start offset in milliseconds
 * @attr ref android.R.styleable#Animation_startOffset
 */
Int64 Animation::GetStartOffset()
{
    return mStartOffset;
}

/**
 * Defines what this animation should do when it reaches the end.
 *
 * @return either one of {@link #REVERSE} or {@link #Animation_RESTART}
 * @attr ref android.R.styleable#Animation_repeatMode
 */
Int32 Animation::GetRepeatMode()
{
    return mRepeatMode;
}

/**
 * Defines how many times the animation should repeat. The default value
 * is 0.
 *
 * @return the number of times the animation should repeat, or {@link #INFINITE}
 * @attr ref android.R.styleable#Animation_repeatCount
 */
Int32 Animation::GetRepeatCount()
{
    return mRepeatCount;
}

/**
 * If fillBefore is TRUE, this animation will apply its transformation
 * before the start time of the animation.
 *
 * @return TRUE if the animation applies its transformation before it starts
 * @attr ref android.R.styleable#Animation_fillBefore
 */
Boolean Animation::GetFillBefore()
{
    return mFillBefore;
}

/**
 * If fillAfter is TRUE, this animation will apply its transformation
 * after the end time of the animation.
 *
 * @return TRUE if the animation applies its transformation after it ends
 * @attr ref android.R.styleable#Animation_fillAfter
 */
Boolean Animation::GetFillAfter()
{
    return mFillAfter;
}

/**
 * Returns the Z ordering mode to use while running the animation as
 * previously Set by {@link #setZAdjustment}.
 *
 * @return Returns one of {@link #ZORDER_NORMAL},
 * {@link #ZORDER_TOP}, or {@link #ZORDER_BOTTOM}.
 * @attr ref android.R.styleable#Animation_zAdjustment
 */
Int32 Animation::GetZAdjustment()
{
    return mZAdjustment;
}

/**
 * Return value of {@link #setDetachWallpaper(Boolean)}.
 * @attr ref android.R.styleable#Animation_detachWallpaper
 */
Boolean Animation::GetDetachWallpaper()
{
    return mDetachWallpaper;
}

/**
 * <p>Indicates whether or not this animation will affect the transformation
 * matrix. For instance, a fade animation will not affect the matrix whereas
 * a scale animation will.</p>
 *
 * @return TRUE if this animation will change the transformation matrix
 */
Boolean Animation::WillChangeTransformationMatrix()
{
    // assume we will change the matrix
    return TRUE;
}

/**
 * <p>Indicates whether or not this animation will affect the bounds of the
 * animated view. For instance, a fade animation will not affect the bounds
 * whereas a 200% scale animation will.</p>
 *
 * @return TRUE if this animation will change the view's bounds
 */
Boolean Animation::WillChangeBounds()
{
    // assume we will change the bounds
    return TRUE;
}

/**
 * <p>Binds an animation listener to this animation. The animation listener
 * is notified of animation events such as the end of the animation or the
 * repetition of the animation.</p>
 *
 * @param listener the animation listener to be notified
 */
ECode Animation::SetAnimationListener(
    /* [in] */ IAnimationListener* listener)
{
    mListener = listener;

    return NOERROR;
}

/**
 * Gurantees that this animation has an interpolator. Will use
 * a AccelerateDecelerateInterpolator is nothing else was specified.
 */
void Animation::EnsureInterpolator()
{
    if (mInterpolator == NULL) {
        AutoPtr<CAccelerateDecelerateInterpolator> temp;
        CAccelerateDecelerateInterpolator::NewByFriend(
            (CAccelerateDecelerateInterpolator**)&temp);
        mInterpolator = (IInterpolator*)temp.Get();
    }
}

/**
 * Compute a hint at how Int64 the entire animation may last, in milliseconds.
 * Animations can be written to cause themselves to run for a different
 * duration than what is computed here, but generally this should be
 * accurate.
 */
Int64 Animation::ComputeDurationHint()
{
    return (GetStartOffset() + GetDuration()) * (GetRepeatCount() + 1);
}

/**
 * Gets the transformation to apply at a specified point in time. Implementations of this
 * method should always replace the specified Transformation or document they are doing
 * otherwise.
 *
 * @param currentTime Where we are in the animation. This is wall clock time.
 * @param outTransformation A tranformation object that is provided by the
 *        caller and will be filled in by the animation.
 * @return True if the animation is still running
 */
Boolean Animation::GetTransformation(
    /* [in] */ Int64 currentTime,
    /* [in] */ ITransformation* outTransformation)
{
    if (mStartTime == -1) {
        mStartTime = currentTime;
    }

    Int64 startOffset = GetStartOffset();
    Int64 duration = mDuration;
    Float normalizedTime;
    if (duration != 0) {
        normalizedTime = ((Float)(currentTime - (mStartTime + startOffset))) /
                (Float)duration;
    }
    else {
        // time is a step-change with a zero duration
        normalizedTime = currentTime < mStartTime ? 0.0f : 1.0f;
    }

    Boolean expired = normalizedTime >= 1.0f;
    mMore = !expired;

    if (!mFillEnabled) {
        normalizedTime = Math::Max(Math::Min(normalizedTime, 1.0f), 0.0f);
    }

    if ((normalizedTime >= 0.0f || mFillBefore)
        && (normalizedTime <= 1.0f || mFillAfter)) {
        if (!mStarted) {
            if (mListener != NULL) {
                mListener->OnAnimationStart((IAnimation*)this->Probe(EIID_IAnimation));
            }
            mStarted = TRUE;
        }

        if (mFillEnabled) {
            normalizedTime = Math::Max(Math::Min(normalizedTime, 1.0f), 0.0f);
        }

        if (mCycleFlip) {
            normalizedTime = 1.0f - normalizedTime;
        }

        Float interpolatedTime;
        mInterpolator->GetInterpolation(normalizedTime, &interpolatedTime);
        ApplyTransformation(interpolatedTime, outTransformation);
    }

    if (expired) {
        if (mRepeatCount == mRepeated) {
            if (!mEnded) {
                mEnded = TRUE;
                if (mListener != NULL) {
                    mListener->OnAnimationEnd((IAnimation*)this->Probe(EIID_IAnimation));
                }
            }
        }
        else {
            if (mRepeatCount > 0) {
                mRepeated++;
            }

            if (mRepeatMode == Animation_REVERSE) {
                mCycleFlip = !mCycleFlip;
            }

            mStartTime = -1;
            mMore = TRUE;

            if (mListener != NULL) {
                mListener->OnAnimationRepeat((IAnimation*)this->Probe(EIID_IAnimation));
            }
        }
    }

    if (!mMore && mOneMoreTime) {
        mOneMoreTime = FALSE;
        return TRUE;
    }

    return mMore;
}

/**
 * <p>Indicates whether this animation has started or not.</p>
 *
 * @return TRUE if the animation has started, FALSE otherwise
 */
Boolean Animation::HasStarted()
{
    return mStarted;
}

/**
 * <p>Indicates whether this animation has ended or not.</p>
 *
 * @return TRUE if the animation has ended, FALSE otherwise
 */
Boolean Animation::HasEnded() {
    return mEnded;
}

/**
 * Helper for getTransformation. Subclasses should implement this to apply
 * their transforms given an interpolation value.  Implementations of this
 * method should always replace the specified Transformation or document
 * they are doing otherwise.
 *
 * @param interpolatedTime The value of the normalized time (0.0 to 1.0)
 *        after it has been run through the interpolation function.
 * @param t The Transofrmation object to fill in with the current
 *        transforms.
 */
void Animation::ApplyTransformation(
    /* [in] */ Float interpolatedTime,
    /* [in] */ ITransformation* t)
{
}

/**
 * Convert the information in the description of a size to an actual
 * dimension
 *
 * @param type One of Animation.ABSOLUTE, Animation.RELATIVE_TO_SELF, or
 *             Animation.RELATIVE_TO_PARENT.
 * @param value The dimension associated with the type parameter
 * @param size The size of the object being animated
 * @param parentSize The size of the parent of the object being animated
 * @return The dimension to use for the animation
 */
Float Animation::ResolveSize(
    /* [in] */ Int32 type,
    /* [in] */ Float value,
    /* [in] */ Int32 size,
    /* [in] */ Int32 parentSize)
{
    switch (type) {
        case Animation_ABSOLUTE:
            return value;
        case Animation_RELATIVE_TO_SELF:
            return size * value;
        case Animation_RELATIVE_TO_PARENT:
            return parentSize * value;
        default:
            return value;
    }
}

/**
 * @param left
 * @param top
 * @param right
 * @param bottom
 * @param invalidate
 * @param transformation
 *
 * @hide
 */
ECode Animation::GetInvalidateRegion(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom,
    /* [in] */ IRectF* invalidate,
    /* [in] */ ITransformation* transformation)
{
    IRectF* tempRegion = mRegion;
    IRectF* previousRegion = mPreviousRegion;

    invalidate->Set(left, top, right, bottom);

    AutoPtr<IMatrix> matrix;
    transformation->GetMatrix((IMatrix**)&matrix);
    Boolean res;
    matrix->MapRectEx(invalidate, &res);

    // Enlarge the invalidate region to account for rounding errors
    invalidate->Inset(-1.0f, -1.0f);
    tempRegion->SetEx(invalidate);
    invalidate->UnionEx(previousRegion);

    previousRegion->SetEx(tempRegion);

    mTransformation->Set(transformation);
    transformation->Set(mPreviousTransformation);
    mPreviousTransformation->Set(mTransformation);

    return NOERROR;
}

/**
 * @param left
 * @param top
 * @param right
 * @param bottom
 *
 * @hide
 */
ECode Animation::InitializeInvalidateRegion(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    IRectF* region = mPreviousRegion;
    region->Set(left, top, right, bottom);
    // Enlarge the invalidate region to account for rounding errors
    region->Inset(-1.0f, -1.0f);
    if (mFillBefore) {
        Float time;
        mInterpolator->GetInterpolation(0.0f, &time);
        ApplyTransformation(time, mPreviousTransformation);
    }

    return NOERROR;
}

static Int32 R_Styleable_Animation[] = {
    0x01010141, 0x01010198, 0x010101bc, 0x010101bd,
    0x010101be, 0x010101bf, 0x010101c0, 0x010101c1,
    0x0101024f, 0x010102a6
};

/**
 * Creates a new animation whose parameters come from the specified context and
 * attributes Set.
 *
 * @param context the application environment
 * @param attrs the Set of attributes holding the animation parameters
 */
ECode Animation::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributesEx2(attrs, ArrayOf<Int32>(R_Styleable_Animation,
        sizeof(R_Styleable_Animation) / sizeof(Int32))/*com.android.internal.R.styleable.Animation*/,
        (ITypedArray**)&a);

    Int32 data;
    a->GetInt32(1/*com.android.internal.R.styleable.Animation_duration*/, 0, &data);
    SetDuration((Int64)data);
    a->GetInt32(4/*com.android.internal.R.styleable.Animation_startOffset*/, 0, &data);
    SetStartOffset((Int64)data);

    Boolean flag;
    a->GetBoolean(
        8/*com.android.internal.R.styleable.Animation_fillEnabled*/,
        mFillEnabled, &flag);
    SetFillEnabled(flag);
    a->GetBoolean(
        2/*com.android.internal.R.styleable.Animation_fillBefore*/,
        mFillBefore, &flag);
    SetFillBefore(flag);
    a->GetBoolean(
        3/*com.android.internal.R.styleable.Animation_fillAfter*/,
        mFillAfter, &flag);
    SetFillAfter(flag);

    Int32 resID;
    a->GetResourceId(
        0/*com.android.internal.R.styleable.Animation_interpolator*/, 0, &resID);
    if (resID > 0) {
        SetInterpolator(context, resID);
    }

    a->GetInt32(
        5/*com.android.internal.R.styleable.Animation_repeatCount*/,
        mRepeatCount, &data);
    SetRepeatCount(data);
    a->GetInt32(
        6/*com.android.internal.R.styleable.Animation_repeatMode*/,
        Animation_RESTART, &data);
    SetRepeatMode(data);
    a->GetInt32(
        7/*com.android.internal.R.styleable.Animation_zAdjustment*/,
        Animation_ZORDER_NORMAL, &data);
    SetZAdjustment(data);
    a->GetBoolean(
        9/*com.android.internal.R.styleable.Animation_detachWallpaper*/,
        FALSE, &flag);
    SetDetachWallpaper(flag);

    a->Recycle();

    return NOERROR;
}
