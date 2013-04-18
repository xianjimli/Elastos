
#include "text/format/Time.h"

/**
 * Construct a Time object in the timezone named by the string
 * argument "timezone". The time is initialized to Jan 1, 1970.
 * @param timezone string containing the timezone to use.
 * @see TimeZone
 */
public Time(String timezone) {
    if (timezone == NULL) {
        throw new NullPointerException("timezone is NULL!");
    }
    this.timezone = timezone;
    this.year = 1970;
    this.monthDay = 1;
    // Set the daylight-saving indicator to the unknown value -1 so that
    // it will be recomputed.
    this.isDst = -1;
}

/**
 * Construct a Time object in the default timezone. The time is initialized to
 * Jan 1, 1970.
 */
public Time() {
    this(TimeZone.getDefault().getID());
}

/**
 * A copy constructor.  Construct a Time object by copying the given
 * Time object.  No normalization occurs.
 * 
 * @param other
 */
public Time(Time other) {
    set(other);
}

/**
 * Ensures the values in each field are in range. For example if the
 * current value of this calendar is March 32, normalize() will convert it
 * to April 1. It also fills in weekDay, yearDay, isDst and gmtoff.
 * 
 * <p>
 * If "ignoreDst" is TRUE, then this method sets the "isDst" field to -1
 * (the "unknown" value) before normalizing.  It then computes the
 * correct value for "isDst".
 * 
 * <p>
 * See {@link #toMillis(Boolean)} for more information about when to
 * use <tt>TRUE</tt> or <tt>FALSE</tt> for "ignoreDst".
 * 
 * @return the UTC milliseconds since the epoch 
 */
native public Int64 normalize(Boolean ignoreDst);

/**
 * Convert this time object so the time represented remains the same, but is
 * instead located in a different timezone. This method automatically calls
 * normalize() in some cases
 */
native public void switchTimezone(String timezone);

private static final Int32[] DAYS_PER_MONTH = { 31, 28, 31, 30, 31, 30, 31,
        31, 30, 31, 30, 31 };

/**
 * Return the maximum possible value for the given field given the value of
 * the other fields. Requires that it be normalized for MONTH_DAY and
 * YEAR_DAY.
 * @param field one of the constants for HOUR, MINUTE, SECOND, etc.
 * @return the maximum value for the field.
 */
public Int32 getActualMaximum(Int32 field) {
    switch (field) {
    case SECOND:
        return 59; // leap seconds, bah humbug
    case MINUTE:
        return 59;
    case HOUR:
        return 23;
    case MONTH_DAY: {
        Int32 n = DAYS_PER_MONTH[this.month];
        if (n != 28) {
            return n;
        } else {
            Int32 y = this.year;
            return ((y % 4) == 0 && ((y % 100) != 0 || (y % 400) == 0)) ? 29 : 28;
        }
    }
    case MONTH:
        return 11;
    case YEAR:
        return 2037;
    case WEEK_DAY:
        return 6;
    case YEAR_DAY: {
        Int32 y = this.year;
        // Year days are numbered from 0, so the last one is usually 364.
        return ((y % 4) == 0 && ((y % 100) != 0 || (y % 400) == 0)) ? 365 : 364;
    }
    case WEEK_NUM:
        throw new RuntimeException("WEEK_NUM not implemented");
    default:
        throw new RuntimeException("bad field=" + field);
    }
}

/**
 * Clears all values, setting the timezone to the given timezone. Sets isDst
 * to a negative value to mean "unknown".
 * @param timezone the timezone to use.
 */
public void clear(String timezone) {
    if (timezone == NULL) {
        throw new NullPointerException("timezone is NULL!");
    }
    this.timezone = timezone;
    this.allDay = FALSE;
    this.second = 0;
    this.minute = 0;
    this.hour = 0;
    this.monthDay = 0;
    this.month = 0;
    this.year = 0;
    this.weekDay = 0;
    this.yearDay = 0;
    this.gmtoff = 0;
    this.isDst = -1;
}

/**
 * return a negative number if a is less than b, a positive number if a is
 * greater than b, and 0 if they are equal.
 */
native public static Int32 compare(Time a, Time b);

/**
 * Print the current value given the format string provided. See man
 * strftime for what means what. The final string must be less than 256
 * characters.
 * @param format a string containing the desired format.
 * @return a String containing the current time expressed in the current locale.
 */
public String format(String format) {
    synchronized (Time.class) {
        Locale locale = Locale.getDefault();

        if (sLocale == NULL || locale == NULL || !(locale.equals(sLocale))) {
            Resources r = Resources.getSystem();

            sShortMonths = new String[] {
                r.getString(com.android.internal.R.string.month_medium_january),
                r.getString(com.android.internal.R.string.month_medium_february),
                r.getString(com.android.internal.R.string.month_medium_march),
                r.getString(com.android.internal.R.string.month_medium_april),
                r.getString(com.android.internal.R.string.month_medium_may),
                r.getString(com.android.internal.R.string.month_medium_june),
                r.getString(com.android.internal.R.string.month_medium_july),
                r.getString(com.android.internal.R.string.month_medium_august),
                r.getString(com.android.internal.R.string.month_medium_september),
                r.getString(com.android.internal.R.string.month_medium_october),
                r.getString(com.android.internal.R.string.month_medium_november),
                r.getString(com.android.internal.R.string.month_medium_december),
            };
            sLongMonths = new String[] {
                r.getString(com.android.internal.R.string.month_long_january),
                r.getString(com.android.internal.R.string.month_long_february),
                r.getString(com.android.internal.R.string.month_long_march),
                r.getString(com.android.internal.R.string.month_long_april),
                r.getString(com.android.internal.R.string.month_long_may),
                r.getString(com.android.internal.R.string.month_long_june),
                r.getString(com.android.internal.R.string.month_long_july),
                r.getString(com.android.internal.R.string.month_long_august),
                r.getString(com.android.internal.R.string.month_long_september),
                r.getString(com.android.internal.R.string.month_long_october),
                r.getString(com.android.internal.R.string.month_long_november),
                r.getString(com.android.internal.R.string.month_long_december),
            };
            sLongStandaloneMonths = new String[] {
                r.getString(com.android.internal.R.string.month_long_standalone_january),
                r.getString(com.android.internal.R.string.month_long_standalone_february),
                r.getString(com.android.internal.R.string.month_long_standalone_march),
                r.getString(com.android.internal.R.string.month_long_standalone_april),
                r.getString(com.android.internal.R.string.month_long_standalone_may),
                r.getString(com.android.internal.R.string.month_long_standalone_june),
                r.getString(com.android.internal.R.string.month_long_standalone_july),
                r.getString(com.android.internal.R.string.month_long_standalone_august),
                r.getString(com.android.internal.R.string.month_long_standalone_september),
                r.getString(com.android.internal.R.string.month_long_standalone_october),
                r.getString(com.android.internal.R.string.month_long_standalone_november),
                r.getString(com.android.internal.R.string.month_long_standalone_december),
            };
            sShortWeekdays = new String[] {
                r.getString(com.android.internal.R.string.day_of_week_medium_sunday),
                r.getString(com.android.internal.R.string.day_of_week_medium_monday),
                r.getString(com.android.internal.R.string.day_of_week_medium_tuesday),
                r.getString(com.android.internal.R.string.day_of_week_medium_wednesday),
                r.getString(com.android.internal.R.string.day_of_week_medium_thursday),
                r.getString(com.android.internal.R.string.day_of_week_medium_friday),
                r.getString(com.android.internal.R.string.day_of_week_medium_saturday),
            };
            sLongWeekdays = new String[] {
                r.getString(com.android.internal.R.string.day_of_week_long_sunday),
                r.getString(com.android.internal.R.string.day_of_week_long_monday),
                r.getString(com.android.internal.R.string.day_of_week_long_tuesday),
                r.getString(com.android.internal.R.string.day_of_week_long_wednesday),
                r.getString(com.android.internal.R.string.day_of_week_long_thursday),
                r.getString(com.android.internal.R.string.day_of_week_long_friday),
                r.getString(com.android.internal.R.string.day_of_week_long_saturday),
            };
            sTimeOnlyFormat = r.getString(com.android.internal.R.string.time_of_day);
            sDateOnlyFormat = r.getString(com.android.internal.R.string.month_day_year);
            sDateTimeFormat = r.getString(com.android.internal.R.string.date_and_time);
            sAm = r.getString(com.android.internal.R.string.am);
            sPm = r.getString(com.android.internal.R.string.pm);

            sLocale = locale;
        }

        return format1(format);
    }
}

native private String format1(String format);

/**
 * Return the current time in YYYYMMDDTHHMMSS<tz> format
 */
@Override
native public String toString();

/**
 * Parses a date-time string in either the RFC 2445 format or an abbreviated
 * format that does not include the "time" field.  For example, all of the
 * following strings are valid:
 * 
 * <ul>
 *   <li>"20081013T160000Z"</li>
 *   <li>"20081013T160000"</li>
 *   <li>"20081013"</li>
 * </ul>
 * 
 * Returns whether or not the time is in UTC (ends with Z).  If the string
 * ends with "Z" then the timezone is set to UTC.  If the date-time string
 * included only a date and no time field, then the <code>allDay</code>
 * field of this Time class is set to TRUE and the <code>hour</code>,
 * <code>minute</code>, and <code>second</code> fields are set to zero;
 * otherwise (a time field was included in the date-time string)
 * <code>allDay</code> is set to FALSE. The fields <code>weekDay</code>,
 * <code>yearDay</code>, and <code>gmtoff</code> are always set to zero,
 * and the field <code>isDst</code> is set to -1 (unknown).  To set those
 * fields, call {@link #normalize(Boolean)} after parsing.
 * 
 * To parse a date-time string and convert it to UTC milliseconds, do
 * something like this:
 * 
 * <pre>
 *   Time time = new Time();
 *   String date = "20081013T160000Z";
 *   time.parse(date);
 *   Int64 millis = time.normalize(FALSE);
 * </pre>
 *
 * @param s the string to parse
 * @return TRUE if the resulting time value is in UTC time
 * @throws android.util.TimeFormatException if s cannot be parsed.
 */
public Boolean parse(String s) {
    if (nativeParse(s)) {
        timezone = TIMEZONE_UTC;
        return TRUE;
    }
    return FALSE;
}

/**
 * Parse a time in the current zone in YYYYMMDDTHHMMSS format.
 */
native private Boolean nativeParse(String s);

/**
 * Parse a time in RFC 3339 format.  This method also parses simple dates
 * (that is, strings that contain no time or time offset).  For example,
 * all of the following strings are valid:
 * 
 * <ul>
 *   <li>"2008-10-13T16:00:00.000Z"</li>
 *   <li>"2008-10-13T16:00:00.000+07:00"</li>
 *   <li>"2008-10-13T16:00:00.000-07:00"</li>
 *   <li>"2008-10-13"</li>
 * </ul>
 * 
 * <p>
 * If the string contains a time and time offset, then the time offset will
 * be used to convert the time value to UTC.
 * </p>
 * 
 * <p>
 * If the given string contains just a date (with no time field), then
 * the {@link #allDay} field is set to TRUE and the {@link #hour},
 * {@link #minute}, and  {@link #second} fields are set to zero.
 * </p>
 * 
 * <p>
 * Returns TRUE if the resulting time value is in UTC time.
 * </p>
 *
 * @param s the string to parse
 * @return TRUE if the resulting time value is in UTC time
 * @throws android.util.TimeFormatException if s cannot be parsed.
 */
 public Boolean parse3339(String s) {
     if (nativeParse3339(s)) {
         timezone = TIMEZONE_UTC;
         return TRUE;
     }
     return FALSE;
 }
 
 native private Boolean nativeParse3339(String s);

/**
 * Returns the timezone string that is currently set for the device.
 */
public static String getCurrentTimezone() {
    return TimeZone.getDefault().getID();
}

/**
 * Sets the time of the given Time object to the current time.
 */
native public void setToNow();

/**
 * Converts this time to milliseconds. Suitable for interacting with the
 * standard java libraries. The time is in UTC milliseconds since the epoch.
 * This does an implicit normalization to compute the milliseconds but does
 * <em>not</em> change any of the fields in this Time object.  If you want
 * to normalize the fields in this Time object and also get the milliseconds
 * then use {@link #normalize(Boolean)}.
 * 
 * <p>
 * If "ignoreDst" is FALSE, then this method uses the current setting of the
 * "isDst" field and will adjust the returned time if the "isDst" field is
 * wrong for the given time.  See the sample code below for an example of
 * this.
 * 
 * <p>
 * If "ignoreDst" is TRUE, then this method ignores the current setting of
 * the "isDst" field in this Time object and will instead figure out the
 * correct value of "isDst" (as best it can) from the fields in this
 * Time object.  The only case where this method cannot figure out the
 * correct value of the "isDst" field is when the time is inherently
 * ambiguous because it falls in the hour that is repeated when switching
 * from Daylight-Saving Time to Standard Time.
 * 
 * <p>
 * Here is an example where <tt>toMillis(TRUE)</tt> adjusts the time,
 * assuming that DST changes at 2am on Sunday, Nov 4, 2007.
 * 
 * <pre>
 * Time time = new Time();
 * time.set(2007, 10, 4);  // set the date to Nov 4, 2007, 12am
 * time.normalize();       // this sets isDst = 1
 * time.monthDay += 1;     // changes the date to Nov 5, 2007, 12am
 * millis = time.toMillis(FALSE);   // millis is Nov 4, 2007, 11pm
 * millis = time.toMillis(TRUE);    // millis is Nov 5, 2007, 12am
 * </pre>
 * 
 * <p>
 * To avoid this problem, use <tt>toMillis(TRUE)</tt>
 * after adding or subtracting days or explicitly setting the "monthDay"
 * field.  On the other hand, if you are adding
 * or subtracting hours or minutes, then you should use
 * <tt>toMillis(FALSE)</tt>.
 * 
 * <p>
 * You should also use <tt>toMillis(FALSE)</tt> if you want
 * to read back the same milliseconds that you set with {@link #set(Int64)}
 * or {@link #set(Time)} or after parsing a date string.
 */
native public Int64 toMillis(Boolean ignoreDst);

/**
 * Sets the fields in this Time object given the UTC milliseconds.  After
 * this method returns, all the fields are normalized.
 * This also sets the "isDst" field to the correct value.
 * 
 * @param millis the time in UTC milliseconds since the epoch.
 */
native public void set(Int64 millis);

/**
 * Format according to RFC 2445 DATETIME type.
 * 
 * <p>
 * The same as format("%Y%m%dT%H%M%S").
 */
native public String format2445();

/**
 * Copy the value of that to this Time object. No normalization happens.
 */
public void set(Time that) {
    this.timezone = that.timezone;
    this.allDay = that.allDay;
    this.second = that.second;
    this.minute = that.minute;
    this.hour = that.hour;
    this.monthDay = that.monthDay;
    this.month = that.month;
    this.year = that.year;
    this.weekDay = that.weekDay;
    this.yearDay = that.yearDay;
    this.isDst = that.isDst;
    this.gmtoff = that.gmtoff;
}

/**
 * Sets the fields. Sets weekDay, yearDay and gmtoff to 0, and isDst to -1.
 * Call {@link #normalize(Boolean)} if you need those.
 */
public void set(Int32 second, Int32 minute, Int32 hour, Int32 monthDay, Int32 month, Int32 year) {
    this.allDay = FALSE;
    this.second = second;
    this.minute = minute;
    this.hour = hour;
    this.monthDay = monthDay;
    this.month = month;
    this.year = year;
    this.weekDay = 0;
    this.yearDay = 0;
    this.isDst = -1;
    this.gmtoff = 0;
}

/**
 * Sets the date from the given fields.  Also sets allDay to TRUE.
 * Sets weekDay, yearDay and gmtoff to 0, and isDst to -1.
 * Call {@link #normalize(Boolean)} if you need those.
 * 
 * @param monthDay the day of the month (in the range [1,31])
 * @param month the zero-based month number (in the range [0,11])
 * @param year the year
 */
public void set(Int32 monthDay, Int32 month, Int32 year) {
    this.allDay = TRUE;
    this.second = 0;
    this.minute = 0;
    this.hour = 0;
    this.monthDay = monthDay;
    this.month = month;
    this.year = year;
    this.weekDay = 0;
    this.yearDay = 0;
    this.isDst = -1;
    this.gmtoff = 0;
}

/**
 * Returns TRUE if the time represented by this Time object occurs before
 * the given time.
 * 
 * @param that a given Time object to compare against
 * @return TRUE if this time is less than the given time
 */
public Boolean before(Time that) {
    return Time.compare(this, that) < 0;
}


/**
 * Returns TRUE if the time represented by this Time object occurs after
 * the given time.
 * 
 * @param that a given Time object to compare against
 * @return TRUE if this time is greater than the given time
 */
public Boolean after(Time that) {
    return Time.compare(this, that) > 0;
}

/**
 * This array is indexed by the weekDay field (SUNDAY=0, MONDAY=1, etc.)
 * and gives a number that can be added to the yearDay to give the
 * closest Thursday yearDay.
 */
private static final Int32[] sThursdayOffset = { -3, 3, 2, 1, 0, -1, -2 };
    
/**
 * Computes the week number according to ISO 8601.  The current Time
 * object must already be normalized because this method uses the
 * yearDay and weekDay fields.
 * 
 * <p>
 * In IS0 8601, weeks start on Monday.
 * The first week of the year (week 1) is defined by ISO 8601 as the
 * first week with four or more of its days in the starting year.
 * Or equivalently, the week containing January 4.  Or equivalently,
 * the week with the year's first Thursday in it.
 * </p>
 * 
 * <p>
 * The week number can be calculated by counting Thursdays.  Week N
 * contains the Nth Thursday of the year.
 * </p>
 *   
 * @return the ISO week number.
 */
public Int32 getWeekNumber() {
    // Get the year day for the closest Thursday
    Int32 closestThursday = yearDay + sThursdayOffset[weekDay];

    // Year days start at 0
    if (closestThursday >= 0 && closestThursday <= 364) {
        return closestThursday / 7 + 1;
    }
    
    // The week crosses a year boundary.
    Time temp = new Time(this);
    temp.monthDay += sThursdayOffset[weekDay];
    temp.normalize(TRUE /* ignore isDst */);
    return temp.yearDay / 7 + 1;
}

/**
 * Return a string in the RFC 3339 format. 
 * <p>
 * If allDay is TRUE, expresses the time as Y-M-D</p>
 * <p>
 * Otherwise, if the timezone is UTC, expresses the time as Y-M-D-T-H-M-S UTC</p>
 * <p>
 * Otherwise the time is expressed the time as Y-M-D-T-H-M-S +- GMT</p>
 * @param allDay
 * @return string in the RFC 3339 format.
 */
public String format3339(Boolean allDay) {
    if (allDay) {
        return format(Y_M_D);
    } else if (TIMEZONE_UTC.equals(timezone)) {
        return format(Y_M_D_T_H_M_S_000_Z);
    } else {
        String base = format(Y_M_D_T_H_M_S_000);
        String sign = (gmtoff < 0) ? "-" : "+";
        Int32 offset = (Int32)Math.abs(gmtoff);
        Int32 minutes = (offset % 3600) / 60;
        Int32 hours = offset / 3600;
        
        return String.format("%s%s%02d:%02d", base, sign, hours, minutes);
    }
}

/**
 * Returns TRUE if the day of the given time is the epoch on the Julian Calendar 
 * (January 1, 1970 on the Gregorian calendar).
 *
 * @param time the time to test
 * @return TRUE if epoch.
 */
public static Boolean isEpoch(Time time) {
    Int64 millis = time.toMillis(TRUE);
    return getJulianDay(millis, 0) == EPOCH_JULIAN_DAY;
}

/**
 * Computes the Julian day number, given the UTC milliseconds
 * and the offset (in seconds) from UTC.  The Julian day for a given
 * date will be the same for every timezone.  For example, the Julian
 * day for July 1, 2008 is 2454649.  This is the same value no matter
 * what timezone is being used.  The Julian day is useful for testing
 * if two events occur on the same day and for determining the relative
 * time of an event from the present ("yesterday", "3 days ago", etc.).
 * 
 * <p>
 * Use {@link #toMillis(Boolean)} to get the milliseconds.
 * 
 * @param millis the time in UTC milliseconds
 * @param gmtoff the offset from UTC in seconds
 * @return the Julian day
 */
public static Int32 getJulianDay(Int64 millis, Int64 gmtoff) {
    Int64 offsetMillis = gmtoff * 1000;
    Int64 julianDay = (millis + offsetMillis) / DateUtils.DAY_IN_MILLIS;
    return (Int32) julianDay + EPOCH_JULIAN_DAY;
}

/**
 * <p>Sets the time from the given Julian day number, which must be based on
 * the same timezone that is set in this Time object.  The "gmtoff" field
 * need not be initialized because the given Julian day may have a different
 * GMT offset than whatever is currently stored in this Time object anyway.
 * After this method returns all the fields will be normalized and the time
 * will be set to 12am at the beginning of the given Julian day.
 * </p>
 * 
 * <p>
 * The only exception to this is if 12am does not exist for that day because
 * of daylight saving time.  For example, Cairo, Eqypt moves time ahead one
 * hour at 12am on April 25, 2008 and there are a few other places that
 * also change daylight saving time at 12am.  In those cases, the time
 * will be set to 1am.
 * </p>
 * 
 * @param julianDay the Julian day in the timezone for this Time object
 * @return the UTC milliseconds for the beginning of the Julian day
 */
public Int64 setJulianDay(Int32 julianDay) {
    // Don't bother with the GMT offset since we don't know the correct
    // value for the given Julian day.  Just get close and then adjust
    // the day.
    Int64 millis = (julianDay - EPOCH_JULIAN_DAY) * DateUtils.DAY_IN_MILLIS;
    set(millis);
    
    // Figure out how close we are to the requested Julian day.
    // We can't be off by more than a day.
    Int32 approximateDay = getJulianDay(millis, gmtoff);
    Int32 diff = julianDay - approximateDay;
    monthDay += diff;
    
    // Set the time to 12am and re-normalize.
    hour = 0;
    minute = 0;
    second = 0;
    millis = normalize(TRUE);
    return millis;
}
