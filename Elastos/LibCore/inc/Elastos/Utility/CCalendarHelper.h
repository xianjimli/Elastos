
#ifndef __CCALENDARHELPER_H__
#define __CCALENDARHELPER_H__

#include "_CCalendarHelper.h"
#include "Calendar.h"

CarClass(CCalendarHelper)
{
public:
    CARAPI GetInstance(
        /* [out] */ ICalendar ** ppCalenar);

    CARAPI GetInstanceEx(
        /* [in] */ ILocale * pLocale,
        /* [out] */ ICalendar ** ppCalenar);

    CARAPI GetInstanceEx1(
        /* [in] */ ITimeZone * pTimezone,
        /* [out] */ ICalendar ** ppCalenar);

    CARAPI GetInstanceEx2(
        /* [in] */ ITimeZone * pTimezone,
        /* [in] */ ILocale * pLocale,
        /* [out] */ ICalendar ** ppCalenar);

private:
    // TODO: Add your private member variables here.
};

#endif // __CCALENDARHELPER_H__
