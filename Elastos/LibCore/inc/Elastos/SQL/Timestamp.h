#ifndef _TIMESTAMP_H_
#define _TIMESTAMP_H_

#include <elastos.h>

using namespace Elastos;

class Timestamp {
public:
    Timestamp();

    ~Timestamp();

    /**
     * Returns a {@code Timestamp} corresponding to the time specified by the
     * supplied values for <i>Year</i>, <i>Month</i>, <i>Date</i>, <i>Hour</i>,
     * <i>Minutes</i>, <i>Seconds</i> and <i>Nanoseconds</i>.
     *
     * @deprecated Use the constructor {@link #Timestamp(long)}.
     * @param theYear
     *            specified as the year minus 1900.
     * @param theMonth
     *            specified as an integer in the range [0,11].
     * @param theDate
     *            specified as an integer in the range [1,31].
     * @param theHour
     *            specified as an integer in the range [0,23].
     * @param theMinute
     *            specified as an integer in the range [0,59].
     * @param theSecond
     *            specified as an integer in the range [0,59].
     * @param theNano
     *            which defines the nanosecond value of the timestamp specified
     *            as an integer in the range [0,999'999'999]
     * @throws IllegalArgumentException
     *             if any of the parameters is out of range.
     */
    //@SuppressWarnings("deprecation")
    //@Deprecated
    CARAPI Init(
        /**[in]**/Int32 theYear, 
        /**[in]**/Int32 theMonth, 
        /**[in]**/Int32 theDate, 
        /**[in]**/Int32 theHour,
        /**[in]**/Int32 theMinute, 
        /**[in]**/Int32 theSecond, 
        /**[in]**/Int32 theNano);

    /**
     * Returns a {@code Timestamp} object corresponding to the time represented
     * by a supplied time value.
     *
     * @param theTime
     *            a time value in the format of milliseconds since the Epoch
     *            (January 1 1970 00:00:00.000 GMT).
     */
    CARAPI Init(
        /**[in]**/Int64 theTime);

    /**
     * Returns {@code true} if this timestamp object is later than the supplied
     * timestamp, otherwise returns {@code false}.
     *
     * @param theTimestamp
     *            the timestamp to compare with this timestamp object.
     * @return {@code true} if this {@code Timestamp} object is later than the
     *         supplied timestamp, {@code false} otherwise.
     */
    CARAPI After(
        /**[in]**/ITimestamp* theTimestamp,
        /**[out]**/Boolean* result);

    /**
     * Returns {@code true} if this {@code Timestamp} object is earlier than the
     * supplied timestamp, otherwise returns {@code false}.
     *
     * @param theTimestamp
     *            the timestamp to compare with this {@code Timestamp} object.
     * @return {@code true} if this {@code Timestamp} object is earlier than the
     *         supplied timestamp, {@code false} otherwise.
     */
    CARAPI Before(
        /**[in]**/ITimestamp* theTimestamp,
        /**[out]**/Boolean* result);

    /**
     * Compares this {@code Timestamp} object with a supplied {@code Timestamp}
     * object.
     *
     * @param theObject
     *            the timestamp to compare with this {@code Timestamp} object,
     *            passed as an {@code Object}.
     * @return <dd>
     *         <dl>
     *         {@code 0} if the two {@code Timestamp} objects are equal in time
     *         </dl>
     *         <dl>
     *         a value {@code < 0} if this {@code Timestamp} object is before
     *         the supplied {@code Timestamp} and a value
     *         </dl>
     *         <dl>
     *         {@code > 0} if this {@code Timestamp} object is after the
     *         supplied {@code Timestamp}
     *         </dl>
     *         </dd>
     * @throws ClassCastException
     *             if the supplied object is not a {@code Timestamp} object.
     */
    //@Override
    CARAPI CompareTo(
        /**[in]**/IDate* theObject,
        /**[out]**/Int32* result);

    /**
     * Compares this {@code Timestamp} object with a supplied {@code Timestamp}
     * object.
     *
     * @param theTimestamp
     *            the timestamp to compare with this {@code Timestamp} object,
     *            passed in as a {@code Timestamp}.
     * @return one of the following:
     *         <ul>
     *         <li>{@code 0}, if the two {@code Timestamp} objects are
     *         equal in time</li>
     *         <li>{@code < 0}, if this {@code Timestamp} object is before the
     *         supplied {@code Timestamp}</li>
     *         <li> {@code > 0}, if this {@code Timestamp} object is after the
     *         supplied {@code Timestamp}</li>
     *         </ul>
     */
    CARAPI CompareToEx(
        /**[in]**/ITimestamp* theTimestamp,
        /**[out]**/Int32* result);

    /**
     * Tests to see if this timestamp is equal to a supplied object.
     *
     * @param theObject
     *            the object to which this timestamp is compared.
     * @return {@code true} if this {@code Timestamp} object is equal to the
     *         supplied {@code Timestamp} object<br>{@code false} if the object
     *         is not a {@code Timestamp} object or if the object is a {@code
     *         Timestamp} but represents a different instant in time.
     */
    //@Override
    CARAPI Equals(
        /**[in]**/IInterface* theObject,
        /**[out]**/Boolean* result);

    /**
     * Tests to see if this timestamp is equal to a supplied timestamp.
     *
     * @param theTimestamp
     *            the timestamp to compare with this {@code Timestamp} object,
     *            passed as an {@code Object}.
     * @return {@code true} if this {@code Timestamp} object is equal to the
     *         supplied {@code Timestamp} object, {@code false} otherwise.
     */
    CARAPI EqualsEx(
        /**[in]**/IInterface* theObject,
        /**[out]**/Boolean* result);

    /**
     * Gets this {@code Timestamp}'s nanosecond value
     *
     * @return The timestamp's nanosecond value, an integer between 0 and
     *         999,999,999.
     */
    CARAPI GetNanos(
        /**[out]**/Int32* result);

    /**
     * Returns the time represented by this {@code Timestamp} object, as a long
     * value containing the number of milliseconds since the Epoch (January 1
     * 1970, 00:00:00.000 GMT).
     *
     * @return the number of milliseconds that have passed since January 1 1970,
     *         00:00:00.000 GMT.
     */
    //@Override
    CARAPI GetTime(
        /**[out]**/Int64* result);

    /**
     * Sets the nanosecond value for this {@code Timestamp}.
     *
     * @param n
     *            number of nanoseconds.
     * @throws IllegalArgumentException
     *             if number of nanoseconds smaller than 0 or greater than
     *             999,999,999.
     */
    CARAPI SetNanos(
        /**[in]**/Int32 n);

    /**
     * Sets the time represented by this {@code Timestamp} object to the
     * supplied time, defined as the number of milliseconds since the Epoch
     * (January 1 1970, 00:00:00.000 GMT).
     *
     * @param theTime
     *            number of milliseconds since the Epoch (January 1 1970,
     *            00:00:00.000 GMT).
     */
    //@Override
    CARAPI SetTime(
        /**[in]**/Int64 theTime);

    /**
     * Returns the timestamp formatted as a String in the JDBC Timestamp Escape
     * format, which is {@code "yyyy-MM-dd HH:mm:ss.nnnnnnnnn"}.
     *
     * @return A string representing the instant defined by the {@code
     *         Timestamp}, in JDBC Timestamp escape format.
     */
    //@SuppressWarnings("deprecation")
    //@Override
    CARAPI ToString(
        /**[out]**/String* str);

    /**
     * Creates a {@code Timestamp} object with a time value equal to the time
     * specified by a supplied String holding the time in JDBC timestamp escape
     * format, which is {@code "yyyy-MM-dd HH:mm:ss.nnnnnnnnn}"
     *
     * @param s
     *            the {@code String} containing a time in JDBC timestamp escape
     *            format.
     * @return A {@code Timestamp} object with time value as defined by the
     *         supplied {@code String}.
     * @throws IllegalArgumentException
     *             if the provided string is {@code null}.
     */
    CARAPI static ValueOf(
        /**[in]**/String s,
        /**[out]**/ITimestamp** result);

private:
    void SetTimeImpl(
        /**[in]**/Int64 theTime);

    /*
    * Private method to format the time
    */
    void Format(
        /**[in]**/Int32 date, 
        /**[in]**/Int32 digits, 
        /**[in]**/StringBuilder sb);

//    static IllegalArgumentException badTimestampString(String s);
private:
    static const String PADDING;// = "000000000";

    static const Int64 mSerialVersionUID;// = 2745179027874758501L;

    // The nanoseconds time value of the Timestamp
    Int32 mNanos;

    // The regex pattern of yyyy-MM-dd HH:mm:ss
    static const String TIME_FORMAT_REGEX;// = "[0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2}.*";
};
#endif//_TIMESTAMP_H_
