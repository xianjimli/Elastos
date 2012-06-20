#include "CFilePermission.h"

CFilePermission::CFilePermission()
{
}

CFilePermission::~CFilePermission()
{
}

ECode CFilePermission::constructor(
    /* [in] */ String path,
    /* [in] */ String actions)
{
    return Init(path, actions);
}

ECode CFilePermission::GetActions(
    /* [out] */ String* actions)
{
    return FilePermission::GetActions(actions);
}

ECode CFilePermission::Equals(
    /* [in] */ IObject* obj,
    /* [out] */ Boolean* isEqual)
{
    return FilePermission::Equals(obj, isEqual);
}

ECode CFilePermission::Implies(
//    /* [in] */ IPermission* p,
    /* [out] */ Boolean* isimplied)
{
    return FilePermission::Implies(isimplied);
}

ECode CFilePermission::NewPermissionCollection()
//    /* [out] */ PermissionCollection* object)
{
    return FilePermission::NewPermissionCollection();
}

ECode CFilePermission::HashCode(
    /* [out] */ Int32* hasCode)
{
    return FilePermission::HashCode(hasCode);
}