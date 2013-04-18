
#ifndef __SUBMENUBUILDERBASE_H__
#define __SUBMENUBUILDERBASE_H__

#include "ext/frameworkext.h"
#include "view/menu/MenuBuilderBase.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>

/**
 * The model for a sub menu, which is an extension of the menu.  Most methods are proxied to
 * the parent menu.
 */
class SubMenuBuilderBase
    : public MenuBuilderBase
{
public:
    using MenuBuilderBase::GetItem;

    SubMenuBuilderBase(
        /* [in] */ IContext* context,
        /* [in] */ IMenuBuilder* parentMenu,
        /* [in] */ IMenuItemImpl* item);

    //@override
    CARAPI SetQwertyMode(
        /* [in] */ Boolean isQwerty);

    //@override
    CARAPI_(Boolean) IsQwertyMode();

    //@override
    CARAPI SetShortcutsVisible(
        /* [in] */ Boolean shortcutsVisible);

    //@override
    CARAPI_(Boolean) IsShortcutsVisible();

    virtual CARAPI_(AutoPtr<IMenu>) GetParentMenu();

    virtual CARAPI_(AutoPtr<IMenuItem>) GetItem();

    //@override
    CARAPI_(AutoPtr<IMenuBuilderCallback>) GetCallback();

    //@override
    CARAPI SetCallback(
        /* [in] */ IMenuBuilderCallback* callback);

    //@override
    CARAPI_(AutoPtr<IMenuBuilder>) GetRootMenu();

    CARAPI SetIcon(
        /* [in] */ IDrawable* icon);

    CARAPI SetIcon(
        /* [in] */ Int32 iconRes);

    CARAPI SetHeaderIcon(
        /* [in] */ IDrawable* icon);

    CARAPI SetHeaderIcon(
        /* [in] */ Int32 iconRes);

    CARAPI SetHeaderTitle(
        /* [in] */ ICharSequence* title);

    CARAPI SetHeaderTitle(
        /* [in] */ Int32 titleRes);

    CARAPI SetHeaderView(
        /* [in] */ IView* view);

private:
    AutoPtr<IMenuBuilder> mParentMenu;
    AutoPtr<IMenuItem> mItem;
};

#endif  //__SUBMENUBUILDERBASE_H__
