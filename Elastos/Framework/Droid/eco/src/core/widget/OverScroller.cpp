
#include "widget/OverScroller.h"
#include "utils/CDisplayMetrics.h"
#include "os/SystemClock.h"
#include "view/ViewConfiguration.h"
#include "view/animation/AnimationUtils.h"
#include "widget/Scroller.h"

const Int32 OverScroller::DEFAULT_DURATION;
const Int32 OverScroller::SCROLL_MODE;
const Int32 OverScroller::FLING_MODE;

Float OverScroller::MagneticOverScroller::GRAVITY;
const Int32 OverScroller::MagneticOverScroller::TO_EDGE;
const Int32 OverScroller::MagneticOverScroller::TO_BOUNDARY;
const Int32 OverScroller::MagneticOverScroller::TO_BOUNCE;
const Int32 OverScroller::MagneticOverScroller::OVERSCROLL_SPRINGBACK_DURATION;
const Float OverScroller::MagneticOverScroller::TIME_COEF =
    1000.0f * (Float)Math::DOUBLE_PI / OverScroller::MagneticOverScroller::OVERSCROLL_SPRINGBACK_DURATION;
const Float OverScroller::MagneticOverScroller::MINIMUM_VELOCITY_FOR_BOUNCE = Math::FLOAT_MAX_VALUE;
const Float OverScroller::MagneticOverScroller::DEFAULT_BOUNCE_COEFFICIENT;

void OverScroller::MagneticOverScroller::InitializeFromContext(
    /* [in] */ IContext* context)
{
    AutoPtr<IResources> r;
    context->GetResources((IResources**)&r);
    AutoPtr<IDisplayMetrics> dm;
    r->GetDisplayMetrics((IDisplayMetrics**)&dm);
    Float ppi = ((CDisplayMetrics*)dm.Get())->mDensity * 160.0f;
    const Int32 GRAVITY_EARTH = 9.80665f;
    GRAVITY = GRAVITY_EARTH //SensorManager.GRAVITY_EARTH // g (m/s^2)
            * 39.37f // inch/meter
            * ppi // pixels per inch
            * ViewConfiguration::GetScrollFriction();
}

OverScroller::MagneticOverScroller::MagneticOverScroller()
    : mFinished(TRUE)
    , mState(TO_EDGE)
    , mBounceCoefficient(DEFAULT_BOUNCE_COEFFICIENT)
{
}

void OverScroller::MagneticOverScroller::UpdateScroll(
    /* [in] */ Float q)
{
    mCurrentPosition = mStart + Math::Round(q * (mFinal - mStart));
}

/*
 * Get a signed deceleration that will reduce the velocity.
 */
Float OverScroller::MagneticOverScroller::GetDeceleration(
    /* [in] */ Int32 velocity)
{
    return velocity > 0 ? -GRAVITY : GRAVITY;
}

/*
 * Returns the time (in milliseconds) it will take to go from start to end.
 */
Int32 OverScroller::MagneticOverScroller::ComputeDuration(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Float initialVelocity,
    /* [in] */ Float deceleration)
{
    Int32 distance = start - end;
    Float discriminant = initialVelocity * initialVelocity - 2.0f * deceleration
            * distance;
    if (discriminant >= 0.0f) {
        Float delta = (Float)Math::Sqrt(discriminant);
        if (deceleration < 0.0f) {
            delta = -delta;
        }

        return (Int32)(1000.0f * (-initialVelocity - delta) / deceleration);
    }

    // End position can not be reached
    return 0;
}

void OverScroller::MagneticOverScroller::StartScroll(
    /* [in] */ Int32 start,
    /* [in] */ Int32 distance,
    /* [in] */ Int32 duration)
{
    mFinished = FALSE;

    mStart = start;
    mFinal = start + distance;

    mStartTime = AnimationUtils::CurrentAnimationTimeMillis();
    mDuration = duration;

    // Unused
    mDeceleration = 0.0f;
    mVelocity = 0;
}

void OverScroller::MagneticOverScroller::Fling(
    /* [in] */ Int32 start,
    /* [in] */ Int32 velocity,
    /* [in] */ Int32 min,
    /* [in] */ Int32 max)
{
    mFinished = FALSE;

    mStart = start;
    mStartTime = AnimationUtils::CurrentAnimationTimeMillis();

    mVelocity = velocity;

    mDeceleration = GetDeceleration(velocity);

    // A start from an invalid position immediately brings back to a valid position
    if (mStart < min) {
        mDuration = 0;
        mFinal = min;
        return;
    }

    if (mStart > max) {
        mDuration = 0;
        mFinal = max;
        return;
    }

    // Duration are expressed in milliseconds
    mDuration = (Int32)(-1000.0f * velocity / mDeceleration);

    mFinal = start - Math::Round((velocity * velocity) / (2.0f * mDeceleration));

    // Clamp to a valid final position
    if (mFinal < min) {
        mFinal = min;
        mDuration = ComputeDuration(mStart, min, mVelocity, mDeceleration);
    }

    if (mFinal > max) {
        mFinal = max;
        mDuration = ComputeDuration(mStart, max, mVelocity, mDeceleration);
    }
}

void OverScroller::MagneticOverScroller::Finish()
{
    mCurrentPosition = mFinal;
    // Not reset since WebView relies on this value for fast fling.
    // mCurrVelocity = 0.0f;
    mFinished = TRUE;
}

void OverScroller::MagneticOverScroller::SetFinalPosition(
    /* [in] */ Int32 position)
{
    mFinal = position;
    mFinished = FALSE;
}

void OverScroller::MagneticOverScroller::ExtendDuration(
    /* [in] */ Int32 extend)
{
    Int64 time = AnimationUtils::CurrentAnimationTimeMillis();
    Int32 elapsedTime = (Int32)(time - mStartTime);
    mDuration = elapsedTime + extend;
    mFinished = FALSE;
}

void OverScroller::MagneticOverScroller::SetBounceCoefficient(
    /* [in] */ Float coefficient)
{
    mBounceCoefficient = coefficient;
}

Boolean OverScroller::MagneticOverScroller::Springback(
    /* [in] */ Int32 start,
    /* [in] */ Int32 min,
    /* [in] */ Int32 max)
{
    mFinished = TRUE;

    mStart = start;
    mVelocity = 0;

    mStartTime = AnimationUtils::CurrentAnimationTimeMillis();
    mDuration = 0;

    if (start < min) {
        StartSpringback(start, min, FALSE);
    }
    else if (start > max) {
        StartSpringback(start, max, TRUE);
    }

    return !mFinished;
}

void OverScroller::MagneticOverScroller::StartSpringback(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Boolean positive)
{
    mFinished = FALSE;
    mState = TO_BOUNCE;
    mStart = mFinal = end;
    mDuration = OVERSCROLL_SPRINGBACK_DURATION;
    mStartTime -= OVERSCROLL_SPRINGBACK_DURATION / 2;
    mVelocity = (Int32)(Math::Abs(end - start) * TIME_COEF * (positive ? 1.0 : -1.0f));
}

void OverScroller::MagneticOverScroller::Fling(
    /* [in] */ Int32 start,
    /* [in] */ Int32 velocity,
    /* [in] */ Int32 min,
    /* [in] */ Int32 max,
    /* [in] */ Int32 over)
{
    mState = TO_EDGE;
    mOver = over;

    mFinished = FALSE;

    mStart = start;
    mStartTime = AnimationUtils::CurrentAnimationTimeMillis();

    mVelocity = velocity;

    mDeceleration = GetDeceleration(velocity);

    // Duration are expressed in milliseconds
    mDuration = (Int32)(-1000.0f * velocity / mDeceleration);

    mFinal = start - Math::Round((velocity * velocity) / (2.0f * mDeceleration));

    // Clamp to a valid final position
    if (mFinal < min) {
        mFinal = min;
        mDuration = ComputeDuration(mStart, min, mVelocity, mDeceleration);
    }

    if (mFinal > max) {
        mFinal = max;
        mDuration = ComputeDuration(mStart, max, mVelocity, mDeceleration);
    }

    if (start > max) {
        if (start >= max + over) {
            Springback(max + over, min, max);
        }
        else {
            if (velocity <= 0) {
                Springback(start, min, max);
            }
            else {
                Int64 time = AnimationUtils::CurrentAnimationTimeMillis();
                Double durationSinceEdge =
                    Math::Atan((start-max) * TIME_COEF / velocity) / TIME_COEF;
                mStartTime = (Int32)(time - 1000.0f * durationSinceEdge);

                // Simulate a bounce that started from edge
                mStart = max;

                mVelocity = (Int32)(velocity / Math::Cos(durationSinceEdge * TIME_COEF));

                OnEdgeReached();
            }
        }
    }
    else {
        if (start < min) {
            if (start <= min - over) {
                Springback(min - over, min, max);
            }
            else {
                if (velocity >= 0) {
                    Springback(start, min, max);
                }
                else {
                    Int64 time = AnimationUtils::CurrentAnimationTimeMillis();
                    Double durationSinceEdge =
                        Math::Atan((start-min) * TIME_COEF / velocity) / TIME_COEF;
                    mStartTime = (Int32)(time - 1000.0f * durationSinceEdge);

                    // Simulate a bounce that started from edge
                    mStart = min;

                    mVelocity = (Int32)(velocity / Math::Cos(durationSinceEdge * TIME_COEF));

                    OnEdgeReached();
                }
            }
        }
    }
}

void OverScroller::MagneticOverScroller::NotifyEdgeReached(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 over)
{
    mDeceleration = GetDeceleration(mVelocity);

    // Local time, used to compute edge crossing time.
    Float timeCurrent = mCurrVelocity / mDeceleration;
    Int32 distance = end - start;
    Float timeEdge = -(Float)Math::Sqrt((2.0f * distance / mDeceleration)
            + (timeCurrent * timeCurrent));

    mVelocity = (Int32)(mDeceleration * timeEdge);

    // Simulate a symmetric bounce that started from edge
    mStart = end;

    mOver = over;

    Int64 time = AnimationUtils::CurrentAnimationTimeMillis();
    mStartTime = (Int32)(time - 1000.0f * (timeCurrent - timeEdge));

    OnEdgeReached();
}

void OverScroller::MagneticOverScroller::OnEdgeReached()
{
    // mStart, mVelocity and mStartTime were adjusted to their values when edge was reached.
    Float distance = mVelocity / TIME_COEF;

    if (Math::Abs(distance) < mOver) {
        // Spring force will bring us back to final position
        mState = TO_BOUNCE;
        mFinal = mStart;
        mDuration = OVERSCROLL_SPRINGBACK_DURATION;
    }
    else {
        // Velocity is too high, we will hit the boundary limit
        mState = TO_BOUNDARY;
        Int32 over = mVelocity > 0 ? mOver : -mOver;
        mFinal = mStart + over;
        mDuration = (Int32)(1000.0f * Math::Asin(over / distance) / TIME_COEF);
    }
}

Boolean OverScroller::MagneticOverScroller::ContinueWhenFinished()
{
    switch (mState) {
    case TO_EDGE:
        {
            // Duration from start to NULL velocity
            Int32 duration = (Int32)(-1000.0f * mVelocity / mDeceleration);
            if (mDuration < duration) {
                // If the animation was clamped, we reached the edge
                mStart = mFinal;
                // Speed when edge was reached
                mVelocity = (Int32)(mVelocity + mDeceleration * mDuration / 1000.0f);
                mStartTime += mDuration;
                OnEdgeReached();
            }
            else {
                // Normal stop, no need to continue
                return FALSE;
            }
        }
        break;
    case TO_BOUNDARY:
        {
            mStartTime += mDuration;
            StartSpringback(
                mFinal, mFinal - (mVelocity > 0 ? mOver:-mOver),
                mVelocity > 0);
        }
        break;
    case TO_BOUNCE:
        {
            //mVelocity = (Int32)(mVelocity * BOUNCE_COEFFICIENT);
            mVelocity = (Int32)(mVelocity * mBounceCoefficient);
            if (Math::Abs(mVelocity) < MINIMUM_VELOCITY_FOR_BOUNCE) {
                return FALSE;
            }
            mStartTime += mDuration;
        }
        break;
    default:
        break;
    }

    Update();
    return TRUE;
}

/*
 * Update the current position and velocity for current time. Returns
 * TRUE if update has been done and FALSE if animation duration has been
 * reached.
 */
Boolean OverScroller::MagneticOverScroller::Update()
{
    Int64 time = AnimationUtils::CurrentAnimationTimeMillis();
    Int64 duration = time - mStartTime;

    if (duration > mDuration) {
        return FALSE;
    }

    Double distance;
    Float t = duration / 1000.0f;
    if (mState == TO_EDGE) {
        mCurrVelocity = mVelocity + mDeceleration * t;
        distance = mVelocity * t + mDeceleration * t * t / 2.0f;
    }
    else {
        Float d = t * TIME_COEF;
        mCurrVelocity = mVelocity * (Float)Math::Cos(d);
        distance = mVelocity / TIME_COEF * Math::Sin(d);
    }

    mCurrentPosition = mStart + (Int32)distance;
    return TRUE;
}

/**
 * Creates an OverScroller.
 * @param context The context of this application.
 * @param interpolator The scroll interpolator. If NULL, a default (viscous) interpolator will
 * be used.
 * @param bounceCoefficientX A value between 0 and 1 that will determine the proportion of the
 * velocity which is preserved in the bounce when the horizontal edge is reached. A NULL value
 * means no bounce.
 * @param bounceCoefficientY Same as bounceCoefficientX but for the vertical direction.
 */
OverScroller::OverScroller(
    /* [in] */ IContext* context,
    /* [in] */ IInterpolator* interpolator /* = NULL*/,
    /* [in] */ Float bounceCoefficientX /*=
        MagneticOverScroller::DEFAULT_BOUNCE_COEFFICIENT*/,
    /* [in] */ Float bounceCoefficientY /*=
        MagneticOverScroller::DEFAULT_BOUNCE_COEFFICIENT*/)
{
    mInterpolator = interpolator;
    mScrollerX = new MagneticOverScroller();
    mScrollerY = new MagneticOverScroller();
    MagneticOverScroller::InitializeFromContext(context);

    mScrollerX->SetBounceCoefficient(bounceCoefficientX);
    mScrollerY->SetBounceCoefficient(bounceCoefficientY);
}

OverScroller::~OverScroller()
{
    delete mScrollerX;
    delete mScrollerY;
}
/**
 *
 * Returns whether the scroller has finished scrolling.
 *
 * @return True if the scroller has finished scrolling, FALSE otherwise.
 */
Boolean OverScroller::IsFinished()
{
    return mScrollerX->mFinished && mScrollerY->mFinished;
}

/**
 * Force the finished field to a particular value. Contrary to
 * {@link #abortAnimation()}, forcing the animation to finished
 * does NOT cause the scroller to move to the final x and y
 * position.
 *
 * @param finished The new finished value.
 */
void OverScroller::ForceFinished(
    /* [in] */ Boolean finished)
{
    mScrollerX->mFinished = mScrollerY->mFinished = finished;
}

/**
 * Returns the current X offset in the scroll.
 *
 * @return The new X offset as an absolute distance from the origin.
 */
Int32 OverScroller::GetCurrX()
{
    return mScrollerX->mCurrentPosition;
}

/**
 * Returns the current Y offset in the scroll.
 *
 * @return The new Y offset as an absolute distance from the origin.
 */
Int32 OverScroller::GetCurrY()
{
    return mScrollerY->mCurrentPosition;
}

/**
 * @hide
 * Returns the current velocity.
 *
 * @return The original velocity less the deceleration, norm of the X and Y velocity vector.
 */
Float OverScroller::GetCurrVelocity()
{
    Float squaredNorm = mScrollerX->mCurrVelocity * mScrollerX->mCurrVelocity;
    squaredNorm += mScrollerY->mCurrVelocity * mScrollerY->mCurrVelocity;
    return (Float)Math::Sqrt(squaredNorm);
}

/**
 * Returns the start X offset in the scroll.
 *
 * @return The start X offset as an absolute distance from the origin.
 */
Int32 OverScroller::GetStartX()
{
    return mScrollerX->mStart;
}

/**
 * Returns the start Y offset in the scroll.
 *
 * @return The start Y offset as an absolute distance from the origin.
 */
Int32 OverScroller::GetStartY()
{
    return mScrollerY->mStart;
}

/**
 * Returns where the scroll will end. Valid only for "fling" scrolls.
 *
 * @return The final X offset as an absolute distance from the origin.
 */
Int32 OverScroller::GetFinalX()
{
    return mScrollerX->mFinal;
}

/**
 * Returns where the scroll will end. Valid only for "fling" scrolls.
 *
 * @return The final Y offset as an absolute distance from the origin.
 */
Int32 OverScroller::GetFinalY()
{
    return mScrollerY->mFinal;
}

/**
 * Returns how Int64 the scroll event will take, in milliseconds.
 *
 * @return The duration of the scroll in milliseconds.
 *
 * @hide Pending removal once nothing depends on it
 * @deprecated OverScrollers don't necessarily have a fixed duration.
 *             This function will lie to the best of its ability.
 */
Int32 OverScroller::GetDuration()
{
    return Math::Max(mScrollerX->mDuration, mScrollerY->mDuration);
}

/**
 * Extend the scroll animation. This allows a running animation to scroll
 * further and longer, when used with {@link #setFinalX(Int32)} or {@link #setFinalY(Int32)}.
 *
 * @param extend Additional time to scroll in milliseconds.
 * @see #setFinalX(Int32)
 * @see #setFinalY(Int32)
 *
 * @hide Pending removal once nothing depends on it
 * @deprecated OverScrollers don't necessarily have a fixed duration.
 *             Instead of setting a new final position and extending
 *             the duration of an existing scroll, use startScroll
 *             to begin a new animation.
 */
void OverScroller::ExtendDuration(
    /* [in] */ Int32 extend)
{
    mScrollerX->ExtendDuration(extend);
    mScrollerY->ExtendDuration(extend);
}

/**
 * Sets the final position (X) for this scroller.
 *
 * @param newX The new X offset as an absolute distance from the origin.
 * @see #extendDuration(Int32)
 * @see #setFinalY(Int32)
 *
 * @hide Pending removal once nothing depends on it
 * @deprecated OverScroller's final position may change during an animation.
 *             Instead of setting a new final position and extending
 *             the duration of an existing scroll, use startScroll
 *             to begin a new animation.
 */
void OverScroller::SetFinalX(
    /* [in] */ Int32 newX)
{
    mScrollerX->SetFinalPosition(newX);
}

/**
 * Sets the final position (Y) for this scroller.
 *
 * @param newY The new Y offset as an absolute distance from the origin.
 * @see #extendDuration(Int32)
 * @see #setFinalX(Int32)
 *
 * @hide Pending removal once nothing depends on it
 * @deprecated OverScroller's final position may change during an animation.
 *             Instead of setting a new final position and extending
 *             the duration of an existing scroll, use startScroll
 *             to begin a new animation.
 */
void OverScroller::SetFinalY(
    /* [in] */ Int32 newY)
{
    mScrollerY->SetFinalPosition(newY);
}

/**
 * Call this when you want to know the new location. If it returns TRUE, the
 * animation is not yet finished.
 */
Boolean OverScroller::ComputeScrollOffset()
{
    if (IsFinished()) {
        return FALSE;
    }

    switch (mMode) {
    case SCROLL_MODE:
        {
            Int64 time = AnimationUtils::CurrentAnimationTimeMillis();
            // Any scroller can be used for time, since they were started
            // together in scroll mode. We use X here.
            Int64 elapsedTime = time - mScrollerX->mStartTime;

            Int32 duration = mScrollerX->mDuration;
            if (elapsedTime < duration) {
                Float q = (Float)(elapsedTime) / duration;

                if (mInterpolator == NULL) {
                    q = Scroller::ViscousFluid(q);
                }
                else {
                    mInterpolator->GetInterpolation(q, &q);
                }

                mScrollerX->UpdateScroll(q);
                mScrollerY->UpdateScroll(q);
            }
            else {
                AbortAnimation();
            }
        }
        break;
    case FLING_MODE:
        {
            if (!mScrollerX->mFinished) {
                if (!mScrollerX->Update()) {
                    if (!mScrollerX->ContinueWhenFinished()) {
                        mScrollerX->Finish();
                    }
                }
            }

            if (!mScrollerY->mFinished) {
                if (!mScrollerY->Update()) {
                    if (!mScrollerY->ContinueWhenFinished()) {
                        mScrollerY->Finish();
                    }
                }
            }
        }
        break;
    default:
        break;
    }

    return TRUE;
}

/**
 * Start scrolling by providing a starting point and the distance to travel.
 *
 * @param startX Starting horizontal scroll offset in pixels. Positive
 *        numbers will scroll the content to the left.
 * @param startY Starting vertical scroll offset in pixels. Positive numbers
 *        will scroll the content up.
 * @param dx Horizontal distance to travel. Positive numbers will scroll the
 *        content to the left.
 * @param dy Vertical distance to travel. Positive numbers will scroll the
 *        content up.
 * @param duration Duration of the scroll in milliseconds.
 */
void OverScroller::StartScroll(
    /* [in] */ Int32 startX,
    /* [in] */ Int32 startY,
    /* [in] */ Int32 dx,
    /* [in] */ Int32 dy,
    /* [in] */ Int32 duration/* = DEFAULT_DURATION*/)
{
    mMode = SCROLL_MODE;
    mScrollerX->StartScroll(startX, dx, duration);
    mScrollerY->StartScroll(startY, dy, duration);
}

/**
 * Call this when you want to 'spring back' into a valid coordinate range.
 *
 * @param startX Starting X coordinate
 * @param startY Starting Y coordinate
 * @param minX Minimum valid X value
 * @param maxX Maximum valid X value
 * @param minY Minimum valid Y value
 * @param maxY Minimum valid Y value
 * @return TRUE if a springback was initiated, FALSE if startX and startY were
 *          already within the valid range.
 */
Boolean OverScroller::SpringBack(
    /* [in] */ Int32 startX,
    /* [in] */ Int32 startY,
    /* [in] */ Int32 minX,
    /* [in] */ Int32 maxX,
    /* [in] */ Int32 minY,
    /* [in] */ Int32 maxY)
{
    mMode = FLING_MODE;

    // Make sure both methods are called.
    Boolean spingbackX = mScrollerX->Springback(startX, minX, maxX);
    Boolean spingbackY = mScrollerY->Springback(startY, minY, maxY);

    return spingbackX || spingbackY;
}

/**
 * Start scrolling based on a fling gesture. The distance traveled will
 * depend on the initial velocity of the fling.
 *
 * @param startX Starting point of the scroll (X)
 * @param startY Starting point of the scroll (Y)
 * @param velocityX Initial velocity of the fling (X) measured in pixels per
 *            second.
 * @param velocityY Initial velocity of the fling (Y) measured in pixels per
 *            second
 * @param minX Minimum X value. The scroller will not scroll past this point
 *            unless overX > 0. If overfling is allowed, it will use minX as
 *            a springback boundary.
 * @param maxX Maximum X value. The scroller will not scroll past this point
 *            unless overX > 0. If overfling is allowed, it will use maxX as
 *            a springback boundary.
 * @param minY Minimum Y value. The scroller will not scroll past this point
 *            unless overY > 0. If overfling is allowed, it will use minY as
 *            a springback boundary.
 * @param maxY Maximum Y value. The scroller will not scroll past this point
 *            unless overY > 0. If overfling is allowed, it will use maxY as
 *            a springback boundary.
 * @param overX Overfling range. If > 0, horizontal overfling in either
 *            direction will be possible.
 * @param overY Overfling range. If > 0, vertical overfling in either
 *            direction will be possible.
 */
void OverScroller::Fling(
    /* [in] */ Int32 startX,
    /* [in] */ Int32 startY,
    /* [in] */ Int32 velocityX,
    /* [in] */ Int32 velocityY,
    /* [in] */ Int32 minX,
    /* [in] */ Int32 maxX,
    /* [in] */ Int32 minY,
    /* [in] */ Int32 maxY,
    /* [in] */ Int32 overX/* = 0*/,
    /* [in] */ Int32 overY/* = 0*/)
{
    mMode = FLING_MODE;
    mScrollerX->Fling(startX, velocityX, minX, maxX, overX);
    mScrollerY->Fling(startY, velocityY, minY, maxY, overY);
}

/**
 * Notify the scroller that we've reached a horizontal boundary.
 * Normally the information to handle this will already be known
 * when the animation is started, such as in a call to one of the
 * fling functions. However there are cases where this cannot be known
 * in advance. This function will transition the current motion and
 * animate from startX to finalX as appropriate.
 *
 * @param startX Starting/current X position
 * @param finalX Desired final X position
 * @param overX Magnitude of overscroll allowed. This should be the maximum
 *              desired distance from finalX. Absolute value - must be positive.
 */
void OverScroller::NotifyHorizontalEdgeReached(
    /* [in] */ Int32 startX,
    /* [in] */ Int32 finalX,
    /* [in] */ Int32 overX)
{
    mScrollerX->NotifyEdgeReached(startX, finalX, overX);
}

/**
 * Notify the scroller that we've reached a vertical boundary.
 * Normally the information to handle this will already be known
 * when the animation is started, such as in a call to one of the
 * fling functions. However there are cases where this cannot be known
 * in advance. This function will animate a parabolic motion from
 * startY to finalY.
 *
 * @param startY Starting/current Y position
 * @param finalY Desired final Y position
 * @param overY Magnitude of overscroll allowed. This should be the maximum
 *              desired distance from finalY.
 */
void OverScroller::NotifyVerticalEdgeReached(
    /* [in] */ Int32 startY,
    /* [in] */ Int32 finalY,
    /* [in] */ Int32 overY)
{
    mScrollerY->NotifyEdgeReached(startY, finalY, overY);
}

/**
 * Returns whether the current Scroller is currently returning to a valid position.
 * Valid bounds were provided by the
 * {@link #fling(Int32, Int32, Int32, Int32, Int32, Int32, Int32, Int32, Int32, Int32)} method.
 *
 * One should check this value before calling
 * {@link #startScroll(Int32, Int32, Int32, Int32)} as the interpolation currently in progress
 * to restore a valid position will then be stopped. The caller has to take into account
 * the fact that the started scroll will start from an overscrolled position.
 *
 * @return TRUE when the current position is overscrolled and in the process of
 *         interpolating back to a valid value.
 */
Boolean OverScroller::IsOverScrolled()
{
    return ((!mScrollerX->mFinished &&
        mScrollerX->mState != MagneticOverScroller::TO_EDGE) ||
        (!mScrollerY->mFinished &&
        mScrollerY->mState != MagneticOverScroller::TO_EDGE));
}

/**
 * Stops the animation. Contrary to {@link #forceFinished(Boolean)},
 * aborting the animating causes the scroller to move to the final x and y
 * positions.
 *
 * @see #forceFinished(Boolean)
 */
void OverScroller::AbortAnimation()
{
    mScrollerX->Finish();
    mScrollerY->Finish();
}

/**
 * Returns the time elapsed since the beginning of the scrolling.
 *
 * @return The elapsed time in milliseconds.
 *
 * @hide
 */
Int32 OverScroller::TimePassed()
{
    Int64 time = AnimationUtils::CurrentAnimationTimeMillis();
    Int64 startTime = Math::Min(mScrollerX->mStartTime, mScrollerY->mStartTime);

    return (Int32)(time - startTime);
}
