
#include "CChoiceFormat.h"
ECode CChoiceFormat::constructor(
    /* [in] */ ArrayOf<Double> * pLimits,
    /* [in] */ ArrayOf<String> * pFormats)
{
    return ChoiceFormat::Init(pLimits, pFormats);
}

ECode CChoiceFormat::constructor(
    /* [in] */ const String& tem)
{
    return ChoiceFormat::Init(tem);
}

ECode CChoiceFormat::format(
    /* [in] */ IInterface * pObject,
    /* [out] */ String * pValue)
{
    return ChoiceFormat::format(pObject, pValue);
}

ECode CChoiceFormat::FormatToCharacterIterator(
    /* [in] */ IInterface * pObject,
    /* [out] */ IAttributedCharacterIterator ** ppCharactorIterator)
{
    return ChoiceFormat::FormatToCharacterIterator(pObject, ppCharactorIterator);
}

ECode CChoiceFormat::ParseObject(
    /* [in] */ const String& string,
    /* [out] */ IInterface ** ppObject)
{
    return ChoiceFormat::ParseObject(string, ppObject);
}

ECode CChoiceFormat::ParseObjectEx(
    /* [in] */ const String& string,
    /* [in] */ IParsePosition * pPosition,
    /* [out] */ IInterface ** ppObject)
{
    return ChoiceFormat::ParseObjectEx(string, pPosition, ppObject);
}

ECode CChoiceFormat::formatEx2(
    /* [in] */ Double value,
    /* [out] */ String * pFormat)
{
    return ChoiceFormat::formatEx2(value, pFormat);
}

ECode CChoiceFormat::formatEx4(
    /* [in] */ Int64 value,
    /* [out] */ String * pFormat)
{
    return ChoiceFormat::formatEx4(value, pFormat);
}

ECode CChoiceFormat::GetMaximumFractionDigits(
    /* [out] */ Int32 * pMaximumFractionDigits)
{
    return ChoiceFormat::GetMaximumIntegerDigits(pMaximumFractionDigits);
}

ECode CChoiceFormat::GetMaximumIntegerDigits(
    /* [out] */ Int32 * pMaximumIntegerDigits)
{
    return ChoiceFormat::GetMaximumIntegerDigits(pMaximumIntegerDigits);
}

ECode CChoiceFormat::GetMinimumFractionDigits(
    /* [out] */ Int32 * pMinimumFractionDigits)
{
    return ChoiceFormat::GetMinimumFractionDigits(pMinimumFractionDigits);
}

ECode CChoiceFormat::GetMinimumIntegerDigits(
    /* [out] */ Int32 * pMinimumIntegerDigits)
{
    return ChoiceFormat::GetMinimumIntegerDigits(pMinimumIntegerDigits);
}

ECode CChoiceFormat::IsGroupingUsed(
    /* [out] */ Boolean * pIsGroupingUsed)
{
    return ChoiceFormat::IsGroupingUsed(pIsGroupingUsed);
}

ECode CChoiceFormat::IsParseIntegerOnly(
    /* [out] */ Boolean * pIsParseIntegerOnly)
{
    return ChoiceFormat::IsParseIntegerOnly(pIsParseIntegerOnly);
}

ECode CChoiceFormat::Parse(
    /* [in] */ const String& string,
    /* [out] */ INumber ** ppValue)
{
    return ChoiceFormat::Parse(string, ppValue);
}

ECode CChoiceFormat::ParseEx(
    /* [in] */ const String& string,
    /* [in] */ IParsePosition * pPosition,
    /* [out] */ INumber ** ppValue)
{
    return ChoiceFormat::ParseEx(string, pPosition, ppValue);
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
    /* [out, callee] */ ArrayOf<IInterface *> ** ppFormats)
{
    return ChoiceFormat::GetFormats(ppFormats);
}

ECode CChoiceFormat::GetLimits(
    /* [out, callee] */ ArrayOf<Double> ** ppLimits)
{
    return ChoiceFormat::GetLimits(ppLimits);
}

ECode CChoiceFormat::SetChoices(
    /* [in] */ ArrayOf<Double> * pLimits,
    /* [in] */ ArrayOf<String> * pFormats)
{
    return ChoiceFormat::SetChoices(pLimits, pFormats);
}

ECode CChoiceFormat::ToPattern(
    /* [out] */ String * pPattern)
{
    return ChoiceFormat::ToPattern(pPattern);
}


