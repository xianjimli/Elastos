
#ifndef __CDIALOGTITLE_H__
#define __CDIALOGTITLE_H__

#include "_CDialogTitle.h"
#include "view/ViewMacro.h"
#include "widget/TextViewMacro.h"
#include "widget/TextView.h"

/**
 * Used by dialogs to change the font size and number of lines to try to fit
 * the text to the available space.
 */
CarClass(CDialogTitle), public TextView
{
public:
    CDialogTitle();

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* ctx);

    CARAPI_(PInterface) Probe(
    /* [in] */ REIID riid);

    IVIEW_METHODS_DECL();

    IDrawableCallback_METHODS_DECL();

    IKeyEventCallback_METHODS_DECL();

    IAccessibilityEventSource_METHODS_DECL();

    ITEXTVIEW_METHODS_DECL();

    CARAPI OnPreDraw(
        /* [out] */ Boolean* result);

protected:
    //@Override
    CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);
};

#endif //__CDIALOGTITLE_H__
