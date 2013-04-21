
#ifndef __CSENSOR_H__
#define __CSENSOR_H__

#include "_CSensor.h"

CarClass(CSensor)
{
public:
    friend class CSensorManagerListenerDelegate;
    friend class CSensorManagerLegacyListener;
    friend class CSensorManager;

public:
    CSensor();

    CARAPI GetName(
        /* [out] */ String* name);

    CARAPI GetVendor(
        /* [out] */ String* vendor);

    CARAPI GetType(
        /* [out] */ Int32* type);

    CARAPI GetVersion(
        /* [out] */ Int32* value);

    CARAPI GetMaximumRange(
        /* [out] */ Float* range);

    CARAPI GetResolution(
        /* [out] */ Float* resolution);

    CARAPI GetPower(
        /* [out] */ Float* power);

    CARAPI GetMinDelay(
        /* [out] */ Int32* minDelay);

private:
    CARAPI GetHandle(
        /* [out] */ Int32* handle);

    CARAPI SetRange(
        /* [in] */ Float max,
        /* [in] */ Float res);

    CARAPI SetLegacyType(
        /* [in] */ Int32 legacyType);

    CARAPI GetLegacyType(
        /* [out] */ Int32* legacyType);

private:
    /* Some of these fields are set only by the native bindings in
     * SensorManager.
     */
    String mName;
    String mVendor;
    Int32 mVersion;
    Int32 mHandle;
    Int32 mType;
    Float mMaxRange;
    Float mResolution;
    Float mPower;
    Int32 mMinDelay;
    Int32 mLegacyType;
};

#endif // __CSENSOR_H__
