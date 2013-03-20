
#include "CSimpleTimeZone.h"
#include "Grego.h"
#include "cmdef.h"
#include "CGregorianCalendar.h"
#include <stdio.h>
const static Int32 DOM_MODE = 1;
const static Int32 DOW_IN_MONTH_MODE = 2;
const static Int32 DOW_GE_DOM_MODE = 3;
const static Int32 DOW_LE_DOM_MODE = 4;


ECode CSimpleTimeZone::GetDisplayName(
    /* [out] */ String * pName)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSimpleTimeZone::GetDisplayNameEx(
    /* [in] */ ILocale * pLocale,
    /* [out] */ String * pName)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSimpleTimeZone::GetDisplayNameEx1(
    /* [in] */ Boolean daylightTime,
    /* [in] */ Int32 style,
    /* [out] */ String * pName)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSimpleTimeZone::GetDisplayNameEx2(
    /* [in] */ Boolean daylightTime,
    /* [in] */ Int32 style,
    /* [in] */ ILocale * pLocale,
    /* [out] */ String * pName)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSimpleTimeZone::GetID(
    /* [out] */ String * pId)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSimpleTimeZone::GetDSTSavings(
    /* [out] */ Int32 * pSavings)
{
    if (!mUseDaylight) {
        *pSavings = 0;
        return NOERROR;
    }
    *pSavings = mDstSavings;
    return NOERROR;
}

ECode CSimpleTimeZone::GetOffset(
    /* [in] */ Int64 time,
    /* [out] */ Int32 * pOffset)
{
    Boolean isUsed;
    UseDaylightTimeVir(&isUsed);
    if (!isUsed) {
        return mRawOffset;
    }
    ArrayOf<Int32> *fields;
    fields = Grego::TimeToFields(time + mRawOffset, NULL);
    return GetOffsetVir(Calendar_AD, (*fields)[0], (*fields)[1], (*fields)[2],
            (*fields)[3], (*fields)[5], pOffset);
}

ECode CSimpleTimeZone::GetOffsetVir(
    /* [in] */ Int32 era,
    /* [in] */ Int32 year,
    /* [in] */ Int32 month,
    /* [in] */ Int32 day,
    /* [in] */ Int32 dayOfWeek,
    /* [in] */ Int32 time,
    /* [out] */ Int32 * pOffset)
{
    if (era != Calendar_BC && era != Calendar_AD) {
        return E_INVALID_ARGUMENT;
//        throw new IllegalArgumentException("Invalid era: " + era);
    }

    FAIL_RETURN(CheckRange(month, dayOfWeek, time));

    if (month != Calendar_FEBRUARY || day != 29 || !IsLeapYear(year)) {
        FAIL_RETURN(CheckDay(month, day));
    }

    // BEGIN android-changed
    // return icuTZ.getOffset(era, year, month, day, dayOfWeek, time);
    Boolean isUsed;
    UseDaylightTimeVir(&isUsed);
    if (!isUsed || era != Calendar_AD || year < mStartYear) {
        *pOffset = mRawOffset;
        return NOERROR;
    }

    if (mEndMonth < mStartMonth) {
        if (month > mEndMonth && month < mStartMonth) {
            *pOffset = mRawOffset;
            return NOERROR;
        }
    } else {
        if (month < mStartMonth || month > mEndMonth) {
            *pOffset = mRawOffset;
            return NOERROR;
        }
    }

    Int32 ruleDay = 0, daysInMonth, firstDayOfMonth = Mod7(dayOfWeek - day);
    if (month == mStartMonth) {
        switch (mStartMode) {
            case DOM_MODE:
                ruleDay = mStartDay;
                break;
            case DOW_IN_MONTH_MODE:
                if (mStartDay >= 0) {
                    ruleDay = Mod7(mStartDayOfWeek - firstDayOfMonth) + 1
                            + (mStartDay - 1) * 7;
                } else {
                    daysInMonth = CGregorianCalendar::mDaysInMonth[mStartMonth];
                    if (mStartMonth == Calendar_FEBRUARY && IsLeapYear(
                            year)) {
                        daysInMonth += 1;
                    }
                    ruleDay = daysInMonth
                            + 1
                            + Mod7(mStartDayOfWeek
                            - (firstDayOfMonth + daysInMonth))
                            + mStartDay * 7;
                }
                break;
            case DOW_GE_DOM_MODE:
                ruleDay = mStartDay
                        + Mod7(mStartDayOfWeek
                        - (firstDayOfMonth + mStartDay - 1));
                break;
            case DOW_LE_DOM_MODE:
                ruleDay = mStartDay
                        + Mod7(mStartDayOfWeek
                        - (firstDayOfMonth + mStartDay - 1));
                if (ruleDay != mStartDay) {
                    ruleDay -= 7;
                }
                break;
        }
        if (ruleDay > day || ruleDay == day && time < mStartTime) {
            *pOffset = mRawOffset;
            return NOERROR;
        }
    }

    int ruleTime = mEndTime - mDstSavings;
    int nextMonth = (month + 1) % 12;
    if (month == mEndMonth || (ruleTime < 0 && nextMonth == mEndMonth)) {
        switch (mEndMode) {
            case DOM_MODE:
                ruleDay = mEndDay;
                break;
            case DOW_IN_MONTH_MODE:
                if (mEndDay >= 0) {
                    ruleDay = Mod7(mEndDayOfWeek - firstDayOfMonth) + 1
                            + (mEndDay - 1) * 7;
                } else {
                    daysInMonth = CGregorianCalendar::mDaysInMonth[mEndMonth];
                    if (mEndMonth == Calendar_FEBRUARY && IsLeapYear(year)) {
                        daysInMonth++;
                    }
                    ruleDay = daysInMonth
                            + 1
                            + Mod7(mEndDayOfWeek
                            - (firstDayOfMonth + daysInMonth)) + mEndDay
                            * 7;
                }
                break;
            case DOW_GE_DOM_MODE:
                ruleDay = mEndDay
                        + Mod7(
                        mEndDayOfWeek - (firstDayOfMonth + mEndDay - 1));
                break;
            case DOW_LE_DOM_MODE:
                ruleDay = mEndDay
                        + Mod7(
                        mEndDayOfWeek - (firstDayOfMonth + mEndDay - 1));
                if (ruleDay != mEndDay) {
                    ruleDay -= 7;
                }
                break;
        }

        int ruleMonth = mEndMonth;
        if (ruleTime < 0) {
            int changeDays = 1 - (ruleTime / 86400000);
            ruleTime = (ruleTime % 86400000) + 86400000;
            ruleDay -= changeDays;
            if (ruleDay <= 0) {
                if (--ruleMonth < Calendar_JANUARY) {
                    ruleMonth = Calendar_DECEMBER;
                }
                ruleDay += CGregorianCalendar::mDaysInMonth[ruleMonth];
                if (ruleMonth == Calendar_FEBRUARY && IsLeapYear(year)) {
                    ruleDay++;
                }
            }
        }

        if (month == ruleMonth) {
            if (ruleDay < day || ruleDay == day && time >= ruleTime) {
                *pOffset = mRawOffset;
                return NOERROR;
            }
        } else if (nextMonth != ruleMonth) {
                *pOffset = mRawOffset;
                return NOERROR;
        }
    }

    *pOffset = mRawOffset + mDstSavings;
    return NOERROR;
    // END android-changed
}

ECode CSimpleTimeZone::GetRawOffsetVir(
    /* [out] */ Int32 * pRawOffset)
{
    // TODO: Add your code here
    return mRawOffset;
}

ECode CSimpleTimeZone::HasSameRules(
    /* [in] */ ITimeZone * pTimeZone,
    /* [out] */ Boolean * pHasRules)
{
    CSimpleTimeZone *tz = (CSimpleTimeZone*) pTimeZone;
    if (mUseDaylight!= tz->mUseDaylight) {
        *pHasRules = FALSE;
        return NOERROR;
    }
    if (!mUseDaylight) {
        *pHasRules = (mRawOffset == tz->mRawOffset);
        return NOERROR;
    }
    *pHasRules = (mRawOffset == tz->mRawOffset && mDstSavings == tz->mDstSavings
            && mStartYear == tz->mStartYear && mStartMonth == tz->mStartMonth
            && mStartDay == tz->mStartDay && mStartMode == tz->mStartMode
            && mStartDayOfWeek == tz->mStartDayOfWeek
            && mStartTime == tz->mStartTime && mEndMonth == tz->mEndMonth
            && mEndDay == tz->mEndDay && mEndDayOfWeek == tz->mEndDayOfWeek
            && mEndTime == tz->mEndTime && mEndMode == tz->mEndMode);
    return NOERROR;
}

ECode CSimpleTimeZone::InDaylightTimeVir(
    /* [in] */ IDate * pTime,
    /* [out] */ Boolean * pIsIn)
{
    Boolean isUsed;
    UseDaylightTimeVir(&isUsed);

    Int64 ti;
    pTime->GetTime(&ti);

    Int32 offset;
    GetOffset(ti, &offset);

    *pIsIn =(isUsed && offset) != mRawOffset;
    // TODO: Add your code here
    return NOERROR;
}

ECode CSimpleTimeZone::SetID(
    /* [in] */ const String& id)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSimpleTimeZone::SetRawOffsetVir(
    /* [in] */ Int32 offsetMillis)
{
    // TODO: Add your code here
    mRawOffset = offsetMillis;
    return NOERROR;
}

ECode CSimpleTimeZone::UseDaylightTimeVir(
    /* [out] */ Boolean * pIsUsed)
{
    // TODO: Add your code here
    *pIsUsed = mUseDaylight;
    return NOERROR;
}

ECode CSimpleTimeZone::SetEndRule(
    /* [in] */ Int32 month,
    /* [in] */ Int32 dayOfMonth,
    /* [in] */ Int32 time)
{
    // TODO: Add your code here
    mEndMonth = month;
    mEndDay = dayOfMonth;
    mEndDayOfWeek = 0; // Initialize this value for hasSameRules()
    mEndTime = time;
    SetEndMode();
    return NOERROR;
}

ECode CSimpleTimeZone::SetEndRuleEx(
    /* [in] */ Int32 month,
    /* [in] */ Int32 day,
    /* [in] */ Int32 dayOfWeek,
    /* [in] */ Int32 time)
{
    // TODO: Add your code here
    mEndMonth = month;
    mEndDay = day;
    mEndDayOfWeek = dayOfWeek;
    mEndTime = time;
    SetEndMode();
    return NOERROR;
}

ECode CSimpleTimeZone::SetEndRuleEx1(
    /* [in] */ Int32 month,
    /* [in] */ Int32 day,
    /* [in] */ Int32 dayOfWeek,
    /* [in] */ Int32 time,
    /* [in] */ Boolean after)
{
    // TODO: Add your code here
    mEndMonth = month;
    mEndDay = after ? day : -day;
    mEndDayOfWeek = -dayOfWeek;
    mEndTime = time;
    SetEndMode();
}

ECode CSimpleTimeZone::SetStartRule(
    /* [in] */ Int32 month,
    /* [in] */ Int32 dayOfMonth,
    /* [in] */ Int32 time)
{
    mStartMonth = month;
    mStartDay = dayOfMonth;
    mStartDayOfWeek = 0; // Initialize this value for hasSameRules()
    mStartTime = time;
    SetStartMode();
    // TODO: Add your code here
    return NOERROR;
}

ECode CSimpleTimeZone::SetStartRuleEx(
    /* [in] */ Int32 month,
    /* [in] */ Int32 day,
    /* [in] */ Int32 dayOfWeek,
    /* [in] */ Int32 time)
{
    // TODO: Add your code here
    mStartMonth = month;
    mStartDay = day;
    mStartDayOfWeek = dayOfWeek;
    mStartTime = time;
    SetStartMode();
    return NOERROR;
}

ECode CSimpleTimeZone::SetStartRuleEx1(
    /* [in] */ Int32 month,
    /* [in] */ Int32 day,
    /* [in] */ Int32 dayOfWeek,
    /* [in] */ Int32 time,
    /* [in] */ Boolean after)
{
    mStartMonth = month;
    mStartDay = after ? day : -day;
    mStartDayOfWeek = -dayOfWeek;
    mStartTime = time;
    SetStartMode();
    return NOERROR;
}

ECode CSimpleTimeZone::SetStartYear(
    /* [in] */ Int32 year)
{
    // TODO: Add your code here
    mStartYear = year;
    mUseDaylight = TRUE;
}

ECode CSimpleTimeZone::CheckRange(
    /* [in] */ Int32 month,
    /* [in] */ Int32 dayOfWeek,
    /* [in] */ Int32 time)
{
    if (month < Calendar_JANUARY || month > Calendar_DECEMBER) {
        return E_INVALID_ARGUMENT;
    }
    if (dayOfWeek < Calendar_SUNDAY || dayOfWeek > Calendar_SATURDAY) {
        return E_INVALID_ARGUMENT;
    }
    if (time < 0 || time >= 24 * 3600000) {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode CSimpleTimeZone::CheckDay(
    /* [in] */ Int32 month,
    /* [in] */ Int32 day)
{
    if (day <= 0 || day > CGregorianCalendar::mDaysInMonth[month]) {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}


Boolean CSimpleTimeZone::IsLeapYear(
    /* [in] */ Int32 year)
{
    if (year > 1582) {
        return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
    }
    return year % 4 == 0;
}

Int32 CSimpleTimeZone::Mod7(
    /* [in] */ Int32 num1)
{
    Int32 rem = num1 % 7;
    return (num1 < 0 && rem < 0) ? 7 + rem : rem;
}

void CSimpleTimeZone::SetStartMode()
{
    if (mStartDayOfWeek == 0) {
        mStartMode = DOM_MODE;
    } else if (mStartDayOfWeek < 0) {
        mStartDayOfWeek = -mStartDayOfWeek;
        if (mStartDay < 0) {
            mStartDay = -mStartDay;
            mStartMode = DOW_LE_DOM_MODE;
        } else {
            mStartMode = DOW_GE_DOM_MODE;
        }
    } else {
        mStartMode = DOW_IN_MONTH_MODE;
    }
    mUseDaylight = mStartDay != 0 && mEndDay != 0;
    if (mStartDay != 0) {
        CheckRange(mStartMonth, mStartMode == DOM_MODE ? 1 : mStartDayOfWeek,
                mStartTime);
        if (mStartMode != DOW_IN_MONTH_MODE) {
            CheckDay(mStartMonth, mStartDay);
        } else {
            if (mStartDay < -5 || mStartDay > 5) {
                return;
            }
        }
    }
    if (mStartMode != DOM_MODE) {
        mStartDayOfWeek--;
    }
}

void CSimpleTimeZone::SetEndMode()
{
    if (mEndDayOfWeek == 0) {
        mEndMode = DOM_MODE;
    } else if (mEndDayOfWeek < 0) {
        mEndDayOfWeek = -mEndDayOfWeek;
        if (mEndDay < 0) {
            mEndDay = -mEndDay;
            mEndMode = DOW_LE_DOM_MODE;
        } else {
            mEndMode = DOW_GE_DOM_MODE;
        }
    } else {
        mEndMode = DOW_IN_MONTH_MODE;
    }
    mUseDaylight = mStartDay != 0 && mEndDay != 0;
    if (mEndDay != 0) {
        CheckRange(mEndMonth, mEndMode == DOM_MODE ? 1 : mEndDayOfWeek,
                mEndTime);
        if (mEndMode != DOW_IN_MONTH_MODE) {
            CheckDay(mEndMonth, mEndDay);
        } else {
            if (mEndDay < -5 || mEndDay > 5) {
                return;
            }
        }
    }
    if (mEndMode != DOM_MODE) {
        mEndDayOfWeek--;
    }
}

ECode CSimpleTimeZone::constructor(
    /* [in] */ Int32 offset,
    /* [in] */ const String& name)
{
}