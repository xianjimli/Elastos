
#ifndef __CCHECKBOX_H__
#define __CCHECKBOX_H__

#include "_CCheckBox.h"
#include "widget/CheckBox.h"
#include "view/ViewMacro.h"
#include "widget/TextViewMacro.h"

CarClass(CCheckBox), public CheckBox
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

    CARAPI SetOnCheckedChangeListener(
        /* [in] */ IOnCheckedChangeListener * pListener);

    CARAPI SetButtonDrawable(
        /* [in] */ Int32 resid);

    CARAPI SetButtonDrawableEx(
        /* [in] */ IDrawable * pD);

    CARAPI constructor(
        /* [in] */ IContext * pCtx);

    CARAPI constructor(
        /* [in] */ IContext * pCtx,
        /* [in] */ IAttributeSet * pAttrs);

    CARAPI constructor(
        /* [in] */ IContext * pContext,
        /* [in] */ IAttributeSet * pAttrs,
        /* [in] */ Int32 defStyle);

    CARAPI OnPreDraw(
        /* [out] */ Boolean* result);

private:
    // TODO: Add your private member variables here.
};

#endif // __CCHECKBOX_H__
