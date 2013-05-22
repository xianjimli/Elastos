
#include "app/CNativeContentView.h"

IVIEW_METHODS_IMPL(CNativeContentView, View, View);

IDrawableCallback_METHODS_IMPL(CNativeContentView, View, View);

IKeyEventCallback_METHODS_IMPL(CNativeContentView, View, View);

IAccessibilityEventSource_METHODS_IMPL(CNativeContentView, View, View);

ECode CNativeContentView::constructor(
    /* [in] */ IContext* ctx)
{
    return Init(ctx);
}

ECode CNativeContentView::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs)
{
    return Init(ctx, attrs);
}

PInterface CNativeContentView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CNativeContentView::Probe(riid);
}
