
#ifndef __CDATEFORMATSYMBOLSHELPER_H__
#define __CDATEFORMATSYMBOLSHELPER_H__

#include "_CDateFormatSymbolsHelper.h"

CarClass(CDateFormatSymbolsHelper)
{
public:
    CARAPI GetInstance(
        /* [out] */ IDateFormatSymbols** instance);

    CARAPI GetInstanceEx(
        /* [in] */ ILocale* locale,
        /* [out] */ IDateFormatSymbols** instance);

    CARAPI GetAvailableLocales(
        /* [out, callee] */ ArrayOf<ILocale*>** locales);

private:
    // TODO: Add your private member variables here.
};

#endif // __CDATEFORMATSYMBOLSHELPER_H__
