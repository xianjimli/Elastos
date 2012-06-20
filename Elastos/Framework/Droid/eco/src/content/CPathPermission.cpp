
#include "content/CPathPermission.h"

ECode CPathPermission::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    return E_NOT_IMPLEMENTED;
}

ECode CPathPermission::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    return E_NOT_IMPLEMENTED;
}

ECode CPathPermission::constructor(
    /* [in] */ String pattern,
    /* [in] */ Int32 type,
    /* [in] */ String readPermission,
    /* [in] */ String writePermission)
{
    return NOERROR;
}

ECode CPathPermission::constructor(
    /* [in] */ IParcel* src)
{
    return E_NOT_IMPLEMENTED;
}

String CPathPermission::GetReadPermission()
{
    return NULL;
}

String CPathPermission::GetWritePermission()
{
    return NULL;
}
