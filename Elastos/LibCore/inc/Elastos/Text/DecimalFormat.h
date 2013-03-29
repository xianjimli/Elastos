#ifndef __DECIMALFORMAT__
#define __DECIMALFORMAT__


#include "Elastos.Text_server.h"
#include "NumberFormat.h"
//#include "LocaleData.h"
#include <elastos/AutoPtr.h>
// #include <StringBuffer.h>


using namespace Elastos;

class DecimalFormat : public NumberFormat
{
public:
    /**
     * Constructs a new {@code DecimalFormat} for formatting and parsing numbers
     * for the user's default locale.
     * See "<a href="../util/Locale.html#default_locale">Be wary of the default locale</a>".
     */
    DecimalFormat();

    /**
     * Constructs a new {@code DecimalFormat} using the specified non-localized
     * pattern and the {@code DecimalFormatSymbols} for the user's default Locale.
     * See "<a href="../util/Locale.html#default_locale">Be wary of the default locale</a>".
     * @param pattern
     *            the non-localized pattern.
     * @throws IllegalArgumentException
     *            if the pattern cannot be parsed.
     */
    DecimalFormat(
        /* [in] */ const String& pattern);

    /**
     * Constructs a new {@code DecimalFormat} using the specified non-localized
     * pattern and {@code DecimalFormatSymbols}.
     *
     * @param pattern
     *            the non-localized pattern.
     * @param value
     *            the DecimalFormatSymbols.
     * @throws IllegalArgumentException
     *            if the pattern cannot be parsed.
     */
    DecimalFormat(
        /* [in] */ const String& pattern,
        /* [in] */ IDecimalFormatSymbols* value);

    /**
     * Changes the pattern of this decimal format to the specified pattern which
     * uses localized pattern characters.
     *
     * @param pattern
     *            the localized pattern.
     * @throws IllegalArgumentException
     *            if the pattern cannot be parsed.
     */
    virtual CARAPI ApplyLocalizedPattern(
        /* [in] */ const String& pattern);

    /**
     * Changes the pattern of this decimal format to the specified pattern which
     * uses non-localized pattern characters.
     *
     * @param pattern
     *            the non-localized pattern.
     * @throws IllegalArgumentException
     *            if the pattern cannot be parsed.
     */
    virtual CARAPI ApplyPattern(
        /* [in] */ const String& pattern);

    /**
     * Returns a new instance of {@code DecimalFormat} with the same pattern and
     * properties as this decimal format.
     *
     * @return a shallow copy of this decimal format.
     * @see java.lang.Cloneable
     */
    //@Override
    //public Object clone() {
    //    DecimalFormat clone = (DecimalFormat) super.clone();
    //    clone.dform = (NativeDecimalFormat) dform.clone();
    //    clone.symbols = (DecimalFormatSymbols) symbols.clone();
    //    return clone;
    //}

    /**
     * Compares the specified object to this decimal format and indicates if
     * they are equal. In order to be equal, {@code object} must be an instance
     * of {@code DecimalFormat} with the same pattern and properties.
     *
     * @param object
     *            the object to compare with this object.
     * @return {@code true} if the specified object is equal to this decimal
     *         format; {@code false} otherwise.
     * @see #hashCode
     */
    //@Override
    //public boolean equals(Object object) {
    //    if (this == object) {
    //        return true;
    //    }
    //    if (!(object instanceof DecimalFormat)) {
    //        return false;
    //    }
    //    DecimalFormat other = (DecimalFormat) object;
    //    return (this.dform == null ? other.dform == null : this.dform.equals(other.dform)) &&
    //            getDecimalFormatSymbols().equals(other.getDecimalFormatSymbols());
    //}

    /**
     * Formats the specified object using the rules of this decimal format and
     * returns an {@code AttributedCharacterIterator} with the formatted number
     * and attributes.
     *
     * @param object
     *            the object to format.
     * @return an AttributedCharacterIterator with the formatted number and
     *         attributes.
     * @throws IllegalArgumentException
     *             if {@code object} cannot be formatted by this format.
     * @throws NullPointerException
     *             if {@code object} is {@code null}.
     */
    //@Override
    CARAPI FormatToCharacterIterator(
        /* [in] */ IInterface* object,
        /* [out] */ IAttributedCharacterIterator** characterIterator);

    //@Override
    CARAPI FormatDoubleEx(
        /* [in] */ Double value,
        /* [in] */ const String& buffer,
        /* [in] */ IFieldPosition* field,
        /* [out] */ String* result);

    //@Override
    CARAPI FormatInt64Ex(
        /* [in] */ Int64 value,
        /* [in] */ const String& buffer,
        /* [in] */ IFieldPosition* field,
        /* [out] */ String* result);

    //@Override
    CARAPI FormatObjectEx(
        /* [in] */ IInterface* object,
        /* [in] */ const String& buffer,
        /* [in] */ IFieldPosition* field,
        /* [out] */ String* value);

    /**
     * Returns the {@code DecimalFormatSymbols} used by this decimal format.
     *
     * @return a copy of the {@code DecimalFormatSymbols} used by this decimal
     *         format.
     */
    virtual CARAPI GetDecimalFormatSymbols(
        /* [out] */ IDecimalFormatSymbols** symbols);

    /**
     * Returns the currency used by this decimal format.
     *
     * @return the currency used by this decimal format.
     * @see DecimalFormatSymbols#getCurrency()
     */
    // @Override
    CARAPI GetCurrency(
        /* [out] */ ICurrency** currency);

    /**
     * Returns the number of digits grouped together by the grouping separator.
     * This only allows to get the primary grouping size. There is no API to get
     * the secondary grouping size.
     *
     * @return the number of digits grouped together.
     */
    virtual CARAPI GetGroupingSize(
        /* [out] */ Int32* size);

    /**
     * Returns the multiplier which is applied to the number before formatting
     * or after parsing.
     *
     * @return the multiplier.
     */
    virtual CARAPI GetMultiplier(
        /* [out] */ Int32* multiplier);

    /**
     * Returns the prefix which is formatted or parsed before a negative number.
     *
     * @return the negative prefix.
     */
    virtual CARAPI GetNegativePrefix(
        /* [out] */ String* prefix);

    /**
     * Returns the suffix which is formatted or parsed after a negative number.
     *
     * @return the negative suffix.
     */
    virtual CARAPI GetNegativeSuffix(
        /* [out] */ String* suffix);

    /**
     * Returns the prefix which is formatted or parsed before a positive number.
     *
     * @return the positive prefix.
     */
    virtual CARAPI GetPositivePrefix(
        /* [out] */ String* prefix);

    /**
     * Returns the suffix which is formatted or parsed after a positive number.
     *
     * @return the positive suffix.
     */
    virtual CARAPI GetPositiveSuffix(
        /* [out] */ String* suffix);

    //@Override
    //public int hashCode() {
    //    return dform.hashCode();
    //}

    /**
     * Indicates whether the decimal separator is shown when there are no
     * fractional digits.
     *
     * @return {@code true} if the decimal separator should always be formatted;
     *         {@code false} otherwise.
     */
    virtual CARAPI IsDecimalSeparatorAlwaysShown(
        /* [out] */ Boolean* alwaysShown);

    /**
     * This value indicates whether the return object of the parse operation is
     * of type {@code BigDecimal}. This value defaults to {@code false}.
     *
     * @return {@code true} if parse always returns {@code BigDecimals},
     *         {@code false} if the type of the result is {@code Long} or
     *         {@code Double}.
     */
    virtual CARAPI IsParseBigDecimal(
        /* [out] */ Boolean* isBigDecimal);

    /**
     * Sets the flag that indicates whether numbers will be parsed as integers.
     * When this decimal format is used for parsing and this value is set to
     * {@code true}, then the resulting numbers will be of type
     * {@code java.lang.Integer}. Special cases are NaN, positive and negative
     * infinity, which are still returned as {@code java.lang.Double}.
     *
     *
     * @param value
     *            {@code true} that the resulting numbers of parse operations
     *            will be of type {@code java.lang.Integer} except for the
     *            special cases described above.
     */
    //@Override
    CARAPI SetParseIntegerOnly(
        /* [in] */ Boolean value);

    /**
     * Indicates whether parsing with this decimal format will only
     * return numbers of type {@code java.lang.Integer}.
     *
     * @return {@code true} if this {@code DecimalFormat}'s parse method only
     *         returns {@code java.lang.Integer}; {@code false} otherwise.
     */
    //@Override
    CARAPI IsParseIntegerOnly(
        /* [out] */ Boolean* isParseIntegerOnly);

    /**
     * Parses a {@code Long} or {@code Double} from the specified string
     * starting at the index specified by {@code position}. If the string is
     * successfully parsed then the index of the {@code ParsePosition} is
     * updated to the index following the parsed text. On error, the index is
     * unchanged and the error index of {@code ParsePosition} is set to the
     * index where the error occurred.
     *
     * @param string
     *            the string to parse.
     * @param position
     *            input/output parameter, specifies the start index in
     *            {@code string} from where to start parsing. If parsing is
     *            successful, it is updated with the index following the parsed
     *            text; on error, the index is unchanged and the error index is
     *            set to the index where the error occurred.
     * @return a {@code Long} or {@code Double} resulting from the parse or
     *         {@code null} if there is an error. The result will be a
     *         {@code Long} if the parsed number is an integer in the range of a
     *         long, otherwise the result is a {@code Double}. If
     *         {@code isParseBigDecimal} is {@code true} then it returns the
     *         result as a {@code BigDecimal}.
     */
    //@Override
    CARAPI ParseEx(
        /* [in] */ const String& string,
        /* [in] */ IParsePosition* position,
        /* [out] */ INumber** value);

    /**
     * Sets the {@code DecimalFormatSymbols} used by this decimal format.
     *
     * @param value
     *            the {@code DecimalFormatSymbols} to set.
     */
    virtual CARAPI SetDecimalFormatSymbols(
        /* [in] */ IDecimalFormatSymbols* value);

    /**
     * Sets the currency used by this decimal format. The min and max fraction
     * digits remain the same.
     *
     * @param currency
     *            the currency this {@code DecimalFormat} should use.
     * @see DecimalFormatSymbols#setCurrency(Currency)
     */
    //@Override
    CARAPI SetCurrency(
        /* [in] */ ICurrency* currency);

    /**
     * Sets whether the decimal separator is shown when there are no fractional
     * digits.
     *
     * @param value
     *            {@code true} if the decimal separator should always be
     *            formatted; {@code false} otherwise.
     */
    virtual CARAPI SetDecimalSeparatorAlwaysShown(
        /* [in] */ Boolean value);

    /**
     * Sets the number of digits grouped together by the grouping separator.
     * This only allows to set the primary grouping size; the secondary grouping
     * size can only be set with a pattern.
     *
     * @param value
     *            the number of digits grouped together.
     */
    virtual CARAPI SetGroupingSize(
        /* [in] */ Int32 value);

    /**
     * Sets whether or not grouping will be used in this format. Grouping
     * affects both parsing and formatting.
     *
     * @param value
     *            {@code true} if grouping is used; {@code false} otherwise.
     */
    //@Override
    CARAPI SetGroupingUsed(
            /* [in] */ Boolean value);

    /**
     * Indicates whether grouping will be used in this format.
     *
     * @return {@code true} if grouping is used; {@code false} otherwise.
     */
    //@Override
    CARAPI IsGroupingUsed(
        /* [out] */ Boolean* isGroupingUsed);

    /**
     * Sets the maximum number of digits after the decimal point.
     * If the value passed is negative then it is replaced by 0.
     * Regardless of this setting, no more than 340 digits will be used.
     *
     * @param value the maximum number of fraction digits.
     */
    //@Override
    CARAPI SetMaximumFractionDigits(
        /* [in] */ Int32 value);

    /**
     * Sets the maximum number of digits before the decimal point.
     * If the value passed is negative then it is replaced by 0.
     * Regardless of this setting, no more than 309 digits will be used.
     *
     * @param value the maximum number of integer digits.
     */
    //@Override
    CARAPI SetMaximumIntegerDigits(
        /* [in] */ Int32 value);

    /**
     * Sets the minimum number of digits after the decimal point.
     * If the value passed is negative then it is replaced by 0.
     * Regardless of this setting, no more than 340 digits will be used.
     *
     * @param value the minimum number of fraction digits.
     */
    //@Override
    CARAPI SetMinimumFractionDigits(
        /* [in] */ Int32 value);

    /**
     * Sets the minimum number of digits before the decimal point.
     * If the value passed is negative then it is replaced by 0.
     * Regardless of this setting, no more than 309 digits will be used.
     *
     * @param value the minimum number of integer digits.
     */
    //@Override
    CARAPI SetMinimumIntegerDigits(
        /* [in] */ Int32 value);

    /**
     * Sets the multiplier which is applied to the number before formatting or
     * after parsing.
     *
     * @param value
     *            the multiplier.
     */
    virtual CARAPI SetMultiplier(
        /* [in] */ Int32 value);

    /**
     * Sets the prefix which is formatted or parsed before a negative number.
     *
     * @param value
     *            the negative prefix.
     */
    virtual CARAPI SetNegativePrefix(
        /* [in] */ const String& value);

    /**
     * Sets the suffix which is formatted or parsed after a negative number.
     *
     * @param value
     *            the negative suffix.
     */
    virtual CARAPI SetNegativeSuffix(
        /* [in] */ const String& value);

    /**
     * Sets the prefix which is formatted or parsed before a positive number.
     *
     * @param value
     *            the positive prefix.
     */
    virtual CARAPI SetPositivePrefix(
        /* [in] */ const String& value);

    /**
     * Sets the suffix which is formatted or parsed after a positive number.
     *
     * @param value
     *            the positive suffix.
     */
    virtual CARAPI SetPositiveSuffix(
        /* [in] */ const String& value);

    /**
     * Sets the behaviour of the parse method. If set to {@code true} then all
     * the returned objects will be of type {@code BigDecimal}.
     *
     * @param newValue
     *            {@code true} if all the returned objects should be of type
     *            {@code BigDecimal}; {@code false} otherwise.
     */
    virtual CARAPI SetParseBigDecimal(
        /* [in] */ Boolean newValue);

    /**
     * Returns the pattern of this decimal format using localized pattern
     * characters.
     *
     * @return the localized pattern.
     */
    virtual CARAPI ToLocalizedPattern(
        /* [out] */ String* pattern);

    /**
     * Returns the pattern of this decimal format using non-localized pattern
     * characters.
     *
     * @return the non-localized pattern.
     */
    virtual CARAPI ToPattern(
        /* [out] */ String* pattern);

    /**
     * Returns the {@code RoundingMode} used by this {@code NumberFormat}.
     * @since 1.6
     */
    //override
    CARAPI GetRoundingMode(
        /* [out] */ RoundingMode* mode);

    /**
     * Sets the {@code RoundingMode} used by this {@code NumberFormat}.
     * @since 1.6
     */
    //override
    CARAPI SetRoundingMode(
        /* [in] */ RoundingMode mode);

protected:
    /**
     * Constructs a new {@code DecimalFormat} for formatting and parsing numbers
     * for the user's default locale.
     * See "<a href="../util/Locale.html#default_locale">Be wary of the default locale</a>".
     */
    CARAPI Init();

    /**
     * Constructs a new {@code DecimalFormat} using the specified non-localized
     * pattern and the {@code DecimalFormatSymbols} for the user's default Locale.
     * See "<a href="../util/Locale.html#default_locale">Be wary of the default locale</a>".
     * @param pattern
     *            the non-localized pattern.
     * @throws IllegalArgumentException
     *            if the pattern cannot be parsed.
     */
    CARAPI Init(
        /* [in] */ const String& pattern);

    /**
     * Constructs a new {@code DecimalFormat} using the specified non-localized
     * pattern and {@code DecimalFormatSymbols}.
     *
     * @param pattern
     *            the non-localized pattern.
     * @param value
     *            the DecimalFormatSymbols.
     * @throws IllegalArgumentException
     *            if the pattern cannot be parsed.
     */
    CARAPI Init(
        /* [in] */ const String& pattern,
        /* [in] */ IDecimalFormatSymbols* value);

    // Used by NumberFormat.getInstance because cloning DecimalFormatSymbols is slow.
    CARAPI Init(
        /* [in] */ const String& pattern,
        /* [in] */ ILocale* locale);

private:
    CARAPI InitNative(
        /* [in] */ const String& pattern);

    CARAPI CheckBufferAndFieldPosition(
        /* [in] */ const String& buffer,
        /* [in] */ IFieldPosition* position);

        /**
     * Writes serialized fields following serialized forms specified by Java
     * specification.
     *
     * @param stream
     *            the output stream to write serialized bytes
     * @throws IOException
     *             if some I/O error occurs
     * @throws ClassNotFoundException
     */
/*
    @SuppressWarnings("nls")
    private void writeObject(ObjectOutputStream stream) throws IOException,
            ClassNotFoundException {
        ObjectOutputStream.PutField fields = stream.putFields();
        fields.put("positivePrefix", dform.getPositivePrefix());
        fields.put("positiveSuffix", dform.getPositiveSuffix());
        fields.put("negativePrefix", dform.getNegativePrefix());
        fields.put("negativeSuffix", dform.getNegativeSuffix());
        fields.put("posPrefixPattern", (String) null);
        fields.put("posSuffixPattern", (String) null);
        fields.put("negPrefixPattern", (String) null);
        fields.put("negSuffixPattern", (String) null);
        fields.put("multiplier", dform.getMultiplier());
        fields.put("groupingSize", (byte) dform.getGroupingSize());
        // BEGIN android-added
        fields.put("groupingUsed", dform.isGroupingUsed());
        // END android-added
        fields.put("decimalSeparatorAlwaysShown", dform
                .isDecimalSeparatorAlwaysShown());
        fields.put("parseBigDecimal", dform.isParseBigDecimal());
        fields.put("roundingMode", roundingMode);
        fields.put("symbols", symbols);
        fields.put("useExponentialNotation", false);
        fields.put("minExponentDigits", (byte) 0);
        fields.put("maximumIntegerDigits", dform.getMaximumIntegerDigits());
        fields.put("minimumIntegerDigits", dform.getMinimumIntegerDigits());
        fields.put("maximumFractionDigits", dform.getMaximumFractionDigits());
        fields.put("minimumFractionDigits", dform.getMinimumFractionDigits());
        fields.put("serialVersionOnStream", 4);
        stream.writeFields();
    }
*/

    /**
     * Reads serialized fields following serialized forms specified by Java
     * specification.
     *
     * @param stream
     *            the input stream to read serialized bytes
     * @throws IOException
     *             if some I/O error occurs
     * @throws ClassNotFoundException
     *             if some class of serialized objects or fields cannot be found
     */
/*
    @SuppressWarnings("nls")
    private void readObject(ObjectInputStream stream) throws IOException, ClassNotFoundException {
        // BEGIN android-changed
        ObjectInputStream.GetField fields = stream.readFields();
        this.symbols = (DecimalFormatSymbols) fields.get("symbols", null);

        initNative("");
        dform.setPositivePrefix((String) fields.get("positivePrefix", ""));
        dform.setPositiveSuffix((String) fields.get("positiveSuffix", ""));
        dform.setNegativePrefix((String) fields.get("negativePrefix", "-"));
        dform.setNegativeSuffix((String) fields.get("negativeSuffix", ""));
        dform.setMultiplier(fields.get("multiplier", 1));
        dform.setGroupingSize(fields.get("groupingSize", (byte) 3));
        dform.setGroupingUsed(fields.get("groupingUsed", true));
        dform.setDecimalSeparatorAlwaysShown(fields.get("decimalSeparatorAlwaysShown", false));

        setRoundingMode((RoundingMode) fields.get("roundingMode", RoundingMode.HALF_EVEN));

        final int maximumIntegerDigits = fields.get("maximumIntegerDigits", 309);
        final int minimumIntegerDigits = fields.get("minimumIntegerDigits", 309);
        final int maximumFractionDigits = fields.get("maximumFractionDigits", 340);
        final int minimumFractionDigits = fields.get("minimumFractionDigits", 340);
        // BEGIN android-changed: tell ICU what we want, then ask it what we can have, and then
        // set that in our Java object. This isn't RI-compatible, but then very little of our
        // behavior in this area is, and it's not obvious how we can second-guess ICU (or tell
        // it to just do exactly what we ask). We only need to do this with maximumIntegerDigits
        // because ICU doesn't seem to have its own ideas about the other options.
        dform.setMaximumIntegerDigits(maximumIntegerDigits);
        super.setMaximumIntegerDigits(dform.getMaximumIntegerDigits());

        setMinimumIntegerDigits(minimumIntegerDigits);
        setMinimumFractionDigits(minimumFractionDigits);
        setMaximumFractionDigits(maximumFractionDigits);
        setParseBigDecimal(fields.get("parseBigDecimal", false));

        if (fields.get("serialVersionOnStream", 0) < 3) {
            setMaximumIntegerDigits(super.getMaximumIntegerDigits());
            setMinimumIntegerDigits(super.getMinimumIntegerDigits());
            setMaximumFractionDigits(super.getMaximumFractionDigits());
            setMinimumFractionDigits(super.getMinimumFractionDigits());
        }
        // END android-changed
    }
*/

private:
    mutable AutoPtr<IDecimalFormatSymbols> mSymbols;

    mutable AutoPtr<INativeDecimalFormat> mDform;
    /*
    private final Object finalizerGuardian = new Object() {
        @Override protected void finalize() throws Throwable {
            try {
                dform.close();
            } finally {
                super.finalize();
            }
        }
    };
    */

    mutable RoundingMode mRoundingMode;

    static const Double NEGATIVE_ZERO_DOUBLE = -0.0;

        // the fields list to be serialized
/*
    private static final ObjectStreamField[] serialPersistentFields = {
            new ObjectStreamField("positivePrefix", String.class),
            new ObjectStreamField("positiveSuffix", String.class),
            new ObjectStreamField("negativePrefix", String.class),
            new ObjectStreamField("negativeSuffix", String.class),
            new ObjectStreamField("posPrefixPattern", String.class),
            new ObjectStreamField("posSuffixPattern", String.class),
            new ObjectStreamField("negPrefixPattern", String.class),
            new ObjectStreamField("negSuffixPattern", String.class),
            new ObjectStreamField("multiplier", int.class),
            new ObjectStreamField("groupingSize", byte.class),
            new ObjectStreamField("groupingUsed", boolean.class),
            new ObjectStreamField("decimalSeparatorAlwaysShown", boolean.class),
            new ObjectStreamField("parseBigDecimal", boolean.class),
            new ObjectStreamField("roundingMode", RoundingMode.class),
            new ObjectStreamField("symbols", DecimalFormatSymbols.class),
            new ObjectStreamField("useExponentialNotation", boolean.class),
            new ObjectStreamField("minExponentDigits", byte.class),
            new ObjectStreamField("maximumIntegerDigits", int.class),
            new ObjectStreamField("minimumIntegerDigits", int.class),
            new ObjectStreamField("maximumFractionDigits", int.class),
            new ObjectStreamField("minimumFractionDigits", int.class),
            new ObjectStreamField("serialVersionOnStream", int.class), };
*/
};

#endif //__DECIMALFORMAT__
