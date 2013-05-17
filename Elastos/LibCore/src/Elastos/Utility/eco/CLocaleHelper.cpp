
#include "cmdef.h"
#include "CLocaleHelper.h"


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
ECode CLocaleHelper::GetAvailableLocales(
    /* [out] */ ArrayOf<ILocale*>** locales)
{
    VALIDATE_NOT_NULL(locales);

    return CLocale::GetAvailableLocales(locales);
}

/**
 * Returns the user's preferred locale. This may have been overridden for
 * this process with {@link #setDefault}.
 *
 * <p>Since the user's locale changes dynamically, avoid caching this value.
 * Instead, use this method to look it up for each use.
 */
ECode CLocaleHelper::GetDefault(
    /* [out] */ ILocale** locale)
{
    VALIDATE_NOT_NULL(locale);

    AutoPtr<ILocale> l = CLocale::GetDefault();
    *locale = l;
    (*locale)->AddRef();
    return NOERROR;
}

/**
 * Gets the list of two letter ISO country codes which can be used as the
 * country code for a {@code Locale}.
 *
 * @return an array of strings.
 */
ECode CLocaleHelper::GetISOCountries(
    /* [out] */ ArrayOf<String>** codes)
{
    VALIDATE_NOT_NULL(codes);

    return CLocale::GetISOCountries(codes);
}

/**
 * Gets the list of two letter ISO language codes which can be used as the
 * language code for a {@code Locale}.
 *
 * @return an array of strings.
 */
ECode CLocaleHelper::GetISOLanguages(
    /* [out] */ ArrayOf<String>** codes)
{
    VALIDATE_NOT_NULL(codes);

    return CLocale::GetISOLanguages(codes);
}

/**
 * Overrides the default locale. This does not affect system configuration,
 * and attempts to override the system-provided default locale may
 * themselves be overridden by actual changes to the system configuration.
 * Code that calls this method is usually incorrect, and should be fixed by
 * passing the appropriate locale to each locale-sensitive method that's
 * called.
 */
ECode CLocaleHelper::SetDefault(
    /* [in] */ ILocale* locale)
{
    return CLocale::SetDefault(locale);
}
