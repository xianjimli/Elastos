
#include "location/CGeocoderParams.h"

ECode CGeocoderParams::GetLocale(
    /* [out] */ ILocale** locale)
{
    VALIDATE_NOT_NULL(locale);
    *locale = mLocale;

    return NOERROR;
}

ECode CGeocoderParams::GetClientCapsule(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mCapsuleName;

    return NOERROR;
}

ECode CGeocoderParams::GetDescription(
    /* [out] */ String* str)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CGeocoderParams::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    AutoPtr<CGeocoderParams> gp;
    ASSERT_SUCCEEDED(CGeocoderParams::NewByFriend((CGeocoderParams**)&gp));
    String language, country, variant;
    source->ReadString(&language);
    source->ReadString(&country);
    source->ReadString(&variant);
    ASSERT_SUCCEEDED(CLocale::New(language, country, variant, (ILocale**)&(gp->mLocale)));
    source->ReadString(&(gp->mCapsuleName));

    return NOERROR;
}

ECode CGeocoderParams::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return E_NOT_IMPLEMENTED;
}

ECode CGeocoderParams::constructor()
{
    return NOERROR;
}

ECode CGeocoderParams::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ ILocale* locale)
{
    mLocale = locale;
    ctx->GetCapsuleName(&mCapsuleName);

    return NOERROR;
}
