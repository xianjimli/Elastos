
#include "widget/CRadioButton.h"

IVIEW_METHODS_IMPL(CRadioButton, RadioButton, RadioButton);

IDrawableCallback_METHODS_IMPL(CRadioButton, RadioButton, RadioButton);

IKeyEventCallback_METHODS_IMPL(CRadioButton, RadioButton, RadioButton);

IAccessibilityEventSource_METHODS_IMPL(CRadioButton, RadioButton, RadioButton);

ITEXTVIEW_METHODS_IMPL(CRadioButton, RadioButton, RadioButton);

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
    return RadioButton::Toggle();
}

ECode CRadioButton::IsChecked(
    /* [out] */ Boolean* checked)
{
    VALIDATE_NOT_NULL(checked);
    *checked = RadioButton::IsChecked();

    return NOERROR;
}

ECode CRadioButton::SetChecked(
    /* [in] */ Boolean checked)
{
    return RadioButton::SetChecked(checked);
}

ECode CRadioButton::SetOnCheckedChangeListener(
    /* [in] */ IOnCheckedChangeListener* listener)
{
    return RadioButton::SetOnCheckedChangeListener(listener);
}

ECode CRadioButton::SetButtonDrawable(
    /* [in] */ Int32 resid)
{
    return RadioButton::SetButtonDrawable(resid);
}

ECode CRadioButton::SetButtonDrawableEx(
    /* [in] */ IDrawable* d)
{
    return RadioButton::SetButtonDrawable(d);
}

ECode CRadioButton::constructor(
    /* [in] */ IContext* ctx)
{
    return RadioButton::Init(ctx);
}

ECode CRadioButton::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs)
{
    return RadioButton::Init(ctx, attrs);
}

ECode CRadioButton::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* atrs,
    /* [in] */ Int32 defStyle)
{
    return RadioButton::Init(context, atrs, defStyle);
}

ECode CRadioButton::OnPreDraw(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = RadioButton::OnPreDraw();

    return NOERROR;
}
