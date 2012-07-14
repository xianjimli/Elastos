
#include "widget/CImageView.h"

IVIEW_METHODS_IMPL(CImageView, ImageView, ImageView);

IDrawableCallback_METHODS_IMPL(CImageView, ImageView, ImageView);

IKeyEventCallback_METHODS_IMPL(CImageView, ImageView, ImageView);

IAccessibilityEventSource_METHODS_IMPL(CImageView, ImageView, ImageView);

ECode CImageView::constructor(
    /* [in] */ IContext * pCtx)
{
    return Init(pCtx);
}

ECode CImageView::constructor(
    /* [in] */ IContext*  ctx,
    /* [in] */ IAttributeSet* attrs)
{
    return Init(ctx, attrs);
}

ECode CImageView::constructor(
    /* [in] */ IContext * pCtx,
    /* [in] */ IAttributeSet * pAttrs,
    /* [in] */ Int32 defStyle)
{
    return Init(pCtx, pAttrs, defStyle);
}

PInterface CImageView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CImageView::Probe(riid);
}


ECode CImageView::SetAdjustViewBounds(
    /* [in] */ Boolean adjustViewBounds)
{
    return ImageView::SetAdjustViewBounds(adjustViewBounds);
}

ECode CImageView::SetMaxWidth(
    /* [in] */ Int32 maxWidth)
{
    return ImageView::SetMaxHeight(maxWidth);
}

ECode CImageView::SetMaxHeight(
    /* [in] */ Int32 maxHeight)
{
    return ImageView::SetMaxHeight(maxHeight);
}

ECode CImageView::GetDrawable(
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    AutoPtr<IDrawable> temp = ImageView::GetDrawable();
    *drawable = temp;
    if (*drawable) {
        (*drawable)->AddRef();
    }

    return NOERROR;
}

ECode CImageView::SetImageResource(
    /* [in] */ Int32 resId)
{
    return ImageView::SetImageResource(resId);
}

ECode CImageView::SetImageURI(
    /* [in] */ IUri* uri)
{
    return ImageView::SetImageURI(uri);
}

ECode CImageView::SetImageDrawable(
    /* [in] */ IDrawable* drawable)
{
    return ImageView::SetImageDrawable(drawable);
}

ECode CImageView::SetImageBitmap(
    /* [in] */ IBitmap* bm)
{
    return ImageView::SetImageBitmap(bm);
}

ECode CImageView::SetImageState(
    /* [in] */ const ArrayOf<Int32>& state,
    /* [in] */ Boolean merge)
{
    return ImageView::SetImageState(state, merge);
}

ECode CImageView::SetImageLevel(
    /* [in] */ Int32 level)
{
    return ImageView::SetImageLevel(level);
}

ECode CImageView::SetScaleType(
    /* [in] */ ImageViewScaleType scaleType)
{
    return ImageView::SetScaleType(scaleType);
}

ECode CImageView::GetScaleType(
    /* [out] */ ImageViewScaleType* scaleType)
{
    VALIDATE_NOT_NULL(scaleType);
    *scaleType = ImageView::GetScaleType();

    return NOERROR;
}

ECode CImageView::GetImageMatrix(
    /* [out] */ IMatrix** martix)
{
    VALIDATE_NOT_NULL(martix);
    AutoPtr<IMatrix> temp = ImageView::GetImageMatrix();
    *martix = temp;
    if (*martix) {
        (*martix)->AddRef();
    }

    return NOERROR;
}

ECode CImageView::SetImageMatrix(
    /* [in] */ IMatrix* matrix)
{
    return ImageView::SetImageMatrix(matrix);
}
