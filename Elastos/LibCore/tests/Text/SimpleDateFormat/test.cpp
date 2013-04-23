#include "test.h"
#include <stdio.h>

#include "Elastos.Text.h"
#include <elastos/AutoPtr.h>
#include <elastos/System.h>


using namespace Elastos;

int CTest::test_2DigitYearStartIsCloned(int argc, char* argv[])
{
    AutoPtr<ISimpleDateFormat> sdf;
    printf("%s %d\n", __FILE__, __LINE__);
    CSimpleDateFormat::New((ISimpleDateFormat**)&sdf);
    AutoPtr<IDate> originalDate;
    sdf->Get2DigitYearStart((IDate**)&originalDate);
    originalDate->SetTime(0);
    AutoPtr<IDate> originalDate2;
    sdf->Get2DigitYearStart((IDate**)&originalDate2);
    assert(originalDate2 != originalDate);
    AutoPtr<IDate> newDate;
    CDate::New((IDate**)&newDate);
    sdf->Set2DigitYearStart(newDate);
    AutoPtr<IDate> originalDate3;
    sdf->Get2DigitYearStart((IDate**)&originalDate3);
    assert(originalDate3 == newDate);
    newDate->SetTime(0);
    AutoPtr<IDate> originalDate4;
    sdf->Get2DigitYearStart((IDate**)&originalDate4);
    assert(originalDate4 != newDate);
    return 0;
}

String formatDate(ILocale* l, const String& fmt)
{
    AutoPtr<ISimpleDateFormat> sdf;
    CSimpleDateFormat::New(fmt, l, (ISimpleDateFormat**)&sdf);
    AutoPtr<IDate> date;
    CDate::New(0, (IDate**)&date);
    String str("");
    sdf->FormatDate(date, &str);
    return str;
}

AutoPtr<ICalendar> parseDate(ILocale* l, const String& fmt, const String& value)
{
    AutoPtr<ISimpleDateFormat> sdf;
    CSimpleDateFormat::New(fmt, l, (ISimpleDateFormat**)&sdf);
    AutoPtr<IParsePosition> pp;
    CParsePosition::New(0, (IParsePosition**)&pp);
    AutoPtr<IDate> d;
    sdf->ParseEx(value, pp, (IDate**)&d);
    if (d == NULL) {
        printf("fail\n");
    }
    AutoPtr<ITimeZoneHelper> tzh;
    CTimeZoneHelper::AcquireSingleton((ITimeZoneHelper**)&tzh);
    AutoPtr<ITimeZone> tz;
    tzh->GetTimeZone(String("UTC"), (ITimeZone**)&tz);

    AutoPtr<ICalendarHelper> ch;
    CCalendarHelper::AcquireSingleton((ICalendarHelper**)&ch);
    AutoPtr<ICalendar> c;
    ch->GetInstanceEx1(tz, (ICalendar**)&c);

    c->SetTime(d);
    return c;
}

int CTest::test_StandAloneNames(int argc, char* argv[])
{
    AutoPtr<ILocale> en;
    CLocale::New(String("en"), String(""), (ILocale**)&en);
    AutoPtr<ILocale> pl;
    CLocale::New(String("pl"), (ILocale**)&pl);
    AutoPtr<ILocale> ru;
    CLocale::New(String("ru"), (ILocale**)&ru);
    assert(String("January") == formatDate(en, String("MMMM")));
    assert(String("January") == formatDate(en, String("LLLL")));
    assert(String("stycznia") == formatDate(pl, String("MMMM")));
    assert(String("stycze\u0144") == formatDate(pl, String("LLLL")));

    assert(String("Thursday") == formatDate(en, String("EEEE")));
    assert(String("Thursday") == formatDate(en, String("cccc")));
    assert(String("\u0447\u0435\u0442\u0432\u0435\u0440\u0433") == formatDate(ru, String("EEEE")));
    assert(String("\u0427\u0435\u0442\u0432\u0435\u0440\u0433") == formatDate(ru, String("cccc")));

    Int32 value;
    parseDate(en, String("yyyy-MMMM-dd"), String("1980-June-12"))->Get(Calendar_MONTH, &value);
    assert(Calendar_JUNE == value);
    parseDate(en, String("yyyy-LLLL-dd"), String("1980-June-12"))->Get(Calendar_MONTH, &value);
    assert(Calendar_JUNE == value);
    parseDate(en, String("yyyy-MMMM-dd"), String("1980-czerwca-12"))->Get(Calendar_MONTH, &value);
    assert(Calendar_JUNE == value);
    parseDate(en, String("yyyy-LLLL-dd"), String("1980-czerwiec-12"))->Get(Calendar_MONTH, &value);
    assert(Calendar_JUNE == value);

    parseDate(en, String("EEEE"), String("Tuesday"))->Get(Calendar_DAY_OF_WEEK, &value);
    assert(Calendar_TUESDAY == value);
    parseDate(en, String("cccc"), String("Tuesday"))->Get(Calendar_DAY_OF_WEEK, &value);
    assert(Calendar_TUESDAY == value);
    parseDate(ru, String("EEEE"), String("\u0432\u0442\u043e\u0440\u043d\u0438\u043a"))->Get(Calendar_DAY_OF_WEEK, &value);
    assert(Calendar_TUESDAY == value);
    parseDate(ru, String("cccc"), String("\u0412\u0442\u043e\u0440\u043d\u0438\u043a"))->Get(Calendar_DAY_OF_WEEK, &value);
    assert(Calendar_TUESDAY == value);

    return 0;
}

