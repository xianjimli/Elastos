
#include "cmdef.h"
#include "CNativeDecimalFormat.h"

ECode CNativeDecimalFormat::constructor(
    /* [in] */ const String& pattern,
    /* [in] */ IDecimalFormatSymbols* dfs)
{
    return NativeDecimalFormat::Init(pattern, dfs);
}

ECode CNativeDecimalFormat::constructor(
    /* [in] */ const String& pattern,
    /* [in] */ ILocaleData * data)
{
    return NativeDecimalFormat::Init(pattern, data);
}

ECode CNativeDecimalFormat::Close()
{
    return NativeDecimalFormat::Close();
}

ECode CNativeDecimalFormat::SetDecimalFormatSymbols(
    /* [in] */ IDecimalFormatSymbols* dfs)
{
    return NativeDecimalFormat::SetDecimalFormatSymbols(dfs);
}

ECode CNativeDecimalFormat::SetDecimalFormatSymbolsEx(
    /* [in] */ ILocaleData* ld)
{
    return NativeDecimalFormat::SetDecimalFormatSymbols(ld);
}

ECode CNativeDecimalFormat::FormatBigDecimal(
    /* [in] */ IBigDecimal* value,
    /* [in] */ IFieldPosition* field,
    /* [out, callee] */ ArrayOf<Char32>** result)
{
    VALIDATE_NOT_NULL(result);
    return NativeDecimalFormat::FormatBigDecimal(value, field, result);
}

ECode CNativeDecimalFormat::FormatBigInteger(
    /* [in] */ IBigInteger* value,
    /* [in] */ IFieldPosition* field,
    /* [out, callee] */ ArrayOf<Char32>** result)
{
    VALIDATE_NOT_NULL(result);
    return NativeDecimalFormat::FormatBigInteger(value, field, result);
}

ECode CNativeDecimalFormat::FormatInt64(
    /* [in] */ Int64 value,
    /* [in] */ IFieldPosition* field,
    /* [out, callee] */ ArrayOf<Char32>** array)
{
    VALIDATE_NOT_NULL(array);
    return NativeDecimalFormat::FormatInt64(value, field, array);
}

ECode CNativeDecimalFormat::FormatDouble(
    /* [in] */ Double value,
    /* [in] */ IFieldPosition* field,
    /* [out, callee] */ ArrayOf<Char32>** array)
{
    VALIDATE_NOT_NULL(array);
    return NativeDecimalFormat::FormatDouble(value, field, array);
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
    /* [in] */ IInterface* object,
    /* [out] */ IAttributedCharacterIterator** result)
{
    VALIDATE_NOT_NULL(result);
    return NativeDecimalFormat::FormatToCharacterIterator(object, result);
}

ECode CNativeDecimalFormat::ToLocalizedPattern(
    /* [out] */ String* pattern)
{
    VALIDATE_NOT_NULL(pattern);
    return NativeDecimalFormat::ToLocalizedPattern(pattern);
}

ECode CNativeDecimalFormat::ToPattern(
    /* [out] */ String* pattern)
{
    VALIDATE_NOT_NULL(pattern);
    return NativeDecimalFormat::ToPattern(pattern);
}

ECode CNativeDecimalFormat::Parse(
    /* [in] */ const String& string,
    /* [in] */ IParsePosition* position,
    /* [out] */ INumber** result)
{
    VALIDATE_NOT_NULL(result);
    return NativeDecimalFormat::Parse(string, position, result);
}

ECode CNativeDecimalFormat::GetMaximumFractionDigits(
    /* [out] */ Int32* maxFractionDigits)
{
    VALIDATE_NOT_NULL(maxFractionDigits);
    return NativeDecimalFormat::GetMaximumFractionDigits(maxFractionDigits);
}

ECode CNativeDecimalFormat::GetMaximumIntegerDigits(
    /* [out] */ Int32* maxIntegerDigits)
{
    VALIDATE_NOT_NULL(maxIntegerDigits);
    return NativeDecimalFormat::GetMaximumIntegerDigits(maxIntegerDigits);
}

ECode CNativeDecimalFormat::GetMinimumFractionDigits(
    /* [out] */ Int32* minFractionDigits)
{
    VALIDATE_NOT_NULL(minFractionDigits);
    return NativeDecimalFormat::GetMinimumFractionDigits(minFractionDigits);
}

ECode CNativeDecimalFormat::GetMinimumIntegerDigits(
    /* [out] */ Int32* minIntegerDigits)
{
    VALIDATE_NOT_NULL(minIntegerDigits);
    return NativeDecimalFormat::GetMinimumIntegerDigits(minIntegerDigits);
}

ECode CNativeDecimalFormat::GetGroupingSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    return NativeDecimalFormat::GetGroupingSize(size);
}

ECode CNativeDecimalFormat::GetMultiplier(
    /* [out] */ Int32* multiplier)
{
    VALIDATE_NOT_NULL(multiplier);
    return NativeDecimalFormat::GetMultiplier(multiplier);
}

ECode CNativeDecimalFormat::GetNegativePrefix(
    /* [out] */ String* prefix)
{
    VALIDATE_NOT_NULL(prefix);
    return NativeDecimalFormat::GetNegativePrefix(prefix);
}

ECode CNativeDecimalFormat::GetNegativeSuffix(
    /* [out] */ String* suffix)
{
    VALIDATE_NOT_NULL(suffix);
    return NativeDecimalFormat::GetNegativeSuffix(suffix);
}

ECode CNativeDecimalFormat::GetPositivePrefix(
    /* [out] */ String* prefix)
{
    VALIDATE_NOT_NULL(prefix);
    return NativeDecimalFormat::GetPositivePrefix(prefix);
}

ECode CNativeDecimalFormat::GetPositiveSuffix(
    /* [out] */ String* suffix)
{
    VALIDATE_NOT_NULL(suffix);
    return NativeDecimalFormat::GetPositiveSuffix(suffix);
}

ECode CNativeDecimalFormat::IsDecimalSeparatorAlwaysShown(
    /* [out] */ Boolean* isAlwaysShown)
{
    VALIDATE_NOT_NULL(isAlwaysShown);
    return NativeDecimalFormat::IsDecimalSeparatorAlwaysShown(isAlwaysShown);
}

ECode CNativeDecimalFormat::IsParseBigDecimal(
    /* [out] */ Boolean* isParseBigDecimal)
{
    VALIDATE_NOT_NULL(isParseBigDecimal);
    return NativeDecimalFormat::IsParseBigDecimal(isParseBigDecimal);
}

ECode CNativeDecimalFormat::IsParseIntegerOnly(
    /* [out] */ Boolean* isParseIntegerOnly)
{
    VALIDATE_NOT_NULL(isParseIntegerOnly);
    return NativeDecimalFormat::IsParseIntegerOnly(isParseIntegerOnly);
}

ECode CNativeDecimalFormat::IsGroupingUsed(
    /* [out] */ Boolean* isGroupingUsed)
{
    VALIDATE_NOT_NULL(isGroupingUsed);
    return NativeDecimalFormat::IsGroupingUsed(isGroupingUsed);
}

ECode CNativeDecimalFormat::SetDecimalSeparatorAlwaysShown(
    /* [in] */ Boolean value)
{
    return NativeDecimalFormat::SetDecimalSeparatorAlwaysShown(value);
}

ECode CNativeDecimalFormat::SetCurrency(
    /* [in] */ ICurrency* currency)
{
    return NativeDecimalFormat::SetCurrency(currency);
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
    /* [in] */ RoundingMode roundingMode,
    /* [in] */ Double roundingIncrement)
{
    return NativeDecimalFormat::SetRoundingMode(roundingMode, roundingIncrement);
}

Mutex* CNativeDecimalFormat::GetSelfLock()
{
    return &_m_syncLock;
}
