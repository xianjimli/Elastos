
#include "privacy/CPrivacySettings.h"

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

    mLocationGpsSetting = PrivacySettings_REAL;
    mLocationGpsLat = locationGpsLat;
    mLocationGpsLon = locationGpsLon;
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
