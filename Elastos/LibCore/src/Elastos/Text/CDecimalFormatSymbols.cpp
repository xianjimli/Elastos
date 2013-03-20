
#include "CDecimalFormatSymbols.h"
ECode CDecimalFormatSymbols::constructor()
{
    return DecimalFormatSymbols::Init();
}

ECode CDecimalFormatSymbols::constructor(
    /* [in] */ ILocale * pLocale)
{
    return DecimalFormatSymbols::Init(pLocale);
}

ECode CDecimalFormatSymbols::GetCurrency(
    /* [out] */ ICurrency ** ppCurrency)
{
    return DecimalFormatSymbols::GetCurrency(ppCurrency);
}

ECode CDecimalFormatSymbols::GetInternationalCurrencySymbol(
    /* [out] */ String * pInternationalCurrencySymbol)
{
    return DecimalFormatSymbols::GetInternationalCurrencySymbol(pInternationalCurrencySymbol);
}

ECode CDecimalFormatSymbols::GetCurrencySymbol(
    /* [out] */ String * pCurrencySymbol)
{
    return DecimalFormatSymbols::GetCurrencySymbol(pCurrencySymbol);
}

ECode CDecimalFormatSymbols::GetDecimalSeparator(
    /* [out] */ Char32 * pDecimalSeparator)
{
    return DecimalFormatSymbols::GetDecimalSeparator(pDecimalSeparator);
}

ECode CDecimalFormatSymbols::GetDigit(
    /* [out] */ Char32 * pDigit)
{
    return DecimalFormatSymbols::GetDigit(pDigit);
}

ECode CDecimalFormatSymbols::GetGroupingSeparator(
    /* [out] */ Char32 * pGroupingSeparator)
{
    return DecimalFormatSymbols::GetGroupingSeparator(pGroupingSeparator);
}

ECode CDecimalFormatSymbols::GetInfinity(
    /* [out] */ String * pInfinity)
{
    return DecimalFormatSymbols::GetInfinity(pInfinity);
}

ECode CDecimalFormatSymbols::GetMinusSign(
    /* [out] */ Char32 * pMinusSign)
{
    return DecimalFormatSymbols::GetMinusSign(pMinusSign);
}

ECode CDecimalFormatSymbols::GetMonetaryDecimalSeparator(
    /* [out] */ Char32 * pMonetaryDecimalSeparator)
{
    return DecimalFormatSymbols::GetMonetaryDecimalSeparator(pMonetaryDecimalSeparator);
}

ECode CDecimalFormatSymbols::GetNaN(
    /* [out] */ String * pNaN)
{
    return DecimalFormatSymbols::GetNaN(pNaN);
}

ECode CDecimalFormatSymbols::GetPatternSeparator(
    /* [out] */ Char32 * pPatternSeparator)
{
    return DecimalFormatSymbols::GetPatternSeparator(pPatternSeparator);
}

ECode CDecimalFormatSymbols::GetPercent(
    /* [out] */ Char32 * pPercent)
{
    return DecimalFormatSymbols::GetPercent(pPercent);
}

ECode CDecimalFormatSymbols::GetPerMill(
    /* [out] */ Char32 * pPerMill)
{
    return DecimalFormatSymbols::GetPerMill(pPerMill);
}

ECode CDecimalFormatSymbols::GetZeroDigit(
    /* [out] */ Char32 * pZeroDigit)
{
    return DecimalFormatSymbols::GetZeroDigit(pZeroDigit);
}

ECode CDecimalFormatSymbols::GetExponentSeparator(
    /* [out] */ String * pExponentSeparator)
{
    return DecimalFormatSymbols::GetExponentSeparator(pExponentSeparator);
}

ECode CDecimalFormatSymbols::SetCurrency(
    /* [in] */ ICurrency * pCurrency)
{
    return DecimalFormatSymbols::SetCurrency(pCurrency);
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