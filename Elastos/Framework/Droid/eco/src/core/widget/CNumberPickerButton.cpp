
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
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CNumberPickerButton::SetMaxWidth(
    /* [in] */ Int32 maxWidth)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CNumberPickerButton::SetMaxHeight(
    /* [in] */ Int32 maxHeight)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CNumberPickerButton::GetDrawable(
    /* [out] */ IDrawable ** ppDrawable)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CNumberPickerButton::SetImageResource(
    /* [in] */ Int32 resId)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CNumberPickerButton::SetImageURI(
    /* [in] */ IUri * pUri)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CNumberPickerButton::SetImageDrawable(
    /* [in] */ IDrawable * pDrawable)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CNumberPickerButton::SetImageBitmap(
    /* [in] */ IBitmap * pBm)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CNumberPickerButton::SetImageState(
    /* [in] */ const ArrayOf<Int32> & state,
    /* [in] */ Boolean mg)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CNumberPickerButton::SetImageLevel(
    /* [in] */ Int32 level)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CNumberPickerButton::SetScaleType(
    /* [in] */ ImageViewScaleType scaleType)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CNumberPickerButton::GetScaleType(
    /* [out] */ ImageViewScaleType * pScaleType)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CNumberPickerButton::GetImageMatrix(
    /* [out] */ IMatrix ** ppMartix)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CNumberPickerButton::SetImageMatrix(
    /* [in] */ IMatrix * pMatrix)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CNumberPickerButton::SetNumberPicker(
    /* [in] */ INumberPicker * pPicker)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CNumberPickerButton::constructor(
    /* [in] */ IContext * pCtx)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CNumberPickerButton::constructor(
    /* [in] */ IContext * pCtx,
    /* [in] */ IAttributeSet * pAttrs)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CNumberPickerButton::constructor(
    /* [in] */ IContext * pCtx,
    /* [in] */ IAttributeSet * pAttrs,
    /* [in] */ Int32 defStyle)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

