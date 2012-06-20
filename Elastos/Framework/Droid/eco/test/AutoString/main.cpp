
#include <stdlib.h>
#include <stdio.h>
#include <utils/AutoString.h>

using namespace Elastos;

int main()
{
    char* buf = (char*)malloc(100*sizeof(char));
    strcpy(buf, "hello world");

    AutoString str(buf);
    printf("AutoString str((char*)*buf);\n");
    printf("str.GetLength() = %d;\n", str.GetLength());
    printf("(const char*)str = %s;\n", (const char*)str);

    printf("=========> end test <==========\n");
    return 0;
}

