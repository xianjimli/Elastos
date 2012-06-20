
#ifndef __VIEWCONFIGURATION_H__
#define __VIEWCONFIGURATION_H__

#include "ext/frameworkext.h"
#include "utils/ElRefBase.h"
#include <elastos/AutoPtr.h>
#include <elastos/Map.h>

/**
 * Contains methods to standard constants used in the UI for timeouts, sizes, and distances.
 */
class ViewConfiguration : public ElRefBase
{
public:
    /**
     * @deprecated Use {@link android.view.ViewConfiguration#get(android.content.Context)} instead.
     */
    ViewConfiguration();

private:
    /**
     * Creates a new configuration for the specified context. The configuration depends on
     * various parameters of the context, like the dimension of the display or the density
     * of the display.
     *
     * @param context The application context used to initialize this view configuration.
     *
     * @see #get(android.content.Context)
     * @see android.util.DisplayMetrics
     */
    ViewConfiguration(
        /* [in] */ IContext* context);

public:
    /**
     * Returns a configuration for the specified context. The configuration depends on
     * various parameters of the context, like the dimension of the display or the
     * density of the display.
     *
     * @param context The application context used to initialize the view configuration.
     */
    static CARAPI_(AutoPtr<ViewConfiguration>) Get(
        /* [in] */ IContext* context);

    /**
     * @return The width of the horizontal scrollbar and the height of the vertical
     *         scrollbar in pixels
     *
     * @deprecated Use {@link #getScaledScrollBarSize()} instead.
     */
    static CARAPI_(Int32) GetScrollBarSize();

    /**
     * @return The width of the horizontal scrollbar and the height of the vertical
     *         scrollbar in pixels
     */
    CARAPI_(Int32) GetScaledScrollBarSize();

    /**
     * @return Duration of the fade when scrollbars fade away in milliseconds
     */
    static CARAPI_(Int32) GetScrollBarFadeDuration();

    /**
     * @return Default delay before the scrollbars fade in milliseconds
     */
    static CARAPI_(Int32) GetScrollDefaultDelay();

    /**
     * @return the length of the fading edges in pixels
     *
     * @deprecated Use {@link #getScaledFadingEdgeLength()} instead.
     */
    static CARAPI_(Int32) GetFadingEdgeLength();

    /**
     * @return the length of the fading edges in pixels
     */
    CARAPI_(Int32) GetScaledFadingEdgeLength();

    /**
     * @return the duration in milliseconds of the pressed state in child
     * components.
     */
    static CARAPI_(Int32) GetPressedStateDuration();

    /**
     * @return the duration in milliseconds before a press turns into
     * a Int64 press
     */
    static CARAPI_(Int32) GetLongPressTimeout();

    /**
     * @return the duration in milliseconds we will wait to see if a touch event
     * is a tap or a scroll. If the user does not move within this interval, it is
     * considered to be a tap.
     */
    static CARAPI_(Int32) GetTapTimeout();

    /**
     * @return the duration in milliseconds we will wait to see if a touch event
     * is a jump tap. If the user does not move within this interval, it is
     * considered to be a tap.
     */
    static CARAPI_(Int32) GetJumpTapTimeout();

    /**
     * @return the duration in milliseconds between the first tap's up event and
     * the second tap's down event for an interaction to be considered a
     * double-tap.
     */
    static CARAPI_(Int32) GetDoubleTapTimeout();

    /**
     * @return Inset in pixels to look for touchable content when the user touches the edge of the
     *         screen
     *
     * @deprecated Use {@link #getScaledEdgeSlop()} instead.
     */
    static CARAPI_(Int32) GetEdgeSlop();

    /**
     * @return Inset in pixels to look for touchable content when the user touches the edge of the
     *         screen
     */
    CARAPI_(Int32) GetScaledEdgeSlop();

    /**
     * @return Distance a touch can wander before we think the user is scrolling in pixels
     *
     * @deprecated Use {@link #getScaledTouchSlop()} instead.
     */
    static CARAPI_(Int32) GetTouchSlop();

    /**
     * @return Distance a touch can wander before we think the user is scrolling in pixels
     */
    CARAPI_(Int32) GetScaledTouchSlop();

    /**
     * @return Distance a touch can wander before we think the user is scrolling a full page
     *         in dips
     */
    CARAPI_(Int32) GetScaledPagingTouchSlop();

    /**
     * @return Distance between the first touch and second touch to still be
     *         considered a double tap
     * @deprecated Use {@link #getScaledDoubleTapSlop()} instead.
     * @hide The only client of this should be GestureDetector, which needs this
     *       for clients that still use its deprecated constructor.
     */
    static CARAPI_(Int32) GetDoubleTapSlop();

    /**
     * @return Distance between the first touch and second touch to still be
     *         considered a double tap
     */
    CARAPI_(Int32) GetScaledDoubleTapSlop();

    /**
     * @return Distance a touch must be outside the bounds of a window for it
     * to be counted as outside the window for purposes of dismissing that
     * window.
     *
     * @deprecated Use {@link #getScaledWindowTouchSlop()} instead.
     */
    static CARAPI_(Int32) GetWindowTouchSlop();

    /**
     * @return Distance a touch must be outside the bounds of a window for it
     * to be counted as outside the window for purposes of dismissing that
     * window.
     */
    CARAPI_(Int32) GetScaledWindowTouchSlop();

    /**
     * @return Minimum velocity to initiate a fling, as measured in pixels per second.
     *
     * @deprecated Use {@link #getScaledMinimumFlingVelocity()} instead.
     */
    static CARAPI_(Int32) GetMinimumFlingVelocity();

    /**
     * @return Minimum velocity to initiate a fling, as measured in pixels per second.
     */
    CARAPI_(Int32) GetScaledMinimumFlingVelocity();

    /**
     * @return Maximum velocity to initiate a fling, as measured in pixels per second.
     *
     * @deprecated Use {@link #getScaledMaximumFlingVelocity()} instead.
     */
    static CARAPI_(Int32) GetMaximumFlingVelocity();

    /**
     * @return Maximum velocity to initiate a fling, as measured in pixels per second.
     */
    CARAPI_(Int32) GetScaledMaximumFlingVelocity();

    /**
     * The maximum drawing cache size expressed in bytes.
     *
     * @return the maximum size of View's drawing cache expressed in bytes
     *
     * @deprecated Use {@link #getScaledMaximumDrawingCacheSize()} instead.
     */
    static CARAPI_(Int32) GetMaximumDrawingCacheSize();

    /**
     * The maximum drawing cache size expressed in bytes.
     *
     * @return the maximum size of View's drawing cache expressed in bytes
     */
    CARAPI_(Int32) GetScaledMaximumDrawingCacheSize();

    /**
     * @return The maximum distance a View should overscroll by when showing edge effects.
     */
    CARAPI_(Int32) GetScaledOverscrollDistance();

    /**
     * @return The maximum distance a View should overfling by when showing edge effects.
     */
    CARAPI_(Int32) GetScaledOverflingDistance();

    /**
     * The amount of time that the zoom controls should be
     * displayed on the screen expressed in milliseconds.
     *
     * @return the time the zoom controls should be visible expressed
     * in milliseconds.
     */
    static CARAPI_(Int64) GetZoomControlsTimeout();

    /**
     * The amount of time a user needs to press the relevant key to bring up
     * the global actions dialog.
     *
     * @return how Int64 a user needs to press the relevant key to bring up
     *   the global actions dialog.
     */
    static CARAPI_(Int64) GetGlobalActionKeyTimeout();

    /**
     * The amount of friction applied to scrolls and flings.
     *
     * @return A scalar dimensionless value representing the coefficient of
     *         friction.
     */
    static CARAPI_(Float) GetScrollFriction();

private:
    /**
     * Defines the width of the horizontal scrollbar and the height of the vertical scrollbar in
     * pixels
     */
    static const Int32 SCROLL_BAR_SIZE = 10;

    /**
     * Duration of the fade when scrollbars fade away in milliseconds
     */
    static const Int32 SCROLL_BAR_FADE_DURATION = 250;

    /**
     * Default delay before the scrollbars fade in milliseconds
     */
    static const Int32 SCROLL_BAR_DEFAULT_DELAY = 300;

    /**
     * Defines the length of the fading edges in pixels
     */
    static const Int32 FADING_EDGE_LENGTH = 12;

    /**
     * Defines the duration in milliseconds of the pressed state in child
     * components.
     */
    static const Int32 PRESSED_STATE_DURATION = 125;

    /**
     * Defines the duration in milliseconds before a press turns into
     * a Int64 press
     */
    static const Int32 LONG_PRESS_TIMEOUT = 500;

    /**
     * Defines the duration in milliseconds a user needs to hold down the
     * appropriate button to bring up the global actions dialog (power off,
     * lock screen, etc).
     */
    static const Int32 GLOBAL_ACTIONS_KEY_TIMEOUT = 500;

    /**
     * Defines the duration in milliseconds we will wait to see if a touch event
     * is a tap or a scroll. If the user does not move within this interval, it is
     * considered to be a tap.
     */
    static const Int32 TAP_TIMEOUT = 115;

    /**
     * Defines the duration in milliseconds we will wait to see if a touch event
     * is a jump tap. If the user does not complete the jump tap within this interval, it is
     * considered to be a tap.
     */
    static const Int32 JUMP_TAP_TIMEOUT = 500;

    /**
     * Defines the duration in milliseconds between the first tap's up event and
     * the second tap's down event for an interaction to be considered a
     * double-tap.
     */
    static const Int32 DOUBLE_TAP_TIMEOUT = 300;

    /**
     * Defines the duration in milliseconds we want to display zoom controls in response
     * to a user panning within an application.
     */
    static const Int32 ZOOM_CONTROLS_TIMEOUT = 3000;

    /**
     * Inset in pixels to look for touchable content when the user touches the edge of the screen
     */
    static const Int32 EDGE_SLOP = 12;

    /**
     * Distance a touch can wander before we think the user is scrolling in pixels
     */
    static const Int32 TOUCH_SLOP = 16;

    /**
     * Distance a touch can wander before we think the user is attempting a paged scroll
     * (in dips)
     */
    static const Int32 PAGING_TOUCH_SLOP = TOUCH_SLOP * 2;

    /**
     * Distance between the first touch and second touch to still be considered a double tap
     */
    static const Int32 DOUBLE_TAP_SLOP = 100;

    /**
     * Distance a touch needs to be outside of a window's bounds for it to
     * count as outside for purposes of dismissing the window.
     */
    static const Int32 WINDOW_TOUCH_SLOP = 16;

    /**
     * Minimum velocity to initiate a fling, as measured in pixels per second
     */
    static const Int32 MINIMUM_FLING_VELOCITY = 50;

    /**
     * Maximum velocity to initiate a fling, as measured in pixels per second
     */
    static const Int32 MAXIMUM_FLING_VELOCITY = 4000;

    /**
     * The maximum size of View's drawing cache, expressed in bytes. This size
     * should be at least equal to the size of the screen in ARGB888 format.
     */
    static const Int32 MAXIMUM_DRAWING_CACHE_SIZE = 320 * 480 * 4; // HVGA screen, ARGB8888

    /**
     * Max distance to overscroll for edge effects
     */
    static const Int32 OVERSCROLL_DISTANCE = 0;

    /**
     * Max distance to overfling for edge effects
     */
    static const Int32 OVERFLING_DISTANCE = 4;

    /**
     * The coefficient of friction applied to flings/scrolls.
     */
    static Float SCROLL_FRICTION;

private:
    Int32 mEdgeSlop;
    Int32 mFadingEdgeLength;
    Int32 mMinimumFlingVelocity;
    Int32 mMaximumFlingVelocity;
    Int32 mScrollbarSize;
    Int32 mTouchSlop;
    Int32 mPagingTouchSlop;
    Int32 mDoubleTapSlop;
    Int32 mWindowTouchSlop;
    Int32 mMaximumDrawingCacheSize;
    Int32 mOverscrollDistance;
    Int32 mOverflingDistance;

    static Map<Int32, AutoPtr<ViewConfiguration> > sConfigurations;
};
#endif //__VIEWCONFIGURATION_H__
