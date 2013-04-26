#include "test.h"

#include "Elastos.Text.h"
#include <Elastos.IO.h>
#include <elastos/AutoPtr.h>
#include <elastos/System.h>

using namespace Elastos;

void AssertLocaleIsEquivalentToRoot(ILocale * locale)
{
    AutoPtr<IDateFormatSymbolsHelper> dfsh;
    CDateFormatSymbolsHelper::AcquireSingleton((IDateFormatSymbolsHelper**)&dfsh);
    AutoPtr<IDateFormatSymbols> dfs, dfs2;
    dfsh->GetInstanceEx(locale, (IDateFormatSymbols**)&dfs);
    AutoPtr<ILocale> Locale_ROOT;
    CLocale::New(String(""), String(""), String(""), (ILocale**)&Locale_ROOT);
    dfsh->GetInstanceEx(Locale_ROOT, (IDateFormatSymbols**)&dfs2);
    assert(dfs2 == dfs);
}

int CTest::test_getInstance_unknown_locale(int argc, char* argv[])
{
    AutoPtr<ILocale> locale;
    CLocale::New(String("xx"), String("XX"), (ILocale**)&locale);
    AssertLocaleIsEquivalentToRoot(locale);
    return 0;
}

int CTest::test_getInstance_invalid_locale(int argc, char* argv[])
{
    AutoPtr<ILocale> locale;
    CLocale::New(String("not exist language"), String("not exist country"), (ILocale**)&locale);
    AssertLocaleIsEquivalentToRoot(locale);
    return 0;
}

String FormatDate(ILocale * l, const String& fmt, IDateFormatSymbols * dfs)
{
    AutoPtr<ISimpleDateFormat> sdf;
    CSimpleDateFormat::New(fmt, l, (ISimpleDateFormat**)&sdf);
    sdf->SetDateFormatSymbols(dfs);
    AutoPtr<ITimeZoneHelper> tzh;
    CTimeZoneHelper::AcquireSingleton((ITimeZoneHelper**)&tzh);
    AutoPtr<ITimeZone> tz;
    tzh->GetTimeZone(String("UTC"), (ITimeZone**)&tz);
    sdf->SetTimeZone(tz);
    AutoPtr<IDate> date;
    CDate::New(0, (IDate**)&date);
    String str("");
    sdf->FormatDate(date, &str);
    return str;
}

int CTest::testSerialization(int argc, char* argv[])
{
    AutoPtr<ILocale> pl;
    CLocale::New(String("pl"), (ILocale**)&pl);
    AutoPtr<IDateFormatSymbols> originalDfs;
    CDateFormatSymbols::New(pl, (IDateFormatSymbols**)&originalDfs);

    AutoPtr<IByteArrayOutputStream> out;
    CByteArrayOutputStream::New((IByteArrayOutputStream**)&out);

/*
    new ObjectOutputStream(out).writeObject(originalDfs);
    byte[] bytes = out.toByteArray();

    // Deserialize...
    ObjectInputStream in = new ObjectInputStream(new ByteArrayInputStream(bytes));
    DateFormatSymbols deserializedDfs = (DateFormatSymbols) in.readObject();
    assertEquals(-1, in.read());

    // The two objects should claim to be equal, even though they aren't really.
    assertEquals(originalDfs, deserializedDfs);

    // The original differentiates between regular month names and stand-alone month names...
    assertEquals("stycznia", formatDate(pl, "MMMM", originalDfs));
    assertEquals("stycze\u0144", formatDate(pl, "LLLL", originalDfs));

    // Whereas the deserialized object can't, because it lost the strings...
    assertEquals("stycznia", formatDate(pl, "MMMM", deserializedDfs));
    assertEquals("stycznia", formatDate(pl, "LLLL", deserializedDfs));
*/
    return 0;
}




