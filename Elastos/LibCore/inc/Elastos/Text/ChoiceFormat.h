#ifndef __CHOICEFORMAT_H__
#define __CHOICEFORMAT_H__

#include "cmdef.h"
#include <elastos.h>
#include <elastos/List.h>
#include <elastos/AutoPtr.h>
#include <StringBuffer.h>
#include <Character.h>
#include "Elastos.Text_server.h"
#include "NumberFormat.h"
#include "CParsePosition.h"

class ChoiceFormat : public NumberFormat
{
public:
    virtual ~ChoiceFormat();

    /**
     * Constructs a new {@code ChoiceFormat} with the specified double values
     * and associated strings. When calling
     * {@link #format(double, StringBuffer, FieldPosition) format} with a double
     * value {@code d}, then the element {@code i} in {@code formats} is
     * selected where {@code i} fulfills {@code limits[i] <= d < limits[i+1]}.
     * <p>
     * The length of the {@code limits} and {@code formats} arrays must be the
     * same.
     *
     * @param limits
     *            an array of doubles in ascending order. The lowest and highest
     *            possible values are negative and positive infinity.
     * @param formats
     *            the strings associated with the ranges defined through {@code
     *            limits}. The lower bound of the associated range is at the
     *            same index as the string.
     */
    CARAPI Init(
        /* [in] */ const ArrayOf<Double>& limits,
        /* [in] */ const ArrayOf<String>& formats);

    /**
     * Constructs a new {@code ChoiceFormat} with the strings and limits parsed
     * from the specified pattern.
     *
     * @param template
     *            the pattern of strings and ranges.
     * @throws IllegalArgumentException
     *            if an error occurs while parsing the pattern.
     */
    CARAPI Init(
        /* [in] */ String tem);

    /**
     * Parses the pattern to determine new strings and ranges for this
     * {@code ChoiceFormat}.
     *
     * @param template
     *            the pattern of strings and ranges.
     * @throws IllegalArgumentException
     *            if an error occurs while parsing the pattern.
     */
    virtual CARAPI ApplyPattern(
        /* [in] */ String tem);

    /**
     * Appends the string associated with the range in which the specified
     * double value fits to the specified string buffer.
     *
     * @param value
     *            the double to format.
     * @param buffer
     *            the target string buffer to append the formatted value to.
     * @param field
     *            a {@code FieldPosition} which is ignored.
     * @return the string buffer.
     */
    //@Override
    CARAPI FormatDoubleEx(
        /* [in] */ Double value,
        /* [in] */ const String& buffer,
        /* [in] */ IFieldPosition* field,
        /* [out] */ String* result);

    /**
     * Appends the string associated with the range in which the specified long
     * value fits to the specified string buffer.
     *
     * @param value
     *            the long to format.
     * @param buffer
     *            the target string buffer to append the formatted value to.
     * @param field
     *            a {@code FieldPosition} which is ignored.
     * @return the string buffer.
     */
    //@Override
    CARAPI FormatInt64Ex(
        /* [in] */ Int64 value,
        /* [in] */ const String& buffer,
        /* [in] */ IFieldPosition* field,
        /* [out] */ String* formattedString);

    /**
     * Returns the strings associated with the ranges of this {@code
     * ChoiceFormat}.
     *
     * @return an array of format strings.
     */
    virtual CARAPI GetFormats(
        /* [out, callee] */ ArrayOf<IInterface*>** arrayOfFormattedString);

    /**
     * Returns the limits of this {@code ChoiceFormat}.
     *
     * @return the array of doubles which make up the limits of this {@code
     *         ChoiceFormat}.
     */
    virtual CARAPI GetLimits(
        /* [out, callee] */ ArrayOf<Double>** arrayOfDoubles);

    /**
     * Returns the double value which is closest to the specified double but
     * larger.
     *
     * @param value
     *            a double value.
     * @return the next larger double value.
     */
    static CARAPI_(Double) NextDouble(
        /* [in] */ Double value);

    /**
     * Returns the double value which is closest to the specified double but
     * either larger or smaller as specified.
     *
     * @param value
     *            a double value.
     * @param increment
     *            {@code true} to get the next larger value, {@code false} to
     *            get the previous smaller value.
     * @return the next larger or smaller double value.
     */
    static CARAPI_(Double) NextDouble(
        /* [in] */ Double value,
        /* [in] */ Boolean increment);

    /**
     * Parses a double from the specified string starting at the index specified
     * by {@code position}. The string is compared to the strings of this
     * {@code ChoiceFormat} and if a match occurs then the lower bound of the
     * corresponding range in the limits array is returned. If the string is
     * successfully parsed then the index of the {@code ParsePosition} passed to
     * this method is updated to the index following the parsed text.
     * <p>
     * If one of the format strings of this {@code ChoiceFormat} instance is
     * found in {@code string} starting at {@code position.getIndex()} then
     * <ul>
     * <li>the index in {@code position} is set to the index following the
     * parsed text;
     * <li>the {@link java.lang.Double Double} corresponding to the format
     * string is returned.</li>
     * </ul>
     * <p>
     * If none of the format strings is found in {@code string} then
     * <ul>
     * <li>the error index in {@code position} is set to the current index in
     * {@code position};</li>
     * <li> {@link java.lang.Double#NaN Double.NaN} is returned.
     * </ul>
     * @param string
     *            the source string to parse.
     * @param position
     *            input/output parameter, specifies the start index in {@code
     *            string} from where to start parsing. See the <em>Returns</em>
     *            section for a description of the output values.
     * @return a Double resulting from the parse, or Double.NaN if there is an
     *         error
     */
    //@Override
    CARAPI ParseEx(
        /* [in] */ String string,
        /* [in] */ IParsePosition* position,
        /* [out] */ INumber** value);

    /**
     * Returns the double value which is closest to the specified double but
     * smaller.
     *
     * @param value
     *            a double value.
     * @return the next smaller double value.
     */
    static CARAPI_(Double) PreviousDouble(
        /* [in] */ Double value);

    /**
     * Sets the double values and associated strings of this ChoiceFormat. When
     * calling {@link #format(double, StringBuffer, FieldPosition) format} with
     * a double value {@code d}, then the element {@code i} in {@code formats}
     * is selected where {@code i} fulfills
     * {@code limits[i] <= d < limits[i+1]}.
     * <p>
     * The length of the {@code limits} and {@code formats} arrays must be the
     * same.
     *
     * @param limits
     *            an array of doubles in ascending order. The lowest and highest
     *            possible values are negative and positive infinity.
     * @param formats
     *            the strings associated with the ranges defined through {@code
     *            limits}. The lower bound of the associated range is at the
     *            same index as the string.
     */
    virtual CARAPI SetChoices(
        /* [in] */ const ArrayOf<Double>& limits,
        /* [in] */ const ArrayOf<String>& formats);

    /**
     * Returns the pattern of this {@code ChoiceFormat} which specifies the
     * ranges and their associated strings.
     *
     * @return the pattern.
     */
    virtual CARAPI ToPattern(
        /* [out] */ String* pattern);

private:
    CARAPI_(Int32) SkipWhitespace(
        /* [in] */ String string,
        /* [in] */ Int32 index);

private:
    ArrayOf<Double>* mChoiceLimits;

    ArrayOf<String>* mChoiceFormats;

};
#endif // __CHOICEFORMAT_H__