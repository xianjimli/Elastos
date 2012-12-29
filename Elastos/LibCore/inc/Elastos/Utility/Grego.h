/**
 *******************************************************************************
 * Copyright (C) 2003-2008, International Business Machines Corporation and
 * others. All Rights Reserved.
 *******************************************************************************
 * Partial port from ICU4C's Grego class in i18n/gregoimp.h.
 *
 * Methods ported, or moved here from OlsonTimeZone, initially
 * for work on Jitterbug 5470:
 *   tzdata2006n Brazil incorrect fall-back date 2009-mar-01
 * Only the methods necessary for that work are provided - this is not a full
 * port of ICU4C's Grego class (yet).
 *
 * These utilities are used by both OlsonTimeZone and SimpleTimeZone.
 */




/**
 * A utility class providing proleptic Gregorian calendar functions
 * used by time zone and calendar code.  Do not instantiate.
 *
 * Note:  Unlike GregorianCalendar, all computations performed by this
 * class occur in the pure proleptic GregorianCalendar.
 */
// android-changed: public
class Grego {
    // Max/min milliseconds 
public:
    static const Int64 MIN_MILLIS = -184303902528000000L;
    static const Int64 MAX_MILLIS = 183882168921600000L;

    static const Int32 MILLIS_PER_SECOND = 1000;
    static const Int32 MILLIS_PER_MINUTE = 60*MILLIS_PER_SECOND;
    static const Int32 MILLIS_PER_HOUR = 60*MILLIS_PER_MINUTE;
    static const Int32 MILLIS_PER_DAY = 24*MILLIS_PER_HOUR;
    
    //  January 1, 1 CE Gregorian
private:
    static const Int32 JULIAN_1_CE = 1721426;
    //  January 1, 1970 CE Gregorian
    static const Int32 JULIAN_1970_CE = 2440588;

    static const Int32 MONTH_LENGTH[] = {
        31,28,31,30,31,30,31,31,30,31,30,31,
        31,29,31,30,31,30,31,31,30,31,30,31
    };

    static const Int32 DAYS_BEFORE[] = {
        0,31,59,90,120,151,181,212,243,273,304,334,
        0,31,60,91,121,152,182,213,244,274,305,335 };

    /**
     * Return true if the given year is a leap year.
     * @param year Gregorian year, with 0 == 1 BCE, -1 == 2 BCE, etc.
     * @return true if the year is a leap year
     */
public:
    static Boolean IsLeapYear(Int32 year) {
        // year&0x3 == year%4
        return ((year&0x3) == 0) && ((year%100 != 0) || (year%400 == 0));
    }

    /**
     * Return the number of days in the given month.
     * @param year Gregorian year, with 0 == 1 BCE, -1 == 2 BCE, etc.
     * @param month 0-based month, with 0==Jan
     * @return the number of days in the given month
     */
    static Int32 MonthLength(Int32 year, Int32 month) {
        return MONTH_LENGTH[month + (IsLeapYear(year) ? 12 : 0)];
    }

    /**
     * Return the length of a previous month of the Gregorian calendar.
     * @param year Gregorian year, with 0 == 1 BCE, -1 == 2 BCE, etc.
     * @param month 0-based month, with 0==Jan
     * @return the number of days in the month previous to the given month
     */
    static Int32 PreviousMonthLength(Int32 year, Int32 month) {
        return (month > 0) ? MonthLength(year, month-1) : 31;
    }

    /**
     * Convert a year, month, and day-of-month, given in the proleptic
     * Gregorian calendar, to 1970 epoch days.
     * @param year Gregorian year, with 0 == 1 BCE, -1 == 2 BCE, etc.
     * @param month 0-based month, with 0==Jan
     * @param dom 1-based day of month
     * @return the day number, with day 0 == Jan 1 1970
     */
    static Int64 FieldsToDay(Int32 year, Int32 month, Int32 dom) {
        Int32 y = year - 1;
        Int64 julian =
            365 * y + FloorDivide(y, 4) + (JULIAN_1_CE - 3) +    // Julian cal
            FloorDivide(y, 400) - FloorDivide(y, 100) + 2 +   // => Gregorian cal
            DAYS_BEFORE[month + (IsLeapYear(year) ? 12 : 0)] + dom; // => month/dom
        return julian - JULIAN_1970_CE; // JD => epoch day
    }

    /**
     * Return the day of week on the 1970-epoch day
     * @param day the 1970-epoch day (integral value)
     * @return the day of week
     */
    static Int32 DayOfWeek(Int64 day) {
        Int64 remainder[1] = {0};
        //FloorDivide(day + Calendar.THURSDAY, 7, remainder);
        int dayOfWeek = (Int32)remainder[0];
        dayOfWeek = (dayOfWeek == 0) ? 7 : dayOfWeek;
        return dayOfWeek;
    }

    static ArrayOf<Int32>* DayToFields(Int64 day, ArrayOf<Int32> *fields) {
        if (fields == NULL || fields->GetLength() < 5) {
            fields = ArrayOf<Int32>::Alloc(5);
        }
        // Convert from 1970 CE epoch to 1 CE epoch (Gregorian calendar)
        day += JULIAN_1970_CE - JULIAN_1_CE;

        Int64 rem[1] = {0};
        Int64 n400 = FloorDivide(day, 146097, rem);
        Int64 n100 = FloorDivide(rem[0], 36524, rem);
        Int64 n4 = FloorDivide(rem[0], 1461, rem);
        Int64 n1 = FloorDivide(rem[0], 365, rem);

        Int32 year = (int)(400 * n400 + 100 * n100 + 4 * n4 + n1);
        Int32 dayOfYear = (int)rem[0];
        if (n100 == 4 || n1 == 4) {
            dayOfYear = 365;    // Dec 31 at end of 4- or 400-yr cycle
        }
        else {
            ++year;
        }

        Boolean isLeap = IsLeapYear(year);
        Int32 correction = 0;
        Int32 march1 = isLeap ? 60 : 59;  // zero-based DOY for March 1
        if (dayOfYear >= march1) {
            correction = isLeap ? 1 : 2;
        }
        Int32 month = (12 * (dayOfYear + correction) + 6) / 367;  // zero-based month
        Int32 dayOfMonth = dayOfYear - DAYS_BEFORE[isLeap ? month + 12 : month] + 1; // one-based DOM
        Int32 dayOfWeek = (int)((day + 2) % 7);  // day 0 is Monday(2)
        if (dayOfWeek < 1 /* Sunday */) {
            dayOfWeek += 7;
        }
        dayOfYear++; // 1-based day of year

        fields[0] = year;
        fields[1] = month;
        fields[2] = dayOfMonth;
        fields[3] = dayOfWeek;
        fields[4] = dayOfYear;

        return fields;
    }

    /*
     * Convert long time to date/time fields
     * 
     * result[0] : year
     * result[1] : month
     * result[2] : dayOfMonth
     * result[3] : dayOfWeek
     * result[4] : dayOfYear
     * result[5] : millisecond in day
     */
    static ArrayOf<Int32>* TimeToFields(Int64 time, ArrayOf<Int32>  **fields) {
        if (fields == NULL || (*fields)->GetLength() < 6) {
            fields = ArrayOf<Int32>::Alloc(6);
        }
        Int64 remainder[1] = {0};
        Int64 day = FloorDivide(time, 24*60*60*1000 /* milliseconds per day */, remainder);
        DayToFields(day, fields);
        fields[5] = (Int32)remainder[0];
        return *fields;
    }

    static Int64 FloorDivide(Int64 numerator, Int64 denominator) {
        // We do this computation in order to handle
        // a numerator of Long.MIN_VALUE correctly
        return (numerator >= 0) ?
            numerator / denominator :
            ((numerator + 1) / denominator) - 1;
    }

    static Int64 FloorDivide(Int64 numerator, Int64 denominator, ArrayOf<Int64> *remainder) {
        if (numerator >= 0) {
            remainder[0] = numerator % denominator;
            return numerator / denominator;
        }
        Int64 quotient = ((numerator + 1) / denominator) - 1;
        remainder[0] = numerator - (quotient * denominator);
        return quotient;
    }

    /*
     * Returns the ordinal number for the specified day of week in the month.
     * The valid return value is 1, 2, 3, 4 or -1.
     */
    static Int32 GetDayOfWeekInMonth(Int32 year, Int32 month, Int32 dayOfMonth) {
        Int32 weekInMonth = (dayOfMonth + 6)/7;
        if (weekInMonth == 4) {
            if (dayOfMonth + 7 > monthLength(year, month)) {
                weekInMonth = -1;
            }
        } else if (weekInMonth == 5) {
            weekInMonth = -1;
        }
        return weekInMonth;
    }
};
