#include "cmdef.h"
#include "CFilePermissionCollection.h"


CFilePermissionCollection::CFilePermissionCollection()
{
}

CFilePermissionCollection::~CFilePermissionCollection()
{
}

ECode CFilePermissionCollection::constructor()
{
    return FilePermissionCollection::Init();
}

ECode CFilePermissionCollection::Add()
//   /* [in] */ IPermission *permission)
{
    return FilePermissionCollection::Add();
}

ECode CFilePermissionCollection::Elements()
//   /* [out] */ Enumeration<Permission> elements)
{
    return FilePermissionCollection::Elements();
}

ECode CFilePermissionCollection::Implies(
//    /* [in] */ IPermission *permission,
    /* [out] */ Boolean* IsImplies)
{
    VALIDATE_NOT_NULL(IsImplies != NULL);
    return FilePermissionCollection::Implies(IsImplies);
}