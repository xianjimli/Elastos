
#ifndef __CNATIVEDECIMALFORMAT_H__
#define __CNATIVEDECIMALFORMAT_H__

#include "_CNativeDecimalFormat.h"
#include "NativeDecimalFormat.h"

CarClass(CNativeDecimalFormat), public NativeDecimalFormat
{
public:
    CARAPI constructor(
        /* [in] */ const String& pattern,
        /* [in] */ IDecimalFormatSymbols* dfs);

    CARAPI Close();

    CARAPI SetDecimalFormatSymbols(
        /* [in] */ IDecimalFormatSymbols* dfs);

    CARAPI SetDecimalFormatSymbolsEx(
        /* [in] */ ILocaleData* ld);

    CARAPI FormatBigDecimal(
        /* [in] */ IBigDecimal* value,
        /* [in] */ IFieldPosition* field,
        /* [out, callee] */ ArrayOf<Char32>** result);

    CARAPI FormatBigInteger(
        /* [in] */ IBigInteger* value,
        /* [in] */ IFieldPosition* field,
        /* [out, callee] */ ArrayOf<Char32>** result);

    CARAPI FormatInt64(
        /* [in] */ Int64 value,
        /* [in] */ IFieldPosition* field,
        /* [out, callee] */ ArrayOf<Char32>** array);

    CARAPI FormatDouble(
        /* [in] */ Double value,
        /* [in] */ IFieldPosition* field,
        /* [out, callee] */ ArrayOf<Char32>** array);

    CARAPI ApplyLocalizedPattern(
        /* [in] */ const String& pattern);

    CARAPI ApplyPattern(
        /* [in] */ const String& pattern);

    CARAPI FormatToCharacterIterator(
        /* [in] */ IInterface* object,
        /* [out] */ IAttributedCharacterIterator** result);

    CARAPI ToLocalizedPattern(
        /* [out] */ String* localizedPattern);

    CARAPI ToPattern(
        /* [out] */ String* pattern);

    CARAPI Parse(
        /* [in] */ const String& string,
        /* [in] */ IParsePosition* position,
        /* [out] */ INumber** result);

    CARAPI GetMaximumFractionDigits(
        /* [out] */ Int32* maxFractionDigits);

    CARAPI GetMaximumIntegerDigits(
        /* [out] */ Int32* maxIntegerDigits);

    CARAPI GetMinimumFractionDigits(
        /* [out] */ Int32* minFractionDigits);

    CARAPI GetMinimumIntegerDigits(
        /* [out] */ Int32* minIntegerDigits);

    CARAPI GetGroupingSize(
        /* [out] */ Int32* size);

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

    CARAPI IsParseIntegerOnly(
        /* [out] */ Boolean* isParseIntegerOnly);

    CARAPI IsGroupingUsed(
        /* [out] */ Boolean* isGroupingUsed);

    CARAPI SetDecimalSeparatorAlwaysShown(
        /* [in] */ Boolean value);

    CARAPI SetCurrency(
        /* [in] */ ICurrency* currency);

    CARAPI SetGroupingSize(
        /* [in] */ Int32 value);

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
        /* [in] */ Boolean value);

    CARAPI SetParseIntegerOnly(
        /* [in] */ Boolean value);

    CARAPI SetRoundingMode(
        /* [in] */ RoundingMode roundingMode,
        /* [in] */ Double roundingIncrement);

    CARAPI_(Mutex*) GetSelfLock();
};

#endif // __CNATIVEDECIMALFORMAT_H__
