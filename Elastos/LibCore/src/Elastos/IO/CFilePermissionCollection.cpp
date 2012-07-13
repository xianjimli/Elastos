
#include "CFilePermissionCollection.h"


const Int64 CFilePermissionCollection::sSerialVersionUID = 2202956749081564585LL;

CFilePermissionCollection::CFilePermissionCollection()
{
}

CFilePermissionCollection::~CFilePermissionCollection()
{
}

ECode CFilePermissionCollection::Add(
    /* [in] */ IPermission* permission)
{
//    if (isReadOnly()) {
//        throw new IllegalStateException();
//    }
//    if (permission instanceof FilePermission) {
//        permissions.addElement(permission);
//    } else {
//        throw new IllegalArgumentException(permission.toString());
//    }
    return E_NOT_IMPLEMENTED;
}

ECode CFilePermissionCollection::Elements(
    /* [out] */ IObjectEnumerator** elements)
{
    VALIDATE_NOT_NULL(elements);
//  return permissions.elements();
    return E_NOT_IMPLEMENTED;
}

ECode CFilePermissionCollection::Implies(
    /* [in] */ IPermission* permission,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

//    if (permission instanceof FilePermission) {
//        FilePermission fp = (FilePermission) permission;
//        int matchedMask = 0;
//        int i = 0;
//        while (i < permissions.size()
//                && ((matchedMask & fp.mask) != fp.mask)) {
//            // Cast will not fail since we added it
//            matchedMask |= ((FilePermission) permissions.elementAt(i))
//                    .impliesMask(permission);
//            i++;
//        }
//        return ((matchedMask & fp.mask) == fp.mask);
//    }
//    return false;
    return E_NOT_IMPLEMENTED;
}