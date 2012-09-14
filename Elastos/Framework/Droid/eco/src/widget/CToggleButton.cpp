
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
    return ToggleButton::Toggle();
}

ECode CToggleButton::IsChecked(
    /* [out] */ Boolean* checked)
{
    VALIDATE_NOT_NULL(checked);
    *checked = ToggleButton::IsChecked();

    return NOERROR;
}

ECode CToggleButton::SetChecked(
    /* [in] */ Boolean checked)
{
    return ToggleButton::SetChecked(checked);
}

ECode CToggleButton::SetOnCheckedChangeListener(
    /* [in] */ IOnCheckedChangeListener* listener)
{
    return ToggleButton::SetOnCheckedChangeListener(listener);
}

ECode CToggleButton::SetButtonDrawable(
    /* [in] */ Int32 resid)
{
    return ToggleButton::SetButtonDrawable(resid);
}

ECode CToggleButton::SetButtonDrawableEx(
    /* [in] */ IDrawable* d)
{
    return ToggleButton::SetButtonDrawable(d);
}

ECode CToggleButton::GetTextOn(
    /* [out] */ ICharSequence** on)
{
    VALIDATE_NOT_NULL(on);
    AutoPtr<ICharSequence> temp = ToggleButton::GetTextOn();
    *on = temp;
    if (*on) {
        (*on)->AddRef();
    }

    return NOERROR;
}

ECode CToggleButton::SetTextOn(
    /* [in] */ ICharSequence* textOn)
{
    return ToggleButton::SetTextOn(textOn);
}

ECode CToggleButton::GetTextOff(
    /* [out] */ ICharSequence** off)
{
    VALIDATE_NOT_NULL(off);
    AutoPtr<ICharSequence> temp = ToggleButton::GetTextOff();
    *off = temp;
    if (*off) {
        (*off)->AddRef();
    }

    return NOERROR;
}

ECode CToggleButton::SetTextOff(
    /* [in] */ ICharSequence* textOff)
{
    return ToggleButton::SetTextOff(textOff);
}

ECode CToggleButton::constructor(
    /* [in] */ IContext* ctx)
{
    return ToggleButton::Init(ctx);
}

ECode CToggleButton::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs)
{
    return ToggleButton::Init(ctx, attrs);
}

ECode CToggleButton::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* atrs,
    /* [in] */ Int32 defStyle)
{
    return ToggleButton::Init(context, atrs, defStyle);
}

ECode CToggleButton::OnPreDraw(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = ToggleButton::OnPreDraw();

    return NOERROR;
}
