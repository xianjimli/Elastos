
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
    /* [in] */ IContext* context)
{
    return Init(context);
}

ECode CAbsoluteLayout::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return Init(context, attrs);
}

ECode CAbsoluteLayout::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return Init(context, attrs, defStyle);
}

