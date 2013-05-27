
#include "webkit/CWebTextView.h"
#include "../../../res/gen/R.h"

IVIEW_METHODS_IMPL(CWebTextView, WebTextView, WebTextView);

IDrawableCallback_METHODS_IMPL(CWebTextView, WebTextView, WebTextView);

IKeyEventCallback_METHODS_IMPL(CWebTextView, WebTextView, WebTextView);

IAccessibilityEventSource_METHODS_IMPL(CWebTextView, WebTextView, WebTextView);

ITEXTVIEW_METHODS_IMPL(CWebTextView, WebTextView, WebTextView);

IAUTOCOMPLETETEXTVIEW_METHODS_IMPL(CWebTextView, WebTextView, WebTextView);

CARAPI_(PInterface) CWebTextView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CWebTextView::Probe(riid);
}

ECode CWebTextView::OnDrawSubstitute()
{
    return WebTextView::OnDrawSubstitute();
}

ECode CWebTextView::SetAdapterCustom(
    /* [in] */ IAutoCompleteAdapter * pAdapter)
{
    WebTextView::SetAdapterCustom((AutoCompleteAdapter*)pAdapter);
    return NOERROR;
}

ECode CWebTextView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IWebView* webView)
{
	WebTextView::Init(context,webView);
    return NOERROR;	
}
