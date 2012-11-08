
#include "ext/frameworkext.h"
#include "view/menu/ContextMenuBuilder.h"


ContextMenuBuilder::ContextMenuBuilder(
    /* [in] */ IContext* context)
    : ContextMenuBuilderBase(context)
{}

PInterface ContextMenuBuilder::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IMenuBuilder) {
        return (IMenuBuilder*)this;
    }
    else if (riid == EIID_IContextMenu) {
        return (IContextMenu*)this;
    }
    else if (riid == EIID_IMenu) {
        return (IMenu*)(IContextMenu*)(this);
    }
    else if (EIID_MenuBuilderBase == riid) {
        return reinterpret_cast<PInterface>((MenuBuilderBase *)this);
    }

    return NULL;
}

UInt32 ContextMenuBuilder::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ContextMenuBuilder::Release()
{
    return ElRefBase::Release();
}

ECode ContextMenuBuilder::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IMenuBuilder*)this) {
        *pIID = EIID_IMenuBuilder;
    }
    else if (pObject == (IInterface*)(IContextMenu*)this) {
        *pIID = EIID_IContextMenu;
    }
    else if (pObject == (IInterface*)(IMenu*)(IContextMenu*)this) {
        *pIID = EIID_IMenu;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

IMENU_METHODS_IMPL(ContextMenuBuilder, ContextMenuBuilderBase, ContextMenuBuilderBase);
IMENUBUILDER_METHODS_IMPL(ContextMenuBuilder, ContextMenuBuilderBase, ContextMenuBuilderBase);

ECode ContextMenuBuilder::SetHeaderIconEx(
    /* [in] */ IDrawable* icon)
{
    return ContextMenuBuilderBase::SetHeaderIcon(icon);
}

ECode ContextMenuBuilder::SetHeaderIcon(
    /* [in] */ Int32 iconRes)
{
    return ContextMenuBuilderBase::SetHeaderIcon(iconRes);
}

ECode ContextMenuBuilder::SetHeaderTitleEx(
    /* [in] */ ICharSequence* title)
{
    return ContextMenuBuilderBase::SetHeaderTitle(title);
}

ECode ContextMenuBuilder::SetHeaderTitle(
    /* [in] */ Int32 titleRes)
{
    return ContextMenuBuilderBase::SetHeaderTitle(titleRes);
}

ECode ContextMenuBuilder::SetHeaderView(
    /* [in] */ IView* view)
{
    return ContextMenuBuilderBase::SetHeaderView(view);
}

ECode ContextMenuBuilder::ClearHeader()
{
    return ContextMenuBuilderBase::ClearHeader();
}
