
#include "ext/frameworkext.h"
#include "view/menu/SubMenuBuilderBase.h"

/**
 * The model for a sub menu, which is an extension of the menu.  Most methods are proxied to
 * the parent menu.
 */
SubMenuBuilderBase::SubMenuBuilderBase(
        /* [in] */ IContext* context,
        /* [in] */ IMenuBuilder* parentMenu,
        /* [in] */ IMenuItemImpl* item)
    : MenuBuilderBase(context)
    , mParentMenu(parentMenu)
    , mItem(item)
{}

//@override
ECode SubMenuBuilderBase::SetQwertyMode(
        /* [in] */ Boolean isQwerty)
{
    return mParentMenu->SetQwertyMode(isQwerty);
}

//@override
Boolean SubMenuBuilderBase::IsQwertyMode()
{
    return ((MenuBuilderBase*)mParentMenu->Probe(EIID_MenuBuilderBase))->IsQwertyMode();
}

//@override
ECode SubMenuBuilderBase::SetShortcutsVisible(
    /* [in] */ Boolean shortcutsVisible)
{
    return mParentMenu->SetShortcutsVisible(shortcutsVisible);
}

//@override
Boolean SubMenuBuilderBase::IsShortcutsVisible()
{
    Boolean isVisible;

    mParentMenu->IsShortcutsVisible(&isVisible);
    return isVisible;
}

AutoPtr<IMenu> SubMenuBuilderBase::GetParentMenu()
{
    return IMenu::Probe(mParentMenu);
}

AutoPtr<IMenuItem> SubMenuBuilderBase::GetItem()
{
    return IMenuItem::Probe(mItem);
}

//@override
AutoPtr<IMenuBuilderCallback> SubMenuBuilderBase::GetCallback()
{
    AutoPtr<IMenuBuilderCallback> callback;
    mParentMenu->GetCallback((IMenuBuilderCallback**)&callback);
    return callback;
}

//@override
ECode SubMenuBuilderBase::SetCallback(
    /* [in] */ IMenuBuilderCallback* callback)
{
    return mParentMenu->SetCallback(callback);
}

//@override
AutoPtr<IMenuBuilder> SubMenuBuilderBase::GetRootMenu()
{
    return mParentMenu;
}

ECode SubMenuBuilderBase::SetIcon(
    /* [in] */ IDrawable* icon)
{
    return mItem->SetIcon(icon);
}

ECode SubMenuBuilderBase::SetIcon(
    /* [in] */ Int32 iconRes)
{
    return mItem->SetIconEx(iconRes);
}

ECode SubMenuBuilderBase::SetHeaderIcon(
    /* [in] */ IDrawable* icon)
{
    MenuBuilderBase::SetHeaderIconInt(icon);
    return NOERROR;
}

ECode SubMenuBuilderBase::SetHeaderIcon(
    /* [in] */ Int32 iconRes)
{
    MenuBuilderBase::SetHeaderIconInt(iconRes);
    return NOERROR;
}

ECode SubMenuBuilderBase::SetHeaderTitle(
    /* [in] */ ICharSequence* title)
{
    MenuBuilderBase::SetHeaderTitleInt(title);
    return NOERROR;
}

ECode SubMenuBuilderBase::SetHeaderTitle(
    /* [in] */ Int32 titleRes)
{
    MenuBuilderBase::SetHeaderTitleInt(titleRes);
    return NOERROR;
}

ECode SubMenuBuilderBase::SetHeaderView(
    /* [in] */ IView* view)
{
    MenuBuilderBase::SetHeaderViewInt(view);
    return NOERROR;
}
