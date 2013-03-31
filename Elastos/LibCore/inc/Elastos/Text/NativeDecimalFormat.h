
#ifndef __NATIVEDECIMALFORMAT_H__
#define __NATIVEDECIMALFORMAT_H__

#include "Elastos.Text_server.h"
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>
#include <elastos/AutoFree.h>
#include <elastos/Mutex.h>
#include <StringBuffer.h>

using namespace Elastos::Core::Threading;

class NativeDecimalFormat
{
public:
    // Utility to get information about field positions from native (ICU) code.
    class FieldPositionIterator : public ElRefBase
    {
        friend class NativeDecimalFormat;

    public:
        static CARAPI_(AutoPtr<FieldPositionIterator>) ForFieldPosition(
            /* [in] */ IFieldPosition* fp);

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

        // called by native
        CARAPI SetData(
            /* [in] */ ArrayOf<Int32>* data);

    private:
        FieldPositionIterator();

        static CARAPI_(Int32) GetNativeFieldPositionId(
            /* [in] */ IFieldPosition* fp);

        static CARAPI SetFieldPosition(
            /* [in] */ FieldPositionIterator* fpi,
            /* [in] */ IFieldPosition* fp);

        CARAPI CheckValid();

    private:
        AutoFree< ArrayOf<Int32> > mData;
        Int32 mPos; // so first call to next() leaves pos at 0
        static ArrayOf< AutoPtr<IFormatField> >* sFields;
    };

public:
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

    CARAPI SetDecimalFormatSymbols(
        /* [in] */ ILocaleData* localeData);

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
        /* [out] */ IAttributedCharacterIterator** iter);

    CARAPI ToLocalizedPattern(
        /* [out] */ String* localizedPattern);

    CARAPI ToPattern(
        /* [out] */ String* pattern);

    CARAPI Parse(
        /* [in] */ const String& string,
        /* [in] */ IParsePosition* position,
        /* [out] */ INumber** number);

    // start getter and setter

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
        /* [in] */ RoundingMode mode,
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

protected:
    NativeDecimalFormat();

    CARAPI Init(
        /* [in] */ const String& pattern,
        /* [in] */ IDecimalFormatSymbols* dfs);

    // Used so java.util.Formatter doesn't need to allocate DecimalFormatSymbols instances.
    CARAPI Init(
        /* [in] */ const String& pattern,
        /* [in] */ ILocaleData* data);

    virtual CARAPI_(Mutex*) GetSelfLock() = 0;

private:
    // Used to implement clone.
    // NativeDecimalFormat(
    //     /* [in] */ INativeDecimalFormat* other);

    CARAPI_(Int32) MakeScalePositive(
        /* [in] */ Int32 scale,
        /* [in] */ StringBuffer& val);

    static CARAPI ApplyPattern(
        /* [in] */ Handle32 addr,
        /* [in] */ Boolean localized,
        /* [in] */ const String& pattern);

    static CARAPI ApplyPatternImpl(
        /* [in] */ Handle32 addr,
        /* [in] */ Boolean localized,
        /* [in] */ const String& pattern);

    static CARAPI_(Handle32) CloneImpl(
        /* [in] */ Handle32 addr);

    static CARAPI_(void) Close(
        /* [in] */ Handle32 addr);

    static CARAPI_(ArrayOf<Char32>*) FormatInt64(
        /* [in] */ Handle32 addr,
        /* [in] */ Int64 value,
        /* [in] */ FieldPositionIterator* iter);

    static CARAPI_(ArrayOf<Char32>*) FormatDouble(
        /* [in] */ Handle32 addr,
        /* [in] */ Double value,
        /* [in] */ FieldPositionIterator* iter);

    static CARAPI_(ArrayOf<Char32>*) FormatDigitList(
        /* [in] */ Handle32 addr,
        /* [in] */ const String& value,
        /* [in] */ FieldPositionIterator* iter);

    static CARAPI_(Int32) GetAttribute(
        /* [in] */ Handle32 addr,
        /* [in] */ Int32 symbol);

    static CARAPI GetTextAttribute(
        /* [in] */ Handle32 addr,
        /* [in] */ Int32 symbol,
        /* [out] */ String* attr);

    static CARAPI Open(
        /* [in] */ const String& pattern,
        /* [in] */ const String& currencySymbol,
        /* [in] */ Char32 decimalSeparator,
        /* [in] */ Char32 digit,
        /* [in] */ const String& exponentSeparator,
        /* [in] */ Char32 groupingSeparator,
        /* [in] */ const String& infinity,
        /* [in] */ const String& internationalCurrencySymbol,
        /* [in] */ Char32 minusSign,
        /* [in] */ Char32 monetaryDecimalSeparator,
        /* [in] */ const String& nan,
        /* [in] */ Char32 patternSeparator,
        /* [in] */ Char32 percent,
        /* [in] */ Char32 perMill,
        /* [in] */ Char32 zeroDigit,
        /* [out] */ Handle32* addr);

    static CARAPI_(AutoPtr<INumber>) Parse(
        /* [in] */ Handle32 addr,
        /* [in] */ const String& string,
        /* [in] */ IParsePosition* position,
        /* [in] */ Boolean parseBigDecimal);

    static CARAPI_(void) SetDecimalFormatSymbols(
        /* [in] */ Handle32 addr,
        /* [in] */ const String& currencySymbol,
        /* [in] */ Char32 decimalSeparator,
        /* [in] */ Char32 digit,
        /* [in] */ const String& exponentSeparator,
        /* [in] */ Char32 groupingSeparator,
        /* [in] */ const String& infinity,
        /* [in] */ const String& internationalCurrencySymbol,
        /* [in] */ Char32 minusSign,
        /* [in] */ Char32 monetaryDecimalSeparator,
        /* [in] */ const String& nan,
        /* [in] */ Char32 patternSeparator,
        /* [in] */ Char32 percent,
        /* [in] */ Char32 perMill,
        /* [in] */ Char32 zeroDigit);

    static CARAPI SetSymbol(
        /* [in] */ Handle32 addr,
        /* [in] */ Int32 symbol,
        /* [in] */ const String& str);

    static CARAPI_(void) SetAttribute(
        /* [in] */ Handle32 addr,
        /* [in] */ Int32 symbol,
        /* [in] */ Int32 i);

    static CARAPI_(void) SetRoundingMode(
        /* [in] */ Handle32 addr,
        /* [in] */ Int32 roundingMode,
        /* [in] */ Double roundingIncrement);

    static CARAPI SetTextAttribute(
        /* [in] */ Handle32 addr,
        /* [in] */ Int32 symbol,
        /* [in] */ const String& str);

    static CARAPI_(String) ToPatternImpl(
        /* [in] */ Handle32 addr,
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
    Handle32 mAddr;

    /**
     * The last pattern we gave to ICU, so we can make repeated applications cheap.
     * This helps in cases like String.format("%.2f,%.2f\n", x, y) where the DecimalFormat is
     * reused.
     */
    String mLastPattern;

    // TODO: store all these in DecimalFormat instead!
    Boolean mNegPrefNull;
    Boolean mNegSuffNull;
    Boolean mPosPrefNull;
    Boolean mPosSuffNull;

    mutable Boolean mParseBigDecimal;

    /**
     * Cache the BigDecimal form of the multiplier. This is null until we've
     * formatted a BigDecimal (with a multiplier that is not 1), or the user has
     * explicitly called {@link #setMultiplier(int)} with any multiplier.
     */
//    private BigDecimal multiplierBigDecimal = null;
};

#endif //__NATIVEDECIMALFORMAT_H__
