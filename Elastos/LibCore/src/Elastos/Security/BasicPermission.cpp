
#include "cmdef.h"
#include "BasicPermission.h"
#include <Com.Kortide.Platform.h>

const Int64 BasicPermission::sSerialVersionUID = -5636570222231596674L;

ECode BasicPermission::Init(
    /* [in] */ const String& name)
{
    Permission::Init(name);
    return CheckName(name);
}

ECode BasicPermission::Init(
    /* [in] */ const String& name,
    /* [in] */ const String& action)
{
    Permission::Init(name);
    return CheckName(name);
}

ECode BasicPermission::CheckName(
    /* [in] */ const String& name)
{
    if (name.IsNull()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new NullPointerException("name == null");
    }
    if (name.IsEmpty()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("name.isEmpty()");
    }

    return NOERROR;
}

ECode BasicPermission::GetActions(
    /* [out] */ String* actions)
{
    VALIDATE_NOT_NULL(actions);
    *actions = String("");
    return NOERROR;
}

ECode BasicPermission::Implies(
    /* [in] */ IPermission* permission,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

Boolean BasicPermission::NameImplies(
    /* [in] */ const String& thisName,
    /* [in] */ const String& thatName)
{
    if (thisName == thatName) {
        return TRUE;
    }
    Int32 end = thisName.GetLength();
    if ((UInt32)end > thatName.GetLength()) {
        return FALSE;
    }
    if (thisName[--end] == '*'
        && (end == 0 || thisName[end - 1] == '.')) {
        //wildcard found
        end--;
    }
    else if ((UInt32)end != (thatName.GetLength()-1)) {
        //names are not equal
        return FALSE;
    }
    for (Int32 i = end; i >= 0; i--) {
        if (thisName[i] != thatName[i]) {
            return FALSE;
        }
    }
    return TRUE;
}

ECode BasicPermission::NewPermissionCollection(
    /* [out] */ IPermissionCollection** permissionCollection)
{
    return E_NOT_IMPLEMENTED;
}

