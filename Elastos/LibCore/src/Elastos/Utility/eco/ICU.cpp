
#include "cmdef.h"
#include "ElStringByteSink.h"
#include "ICU.h"
#include "CLocale.h"
#include <Elastos.Core.h>

#include <unicode/ucat.h>
#include <unicode/ureslocs.h>
// #include <unicode/calendar.h>
// #include <unicode/datefmt.h>
// #include <unicode/dcfmtsym.h>
// #include <unicode/decimfmt.h>
// #include <unicode/dtfmtsym.h>
// #include <unicode/gregocal.h>
#include <unicode/locid.h>
#include <unicode/numfmt.h>
#include <unicode/strenum.h>
#include <unicode/ubrk.h>
#include <unicode/ucal.h>
#include <unicode/uclean.h>
#include <unicode/ucol.h>
// #include <unicode/ucurr.h>
#include <unicode/udat.h>
#include <unicode/ustring.h>
#include <unicode/unum.h>

class ScopedResourceBundle
{
public:
    ScopedResourceBundle(UResourceBundle* bundle)
        : mBundle(bundle)
    {}

    ~ScopedResourceBundle()
    {
        if (mBundle != NULL) {
            ures_close(mBundle);
        }
    }

    UResourceBundle* get()
    {
        return mBundle;
    }

private:
    UResourceBundle* mBundle;

    // Disallow copy and assignment.
    ScopedResourceBundle(const ScopedResourceBundle&);
    void operator=(const ScopedResourceBundle&);
};

Locale GetLocale(
    /* [in] */ const String& localeName)
{
    return Locale::createFromName(localeName.string());
}

/**
 * Cache for ISO language names.
 */
ArrayOf<String>* ICU::sIsoLanguages = NULL;

/**
 * Cache for ISO country names.
 */
ArrayOf<String>* ICU::sIsoCountries = NULL;

ArrayOf< AutoPtr<ILocale> >* ICU::sAvailableLocalesCache = NULL;


ECode ICU::GetISOLanguages(
    /* [out, callee] */ ArrayOf<String>** languages)
{
    VALIDATE_NOT_NULL(languages);

    if (sIsoLanguages == NULL) {
        sIsoLanguages = GetISOLanguagesNative();
    }
    *languages = ArrayOf<String>::Alloc(sIsoLanguages->GetLength());
    for (Int32 i = 0; i < sIsoLanguages->GetLength(); ++i) {
        (**languages)[i] = (*sIsoLanguages)[i];
    }
    return NOERROR;
}

ECode ICU::GetISOCountries(
    /* [out, callee] */ ArrayOf<String>** countries)
{
    VALIDATE_NOT_NULL(countries);

    if (sIsoCountries == NULL) {
        sIsoCountries = GetISOCountriesNative();
    }
    *countries = ArrayOf<String>::Alloc(sIsoCountries->GetLength());
    for (Int32 i = 0; i < sIsoCountries->GetLength(); ++i) {
        (**countries)[i] = (*sIsoCountries)[i];
    }
    return NOERROR;
}

ECode ICU::LocaleFromString(
    /* [in] */ const String& localeName,
    /* [out] */ ILocale** locale)
{
    VALIDATE_NOT_NULL(locale);

    Int32 first = localeName.IndexOf('_');
    Int32 second = localeName.IndexOf('_', first + 1);
    if (first == -1) {
        // Language only ("ja").
        return CLocale::New(localeName, locale);
    }
    else if (second == -1) {
        // Language and country ("ja_JP").
        return CLocale::New(localeName.Substring(0, first), localeName.Substring(first + 1), locale);
    }
    else {
        // Language and country and variant ("ja_JP_TRADITIONAL").
        return CLocale::New(localeName.Substring(0, first), localeName.Substring(first + 1, second),
                localeName.Substring(second + 1), locale);
    }
}

ECode ICU::LocalesFromStrings(
    /* [in] */ const ArrayOf<String>& localeNames,
    /* [out, callee] */ ArrayOf<ILocale*>** locales)
{
    VALIDATE_NOT_NULL(locales);

    *locales = ArrayOf<ILocale*>::Alloc(localeNames.GetLength());
    for (Int32 i = 0; i < localeNames.GetLength(); ++i) {
        FAIL_RETURN(LocaleFromString(localeNames[i], &(**locales)[i]));
    }
    return NOERROR;
}

ECode ICU::GetAvailableLocales(
    /* [out, callee] */ ArrayOf<ILocale*>** locales)
{
    VALIDATE_NOT_NULL(locales);

    if (sAvailableLocalesCache == NULL) {
        ArrayOf<String>* _locales;
        FAIL_RETURN(GetAvailableLocalesNative(&_locales));
        FAIL_RETURN(LocalesFromStrings(*_locales, (ArrayOf<ILocale*>**)&sAvailableLocalesCache));
        //free _locales;
        FREE_ARRAY_OF_STRING(_locales);
    }
    *locales = ArrayOf<ILocale*>::Alloc(sAvailableLocalesCache->GetLength());
    for (Int32 i = 0; i < sAvailableLocalesCache->GetLength(); ++i) {
        (**locales)[i] = (*sAvailableLocalesCache)[i];
        if ((**locales)[i] != NULL) (**locales)[i]->AddRef();
    }
    return NOERROR;
}

ECode ICU::GetAvailableBreakIteratorLocales(
    /* [out, callee] */ ArrayOf<ILocale*>** locales)
{
    VALIDATE_NOT_NULL(locales);

    ArrayOf<String>* _locales;
    FAIL_RETURN(GetAvailableBreakIteratorLocalesNative(&_locales));
    ECode ec = LocalesFromStrings(*_locales, locales);
    FREE_ARRAY_OF_STRING(_locales);
    return ec;
}

ECode ICU::GetAvailableCalendarLocales(
    /* [out, callee] */ ArrayOf<ILocale*>** locales)
{
    VALIDATE_NOT_NULL(locales);

    ArrayOf<String>* _locales;
    FAIL_RETURN(GetAvailableCalendarLocalesNative(&_locales));
    ECode ec = LocalesFromStrings(*_locales, locales);
    FREE_ARRAY_OF_STRING(_locales);
    return ec;
}

ECode ICU::GetAvailableCollatorLocales(
    /* [out, callee] */ ArrayOf<ILocale*>** locales)
{
    VALIDATE_NOT_NULL(locales);

    ArrayOf<String>* _locales;
    FAIL_RETURN(GetAvailableCollatorLocalesNative(&_locales));
    ECode ec = LocalesFromStrings(*_locales, locales);
    FREE_ARRAY_OF_STRING(_locales);
    return ec;
}

ECode ICU::GetAvailableDateFormatLocales(
    /* [out, callee] */ ArrayOf<ILocale*>** locales)
{
    VALIDATE_NOT_NULL(locales);

    ArrayOf<String>* _locales;
    FAIL_RETURN(GetAvailableDateFormatLocalesNative(&_locales));
    ECode ec = LocalesFromStrings(*_locales, locales);
    FREE_ARRAY_OF_STRING(_locales);
    return ec;
}

ECode ICU::GetAvailableDateFormatSymbolsLocales(
    /* [out, callee] */ ArrayOf<ILocale*>** locales)
{
    return GetAvailableDateFormatLocales(locales);
}

ECode ICU::GetAvailableDecimalFormatSymbolsLocales(
    /* [out, callee] */ ArrayOf<ILocale*>** locales)
{
    return GetAvailableNumberFormatLocales(locales);
}

ECode ICU::GetAvailableNumberFormatLocales(
    /* [out, callee] */ ArrayOf<ILocale*>** locales)
{
    VALIDATE_NOT_NULL(locales);

    ArrayOf<String>* _locales;
    FAIL_RETURN(GetAvailableNumberFormatLocalesNative(&_locales));
    ECode ec = LocalesFromStrings(*_locales, locales);
    FREE_ARRAY_OF_STRING(_locales);
    return ec;
}

String ICU::ToLowerCase(
    /* [in] */ const String& s,
    /* [in] */ const String& localname)
{
    UnicodeString _s = UnicodeString::fromUTF8(s.string());
    UnicodeString original(_s);
    _s.toLower(Locale::createFromName(localname.string()));
    if (_s == original) {
        return s;
    }
    else {
        String s;
        ElStringByteSink sink(&s);
        _s.toUTF8(sink);
        return s;
    }
}

String ICU::ToUpperCase(
    /* [in] */ const String& s,
    /* [in] */ const String& localname)
{
    UnicodeString _s = UnicodeString::fromUTF8(s.string());
    UnicodeString original(_s);
    _s.toUpper(Locale::createFromName(localname.string()));
    if (_s == original) {
        return s;
    }
    else {
        String s;
        ElStringByteSink sink(&s);
        _s.toUTF8(sink);
        return s;
    }
}

template <typename Counter, typename Getter>
static ECode getAvailableLocales(
    /* [in] */ Counter* counter,
    /* [in] */ Getter* getter,
    /* [out, callee] */ ArrayOf<String>** locales)
{
    assert(locales != NULL);

    Int32 count = (*counter)();
    *locales = ArrayOf<String>::Alloc(count);
    for (Int32 i = 0; i < count; ++i) {
        (**locales)[i] = (*getter)(i);
    }
    return NOERROR;
}

ECode ICU::GetAvailableBreakIteratorLocalesNative(
    /* [out, callee] */ ArrayOf<String>** locales)
{
    VALIDATE_NOT_NULL(locales);

    return getAvailableLocales(ubrk_countAvailable, ubrk_getAvailable, locales);
}

ECode ICU::GetAvailableCalendarLocalesNative(
    /* [out, callee] */ ArrayOf<String>** locales)
{
    VALIDATE_NOT_NULL(locales);

    return getAvailableLocales(ucal_countAvailable, ucal_getAvailable, locales);
}

ECode ICU::GetAvailableCollatorLocalesNative(
    /* [out, callee] */ ArrayOf<String>** locales)
{
    VALIDATE_NOT_NULL(locales);

    return getAvailableLocales(ucol_countAvailable, ucol_getAvailable, locales);
}

ECode ICU::GetAvailableDateFormatLocalesNative(
    /* [out, callee] */ ArrayOf<String>** locales)
{
    VALIDATE_NOT_NULL(locales);

    return getAvailableLocales(udat_countAvailable, udat_getAvailable, locales);
}

ECode ICU::GetAvailableLocalesNative(
    /* [out, callee] */ ArrayOf<String>** locales)
{
    VALIDATE_NOT_NULL(locales);

    return getAvailableLocales(uloc_countAvailable, uloc_getAvailable, locales);
}

ECode ICU::GetAvailableNumberFormatLocalesNative(
    /* [out, callee] */ ArrayOf<String>** locales)
{
    VALIDATE_NOT_NULL(locales);

    return getAvailableLocales(unum_countAvailable, unum_getAvailable, locales);
}

String ICU::GetCurrencyCode(
    /* [in] */ const String& locale)
{
    UErrorCode status = U_ZERO_ERROR;
    ScopedResourceBundle supplData(ures_openDirect(U_ICUDATA_CURR, "supplementalData", &status));
    if (U_FAILURE(status)) {
        return String(NULL);
    }

    ScopedResourceBundle currencyMap(ures_getByKey(supplData.get(), "CurrencyMap", NULL, &status));
    if (U_FAILURE(status)) {
        return String(NULL);
    }

    ScopedResourceBundle currency(ures_getByKey(currencyMap.get(), locale.string(), NULL, &status));
    if (U_FAILURE(status)) {
        return String(NULL);
    }

    ScopedResourceBundle currencyElem(ures_getByIndex(currency.get(), 0, NULL, &status));
    if (U_FAILURE(status)) {
        return String("None");
    }

    // check if there is a 'to' date. If there is, the currency isn't used anymore.
    ScopedResourceBundle currencyTo(ures_getByKey(currencyElem.get(), "to", NULL, &status));
    if (!U_FAILURE(status)) {
        // return and let the caller throw an exception
        return String(NULL);
    }
    // We need to reset 'status'. It works like errno in that ICU doesn't set it
    // to U_ZERO_ERROR on success: it only touches it on error, and the test
    // above means it now holds a failure code.
    status = U_ZERO_ERROR;

    ScopedResourceBundle currencyId(ures_getByKey(currencyElem.get(), "id", NULL, &status));
    if (U_FAILURE(status)) {
        // No id defined for this country
        return String("None");
    }

    char buffer[256];
    Int32 length;
    ures_getUTF8String(currencyId.get(), buffer, &length, TRUE, &status);
    if (U_FAILURE(status) || length == 0) {
        return String("None");
    }
    else {
        buffer[length] = '\0';
        return String(buffer);
    }
}

Int32 ICU::GetCurrencyFractionDigits(
    /* [in] */ const String& currencyCode)
{
    UErrorCode status = U_ZERO_ERROR;
    NumberFormat* fmt = NumberFormat::createCurrencyInstance(status);
    if (U_FAILURE(status)) {
        if (fmt != NULL) delete fmt;
        return -1;
    }

    UnicodeString _currencyCode = UnicodeString::fromUTF8(currencyCode.string());
    fmt->setCurrency(_currencyCode.getBuffer(), status);
    if (U_FAILURE(status)) {
        if (fmt != NULL) delete fmt;
        return -1;
    }
    // for CurrencyFormats the minimum and maximum fraction digits are the same.
    Int32 ret = fmt->getMinimumFractionDigits();
    if (fmt != NULL) delete fmt;
    return ret;
}

String ICU::GetCurrencySymbol(
    /* [in] */ const String& locale,
    /* [in] */ const String& currencyCode)
{
    UErrorCode status = U_ZERO_ERROR;
    ScopedResourceBundle currLoc(ures_open(U_ICUDATA_CURR, locale.string(), &status));
    if (U_FAILURE(status)) {
        return String(NULL);
    }

    ScopedResourceBundle currencies(ures_getByKey(currLoc.get(), "Currencies", NULL, &status));
    if (U_FAILURE(status)) {
        return String(NULL);
    }

    ScopedResourceBundle currencyElems(ures_getByKey(currencies.get(), currencyCode.string(), NULL, &status));
    if (U_FAILURE(status)) {
        return String(NULL);
    }

    char buffer[256];
    Int32 length;
    ures_getUTF8StringByIndex(currencyElems.get(), 0, buffer, &length, TRUE, &status);
    if (U_FAILURE(status) || length == 0) {
        return String(NULL);
    }
    else {
        buffer[length] = '\0';
        return String(buffer);
    }
}

String ICU::GetDisplayCountry(
    /* [in] */ const String& countryCode,
    /* [in] */ const String& locale)
{
    Locale loc = GetLocale(locale);
    Locale targetLoc = GetLocale(countryCode);
    UnicodeString str;
    targetLoc.getDisplayCountry(loc, str);

    String s;
    ElStringByteSink sink(&s);
    str.toUTF8(sink);
    return s;
}

String ICU::GetDisplayLanguage(
    /* [in] */ const String& languageCode,
    /* [in] */ const String& locale)
{
    Locale loc = GetLocale(locale);
    Locale targetLoc = GetLocale(languageCode);
    UnicodeString str;
    targetLoc.getDisplayLanguage(loc, str);

    String s;
    ElStringByteSink sink(&s);
    str.toUTF8(sink);
    return s;
}

String ICU::GetDisplayVariant(
    /* [in] */ const String& variantCode,
    /* [in] */ const String& locale)
{
    Locale loc = GetLocale(locale);
    Locale targetLoc = GetLocale(variantCode);
    UnicodeString str;
    targetLoc.getDisplayVariant(loc, str);

    String s;
    ElStringByteSink sink(&s);
    str.toUTF8(sink);
    return s;
}

String ICU::GetISO3Country(
    /* [in] */ const String& locale)
{
    Locale loc = GetLocale(locale);
    return String(loc.getISO3Country());
}

String ICU::GetISO3Language(
    /* [in] */ const String& locale)
{
    Locale loc = GetLocale(locale);
    return String(loc.getISO3Language());
}

static ArrayOf<String>* ToStringArray(const char* const* strings)
{
    Int32 count = 0;
    while (strings[count] != NULL) {
        ++count;
    }
    ArrayOf<String>* result = ArrayOf<String>::Alloc(count);
    for (Int32 i = 0; i < count; ++i) {
        (*result)[i] = strings[i];
    }
    return result;
}

ArrayOf<String>* ICU::GetISOLanguagesNative()
{
    return ToStringArray(Locale::getISOLanguages());
}

ArrayOf<String>* ICU::GetISOCountriesNative()
{
    return ToStringArray(Locale::getISOCountries());
}

Boolean ICU::InitLocaleDataImpl(
    /* [in] */ const String& locale,
    /* [in] */ LocaleData* result)
{
    // ScopedUtfChars localeName(env, locale);
    // UErrorCode status = U_ZERO_ERROR;
    // ScopedResourceBundle root(ures_open(NULL, localeName.c_str(), &status));
    // if (U_FAILURE(status)) {
    //     LOGE("Error getting ICU resource bundle: %s", u_errorName(status));
    //     status = U_ZERO_ERROR;
    //     return JNI_FALSE;
    // }

    // ScopedResourceBundle calendar(ures_getByKey(root.get(), "calendar", NULL, &status));
    // if (U_FAILURE(status)) {
    //     LOGE("Error getting ICU calendar resource bundle: %s", u_errorName(status));
    //     return JNI_FALSE;
    // }

    // ScopedResourceBundle gregorian(ures_getByKey(calendar.get(), "gregorian", NULL, &status));
    // if (U_FAILURE(status)) {
    //     LOGE("Error getting ICU gregorian resource bundle: %s", u_errorName(status));
    //     return JNI_FALSE;
    // }

    // int firstDayVals[] = { 0, 0 };
    // if (getDayIntVector(env, gregorian.get(), firstDayVals)) {
    //     setIntegerField(env, localeData, "firstDayOfWeek", firstDayVals[0]);
    //     setIntegerField(env, localeData, "minimalDaysInFirstWeek", firstDayVals[1]);
    // }

    // setStringArrayField(env, localeData, "amPm", getAmPmMarkers(env, gregorian.get()));
    // setStringArrayField(env, localeData, "eras", getEras(env, gregorian.get()));

    // ScopedResourceBundle dayNames(ures_getByKey(gregorian.get(), "dayNames", NULL, &status));
    // ScopedResourceBundle monthNames(ures_getByKey(gregorian.get(), "monthNames", NULL, &status));

    // // Get the regular month and weekday names.
    // jobjectArray longMonthNames = getNames(env, monthNames.get(), true, REGULAR, LONG);
    // jobjectArray shortMonthNames = getNames(env, monthNames.get(), true, REGULAR, SHORT);
    // jobjectArray longWeekdayNames = getNames(env, dayNames.get(), false, REGULAR, LONG);
    // jobjectArray shortWeekdayNames = getNames(env, dayNames.get(), false, REGULAR, SHORT);
    // setStringArrayField(env, localeData, "longMonthNames", longMonthNames);
    // setStringArrayField(env, localeData, "shortMonthNames", shortMonthNames);
    // setStringArrayField(env, localeData, "longWeekdayNames", longWeekdayNames);
    // setStringArrayField(env, localeData, "shortWeekdayNames", shortWeekdayNames);

    // // Get the stand-alone month and weekday names. If they're not available (as they aren't for
    // // English), we reuse the regular names. If we returned null to Java, the usual fallback
    // // mechanisms would come into play and we'd end up with the bogus stand-alone names from the
    // // root locale ("1" for January, and so on).
    // jobjectArray longStandAloneMonthNames = getNames(env, monthNames.get(), true, STAND_ALONE, LONG);
    // if (longStandAloneMonthNames == NULL) {
    //     longStandAloneMonthNames = longMonthNames;
    // }
    // jobjectArray shortStandAloneMonthNames = getNames(env, monthNames.get(), true, STAND_ALONE, SHORT);
    // if (shortStandAloneMonthNames == NULL) {
    //     shortStandAloneMonthNames = shortMonthNames;
    // }
    // jobjectArray longStandAloneWeekdayNames = getNames(env, dayNames.get(), false, STAND_ALONE, LONG);
    // if (longStandAloneWeekdayNames == NULL) {
    //     longStandAloneWeekdayNames = longWeekdayNames;
    // }
    // jobjectArray shortStandAloneWeekdayNames = getNames(env, dayNames.get(), false, STAND_ALONE, SHORT);
    // if (shortStandAloneWeekdayNames == NULL) {
    //     shortStandAloneWeekdayNames = shortWeekdayNames;
    // }
    // setStringArrayField(env, localeData, "longStandAloneMonthNames", longStandAloneMonthNames);
    // setStringArrayField(env, localeData, "shortStandAloneMonthNames", shortStandAloneMonthNames);
    // setStringArrayField(env, localeData, "longStandAloneWeekdayNames", longStandAloneWeekdayNames);
    // setStringArrayField(env, localeData, "shortStandAloneWeekdayNames", shortStandAloneWeekdayNames);

    // ScopedResourceBundle dateTimePatterns(ures_getByKey(gregorian.get(), "DateTimePatterns", NULL, &status));
    // if (U_SUCCESS(status)) {
    //     setStringField(env, localeData, "fullTimeFormat", dateTimePatterns.get(), 0);
    //     setStringField(env, localeData, "longTimeFormat", dateTimePatterns.get(), 1);
    //     setStringField(env, localeData, "mediumTimeFormat", dateTimePatterns.get(), 2);
    //     setStringField(env, localeData, "shortTimeFormat", dateTimePatterns.get(), 3);
    //     setStringField(env, localeData, "fullDateFormat", dateTimePatterns.get(), 4);
    //     setStringField(env, localeData, "longDateFormat", dateTimePatterns.get(), 5);
    //     setStringField(env, localeData, "mediumDateFormat", dateTimePatterns.get(), 6);
    //     setStringField(env, localeData, "shortDateFormat", dateTimePatterns.get(), 7);
    // }
    // status = U_ZERO_ERROR;

    // ScopedResourceBundle numberElements(ures_getByKey(root.get(), "NumberElements", NULL, &status));
    // if (U_SUCCESS(status) && ures_getSize(numberElements.get()) >= 11) {
    //     setCharField(env, localeData, "zeroDigit", numberElements.get(), 4);
    //     setCharField(env, localeData, "digit", numberElements.get(), 5);
    //     setCharField(env, localeData, "decimalSeparator", numberElements.get(), 0);
    //     setCharField(env, localeData, "groupingSeparator", numberElements.get(), 1);
    //     setCharField(env, localeData, "patternSeparator", numberElements.get(), 2);
    //     setCharField(env, localeData, "percent", numberElements.get(), 3);
    //     setCharField(env, localeData, "perMill", numberElements.get(), 8);
    //     setCharField(env, localeData, "monetarySeparator", numberElements.get(), 0);
    //     setCharField(env, localeData, "minusSign", numberElements.get(), 6);
    //     setStringField(env, localeData, "exponentSeparator", numberElements.get(), 7);
    //     setStringField(env, localeData, "infinity", numberElements.get(), 9);
    //     setStringField(env, localeData, "NaN", numberElements.get(), 10);
    // }
    // status = U_ZERO_ERROR;

    // jstring internationalCurrencySymbol = getIntCurrencyCode(env, locale);
    // jstring currencySymbol = NULL;
    // if (internationalCurrencySymbol != NULL) {
    //     currencySymbol = ICU_getCurrencySymbolNative(env, NULL, locale, internationalCurrencySymbol);
    // } else {
    //     internationalCurrencySymbol = env->NewStringUTF("XXX");
    // }
    // if (currencySymbol == NULL) {
    //     // This is the UTF-8 encoding of U+00A4 (CURRENCY SIGN).
    //     currencySymbol = env->NewStringUTF("\xc2\xa4");
    // }
    // setStringField(env, localeData, "currencySymbol", currencySymbol);
    // setStringField(env, localeData, "internationalCurrencySymbol", internationalCurrencySymbol);

    // ScopedResourceBundle numberPatterns(ures_getByKey(root.get(), "NumberPatterns", NULL, &status));
    // if (U_SUCCESS(status) && ures_getSize(numberPatterns.get()) >= 3) {
    //     setStringField(env, localeData, "numberPattern", numberPatterns.get(), 0);
    //     setStringField(env, localeData, "currencyPattern", numberPatterns.get(), 1);
    //     setStringField(env, localeData, "percentPattern", numberPatterns.get(), 2);
    // }

    // return JNI_TRUE;
    return FALSE;
}
