
#include "widget/CTextView.h"

IVIEW_METHODS_IMPL(CTextView, TextView, TextView);

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
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
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
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

