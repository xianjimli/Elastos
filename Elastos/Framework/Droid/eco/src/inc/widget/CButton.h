
#ifndef __CBUTTON_H__
#define __CBUTTON_H__

#include "_CButton.h"
#include "widget/Button.h"
#include "view/ViewMacro.h"
#include "widget/TextViewMacro.h"

CarClass(CButton), public Button
{
public:
    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    IVIEW_METHODS_DECL();

    ITEXTVIEW_METHODS_DECL();

    CARAPI constructor(
        /* [in] */ IContext * pCtx);

    CARAPI constructor(
        /* [in] */ IContext * pCtx,
        /* [in] */ IAttributeSet * pAttrs);

    CARAPI constructor(
        /* [in] */ IContext * pContext,
        /* [in] */ IAttributeSet * pAttrs,
        /* [in] */ Int32 defStyle);

private:
    // TODO: Add your private member variables here.
};

#endif // __CBUTTON_H__
