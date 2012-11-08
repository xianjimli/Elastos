
#include "view/menu/MenuBuilder.h"
#include "view/menu/MenuBuilderMacro.h"


MenuBuilder::MenuBuilder(
    /*[in]*/ IContext* context)
    : MenuBuilderBase(context)
{}

PInterface MenuBuilder::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IMenu == riid) {
        return (IMenu *)this;
    }
    else if (EIID_IMenuBuilder == riid) {
        return (IMenuBuilder *)this;
    }
    else if (EIID_MenuBuilderBase == riid) {
        return reinterpret_cast<PInterface>((MenuBuilderBase *)this);
    }

    return NULL;
}

UInt32 MenuBuilder::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 MenuBuilder::Release()
{
    return ElRefBase::Release();
}

ECode MenuBuilder::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IMenu*)this) {
        *pIID = EIID_IMenu;
    }
    else if (pObject == (IInterface*)(IMenuBuilder*)this) {
        *pIID = EIID_IMenuBuilder;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

IMENU_METHODS_IMPL(MenuBuilder, MenuBuilderBase, NULL);
IMENUBUILDER_METHODS_IMPL(MenuBuilder, MenuBuilderBase, NULL);
