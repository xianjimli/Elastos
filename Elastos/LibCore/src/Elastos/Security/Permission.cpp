
#include "Permission.h"

const Int64 Permission::sSerialVersionUID = -5636570222231596674LL;

Permission(
    /* [in] */ const String& name)
    : mName(AutoString::Duplicate(name))
{}

~Permission()
{}

ECode Permission::GetName(
    /* [out] */ String* name)
{
    assert(name != NULL);
    *name = String::Dupliate(mName);
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

