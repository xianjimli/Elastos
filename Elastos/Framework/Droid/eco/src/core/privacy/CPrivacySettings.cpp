
#include "ext/frameworkdef.h"
#include "privacy/CPrivacySettings.h"


ECode CPrivacySettings::constructor()
{
    return NOERROR;
}

ECode CPrivacySettings::constructor(
    /* [in] */ Int32 id,
    /* [in] */ const String& capsuleName,
    /* [in] */ Int32 uid)
{
    mId = id;
    mCapsuleName = capsuleName;
    mUid = uid;

    mLocationGpsSetting = PrivacySettings_REAL;
    mLocationGpsLat = NULL;
    mLocationGpsLon = NULL;
    return NOERROR;
}

ECode CPrivacySettings::constructor(
    /* [in] */ Int32 id,
    /* [in] */ const String& capsuleName,
    /* [in] */ Int32 uid,
    /* [in] */ Byte locationGpsSetting,
    /* [in] */ const String& locationGpsLat,
    /* [in] */ const String& locationGpsLon)
{
    mId = id;
    mCapsuleName = capsuleName;
    mUid = uid;

    mLocationGpsSetting = locationGpsSetting;
    mLocationGpsLat = locationGpsLat;
    mLocationGpsLon = locationGpsLon;
    return NOERROR;
}

ECode CPrivacySettings::GetLocationGpsSetting(
    /* [out] */ Byte* setting)
{
    VALIDATE_NOT_NULL(setting);

    *setting = mLocationGpsSetting;
    return NOERROR;
}

ECode CPrivacySettings::GetLocationGpsLat(
    /* [out] */ String* latitude)
{
    VALIDATE_NOT_NULL(latitude);

    if (mLocationGpsSetting == PrivacySettings_EMPTY) {
        *latitude = "";
        return NOERROR;
    }
    if (mLocationGpsSetting == PrivacySettings_RANDOM) {
        *latitude = GetRandomLat();
        return NOERROR;
    }
    *latitude = mLocationGpsLat;
    return NOERROR;
}

ECode CPrivacySettings::SetLocationGpsLat(
    /* [in] */ const String& latitude)
{
    mLocationGpsLat = latitude;
    return NOERROR;
}

ECode CPrivacySettings::GetLocationGpsLon(
    /* [out] */ String* longitude)
{
    VALIDATE_NOT_NULL(longitude);

    if (mLocationGpsSetting == PrivacySettings_EMPTY) {
        *longitude = "";
        return NOERROR;
    }
    if (mLocationGpsSetting == PrivacySettings_RANDOM) {
        *longitude = GetRandomLon();
        return NOERROR;
    }
    *longitude = mLocationGpsLon;
    return NOERROR;
}

ECode CPrivacySettings::SetLocationGpsLon(
    /* [in] */ const String& longitude)
{
    mLocationGpsLon = longitude;
    return NOERROR;
}

ECode CPrivacySettings::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    source->ReadInt32(&mId);
    source->ReadString(&mCapsuleName);
    source->ReadInt32(&mUid);
    source->ReadByte(&mLocationGpsSetting);
    source->ReadString(&mLocationGpsLat);
    source->ReadString(&mLocationGpsLon);
    return NOERROR;
}

ECode CPrivacySettings::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteInt32(mId);
    dest->WriteString(mCapsuleName);
    dest->WriteInt32(mUid);
    dest->WriteByte(mLocationGpsSetting);
    dest->WriteString(mLocationGpsLat);
    dest->WriteString(mLocationGpsLon);
    return NOERROR;
}

String CPrivacySettings::GetRandomLat()
{
    // BigDecimal latitude;
    // double lat = Math.random() * 180;
    // if (lat > 90) latitude = new BigDecimal(lat - 90);
    // else latitude = new BigDecimal(-lat);
    // return latitude.setScale(6, BigDecimal.ROUND_HALF_UP) + "";
    return String("31.13");
}

String CPrivacySettings::GetRandomLon()
{
    // BigDecimal longitude;
    // double lon = Math.random() * 360;
    // if (lon > 180) longitude = new BigDecimal(lon - 180);
    // else longitude = new BigDecimal(-lon);
    // return longitude.setScale(6, BigDecimal.ROUND_HALF_UP) + "";
    return String("121.28");
}
