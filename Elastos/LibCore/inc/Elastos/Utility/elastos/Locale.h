
#ifndef __HH_LOCALE_H
#define __HH_LOCALE_H

#include <elastos.h>

using namespace Elastos;

namespace Elastos {
namespace Utility {

class Locale
{
public:
    /**
     * Returns the user's preferred locale. This may have been overridden for
     * this process with {@link #setDefault}.
     *
     * <p>Since the user's locale changes dynamically, avoid caching this value.
     * Instead, use this method to look it up for each use.
     */
    static CARAPI_(Locale*) GetDefault();

public:
    /**
     * Returns the country code for this locale, or {@code ""} if this locale
     * doesn't correspond to a specific country.
     */
    CARAPI_(String) GetCountry();

    /**
     * Gets the language code for this {@code Locale} or the empty string of no language
     * was set.
     *
     * @return a language code.
     */
    CARAPI_(String) GetLanguage();

    CARAPI_(Locale*) Clone();
};

} // Utility
} // Elastos

#endif // __HH_LOCALE_H
