
#ifndef __CABSOLUTELAYOUT_H__
#define __CABSOLUTELAYOUT_H__

#include "_CAbsoluteLayout.h"
#include "widget/AbsoluteLayout.h"
#include "view/ViewMacro.h"

CarClass(CAbsoluteLayout), public AbsoluteLayout
{
public:
    IVIEW_METHODS_DECL();

    IVIEWGROUP_METHODS_DECL();

    IVIEWPARENT_METHODS_DECL();

    IVIEWMANAGER_METHODS_DECL();

    IDrawableCallback_METHODS_DECL();

    IKeyEventCallback_METHODS_DECL();

    IAccessibilityEventSource_METHODS_DECL();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext * pContext);

    CARAPI constructor(
        /* [in] */ IContext * pContext,
        /* [in] */ IAttributeSet * pAttrs);

    CARAPI constructor(
        /* [in] */ IContext * pContext,
        /* [in] */ IAttributeSet * pAttrs,
        /* [in] */ Int32 defStyle);

private:
    // TODO: Add your private member variables here.
};

#endif // __CABSOLUTELAYOUT_H__
