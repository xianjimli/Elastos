
#include "cmdef.h"
#include "PermissionCollection.h"
#include <Com.Kortide.Platform.h>

const Int64 PermissionCollection::sSerialVersionUID = -6727011328946861783L;

PermissionCollection::PermissionCollection()
	: mReadOnly(FALSE)
{}

PermissionCollection::~PermissionCollection()
{}

ECode PermissionCollection::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    VALIDATE_NOT_NULL(isReadOnly);
    *isReadOnly = mReadOnly;
    return NOERROR;
}

ECode PermissionCollection::SetReadOnly()
{
	mReadOnly = TRUE;
    return NOERROR;
}

