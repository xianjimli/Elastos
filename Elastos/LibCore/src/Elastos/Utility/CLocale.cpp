
#include "CLocale.h"
#include <stdio.h>
/**
 * Returns the country code for this locale, or {@code ""} if this locale
 * doesn't correspond to a specific country.
 */
ECode CLocale::GetCountry(
    /* [out] */ String* country)
{
    // TODO: Add your code here
    printf("***%s %d\n", __FILE__, __LINE__);
    *country = countryCode;
    return NOERROR;
}

/**
 * Equivalent to {@code getDisplayCountry(Locale.getDefault())}.
 */
ECode CLocale::GetDisplayCountry(
    /* [out] */ String* country)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

 /**
 * Returns the name of this locale's country, localized to {@code locale}.
 * Returns the empty string if this locale does not correspond to a specific
 * country.
 */
ECode CLocale::GetDisplayCountryEx(
    /* [in] */ ILocale* locale,
    /* [out] */ String* country)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Equivalent to {@code getDisplayLanguage(Locale.getDefault())}.
 */
ECode CLocale::GetDisplayLanguage(
    /* [out] */ String* language)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Returns the name of this locale's language, localized to {@code locale}.
 * If the language name is unknown, the language code is returned.
 */
ECode CLocale::GetDisplayLanguageEx(
    /* [in] */ ILocale* locale,
    /* [out] */ String* language)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Equivalent to {@code getDisplayName(Locale.getDefault())}.
 */
ECode CLocale::GetDisplayName(
    /* [out] */ String* name)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Returns this locale's language name, country name, and variant, localized
 * to {@code locale}. The exact output form depends on whether this locale
 * corresponds to a specific language, country and variant, such as:
 * {@code English}, {@code English (United States)}, {@code English (United
 * States,Computer)}, {@code anglais (&#x00c9;tats-Unis)}, {@code anglais
 * (&#x00c9;tats-Unis,informatique)}.
 */
ECode CLocale::GetDisplayNameEx(
    /* [in] */ ILocale* locale,
    /* [out] */ String* name)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Gets the full variant name in the default {@code Locale} for the variant code of
 * this {@code Locale}. If there is no matching variant name, the variant code is
 * returned.
 *
 * @return a variant name.
 */
ECode CLocale::GetDisplayVariant(
    /* [out] */ String* variantName)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Gets the full variant name in the specified {@code Locale} for the variant code
 * of this {@code Locale}. If there is no matching variant name, the variant code is
 * returned.
 *
 * @param locale
 *            the {@code Locale} for which the display name is retrieved.
 * @return a variant name.
 */
ECode CLocale::GetDisplayVariantEx(
    /* [in] */ ILocale* locale,
    /* [out] */ String* variantName)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

 /**
 * Gets the three letter ISO country code which corresponds to the country
 * code for this {@code Locale}.
 *
 * @return a three letter ISO language code.
 * @throws MissingResourceException
 *                if there is no matching three letter ISO country code.
 */
ECode CLocale::GetISO3Country(
    /* [out] */ String* country)
{
    // TODO: Add your code here
    if (countryCode.GetLength() == 0) {
        *country = countryCode;
        return NOERROR;
    }
    //icuHelper->GetISO3CountryNative(ToString(), country);
    return NOERROR;
}

/**
 * Gets the three letter ISO language code which corresponds to the language
 * code for this {@code Locale}.
 *
 * @return a three letter ISO language code.
 * @throws MissingResourceException
 *                if there is no matching three letter ISO language code.
 */
ECode CLocale::GetISO3Language(
    /* [out] */ String* language)
{
    // TODO: Add your code here
    if (languageCode.GetLength() == 0) {
        *language = languageCode;
        return NOERROR;
    }
    //icuHelper->GetISO3LanguageNative(ToString(), language);
    return NOERROR;
}

/**
 * Gets the language code for this {@code Locale} or the empty string of no language
 * was set.
 *
 * @return a language code.
 */
ECode CLocale::GetLanguage(
    /* [out] */ String* language)
{
    // TODO: Add your code here
    *language = languageCode;
    return NOERROR;
}

/**
 * Gets the variant code for this {@code Locale} or an empty {@code String} if no variant
 * was set.
 *
 * @return a variant code.
 */
ECode CLocale::GetVariant(
    /* [out] */ String* variant)
{
    // TODO: Add your code here
    *variant = variantCode;
    return NOERROR;
}

ECode CLocale::constructor()
{
    // TODO: Add your code here
    return constructor(String("en"), String("US"), String(""));
}

/**
 * Constructs a new {@code Locale} using the specified language.
 */
ECode CLocale::constructor(
    /* [in] */ String language)
{
    // TODO: Add your code here
    return constructor(language, String(""), String(""));
}

/**
 * Constructs a new {@code Locale} using the specified language and country codes.
 */
ECode CLocale::constructor(
    /* [in] */ String language,
    /* [in] */ String country)
{
    // TODO: Add your code here
    return constructor(language, country, String(""));
}

/**
 * Constructs a new {@code Locale} using the specified language, country,
 * and variant codes.
 */
ECode CLocale::constructor(
    /* [in] */ String language,
    /* [in] */ String country,
    /* [in] */ String variant)
{
    // TODO: Add your code here
//        if (language == null || country == null || variant == null) {
//            throw new NullPointerException();
//        }
//    ECode ec = NOERROR;
//    ec = CICUHelper::AcquireSingleton((IICUHelper**) &icuHelper);

    if(language.IsNull() && country.IsNull()){
        printf("%s, %d\n", __FILE__, __LINE__);
        languageCode = String("");
        countryCode = String("");
        variantCode = variant;
        return NOERROR;
    }
        // BEGIN android-changed
        // this.uLocale = new ULocale(language, country, variant);
        // languageCode = uLocale.getLanguage();
//---gaojianfeng delete-------languageCode = Util.toASCIILowerCase(language);
    languageCode = language;
        // END android-changed
        // Map new language codes to the obsolete language
        // codes so the correct resource bundles will be used.
    if (languageCode.Equals("he")) {
        languageCode = "iw";
    } else if (languageCode.Equals("id")) {
        languageCode = "in";
    } else if (languageCode.Equals("yi")) {
        languageCode = "ji";
    }

        // countryCode is defined in ASCII character set
        // BEGIN android-changed
        // countryCode = country.length()!=0?uLocale.getCountry():"";
//---gaojianfeng delete-------countryCode = Util.toASCIIUpperCase(country);
    countryCode = country;
        // END android-changed

        // Work around for be compatible with RI
    variantCode = variant;
    return NOERROR;
}

String CLocale::ToString()
{
    return String("");
}

String CLocale::ToNewString()
{
    return String("");
}