
#include "cmdef.h"
#include "Permission.h"
#include <Com.Kortide.Platform.h>

const Int64 Permission::sSerialVersionUID = -5636570222231596674LL;

Permission::Permission()
{}

void Permission::Init(
    /* [in] */ const String& name)
{
	mName = name;
}

Permission::~Permission()
{}

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
    return E_NOT_IMPLEMENTED;
}

