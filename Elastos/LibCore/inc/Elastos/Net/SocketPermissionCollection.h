
#ifndef __SOCKETPERMISSIONCOLLECTION_H__
#define __SOCKETPERMISSIONCOLLECTION_H__

#include "Elastos.Net_server.h"
#include "PermissionCollection.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>
#include <elastos/Vector.h>

using namespace Elastos;

/**
 * This class represents a list of {@code SocketPermission} objects and provides
 * a method to check whether or not a specific permission is implied by this
 * {@code SocketPermissionCollection}.
 */
class SocketPermissionCollection
    : public IPermissionCollection
    , public ElRefBase
    , public PermissionCollection
{
public:
    // Constructs a new instance of this class.
    SocketPermissionCollection();

    CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    // Adds the argument to the collection.
    // @Override
    CARAPI Add(
        /* [in] */ IPermission* permission);

    // Returns an enumeration of the permissions
    // @Override
    CARAPI GetElements(
        /* [out] */ IObjectContainer** permissions);

    /**
     * Returns whether this permission collection implies {@code permission}.
     * Basically it tests whether {@code permission} is the subset of this
     * collection.
     */
    // @Override
    CARAPI Implies(
        /* [in] */ IPermission* permission,
        /* [out] */ Boolean* isImplied);

    CARAPI IsReadOnly(
        /* [out] */ Boolean* isReadOnly);

    CARAPI SetReadOnly();

private:
    Vector< AutoPtr<IPermission> > mPermissions;
};

#endif //__SOCKETPERMISSIONCOLLECTION_H__
