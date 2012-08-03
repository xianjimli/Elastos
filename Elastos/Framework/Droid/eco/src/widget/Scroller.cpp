
#include "widget/Scroller.h"
#include "view/ViewConfiguration.h"
#include "view/animation/AnimationUtils.h"
#include "os/SystemClock.h"
#include <elastos/Math.h>

using namespace Elastos::Core;

const Int32 Scroller::DEFAULT_DURATION;
const Int32 Scroller::SCROLL_MODE;
const Int32 Scroller::FLING_MODE;

// This controls the viscous fluid effect (how much of it)
Float Scroller::sViscousFluidScale = 8.0f;
// must be set to 1.0 (used in viscousFluid())
//Float Scroller::sViscousFluidNormalize = 1.0f;
Float Scroller::sViscousFluidNormalize = 1.0f / Scroller::ViscousFluid(1.0f);

//temp
const Float GRAVITY_EARTH = 9.80665f;

Scroller::Scroller(
    /* [in] */ IContext* context)
{
    Scroller(context, NULL);
}

Scroller::Scroller(
    /* [in] */ IContext* context,
    /* [in] */ IInterpolator* interpolator) :
    mFinished(TRUE),
    mInterpolator(interpolator),
    mCoeffX(0.0f),
    mCoeffY(1.0f)
{
    Float ppi = 160.0f;//context.getResources().getDisplayMetrics().density * 160.0f;
    mDeceleration =  GRAVITY_EARTH //SensorManager.GRAVITY_EARTH   // g (m/s^2)
        * 39.37f                   // inch/meter
        * ppi                      // pixels per inch
        * ViewConfiguration::GetScrollFriction();
}

Boolean Scroller::IsFinished()
{
    return mFinished;
}

void Scroller::ForceFinished(
    /* [in] */ Boolean finished)
{
    mFinished = finished;
}

Int32 Scroller::GetDuration()
{
    return mDuration;
}

Int32 Scroller::GetCurrX()
{
    return mCurrX;
}

Int32 Scroller::GetCurrY()
{
    return mCurrY;
}

Float Scroller::GetCurrVelocity()
{
    return mVelocity - mDeceleration * TimePassed() / 2000.0f;
}

Int32 Scroller::GetStartX()
{
    return mStartX;
}

Int32 Scroller::GetStartY()
{
    return mStartY;
}

Int32 Scroller::GetFinalX()
{
    return mFinalX;
}

Int32 Scroller::GetFinalY()
{
    return mFinalY;
}

Boolean Scroller::ComputeScrollOffset()
{
    if (mFinished) {
        return FALSE;
    }

    Int32 timePassed = (Int32)(SystemClock::GetUptimeMillis() - mStartTime);

    if (timePassed < mDuration) {
        switch (mMode) {
        case SCROLL_MODE:
            {
                Float x = (Float)timePassed * mDurationReciprocal;

                if (mInterpolator == NULL) {
                    x = ViscousFluid(x);
                }
                else {
                    Float xTemp;
                    mInterpolator->GetInterpolation(x, &xTemp);
                    x = xTemp;
                }

                mCurrX = mStartX + Int32(x * mDeltaX + 0.5f); //Math.round(x * mDeltaX)
                mCurrY = mStartY + Int32(x * mDeltaY + 0.5f);
            }
            break;
        case FLING_MODE:
            {
                Float timePassedSeconds = timePassed / 1000.0f;
                Float distance = (mVelocity * timePassedSeconds)
                    - (mDeceleration * timePassedSeconds * timePassedSeconds / 2.0f);

                mCurrX = mStartX + Math::Round(distance * mCoeffX);
                // Pin to mMinX <= mCurrX <= mMaxX
                mCurrX = Math::Min(mCurrX, mMaxX);
                mCurrX = Math::Max(mCurrX, mMinX);

                mCurrY = mStartY + Math::Round(distance * mCoeffY);
                // Pin to mMinY <= mCurrY <= mMaxY
                mCurrY = Math::Min(mCurrY, mMaxY);
                mCurrY = Math::Max(mCurrY, mMinY);

                if (mCurrX == mFinalX && mCurrY == mFinalY) {
                    mFinished = TRUE;
                }
            }
            break;
        default:
            break;
        }
    }
    else {
        mCurrX = mFinalX;
        mCurrY = mFinalY;
        mFinished = TRUE;
    }

    return TRUE;
}

void Scroller::StartScroll(
    /* [in] */ Int32 startX,
    /* [in] */ Int32 startY,
    /* [in] */ Int32 dx,
    /* [in] */ Int32 dy)
{
    StartScroll(startX, startY, dx, dy, DEFAULT_DURATION);
}

void Scroller::StartScroll(
    /* [in] */ Int32 startX,
    /* [in] */ Int32 startY,
    /* [in] */ Int32 dx,
    /* [in] */ Int32 dy,
    /* [in] */ Int32 duration)
{
    mMode = SCROLL_MODE;
    mFinished = FALSE;
    mDuration = duration;
    mStartTime = AnimationUtils::CurrentAnimationTimeMillis();
    mStartX = startX;
    mStartY = startY;
    mFinalX = startX + dx;
    mFinalY = startY + dy;
    mDeltaX = dx;
    mDeltaY = dy;
    mDurationReciprocal = 1.0f / (Float) mDuration;
}

void Scroller::Fling(
    /* [in] */ Int32 startX,
    /* [in] */ Int32 startY,
    /* [in] */ Int32 velocityX,
    /* [in] */ Int32 velocityY,
    /* [in] */ Int32 minX,
    /* [in] */ Int32 maxX,
    /* [in] */ Int32 minY,
    /* [in] */ Int32 maxY)
{
    mMode = FLING_MODE;
    mFinished = FALSE;

    Float velocity = (Float)Math::Hypot(velocityX, velocityY);

    mVelocity = velocity;
    mDuration = (Int32) (1000 * velocity / mDeceleration); // Duration is in
    // milliseconds
    mStartTime = SystemClock::GetUptimeMillis();
    mStartX = startX;
    mStartY = startY;

    mCoeffX = velocity == 0 ? 1.0f : velocityX / velocity;
    mCoeffY = velocity == 0 ? 1.0f : velocityY / velocity;

    Int32 totalDistance = (Int32)((velocity * velocity) / (2 * mDeceleration));

    mMinX = minX;
    mMaxX = maxX;
    mMinY = minY;
    mMaxY = maxY;


    mFinalX = startX + Math::Round(totalDistance * mCoeffX);
    // Pin to mMinX <= mFinalX <= mMaxX
    mFinalX = Math::Min(mFinalX, mMaxX);
    mFinalX = Math::Max(mFinalX, mMinX);

    mFinalY = startY + Math::Round(totalDistance * mCoeffY);
    // Pin to mMinY <= mFinalY <= mMaxY
    mFinalY = Math::Min(mFinalY, mMaxY);
    mFinalY = Math::Max(mFinalY, mMinY);
}

Float Scroller::ViscousFluid(
    /* [in] */ Float x)
{
    x *= sViscousFluidScale;
    if (x < 1.0f) {
        x -= (1.0f - (Float)Math::Exp(-x));
    }
    else {
        Float start = 0.36787944117f;   // 1/e == exp(-1)
        x = 1.0f - (Float)Math::Exp(1.0f - x);
        x = start + x * (1.0f - start);
    }
    x *= sViscousFluidNormalize;
    return x;
}

void Scroller::AbortAnimation()
{
    mCurrX = mFinalX;
    mCurrY = mFinalY;
    mFinished = TRUE;
}

void Scroller::ExtendDuration(
    /* [in] */ Int32 extend)
{
    Int32 passed = TimePassed();
    mDuration = passed + extend;
    mDurationReciprocal = 1.0f / (Float)mDuration;
    mFinished = FALSE;
}

Int32 Scroller::TimePassed()
{
    return (Int32)(AnimationUtils::CurrentAnimationTimeMillis() - mStartTime);
}

void Scroller::SetFinalX(
    /* [in] */ Int32 newX)
{
    mFinalX = newX;
    mDeltaX = mFinalX - mStartX;
    mFinished = FALSE;
}

void Scroller::SetFinalY(
    /* [in] */ Int32 newY)
{
    mFinalY = newY;
    mDeltaY = mFinalY - mStartY;
    mFinished = FALSE;
}
