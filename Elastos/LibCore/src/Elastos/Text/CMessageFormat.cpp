
#include "CMessageFormat.h"

ECode CMessageFormat::constructor(
    /* [in] */ const String& tem,
    /* [in] */ ILocale * pLocale)
{
    MessageFormat::Init(tem, pLocale);
    return NOERROR;
}

ECode CMessageFormat::constructor(
    /* [in] */ const String& tem)
{
    MessageFormat::Init(tem);
    return NOERROR;
}

ECode CMessageFormat::format(
    /* [in] */ IInterface * pObject,
    /* [out] */ String * pValue)
{
    return MessageFormat::format(pObject, pValue);
}

ECode CMessageFormat::FormatToCharacterIterator(
    /* [in] */ IInterface * pObject,
    /* [out] */ IAttributedCharacterIterator ** ppCharactorIterator)
{
    return MessageFormat::FormatToCharacterIterator(pObject, ppCharactorIterator);
}

ECode CMessageFormat::ParseObject(
    /* [in] */ const String& string,
    /* [out] */ IInterface ** ppObject)
{
    return MessageFormat::ParseObject(string, ppObject);
}

ECode CMessageFormat::ParseObjectEx(
    /* [in] */ const String& string,
    /* [in] */ IParsePosition * pPosition,
    /* [out] */ IInterface ** ppObject)
{
    return MessageFormat::ParseObjectEx(string, pPosition, ppObject);
}

ECode CMessageFormat::ApplyPattern(
    /* [in] */ const String& tem)
{
    return MessageFormat::ApplyPattern(tem);
}

ECode CMessageFormat::GetFormats(
    /* [out, callee] */ ArrayOf<IFormat *> ** ppFormats)
{
    return MessageFormat::GetFormats(ppFormats);
}

ECode CMessageFormat::GetFormatsByArgumentIndex(
    /* [out, callee] */ ArrayOf<IFormat *> ** ppFormatsByArgumentIndex)
{
    return MessageFormat::GetFormatsByArgumentIndex(ppFormatsByArgumentIndex);
}

ECode CMessageFormat::SetFormatByArgumentIndex(
    /* [in] */ Int32 argIndex,
    /* [in] */ IFormat * pFormat)
{
    return MessageFormat::SetFormatByArgumentIndex(argIndex, pFormat);
}

ECode CMessageFormat::SetFormatsByArgumentIndexEx(
    /* [in] */ ArrayOf<IFormat *> * pFormats)
{
    return MessageFormat::SetFormatsByArgumentIndexEx(pFormats);
}

ECode CMessageFormat::GetLocale(
    /* [out] */ ILocale ** ppLocale)
{
    return MessageFormat::GetLocale(ppLocale);
}

ECode CMessageFormat::Parse(
    /* [in] */ const String& string,
    /* [out, callee] */ ArrayOf<IInterface *> ** ppResult)
{
    return MessageFormat::Parse(string, ppResult);
}

ECode CMessageFormat::ParseEx(
    /* [in] */ const String& string,
    /* [in] */ IParsePosition * pPosition,
    /* [out, callee] */ ArrayOf<IInterface *> ** ppResult)
{
    return MessageFormat::ParseEx(string, pPosition, ppResult);
}

ECode CMessageFormat::SetFormat(
    /* [in] */ Int32 offset,
    /* [in] */ IFormat * pFormat)
{
    return MessageFormat::SetFormat(offset, pFormat);
}

ECode CMessageFormat::SetFormats(
    /* [in] */ ArrayOf<IFormat *> * pFormats)
{
    return MessageFormat::SetFormats(pFormats);
}

ECode CMessageFormat::SetLocale(
    /* [in] */ ILocale * pLocale)
{
    return MessageFormat::SetLocale(pLocale);
}

ECode CMessageFormat::ToPattern(
    /* [out] */ String * pPattern)
{
    return MessageFormat::ToPattern(pPattern);
}


