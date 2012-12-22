
#include "location/CGpsSatellite.h"
#include <elastos/AutoPtr.h>

CGpsSatellite::CGpsSatellite()
    : mValid(FALSE)
    , mHasEphemeris(FALSE)
    , mHasAlmanac(FALSE)
    , mUsedInFix(FALSE)
    , mPrn(0)
{
}

ECode CGpsSatellite::SetStatus(
    /* [in] */ IGpsSatellite* satellite)
{
    AutoPtr<CGpsSatellite> satelliteCls = (CGpsSatellite*)satellite;
    mValid = satelliteCls->mValid;
    mHasEphemeris = satelliteCls->mHasEphemeris;
    mHasAlmanac = satelliteCls->mHasAlmanac;
    mUsedInFix = satelliteCls->mUsedInFix;
    mSnr = satelliteCls->mSnr;
    mElevation = satelliteCls->mElevation;
    mAzimuth = satelliteCls->mAzimuth;

    return NOERROR;
}

ECode CGpsSatellite::GetPrn(
    /* [out] */ Int32* prn)
{
    VALIDATE_NOT_NULL(prn);
    *prn = mPrn;

    return NOERROR;
}

ECode CGpsSatellite::GetSnr(
    /* [out] */ Int32* snr)
{
    VALIDATE_NOT_NULL(snr);
    *snr = mSnr;

    return NOERROR;
}

ECode CGpsSatellite::GetElevation(
    /* [out] */ Float* elevation)
{
    VALIDATE_NOT_NULL(elevation);
    *elevation = mElevation;

    return NOERROR;
}

ECode CGpsSatellite::GetAzimuth(
    /* [out] */ Float* azimuth)
{
    VALIDATE_NOT_NULL(azimuth);
    *azimuth = mAzimuth;

    return NOERROR;
}

ECode CGpsSatellite::HasEphemeris(
    /* [out] */ Boolean* hasEphemeris)
{
    VALIDATE_NOT_NULL(hasEphemeris);
    *hasEphemeris = mHasEphemeris;

    return NOERROR;
}

ECode CGpsSatellite::HasAlmanac(
    /* [out] */ Boolean* hasAlmanac)
{
    VALIDATE_NOT_NULL(hasAlmanac);
    *hasAlmanac = mHasAlmanac;

    return NOERROR;
}

ECode CGpsSatellite::UsedInFix(
    /* [out] */ Boolean* usedInFix)
{
    VALIDATE_NOT_NULL(usedInFix);
    *usedInFix = mUsedInFix;

    return NOERROR;
}

ECode CGpsSatellite::constructor(
    /* [in] */ Int32 prn)
{
    mPrn = prn;

    return NOERROR;
}
