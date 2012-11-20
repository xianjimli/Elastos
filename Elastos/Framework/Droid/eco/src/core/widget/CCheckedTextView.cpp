
#include "widget/CCheckedTextView.h"

IVIEW_METHODS_IMPL(CCheckedTextView, CheckedTextView, CheckedTextView);

IDrawableCallback_METHODS_IMPL(CCheckedTextView, CheckedTextView, CheckedTextView);

IKeyEventCallback_METHODS_IMPL(CCheckedTextView, CheckedTextView, CheckedTextView);

IAccessibilityEventSource_METHODS_IMPL(CCheckedTextView, CheckedTextView, CheckedTextView);

ITEXTVIEW_METHODS_IMPL(CCheckedTextView, CheckedTextView, CheckedTextView);

PInterface CCheckedTextView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CCheckedTextView::Probe(riid);
}

ECode CCheckedTextView::Toggle()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCheckedTextView::IsChecked(
    /* [out] */ Boolean * pChecked)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCheckedTextView::SetChecked(
    /* [in] */ Boolean checked)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCheckedTextView::SetCheckMarkDrawable(
    /* [in] */ Int32 resid)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCheckedTextView::SetCheckMarkDrawableEx(
    /* [in] */ IDrawable * pD)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCheckedTextView::OnPreDraw(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCheckedTextView::constructor(
    /* [in] */ IContext * pCtx)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCheckedTextView::constructor(
    /* [in] */ IContext * pCtx,
    /* [in] */ IAttributeSet * pAttrs)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCheckedTextView::constructor(
    /* [in] */ IContext * pCtx,
    /* [in] */ IAttributeSet * pAttrs,
    /* [in] */ Int32 defStyle)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

