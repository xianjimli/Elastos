
#ifndef __CWEBTEXTVIEW_H__
#define __CWEBTEXTVIEW_H__

#include "_CWebTextView.h"
#include "webkit/WebTextView.h"
#include "view/ViewMacro.h"
#include "widget/TextViewMacro.h"
#include "webkit/AutoCompleteTextViewMacro.h"

CarClass(CWebTextView), public WebTextView
{
public:
	
    IVIEW_METHODS_DECL();

    IDrawableCallback_METHODS_DECL();

    IKeyEventCallback_METHODS_DECL();

    IAccessibilityEventSource_METHODS_DECL();

    ITEXTVIEW_METHODS_DECL();

    IAUTOCOMPLETETEXTVIEW_METHODS_DECL();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);    

    /**
     * Create a new WebTextView.
     * @param   context The Context for this WebTextView.
     * @param   webView The WebView that created this.
     */
    /* package */
    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IWebView* webView);

    CARAPI OnDrawSubstitute();

    CARAPI SetAdapterCustom(
        /* [in] */ IAutoCompleteAdapter * pAdapter);

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
    virtual CARAPI_(void) SetRect(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

private:
    // TODO: Add your private member variables here.
};

#endif // __CWEBTEXTVIEW_H__
