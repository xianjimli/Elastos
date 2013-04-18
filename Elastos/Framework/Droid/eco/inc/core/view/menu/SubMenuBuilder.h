
#ifndef __CSUBMENUBUILDER_H__
#define __CSUBMENUBUILDER_H__

//#include "_CSubMenuBuilder.h"
//#include "view/menu/CMenuBuilder.h"
#include "view/menu/SubMenuBuilderBase.h"
#include "view/menu/MenuBuilderMacro.h"
#include "view/MenuMacro.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>

/**
 * The model for a sub menu, which is an extension of the menu.  Most methods are proxied to
 * the parent menu.
 */
class SubMenuBuilder
    : public ElRefBase
    , public SubMenuBuilderBase
    , public ISubMenuBuilder
    , public ISubMenu
{
public:
    SubMenuBuilder(
        /* [in] */ IContext* context,
        /* [in] */ IMenuBuilder* parentMenu,
        /* [in] */ IMenuItemImpl* item);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    IMENU_METHODS_DECL();
    IMENUBUILDER_METHODS_DECL();

    CARAPI GetItemEx(
        /* [out] */ IMenuItem** menuItem);

    CARAPI SetIconEx(
        /* [in] */ IDrawable* icon);

    CARAPI SetIcon(
        /* [in] */ Int32 iconRes);

    CARAPI SetHeaderIconEx(
        /* [in] */ IDrawable* icon);

    CARAPI SetHeaderIcon(
        /* [in] */ Int32 iconRes);

    CARAPI SetHeaderTitleEx(
        /* [in] */ ICharSequence* title);

    CARAPI SetHeaderTitle(
        /* [in] */ Int32 titleRes);

    CARAPI SetHeaderView(
        /* [in] */ IView* view);

    CARAPI GetParentMenu(
        /* [out] */ IMenu** menu);
};

#endif  //__CSUBMENUBUILDER_H__
