
#include "CDecimalFormat.h"

ECode CDecimalFormat::constructor()
{
    return DecimalFormat::Init();
}

ECode CDecimalFormat::constructor(
    /* [in] */ const String& pattern)
{
    return DecimalFormat::Init(pattern);
}

ECode CDecimalFormat::constructor(
    /* [in] */ const String& pattern,
    /* [in] */ IDecimalFormatSymbols * pValue)
{
    return DecimalFormat::Init(pattern, pValue);
}

ECode CDecimalFormat::constructor(
    /* [in] */ const String& pattern,
    /* [in] */ ILocale * pLocale)
{
    return DecimalFormat::Init(pattern, pLocale);
}

ECode CDecimalFormat::format(
    /* [in] */ IInterface * pObject,
    /* [out] */ String * pValue)
{
    return DecimalFormat::format(pObject, pValue);
}

ECode CDecimalFormat::FormatToCharacterIterator(
    /* [in] */ IInterface * pObject,
    /* [out] */ IAttributedCharacterIterator ** ppCharactorIterator)
{
    return DecimalFormat::FormatToCharacterIterator(pObject, ppCharactorIterator);
}

ECode CDecimalFormat::ParseObject(
    /* [in] */ const String& string,
    /* [out] */ IInterface ** ppObject)
{
    return DecimalFormat::ParseObject(string, ppObject);
}

ECode CDecimalFormat::ParseObjectEx(
    /* [in] */ const String& string,
    /* [in] */ IParsePosition * pPosition,
    /* [out] */ IInterface ** ppObject)
{
    return DecimalFormat::ParseObjectEx(string, pPosition, ppObject);
}

ECode CDecimalFormat::formatEx2(
    /* [in] */ Double value,
    /* [out] */ String * pFormat)
{
    return DecimalFormat::formatEx2(value, pFormat);
}

ECode CDecimalFormat::formatEx4(
    /* [in] */ Int64 value,
    /* [out] */ String * pFormat)
{
    return DecimalFormat::formatEx4(value, pFormat);
}

ECode CDecimalFormat::GetMaximumFractionDigits(
    /* [out] */ Int32 * pMaximumFractionDigits)
{
    return DecimalFormat::GetMaximumFractionDigits(pMaximumFractionDigits);
}

ECode CDecimalFormat::GetMaximumIntegerDigits(
    /* [out] */ Int32 * pMaximumIntegerDigits)
{
    return DecimalFormat::GetMaximumIntegerDigits(pMaximumIntegerDigits);
}

ECode CDecimalFormat::GetMinimumFractionDigits(
    /* [out] */ Int32 * pMinimumFractionDigits)
{
    return DecimalFormat::GetMinimumFractionDigits(pMinimumFractionDigits);
}

ECode CDecimalFormat::GetMinimumIntegerDigits(
    /* [out] */ Int32 * pMinimumIntegerDigits)
{
    return DecimalFormat::GetMinimumIntegerDigits(pMinimumIntegerDigits);
}

ECode CDecimalFormat::IsGroupingUsed(
    /* [out] */ Boolean * pIsGroupingUsed)
{
    return DecimalFormat::IsGroupingUsed(pIsGroupingUsed);
}

ECode CDecimalFormat::IsParseIntegerOnly(
    /* [out] */ Boolean * pIsParseIntegerOnly)
{
    return DecimalFormat::IsParseIntegerOnly(pIsParseIntegerOnly);
}

ECode CDecimalFormat::Parse(
    /* [in] */ const String& string,
    /* [out] */ INumber ** ppValue)
{
    return DecimalFormat::Parse(string, ppValue);
}

ECode CDecimalFormat::ParseEx(
    /* [in] */ const String& string,
    /* [in] */ IParsePosition * pPosition,
    /* [out] */ INumber ** ppValue)
{
    return DecimalFormat::ParseEx(string, pPosition, ppValue);
}

ECode CDecimalFormat::SetGroupingUsed(
    /* [in] */ Boolean value)
{
    return DecimalFormat::SetGroupingUsed(value);
}

ECode CDecimalFormat::SetMaximumFractionDigits(
    /* [in] */ Int32 value)
{
    return DecimalFormat::SetMaximumFractionDigits(value);
}

ECode CDecimalFormat::SetMaximumIntegerDigits(
    /* [in] */ Int32 value)
{
    return DecimalFormat::SetMaximumIntegerDigits(value);
}

ECode CDecimalFormat::SetMinimumFractionDigits(
    /* [in] */ Int32 value)
{
    return DecimalFormat::SetMinimumFractionDigits(value);
}

ECode CDecimalFormat::SetMinimumIntegerDigits(
    /* [in] */ Int32 value)
{
    return DecimalFormat::SetMinimumIntegerDigits(value);
}

ECode CDecimalFormat::SetParseIntegerOnly(
    /* [in] */ Boolean value)
{
    return DecimalFormat::SetParseIntegerOnly(value);
}

ECode CDecimalFormat::ApplyLocalizedPattern(
    /* [in] */ const String& pattern)
{
    return DecimalFormat::ApplyLocalizedPattern(pattern);
}

ECode CDecimalFormat::ApplyPattern(
    /* [in] */ const String& pattern)
{
    return DecimalFormat::ApplyPattern(pattern);
}

ECode CDecimalFormat::GetDecimalFormatSymbols(
    /* [out] */ IDecimalFormatSymbols ** ppDecimalFormatSymbols)
{
    return DecimalFormat::GetDecimalFormatSymbols(ppDecimalFormatSymbols);
}

ECode CDecimalFormat::GetGroupingSize(
    /* [out] */ Int32 * pGroupingSize)
{
    return DecimalFormat::GetGroupingSize(pGroupingSize);
}

ECode CDecimalFormat::GetMultiplier(
    /* [out] */ Int32 * pMultiplier)
{
    return DecimalFormat::GetMultiplier(pMultiplier);
}

ECode CDecimalFormat::GetNegativePrefix(
    /* [out] */ String * pNegativePrefix)
{
    return DecimalFormat::GetNegativePrefix(pNegativePrefix);
}

ECode CDecimalFormat::GetNegativeSuffix(
    /* [out] */ String * pNegativeSuffix)
{
    return DecimalFormat::GetNegativeSuffix(pNegativeSuffix);
}

ECode CDecimalFormat::GetPositivePrefix(
    /* [out] */ String * pPositivePrefix)
{
    return DecimalFormat::GetPositivePrefix(pPositivePrefix);
}

ECode CDecimalFormat::GetPositiveSuffix(
    /* [out] */ String * pPositiveSuffix)
{
    return DecimalFormat::GetPositiveSuffix(pPositiveSuffix);
}

ECode CDecimalFormat::IsDecimalSeparatorAlwaysShown(
    /* [out] */ Boolean * pIsDecimalSeparatorAlwaysShown)
{
    return DecimalFormat::IsDecimalSeparatorAlwaysShown(pIsDecimalSeparatorAlwaysShown);
}

ECode CDecimalFormat::IsParseBigDecimal(
    /* [out] */ Boolean * pIsParseBigDecimal)
{
    return DecimalFormat::IsParseBigDecimal(pIsParseBigDecimal);
}

ECode CDecimalFormat::SetDecimalFormatSymbols(
    /* [in] */ IDecimalFormatSymbols * pValue)
{
    return DecimalFormat::SetDecimalFormatSymbols(pValue);
}

ECode CDecimalFormat::SetDecimalSeparatorAlwaysShown(
    /* [in] */ Boolean value)
{
    return DecimalFormat::SetDecimalSeparatorAlwaysShown(value);
}

ECode CDecimalFormat::SetGroupingSize(
    /* [in] */ Int32 value)
{
    return DecimalFormat::SetGroupingSize(value);
}

ECode CDecimalFormat::SetMultiplier(
    /* [in] */ Int32 value)
{
    return DecimalFormat::SetMultiplier(value);
}

ECode CDecimalFormat::SetNegativePrefix(
    /* [in] */ const String& value)
{
    return DecimalFormat::SetNegativePrefix(value);
}

ECode CDecimalFormat::SetNegativeSuffix(
    /* [in] */ const String& value)
{
    return DecimalFormat::SetNegativeSuffix(value);
}

ECode CDecimalFormat::SetPositivePrefix(
    /* [in] */ const String& value)
{
    return DecimalFormat::SetPositivePrefix(value);
}

ECode CDecimalFormat::SetPositiveSuffix(
    /* [in] */ const String& value)
{
    return DecimalFormat::SetPositiveSuffix(value);
}

ECode CDecimalFormat::SetParseBigDecimal(
    /* [in] */ Boolean newValue)
{
    return DecimalFormat::SetParseBigDecimal(newValue);
}

ECode CDecimalFormat::ToLocalizedPattern(
    /* [out] */ String * pLocalizedPattern)
{
    return DecimalFormat::ToLocalizedPattern(pLocalizedPattern);
}

ECode CDecimalFormat::ToPattern(
    /* [out] */ String * pPattern)
{
    return DecimalFormat::ToPattern(pPattern);
}

ECode CDecimalFormat::GetRoundingMode(
    /* [out] */ RRoundingMode* pRoundingMode)
{
    return DecimalFormat::GetRoundingMode(pRoundingMode);
}

ECode CDecimalFormat::SetRoundingMode(
    /* [in] */ RRoundingMode roundingMode)
{
    return DecimalFormat::SetRoundingMode(roundingMode);    
}
