#ifndef __CFILEPERMISSION_H__
#define __CFILEPERMISSION_H__

#include "_CFilePermission.h"

using namespace Elastos;

CarClass(CFilePermission)
{
public:
    CFilePermission();

    ~CFilePermission();

    CARAPI constructor(
        /* [in] */ const String& path,
        /* [in] */ const String& actions);

    CARAPI GetActions(
        /* [out] */ String* actions);

    CARAPI Equals(
        /* [in] */ IFilePermission* fp,
        /* [out] */ Boolean* isEqual);

    CARAPI Implies(
        /* [in] */ IPermission* p,
        /* [out] */ Boolean* result);

    CARAPI NewPermissionCollection(
        /* [out] */ IPermissionCollection** object);

    CARAPI GetHashCode(
        /* [out] */ Int32* hasCode);

private:
    CARAPI Init(
        /* [in] */ const String& path,
        /* [in] */ const String& actions);

    /**
     * Returns the string representing this permission's actions. It must be of
     * the form "read,write,execute,delete", all lower case and in the correct
     * order if there is more than one action.
     *
     * @param action
     *            the action name
     * @return the string representing this permission's actions
     */
    CARAPI_(String) ToCanonicalActionString(
        /* [in] */ const String& action);

    /**
     * Returns the numerical representation of the argument.
     *
     * @param actionNames
     *            the action names
     * @return the action mask
     */
    CARAPI_(Int32) GetMask(
        /* [in] */ const String& actionNames);

    /**
     * Returns an int describing what masks are implied by a specific
     * permission.
     *
     * @param p
     *            the permission
     * @return the mask applied to the given permission
     */
    CARAPI_(Int32) ImpliesMask(
        /* [in] */ IPermission* p);

private:
    static const Int64 sSerialVersionUID;

    // canonical path of this permission
    String mCanonPath;

    // list of actions permitted for socket permission in order
    static const String sActionList[];

    // "canonicalized" action list
    String mActions;

    // the numeric representation of this action list
    // for implies() to check if one action list is the subset of another.
    Int32 mMask;

    // global include all permission?
    Boolean mIncludeAll;

    Boolean mAllDir;

    Boolean mAllSubdir;
};

#endif // __CFILEPERMISSION_H__