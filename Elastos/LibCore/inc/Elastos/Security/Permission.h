
#ifndef __PERMISSION_H__
#define __PERMISSION_H__

#include <elastos.h>
#include "AutoString.h"

using namespace Elastos;

/**
 * {@code Permission} is the common base class of all permissions that
 * participate in the access control security framework around
 * {@link AccessController} and {@link AccessControlContext}. A permission
 * constitutes of a name and associated actions.
 */
class Permission
{
public:
    /**
     * Constructs a new instance of {@code Permission} with its name.
     *
     * @param name
     *            the name of the permission.
     */
    Permission(
        /* [in] */ const String& name);

    ~Permission();

    /**
     * Compares the specified object with this {@code Permission} for equality
     * and returns {@code true} if the specified object is equal, {@code false}
     * otherwise.
     * <p>
     * The {@link #implies(Permission)} method should be used for making access
     * control checks.
     *
     * @param obj
     *            object to be compared for equality with this {@code
     *            Permission}.
     * @return {@code true} if the specified object is equal to this {@code
     *         Permission}, otherwise {@code false}.
     */
    virtual CARAPI Equals(
        /* [in] */ IInterface* obj,
        /* [out] */ Boolean* result) = 0;

    /**
     * Returns the hash code value for this {@code Permission}. Returns the same
     * hash code for {@code Permission}s that are equal to each other as
     * required by the general contract of {@link Object#hashCode}.
     *
     * @return the hash code value for this {@code Permission}.
     * @see Object#equals(Object)
     * @see Permission#equals(Object)
     */
    virtual CARAPI GetHashCode(
        /* [out] */ Int32* hashCode) = 0;

    /**
     * Returns a comma separated string identifying the actions associated with
     * this permission. The returned actions are in canonical form. For example:
     *
     * <pre>
     * sp0 = new SocketPermission(&quot;www.example.com&quot;, &quot;connect,resolve&quot;)
     * sp1 = new SocketPermission(&quot;www.example.com&quot;, &quot;resolve,connect&quot;)
     * sp0.getActions().equals(sp1.getActions()) //yields true
     * </pre>
     *
     * Both permissions return "connect,resolve" (in that order) if {@code
     * #getActions()} is invoked. Returns an empty String, if no actions are
     * associated with this permission.
     *
     * @return the actions associated with this permission or an empty string if
     *         no actions are associated with this permission.
     */
    virtual CARAPI GetActions(
        /* [out] */ String* actions) = 0;

    /**
     * Indicates whether the specified permission is implied by this permission.
     * The {@link AccessController} uses this method to check whether permission
     * protected access is allowed with the present policy.
     *
     * @param permission
     *            the permission to check against this permission.
     * @return {@code true} if the specified permission is implied by this
     *         permission, {@code false} otherwise.
     */
    virtual CARAPI Implies(
        /* [in] */ IPermission* permission,
        /* [out] */ Boolean* result) = 0;

    /**
     * Returns the name of this permission.
     *
     * @return the name of this permission.
     */
    CARAPI GetName(
        /* [out] */ String* name);

    /**
     * Invokes {@link SecurityManager#checkPermission(Permission)} with this
     * permission as its argument. This method implements the {@link Guard}
     * interface.
     *
     * @param obj
     *            as specified in {@link Guard#checkGuard(Object)} but ignored
     *            in this implementation.
     * @throws SecurityException
     *             if this permission is not granted.
     * @see Guard
     * @see SecurityManager#checkPermission(Permission)
     */
    virtual CARAPI CheckGuard(
        /* [in] */ IInterface* obj);

    /**
     * Returns a specific {@link PermissionCollection} container for permissions
     * of this type. Returns {@code null} if any permission collection can be
     * used.
     * <p>
     * Subclasses may override this method to return an appropriate collection
     * for the specific permissions they implement.
     *
     * @return an empty {@link PermissionCollection} or {@code null} if any
     *         permission collection can be used.
     */
//    public PermissionCollection newPermissionCollection() {
//        return null;
//    }

    /**
     * Returns a string containing a concise, human-readable description of the
     * this {@code Permission} including its name and its actions.
     *
     * @return a printable representation for this {@code Permission}.
     */
//    @Override
//    public String toString() {
//        String actions = getActions();
//        actions = (actions == null || actions.length() == 0) ? "" : " "
//                + getActions();
//        return "(" + getClass().getName() + " " + getName() + actions + ")";
//    }

private:
    static const Int64 sSerialVersionUID = -5636570222231596674LL;

    AutoString mName;
}

#endif //__PERMISSION_H__
