
#ifndef __CICUHELPER_H__
#define __CICUHELPER_H__

#include "_CICUHelper.h"

CarClass(CICUHelper)
{
public:
    CARAPI GetISOLanguages(
        /* [out, callee] */ ArrayOf<String> ** ppLanguages);

    CARAPI GetISOCountries(
        /* [out, callee] */ ArrayOf<String> ** ppCountries);

    CARAPI LocaleFromString(
        /* [in] */ const String& localeName,
        /* [out] */ ILocale ** ppLocale);

    CARAPI LocalesFromStrings(
        /* [in] */ const ArrayOf<String> & localeNames,
        /* [out, callee] */ ArrayOf<ILocale*> ** ppLocales);

    CARAPI GetAvailableLocales(
        /* [out, callee] */ ArrayOf<ILocale*> ** ppLocales);

    CARAPI GetAvailableBreakIteratorLocales(
        /* [out, callee] */ ArrayOf<ILocale*> ** ppLocales);

    CARAPI GetAvailableCalendarLocales(
        /* [out, callee] */ ArrayOf<ILocale*> ** ppLocales);

    CARAPI GetAvailableCollatorLocales(
        /* [out, callee] */ ArrayOf<ILocale*> ** ppLocales);

    CARAPI GetAvailableDateFormatLocales(
        /* [out, callee] */ ArrayOf<ILocale*> ** ppLocales);

    CARAPI GetAvailableDateFormatSymbolsLocales(
        /* [out, callee] */ ArrayOf<ILocale*> ** ppLocales);

    CARAPI GetAvailableDecimalFormatSymbolsLocales(
        /* [out, callee] */ ArrayOf<ILocale*> ** ppLocales);

    CARAPI GetAvailableNumberFormatLocales(
        /* [out, callee] */ ArrayOf<ILocale*> ** ppLocales);

    CARAPI ToLowerCase(
        /* [in] */ const String& inStr,
        /* [out] */ String * pLocalname);

    CARAPI ToUpperCase(
        /* [in] */ const String& inStr,
        /* [out] */ String * pLocalname);

    CARAPI GetISO3CountryNative(
        /* [in] */ const String& locale,
        /* [out] */ String * country);

   CARAPI GetISO3LanguageNative(
        /* [in] */ const String& locale,
        /* [out] */ String * language);

   CARAPI GetAvailableLocalesNative(
        /* [out, callee] */ ArrayOf<String> **ppLocales);

private:
    ArrayOf<String> * ToArrayString(const char* const* strings);

private:
    ArrayOf<ILocale*> *availableLocalesCache;
    // TODO: Add your private member variables here.
};

#endif // __CICUHELPER_H__
