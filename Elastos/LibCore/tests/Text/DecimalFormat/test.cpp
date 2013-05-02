#include "test.h"

#include "Elastos.Text.h"
#include <elastos/AutoPtr.h>
#include <elastos/System.h>

using namespace Elastos;

int CTest::test_exponentSeparator(int argc, char* argv[])
{
    AutoPtr<IDecimalFormat> df;
    CDecimalFormat::New(String("0E0"), (IDecimalFormat**)&df);
    String str;
    df->FormatDouble(12345., &str);
    assert(String("1E4").Equals(str));

    AutoPtr<IDecimalFormatSymbols> dfs;
    df->GetDecimalFormatSymbols((IDecimalFormatSymbols**)&dfs);
    dfs->SetExponentSeparator(String("-useless-api-"));
    df->SetDecimalFormatSymbols(dfs);
    df->FormatDouble(12345., &str);
    assert(String("1-useless-api-4").Equals(str));

    return 0;
}

int CTest::test_setMaximumFractionDigitsAffectsRoundingMode(int argc, char* argv[])
{
/*
        DecimalFormat df = (DecimalFormat) DecimalFormat.getInstance(Locale.US);
        df.setMaximumFractionDigits(0);
        df.setRoundingMode(RoundingMode.HALF_UP);
        assertEquals("-0", df.format(-0.2));
        df.setMaximumFractionDigits(1);
        assertEquals("-0.2", df.format(-0.2));
*/
    return 0;
}

int CTest::test_setMaximumIntegerDigits(int argc, char* argv[])
{
    AutoPtr<INumberFormatHelper> nfh;
    CNumberFormatHelper::AcquireSingleton((INumberFormatHelper**)&nfh);
    AutoPtr<INumberFormat> numberFormat;
    AutoPtr<ILocale> Locale_US;
    CLocale::New(String("en"), String("US"), (ILocale**)&Locale_US);
    nfh->GetNumberInstanceEx(Locale_US, (INumberFormat**)&numberFormat);
    numberFormat->SetGroupingUsed(FALSE);
    numberFormat->SetMinimumIntegerDigits(400);
    String str;
    numberFormat->FormatInt64(123L, &str);
    assert(309 == str.GetLength());
    //assertEquals(309, numberFormat.format(BigInteger.valueOf(123)).length());
    return 0;
}

/*

    // Regression test for http://b/1897917: BigDecimal does not take into account multiplier.
    public void testBigDecimalBug1897917() {
        // For example. the BigDecimal 0.17 formatted in PercentInstance is 0% instead of 17%:
        NumberFormat pf = NumberFormat.getPercentInstance();
        assertEquals("17%", pf.format(BigDecimal.valueOf(0.17)));

        // Test long decimal formatted in PercentInstance with various fractions.
        String longDec = "11.2345678901234567890123456789012345678901234567890";
        BigDecimal bd = new BigDecimal(longDec);
        assertBigDecimalWithFraction(bd, "1,123.46%", 2);
        assertBigDecimalWithFraction(bd, "1,123.45678901%", 8);
        assertBigDecimalWithFraction(bd, "1,123.4567890123%", 10);
        assertBigDecimalWithFraction(bd, "1,123.45678901234567890123%", 20);
        assertBigDecimalWithFraction(bd, "1,123.456789012345678901234567890123%", 30);

        // Test trailing zeros.
        assertDecFmtWithMultiplierAndFraction("3333.33333333", 3, 4, "10,000");
        assertDecFmtWithMultiplierAndFraction("3333.33333333", -3, 4, "-10,000");
        assertDecFmtWithMultiplierAndFraction("0.00333333", 3, 4, "0.01");
        assertDecFmtWithMultiplierAndFraction("3330000000000000000000000000000000", 3, 4,
                "9,990,000,000,000,000,000,000,000,000,000,000");
    }

    public void testBigDecimalTestBigIntWithMultiplier() {
        // Big integer tests.
        assertDecFmtWithMultiplierAndFraction("123456789012345", 10, 0,
                "1,234,567,890,123,450");
        assertDecFmtWithMultiplierAndFraction("12345678901234567890", 10, 0,
                "123,456,789,012,345,678,900");
        assertDecFmtWithMultiplierAndFraction("98765432109876543210987654321", 10, 0,
                "987,654,321,098,765,432,109,876,543,210");

        assertDecFmtWithMultiplierAndFraction("123456789012345", -10, 0,
                "-1,234,567,890,123,450");
        assertDecFmtWithMultiplierAndFraction("12345678901234567890", -10, 0,
                "-123,456,789,012,345,678,900");
        assertDecFmtWithMultiplierAndFraction("98765432109876543210987654321", -10, 0,
                "-987,654,321,098,765,432,109,876,543,210");
    }

    public void testBigDecimalICUConsistency() {
        DecimalFormat df = (DecimalFormat) NumberFormat.getInstance();
        df.setMaximumFractionDigits(2);
        df.setMultiplier(2);
        assertEquals(df.format(BigDecimal.valueOf(0.16)),
        df.format(BigDecimal.valueOf(0.16).doubleValue()));
        assertEquals(df.format(BigDecimal.valueOf(0.0293)),
        df.format(BigDecimal.valueOf(0.0293).doubleValue()));
        assertEquals(df.format(BigDecimal.valueOf(0.006)),
        df.format(BigDecimal.valueOf(0.006).doubleValue()));
        assertEquals(df.format(BigDecimal.valueOf(0.00283)),
        df.format(BigDecimal.valueOf(0.00283).doubleValue()));
        assertEquals(df.format(BigDecimal.valueOf(1.60)),
        df.format(BigDecimal.valueOf(1.60).doubleValue()));
        assertEquals(df.format(BigDecimal.valueOf(15)),
        df.format(BigDecimal.valueOf(15).doubleValue()));
        assertEquals(df.format(BigDecimal.valueOf(170)),
        df.format(BigDecimal.valueOf(170).doubleValue()));
        assertEquals(df.format(BigDecimal.valueOf(234.56)),
        df.format(BigDecimal.valueOf(234.56).doubleValue()));
        assertEquals(df.format(BigDecimal.valueOf(0)),
        df.format(BigDecimal.valueOf(0).doubleValue()));
        assertEquals(df.format(BigDecimal.valueOf(-1)),
        df.format(BigDecimal.valueOf(-1).doubleValue()));
        assertEquals(df.format(BigDecimal.valueOf(-10000)),
        df.format(BigDecimal.valueOf(-10000).doubleValue()));
        assertEquals(df.format(BigDecimal.valueOf(-0.001)),
        df.format(BigDecimal.valueOf(-0.001).doubleValue()));
        assertEquals(df.format(BigDecimal.valueOf(1234567890.1234567)),
        df.format(BigDecimal.valueOf(1234567890.1234567).doubleValue()));
        assertEquals(df.format(BigDecimal.valueOf(1.234567E100)),
        df.format(BigDecimal.valueOf(1.234567E100).doubleValue()));
    }

    private void assertBigDecimalWithFraction(BigDecimal bd, String expectedResult, int fraction) {
        NumberFormat pf = NumberFormat.getPercentInstance();
        pf.setMaximumFractionDigits(fraction);
        assertEquals(expectedResult, pf.format(bd));
    }

    private void assertDecFmtWithMultiplierAndFraction(String value, int multiplier, int fraction, String expectedResult) {
        DecimalFormat df = (DecimalFormat)NumberFormat.getInstance();
        df.setMultiplier(multiplier);
        df.setMaximumFractionDigits(fraction);
        BigDecimal d = new BigDecimal(value);
        assertEquals(expectedResult, df.format(d));
    }
*/


