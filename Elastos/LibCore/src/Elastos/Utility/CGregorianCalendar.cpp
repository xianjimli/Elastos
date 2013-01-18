
#include "CGregorianCalendar.h"
#include <elastos/System.h>
#include <stdio.h>

const Byte CGregorianCalendar::mDaysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31,
            30, 31, 30, 31 };

const Int32 CGregorianCalendar::mDaysInYear[] = { 0, 31, 59, 90, 120, 151, 181,
            212, 243, 273, 304, 334 };

const Int64 CGregorianCalendar::mDefaultGregorianCutover = -12219292800000l;

Int32 CGregorianCalendar::mMaximums[] = { 1, 292278994, 11, 53, 6, 31,
            366, 7, 6, 1, 11, 23, 59, 59, 999, 14 * 3600 * 1000, 7200000 };

Int32 CGregorianCalendar::mMinimums[] = { 0, 1, 0, 1, 0, 1, 1, 1, 1, 0,
            0, 0, 0, 0, 0, -13 * 3600 * 1000, 0 };

Int32 CGregorianCalendar::mLeastMaximums[] = { 1, 292269054, 11, 50, 3,
            28, 355, 7, 3, 1, 11, 23, 59, 59, 999, 50400000, 1200000 };


CGregorianCalendar::CGregorianCalendar()
{
    mChangeYear = 1582;
    mGregorianCutover = mDefaultGregorianCutover;
    mLastYearSkew = 0;
    mCurrentYearSkew = 10;
    mJulianSkew = ((mChangeYear - 2000) / 400)
            + JulianError() - ((mChangeYear - 2000) / 100);
    mCachedFields = ArrayOf<Int32>::Alloc(10);
    mNextMidnightMillis = 0L;
    mLastMidnightMillis = 0L;
}

CGregorianCalendar::CGregorianCalendar(ITimeZone *tz,
    ILocale *locale)
{
    mChangeYear = 1582;
    mGregorianCutover = mDefaultGregorianCutover;
    mLastYearSkew = 0;
    mCurrentYearSkew = 10;
}

ECode CGregorianCalendar::Add(
    /* [in] */ Int32 field,
    /* [in] */ Int32 value)
{
    // TODO: Add your code here
//    return NOERROR;
    if (value == 0) {
        return E_INVALID_ARGUMENT;
    }
    if (field < 0 || field >= Calendar_ZONE_OFFSET) {
        return E_INVALID_ARGUMENT;;
    }

    mIsCached = FALSE;

    if (field == Calendar_ERA) {
        Complete();
        if ((*mFields)[Calendar_ERA] == Calendar_AD) {
            if (value >= 0) {
                return NOERROR;
            }
            Set(Calendar_ERA, Calendar_BC);
        } else {
            if (value <= 0) {
                return NOERROR;
            }
            Set(Calendar_ERA, Calendar_AD);
        }
        Complete();
        return NOERROR;
    }

    if (field == Calendar_YEAR || field == Calendar_MONTH) {
        Complete();
        if (field == Calendar_MONTH) {
            Int32 month = (*mFields)[Calendar_MONTH] + value;
            if (month < 0) {
                value = (month - 11) / 12;
                month = 12 + (month % 12);
            } else {
                value = month / 12;
            }
            Set(Calendar_MONTH, month % 12);
        }
        Set(Calendar_YEAR, (*mFields)[Calendar_YEAR] + value);

        Int32 days = DaysInMonth(IsLeapYear((*mFields)[Calendar_YEAR]), (*mFields)[Calendar_MONTH]);
        if ((*mFields)[Calendar_DATE] > days) {
            Set(Calendar_DATE, days);
        }
        Complete();
        return NOERROR;
    }

    Int64 multiplier = 0;
    Int64 timeMillis;
    GetTimeInMillis(&timeMillis); // Update the time

    switch (field) {
        case Calendar_MILLISECOND:
            mTime += value;
            break;
        case Calendar_SECOND:
            mTime += value * 1000L;
            break;
        case Calendar_MINUTE:
            mTime += value * 60000L;
            break;
        case Calendar_HOUR:
        case Calendar_HOUR_OF_DAY:
            mTime += value * 3600000L;
            break;
        case Calendar_AM_PM:
            multiplier = 43200000L;
            break;
        case Calendar_DATE:
        case Calendar_DAY_OF_YEAR:
        case Calendar_DAY_OF_WEEK:
            multiplier = 86400000L;
            break;
        case Calendar_WEEK_OF_YEAR:
        case Calendar_WEEK_OF_MONTH:
        case Calendar_DAY_OF_WEEK_IN_MONTH:
            multiplier = 604800000L;
            break;
    }
    if (multiplier > 0) {
        ITimeZone  *timezone;
        GetTimeZone((ITimeZone**)&timezone);
        Int32 rawOffset;
        timezone->GetRawOffsetVir(&rawOffset);

        Int32 zoneOffset = rawOffset;
        Int32 offset;
        GetOffset(mTime + zoneOffset, &offset);

        mTime += value * multiplier;
        Int32 newOffset;
        GetOffset(mTime + zoneOffset, &newOffset);
        // Adjust for moving over a DST boundary
        if (newOffset != offset) {
            mTime += offset - newOffset;
        }
    }
    mAreFieldsSet = FALSE;
    Complete();
    return NOERROR;
}

ECode CGregorianCalendar::IsAfter(
    /* [in] */ IInterface * pCalendar,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CGregorianCalendar::IsBefore(
    /* [in] */ IInterface * pCalendar,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    *pResult = Before(pCalendar);
    return NOERROR;
}

ECode CGregorianCalendar::Clear()
{
    // TODO: Add your code here
    Calendar::Clear();
    return NOERROR;
}

ECode CGregorianCalendar::ClearEx(
    /* [in] */ Int32 field)
{
    // TODO: Add your code here
    Calendar::Clear(field);
    return NOERROR;
}

ECode CGregorianCalendar::Get(
    /* [in] */ Int32 field,
    /* [out] */ Int32 * pValue)
{
    // TODO: Add your code here
    return Calendar::Get(field, pValue);
}

ECode CGregorianCalendar::GetActualMaximum(
    /* [in] */ Int32 field,
    /* [out] */ Int32 * pValue)
{
    // TODO: Add your code here
    Int32 value;
    if ((value = mMaximums[field]) == mLeastMaximums[field]) {
        return value;
    }

    switch (field) {
        case Calendar_WEEK_OF_YEAR:
        case Calendar_WEEK_OF_MONTH:
            mIsCached = FALSE;
            break;
    }

    Complete();
    Int64 orgTime = mTime;
    Int32 result = 0;
    switch (field) {
        case Calendar_WEEK_OF_YEAR:
            Set(Calendar_DATE, 31);
            Set(Calendar_MONTH, Calendar_DECEMBER);
            Get(Calendar_WEEK_OF_YEAR, &result);
            if (result == 1) {
                Set(Calendar_DATE, 31 - 7);
                Get(Calendar_WEEK_OF_YEAR, &result);
            }
            mAreFieldsSet = false;
            break;
        case Calendar_WEEK_OF_MONTH:
            Set(Calendar_DATE, DaysInMonth());
            Get(Calendar_WEEK_OF_MONTH, &result);
            mAreFieldsSet = FALSE;
            break;
        case Calendar_DATE:
            return DaysInMonth();
        case Calendar_DAY_OF_YEAR:
            return DaysInYear((*mFields)[Calendar_YEAR]);
        case Calendar_DAY_OF_WEEK_IN_MONTH:
            Int32 rtmp;
            Get(Calendar_DAY_OF_WEEK_IN_MONTH, &result);
            Get(Calendar_DATE, &rtmp);
            result += ((DaysInMonth()-rtmp) / 7);
        case Calendar_YEAR:
            //GregorianCalendar clone = (GregorianCalendar) clone();
            CGregorianCalendar* clone;;
            Int32 ret;
            Get(Calendar_ERA, &ret);
            if (ret == Calendar_AD) {
                #define LONGMAXVALUE 0x123
                #define LONGMINVALUE 0x123
                clone->SetTimeInMillis(LONGMAXVALUE);
            } else {
                clone->SetTimeInMillis(LONGMINVALUE);
            }
            clone->Get(Calendar_YEAR, &result);
            Int32 hretmp;
            Get(Calendar_YEAR, &hretmp);
            clone->Set(Calendar_YEAR, hretmp);
//            if (clone->before(this)) {
//                result--;
//            }
            break;
        case Calendar_DST_OFFSET:
            GetMaximum(Calendar_DST_OFFSET, &result);
            break;
    }
    mTime = orgTime;
    *pValue = result;
    return NOERROR;
}

ECode CGregorianCalendar::GetActualMinimum(
    /* [in] */ Int32 field,
    /* [out] */ Int32 * pValue)
{
    // TODO: Add your code here
    return GetMinimum(field, pValue);
}

ECode CGregorianCalendar::GetFirstDayOfWeek(
    /* [out] */ Int32 * pFirstDayOfWeek)
{
    // TODO: Add your code here
    *pFirstDayOfWeek = GetFirstDayOfWeek();
    return NOERROR;
}

Int32 CGregorianCalendar::GetFirstDayOfWeek()
{
    // TODO: Add your code here
    Int32 firstWeek;
    Calendar::GetFirstDayOfWeek(&firstWeek);
    return firstWeek;
}

ECode CGregorianCalendar::GetGreatestMinimum(
    /* [in] */ Int32 field,
    /* [out] */ Int32 * pMinimum)
{
    // TODO: Add your code here
    *pMinimum = mMinimums[field];
    return NOERROR;
}

ECode CGregorianCalendar::GetLeastMaximum(
    /* [in] */ Int32 field,
    /* [out] */ Int32 * pMaximum)
{
    // TODO: Add your code here
    if (mGregorianCutover != mDefaultGregorianCutover
            && field == Calendar_WEEK_OF_YEAR) {
        Int64 currentTimeInMillis = mTime;
        SetTimeInMillis(mGregorianCutover);
        Int32 actual;
        GetActualMaximum(field, &actual);
        SetTimeInMillis(currentTimeInMillis);
        *pMaximum = actual;
        return NOERROR;
    }

    *pMaximum = mLeastMaximums[field];
    return NOERROR;
}

ECode CGregorianCalendar::GetMaximum(
    /* [in] */ Int32 field,
    /* [out] */ Int32 * pMaximum)
{
    // TODO: Add your code here
    *pMaximum = mMaximums[field];
    return NOERROR;
}

ECode CGregorianCalendar::GetMinimalDaysInFirstWeek(
    /* [out] */ Int32 * pMinimalDaysInFirstWeek)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

Int32 CGregorianCalendar::GetMinimalDaysInFirstWeek()
{
    // TODO: Add your code here
    Int32 day;
    Calendar::GetMinimalDaysInFirstWeek(&day);
    return day;
}

ECode CGregorianCalendar::GetMinimum(
    /* [in] */ Int32 field,
    /* [out] */ Int32 * pMinimum)
{
    // TODO: Add your code here
    *pMinimum = mMinimums[field];
    return NOERROR;
}

ECode CGregorianCalendar::GetTime(
    /* [out] */ IDate ** ppNewObj)
{
    // TODO: Add your code here
    return Calendar::GetTime(ppNewObj);
}

ECode CGregorianCalendar::GetTimeInMillis(
    /* [out] */ Int64 * pTime)
{
    // TODO: Add your code here
    *pTime = Calendar::GetTimeInMillis();
    return NOERROR;
}

ECode CGregorianCalendar::IsLenient(
    /* [out] */ Boolean * pLenient)
{
    // TODO: Add your code here
    return Calendar::IsLenient(pLenient);
}

Boolean CGregorianCalendar::IsLenient()
{
    // TODO: Add your code here
    Boolean lenient;
    Calendar::IsLenient(&lenient);
    return lenient;
}

ECode CGregorianCalendar::IsSet(
    /* [in] */ Int32 field,
    /* [out] */ Boolean * pSet)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CGregorianCalendar::Roll(
    /* [in] */ Int32 field,
    /* [in] */ Int32 value)
{
    // TODO: Add your code here
    if (value == 0) {
        return E_INVALID_ARGUMENT;
    }
    if (field < 0 || field >= Calendar_ZONE_OFFSET) {
        return E_INVALID_ARGUMENT;
    }

    mIsCached = FALSE;

    Complete();
    Int32 days, day, mod, maxWeeks, newWeek;
    Int32 max = -1;
    switch (field) {
    case Calendar_YEAR:
        max = mMaximums[field];
        break;
    case Calendar_WEEK_OF_YEAR:
        days = DaysInYear((*mFields)[Calendar_YEAR]);
        day = Calendar_DAY_OF_YEAR;
        mod = Mod7((*mFields)[Calendar_DAY_OF_WEEK] - (*mFields)[day]
                - (GetFirstDayOfWeek() - 1));
        maxWeeks = (days - 1 + mod) / 7 + 1;
        newWeek = Mod((*mFields)[field] - 1 + value, maxWeeks) + 1;
        if (newWeek == maxWeeks) {
            Int32 addDays = (newWeek - (*mFields)[field]) * 7;
            if ((*mFields)[day] > addDays && (*mFields)[day] + addDays > days) {
                Set(field, 1);
            } else {
                Set(field, newWeek - 1);
            }
        } else if (newWeek == 1) {
            int week = ((*mFields)[day] - (((*mFields)[day] - 1) / 7 * 7) - 1 + mod) / 7 + 1;
            if (week > 1) {
                Set(field, 1);
            } else {
                Set(field, newWeek);
            }
        } else {
            Set(field, newWeek);
        }
        break;
    case Calendar_WEEK_OF_MONTH:
        days = DaysInMonth();
        day = Calendar_DATE;
        mod = Mod7((*mFields)[Calendar_DAY_OF_WEEK] - (*mFields)[day]
                - (GetFirstDayOfWeek() - 1));
        maxWeeks = (days - 1 + mod) / 7 + 1;
        newWeek = Mod((*mFields)[field] - 1 + value, maxWeeks) + 1;
        if (newWeek == maxWeeks) {
            if ((*mFields)[day] + (newWeek - (*mFields)[field]) * 7 > days) {
                Set(day, days);
            } else {
                Set(field, newWeek);
            }
        } else if (newWeek == 1) {
            Int32 week = ((*mFields)[day] - (((*mFields)[day] - 1) / 7 * 7) - 1 + mod) / 7 + 1;
            if (week > 1) {
                Set(day, 1);
            } else {
                Set(field, newWeek);
            }
        } else {
            Set(field, newWeek);
        }
        break;
    case Calendar_DATE:
        max = DaysInMonth();
        break;
    case Calendar_DAY_OF_YEAR:
        max = DaysInYear((*mFields)[Calendar_YEAR]);
        break;
    case Calendar_DAY_OF_WEEK:
        max = mMaximums[field];
        mLastDateFieldSet = Calendar_WEEK_OF_MONTH;
        break;
    case Calendar_DAY_OF_WEEK_IN_MONTH:
        max = ((*mFields)[Calendar_DATE] + ((DaysInMonth() - (*mFields)[Calendar_DATE]) / 7 * 7) - 1) / 7 + 1;
        break;

    case Calendar_ERA:
    case Calendar_MONTH:
    case Calendar_AM_PM:
    case Calendar_HOUR:
    case Calendar_HOUR_OF_DAY:
    case Calendar_MINUTE:
    case Calendar_SECOND:
    case Calendar_MILLISECOND:
        Set(field, Mod((*mFields)[field] + value, mMaximums[field] + 1));
        if (field == Calendar_MONTH && (*mFields)[Calendar_DATE] > DaysInMonth()) {
            Set(Calendar_DATE, DaysInMonth());
        } else if (field == Calendar_AM_PM) {
            mLastTimeFieldSet = Calendar_HOUR;
        }
        break;
    }
    if (max != -1) {
        Set(field, Mod((*mFields)[field] - 1 + value, max) + 1);
    }
    Complete();
    return NOERROR;
}

ECode CGregorianCalendar::RollEx(
    /* [in] */ Int32 field,
    /* [in] */ Boolean increment)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CGregorianCalendar::Set(
    /* [in] */ Int32 field,
    /* [in] */ Int32 value)
{
    // TODO: Add your code here
    return Calendar::Set(field, value);
}

ECode CGregorianCalendar::SetEx(
    /* [in] */ Int32 year,
    /* [in] */ Int32 month,
    /* [in] */ Int32 day)
{
    // TODO: Add your code here
    return Calendar::Set(year, month, day);
}

ECode CGregorianCalendar::SetEx2(
    /* [in] */ Int32 year,
    /* [in] */ Int32 month,
    /* [in] */ Int32 day,
    /* [in] */ Int32 hourOfDay,
    /* [in] */ Int32 minute)
{
    // TODO: Add your code here
    return Calendar::Set(year, year, day, hourOfDay, minute);
}

ECode CGregorianCalendar::SetEx3(
    /* [in] */ Int32 year,
    /* [in] */ Int32 month,
    /* [in] */ Int32 day,
    /* [in] */ Int32 hourOfDay,
    /* [in] */ Int32 minute,
    /* [in] */ Int32 second)
{
    // TODO: Add your code here
    return Calendar::Set(year, year, day, hourOfDay, minute, second);
}

ECode CGregorianCalendar::SetFirstDayOfWeek(
    /* [in] */ Int32 value)
{
    // TODO: Add your code here
    Calendar::SetFirstDayOfWeek(value);
    mIsCached = FALSE;
    return NOERROR;
}

ECode CGregorianCalendar::SetLenient(
    /* [in] */ Boolean value)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CGregorianCalendar::SetMinimalDaysInFirstWeek(
    /* [in] */ Int32 value)
{
    Calendar::SetMinimalDaysInFirstWeek(value);
    mIsCached = FALSE;
    return NOERROR;
}

ECode CGregorianCalendar::SetTime(
    /* [in] */ IDate * pDate)
{
    // TODO: Add your code here
    return Calendar::SetTime(pDate);
}

ECode CGregorianCalendar::SetTimeInMillis(
    /* [in] */ Int64 milliseconds)
{
    // TODO: Add your code here
    Calendar::SetTimeInMillis(milliseconds);
    return NOERROR;
}

ECode CGregorianCalendar::CompareTo(
    /* [in] */ ICalendar * pAnotherCalendar,
    /* [out] */ Int32 * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CGregorianCalendar::SetGregorianChange(
    /* [in] */ IDate * pDate)
{
    // TODO: Add your code here
    pDate->GetTime(&mGregorianCutover);
    AutoPtr<IGregorianCalendar> cal;
    CGregorianCalendar::New((ITimeZone *) TimeZone::GMT, (IGregorianCalendar **) &cal);

    cal->SetTime(pDate);
    cal->Get(Calendar_YEAR, &mChangeYear);
    Int32 value;
    cal->Get(Calendar_ERA, &value);
    if (value == Calendar_BC) {
        mChangeYear = 1 - mChangeYear;
    }
    mJulianSkew = ((mChangeYear - 2000) / 400) + JulianError()
            - ((mChangeYear - 2000) / 100);

    mIsCached = FALSE;
    Int32 dayOfYear;
    cal->Get(Calendar_DAY_OF_YEAR, &dayOfYear);
    if (dayOfYear < mJulianSkew) {
        mCurrentYearSkew = dayOfYear-1;
        mLastYearSkew = mJulianSkew - dayOfYear + 1;
    } else {
        mLastYearSkew = 0;
        mCurrentYearSkew = mJulianSkew;
    }
    mIsCached = FALSE;
    return NOERROR;
}

ECode CGregorianCalendar::ComputeTime()
{
    if (!IsLenient()) {
        if ((*mIsSet)[Calendar_HOUR_OF_DAY]) {
            if ((*mFields)[Calendar_HOUR_OF_DAY] < 0 || (*mFields)[Calendar_HOUR_OF_DAY] > 23) {
                return E_INVALID_ARGUMENT;
            }
        } else if ((*mIsSet)[Calendar_HOUR] && ((*mFields)[Calendar_HOUR] < 0 || (*mFields)[Calendar_HOUR] > 11)) {
            return E_INVALID_ARGUMENT;
        }
        if ((*mIsSet)[Calendar_MINUTE] && ((*mFields)[Calendar_MINUTE] < 0 || (*mFields)[Calendar_MINUTE] > 59)) {
            return E_INVALID_ARGUMENT;
        }
        if ((*mIsSet)[Calendar_SECOND] && ((*mFields)[Calendar_SECOND] < 0 || (*mFields)[Calendar_SECOND] > 59)) {
            return E_INVALID_ARGUMENT;
        }
        if ((*mIsSet)[Calendar_MILLISECOND]
                && ((*mFields)[Calendar_MILLISECOND] < 0 || (*mFields)[Calendar_MILLISECOND] > 999)) {
            return E_INVALID_ARGUMENT;
        }
        if ((*mIsSet)[Calendar_WEEK_OF_YEAR]
                && ((*mFields)[Calendar_WEEK_OF_YEAR] < 1 || (*mFields)[Calendar_WEEK_OF_YEAR] > 53)) {
            return E_INVALID_ARGUMENT;
        }
        if ((*mIsSet)[Calendar_DAY_OF_WEEK]
                && ((*mFields)[Calendar_DAY_OF_WEEK] < 1 || (*mFields)[Calendar_DAY_OF_WEEK] > 7)) {
            return E_INVALID_ARGUMENT;
        }
        if ((*mIsSet)[Calendar_DAY_OF_WEEK_IN_MONTH]
                && ((*mFields)[Calendar_DAY_OF_WEEK_IN_MONTH] < 1 || (*mFields)[Calendar_DAY_OF_WEEK_IN_MONTH] > 6)) {
            return E_INVALID_ARGUMENT;
        }
        if ((*mIsSet)[Calendar_WEEK_OF_MONTH]
                && ((*mFields)[Calendar_WEEK_OF_MONTH] < 1 || (*mFields)[Calendar_WEEK_OF_MONTH] > 6)) {
            return E_INVALID_ARGUMENT;
        }
        if ((*mIsSet)[Calendar_AM_PM] && (*mFields)[Calendar_AM_PM] != Calendar_AM && (*mFields)[Calendar_AM_PM] != Calendar_PM) {
            return E_INVALID_ARGUMENT;
        }
        if ((*mIsSet)[Calendar_HOUR] && ((*mFields)[Calendar_HOUR] < 0 || (*mFields)[Calendar_HOUR] > 11)) {
            return E_INVALID_ARGUMENT;
        }
        if ((*mIsSet)[Calendar_YEAR]) {
            if ((*mIsSet)[Calendar_ERA] && (*mFields)[Calendar_ERA] == Calendar_BC
                    && ((*mFields)[Calendar_YEAR] < 1 || (*mFields)[Calendar_YEAR] > 292269054)) {
                return E_INVALID_ARGUMENT;
            } else if ((*mFields)[Calendar_YEAR] < 1 || (*mFields)[Calendar_YEAR] > 292278994) {
                return E_INVALID_ARGUMENT;
            }
        }
        if ((*mIsSet)[Calendar_MONTH] && ((*mFields)[Calendar_MONTH] < 0 || (*mFields)[Calendar_MONTH] > 11)) {
            return E_INVALID_ARGUMENT;
        }
    }

    Int64 timeVal;
    Int64 hour = 0;
    if ((*mIsSet)[Calendar_HOUR_OF_DAY] && mLastTimeFieldSet != Calendar_HOUR) {
        hour = (*mFields)[Calendar_HOUR_OF_DAY];
    } else if ((*mIsSet)[Calendar_HOUR]) {
        hour = ((*mFields)[Calendar_AM_PM] * 12) + (*mFields)[Calendar_HOUR];
    }
    timeVal = hour * 3600000;

    if ((*mIsSet)[Calendar_MINUTE]) {
        timeVal += ((Int64) (*mFields)[Calendar_MINUTE]) * 60000;
    }
    if ((*mIsSet)[Calendar_SECOND]) {
        timeVal += ((Int64) (*mFields)[Calendar_SECOND]) * 1000;
    }
    if ((*mIsSet)[Calendar_MILLISECOND]) {
        timeVal += (*mFields)[Calendar_MILLISECOND];
    }

    Int64 days;
    Int32 year = (*mIsSet)[Calendar_YEAR] ? (*mFields)[Calendar_YEAR] : 1970;
    if ((*mIsSet)[Calendar_ERA]) {
        // Always test for valid ERA, even if the Calendar is lenient
        if ((*mFields)[Calendar_ERA] != Calendar_BC && (*mFields)[Calendar_ERA] != Calendar_AD) {
            return E_INVALID_ARGUMENT;
        }
        if ((*mFields)[Calendar_ERA] == Calendar_BC) {
            year = 1 - year;
        }
    }

    Boolean weekMonthSet = (*mIsSet)[Calendar_WEEK_OF_MONTH]
            || (*mIsSet)[Calendar_DAY_OF_WEEK_IN_MONTH];
    Boolean useMonth = ((*mIsSet)[Calendar_DATE] || (*mIsSet)[Calendar_MONTH] || weekMonthSet)
            && mLastDateFieldSet != Calendar_DAY_OF_YEAR;
    if (useMonth
            && (mLastDateFieldSet == Calendar_DAY_OF_WEEK || mLastDateFieldSet == Calendar_WEEK_OF_YEAR)) {
        if ((*mIsSet)[Calendar_WEEK_OF_YEAR] && (*mIsSet)[Calendar_DAY_OF_WEEK]) {
            useMonth = mLastDateFieldSet != Calendar_WEEK_OF_YEAR && weekMonthSet
                    && (*mIsSet)[Calendar_DAY_OF_WEEK];
        } else if ((*mIsSet)[Calendar_DAY_OF_YEAR]) {
            useMonth = (*mIsSet)[Calendar_DATE] && (*mIsSet)[Calendar_MONTH];
        }
    }

    if (useMonth) {
        Int32 month = (*mFields)[Calendar_MONTH];
        year += month / 12;
        month %= 12;
        if (month < 0) {
            year--;
            month += 12;
        }
        Boolean leapYear = IsLeapYear(year);
        days = DaysFromBaseYear(year) + DaysInYear(leapYear, month);
        Boolean useDate = (*mIsSet)[Calendar_DATE];
        if (useDate
                && (mLastDateFieldSet == Calendar_DAY_OF_WEEK
                        || mLastDateFieldSet == Calendar_WEEK_OF_MONTH || mLastDateFieldSet == Calendar_DAY_OF_WEEK_IN_MONTH)) {
            useDate = !((*mIsSet)[Calendar_DAY_OF_WEEK] && weekMonthSet);
        }
        if (useDate) {
            if (!IsLenient()
                    && ((*mFields)[Calendar_DATE] < 1 || (*mFields)[Calendar_DATE] > DaysInMonth(
                            leapYear, month))) {
                return NOERROR;
            }
            days += (*mFields)[Calendar_DATE] - 1;
        } else {
            Int32 dayOfWeek;
            if ((*mIsSet)[Calendar_DAY_OF_WEEK]) {
                dayOfWeek = (*mFields)[Calendar_DAY_OF_WEEK] - 1;
            } else {
                dayOfWeek = GetFirstDayOfWeek() - 1;
            }
            if ((*mIsSet)[Calendar_WEEK_OF_MONTH]
                    && mLastDateFieldSet != Calendar_DAY_OF_WEEK_IN_MONTH) {
                Int32 skew = Mod7(days - 3 - (GetFirstDayOfWeek() - 1));
                days += ((*mFields)[Calendar_WEEK_OF_MONTH] - 1) * 7
                        + Mod7(skew + dayOfWeek - (days - 3)) - skew;
            } else if ((*mIsSet)[Calendar_DAY_OF_WEEK_IN_MONTH]) {
                if ((*mFields)[Calendar_DAY_OF_WEEK_IN_MONTH] >= 0) {
                    days += Mod7(dayOfWeek - (days - 3))
                            + ((*mFields)[Calendar_DAY_OF_WEEK_IN_MONTH] - 1) * 7;
                } else {
                    days += DaysInMonth(leapYear, month)
                            + Mod7(dayOfWeek
                                    - (days + DaysInMonth(leapYear, month) - 3))
                            + (*mFields)[Calendar_DAY_OF_WEEK_IN_MONTH] * 7;
                }
            } else if ((*mIsSet)[Calendar_DAY_OF_WEEK]) {
                Int32 skew = Mod7(days - 3 - (GetFirstDayOfWeek() - 1));
                days += Mod7(Mod7(skew + dayOfWeek - (days - 3)) - skew);
            }
        }
    } else {
        Boolean useWeekYear = (*mIsSet)[Calendar_WEEK_OF_YEAR]
                && mLastDateFieldSet != Calendar_DAY_OF_YEAR;
        if (useWeekYear && (*mIsSet)[Calendar_DAY_OF_YEAR]) {
            useWeekYear = (*mIsSet)[Calendar_DAY_OF_WEEK];
        }
        days = DaysFromBaseYear(year);
        if (useWeekYear) {
            Int32 dayOfWeek;
            if ((*mIsSet)[Calendar_DAY_OF_WEEK]) {
                dayOfWeek = (*mFields)[Calendar_DAY_OF_WEEK] - 1;
            } else {
                dayOfWeek = GetFirstDayOfWeek() - 1;
            }
            Int32 skew = Mod7(days - 3 - (GetFirstDayOfWeek() - 1));
            days += ((*mFields)[Calendar_WEEK_OF_YEAR] - 1) * 7
                    + Mod7(skew + dayOfWeek - (days - 3)) - skew;
            if (7 - skew < GetMinimalDaysInFirstWeek()) {
                days += 7;
            }
        } else if ((*mIsSet)[Calendar_DAY_OF_YEAR]) {
            if (!IsLenient()
                    && ((*mFields)[Calendar_DAY_OF_YEAR] < 1 || (*mFields)[Calendar_DAY_OF_YEAR] > (365 + (IsLeapYear(year) ? 1
                            : 0)))) {
                return E_INVALID_ARGUMENT;
            }
            days += (*mFields)[Calendar_DAY_OF_YEAR] - 1;
        } else if ((*mIsSet)[Calendar_DAY_OF_WEEK]) {
            days += Mod7((*mFields)[Calendar_DAY_OF_WEEK] - 1 - (days - 3));
        }
    }
    mLastDateFieldSet = 0;

    timeVal += days * 86400000;
    // Use local time to compare with the gregorian change
    if (year == mChangeYear
            && timeVal >= mGregorianCutover + JulianError() * 86400000L) {
        timeVal -= JulianError() * 86400000L;
    }

    // It is not possible to simply subtract getOffset(timeVal) from timeVal
    // to get UTC.
    // The trick is needed for the moment when DST transition occurs,
    // say 1:00 is a transition time when DST offset becomes +1 hour,
    // then wall time in the interval 1:00 - 2:00 is invalid and is
    // treated as UTC time.
    ITimeZone *tz;
    GetTimeZone((ITimeZone **)&tz);

    Int32 rawOffset;

    tz->GetRawOffsetVir(&rawOffset);

    Int64 timeValWithoutDST = timeVal - GetOffset(timeVal)
            + rawOffset;

    timeVal -= GetOffset(timeValWithoutDST);
    // Need to update wall time in fields, since it was invalid due to DST
    // transition

    mTime = timeVal;
    if (timeValWithoutDST != timeVal) {
        ComputeFields();
        mAreFieldsSet = TRUE;
    }
}

//void CGregorianCalendar::Complete()
//{
////    if (!isTimeSet) {
////        ComputeTime();
////        isTimeSet = true;
////    }
////    if (!areFieldsSet) {
////        ComputeFields();
////        areFieldsSet = true;
////    }
//}


ECode CGregorianCalendar::constructor()
{
    ITimeZone  *timezone;
    ILocale *locale;
    TimeZone::GetDefault((ITimeZone**) &timezone);
    Locale::GetDefault((ILocale**) &locale);
    constructor((ITimeZone*)timezone, (ILocale*)locale);
    return NOERROR;
}

ECode CGregorianCalendar::constructor(
    /* [in] */ ILocale* locale)
{
    ITimeZone *timezone;
    TimeZone::GetDefault((ITimeZone**) &timezone);
    constructor((ITimeZone*)timezone, locale);
    return NOERROR;
}

ECode CGregorianCalendar::constructor(
    /* [in] */ ITimeZone* timezone)
{
    ILocale *locale;
    Locale::GetDefault((ILocale**) &locale);
    constructor(timezone, (ILocale *)locale);
    return NOERROR;
}

ECode CGregorianCalendar::constructor(
    /* [in] */ ITimeZone* timezone,
    /* [in] */ ILocale* locale)
{
    Calendar::constructor(timezone, locale);
    SetTimeInMillis(System::GetCurrentTimeMillis());
    return NOERROR;
}

ECode CGregorianCalendar::constructor(
    /* [in] */ Int64 milliseconds)
{
    constructor(FALSE);
    SetTimeInMillis(milliseconds);
    return NOERROR;
}

ECode CGregorianCalendar::constructor(
    /* [in] */ Boolean ignored)
{
    ITimeZone *timezone;
    TimeZone::GetDefault((ITimeZone**) &timezone);
    Calendar::constructor(timezone);
    SetFirstDayOfWeek(Calendar_SUNDAY);
    SetMinimalDaysInFirstWeek(1);
    return NOERROR;
}

void CGregorianCalendar::ComputeFields()
{
    ITimeZone *timeZone;
    Boolean isUsed;

    GetTimeZone((ITimeZone **)&timeZone);
    AutoPtr<IDate> date;
    ECode ec = NOERROR;
    ec = CDate::New(mTime, (IDate **) &date);

    Boolean isIn;
    timeZone->InDaylightTimeVir((IDate*) date, &isIn);
    Int32 dstOffset = 0;

    if (isIn) {
        Int32 savings;
        timeZone->GetDSTSavings(&savings);
        dstOffset = savings;
    }

    Int32 zoneOffset;
    timeZone->GetRawOffsetVir(&zoneOffset);

    (*mFields)[Calendar_DST_OFFSET] = dstOffset;
    (*mFields)[Calendar_ZONE_OFFSET] = zoneOffset;

    Int32 millis = (Int32) (mTime % 86400000);
    Int32 savedMillis = millis;
    // compute without a change in daylight saving time
    Int32 offset = zoneOffset + dstOffset;
    Int64 newTime = mTime + offset;

    if (mTime > 0L && newTime < 0L && offset > 0) {
        newTime = 0x7fffffffffffffffL;
    } else if (mTime < 0L && newTime > 0L && offset < 0) {
        newTime = 0x8000000000000000L;
    }

    // FIXME: I don't think this caching ever really gets used, because it requires that the
    // time zone doesn't use daylight savings (ever). So unless you're somewhere like Taiwan...
    if (mIsCached) {
        if (millis < 0) {
            millis += 86400000;
        }

        // Cannot add ZONE_OFFSET to time as it might overflow
        millis += zoneOffset;
        millis += dstOffset;

        if (millis < 0) {
            millis += 86400000;
        } else if (millis >= 86400000) {
            millis -= 86400000;
        }

        (*mFields)[Calendar_MILLISECOND] = (millis % 1000);
        millis /= 1000;
        (*mFields)[Calendar_SECOND] = (millis % 60);
        millis /= 60;
        (*mFields)[Calendar_MINUTE] = (millis % 60);
        millis /= 60;
        (*mFields)[Calendar_HOUR_OF_DAY] = (millis % 24);
        millis /= 24;
        (*mFields)[Calendar_AM_PM] = (*mFields)[Calendar_HOUR_OF_DAY] > 11 ? 1 : 0;
        (*mFields)[Calendar_HOUR] = (*mFields)[Calendar_HOUR_OF_DAY] % 12;

        // FIXME: this has to be wrong; useDaylightTime doesn't mean what they think it means!
        Int64 newTimeAdjusted = newTime;
        timeZone->UseDaylightTimeVir(&isUsed);
        if (isUsed) {
            // BEGIN android-changed: removed unnecessary cast
            Int32 dstSavings;
            timeZone->GetDSTSavings(&dstSavings);
            // END android-changed
            newTimeAdjusted += (dstOffset == 0) ? dstSavings : -dstSavings;
        }

        if (newTime > 0L && newTimeAdjusted < 0L && dstOffset == 0) {
            newTimeAdjusted = 0x7fffffffffffffffL;
        } else if (newTime < 0L && newTimeAdjusted > 0L && dstOffset != 0) {
            newTimeAdjusted = 0x8000000000000000L;
        }

        CachedFieldsCheckAndGet(mTime, newTime, newTimeAdjusted,
                savedMillis, zoneOffset);
    } else {
        FullFieldsCalc(mTime, savedMillis, zoneOffset);
    }

    for (Int32 i = 0; i < Calendar_FIELD_COUNT; i++) {
        (*mIsSet)[i] = true;
    }

    // Caching
    timeZone->UseDaylightTimeVir(&isUsed);
    if (!mIsCached
            && newTime != 0x7fffffffffffffffL
            && newTime != 0x8000000000000000L
            && (!isUsed)) {
        Int32 cacheMillis = 0;

        (*mCachedFields)[0] =  (*mFields)[Calendar_YEAR];
        (*mCachedFields)[1] =  (*mFields)[Calendar_MONTH];
        (*mCachedFields)[2] =  (*mFields)[Calendar_DATE];
        (*mCachedFields)[3] =  (*mFields)[Calendar_DAY_OF_WEEK];
        (*mCachedFields)[4] = zoneOffset;
        (*mCachedFields)[5] =  (*mFields)[Calendar_ERA];
        (*mCachedFields)[6] =  (*mFields)[Calendar_WEEK_OF_YEAR];
        (*mCachedFields)[7] =  (*mFields)[Calendar_WEEK_OF_MONTH];
        (*mCachedFields)[8] =  (*mFields)[Calendar_DAY_OF_YEAR];
        (*mCachedFields)[9] =  (*mFields)[Calendar_DAY_OF_WEEK_IN_MONTH];

        cacheMillis += (23 -  (*mFields)[Calendar_HOUR_OF_DAY]) * 60 * 60 * 1000;
        cacheMillis += (59 -  (*mFields)[Calendar_MINUTE]) * 60 * 1000;
        cacheMillis += (59 -  (*mFields)[Calendar_SECOND]) * 1000;
        mNextMidnightMillis = newTime + cacheMillis;

        cacheMillis = (*mFields)[Calendar_HOUR_OF_DAY] * 60 * 60 * 1000;
        cacheMillis += (*mFields)[Calendar_MINUTE] * 60 * 1000;
        cacheMillis += (*mFields)[Calendar_SECOND] * 1000;
        mLastMidnightMillis = newTime - cacheMillis;

        mIsCached = TRUE;
    }
}


ECode CGregorianCalendar::GetOffset(
    /* [in] */ Int64 localTime,
    /* [out] */ Int32* offset)
{
    *offset = GetOffset(localTime);
    return NOERROR;
}

Int32 CGregorianCalendar::GetOffset(
    /* [in] */ Int64 localTime)
{
    ITimeZone *timeZone;
    GetTimeZone((ITimeZone **) &timeZone);
    Int32 rawOffset;

    Boolean isUsed;
    timeZone->UseDaylightTimeVir(&isUsed);

    if (!isUsed) {
        timeZone->GetRawOffsetVir(&rawOffset);
        return rawOffset;
    }

    Int64 dayCount = localTime / 86400000;
    Int32 millis = (Int32) (localTime % 86400000);
    if (millis < 0) {
        millis += 86400000;
        dayCount--;
    }

    Int32 year = 1970;
    Int64 days = dayCount;
    if (localTime < mGregorianCutover) {
        days -= mJulianSkew;
    }
    Int32 approxYears;

    while ((approxYears = (Int32) (days / 365)) != 0) {
        year = year + approxYears;
        days = dayCount - DaysFromBaseYear(year);
    }
    if (days < 0) {
        year = year - 1;
        days = days + 365 + (IsLeapYear(year) ? 1 : 0);
        if (year == mChangeYear && localTime < mGregorianCutover) {
            days -= JulianError();
        }
    }
    if (year <= 0) {
        timeZone->GetRawOffsetVir(&rawOffset);
        return rawOffset;
    }
    Int32 dayOfYear = (int) days + 1;

    Int32 month = dayOfYear / 32;
    Boolean leapYear = IsLeapYear(year);
    Int32 date = dayOfYear - DaysInYear(leapYear, month);
    if (date > DaysInMonth(leapYear, month)) {
        date -= DaysInMonth(leapYear, month);
        month++;
    }
    Int32 dayOfWeek = Mod7(dayCount - 3) + 1;
    Int32 offset;
    timeZone->GetOffsetVir(Calendar_AD, year, month, date, dayOfWeek,
            millis, &offset);
    return offset;
}

Int32 CGregorianCalendar::DaysInMonth()
{
    return DaysInMonth(IsLeapYear((*mFields)[Calendar_YEAR]), (*mFields)[Calendar_MONTH]);
}

Int32 CGregorianCalendar::DaysInMonth(
        /* [in] */ Boolean leapYear,
        /* [in] */ Int32 month)
{
    if (leapYear && month == Calendar_FEBRUARY) {
        return mDaysInMonth[month] + 1;
    }

    return mDaysInMonth[month];
}

Int32 CGregorianCalendar::DaysInYear(
        /* [in] */ Int32 year)
{
    Int32 daysInYear = IsLeapYear(year) ? 366 : 365;
    if (year == mChangeYear) {
        daysInYear -= mCurrentYearSkew;
    }
    if (year == mChangeYear - 1) {
        daysInYear -= mLastYearSkew;
    }
    return daysInYear;
}

Boolean CGregorianCalendar::IsLeapYear(
    /* [in] */ Int32 year)
{
    if (year > mChangeYear) {
        return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
    }

    return year % 4 == 0;
}

ECode CGregorianCalendar::IsLeapYear(
        /* [in] */ Int32 year,
        /* [out] */ Boolean* isLeap)
{
    *isLeap = IsLeapYear(year);
    return NOERROR;
}

ECode CGregorianCalendar::GetGregorianChange(
        /* [out] */ IDate** ppDate)
{
    return CDate::New(mGregorianCutover, ppDate);
}

Int32 CGregorianCalendar::Mod(
    /* [in] */ Int32 value,
    /* [in] */ Int32 mod)
{
    Int32 rem = value % mod;
    if (value < 0 && rem < 0) {
        return rem + mod;
    }
    return rem;
}

Int32 CGregorianCalendar::Mod7(
    /* [in] */ Int64 num1)
{
    Int32 rem = (int) (num1 % 7);
    if (num1 < 0 && rem < 0) {
        return rem + 7;
    }
    return rem;
}

Int32 CGregorianCalendar::JulianError() {
    return mChangeYear / 100 - mChangeYear / 400 - 2;
}

Int64 CGregorianCalendar::DaysFromBaseYear(
    /* [in] */ Int32 iyear)
{
    Int64 year = iyear;

    if (year >= 1970) {
        Int64 days = (year - 1970) * 365 + ((year - 1969) / 4);
        if (year > mChangeYear) {
            days -= ((year - 1901) / 100) - ((year - 1601) / 400);
        } else {
            if(year == mChangeYear){
                days += mCurrentYearSkew;
            }else if(year == mChangeYear -1){
                days += mLastYearSkew;
            }else{
                days += mJulianSkew;
            }
        }
        return days;
    } else if (year <= mChangeYear) {
        return (year - 1970) * 365 + ((year - 1972) / 4) + mJulianSkew;
    }
    return (year - 1970) * 365 + ((year - 1972) / 4)
            - ((year - 2000) / 100) + ((year - 2000) / 400);
}

Int32 CGregorianCalendar::DaysInYear(
    /* [in] */ Boolean leapYear,
    /* [in] */ Int32 month)
{
    if (leapYear && month > Calendar_FEBRUARY) {
        return mDaysInYear[month] + 1;
    }

    return mDaysInYear[month];
}

void CGregorianCalendar::CachedFieldsCheckAndGet(
    /* [in] */ Int64 timeVal,
    /* [in] */ Int64 newTimeMillis,
    /* [in] */ Int64 newTimeMillisAdjusted,
    /* [in] */ Int32 millis,
    /* [in] */ Int32 zoneOffset)
{
    Int32 dstOffset = (*mFields)[Calendar_DST_OFFSET];
    if (!mIsCached
            || newTimeMillis >= mNextMidnightMillis
            || newTimeMillis <= mLastMidnightMillis
            || (*mCachedFields)[4] != zoneOffset
            || (dstOffset == 0 && (newTimeMillisAdjusted >= mNextMidnightMillis))
            || (dstOffset != 0 && (newTimeMillisAdjusted <= mLastMidnightMillis))) {
        FullFieldsCalc(timeVal, millis, zoneOffset);
        mIsCached = FALSE;
    } else {
        (*mFields)[Calendar_YEAR] = (*mCachedFields)[0];
        (*mFields)[Calendar_MONTH] = (*mCachedFields)[1];
        (*mFields)[Calendar_DATE] = (*mCachedFields)[2];
        (*mFields)[Calendar_DAY_OF_WEEK] = (*mCachedFields)[3];
        (*mFields)[Calendar_ERA] = (*mCachedFields)[5];
        (*mFields)[Calendar_WEEK_OF_YEAR] = (*mCachedFields)[6];
        (*mFields)[Calendar_WEEK_OF_MONTH] = (*mCachedFields)[7];
        (*mFields)[Calendar_DAY_OF_YEAR] = (*mCachedFields)[8];
        (*mFields)[Calendar_DAY_OF_WEEK_IN_MONTH] = (*mCachedFields)[9];
    }
}


void CGregorianCalendar::FullFieldsCalc(
    /* [in] */ Int64 timeVal,
    /* [in] */ Int32 millis,
    /* [in] */ Int32 zoneOffset)
{
    Int64 days = timeVal / 86400000;

    if (millis < 0) {
        millis += 86400000;
        days--;
    }
    // Cannot add ZONE_OFFSET to time as it might overflow
    millis += zoneOffset;
    while (millis < 0) {
        millis += 86400000;
        days--;
    }
    while (millis >= 86400000) {
        millis -= 86400000;
        days++;
    }

    Int32 dayOfYear = ComputeYearAndDay(days, timeVal + zoneOffset);
    (*mFields)[Calendar_DAY_OF_YEAR] = dayOfYear;
    if((*mFields)[Calendar_YEAR] == mChangeYear && mGregorianCutover <= timeVal + zoneOffset){
        dayOfYear += mCurrentYearSkew;
    }
    Int32 month = dayOfYear / 32;
    Boolean leapYear = IsLeapYear((*mFields)[Calendar_YEAR]);
    Int32 date = dayOfYear - DaysInYear(leapYear, month);
    if (date > DaysInMonth(leapYear, month)) {
        date -= DaysInMonth(leapYear, month);
        month++;
    }
    (*mFields)[Calendar_DAY_OF_WEEK] = Mod7(days - 3) + 1;
    Int32 dstOffset;
    if ((*mFields)[Calendar_YEAR] <= 0) {
        dstOffset = 0;
    } else {
        ITimeZone *tz;
        GetTimeZone((ITimeZone**)&tz);
        tz->GetOffsetVir(Calendar_AD,(*mFields)[Calendar_YEAR], month, date,
                (*mFields)[Calendar_DAY_OF_WEEK], millis, &dstOffset);
    }
    if ((*mFields)[Calendar_YEAR] > 0) {
        dstOffset -= zoneOffset;
    }
    (*mFields)[Calendar_DST_OFFSET] = dstOffset;
    if (dstOffset != 0) {
        Int64 oldDays = days;
        millis += dstOffset;
        if (millis < 0) {
            millis += 86400000;
            days--;
        } else if (millis >= 86400000) {
            millis -= 86400000;
            days++;
        }
        if (oldDays != days) {
            dayOfYear = ComputeYearAndDay(days, timeVal - zoneOffset
                    + dstOffset);
            (*mFields)[Calendar_DAY_OF_YEAR] = dayOfYear;
            if((*mFields)[Calendar_YEAR] == mChangeYear && mGregorianCutover <= timeVal - zoneOffset + dstOffset){
                dayOfYear += mCurrentYearSkew;
            }
            month = dayOfYear / 32;
            leapYear = IsLeapYear((*mFields)[Calendar_YEAR]);
            date = dayOfYear - DaysInYear(leapYear, month);
            if (date > DaysInMonth(leapYear, month)) {
                date -= DaysInMonth(leapYear, month);
                month++;
            }
            (*mFields)[Calendar_DAY_OF_WEEK] = Mod7(days - 3) + 1;
        }
    }

    (*mFields)[Calendar_MILLISECOND] = (millis % 1000);
    millis /= 1000;
    (*mFields)[Calendar_SECOND] = (millis % 60);
    millis /= 60;
    (*mFields)[Calendar_MINUTE] = (millis % 60);
    millis /= 60;
    (*mFields)[Calendar_HOUR_OF_DAY] = (millis % 24);
    (*mFields)[Calendar_AM_PM] = (*mFields)[Calendar_HOUR_OF_DAY] > 11 ? 1 : 0;
    (*mFields)[Calendar_HOUR] = (*mFields)[Calendar_HOUR_OF_DAY] % 12;

    if ((*mFields)[Calendar_YEAR] <= 0) {
        (*mFields)[Calendar_ERA] = Calendar_BC;
        (*mFields)[Calendar_YEAR] = -(*mFields)[Calendar_YEAR] + 1;
    } else {
        (*mFields)[Calendar_ERA] = Calendar_AD;
    }
    (*mFields)[Calendar_MONTH] = month;
    (*mFields)[Calendar_DATE] = date;
    (*mFields)[Calendar_DAY_OF_WEEK_IN_MONTH] = (date - 1) / 7 + 1;
    (*mFields)[Calendar_WEEK_OF_MONTH] = (date - 1 + Mod7(days - date - 2
            - (GetFirstDayOfWeek() - 1))) / 7 + 1;
    Int32 daysFromStart = Mod7(days - 3 - ((*mFields)[Calendar_DAY_OF_YEAR] - 1)
            - (GetFirstDayOfWeek() - 1));
    Int32 week = ((*mFields)[Calendar_DAY_OF_YEAR] - 1 + daysFromStart) / 7
            + (7 - daysFromStart >= GetMinimalDaysInFirstWeek() ? 1 : 0);
    if (week == 0) {
        (*mFields)[Calendar_WEEK_OF_YEAR] = 7 - Mod7(daysFromStart
                - (IsLeapYear((*mFields)[Calendar_YEAR] - 1) ? 2 : 1)) >= GetMinimalDaysInFirstWeek() ? 53
                : 52;
    } else if ((*mFields)[Calendar_DAY_OF_YEAR] >= (leapYear ? 367 : 366)
            - Mod7(daysFromStart + (leapYear ? 2 : 1))) {
        (*mFields)[Calendar_WEEK_OF_YEAR] = 7 - Mod7(daysFromStart + (leapYear ? 2 : 1)) >= GetMinimalDaysInFirstWeek() ? 1
                : week;
    } else {
        (*mFields)[Calendar_WEEK_OF_YEAR] = week;
    }
}


CARAPI_(Int32) CGregorianCalendar::ComputeYearAndDay(
    /* [in] */ Int64 dayCount,
    /* [in] */ Int64 localTime)
{
    Int32 year = 1970;
    Int64 days = dayCount;
    if (localTime < mGregorianCutover) {
        days -= mJulianSkew;
    }
    Int32 approxYears;

    while ((approxYears = (Int32) (days / 365)) != 0) {
        year = year + approxYears;
        days = dayCount - DaysFromBaseYear(year);
    }
    if (days < 0) {
        year = year - 1;
        days = days + DaysInYear(year);
    }
    (*mFields)[Calendar_YEAR] = year;
    return (Int32) days + 1;
}