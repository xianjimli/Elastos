#ifndef __FILEPERMISSION_H__
#define __FILEPERMISSION_H__

#include <elastos.h>
#include <Elastos.System.h>
#include "Elastos.IO2_server.h"


using namespace Elastos;

class FilePermission
{
public:
    FilePermission();

    virtual ~FilePermission();

    /**
     * Constructs a new FilePermission with the path and actions specified.
     *
     * @param path
     *            the pathname of the file or directory to apply the actions to.
     * @param actions
     *            the actions for the {@code path}. May be any combination of
     *            "read", "write", "execute" and "delete".
     * @throws IllegalArgumentException
     *             if {@code actions} is {@code null} or an empty string, or if
     *             it contains a string other than "read", "write", "execute"
     *             and "delete".
     * @throws NullPointerException
     *             if {@code path} is {@code null}.
     */
    CARAPI Init(
        /* [in] */ const String path,
        /* [in] */ String actions);

    /**
     * Returns the actions associated with this file permission.
     *
     * @return the actions associated with this file permission.
     */
    CARAPI GetActions(
        /* [out] */ String* actions);

    /**
     * Indicates if this file permission is equal to another. The two are equal
     * if {@code obj} is a FilePermission, they have the same path, and they
     * have the same actions.
     *
     * @param obj
     *            the object to check equality with.
     * @return {@code true} if this file permission is equal to {@code obj},
     *         {@code false} otherwise.
     */
    CARAPI Equals(
        /* [in] */ IObject *obj,
        /* [out] */ Boolean* isEqual);

    /**
     * Indicates whether the permission {@code p} is implied by this file
     * permission. This is the case if {@code p} is an instance of
     * {@code FilePermission}, if {@code p}'s actions are a subset of this
     * file permission's actions and if {@code p}'s path is implied by this
     * file permission's path.
     *
     * @param p
     *            the permission to check.
     * @return {@code true} if the argument permission is implied by the
     *         receiver, and {@code false} if it is not.
     */
    CARAPI Implies(
//        /* [in] */ IPermission *p,
        /* [out] */ Boolean* isimplied);

    /**
     * Returns a new PermissionCollection in which to place FilePermission
     * objects.
     *
     * @return A new PermissionCollection object suitable for storing
     *         FilePermission objects.
     */
    CARAPI NewPermissionCollection();
//        /* [out] */ PermissionCollection* object);

    /**
     * Calculates the hash code value for this file permission.
     *
     * @return the hash code value for this file permission.
     */
    CARAPI HashCode(
        /* [out] */ Int32* hasCode);

protected:
    /**
     * Returns an int describing what masks are implied by a specific
     * permission.
     *
     * @param p
     *            the permission
     * @return the mask applied to the given permission
     */
    CARAPI ImpliesMask(
//        /* [in] */ IPermission *p,
        /* [out] */ Int32* result);

private:
    /**
     * Returns the string representing this permission's actions. It must be of
     * the form "read,write,execute,delete", all lower case and in the correct
     * order if there is more than one action.
     *
     * @param action
     *            the action name
     * @return the string representing this permission's actions
     */
    CARAPI ToCanonicalActionString(
        /* [in] */ String action,
        /* [out] */ String* result);

    /**
     * Returns the numerical representation of the argument.
     *
     * @param actionNames
     *            the action names
     * @return the action mask
     */
    CARAPI GetMask(
        /* [in] */ String actionNames,
        /* [out] */ Int32* result);

    CARAPI WriteObject();
//        /* [in] */ ObjectOutputStream stream);

    CARAPI ReadObject();
//        /* [in] */ ObjectInputStream stream);

private:
    String m_actions;
    Boolean m_includeAll;
    Boolean m_allDir;
    Boolean m_allSubdir;
    String m_canonPath;
    static const String actionList[4];
protected:
    Int32 m_mask;
};

#endif //__FILEPERMISSION_H__