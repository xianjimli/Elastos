
#include "widget/CNumberPickerButton.h"

IVIEW_METHODS_IMPL(CNumberPickerButton, NumberPickerButton, NumberPickerButton);

IDrawableCallback_METHODS_IMPL(CNumberPickerButton, NumberPickerButton, NumberPickerButton);

IKeyEventCallback_METHODS_IMPL(CNumberPickerButton, NumberPickerButton, NumberPickerButton);

IAccessibilityEventSource_METHODS_IMPL(CNumberPickerButton, NumberPickerButton, NumberPickerButton);

PInterface CNumberPickerButton::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CNumberPickerButton::Probe(riid);
}

ECode CNumberPickerButton::SetAdjustViewBounds(
    /* [in] */ Boolean adjustViewBounds)
{

    return NumberPickerButton::SetAdjustViewBounds(adjustViewBounds);
}

ECode CNumberPickerButton::SetMaxWidth(
    /* [in] */ Int32 maxWidth)
{
    return NumberPickerButton::SetMaxWidth(maxWidth);
}

ECode CNumberPickerButton::SetMaxHeight(
    /* [in] */ Int32 maxHeight)
{
    return NumberPickerButton::SetMaxHeight(maxHeight);
}

ECode CNumberPickerButton::GetDrawable(
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);

    AutoPtr<IDrawable> temp = NumberPickerButton::GetDrawable();
    *drawable = temp;
    (*drawable)->AddRef();
    return NOERROR;
}

ECode CNumberPickerButton::SetImageResource(
    /* [in] */ Int32 resId)
{
    return NumberPickerButton::SetImageResource(resId);
}

ECode CNumberPickerButton::SetImageURI(
    /* [in] */ IUri* uri)
{
    return NumberPickerButton::SetImageURI(uri);
}

ECode CNumberPickerButton::SetImageDrawable(
    /* [in] */ IDrawable* drawable)
{
    // TODO: Add your NumberPickerButton::SetAdjustViewBounds(adjustViewBounds) here
    return NumberPickerButton::SetImageDrawable(drawable);
}

ECode CNumberPickerButton::SetImageBitmap(
    /* [in] */ IBitmap* bm)
{
    return NumberPickerButton::SetImageBitmap(bm);
}

ECode CNumberPickerButton::SetImageState(
    /* [in] */ const ArrayOf<Int32> & state,
    /* [in] */ Boolean mg)
{
    return NumberPickerButton::SetImageState(state, mg);
}

ECode CNumberPickerButton::SetImageLevel(
    /* [in] */ Int32 level)
{
    return NumberPickerButton::SetImageLevel(level);
}

ECode CNumberPickerButton::SetScaleType(
    /* [in] */ ImageViewScaleType scaleType)
{
    return NumberPickerButton::SetScaleType(scaleType);
}

ECode CNumberPickerButton::GetScaleType(
    /* [out] */ ImageViewScaleType* scaleType)
{
    VALIDATE_NOT_NULL(scaleType);

    *scaleType = NumberPickerButton::GetScaleType();
    return NOERROR;
}

ECode CNumberPickerButton::GetImageMatrix(
    /* [out] */ IMatrix** matrix)
{
    VALIDATE_NOT_NULL(matrix);

    AutoPtr<IMatrix> temp = NumberPickerButton::GetImageMatrix();
    *matrix = temp;
    (*matrix)->AddRef();
    return NOERROR;
}

ECode CNumberPickerButton::SetImageMatrix(
    /* [in] */ IMatrix* matrix)
{
    return NumberPickerButton::SetImageMatrix(matrix);
}

ECode CNumberPickerButton::SetNumberPicker(
    /* [in] */ INumberPicker * picker)
{
    return NumberPickerButton::SetNumberPicker(picker);
}

ECode CNumberPickerButton::constructor(
    /* [in] */ IContext* ctx)
{
    return constructor(ctx, NULL);
}

ECode CNumberPickerButton::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs)
{
    return constructor(ctx, attrs,
            0x01010072/*com.android.internal.R.attr.imageButtonStyle*/);
}

ECode CNumberPickerButton::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    NumberPickerButton::Init(ctx, attrs, defStyle);
    return NOERROR;
}

