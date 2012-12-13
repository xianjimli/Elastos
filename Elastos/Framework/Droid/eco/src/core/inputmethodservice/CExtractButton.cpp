
#include "inputmethodservice/CExtractButton.h"

IVIEW_METHODS_IMPL(CExtractButton, _ExtractButton, _ExtractButton);
IDrawableCallback_METHODS_IMPL(CExtractButton, _ExtractButton, _ExtractButton);
IKeyEventCallback_METHODS_IMPL(CExtractButton, _ExtractButton, _ExtractButton);
IAccessibilityEventSource_METHODS_IMPL(CExtractButton, _ExtractButton, _ExtractButton);
ITEXTVIEW_METHODS_IMPL(CExtractButton, _ExtractButton, _ExtractButton);


_ExtractButton::_ExtractButton()
{}

_ExtractButton::~_ExtractButton()
{}

Boolean _ExtractButton::HasWindowFocus()
{
	return IsEnabled();
}

CExtractButton::CExtractButton()
{}

PInterface CExtractButton::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }

    return _CExtractButton::Probe(riid);
}

ECode CExtractButton::constructor(
    /* [in] */ IContext* context)
{
    return Button::Init(context, NULL);
}

ECode CExtractButton::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return Button::Init(context, attrs, 0x01010048 /*com.android.internal.R.attr.buttonStyle*/);
}

ECode CExtractButton::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return Button::Init(context, attrs, defStyle);
}

ECode CExtractButton::OnPreDraw(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = Button::OnPreDraw();

    return NOERROR;
}
