
#ifndef __CNATIVEDECIMALFORMAT_H__
#define __CNATIVEDECIMALFORMAT_H__

#include "_CNativeDecimalFormat.h"
#include "NativeDecimalFormat.h"
CarClass(CNativeDecimalFormat), public NativeDecimalFormat
{
public:
    CARAPI constructor(
        /* [in] */ const String& pattern,
        /* [in] */ IDecimalFormatSymbols * pDfs);

    CARAPI Close();

    CARAPI SetDecimalFormatSymbols(
        /* [in] */ IDecimalFormatSymbols * pDfs);

    CARAPI FormatLong(
        /* [in] */ Int64 value,
        /* [in] */ IFieldPosition * pField,
        /* [out, callee] */ ArrayOf<Char32> ** ppArray);

    CARAPI FormatDouble(
        /* [in] */ Double value,
        /* [in] */ IFieldPosition * pField,
        /* [out, callee] */ ArrayOf<Char32> ** ppArray);

    CARAPI ApplyLocalizedPattern(
        /* [in] */ const String& pattern);

    CARAPI ApplyPattern(
        /* [in] */ const String& pattern);

    CARAPI FormatToCharacterIterator(
        /* [in] */ IInterface * pObject,
        /* [out] */ IAttributedCharacterIterator ** ppResult);

    CARAPI ToLocalizedPattern(
        /* [out] */ String * pLocalizedPattern);

    CARAPI ToPattern(
        /* [out] */ String * pPattern);

    CARAPI Parse(
        /* [in] */ const String& string,
        /* [in] */ IParsePosition * pPosition,
        /* [out] */ INumber ** ppResult);

    CARAPI GetMaximumFractionDigits(
        /* [out] */ Int32 * pMaximumFractionDigits);

    CARAPI GetMaximumIntegerDigits(
        /* [out] */ Int32 * pMaximumIntegerDigits);

    CARAPI GetMinimumFractionDigits(
        /* [out] */ Int32 * pMinimumFractionDigits);

    CARAPI GetMinimumIntegerDigits(
        /* [out] */ Int32 * pMinimumIntegerDigits);

    CARAPI GetGroupingSize(
        /* [out] */ Int32 * pGroupingSize);

    CARAPI GetMultiplier(
        /* [out] */ Int32 * pMultiplier);

    CARAPI GetNegativePrefix(
        /* [out] */ String * pNegativePrefix);

    CARAPI GetNegativeSuffix(
        /* [out] */ String * pNegativeSuffix);

    CARAPI GetPositivePrefix(
        /* [out] */ String * pPositivePrefix);

    CARAPI GetPositiveSuffix(
        /* [out] */ String * pPositiveSuffix);

    CARAPI IsDecimalSeparatorAlwaysShown(
        /* [out] */ Boolean * pIsDecimalSeparatorAlwaysShown);

    CARAPI IsParseBigDecimal(
        /* [out] */ Boolean * pIsParseBigDecimal);

    CARAPI IsParseIntegerOnly(
        /* [out] */ Boolean * pIsParseIntegerOnly);

    CARAPI IsGroupingUsed(
        /* [out] */ Boolean * pIsGroupingUsed);

    CARAPI SetDecimalSeparatorAlwaysShown(
        /* [in] */ Boolean value);

    CARAPI SetCurrency(
        /* [in] */ ICurrency * pCurrency);

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

    CARAPI GetAddr(
        /* [out] */ Int32 * pValue);

    CARAPI GetLastPattern(
        /* [out] */ String* pValue);

    CARAPI GetNegPrefNull(
        /* [out] */ Boolean * pValue);

    CARAPI GetNegSuffNull(
        /* [out] */ Boolean * pValue);

    CARAPI GetPosPrefNull(
        /* [out] */ Boolean * pValue);

    CARAPI GetPosSuffNull(
        /* [out] */ Boolean * pValue);
private:
    // TODO: Add your private member variables here.
};

#endif // __CNATIVEDECIMALFORMAT_H__
