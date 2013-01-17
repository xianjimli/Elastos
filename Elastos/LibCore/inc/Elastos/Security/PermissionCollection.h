
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
    /**
     * Adds the specified {@code Permission} to this collection.
     *
     * @param permission
     *            the {@code Permission} to add.
     * @throws IllegalStateException
     *             if the collection is read only.
     */
    virtual CARAPI Add(
        /* [in] */ IPermission* permission) = 0;

    /**
     * Returns an enumeration over all {@link Permission}s encapsulated by this
     * {@code PermissionCollection}.
     *
     * @return an enumeration over all {@link Permission}s.
     */
    virtual CARAPI GetElements(
        /* [out] */ IObjectContainer** permissions) = 0;

    /**
     * Indicates whether the specified permission is implied by this {@code
     * PermissionCollection}.
     *
     * @param permission
     *            the permission to check.
     * @return {@code true} if the given permission is implied by the
     *         permissions in this collection, {@code false} otherwise.
     */
    virtual CARAPI Implies(
        /* [in] */ IPermission* permission,
        /* [out] */ Boolean* isImplied) = 0;

    /**
     * Indicates whether new permissions can be added to this {@code
     * PermissionCollection}.
     *
     * @return {@code true} if the receiver is read only, {@code false} if new
     *         elements can still be added to this {@code PermissionCollection}.
     */
    virtual CARAPI IsReadOnly(
        /* [out] */ Boolean* isReadOnly);

    /**
     * Marks this {@code PermissionCollection} as read only, so that no new
     * permissions can be added to it.
     */
    virtual CARAPI SetReadOnly();

protected:
    PermissionCollection();

private:
    Boolean mReadOnly;
};

#endif //__PERMISSIONCOLLECTION_H__
