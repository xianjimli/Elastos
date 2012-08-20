
#include "widget/CGallery.h"

IVIEW_METHODS_IMPL(CGallery, Gallery, Gallery);

IVIEWGROUP_METHODS_IMPL(CGallery, Gallery, Gallery);

IADAPTERVIEW_METHODS_IMPL(CGallery, Gallery, Gallery);

IVIEWPARENT_METHODS_IMPL(CGallery, Gallery, Gallery);

IVIEWMANAGER_METHODS_IMPL(CGallery, Gallery, Gallery);

IDrawableCallback_METHODS_IMPL(CGallery, Gallery, Gallery);

IKeyEventCallback_METHODS_IMPL(CGallery, Gallery, Gallery);

IAccessibilityEventSource_METHODS_IMPL(CGallery, Gallery, Gallery);

PInterface CGallery::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CGallery::Probe(riid);
}

ECode CGallery::SetSelectionEx(
    /* [in] */ Int32 position,
    /* [in] */ Boolean animate)
{
    return Gallery::SetSelection(position, animate);
}

ECode CGallery::PointToPosition(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [out] */ Int32* pos)
{
    VALIDATE_NOT_NULL(pos);
    Gallery::PointToPosition(x, y);

    return NOERROR;
}

ECode CGallery::SetCallbackDuringFling(
    /* [in] */ Boolean shouldCallback)
{
    return Gallery::SetCallbackDuringFling(shouldCallback);
}

ECode CGallery::SetCallbackOnUnselectedItemClick(
    /* [in] */ Boolean shouldCallback)
{
    return Gallery::SetCallbackOnUnselectedItemClick(shouldCallback);
}

ECode CGallery::SetAnimationDuration(
    /* [in] */ Int32 animationDurationMillis)
{
    return Gallery::SetAnimationDuration(animationDurationMillis);
}

ECode CGallery::SetSpacing(
    /* [in] */ Int32 spacing)
{
    return Gallery::SetSpacing(spacing);
}

ECode CGallery::SetUnselectedAlpha(
    /* [in] */ Float unselectedAlpha)
{
    return Gallery::SetUnselectedAlpha(unselectedAlpha);
}

ECode CGallery::OnSingleTapUp(
    /* [in] */ IMotionEvent* e,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = Gallery::OnSingleTapUp(e);

    return NOERROR;
}

ECode CGallery::OnFling(
    /* [in] */ IMotionEvent* e1,
    /* [in] */ IMotionEvent* e2,
    /* [in] */ Float velocityX,
    /* [in] */ Float velocityY,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = Gallery::OnFling(e1, e2, velocityX, velocityY);

    return NOERROR;
}

ECode CGallery::OnScroll(
    /* [in] */ IMotionEvent* e1,
    /* [in] */ IMotionEvent* e2,
    /* [in] */ Float distanceX,
    /* [in] */ Float distanceY,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = Gallery::OnScroll(e1, e2, distanceX, distanceY);

    return NOERROR;
}

ECode CGallery::OnDown(
    /* [in] */ IMotionEvent* e,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = Gallery::OnDown(e);

    return NOERROR;
}

ECode CGallery::OnLongPress(
    /* [in] */ IMotionEvent* e)
{
    return Gallery::OnLongPress(e);
}

ECode CGallery::OnShowPress(
    /* [in] */ IMotionEvent* e)
{
    return Gallery::OnShowPress(e);
}

ECode CGallery::SetGravity(
    /* [in] */ Int32 gravity)
{
    return Gallery::SetGravity(gravity);
}

ECode CGallery::constructor(
    /* [in] */ IContext* ctx)
{
    return Gallery::Init(ctx);
}

ECode CGallery::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs)
{
    return Gallery::Init(ctx, attrs);
}

ECode CGallery::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return Gallery::Init(ctx, attrs, defStyle);
}

