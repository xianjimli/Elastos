
#include "cmdef.h"
#include "CLocale.h"
#include "Util.h"
#include "ICU.h"
#include <Elastos.Core.h>

/**
 * Returns the country code for this locale, or {@code ""} if this locale
 * doesn't correspond to a specific country.
 */
ECode CLocale::GetCountry(
    /* [out] */ String* country)
{
    VALIDATE_NOT_NULL(country);

    *country = mCountryCode;
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
    VALIDATE_NOT_NULL(country);

    if (mCountryCode.GetLength() == 0) {
        *country = mCountryCode;
        return NOERROR;
    }
    *country = ICU::GetISO3Country(ToString());
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
    VALIDATE_NOT_NULL(language);

    if (mLanguageCode.GetLength() == 0) {
        *language = mLanguageCode;
        return NOERROR;
    }
    *language = ICU::GetISO3Language(ToString());
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
    VALIDATE_NOT_NULL(language);

    *language = mLanguageCode;
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
    VALIDATE_NOT_NULL(variant);

    *variant = mVariantCode;
    return NOERROR;
}

ECode CLocale::constructor()
{
    mLanguageCode = "en";
    mCountryCode = "US";
    mVariantCode = "";
    return NOERROR;
}

/**
 * Constructs a new {@code Locale} using the specified language.
 */
ECode CLocale::constructor(
    /* [in] */ const String& language)
{
    return constructor(language, String(""), String(""));
}

/**
 * Constructs a new {@code Locale} using the specified language and country codes.
 */
ECode CLocale::constructor(
    /* [in] */ const String& language,
    /* [in] */ const String& country)
{
    return constructor(language, country, String(""));
}

/**
 * Constructs a new {@code Locale} using the specified language, country,
 * and variant codes.
 */
ECode CLocale::constructor(
    /* [in] */ const String& language,
    /* [in] */ const String& country,
    /* [in] */ const String& variant)
{
    if (language.IsNull() || country.IsNull() || variant.IsNull()) {
        // throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    if(language.IsEmpty() && country.IsEmpty()){
        mLanguageCode = "";
        mCountryCode = "";
        mVariantCode = variant;
        return NOERROR;
    }
    // BEGIN android-changed
    // this.uLocale = new ULocale(language, country, variant);
    // languageCode = uLocale.getLanguage();
    mLanguageCode = Util::ToASCIILowerCase(language);
    // END android-changed
    // Map new language codes to the obsolete language
    // codes so the correct resource bundles will be used.
    if (mLanguageCode.Equals("he")) {
        mLanguageCode = "iw";
    }
    else if (mLanguageCode.Equals("id")) {
        mLanguageCode = "in";
    }
    else if (mLanguageCode.Equals("yi")) {
        mLanguageCode = "ji";
    }

    // countryCode is defined in ASCII character set
    // BEGIN android-changed
    // countryCode = country.length()!=0?uLocale.getCountry():"";
    mCountryCode = Util::ToASCIIUpperCase(country);
    // END android-changed

    // Work around for be compatible with RI
    mVariantCode = variant;
    return NOERROR;
}

String CLocale::ToString()
{
    // String result = cachedToStringResult;
    // return (result == null) ? (cachedToStringResult = toNewString()) : result;
    return String("");
}

String CLocale::ToNewString()
{
    // // The string form of a locale that only has a variant is the empty string.
    // if (languageCode.length() == 0 && countryCode.length() == 0) {
    //     return "";
    // }
    // // Otherwise, the output format is "ll_cc_variant", where language and country are always
    // // two letters, but the variant is an arbitrary length. A size of 11 characters has room
    // // for "en_US_POSIX", the largest "common" value. (In practice, the string form is almost
    // // always 5 characters: "ll_cc".)
    // StringBuilder result = new StringBuilder(11);
    // result.append(languageCode);
    // if (countryCode.length() > 0 || variantCode.length() > 0) {
    //     result.append('_');
    // }
    // result.append(countryCode);
    // if (variantCode.length() > 0) {
    //     result.append('_');
    // }
    // result.append(variantCode);
    // return result.toString();
    return String("");
}