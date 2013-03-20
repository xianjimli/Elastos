
#include "CCalendarHelper.h"

ECode CCalendarHelper::GetInstance(
    /* [out] */ ICalendar ** ppCalenar)
{
    // TODO: Add your code here
    return Calendar::GetInstance(ppCalenar);
}

ECode CCalendarHelper::GetInstanceEx(
    /* [in] */ ILocale * pLocale,
    /* [out] */ ICalendar ** ppCalenar)
{
    // TODO: Add your code here
    return Calendar::GetInstance(pLocale, ppCalenar);
}

ECode CCalendarHelper::GetInstanceEx1(
    /* [in] */ ITimeZone * pTimezone,
    /* [out] */ ICalendar ** ppCalenar)
{
    // TODO: Add your code here
    return Calendar::GetInstance(pTimezone, ppCalenar);
}

ECode CCalendarHelper::GetInstanceEx2(
    /* [in] */ ITimeZone * pTimezone,
    /* [in] */ ILocale * pLocale,
    /* [out] */ ICalendar ** ppCalenar)
{
    // TODO: Add your code here
    return Calendar::GetInstance(pTimezone, pLocale, ppCalenar);
}

