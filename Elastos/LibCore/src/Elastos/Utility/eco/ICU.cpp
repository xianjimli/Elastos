
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
#include "Logging/Logger.h"
#include <StringBuffer.h>

using namespace Elastos::Utility::Logging;

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

static Boolean getDayIntVector(
    /* [in] */ UResourceBundle* gregorian,
    /* [in] */ Int32* values)
{
    // get the First day of week and the minimal days in first week numbers
    UErrorCode status = U_ZERO_ERROR;
    ScopedResourceBundle gregorianElems(ures_getByKey(gregorian, "DateTimeElements", NULL, &status));
    if (U_FAILURE(status)) {
        return FALSE;
    }

    Int32 intVectSize;
    const Int32* result = ures_getIntVector(gregorianElems.get(), &intVectSize, &status);
    if (U_FAILURE(status) || intVectSize != 2) {
        return FALSE;
    }

    values[0] = result[0];
    values[1] = result[1];
    return TRUE;
}

static ArrayOf<String>* getAmPmMarkers(
    /* [in] */ UResourceBundle* gregorian)
{
    UErrorCode status = U_ZERO_ERROR;
    ScopedResourceBundle gregorianElems(ures_getByKey(gregorian, "AmPmMarkers", NULL, &status));
    if (U_FAILURE(status)) {
        return NULL;
    }

    Int32 lengthAm, lengthPm;
    UnicodeString am = ures_getStringByIndex(gregorianElems.get(), 0, &lengthAm, &status);
    UnicodeString pm = ures_getStringByIndex(gregorianElems.get(), 1, &lengthPm, &status);

    if (U_FAILURE(status)) {
        return NULL;
    }

    ArrayOf<String>* amPmMarkers = ArrayOf<String>::Alloc(2);
    String amS("");
    String pmS("");
    ElStringByteSink sink1(&amS), sink2(&pmS);
    am.toUTF8(sink1);
    pm.toUTF8(sink2);
    (*amPmMarkers)[0] = amS;
    (*amPmMarkers)[1] = pmS;

    return amPmMarkers;
}

static ArrayOf<String>* getEras(
    /* [in] */ UResourceBundle* gregorian)
{
    UErrorCode status = U_ZERO_ERROR;
    ScopedResourceBundle gregorianElems(ures_getByKey(gregorian, "eras", NULL, &status));
    if (U_FAILURE(status)) {
        return NULL;
    }

    ScopedResourceBundle eraElems(ures_getByKey(gregorianElems.get(), "abbreviated", NULL, &status));
    if (U_FAILURE(status)) {
        return NULL;
    }

    Int32 eraCount = ures_getSize(eraElems.get());
    ArrayOf<String>* eras = ArrayOf<String>::Alloc(eraCount);
    for (Int32 i = 0; i < eraCount; ++i) {
        Int32 eraLength;
        UnicodeString era = ures_getStringByIndex(eraElems.get(), i, &eraLength, &status);
        if (U_FAILURE(status)) {
            ArrayOf<String>::Free(eras);
            return NULL;
        }
        String s("");
        ElStringByteSink sink(&s);
        era.toUTF8(sink);
        (*eras)[i] = s;
    }
    return eras;
}

enum NameType { REGULAR, STAND_ALONE };
enum NameWidth { LONG, SHORT };
static ArrayOf<String>* getNames(
    /* [in] */ UResourceBundle* namesBundle,
    /* [in] */ Boolean months,
    /* [in] */ NameType type,
    /* [in] */ NameWidth width)
{
    const char* typeKey = (type == REGULAR) ? "format" : "stand-alone";
    const char* widthKey = (width == LONG) ? "wide" : "abbreviated";
    UErrorCode status = U_ZERO_ERROR;
    ScopedResourceBundle formatBundle(ures_getByKey(namesBundle, typeKey, NULL, &status));
    ScopedResourceBundle valuesBundle(ures_getByKey(formatBundle.get(), widthKey, NULL, &status));
    if (U_FAILURE(status)) {
        return NULL;
    }
    // The months array has a trailing empty string. The days array has a leading empty string.
    Int32 count = ures_getSize(valuesBundle.get());
    ArrayOf<String>* result = ArrayOf<String>::Alloc(count + 1);
    assert(result != NULL);
    if (months) {
        for (Int32 i = 0; i < count; ++i) {
            (*result)[i] = String("");
        }
    }

    Int32 arrayOffset = months ? 0 : 1;
    for (Int32 i = 0; i < count; ++i) {
        Int32 nameLength;
        UnicodeString name = ures_getStringByIndex(valuesBundle.get(), i, &nameLength, &status);
        if (U_FAILURE(status)) {
            ArrayOf<String>::Free(result);
            return NULL;
        }
        String s("");
        ElStringByteSink sink(&s);
        name.toUTF8(sink);
        (*result)[arrayOffset + i] = s;
    }
    return result;
}

static String getIntCurrencyCode(
    /* [in] */ const String& locale)
{
    // Extract the 2-character country name.
    if (locale.GetLength() < 5) {
        return String(NULL);
    }
    if (locale[3] < 'A' || locale[3] > 'Z' || locale[4] < 'A' || locale[4] > 'Z') {
        return String(NULL);
    }

    StringBuffer buffer;
    buffer += locale[3];
    buffer += locale[4];
    return ICU::GetCurrencyCode(String(buffer));
}

static void getUresString(
    /* [in] */ UResourceBundle* bundle,
    /* [in] */ Int32 index,
    /* [out] */ String* field)
{
    UErrorCode status = U_ZERO_ERROR;
    Int32 charCount;
    UnicodeString chars = ures_getStringByIndex(bundle, index, &charCount, &status);
    if (U_SUCCESS(status)) {
        String s("");
        ElStringByteSink sink(&s);
        chars.toUTF8(sink);
        *field = s;
    }
    else {
//        LOGE("Error setting String field %s from ICU resource: %s", fieldName, u_errorName(status));
    }
}

static void getUresChar(
    /* [in] */ UResourceBundle* bundle,
    /* [in] */ Int32 index,
    /* [out] */ Char32* field)
{
    UErrorCode status = U_ZERO_ERROR;
    Int32 charCount;
    UnicodeString chars = ures_getStringByIndex(bundle, index, &charCount, &status);
    if (U_SUCCESS(status)) {
        *field = chars[0];
    }
    else {
        //LOGE("Error setting char field %s from ICU resource: %s", fieldName, u_errorName(status));
    }
}

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
        String s("");
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
        String s("");
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

    String s("");
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

    String s("");
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

    String s("");
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
    /* [in] */ const String& localeName,
    /* [in] */ LocaleData* localeData)
{
    //ScopedUtfChars localeName(env, locale);
    UErrorCode status = U_ZERO_ERROR;
    ScopedResourceBundle root(ures_open(NULL, localeName, &status));
    if (U_FAILURE(status)) {
        //Logger::E("Trying to pause when pause is already pending for ", u_errorName(status));
        status = U_ZERO_ERROR;
        return FALSE;
    }

    ScopedResourceBundle calendar(ures_getByKey(root.get(), "calendar", NULL, &status));
    if (U_FAILURE(status)) {
        //LOGE("Error getting ICU calendar resource bundle: %s", u_errorName(status));
        return FALSE;
    }

    ScopedResourceBundle gregorian(ures_getByKey(calendar.get(), "gregorian", NULL, &status));
    if (U_FAILURE(status)) {
        //Logger::E("Error getting ICU gregorian resource bundle: ", u_errorName(status));
        return FALSE;
    }

    Int32 firstDayVals[] = { 0, 0 };
    if (getDayIntVector(gregorian.get(), firstDayVals)) {
        CInteger32::New(firstDayVals[0], (IInteger32**)&localeData->mFirstDayOfWeek);
        CInteger32::New(firstDayVals[1], (IInteger32**)&localeData->mMinimalDaysInFirstWeek);
    }

    localeData->mAmPm = getAmPmMarkers(gregorian.get());
    localeData->mEras = getEras(gregorian.get());

    ScopedResourceBundle dayNames(ures_getByKey(gregorian.get(), "dayNames", NULL, &status));
    ScopedResourceBundle monthNames(ures_getByKey(gregorian.get(), "monthNames", NULL, &status));

    // Get the regular month and weekday names.
    localeData->mLongMonthNames = getNames(monthNames.get(), TRUE, REGULAR, LONG);
    localeData->mShortMonthNames = getNames(monthNames.get(), TRUE, REGULAR, SHORT);
    localeData->mLongWeekdayNames = getNames(dayNames.get(), FALSE, REGULAR, LONG);
    localeData->mShortWeekdayNames = getNames(dayNames.get(), FALSE, REGULAR, SHORT);

    // Get the stand-alone month and weekday names. If they're not available (as they aren't for
    // English), we reuse the regular names. If we returned null to Java, the usual fallback
    // mechanisms would come into play and we'd end up with the bogus stand-alone names from the
    // root locale ("1" for January, and so on).
    localeData->mLongStandAloneMonthNames = getNames(monthNames.get(), TRUE, STAND_ALONE, LONG);
    if (localeData->mLongStandAloneMonthNames == NULL) {
        CLONE_ARRAY_OF_STRING(&localeData->mLongStandAloneMonthNames, localeData->mLongMonthNames);
    }
    localeData->mShortStandAloneMonthNames = getNames(monthNames.get(), TRUE, STAND_ALONE, SHORT);
    if (localeData->mShortStandAloneMonthNames == NULL) {
        CLONE_ARRAY_OF_STRING(&localeData->mShortStandAloneMonthNames, localeData->mShortMonthNames);
    }
    localeData->mLongStandAloneWeekdayNames = getNames(dayNames.get(), FALSE, STAND_ALONE, LONG);
    if (localeData->mLongStandAloneWeekdayNames == NULL) {
        CLONE_ARRAY_OF_STRING(&localeData->mLongStandAloneWeekdayNames, localeData->mLongWeekdayNames);
    }
    localeData->mShortStandAloneWeekdayNames = getNames(dayNames.get(), FALSE, STAND_ALONE, SHORT);
    if (localeData->mShortStandAloneWeekdayNames == NULL) {
        CLONE_ARRAY_OF_STRING(&localeData->mShortStandAloneWeekdayNames, localeData->mShortWeekdayNames);
    }

    ScopedResourceBundle dateTimePatterns(ures_getByKey(gregorian.get(), "DateTimePatterns", NULL, &status));
    if (U_SUCCESS(status)) {
        getUresString(dateTimePatterns.get(), 0, &localeData->mFullTimeFormat);
        getUresString(dateTimePatterns.get(), 1, &localeData->mLongTimeFormat);
        getUresString(dateTimePatterns.get(), 2, &localeData->mMediumTimeFormat);
        getUresString(dateTimePatterns.get(), 3, &localeData->mShortTimeFormat);
        getUresString(dateTimePatterns.get(), 4, &localeData->mFullDateFormat);
        getUresString(dateTimePatterns.get(), 5, &localeData->mLongDateFormat);
        getUresString(dateTimePatterns.get(), 6, &localeData->mMediumDateFormat);
        getUresString(dateTimePatterns.get(), 7, &localeData->mShortDateFormat);
    }
    status = U_ZERO_ERROR;

    ScopedResourceBundle numberElements(ures_getByKey(root.get(), "NumberElements", NULL, &status));
    if (U_SUCCESS(status) && ures_getSize(numberElements.get()) >= 11) {
        getUresChar(numberElements.get(), 4, &localeData->mZeroDigit);
        getUresChar(numberElements.get(), 5, &localeData->mDigit);
        getUresChar(numberElements.get(), 0, &localeData->mDecimalSeparator);
        getUresChar(numberElements.get(), 1, &localeData->mGroupingSeparator);
        getUresChar(numberElements.get(), 2, &localeData->mPatternSeparator);
        getUresChar(numberElements.get(), 3, &localeData->mPercent);
        getUresChar(numberElements.get(), 8, &localeData->mPerMill);
        getUresChar(numberElements.get(), 0, &localeData->mMonetarySeparator);
        getUresChar(numberElements.get(), 6, &localeData->mMinusSign);
        getUresString(numberElements.get(), 7, &localeData->mExponentSeparator);
        getUresString(numberElements.get(), 9, &localeData->mInfinity);
        getUresString(numberElements.get(), 10, &localeData->mNaN);
    }
    status = U_ZERO_ERROR;

    String internationalCurrencySymbol = getIntCurrencyCode(localeName);
    String currencySymbol = String(NULL);
    if (!internationalCurrencySymbol.IsNull()) {
        currencySymbol = GetCurrencySymbol(localeName, internationalCurrencySymbol);
    }
    else {
        internationalCurrencySymbol = "XXX";
    }
    if (currencySymbol.IsNull()) {
        // This is the UTF-8 encoding of U+00A4 (CURRENCY SIGN).
        currencySymbol = "\xc2\xa4";
    }
    localeData->mCurrencySymbol = currencySymbol;
    localeData->mInternationalCurrencySymbol = internationalCurrencySymbol;

    ScopedResourceBundle numberPatterns(ures_getByKey(root.get(), "NumberPatterns", NULL, &status));
    if (U_SUCCESS(status) && ures_getSize(numberPatterns.get()) >= 3) {
        getUresString(numberPatterns.get(), 0, &localeData->mNumberPattern);
        getUresString(numberPatterns.get(), 1, &localeData->mCurrencyPattern);
        getUresString(numberPatterns.get(), 2, &localeData->mPercentPattern);
    }

    return TRUE;
}
