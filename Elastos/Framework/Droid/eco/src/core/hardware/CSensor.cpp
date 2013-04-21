
#include "ext/frameworkext.h"
#include "hardware/CSensor.h"

CSensor::CSensor()
    : mVersion(0)
    , mHandle(0)
    , mType(0)
    , mMaxRange(0.0f)
    , mResolution(0.0f)
    , mPower(0.0f)
    , mMinDelay(0)
    , mLegacyType(0)
{
}

ECode CSensor::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);

    *name = mName;

    return NOERROR;
}

ECode CSensor::GetVendor(
    /* [out] */ String* vendor)
{
    VALIDATE_NOT_NULL(vendor);

    *vendor = mVendor;

    return NOERROR;
}

ECode CSensor::GetType(
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type);

    *type = mType;

    return NOERROR;
}

ECode CSensor::GetVersion(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    *value = mVersion;

    return NOERROR;
}

ECode CSensor::GetMaximumRange(
    /* [out] */ Float* range)
{
    VALIDATE_NOT_NULL(range);

    *range = mMaxRange;

    return NOERROR;
}

ECode CSensor::GetResolution(
    /* [out] */ Float* resolution)
{
    VALIDATE_NOT_NULL(resolution);

    *resolution = mResolution;

    return NOERROR;
}

ECode CSensor::GetPower(
    /* [out] */ Float* power)
{
    VALIDATE_NOT_NULL(power);

    *power = mPower;

    return NOERROR;
}

ECode CSensor::GetMinDelay(
    /* [out] */ Int32* minDelay)
{
    VALIDATE_NOT_NULL(minDelay);

    *minDelay = mMinDelay;

    return NOERROR;
}

ECode CSensor::GetHandle(
    /* [out] */ Int32* handle)
{
    VALIDATE_NOT_NULL(handle);

    *handle = mHandle;

    return NOERROR;
}

ECode CSensor::SetRange(
    /* [in] */ Float max,
    /* [in] */ Float res)
{
    mMaxRange = max;
    mResolution = res;

    return NOERROR;
}

ECode CSensor::SetLegacyType(
    /* [in] */ Int32 legacyType)
{
    mLegacyType = legacyType;

    return NOERROR;
}

ECode CSensor::GetLegacyType(
    /* [out] */ Int32* legacyType)
{
    VALIDATE_NOT_NULL(legacyType);

    *legacyType = mLegacyType;

    return NOERROR;
}

