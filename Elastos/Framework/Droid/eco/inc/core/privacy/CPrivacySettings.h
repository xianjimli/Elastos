
#ifndef __CPRIVACYSETTINGS_H__
#define __CPRIVACYSETTINGS_H__

#include "_CPrivacySettings.h"

CarClass(CPrivacySettings)
{
public:
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

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

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
