
#ifndef __BASICPERMISSION_H__
#define __BASICPERMISSION_H__

#ifdef _ELASTOSSECURITY_
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

    CARAPI GetActions(
        /* [out] */ String* actions);

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

    CARAPI NewPermissionCollection(
        /* [out] */ IPermissionCollection** permissionCollection);

private:
    /**
     * Checks name parameter
     */
    CARAPI CheckName(
        /* [in] */ const String& name);

private:
    static const Int64 sSerialVersionUID;
    String name;
};

#endif //__BASICPERMISSION_H__
