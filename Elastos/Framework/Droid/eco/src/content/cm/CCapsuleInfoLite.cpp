
#include "ext/frameworkdef.h"
#include "content/cm/CCapsuleInfoLite.h"

CCapsuleInfoLite::CCapsuleInfoLite()
    : mRecommendedInstallLocation(0)
    , mInstallLocation(0)
{
}

ECode CCapsuleInfoLite::ToString(
    /* [out] */ String* des)
{
    VALIDATE_NOT_NULL(des);

//    return "PackageInfoLite{"
//            + Integer.toHexString(System.identityHashCode(this))
//            + " " + packageName + "}";
    return E_NOT_IMPLEMENTED;
}

ECode CCapsuleInfoLite::DescribeContents(
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);

    *ret = 0;
    return NOERROR;
}

ECode CCapsuleInfoLite::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    source->ReadString(&mCapsuleName);
    source->ReadInt32(&mRecommendedInstallLocation);
    source->ReadInt32(&mInstallLocation);
    return NOERROR;
}

ECode CCapsuleInfoLite::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteString(mCapsuleName);
    dest->WriteInt32(mRecommendedInstallLocation);
    dest->WriteInt32(mInstallLocation);
    return NOERROR;
}

ECode CCapsuleInfoLite::constructor()
{
    return NOERROR;
}

ECode CCapsuleInfoLite::constructor(
    /* [in] */ IParcel* source)
{
    return E_NOT_IMPLEMENTED;
}

