
#include "widget/CTextView.h"

IVIEW_METHODS_IMPL(CTextView, TextView, TextView);

IDrawableCallback_METHODS_IMPL(CTextView, TextView, TextView);

IKeyEventCallback_METHODS_IMPL(CTextView, TextView, TextView);

IAccessibilityEventSource_METHODS_IMPL(CTextView, TextView, TextView);

ITEXTVIEW_METHODS_IMPL(CTextView, TextView, TextView);

PInterface CTextView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CTextView::Probe(riid);
}

ECode CTextView::constructor(
    /* [in] */ IContext* contxt)
{
    return TextView::Init(contxt);
}

ECode CTextView::constructor(
    /* [in] */ IContext* contxt,
    /* [in] */ IAttributeSet* attrs)
{
    return TextView::Init(contxt, attrs);
}

ECode CTextView::constructor(
    /* [in] */ IContext* contxt,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return TextView::Init(contxt, attrs, defStyle);
}

ECode CTextView::OnPreDraw(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = TextView::OnPreDraw();

    return NOERROR;
}
