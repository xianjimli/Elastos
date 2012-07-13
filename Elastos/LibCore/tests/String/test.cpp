
#include <elastos.h>
#include <stdio.h>
#include <new>

using namespace Elastos;

String f1()
{
    String tmp("Hello World");
    return tmp;
}

void f2(String& s)
{
    assert(NULL != s);
    s = "Hello World!";
    return;
}

int main(int argc, char *argv[])
{
    String s1(NULL);
    String s2;
    String s3("Hello World");

    printf("s2:%s\n", (const char*)s2);
    printf("s3:%s\n", (const char*)s3);

    String s4 = s3;
    printf("s4:%s\n", (const char*)s4);

    String s5 = s4 + String("!");
    printf("s5:%s\n", (const char*)s5);

    String s6("    Hello World!    ");
    printf("s6 trimStart :%s\n", (const char*)s6.TrimStart());
    printf("s6 trimEnd :%s\n", (const char*)s6.TrimEnd());
    printf("s6 trim:%s\n", (const char*)s6.Trim());

    String s7 = f1();
    printf("s7:%s\n", (const char*)s7);

    String s8;
    f2(s8);
    printf("s8:%s\n", (const char*)s8);

    s3.~String();

    ArrayOf<String>* arr = ArrayOf<String>::Alloc(1);
    (*arr)[0] = "Hello World!";
	(*arr)[0].~String();
//    new((*arr)[0]) String();
//    (*arr)[0] = "Hello World!";

    String s9("Hello World!");
    s9 += String(" Hello World!");

    return 0;
}
