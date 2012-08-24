
#include "widget/CCheckBox.h"

IVIEW_METHODS_IMPL(CCheckBox, CheckBox, CheckBox);

IDrawableCallback_METHODS_IMPL(CCheckBox, CheckBox, CCheckBox);

IKeyEventCallback_METHODS_IMPL(CCheckBox, CheckBox, CCheckBox);

IAccessibilityEventSource_METHODS_IMPL(CCheckBox, CheckBox, CCheckBox);

ITEXTVIEW_METHODS_IMPL(CCheckBox, CheckBox, CheckBox);

PInterface CCheckBox::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CCheckBox::Probe(riid);
}

ECode CCheckBox::Toggle()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCheckBox::IsChecked(
    /* [out] */ Boolean * pChecked)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCheckBox::SetChecked(
    /* [in] */ Boolean checked)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCheckBox::SetOnCheckedChangeListener(
    /* [in] */ IOnCheckedChangeListener * pListener)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCheckBox::SetButtonDrawable(
    /* [in] */ Int32 resid)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCheckBox::SetButtonDrawableEx(
    /* [in] */ IDrawable * pD)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCheckBox::constructor(
    /* [in] */ IContext * pCtx)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCheckBox::constructor(
    /* [in] */ IContext * pCtx,
    /* [in] */ IAttributeSet * pAttrs)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCheckBox::constructor(
    /* [in] */ IContext * pContext,
    /* [in] */ IAttributeSet * pAttrs,
    /* [in] */ Int32 defStyle)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCheckBox::OnPreDraw(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = CheckBox::OnPreDraw();

    return NOERROR;
}
