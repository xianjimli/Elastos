
#ifndef __CICONMENIITEMVIEW_H__
#define __CICONMENIITEMVIEW_H__


#include "_CIconMenuItemView.h"
#include "view/menu/IconMenuItemView.h"
#include "view/ViewMacro.h"
#include "widget/TextViewMacro.h"


CarClass(CIconMenuItemView), public IconMenuItemView
{
public:
    using IconMenuItemView::Initialize;

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    IVIEW_METHODS_DECL();
    ITEXTVIEW_METHODS_DECL();
    IDrawableCallback_METHODS_DECL();
    IKeyEventCallback_METHODS_DECL();
    IAccessibilityEventSource_METHODS_DECL();

    CARAPI OnPreDraw(
        /* [out] */ Boolean* result);

    CARAPI SetItemInvoker(
        /* [in] */ IMenuBuilderItemInvoker* itemInvoker);

    //from IMenuItemView
    CARAPI Initialize(
        /* [in] */ IMenuItemImpl* itemData,
        /* [in] */ Int32 menuType);

    CARAPI GetItemData(
        /* [out] */ IMenuItemImpl** itemData);

    CARAPI SetTitle(
        /* [in] */ ICharSequence* title);

    CARAPI SetEnabledEx(
        /* [in] */ Boolean enabled);

    CARAPI SetCheckable(
        /* [in] */ Boolean checkable);

    CARAPI SetChecked(
        /* [in] */ Boolean checked);

    CARAPI SetShortcut(
        /* [in] */ Boolean showShortcut,
        /* [in] */ Char32 shortcutKey);

    CARAPI SetIcon(
        /* [in] */ IDrawable* icon);

    CARAPI PrefersCondensedTitle(
        /* [out] */ Boolean* supported);

    CARAPI ShowsIcon(
        /* [out] */ Boolean* show);
};

#endif //__CICONMENIITEMVIEW_H__
