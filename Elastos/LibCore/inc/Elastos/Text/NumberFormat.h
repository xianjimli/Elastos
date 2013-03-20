#ifndef __NUMBERFORMAT_H__
#define __NUMBERFORMAT_H__

#include <elastos.h>
#include <StringBuffer.h>
#include <elastos/AutoPtr.h>

#include "Elastos.Text_server.h"
#include "cmdef.h"
#include "Format.h"
#include "CFieldPosition.h"
#include "CParsePosition.h"

using namespace Elastos;

class NumberFormat : public Format {
protected:
    /**
     * Used by subclasses. This was public in Java 5.
     */
    NumberFormat();

    virtual ~NumberFormat();

public:
    /**
     * Returns a new {@code NumberFormat} with the same properties as this
     * {@code NumberFormat}.
     *
     * @return a shallow copy of this {@code NumberFormat}.
     * @see java.lang.Cloneable
     */
//    @Override
//    public Object clone() {
//        return super.clone();
//    }

    /**
     * Compares the specified object to this number format and indicates if
     * they are equal. In order to be equal, {@code object} must be an instance
     * of {@code NumberFormat} with the same pattern and properties.
     *
     * @param object
     *            the object to compare with this object.
     * @return {@code true} if the specified object is equal to this number
     *         format; {@code false} otherwise.
     * @see #hashCode
     */
//    @Override
//    public boolean equals(Object object) {
//        if (object == this) {
//            return true;
//        }
//        if (!(object instanceof NumberFormat)) {
//            return false;
//        }
//        NumberFormat obj = (NumberFormat) object;
//        return groupingUsed == obj.groupingUsed
//                && parseIntegerOnly == obj.parseIntegerOnly
//                && maximumFractionDigits == obj.maximumFractionDigits
//                && maximumIntegerDigits == obj.maximumIntegerDigits
//                && minimumFractionDigits == obj.minimumFractionDigits
//                && minimumIntegerDigits == obj.minimumIntegerDigits;
//    }

    /**
     * Formats the specified double using the rules of this number format.
     *
     * @param value
     *            the double to format.
     * @return the formatted string.
     */
    CARAPI formatEx2(
        /* [in] */ Double value,
        /* [out] */ String* formattedString);

    /**
     * Formats the specified double value as a string using the pattern of this
     * number format and appends the string to the specified string buffer.
     * <p>
     * If the {@code field} member of {@code position} contains a value
     * specifying a format field, then its {@code beginIndex} and
     * {@code endIndex} members will be updated with the position of the first
     * occurrence of this field in the formatted text.
     *
     * @param value
     *            the double to format.
     * @param buffer
     *            the target string buffer to append the formatted double value
     *            to.
     * @param field
     *            on input: an optional alignment field; on output: the offsets
     *            of the alignment field in the formatted text.
     * @return the string buffer.
     */
    virtual CARAPI formatEx3(
        /* [in] */ Double value,
        /* [in] */ StringBuffer* buffer,
        /* [in] */ IFieldPosition* field,
        /* [out] */ StringBuffer* result) = 0;

    /**
     * Formats the specified long using the rules of this number format.
     *
     * @param value
     *            the long to format.
     * @return the formatted string.
     */
    CARAPI formatEx4(
        /* [in] */ Int64 value,
        /* [out] */ String* formattedString);

    /**
     * Formats the specified long value as a string using the pattern of this
     * number format and appends the string to the specified string buffer.
     * <p>
     * If the {@code field} member of {@code position} contains a value
     * specifying a format field, then its {@code beginIndex} and
     * {@code endIndex} members will be updated with the position of the first
     * occurrence of this field in the formatted text.
     *
     * @param value
     *            the long to format.
     * @param buffer
     *            the target string buffer to append the formatted long value
     *            to.
     * @param field
     *            on input: an optional alignment field; on output: the offsets
     *            of the alignment field in the formatted text.
     * @return the string buffer.
     */
    virtual CARAPI formatEx5(
        /* [in] */ Int64 value, 
        /* [in] */ StringBuffer* buffer, 
        /* [in] */ IFieldPosition* field,
        /* [out] */ StringBuffer* formattedString) = 0;

    /**
     * Formats a number into a supplied buffer.
     * <p>
     * The number must be a subclass of {@code Number}. Instances of {@code Byte}, {@code Short},
     * {@code Integer}, and {@code Long} have {@code Number.longValue} invoked, as do instances of
     * {@code BigInteger} where {@code BigInteger.bitLength} returns <i>less than</i> 64. All other
     * values have {@code Number.doubleValue} invoked instead.
     * <p>
     * If the {@code field} member of {@code field} contains a value specifying
     * a format field, then its {@code beginIndex} and {@code endIndex} members
     * will be updated with the position of the first occurrence of this field
     * in the formatted text.
     *
     * @param object
     *            the object to format, must be a {@code Number}.
     * @param buffer
     *            the target string buffer to append the formatted number to.
     * @param field
     *            on input: an optional alignment field; on output: the offsets
     *            of the alignment field in the formatted text.
     * @return the string buffer.
     * @throws IllegalArgumentException
     *             if {@code object} is not an instance of {@code Number}.
     */
    //@Override
    CARAPI formatEx(
            /* [in] */ IInterface* object,
            /* [in] */ StringBuffer* buffer,
            /* [in] */ IFieldPosition* field,
            /* [out] */ StringBuffer* value);

    /**
     * Returns an array of locales for which custom {@code NumberFormat} instances
     * are available.
     * <p>Note that Android does not support user-supplied locale service providers.
     */
    static CARAPI GetAvailableLocales(
        /* [out] */ ArrayOf<ILocale*>** locales);

    /**
     * Returns the currency used by this number format.
     * <p>
     * This implementation throws {@code UnsupportedOperationException},
     * concrete subclasses should override this method if they support currency
     * formatting.
     * <p>
     *
     * @return the currency that was set in getInstance() or in setCurrency(),
     *         or {@code null}.
     * @throws UnsupportedOperationException
     */
//    virtual CARAPI GetCurrency(
//        /* [out] */ ICurrency** currency);

    /**
     * Returns a {@code NumberFormat} for formatting and parsing currency values
     * for the user's default locale.
     * See "<a href="../util/Locale.html#default_locale">Be wary of the default locale</a>".
     *
     * @return a {@code NumberFormat} for handling currency values.
     */
    static CARAPI GetCurrencyInstance(
        /* [out] */ INumberFormat** instance);

    /**
     * Returns a {@code NumberFormat} for formatting and parsing currency values
     * for the specified locale.
     *
     * @param locale
     *            the locale to use.
     * @return a {@code NumberFormat} for handling currency values.
     */
    static CARAPI GetCurrencyInstance(
        /* [in] */ ILocale* locale,
        /* [out] */ INumberFormat** instance);

    /**
     * Returns a {@code NumberFormat} for formatting and parsing integers for the
     * user's default locale.
     * See "<a href="../util/Locale.html#default_locale">Be wary of the default locale</a>".
     *
     * @return a {@code NumberFormat} for handling integers.
     */
    static CARAPI GetIntegerInstance(
        /* [out] */ INumberFormat** instance);

    /**
     * Returns a {@code NumberFormat} for formatting and parsing integers for
     * the specified locale.
     *
     * @param locale
     *            the locale to use.
     * @return a {@code NumberFormat} for handling integers.
     */
    static CARAPI GetIntegerInstance(
        /* [in] */ ILocale* locale,
        /* [out] */ INumberFormat** instance);

    /**
     * Returns a {@code NumberFormat} for formatting and parsing numbers for the
     * default locale.
     *
     * @return a {@code NumberFormat} for handling {@code Number} objects.
     */
    static CARAPI GetInstance(
        /* [out] */ INumberFormat** instance);

    /**
     * Returns a {@code NumberFormat} for formatting and parsing numbers for the
     * specified locale.
     *
     * @param locale
     *            the locale to use.
     * @return a {@code NumberFormat} for handling {@code Number} objects.
     */
    static CARAPI GetInstance(
        /* [in] */ ILocale* locale,
        /* [out] */ INumberFormat** instance);

    /**
     * Returns the maximum number of fraction digits that are printed when
     * formatting. If the maximum is less than the number of fraction digits,
     * the least significant digits are truncated.
     *
     * @return the maximum number of fraction digits.
     */
    virtual CARAPI GetMaximumFractionDigits(
        /* [out] */ Int32* number);

    /**
     * Returns the maximum number of integer digits that are printed when
     * formatting. If the maximum is less than the number of integer digits, the
     * most significant digits are truncated.
     *
     * @return the maximum number of integer digits.
     */
    virtual CARAPI GetMaximumIntegerDigits(
        /* [out] */ Int32* number);

    /**
     * Returns the minimum number of fraction digits that are printed when
     * formatting.
     *
     * @return the minimum number of fraction digits.
     */
    virtual CARAPI GetMinimumFractionDigits(
        /* [out] */ Int32* number);

    /**
     * Returns the minimum number of integer digits that are printed when
     * formatting.
     *
     * @return the minimum number of integer digits.
     */
    virtual CARAPI GetMinimumIntegerDigits(
        /* [out] */ Int32* number);

    /**
     * Returns a {@code NumberFormat} for formatting and parsing numbers for the
     * user's default locale.
     * See "<a href="../util/Locale.html#default_locale">Be wary of the default locale</a>".
     *
     * @return a {@code NumberFormat} for handling {@code Number} objects.
     */
    static CARAPI GetNumberInstance(
        /* [out] */ INumberFormat** instance);

    /**
     * Returns a {@code NumberFormat} for formatting and parsing numbers for the
     * specified locale.
     *
     * @param locale
     *            the locale to use.
     * @return a {@code NumberFormat} for handling {@code Number} objects.
     */
    static CARAPI GetNumberInstance(
        /* [in] */ ILocale* locale,
        /* [out] */ INumberFormat** instance);

    /**
     * Returns a {@code NumberFormat} for formatting and parsing percentage
     * values for the user's default locale.
     * See "<a href="../util/Locale.html#default_locale">Be wary of the default locale</a>".
     *
     * <p>The {@code NumberFormat} returned by this method should only be used
     * to format floating-point numbers typically between 0 and 1 (with 1 being 100%).
     * A value such as 0.53 will be treated as 53%, but 53.0 (or the integer 53) will be
     * treated as 5,300%, which is rarely what you intended.
     */
    static CARAPI GetPercentInstance(
        /* [out] */ INumberFormat** instance);

    /**
     * Returns a {@code NumberFormat} for formatting and parsing percentage
     * values for the given {@code locale}.
     *
     * <p>The {@code NumberFormat} returned by this method should only be used
     * to format floating-point numbers typically between 0 and 1 (with 1 being 100%).
     * A value such as 0.53 will be treated as 53%, but 53.0 (or the integer 53) will be
     * treated as 5,300%, which is rarely what you intended.
     */
    static CARAPI GetPercentInstance(
        /* [in] */ ILocale* locale,
        /* [out] */ INumberFormat** instance);

//    @Override
//    public int hashCode() {
//        return (groupingUsed ? 1231 : 1237) + (parseIntegerOnly ? 1231 : 1237)
//                + maximumFractionDigits + maximumIntegerDigits
//                + minimumFractionDigits + minimumIntegerDigits;/
//      }

    /**
     * Indicates whether this number format formats and parses numbers using a
     * grouping separator.
     *
     * @return {@code true} if a grouping separator is used; {@code false}
     *         otherwise.
     */
    virtual CARAPI IsGroupingUsed(
        /* [out] */ Boolean* isGroupingUsed);

    /**
     * Indicates whether this number format only parses integer numbers. Parsing
     * stops if a decimal separator is encountered.
     *
     * @return {@code true} if this number format only parses integers,
     *         {@code false} if if parsese integers as well as fractions.
     */
    virtual CARAPI IsParseIntegerOnly(
        /* [out] */ Boolean* isParseIntegerOnly);

    /**
     * Parses a {@code Number} from the specified string using the rules of this
     * number format.
     *
     * @param string
     *            the string to parse.
     * @return the {@code Number} resulting from the parsing.
     * @throws ParseException
     *            if an error occurs during parsing.
     */
    virtual CARAPI Parse(
        /* [in] */ String string,
        /* [out] */ INumber** value);

    /**
     * Parses a {@code Number} from the specified string starting at the index
     * specified by {@code position}. If the string is successfully parsed then
     * the index of the {@code ParsePosition} is updated to the index following
     * the parsed text. On error, the index is unchanged and the error index of
     * {@code ParsePosition} is set to the index where the error occurred.
     *
     * @param string
     *            the string to parse.
     * @param position
     *            input/output parameter, specifies the start index in
     *            {@code string} from where to start parsing. If parsing is
     *            successful, it is updated with the index following the parsed
     *            text; on error, the index is unchanged and the error index is
     *            set to the index where the error occurred.
     * @return the {@code Number} resulting from the parse or {@code null} if
     *         there is an error.
     */
    virtual CARAPI ParseEx(
        /* [in] */ String string, 
        /* [in] */ IParsePosition* position,
        /* [out] */ INumber** value) = 0;

    //@Override
    CARAPI ParseObjectEx(
        /* [in] */ String string,
        /* [in] */ IParsePosition* position,
        /* [out] */ IInterface** object);

    /**
     * Sets the currency used by this number format when formatting currency
     * values. The min and max fraction digits remain the same.
     * <p>
     * This implementation throws {@code UnsupportedOperationException},
     * concrete subclasses should override this method if they support currency
     * formatting.
     *
     * @param currency
     *            the new currency.
     * @throws UnsupportedOperationException
     */
//    virtual CARAPI SetCurrency(
//        /* [in] */ ICurrency* currency);

    /**
     * Sets whether this number format formats and parses numbers using a
     * grouping separator.
     *
     * @param value
     *            {@code true} if a grouping separator is used; {@code false}
     *            otherwise.
     */
    virtual CARAPI SetGroupingUsed(
        /* [in] */ Boolean value);

    /**
     * Sets the maximum number of fraction digits that are printed when
     * formatting. If the maximum is less than the number of fraction digits,
     * the least significant digits are truncated.
     *
     * @param value
     *            the maximum number of fraction digits.
     */
    virtual CARAPI SetMaximumFractionDigits(
        /* [in] */ Int32 value);

    /**
     * Sets the new maximum count of integer digits that are printed when
     * formatting. If the maximum is less than the number of integer digits, the
     * most significant digits are truncated.
     *
     * @param value
     *            the new maximum number of integer numerals for display.
     */
    virtual CARAPI SetMaximumIntegerDigits(
        /* [in] */ Int32 value);

    /**
     * Sets the minimum number of fraction digits that are printed when
     * formatting.
     *
     * @param value
     *            the minimum number of fraction digits.
     */
    virtual CARAPI SetMinimumFractionDigits(
        /* [in] */ Int32 value);

    /**
     * Sets the minimum number of integer digits that are printed when
     * formatting.
     *
     * @param value
     *            the minimum number of integer digits.
     */
    virtual CARAPI SetMinimumIntegerDigits(
        /* [in] */ Int32 value);

    /**
     * Specifies if this number format should parse numbers only as integers or
     * else as any kind of number. If this method is called with a {@code true}
     * value then subsequent parsing attempts will stop if a decimal separator
     * is encountered.
     *
     * @param value
     *            {@code true} to only parse integers, {@code false} to parse
     *            integers as well as fractions.
     */
    virtual CARAPI SetParseIntegerOnly(
        /* [in] */ Boolean value);

    /**
     * Returns the {@code RoundingMode} used by this {@code NumberFormat}. The default
     * implementation in {@code NumberFormat} throws {@code UnsupportedOperationException}.
     * Subclasses for which a rounding mode is meaningful are expected to override this method.
     * @since 1.6
     */
//    virtual CARAPI GetRoundingMode(
//        /* [out] */ IRoundingMode** roundingMode);

    /**
     * Sets the {@code RoundingMode} used by this {@code NumberFormat}. The default
     * implementation in {@code NumberFormat} throws {@code UnsupportedOperationException}.
     * Subclasses for which a rounding mode is meaningful are expected to override this method.
     * @since 1.6
     */
//    virtual CARAPI SetRoundingMode(
//        /* [in] */ IRoundingMode* roundingMode);

private:
    // BEGIN android-added
    static CARAPI GetInstance(
        /* [in] */ String pattern,
        /* [in] */ ILocale* locale,
        /* [out] */ INumberFormat** instance);
    // END android-added

//    private static final ObjectStreamField[] serialPersistentFields = {
//            new ObjectStreamField("groupingUsed", Boolean.TYPE),
//            new ObjectStreamField("maxFractionDigits", Byte.TYPE),
//            new ObjectStreamField("maximumFractionDigits", Integer.TYPE),
//            new ObjectStreamField("maximumIntegerDigits", Integer.TYPE),
//            new ObjectStreamField("maxIntegerDigits", Byte.TYPE),
//            new ObjectStreamField("minFractionDigits", Byte.TYPE),
//            new ObjectStreamField("minimumFractionDigits", Integer.TYPE),
//            new ObjectStreamField("minimumIntegerDigits", Integer.TYPE),
//            new ObjectStreamField("minIntegerDigits", Byte.TYPE),
//            new ObjectStreamField("parseIntegerOnly", Boolean.TYPE),
//            new ObjectStreamField("serialVersionOnStream", Integer.TYPE), };

//    private void writeObject(ObjectOutputStream stream) {
//        ObjectOutputStream.PutField fields = stream.putFields();
//        fields.put("groupingUsed", groupingUsed);
//        fields.put("maxFractionDigits",
//                        maximumFractionDigits < Byte.MAX_VALUE ? (byte) maximumFractionDigits
//                                : Byte.MAX_VALUE);
//        fields.put("maximumFractionDigits", maximumFractionDigits);
//        fields.put("maximumIntegerDigits", maximumIntegerDigits);
//        fields.put("maxIntegerDigits",
//                        maximumIntegerDigits < Byte.MAX_VALUE ? (byte) maximumIntegerDigits
//                                : Byte.MAX_VALUE);
//        fields.put("minFractionDigits",
//                        minimumFractionDigits < Byte.MAX_VALUE ? (byte) minimumFractionDigits
//                                : Byte.MAX_VALUE);
//        fields.put("minimumFractionDigits", minimumFractionDigits);
//        fields.put("minimumIntegerDigits", minimumIntegerDigits);
//        fields.put("minIntegerDigits",
//                minimumIntegerDigits < Byte.MAX_VALUE ? (byte) minimumIntegerDigits : Byte.MAX_VALUE);
//        fields.put("parseIntegerOnly", parseIntegerOnly);
//        fields.put("serialVersionOnStream", 1);
//        stream.writeFields();
//    }

//    private void readObject(ObjectInputStream stream) throws IOException,
//            ClassNotFoundException {
//        ObjectInputStream.GetField fields = stream.readFields();
//        groupingUsed = fields.get("groupingUsed", true);
//        parseIntegerOnly = fields.get("parseIntegerOnly", false);
//        if (fields.get("serialVersionOnStream", 0) == 0) {
//            maximumFractionDigits = fields.get("maxFractionDigits", (byte) 3);
//            maximumIntegerDigits = fields.get("maxIntegerDigits", (byte) 40);
//            minimumFractionDigits = fields.get("minFractionDigits", (byte) 0);
//            minimumIntegerDigits = fields.get("minIntegerDigits", (byte) 1);
//        } else {
//            maximumFractionDigits = fields.get("maximumFractionDigits", 3);
//            maximumIntegerDigits = fields.get("maximumIntegerDigits", 40);
//            minimumFractionDigits = fields.get("minimumFractionDigits", 0);
//            minimumIntegerDigits = fields.get("minimumIntegerDigits", 1);
//        }
//        if (minimumIntegerDigits > maximumIntegerDigits
//                || minimumFractionDigits > maximumFractionDigits) {
//            throw new InvalidObjectException("min digits greater than max digits");
//        }
//        if (minimumIntegerDigits < 0 || maximumIntegerDigits < 0
//                || minimumFractionDigits < 0 || maximumFractionDigits < 0) {
//            throw new InvalidObjectException("min or max digits negative");
//        }
//    }

    /**
     * The instances of this inner class are used as attribute keys and values
     * in {@code AttributedCharacterIterator} that the
     * {@link NumberFormat#formatToCharacterIterator(Object)} method returns.
     * <p>
     * There is no public constructor in this class, the only instances are the
     * constants defined here.
     * <p>
     */
    class NumberFormat_Field : public Format::Format_Field {
    protected:
        /**
         * Constructs a new instance of {@code NumberFormat.Field} with the
         * given field name.
         *
         * @param fieldName
         *            the field name.
         */
        NumberFormat_Field(
            /* [in] */ String fn);

        /**
         * Resolves instances that are deserialized to the constant
         * {@code NumberFormat.Field} values.
         *
         * @return the resolved field object.
         * @throws InvalidObjectException
         *             if an error occurs while resolving the field object.
         */
        //@Override
        CARAPI ReadResolve(
            /* [out] */ IInterface** resolvedObject);
    public:
        /**
         * This constant stands for the number sign.
         */
        //public static final Field SIGN = new Field("sign");
        const static AutoPtr<INumberFormat_Field> SIGN;

        /**
         * This constant stands for the integer part of the number.
         */
        //public static final Field INTEGER = new Field("integer");
        const static AutoPtr<INumberFormat_Field> INTEGER;

        /**
         * This constant stands for the fraction part of the number.
         */
        //public static final Field FRACTION = new Field("fraction");
        const static AutoPtr<INumberFormat_Field> FRACTION;

        /**
         * This constant stands for the exponent part of the number.
         */
        //public static final Field EXPONENT = new Field("exponent");
        const static AutoPtr<INumberFormat_Field> EXPONENT;

        /**
         * This constant stands for the exponent sign symbol.
         */
        //public static final Field EXPONENT_SIGN = new Field("exponent sign");
        const static AutoPtr<INumberFormat_Field> EXPONENT_SIGN;

        /**
         * This constant stands for the exponent symbol.
         */
        //public static final Field EXPONENT_SYMBOL = new Field("exponent symbol");
        const static AutoPtr<INumberFormat_Field> EXPONENT_SYMBOL;

        /**
         * This constant stands for the decimal separator.
         */
        //public static final Field DECIMAL_SEPARATOR = new Field("decimal separator");
        const static AutoPtr<INumberFormat_Field> DECIMAL_SEPARATOR;

        /**
         * This constant stands for the grouping separator.
         */
        //public static final Field GROUPING_SEPARATOR = new Field("grouping separator");
        const static AutoPtr<INumberFormat_Field> GROUPING_SEPARATOR;

        /**
         * This constant stands for the percent symbol.
         */
        //public static final Field PERCENT = new Field("percent");
        const static AutoPtr<INumberFormat_Field> PERCENT;

        /**
         * This constant stands for the permille symbol.
         */
        //public static final Field PERMILLE = new Field("per mille");
        const static AutoPtr<INumberFormat_Field> PERMILLE;

        /**
         * This constant stands for the currency symbol.
         */
        //public static final Field CURRENCY = new Field("currency");
        const static AutoPtr<INumberFormat_Field> CURRENCY;
    private:

        //const static Int64 serialVersionUID = 7494728892700160890L;
    };

private:
    //const static Int64 serialVersionUID = -2308460125733713944L;

    Boolean groupingUsed;
    Boolean parseIntegerOnly;

    Int32 maximumIntegerDigits;
    Int32 minimumIntegerDigits;
    Int32 maximumFractionDigits;
    Int32 minimumFractionDigits;

};
#endif //__NUMBERFORMAT_H__