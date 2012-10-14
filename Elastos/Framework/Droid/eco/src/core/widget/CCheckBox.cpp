
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
    return CheckBox::Toggle();
}

ECode CCheckBox::IsChecked(
    /* [out] */ Boolean* checked)
{
    VALIDATE_NOT_NULL(checked);
    *checked = CheckBox::IsChecked();

    return NOERROR;
}

ECode CCheckBox::SetChecked(
    /* [in] */ Boolean checked)
{
    return CheckBox::SetChecked(checked);
}

ECode CCheckBox::SetOnCheckedChangeListener(
    /* [in] */ ICompoundButtonOnCheckedChangeListener* listener)
{
    return CheckBox::SetOnCheckedChangeListener(listener);
}

ECode CCheckBox::SetButtonDrawable(
    /* [in] */ Int32 resid)
{
    return CheckBox::SetButtonDrawable(resid);
}

ECode CCheckBox::SetButtonDrawableEx(
    /* [in] */ IDrawable* d)
{
    return CheckBox::SetButtonDrawable(d);
}

ECode CCheckBox::constructor(
    /* [in] */ IContext* ctx)
{
    return CheckBox::Init(ctx);
}

ECode CCheckBox::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs)
{
    return CheckBox::Init(ctx, attrs);
}

ECode CCheckBox::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* atrs,
    /* [in] */ Int32 defStyle)
{
    return CheckBox::Init(context, atrs, defStyle);
}

ECode CCheckBox::OnPreDraw(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = CheckBox::OnPreDraw();

    return NOERROR;
}
