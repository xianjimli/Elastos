
#include <Elastos.Text.h>
#include <elastos/AutoPtr.h>
#include <stdio.h>

using namespace Elastos;

// typedef int (CTest::*PTestEntry)(int argc, char *argv[]);

// PTestEntry TestEntry[] =
// {
//     &CTest::test_custom_Number_gets_longValue,
//     //&CTest::test_small_BigInteger_gets_longValue,
//     &CTest::test_getIntegerInstance_ar,
//     &CTest::test_numberLocalization,
//     &CTest::test_10333,

// };


#define ASSERT_SUCCEEDED(expr) assert(SUCCEEDED(expr));

int main(int argc, char *argv[])
{
//     if (argc == 1) {
//         printf("*ERROR* Require test case number\n");
//         return -1;
//     }

//     int nIndex = atoi(argv[1]) - 1;
//     int nLength = sizeof(TestEntry) / sizeof(TestEntry[0]);

//     if (nIndex < 0 || nIndex >= nLength) {
//         printf("*ERROR* Invalid testing case number\n");
//         return -1;
//     }

// //    TPINIT;

//     CTest TestObject;

//     int nQuitCode = (TestObject.*TestEntry[nIndex])(argc, argv);

// //    TPEND;

//     return nQuitCode;

    Double percent = 0;

    printf("==== File: %s, Line: %d ====\n", __FILE__, __LINE__);
    AutoPtr<INumberFormatHelper> helper;
    ASSERT_SUCCEEDED(CNumberFormatHelper::AcquireSingleton((INumberFormatHelper**)&helper));

    printf("==== File: %s, Line: %d ====\n", __FILE__, __LINE__);
    AutoPtr<INumberFormat> format;
    ASSERT_SUCCEEDED(helper->GetPercentInstance((INumberFormat**)&format));

    printf("==== File: %s, Line: %d ====\n", __FILE__, __LINE__);
    String result;
    ASSERT_SUCCEEDED(format->FormatDouble(percent, &result));
    printf("==== File: %s, Line: %d ====\n", __FILE__, __LINE__);
    return 0;
}
