
#ifndef __CLOCALEHELPER_H__
#define __CLOCALEHELPER_H__

#include "_CLocaleHelper.h"
#include "CLocale.h"
#include <elastos/AutoPtr.h>

CarClass(CLocaleHelper)
{
public:
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
    CARAPI GetAvailableLocales(
        /* [out] */ ArrayOf<ILocale*>** locales);

    /**
     * Returns the user's preferred locale. This may have been overridden for
     * this process with {@link #setDefault}.
     *
     * <p>Since the user's locale changes dynamically, avoid caching this value.
     * Instead, use this method to look it up for each use.
     */
    CARAPI GetDefault(
        /* [out] */ ILocale** defaultLocale);

    CARAPI GetRoot(
        /* [out] */ ILocale** rootLocale);

    /**
     * Gets the list of two letter ISO country codes which can be used as the
     * country code for a {@code Locale}.
     *
     * @return an array of strings.
     */
    CARAPI GetISOCountries(
        /* [out] */ ArrayOf<String>** codes);

    /**
     * Gets the list of two letter ISO language codes which can be used as the
     * language code for a {@code Locale}.
     *
     * @return an array of strings.
     */
    CARAPI GetISOLanguages(
        /* [out] */ ArrayOf<String>** codes);

    /**
     * Overrides the default locale. This does not affect system configuration,
     * and attempts to override the system-provided default locale may
     * themselves be overridden by actual changes to the system configuration.
     * Code that calls this method is usually incorrect, and should be fixed by
     * passing the appropriate locale to each locale-sensitive method that's
     * called.
     */
    CARAPI SetDefault(
        /* [in] */ ILocale* locale);
};

#endif //__CLOCALEHELPER_H__
