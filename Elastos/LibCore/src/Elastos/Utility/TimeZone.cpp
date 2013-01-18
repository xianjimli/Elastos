
#include "TimeZone.h"
#include "CSimpleTimeZone.h"

static AutoPtr<ITimeZone> CreateSimpleZone()
{
    AutoPtr<ISimpleTimeZone> tz;
    CSimpleTimeZone::NewByFriend(12, String("GMT"), (CSimpleTimeZone **) &tz);
    tz->AddRef();
    return (ITimeZone *) tz;
}

AutoPtr<ITimeZone> TimeZone::GMT = CreateSimpleZone();

ECode TimeZone::GetAvailableIDs(
    /* [out, callee] */ ArrayOf<String> ** ppArray)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode TimeZone::GetAvailableIDsEx(
    /* [in] */ Int32 offsetMillis,
    /* [out, callee] */ ArrayOf<String> ** ppArray)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode TimeZone::GetDefault(
    /* [out] */ ITimeZone ** ppTimezone)
{
    // TODO: Add your code here
    *ppTimezone = (ITimeZone *) GMT;
    return NOERROR;
}

ECode TimeZone::GetTimeZone(
    /* [in] */ const String& id,
    /* [out] */ ITimeZone ** ppTimezone)
{
    // TODO: Add your code here
//    TimeZone zone = ZoneInfoDB.getTimeZone(id);
//    if (zone != null) {
//        return zone;
//    }
//    if (zone == null && id.length() > 3 && id.startsWith("GMT")) {
//        zone = getCustomTimeZone(id);
//    }
//    if (zone == null) {
//        zone = (TimeZone) GMT.clone();
//    }
//    return zone;
}

ECode TimeZone::SetDefault(
    /* [in] */ ITimeZone * pTimeZone)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

