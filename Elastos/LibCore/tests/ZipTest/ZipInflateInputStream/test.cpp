#include <elastos.h>
#include <stdio.h>
#include <new>
#include <Elastos.Utility.Zip.h>
#include <Elastos.IO.h>
using namespace Elastos;

int main(int argc, char *argv[])
{
    //IZipFile *pZipFile = NULL;
    //ECode ec;
    
    //ec = CZipFile::New(String("/system/app/Phone.apk"), &pZipFile);    
    //if (FAILED(ec)) {
    //    printf("Cann't open the file ec = %x!\n", ec);
    //} 
 
    //Int32 size = 0;
    //pZipFile->GetSize(&size);
    //printf("== the size is %d\n", size);

    //String name;
    //pZipFile->GetName(&name);
    //printf("== name is %s\n", (const char *)name);
    IFileInputStream *pfInputStream = NULL;
    ECode ec = NOERROR;
    IFile *pFile = NULL;

    ec = CFileInputStream::New(String("/data/data/com.elastos.runtime/elastos/rdata.zip"), &pfInputStream);	
    if (FAILED(ec)) {
    	printf("Cann't open CFileInputStream, ec = %x\n", ec);
        return -1;
    }

    printf("%s, %d\n", __FILE__, __LINE__);
    IZipInputStream *pzInputStream = NULL;
    ec = CZipInputStream::New((IInputStream *)pfInputStream, &pzInputStream);
    if (FAILED(ec)) {
        printf("Create CZipInputStream FAILEd ec = %x\n", ec);
        return -1;
    }

    
//    IInflaterInputStream *piInputStream = NULL;
//    ec = CInflaterInputStream::New((IInputStream *)pzInputStream, &piInputStream);
//    if (FAILED(ec)) {
//         printf("Cann't New CInflaterInputStream ec = %x\n", ec);
//         return -1;
//    }

    printf("%s, %d\n", __FILE__, __LINE__);
    IZipEntry *pzEntry = NULL;
    ec = pzInputStream->GetNextEntry(&pzEntry);
    printf("#####################################################\n");
    if (FAILED(ec)) {
        printf("the ec is %x\n", ec);
        printf("Get Entry Failed ==========================================\n");
        return -1;
    } else {
        printf("*****************************************\n");
    }
    Int32 i = 0; 
    while(pzEntry != NULL) {
        pzEntry = NULL;
        printf("----------fffffffffffffffffffff------------------\n");
        //String name;
        //pzEntry->GetName(&name);
        //printf("asdfasdfadfasdfasdfasdsf\n");
        //printf("the name is %s\n", (const char *)name);
        i++;
        pzInputStream->GetNextEntry(&pzEntry);
    }
printf("the is is %d\n", i);
    return 0;

}
