
#ifndef __TIMEZONE_H__
#define __TIMEZONE_H__

#include "Elastos.Utility_server.h"
#include "CTimeZone.h"

class TimeZone
{
public:
    TimeZone();

public:
    CARAPI GetDisplayName(
        /* [out] */ String * pName);

    CARAPI GetDisplayNameEx(
        /* [in] */ ILocale * pLocale,
        /* [out] */ String * pName);

    CARAPI GetDisplayNameEx2(
        /* [in] */ Boolean daylightTime,
        /* [in] */ Int32 style,
        /* [out] */ String * pName);

    CARAPI GetDisplayNameEx3(
        /* [in] */ Boolean daylightTime,
        /* [in] */ Int32 style,
        /* [in] */ ILocale * pLocale,
        /* [out] */ String * pName);

    CARAPI GetID(
        /* [out] */ String * pId);

    CARAPI GetDSTSavings(
        /* [out] */ Int32 * pSavings);

    CARAPI GetOffset(
        /* [in] */ Int64 time,
        /* [out] */ Int32 * pOffset);

    CARAPI HasSameRules(
        /* [in] */ ITimeZone * pTimeZone,
        /* [out] */ Boolean * pHasRules);

    CARAPI SetID(
        /* [in] */ const String& id);

    virtual CARAPI GetOffsetEx(
        /* [in] */ Int32 era,
        /* [in] */ Int32 year,
        /* [in] */ Int32 month,
        /* [in] */ Int32 day,
        /* [in] */ Int32 dayOfWeek,
        /* [in] */ Int32 timeOfDayMillis,
        /* [out] */ Int32 * pOffset) = 0;

    virtual CARAPI GetRawOffset(
        /* [out] */ Int32 * pRawOffset) = 0;

    virtual CARAPI InDaylightTime(
        /* [in] */ IDate * pTime,
        /* [out] */ Boolean * pIsIn) = 0;

    virtual CARAPI SetRawOffset(
        /* [in] */ Int32 offsetMillis) = 0;

    virtual CARAPI UseDaylightTime(
        /* [out] */ Boolean * pIsUsed) = 0;

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

    static ITimeZone* mDefaultTimeZone;

private:
    static ITimeZone* GetCustomTimeZone(
        /* [in] */ const String& id);

    static String FormatTimeZoneName(
        /* [in] */ const String& name,
        /* [in] */ Int32 offset);

    static Int32 ParseNumber(
        /* [in] */ const String& string,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 position);

private:
    void AppendNumber(
        /* [out] */ StringBuffer* buffer,
        /* [in] */ Int32 count,
        /* [in] */ Int32 value);

private:
    String mID;

    // TODO: Add your private member variables here.
};

#endif // __CTIMEZONE_H__
