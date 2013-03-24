
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
        /* [in] */ IDecimalFormatSymbols * pValue);

    CARAPI constructor(
        /* [in] */ const String& pattern,
        /* [in] */ ILocale * pLocale);

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

    CARAPI FormatDouble(
        /* [in] */ Double value,
        /* [out] */ String * pFormat);

    CARAPI FormatDoubleEx(
        /* [in] */ Double value,
        /* [in] */ const String& buffer,
        /* [in] */ IFieldPosition* field,
        /* [out] */ String* result);

    CARAPI FormatInt64(
        /* [in] */ Int64 value,
        /* [out] */ String * pFormat);

    CARAPI FormatInt64Ex(
        /* [in] */ Int64 value,
        /* [in] */ const String& buffer,
        /* [in] */ IFieldPosition* field,
        /* [out] */ String* result);

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

    CARAPI ApplyLocalizedPattern(
        /* [in] */ const String& pattern);

    CARAPI ApplyPattern(
        /* [in] */ const String& pattern);

    CARAPI GetDecimalFormatSymbols(
        /* [out] */ IDecimalFormatSymbols ** ppDecimalFormatSymbols);

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

    CARAPI SetDecimalFormatSymbols(
        /* [in] */ IDecimalFormatSymbols * pValue);

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
        /* [out] */ String * pLocalizedPattern);

    CARAPI ToPattern(
        /* [out] */ String * pPattern);

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
