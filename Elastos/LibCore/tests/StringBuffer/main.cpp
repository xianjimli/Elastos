
#include "StringBuffer.h"
#include <stdio.h>
#include <elastos.h>

using namespace Elastos;
using namespace Elastos::System;

void test_constructor()
{
    printf("<%d>start test_constructor()\n", __LINE__);

    StringBuffer str;
    printf("<%d>%s\n", __LINE__, (const char*)str);

    StringBuffer str1(NULL);
    printf("<%d>%s\n", __LINE__, (const char*)str1);

    StringBuffer str2("Hello World!");
    printf("<%d>%s\n", __LINE__, (const char*)str2);

    StringBuffer str3((char*)"Hello World!");
    printf("<%d>%s\n", __LINE__, (const char*)str3);

    StringBuffer str4(String("Hello World!"));
    printf("<%d>%s\n", __LINE__, (const char*)str4);

    const String s = String("Hello World!");
    StringBuffer str5(s);
    printf("<%d>%s\n", __LINE__, (const char*)str5);

    StringBuffer str6(str2);
    printf("<%d>%s\n", __LINE__, (const char*)str6);

    StringBuffer str7 = str2;
    printf("<%d>%s\n", __LINE__, (const char*)str7);

    printf("<%d>end test_constructor()\n", __LINE__);
}

void test_operator_add()
{
    printf("<%d>start test_operator_add()\n", __LINE__);

    StringBuffer str("Hello World!");
    StringBuffer str1("Hello World!");
    Int32 i32 = 32;
    Int64 i64 = 64;
    Boolean bT = TRUE;
    Boolean bF = FALSE;
    Char32 c32 = 'x';
    str += str;
    str += str1;
    str += i32;
    str += i64;
    str += bT;
    str += bF;
    str += c32;
    printf("<%d>%s\n", __LINE__, (const char*)str);

    printf("<%d>%s\n", __LINE__,
        (const char*)(str + str + str1 + i32 + i64 + bT + bF + c32));

//	    Float f = 32.0f;
//	    Double d = 64.0;
//	    str += f;
//	    str += d;
//	    printf("<%d>%s\n", __LINE__, (const char*)str);
//
//	    printf("<%d>%s\n", __LINE__, (const char*)(str + f + d));

    printf("<%d>end test_operator_add()\n", __LINE__);
}

void test_pointer()
{
    printf("<%d>start test_pointer()\n", __LINE__);

    StringBuffer str;
    const char* hello = "Hello World!";
    char* pc = 0;
    int i = 5;
    int* p = &i;
    str += hello;
    str += (char*)hello;
    str += pc;
    str += p;
    printf("<%d>%s\n", __LINE__, (const char*)str);

    printf("<%d>%s\n", __LINE__, (const char*)(str + hello + (char*)hello + pc + p));

    printf("<%d>end test_pointer()\n", __LINE__);
}

int main()
{
    test_constructor();
    test_operator_add();
    test_pointer();
	return 0;
}

