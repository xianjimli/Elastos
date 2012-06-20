
#include "widget/CRadioButton.h"

IVIEW_METHODS_IMPL(CRadioButton, RadioButton, RadioButton);

ITEXTVIEW_METHODS_IMPL(CRadioButton, RadioButton, RadioButton);

CRadioButton::CRadioButton()
{}

PInterface CRadioButton::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CRadioButton::Probe(riid);
}

ECode CRadioButton::Toggle()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CRadioButton::IsChecked(
    /* [out] */ Boolean * pChecked)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CRadioButton::SetChecked(
    /* [in] */ Boolean checked)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CRadioButton::SetOnCheckedChangeListener(
    /* [in] */ IOnCheckedChangeListener * pListener)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CRadioButton::SetButtonDrawable(
    /* [in] */ Int32 resid)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CRadioButton::SetButtonDrawableEx(
    /* [in] */ IDrawable * pD)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CRadioButton::constructor(
    /* [in] */ IContext * pCtx)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CRadioButton::constructor(
    /* [in] */ IContext * pCtx,
    /* [in] */ IAttributeSet * pAttrs)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CRadioButton::constructor(
    /* [in] */ IContext * pContext,
    /* [in] */ IAttributeSet * pAttrs,
    /* [in] */ Int32 defStyle)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

