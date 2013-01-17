
#include "cmdef.h"
#include "BasicPermission.h"
#include <Elastos.Core.h>

ECode BasicPermission::Init(
    /* [in] */ const String& name)
{
    FAIL_RETURN(Permission::Init(name));
    return CheckName(name);
}

ECode BasicPermission::Init(
    /* [in] */ const String& name,
    /* [in] */ const String& action)
{
    FAIL_RETURN(Permission::Init(name));
    return CheckName(name);
}

ECode BasicPermission::CheckName(
    /* [in] */ const String& name)
{
    if (name.IsNull()) {
//        throw new NullPointerException("name == null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (name.IsEmpty()) {
//        throw new IllegalArgumentException("name.isEmpty()");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    return NOERROR;
}

ECode BasicPermission::GetActions(
    /* [out] */ String* actions)
{
    VALIDATE_NOT_NULL(actions);
    *actions = "";
    return NOERROR;
}

ECode BasicPermission::Implies(
    /* [in] */ IPermission* permission,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // if (permission != null && permission.getClass() == this.getClass()) {
    //     return nameImplies(getName(), permission.getName());
    // }
    // return false;
    return E_NOT_IMPLEMENTED;
}

Boolean BasicPermission::NameImplies(
    /* [in] */ const String& thisName,
    /* [in] */ const String& thatName)
{
    if (thisName == thatName) {
        return TRUE;
    }
    Int32 end = thisName.GetCharCount();
    if (end > thatName.GetCharCount()) {
        return FALSE;
    }
    if (thisName.GetChar(--end) == '*'
        && (end == 0 || thisName.GetChar(end - 1) == '.')) {
        //wildcard found
        end--;
    }
    else if (end != (thatName.GetCharCount() - 1)) {
        //names are not equal
        return FALSE;
    }
    for (Int32 i = end; i >= 0; i--) {
        if (thisName.GetChar(i) != thatName.GetChar(i)) {
            return FALSE;
        }
    }
    return TRUE;
}

ECode BasicPermission::NewPermissionCollection(
    /* [out] */ IPermissionCollection** permissionCollection)
{
    // return new BasicPermissionCollection();
    return E_NOT_IMPLEMENTED;
}

