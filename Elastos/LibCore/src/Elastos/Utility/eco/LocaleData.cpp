
#include "LocaleData.h"
#include "CLocale.h"
#include "ICU.h"

HashMap< String, AutoPtr<LocaleData> > LocaleData::sLocaleDataCache(11);
Mutex LocaleData::sLocaleDataCacheLock;

LocaleData::LocaleData()
    : mAmPm(NULL)
    , mEras(NULL)
    , mLongMonthNames(NULL)
    , mShortMonthNames(NULL)
    , mLongStandAloneMonthNames(NULL)
    , mShortStandAloneMonthNames(NULL)
    , mLongWeekdayNames(NULL)
    , mShortWeekdayNames(NULL)
    , mLongStandAloneWeekdayNames(NULL)
    , mShortStandAloneWeekdayNames(NULL)
{}

LocaleData::~LocaleData()
{
    FREE_ARRAY_OF_STRING(mAmPm);
    FREE_ARRAY_OF_STRING(mEras);
    FREE_ARRAY_OF_STRING(mLongMonthNames);
    FREE_ARRAY_OF_STRING(mShortMonthNames);
    FREE_ARRAY_OF_STRING(mLongStandAloneMonthNames);
    FREE_ARRAY_OF_STRING(mShortStandAloneMonthNames);
    FREE_ARRAY_OF_STRING(mLongWeekdayNames);
    FREE_ARRAY_OF_STRING(mShortWeekdayNames);
    FREE_ARRAY_OF_STRING(mLongStandAloneWeekdayNames);
    FREE_ARRAY_OF_STRING(mShortStandAloneWeekdayNames);
}

PInterface LocaleData::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(ILocaleData*)this;
    }
    else if (riid == EIID_ILocaleData) {
        return (ILocaleData*)this;
    }

    return NULL;
}

UInt32 LocaleData::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 LocaleData::Release()
{
    return ElRefBase::Release();
}

ECode LocaleData::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(ILocaleData*)this) {
        *pIID = EIID_ILocaleData;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

AutoPtr<ILocaleData> LocaleData::Get(
    /* [in] */ ILocale* _locale)
{
    AutoPtr<ILocale> locale = _locale;
    if (locale == NULL) {
        locale = CLocale::GetDefault();
    }

    String localeName;
    locale->ToString(&localeName);
    {
        Mutex::Autolock lock(&sLocaleDataCacheLock);

        HashMap< String, AutoPtr<LocaleData> >::Iterator it =
                sLocaleDataCache.Find(localeName);
        if (it != sLocaleDataCache.End()) {
            return (ILocaleData*)it->mSecond.Get();
        }
    }

    AutoPtr<LocaleData> newLocaleData = MakeLocaleData(locale);
    {
        Mutex::Autolock lock(&sLocaleDataCacheLock);

        HashMap< String, AutoPtr<LocaleData> >::Iterator it =
                sLocaleDataCache.Find(localeName);
        if (it != sLocaleDataCache.End()) {
            return (ILocaleData*)it->mSecond.Get();
        }
        sLocaleDataCache[localeName] = newLocaleData;
        return (ILocaleData*)newLocaleData.Get();
    }
}

AutoPtr<LocaleData> LocaleData::MakeLocaleData(
    /* [in] */ ILocale* locale)
{
    String language;
    locale->GetLanguage(&language);
    String country;
    locale->GetCountry(&country);
    String variant;
    locale->GetVariant(&variant);
    // Start with data from the parent (next-most-specific) locale...
    LocaleData* result = new LocaleData();
    AutoPtr<ILocale> l;
    if (!variant.IsEmpty()) {
        CLocale::New(language, country, String(""), (ILocale**)&l);
        result->OverrideWithDataFrom(Get(l));
    }
    else if (!country.IsEmpty()) {
        CLocale::New(language, String(""), String(""), (ILocale**)&l);
        result->OverrideWithDataFrom(Get(l));
    }
    else if(!language.IsEmpty()) {
        result->OverrideWithDataFrom(Get(CLocale::ROOT));
    }
    // Override with data from this locale.
    result->OverrideWithDataFrom(InitLocaleData(locale));
    return result;
}

String LocaleData::ToString()
{
    return String("LocaleData");
    // return "LocaleData[" +
    //             "firstDayOfWeek=" + firstDayOfWeek + "," +
    //             "minimalDaysInFirstWeek=" + minimalDaysInFirstWeek + "," +
    //             "amPm=" + Arrays.toString(amPm) + "," +
    //             "eras=" + Arrays.toString(eras) + "," +
    //             "longMonthNames=" + Arrays.toString(longMonthNames) + "," +
    //             "shortMonthNames=" + Arrays.toString(shortMonthNames) + "," +
    //             "longStandAloneMonthNames=" + Arrays.toString(longStandAloneMonthNames) + "," +
    //             "shortStandAloneMonthNames=" + Arrays.toString(shortStandAloneMonthNames) + "," +
    //             "longWeekdayNames=" + Arrays.toString(longWeekdayNames) + "," +
    //             "shortWeekdayNames=" + Arrays.toString(shortWeekdayNames) + "," +
    //             "longStandAloneWeekdayNames=" + Arrays.toString(longStandAloneWeekdayNames) + "," +
    //             "shortStandAloneWeekdayNames=" + Arrays.toString(shortStandAloneWeekdayNames) + "," +
    //             "fullTimeFormat=" + fullTimeFormat + "," +
    //             "longTimeFormat=" + longTimeFormat + "," +
    //             "mediumTimeFormat=" + mediumTimeFormat + "," +
    //             "shortTimeFormat=" + shortTimeFormat + "," +
    //             "fullDateFormat=" + fullDateFormat + "," +
    //             "longDateFormat=" + longDateFormat + "," +
    //             "mediumDateFormat=" + mediumDateFormat + "," +
    //             "shortDateFormat=" + shortDateFormat + "," +
    //             "zeroDigit=" + zeroDigit + "," +
    //             "digit=" + digit + "," +
    //             "decimalSeparator=" + decimalSeparator + "," +
    //             "groupingSeparator=" + groupingSeparator + "," +
    //             "patternSeparator=" + patternSeparator + "," +
    //             "percent=" + percent + "," +
    //             "perMill=" + perMill + "," +
    //             "monetarySeparator=" + monetarySeparator + "," +
    //             "minusSign=" + minusSign + "," +
    //             "exponentSeparator=" + exponentSeparator + "," +
    //             "infinity=" + infinity + "," +
    //             "NaN=" + NaN + "," +
    //             "currencySymbol=" + currencySymbol + "," +
    //             "internationalCurrencySymbol=" + internationalCurrencySymbol + "," +
    //             "numberPattern=" + numberPattern + "," +
    //             "integerPattern=" + integerPattern + "," +
    //             "currencyPattern=" + currencyPattern + "," +
    //             "percentPattern=" + percentPattern + "]";
}

void LocaleData::OverrideWithDataFrom(
    /* [in] */ ILocaleData* _overrides)
{
    LocaleData* overrides = (LocaleData*)_overrides;
    if (overrides->mFirstDayOfWeek != NULL) {
        mFirstDayOfWeek = overrides->mFirstDayOfWeek;
    }
    if (overrides->mMinimalDaysInFirstWeek != NULL) {
        mMinimalDaysInFirstWeek = overrides->mMinimalDaysInFirstWeek;
    }
    if (overrides->mAmPm != NULL) {
        FREE_ARRAY_OF_STRING(mAmPm);
        CLONE_ARRAY_OF_STRING(&mAmPm, overrides->mAmPm);
    }
    if (overrides->mEras != NULL) {
        FREE_ARRAY_OF_STRING(mEras);
        CLONE_ARRAY_OF_STRING(&mEras, overrides->mEras);
    }
    if (overrides->mLongMonthNames != NULL) {
        FREE_ARRAY_OF_STRING(mLongMonthNames);
        CLONE_ARRAY_OF_STRING(&mLongMonthNames, overrides->mLongMonthNames);
    }
    if (overrides->mShortMonthNames != NULL) {
        FREE_ARRAY_OF_STRING(mShortMonthNames);
        CLONE_ARRAY_OF_STRING(&mShortMonthNames, overrides->mShortMonthNames);
    }
    if (overrides->mLongStandAloneMonthNames != NULL) {
        FREE_ARRAY_OF_STRING(mLongStandAloneMonthNames);
        CLONE_ARRAY_OF_STRING(&mLongStandAloneMonthNames, overrides->mLongStandAloneMonthNames);
    }
    if (overrides->mShortStandAloneMonthNames != NULL) {
        FREE_ARRAY_OF_STRING(mShortStandAloneMonthNames);
        CLONE_ARRAY_OF_STRING(&mShortStandAloneMonthNames, overrides->mShortStandAloneMonthNames);
    }
    if (overrides->mLongWeekdayNames != NULL) {
        FREE_ARRAY_OF_STRING(mLongWeekdayNames);
        CLONE_ARRAY_OF_STRING(&mLongWeekdayNames, overrides->mLongWeekdayNames);
    }
    if (overrides->mShortWeekdayNames != NULL) {
        FREE_ARRAY_OF_STRING(mShortWeekdayNames);
        CLONE_ARRAY_OF_STRING(&mShortWeekdayNames, overrides->mShortWeekdayNames);
    }
    if (overrides->mLongStandAloneWeekdayNames != NULL) {
        FREE_ARRAY_OF_STRING(mLongStandAloneWeekdayNames);
        CLONE_ARRAY_OF_STRING(&mLongStandAloneWeekdayNames, overrides->mLongStandAloneWeekdayNames);
    }
    if (overrides->mShortStandAloneWeekdayNames != NULL) {
        FREE_ARRAY_OF_STRING(mShortStandAloneWeekdayNames);
        CLONE_ARRAY_OF_STRING(&mShortStandAloneWeekdayNames, overrides->mShortStandAloneWeekdayNames);
    }
    if (!overrides->mFullTimeFormat.IsNull()) {
        mFullTimeFormat = overrides->mFullTimeFormat;
    }
    if (!overrides->mLongTimeFormat.IsNull()) {
        mLongTimeFormat = overrides->mLongTimeFormat;
    }
    if (!overrides->mMediumTimeFormat.IsNull()) {
        mMediumTimeFormat = overrides->mMediumTimeFormat;
    }
    if (!overrides->mShortTimeFormat.IsNull()) {
        mShortTimeFormat = overrides->mShortTimeFormat;
    }
    if (!overrides->mFullDateFormat.IsNull()) {
        mFullDateFormat = overrides->mFullDateFormat;
    }
    if (!overrides->mLongDateFormat.IsNull()) {
        mLongDateFormat = overrides->mLongDateFormat;
    }
    if (!overrides->mMediumDateFormat.IsNull()) {
        mMediumDateFormat = overrides->mMediumDateFormat;
    }
    if (!overrides->mShortDateFormat.IsNull()) {
        mShortDateFormat = overrides->mShortDateFormat;
    }
    if (overrides->mZeroDigit != '\0') {
        mZeroDigit = overrides->mZeroDigit;
    }
    if (overrides->mDigit != '\0') {
        mDigit = overrides->mDigit;
    }
    if (overrides->mDecimalSeparator != '\0') {
        mDecimalSeparator = overrides->mDecimalSeparator;
    }
    if (overrides->mGroupingSeparator != '\0') {
        mGroupingSeparator = overrides->mGroupingSeparator;
    }
    if (overrides->mPatternSeparator != '\0') {
        mPatternSeparator = overrides->mPatternSeparator;
    }
    if (overrides->mPercent != '\0') {
        mPercent = overrides->mPercent;
    }
    if (overrides->mPerMill != '\0') {
        mPerMill = overrides->mPerMill;
    }
    if (overrides->mMonetarySeparator != '\0') {
        mMonetarySeparator = overrides->mMonetarySeparator;
    }
    if (overrides->mMinusSign != '\0') {
        mMinusSign = overrides->mMinusSign;
    }
    if (!overrides->mExponentSeparator.IsNull()) {
        mExponentSeparator = overrides->mExponentSeparator;
    }
    if (!overrides->mNaN.IsNull()) {
        mNaN = overrides->mNaN;
    }
    if (!overrides->mInfinity.IsNull()) {
        mInfinity = overrides->mInfinity;
    }
    if (!overrides->mCurrencySymbol.IsNull()) {
        mCurrencySymbol = overrides->mCurrencySymbol;
    }
    if (!overrides->mInternationalCurrencySymbol.IsNull()) {
        mInternationalCurrencySymbol = overrides->mInternationalCurrencySymbol;
    }
    if (!overrides->mNumberPattern.IsNull()) {
        mNumberPattern = overrides->mNumberPattern;
    }
    if (!overrides->mIntegerPattern.IsNull()) {
        mIntegerPattern = overrides->mIntegerPattern;
    }
    if (!overrides->mCurrencyPattern.IsNull()) {
        mCurrencyPattern = overrides->mCurrencyPattern;
    }
    if (!overrides->mPercentPattern.IsNull()) {
        mPercentPattern = overrides->mPercentPattern;
    }
}

ECode LocaleData::GetDateFormat(
    /* [in] */ Int32 style,
    /* [out] */ String* format)
{
    VALIDATE_NOT_NULL(format);

    switch (style) {
    case 3/*IDateFormat_SHORT*/:
        *format = mShortDateFormat;
        return NOERROR;
    case 2/*IDateFormat_MEDIUM*/:
        *format = mMediumDateFormat;
        return NOERROR;
    case 1/*IDateFormat_LONG*/:
        *format = mLongDateFormat;
        return NOERROR;
    case 0/*IDateFormat_FULL*/:
        *format = mFullDateFormat;
        return NOERROR;
    }
    // throw new AssertionError();
    return E_ASSERTION_ERROR;
}

ECode LocaleData::GetTimeFormat(
    /* [in] */ Int32 style,
    /* [out] */ String* format)
{
    VALIDATE_NOT_NULL(format);

    switch (style) {
    case 3/*IDateFormat_SHORT*/:
        *format = mShortTimeFormat;
        return NOERROR;
    case 2/*IDateFormat_MEDIUM*/:
        *format = mMediumTimeFormat;
        return NOERROR;
    case 1/*IDateFormat_LONG*/:
        *format = mLongTimeFormat;
        return NOERROR;
    case 0/*IDateFormat_FULL*/:
        *format = mFullTimeFormat;
        return NOERROR;
    }
    // throw new AssertionError();
    return E_ASSERTION_ERROR;
}

ECode LocaleData::GetAmPm(
    /* [out] */ ArrayOf<String>** amPm)
{
    VALIDATE_NOT_NULL(amPm);
    *amPm = mAmPm;
    return NOERROR;
}

ECode LocaleData::GetEras(
    /* [out] */ ArrayOf<String>** eras)
{
    VALIDATE_NOT_NULL(eras);
    *eras = mEras;
    return NOERROR;
}

ECode LocaleData::GetLongMonthNames(
    /* [out] */ ArrayOf<String>** names)
{
    VALIDATE_NOT_NULL(names);
    *names = mLongMonthNames;
    return NOERROR;
}

ECode LocaleData::GetShortMonthNames(
    /* [out] */ ArrayOf<String>** names)
{
    VALIDATE_NOT_NULL(names);
    *names = mShortMonthNames;
    return NOERROR;
}

ECode LocaleData::GetLongStandAloneMonthNames(
    /* [out] */ ArrayOf<String>** names)
{
    VALIDATE_NOT_NULL(names);
    *names = mLongStandAloneMonthNames;
    return NOERROR;
}

ECode LocaleData::GetShortStandAloneMonthNames(
    /* [out] */ ArrayOf<String>** names)
{
    VALIDATE_NOT_NULL(names);
    *names = mShortStandAloneMonthNames;
    return NOERROR;
}

ECode LocaleData::GetLongWeekdayNames(
    /* [out] */ ArrayOf<String>** names)
{
    VALIDATE_NOT_NULL(names);
    *names = mLongWeekdayNames;
    return NOERROR;
}

ECode LocaleData::GetShortWeekdayNames(
    /* [out] */ ArrayOf<String>** names)
{
    VALIDATE_NOT_NULL(names);
    *names = mShortWeekdayNames;
    return NOERROR;
}

ECode LocaleData::GetLongStandAloneWeekdayNames(
    /* [out] */ ArrayOf<String>** names)
{
    VALIDATE_NOT_NULL(names);
    *names = mLongStandAloneWeekdayNames;
    return NOERROR;
}

ECode LocaleData::GetShortStandAloneWeekdayNames(
    /* [out] */ ArrayOf<String>** names)
{
    VALIDATE_NOT_NULL(names);
    *names = mShortStandAloneWeekdayNames;
    return NOERROR;
}

ECode LocaleData::GetZeroDigit(
    /* [out] */ Char32* zeroDigit)
{
    VALIDATE_NOT_NULL(zeroDigit);
    *zeroDigit = mZeroDigit;
    return NOERROR;
}

ECode LocaleData::GetDigit(
    /* [out] */ Char32* digit)
{
    VALIDATE_NOT_NULL(digit);
    *digit = mDigit;
    return NOERROR;
}

ECode LocaleData::GetDecimalSeparator(
    /* [out] */ Char32* decimalSeparator)
{
    VALIDATE_NOT_NULL(decimalSeparator);
    *decimalSeparator = mDecimalSeparator;
    return NOERROR;
}

ECode LocaleData::GetGroupingSeparator(
    /* [out] */ Char32* groupingSeparator)
{
    VALIDATE_NOT_NULL(groupingSeparator);
    *groupingSeparator = mGroupingSeparator;
    return NOERROR;
}

ECode LocaleData::GetPatternSeparator(
    /* [out] */ Char32* patternSeparator)
{
    VALIDATE_NOT_NULL(patternSeparator);
    *patternSeparator = mPatternSeparator;
    return NOERROR;
}

ECode LocaleData::GetPercent(
    /* [out] */ Char32* percent)
{
    VALIDATE_NOT_NULL(percent);
    *percent = mPercent;
    return NOERROR;
}

ECode LocaleData::GetPerMill(
    /* [out] */ Char32* perMill)
{
    VALIDATE_NOT_NULL(perMill);
    *perMill = mPerMill;
    return NOERROR;
}

ECode LocaleData::GetMonetarySeparator(
    /* [out] */ Char32* monetarySeparator)
{
    VALIDATE_NOT_NULL(monetarySeparator);
    *monetarySeparator = mMonetarySeparator;
    return NOERROR;
}

ECode LocaleData::GetMinusSign(
    /* [out] */ Char32* minusSign)
{
    VALIDATE_NOT_NULL(minusSign);
    *minusSign = mMinusSign;
    return NOERROR;
}

ECode LocaleData::GetExponentSeparator(
    /* [out] */ String* exponentSeparator)
{
    VALIDATE_NOT_NULL(exponentSeparator);
    *exponentSeparator = mExponentSeparator;
    return NOERROR;
}

ECode LocaleData::GetInfinity(
    /* [out] */ String* infinity)
{
    VALIDATE_NOT_NULL(infinity);
    *infinity = mInfinity;
    return NOERROR;
}

ECode LocaleData::GetNaN(
    /* [out] */ String* naN)
{
    VALIDATE_NOT_NULL(naN);
    *naN = mNaN;
    return NOERROR;
}

ECode LocaleData::GetCurrencySymbol(
    /* [out] */ String* currencySymbol)
{
    VALIDATE_NOT_NULL(currencySymbol);
    *currencySymbol = mCurrencySymbol;
    return NOERROR;
}

ECode LocaleData::GetInternationalCurrencySymbol(
    /* [out] */ String* internationalCurrencySymbol)
{
    VALIDATE_NOT_NULL(internationalCurrencySymbol);
    *internationalCurrencySymbol = mInternationalCurrencySymbol;
    return NOERROR;
}

ECode LocaleData::GetNumberPattern(
    /* [out] */ String* numberPattern)
{
    VALIDATE_NOT_NULL(numberPattern);
    *numberPattern = mNumberPattern;
    return NOERROR;
}

ECode LocaleData::GetCurrencyPattern(
    /* [out] */ String* currencyPattern)
{
    VALIDATE_NOT_NULL(currencyPattern);
    *currencyPattern = mCurrencyPattern;
    return NOERROR;
}

ECode LocaleData::GetPercentPattern(
    /* [out] */ String* percentPattern)
{
    VALIDATE_NOT_NULL(percentPattern);
    *percentPattern = mPercentPattern;
    return NOERROR;
}

AutoPtr<ILocaleData> LocaleData::InitLocaleData(
    /* [in] */ ILocale* locale)
{
    LocaleData* localeData = new LocaleData();
    String localeName;
    locale->ToString(&localeName);
    if (!ICU::InitLocaleDataImpl(localeName, localeData)) {
        return NULL;
    }
    if (!localeData->mFullTimeFormat.IsNull()) {
        // There are some full time format patterns in ICU that use the pattern character 'v'.
        // Java doesn't accept this, so we replace it with 'z' which has about the same result
        // as 'v', the timezone name.
        // 'v' -> "PT", 'z' -> "PST", v is the generic timezone and z the standard tz
        // "vvvv" -> "Pacific Time", "zzzz" -> "Pacific Standard Time"
        localeData->mFullTimeFormat = (localeData->mFullTimeFormat).Replace("v", "z");
    }
    if (!localeData->mNumberPattern.IsNull()) {
        // The number pattern might contain positive and negative subpatterns. Arabic, for
        // example, might look like "#,##0.###;#,##0.###-" because the minus sign should be
        // written last. Macedonian supposedly looks something like "#,##0.###;(#,##0.###)".
        // (The negative subpattern is optional, though, and not present in most locales.)
        // By only swallowing '#'es and ','s after the '.', we ensure that we don't
        // accidentally eat too much.
        //localeData->mIntegerPattern = localeData->mNumberPattern.ReplaceAll("\\.[#,]*", "");
    }
    return (ILocaleData*)localeData;
}