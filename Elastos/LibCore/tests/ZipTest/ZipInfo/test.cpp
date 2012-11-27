#include <elastos.h>
#include <stdio.h>
#include <new>
#include <Elastos.Utility.Zip.h>

using namespace Elastos;

int main(int argc, char *argv[])
{
    IZipFile *pZipFile = NULL;
    ECode ec;
    
    ec = CZipFile::New(String("/system/app/Phone.apk"), &pZipFile);    
    if (FAILED(ec)) {
        printf("Cann't open the file ec = %x!\n", ec);
    } 
 
    Int32 size = 0;
    pZipFile->GetSize(&size);
    printf("== the size is %d\n", size);

    String name;
    pZipFile->GetName(&name);
    printf("== name is %s\n", (const char *)name);
    return 0;
}
