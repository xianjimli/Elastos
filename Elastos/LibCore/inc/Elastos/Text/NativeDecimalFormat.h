#ifndef __NATIVEDECIMALFORMAT_H__
#define __NATIVEDECIMALFORMAT_H__

#include "cmdef.h"
#include "Elastos.Text_server.h"
#include <elastos.h>
#include <elastos/AutoPtr.h>
//#include "LocaleData.h"
#include <StringBuffer.h>
using namespace Elastos;

class NativeDecimalFormat {
private:
    // Utility to get information about field positions from native (ICU) code.
    class FieldPositionIterator {
    public:
        static CARAPI ForFieldPosition(
            /* [in] */ IFieldPosition* fp,
            /* [out] */ FieldPositionIterator** fpi);

        CARAPI Next(
            /* [out] */ Boolean* next);

        CARAPI FieldId(
            /* [out] */ Int32* fieldId);

        CARAPI Field(
            /* [out] */ IFormatField** ff);

        CARAPI Start(
            /* [out] */ Int32* start);

        CARAPI Limit(
            /* [out] */ Int32* limit);

        static CARAPI SetFieldPosition(
            /* [in] */ FieldPositionIterator* fpi,
            /* [in] */ IFieldPosition* fp);
    private:
        FieldPositionIterator();

        static CARAPI GetNativeFieldPositionId(
            /* [in] */ IFieldPosition* fp,
            /* [out] */ Int32* nfpid);

        CARAPI CheckValid();

        // called by native
        CARAPI SetData(
            /* [in] */ ArrayOf<Int32>* data);
    private:
        ArrayOf<Int32>* data;
        Int32 pos; // so first call to next() leaves pos at 0
        static ArrayOf<IFormatField*>* fields; /*= {
            // The old java field values were 0 for integer and 1 for fraction.
            // The new java field attributes are all objects.  ICU assigns the values
            // starting from 0 in the following order; note that integer and
            // fraction positions match the old field values.
            NumberFormat.Field.INTEGER,
            NumberFormat.Field.FRACTION,
            NumberFormat.Field.DECIMAL_SEPARATOR,
            NumberFormat.Field.EXPONENT_SYMBOL,
            NumberFormat.Field.EXPONENT_SIGN,
            NumberFormat.Field.EXPONENT,
            NumberFormat.Field.GROUPING_SEPARATOR,
            NumberFormat.Field.CURRENCY,
            NumberFormat.Field.PERCENT,
            NumberFormat.Field.PERMILLE,
            NumberFormat.Field.SIGN,
        };*/

    };
public:
    NativeDecimalFormat();
    /**
     * Cache the BigDecimal form of the multiplier. This is null until we've
     * formatted a BigDecimal (with a multiplier that is not 1), or the user has
     * explicitly called {@link #setMultiplier(int)} with any multiplier.
     */
//    private BigDecimal multiplierBigDecimal = null;

    CARAPI Init(
        /* [in] */ String pattern,
        /* [in] */ IDecimalFormatSymbols* dfs);

    // Used so java.util.Formatter doesn't need to allocate DecimalFormatSymbols instances.
//    NativeDecimalFormat(
//        /* [in] */ String pattern,
//        /* [in] */ LocaleData* data);

    // TODO: remove this and just have DecimalFormat.hashCode do the right thing itself.
    //@Override
    //public int hashCode() {
    //    return this.getPositivePrefix().hashCode();
    //}

    //synchronized
    CARAPI Close();

    //@Override
    //public Object clone() {
    //    return new NativeDecimalFormat(this);
    //}

    /**
     * Note: this doesn't check that the underlying native DecimalFormat objects' configured
     * native DecimalFormatSymbols objects are equal. It is assumed that the
     * caller (DecimalFormat) will check the DecimalFormatSymbols objects
     * instead, for performance.
     *
     * This is also unreasonably expensive, calling down to JNI multiple times.
     *
     * TODO: remove this and just have DecimalFormat.equals do the right thing itself.
     */
    //@Override
    /*
    public boolean equals(Object object) {
        if (object == this) {
            return true;
        }
        if (!(object instanceof NativeDecimalFormat)) {
            return false;
        }
        NativeDecimalFormat obj = (NativeDecimalFormat) object;
        if (obj.addr == this.addr) {
            return true;
        }
        return obj.toPattern().equals(this.toPattern()) &&
                obj.isDecimalSeparatorAlwaysShown() == this.isDecimalSeparatorAlwaysShown() &&
                obj.getGroupingSize() == this.getGroupingSize() &&
                obj.getMultiplier() == this.getMultiplier() &&
                obj.getNegativePrefix().equals(this.getNegativePrefix()) &&
                obj.getNegativeSuffix().equals(this.getNegativeSuffix()) &&
                obj.getPositivePrefix().equals(this.getPositivePrefix()) &&
                obj.getPositiveSuffix().equals(this.getPositiveSuffix()) &&
                obj.getMaximumIntegerDigits() == this.getMaximumIntegerDigits() &&
                obj.getMaximumFractionDigits() == this.getMaximumFractionDigits() &&
                obj.getMinimumIntegerDigits() == this.getMinimumIntegerDigits() &&
                obj.getMinimumFractionDigits() == this.getMinimumFractionDigits() &&
                obj.isGroupingUsed() == this.isGroupingUsed();
    }
    */

    /**
     * Copies the DecimalFormatSymbols settings into our native peer in bulk.
     */
    CARAPI SetDecimalFormatSymbols(
        /* [in] */ IDecimalFormatSymbols* dfs);

//    CARAPI SetDecimalFormatSymbols(
//        /* [in] */ const LocaleData* localeData);

/*
    public char[] formatBigDecimal(BigDecimal value, FieldPosition field) {
        FieldPositionIterator fpi = FieldPositionIterator.forFieldPosition(field);
        char[] result = formatDigitList(this.addr, value.toString(), fpi);
        if (fpi != null) {
            FieldPositionIterator.setFieldPosition(fpi, field);
        }
        return result;
    }

    public char[] formatBigInteger(BigInteger value, FieldPosition field) {
        FieldPositionIterator fpi = FieldPositionIterator.forFieldPosition(field);
        char[] result = formatDigitList(this.addr, value.toString(10), fpi);
        if (fpi != null) {
            FieldPositionIterator.setFieldPosition(fpi, field);
        }
        return result;
    }
*/
    CARAPI FormatLong(
        /* [in] */ Int64 value,
        /* [in] */ IFieldPosition* field,
        /* [out, callee] */ ArrayOf<Char32>** array);

    CARAPI FormatDouble(
        /* [in] */ Double value,
        /* [in] */ IFieldPosition* field,
        /* [out, callee] */ ArrayOf<Char32>** array);

    CARAPI ApplyLocalizedPattern(
        /* [in] */ String pattern);

    CARAPI ApplyPattern(
        /* [in] */ String pattern);

    CARAPI FormatToCharacterIterator(
        /* [in] */ IInterface* object,
        /* [out] */ IAttributedCharacterIterator** iter);

    CARAPI ToLocalizedPattern(
        /* [out] */ String* localizedPattern);

    CARAPI ToPattern(
        /* [out] */ String* pattern);

    CARAPI Parse(
        /* [in] */ String string,
        /* [in] */ IParsePosition* position,
        /* [out] */ INumber** number);

    // start getter and setter

    CARAPI GetMaximumFractionDigits(
        /* [out] */ Int32* maximumFractionDigits);

    CARAPI GetMaximumIntegerDigits(
        /* [out] */ Int32* maximumIntegerDigits);

    CARAPI GetMinimumFractionDigits(
        /* [out] */ Int32* minimumFractionDigits);

    CARAPI GetMinimumIntegerDigits(
        /* [out] */ Int32* minimumIntegerDigits);

    CARAPI GetGroupingSize(
        /* [out] */ Int32* groupingSize);

    CARAPI GetMultiplier(
        /* [out] */ Int32* multiplier);

    CARAPI GetNegativePrefix(
        /* [out] */ String* negativePrefix);

    CARAPI GetNegativeSuffix(
        /* [out] */ String* negativeSuffix);

    CARAPI GetPositivePrefix(
        /* [out] */ String* positivePrefix);

    CARAPI GetPositiveSuffix(
        /* [out] */ String* positiveSuffix);

    CARAPI IsDecimalSeparatorAlwaysShown(
        /* [out] */ Boolean* isDecimalSeparatorAlwaysShown);

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
        /* [in] */ String value);

    CARAPI SetNegativeSuffix(
        /* [in] */ String value);

    CARAPI SetPositivePrefix(
        /* [in] */ String value);

    CARAPI SetPositiveSuffix(
        /* [in] */ String value);

    CARAPI SetParseBigDecimal(
        /* [in] */ Boolean value);

    CARAPI SetParseIntegerOnly(
        /* [in] */ Boolean value);

    CARAPI SetRoundingMode(
        /* [in] */ RoundingMode roundingMode,
        /* [in] */ Double roundingIncrement);

    CARAPI GetAddr(
        /* [out] */ Int32* value);

    CARAPI GetLastPattern(
        /* [out] */ String* value);

    CARAPI GetNegPrefNull(
        /* [out] */ Boolean* value);

    CARAPI GetNegSuffNull(
        /* [out] */ Boolean* value);

    CARAPI GetPosPrefNull(
        /* [out] */ Boolean* value);

    CARAPI GetPosSuffNull(
        /* [out] */ Boolean* value);
private:
    // Used to implement clone.
    NativeDecimalFormat(
        /* [in] */ INativeDecimalFormat* other);

    CARAPI MakeScalePositive(
        /* [in] */ Int32 scale,
        /* [in] */ StringBuffer val,
        /* [out] */ Int32* value);

    static CARAPI ApplyPattern(
        /* [in] */ Int32 addr,
        /* [in] */ Boolean localized,
        /* [in] */ String pattern);

    static CARAPI_(void) ApplyPatternImpl(
        /* [in] */ Int32 addr,
        /* [in] */ Boolean localized,
        /* [in] */ String pattern);

    static CARAPI_(Int32) CloneImpl(
        /* [in] */ Int32 addr);

    static CARAPI_(void) Close(
        /* [in] */ Int32 addr);

    static CARAPI_(ArrayOf<Char32>*) FormatLong(
        /* [in] */ Int32 addr,
        /* [in] */ Int64 value,
        /* [in] */ FieldPositionIterator* iter);

    static CARAPI_(ArrayOf<Char32>*) FormatDouble(
        /* [in] */ Int32 addr,
        /* [in] */ Double value,
        /* [in] */ FieldPositionIterator* iter);

    static CARAPI_(ArrayOf<Char32>*) FormatDigitList(
        /* [in] */ Int32 addr,
        /* [in] */ String value,
        /* [in] */ FieldPositionIterator* iter);

    static CARAPI_(Int32) GetAttribute(
        /* [in] */ Int32 addr,
        /* [in] */ Int32 symbol);

    static CARAPI_(String) GetTextAttribute(
        /* [in] */ Int32 addr,
        /* [in] */ Int32 symbol);

    static CARAPI_(Int32) Open(
        /* [in] */ String pattern,
        /* [in] */ String currencySymbol,
        /* [in] */ Char32 decimalSeparator,
        /* [in] */ Char32 digit,
        /* [in] */ String exponentSeparator,
        /* [in] */ Char32 groupingSeparator,
        /* [in] */ String infinity,
        /* [in] */ String internationalCurrencySymbol,
        /* [in] */ Char32 minusSign,
        /* [in] */ Char32 monetaryDecimalSeparator,
        /* [in] */ String nan,
        /* [in] */ Char32 patternSeparator,
        /* [in] */ Char32 percent,
        /* [in] */ Char32 perMill,
        /* [in] */ Char32 zeroDigit);

    static CARAPI_(INumber*) Parse(
        /* [in] */ Int32 addr,
        /* [in] */ String string,
        /* [in] */ IParsePosition* position,
        /* [in] */ Boolean parseBigDecimal);

    static CARAPI_(void) SetDecimalFormatSymbols(
        /* [in] */ Int32 addr,
        /* [in] */ String currencySymbol,
        /* [in] */ Char32 decimalSeparator,
        /* [in] */ Char32 digit,
        /* [in] */ String exponentSeparator,
        /* [in] */ Char32 groupingSeparator,
        /* [in] */ String infinity,
        /* [in] */ String internationalCurrencySymbol,
        /* [in] */ Char32 minusSign,
        /* [in] */ Char32 monetaryDecimalSeparator,
        /* [in] */ String nan,
        /* [in] */ Char32 patternSeparator,
        /* [in] */ Char32 percent,
        /* [in] */ Char32 perMill,
        /* [in] */ Char32 zeroDigit);

    static CARAPI_(void) SetSymbol(
        /* [in] */ Int32 addr,
        /* [in] */ Int32 symbol,
        /* [in] */ String str);
    static CARAPI_(void) SetAttribute(
        /* [in] */ Int32 addr,
        /* [in] */ Int32 symbol,
        /* [in] */ Int32 i);

    static CARAPI_(void) SetRoundingMode(
        /* [in] */ Int32 addr,
        /* [in] */ Int32 roundingMode,
        /* [in] */ Double roundingIncrement);

    static CARAPI_(void) SetTextAttribute(
        /* [in] */ Int32 addr,
        /* [in] */ Int32 symbol,
        /* [in] */ String str);

    static CARAPI_(String) ToPatternImpl(
        /* [in] */ Int32 addr,
        /* [in] */ Boolean localized);
private:
    /**
     * Constants corresponding to the native type UNumberFormatSymbol, for setSymbol.
     */
    const static Int32 UNUM_DECIMAL_SEPARATOR_SYMBOL = 0;
    const static Int32 UNUM_GROUPING_SEPARATOR_SYMBOL = 1;
    const static Int32 UNUM_PATTERN_SEPARATOR_SYMBOL = 2;
    const static Int32 UNUM_PERCENT_SYMBOL = 3;
    const static Int32 UNUM_ZERO_DIGIT_SYMBOL = 4;
    const static Int32 UNUM_DIGIT_SYMBOL = 5;
    const static Int32 UNUM_MINUS_SIGN_SYMBOL = 6;
    const static Int32 UNUM_PLUS_SIGN_SYMBOL = 7;
    const static Int32 UNUM_CURRENCY_SYMBOL = 8;
    const static Int32 UNUM_INTL_CURRENCY_SYMBOL = 9;
    const static Int32 UNUM_MONETARY_SEPARATOR_SYMBOL = 10;
    const static Int32 UNUM_EXPONENTIAL_SYMBOL = 11;
    const static Int32 UNUM_PERMILL_SYMBOL = 12;
    const static Int32 UNUM_PAD_ESCAPE_SYMBOL = 13;
    const static Int32 UNUM_INFINITY_SYMBOL = 14;
    const static Int32 UNUM_NAN_SYMBOL = 15;
    const static Int32 UNUM_SIGNIFICANT_DIGIT_SYMBOL = 16;
    const static Int32 UNUM_MONETARY_GROUPING_SEPARATOR_SYMBOL = 17;
    const static Int32 UNUM_FORMAT_SYMBOL_COUNT = 18;

    /**
     * Constants corresponding to the native type UNumberFormatAttribute, for
     * getAttribute/setAttribute.
     */
    const static Int32 UNUM_PARSE_INT_ONLY = 0;
    const static Int32 UNUM_GROUPING_USED = 1;
    const static Int32 UNUM_DECIMAL_ALWAYS_SHOWN = 2;
    const static Int32 UNUM_MAX_INTEGER_DIGITS = 3;
    const static Int32 UNUM_MIN_INTEGER_DIGITS = 4;
    const static Int32 UNUM_INTEGER_DIGITS = 5;
    const static Int32 UNUM_MAX_FRACTION_DIGITS = 6;
    const static Int32 UNUM_MIN_FRACTION_DIGITS = 7;
    const static Int32 UNUM_FRACTION_DIGITS = 8;
    const static Int32 UNUM_MULTIPLIER = 9;
    const static Int32 UNUM_GROUPING_SIZE = 10;
    const static Int32 UNUM_ROUNDING_MODE = 11;
    const static Int32 UNUM_ROUNDING_INCREMENT = 12;
    const static Int32 UNUM_FORMAT_WIDTH = 13;
    const static Int32 UNUM_PADDING_POSITION = 14;
    const static Int32 UNUM_SECONDARY_GROUPING_SIZE = 15;
    const static Int32 UNUM_SIGNIFICANT_DIGITS_USED = 16;
    const static Int32 UNUM_MIN_SIGNIFICANT_DIGITS = 17;
    const static Int32 UNUM_MAX_SIGNIFICANT_DIGITS = 18;
    const static Int32 UNUM_LENIENT_PARSE = 19;

    /**
     * Constants corresponding to the native type UNumberFormatTextAttribute, for
     * getTextAttribute/setTextAttribute.
     */
    const static Int32 UNUM_POSITIVE_PREFIX = 0;
    const static Int32 UNUM_POSITIVE_SUFFIX = 1;
    const static Int32 UNUM_NEGATIVE_PREFIX = 2;
    const static Int32 UNUM_NEGATIVE_SUFFIX = 3;
    const static Int32 UNUM_PADDING_CHARACTER = 4;
    const static Int32 UNUM_CURRENCY_CODE = 5;
    const static Int32 UNUM_DEFAULT_RULESET = 6;
    const static Int32 UNUM_PUBLIC_RULESETS = 7;

    /**
     * The address of the ICU DecimalFormat* on the native heap.
     */
    Int32 addr;

    /**
     * The last pattern we gave to ICU, so we can make repeated applications cheap.
     * This helps in cases like String.format("%.2f,%.2f\n", x, y) where the DecimalFormat is
     * reused.
     */
    String* lastPattern;

    // TODO: store all these in DecimalFormat instead!
    Boolean negPrefNull;
    Boolean negSuffNull;
    Boolean posPrefNull;
    Boolean posSuffNull;

    mutable Boolean parseBigDecimal;
};
#endif //__NATIVEDECIMALFORMAT_H__