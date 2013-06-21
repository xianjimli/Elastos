
#include "cmdef.h"
#include "DateFormatSymbols.h"
#include "TimeZones.h"

DateFormatSymbols::DateFormatSymbols()
    : mCustomZoneStrings(FALSE)
{ }

DateFormatSymbols::~DateFormatSymbols()
{
    FREE_ARRAY_OF_STRING(mAmpms);
    FREE_ARRAY_OF_STRING(mEras);
    FREE_ARRAY_OF_STRING(mMonths);
    FREE_ARRAY_OF_STRING(mShortMonths);
    FREE_ARRAY_OF_STRING(mShortWeekdays);
    FREE_ARRAY_OF_STRING(mWeekdays);
    FREE_ARRAY_OF_STRING(mLongStandAloneMonths);
    FREE_ARRAY_OF_STRING(mShortStandAloneMonths);
    FREE_ARRAY_OF_STRING(mLongStandAloneWeekdays);
    FREE_ARRAY_OF_STRING(mShortStandAloneWeekdays);

    if (mZoneStrings != NULL) {
        for (Int32 i = 0; i < mZoneStrings->GetLength(); ++i) {
            ArrayOf<String>::Free((*mZoneStrings)[i]);
        }
        ArrayOf<ArrayOf<String>*>::Free(mZoneStrings);
    }
}

ArrayOf<ArrayOf<String>*>* DateFormatSymbols::InternalZoneStrings()
{
    Mutex::Autolock lock(mLock);

    if (mZoneStrings == NULL) {
        TimeZones::GetZoneStrings(mLocale, &mZoneStrings);
    }

    return mZoneStrings;
}

ECode DateFormatSymbols::Init()
{
    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    AutoPtr<ILocale> locale;
    localeHelper->GetDefault((ILocale**)&locale);
    return Init(locale);
}

ECode DateFormatSymbols::Init(
    /* [in] */ ILocale* locale)
{
    mLocale = locale;
    mLocalPatternChars = ISimpleDateFormat_PATTERN_CHARS;
    AutoPtr<ILocaleDataHelper> localeDataHelper;
    FAIL_RETURN(CLocaleDataHelper::AcquireSingleton((ILocaleDataHelper**)&localeDataHelper));
    AutoPtr<ILocaleData> localeData;
    FAIL_RETURN(localeDataHelper->Get(locale, (ILocaleData**)&localeData));

    localeData->GetAmPm(&mAmpms);
    localeData->GetEras(&mEras);
    localeData->GetLongMonthNames(&mMonths);
    localeData->GetShortMonthNames(&mShortMonths);
    localeData->GetLongWeekdayNames(&mWeekdays);
    localeData->GetShortWeekdayNames(&mShortWeekdays);

    // ICU/Android extensions.
    localeData->GetLongStandAloneMonthNames(&mLongStandAloneMonths);
    localeData->GetShortStandAloneMonthNames(&mShortStandAloneMonths);
    localeData->GetLongStandAloneWeekdayNames(&mLongStandAloneWeekdays);
    localeData->GetShortStandAloneWeekdayNames(&mShortStandAloneWeekdays);

    return NOERROR;
}

ECode DateFormatSymbols::GetInstance(
        /* [out] */ IDateFormatSymbols** instance)
{
    AutoPtr<ILocaleHelper> pLocaleHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&pLocaleHelper);
    AutoPtr<ILocale> pLocale;
    pLocaleHelper->GetDefault((ILocale**)&pLocale);
    return GetInstance((ILocale*)pLocale, instance);
}

ECode DateFormatSymbols::GetInstance(
        /* [in] */ ILocale* locale,
        /* [out] */ IDateFormatSymbols** instance)
{
    if (locale == NULL) {
        //throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
//    CDateFormatSymbols::New(locale, instance);
    return NOERROR;
}

ECode DateFormatSymbols::GetAvailableLocales(
        /* [out, callee] */ ArrayOf<ILocale*>** arrayOfLocales)
{
    AutoPtr<IICUHelper> pICUHelper;
    CICUHelper::AcquireSingleton((IICUHelper**)&pICUHelper);
    return pICUHelper->GetAvailableDateFormatSymbolsLocales(arrayOfLocales);
}

Boolean DateFormatSymbols::TimeZoneStringsEqual(
        /* [in] */ IDateFormatSymbols *lhs,
        /* [in] */ IDateFormatSymbols *rhs)
{
    assert(0);
    // Quick check that may keep us from having to load the zone strings.
    // Note that different locales may have the same strings, so the opposite check isn't valid.
//    if (lhs.zoneStrings == null && rhs.zoneStrings == null && lhs.locale.equals(rhs.locale)) {
//        return true;
//    }
    // Make sure zone strings are loaded, then check.
//    return Arrays.deepEquals(lhs.internalZoneStrings(), rhs.internalZoneStrings());

    return NOERROR;
}

ECode DateFormatSymbols::GetLongStandAloneMonths(
    /* [out, callee] */ ArrayOf<String>** longStandAloneMonths)
{
    *longStandAloneMonths = mLongStandAloneMonths->Clone();
    return NOERROR;
}

ECode DateFormatSymbols::GetShortStandAloneMonths(
    /* [out, callee] */ ArrayOf<String>** shortStandAloneMonths)
{
    *shortStandAloneMonths = mShortStandAloneMonths->Clone();
    return NOERROR;
}

ECode DateFormatSymbols::GetLongStandAloneWeekdays(
    /* [out, callee] */ ArrayOf<String>** longStandAloneWeekdays)
{
    *longStandAloneWeekdays = mLongStandAloneWeekdays->Clone();
    return NOERROR;
}

ECode DateFormatSymbols::GetShortStandAloneWeekdays(
    /* [out, callee] */ ArrayOf<String>** shortStandAloneWeekdays)
{
    *shortStandAloneWeekdays = mShortStandAloneWeekdays->Clone();
    return NOERROR;
}

ECode DateFormatSymbols::GetCustomZoneStrings(
    /* [out] */ Boolean* customZoneStrings)
{
    VALIDATE_NOT_NULL(customZoneStrings);
    *customZoneStrings = mCustomZoneStrings;
    return NOERROR;
}

ECode DateFormatSymbols::GetLocale(
    /* [out] */ ILocale** locale)
{
    *locale = (ILocale*)mLocale;
    return NOERROR;
}

ECode DateFormatSymbols::GetAmPmStrings(
    /* [out, callee] */ ArrayOf<String>** arrayOfStrings)
{
    *arrayOfStrings = mAmpms->Clone();
    return NOERROR;
}

ECode DateFormatSymbols::GetEras(
    /* [out, callee] */ ArrayOf<String>** arrayOfStrings)
{
    *arrayOfStrings = mEras->Clone();
    return NOERROR;
}

ECode DateFormatSymbols::GetLocalPatternChars(
    /* [out] */ String* string)
{
    VALIDATE_NOT_NULL(string);
    *string = mLocalPatternChars;
    return NOERROR;
}

ECode DateFormatSymbols::GetMonths(
    /* [out, callee] */ ArrayOf<String>** arrayOfStrings)
{
    *arrayOfStrings = mMonths->Clone();
    return NOERROR;
}

ECode DateFormatSymbols::GetShortMonths(
            /* [out, callee] */ ArrayOf<String>** arrayOfStrings)
{
    *arrayOfStrings = mShortMonths->Clone();
    return NOERROR;
}

ECode DateFormatSymbols::GetShortWeekdays(
    /* [out, callee] */ ArrayOf<String>** arrayOfStrings)
{
    *arrayOfStrings = mShortWeekdays->Clone();
    return NOERROR;
}

ECode DateFormatSymbols::GetWeekdays(
    /* [out, callee] */ ArrayOf<String>** arrayOfStrings)
{
    *arrayOfStrings = mWeekdays->Clone();
    return NOERROR;
}

ECode DateFormatSymbols::GetZoneStrings(
    /* [out, callee] */ ArrayOf<ArrayOf<String>*>** zoneStrings)
{
    return TimeZones::Clone2dStringArray(InternalZoneStrings(), zoneStrings);
}

ECode DateFormatSymbols::SetAmPmStrings(
    /* [in] */ const ArrayOf<String>& data)
{
    mAmpms = data.Clone();
    return NOERROR;
}

ECode DateFormatSymbols::SetEras(
    /* [in] */ const ArrayOf<String>& data)
{
    mEras = data.Clone();
    return NOERROR;
}

ECode DateFormatSymbols::SetLocalPatternChars(
    /* [in] */ String data)
{
    if (data.IsNull()) {
        //throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    mLocalPatternChars = data;
    return NOERROR;
}

ECode DateFormatSymbols::SetMonths(
    /* [in] */ const ArrayOf<String>& data)
{
    mMonths = data.Clone();
    return NOERROR;
}

ECode DateFormatSymbols::SetShortMonths(
    /* [in] */ const ArrayOf<String>& data)
{
    mShortMonths = data.Clone();
    return NOERROR;
}

ECode DateFormatSymbols::SetShortWeekdays(
    /* [in] */ const ArrayOf<String>& data)
{
    mShortWeekdays = data.Clone();
    return NOERROR;
}

ECode DateFormatSymbols::SetWeekdays(
    /* [in] */ const ArrayOf<String>& data)
{
    mWeekdays = data.Clone();
    return NOERROR;
}

ECode DateFormatSymbols::SetZoneStrings(
    /* [in] */ ArrayOf<ArrayOf<String>*>* zoneStrings)
{
    if (zoneStrings == NULL) {
        //throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }

    for (Int32 i = 0; i < zoneStrings->GetLength(); i++) {
        if ((*zoneStrings)[i]->GetLength() < 5) {
            //throw new IllegalArgumentException(Arrays.toString(row) + ".length < 5");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
    }
    TimeZones::Clone2dStringArray(zoneStrings, &mZoneStrings);
    mCustomZoneStrings = TRUE;
    return NOERROR;
}