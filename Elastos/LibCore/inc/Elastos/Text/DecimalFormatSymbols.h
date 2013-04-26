#ifndef __DECIMALFORMATSYMBOLS_H__
#define __DECIMALFORMATSYMBOLS_H__

#include "Elastos.Text_server.h"
#include <elastos/AutoPtr.h>


class DecimalFormatSymbols
{
public:
    //@Override
    virtual CARAPI Clone(
        /* [out] */ IDecimalFormatSymbols** object);

    /**
     * Compares the specified object to this {@code DecimalFormatSymbols} and
     * indicates if they are equal. In order to be equal, {@code object} must be
     * an instance of {@code DecimalFormatSymbols} and contain the same symbols.
     *
     * @param object
     *            the object to compare with this object.
     * @return {@code true} if the specified object is equal to this
     *         {@code DecimalFormatSymbols}; {@code false} otherwise.
     * @see #hashCode
     */
    //@Override
    //public boolean equals(Object object) {
    //    if (this == object) {
    //        return true;
    //    }
    //    if (!(object instanceof DecimalFormatSymbols)) {
    //        return false;
    //    }
    //    DecimalFormatSymbols obj = (DecimalFormatSymbols) object;
    //    return currency.equals(obj.currency) &&
    //            currencySymbol.equals(obj.currencySymbol) &&
    //            decimalSeparator == obj.decimalSeparator &&
    //            digit == obj.digit &&
    //            exponentSeparator.equals(obj.exponentSeparator) &&
    //            groupingSeparator == obj.groupingSeparator &&
    //            infinity.equals(obj.infinity) &&
    //            intlCurrencySymbol.equals(obj.intlCurrencySymbol) &&
    //            minusSign == obj.minusSign &&
    //            monetarySeparator == obj.monetarySeparator &&
    //            NaN.equals(obj.NaN) &&
    //            patternSeparator == obj.patternSeparator &&
    //            perMill == obj.perMill &&
    //            percent == obj.percent &&
    //            zeroDigit == obj.zeroDigit;
    //}

    //@Override
    //public String toString() {
    //    return getClass().getName() +
    //            "[currency=" + currency +
    //            ",currencySymbol=" + currencySymbol +
    //            ",decimalSeparator=" + decimalSeparator +
    //            ",digit=" + digit +
    //            ",exponentSeparator=" + exponentSeparator +
    //            ",groupingSeparator=" + groupingSeparator +
    //            ",infinity=" + infinity +
    //            ",intlCurrencySymbol=" + intlCurrencySymbol +
    //            ",minusSign=" + minusSign +
    //            ",monetarySeparator=" + monetarySeparator +
    //            ",NaN=" + NaN +
    //            ",patternSeparator=" + patternSeparator +
    //            ",perMill=" + perMill +
    //            ",percent=" + percent +
    //            ",zeroDigit=" + zeroDigit +
    //            "]";
    //}

    /**
     * Returns the currency.
     * <p>
     * {@code null} is returned if {@code setInternationalCurrencySymbol()} has
     * been previously called with a value that is not a valid ISO 4217 currency
     * code.
     * <p>
     *
     * @return the currency that was set in the constructor or by calling
     *         {@code setCurrency()} or {@code setInternationalCurrencySymbol()},
     *         or {@code null} if an invalid currency was set.
     * @see #setCurrency(Currency)
     * @see #setInternationalCurrencySymbol(String)
     */
    virtual CARAPI GetCurrency(
        /* [out] */ ICurrency** currency);

    /**
     * Returns the international currency symbol.
     *
     * @return the international currency symbol as string.
     */
    virtual CARAPI GetInternationalCurrencySymbol(
        /* [out] */ String* symbol);

    /**
     * Returns the currency symbol.
     *
     * @return the currency symbol as string.
     */
    virtual CARAPI GetCurrencySymbol(
        /* [out] */ String* symbol);

    /**
     * Returns the character which represents the decimal point in a number.
     *
     * @return the decimal separator character.
     */
    virtual CARAPI GetDecimalSeparator(
        /* [out] */ Char32* separator);

    /**
     * Returns the character which represents a single digit in a format
     * pattern.
     *
     * @return the digit pattern character.
     */
    virtual CARAPI GetDigit(
        /* [out] */ Char32* digit);

    /**
     * Returns the character used as the thousands separator in a number.
     *
     * @return the thousands separator character.
     */
    virtual CARAPI GetGroupingSeparator(
        /* [out] */ Char32* separator);

    /**
     * Returns the string which represents infinity.
     *
     * @return the infinity symbol as a string.
     */
    virtual CARAPI GetInfinity(
        /* [out] */ String* infinity);

    /**
     * Returns the minus sign character.
     *
     * @return the minus sign as a character.
     */
    virtual CARAPI GetMinusSign(
        /* [out] */ Char32* minusSign);

    /**
     * Returns the character which represents the decimal point in a monetary
     * value.
     *
     * @return the monetary decimal point as a character.
     */
    virtual CARAPI GetMonetaryDecimalSeparator(
        /* [out] */ Char32* separator);

    /**
     * Returns the string which represents NaN.
     *
     * @return the symbol NaN as a string.
     */
    virtual CARAPI GetNaN(
            /* [out] */ String* nan);

    /**
     * Returns the character which separates the positive and negative patterns
     * in a format pattern.
     *
     * @return the pattern separator character.
     */
    virtual CARAPI GetPatternSeparator(
        /* [out] */ Char32* separator);

    /**
     * Returns the percent character.
     *
     * @return the percent character.
     */
    virtual CARAPI GetPercent(
            /* [out] */ Char32* percent);

    /**
     * Returns the per mill sign character.
     *
     * @return the per mill sign character.
     */
    virtual CARAPI GetPerMill(
        /* [out] */ Char32* perMill);

    /**
     * Returns the character which represents zero.
     *
     * @return the zero character.
     */
    virtual CARAPI GetZeroDigit(
        /* [out] */ Char32* zeroDigit);

    /*
     * Returns the string used to separate mantissa and exponent. Typically "E", as in "1.2E3".
     * @since 1.6
     */
    virtual CARAPI GetExponentSeparator(
        /* [out] */ String* separator);

/*
    @Override
    public int hashCode() {
        int result = 17;
        result = 31*result + zeroDigit;
        result = 31*result + digit;
        result = 31*result + decimalSeparator;
        result = 31*result + groupingSeparator;
        result = 31*result + patternSeparator;
        result = 31*result + percent;
        result = 31*result + perMill;
        result = 31*result + monetarySeparator;
        result = 31*result + minusSign;
        result = 31*result + exponentSeparator.hashCode();
        result = 31*result + infinity.hashCode();
        result = 31*result + NaN.hashCode();
        result = 31*result + currencySymbol.hashCode();
        result = 31*result + intlCurrencySymbol.hashCode();
        return result;
    }
*/

    /**
     * Sets the currency.
     * <p>
     * The international currency symbol and the currency symbol are updated,
     * but the min and max number of fraction digits stays the same.
     * <p>
     *
     * @param currency
     *            the new currency.
     * @throws NullPointerException
     *             if {@code currency} is {@code null}.
     */
    virtual CARAPI SetCurrency(
        /* [in] */ ICurrency* currency);

    /**
     * Sets the international currency symbol.
     * <p>
     * The currency and currency symbol are also updated if {@code value} is a
     * valid ISO4217 currency code.
     * <p>
     * The min and max number of fraction digits stay the same.
     *
     * @param value
     *            the currency code.
     */
    virtual CARAPI SetInternationalCurrencySymbol(
        /* [in] */ const String& value);

    /**
     * Sets the currency symbol.
     *
     * @param value
     *            the currency symbol.
     */
    virtual CARAPI SetCurrencySymbol(
        /* [in] */ const String& value);

    /**
     * Sets the character which represents the decimal point in a number.
     *
     * @param value
     *            the decimal separator character.
     */
    virtual CARAPI SetDecimalSeparator(
        /* [in] */ Char32 value);

    /**
     * Sets the character which represents a single digit in a format pattern.
     *
     * @param value
     *            the digit character.
     */
    virtual CARAPI SetDigit(
        /* [in] */ Char32 value);

    /**
     * Sets the character used as the thousands separator in a number.
     *
     * @param value
     *            the grouping separator character.
     */
    virtual CARAPI SetGroupingSeparator(
            /* [in] */ Char32 value);

    /**
     * Sets the string which represents infinity.
     *
     * @param value
     *            the string representing infinity.
     */
    virtual CARAPI SetInfinity(
        /* [in] */ const String& value);

    /**
     * Sets the minus sign character.
     *
     * @param value
     *            the minus sign character.
     */
    virtual CARAPI SetMinusSign(
        /* [in] */ Char32 value);

    /**
     * Sets the character which represents the decimal point in a monetary
     * value.
     *
     * @param value
     *            the monetary decimal separator character.
     */
    virtual CARAPI SetMonetaryDecimalSeparator(
        /* [in] */ Char32 value);

    /**
     * Sets the string which represents NaN.
     *
     * @param value
     *            the string representing NaN.
     */
    virtual CARAPI SetNaN(
        /* [in] */ const String& value);

    /**
     * Sets the character which separates the positive and negative patterns in
     * a format pattern.
     *
     * @param value
     *            the pattern separator character.
     */
    virtual CARAPI SetPatternSeparator(
        /* [in] */ Char32 value);

    /**
     * Sets the percent character.
     *
     * @param value
     *            the percent character.
     */
    virtual CARAPI SetPercent(
        /* [in] */ Char32 value);

    /**
     * Sets the per mill sign character.
     *
     * @param value
     *            the per mill character.
     */
    virtual CARAPI SetPerMill(
        /* [in] */ Char32 value);

    /**
     * Sets the character which represents zero.
     *
     * @param value
     *            the zero digit character.
     */
    virtual CARAPI SetZeroDigit(
        /* [in] */ Char32 value);

    /**
     * Sets the string used to separate mantissa and exponent. Typically "E", as in "1.2E3".
     * @since 1.6
     */
    virtual CARAPI SetExponentSeparator(
        /* [in] */ const String& value);

/*    private static ObjectStreamField[] serialPersistentFields = {
        new ObjectStreamField("currencySymbol", String.class),
        new ObjectStreamField("decimalSeparator", Character.TYPE),
        new ObjectStreamField("digit", Character.TYPE),
        new ObjectStreamField("exponential", Character.TYPE),
        new ObjectStreamField("exponentialSeparator", String.class),
        new ObjectStreamField("groupingSeparator", Character.TYPE),
        new ObjectStreamField("infinity", String.class),
        new ObjectStreamField("intlCurrencySymbol", String.class),
        new ObjectStreamField("minusSign", Character.TYPE),
        new ObjectStreamField("monetarySeparator", Character.TYPE),
        new ObjectStreamField("NaN", String.class),
        new ObjectStreamField("patternSeparator", Character.TYPE),
        new ObjectStreamField("percent", Character.TYPE),
        new ObjectStreamField("perMill", Character.TYPE),
        new ObjectStreamField("serialVersionOnStream", Integer.TYPE),
        new ObjectStreamField("zeroDigit", Character.TYPE),
        new ObjectStreamField("locale", Locale.class),
    };

    private void writeObject(ObjectOutputStream stream) throws IOException {
        ObjectOutputStream.PutField fields = stream.putFields();
        fields.put("currencySymbol", currencySymbol);
        fields.put("decimalSeparator", getDecimalSeparator());
        fields.put("digit", getDigit());
        fields.put("exponential", exponentSeparator.charAt(0));
        fields.put("exponentialSeparator", exponentSeparator);
        fields.put("groupingSeparator", getGroupingSeparator());
        fields.put("infinity", infinity);
        fields.put("intlCurrencySymbol", intlCurrencySymbol);
        fields.put("minusSign", getMinusSign());
        fields.put("monetarySeparator", getMonetaryDecimalSeparator());
        fields.put("NaN", NaN);
        fields.put("patternSeparator", getPatternSeparator());
        fields.put("percent", getPercent());
        fields.put("perMill", getPerMill());
        fields.put("serialVersionOnStream", 3);
        fields.put("zeroDigit", getZeroDigit());
        fields.put("locale", locale);
        stream.writeFields();
    }

    private void readObject(ObjectInputStream stream) throws IOException, ClassNotFoundException {
        ObjectInputStream.GetField fields = stream.readFields();
        final int serialVersionOnStream = fields.get("serialVersionOnStream", 0);
        currencySymbol = (String) fields.get("currencySymbol", "");
        setDecimalSeparator(fields.get("decimalSeparator", '.'));
        setDigit(fields.get("digit", '#'));
        setGroupingSeparator(fields.get("groupingSeparator", ','));
        infinity = (String) fields.get("infinity", "");
        intlCurrencySymbol = (String) fields.get("intlCurrencySymbol", "");
        setMinusSign(fields.get("minusSign", '-'));
        NaN = (String) fields.get("NaN", "");
        setPatternSeparator(fields.get("patternSeparator", ';'));
        setPercent(fields.get("percent", '%'));
        setPerMill(fields.get("perMill", '\u2030'));
        setZeroDigit(fields.get("zeroDigit", '0'));
        locale = (Locale) fields.get("locale", null);
        if (serialVersionOnStream == 0) {
            setMonetaryDecimalSeparator(getDecimalSeparator());
        } else {
            setMonetaryDecimalSeparator(fields.get("monetarySeparator", '.'));
        }

        if (serialVersionOnStream == 0) {
            // Prior to Java 1.1.6, the exponent separator wasn't configurable.
            exponentSeparator = "E";
        } else if (serialVersionOnStream < 3) {
            // In Javas 1.1.6 and 1.4, there was a character field "exponential".
            setExponentSeparator(String.valueOf(fields.get("exponential", 'E')));
        } else {
            // In Java 6, there's a new "exponentialSeparator" field.
            setExponentSeparator((String) fields.get("exponentialSeparator", "E"));
        }

        try {
            currency = Currency.getInstance(intlCurrencySymbol);
        } catch (IllegalArgumentException e) {
            currency = null;
        }
    }
*/

protected:
    /**
     * Constructs a new {@code DecimalFormatSymbols} containing the symbols for
     * the user's default locale.
     * See "<a href="../util/Locale.html#default_locale">Be wary of the default locale</a>".
     * Best practice is to create a {@code DecimalFormat}
     * and then to get the {@code DecimalFormatSymbols} from that object by
     * calling {@link DecimalFormat#getDecimalFormatSymbols()}.
     */
    CARAPI Init();

    /**
     * Constructs a new DecimalFormatSymbols containing the symbols for the
     * specified Locale.
     * See "<a href="../util/Locale.html#default_locale">Be wary of the default locale</a>".
     * Best practice is to create a {@code DecimalFormat}
     * and then to get the {@code DecimalFormatSymbols} from that object by
     * calling {@link DecimalFormat#getDecimalFormatSymbols()}.
     *
     * @param locale
     *            the locale.
     */
    CARAPI Init(
        /* [in] */ ILocale* locale);

private:
    Char32 mZeroDigit;
    Char32 mDigit;
    Char32 mDecimalSeparator;
    Char32 mGroupingSeparator;
    Char32 mPatternSeparator;
    Char32 mPercent;
    Char32 mPerMill;
    Char32 mMonetarySeparator;
    Char32 mMinusSign;
    String mInfinity, mNaN, mCurrencySymbol, mIntlCurrencySymbol;

    mutable AutoPtr<ICurrency> mCurrency;
    mutable AutoPtr<ILocale> mLocale;
    mutable String mExponentSeparator;
};

#endif //__DECIMALFORMATSYMBOLS_H__
