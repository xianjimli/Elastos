
#include "view/menu/CSubMenuBuilder.h"

IMENUBUILDER_METHODS_IMPL(CSubMenuBuilder, MenuBuilder, NULL);

/**
 * The model for a sub menu, which is an extension of the menu.  Most methods are proxied to
 * the parent menu.
 */
CSubMenuBuilder::CSubMenuBuilder(
    /* [in] */ IContext* context,
    /* [in] */ IMenuBuilder* parentMenu,
    /* [in] */ IMenuItem* item): SubMenuBuilder(context, parentMenu, item) {
}

ECode CSubMenuBuilder::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IMenuBuilder* parentMenu,
    /* [in] */ IMenuItem* item){
    return SubMenuBuilder::constructor(context, parentMenu, item);
}

ECode CSubMenuBuilder::SetIconEx(
    /* [in] */ IDrawable* icon,
    /* [out] */ ISubMenu** submenu) {
    return SubMenuBuilder::SetIconEx(icon, submenu);
}

ECode CSubMenuBuilder::SetIcon(
    /* [in] */ Int32 iconRes,
    /* [out] */ ISubMenu** submenu) {
    return SubMenuBuilder::SetIcon(iconRes, submenu);
}

ECode CSubMenuBuilder::SetHeaderIconEx(
    /* [in] */ IDrawable* icon,
    /* [out] */ ISubMenu** submenu) {
    return SubMenuBuilder::SetHeaderIconEx(icon, submenu);
}

ECode CSubMenuBuilder::SetHeaderIcon(
    /* [in] */ Int32 iconRes,
    /* [out] */ ISubMenu** submenu) {
    return SubMenuBuilder::SetHeaderIcon(iconRes, submenu);
}

ECode CSubMenuBuilder::SetHeaderTitleEx(
    /* [in] */ ICharSequence* title,
    /* [out] */ ISubMenu** submenu) {
    return SubMenuBuilder::SetHeaderTitleEx(title, submenu);
}

ECode CSubMenuBuilder::SetHeaderTitle(
    /* [in] */ Int32 titleRes,
    /* [out] */ ISubMenu** submenu) {
    return SubMenuBuilder::SetHeaderTitle(titleRes, submenu);
}

ECode CSubMenuBuilder::SetHeaderView(
    /* [in] */ IView* view,
    /* [out] */ ISubMenu** submenu) {
    return SubMenuBuilder::SetHeaderView(view, submenu);
}

ECode CSubMenuBuilder::GetItemEx(
    /* [out] */ IMenuItem** menuItem) {
    return SubMenuBuilder::GetItemEx(menuItem);
}
