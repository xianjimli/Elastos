
#ifndef __CBUTTON_H__
#define __CBUTTON_H__

#include "_CButton.h"
#include "widget/Button.h"
#include "view/ViewMacro.h"
#include "widget/TextViewMacro.h"

CarClass(CButton), public Button
{
public:
    IVIEW_METHODS_DECL();

    ITEXTVIEW_METHODS_DECL();

    IDrawableCallback_METHODS_DECL();

    IKeyEventCallback_METHODS_DECL();

    IAccessibilityEventSource_METHODS_DECL();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

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

#endif // __CBUTTON_H__
