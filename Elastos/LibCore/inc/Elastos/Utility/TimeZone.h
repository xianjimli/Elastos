
#ifndef __TIMEZONE_H__
#define __TIMEZONE_H__

#include "Elastos.Utility_server.h"
#include "CTimeZone.h"

class TimeZone
{
public:
    static CARAPI GetAvailableIDs(
        /* [out, callee] */ ArrayOf<String> ** ppArray);

    static CARAPI GetAvailableIDsEx(
        /* [in] */ Int32 offsetMillis,
        /* [out, callee] */ ArrayOf<String> ** ppArray);

    static CARAPI GetDefault(
        /* [out] */ ITimeZone ** ppTimezone);

    static CARAPI GetTimeZone(
        /* [in] */ const String& id,
        /* [out] */ ITimeZone ** ppTimezone);

    static CARAPI SetDefault(
        /* [in] */ ITimeZone * pTimeZone);

    static AutoPtr<ITimeZone> GMT;
private:

    // TODO: Add your private member variables here.
};

#endif // __CTIMEZONE_H__
