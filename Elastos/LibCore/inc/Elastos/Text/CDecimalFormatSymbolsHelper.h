
#ifndef __CDECIMALFORMATSYMBOLSHELPER_H__
#define __CDECIMALFORMATSYMBOLSHELPER_H__

#include "_CDecimalFormatSymbolsHelper.h"

CarClass(CDecimalFormatSymbolsHelper)
{
public:
    CARAPI GetInstance(
        /* [out] */ IDecimalFormatSymbols ** instance);

    CARAPI GetInstanceEx(
        /* [in] */ ILocale * locale,
        /* [out] */ IDecimalFormatSymbols ** instance);

    CARAPI GetAvailableLocales(
        /* [out, callee] */ ArrayOf<ILocale *> ** locales);

private:
    // TODO: Add your private member variables here.
};

#endif // __CDECIMALFORMATSYMBOLSHELPER_H__
