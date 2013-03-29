
#include "cmdef.h"
#include "DateFormatSymbols.h"

ECode DateFormatSymbols::InternalZoneStrings(
        /* [out, callee] */ ArrayOf<ArrayOf<String> * > ** zoneStrings)
{
    //Mutex::Autolock lock(mLock);

    if (mZoneStrings == NULL) {
//        mZoneStrings = TimeZones.getZoneStrings(locale);
    }
    *zoneStrings = mZoneStrings->Clone();
    return NOERROR;
}

ECode DateFormatSymbols::Init()
{
    AutoPtr<ILocaleHelper> pLocaleHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&pLocaleHelper);
    AutoPtr<ILocale> pLocale;
    pLocaleHelper->GetDefault((ILocale**)&pLocale);
    return Init((ILocale*)pLocale);
}

ECode DateFormatSymbols::Init(
        /* [in] */ ILocale* locale)
{
    mLocale = locale;
    String SimpleDateFormat_PATTERN_CHARS = String("GyMdkHmsSEDFwWahKzZLc");
    mLocalPatternChars = SimpleDateFormat_PATTERN_CHARS;
/*    LocaleData *localeData = LocaleData::Get(locale);
    mAmpms = (localeData->mAmPm)->Clone();
    mEras = (localeData->mEras)->Clone();
    mMonths = (localeData->mLongMonthNames)->Clone();
    mShortMonths = (localeData->mShortMonthNames)->Clone();
    mWeekdays = (localeData->mLongWeekdayNames)->Clone();
    mShortWeekdays = (localeData->mShortWeekdayNames)->Clone();

    // ICU/Android extensions.
    mLongStandAloneMonths = (localeData->mLongStandAloneMonthNames)->Clone();
    mShortStandAloneMonths = (localeData->mShortStandAloneMonthNames)->Clone();
    mLongStandAloneWeekdays = (localeData->mLongStandAloneWeekdayNames)->Clone();
    mShortStandAloneWeekdays = (localeData->mShortStandAloneWeekdayNames)->Clone();*/
    return NOERROR;
}

DateFormatSymbols::~DateFormatSymbols()
{
    if (mAmpms != NULL) {
        ArrayOf<String>::Free(mAmpms);
    }
    if (mEras != NULL) {
        ArrayOf<String>::Free(mEras);
    }
    if (mMonths != NULL) {
        ArrayOf<String>::Free(mMonths);
    }
    if (mShortMonths != NULL) {
        ArrayOf<String>::Free(mShortMonths);
    }
    if (mShortWeekdays != NULL) {
        ArrayOf<String>::Free(mShortWeekdays);
    }
    if (mWeekdays != NULL) {
        ArrayOf<String>::Free(mWeekdays);
    }
    if (mLongStandAloneMonths != NULL) {
        ArrayOf<String>::Free(mLongStandAloneMonths);
    }
    if (mShortStandAloneMonths != NULL) {
        ArrayOf<String>::Free(mShortStandAloneMonths);
    }
    if (mLongStandAloneWeekdays != NULL) {
        ArrayOf<String>::Free(mLongStandAloneWeekdays);
    }
    if (mShortStandAloneWeekdays != NULL) {
        ArrayOf<String>::Free(mShortStandAloneWeekdays);
    }
    if (mZoneStrings != NULL) {
        ArrayOf<ArrayOf<String> * >::Free(mZoneStrings);
    }
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
/*    // Quick check that may keep us from having to load the zone strings.
    // Note that different locales may have the same strings, so the opposite check isn't valid.
    if (lhs.zoneStrings == null && rhs.zoneStrings == null && lhs.locale.equals(rhs.locale)) {
        return true;
    }
    // Make sure zone strings are loaded, then check.
    return Arrays.deepEquals(lhs.internalZoneStrings(), rhs.internalZoneStrings());
*/
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
        /* [out, callee] */ ArrayOf<String> ** arrayOfStrings)
{
    *arrayOfStrings = mAmpms->Clone();
    return NOERROR;
}

ECode DateFormatSymbols::GetEras(
        /* [out, callee] */ ArrayOf<String> ** arrayOfStrings)
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
        /* [out, callee] */ ArrayOf<ArrayOf<String> * > ** zoneStrings)
{
//    return TimeZones.clone2dStringArray(internalZoneStrings());
    return NOERROR;
}

ECode DateFormatSymbols::SetAmPmStrings(
        /* [in] */ ArrayOf<String>* data)
{
    mAmpms = data->Clone();
    return NOERROR;
}

ECode DateFormatSymbols::SetEras(
        /* [in] */ ArrayOf<String>* data)
{
    mEras = data->Clone();
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
        /* [in] */ ArrayOf<String>* data)
{
    mMonths = data->Clone();
    return NOERROR;
}

ECode DateFormatSymbols::SetShortMonths(
        /* [in] */ ArrayOf<String>* data)
{
    mShortMonths = data->Clone();
    return NOERROR;
}

ECode DateFormatSymbols::SetShortWeekdays(
        /* [in] */ ArrayOf<String>* data)
{
    mShortWeekdays = data->Clone();
    return NOERROR;
}

ECode DateFormatSymbols::SetWeekdays(
        /* [in] */ ArrayOf<String>* data)
{
    mWeekdays = data->Clone();
    return NOERROR;
}

ECode DateFormatSymbols::SetZoneStrings(
        /* [in] */ ArrayOf<ArrayOf<String> *>* zoneStrings)
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

//    this.zoneStrings = TimeZones.clone2dStringArray(zoneStrings);
    mCustomZoneStrings = TRUE;
    return NOERROR;
}