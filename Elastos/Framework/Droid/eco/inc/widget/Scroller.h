#ifndef __SCROLLER_H__
#define __SCROLLER_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>

/**
 * This class encapsulates scrolling.  The duration of the scroll
 * can be passed in the constructor and specifies the maximum time that
 * the scrolling animation should take.  Past this time, the scrolling is
 * automatically moved to its final stage and computeScrollOffset()
 * will always return FALSE to indicate that scrolling is over.
 */
class Scroller
{
public:
    static CARAPI_(Float) ViscousFluid(
        /* [in] */ Float x);

public:
    /**
      * Create a Scroller with the default duration and interpolator.
      */
    Scroller(
        /* [in] */ IContext* context);

    /**
      * Create a Scroller with the specified interpolator. If the interpolator is
      * NULL, the default (viscous) interpolator will be used.
      */
    Scroller(
        /* [in] */ IContext* context,
        /* [in] */ IInterpolator* interpolator);

    /**
      *
      * Returns whether the scroller has finished scrolling.
      *
      * @return True if the scroller has finished scrolling, FALSE otherwise.
      */
    CARAPI_(Boolean) IsFinished();

    /**
      * Force the finished field to a particular value.
      *
      * @param finished The new finished value.
      */
    CARAPI_(void) ForceFinished(
        /* [in] */ Boolean finished);

    /**
      * Returns how long the scroll event will take, in milliseconds.
      *
      * @return The duration of the scroll in milliseconds.
      */
    CARAPI_(Int32) GetDuration();

    /**
     * Returns the current X offset in the scroll.
     *
     * @return The new X offset as an absolute distance from the origin.
     */
    CARAPI_(Int32) GetCurrX();

    /**
     * Returns the current Y offset in the scroll.
     *
     * @return The new Y offset as an absolute distance from the origin.
     */
    CARAPI_(Int32) GetCurrY();

    /**
     * @hide
     * Returns the current velocity.
     *
     * @return The original velocity less the deceleration. Result may be
     * negative.
     */
    CARAPI_(Float) GetCurrVelocity();

    /**
     * Returns the start X offset in the scroll.
     *
     * @return The start X offset as an absolute distance from the origin.
     */
    CARAPI_(Int32) GetStartX();

    /**
     * Returns the start Y offset in the scroll.
     *
     * @return The start Y offset as an absolute distance from the origin.
     */
    CARAPI_(Int32) GetStartY();

    /**
     * Returns where the scroll will end. Valid only for "fling" scrolls.
     *
     * @return The final X offset as an absolute distance from the origin.
     */
    CARAPI_(Int32) GetFinalX();

    /**
     * Returns where the scroll will end. Valid only for "fling" scrolls.
     *
     * @return The final Y offset as an absolute distance from the origin.
     */
    CARAPI_(Int32) GetFinalY();

    /**
     * Call this when you want to know the new location.  If it returns TRUE,
     * the animation is not yet finished.  loc will be altered to provide the
     * new location.
     */
    Boolean ComputeScrollOffset();

    /**
     * Start scrolling by providing a starting point and the distance to travel.
     * The scroll will use the default value of 250 milliseconds for the
     * duration.
     *
     * @param startX Starting horizontal scroll offset in pixels. Positive
     *        numbers will scroll the content to the left.
     * @param startY Starting vertical scroll offset in pixels. Positive numbers
     *        will scroll the content up.
     * @param dx Horizontal distance to travel. Positive numbers will scroll the
     *        content to the left.
     * @param dy Vertical distance to travel. Positive numbers will scroll the
     *        content up.
     */
    CARAPI_(void) StartScroll(
        /* [in] */ Int32 startX,
        /* [in] */ Int32 startY,
        /* [in] */ Int32 dx,
        /* [in] */ Int32 dy);

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
    CARAPI_(void) StartScroll(
        /* [in] */ Int32 startX,
        /* [in] */ Int32 startY,
        /* [in] */ Int32 dx,
        /* [in] */ Int32 dy,
        /* [in] */ Int32 duration);

    /**
     * Start scrolling based on a fling gesture. The distance travelled will
     * depend on the initial velocity of the fling.
     *
     * @param startX Starting point of the scroll (X)
     * @param startY Starting point of the scroll (Y)
     * @param velocityX Initial velocity of the fling (X) measured in pixels per
     *        second.
     * @param velocityY Initial velocity of the fling (Y) measured in pixels per
     *        second
     * @param minX Minimum X value. The scroller will not scroll past this
     *        point.
     * @param maxX Maximum X value. The scroller will not scroll past this
     *        point.
     * @param minY Minimum Y value. The scroller will not scroll past this
     *        point.
     * @param maxY Maximum Y value. The scroller will not scroll past this
     *        point.
     */
    CARAPI_(void) Fling(
        /* [in] */ Int32 startX,
        /* [in] */ Int32 startY,
        /* [in] */ Int32 velocityX,
        /* [in] */ Int32 velocityY,
        /* [in] */ Int32 minX,
        /* [in] */ Int32 maxX,
        /* [in] */ Int32 minY,
        /* [in] */ Int32 maxY);

    /**
     * Stops the animation. Contrary to {@link #forceFinished(Boolean)},
     * aborting the animating cause the scroller to move to the final x and y
     * position
     *
     * @see #forceFinished(Boolean)
     */
    CARAPI_(void) AbortAnimation();

    /**
     * Extend the scroll animation. This allows a running animation to scroll
     * further and longer, when used with {@link #setFinalX(Int32)} or {@link #setFinalY(Int32)}.
     *
     * @param extend Additional time to scroll in milliseconds.
     * @see #setFinalX(Int32)
     * @see #setFinalY(Int32)
     */
    CARAPI_(void) ExtendDuration(
        /* [in] */ Int32 extend);

    /**
     * Returns the time elapsed since the beginning of the scrolling.
     *
     * @return The elapsed time in milliseconds.
     */
    CARAPI_(Int32) TimePassed();

    /**
     * Sets the final position (X) for this scroller.
     *
     * @param newX The new X offset as an absolute distance from the origin.
     * @see #extendDuration(Int32)
     * @see #setFinalY(Int32)
     */
    CARAPI_(void) SetFinalX(
        /* [in] */ Int32 newX);

    /**
     * Sets the final position (Y) for this scroller.
     *
     * @param newY The new Y offset as an absolute distance from the origin.
     * @see #extendDuration(Int32)
     * @see #setFinalX(Int32)
     */
    CARAPI_(void) SetFinalY(
        /* [in] */ Int32 newY);

private:
    friend Float _sInit();

    static const Int32 DEFAULT_DURATION  = 250;
    static const Int32 SCROLL_MODE = 0;
    static const Int32 FLING_MODE = 1;

    static Float sViscousFluidScale;
    static Float sViscousFluidNormalize;

private:
    Int32 mMode;

    Int32 mStartX;
    Int32 mStartY;
    Int32 mFinalX;
    Int32 mFinalY;

    Int32 mMinX;
    Int32 mMaxX;
    Int32 mMinY;
    Int32 mMaxY;

    Int32 mCurrX;
    Int32 mCurrY;
    Int64 mStartTime;
    Int32 mDuration;
    Float mDurationReciprocal;
    Float mDeltaX;
    Float mDeltaY;
    Boolean mFinished;
    AutoPtr<IInterpolator> mInterpolator;

    Float mCoeffX;
    Float mCoeffY;
    Float mVelocity;

    Float mDeceleration;
};
#endif //__SCROLLER_H__
