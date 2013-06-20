
#ifndef __CCOLLATORHELPER_H__
#define __CCOLLATORHELPER_H__

#include "_CCollatorHelper.h"

CarClass(CCollatorHelper)
{
public:
    /**
     * Returns an array of locales for which custom {@code Collator} instances
     * are available.
     * <p>Note that Android does not support user-supplied locale service providers.
     */
    CARAPI GetAvailableLocales(
        /* [out, callee] */ ArrayOf<ILocale*>** locales);

    /**
     * Returns a {@code Collator} instance which is appropriate for the user's default
     * {@code Locale}.
     * See "<a href="../util/Locale.html#default_locale">Be wary of the default locale</a>".
     */
    CARAPI GetInstance(
        /* [out] */ ICollator** instance);

    /**
     * Returns a {@code Collator} instance which is appropriate for {@code locale}.
     */
    CARAPI GetInstanceEx(
        /* [in] */ ILocale* locale,
        /* [out] */ ICollator** instance);
};

#endif // __CCOLLATORHELPER_H__
