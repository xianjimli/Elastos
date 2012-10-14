
#include "ext/frameworkext.h"
#include "content/CFeatureInfo.h"

ECode CFeatureInfo::GetDescription(
    /* [out] */ String* des)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFeatureInfo::GetGlEsVersion(
    /* [out] */ String* ver)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFeatureInfo::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mName;
    return NOERROR;
}

ECode CFeatureInfo::SetName(
    /* [in] */ const String& name)
{
    mName = name;
    return NOERROR;
}

ECode CFeatureInfo::GetReqGlEsVersion(
    /* [out] */ Int32* reqGlEsVersion)
{
    VALIDATE_NOT_NULL(reqGlEsVersion);
    *reqGlEsVersion = mReqGlEsVersion;
    return NOERROR;
}

ECode CFeatureInfo::SetReqGlEsVersion(
    /* [in] */ Int32 reqGlEsVersion)
{
    mReqGlEsVersion = reqGlEsVersion;
    return NOERROR;
}

ECode CFeatureInfo::GetFlags(
    /* [out] */ Int32* flags)
{
    VALIDATE_NOT_NULL(flags);
    *flags = mFlags;
    return NOERROR;
}

ECode CFeatureInfo::SetFlags(
    /* [in] */ Int32 flags)
{
    mFlags = flags;
    return NOERROR;
}

ECode CFeatureInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFeatureInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFeatureInfo::constructor()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFeatureInfo::constructor(
    /* [in] */ IFeatureInfo* orig)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFeatureInfo::constructor(
    /* [in] */ IParcel* source)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

