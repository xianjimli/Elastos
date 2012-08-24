
#include "widget/CToggleButton.h"

IVIEW_METHODS_IMPL(CToggleButton, ToggleButton, ToggleButton);

IDrawableCallback_METHODS_IMPL(CToggleButton, ToggleButton, ToggleButton);

IKeyEventCallback_METHODS_IMPL(CToggleButton, ToggleButton, ToggleButton);

IAccessibilityEventSource_METHODS_IMPL(CToggleButton, ToggleButton, ToggleButton);

ITEXTVIEW_METHODS_IMPL(CToggleButton, ToggleButton, ToggleButton);

PInterface CToggleButton::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CToggleButton::Probe(riid);
}

ECode CToggleButton::Toggle()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CToggleButton::IsChecked(
    /* [out] */ Boolean * pChecked)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CToggleButton::SetChecked(
    /* [in] */ Boolean checked)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CToggleButton::SetOnCheckedChangeListener(
    /* [in] */ IOnCheckedChangeListener * pListener)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CToggleButton::SetButtonDrawable(
    /* [in] */ Int32 resid)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CToggleButton::SetButtonDrawableEx(
    /* [in] */ IDrawable * pD)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CToggleButton::GetTextOn(
    /* [out] */ ICharSequence ** ppOn)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CToggleButton::SetTextOn(
    /* [in] */ ICharSequence * pTextOn)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CToggleButton::GetTextOff(
    /* [out] */ ICharSequence ** ppOn)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CToggleButton::SetTextOff(
    /* [in] */ ICharSequence * pTextOff)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CToggleButton::constructor(
    /* [in] */ IContext * pCtx)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CToggleButton::constructor(
    /* [in] */ IContext * pCtx,
    /* [in] */ IAttributeSet * pAttrs)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CToggleButton::constructor(
    /* [in] */ IContext * pContext,
    /* [in] */ IAttributeSet * pAttrs,
    /* [in] */ Int32 defStyle)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CToggleButton::OnPreDraw(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = ToggleButton::OnPreDraw();

    return NOERROR;
}
