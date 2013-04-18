#ifndef __CALENDAR_H__
#define __CALENDAR_H__

#include "CTimeZone.h"

class Calendar {
protected:
    CARAPI constructor();
    CARAPI constructor(
        /* [in] */ ITimeZone* timezone,
        /* [in] */ ILocale* locale);

public:
    CARAPI constructor(
        /* [in] */ ITimeZone* timezone);


    virtual CARAPI SetTimeZone(
        /* [in] */ ITimeZone* timezone);

    virtual CARAPI Add(
        /* [in] */ Int32 field,
        /* [in] */ Int32 value) = 0;

    virtual CARAPI ComputeTime() = 0;



    CARAPI_(Boolean) After(
        /* [in */ IInterface* calendar);

    CARAPI_(Boolean) Before(
        /* [in */ IInterface* calendar);

    Int64 GetTimeInMillis();

    CARAPI_(void) Clear();

    CARAPI_(void) Clear(
        /* [in] */ Int32 field);

    CARAPI Get(
        /* [in] */ Int32 field,
        /* [out] */ Int32* pValue);

    CARAPI GetActualMaximum(
        /* [in] */ Int32 field,
        /* [out] */ Int32* pValue);

    CARAPI GetActualMinimum(
        /* [in] */ Int32 field,
        /* [out] */ Int32* pValue);

    CARAPI GetFirstDayOfWeek(
         /* [out] */ Int32* pDay);

    CARAPI GetMinimalDaysInFirstWeek(
         /* [out] */ Int32* pDay);

    CARAPI GetTime(
         /* [out] */ IDate** ppDate);

    virtual CARAPI GetTimeZone(
         /* [out] */ ITimeZone** ppZone);

    CARAPI IsLenient(
        /* [out] */ Boolean* isLenient);

    CARAPI IsSet(
        /* [in] */ Int32 field,
        /* [out] */ Boolean* isSet);

    virtual CARAPI GetMaximum(
        /* [in] */ Int32 field,
        /* [out] */ Int32* pValue) = 0;

    virtual CARAPI GetLeastMaximum(
        /* [in] */ Int32 field,
        /* [out] */ Int32* pValue) = 0;

    virtual CARAPI GetMinimum(
        /* [in] */ Int32 field,
        /* [out] */ Int32* pValue) = 0;

    virtual CARAPI GetGreatestMinimum(
        /* [in] */ Int32 field,
        /* [out] */ Int32* pValue) = 0;

    CARAPI Set(
        /* [in] */ Int32 field,
        /* [in] */ Int32 value);

    CARAPI Set(
        /* [in] */ Int32 year,
        /* [in] */ Int32 month,
        /* [in] */ Int32 day);

    CARAPI Set(
        /* [in] */ Int32 year,
        /* [in] */ Int32 month,
        /* [in] */ Int32 day,
        /* [in] */ Int32 hourOfDay,
        /* [in] */ Int32 minute);

    CARAPI Set(
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
        /* [in] */ IDate* date);

    CARAPI SetTimeInMillis(
        /* [in] */ Int64 milliseconds);

    CARAPI GetDisplayName(
        /* [in] */ Int32 field,
        /* [in] */ Int32 style,
        /* [in] */ ILocale* locale,
        /* [out] */ String* name);

    CARAPI GetDisplayNameArray(
        /* [in] */ Int32 field,
        /* [in] */ Int32 style,
        /* [in] */ ILocale* locale,
        /* [out, callee] */ ArrayOf<String>** array);

    CARAPI Roll(
        /* [in] */ Int32 field,
        /* [in] */ Int32 value);

public:
    static CARAPI GetAvailableLocales(
        /* [out, callee] */ ArrayOf<ILocale*>** ppLocales);

    static CARAPI GetInstance(ICalendar** calendar);
    static CARAPI GetInstance(ILocale* locale, ICalendar** calendar);
    static CARAPI GetInstance(ITimeZone* timezone, ICalendar** calendar);
    static CARAPI GetInstance(ITimeZone* timezone, ILocale* locale, ICalendar** calendar);

private:
    static CARAPI CheckStyle(Int32 style);

protected:
    CARAPI_(void) Complete();
    virtual CARAPI_(void) ComputeFields() = 0;

protected:
    ArrayOf<Int32>* mFields;
    ArrayOf<Boolean>* mIsSet;
    Boolean mAreFieldsSet;
    Boolean mIsTimeSet;

    Int64 mTime;

private:
    Boolean mLenient;
    ITimeZone* mZone;
    Int32 mFirstDayOfWeek;
    Int32 mMinimalDaysInFirstWeek;

public:
    Int32 mLastTimeFieldSet;
    Int32 mLastDateFieldSet;
};

#endif //__CCALENDAR_H__
