
#ifndef __CDATE_H__
#define __CDATE_H__

#include "_CDate.h"

CarClass(CDate)
{
public:
    CARAPI IsAfter(
        /* [in] */ IDate * pDate,
        /* [out] */ Boolean * pIsAfter);

    CARAPI IsBefore(
        /* [in] */ IDate * pDate,
        /* [out] */ Boolean * pIsBefore);

    CARAPI CompareTo(
        /* [in] */ IDate * pDate,
        /* [out] */ Int32 * pResult);

    CARAPI GetDate(
        /* [out] */ Int32 * pDate);

    CARAPI GetDay(
        /* [out] */ Int32 * pDay);

    CARAPI GetHours(
        /* [out] */ Int32 * pHours);

    CARAPI GetMinutes(
        /* [out] */ Int32 * pMinutes);

    CARAPI GetMonth(
        /* [out] */ Int32 * pMonth);

    CARAPI GetSeconds(
        /* [out] */ Int32 * pSeconds);

    CARAPI GetTime(
        /* [out] */ Int64 * pTime);

    CARAPI GetTimezoneOffset(
        /* [out] */ Int32 * pTimezoneOffset);

    CARAPI GetYear(
        /* [out] */ Int32 * pYear);

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
        /* [out] */ String * pGmtStr);

    CARAPI ToLocaleString(
        /* [out] */ String * pLocaleStr);

    CARAPI ToString(
        /* [out] */ String * pStr);

private:
    // TODO: Add your private member variables here.
};

#endif // __CDATE_H__
