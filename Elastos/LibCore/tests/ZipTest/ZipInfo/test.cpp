#include <elastos.h>
#include <stdio.h>
#include <new>
#include <Elastos.Utility.Zip.h>
#include <elastos/AutoPtr.h>
using namespace Elastos;

void test1(IObjectContainer *container)
{
    AutoPtr<IObjectEnumerator> emu;
    AutoPtr<IInterface> itf; 
    Boolean hasNext;
    container->GetObjectEnumerator((IObjectEnumerator **)&emu);
    while(emu->MoveNext(&hasNext), hasNext) {
        AutoPtr<IZipEntry> entry;
        emu->Current((IInterface **)&itf);
        entry = IZipEntry::Probe(itf);
        String name;
        entry->GetName(&name);
        printf("the entry name is %s\n", (const char *) name);
    }
    printf("**OK** End Test ZipFile::GetEnties() Function\n");
}


void test2(IZipFile *pZipFile)
{
    Int32 nSize;
    pZipFile->GetSize(&nSize);
    printf("the size is %d\n", nSize); 
    printf("**OK** End Test ZipFile::GetSize() Function\n");
}


void test3(IZipFile *pZipFile)
{
     String name("TextViewDemo/res/values/strings.xml");
     IZipEntry *pEntry = NULL;
     ECode ec = NOERROR;
     ec = pZipFile->GetEntry(name, &pEntry);
     if (FAILED(ec)) {
         printf("1**FAILED NOT FOUND\n");
     }
    
     String name1("");
     ec = pZipFile->GetEntry(name1, &pEntry);
     if (FAILED(ec)) {
         printf("2**FAILED NOT FOUND\n");
     }
     printf("**OK** End Test ZipFile::GetEntry() Function\n");
}

void test4(IZipFile *pZipFile)
{
     String name("TextViewDemo/res/values/strings.xml");
     IZipEntry *pEntry = NULL;
     ECode ec = NOERROR;
     ec = pZipFile->GetEntry(name, &pEntry);
     if (pEntry == NULL) {
          printf("Cann't Find entry!\n");
          return;
     }
    
     AutoPtr<IInputStream> is;
     ec = pZipFile->GetInputStream(pEntry, (IInputStream **) &is);
     if (FAILED(ec)) {
         printf("Cann't Get ZipFile InputStream!\n");
         return;
     }

     AutoPtr<IFile> file;
     ec = CFile::New(String("/data/data/com.elastos.runtime/pzipfile.xml"), (IFile **) &file);
     if (FAILED(ec)) {
         printf("Cann't Create the pzipfile.xml!\n");
         return;
     }     

     AutoPtr<IFileOutputStream> fos;
     ec = CFileOutputStream::New(file, (IFileOutputStream **) &fos);
     if (FAILED(ec)) {
         printf("Cann't Create the pzipfile.xml OutputStream!\n");
         return;
     }     
     
     ArrayOf<Byte> *buf = ArrayOf<Byte>::Alloc(1024);
     Int32 len = 0;
     ec = is->ReadBufferEx(0, 1024, buf, &len);
     while( ec == NOERROR && len > 0) {
        ec = fos->WriteBufferEx(0, len, *buf);
        len = 0;
        ec = is->ReadBufferEx(0,1024,buf, &len);
    
     }
     
     is->Close();
     fos->Close();
     
     printf("** OK ** Test GetInputStream Success!\n");
}

int main(int argc, char *argv[])
{
    IZipFile *pZipFile = NULL;
    ECode ec;
    AutoPtr<IObjectContainer> container;

    ec = CZipFile::New(String("/data/data/com.elastos.runtime/elastos.zip"), &pZipFile);    
    if (FAILED(ec)) {
        printf("Cann't open the file ec = %x!\n", ec);
        return -1;
    } 
    
    ec = pZipFile->GetEntries((IObjectContainer **) &container);
    if (FAILED(ec)) {
        printf("Cann't Get Entries ec is %x!\n", ec);
        return -1;
    } 
    
    //test1(container);
    //test2(pZipFile);
    //test3(pZipFile);
    test4(pZipFile);

    pZipFile->Release();
    return 0;
}

