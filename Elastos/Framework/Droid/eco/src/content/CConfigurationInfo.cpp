
#include "ext/frameworkdef.h"
#include "content/CConfigurationInfo.h"

ECode CConfigurationInfo::GetDescription(
    /* [out] */ String* des)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CConfigurationInfo::GetGlEsVersion(
    /* [out] */ String* ver)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CConfigurationInfo::GetReqTouchScreen(
    /* [out] */ Int32* reqTouchScreen)
{
    VALIDATE_NOT_NULL(reqTouchScreen);
    *reqTouchScreen = mReqTouchScreen;
    return NOERROR;
}

ECode CConfigurationInfo::SetReqTouchScreen(
    /* [in] */ Int32 reqTouchScreen)
{
    mReqTouchScreen = reqTouchScreen;
    return NOERROR;
}

ECode CConfigurationInfo::GetReqKeyboardType(
    /* [out] */ Int32* reqKeyboardType)
{
    VALIDATE_NOT_NULL(reqKeyboardType);
    *reqKeyboardType = mReqKeyboardType;
    return NOERROR;
}

ECode CConfigurationInfo::SetReqKeyboardType(
    /* [in] */ Int32 reqKeyboardType)
{
    mReqKeyboardType = reqKeyboardType;
    return NOERROR;
}

ECode CConfigurationInfo::GetReqNavigation(
    /* [out] */ Int32* reqNavigation)
{
    VALIDATE_NOT_NULL(reqNavigation);
    *reqNavigation = mReqNavigation;
    return NOERROR;
}

ECode CConfigurationInfo::SetReqNavigation(
    /* [in] */ Int32 reqNavigation)
{
    mReqNavigation = reqNavigation;
    return NOERROR;
}

ECode CConfigurationInfo::GetReqInputFeatures(
    /* [out] */ Int32* reqInputFeatures)
{
    VALIDATE_NOT_NULL(reqInputFeatures);
    *reqInputFeatures = mReqInputFeatures;
    return NOERROR;
}

ECode CConfigurationInfo::SetReqInputFeatures(
    /* [in] */ Int32 reqInputFeatures)
{
    mReqInputFeatures = reqInputFeatures;
    return NOERROR;
}

ECode CConfigurationInfo::GetReqGlEsVersion(
    /* [out] */ Int32* reqGlEsVersion)
{
    VALIDATE_NOT_NULL(reqGlEsVersion);
    *reqGlEsVersion = mReqGlEsVersion;
    return NOERROR;
}

ECode CConfigurationInfo::SetReqGlEsVersion(
    /* [in] */ Int32 reqGlEsVersion)
{
    mReqGlEsVersion = reqGlEsVersion;
    return NOERROR;
}

ECode CConfigurationInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CConfigurationInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CConfigurationInfo::constructor()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CConfigurationInfo::constructor(
    /* [in] */ IConfigurationInfo* orig)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CConfigurationInfo::constructor(
    /* [in] */ IParcel* source)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

