
#ifndef __CDECIMALFORMAT_H__
#define __CDECIMALFORMAT_H__

#include "_CDecimalFormat.h"
#include "DecimalFormat.h"

CarClass(CDecimalFormat), public DecimalFormat
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ const String& pattern);

    CARAPI constructor(
        /* [in] */ const String& pattern,
        /* [in] */ IDecimalFormatSymbols* value);

    CARAPI constructor(
        /* [in] */ const String& pattern,
        /* [in] */ ILocale* locale);

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
        /* [out] */ IAttributedCharacterIterator** iterator);

    CARAPI ParseObject(
        /* [in] */ const String& string,
        /* [out] */ IInterface** object);

    CARAPI ParseObjectEx(
        /* [in] */ const String& string,
        /* [in] */ IParsePosition* position,
        /* [out] */ IInterface** object);

    CARAPI FormatDouble(
        /* [in] */ Double value,
        /* [out] */ String* result);

    CARAPI FormatDoubleEx(
        /* [in] */ Double value,
        /* [in] */ const String& buffer,
        /* [in] */ IFieldPosition* field,
        /* [out] */ String* result);

    CARAPI FormatInt64(
        /* [in] */ Int64 value,
        /* [out] */ String* result);

    CARAPI FormatInt64Ex(
        /* [in] */ Int64 value,
        /* [in] */ const String& buffer,
        /* [in] */ IFieldPosition* field,
        /* [out] */ String* result);

    CARAPI GetMaximumFractionDigits(
        /* [out] */ Int32* maxFractionDigits);

    CARAPI GetMaximumIntegerDigits(
        /* [out] */ Int32* maxIntegerDigits);

    CARAPI GetMinimumFractionDigits(
        /* [out] */ Int32* minFractionDigits);

    CARAPI GetMinimumIntegerDigits(
        /* [out] */ Int32* minIntegerDigits);

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

    CARAPI ApplyLocalizedPattern(
        /* [in] */ const String& pattern);

    CARAPI ApplyPattern(
        /* [in] */ const String& pattern);

    CARAPI GetDecimalFormatSymbols(
        /* [out] */ IDecimalFormatSymbols** symbols);

    CARAPI GetGroupingSize(
        /* [out] */ Int32* groupingSize);

    CARAPI GetMultiplier(
        /* [out] */ Int32* multiplier);

    CARAPI GetNegativePrefix(
        /* [out] */ String* prefix);

    CARAPI GetNegativeSuffix(
        /* [out] */ String* suffix);

    CARAPI GetPositivePrefix(
        /* [out] */ String* prefix);

    CARAPI GetPositiveSuffix(
        /* [out] */ String* suffix);

    CARAPI IsDecimalSeparatorAlwaysShown(
        /* [out] */ Boolean* isAlwaysShown);

    CARAPI IsParseBigDecimal(
        /* [out] */ Boolean* isParseBigDecimal);

    CARAPI SetDecimalFormatSymbols(
        /* [in] */ IDecimalFormatSymbols* value);

    CARAPI SetDecimalSeparatorAlwaysShown(
        /* [in] */ Boolean value);

    CARAPI SetGroupingSize(
        /* [in] */ Int32 value);

    CARAPI SetMultiplier(
        /* [in] */ Int32 value);

    CARAPI SetNegativePrefix(
        /* [in] */ const String& value);

    CARAPI SetNegativeSuffix(
        /* [in] */ const String& value);

    CARAPI SetPositivePrefix(
        /* [in] */ const String& value);

    CARAPI SetPositiveSuffix(
        /* [in] */ const String& value);

    CARAPI SetParseBigDecimal(
        /* [in] */ Boolean newValue);

    CARAPI ToLocalizedPattern(
        /* [out] */ String* localizedPattern);

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

#endif // __CDECIMALFORMAT_H__
