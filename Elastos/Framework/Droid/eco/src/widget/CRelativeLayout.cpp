
#include "widget/CRelativeLayout.h"

IVIEW_METHODS_IMPL(CRelativeLayout, RelativeLayout, RelativeLayout);

IVIEWGROUP_METHODS_IMPL(CRelativeLayout, RelativeLayout, RelativeLayout);

IVIEWPARENT_METHODS_IMPL(CRelativeLayout, RelativeLayout, RelativeLayout);

IVIEWMANAGER_METHODS_IMPL(CRelativeLayout, RelativeLayout, RelativeLayout);

IDrawableCallback_METHODS_IMPL(CRelativeLayout, RelativeLayout, RelativeLayout);

IKeyEventCallback_METHODS_IMPL(CRelativeLayout, RelativeLayout, RelativeLayout);

IAccessibilityEventSource_METHODS_IMPL(CRelativeLayout, RelativeLayout, RelativeLayout);


ECode CRelativeLayout::constructor(
    /* [in] */ IContext * pCtx)
{
    return RelativeLayout::Init(pCtx);
}

ECode CRelativeLayout::constructor(
    /* [in] */ IContext*  ctx,
    /* [in] */ IAttributeSet* attrs)
{
    return RelativeLayout::Init(ctx, attrs);
}

ECode CRelativeLayout::constructor(
    /* [in] */ IContext * pCtx,
    /* [in] */ IAttributeSet * pAttrs,
    /* [in] */ Int32 defStyle)
{
    return RelativeLayout::Init(pCtx, pAttrs, defStyle);
}

PInterface CRelativeLayout::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CRelativeLayout::Probe(riid);
}

/**
 * Defines which View is ignored when the gravity is applied. This setting has no
 * effect if the gravity is <code>Gravity.LEFT | Gravity.TOP</code>.
 *
 * @param viewId The id of the View to be ignored by gravity, or 0 if no View
 *        should be ignored.
 *
 * @see #setGravity(int)
 *
 * @attr ref android.R.styleable#RelativeLayout_ignoreGravity
 */
ECode CRelativeLayout::SetIgnoreGravity(
    /* [in] */ Int32 viewId)
{
    return RelativeLayout::SetIgnoreGravity(viewId);
}

/**
 * Describes how the child views are positioned. Defaults to
 * <code>Gravity.LEFT | Gravity.TOP</code>.
 *
 * @param gravity See {@link android.view.Gravity}
 *
 * @see #setHorizontalGravity(int)
 * @see #setVerticalGravity(int)
 *
 * @attr ref android.R.styleable#RelativeLayout_gravity
 */
ECode CRelativeLayout::SetGravity(
    /* [in] */ Int32 gravity)
{

    return RelativeLayout::SetGravity(gravity);
}

ECode CRelativeLayout::SetHorizontalGravity(
    /* [in] */ Int32 horizontalGravity)
{
    return RelativeLayout::SetHorizontalGravity(horizontalGravity);
}

ECode CRelativeLayout::SetVerticalGravity(
    /* [in] */ Int32 verticalGravity)
{
    return RelativeLayout::SetVerticalGravity(verticalGravity);
}
