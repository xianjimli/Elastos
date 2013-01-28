#ifdef _DIME_H_
#define _DIME_H_

#include <elastos.h>

using namespace Elastos;

class Time {
public:
    Time();

    ~Time();

    /**
     * Constructs a {@code Time} object using the supplied values for <i>Hour</i>,
     * <i>Minute</i> and <i>Second</i>. The <i>Year</i>, <i>Month</i> and
     * <i>Day</i> elements of the {@code Time} object are set to the date
     * of the Epoch (January 1, 1970).
     * <p>
     * Any attempt to access the <i>Year</i>, <i>Month</i> or <i>Day</i>
     * elements of a {@code Time} object will result in an {@code
     * IllegalArgumentException}.
     * <p>
     * The result is undefined if any argument is out of bounds.
     *
     * @deprecated Use the constructor {@link #Time(long)}.
     * @param theHour
     *            a value in the range {@code [0,23]}.
     * @param theMinute
     *            a value in the range {@code [0,59]}.
     * @param theSecond
     *            a value in the range {@code [0,59]}.
     */
    //@SuppressWarnings("deprecation")
    //@Deprecated
    CARAPI Init(
        /**[in]**/Int32 theHour, 
        /**[in]**/Int32 theMinute, 
        /**[in]**/Int32 theSecond);

    /**
     * Constructs a {@code Time} object using a supplied time specified in
     * milliseconds.
     *
     * @param theTime
     *            a {@code Time} specified in milliseconds since the
     *            <i>Epoch</i> (January 1st 1970, 00:00:00.000).
     */
    CARAPI Init(
        /**[in]**/Int64 theTime);

    /**
     * @deprecated This method is deprecated and must not be used. An SQL
     *             {@code Time} object does not have a {@code Date} component.
     * @return does not return anything.
     * @throws IllegalArgumentException
     *             if this method is called.
     */
    //@SuppressWarnings("deprecation")
    //@Deprecated
    //@Override
    CARAPI GetDate(
        /**[out]**/Int32* result);

    /**
     * @deprecated This method is deprecated and must not be used. An SQL
     *             {@code Time} object does not have a <i>Day</i> component.
     * @return does not return anything.
     * @throws IllegalArgumentException
     *             if this method is called.
     */
    //@SuppressWarnings("deprecation")
    //@Deprecated
    //@Override
    CARAPI GetDayEx(
        /**[out]**/Int32* result);

    /**
     * @deprecated This method is deprecated and must not be used. An SQL
     *             {@code Time} object does not have a <i>Month</i> component.
     * @return does not return anything.
     * @throws IllegalArgumentException
     *             if this method is called.
     */
    //@SuppressWarnings("deprecation")
    //@Deprecated
    //@Override
    CARAPI GetMonth(
        /**[out]**/Int32* result);

    /**
     * @deprecated This method is deprecated and must not be used. An SQL
     *             {@code Time} object does not have a <i>Year</i> component.
     * @return does not return anything.
     * @throws IllegalArgumentException
     *             if this method is called.
     */
    //@SuppressWarnings("deprecation")
    //@Deprecated
    //@Override
    CARAPI GetYear(
        /**[out]**/Int32* result);

    /**
     * @deprecated This method is deprecated and must not be used. An SQL
     *             {@code Time} object does not have a {@code Date} component.
     * @throws IllegalArgumentException
     *             if this method is called.
     */
    //@SuppressWarnings("deprecation")
    //@Deprecated
    //@Override
    CARAPI SetDate(
        /**[in]**/Int32 i);

    /**
     * @deprecated This method is deprecated and must not be used. An SQL
     *             {@code Time} object does not have a <i>Month</i> component.
     * @throws IllegalArgumentException
     *             if this method is called.
     */
    //@SuppressWarnings("deprecation")
    //@Deprecated
    //@Override
    CARAPI SetMonth(
        /**[in]**/Int32 i);

    /**
     * @deprecated This method is deprecated and must not be used. An SQL
     *             {@code Time} object does not have a <i>Year</i> component.
     * @throws IllegalArgumentException
     *             if this method is called.
     */
    //@SuppressWarnings("deprecation")
    //@Deprecated
    //@Override
    CARAPI SetYear(
        /**[in]**/Int32 i);

    /**
     * Sets the time for this {@code Time} object to the supplied milliseconds
     * value.
     *
     * @param time
     *            A time value expressed as milliseconds since the <i>Epoch</i>.
     *            Negative values are milliseconds before the Epoch. The Epoch
     *            is January 1 1970, 00:00:00.000.
     */
    //@Override
    CARAPI SetTime(
        /**[in]**/Int64 time);

    /**
     * Formats the {@code Time} as a String in JDBC escape format: {@code
     * hh:mm:ss}.
     *
     * @return A String representing the {@code Time} value in JDBC escape
     *         format: {@code HH:mm:ss}
     */
    //@Override
    CARAPI ToString(
        /**[out]**/String* str);


    /**
     * Creates a {@code Time} object from a string holding a time represented in
     * JDBC escape format: {@code hh:mm:ss}.
     * <p>
     * An exception occurs if the input string does not comply with this format.
     *
     * @param timeString
     *            A String representing the time value in JDBC escape format:
     *            {@code hh:mm:ss}.
     * @return The {@code Time} object set to a time corresponding to the given
     *         time.
     * @throws IllegalArgumentException
     *             if the supplied time string is not in JDBC escape format.
     */
    CARAPI static ValueOf(
        /**[in]**/String timeString,
        /**[out]**/ITime** time);

private:
    /*
    * Private method to format the time
    */
    void Format(
        /**[in]**/Int32 date, 
        /**[in]**/Int32 digits, 
        /**[in]**/StringBuilder sb);

private:
    static const Int64 mSerialVersionUID;// = 8397324403548013681L;

    static const String mPADDING;// = "00";
};
#endif//_DIME_H_