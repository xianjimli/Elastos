
#include "view/menu/ContextMenuBuilder.h"
#include "view/menu/CMenuDialogHelper.h"

IMENUBUILDER_METHODS_IMPL(ContextMenuBuilder, MenuBuilder, NULL);

PInterface ContextMenuBuilder::Probe(
    /* [in]  */ REIID riid) {
    if (riid == EIID_IContextMenu) {
        return (IContextMenu*)this;
    }
    else if (riid == EIID_IMenu) {
        return (IMenu*)(this);
    }

    return NULL;
}

UInt32 ContextMenuBuilder::AddRef() {
    return ElRefBase::AddRef();
}

UInt32 ContextMenuBuilder::Release() {
    return ElRefBase::Release();
}

ECode ContextMenuBuilder::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID) {
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IMenu*)MenuBuilder::Probe(EIID_IMenu)) {
        *pIID = EIID_IMenu;
    }
    else if (pObject == (IInterface*)(IContextMenu*)this) {
        *pIID = EIID_IContextMenu;
    }

    return E_INVALID_ARGUMENT;
}

ECode ContextMenuBuilder::SetHeaderTitle(
    /* [in] */ Int32 titleRes,
    /* [out] */ IContextMenu** contextMenu) {
    return SetHeaderTitleIntEx(titleRes, (IMenuBuilder**) contextMenu);
}

ECode ContextMenuBuilder::SetHeaderTitleEx(
    /* [in] */ ICharSequence* title,
    /* [out] */ IContextMenu** contextMenu) {
    return SetHeaderTitleInt(title, (IMenuBuilder**) contextMenu);
}

ECode ContextMenuBuilder::SetHeaderIcon(
    /* [in] */ Int32 iconRes,
    /* [out] */ IContextMenu** contextMenu) {
    return SetHeaderIconIntEx(iconRes, (IMenuBuilder**) contextMenu);
}

ECode ContextMenuBuilder::SetHeaderIconEx(
    /* [in] */ IDrawable* icon,
    /* [out] */ IContextMenu** contextMenu) {
    return SetHeaderIconInt(icon, (IMenuBuilder**) contextMenu);
}

ECode ContextMenuBuilder::SetHeaderView(
    /* [in] */ IView* view,
    /* [out] */ IContextMenu** contextMenu) {
    return SetHeaderViewInt(view, (IMenuBuilder**) contextMenu);
}

ECode ContextMenuBuilder::Show(
    /* [in] */ IView* originalView,
    /* [in] */ IBinder* token,
    /* [out] */ CMenuDialogHelper** helper) {
    *helper = NULL;

    if (originalView != NULL) {
        originalView->CreateContextMenu(this);
    }

    AutoPtr<IObjectContainer> list;
    CObjectContainer::New((IObjectContainer**)&list);
    GetVisibleItems((IObjectContainer**) &list);

    Int32 objCount;
    list->GetObjectCount(&objCount);

    if (objCount > 0) {
        //EventLog.writeEvent(50001. 1);
        *helper = new CMenuDialogHelper((IMenuBuilder*)(this->Probe(EIID_IMenu)));
    }

    return NOERROR;
}
