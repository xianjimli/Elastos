
#include "widget/CScrollView.h"
#include "widget/FrameLayout.h"

IVIEW_METHODS_IMPL(CScrollView, ScrollView, ScrollView);

IVIEWGROUP_METHODS_IMPL(CScrollView, ScrollView, ScrollView);

IVIEWPARENT_METHODS_IMPL(CScrollView, ScrollView, ScrollView);

IVIEWMANAGER_METHODS_IMPL(CScrollView, ScrollView, ScrollView);

IKeyEventCallback_METHODS_IMPL(CScrollView, ScrollView, ScrollView);

PInterface CScrollView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CScrollView::Probe(riid);
}

ECode CScrollView::SetForegroundGravity(
    /* [in] */ Int32 foregroundGravity)
{
    FrameLayout::SetForegroundGravity(foregroundGravity);

    return NOERROR;
}

ECode CScrollView::SetForeground(
    /* [in] */ IDrawable* drawable)
{
    FrameLayout::SetForeground(drawable);

    return NOERROR;
}

ECode CScrollView::GetForeground(
    /* [out] */ IDrawable** foreground)
{
    VALIDATE_NOT_NULL(foreground);
    AutoPtr<IDrawable> d = FrameLayout::GetForeground();
    *foreground = d.Get();
    if (*foreground) {
        (*foreground)->AddRef();
    }

    return NOERROR;
}

ECode CScrollView::SetMeasureAllChildren(
    /* [in] */ Boolean measureAll)
{
    FrameLayout::SetMeasureAllChildren(measureAll);

    return NOERROR;
}

ECode CScrollView::GetConsiderGoneChildrenWhenMeasuring(
    /* [out] */ Boolean* measureAll)
{
    VALIDATE_NOT_NULL(measureAll);
    *measureAll = FrameLayout::GetConsiderGoneChildrenWhenMeasuring();

    return NOERROR;
}

/**
 * Handle scrolling in response to an up or down arrow click.
 *
 * @param direction The direction corresponding to the arrow key that was
 *                  pressed
 * @return True if we consumed the event, false otherwise
 */
ECode CScrollView::ArrowScroll(
    /* [in] */ Int32 direction,
    /* [out] */ Boolean* consumed)
{
    return ScrollView::ArrowScroll(direction, consumed);
}

/**
 * You can call this function yourself to have the scroll view perform
 * scrolling from a key event, just as if the event had been dispatched to
 * it by the view hierarchy.
 *
 * @param event The key event to execute.
 * @return Return true if the event was handled, else false.
 */
ECode CScrollView::ExecuteKeyEvent(
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* handled)
{
    return ScrollView::ExecuteKeyEvent(event, handled);
}

/**
 * Fling the scroll view
 *
 * @param velocityY The initial velocity in the Y direction. Positive
 *                  numbers mean that the finger/cursor is moving down the screen,
 *                  which means we want to scroll towards the top.
 */
ECode CScrollView::Fling(
    /* [in] */ Int32 velocityY)
{
    return ScrollView::Fling(velocityY);
}

/**
 * <p>Handles scrolling in response to a "home/end" shortcut press. This
 * method will scroll the view to the top or bottom and give the focus
 * to the topmost/bottommost component in the new visible area. If no
 * component is a good candidate for focus, this scrollview reclaims the
 * focus.</p>
 *
 * @param direction the scroll direction: {@link android.view.View#FOCUS_UP}
 *                  to go the top of the view or
 *                  {@link android.view.View#FOCUS_DOWN} to go the bottom
 * @return true if the key event is consumed by this method, false otherwise
 */
ECode CScrollView::FullScroll(
    /* [in] */ Int32 direction,
    /* [out] */ Boolean* consumed)
{
    return ScrollView::FullScroll(direction, consumed);
}

/**
 * @return The maximum amount this scroll view will scroll in response to
 *   an arrow event.
 */
ECode CScrollView::GetMaxScrollAmount(
    /* [out] */ Int32* maxAmount)
{
    return ScrollView::GetMaxScrollAmount(maxAmount);
}

/**
 * Indicates whether this ScrollView's content is stretched to fill the viewport.
 *
 * @return True if the content fills the viewport, false otherwise.
 */
ECode CScrollView::IsFillViewport(
    /* [out] */ Boolean* isFilled)
{
    return ScrollView::IsFillViewport(isFilled);
}

/**
 * @return Whether arrow scrolling will animate its transition.
 */
ECode CScrollView::IsSmoothScrollingEnabled(
    /* [out] */ Boolean* enabled)
{
    return ScrollView::IsSmoothScrollingEnabled(enabled);
}

/**
 * <p>Handles scrolling in response to a "page up/down" shortcut press. This
 * method will scroll the view by one page up or down and give the focus
 * to the topmost/bottommost component in the new visible area. If no
 * component is a good candidate for focus, this scrollview reclaims the
 * focus.</p>
 *
 * @param direction the scroll direction: {@link android.view.View#FOCUS_UP}
 *                  to go one page up or
 *                  {@link android.view.View#FOCUS_DOWN} to go one page down
 * @return true if the key event is consumed by this method, false otherwise
 */
ECode CScrollView::PageScroll(
    /* [in] */ Int32 direction,
    /* [out] */ Boolean* consumed)
{
    return ScrollView::PageScroll(direction, consumed);
}

/**
 * Indicates this ScrollView whether it should stretch its content height to fill
 * the viewport or not.
 *
 * @param fillViewport True to stretch the content's height to the viewport's
 *        boundaries, false otherwise.
 */
ECode CScrollView::SetFillViewport(
    /* [in] */ Boolean fillViewport)
{
    return ScrollView::SetFillViewport(fillViewport);
}

/**
 * Set whether arrow scrolling will animate its transition.
 * @param smoothScrollingEnabled whether arrow scrolling will animate its transition
 */
ECode CScrollView::SetSmoothScrollingEnabled(
    /* [in] */ Boolean smoothScrollingEnabled)
{
    return ScrollView::SetSmoothScrollingEnabled(smoothScrollingEnabled);
}

/**
 * Like {@link View#scrollBy}, but scroll smoothly instead of immediately.
 *
 * @param dx the number of pixels to scroll by on the X axis
 * @param dy the number of pixels to scroll by on the Y axis
 */
ECode CScrollView::SmoothScrollBy(
    /* [in] */ Int32 dx,
    /* [in] */ Int32 dy)
{
    return ScrollView::SmoothScrollBy(dx, dy);
}

/**
 * Like {@link #scrollTo}, but scroll smoothly instead of immediately.
 *
 * @param x the position where to scroll on the X axis
 * @param y the position where to scroll on the Y axis
 */
ECode CScrollView::SmoothScrollTo(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    return ScrollView::SmoothScrollTo(x, y);
}

ECode CScrollView::constructor(
    /* [in] */ IContext* ctx)
{
    return ScrollView::Init(ctx);
}

ECode CScrollView::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs)
{
    return ScrollView::Init(ctx, attrs);
}

ECode CScrollView::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return ScrollView::Init(ctx, attrs, defStyle);
}
