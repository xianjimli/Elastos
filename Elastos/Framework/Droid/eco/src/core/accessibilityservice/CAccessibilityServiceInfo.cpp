

#include "accessibilityservice/CAccessibilityServiceInfo.h"
#include "ext/frameworkdef.h"

CAccessibilityServiceInfo::CAccessibilityServiceInfo()
    : mEventTypes(-1)
    , mFeedbackType(-1)
    , mNotificationTimeout(0)
    , mFlags(-1)
{}

CAccessibilityServiceInfo::~CAccessibilityServiceInfo()
{
    ArrayOf<String>::Free(mPackageNames);
}

ECode CAccessibilityServiceInfo::SetEventTypes (
    /* [in] */ Int32 types)
{
    mEventTypes = types;
    return NOERROR;
}

ECode CAccessibilityServiceInfo::GetEventTypes (
    /* [out] */ Int32* types)
{
    VALIDATE_NOT_NULL(types);
    *types = mEventTypes;
    return NOERROR;
}

ECode CAccessibilityServiceInfo::SetPackageNames (
    /* [in] */ const ArrayOf<String>& names)
{
    mPackageNames = names.Clone();
    return NOERROR;
}

ECode CAccessibilityServiceInfo::GetPackageNames (
    /* [out, callee] */ ArrayOf<String>** names)
{
    VALIDATE_NOT_NULL(names);
    *names = mPackageNames;
    return NOERROR;
}

ECode CAccessibilityServiceInfo::SetFeedbackType (
    /* [in] */ Int32 type)
{
    mFeedbackType = type;
    return NOERROR;
}

ECode CAccessibilityServiceInfo::GetFeedbackType (
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type);
    *type = mFeedbackType;
    return NOERROR;
}

ECode CAccessibilityServiceInfo::SetNotificationTimeout (
    /* [in] */ Int64 timeout)
{
    mNotificationTimeout = timeout;
    return NOERROR;
}

ECode CAccessibilityServiceInfo::GetNotificationTimeout (
    /* [out] */ Int64* timeout)
{
    VALIDATE_NOT_NULL(timeout);
    *timeout = mNotificationTimeout;
    return NOERROR;
}

ECode CAccessibilityServiceInfo::SetFlags (
    /* [in] */ Int32 flags)
{
    mFlags = flags;
    return NOERROR;
}

ECode CAccessibilityServiceInfo::GetFlags (
    /* [out] */ Int32* flags)
{
    VALIDATE_NOT_NULL(flags);
    *flags = mFlags;
    return NOERROR;
}

ECode CAccessibilityServiceInfo::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    source->ReadInt32(&mEventTypes);
    source->ReadArrayOfString((ArrayOf<String>**)&mPackageNames);
    source->ReadInt32(&mFeedbackType);
    source->ReadInt64(&mNotificationTimeout);
    source->ReadInt32(&mFlags);
    return NOERROR;
}

ECode CAccessibilityServiceInfo::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    dest->WriteInt32(mEventTypes);
    dest->WriteArrayOfString(mPackageNames);
    dest->WriteInt32(mFeedbackType);
    dest->WriteInt64(mNotificationTimeout);
    dest->WriteInt32(mFlags);
    return NOERROR;
}
