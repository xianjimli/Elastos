
#include "ext/frameworkext.h"
#include "view/menu/SubMenuBuilder.h"



SubMenuBuilder::SubMenuBuilder(
    /* [in] */ IContext* context,
    /* [in] */ IMenuBuilder* parentMenu,
    /* [in] */ IMenuItemImpl* item)
    : SubMenuBuilderBase(context, parentMenu, item)
{}

PInterface SubMenuBuilder::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_ISubMenuBuilder == riid) {
        return (ISubMenuBuilder *)this;
    }
    else if (EIID_IMenuBuilder == riid) {
        return (IMenuBuilder *)this;
    }
    else if (EIID_ISubMenu == riid) {
        return (ISubMenu *)this;
    }
    else if (EIID_IMenu == riid) {
        return (IMenu *)(ISubMenu *)this;
    }
    else if (EIID_MenuBuilderBase == riid) {
        return reinterpret_cast<PInterface>((MenuBuilderBase *)this);
    }

    return NULL;
}

UInt32 SubMenuBuilder::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 SubMenuBuilder::Release()
{
    return ElRefBase::Release();
}

ECode SubMenuBuilder::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(ISubMenuBuilder*)this) {
        *pIID = EIID_ISubMenuBuilder;
    }
    else if (pObject == (IInterface*)(IMenuBuilder*)this) {
        *pIID = EIID_IMenuBuilder;
    }
    else if (pObject == (IInterface*)(ISubMenu*)this) {
        *pIID = EIID_ISubMenu;
    }
    if (pObject == (IInterface*)(IMenu*)(ISubMenu*)this) {
        *pIID = EIID_IMenu;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

IMENU_METHODS_IMPL(SubMenuBuilder, SubMenuBuilderBase, SubMenuBuilderBase);
IMENUBUILDER_METHODS_IMPL(SubMenuBuilder, SubMenuBuilderBase, SubMenuBuilderBase);

ECode SubMenuBuilder::SetIconEx(
    /* [in] */ IDrawable* icon)
{
    SubMenuBuilderBase::SetIcon(icon);
    return NOERROR;
}

ECode SubMenuBuilder::SetIcon(
    /* [in] */ Int32 iconRes)
{
    SubMenuBuilderBase::SetIcon(iconRes);
    return NOERROR;
}

ECode SubMenuBuilder::SetHeaderIconEx(
    /* [in] */ IDrawable* icon)
{
    SubMenuBuilderBase::SetHeaderIcon(icon);
    return NOERROR;
}

ECode SubMenuBuilder::SetHeaderIcon(
    /* [in] */ Int32 iconRes)
{
    SubMenuBuilderBase::SetHeaderIcon(iconRes);
    return NOERROR;
}

ECode SubMenuBuilder::SetHeaderTitleEx(
    /* [in] */ ICharSequence* title)
{
    SubMenuBuilderBase::SetHeaderTitle(title);
    return NOERROR;
}

ECode SubMenuBuilder::SetHeaderTitle(
    /* [in] */ Int32 titleRes)
{
    SubMenuBuilderBase::SetHeaderTitle(titleRes);
    return NOERROR;
}

ECode SubMenuBuilder::SetHeaderView(
    /* [in] */ IView* view)
{
    SubMenuBuilderBase::SetHeaderView(view);
    return NOERROR;
}

ECode SubMenuBuilder::GetItemEx(
    /* [out] */ IMenuItem** menuItem)
{
    VALIDATE_NOT_NULL(menuItem);

    AutoPtr<IMenuItem> _menuItem = SubMenuBuilderBase::GetItem();
    *menuItem = _menuItem.Get();
    if (*menuItem != NULL) (*menuItem)->AddRef();
    return NOERROR;
}

ECode SubMenuBuilder::GetParentMenu(
    /* [out] */ IMenu** menu)
{
    VALIDATE_NOT_NULL(menu);

    AutoPtr<IMenu> _menu = SubMenuBuilderBase::GetParentMenu();
    *menu = _menu.Get();
    if (*menu != NULL) (*menu)->AddRef();
    return NOERROR;
}
