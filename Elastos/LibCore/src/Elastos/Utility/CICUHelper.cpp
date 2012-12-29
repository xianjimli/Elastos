
#include "CICUHelper.h"
#include "unicode/locid.h"
#include "CLocale.h"
#include <stdio.h>

ArrayOf<String> *CICUHelper::ToArrayString(const char * const * strings)
{
    Int32 count = 0;
    while(strings[count] != NULL) {
        count++;
    }

    ArrayOf<String>* strArray = ArrayOf<String>::Alloc(count);
    count = 0;
    while(strings[count] != NULL) {
        (*strArray)[count] = String(strings[count]);
        count ++;
    }

    return strArray;
}

ECode CICUHelper::GetISOLanguages(
    /* [out, callee] */ ArrayOf<String> ** ppLanguages)
{
    // TODO: Add your code here
    *ppLanguages = ToArrayString(Locale::getISOLanguages());
    return NOERROR;
}

ECode CICUHelper::GetISOCountries(
    /* [out, callee] */ ArrayOf<String> ** ppCountries)
{
    // TODO: Add your code here
    *ppCountries = ToArrayString(Locale::getISOCountries());
    return NOERROR;
}

ECode CICUHelper::LocaleFromString(
    /* [in] */ const String& localeName,
    /* [out] */ ILocale ** ppLocale)
{
    ECode ec = NOERROR;
    Int32 first = localeName.IndexOf('_');
    Int32 second = localeName.IndexOf('_', first + 1);

    printf("the first is %d, %d\n", first, second);
    printf("the string is %s\n", (const char *)localeName);
    if (first == -1) {
        // Language only ("ja").
        ec = CLocale::New(localeName, ppLocale);
    } else if (second == -1) {
        // Language and country ("ja_JP").
        ec = CLocale::New(localeName.Substring(0, first), localeName.Substring(first + 1), ppLocale);
    } else {
        // Language and country and variant ("ja_JP_TRADITIONAL").
        ec = CLocale::New(localeName.Substring(0, first), localeName.Substring(first + 1, second), localeName.Substring(first + second + 2), ppLocale);
    }
    //printf("the third string is %s", (const char *)localeName.Substring(first + second + 2));
    // TODO: Add your code here
    return ec;
}

ECode CICUHelper::LocalesFromStrings(
    /* [in] */ const ArrayOf<String> & localeNames,
    /* [out, callee] */ ArrayOf<ILocale*> ** ppLocales)
{
    // TODO: Add your code here
    Int32 len = localeNames.GetLength();
    if (len < 1) {
        printf("%s ,%d\n", __FILE__, __LINE__);
        *ppLocales = NULL;
        return NOERROR;
    }
    printf("%s ,%d\n", __FILE__, __LINE__);
    *ppLocales = ArrayOf<ILocale *>::Alloc(len);
    for (Int32 count = 0; count < len; ++count) {
        printf("===========================\n");
        ILocale *locale;
        LocaleFromString(localeNames[count], &locale);
        (**ppLocales)[count] = locale;
    }
    printf("===========================1\n");
    return NOERROR;
}

ECode CICUHelper::GetAvailableLocales(
    /* [out, callee] */ ArrayOf<ILocale*> ** ppLocales)
{
    // TODO: Add your code here
    if (availableLocalesCache != NULL) {
        *ppLocales = availableLocalesCache;
    } else {
        //GetAvailableLocalesNative(&availableLocalesCache);
        *ppLocales = availableLocalesCache;
    }
    return NOERROR;
}

ECode CICUHelper::GetAvailableBreakIteratorLocales(
    /* [out, callee] */ ArrayOf<ILocale*> ** ppLocales)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CICUHelper::GetAvailableCalendarLocales(
    /* [out, callee] */ ArrayOf<ILocale*> ** ppLocales)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CICUHelper::GetAvailableCollatorLocales(
    /* [out, callee] */ ArrayOf<ILocale*> ** ppLocales)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CICUHelper::GetAvailableDateFormatLocales(
    /* [out, callee] */ ArrayOf<ILocale*> ** ppLocales)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CICUHelper::GetAvailableDateFormatSymbolsLocales(
    /* [out, callee] */ ArrayOf<ILocale*> ** ppLocales)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CICUHelper::GetAvailableDecimalFormatSymbolsLocales(
    /* [out, callee] */ ArrayOf<ILocale*> ** ppLocales)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CICUHelper::GetAvailableNumberFormatLocales(
    /* [out, callee] */ ArrayOf<ILocale*> ** ppLocales)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CICUHelper::ToLowerCase(
    /* [in] */ const String& inStr,
    /* [out] */ String * pLocalname)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CICUHelper::ToUpperCase(
    /* [in] */ const String& inStr,
    /* [out] */ String * pLocalname)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CICUHelper::GetISO3CountryNative(
    /* [in] */ const String& locale,
    /* [out] */ String * country)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}


ECode CICUHelper::GetISO3LanguageNative(
    /* [in] */ const String& locale,
    /* [out] */ String * language)
{
    return E_NOT_IMPLEMENTED;
}

ECode CICUHelper::GetAvailableLocalesNative(
    /* [out, callee] */ ArrayOf<String> **ppLocales)
{
    return E_NOT_IMPLEMENTED;
}