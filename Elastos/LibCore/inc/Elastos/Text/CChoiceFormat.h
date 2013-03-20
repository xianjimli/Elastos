
#ifndef __CCHOICEFORMAT_H__
#define __CCHOICEFORMAT_H__

#include "_CChoiceFormat.h"
#include "ChoiceFormat.h"

CarClass(CChoiceFormat), public ChoiceFormat
{
public:
    CARAPI constructor(
        /* [in] */ ArrayOf<Double> * pLimits,
        /* [in] */ ArrayOf<String> * pFormats);

    CARAPI constructor(
        /* [in] */ const String& tem);

    CARAPI format(
        /* [in] */ IInterface * pObject,
        /* [out] */ String * pValue);

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

    CARAPI formatEx2(
        /* [in] */ Double value,
        /* [out] */ String * pFormat);

    CARAPI formatEx4(
        /* [in] */ Int64 value,
        /* [out] */ String * pFormat);

    CARAPI GetMaximumFractionDigits(
        /* [out] */ Int32 * pMaximumFractionDigits);

    CARAPI GetMaximumIntegerDigits(
        /* [out] */ Int32 * pMaximumIntegerDigits);

    CARAPI GetMinimumFractionDigits(
        /* [out] */ Int32 * pMinimumFractionDigits);

    CARAPI GetMinimumIntegerDigits(
        /* [out] */ Int32 * pMinimumIntegerDigits);

    CARAPI IsGroupingUsed(
        /* [out] */ Boolean * pIsGroupingUsed);

    CARAPI IsParseIntegerOnly(
        /* [out] */ Boolean * pIsParseIntegerOnly);

    CARAPI Parse(
        /* [in] */ const String& string,
        /* [out] */ INumber ** ppValue);

    CARAPI ParseEx(
        /* [in] */ const String& string,
        /* [in] */ IParsePosition * pPosition,
        /* [out] */ INumber ** ppValue);

    CARAPI SetGroupingUsed(
        /* [in] */ Boolean value);

    CARAPI SetMaximumFractionDigits(
        /* [in] */ Int32 value);

    CARAPI SetMaximumIntegerDigits(
        /* [in] */ Int32 value);

    CARAPI SetMinimumFractionDigits(
        /* [in] */ Int32 value);

    CARAPI SetMinimumIntegerDigits(
        /* [in] */ Int32 value);

    CARAPI SetParseIntegerOnly(
        /* [in] */ Boolean value);

    CARAPI ApplyPattern(
        /* [in] */ const String& tem);

    CARAPI GetFormats(
        /* [out, callee] */ ArrayOf<IInterface *> ** ppFormats);

    CARAPI GetLimits(
        /* [out, callee] */ ArrayOf<Double> ** ppLimits);

    CARAPI SetChoices(
        /* [in] */ ArrayOf<Double> * pLimits,
        /* [in] */ ArrayOf<String> * pFormats);

    CARAPI ToPattern(
        /* [out] */ String * pPattern);

private:
    // TODO: Add your private member variables here.
};

#endif // __CCHOICEFORMAT_H__
