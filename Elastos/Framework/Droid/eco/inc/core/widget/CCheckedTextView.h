
#ifndef __CCHECKEDTEXTVIEW_H__
#define __CCHECKEDTEXTVIEW_H__

#include "_CCheckedTextView.h"
#include "widget/CheckedTextView.h"
#include "view/ViewMacro.h"
#include "widget/TextViewMacro.h"

CarClass(CCheckedTextView), public CheckedTextView
{
public:
    IVIEW_METHODS_DECL();

    IDrawableCallback_METHODS_DECL();

    IKeyEventCallback_METHODS_DECL();

    IAccessibilityEventSource_METHODS_DECL();

    ITEXTVIEW_METHODS_DECL();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI Toggle();

    CARAPI IsChecked(
        /* [out] */ Boolean * pChecked);

    CARAPI SetChecked(
        /* [in] */ Boolean checked);

    CARAPI SetCheckMarkDrawable(
        /* [in] */ Int32 resid);

    CARAPI SetCheckMarkDrawableEx(
        /* [in] */ IDrawable * pD);

    CARAPI OnPreDraw(
        /* [out] */ Boolean * pResult);

    CARAPI constructor(
        /* [in] */ IContext * pCtx);

    CARAPI constructor(
        /* [in] */ IContext * pCtx,
        /* [in] */ IAttributeSet * pAttrs);

    CARAPI constructor(
        /* [in] */ IContext * pCtx,
        /* [in] */ IAttributeSet * pAttrs,
        /* [in] */ Int32 defStyle);

private:
    // TODO: Add your private member variables here.
};

#endif // __CCHECKEDTEXTVIEW_H__
