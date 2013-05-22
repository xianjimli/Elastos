
#ifndef __CNATIVECONTENTVIEW_H__
#define __CNATIVECONTENTVIEW_H__

#include "_CNativeContentView.h"
#include "view/View.h"
#include "view/ViewMacro.h"

CarClass(CNativeContentView), public View
{
public:
    IVIEW_METHODS_DECL();

    IDrawableCallback_METHODS_DECL();

    IKeyEventCallback_METHODS_DECL();

    IAccessibilityEventSource_METHODS_DECL();

    CARAPI constructor(
        /* [in] */ IContext* ctx);

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);
};
#endif //__CNATIVECONTENTVIEW_H__
