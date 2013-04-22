
#include "ext/frameworkdef.h"
#include "accounts/CAuthenticatorDescription.h"

CAuthenticatorDescription::CAuthenticatorDescription()
    : mLabelId(0)
    , mIconId(0)
    , mSmallIconId(0)
    , mAccountPreferencesId(0)
    , mCustomTokens(FALSE)
{}

ECode CAuthenticatorDescription::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAuthenticatorDescription::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAuthenticatorDescription::constructor(
    /* [in] */ const String& type,
    /* [in] */ const String& capsuleName,
    /* [in] */ Int32 labelId,
    /* [in] */ Int32 iconId,
    /* [in] */ Int32 smallIconId,
    /* [in] */ Int32 prefId,
    /* [in] */ Boolean customTokens)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAuthenticatorDescription::constructor(
    /* [in] */ const String& type,
    /* [in] */ const String& capsuleName,
    /* [in] */ Int32 labelId,
    /* [in] */ Int32 iconId,
    /* [in] */ Int32 smallIconId,
    /* [in] */ Int32 prefId)
{
    return E_NOT_IMPLEMENTED;
}
