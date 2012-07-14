
#include "widget/CButton.h"

IVIEW_METHODS_IMPL(CButton, Button, Button);

IDrawableCallback_METHODS_IMPL(CButton, Button, Button);

IKeyEventCallback_METHODS_IMPL(CButton, Button, Button);

IAccessibilityEventSource_METHODS_IMPL(CButton, Button, Button);

ITEXTVIEW_METHODS_IMPL(CButton, Button, Button);

PInterface CButton::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CButton::Probe(riid);
}

ECode CButton::constructor(
    /* [in] */ IContext * pCtx)
{
    return Init(pCtx);
}

ECode CButton::constructor(
    /* [in] */ IContext * pCtx,
    /* [in] */ IAttributeSet * pAttrs)
{
    return Init(pCtx, pAttrs);
}

ECode CButton::constructor(
    /* [in] */ IContext * pContext,
    /* [in] */ IAttributeSet * pAttrs,
    /* [in] */ Int32 defStyle)
{
    return Init(pContext, pAttrs, defStyle);
}

ECode CButton::OnPreDraw(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = Button::OnPreDraw();

    return NOERROR;
}
