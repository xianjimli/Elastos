
#ifndef __CICUHELPER_H__
#define __CICUHELPER_H__

#include "_CICUHelper.h"

CarClass(CICUHelper)
{
public:
    CARAPI GetISOLanguages(
        /* [out, callee] */ ArrayOf<String>** languages);

    CARAPI GetISOCountries(
        /* [out, callee] */ ArrayOf<String>** countries);

    CARAPI LocaleFromString(
        /* [in] */ const String& localeName,
        /* [out] */ ILocale** locale);

    CARAPI LocalesFromStrings(
        /* [in] */ const ArrayOf<String>& localeNames,
        /* [out, callee] */ ArrayOf<ILocale*>** locales);

    CARAPI GetAvailableLocales(
        /* [out, callee] */ ArrayOf<ILocale*>** locales);

    CARAPI GetAvailableBreakIteratorLocales(
        /* [out, callee] */ ArrayOf<ILocale*>** locales);

    CARAPI GetAvailableCalendarLocales(
        /* [out, callee] */ ArrayOf<ILocale*>** locales);

    CARAPI GetAvailableCollatorLocales(
        /* [out, callee] */ ArrayOf<ILocale*>** locales);

    CARAPI GetAvailableDateFormatLocales(
        /* [out, callee] */ ArrayOf<ILocale*>** locales);

    CARAPI GetAvailableDateFormatSymbolsLocales(
        /* [out, callee] */ ArrayOf<ILocale*>** locales);

    CARAPI GetAvailableDecimalFormatSymbolsLocales(
        /* [out, callee] */ ArrayOf<ILocale*>** locales);

    CARAPI GetAvailableNumberFormatLocales(
        /* [out, callee] */ ArrayOf<ILocale*>** locales);

    CARAPI ToLowerCase(
        /* [in] */ const String& s,
        /* [in] */ const String& localname,
        /* [out] */ String* ls);

    CARAPI ToUpperCase(
        /* [in] */ const String& s,
        /* [in] */ const String& localname,
        /* [out] */ String* us);

    CARAPI GetCurrencyCode(
        /* [in] */ const String& locale,
        /* [out] */ String* currencyCode);

    CARAPI GetCurrencyFractionDigits(
        /* [in] */ const String& currencyCode,
        /* [out] */ Int32* currencyFractionDigits);

    CARAPI GetCurrencySymbol(
        /* [in] */ const String& locale,
        /* [in] */ const String& currencyCode,
        /* [out] */ String* currencySymbol);

    CARAPI GetDisplayCountry(
        /* [in] */ const String& countryCode,
        /* [in] */ const String& locale,
        /* [out] */ String* displayCountry);

    CARAPI GetDisplayLanguage(
        /* [in] */ const String& languageCode,
        /* [in] */ const String& locale,
        /* [out] */ String* displayLanguage);

    CARAPI GetDisplayVariant(
        /* [in] */ const String& variantCode,
        /* [in] */ const String& locale,
        /* [out] */ String* displayVariant);

    CARAPI GetISO3Country(
        /* [in] */ const String& locale,
        /* [out] */ String* ISO3country);

    CARAPI GetISO3Language(
        /* [in] */ const String& locale,
        /* [out] */ String* ISO3Language);
};

#endif // __CICUHELPER_H__
