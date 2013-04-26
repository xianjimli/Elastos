
#include "CDecimalFormatSymbolsHelper.h"
#include "cmdef.h"
#include <elastos/AutoPtr.h>
#include "CDecimalFormatSymbols.h"

ECode CDecimalFormatSymbolsHelper::GetInstance(
    /* [out] */ IDecimalFormatSymbols ** instance)
{
    VALIDATE_NOT_NULL(instance);

    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    AutoPtr<ILocale> locale;
    FAIL_RETURN(localeHelper->GetDefault((ILocale**)&locale));
    return GetInstanceEx(locale, instance);
}

ECode CDecimalFormatSymbolsHelper::GetInstanceEx(
    /* [in] */ ILocale * locale,
    /* [out] */ IDecimalFormatSymbols ** instance)
{
    VALIDATE_NOT_NULL(instance);

    if (locale == NULL) {
        //throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    return CDecimalFormatSymbols::New(locale, instance);
}

ECode CDecimalFormatSymbolsHelper::GetAvailableLocales(
    /* [out, callee] */ ArrayOf<ILocale *> ** locales)
{
    VALIDATE_NOT_NULL(locales);

    AutoPtr<IICUHelper> ICUHelper;
    CICUHelper::AcquireSingleton((IICUHelper**)&ICUHelper);
    return ICUHelper->GetAvailableDecimalFormatSymbolsLocales(locales);
}

