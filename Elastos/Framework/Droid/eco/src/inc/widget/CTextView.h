
#ifndef __CTEXTVIEW_H__
#define __CTEXTVIEW_H__

#include "_CTextView.h"
#include "widget/TextView.h"
#include "view/ViewMacro.h"
#include "widget/TextViewMacro.h"

CarClass(CTextView), public TextView
{
public:
    IVIEW_METHODS_DECL();

    IDrawableCallback_METHODS_DECL();

    IKeyEventCallback_METHODS_DECL();

    IAccessibilityEventSource_METHODS_DECL();

    ITEXTVIEW_METHODS_DECL();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* Context,
        /* [in] */ IAttributeSet* Attrs);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI OnPreDraw(
        /* [out] */ Boolean* result);

private:
    // TODO: Add your private member variables here.
};

#endif // __CTEXTVIEW_H__
