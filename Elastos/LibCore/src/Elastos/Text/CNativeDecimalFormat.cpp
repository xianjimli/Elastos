
#include "CNativeDecimalFormat.h"

ECode CNativeDecimalFormat::constructor(
    /* [in] */ const String& pattern,
    /* [in] */ IDecimalFormatSymbols * pDfs)
{
    NativeDecimalFormat::Init(pattern, pDfs);
    return NOERROR;
}

ECode CNativeDecimalFormat::Close()
{
    return NativeDecimalFormat::Close();
}

ECode CNativeDecimalFormat::SetDecimalFormatSymbols(
    /* [in] */ IDecimalFormatSymbols * pDfs)
{
    return NativeDecimalFormat::SetDecimalFormatSymbols(pDfs);
}

ECode CNativeDecimalFormat::FormatLong(
    /* [in] */ Int64 value,
    /* [in] */ IFieldPosition * pField,
    /* [out, callee] */ ArrayOf<Char32> ** ppArray)
{
    return NativeDecimalFormat::FormatLong(value, pField, ppArray);
}

ECode CNativeDecimalFormat::FormatDouble(
    /* [in] */ Double value,
    /* [in] */ IFieldPosition * pField,
    /* [out, callee] */ ArrayOf<Char32> ** ppArray)
{
    return NativeDecimalFormat::FormatDouble(value, pField, ppArray);
}

ECode CNativeDecimalFormat::ApplyLocalizedPattern(
    /* [in] */ const String& pattern)
{
    return NativeDecimalFormat::ApplyLocalizedPattern(pattern);
}

ECode CNativeDecimalFormat::ApplyPattern(
    /* [in] */ const String& pattern)
{
    return NativeDecimalFormat::ApplyPattern(pattern);
}

ECode CNativeDecimalFormat::FormatToCharacterIterator(
    /* [in] */ IInterface * pObject,
    /* [out] */ IAttributedCharacterIterator ** ppResult)
{
    return NativeDecimalFormat::FormatToCharacterIterator(pObject, ppResult);
}

ECode CNativeDecimalFormat::ToLocalizedPattern(
    /* [out] */ String * pLocalizedPattern)
{
    return NativeDecimalFormat::ToLocalizedPattern(pLocalizedPattern);
}

ECode CNativeDecimalFormat::ToPattern(
    /* [out] */ String * pPattern)
{
    return NativeDecimalFormat::ToPattern(pPattern);
}

ECode CNativeDecimalFormat::Parse(
    /* [in] */ const String& string,
    /* [in] */ IParsePosition * pPosition,
    /* [out] */ INumber ** ppResult)
{
    return NativeDecimalFormat::Parse(string, pPosition, ppResult);
}

ECode CNativeDecimalFormat::GetMaximumFractionDigits(
    /* [out] */ Int32 * pMaximumFractionDigits)
{
    return NativeDecimalFormat::GetMaximumFractionDigits(pMaximumFractionDigits);
}

ECode CNativeDecimalFormat::GetMaximumIntegerDigits(
    /* [out] */ Int32 * pMaximumIntegerDigits)
{
    return NativeDecimalFormat::GetMaximumIntegerDigits(pMaximumIntegerDigits);
}

ECode CNativeDecimalFormat::GetMinimumFractionDigits(
    /* [out] */ Int32 * pMinimumFractionDigits)
{
    return NativeDecimalFormat::GetMinimumFractionDigits(pMinimumFractionDigits);
}

ECode CNativeDecimalFormat::GetMinimumIntegerDigits(
    /* [out] */ Int32 * pMinimumIntegerDigits)
{
    return NativeDecimalFormat::GetMinimumIntegerDigits(pMinimumIntegerDigits);
}

ECode CNativeDecimalFormat::GetGroupingSize(
    /* [out] */ Int32 * pGroupingSize)
{
    return NativeDecimalFormat::GetGroupingSize(pGroupingSize);
}

ECode CNativeDecimalFormat::GetMultiplier(
    /* [out] */ Int32 * pMultiplier)
{
    return NativeDecimalFormat::GetMultiplier(pMultiplier);
}

ECode CNativeDecimalFormat::GetNegativePrefix(
    /* [out] */ String * pNegativePrefix)
{
    return NativeDecimalFormat::GetNegativePrefix(pNegativePrefix);
}

ECode CNativeDecimalFormat::GetNegativeSuffix(
    /* [out] */ String * pNegativeSuffix)
{
    return NativeDecimalFormat::GetNegativeSuffix(pNegativeSuffix);
}

ECode CNativeDecimalFormat::GetPositivePrefix(
    /* [out] */ String * pPositivePrefix)
{
    return NativeDecimalFormat::GetPositivePrefix(pPositivePrefix);
}

ECode CNativeDecimalFormat::GetPositiveSuffix(
    /* [out] */ String * pPositiveSuffix)
{
    return NativeDecimalFormat::GetPositiveSuffix(pPositiveSuffix);
}

ECode CNativeDecimalFormat::IsDecimalSeparatorAlwaysShown(
    /* [out] */ Boolean * pIsDecimalSeparatorAlwaysShown)
{
    return NativeDecimalFormat::IsDecimalSeparatorAlwaysShown(pIsDecimalSeparatorAlwaysShown);
}

ECode CNativeDecimalFormat::IsParseBigDecimal(
    /* [out] */ Boolean * pIsParseBigDecimal)
{
    return NativeDecimalFormat::IsParseBigDecimal(pIsParseBigDecimal);
}

ECode CNativeDecimalFormat::IsParseIntegerOnly(
    /* [out] */ Boolean * pIsParseIntegerOnly)
{
    return NativeDecimalFormat::IsParseIntegerOnly(pIsParseIntegerOnly);
}

ECode CNativeDecimalFormat::IsGroupingUsed(
    /* [out] */ Boolean * pIsGroupingUsed)
{
    return NativeDecimalFormat::IsGroupingUsed(pIsGroupingUsed);
}

ECode CNativeDecimalFormat::SetDecimalSeparatorAlwaysShown(
    /* [in] */ Boolean value)
{
    return NativeDecimalFormat::SetDecimalSeparatorAlwaysShown(value);
}

ECode CNativeDecimalFormat::SetCurrency(
    /* [in] */ ICurrency * pCurrency)
{
    return NativeDecimalFormat::SetCurrency(pCurrency);
}

ECode CNativeDecimalFormat::SetGroupingSize(
    /* [in] */ Int32 value)
{
    return NativeDecimalFormat::SetGroupingSize(value);
}

ECode CNativeDecimalFormat::SetGroupingUsed(
    /* [in] */ Boolean value)
{
    return NativeDecimalFormat::SetGroupingUsed(value);
}

ECode CNativeDecimalFormat::SetMaximumFractionDigits(
    /* [in] */ Int32 value)
{
    return NativeDecimalFormat::SetMaximumIntegerDigits(value);
}

ECode CNativeDecimalFormat::SetMaximumIntegerDigits(
    /* [in] */ Int32 value)
{
    return NativeDecimalFormat::SetMaximumIntegerDigits(value);
}

ECode CNativeDecimalFormat::SetMinimumFractionDigits(
    /* [in] */ Int32 value)
{
    return NativeDecimalFormat::SetMaximumFractionDigits(value);
}

ECode CNativeDecimalFormat::SetMinimumIntegerDigits(
    /* [in] */ Int32 value)
{
    return NativeDecimalFormat::SetMinimumIntegerDigits(value);
}

ECode CNativeDecimalFormat::SetMultiplier(
    /* [in] */ Int32 value)
{
    return NativeDecimalFormat::SetMultiplier(value);
}

ECode CNativeDecimalFormat::SetNegativePrefix(
    /* [in] */ const String& value)
{
    return NativeDecimalFormat::SetNegativePrefix(value);
}

ECode CNativeDecimalFormat::SetNegativeSuffix(
    /* [in] */ const String& value)
{
    return NativeDecimalFormat::SetNegativeSuffix(value);
}

ECode CNativeDecimalFormat::SetPositivePrefix(
    /* [in] */ const String& value)
{
    return NativeDecimalFormat::SetPositivePrefix(value);
}

ECode CNativeDecimalFormat::SetPositiveSuffix(
    /* [in] */ const String& value)
{
    return NativeDecimalFormat::SetPositiveSuffix(value);
}

ECode CNativeDecimalFormat::SetParseBigDecimal(
    /* [in] */ Boolean value)
{
    return NativeDecimalFormat::SetParseBigDecimal(value);
}

ECode CNativeDecimalFormat::SetParseIntegerOnly(
    /* [in] */ Boolean value)
{
    return NativeDecimalFormat::SetParseIntegerOnly(value);
}

ECode CNativeDecimalFormat::SetRoundingMode(
    /* [in] */ RRoundingMode roundingMode,
    /* [in] */ Double roundingIncrement)
{
    return NativeDecimalFormat::SetRoundingMode(roundingMode, roundingIncrement);
}

ECode CNativeDecimalFormat::GetAddr(
    /* [out] */ Int32 * pValue)
{
    return NativeDecimalFormat::GetAddr(pValue);
}

ECode CNativeDecimalFormat::GetLastPattern(
    /* [out] */ String * pValue)
{
    return NativeDecimalFormat::GetLastPattern(pValue);
}

ECode CNativeDecimalFormat::GetNegPrefNull(
    /* [out] */ Boolean * pValue)
{
    return NativeDecimalFormat::GetNegPrefNull(pValue);
}

ECode CNativeDecimalFormat::GetNegSuffNull(
    /* [out] */ Boolean * pValue)
{
    return NativeDecimalFormat::GetNegSuffNull(pValue);
}

ECode CNativeDecimalFormat::GetPosPrefNull(
    /* [out] */ Boolean * pValue)
{
    return NativeDecimalFormat::GetPosPrefNull(pValue);
}

ECode CNativeDecimalFormat::GetPosSuffNull(
    /* [out] */ Boolean * pValue)
{
    return NativeDecimalFormat::GetPosSuffNull(pValue);
}


