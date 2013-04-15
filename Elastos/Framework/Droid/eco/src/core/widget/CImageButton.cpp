
#include "widget/CImageButton.h"

IVIEW_METHODS_IMPL(CImageButton, ImageButton, ImageButton);

IDrawableCallback_METHODS_IMPL(CImageButton, ImageButton, ImageButton);

IKeyEventCallback_METHODS_IMPL(CImageButton, ImageButton, ImageButton);

IAccessibilityEventSource_METHODS_IMPL(CImageButton, ImageButton, ImageButton);

ECode CImageButton::constructor(
    /* [in] */ IContext* ctx)
{
    return ImageButton::Init(ctx);
}

ECode CImageButton::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs)
{
    return ImageButton::Init(ctx, attrs);
}

ECode CImageButton::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return ImageButton::Init(ctx, attrs, defStyle);
}

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
    return ImageButton::SetAdjustViewBounds(adjustViewBounds);
}

ECode CImageButton::SetMaxWidth(
    /* [in] */ Int32 maxWidth)
{
    return ImageButton::SetMaxWidth(maxWidth);
}

ECode CImageButton::SetMaxHeight(
    /* [in] */ Int32 maxHeight)
{
    return ImageButton::SetMaxHeight(maxHeight);
}

ECode CImageButton::GetDrawable(
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);

    AutoPtr<IDrawable> d = ImageButton::GetDrawable();
    *drawable = d;
    if (*drawable != NULL) (*drawable)->AddRef();
    return NOERROR;
}

ECode CImageButton::SetImageResource(
    /* [in] */ Int32 resId)
{
    return ImageButton::SetImageResource(resId);
}

ECode CImageButton::SetImageURI(
    /* [in] */ IUri* uri)
{
    return ImageButton::SetImageURI(uri);
}

ECode CImageButton::SetImageDrawable(
    /* [in] */ IDrawable* drawable)
{
    return ImageButton::SetImageDrawable(drawable);
}

ECode CImageButton::SetImageBitmap(
    /* [in] */ IBitmap* bm)
{
    return ImageButton::SetImageBitmap(bm);
}

ECode CImageButton::SetImageState(
    /* [in] */ const ArrayOf<Int32>& state,
    /* [in] */ Boolean mg)
{
    return ImageButton::SetImageState(state, mg);
}

ECode CImageButton::SetImageLevel(
    /* [in] */ Int32 level)
{
    return ImageButton::SetImageLevel(level);
}

ECode CImageButton::SetScaleType(
    /* [in] */ ImageViewScaleType scaleType)
{
    return ImageButton::SetScaleType(scaleType);
}

ECode CImageButton::GetScaleType(
    /* [out] */ ImageViewScaleType* scaleType)
{
    VALIDATE_NOT_NULL(scaleType);

    *scaleType = ImageButton::GetScaleType();
    return NOERROR;
}

ECode CImageButton::GetImageMatrix(
    /* [out] */ IMatrix** martix)
{
    VALIDATE_NOT_NULL(martix);

    AutoPtr<IMatrix> m = ImageButton::GetImageMatrix();
    *martix = m;
    if (*martix != NULL) (*martix)->AddRef();
    return NOERROR;
}

ECode CImageButton::SetImageMatrix(
    /* [in] */ IMatrix* matrix)
{
    return ImageButton::SetImageMatrix(matrix);
}
