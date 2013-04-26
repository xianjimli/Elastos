
#include "CDateFormatSymbolsHelper.h"
#include <elastos/AutoPtr.h>
#include "CDateFormatSymbols.h"

ECode CDateFormatSymbolsHelper::GetInstance(
    /* [out] */ IDateFormatSymbols ** instance)
{
    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    AutoPtr<ILocale> locale;
    localeHelper->GetDefault((ILocale**)&locale);
    return GetInstanceEx((ILocale*)locale, instance);
}

ECode CDateFormatSymbolsHelper::GetInstanceEx(
    /* [in] */ ILocale * locale,
    /* [out] */ IDateFormatSymbols ** instance)
{
    if (locale == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    CDateFormatSymbols::New(locale, instance);
    return NOERROR;
}

ECode CDateFormatSymbolsHelper::GetAvailableLocales(
    /* [out, callee] */ ArrayOf<ILocale *> ** arrayOfLocales)
{
    AutoPtr<IICUHelper> icuHelper;
    CICUHelper::AcquireSingleton((IICUHelper**)&icuHelper);
    return icuHelper->GetAvailableDateFormatSymbolsLocales(arrayOfLocales);
}

