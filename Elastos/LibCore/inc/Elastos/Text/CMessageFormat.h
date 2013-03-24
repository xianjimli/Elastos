
#ifndef __CMESSAGEFORMAT_H__
#define __CMESSAGEFORMAT_H__

#include "_CMessageFormat.h"
#include "MessageFormat.h"

CarClass(CMessageFormat), public MessageFormat
{
public:
    CARAPI constructor(
        /* [in] */ const String& tem,
        /* [in] */ ILocale * pLocale);

    CARAPI constructor(
        /* [in] */ const String& tem);

    CARAPI FormatObject(
        /* [in] */ IInterface * pObject,
        /* [out] */ String * pValue);

    CARAPI FormatObjectEx(
        /* [in] */ IInterface* object,
        /* [in] */ const String& buffer,
        /* [in] */ IFieldPosition* field,
        /* [out] */ String* value);

    CARAPI FormatToCharacterIterator(
        /* [in] */ IInterface * pObject,
        /* [out] */ IAttributedCharacterIterator ** ppCharactorIterator);

    CARAPI ParseObject(
        /* [in] */ const String& string,
        /* [out] */ IInterface ** ppObject);

    CARAPI ParseObjectEx(
        /* [in] */ const String& string,
        /* [in] */ IParsePosition * pPosition,
        /* [out] */ IInterface ** ppObject);

    CARAPI ApplyPattern(
        /* [in] */ const String& tem);

    CARAPI GetFormats(
        /* [out, callee] */ ArrayOf<IFormat *> ** ppFormats);

    CARAPI GetFormatsByArgumentIndex(
        /* [out, callee] */ ArrayOf<IFormat *> ** ppFormatsByArgumentIndex);

    CARAPI SetFormatByArgumentIndex(
        /* [in] */ Int32 argIndex,
        /* [in] */ IFormat * pFormat);

    CARAPI SetFormatsByArgumentIndexEx(
        /* [in] */ ArrayOf<IFormat *> * pFormats);

    CARAPI GetLocale(
        /* [out] */ ILocale ** ppLocale);

    CARAPI Parse(
        /* [in] */ const String& string,
        /* [out, callee] */ ArrayOf<IInterface *> ** ppResult);

    CARAPI ParseEx(
        /* [in] */ const String& string,
        /* [in] */ IParsePosition * pPosition,
        /* [out, callee] */ ArrayOf<IInterface *> ** ppResult);

    CARAPI SetFormat(
        /* [in] */ Int32 offset,
        /* [in] */ IFormat * pFormat);

    CARAPI SetFormats(
        /* [in] */ ArrayOf<IFormat *> * pFormats);

    CARAPI SetLocale(
        /* [in] */ ILocale * pLocale);

    CARAPI ToPattern(
        /* [out] */ String * pPattern);

private:
    // TODO: Add your private member variables here.
};

#endif // __CMESSAGEFORMAT_H__
