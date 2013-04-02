
#include "ext/frameworkdef.h"
#include "content/CPathPermission.h"

ECode CPathPermission::GetPath(
    /* [out] */ String* path)
{
    return PatternMatcher::GetPath(path);
}

ECode CPathPermission::GetType(
    /* [out] */ Int32* path)
{
    return PatternMatcher::GetType(path);
}

ECode CPathPermission::Match(
    /* [in] */ const String& str,
    /* [out] */ Boolean* isMatch)
{
    return PatternMatcher::Match(str, isMatch);
}

ECode CPathPermission::GetDescription(
    /* [out] */ String* description)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CPathPermission::DescribeContents(
    /* [out] */ Int32* count)
{
    return PatternMatcher::DescribeContents(count);
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
    //TODO:
    return E_NOT_IMPLEMENTED;
}

ECode CPathPermission::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    PatternMatcher::WriteToParcel(dest);
    dest->WriteString(mReadPermission);
    dest->WriteString(mWritePermission);
    return NOERROR;
}

ECode CPathPermission::constructor(
    /* [in] */ const String& pattern,
    /* [in] */ Int32 type,
    /* [in] */ const String& readPermission,
    /* [in] */ const String& writePermission)
{
    PatternMatcher::constructor(pattern, type);
    mReadPermission = readPermission;
    mWritePermission = writePermission;
    return NOERROR;
}

ECode CPathPermission::constructor(
    /* [in] */ IParcel* src)
{
    PatternMatcher::constructor(src);
    src->ReadString(&mReadPermission);
    src->ReadString(&mWritePermission);
    return NOERROR;
}
