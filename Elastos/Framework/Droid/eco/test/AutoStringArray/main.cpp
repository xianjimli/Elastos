
#include <stdlib.h>
#include <stdio.h>
#include <utils/AutoStringArray.h>

using namespace Elastos;

int main()
{
    AutoStringArray strs;
    Int32 len = 10;
    strs = ArrayOf<String>::Alloc(len);
    (*strs)[1] = (char*)malloc(100);
    (*strs)[3] = (char*)malloc(100);
    (*strs)[5] = (char*)malloc(100);
    (*strs)[7] = (char*)malloc(100);

    printf("=========> end test <==========\n");
    return 0;
}

