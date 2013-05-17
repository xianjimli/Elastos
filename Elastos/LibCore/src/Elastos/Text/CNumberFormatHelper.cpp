
#include "cmdef.h"
#include "CNumberFormatHelper.h"
#include "NumberFormat.h"


ECode CNumberFormatHelper::GetAvailableLocales(
    /* [out, callee] */ ArrayOf<ILocale*>** locales)
{
    VALIDATE_NOT_NULL(locales);

    return NumberFormat::GetAvailableLocales(locales);
}

ECode CNumberFormatHelper::GetCurrencyInstance(
    /* [out] */ INumberFormat** instance)
{
    VALIDATE_NOT_NULL(instance);

    return NumberFormat::GetCurrencyInstance(instance);
}

ECode CNumberFormatHelper::GetCurrencyInstanceEx(
    /* [in] */ ILocale* locale,
    /* [out] */ INumberFormat** instance)
{
    VALIDATE_NOT_NULL(instance);

    return NumberFormat::GetCurrencyInstance(locale, instance);
}

ECode CNumberFormatHelper::GetIntegerInstance(
    /* [out] */ INumberFormat** instance)
{
    VALIDATE_NOT_NULL(instance);

    return NumberFormat::GetIntegerInstance(instance);
}

ECode CNumberFormatHelper::GetIntegerInstanceEx(
    /* [in] */ ILocale* locale,
    /* [out] */ INumberFormat** instance)
{
    VALIDATE_NOT_NULL(instance);

    return NumberFormat::GetIntegerInstance(locale, instance);
}

ECode CNumberFormatHelper::GetInstance(
    /* [out] */ INumberFormat** instance)
{
    VALIDATE_NOT_NULL(instance);

    return NumberFormat::GetInstance(instance);
}

ECode CNumberFormatHelper::GetInstanceEx(
    /* [in] */ ILocale* locale,
    /* [out] */ INumberFormat** instance)
{
    VALIDATE_NOT_NULL(instance);

    return NumberFormat::GetInstance(locale, instance);
}

ECode CNumberFormatHelper::GetNumberInstance(
    /* [out] */ INumberFormat** instance)
{
    VALIDATE_NOT_NULL(instance);

    return NumberFormat::GetNumberInstance(instance);
}

ECode CNumberFormatHelper::GetNumberInstanceEx(
    /* [in] */ ILocale* locale,
    /* [out] */ INumberFormat** instance)
{
    VALIDATE_NOT_NULL(instance);

    return NumberFormat::GetNumberInstance(locale, instance);
}

ECode CNumberFormatHelper::GetPercentInstance(
    /* [out] */ INumberFormat** instance)
{
    VALIDATE_NOT_NULL(instance);

    return NumberFormat::GetPercentInstance(instance);
}

ECode CNumberFormatHelper::GetPercentInstanceEx(
    /* [in] */ ILocale* locale,
    /* [out] */ INumberFormat** instance)
{
    VALIDATE_NOT_NULL(instance);

    return NumberFormat::GetPercentInstance(instance);
}

