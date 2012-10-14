
#include "ext/frameworkdef.h"
#include "content/CPathPermission.h"

ECode CPathPermission::GetPath(
    /* [out] */ String* path)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CPathPermission::GetType(
    /* [out] */ Int32* path)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CPathPermission::Match(
    /* [in] */ const String& str,
    /* [out] */ Boolean* isMatch)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CPathPermission::GetDescription(
    /* [out] */ String* des)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CPathPermission::DescribeContents(
    /* [out] */ Int32* count)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CPathPermission::GetReadPermission(
    /* [out] */ String* permission)
{
    VALIDATE_NOT_NULL(permission);
    *permission = mReadPermission;
    return NOERROR;
}

ECode CPathPermission::GetWritePermission(
    /* [out] */ String* permission)
{
    VALIDATE_NOT_NULL(permission);
    *permission = mWritePermission;
    return NOERROR;
}

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
    /* [in] */ const String& pattern,
    /* [in] */ Int32 type,
    /* [in] */ const String& readPermission,
    /* [in] */ const String& writePermission)
{
    return NOERROR;
}

ECode CPathPermission::constructor(
    /* [in] */ IParcel* src)
{
    return E_NOT_IMPLEMENTED;
}
