
#include "cmdef.h"
#include "Permission.h"
#include <Elastos.Core.h>

Permission::Permission()
{}

ECode Permission::Init(
    /* [in] */ const String& name)
{
	mName = name;
    return NOERROR;
}

ECode Permission::GetName(
    /* [out] */ String* name)
{
    assert(name != NULL);
    *name = mName;
    return NOERROR;
}

ECode Permission::NewPermissionCollection(
        /* [out] */ IPermissionCollection** permissionCollection)
{
	VALIDATE_NOT_NULL(permissionCollection);
	*permissionCollection = NULL;
	return NOERROR;
}

ECode Permission::CheckGuard(
    /* [in] */ IInterface* obj)
{
//    SecurityManager sm = System.getSecurityManager();
//    if (sm != null) {
//        sm.checkPermission(this);
//    }
    return NOERROR;
}

