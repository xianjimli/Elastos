
#include "CNumberFormatHelper.h"
#include <elastos/AutoPtr.h>
#include "cmdef.h"

ECode CNumberFormatHelper::GetAvailableLocales(
    /* [out, callee] */ ArrayOf<ILocale *> ** locales)
{
    AutoPtr<IICUHelper> ICUHelper;
    FAIL_RETURN(CICUHelper::AcquireSingleton((IICUHelper **)&ICUHelper));
    return ICUHelper->GetAvailableNumberFormatLocales(locales);
}

ECode CNumberFormatHelper::GetCurrencyInstance(
    /* [out] */ INumberFormat ** instance)
{
    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    AutoPtr<ILocale> locale;
    FAIL_RETURN(localeHelper->GetDefault((ILocale**)&locale));
    return GetCurrencyInstanceEx(locale, instance);
}

ECode CNumberFormatHelper::GetCurrencyInstanceEx(
    /* [in] */ ILocale * locale,
    /* [out] */ INumberFormat ** instance)
{
    assert(0);
    //return getInstance(LocaleData.get(locale).currencyPattern, locale);
    return NOERROR;
}

ECode CNumberFormatHelper::GetIntegerInstance(
    /* [out] */ INumberFormat ** instance)
{
    AutoPtr<ILocaleHelper> pLocaleHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&pLocaleHelper);
    AutoPtr<ILocale> pLocale;
    pLocaleHelper->GetDefault((ILocale**)&pLocale);
    return GetIntegerInstanceEx(pLocale, instance);
}

ECode CNumberFormatHelper::GetIntegerInstanceEx(
    /* [in] */ ILocale * locale,
    /* [out] */ INumberFormat ** instance)
{
    assert(0);
    //NumberFormat result = getInstance(LocaleData.get(locale).integerPattern, locale);
    //result.setParseIntegerOnly(true);
    //return result;
    return NOERROR;
}

ECode CNumberFormatHelper::GetInstance(
    /* [out] */ INumberFormat ** instance)
{
    return GetNumberInstance(instance);
}

ECode CNumberFormatHelper::GetInstanceEx(
    /* [in] */ ILocale * locale,
    /* [out] */ INumberFormat ** instance)
{
    return GetNumberInstanceEx(locale, instance);
}

ECode CNumberFormatHelper::GetNumberInstance(
    /* [out] */ INumberFormat ** instance)
{
    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    AutoPtr<ILocale> locale;
    FAIL_RETURN(localeHelper->GetDefault((ILocale**)&locale));
    return GetNumberInstanceEx(locale, instance);
}

ECode CNumberFormatHelper::GetNumberInstanceEx(
    /* [in] */ ILocale * locale,
    /* [out] */ INumberFormat ** instance)
{
    assert(0);
//    return getInstance(LocaleData.get(locale).numberPattern, locale);
    return NOERROR;
}

ECode CNumberFormatHelper::GetPercentInstance(
    /* [out] */ INumberFormat ** instance)
{
    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    AutoPtr<ILocale> locale;
    FAIL_RETURN(localeHelper->GetDefault((ILocale**)&locale));
    return GetPercentInstanceEx(locale, instance);
}

ECode CNumberFormatHelper::GetPercentInstanceEx(
    /* [in] */ ILocale * locale,
    /* [out] */ INumberFormat ** instance)
{
    assert(0);
//    return getInstance(LocaleData.get(locale).percentPattern, locale);
    return NOERROR;
}

