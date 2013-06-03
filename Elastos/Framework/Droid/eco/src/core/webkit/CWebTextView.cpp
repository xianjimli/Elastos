
#include "webkit/CWebTextView.h"
#include "../../../res/gen/R.h"
#include "widget/CAbsoluteLayoutLayoutParams.h"
#include "webkit/CWebView.h"

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

/**
 * Determine the position and size of WebTextView, and add it to the
 * WebView's view heirarchy.  All parameters are presumed to be in
 * view coordinates.  Also requests Focus and sets the cursor to not
 * request to be in view.
 * @param x         x-position of the textfield.
 * @param y         y-position of the textfield.
 * @param width     width of the textfield.
 * @param height    height of the textfield.
 */
/* package */
void CWebTextView::SetRect(
 /* [in] */ Int32 x,
 /* [in] */ Int32 y,
 /* [in] */ Int32 width,
 /* [in] */ Int32 height)
{    
    AutoPtr<IAbsoluteLayoutLayoutParams> lp = (IAbsoluteLayoutLayoutParams*)((WebTextView::GetLayoutParams()).Get());
    if (NULL == lp.Get()) {
        CAbsoluteLayoutLayoutParams::New(width, height, x, y,(IAbsoluteLayoutLayoutParams**)&lp);
    } 
    else {
        ((CAbsoluteLayoutLayoutParams*)lp.Get()) -> mX = x;
        ((CAbsoluteLayoutLayoutParams*)lp.Get()) -> mY = y;
        ((CAbsoluteLayoutLayoutParams*)lp.Get()) -> mWidth = width;
        ((CAbsoluteLayoutLayoutParams*)lp.Get()) -> mHeight = height;
    }
    
    if ( ((WebTextView::GetParent()).Get()) == NULL) {
        ( (CWebView*)(mWebView.Get()) ) -> AddViewEx3((IView*)this, (IViewGroupLayoutParams*)lp);
    } 
    else {
        WebTextView::SetLayoutParams((IViewGroupLayoutParams*)lp);
    }
    // Set up a measure spec so a layout can always be recreated.
    mWidthSpec = MeasureSpec::MakeMeasureSpec(width, MeasureSpec::EXACTLY);
    mHeightSpec = MeasureSpec::MakeMeasureSpec(height, MeasureSpec::EXACTLY);
}
