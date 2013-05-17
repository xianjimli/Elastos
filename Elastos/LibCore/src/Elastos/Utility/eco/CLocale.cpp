
#include "cmdef.h"
#include "CLocale.h"
#include "Util.h"
#include "ICU.h"
#include <Elastos.Core.h>
#include <StringBuffer.h>

using namespace Elastos::Core;


static AutoPtr<ILocale> CreateDefaultLocale()
{
    AutoPtr<CLocale> l;
    CLocale::NewByFriend((CLocale**)&l);
    return (ILocale*)l.Get();
}

AutoPtr<ILocale> CLocale::sDefaultLocale = CreateDefaultLocale();

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
    if (language.IsEmpty() && country.IsEmpty()){
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

/**
 * Returns the system's installed locales. This array always includes {@code
 * Locale.US}, and usually several others. Most locale-sensitive classes
 * offer their own {@code getAvailableLocales} method, which should be
 * preferred over this general purpose method.
 *
 * @see java.text.BreakIterator#getAvailableLocales()
 * @see java.text.Collator#getAvailableLocales()
 * @see java.text.DateFormat#getAvailableLocales()
 * @see java.text.DateFormatSymbols#getAvailableLocales()
 * @see java.text.DecimalFormatSymbols#getAvailableLocales()
 * @see java.text.NumberFormat#getAvailableLocales()
 * @see java.util.Calendar#getAvailableLocales()
 */
ECode CLocale::GetAvailableLocales(
    /* [out] */ ArrayOf<ILocale*>** locales)
{
    VALIDATE_NOT_NULL(locales);

    return ICU::GetAvailableLocales(locales);
}

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
 * Returns the user's preferred locale. This may have been overridden for
 * this process with {@link #setDefault}.
 *
 * <p>Since the user's locale changes dynamically, avoid caching this value.
 * Instead, use this method to look it up for each use.
 */
AutoPtr<ILocale> CLocale::GetDefault()
{
    return sDefaultLocale;
}

/**
 * Equivalent to {@code getDisplayCountry(Locale.getDefault())}.
 */
ECode CLocale::GetDisplayCountry(
    /* [out] */ String* country)
{
    VALIDATE_NOT_NULL(country);

    return GetDisplayCountryEx(GetDefault(), country);
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
    VALIDATE_NOT_NULL(country);

    if (mCountryCode.IsEmpty()) {
        *country = String("");
        return NOERROR;
    }
    String thisStr, locStr;
    ToString(&thisStr);
    locale->ToString(&locStr);
    String result = ICU::GetDisplayCountry(thisStr, locStr);
    if (result.IsNull()) { // TODO: do we need to do this, or does ICU do it for us?
        GetDefault()->ToString(&locStr);
        result = ICU::GetDisplayCountry(thisStr, locStr);
    }
    *country = result;
    return NOERROR;
}

/**
 * Equivalent to {@code getDisplayLanguage(Locale.getDefault())}.
 */
ECode CLocale::GetDisplayLanguage(
    /* [out] */ String* language)
{
    VALIDATE_NOT_NULL(language);

    return GetDisplayLanguageEx(GetDefault(), language);
}

/**
 * Returns the name of this locale's language, localized to {@code locale}.
 * If the language name is unknown, the language code is returned.
 */
ECode CLocale::GetDisplayLanguageEx(
    /* [in] */ ILocale* locale,
    /* [out] */ String* language)
{
    VALIDATE_NOT_NULL(language);

    if (mLanguageCode.IsEmpty()) {
        *language = String("");
        return NOERROR;
    }
    String thisStr, locStr;
    ToString(&thisStr);
    locale->ToString(&locStr);
    String result = ICU::GetDisplayLanguage(thisStr, locStr);
    if (result.IsNull()) { // TODO: do we need to do this, or does ICU do it for us?
        GetDefault()->ToString(&locStr);
        result = ICU::GetDisplayLanguage(thisStr, locStr);
    }
    *language = result;
    return NOERROR;
}

/**
 * Equivalent to {@code getDisplayName(Locale.getDefault())}.
 */
ECode CLocale::GetDisplayName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);

    return GetDisplayNameEx(GetDefault(), name);;
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
    VALIDATE_NOT_NULL(name);

    Int32 count = 0;
    StringBuffer buffer;
    if (!mLanguageCode.IsEmpty()) {
        String displayLanguage;
        GetDisplayLanguageEx(locale, &displayLanguage);
        if (displayLanguage.IsEmpty()) {
            buffer += mLanguageCode;
        }
        else {
            buffer += displayLanguage;
        }
        ++count;
    }
    if (!mCountryCode.IsEmpty()) {
        if (count == 1) {
            buffer += " (";
        }
        String displayCountry;
        GetDisplayCountryEx(locale, &displayCountry);
        if (displayCountry.IsEmpty()) {
            buffer += mCountryCode;
        }
        else {
            buffer += displayCountry;
        }
        ++count;
    }
    if (!mVariantCode.IsEmpty()) {
        if (count == 1) {
            buffer += " (";
        }
        else if (count == 2) {
            buffer += ",";
        }
        String displayVariant;
        GetDisplayVariantEx(locale, &displayVariant);
        if (displayVariant.IsEmpty()) {
            buffer += mVariantCode;
        }
        else {
            buffer += displayVariant;
        }
        ++count;
    }
    if (count > 1) {
        buffer += ")";
    }
    *name = (const char*)buffer;
    return NOERROR;
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
    VALIDATE_NOT_NULL(variantName);

    return GetDisplayVariantEx(GetDefault(), variantName);;
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
    VALIDATE_NOT_NULL(variantName);

    if (mVariantCode.GetLength() == 0) {
        *variantName = mVariantCode;
        return NOERROR;
    }
    String thisStr, locStr;
    ToString(&thisStr);
    locale->ToString(&locStr);
    String result = ICU::GetDisplayVariant(thisStr, locStr);
    if (result.IsNull()) { // TODO: do we need to do this, or does ICU do it for us?
        GetDefault()->ToString(&locStr);
        result = ICU::GetDisplayVariant(thisStr, locStr);
    }
    *variantName = result;
    return NOERROR;
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
    String thisStr;
    ToString(&thisStr);
    *country = ICU::GetISO3Country(thisStr);
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
    String thisStr;
    ToString(&thisStr);
    *language = ICU::GetISO3Language(thisStr);
    return NOERROR;
}

/**
 * Gets the list of two letter ISO country codes which can be used as the
 * country code for a {@code Locale}.
 *
 * @return an array of strings.
 */
ECode CLocale::GetISOCountries(
    /* [out] */ ArrayOf<String>** codes)
{
    VALIDATE_NOT_NULL(codes);

    return ICU::GetISOCountries(codes);
}

/**
 * Gets the list of two letter ISO language codes which can be used as the
 * language code for a {@code Locale}.
 *
 * @return an array of strings.
 */
ECode CLocale::GetISOLanguages(
    /* [out] */ ArrayOf<String>** codes)
{
    VALIDATE_NOT_NULL(codes);

    return ICU::GetISOLanguages(codes);
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

/**
 * Overrides the default locale. This does not affect system configuration,
 * and attempts to override the system-provided default locale may
 * themselves be overridden by actual changes to the system configuration.
 * Code that calls this method is usually incorrect, and should be fixed by
 * passing the appropriate locale to each locale-sensitive method that's
 * called.
 */
ECode CLocale::SetDefault(
    /* [in] */ ILocale* locale)
{
    if (locale == NULL) {
        // throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }

//    SecurityManager security = System.getSecurityManager();
//    if (security != null) {
//        security.checkPermission(setLocalePermission);
//    }

    sDefaultLocale = locale;
    return NOERROR;
}

ECode CLocale::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    if (mCachedToStringResult.IsNull()) {
        mCachedToStringResult = ToNewString();
    }
    *str = mCachedToStringResult;
    return NOERROR;
}

String CLocale::ToNewString()
{
    // // The string form of a locale that only has a variant is the empty string.
    if (mLanguageCode.GetLength() == 0 && mCountryCode.GetLength() == 0) {
        return String("");
    }
    // // Otherwise, the output format is "ll_cc_variant", where language and country are always
    // // two letters, but the variant is an arbitrary length. A size of 11 characters has room
    // // for "en_US_POSIX", the largest "common" value. (In practice, the string form is almost
    // // always 5 characters: "ll_cc".)
    StringBuffer result(11);
    result += mLanguageCode;
    if (mCountryCode.GetLength() > 0 || mVariantCode.GetLength() > 0) {
         result += String("_");
    }
    result += mCountryCode;
    if (mVariantCode.GetLength() > 0) {
        result += String("_");
    }
    result += mVariantCode;
    return String((const char*)result);
}
