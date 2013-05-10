
#ifndef __CPRIVACYSETTINGS_H__
#define __CPRIVACYSETTINGS_H__

#include "_CPrivacySettings.h"

CarClass(CPrivacySettings)
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 id,
        /* [in] */ const String& capsuleName,
        /* [in] */ Int32 uid);

    CARAPI constructor(
        /* [in] */ Int32 id,
        /* [in] */ const String& capsuleName,
        /* [in] */ Int32 uid,
        /* [in] */ Byte locationGpsSetting,
        /* [in] */ const String& locationGpsLat,
        /* [in] */ const String& locationGpsLon);

    CARAPI GetLocationGpsSetting(
        /* [out] */ Byte* setting);

    CARAPI GetLocationGpsLat(
        /* [out] */ String* latitude);

    CARAPI SetLocationGpsLat(
        /* [in] */ const String& latitude);

    CARAPI GetLocationGpsLon(
        /* [out] */ String* longitude);

    CARAPI SetLocationGpsLon(
        /* [in] */ const String& longitude);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

private:
    CARAPI_(String) GetRandomLat();

    CARAPI_(String) GetRandomLon();

private:
    // Database entry ID
    Int32 mId;

    // Application identifiers
    String mCapsuleName;
    Int32 mUid;

    //
    // Privacy settings
    //

    Byte mLocationGpsSetting;
    String mLocationGpsLat;
    String mLocationGpsLon;
};

#endif // __CPRIVACYSETTINGS_H__
