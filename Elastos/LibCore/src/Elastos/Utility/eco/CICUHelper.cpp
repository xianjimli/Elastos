
#include "cmdef.h"
#include "CICUHelper.h"
#include "ICU.h"
#include <Elastos.Core.h>


ECode CICUHelper::GetISOLanguages(
    /* [out, callee] */ ArrayOf<String>** languages)
{
    VALIDATE_NOT_NULL(languages);

    return ICU::GetISOLanguages(languages);
}

ECode CICUHelper::GetISOCountries(
    /* [out, callee] */ ArrayOf<String>** countries)
{
    VALIDATE_NOT_NULL(countries);

    return ICU::GetISOCountries(countries);
}

ECode CICUHelper::LocaleFromString(
    /* [in] */ const String& localeName,
    /* [out] */ ILocale** locale)
{
    VALIDATE_NOT_NULL(locale);

    return ICU::LocaleFromString(localeName, locale);
}

ECode CICUHelper::LocalesFromStrings(
    /* [in] */ const ArrayOf<String>& localeNames,
    /* [out, callee] */ ArrayOf<ILocale*>** locales)
{
    VALIDATE_NOT_NULL(locales);

    return ICU::LocalesFromStrings(localeNames, locales);
}

ECode CICUHelper::GetAvailableLocales(
    /* [out, callee] */ ArrayOf<ILocale*>** locales)
{
    VALIDATE_NOT_NULL(locales);

    return ICU::GetAvailableLocales(locales);
}

ECode CICUHelper::GetAvailableBreakIteratorLocales(
    /* [out, callee] */ ArrayOf<ILocale*>** locales)
{
    VALIDATE_NOT_NULL(locales);

    return ICU::GetAvailableBreakIteratorLocales(locales);
}

ECode CICUHelper::GetAvailableCalendarLocales(
    /* [out, callee] */ ArrayOf<ILocale*>** locales)
{
    VALIDATE_NOT_NULL(locales);

    return ICU::GetAvailableCalendarLocales(locales);
}

ECode CICUHelper::GetAvailableCollatorLocales(
    /* [out, callee] */ ArrayOf<ILocale*>** locales)
{
    VALIDATE_NOT_NULL(locales);

    return ICU::GetAvailableCollatorLocales(locales);
}

ECode CICUHelper::GetAvailableDateFormatLocales(
    /* [out, callee] */ ArrayOf<ILocale*>** locales)
{
    VALIDATE_NOT_NULL(locales);

    return ICU::GetAvailableDateFormatLocales(locales);
}

ECode CICUHelper::GetAvailableDateFormatSymbolsLocales(
    /* [out, callee] */ ArrayOf<ILocale*>** locales)
{
    VALIDATE_NOT_NULL(locales);

    return ICU::GetAvailableDateFormatSymbolsLocales(locales);
}

ECode CICUHelper::GetAvailableDecimalFormatSymbolsLocales(
    /* [out, callee] */ ArrayOf<ILocale*>** locales)
{
    VALIDATE_NOT_NULL(locales);

    return ICU::GetAvailableDateFormatSymbolsLocales(locales);
}

ECode CICUHelper::GetAvailableNumberFormatLocales(
    /* [out, callee] */ ArrayOf<ILocale*>** locales)
{
    VALIDATE_NOT_NULL(locales);

    return ICU::GetAvailableNumberFormatLocales(locales);
}

ECode CICUHelper::ToLowerCase(
    /* [in] */ const String& s,
    /* [in] */ const String& localname,
    /* [out] */ String* ls)
{
    VALIDATE_NOT_NULL(ls);

    *ls = ICU::ToLowerCase(s, localname);
    return NOERROR;
}

ECode CICUHelper::ToUpperCase(
    /* [in] */ const String& s,
    /* [in] */ const String& localname,
    /* [out] */ String* us)
{
    VALIDATE_NOT_NULL(us);

    *us = ICU::ToUpperCase(s, localname);
    return NOERROR;
}

ECode CICUHelper::GetCurrencyCode(
    /* [in] */ const String& locale,
    /* [out] */ String* currencyCode)
{
    VALIDATE_NOT_NULL(currencyCode);

    *currencyCode = ICU::GetCurrencyCode(locale);
    return NOERROR;
}

ECode CICUHelper::GetCurrencyFractionDigits(
    /* [in] */ const String& currencyCode,
    /* [out] */ Int32* currencyFractionDigits)
{
    VALIDATE_NOT_NULL(currencyFractionDigits);

    *currencyFractionDigits = ICU::GetCurrencyFractionDigits(currencyCode);
    return NOERROR;
}

ECode CICUHelper::GetCurrencySymbol(
    /* [in] */ const String& locale,
    /* [in] */ const String& currencyCode,
    /* [out] */ String* currencySymbol)
{
    VALIDATE_NOT_NULL(currencySymbol);

    *currencySymbol = ICU::GetCurrencySymbol(locale, currencyCode);
    return NOERROR;
}

ECode CICUHelper::GetDisplayCountry(
    /* [in] */ const String& countryCode,
    /* [in] */ const String& locale,
    /* [out] */ String* displayCountry)
{
    VALIDATE_NOT_NULL(displayCountry);

    *displayCountry = ICU::GetDisplayCountry(countryCode, locale);
    return NOERROR;
}

ECode CICUHelper::GetDisplayLanguage(
    /* [in] */ const String& languageCode,
    /* [in] */ const String& locale,
    /* [out] */ String* displayLanguage)
{
    VALIDATE_NOT_NULL(displayLanguage);

    *displayLanguage = ICU::GetDisplayLanguage(languageCode, locale);
    return NOERROR;
}

ECode CICUHelper::GetDisplayVariant(
    /* [in] */ const String& variantCode,
    /* [in] */ const String& locale,
    /* [out] */ String* displayVariant)
{
    VALIDATE_NOT_NULL(displayVariant);

    *displayVariant = ICU::GetDisplayVariant(variantCode, locale);
    return NOERROR;
}

ECode CICUHelper::GetISO3Country(
    /* [in] */ const String& locale,
    /* [out] */ String* ISO3country)
{
    VALIDATE_NOT_NULL(ISO3country);

    *ISO3country = ICU::GetISO3Country(locale);
    return NOERROR;
}

ECode CICUHelper::GetISO3Language(
    /* [in] */ const String& locale,
    /* [out] */ String* ISO3Language)
{
    VALIDATE_NOT_NULL(ISO3Language);

    *ISO3Language = ICU::GetISO3Language(locale);
    return NOERROR;
}
