
#ifndef __CDATE_H__
#define __CDATE_H__

#include "_CDate.h"


CarClass(CDate)
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int64 milliseconds);

    CARAPI IsAfter(
        /* [in] */ IDate* date,
        /* [out] */ Boolean* isAfter);

    CARAPI IsBefore(
        /* [in] */ IDate* date,
        /* [out] */ Boolean* isBefore);

    CARAPI CompareTo(
        /* [in] */ IDate* date,
        /* [out] */ Int32* result);

    CARAPI GetDate(
        /* [out] */ Int32* date);

    CARAPI GetDay(
        /* [out] */ Int32* day);

    CARAPI GetHours(
        /* [out] */ Int32* hours);

    CARAPI GetMinutes(
        /* [out] */ Int32* minutes);

    CARAPI GetMonth(
        /* [out] */ Int32* month);

    CARAPI GetSeconds(
        /* [out] */ Int32* seconds);

    CARAPI GetTime(
        /* [out] */ Int64* time);

    CARAPI GetTimezoneOffset(
        /* [out] */ Int32* timezoneOffset);

    CARAPI GetYear(
        /* [out] */ Int32* year);

    CARAPI SetDate(
        /* [in] */ Int32 day);

    CARAPI SetHours(
        /* [in] */ Int32 hour);

    CARAPI SetMinutes(
        /* [in] */ Int32 minute);

    CARAPI SetMonth(
        /* [in] */ Int32 month);

    CARAPI SetSeconds(
        /* [in] */ Int32 second);

    CARAPI SetTime(
        /* [in] */ Int64 milliseconds);

    CARAPI SetYear(
        /* [in] */ Int32 year);

    CARAPI ToGMTString(
        /* [out] */ String* gmtStr);

    CARAPI ToLocaleString(
        /* [out] */ String* localeStr);

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 year,
        /* [in] */ Int32 month,
        /* [in] */ Int32 day);

    CARAPI constructor(
        /* [in] */ Int32 year,
        /* [in] */ Int32 month,
        /* [in] */ Int32 day,
        /* [in] */ Int32 hour,
        /* [in] */ Int32 minute);

    CARAPI constructor(
        /* [in] */ Int32 year,
        /* [in] */ Int32 month,
        /* [in] */ Int32 day,
        /* [in] */ Int32 hour,
        /* [in] */ Int32 minute,
        /* [in] */ Int32 second);

    CARAPI constructor(
        /* [in] */ Int64 milliseconds);

    CARAPI constructor(
        /* [in] */ const String& string);

private:
    // TODO: Add your private member variables here.
    Int64 mMilliseconds;
};

#endif // __CDATE_H__
