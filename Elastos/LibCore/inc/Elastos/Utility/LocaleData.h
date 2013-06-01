#ifndef __LOCALEDATA_H__
#define __LOCALEDATA_H__

#include "cmdef.h"
#include <Elastos.Utility_server.h>
#include <elastos/HashMap.h>
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>

using namespace Elastos;
using namespace Elastos::Core::Threading;

class LocaleData : public ElRefBase, public ILocaleData
{
public:
    LocaleData();

    ~LocaleData();

    CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    static CARAPI_(AutoPtr<ILocaleData>) Get(
        /* [in] */ ILocale* locale);

    CARAPI_(String) ToString();

    CARAPI GetDateFormat(
        /* [in] */ Int32 style,
        /* [out] */ String* format);

    CARAPI GetTimeFormat(
        /* [in] */ Int32 style,
        /* [out] */ String* format);

    CARAPI GetZeroDigit(
        /* [out] */ Char32* zeroDigit);

    CARAPI GetDigit(
        /* [out] */ Char32* digit);

    CARAPI GetDecimalSeparator(
        /* [out] */ Char32* decimalSeparator);

    CARAPI GetGroupingSeparator(
        /* [out] */ Char32* groupingSeparator);

    CARAPI GetPatternSeparator(
        /* [out] */ Char32* patternSeparator);

    CARAPI GetPercent(
        /* [out] */ Char32* percent);

    CARAPI GetPerMill(
        /* [out] */ Char32* perMill);

    CARAPI GetMonetarySeparator(
        /* [out] */ Char32* monetarySeparator);

    CARAPI GetMinusSign(
        /* [out] */ Char32* minusSign);

    CARAPI GetExponentSeparator(
        /* [out] */ String* exponentSeparator);

    CARAPI GetInfinity(
        /* [out] */ String* infinity);

    CARAPI GetNaN(
        /* [out] */ String* naN);

    CARAPI GetCurrencySymbol(
        /* [out] */ String* currencySymbol);

    CARAPI GetInternationalCurrencySymbol(
        /* [out] */ String* internationalCurrencySymbol);

    CARAPI GetNumberPattern(
        /* [out] */ String* numberPattern);

    CARAPI GetCurrencyPattern(
        /* [out] */ String* currencyPattern);

    CARAPI GetPercentPattern(
        /* [out] */ String* percentPattern);

private:
    static CARAPI_(AutoPtr<LocaleData>) MakeLocaleData(
        /* [in] */ ILocale* locale);

    CARAPI_(void) OverrideWithDataFrom(
        /* [in] */ ILocaleData* overrides);

    static CARAPI_(AutoPtr<ILocaleData>) InitLocaleData(
        /* [in] */ ILocale* locale);

public:
    // Used by Calendar.
    AutoPtr<IInteger32> mFirstDayOfWeek;
    AutoPtr<IInteger32> mMinimalDaysInFirstWeek;

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

    // A cache for the locale-specific data.
private:
    static HashMap< String, AutoPtr<LocaleData> > sLocaleDataCache;
    static Mutex sLocaleDataCacheLock;
};

#endif //__LOCALEDATA_H__
