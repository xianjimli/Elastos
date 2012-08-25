
#ifndef __OVERSCROLLER_H__
#define __OVERSCROLLER_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/Math.h>

using namespace Elastos::Core;

/**
 * This class encapsulates scrolling with the ability to overshoot the bounds
 * of a scrolling operation. This class is a drop-in replacement for
 * {@link android.widget.Scroller} in most cases.
 */
class OverScroller
{
private:
    static const Int32 DEFAULT_DURATION = 250;
    static const Int32 SCROLL_MODE = 0;
    static const Int32 FLING_MODE = 1;

protected:
    class MagneticOverScroller
    {
        friend class OverScroller;
    protected:
        // Constant gravity value, used to scale deceleration
        static Float GRAVITY;

    private:
        static const Int32 TO_EDGE = 0;
        static const Int32 TO_BOUNDARY = 1;
        static const Int32 TO_BOUNCE = 2;

        // Duration in milliseconds to go back from edge to edge. Springback is half of it.
        static const Int32 OVERSCROLL_SPRINGBACK_DURATION = 200;

        // Oscillation period
        static const Float TIME_COEF;

        // If the velocity is smaller than this value, no bounce is triggered
        // when the edge limits are reached (would result in a zero pixels
        // displacement anyway).
        static const Float MINIMUM_VELOCITY_FOR_BOUNCE;//140.0f;

        // Proportion of the velocity that is preserved when the edge is reached.
        static const Float DEFAULT_BOUNCE_COEFFICIENT = 0.16f;

    protected:
        static CARAPI_(void) InitializeFromContext(
            /* [in] */ IContext* context);

        static CARAPI_(Float) GetDeceleration(
            /* [in] */ Int32 velocity);

        static CARAPI_(Int32) ComputeDuration(
            /* [in] */ Int32 start,
            /* [in] */ Int32 end,
            /* [in] */ Float initialVelocity,
            /* [in] */ Float deceleration);

    protected:
        MagneticOverScroller();

        CARAPI_(void) UpdateScroll(
            /* [in] */ Float q);

        CARAPI_(void) StartScroll(
            /* [in] */ Int32 start,
            /* [in] */ Int32 distance,
            /* [in] */ Int32 duration);

        CARAPI_(void) Fling(
            /* [in] */ Int32 start,
            /* [in] */ Int32 velocity,
            /* [in] */ Int32 min,
            /* [in] */ Int32 max);

        CARAPI_(void) Finish();

        CARAPI_(void) SetFinalPosition(
            /* [in] */ Int32 position);

        CARAPI_(void) ExtendDuration(
            /* [in] */ Int32 extend);

        CARAPI_(void) SetBounceCoefficient(
            /* [in] */ Float coefficient);

        CARAPI_(Boolean) Springback(
            /* [in] */ Int32 start,
            /* [in] */ Int32 min,
            /* [in] */ Int32 max);

        CARAPI_(void) Fling(
            /* [in] */ Int32 start,
            /* [in] */ Int32 velocity,
            /* [in] */ Int32 min,
            /* [in] */ Int32 max,
            /* [in] */ Int32 over);

        CARAPI_(void) NotifyEdgeReached(
            /* [in] */ Int32 start,
            /* [in] */ Int32 end,
            /* [in] */ Int32 over);

        CARAPI_(Boolean) ContinueWhenFinished();

        CARAPI_(Boolean) Update();

    private:
        CARAPI_(void) StartSpringback(
            /* [in] */ Int32 start,
            /* [in] */ Int32 end,
            /* [in] */ Boolean positive);

        CARAPI_(void) OnEdgeReached();

    protected:
        // Initial position
        Int32 mStart;

        // Current position
        Int32 mCurrentPosition;

        // Final position
        Int32 mFinal;

        // Initial velocity
        Int32 mVelocity;

        // Current velocity
        Float mCurrVelocity;

        // Constant current deceleration
        Float mDeceleration;

        // Animation starting time, in system milliseconds
        Int64 mStartTime;

        // Animation duration, in milliseconds
        Int32 mDuration;

        // Whether the animation is currently in progress
        Boolean mFinished;

    private:
        Int32 mState;

        // The allowed overshot distance before boundary is reached.
        Int32 mOver;

        Float mBounceCoefficient;
    };

public:
    OverScroller(
        /* [in] */ IContext* context,
        /* [in] */ IInterpolator* interpolator = NULL,
        /* [in] */ Float bounceCoefficientX =
            MagneticOverScroller::DEFAULT_BOUNCE_COEFFICIENT,
        /* [in] */ Float bounceCoefficientY =
            MagneticOverScroller::DEFAULT_BOUNCE_COEFFICIENT);

    ~OverScroller();

    CARAPI_(Boolean) IsFinished();

    CARAPI_(void) ForceFinished(
        /* [in] */ Boolean finished);

    CARAPI_(Int32) GetCurrX();

    CARAPI_(Int32) GetCurrY();

    CARAPI_(Float) GetCurrVelocity();

    CARAPI_(Int32) GetStartX();

    CARAPI_(Int32) GetStartY();

    CARAPI_(Int32) GetFinalX();

    CARAPI_(Int32) GetFinalY();

    CARAPI_(Int32) GetDuration();

    CARAPI_(void) ExtendDuration(
        /* [in] */ Int32 extend);

    CARAPI_(void) SetFinalX(
        /* [in] */ Int32 newX);

    CARAPI_(void) SetFinalY(
        /* [in] */ Int32 newY);

    CARAPI_(Boolean) ComputeScrollOffset();

    CARAPI_(void) StartScroll(
        /* [in] */ Int32 startX,
        /* [in] */ Int32 startY,
        /* [in] */ Int32 dx,
        /* [in] */ Int32 dy,
        /* [in] */ Int32 duration = DEFAULT_DURATION);

    CARAPI_(Boolean) SpringBack(
        /* [in] */ Int32 startX,
        /* [in] */ Int32 startY,
        /* [in] */ Int32 minX,
        /* [in] */ Int32 maxX,
        /* [in] */ Int32 minY,
        /* [in] */ Int32 maxY);

    CARAPI_(void) Fling(
        /* [in] */ Int32 startX,
        /* [in] */ Int32 startY,
        /* [in] */ Int32 velocityX,
        /* [in] */ Int32 velocityY,
        /* [in] */ Int32 minX,
        /* [in] */ Int32 maxX,
        /* [in] */ Int32 minY,
        /* [in] */ Int32 maxY,
        /* [in] */ Int32 overX = 0,
        /* [in] */ Int32 overY = 0);

    CARAPI_(void) NotifyHorizontalEdgeReached(
        /* [in] */ Int32 startX,
        /* [in] */ Int32 finalX,
        /* [in] */ Int32 overX);

    CARAPI_(void) NotifyVerticalEdgeReached(
        /* [in] */ Int32 startY,
        /* [in] */ Int32 finalY,
        /* [in] */ Int32 overY);

    CARAPI_(Boolean) IsOverScrolled();

    CARAPI_(void) AbortAnimation();

    CARAPI_(Int32) TimePassed();

private:
    Int32 mMode;

    MagneticOverScroller* mScrollerX;
    MagneticOverScroller* mScrollerY;

    AutoPtr<IInterpolator> mInterpolator;
};
#endif //__OVERSCROLLER_H__
