
#include "cmdef.h"
#include "CDate.h"
#include "CGregorianCalendar.h"
#include <elastos/System.h>
#include <elastos/AutoPtr.h>

using namespace Elastos::Core;


Int32 CDate::sCreationYear;// = new Date().getYear();

const CString CDate::sDayOfWeekNames[] =
            { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
const CString CDate::sMonthNames[] =
            { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

ECode CDate::IsAfter(
    /* [in] */ IDate* date,
    /* [out] */ Boolean* isAfter)
{
    VALIDATE_NOT_NULL(isAfter);

    Int64 otherTime;
    date->GetTime(&otherTime);
    *isAfter = mMilliseconds > otherTime;
    return NOERROR;
}

ECode CDate::IsBefore(
    /* [in] */ IDate* date,
    /* [out] */ Boolean* isBefore)
{
    VALIDATE_NOT_NULL(isBefore);

    Int64 otherTime;
    date->GetTime(&otherTime);
    *isBefore = mMilliseconds < otherTime;
    return NOERROR;
}

ECode CDate::CompareTo(
    /* [in] */ IDate* date,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    Int64 otherTime;
    date->GetTime(&otherTime);
    if (mMilliseconds < otherTime) {
        *result = -1;
    }
    if (mMilliseconds == otherTime) {
        *result = 0;
    }
    *result = 1;
    return NOERROR;
}

ECode CDate::GetDate(
    /* [out] */ Int32* date)
{
    VALIDATE_NOT_NULL(date);

    AutoPtr<IGregorianCalendar> calendar;
    FAIL_RETURN(CGregorianCalendar::New(mMilliseconds, (IGregorianCalendar**)&calendar));
    return calendar->Get(Calendar_DATE, date);
}

ECode CDate::GetDay(
    /* [out] */ Int32* day)
{
    VALIDATE_NOT_NULL(day);

    AutoPtr<IGregorianCalendar> calendar;
    FAIL_RETURN(CGregorianCalendar::New(mMilliseconds, (IGregorianCalendar**)&calendar));
    FAIL_RETURN(calendar->Get(Calendar_DAY_OF_WEEK, day));
    *day = *day - 1;
    return NOERROR;
}

ECode CDate::GetHours(
    /* [out] */ Int32* hours)
{
    VALIDATE_NOT_NULL(hours);

    AutoPtr<IGregorianCalendar> calendar;
    FAIL_RETURN(CGregorianCalendar::New(mMilliseconds, (IGregorianCalendar**)&calendar));
    return calendar->Get(Calendar_HOUR_OF_DAY, hours);
}

ECode CDate::GetMinutes(
    /* [out] */ Int32* minutes)
{
    VALIDATE_NOT_NULL(minutes);

    AutoPtr<IGregorianCalendar> calendar;
    FAIL_RETURN(CGregorianCalendar::New(mMilliseconds, (IGregorianCalendar**)&calendar));
    return calendar->Get(Calendar_MINUTE, minutes);
}

ECode CDate::GetMonth(
    /* [out] */ Int32* month)
{
    VALIDATE_NOT_NULL(month);

    AutoPtr<IGregorianCalendar> calendar;
    FAIL_RETURN(CGregorianCalendar::New(mMilliseconds, (IGregorianCalendar**)&calendar));
    return calendar->Get(Calendar_MONTH, month);
}

ECode CDate::GetSeconds(
    /* [out] */ Int32* seconds)
{
    VALIDATE_NOT_NULL(seconds);

    AutoPtr<IGregorianCalendar> calendar;
    FAIL_RETURN(CGregorianCalendar::New(mMilliseconds, (IGregorianCalendar**)&calendar));
    return calendar->Get(Calendar_SECOND, seconds);
}

ECode CDate::GetTime(
    /* [out] */ Int64* time)
{
    VALIDATE_NOT_NULL(time);

    *time = mMilliseconds;
    return NOERROR;
}

ECode CDate::GetTimezoneOffset(
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);

    AutoPtr<IGregorianCalendar> calendar;
    FAIL_RETURN(CGregorianCalendar::New(mMilliseconds, (IGregorianCalendar**)&calendar));
    Int32 zoneOff, dstOff;
    calendar->Get(Calendar_ZONE_OFFSET, &zoneOff);
    calendar->Get(Calendar_DST_OFFSET, &dstOff);
    *offset -= (zoneOff + dstOff) / 60000;
    return NOERROR;
}

ECode CDate::GetYear(
    /* [out] */ Int32* year)
{
    VALIDATE_NOT_NULL(year);

    AutoPtr<IGregorianCalendar> calendar;
    FAIL_RETURN(CGregorianCalendar::New(mMilliseconds, (IGregorianCalendar**)&calendar));
    FAIL_RETURN(calendar->Get(Calendar_YEAR, year));
    *year = *year - 1900;
    return NOERROR;
}

Int32 CDate::Parse(
    /* [in] */ const String& string,
    /* [in] */ const ArrayOf<String>& array)
{
    // for (int i = 0, alength = array.length, slength = string.length(); i < alength; i++) {
    //     if (string.regionMatches(true, 0, array[i], 0, slength)) {
    //         return i;
    //     }
    // }
    return -1;
}

ECode CDate::Parse(
    /* [in] */ const String& string,
    /* [out] */ Int64* time)
{
    VALIDATE_NOT_NULL(time);

    // if (string == null) {
    //     throw new IllegalArgumentException("The string argument is null");
    // }

    // char sign = 0;
    // int commentLevel = 0;
    // int offset = 0, length = string.length(), state = 0;
    // int year = -1, month = -1, date = -1;
    // int hour = -1, minute = -1, second = -1, zoneOffset = 0, minutesOffset = 0;
    // boolean zone = false;
    // final int PAD = 0, LETTERS = 1, NUMBERS = 2;
    // StringBuilder buffer = new StringBuilder();

    // while (offset <= length) {
    //     char next = offset < length ? string.charAt(offset) : '\r';
    //     offset++;

    //     if (next == '(') {
    //         commentLevel++;
    //     }
    //     if (commentLevel > 0) {
    //         if (next == ')') {
    //             commentLevel--;
    //         }
    //         if (commentLevel == 0) {
    //             next = ' ';
    //         } else {
    //             continue;
    //         }
    //     }

    //     int nextState = PAD;
    //     if ('a' <= next && next <= 'z' || 'A' <= next && next <= 'Z') {
    //         nextState = LETTERS;
    //     } else if ('0' <= next && next <= '9') {
    //         nextState = NUMBERS;
    //     } else if (!Character.isSpace(next) && ",+-:/".indexOf(next) == -1) {
    //         throw new IllegalArgumentException();
    //     }

    //     if (state == NUMBERS && nextState != NUMBERS) {
    //         int digit = Integer.parseInt(buffer.toString());
    //         buffer.setLength(0);
    //         if (sign == '+' || sign == '-') {
    //             if (zoneOffset == 0) {
    //                 zone = true;
    //                 if (next == ':') {
    //                     minutesOffset = sign == '-' ? -Integer
    //                             .parseInt(string.substring(offset,
    //                                     offset + 2)) : Integer
    //                             .parseInt(string.substring(offset,
    //                                     offset + 2));
    //                     offset += 2;
    //                 }
    //                 zoneOffset = sign == '-' ? -digit : digit;
    //                 sign = 0;
    //             } else {
    //                 throw new IllegalArgumentException();
    //             }
    //         } else if (digit >= 70) {
    //             if (year == -1
    //                     && (Character.isSpace(next) || next == ','
    //                             || next == '/' || next == '\r')) {
    //                 year = digit;
    //             } else {
    //                 throw new IllegalArgumentException();
    //             }
    //         } else if (next == ':') {
    //             if (hour == -1) {
    //                 hour = digit;
    //             } else if (minute == -1) {
    //                 minute = digit;
    //             } else {
    //                 throw new IllegalArgumentException();
    //             }
    //         } else if (next == '/') {
    //             if (month == -1) {
    //                 month = digit - 1;
    //             } else if (date == -1) {
    //                 date = digit;
    //             } else {
    //                 throw new IllegalArgumentException();
    //             }
    //         } else if (Character.isSpace(next) || next == ','
    //                 || next == '-' || next == '\r') {
    //             if (hour != -1 && minute == -1) {
    //                 minute = digit;
    //             } else if (minute != -1 && second == -1) {
    //                 second = digit;
    //             } else if (date == -1) {
    //                 date = digit;
    //             } else if (year == -1) {
    //                 year = digit;
    //             } else {
    //                 throw new IllegalArgumentException();
    //             }
    //         } else if (year == -1 && month != -1 && date != -1) {
    //             year = digit;
    //         } else {
    //             throw new IllegalArgumentException();
    //         }
    //     } else if (state == LETTERS && nextState != LETTERS) {
    //         String text = buffer.toString().toUpperCase();
    //         buffer.setLength(0);
    //         if (text.length() == 1) {
    //             throw new IllegalArgumentException();
    //         }
    //         if (text.equals("AM")) {
    //             if (hour == 12) {
    //                 hour = 0;
    //             } else if (hour < 1 || hour > 12) {
    //                 throw new IllegalArgumentException();
    //             }
    //         } else if (text.equals("PM")) {
    //             if (hour == 12) {
    //                 hour = 0;
    //             } else if (hour < 1 || hour > 12) {
    //                 throw new IllegalArgumentException();
    //             }
    //             hour += 12;
    //         } else {
    //             DateFormatSymbols symbols = new DateFormatSymbols(Locale.US);
    //             String[] weekdays = symbols.getWeekdays(), months = symbols
    //                     .getMonths();
    //             int value;
    //             if (parse(text, weekdays) != -1) {/* empty */
    //             } else if (month == -1 && (month = parse(text, months)) != -1) {/* empty */
    //             } else if (text.equals("GMT") || text.equals("UT") || text.equals("UTC")) {
    //                 zone = true;
    //                 zoneOffset = 0;
    //             } else if ((value = zone(text)) != 0) {
    //                 zone = true;
    //                 zoneOffset = value;
    //             } else {
    //                 throw new IllegalArgumentException();
    //             }
    //         }
    //     }

    //     if (next == '+' || (year != -1 && next == '-')) {
    //         sign = next;
    //     } else if (!Character.isSpace(next) && next != ','
    //             && nextState != NUMBERS) {
    //         sign = 0;
    //     }

    //     if (nextState == LETTERS || nextState == NUMBERS) {
    //         buffer.append(next);
    //     }
    //     state = nextState;
    // }

    // if (year != -1 && month != -1 && date != -1) {
    //     if (hour == -1) {
    //         hour = 0;
    //     }
    //     if (minute == -1) {
    //         minute = 0;
    //     }
    //     if (second == -1) {
    //         second = 0;
    //     }
    //     if (year < (creationYear - 80)) {
    //         year += 2000;
    //     } else if (year < 100) {
    //         year += 1900;
    //     }
    //     minute -= minutesOffset;
    //     if (zone) {
    //         if (zoneOffset >= 24 || zoneOffset <= -24) {
    //             hour -= zoneOffset / 100;
    //             minute -= zoneOffset % 100;
    //         } else {
    //             hour -= zoneOffset;
    //         }
    //         return UTC(year - 1900, month, date, hour, minute, second);
    //     }
    //     return new Date(year - 1900, month, date, hour, minute, second)
    //             .getTime();
    // }
    // throw new IllegalArgumentException();
    return E_NOT_IMPLEMENTED;
}

ECode CDate::SetDate(
    /* [in] */ Int32 day)
{
    AutoPtr<IGregorianCalendar> calendar;
    FAIL_RETURN(CGregorianCalendar::New(mMilliseconds, (IGregorianCalendar**)&calendar));
    FAIL_RETURN(calendar->Set(Calendar_DATE, day));
    return calendar->GetTimeInMillis(&mMilliseconds);
}

ECode CDate::SetHours(
    /* [in] */ Int32 hour)
{
    AutoPtr<IGregorianCalendar> calendar;
    FAIL_RETURN(CGregorianCalendar::New(mMilliseconds, (IGregorianCalendar**)&calendar));
    FAIL_RETURN(calendar->Set(Calendar_HOUR_OF_DAY, hour));
    return calendar->GetTimeInMillis(&mMilliseconds);
}

ECode CDate::SetMinutes(
    /* [in] */ Int32 minute)
{
    AutoPtr<IGregorianCalendar> calendar;
    FAIL_RETURN(CGregorianCalendar::New(mMilliseconds, (IGregorianCalendar**)&calendar));
    FAIL_RETURN(calendar->Set(Calendar_MINUTE, minute));
    return calendar->GetTimeInMillis(&mMilliseconds);
}

ECode CDate::SetMonth(
    /* [in] */ Int32 month)
{
    AutoPtr<IGregorianCalendar> calendar;
    FAIL_RETURN(CGregorianCalendar::New(mMilliseconds, (IGregorianCalendar**)&calendar));
    FAIL_RETURN(calendar->Set(Calendar_MONTH, month));
    return calendar->GetTimeInMillis(&mMilliseconds);
}

ECode CDate::SetSeconds(
    /* [in] */ Int32 second)
{
    AutoPtr<IGregorianCalendar> calendar;
    FAIL_RETURN(CGregorianCalendar::New(mMilliseconds, (IGregorianCalendar**)&calendar));
    FAIL_RETURN(calendar->Set(Calendar_SECOND, second));
    return calendar->GetTimeInMillis(&mMilliseconds);
}

ECode CDate::SetTime(
    /* [in] */ Int64 milliseconds)
{
    mMilliseconds = milliseconds;
    return NOERROR;
}

ECode CDate::SetYear(
    /* [in] */ Int32 year)
{
    AutoPtr<IGregorianCalendar> calendar;
    FAIL_RETURN(CGregorianCalendar::New(mMilliseconds, (IGregorianCalendar**)&calendar));
    FAIL_RETURN(calendar->Set(Calendar_YEAR, year + 1900));
    return calendar->GetTimeInMillis(&mMilliseconds);
}

ECode CDate::ToGMTString(
    /* [out] */ String* gmtStr)
{
    // TODO: why does this insert the year manually instead of using one SimpleDateFormat?
    // SimpleDateFormat format1 = new SimpleDateFormat("d MMM ", Locale.US);
    // SimpleDateFormat format2 = new SimpleDateFormat(" HH:mm:ss 'GMT'", Locale.US);
    // TimeZone gmtZone = TimeZone.getTimeZone("GMT");
    // format1.setTimeZone(gmtZone);
    // format2.setTimeZone(gmtZone);
    // GregorianCalendar gc = new GregorianCalendar(gmtZone);
    // gc.setTimeInMillis(milliseconds);
    // return format1.format(this) + gc.get(Calendar.YEAR)
    //         + format2.format(this);
    return E_NOT_IMPLEMENTED;
}

ECode CDate::ToLocaleString(
    /* [out] */ String* locale)
{
    VALIDATE_NOT_NULL(locale);

    // return DateFormat.getDateTimeInstance().format(this);
    return E_NOT_IMPLEMENTED;
}

ECode CDate::ToString(
    /* [out] */ String* str)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

String CDate::ToTwoDigits(
    /* [in] */ Int32 n)
{
    // if (n >= 10) {
    //     return Integer.toString(n);
    // } else {
    //     return "0" + n;
    // }
    return String(NULL);
}

ECode CDate::constructor()
{
    return constructor(System::GetCurrentTimeMillis());
}

ECode CDate::constructor(
    /* [in] */ Int32 year,
    /* [in] */ Int32 month,
    /* [in] */ Int32 day)
{
    AutoPtr<IGregorianCalendar> calendar;
    FAIL_RETURN(CGregorianCalendar::New(FALSE, (IGregorianCalendar**)&calendar));
    FAIL_RETURN(calendar->SetEx(1900 + year, month, day));
    return calendar->GetTimeInMillis(&mMilliseconds);
}

ECode CDate::constructor(
    /* [in] */ Int32 year,
    /* [in] */ Int32 month,
    /* [in] */ Int32 day,
    /* [in] */ Int32 hour,
    /* [in] */ Int32 minute)
{
    AutoPtr<IGregorianCalendar> calendar;
    FAIL_RETURN(CGregorianCalendar::New(FALSE, (IGregorianCalendar**)&calendar));
    FAIL_RETURN(calendar->SetEx2(1900 + year, month, day, hour, minute));
    return calendar->GetTimeInMillis(&mMilliseconds);
}

ECode CDate::constructor(
    /* [in] */ Int32 year,
    /* [in] */ Int32 month,
    /* [in] */ Int32 day,
    /* [in] */ Int32 hour,
    /* [in] */ Int32 minute,
    /* [in] */ Int32 second)
{
    AutoPtr<IGregorianCalendar> calendar;
    FAIL_RETURN(CGregorianCalendar::New(FALSE, (IGregorianCalendar**)&calendar));
    FAIL_RETURN(calendar->SetEx3(1900 + year, month, day, hour, minute,second));
    return calendar->GetTimeInMillis(&mMilliseconds);
}

ECode CDate::constructor(
    /* [in] */ Int64 milliseconds)
{
    mMilliseconds = milliseconds;
    return NOERROR;
}

ECode CDate::constructor(
    /* [in] */ const String& string)
{
    // milliseconds = parse(string);
    return E_NOT_IMPLEMENTED;
}
