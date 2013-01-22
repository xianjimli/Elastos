
#ifndef __CSIMPLETIMEZONE_H__
#define __CSIMPLETIMEZONE_H__

#include "_CSimpleTimeZone.h"

CarClass(CSimpleTimeZone)
{
public:
    CARAPI GetDisplayName(
        /* [out] */ String * pName);

    CARAPI GetDisplayNameEx(
        /* [in] */ ILocale * pLocale,
        /* [out] */ String * pName);

    CARAPI GetDisplayNameEx1(
        /* [in] */ Boolean daylightTime,
        /* [in] */ Int32 style,
        /* [out] */ String * pName);

    CARAPI GetDisplayNameEx2(
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

    virtual CARAPI GetOffsetVir(
        /* [in] */ Int32 era,
        /* [in] */ Int32 year,
        /* [in] */ Int32 month,
        /* [in] */ Int32 day,
        /* [in] */ Int32 dayOfWeek,
        /* [in] */ Int32 time,
        /* [out] */ Int32 * pOffset);

    virtual CARAPI GetRawOffsetVir(
        /* [out] */ Int32 * pRawOffset);

    CARAPI HasSameRules(
        /* [in] */ ITimeZone * pTimeZone,
        /* [out] */ Boolean * pHasRules);

    virtual CARAPI InDaylightTimeVir(
        /* [in] */ IDate * pTime,
        /* [out] */ Boolean * pIsIn);

    CARAPI SetID(
        /* [in] */ const String& id);

    virtual CARAPI SetRawOffsetVir(
        /* [in] */ Int32 offsetMillis);

    virtual CARAPI UseDaylightTimeVir(
        /* [out] */ Boolean * pIsUsed);

    CARAPI SetEndRule(
        /* [in] */ Int32 month,
        /* [in] */ Int32 dayOfMonth,
        /* [in] */ Int32 time);

    CARAPI SetEndRuleEx(
        /* [in] */ Int32 month,
        /* [in] */ Int32 day,
        /* [in] */ Int32 dayOfWeek,
        /* [in] */ Int32 time);

    CARAPI SetEndRuleEx1(
        /* [in] */ Int32 month,
        /* [in] */ Int32 day,
        /* [in] */ Int32 dayOfWeek,
        /* [in] */ Int32 time,
        /* [in] */ Boolean after);

    CARAPI SetStartRule(
        /* [in] */ Int32 month,
        /* [in] */ Int32 dayOfMonth,
        /* [in] */ Int32 time);

    CARAPI SetStartRuleEx(
        /* [in] */ Int32 month,
        /* [in] */ Int32 day,
        /* [in] */ Int32 dayOfWeek,
        /* [in] */ Int32 time);

    CARAPI SetStartRuleEx1(
        /* [in] */ Int32 month,
        /* [in] */ Int32 day,
        /* [in] */ Int32 dayOfWeek,
        /* [in] */ Int32 time,
        /* [in] */ Boolean after);

    CARAPI SetStartYear(
        /* [in] */ Int32 year);

    CARAPI constructor(
        /* [in] */ Int32 offset,
        /* [in] */ const String& name);

private:
    CARAPI CheckRange(
        /* [in] */ Int32 month,
        /* [in] */ Int32 dayOfWeek,
        /* [in] */ Int32 time);

    CARAPI CheckDay(
        /* [in] */ Int32 month,
        /* [in] */ Int32 day);

    Boolean IsLeapYear(
        /* [in] */ Int32 year);

    Int32 Mod7(
        /* [in] */ Int32 num1);

    void SetStartMode();

    void SetEndMode();

private:
    Int32 mDstSavings;
    Boolean mUseDaylight;
    Int32 mRawOffset;

    Int32 mStartYear;
    Int32 mStartMonth;
    Int32 mStartDay;
    Int32 mStartDayOfWeek;
    Int32 mStartTime;

    Int32 mEndMonth;
    Int32 mEndDay;
    Int32 mEndDayOfWeek;
    Int32 mEndTime;

    Int32 mStartMode;
    Int32 mEndMode;
    // TODO: Add your private member variables here.
};

#endif // __CSIMPLETIMEZONE_H__