
#ifndef __DATEFORMAT_H__
#define __DATEFORMAT_H__

#ifndef __USE_MALLOC
#define __USE_MALLOC
#endif

#include "Elastos.Text_server.h"
#include "Format.h"
#include <elastos/AutoPtr.h>
#include <elastos/HashMap.h>
#include <elastos/ElRefBase.h>

class DateFormat : public Format
{
public:
    /**
     * The instances of this inner class are used as attribute keys and values
     * in {@code AttributedCharacterIterator} that the
     * {@link SimpleDateFormat#formatToCharacterIterator(Object)} method returns.
     * <p>
     * There is no public constructor in this class, the only instances are the
     * constants defined here.
     */
    class Field
        : public ElRefBase
        , public IDateFormatField
        , public Format::Field
    {
    public:
        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI GetClassID(
            /* [out] */ ClassID* clsid);

        Field();

        /**
         * Constructs a new instance of {@code DateFormat.Field} with the given
         * fieldName and calendar field.
         *
         * @param fieldName
         *            the field name.
         * @param calendarField
         *            the calendar field type of the field.
         */
        CARAPI Init(
            /* [in] */ const String& fieldName,
            /* [in] */ Int32 calendarField);

        CARAPI GetName(
            /* [out] */ String* name);

        /**
         * Returns the Calendar field that this field represents.
         *
         * @return the calendar field.
         */
        CARAPI GetCalendarField(
            /* [out] */ Int32* field);

        /**
         * Returns the {@code DateFormat.Field} instance for the given calendar
         * field.
         *
         * @param calendarField
         *            a calendar field constant.
         * @return the {@code DateFormat.Field} corresponding to
         *         {@code calendarField}.
         * @throws IllegalArgumentException
         *             if {@code calendarField} is negative or greater than the
         *             field count of {@code Calendar}.
         */
        static CARAPI OfCalendarField(
            /* [in] */ Int32 calendarField,
            /* [out] */ IDateFormatField** field);

    protected:
        /**
         * Resolves instances that are deserialized to the constant
         * {@code DateFormat.Field} values.
         *
         * @return the resolved field object.
         * @throws InvalidObjectException
         *             if an error occurs while resolving the field object.
         */
        //@Override
        CARAPI ReadResolve(
            /* [out] */ IInterface** resolvedField);

    public:
        /**
         * Marks the era part of a date.
         */
        //public final static Field ERA = new Field("era", Calendar.ERA);
        const static AutoPtr<IDateFormatField> ERA;

        /**
         * Marks the year part of a date.
         */
        //public final static Field YEAR = new Field("year", Calendar.YEAR);
        const static AutoPtr<IDateFormatField> YEAR;

        /**
         * Marks the month part of a date.
         */
        //public final static Field MONTH = new Field("month", Calendar.MONTH);
        const static AutoPtr<IDateFormatField> MONTH;

        /**
         * Marks the hour of the day part of a date (0-11).
         */
        //public final static Field HOUR_OF_DAY0 = new Field("hour of day", Calendar.HOUR_OF_DAY);
        const static AutoPtr<IDateFormatField> HOUR_OF_DAY0;

        /**
         * Marks the hour of the day part of a date (1-12).
         */
        //public final static Field HOUR_OF_DAY1 = new Field("hour of day 1", -1);
        const static AutoPtr<IDateFormatField> HOUR_OF_DAY1;

        /**
         * Marks the minute part of a time.
         */
        //public final static Field MINUTE = new Field("minute", Calendar.MINUTE);
        const static AutoPtr<IDateFormatField> MINUTE;

        /**
         * Marks the second part of a time.
         */
        //public final static Field SECOND = new Field("second", Calendar.SECOND);
        const static AutoPtr<IDateFormatField> SECOND;

        /**
         * Marks the millisecond part of a time.
         */
        //public final static Field MILLISECOND = new Field("millisecond", Calendar.MILLISECOND);
        const static AutoPtr<IDateFormatField> MILLISECOND;

        /**
         * Marks the day of the week part of a date.
         */
        //public final static Field DAY_OF_WEEK = new Field("day of week", Calendar.DAY_OF_WEEK);
        const static AutoPtr<IDateFormatField> DAY_OF_WEEK;

        /**
         * Marks the day of the month part of a date.
         */
        //public final static Field DAY_OF_MONTH = new Field("day of month", Calendar.DAY_OF_MONTH);
        const static AutoPtr<IDateFormatField> DAY_OF_MONTH;

        /**
         * Marks the day of the year part of a date.
         */
        //public final static Field DAY_OF_YEAR = new Field("day of year", Calendar.DAY_OF_YEAR);
        const static AutoPtr<IDateFormatField> DAY_OF_YEAR;

        /**
         * Marks the day of the week in the month part of a date.
         */
        //public final static Field DAY_OF_WEEK_IN_MONTH = new Field("day of week in month",
        //        Calendar.DAY_OF_WEEK_IN_MONTH);
        const static AutoPtr<IDateFormatField> DAY_OF_WEEK_IN_MONTH;

        /**
         * Marks the week of the year part of a date.
         */
        //public final static Field WEEK_OF_YEAR = new Field("week of year",
        //        Calendar.WEEK_OF_YEAR);
        const static AutoPtr<IDateFormatField> WEEK_OF_YEAR;

        /**
         * Marks the week of the month part of a date.
         */
        //public final static Field WEEK_OF_MONTH = new Field("week of month",
        //        Calendar.WEEK_OF_MONTH);
        const static AutoPtr<IDateFormatField> WEEK_OF_MONTH;

        /**
         * Marks the time indicator part of a date.
         */
        //public final static Field AM_PM = new Field("am pm", Calendar.AM_PM);
        const static AutoPtr<IDateFormatField> AM_PM;

        /**
         * Marks the hour part of a date (0-11).
         */
        //public final static Field HOUR0 = new Field("hour", Calendar.HOUR);
        const static AutoPtr<IDateFormatField> HOUR0;

        /**
         * Marks the hour part of a date (1-12).
         */
        ///public final static Field HOUR1 = new Field("hour 1", -1);
        const static AutoPtr<IDateFormatField> HOUR1;

        /**
         * Marks the time zone part of a date.
         */
        //public final static Field TIME_ZONE = new Field("time zone", -1);
        const static AutoPtr<IDateFormatField> TIME_ZONE;

    private:
        static HashMap<Int32, AutoPtr<IDateFormatField> > sTable;

        /**
         * The calendar field that this field represents.
         */
        Int32 mCalendarField;
    };

public:
    /**
     * Formats the specified object as a string using the pattern of this date
     * format and appends the string to the specified string buffer.
     * <p>
     * If the {@code field} member of {@code field} contains a value specifying
     * a format field, then its {@code beginIndex} and {@code endIndex} members
     * will be updated with the position of the first occurrence of this field
     * in the formatted text.
     *
     * @param object
     *            the source object to format, must be a {@code Date} or a
     *            {@code Number}. If {@code object} is a number then a date is
     *            constructed using the {@code longValue()} of the number.
     * @param buffer
     *            the target string buffer to append the formatted date/time to.
     * @param field
     *            on input: an optional alignment field; on output: the offsets
     *            of the alignment field in the formatted text.
     * @return the string buffer.
     * @throws IllegalArgumentException
     *            if {@code object} is neither a {@code Date} nor a
     *            {@code Number} instance.
     */
    //@Override
    CARAPI FormatObjectEx(
        /* [in] */ IInterface* object,
        /* [in] */ const String& buffer,
        /* [in] */ IFieldPosition* field,
        /* [out] */ String* value);

    /**
     * Formats the specified date using the rules of this date format.
     *
     * @param date
     *            the date to format.
     * @return the formatted string.
     */
    CARAPI FormatDate(
        /* [in] */ IDate* date,
        /* [out] */ String* result);

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
     * @param field
     *            on input: an optional alignment field; on output: the offsets
     *            of the alignment field in the formatted text.
     * @return the string buffer.
     */
    virtual CARAPI FormatDateEx(
        /* [in] */ IDate* date,
        /* [in] */ const String& buffer,
        /* [in] */ IFieldPosition* field,
        /* [out] */ String* result) = 0;

    /**
     * Returns the calendar used by this {@code DateFormat}.
     *
     * @return the calendar used by this date format.
     */
    virtual CARAPI GetCalendar(
        /* [out] */ ICalendar** calendar);

    /**
     * Returns the {@code NumberFormat} used by this {@code DateFormat}.
     *
     * @return the {@code NumberFormat} used by this date format.
     */
    virtual CARAPI GetNumberFormat(
        /* [out] */ INumberFormat** numberFormat);

    /**
     * Returns the time zone of this date format's calendar.
     *
     * @return the time zone of the calendar used by this date format.
     */
    virtual CARAPI GetTimeZone(
        /* [out] */ ITimeZone** timeZone);

    //@Override
    //public int hashCode() {
    //    return calendar.getFirstDayOfWeek()
    //            + calendar.getMinimalDaysInFirstWeek()
    //            + calendar.getTimeZone().hashCode()
    //            + (calendar.isLenient() ? 1231 : 1237)
    //            + numberFormat.hashCode();
    //}

    /**
     * Indicates whether the calendar used by this date format is lenient.
     *
     * @return {@code true} if the calendar is lenient; {@code false} otherwise.
     */
    virtual CARAPI IsLenient(
        /* [out] */ Boolean* isLenient);

    /**
     * Parses a date from the specified string using the rules of this date
     * format.
     *
     * @param string
     *            the string to parse.
     * @return the {@code Date} resulting from the parsing.
     * @throws ParseException
     *         if an error occurs during parsing.
     */
    virtual CARAPI Parse(
        /* [in] */ const String& string,
        /* [out] */ IDate** date);

    /**
     * Parses a date from the specified string starting at the index specified
     * by {@code position}. If the string is successfully parsed then the index
     * of the {@code ParsePosition} is updated to the index following the parsed
     * text. On error, the index is unchanged and the error index of {@code
     * ParsePosition} is set to the index where the error occurred.
     * <p>
     * By default, parsing is lenient: If the input is not in the form used by
     * this object's format method but can still be parsed as a date, then the
     * parse succeeds. Clients may insist on strict adherence to the format by
     * calling {@code setLenient(false)}.
     *
     * @param string
     *            the string to parse.
     * @param position
     *            input/output parameter, specifies the start index in {@code
     *            string} from where to start parsing. If parsing is successful,
     *            it is updated with the index following the parsed text; on
     *            error, the index is unchanged and the error index is set to
     *            the index where the error occurred.
     * @return the date resulting from the parse, or {@code null} if there is an
     *         error.
     */
    virtual CARAPI ParseEx(
        /* [in] */ const String& string,
        /* [in] */ IParsePosition* position,
        /* [out] */ IDate** date) = 0;

    /**
     * Parses a date from the specified string starting at the index specified
     * by {@code position}. If the string is successfully parsed then the index
     * of the {@code ParsePosition} is updated to the index following the parsed
     * text. On error, the index is unchanged and the error index of
     * {@code ParsePosition} is set to the index where the error occurred.
     * <p>
     * By default, parsing is lenient: If the input is not in the form used by
     * this object's format method but can still be parsed as a date, then the
     * parse succeeds. Clients may insist on strict adherence to the format by
     * calling {@code setLenient(false)}.
     *
     * @param string
     *            the string to parse.
     * @param position
     *            input/output parameter, specifies the start index in
     *            {@code string} from where to start parsing. If parsing is
     *            successful, it is updated with the index following the parsed
     *            text; on error, the index is unchanged and the error index
     *            is set to the index where the error occurred.
     * @return the date resulting from the parsing, or {@code null} if there is
     *         an error.
     */
    //@Override
    CARAPI ParseObjectEx(
        /* [in] */ const String& string,
        /* [in] */ IParsePosition* position,
        /* [out] */ IInterface** object);

    /**
     * Sets the calendar used by this date format.
     *
     * @param cal
     *            the new calendar.
     */
    virtual CARAPI SetCalendar(
        /* [in] */ ICalendar* cal);

    /**
     * Specifies whether or not date/time parsing shall be lenient. With lenient
     * parsing, the parser may use heuristics to interpret inputs that do not
     * precisely match this object's format. With strict parsing, inputs must
     * match this object's format.
     *
     * @param value
     *            {@code true} to set the calendar to be lenient, {@code false}
     *            otherwise.
     */
    virtual CARAPI SetLenient(
        /* [in] */ Boolean value);

    /**
     * Sets the {@code NumberFormat} used by this date format.
     *
     * @param format
     *            the new number format.
     */
   virtual CARAPI SetNumberFormat(
       /* [in] */ INumberFormat* format);

    /**
     * Sets the time zone of the calendar used by this date format.
     *
     * @param timezone
     *            the new time zone.
     */
    virtual CARAPI SetTimeZone(
        /* [in] */ ITimeZone* timezone);

protected:
    /**
     * Constructs a new instance of {@code DateFormat}.
     */
    DateFormat();

    /**
     * Returns a new instance of {@code DateFormat} with the same properties.
     *
     * @return a shallow copy of this {@code DateFormat}.
     *
     * @see java.lang.Cloneable
     */
    //@Override
    //public Object clone() {
    //    DateFormat clone = (DateFormat) super.clone();
    //    clone.calendar = (Calendar) calendar.clone();
    //    clone.numberFormat = (NumberFormat) numberFormat.clone();
    //    return clone;
    //}

    /**
     * Compares this date format with the specified object and indicates if they
     * are equal.
     *
     * @param object
     *            the object to compare with this date format.
     * @return {@code true} if {@code object} is a {@code DateFormat} object and
     *         it has the same properties as this date format; {@code false}
     *         otherwise.
     * @see #hashCode
     */
    //@Override
    //public boolean equals(Object object) {
    //    if (this == object) {
    //        return true;
    //    }
    //    if (!(object instanceof DateFormat)) {
    //        return false;
    //    }
    //    DateFormat dateFormat = (DateFormat) object;
    //    return numberFormat.equals(dateFormat.numberFormat)
    //            && calendar.getTimeZone().equals(
    //                    dateFormat.calendar.getTimeZone())
    //            && calendar.getFirstDayOfWeek() == dateFormat.calendar
    //                    .getFirstDayOfWeek()
    //            && calendar.getMinimalDaysInFirstWeek() == dateFormat.calendar
    //                    .getMinimalDaysInFirstWeek()
    //            && calendar.isLenient() == dateFormat.calendar.isLenient();
    //}

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid) = 0;

protected:
    /**
     * The calendar that this {@code DateFormat} uses to format a number
     * representing a date.
     */
    AutoPtr<ICalendar> mCalendar;

    /**
     * The number format used to format a number.
     */
    AutoPtr<INumberFormat> mNumberFormat;
};

#endif //__DATEFORMAT_H__
