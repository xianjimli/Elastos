#include "test.h"
#include <stdio.h>
#include <Elastos.Net.h>
#include <elastos/AutoPtr.h>
#include <elastos/System.h>
using namespace Elastos;
using namespace Elastos::Core;
//using namespace Elastos::Core;
#define DINFO() printf("%s. %d\n", __FILE__, __LINE__);
int CTest::test1(int argc, char* argv[])
{
    AutoPtr<IServerSocket> pServerSocket;
    AutoPtr<ISocket> pSock;
    AutoPtr<IBufferedReader> pBufReader;
    AutoPtr<IInputStreamReader> pInReader;
    AutoPtr<IInputStream> pIn;

    ECode ec = NOERROR;
    DINFO();
    DINFO();
    ec = CServerSocket::New(10000, (IServerSocket**)&pServerSocket);
    DINFO();
    if (FAILED(ec)) {
        printf("Cann't Create the server Socket! ec = %x\n", ec);
        return ec;
    }

    DINFO();
    while(1) {
        DINFO();
        ec = pServerSocket->Accept((ISocket**)&pSock);
        if (FAILED(ec)) {
            printf("Cann't Accept from the server Socket! ec = %x\n", ec);
            return ec;
        }

        ec = pSock->GetInputStream((IInputStream**) &pIn);
        if (FAILED(ec)) {
            printf("Cann't GetInputStream from the Socket! ec = %x\n", ec);
            return ec;
        }
        DINFO();
        ec = CInputStreamReader::New(pIn, (IInputStreamReader**)&pInReader);
        if (FAILED(ec)) {
            printf("New IInputStreamReader FAILED! ec = %x\n", ec);
            return ec;
        }

        ec = CBufferedReader::New(pInReader, (IBufferedReader**) &pBufReader);
        if (FAILED(ec)) {
            printf("New CBufferedReader FAILED! ec = %x\n", ec);
            return ec;
        }

        String str;
        pBufReader->ReadLine(&str);

        printf("the str is %s\n", (const char*) str);
        pSock->Close();
    }

    pServerSocket->Close();
    return ec;
}

int CTest::test2(int argc, char* argv[])
{
    AutoPtr<ISocket> pSock;
    AutoPtr<IOutputStream> pOut;
    AutoPtr<IDataOutputStream> pDataOutput;


    ECode ec = NOERROR;
    DINFO();
    ec = CSocket::New(String("127.0.0.1"), 10000, (ISocket**) &pSock);
    if (FAILED(ec)) {
        printf("Cann't Create the socket!ec = %x, line = %d\n", ec, __LINE__);
        return ec;
    }
    DINFO();
    ec = pSock->GetOutputStream((IOutputStream**) &pOut);
    if (FAILED(ec)) {
        printf("Cann't GetOutputStream!ec = %x, line = %d\n", ec, __LINE__);
        return ec;
    }

    DINFO();
    ec = CDataOutputStream::New(pOut, (IDataOutputStream**) &pDataOutput);
    if (FAILED(ec)) {
        printf("Cann't CDataOutputStream = %x, line = %d\n", ec, __LINE__);
        return ec;
    }
    DINFO();
    ArrayOf<Byte> *buf;
    buf = ArrayOf<Byte>::Alloc(100);
    buf->Copy((Byte*)"12345jianfeng simple testdfgop", 30);
    pDataOutput->WriteBuffer(*buf);


    return 0;
}



int CTest::test3(int argc, char* argv[])
{
    return 0;
}

int CTest::test4(int argc, char* argv[])
{
    return 0;
}


int CTest::test5(int argc, char* argv[])
{
    return 0;
}

int CTest::test6(int argc, char* argv[])
{
    return 0;
}

int CTest::test7(int argc, char* argv[])
{
    return 0;
}

int CTest::test8(int argc, char* argv[])
{
    return 0;
}

