#include "test.h"
#include <stdio.h>

#include "Elastos.Text.h"
#include <elastos/AutoPtr.h>
#include <elastos/System.h>


using namespace Elastos;
/*
    public void test2DigitYearStartIsCloned() throws Exception {
        // Test that get2DigitYearStart returns a clone.
        SimpleDateFormat sdf = new SimpleDateFormat();
        Date originalDate = sdf.get2DigitYearStart();
        assertNotSame(sdf.get2DigitYearStart(), originalDate);
        assertEquals(sdf.get2DigitYearStart(), originalDate);
        originalDate.setTime(0);
        assertFalse(sdf.get2DigitYearStart().equals(originalDate));
        // Test that set2DigitYearStart takes a clone.
        Date newDate = new Date();
        sdf.set2DigitYearStart(newDate);
        assertNotSame(sdf.get2DigitYearStart(), newDate);
        assertEquals(sdf.get2DigitYearStart(), newDate);
        newDate.setTime(0);
        assertFalse(sdf.get2DigitYearStart().equals(newDate));
    }
*/
int CTest::test_2DigitYearStartIsCloned(int argc, char* argv[])
{
    printf("%s %d\n", __FILE__, __LINE__);
    AutoPtr<ISimpleDateFormat> sdf;
    printf("%s %d\n", __FILE__, __LINE__);
    CSimpleDateFormat::New((ISimpleDateFormat**)&sdf);
    printf("%s %d\n", __FILE__, __LINE__);
    AutoPtr<IDate> originalDate;
    printf("%s %d\n", __FILE__, __LINE__);
    sdf->Get2DigitYearStart((IDate**)&originalDate);
    printf("%s %d\n", __FILE__, __LINE__);
    return 0;
}
