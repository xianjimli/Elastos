
#include "CDate.h"
#include <elastos/System.h>
#include <elastos/AutoPtr.h>
#include <stdio.h>
#include "CGregorianCalendar.h"

using namespace Elastos::Core;


ECode CDate::IsAfter(
    /* [in] */ IDate* date,
    /* [out] */ Boolean* isAfter)
{
    // TODO: Add your code here
    *isAfter = mMilliseconds > ((CDate*)date)->mMilliseconds;
    return NOERROR;
}

ECode CDate::IsBefore(
    /* [in] */ IDate* date,
    /* [out] */ Boolean* isBefore)
{
    // TODO: Add your code here
    *isBefore = mMilliseconds < ((CDate*)date)->mMilliseconds;
    return NOERROR;
}

ECode CDate::CompareTo(
    /* [in] */ IDate* date,
    /* [out] */ Int32* result)
{
    // TODO: Add your code here
    if (mMilliseconds < ((CDate*)date)->mMilliseconds) {
        *result = -1;
    }
    if (mMilliseconds == ((CDate*)date)->mMilliseconds) {
        *result = 0;
    }
    *result = 1;
    return NOERROR;
}

ECode CDate::GetDate(
    /* [out] */ Int32* date)
{
    AutoPtr<IGregorianCalendar> calendar;
    ECode ec = NOERROR;
    ec = CGregorianCalendar::New(mMilliseconds, (IGregorianCalendar**)&calendar);
    if (FAILED(ec)) {
        printf("%s, %d,ec = %x\n", __FILE__, __LINE__, ec);
        return ec;
    }
    calendar->Get(Calendar_DATE, date);
    return NOERROR;
}

ECode CDate::GetDay(
    /* [out] */ Int32* day)
{
    AutoPtr<IGregorianCalendar> calendar;
    CGregorianCalendar::New(mMilliseconds, (IGregorianCalendar**)&calendar);
    calendar->Get(Calendar_DAY_OF_WEEK, day);
    *day = *day - 1;
    return NOERROR;
}

ECode CDate::GetHours(
    /* [out] */ Int32* hours)
{
    AutoPtr<IGregorianCalendar> calendar;
    CGregorianCalendar::New(mMilliseconds, (IGregorianCalendar**)&calendar);
    calendar->Get(Calendar_HOUR_OF_DAY, hours);
    return NOERROR;
}

ECode CDate::GetMinutes(
    /* [out] */ Int32* minutes)
{
    // TODO: Add your code here
    AutoPtr<IGregorianCalendar> calendar;
    CGregorianCalendar::New(mMilliseconds, (IGregorianCalendar**)&calendar);
    calendar->Get(Calendar_MINUTE, minutes);
    return NOERROR;
}

ECode CDate::GetMonth(
    /* [out] */ Int32* month)
{
    // TODO: Add your code here
    AutoPtr<IGregorianCalendar> calendar;
    CGregorianCalendar::New(mMilliseconds, (IGregorianCalendar**)&calendar);
    calendar->Get(Calendar_MONTH, month);
    return NOERROR;
}

ECode CDate::GetSeconds(
    /* [out] */ Int32* seconds)
{
    // TODO: Add your code here
    AutoPtr<IGregorianCalendar> calendar;
    CGregorianCalendar::New(mMilliseconds, (IGregorianCalendar**)&calendar);
    calendar->Get(Calendar_SECOND, seconds);
    return NOERROR;
}

ECode CDate::GetTime(
    /* [out] */ Int64* time)
{
    // TODO: Add your code here
    *time = mMilliseconds;
    return NOERROR;
}

ECode CDate::GetTimezoneOffset(
    /* [out] */ Int32* timezoneOffset)
{
    // TODO: Add your code here
    AutoPtr<IGregorianCalendar> calendar;
    CGregorianCalendar::New(mMilliseconds, (IGregorianCalendar**)&calendar);
    Int32 offset, dst;
    calendar->Get(Calendar_ZONE_OFFSET, &offset);
    calendar->Get(Calendar_DST_OFFSET, &dst);
    *timezoneOffset -= (offset + dst) / 60000;
    return NOERROR;
}

ECode CDate::GetYear(
    /* [out] */ Int32* year)
{
    // TODO: Add your code here
    AutoPtr<IGregorianCalendar> calendar;
    CGregorianCalendar::New(mMilliseconds, (IGregorianCalendar**)&calendar);
    calendar->Get(Calendar_YEAR, year);
    *year = *year - 1900;
    return NOERROR;
}

ECode CDate::SetDate(
    /* [in] */ Int32 day)
{
    // TODO: Add your code here
    AutoPtr<IGregorianCalendar> calendar;
    CGregorianCalendar::New(mMilliseconds, (IGregorianCalendar**)&calendar);
    calendar->Set(Calendar_DATE, day);
    calendar->GetTimeInMillis(&mMilliseconds);
    return NOERROR;
}

ECode CDate::SetHours(
    /* [in] */ Int32 hour)
{
    // TODO: Add your code here
    AutoPtr<IGregorianCalendar> calendar;
    CGregorianCalendar::New(mMilliseconds, (IGregorianCalendar**)&calendar);
    calendar->Set(Calendar_HOUR_OF_DAY, hour);
    calendar->GetTimeInMillis(&mMilliseconds);
    return NOERROR;
}

ECode CDate::SetMinutes(
    /* [in] */ Int32 minute)
{
    // TODO: Add your code here
    AutoPtr<IGregorianCalendar> calendar;
    CGregorianCalendar::New(mMilliseconds, (IGregorianCalendar**)&calendar);
    calendar->Set(Calendar_MINUTE, minute);
    calendar->GetTimeInMillis(&mMilliseconds);
    return NOERROR;
}

ECode CDate::SetMonth(
    /* [in] */ Int32 month)
{
    // TODO: Add your code here
    AutoPtr<IGregorianCalendar> calendar;
    CGregorianCalendar::New(mMilliseconds, (IGregorianCalendar**)&calendar);
    calendar->Set(Calendar_MONTH, month);
    calendar->GetTimeInMillis(&mMilliseconds);
}

ECode CDate::SetSeconds(
    /* [in] */ Int32 second)
{
    // TODO: Add your code here
    AutoPtr<IGregorianCalendar> calendar;
    CGregorianCalendar::New(mMilliseconds, (IGregorianCalendar**)&calendar);
    calendar->Set(Calendar_SECOND, second);
    calendar->GetTimeInMillis(&mMilliseconds);
}

ECode CDate::SetTime(
    /* [in] */ Int64 milliseconds)
{
    // TODO: Add your code here
    mMilliseconds = milliseconds;
    return NOERROR;
}

ECode CDate::SetYear(
    /* [in] */ Int32 year)
{
    // TODO: Add your code here
    AutoPtr<IGregorianCalendar> calendar;
    CGregorianCalendar::New(mMilliseconds, (IGregorianCalendar**)&calendar);
    calendar->Set(Calendar_YEAR, year + 1900);
    calendar->GetTimeInMillis(&mMilliseconds);
}

ECode CDate::ToGMTString(
    /* [out] */ String* gmtStr)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CDate::ToLocaleString(
    /* [out] */ String* localeStr)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CDate::ToString(
    /* [out] */ String* str)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
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
    CGregorianCalendar::New(FALSE, (IGregorianCalendar**)&calendar);
    calendar->SetEx(1900 + year, month, day);
    calendar->GetTimeInMillis(&mMilliseconds);
    return NOERROR;
}

ECode CDate::constructor(
    /* [in] */ Int32 year,
    /* [in] */ Int32 month,
    /* [in] */ Int32 day,
    /* [in] */ Int32 hour,
    /* [in] */ Int32 minute)
{
    AutoPtr<IGregorianCalendar> calendar;
    CGregorianCalendar::New(FALSE, (IGregorianCalendar**)&calendar);
    calendar->SetEx2(1900 + year, month, day, hour, minute);
    calendar->GetTimeInMillis(&mMilliseconds);
    return NOERROR;
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
    CGregorianCalendar::New(FALSE, (IGregorianCalendar**)&calendar);
    calendar->SetEx3(1900 + year, month, day, hour, minute,second);
    calendar->GetTimeInMillis(&mMilliseconds);
    return NOERROR;
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
    return E_NOT_IMPLEMENTED;
}