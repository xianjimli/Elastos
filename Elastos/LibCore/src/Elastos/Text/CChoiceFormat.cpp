
#include "CChoiceFormat.h"

ECode CChoiceFormat::constructor(
    /* [in] */ const ArrayOf<Double>& limits,
    /* [in] */ const ArrayOf<String>& formats)
{
    return ChoiceFormat::Init(limits, formats);
}

ECode CChoiceFormat::constructor(
    /* [in] */ const String& tem)
{
    return ChoiceFormat::Init(tem);
}

ECode CChoiceFormat::FormatObject(
    /* [in] */ IInterface* object,
    /* [out] */ String* value)
{
    return ChoiceFormat::FormatObject(object, value);
}

ECode CChoiceFormat::FormatObjectEx(
    /* [in] */ IInterface* object,
    /* [in] */ const String& buffer,
    /* [in] */ IFieldPosition* field,
    /* [out] */ String* value)
{
    return ChoiceFormat::FormatObjectEx(object, buffer, field, value);
}

ECode CChoiceFormat::FormatToCharacterIterator(
    /* [in] */ IInterface* object,
    /* [out] */ IAttributedCharacterIterator** charactorIterator)
{
    return ChoiceFormat::FormatToCharacterIterator(object, charactorIterator);
}

ECode CChoiceFormat::ParseObject(
    /* [in] */ const String& string,
    /* [out] */ IInterface** object)
{
    return ChoiceFormat::ParseObject(string, object);
}

ECode CChoiceFormat::ParseObjectEx(
    /* [in] */ const String& string,
    /* [in] */ IParsePosition* position,
    /* [out] */ IInterface** object)
{
    return ChoiceFormat::ParseObjectEx(string, position, object);
}

ECode CChoiceFormat::FormatDouble(
    /* [in] */ Double value,
    /* [out] */ String* format)
{
    return ChoiceFormat::FormatDouble(value, format);
}

ECode CChoiceFormat::FormatDoubleEx(
    /* [in] */ Double value,
    /* [in] */ const String& buffer,
    /* [in] */ IFieldPosition* field,
    /* [out] */ String* result)
{
    return ChoiceFormat::FormatDoubleEx(value, buffer, field, result);
}

ECode CChoiceFormat::FormatInt64(
    /* [in] */ Int64 value,
    /* [out] */ String* format)
{
    return ChoiceFormat::FormatInt64(value, format);
}

ECode CChoiceFormat::FormatInt64Ex(
    /* [in] */ Int64 value,
    /* [in] */ const String& buffer,
    /* [in] */ IFieldPosition* field,
    /* [out] */ String* result)
{
    return ChoiceFormat::FormatInt64Ex(value, buffer, field, result);
}

ECode CChoiceFormat::GetMaximumFractionDigits(
    /* [out] */ Int32* maximumFractionDigits)
{
    return ChoiceFormat::GetMaximumIntegerDigits(maximumFractionDigits);
}

ECode CChoiceFormat::GetMaximumIntegerDigits(
    /* [out] */ Int32* maximumIntegerDigits)
{
    return ChoiceFormat::GetMaximumIntegerDigits(maximumIntegerDigits);
}

ECode CChoiceFormat::GetMinimumFractionDigits(
    /* [out] */ Int32* minimumFractionDigits)
{
    return ChoiceFormat::GetMinimumFractionDigits(minimumFractionDigits);
}

ECode CChoiceFormat::GetMinimumIntegerDigits(
    /* [out] */ Int32* minimumIntegerDigits)
{
    return ChoiceFormat::GetMinimumIntegerDigits(minimumIntegerDigits);
}

ECode CChoiceFormat::IsGroupingUsed(
    /* [out] */ Boolean* isGroupingUsed)
{
    return ChoiceFormat::IsGroupingUsed(isGroupingUsed);
}

ECode CChoiceFormat::IsParseIntegerOnly(
    /* [out] */ Boolean* isParseIntegerOnly)
{
    return ChoiceFormat::IsParseIntegerOnly(isParseIntegerOnly);
}

ECode CChoiceFormat::Parse(
    /* [in] */ const String& string,
    /* [out] */ INumber** value)
{
    return ChoiceFormat::Parse(string, value);
}

ECode CChoiceFormat::ParseEx(
    /* [in] */ const String& string,
    /* [in] */ IParsePosition* position,
    /* [out] */ INumber** value)
{
    return ChoiceFormat::ParseEx(string, position, value);
}

ECode CChoiceFormat::SetGroupingUsed(
    /* [in] */ Boolean value)
{
    return ChoiceFormat::SetGroupingUsed(value);
}

ECode CChoiceFormat::SetMaximumFractionDigits(
    /* [in] */ Int32 value)
{
    return ChoiceFormat::SetMaximumFractionDigits(value);
}

ECode CChoiceFormat::SetMaximumIntegerDigits(
    /* [in] */ Int32 value)
{
    return ChoiceFormat::SetMaximumIntegerDigits(value);
}

ECode CChoiceFormat::SetMinimumFractionDigits(
    /* [in] */ Int32 value)
{
    return ChoiceFormat::SetMinimumFractionDigits(value);
}

ECode CChoiceFormat::SetMinimumIntegerDigits(
    /* [in] */ Int32 value)
{
    return ChoiceFormat::SetMinimumIntegerDigits(value);
}

ECode CChoiceFormat::SetParseIntegerOnly(
    /* [in] */ Boolean value)
{
    return ChoiceFormat::SetParseIntegerOnly(value);
}

ECode CChoiceFormat::ApplyPattern(
    /* [in] */ const String& tem)
{
    return ChoiceFormat::ApplyPattern(tem);
}

ECode CChoiceFormat::GetFormats(
    /* [out, callee] */ ArrayOf<IInterface*>** formats)
{
    return ChoiceFormat::GetFormats(formats);
}

ECode CChoiceFormat::GetLimits(
    /* [out, callee] */ ArrayOf<Double>** limits)
{
    return ChoiceFormat::GetLimits(limits);
}

ECode CChoiceFormat::SetChoices(
    /* [in] */ const ArrayOf<Double>& limits,
    /* [in] */ const ArrayOf<String>& formats)
{
    return ChoiceFormat::SetChoices(limits, formats);
}

ECode CChoiceFormat::ToPattern(
    /* [out] */ String* pattern)
{
    return ChoiceFormat::ToPattern(pattern);
}

ECode CChoiceFormat::GetCurrency(
    /* [out] */ ICurrency** currency)
{
    return ChoiceFormat::GetCurrency(currency);
}

ECode CChoiceFormat::SetCurrency(
    /* [in] */ ICurrency* currency)
{
    return ChoiceFormat::SetCurrency(currency);
}

ECode CChoiceFormat::GetRoundingMode(
    /* [out] */ RoundingMode* mode)
{
    return CChoiceFormat::GetRoundingMode(mode);
}

ECode CChoiceFormat::SetRoundingMode(
    /* [in] */ RoundingMode mode)
{
    return CChoiceFormat::SetRoundingMode(mode);
}
