
#include "CDateFormatSymbolsHelper.h"
#include "DateFormatSymbols.h"

ECode CDateFormatSymbolsHelper::GetInstance(
    /* [out] */ IDateFormatSymbols** instance)
{
    return DateFormatSymbols::GetInstance(instance);
}

ECode CDateFormatSymbolsHelper::GetInstanceEx(
    /* [in] */ ILocale* locale,
    /* [out] */ IDateFormatSymbols** instance)
{
    return DateFormatSymbols::GetInstance(locale, instance);
}

ECode CDateFormatSymbolsHelper::GetAvailableLocales(
    /* [out, callee] */ ArrayOf<ILocale*>** locales)
{
    return DateFormatSymbols::GetAvailableLocales(locales);
}

