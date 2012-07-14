
#include "widget/CGallery.h"

IVIEW_METHODS_IMPL(CGallery, Gallery, Gallery);

IVIEWGROUP_METHODS_IMPL(CGallery, Gallery, Gallery);

IADAPTERVIEW_METHODS_IMPL(CGallery, Gallery, Gallery);

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
    /* [out] */ Int32 * pPos)
{
    VALIDATE_NOT_NULL(pPos);
    Gallery::PointToPosition(x, y);

    return NOERROR;
}

ECode CGallery::LayoutEx(
    /* [in] */ Int32 delta,
    /* [in] */ Boolean animate)
{
    return Gallery::LayoutEx(delta, animate);
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
    /* [in] */ IMotionEvent * pE,
    /* [out] */ Boolean * pResult)
{
    VALIDATE_NOT_NULL(pResult);
    *pResult = Gallery::OnSingleTapUp(pE);

    return NOERROR;
}

ECode CGallery::OnFling(
    /* [in] */ IMotionEvent * pE1,
    /* [in] */ IMotionEvent * pE2,
    /* [in] */ Float velocityX,
    /* [in] */ Float velocityY,
    /* [out] */ Boolean * pResult)
{
    VALIDATE_NOT_NULL(pResult);
    Gallery::OnFling(pE1, pE2, velocityX, velocityY);

    return NOERROR;
}

ECode CGallery::OnScroll(
    /* [in] */ IMotionEvent * pE1,
    /* [in] */ IMotionEvent * pE2,
    /* [in] */ Float distanceX,
    /* [in] */ Float distanceY,
    /* [out] */ Boolean * pResult)
{
    VALIDATE_NOT_NULL(pResult);
    Gallery::OnScroll(pE1, pE2, distanceX, distanceY);

    return NOERROR;
}

ECode CGallery::OnDown(
    /* [in] */ IMotionEvent * pE,
    /* [out] */ Boolean * pResult)
{
    VALIDATE_NOT_NULL(pResult);
    Gallery::OnDown(pE);

    return NOERROR;
}

ECode CGallery::OnLongPress(
    /* [in] */ IMotionEvent * pE)
{
    return Gallery::OnLongPress(pE);
}

ECode CGallery::OnShowPress(
    /* [in] */ IMotionEvent * pE)
{
    return Gallery::OnShowPress(pE);
}

ECode CGallery::SetGravity(
    /* [in] */ Int32 gravity)
{
    return Gallery::SetGravity(gravity);
}

ECode CGallery::constructor(
    /* [in] */ IContext * pCtx)
{
    Gallery::Init(pCtx);

    return NOERROR;
}

ECode CGallery::constructor(
    /* [in] */ IContext * pCtx,
    /* [in] */ IAttributeSet * pAttrs)
{
    Gallery::Init(pCtx, pAttrs);

    return NOERROR;
}

ECode CGallery::constructor(
    /* [in] */ IContext * pCtx,
    /* [in] */ IAttributeSet * pAttrs,
    /* [in] */ Int32 defStyle)
{
    Gallery::Init(pCtx, pAttrs, defStyle);

    return NOERROR;
}

