
#ifndef __CSUBMENUBUILDER_H__
#define __CSUBMENUBUILDER_H__

#include "_CSubMenuBuilder.h"
#include "CMenuBuilder.h"
#include "SubMenuBuilder.h"
#include <elastos/AutoPtr.h>
#include "MenuBuilderMacro.h"

/**
 * The model for a sub menu, which is an extension of the menu.  Most methods are proxied to
 * the parent menu.
 */
CarClass(CSubMenuBuilder), public SubMenuBuilder {
public:
    CSubMenuBuilder(
        /* [in] */ IContext* context,
        /* [in] */ IMenuBuilder* parentMenu,
        /* [in] */ IMenuItem* item);

    CSubMenuBuilder(){}

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IMenuBuilder* parentMenu,
        /* [in] */ IMenuItem* item);

    virtual CARAPI SetHeaderIconEx(
        /* [in] */ IDrawable* icon,
        /* [out] */ ISubMenu** submenu);

    virtual CARAPI SetHeaderIcon(
        /* [in] */ Int32 iconRes,
        /* [out] */ ISubMenu** submenu);

    virtual CARAPI SetHeaderTitleEx(
        /* [in] */ ICharSequence* title,
        /* [out] */ ISubMenu** submenu);

    virtual CARAPI SetHeaderTitle(
        /* [in] */ Int32 titleRes,
        /* [out] */ ISubMenu** submenu);

    virtual CARAPI SetHeaderView(
        /* [in] */ IView* view,
        /* [out] */ ISubMenu** submenu);

    virtual CARAPI SetIconEx(
        /* [in] */ IDrawable* icon,
        /* [out] */ ISubMenu** submenu);

    virtual CARAPI SetIcon(
        /* [in] */ Int32 iconRes,
        /* [out] */ ISubMenu** submenu);

    virtual CARAPI GetItemEx(
        /* [out] */ IMenuItem** menuItem);

	IMENUBUILDER_METHODS_DECL();
};

#endif  //__CSUBMENUBUILDER_H__
