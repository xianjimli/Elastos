
#include "cmdef.h"
#include "PermissionCollection.h"


PermissionCollection::PermissionCollection()
	: mReadOnly(FALSE)
{}

ECode PermissionCollection::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    assert(isReadOnly != NULL);
    *isReadOnly = mReadOnly;
    return NOERROR;
}

ECode PermissionCollection::SetReadOnly()
{
	mReadOnly = TRUE;
    return NOERROR;
}

