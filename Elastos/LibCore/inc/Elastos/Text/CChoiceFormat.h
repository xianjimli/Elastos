
#ifndef __CCHOICEFORMAT_H__
#define __CCHOICEFORMAT_H__

#include "_CChoiceFormat.h"
#include "ChoiceFormat.h"

CarClass(CChoiceFormat), public ChoiceFormat
{
public:
    CARAPI constructor(
        /* [in] */ const ArrayOf<Double>& limits,
        /* [in] */ const ArrayOf<String>& formats);

    CARAPI constructor(
        /* [in] */ const String& tem);

    CARAPI FormatObject(
        /* [in] */ IInterface* object,
        /* [out] */ String* value);

    CARAPI FormatObjectEx(
        /* [in] */ IInterface* object,
        /* [in] */ const String& buffer,
        /* [in] */ IFieldPosition* field,
        /* [out] */ String* value);

    CARAPI FormatToCharacterIterator(
        /* [in] */ IInterface* object,
        /* [out] */ IAttributedCharacterIterator** charactorIterator);

    CARAPI ParseObject(
        /* [in] */ const String& string,
        /* [out] */ IInterface** object);

    CARAPI ParseObjectEx(
        /* [in] */ const String& string,
        /* [in] */ IParsePosition* position,
        /* [out] */ IInterface** object);

    CARAPI FormatDouble(
        /* [in] */ Double value,
        /* [out] */ String* format);

    CARAPI FormatDoubleEx(
        /* [in] */ Double value,
        /* [in] */ const String& buffer,
        /* [in] */ IFieldPosition* field,
        /* [out] */ String* result);

    CARAPI FormatInt64(
        /* [in] */ Int64 value,
        /* [out] */ String* format);

    CARAPI FormatInt64Ex(
        /* [in] */ Int64 value,
        /* [in] */ const String& buffer,
        /* [in] */ IFieldPosition* field,
        /* [out] */ String* result);

    CARAPI GetMaximumFractionDigits(
        /* [out] */ Int32* maximumFractionDigits);

    CARAPI GetMaximumIntegerDigits(
        /* [out] */ Int32* maximumIntegerDigits);

    CARAPI GetMinimumFractionDigits(
        /* [out] */ Int32* minimumFractionDigits);

    CARAPI GetMinimumIntegerDigits(
        /* [out] */ Int32* minimumIntegerDigits);

    CARAPI IsGroupingUsed(
        /* [out] */ Boolean* isGroupingUsed);

    CARAPI IsParseIntegerOnly(
        /* [out] */ Boolean* isParseIntegerOnly);

    CARAPI Parse(
        /* [in] */ const String& string,
        /* [out] */ INumber** value);

    CARAPI ParseEx(
        /* [in] */ const String& string,
        /* [in] */ IParsePosition* position,
        /* [out] */ INumber** value);

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
        /* [out, callee] */ ArrayOf<IInterface*>** formats);

    CARAPI GetLimits(
        /* [out, callee] */ ArrayOf<Double>** limits);

    CARAPI SetChoices(
        /* [in] */ const ArrayOf<Double>& limits,
        /* [in] */ const ArrayOf<String>& formats);

    CARAPI ToPattern(
        /* [out] */ String* pattern);

    CARAPI GetCurrency(
        /* [out] */ ICurrency** currency);

    CARAPI SetCurrency(
        /* [in] */ ICurrency* currency);

    CARAPI GetRoundingMode(
        /* [out] */ RoundingMode* mode);

    CARAPI SetRoundingMode(
        /* [in] */ RoundingMode mode);

private:
    // TODO: Add your private member variables here.
};

#endif // __CCHOICEFORMAT_H__
