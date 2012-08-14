
#include "widget/CAbsoluteLayout.h"

IVIEW_METHODS_IMPL(CAbsoluteLayout, AbsoluteLayout, AbsoluteLayout);

IVIEWGROUP_METHODS_IMPL(CAbsoluteLayout, AbsoluteLayout, AbsoluteLayout);

IVIEWPARENT_METHODS_IMPL(CAbsoluteLayout, AbsoluteLayout, AbsoluteLayout);

IVIEWMANAGER_METHODS_IMPL(CAbsoluteLayout, AbsoluteLayout, AbsoluteLayout);

IDrawableCallback_METHODS_IMPL(CAbsoluteLayout, AbsoluteLayout, AbsoluteLayout);

IKeyEventCallback_METHODS_IMPL(CAbsoluteLayout, AbsoluteLayout, AbsoluteLayout);

IAccessibilityEventSource_METHODS_IMPL(CAbsoluteLayout, AbsoluteLayout, AbsoluteLayout);

CARAPI_(PInterface) CAbsoluteLayout::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CAbsoluteLayout::Probe(riid);
}

ECode CAbsoluteLayout::constructor(
    /* [in] */ IContext * pContext)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAbsoluteLayout::constructor(
    /* [in] */ IContext * pContext,
    /* [in] */ IAttributeSet * pAttrs)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAbsoluteLayout::constructor(
    /* [in] */ IContext * pContext,
    /* [in] */ IAttributeSet * pAttrs,
    /* [in] */ Int32 defStyle)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

