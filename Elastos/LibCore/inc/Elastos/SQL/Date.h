#ifndef _DATE_H_
#define _DATE_H_

#include <elastos.h>

using namespace Elastos;

class Date {
public:
    Date();

    ~Date();

    /**
     * Constructs a {@code Date} object corresponding to the supplied year,
     * month and day.
     *
     * @deprecated Use the constructor {@link #Date(long)}.
     * @param theYear
     *            the year, specified as the year minus 1900. Must be in the
     *            range {@code [0,8099]}.
     * @param theMonth
     *            the month, specified as a number with 0 = January. Must be in
     *            the range {@code [0,11]}.
     * @param theDay
     *            the day in the month. Must be in the range {@code [1,31]}.
     */
    //@Deprecated
    CARAPI Init(
        /** [in] **/Int32 theYear, 
        /** [in] **/Int32 theMonth, 
        /** [in] **/Int32 theDay);

    /**
     * Creates a date which corresponds to the day determined by the supplied
     * milliseconds time value {@code theDate}.
     *
     * @param theDate
     *            a time value in milliseconds since the epoch - January 1 1970
     *            00:00:00 GMT. The time value (hours, minutes, seconds,
     *            milliseconds) stored in the {@code Date} object is adjusted to
     *            correspond to 00:00:00 GMT on the day determined by the supplied
     *            time value.
     */
    CARAPI Init(
        /** [in] **/Int64 theDate);

    /**
     * @deprecated This method is deprecated and must not be used. SQL {@code
     *             Date} values do not have an hours component.
     * @return does not return anything.
     * @throws IllegalArgumentException
     *             if this method is called.
     */
    //@Deprecated
    //@Override
    CARAPI GetHours(
        /** [out] **/Int32* hours);

    /**
     * @deprecated This method is deprecated and must not be used. SQL {@code
     *             Date} values do not have a minutes component.
     * @return does not return anything.
     * @throws IllegalArgumentException
     *             if this method is called.
     */
    //@Deprecated
    //@Override
    CARAPI GetMinutes(
        /** [out] **/Int32* minutes);

    /**
     * @deprecated This method is deprecated and must not be used. SQL {@code
     *             Date} values do not have a seconds component.
     * @return does not return anything.
     * @throws IllegalArgumentException
     *             if this method is called.
     */
    //@Deprecated
    //@Override
    CARAPI GetSeconds(
        /** [out] **/Int32* seconds);

    /**
     * @deprecated This method is deprecated and must not be used. SQL {@code
     *             Date} values do not have an hours component.
     * @param theHours
     *            the number of hours to set.
     * @throws IllegalArgumentException
     *             if this method is called.
     */
    //@Deprecated
    //@Override
    CARAPI SetHours(
        /** [in] **/Int32 theHours);

    /**
     * @deprecated This method is deprecated and must not be used. SQL {@code
     *             Date} values do not have a minutes component.
     * @param theMinutes
     *            the number of minutes to set.
     * @throws IllegalArgumentException
     *             if this method is called.
     */
    //@Deprecated
    //@Override
    CARAPI SetMinutes(
        /** [in] **/Int32 theMinutes);

    /**
     * @deprecated This method is deprecated and must not be used. SQL {@code
     *             Date} values do not have a seconds component.
     * @param theSeconds
     *            the number of seconds to set.
     * @throws IllegalArgumentException
     *             if this method is called.
     */
    //@Deprecated
    //@Override
    CARAPI SetSeconds(
        /** [in] **/Int32 theSeconds);

    /**
     * Sets this date to a date supplied as a milliseconds value. The date is
     * set based on the supplied time value and rounded to zero GMT for that day.
     *
     * @param theTime
     *            the time in milliseconds since the Epoch.
     */
    //@Override
    CARAPI SetTime(
        /** [in] **/Int64 theTime);

    /**
     * Produces a string representation of the date in SQL format
     *
     * @return a string representation of the date in SQL format - {@code "yyyy-MM-dd"}.
     */
    //@Override
    CARAPI ToString(
        /** [out] **/String* str);

    /**
     * Creates a {@code Date} from a string representation of a date in SQL
     * format.
     *
     * @param dateString
     *            the string representation of a date in SQL format - " {@code yyyy-MM-dd}".
     * @return the {@code Date} object.
     * @throws IllegalArgumentException
     *             if the format of the supplied string does not match the SQL
     *             format.
     */
    CARAPI static ValueOf(
        /** [in] **/String dateString,
        /** [out] **/IDate** date);

private:
    /*
    * Private method to format the time
    */
    void Format(
        /** [in] **/Int32 date, 
        /** [in] **/Int32 digits, 
        /** [in] **/StringBuilder sb);

    /*
     * Private method which normalizes a Time value, removing all low
     * significance digits corresponding to milliseconds, seconds, minutes and
     * hours, so that the returned Time value corresponds to 00:00:00 GMT on a
     * particular day.
     */
    static Int64 NormalizeTime(
        /** [in] **/Int64 theTime);

private:
    static const Int64 mSerialVersionUID;// = 1511598038487230103L;

    static const String PADDING;// = "0000";
};
#endif//_DATE_H_
