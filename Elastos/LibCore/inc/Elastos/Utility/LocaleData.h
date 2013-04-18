#ifndef __LOCALEDATA_H__
#define __LOCALEDATA_H__

//#include "Elastos.IO.h"
#include <elastos.h>
#include "cmdef.h"
#include "CLocale.h"
#include <elastos/HashMap.h>

using namespace Elastos;

class LocaleData
{

    // A cache for the locale-specific data.
private:
    static HashMap<String, LocaleData*> *mLocaleDataCache;
    LocaleData();

public:
    // Used by Calendar.
    Int32 mFirstDayOfWeek;
    Int32 mMinimalDaysInFirstWeek;

    // Used by DateFormatSymbols.

    ArrayOf<String>* mAmPm;
    ArrayOf<String>* mEras;

    ArrayOf<String>* mLongMonthNames;
    ArrayOf<String>* mShortMonthNames;
    ArrayOf<String>* mLongStandAloneMonthNames;
    ArrayOf<String>* mShortStandAloneMonthNames;

    ArrayOf<String>* mLongWeekdayNames;
    ArrayOf<String>* mShortWeekdayNames;
    ArrayOf<String>* mLongStandAloneWeekdayNames;
    ArrayOf<String>* mShortStandAloneWeekdayNames;

public:
    String mFullTimeFormat;
    String mLongTimeFormat;
    String mMediumTimeFormat;
    String mShortTimeFormat;

    String mFullDateFormat;
    String mLongDateFormat;
    String mMediumDateFormat;
    String mShortDateFormat;

    // Used by DecimalFormatSymbols.
    Char32 mZeroDigit;
    Char32 mDigit;
    Char32 mDecimalSeparator;
    Char32 mGroupingSeparator;
    Char32 mPatternSeparator;
    Char32 mPercent;
    Char32 mPerMill;
    Char32 mMonetarySeparator;
    Char32 mMinusSign;
    String mExponentSeparator;
    String mInfinity;
    String mNaN;
    // Also used by Currency.
    String mCurrencySymbol;
    String mInternationalCurrencySymbol;

    // Used by DecimalFormat and NumberFormat.
    String mNumberPattern;
    String mIntegerPattern;
    String mCurrencyPattern;
    String mPercentPattern;

public:
    static LocaleData* Get(
                        /* [in] */ ILocale* locale);

    String ToString();

    String GetDateFormat(
                        /* [in] */ Int32 style);

    String GetTimeFormat(
                        /* [in] */ Int32 style);
private:
    static LocaleData* MakeLocaleData(
                        /* [in] */ ILocale* locale);

    void OverrideWithDataFrom(
                        /* [in] */ LocaleData* overrides);

    static LocaleData* InitLocaleData(
                        /* [in] */ ILocale* locale);
};

#endif //__LOCALEDATA_H__