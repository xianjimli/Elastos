#include "cmdef.h"
#include "FilePermissionCollection.h"

FilePermissionCollection::FilePermissionCollection()
{
}

FilePermissionCollection::~FilePermissionCollection()
{
    
}

ECode FilePermissionCollection::Init()
{
//    super();
    return NOERROR;
}

ECode FilePermissionCollection::Add()
//   /* [in] */ IPermission *permission)
{
//    if (isReadOnly()) {
//        throw new IllegalStateException();
//    }
//    if (permission instanceof FilePermission) {
//        permissions.addElement(permission);
//    } else {
//        throw new IllegalArgumentException(permission.toString());
//    }
    return NOERROR;
}

ECode FilePermissionCollection::Elements()
//   /* [out] */ Enumeration<Permission> elements)
{
//  return permissions.elements();
    return NOERROR;
}

ECode FilePermissionCollection::Implies(
//    /* [in] */ IPermission *permission,
    /* [out] */ Boolean* IsImplies)
{
    VALIDATE_NOT_NULL(IsImplies != NULL);

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
    return NOERROR;
}
