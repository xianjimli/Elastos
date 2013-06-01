
#ifndef __CLOCALEDATAHELPER_H__
#define __CLOCALEDATAHELPER_H__

#include "_CLocaleDataHelper.h"

CarClass(CLocaleDataHelper)
{
public:
    /**
     * Returns a shared LocaleData for the given locale.
     */
    CARAPI Get(
        /* [in] */ ILocale* locale,
        /* [out] */ ILocaleData** localeData);
};

#endif // __CLOCALEDATAHELPER_H__
