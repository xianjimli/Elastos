
#include "widget/CImageButton.h"

IVIEW_METHODS_IMPL(CImageButton, ImageButton, ImageButton);

IDrawableCallback_METHODS_IMPL(CImageButton, ImageButton, ImageButton);

IKeyEventCallback_METHODS_IMPL(CImageButton, ImageButton, ImageButton);

IAccessibilityEventSource_METHODS_IMPL(CImageButton, ImageButton, ImageButton);

PInterface CImageButton::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CImageButton::Probe(riid);
}

ECode CImageButton::SetAdjustViewBounds(
    /* [in] */ Boolean adjustViewBounds)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CImageButton::SetMaxWidth(
    /* [in] */ Int32 maxWidth)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CImageButton::SetMaxHeight(
    /* [in] */ Int32 maxHeight)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CImageButton::GetDrawable(
    /* [out] */ IDrawable ** ppDrawable)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CImageButton::SetImageResource(
    /* [in] */ Int32 resId)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CImageButton::SetImageURI(
    /* [in] */ IUri * pUri)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CImageButton::SetImageDrawable(
    /* [in] */ IDrawable * pDrawable)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CImageButton::SetImageBitmap(
    /* [in] */ IBitmap * pBm)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CImageButton::SetImageState(
    /* [in] */ const ArrayOf<Int32> & state,
    /* [in] */ Boolean mg)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CImageButton::SetImageLevel(
    /* [in] */ Int32 level)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CImageButton::SetScaleType(
    /* [in] */ ImageViewScaleType scaleType)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CImageButton::GetScaleType(
    /* [out] */ ImageViewScaleType * pScaleType)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CImageButton::GetImageMatrix(
    /* [out] */ IMatrix ** ppMartix)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CImageButton::SetImageMatrix(
    /* [in] */ IMatrix * pMatrix)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CImageButton::constructor(
    /* [in] */ IContext * pCtx)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CImageButton::constructor(
    /* [in] */ IContext * pCtx,
    /* [in] */ IAttributeSet * pAttrs)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CImageButton::constructor(
    /* [in] */ IContext * pCtx,
    /* [in] */ IAttributeSet * pAttrs,
    /* [in] */ Int32 defStyle)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

