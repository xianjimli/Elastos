#include "LocaleData.h"
#include "Locale.h"
#include "ICU.h"

HashMap<String, LocaleData*>* LocaleData::mLocaleDataCache = new HashMap<String, LocaleData*>();

LocaleData::LocaleData()
{
}

LocaleData* LocaleData::Get(ILocale *locale)
{
    if (locale == NULL) {
        Locale::GetDefault((ILocale**) &locale);
    }

    String localeName = ((CLocale*)locale)->ToString();

//    synchronized (localeDataCache) {
    LocaleData* localeData = (*mLocaleDataCache)[localeName];
    if (localeData != NULL) {
        return localeData;
    }
//    }

    LocaleData* newLocaleData = MakeLocaleData(locale);
//    synchronized (localeDataCache) {
    localeData = (*mLocaleDataCache)[localeName];
    if (localeData != NULL) {
        return localeData;
    }

    (*mLocaleDataCache)[localeName] = newLocaleData;
//    mLocaleDataCache->Put(localeName, newLocaleData);
    return newLocaleData;
//    }
}

String LocaleData::ToString()
{
}

String LocaleData::GetDateFormat(
    /* [in] */ Int32 style)
{
//    switch (style) {
//    case DateFormat.SHORT:
//        return shortDateFormat;
//    case DateFormat.MEDIUM:
//        return mediumDateFormat;
//    case DateFormat.LONG:
//        return longDateFormat;
//    case DateFormat.FULL:
//        return fullDateFormat;
//    }
    return String();
}


String LocaleData::GetTimeFormat(
    /* [in] */ Int32 style)
{
    switch (style) {
//    case DateFormat.SHORT:
//        return shortTimeFormat;
//    case DateFormat.MEDIUM:
//        return mediumTimeFormat;
//    case DateFormat.LONG:
//        return longTimeFormat;
//    case DateFormat.FULL:
//        return fullTimeFormat;
    }
    return String();
}

LocaleData* LocaleData::MakeLocaleData(
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

    AutoPtr<ILocale> loc;
    if (!variant.IsEmpty()) {
        CLocale::New(language, country, String(""), (ILocale**)&loc);
        result->OverrideWithDataFrom(Get((ILocale *)loc));
    } else if (!country.IsEmpty()) {
        CLocale::New(language, String(""), String(""), (ILocale**)&loc);
        result->OverrideWithDataFrom(Get((ILocale *)loc));
    } else if(!language.IsEmpty()) {
        //result->OverrideWithDataFrom(Get((ILocale*)((CLocale *)Locale)-?ROOT));
    }
    // Override with data from this locale.
    result->OverrideWithDataFrom(InitLocaleData(locale));
    return result;
}


void LocaleData::OverrideWithDataFrom(
    /* [in] */ LocaleData* overrides)
{
    if (!overrides->mFirstDayOfWeek) {
        mFirstDayOfWeek = overrides->mFirstDayOfWeek;
    }
    if (!overrides->mMinimalDaysInFirstWeek) {
        mMinimalDaysInFirstWeek = overrides->mMinimalDaysInFirstWeek;
    }
    if (overrides->mAmPm != NULL) {
        mAmPm = overrides->mAmPm;
    }
    if (overrides->mEras != NULL) {
        mEras = overrides->mEras;
    }
    if (overrides->mLongMonthNames != NULL) {
        mLongMonthNames = overrides->mLongMonthNames;
    }
    if (overrides->mShortMonthNames != NULL) {
        mShortMonthNames = overrides->mShortMonthNames;
    }
    if (overrides->mLongStandAloneMonthNames != NULL) {
        mLongStandAloneMonthNames = overrides->mLongStandAloneMonthNames;
    }
    if (overrides->mShortStandAloneMonthNames != NULL) {
        mShortStandAloneMonthNames = overrides->mShortStandAloneMonthNames;
    }
    if (overrides->mLongWeekdayNames != NULL) {
        mLongWeekdayNames = overrides->mLongWeekdayNames;
    }
    if (overrides->mShortWeekdayNames != NULL) {
        mShortWeekdayNames = overrides->mShortWeekdayNames;
    }
    if (overrides->mLongStandAloneWeekdayNames != NULL) {
        mLongStandAloneWeekdayNames = overrides->mLongStandAloneWeekdayNames;
    }
    if (overrides->mShortStandAloneWeekdayNames != NULL) {
        mShortStandAloneWeekdayNames = overrides->mShortStandAloneWeekdayNames;
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

LocaleData* LocaleData::InitLocaleData(
    /* [in] */ ILocale* locale)
{
    LocaleData* localeData = new LocaleData();
//    if (!ICU::InitLocaleDataImpl(((CLocale*)locale)->ToString(), localeData)) {
//        return NULL;
//    }

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
    return localeData;
}