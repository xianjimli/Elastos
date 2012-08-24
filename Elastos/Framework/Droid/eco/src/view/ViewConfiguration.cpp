
#include "view/ViewConfiguration.h"
#include "utils/CDisplayMetrics.h"

#include <stdio.h>

const Int32 ViewConfiguration::SCROLL_BAR_SIZE;
const Int32 ViewConfiguration::SCROLL_BAR_FADE_DURATION;
const Int32 ViewConfiguration::SCROLL_BAR_DEFAULT_DELAY;
const Int32 ViewConfiguration::FADING_EDGE_LENGTH;
const Int32 ViewConfiguration::PRESSED_STATE_DURATION;
const Int32 ViewConfiguration::LONG_PRESS_TIMEOUT;
const Int32 ViewConfiguration::GLOBAL_ACTIONS_KEY_TIMEOUT;
const Int32 ViewConfiguration::TAP_TIMEOUT;
const Int32 ViewConfiguration::JUMP_TAP_TIMEOUT;
const Int32 ViewConfiguration::DOUBLE_TAP_TIMEOUT;
const Int32 ViewConfiguration::ZOOM_CONTROLS_TIMEOUT;
const Int32 ViewConfiguration::EDGE_SLOP;
const Int32 ViewConfiguration::TOUCH_SLOP;
const Int32 ViewConfiguration::PAGING_TOUCH_SLOP;
const Int32 ViewConfiguration::DOUBLE_TAP_SLOP;
const Int32 ViewConfiguration::WINDOW_TOUCH_SLOP;
const Int32 ViewConfiguration::MINIMUM_FLING_VELOCITY;
const Int32 ViewConfiguration::MAXIMUM_FLING_VELOCITY;
const Int32 ViewConfiguration::MAXIMUM_DRAWING_CACHE_SIZE;
const Int32 ViewConfiguration::OVERSCROLL_DISTANCE;
const Int32 ViewConfiguration::OVERFLING_DISTANCE;

Float ViewConfiguration::SCROLL_FRICTION = 0.015f;
Map<Int32, AutoPtr<ViewConfiguration> > ViewConfiguration::sConfigurations;

ViewConfiguration::ViewConfiguration() :
    mEdgeSlop(EDGE_SLOP),
    mFadingEdgeLength(FADING_EDGE_LENGTH),
    mMinimumFlingVelocity(MINIMUM_FLING_VELOCITY),
    mMaximumFlingVelocity(MAXIMUM_FLING_VELOCITY),
    mScrollbarSize(SCROLL_BAR_SIZE),
    mTouchSlop(TOUCH_SLOP),
    mPagingTouchSlop(PAGING_TOUCH_SLOP),
    mDoubleTapSlop(DOUBLE_TAP_SLOP),
    mWindowTouchSlop(WINDOW_TOUCH_SLOP),
    //noinspection deprecation
    mMaximumDrawingCacheSize(MAXIMUM_DRAWING_CACHE_SIZE),
    mOverscrollDistance(OVERSCROLL_DISTANCE),
    mOverflingDistance(OVERFLING_DISTANCE)
{
}

ViewConfiguration::ViewConfiguration(
    /* [in] */ IContext* context)
{
    AutoPtr<IDisplayMetrics> _metrics;
    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    res->GetDisplayMetrics((IDisplayMetrics**)&_metrics);
    CDisplayMetrics* metrics = (CDisplayMetrics*)_metrics.Get();
    const Float density = metrics->mDensity;

    mEdgeSlop = (Int32)(density * EDGE_SLOP + 0.5f);
    mFadingEdgeLength = (Int32)(density * FADING_EDGE_LENGTH + 0.5f);
    mMinimumFlingVelocity = (Int32)(density * MINIMUM_FLING_VELOCITY + 0.5f);
    mMaximumFlingVelocity = (Int32)(density * MAXIMUM_FLING_VELOCITY + 0.5f);
    mScrollbarSize = (Int32)(density * SCROLL_BAR_SIZE + 0.5f);
    mTouchSlop = (Int32)(density * TOUCH_SLOP + 0.5f);
    mPagingTouchSlop = (Int32)(density * PAGING_TOUCH_SLOP + 0.5f);
    mDoubleTapSlop = (Int32)(density * DOUBLE_TAP_SLOP + 0.5f);
    mWindowTouchSlop = (Int32)(density * WINDOW_TOUCH_SLOP + 0.5f);

    // Size of the screen in bytes, in ARGB_8888 format
    mMaximumDrawingCacheSize = 4 * metrics->mWidthPixels * metrics->mHeightPixels;

    mOverscrollDistance = (Int32)(density * OVERSCROLL_DISTANCE + 0.5f);
    mOverflingDistance = (Int32)(density * OVERFLING_DISTANCE + 0.5f);
}

AutoPtr<ViewConfiguration> ViewConfiguration::Get(
    /* [in] */ IContext* context)
{
    AutoPtr<IDisplayMetrics> metrics;
    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    res->GetDisplayMetrics((IDisplayMetrics**)&metrics);
    const Int32 density = (Int32)(100.0f * ((CDisplayMetrics*)metrics.Get())->mDensity);

    AutoPtr<ViewConfiguration> configuration;
    if (sConfigurations.Find(density) == sConfigurations.End()) {
        configuration = new ViewConfiguration(context);
        sConfigurations[density] = configuration;
    }
    else {
        configuration = sConfigurations[density];
    }

    return configuration;
}

Int32 ViewConfiguration::GetScrollBarSize()
{
    return SCROLL_BAR_SIZE;
}

Int32 ViewConfiguration::GetScaledScrollBarSize()
{
    return mScrollbarSize;
}

Int32 ViewConfiguration::GetScrollBarFadeDuration()
{
    return SCROLL_BAR_FADE_DURATION;
}

Int32 ViewConfiguration::GetScrollDefaultDelay()
{
    return SCROLL_BAR_DEFAULT_DELAY;
}

Int32 ViewConfiguration::GetFadingEdgeLength()
{
    return FADING_EDGE_LENGTH;
}

Int32 ViewConfiguration::GetScaledFadingEdgeLength()
{
    return mFadingEdgeLength;
}

Int32 ViewConfiguration::GetPressedStateDuration()
{
    return PRESSED_STATE_DURATION;
}

Int32 ViewConfiguration::GetLongPressTimeout()
{
    return LONG_PRESS_TIMEOUT;
}

Int32 ViewConfiguration::GetTapTimeout()
{
    return TAP_TIMEOUT;
}

Int32 ViewConfiguration::GetJumpTapTimeout()
{
    return JUMP_TAP_TIMEOUT;
}

Int32 ViewConfiguration::GetDoubleTapTimeout()
{
    return DOUBLE_TAP_TIMEOUT;
}

Int32 ViewConfiguration::GetEdgeSlop()
{
    return EDGE_SLOP;
}

Int32 ViewConfiguration::GetScaledEdgeSlop()
{
    return mEdgeSlop;
}

Int32 ViewConfiguration::GetTouchSlop()
{
    return TOUCH_SLOP;
}

Int32 ViewConfiguration::GetScaledTouchSlop()
{
    return mTouchSlop;
}

Int32 ViewConfiguration::GetScaledPagingTouchSlop()
{
    return mPagingTouchSlop;
}

Int32 ViewConfiguration::GetDoubleTapSlop()
{
    return DOUBLE_TAP_SLOP;
}

Int32 ViewConfiguration::GetScaledDoubleTapSlop()
{
    return mDoubleTapSlop;
}

Int32 ViewConfiguration::GetWindowTouchSlop()
{
    return WINDOW_TOUCH_SLOP;
}

Int32 ViewConfiguration::GetScaledWindowTouchSlop()
{
    return mWindowTouchSlop;
}

Int32 ViewConfiguration::GetMinimumFlingVelocity()
{
    return MINIMUM_FLING_VELOCITY;
}

Int32 ViewConfiguration::GetScaledMinimumFlingVelocity()
{
    return mMinimumFlingVelocity;
}

Int32 ViewConfiguration::GetMaximumFlingVelocity()
{
    return MAXIMUM_FLING_VELOCITY;
}

Int32 ViewConfiguration::GetScaledMaximumFlingVelocity()
{
    return mMaximumFlingVelocity;
}

Int32 ViewConfiguration::GetMaximumDrawingCacheSize()
{
    //noinspection deprecation
    return MAXIMUM_DRAWING_CACHE_SIZE;
}

Int32 ViewConfiguration::GetScaledMaximumDrawingCacheSize()
{
    return mMaximumDrawingCacheSize;
}

Int32 ViewConfiguration::GetScaledOverscrollDistance()
{
    return mOverscrollDistance;
}

Int32 ViewConfiguration::GetScaledOverflingDistance()
{
    return mOverflingDistance;
}

Int64 ViewConfiguration::GetZoomControlsTimeout()
{
    return ZOOM_CONTROLS_TIMEOUT;
}

Int64 ViewConfiguration::GetGlobalActionKeyTimeout()
{
    return GLOBAL_ACTIONS_KEY_TIMEOUT;
}

Float ViewConfiguration::GetScrollFriction()
{
    return SCROLL_FRICTION;
}
