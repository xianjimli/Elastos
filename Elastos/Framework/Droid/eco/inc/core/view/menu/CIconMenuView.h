
#ifndef __CICONMENUVIEW_H__
#define __CICONMENUVIEW_H__

#include "_CIconMenuView.h"
#include "view/menu/IconMenuView.h"


CarClass(CIconMenuView), public IconMenuView
{
public:
    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    IVIEW_METHODS_DECL();
    IVIEWGROUP_METHODS_DECL();
    IVIEWPARENT_METHODS_DECL();
    IVIEWMANAGER_METHODS_DECL();
    IDrawableCallback_METHODS_DECL();
    IKeyEventCallback_METHODS_DECL();
    IAccessibilityEventSource_METHODS_DECL();

    CARAPI GetLayout(
        /* [out, callee] */ ArrayOf<Int32>** layout);

    CARAPI GetLayoutNumRows(
        /* [out] */ Int32* num);

    CARAPI Initialize(
        /* [in] */ IMenuBuilder* menu,
        /* [in] */ Int32 menuType);

    CARAPI UpdateChildren(
        /* [in] */ Boolean cleared);

    CARAPI GetWindowAnimations(
        /* [out] */ Int32* animations);

    CARAPI InvokeItem(
        /* [in] */ IMenuItemImpl* item,
        /* [out] */ Boolean* state);

    CARAPI Run();
};

#endif  // __CICONMENUVIEW_H__
