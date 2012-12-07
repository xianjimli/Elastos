
#ifndef __PERMISSIONCOLLECTION_H__
#define __PERMISSIONCOLLECTION_H__

#ifdef _ELASTOSSECURITY_
#include "Elastos.Security_server.h"
#else
#include "Elastos.Security.h"
#endif

using namespace Elastos;

/**
 * {@code PermissionCollection} is the common base class for all collections
 * that provide a convenient method for determining whether or not a given
 * permission is implied by any of the permissions present in this collection.
 * <p>
 * A {@code PermissionCollection} is typically created by using the
 * {@link Permission#newPermissionCollection()} factory method. If the mentioned
 * method returns {@code null}, then a {@code PermissionCollection} of any type
 * can be used. If a collection is returned, it must be used for holding several
 * permissions of the particular type.
 * <p>
 * Subclasses must be implemented thread save.
 */
class PermissionCollection
{
public:
    ~PermissionCollection();

    virtual CARAPI Add(
        /* [in] */ IPermission* permission) = 0;

    virtual CARAPI GetElements(
        /* [out] */ IObjectContainer** permissions) = 0;

    virtual CARAPI Implies(
        /* [in] */ IPermission* permission,
        /* [out] */ Boolean* isImplied) = 0;

    virtual CARAPI IsReadOnly(
        /* [out] */ Boolean* isReadOnly);

    virtual CARAPI SetReadOnly();

protected:
    PermissionCollection();

private:
    static const Int64 sSerialVersionUID;

    Boolean mReadOnly;
};

#endif //__PERMISSIONCOLLECTION_H__
