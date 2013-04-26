
#include "cmdef.h"
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
    /* [in] */ IDecimalFormatSymbols* value)
{
    return DecimalFormat::Init(pattern, value);
}

ECode CDecimalFormat::constructor(
    /* [in] */ const String& pattern,
    /* [in] */ ILocale* locale)
{
    return DecimalFormat::Init(pattern, locale);
}

PInterface CDecimalFormat::Probe(
    /* [in] */ REIID riid)
{
    return _CDecimalFormat::Probe(riid);
}

ECode CDecimalFormat::FormatObject(
    /* [in] */ IInterface* object,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value);
    return DecimalFormat::FormatObject(object, value);
}

ECode CDecimalFormat::FormatObjectEx(
    /* [in] */ IInterface* object,
    /* [in] */ const String& buffer,
    /* [in] */ IFieldPosition* field,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value);
    return DecimalFormat::FormatObjectEx(object, buffer, field, value);
}

ECode CDecimalFormat::FormatToCharacterIterator(
    /* [in] */ IInterface* object,
    /* [out] */ IAttributedCharacterIterator** iterator)
{
    VALIDATE_NOT_NULL(iterator);
    return DecimalFormat::FormatToCharacterIterator(object, iterator);
}

ECode CDecimalFormat::ParseObject(
    /* [in] */ const String& string,
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object);
    return DecimalFormat::ParseObject(string, object);
}

ECode CDecimalFormat::ParseObjectEx(
    /* [in] */ const String& string,
    /* [in] */ IParsePosition* position,
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object);
    return DecimalFormat::ParseObjectEx(string, position, object);
}

ECode CDecimalFormat::FormatDouble(
    /* [in] */ Double value,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    return DecimalFormat::FormatDouble(value, result);
}

ECode CDecimalFormat::FormatDoubleEx(
    /* [in] */ Double value,
    /* [in] */ const String& buffer,
    /* [in] */ IFieldPosition* field,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    return DecimalFormat::FormatDoubleEx(value, buffer, field, result);
}

ECode CDecimalFormat::FormatInt64(
    /* [in] */ Int64 value,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    return DecimalFormat::FormatInt64(value, result);
}

ECode CDecimalFormat::FormatInt64Ex(
    /* [in] */ Int64 value,
    /* [in] */ const String& buffer,
    /* [in] */ IFieldPosition* field,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    return DecimalFormat::FormatInt64Ex(value, buffer, field, result);
}

ECode CDecimalFormat::GetMaximumFractionDigits(
    /* [out] */ Int32* maxFractionDigits)
{
    VALIDATE_NOT_NULL(maxFractionDigits);
    return DecimalFormat::GetMaximumFractionDigits(maxFractionDigits);
}

ECode CDecimalFormat::GetMaximumIntegerDigits(
    /* [out] */ Int32* maxIntegerDigits)
{
    VALIDATE_NOT_NULL(maxIntegerDigits);
    return DecimalFormat::GetMaximumIntegerDigits(maxIntegerDigits);
}

ECode CDecimalFormat::GetMinimumFractionDigits(
    /* [out] */ Int32* minFractionDigits)
{
    VALIDATE_NOT_NULL(minFractionDigits);
    return DecimalFormat::GetMinimumFractionDigits(minFractionDigits);
}

ECode CDecimalFormat::GetMinimumIntegerDigits(
    /* [out] */ Int32* minIntegerDigits)
{
    VALIDATE_NOT_NULL(minIntegerDigits);
    return DecimalFormat::GetMinimumIntegerDigits(minIntegerDigits);
}

ECode CDecimalFormat::IsGroupingUsed(
    /* [out] */ Boolean* isGroupingUsed)
{
    VALIDATE_NOT_NULL(isGroupingUsed);
    return DecimalFormat::IsGroupingUsed(isGroupingUsed);
}

ECode CDecimalFormat::IsParseIntegerOnly(
    /* [out] */ Boolean* isParseIntegerOnly)
{
    VALIDATE_NOT_NULL(isParseIntegerOnly);
    return DecimalFormat::IsParseIntegerOnly(isParseIntegerOnly);
}

ECode CDecimalFormat::Parse(
    /* [in] */ const String& string,
    /* [out] */ INumber** value)
{
    VALIDATE_NOT_NULL(value);
    return DecimalFormat::Parse(string, value);
}

ECode CDecimalFormat::ParseEx(
    /* [in] */ const String& string,
    /* [in] */ IParsePosition* position,
    /* [out] */ INumber** value)
{
    VALIDATE_NOT_NULL(value);
    return DecimalFormat::ParseEx(string, position, value);
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
    /* [out] */ IDecimalFormatSymbols** symbols)
{
    VALIDATE_NOT_NULL(symbols);
    return DecimalFormat::GetDecimalFormatSymbols(symbols);
}

ECode CDecimalFormat::GetGroupingSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    return DecimalFormat::GetGroupingSize(size);
}

ECode CDecimalFormat::GetMultiplier(
    /* [out] */ Int32* multiplier)
{
    VALIDATE_NOT_NULL(multiplier);
    return DecimalFormat::GetMultiplier(multiplier);
}

ECode CDecimalFormat::GetNegativePrefix(
    /* [out] */ String* prefix)
{
    VALIDATE_NOT_NULL(prefix);
    return DecimalFormat::GetNegativePrefix(prefix);
}

ECode CDecimalFormat::GetNegativeSuffix(
    /* [out] */ String* suffix)
{
    VALIDATE_NOT_NULL(suffix);
    return DecimalFormat::GetNegativeSuffix(suffix);
}

ECode CDecimalFormat::GetPositivePrefix(
    /* [out] */ String* prefix)
{
    VALIDATE_NOT_NULL(prefix);
    return DecimalFormat::GetPositivePrefix(prefix);
}

ECode CDecimalFormat::GetPositiveSuffix(
    /* [out] */ String* suffix)
{
    VALIDATE_NOT_NULL(suffix);
    return DecimalFormat::GetPositiveSuffix(suffix);
}

ECode CDecimalFormat::IsDecimalSeparatorAlwaysShown(
    /* [out] */ Boolean * isAlwaysShown)
{
    VALIDATE_NOT_NULL(isAlwaysShown);
    return DecimalFormat::IsDecimalSeparatorAlwaysShown(isAlwaysShown);
}

ECode CDecimalFormat::IsParseBigDecimal(
    /* [out] */ Boolean* isParseBigDecimal)
{
    VALIDATE_NOT_NULL(isParseBigDecimal);
    return DecimalFormat::IsParseBigDecimal(isParseBigDecimal);
}

ECode CDecimalFormat::SetDecimalFormatSymbols(
    /* [in] */ IDecimalFormatSymbols* value)
{
    return DecimalFormat::SetDecimalFormatSymbols(value);
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
    /* [out] */ String* pattern)
{
    VALIDATE_NOT_NULL(pattern);
    return DecimalFormat::ToLocalizedPattern(pattern);
}

ECode CDecimalFormat::ToPattern(
    /* [out] */ String* pattern)
{
    VALIDATE_NOT_NULL(pattern);
    return DecimalFormat::ToPattern(pattern);
}

ECode CDecimalFormat::GetCurrency(
    /* [out] */ ICurrency** currency)
{
    VALIDATE_NOT_NULL(currency);
    return DecimalFormat::GetCurrency(currency);
}

ECode CDecimalFormat::SetCurrency(
    /* [in] */ ICurrency* currency)
{
    return DecimalFormat::SetCurrency(currency);
}

ECode CDecimalFormat::GetRoundingMode(
    /* [out] */ RoundingMode* mode)
{
    VALIDATE_NOT_NULL(mode);
    return DecimalFormat::GetRoundingMode(mode);
}

ECode CDecimalFormat::SetRoundingMode(
    /* [in] */ RoundingMode mode)
{
    return DecimalFormat::SetRoundingMode(mode);
}
