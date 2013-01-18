
#ifndef __CTIMEZONEHELPER_H__
#define __CTIMEZONEHELPER_H__

#include "_CTimeZoneHelper.h"

CarClass(CTimeZoneHelper)
{
public:
    CARAPI GetAvailableIDs(
        /* [out, callee] */ ArrayOf<String> ** ppArray);

    CARAPI GetAvailableIDsEx(
        /* [in] */ Int32 offsetMillis,
        /* [out, callee] */ ArrayOf<String> ** ppArray);

    CARAPI GetDefault(
        /* [out] */ ITimeZone ** ppTimezone);

    CARAPI GetTimeZone(
        /* [in] */ const String& id,
        /* [out] */ ITimeZone ** ppTimezone);

    CARAPI SetDefault(
        /* [in] */ ITimeZone * pTimeZone);

private:
    // TODO: Add your private member variables here.
};

#endif // __CTIMEZONEHELPER_H__
