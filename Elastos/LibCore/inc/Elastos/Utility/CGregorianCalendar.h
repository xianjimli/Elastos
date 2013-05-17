
#ifndef __CGREGORIANCALENDER_H__
#define __CGREGORIANCALENDER_H__

#include "_CGregorianCalendar.h"
#include "Calendar.h"
#include "TimeZone.h"
#include "CTimeZone.h"
#include "CLocale.h"

using namespace Elastos;

CarClass(CGregorianCalendar), public Calendar
{
public:
    CGregorianCalendar();
    CGregorianCalendar(ITimeZone *tz, ILocale *locale);

public:
    CARAPI Add(
        /* [in] */ Int32 field,
        /* [in] */ Int32 value);

    CARAPI IsAfter(
        /* [in] */ IInterface * pCalendar,
        /* [out] */ Boolean * pResult);

    CARAPI IsBefore(
        /* [in] */ IInterface * pCalendar,
        /* [out] */ Boolean * pResult);

    CARAPI Clear();

    CARAPI GetTimeZone(
        /* [out] */ ITimeZone** tz);

    CARAPI SetTimeZone(
        /* [in] */ ITimeZone* tz);
    CARAPI ClearEx(
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

    CARAPI RollEx(
        /* [in] */ Int32 field,
        /* [in] */ Boolean increment);

    CARAPI Set(
        /* [in] */ Int32 field,
        /* [in] */ Int32 value);

    CARAPI SetEx(
        /* [in] */ Int32 year,
        /* [in] */ Int32 month,
        /* [in] */ Int32 day);

    CARAPI SetEx2(
        /* [in] */ Int32 year,
        /* [in] */ Int32 month,
        /* [in] */ Int32 day,
        /* [in] */ Int32 hourOfDay,
        /* [in] */ Int32 minute);

    CARAPI SetEx3(
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

    CARAPI SetGregorianChange(
        /* [in] */ IDate * pDate);

    CARAPI IsLeapYear(
        /* [in] */ Int32 year,
        /* [out] */ Boolean* isLeap);

    CARAPI GetGregorianChange(
        /* [out] */ IDate** ppDate);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ ILocale* locale);

    CARAPI constructor(
        /* [in] */ ITimeZone* timezone);

    CARAPI constructor(
        /* [in] */ ITimeZone* timezone,
        /* [in] */ ILocale* locale);

    CARAPI constructor(
    /* [in] */ Int64 milliseconds);

    CARAPI constructor(
    /* [in] */ Boolean ignored);




protected:
    CARAPI_(void) ComputeFields();

    CARAPI ComputeTime();

//    void Complete();

private:
    CARAPI GetOffset(
        /* [in] */ Int64 localTime,
        /* [out] */ Int32* offset);

    CARAPI_(Int32) GetOffset(
        /* [in] */ Int64 localTime);

    CARAPI_(Int32) DaysInMonth();

    CARAPI_(Int32) DaysInMonth(
        /* [in] */ Boolean leapYear,
        /* [in] */ Int32 month);

    CARAPI_(Int32) DaysInYear(
        /* [in] */ Int32 year);

    CARAPI_(Boolean) IsLeapYear(
        /* [in] */ Int32 year);

    CARAPI_(Int32) GetFirstDayOfWeek();

    CARAPI_(Int32) Mod(
        /* [in] */ Int32 value,
        /* [in] */ Int32 mod);

    CARAPI_(Int32) Mod7(
        /* [in] */ Int64 num1);

    CARAPI_(Int32) JulianError();

    CARAPI_(Boolean) IsLenient();

    CARAPI_(Int64) DaysFromBaseYear(
        /* [in] */ Int32 iyear);

    CARAPI_(Int32) DaysInYear(
        /* [in] */ Boolean leapYear,
        /* [in] */ Int32 month);

    CARAPI_(Int32) GetMinimalDaysInFirstWeek();

    CARAPI_(void) CachedFieldsCheckAndGet(
        /* [in] */ Int64 timeVal,
        /* [in] */ Int64 newTimeMillis,
        /* [in] */ Int64 newTimeMillisAdjusted,
        /* [in] */ Int32 millis,
        /* [in] */ Int32 zoneOffset);

    CARAPI_(void) FullFieldsCalc(
        /* [in] */ Int64 timeVal,
        /* [in] */ Int32 millis,
        /* [in] */ Int32 zoneOffset);

    CARAPI_(Int32) ComputeYearAndDay(
        /* [in] */ Int64 dayCount,
        /* [in] */ Int64 localTime);

public:
    static const Byte mDaysInMonth[];
    static const Int32 mDaysInYear[];


private:
    // TODO: Add your private member variables here.
    Boolean mIsCached;
    Int32 mChangeYear;
    Int64 mGregorianCutover;
    Int32 mLastYearSkew;
    Int32 mCurrentYearSkew;

    static const Int64 mDefaultGregorianCutover;
    static Int32 mMaximums[];
    static Int32 mMinimums[];
    static Int32 mLeastMaximums[];

    Int32 mJulianSkew;
    ArrayOf<Int32> *mCachedFields;
    Int64 mNextMidnightMillis;
    Int64 mLastMidnightMillis;
};

#endif // __CGREGORIANCALENDER_H__
