
#ifndef __CCALENDAR_H__
#define __CCALENDAR_H__

#include "_CCalendar.h"

CarClass(CCalendar)
{
public:
    CARAPI Add(
        /* [in] */ Int32 field,
        /* [in] */ Int32 value);

    CARAPI After(
        /* [in] */ IInterface * pCalendar,
        /* [out] */ Boolean * pResult);

    CARAPI Before(
        /* [in] */ IInterface * pCalendar,
        /* [out] */ Boolean * pResult);

    CARAPI Clear();

    CARAPI Clear2(
        /* [in] */ Int32 field);

    CARAPI Get(
        /* [in] */ Int32 field,
        /* [out] */ Int32 * pValue);

    CARAPI GetActualMaximum(
        /* [in] */ Int32 field,
        /* [out] */ Int32 * pValue);

    CARAPI GetActualMinimum(
        /* [in] */ Int32 field,
        /* [out] */ Int32 * pValue);

    CARAPI GetFirstDayOfWeek(
        /* [out] */ Int32 * pFirstDayOfWeek);

    CARAPI GetGreatestMinimum(
        /* [in] */ Int32 field,
        /* [out] */ Int32 * pMinimum);

    CARAPI GetInstance(
        /* [out] */ ICalendar ** ppNewObj);

    CARAPI GetLeastMaximum(
        /* [in] */ Int32 field,
        /* [out] */ Int32 * pMaximum);

    CARAPI GetMaximum(
        /* [in] */ Int32 field,
        /* [out] */ Int32 * pMaximum);

    CARAPI GetMinimalDaysInFirstWeek(
        /* [out] */ Int32 * pMinimalDaysInFirstWeek);

    CARAPI GetMinimum(
        /* [in] */ Int32 field,
        /* [out] */ Int32 * pMinimum);

    CARAPI GetTime(
        /* [out] */ IDate ** ppNewObj);

    CARAPI GetTimeInMillis(
        /* [out] */ Int64 * pTime);

    CARAPI IsLenient(
        /* [out] */ Boolean * pLenient);

    CARAPI IsSet(
        /* [in] */ Int32 field,
        /* [out] */ Boolean * pSet);

    CARAPI Roll(
        /* [in] */ Int32 field,
        /* [in] */ Int32 value);

    CARAPI Roll2(
        /* [in] */ Int32 field,
        /* [in] */ Boolean increment);

    CARAPI Set(
        /* [in] */ Int32 field,
        /* [in] */ Int32 value);

    CARAPI Set2(
        /* [in] */ Int32 year,
        /* [in] */ Int32 month,
        /* [in] */ Int32 day);

    CARAPI Set3(
        /* [in] */ Int32 year,
        /* [in] */ Int32 month,
        /* [in] */ Int32 day,
        /* [in] */ Int32 hourOfDay,
        /* [in] */ Int32 minute);

    CARAPI Set4(
        /* [in] */ Int32 year,
        /* [in] */ Int32 month,
        /* [in] */ Int32 day,
        /* [in] */ Int32 hourOfDay,
        /* [in] */ Int32 minute,
        /* [in] */ Int32 second);

    CARAPI SetFirstDayOfWeek(
        /* [in] */ Int32 value);

    CARAPI SetLenient(
        /* [in] */ Boolean value);

    CARAPI SetMinimalDaysInFirstWeek(
        /* [in] */ Int32 value);

    CARAPI SetTime(
        /* [in] */ IDate * pDate);

    CARAPI SetTimeInMillis(
        /* [in] */ Int64 milliseconds);

    CARAPI CompareTo(
        /* [in] */ ICalendar * pAnotherCalendar,
        /* [out] */ Int32 * pResult);

private:
    // TODO: Add your private member variables here.
};

#endif // __CCALENDAR_H__
