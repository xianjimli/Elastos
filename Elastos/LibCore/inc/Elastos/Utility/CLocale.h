
#ifndef __CLOCALE_H__
#define __CLOCALE_H__

#include "_CLocale.h"
#include <elastos/AutoPtr.h>
#include "CICUHelper.h"

CarClass(CLocale)
{
public:
    /**
     * Returns the country code for this locale, or {@code ""} if this locale
     * doesn't correspond to a specific country.
     */
    CARAPI GetCountry(
        /* [out] */ String* country);

    /**
     * Equivalent to {@code getDisplayCountry(Locale.getDefault())}.
     */
    CARAPI GetDisplayCountry(
        /* [out] */ String* country);

     /**
     * Returns the name of this locale's country, localized to {@code locale}.
     * Returns the empty string if this locale does not correspond to a specific
     * country.
     */
    CARAPI GetDisplayCountryEx(
        /* [in] */ ILocale* locale,
        /* [out] */ String* country);

    /**
     * Equivalent to {@code getDisplayLanguage(Locale.getDefault())}.
     */
    CARAPI GetDisplayLanguage(
        /* [out] */ String* language);

    /**
     * Returns the name of this locale's language, localized to {@code locale}.
     * If the language name is unknown, the language code is returned.
     */
    CARAPI GetDisplayLanguageEx(
        /* [in] */ ILocale* locale,
        /* [out] */ String* language);

    /**
     * Equivalent to {@code getDisplayName(Locale.getDefault())}.
     */
    CARAPI GetDisplayName(
        /* [out] */ String* name);

    /**
     * Returns this locale's language name, country name, and variant, localized
     * to {@code locale}. The exact output form depends on whether this locale
     * corresponds to a specific language, country and variant, such as:
     * {@code English}, {@code English (United States)}, {@code English (United
     * States,Computer)}, {@code anglais (&#x00c9;tats-Unis)}, {@code anglais
     * (&#x00c9;tats-Unis,informatique)}.
     */
    CARAPI GetDisplayNameEx(
        /* [in] */ ILocale* locale,
        /* [out] */ String* name);

    /**
     * Gets the full variant name in the default {@code Locale} for the variant code of
     * this {@code Locale}. If there is no matching variant name, the variant code is
     * returned.
     *
     * @return a variant name.
     */
    CARAPI GetDisplayVariant(
        /* [out] */ String* variantName);

    /**
     * Gets the full variant name in the specified {@code Locale} for the variant code
     * of this {@code Locale}. If there is no matching variant name, the variant code is
     * returned.
     *
     * @param locale
     *            the {@code Locale} for which the display name is retrieved.
     * @return a variant name.
     */
    CARAPI GetDisplayVariantEx(
        /* [in] */ ILocale* locale,
        /* [out] */ String* variantName);

     /**
     * Gets the three letter ISO country code which corresponds to the country
     * code for this {@code Locale}.
     *
     * @return a three letter ISO language code.
     * @throws MissingResourceException
     *                if there is no matching three letter ISO country code.
     */
    CARAPI GetISO3Country(
        /* [out] */ String* country);

    /**
     * Gets the three letter ISO language code which corresponds to the language
     * code for this {@code Locale}.
     *
     * @return a three letter ISO language code.
     * @throws MissingResourceException
     *                if there is no matching three letter ISO language code.
     */
    CARAPI GetISO3Language(
        /* [out] */ String* language);

    /**
     * Gets the language code for this {@code Locale} or the empty string of no language
     * was set.
     *
     * @return a language code.
     */
    CARAPI GetLanguage(
        /* [out] */ String* language);

    /**
     * Gets the variant code for this {@code Locale} or an empty {@code String} if no variant
     * was set.
     *
     * @return a variant code.
     */
    CARAPI GetVariant(
        /* [out] */ String* variant);

    /**
     * Returns the string representation of this {@code Locale}. It consists of the
     * language code, country code and variant separated by underscores.
     * If the language is missing the string begins
     * with an underscore. If the country is missing there are 2 underscores
     * between the language and the variant. The variant cannot stand alone
     * without a language and/or country code: in this case this method would
     * return the empty string.
     *
     * <p>Examples: "en", "en_US", "_US", "en__POSIX", "en_US_POSIX"
     *
     * @return the string representation of this {@code Locale}.
     */
    //@Override
    CARAPI_(String) ToString();

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI constructor();

    /**
     * Constructs a new {@code Locale} using the specified language.
     */
    CARAPI constructor(
        /* [in] */ const String& language);

    /**
     * Constructs a new {@code Locale} using the specified language and country codes.
     */
    CARAPI constructor(
        /* [in] */ const String& language,
        /* [in] */ const String& country);

    /**
     * Constructs a new {@code Locale} using the specified language, country,
     * and variant codes.
     */
    CARAPI constructor(
        /* [in] */ const String& language,
        /* [in] */ const String& country,
        /* [in] */ const String& variant);

private:
    CARAPI_(String) ToNewString();

private:
    String mCountryCode;
    String mLanguageCode;
    String mVariantCode;
    String mCachedToStringResult;
};

#endif //__CLOCALE_H__

