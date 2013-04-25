#include "test.h"
#include <stdio.h>

#include "Elastos.Text.h"
#include <elastos/AutoPtr.h>
#include <elastos/System.h>
#include "Number.h"
#include <StringBuffer.h>

using namespace Elastos;

int CTest::test_custom_Number_gets_longValue(int argc, char* argv[])
{
    class MyNumber : public Number
    {
    public:
        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid)
        {
            return NULL;
        }

        CARAPI_(Byte) ByteValue()
        { return 0; }

        CARAPI_(Double) DoubleValue()
        { return 123; }

        CARAPI_(Float) FloatValue()
        { return 0; }

        CARAPI_(Int32) Int32Value()
        { return 0; }

        CARAPI_(Int64) Int64Value()
        { return 0L; }

        CARAPI_(String) ToString()
        { return String(NULL); }
    };

    AutoPtr<INumberFormatHelper> nfh;
    CNumberFormatHelper::AcquireSingleton((INumberFormatHelper**)&nfh);
    AutoPtr<ILocale> locale;
    CLocale::New(String("en"), String("US"), (ILocale**)&locale);
    AutoPtr<INumberFormat> nf;
    nfh->GetNumberInstanceEx(locale, (INumberFormat**)&nf);
    String result("");
    MyNumber * mynumber = new MyNumber();
    IInterface * object = reinterpret_cast<IInterface *>(mynumber->Probe(EIID_INumber));
    nf->FormatObject(object, &result);
    assert(String("123").Equals(result));
    return 0;
}

//int CTest::test_small_BigInteger_gets_longValue(int argc, char* argv[])
//{
//    class MyNumberFormat : public NumberFormat {
//    public:
//        CARAPI_(PInterface) Probe(
//            /* [in]  */ REIID riid)
//        {
//            return NULL;
//        }

//        CARAPI FormatDoubleEx(
//            /* [in] */ Double value,
//            /* [in] */ const String& buffer,
//            /* [in] */ IFieldPosition* field,
//            /* [out] */ String* result)
//        { 
//            StringBuffer b(buffer);
//            b += "double";
//            *result = b.Substring(0, b.GetLength());
//            return NOERROR;
//        }

//        CARAPI FormatInt64Ex(
//            /* [in] */ Int64 value,
//            /* [in] */ const String& buffer,
//            /* [in] */ IFieldPosition* field,
//            /* [out] */ String* result)
//        {
//            StringBuffer b(buffer);
//            b += "long";
//            *result = b.Substring(0, b.GetLength());
//            return NOERROR;
//        }

//        CARAPI ParseEx(
//            /* [in] */ const String& string,
//            /* [in] */ IParsePosition* position,
//            /* [out] */ INumber** value)
//        {
//            return E_NOT_IMPLEMENTED;
//        }
/*
        NumberFormat nf = new MyNumberFormat();
        assertEquals("long", nf.format(BigInteger.valueOf(Long.MAX_VALUE)));
        assertEquals("double", nf.format(BigInteger.valueOf(Long.MAX_VALUE).add(BigInteger.ONE)));
        assertEquals("long", nf.format(BigInteger.valueOf(Long.MIN_VALUE)));
        assertEquals("double", nf.format(BigInteger.valueOf(Long.MIN_VALUE).subtract(BigInteger.ONE)));
*/
//    };
//    return 0;
//}

int CTest::test_getIntegerInstance_ar(int argc, char* argv[])
{
    AutoPtr<INumberFormatHelper> nfh;
    CNumberFormatHelper::AcquireSingleton((INumberFormatHelper**)&nfh);
    AutoPtr<ILocale> locale;
    CLocale::New(String("ar"), (ILocale**)&locale);
    AutoPtr<INumberFormat> numberFormat;
    nfh->GetNumberInstanceEx(locale, (INumberFormat**)&numberFormat);

    AutoPtr<INumberFormat> integerFormat;
    nfh->GetNumberInstanceEx(locale, (INumberFormat**)&integerFormat);

    AutoPtr<IDecimalFormat> df = reinterpret_cast<IDecimalFormat *>(numberFormat->Probe(EIID_INumberFormat));
    String str("");
    df->ToPattern(&str);
    assert(String("#,##0.###;#,##0.###-").Equals(str));

    AutoPtr<IDecimalFormat> df2 = reinterpret_cast<IDecimalFormat *>(integerFormat->Probe(EIID_INumberFormat));
    df2->ToPattern(&str);
    assert(String("#,##0;#,##0-").Equals(str));    

    return 0;
}

int CTest::test_numberLocalization(int argc, char* argv[])
{
    AutoPtr<ILocale> arabic;
    CLocale::New(String("ar"), (ILocale**)&arabic);
    AutoPtr<INumberFormatHelper> nfh;
    CNumberFormatHelper::AcquireSingleton((INumberFormatHelper**)&nfh);
    AutoPtr<INumberFormat> nf;
    nfh->GetNumberInstanceEx(arabic, (INumberFormat**)&nf);
    AutoPtr<IDecimalFormatSymbols> dfs;
    CDecimalFormatSymbols::New(arabic, (IDecimalFormatSymbols**)&dfs);
    Char32 result;
    dfs->GetZeroDigit(&result);
    assert('\u0660' == result);
    String str("");
    nf->FormatInt64(1234567890L, &str);
    assert(String("\u0661\u066c\u0662\u0663\u0664\u066c\u0665\u0666\u0667\u066c\u0668\u0669\u0660")
        .Equals(str));
    return 0;
}

int CTest::test_10333(int argc, char* argv[])
{
    AutoPtr<INumberFormatHelper> nfh;
    CNumberFormatHelper::AcquireSingleton((INumberFormatHelper**)&nfh);
    AutoPtr<ILocale> Locale_US;
    CLocale::New(String("en"), String("US"), (ILocale**)&Locale_US);
    AutoPtr<INumberFormat> nf;
    nfh->GetPercentInstanceEx(Locale_US, (INumberFormat**)&nf);
    String str("");
    nf->FormatDouble(0.15, &str);
    assert(String("15%").Equals(str));
    assert(String("1,500%").Equals(str));
/*
        try {
            nf.format("15");
            fail();
        } catch (IllegalArgumentException expected) {
        }
*/
    return 0;
}


