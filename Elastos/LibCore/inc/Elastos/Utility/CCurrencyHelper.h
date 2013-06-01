
#ifndef __CCURRENCYHELPER_H__
#define __CCURRENCYHELPER_H__

#include "_CCurrencyHelper.h"

CarClass(CCurrencyHelper)
{
public:
    CARAPI GetInstance(
        /* [in] */ const String& currencyCode,
        /* [out] */ ICurrency** currency);

    CARAPI GetInstanceEx(
        /* [in] */ ILocale* locale,
        /* [out] */ ICurrency** currency);
};

#endif //__CCURRENCYHELPER_H__
