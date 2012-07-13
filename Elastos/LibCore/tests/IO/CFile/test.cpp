
#include <elastos.h>
#include <stdio.h>
#include <elastos/AutoPtr.h>
#include "Elastos.IO2.h"

using namespace Elastos;

int main(int argc, char *argv[])
{
    printf("==== File: %s, Line: %d ====\n", __FILE__, __LINE__);
    String fp = String("/data/data/com.elastos.runtime/elastos/filetest.txt");
    AutoPtr<IFile> file;
    CFile::New(fp, (IFile**)&file);
    printf("==== File: %s, Line: %d ====\n", __FILE__, __LINE__);
    Boolean succeeded;
    file->CreateNewFile(&succeeded);
    assert(succeeded == TRUE);
    printf("==== File: %s, Line: %d ====\n", __FILE__, __LINE__);
    return 0;
}
