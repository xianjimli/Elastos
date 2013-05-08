#ifndef __PATTTERN_TEST_H
#define __PATTTERN_TEST_H


#include <elastos.h>
using namespace Elastos;

#define TEST(a, x) a.test_##x

class CTest {
public:
    CTest(){};
    ~CTest(){};

public:
    int test_applyPattern(int argc, char* argv[]);
    int test_formatToCharacterIterator(int argc, char* argv[]);
    int test_getLocale(int argc, char* argv[]);
    int test_setFormat(int argc, char* argv[]);

private:
};


#endif //__PATTTERN_TEST_H
