
#include "cmdef.h"
#include "SocketPermissionCollection.h"
#include <Com.Kortide.Platform.h>

const Int64 SocketPermissionCollection::sSerialVersionUID = -2787186408602843674L;

SocketPermissionCollection::SocketPermissionCollection()
	: PermissionCollection()
{}

PInterface SocketPermissionCollection::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IPermissionCollection) {
        return (IPermissionCollection*)this;
    }

    return NULL;
}

UInt32 SocketPermissionCollection::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 SocketPermissionCollection::Release()
{
    return ElRefBase::Release();
}

ECode SocketPermissionCollection::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode SocketPermissionCollection::Add(
    /* [in] */ IPermission* permission)
{
	Boolean isReadOnly;
    IsReadOnly(&isReadOnly);
    if (isReadOnly) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalStateException();
    }
    if (permission->Probe(EIID_ISocketPermission) == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mPermissions.PushBack(permission);

    return NOERROR;
}

ECode SocketPermissionCollection::GetElements(
    /* [out] */ IObjectContainer** permissions)
{
	VALIDATE_NOT_NULL(permissions);

    AutoPtr<IObjectContainer> container;
    ASSERT_SUCCEEDED(CObjectContainer::New((IObjectContainer**)&container));
    Vector<AutoPtr<IPermission> >::Iterator it;
    for (it = mPermissions.Begin(); it != mPermissions.End(); ++it) {
        container->Add(*it);
    }

    *permissions = container;
    return NOERROR;
}

ECode SocketPermissionCollection::Implies(
    /* [in] */ IPermission* permission,
    /* [out] */ Boolean* isImplied)
{
	VALIDATE_NOT_NULL(isImplied);

    if (permission->Probe(EIID_ISocketPermission) == NULL) {
        *isImplied = FALSE;
        return NOERROR;
    }
    // AutoPtr<ISocketPermission> sp, argPerm =
    //         (ISocketPermission*)permission->Probe(EIID_ISocketPermission);
    // Int32 pmask = ((CSocketPermission*)argPerm->Probe(EIID_CSocketPermission))->mActionsMask;
    // Int32 allMask = 0;
    //Int32 i = 0, count = mPermissions.GetSize();
    // while ((i < count) && ((allMask & pmask) != pmask)) {
    //     sp = (ISocketPermission*)mPermissions[i]->Probe(EIID_ISocketPermission);
    //     AutoPtr<CSocketPermission> spCls =
    //             (CSocketPermission*)sp->Probe(EIID_CSocketPermission);
    //     AutoPtr<CSocketPermission> argPermCls =
    //             (CSocketPermission*)argPerm->Probe(EIID_CSocketPermission);
    //     if (spCls->CheckHost(argPerm)) {
    //         if ((spCls->mActionsMask & SocketPermission_SP_RESOLVE) == SocketPermission_SP_RESOLVE) {
    //             allMask |= SocketPermission_SP_RESOLVE;
    //         }
    //         // Only set flags if the port range and host can be implied
    //         if ((argPermCls->mPortMin >= spCls->mPortMin)
    //                 && (argPermCls->mPortMax <= spCls->mPortMax)) {
    //             if ((spCls->mActionsMask & SocketPermission_SP_CONNECT) == SocketPermission_SP_CONNECT) {
    //                 allMask |= SocketPermission.SP_CONNECT;
    //             }
    //             if ((spCls->mActionsMask & SocketPermission_SP_ACCEPT) == SocketPermission_SP_ACCEPT) {
    //                 allMask |= SocketPermission_SP_ACCEPT;
    //             }
    //             if ((spCls->mActionsMask & SocketPermission_SP_LISTEN) == SocketPermission_SP_LISTEN) {
    //                 allMask |= SocketPermission_SP_LISTEN;
    //             }
    //         }
    //     }
    //     ++i;
    // }

    //*isImplied = (allMask & pmask) == pmask;

    return E_NOT_IMPLEMENTED;
}

ECode SocketPermissionCollection::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
	return PermissionCollection::IsReadOnly(isReadOnly);
}

ECode SocketPermissionCollection::SetReadOnly()
{
	return PermissionCollection::SetReadOnly();
}
