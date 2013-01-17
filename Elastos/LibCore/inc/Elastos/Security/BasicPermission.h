
#ifndef __BASICPERMISSION_H__
#define __BASICPERMISSION_H__

#ifdef ELASTOS_SECURITY_ECO
#include "Elastos.Security_server.h"
#else
#include "Elastos.Security.h"
#endif
#include "Permission.h"

using namespace Elastos;

/**
 * {@code BasicPermission} is the common base class for all collections
 * that provide a convenient method for determining whether or not a given
 * permission is implied by any of the permissions present in this collection.
 * <p>
 * A {@code BasicPermission} is typically created by using the
 * {@link Permission#newBasicPermission()} factory method. If the mentioned
 * method returns {@code null}, then a {@code BasicPermission} of any type
 * can be used. If a collection is returned, it must be used for holding several
 * permissions of the particular type.
 * <p>
 * Subclasses must be implemented thread save.
 */
class BasicPermission : public Permission
{
public:
    /**
     * Returns the actions associated with this permission. Since {@code
     * BasicPermission} instances have no actions, an empty string is returned.
     *
     * @return an empty string.
     */
    // @Override
    CARAPI GetActions(
        /* [out] */ String* actions);

    /**
     * Indicates whether the specified permission is implied by this permission.
     *
     * @param permission
     *            the permission to check against this permission.
     * @return {@code true} if the specified permission is implied by this
     *         permission, {@code false} otherwise.
     */
    // @Override
    CARAPI Implies(
        /* [in] */ IPermission* permission,
        /* [out] */ Boolean* result);

    /**
     * Checks if {@code thisName} implies {@code thatName},
     * accordingly to hierarchical property naming convention.
     * It is assumed that names cannot be {@code null} or empty.
     */
    static CARAPI_(Boolean) NameImplies(
        /* [in] */ const String& thisName,
        /* [in] */ const String& thatName);

    /**
     * Returns an empty {@link PermissionCollection} for holding permissions.
     * <p>
     * For {@code PermissionCollection} (and subclasses which do not override
     * this method), the collection which is returned does <em>not</em> invoke
     * the {@link #implies(Permission)} method of the permissions which are
     * stored in it when checking if the collection implies a permission.
     * Instead, it assumes that if the type of the permission is correct, and
     * the name of the permission is correct, there is a match.
     *
     * @return an empty {@link PermissionCollection} for holding permissions.
     * @see BasicPermissionCollection
     */
    // @Override
    CARAPI NewPermissionCollection(
        /* [out] */ IPermissionCollection** permissionCollection);

protected:
    /**
     * Constructs a new instance of {@code BasicPermission} with the specified
     * name.
     *
     * @param name
     *            the name of the permission.
     * @throws NullPointerException if {@code name} is {@code null}.
     * @throws IllegalArgumentException if {@code name.length() == 0}.
     */
    CARAPI Init(
        /* [in] */ const String& name);

    /**
     * Constructs a new instance of {@code BasicPermission} with the specified
     * name. The {@code action} parameter is ignored.
     *
     * @param name
     *            the name of the permission.
     * @param action
     *            is ignored.
     * @throws NullPointerException
     *             if {@code name} is {@code null}.
     * @throws IllegalArgumentException
     *             if {@code name.length() == 0}.
     */
    CARAPI Init(
        /* [in] */ const String& name,
        /* [in] */ const String& action);

private:
    /**
     * Checks name parameter
     */
    CARAPI CheckName(
        /* [in] */ const String& name);

private:
};

#endif //__BASICPERMISSION_H__
