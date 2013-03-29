
#include "cmdef.h"
#include "CDecimalFormatSymbols.h"


ECode CDecimalFormatSymbols::constructor()
{
    return DecimalFormatSymbols::Init();
}

ECode CDecimalFormatSymbols::constructor(
    /* [in] */ ILocale* locale)
{
    return DecimalFormatSymbols::Init(locale);
}

ECode CDecimalFormatSymbols::GetCurrency(
    /* [out] */ ICurrency** currency)
{
    VALIDATE_NOT_NULL(currency);
    return DecimalFormatSymbols::GetCurrency(currency);
}

ECode CDecimalFormatSymbols::GetInternationalCurrencySymbol(
    /* [out] */ String* symbol)
{
    VALIDATE_NOT_NULL(symbol);
    return DecimalFormatSymbols::GetInternationalCurrencySymbol(symbol);
}

ECode CDecimalFormatSymbols::GetCurrencySymbol(
    /* [out] */ String* symbol)
{
    VALIDATE_NOT_NULL(symbol);
    return DecimalFormatSymbols::GetCurrencySymbol(symbol);
}

ECode CDecimalFormatSymbols::GetDecimalSeparator(
    /* [out] */ Char32* separator)
{
    VALIDATE_NOT_NULL(separator);
    return DecimalFormatSymbols::GetDecimalSeparator(separator);
}

ECode CDecimalFormatSymbols::GetDigit(
    /* [out] */ Char32* digit)
{
    VALIDATE_NOT_NULL(digit);
    return DecimalFormatSymbols::GetDigit(digit);
}

ECode CDecimalFormatSymbols::GetGroupingSeparator(
    /* [out] */ Char32* separator)
{
    VALIDATE_NOT_NULL(separator);
    return DecimalFormatSymbols::GetGroupingSeparator(separator);
}

ECode CDecimalFormatSymbols::GetInfinity(
    /* [out] */ String* infinity)
{
    VALIDATE_NOT_NULL(infinity);
    return DecimalFormatSymbols::GetInfinity(infinity);
}

ECode CDecimalFormatSymbols::GetMinusSign(
    /* [out] */ Char32* minusSign)
{
    VALIDATE_NOT_NULL(minusSign);
    return DecimalFormatSymbols::GetMinusSign(minusSign);
}

ECode CDecimalFormatSymbols::GetMonetaryDecimalSeparator(
    /* [out] */ Char32* separator)
{
    VALIDATE_NOT_NULL(separator);
    return DecimalFormatSymbols::GetMonetaryDecimalSeparator(separator);
}

ECode CDecimalFormatSymbols::GetNaN(
    /* [out] */ String* NaN)
{
    VALIDATE_NOT_NULL(NaN);
    return DecimalFormatSymbols::GetNaN(NaN);
}

ECode CDecimalFormatSymbols::GetPatternSeparator(
    /* [out] */ Char32* separator)
{
    VALIDATE_NOT_NULL(separator);
    return DecimalFormatSymbols::GetPatternSeparator(separator);
}

ECode CDecimalFormatSymbols::GetPercent(
    /* [out] */ Char32* percent)
{
    VALIDATE_NOT_NULL(percent);
    return DecimalFormatSymbols::GetPercent(percent);
}

ECode CDecimalFormatSymbols::GetPerMill(
    /* [out] */ Char32* perMill)
{
    VALIDATE_NOT_NULL(perMill);
    return DecimalFormatSymbols::GetPerMill(perMill);
}

ECode CDecimalFormatSymbols::GetZeroDigit(
    /* [out] */ Char32* zeroDigit)
{
    VALIDATE_NOT_NULL(zeroDigit);
    return DecimalFormatSymbols::GetZeroDigit(zeroDigit);
}

ECode CDecimalFormatSymbols::GetExponentSeparator(
    /* [out] */ String* separator)
{
    VALIDATE_NOT_NULL(separator);
    return DecimalFormatSymbols::GetExponentSeparator(separator);
}

ECode CDecimalFormatSymbols::SetCurrency(
    /* [in] */ ICurrency* currency)
{
    return DecimalFormatSymbols::SetCurrency(currency);
}

ECode CDecimalFormatSymbols::SetInternationalCurrencySymbol(
    /* [in] */ const String& value)
{
    return DecimalFormatSymbols::SetInternationalCurrencySymbol(value);
}

ECode CDecimalFormatSymbols::SetCurrencySymbol(
    /* [in] */ const String& value)
{
    return DecimalFormatSymbols::SetCurrencySymbol(value);
}

ECode CDecimalFormatSymbols::SetDecimalSeparator(
    /* [in] */ Char32 value)
{
    return DecimalFormatSymbols::SetDecimalSeparator(value);
}

ECode CDecimalFormatSymbols::SetDigit(
    /* [in] */ Char32 value)
{
    return DecimalFormatSymbols::SetDigit(value);
}

ECode CDecimalFormatSymbols::SetGroupingSeparator(
    /* [in] */ Char32 value)
{
    return DecimalFormatSymbols::SetGroupingSeparator(value);
}

ECode CDecimalFormatSymbols::SetInfinity(
    /* [in] */ const String& value)
{
    return DecimalFormatSymbols::SetInfinity(value);
}

ECode CDecimalFormatSymbols::SetMinusSign(
    /* [in] */ Char32 value)
{
    return DecimalFormatSymbols::SetMinusSign(value);
}

ECode CDecimalFormatSymbols::SetMonetaryDecimalSeparator(
    /* [in] */ Char32 value)
{
    return DecimalFormatSymbols::SetMonetaryDecimalSeparator(value);
}

ECode CDecimalFormatSymbols::SetNaN(
    /* [in] */ const String& value)
{
    return DecimalFormatSymbols::SetNaN(value);
}

ECode CDecimalFormatSymbols::SetPatternSeparator(
    /* [in] */ Char32 value)
{
    return DecimalFormatSymbols::SetPatternSeparator(value);
}

ECode CDecimalFormatSymbols::SetPercent(
    /* [in] */ Char32 value)
{
    return DecimalFormatSymbols::SetPercent(value);
}

ECode CDecimalFormatSymbols::SetPerMill(
    /* [in] */ Char32 value)
{
    return DecimalFormatSymbols::SetPerMill(value);
}

ECode CDecimalFormatSymbols::SetZeroDigit(
    /* [in] */ Char32 value)
{
    return DecimalFormatSymbols::SetZeroDigit(value);
}

ECode CDecimalFormatSymbols::SetExponentSeparator(
    /* [in] */ const String& value)
{
    return DecimalFormatSymbols::SetExponentSeparator(value);
}

ECode CDecimalFormatSymbols::Clone(
    /* [out] */ IDecimalFormatSymbols** object)
{
    VALIDATE_NOT_NULL(object);
    return DecimalFormatSymbols::Clone(object);
}
