#include "ICU.h"
#include "CGregorianCalendar.h"
#include <stdio.h>
ECode Calendar::constructor()
{
    return NOERROR;
}

ECode Calendar::constructor(
        /* [in] */ ITimeZone* timezone,
        /* [in] */ ILocale* locale)
{
    constructor(timezone);
//    LocaleData localeData = LocaleData.get(locale);
//    setFirstDayOfWeek(localeData.firstDayOfWeek.intValue());
//    setMinimalDaysInFirstWeek(localeData.minimalDaysInFirstWeek.intValue());
    return NOERROR;
}

ECode Calendar::constructor(
        /* [in] */ ITimeZone* timezone)
{
    mFields = ArrayOf<Int32>::Alloc(Calendar_FIELD_COUNT);
    mIsSet = ArrayOf<Boolean>::Alloc(Calendar_FIELD_COUNT);
    mAreFieldsSet = mIsTimeSet = FALSE;
    SetLenient(TRUE);
    SetTimeZone(timezone);
    return NOERROR;
}


Boolean Calendar::After(
    /* [in */ IInterface* calendar)
{
    return GetTimeInMillis() > ((Calendar *) calendar)->GetTimeInMillis();
}

Boolean Calendar::Before(
    /* [in */ IInterface* calendar)
{
    return GetTimeInMillis() < ((Calendar*) calendar)->GetTimeInMillis();
}

void Calendar::Clear()
{
    for (Int32 i = 0; i < Calendar_FIELD_COUNT; i++) {
        (*mFields)[i] = 0;
        (*mIsSet)[i] = FALSE;
    }
    mAreFieldsSet = mIsTimeSet = FALSE;
}

void Calendar::Clear(
    /* [in] */ Int32 field)
{
    (*mFields)[field] = 0;
    (*mIsSet)[field] = FALSE;
    mAreFieldsSet = mIsTimeSet = FALSE;
}

ECode Calendar::Get(
    /* [in] */ Int32 field,
    /* [out] */ Int32* pValue)
{
    Complete();
    *pValue = (*mFields)[field];
    return NOERROR;
}

ECode Calendar::GetActualMaximum(
    /* [in] */ Int32 field,
    /* [out] */ Int32* pValue)
{
    Int32 value, next;
    Int32 max, lMax;
    GetMaximum(field, &max);
    GetLeastMaximum(field, &lMax);
    if ( max == (next = lMax) ) {
        *pValue = next;
        return NOERROR;
    }

    Complete();
    Int64 orgTime = mTime;

    Set(field, next);
    do {
        value = next;
        Roll(field, true);
        Get(field, &next);
    } while (next > value);
    mTime = orgTime;
    mAreFieldsSet = FALSE;
    *pValue = value;
    return NOERROR;
}

ECode Calendar::GetActualMinimum(
    /* [in] */ Int32 field,
    /* [out] */ Int32* pValue)
{
    Int32 value, next;
    Int32 min, gMin;

    GetMinimum(field,&min);
    GetGreatestMinimum(field, &gMin);

    if ( min == ( next = gMin )) {
        *pValue = next;
        return NOERROR;
    }

    Complete();
    Int64 orgTime = mTime;

    Set(field, next);
    do {
        value = next;
        Roll(field, FALSE);
        Get(field, &next);
    } while (next < value);

    mTime = orgTime;
    mAreFieldsSet = FALSE;
    *pValue = value;
    return NOERROR;
}

ECode Calendar::GetAvailableLocales(
    /* [out, callee] */ ArrayOf<ILocale*>** ppLocales)
{
    return ICU::GetAvailableCalendarLocales(ppLocales);
}

ECode Calendar::GetFirstDayOfWeek(
     /* [out] */ Int32* pDay)
{
    *pDay = mFirstDayOfWeek;
    return NOERROR;
}

ECode Calendar::GetMinimalDaysInFirstWeek(
         /* [out] */ Int32* pDay)
{
    *pDay = mMinimalDaysInFirstWeek;
    return NOERROR;
}

ECode Calendar::GetTime(
     /* [out] */ IDate** ppDate)
{
    return CDate::New(GetTimeInMillis(), ppDate);
}

ECode Calendar::GetTimeZone(
     /* [out] */ ITimeZone** ppZone)
{
    *ppZone = mZone;
    return NOERROR;
}

ECode Calendar::IsLenient(
    /* [out] */ Boolean* isLenient)
{
    *isLenient = mLenient;
    return NOERROR;
}

ECode Calendar::IsSet(
    /* [in] */ Int32 field,
    /* [out] */ Boolean* isSet)
{
    *isSet = (*mIsSet)[field];
    return NOERROR;
}

ECode Calendar::GetInstance(
    /* [out] */ ICalendar** calendar)
{
    return CGregorianCalendar::New((IGregorianCalendar**)calendar);
}

ECode Calendar::GetInstance(
    /* [in] */ ILocale* locale,
    /* [out] */ ICalendar** calendar)
{
    return CGregorianCalendar::New(locale, (IGregorianCalendar**)calendar);
}

ECode Calendar::GetInstance(
    /* [in] */ ITimeZone* timezone,
    /* [out] */ ICalendar** calendar)
{
    return CGregorianCalendar::New(timezone, (IGregorianCalendar**)calendar);
}

ECode Calendar::GetInstance(
    /* [in] */ ITimeZone* timezone,
    /* [in] */ ILocale* locale,
    /* [out] */ ICalendar** calendar)
{
    return CGregorianCalendar::New(timezone, locale, (IGregorianCalendar**)calendar);
}

ECode Calendar::Set(
        /* [in] */ Int32 field,
        /* [in] */ Int32 value)
{
    (*mFields)[field] = value;
    (*mIsSet)[field] = FALSE;
    mAreFieldsSet = mIsTimeSet = FALSE;

    if (field > Calendar_MONTH && field < Calendar_AM_PM) {
        mLastDateFieldSet = field;
    }
    if (field == Calendar_HOUR || field == Calendar_HOUR_OF_DAY) {
        mLastTimeFieldSet = field;
    }
    if (field == Calendar_AM_PM) {
        mLastTimeFieldSet = Calendar_HOUR;
    }
    return NOERROR;
}

ECode Calendar::Set(
    /* [in] */ Int32 year,
    /* [in] */ Int32 month,
    /* [in] */ Int32 day)
{
    Set(Calendar_YEAR, year);
    Set(Calendar_MONTH, month);
    Set(Calendar_DATE, day);
    return NOERROR;
}

ECode Calendar::Set(
    /* [in] */ Int32 year,
    /* [in] */ Int32 month,
    /* [in] */ Int32 day,
    /* [in] */ Int32 hourOfDay,
    /* [in] */ Int32 minute)
{
    Set(year, month, day);
    Set(Calendar_HOUR_OF_DAY, hourOfDay);
    Set(Calendar_MINUTE, minute);
    return NOERROR;
}

ECode Calendar::Set(
    /* [in] */ Int32 year,
    /* [in] */ Int32 month,
    /* [in] */ Int32 day,
    /* [in] */ Int32 hourOfDay,
    /* [in] */ Int32 minute,
    /* [in] */ Int32 second)
{
    Set(year, month, day, hourOfDay, minute);
    Set(Calendar_SECOND, second);
    return NOERROR;
}

ECode Calendar::SetFirstDayOfWeek(
    /* [in] */ Int32 value)
{
    mFirstDayOfWeek = value;
    return NOERROR;
}

ECode Calendar::SetMinimalDaysInFirstWeek(
    /* [in] */ Int32 value)
{
    mMinimalDaysInFirstWeek = value;
    return NOERROR;
}

ECode Calendar::SetTime(
    /* [in] */ IDate* date)
{
    Int64 time;
    date->GetTime(&time);
    SetTimeInMillis(time);
    return NOERROR;
}

ECode Calendar::SetTimeInMillis(
    /* [in] */ Int64 milliseconds)
{
    if (!mIsTimeSet || !mAreFieldsSet || mTime != milliseconds) {
        mTime = milliseconds;
        mIsTimeSet = TRUE;
        mAreFieldsSet = FALSE;
        Complete();
    }
    return NOERROR;
}

ECode Calendar::GetDisplayName(
    /* [in] */ Int32 field,
    /* [in] */ Int32 style,
    /* [in] */ ILocale* locale,
    /* [out] */ String* name)
{
    if (style == Calendar_ALL_STYLES) {
        style = Calendar_SHORT;
    }
    ArrayOf<String> *array = NULL;
    GetDisplayNameArray(field, style, locale, &array);

    Int32 value;
    Get(field, &value);

    if(array != NULL) {
        *name = (*array)[value];
    } else {
        ;
    }
    return NOERROR;
}

ECode Calendar::GetDisplayNameArray(
    /* [in] */ Int32 field,
    /* [in] */ Int32 style,
    /* [in] */ ILocale* locale,
    /* [out, callee] */ ArrayOf<String>** array)
{
    if (field < 0 || field >= Calendar_FIELD_COUNT) {
        return E_INVALID_ARGUMENT;
    }

//    CheckStyle(style);
//    DateFormatSymbols dfs = DateFormatSymbols.getInstance(locale);
//    switch (field) {
//    case AM_PM:
//        return dfs.getAmPmStrings();
//    case DAY_OF_WEEK:
//        return (style == LONG) ? dfs.getWeekdays() : dfs.getShortWeekdays();
//    case ERA:
//        return dfs.getEras();
//    case MONTH:
//        return (style == LONG) ? dfs.getMonths() : dfs.getShortMonths();
//    }
    return NOERROR;
}

ECode Calendar::CheckStyle(
    /* [in] */ Int32 style)
{
    if (style != Calendar_ALL_STYLES && style != Calendar_SHORT && style != Calendar_LONG) {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}


ECode Calendar::Roll(
        /* [in] */ Int32 field,
        /* [in] */ Int32 value)
{
    Boolean increment = value >= 0;
    Int32 count = increment ? value : -value;
    for (Int32 i = 0; i < count; i++) {
        Roll(field, increment);
    }
    return NOERROR;
}

void Calendar::Complete()
{
    if (!mIsTimeSet) {
        ComputeTime();
        mIsTimeSet = TRUE;
    }
    if (!mAreFieldsSet) {
        ComputeFields();
        mAreFieldsSet = TRUE;
    }
}


ECode Calendar::SetLenient(
    /* [in] */ Boolean value)
{
    mLenient = value;
    return NOERROR;
}

ECode Calendar::SetTimeZone(
    /* [in] */ ITimeZone* timezone)
{
    mZone = timezone;
    Int32 offset;
    timezone->GetRawOffset(&offset);
    mAreFieldsSet = FALSE;
    return NOERROR;
}


Int64 Calendar::GetTimeInMillis()
{
    if (!mIsTimeSet) {
        ComputeTime();
        mIsTimeSet = TRUE;
    }
    return mTime;
}


