#ifndef __SIMPLEDATEFORMAT_H__
#define __SIMPLEDATEFORMAT_H__

#include "cmdef.h"

#include <elastos.h>
#include <elastos/AutoPtr.h>
#include <elastos/Vector.h>
#include <StringBuffer.h>

#include "Elastos.Text_server.h"
#include "DateFormat.h"
#include "CFieldPosition.h"
#include "Character.h"

using namespace Elastos;

extern "C" const InterfaceID EIID_SimpleDateFormat;

class SimpleDateFormat : public DateFormat {
public:
    CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);
    /**
     * Constructs a new {@code SimpleDateFormat} for formatting and parsing
     * dates and times in the {@code SHORT} style for the user's default locale.
     * See "<a href="../util/Locale.html#default_locale">Be wary of the default locale</a>".
     */
    CARAPI Init();

    virtual ~SimpleDateFormat();

    /**
     * Constructs a new {@code SimpleDateFormat} using the specified
     * non-localized pattern and the {@code DateFormatSymbols} and {@code
     * Calendar} for the user's default locale.
     * See "<a href="../util/Locale.html#default_locale">Be wary of the default locale</a>".
     *
     * @param pattern
     *            the pattern.
     * @throws NullPointerException
     *            if the pattern is {@code null}.
     * @throws IllegalArgumentException
     *            if {@code pattern} is not considered to be usable by this
     *            formatter.
     */
    CARAPI Init(
        /* [in] */ String pattern);

    /**
     * Constructs a new {@code SimpleDateFormat} using the specified
     * non-localized pattern and {@code DateFormatSymbols} and the {@code
     * Calendar} for the user's default locale.
     * See "<a href="../util/Locale.html#default_locale">Be wary of the default locale</a>".
     *
     * @param template
     *            the pattern.
     * @param value
     *            the DateFormatSymbols.
     * @throws NullPointerException
     *            if the pattern is {@code null}.
     * @throws IllegalArgumentException
     *            if the pattern is invalid.
     */
    CARAPI Init(
        /* [in] */ String tem,
        /* [in] */ IDateFormatSymbols* value);

    /**
     * Constructs a new {@code SimpleDateFormat} using the specified
     * non-localized pattern and the {@code DateFormatSymbols} and {@code
     * Calendar} for the specified locale.
     *
     * @param template
     *            the pattern.
     * @param locale
     *            the locale.
     * @throws NullPointerException
     *            if the pattern is {@code null}.
     * @throws IllegalArgumentException
     *            if the pattern is invalid.
     */
    CARAPI Init(
        /* [in] */ String tem, 
        /* [in] */ ILocale* locale);

    /**
     * Changes the pattern of this simple date format to the specified pattern
     * which uses localized pattern characters.
     *
     * @param template
     *            the localized pattern.
     */
    virtual CARAPI ApplyLocalizedPattern(
        /* [in] */ String tem);

    /**
     * Changes the pattern of this simple date format to the specified pattern
     * which uses non-localized pattern characters.
     *
     * @param template
     *            the non-localized pattern.
     * @throws NullPointerException
     *                if the pattern is {@code null}.
     * @throws IllegalArgumentException
     *                if the pattern is invalid.
     */
    virtual CARAPI ApplyPattern(
        /* [in] */ String tem);

    /**
     * Formats the specified object using the rules of this simple date format
     * and returns an {@code AttributedCharacterIterator} with the formatted
     * date and attributes.
     *
     * @param object
     *            the object to format.
     * @return an {@code AttributedCharacterIterator} with the formatted date
     *         and attributes.
     * @throws NullPointerException
     *            if the object is {@code null}.
     * @throws IllegalArgumentException
     *            if the object cannot be formatted by this simple date
     *            format.
     */
    //@Override
    CARAPI FormatToCharacterIterator(
        /* [in] */ IInterface* object,
        /* [out] */ IAttributedCharacterIterator** charactorIterator);

    /**
     * Formats the specified date as a string using the pattern of this date
     * format and appends the string to the specified string buffer.
     * <p>
     * If the {@code field} member of {@code field} contains a value specifying
     * a format field, then its {@code beginIndex} and {@code endIndex} members
     * will be updated with the position of the first occurrence of this field
     * in the formatted text.
     *
     * @param date
     *            the date to format.
     * @param buffer
     *            the target string buffer to append the formatted date/time to.
     * @param fieldPos
     *            on input: an optional alignment field; on output: the offsets
     *            of the alignment field in the formatted text.
     * @return the string buffer.
     * @throws IllegalArgumentException
     *             if there are invalid characters in the pattern.
     */
    //@Override
    CARAPI formatEx3(
        /* [in] */ IDate* date,
        /* [in] */ StringBuffer* buffer,
        /* [in] */ IFieldPosition* field,
        /* [out] */ StringBuffer* formatString);

    /**
     * Returns the date which is the start of the one hundred year period for two-digit year values.
     * See {@link #set2DigitYearStart} for details.
     */
    virtual CARAPI Get2DigitYearStart(
        /* [out] */ IDate** start);

    /**
     * Returns the {@code DateFormatSymbols} used by this simple date format.
     *
     * @return the {@code DateFormatSymbols} object.
     */
    virtual CARAPI GetDateFormatSymbols(
        /* [out] */ IDateFormatSymbols** symbols);

    /**
     * Parses a date from the specified string starting at the index specified
     * by {@code position}. If the string is successfully parsed then the index
     * of the {@code ParsePosition} is updated to the index following the parsed
     * text. On error, the index is unchanged and the error index of {@code
     * ParsePosition} is set to the index where the error occurred.
     *
     * @param string
     *            the string to parse using the pattern of this simple date
     *            format.
     * @param position
     *            input/output parameter, specifies the start index in {@code
     *            string} from where to start parsing. If parsing is successful,
     *            it is updated with the index following the parsed text; on
     *            error, the index is unchanged and the error index is set to
     *            the index where the error occurred.
     * @return the date resulting from the parse, or {@code null} if there is an
     *         error.
     * @throws IllegalArgumentException
     *             if there are invalid characters in the pattern.
     */
    //@Override
    CARAPI ParseEx(
        /* [in] */ String string,
        /* [in] */ IParsePosition* position,
        /* [out] */ IDate** date);

    /**
     * Sets the date which is the start of the one hundred year period for two-digit year values.
     *
     * <p>When parsing a date string using the abbreviated year pattern {@code yy}, {@code
     * SimpleDateFormat} must interpret the abbreviated year relative to some
     * century. It does this by adjusting dates to be within 80 years before and 20
     * years after the time the {@code SimpleDateFormat} instance was created. For
     * example, using a pattern of {@code MM/dd/yy}, an
     * instance created on Jan 1, 1997 would interpret the string {@code "01/11/12"}
     * as Jan 11, 2012 but interpret the string {@code "05/04/64"} as May 4, 1964.
     * During parsing, only strings consisting of exactly two digits, as
     * defined by {@link java.lang.Character#isDigit(char)}, will be parsed into the
     * default century. Any other numeric string, such as a one digit string, a
     * three or more digit string, or a two digit string that isn't all digits (for
     * example, {@code "-1"}), is interpreted literally. So using the same pattern, both
     * {@code "01/02/3"} and {@code "01/02/003"} are parsed as Jan 2, 3 AD.
     * Similarly, {@code "01/02/-3"} is parsed as Jan 2, 4 BC.
     *
     * <p>If the year pattern does not have exactly two 'y' characters, the year is
     * interpreted literally, regardless of the number of digits. So using the
     * pattern {@code MM/dd/yyyy}, {@code "01/11/12"} is parsed as Jan 11, 12 A.D.
     */
    virtual CARAPI Set2DigitYearStart(
        /* [in] */ IDate* date);

    /**
     * Sets the {@code DateFormatSymbols} used by this simple date format.
     *
     * @param value
     *            the new {@code DateFormatSymbols} object.
     */
    virtual CARAPI SetDateFormatSymbols(
        /* [in] */ IDateFormatSymbols* value);

    /**
     * Returns the pattern of this simple date format using localized pattern
     * characters.
     *
     * @return the localized pattern.
     */
    virtual CARAPI ToLocalizedPattern(
        /* [out] */ String* pattern);

    /**
     * Returns the pattern of this simple date format using non-localized
     * pattern characters.
     *
     * @return the non-localized pattern.
     */
    virtual CARAPI ToPattern(
        /* [out] */ String* pattern);
private:
    /**
     * Validates the format character.
     *
     * @param format
     *            the format character
     *
     * @throws IllegalArgumentException
     *             when the format character is invalid
     */
    CARAPI ValidateFormat(
        /* [in] */ Char32 format);

    /**
     * Validates the pattern.
     *
     * @param template
     *            the pattern to validate.
     *
     * @throws NullPointerException
     *             if the pattern is null
     * @throws IllegalArgumentException
     *             if the pattern is invalid
     */
    CARAPI ValidatePattern(
        /* [in] */ String tem);

    CARAPI Init(
        /* [in] */ ILocale* locale);

    static CARAPI DefaultPattern(
        /* [out] */ String* pattern);

    CARAPI FormatToCharacterIteratorImpl(
        /* [in] */ IDate* date,
        /* [out] */ IAttributedCharacterIterator** charactorIterator);

    /**
     * Formats the date.
     * <p>
     * If the FieldPosition {@code field} is not null, and the field
     * specified by this FieldPosition is formatted, set the begin and end index
     * of the formatted field in the FieldPosition.
     * <p>
     * If the Vector {@code fields} is not null, find fields of this
     * date, set FieldPositions with these fields, and add them to the fields
     * vector.
     *
     * @param date
     *            Date to Format
     * @param buffer
     *            StringBuffer to store the resulting formatted String
     * @param field
     *            FieldPosition to set begin and end index of the field
     *            specified, if it is part of the format for this date
     * @param fields
     *            Vector used to store the FieldPositions for each field in this
     *            date
     * @return the formatted Date
     * @throws IllegalArgumentException
     *            if the object cannot be formatted by this Format.
     */
    CARAPI FormatImpl(
        /* [in] */ IDate* date,
        /* [in] */ StringBuffer* buffer,
        /* [in] */ IFieldPosition* field,
        /* [in] */ Vector< IFieldPosition* >* fields,
        /* [out] */ StringBuffer* formattedDate);

    CARAPI Append(
        /* [in] */ StringBuffer* buffer,
        /* [in] */ IFieldPosition* position,
        /* [in] */ Vector< IFieldPosition* >* fields,
        /* [in] */ Char32 format,
        /* [in] */ Int32 count);

    CARAPI AppendDayOfWeek(
        /* [in] */ StringBuffer* buffer,
        /* [in] */ Int32 count,
        /* [in] */ ArrayOf<String>* longs,
        /* [in] */ ArrayOf<String>* shorts);

    CARAPI AppendMonth(
        /* [in] */ StringBuffer* buffer,
        /* [in] */ Int32 count,
        /* [in] */ ArrayOf<String>* longs,
        /* [in] */ ArrayOf<String>* shorts);

    /**
     * Append a representation of the time zone of 'calendar' to 'buffer'.
     *
     * @param count the number of z or Z characters in the format string; "zzz" would be 3,
     * for example.
     * @param generalTimeZone true if we should use a display name ("PDT") if available;
     * false implies that we should use RFC 822 format ("-0800") instead. This corresponds to 'z'
     * versus 'Z' in the format string.
     */
    CARAPI AppendTimeZone(
        /* [in] */ StringBuffer* buffer,
        /* [in] */ Int32 count,
        /* [in] */ Boolean generalTimeZone);

    /**
     * @param generalTimeZone "GMT-08:00" rather than "-0800".
     */
    CARAPI AppendNumericTimeZone(
        /* [in] */ StringBuffer* buffer,
        /* [in] */ Boolean generalTimeZone);

    CARAPI AppendNumber(
        /* [in] */ StringBuffer* buffer,
        /* [in] */ Int32 count,
        /* [in] */ Int32 value);

    CARAPI Error(
        /* [in] */ IParsePosition* position,
        /* [in] */ Int32 offset,
        /* [in] */ ITimeZone* zone,
        /* [out] */ IDate** date);

    CARAPI Parse(
        /* [in] */ String string, 
        /* [in] */ Int32 offset, 
        /* [in] */ Char32 format, 
        /* [in] */ Int32 count,
        /* [out] */ Int32* value);

    CARAPI ParseDayOfWeek(
        /* [in] */ String string, 
        /* [in] */ Int32 offset, 
        /* [in] */ ArrayOf<String>* longs, 
        /* [in] */ ArrayOf<String>* shorts,
        /* [out] */ Int32* value);

    CARAPI ParseMonth(
        /* [in] */ String string, 
        /* [in] */ Int32 offset,  
        /* [in] */ Int32 count, 
        /* [in] */ Int32 absolute, 
        /* [in] */ ArrayOf<String>* longs, 
        /* [in] */ ArrayOf<String>* shorts,
        /* [out] */ Int32* value);

    CARAPI ParseNumber(
        /* [in] */ Int32 max, 
        /* [in] */ String string, 
        /* [in] */ IParsePosition* position,
        /* [out] */ INumber** number);

    CARAPI ParseNumber(
        /* [in] */ Int32 max, 
        /* [in] */ String string, 
        /* [in] */ Int32 offset, 
        /* [in] */ Int32 field, 
        /* [in] */ Int32 skew,
        /* [out] */ Int32* value);

    CARAPI ParseText(
        /* [in] */ String string, 
        /* [in] */ Int32 offset, 
        /* [in] */ ArrayOf<String>* text, 
        /* [in] */ Int32 field, 
        /* [out] */ Int32* value);

    CARAPI ParseTimeZone(
        /* [in] */ String string, 
        /* [in] */ Int32 offset, 
        /* [out] */ Int32* value);

    static CARAPI ConvertPattern(
        /* [in] */ String tem, 
        /* [in] */ String fromChars, 
        /* [in] */ String toChars, 
        /* [in] */ Boolean check,
        /* [out] */ String* pattern);

    //CARAPI WriteObject(ObjectOutputStream stream);

    //CARAPI ReadObject(ObjectInputStream stream);

protected:
    // 'L' and 'c' are ICU-compatible extensions for stand-alone month and stand-alone weekday.
    const static String PATTERN_CHARS;

private:
    //const static Int64 serialVersionUID = 4774881970558875024L;

    // The index of 'Z' in the PATTERN_CHARS string. This pattern character is supported by the RI,
    // but has no corresponding public constant.
    const static Int32 RFC_822_TIMEZONE_FIELD = 18;

    // The index of 'L' (cf. 'M') in the PATTERN_CHARS string. This is an ICU-compatible extension
    // necessary for correct localization in various languages (http://b/2633414).
    const static Int32 STAND_ALONE_MONTH_FIELD = 19;
    // The index of 'c' (cf. 'E') in the PATTERN_CHARS string. This is an ICU-compatible extension
    // necessary for correct localization in various languages (http://b/2633414).
    const static Int32 STAND_ALONE_DAY_OF_WEEK_FIELD = 20;

    String pattern;

    AutoPtr<IDateFormatSymbols> formatData;

    mutable Int32 creationYear;

    AutoPtr<IDate> defaultCenturyStart;

//    private static final ObjectStreamField[] serialPersistentFields = {
//            new ObjectStreamField("defaultCenturyStart", Date.class),
//            new ObjectStreamField("formatData", DateFormatSymbols.class),
//            new ObjectStreamField("pattern", String.class),
//            new ObjectStreamField("serialVersionOnStream", Integer.TYPE), };
};
#endif //__SIMPLEDATEFORMAT_H__