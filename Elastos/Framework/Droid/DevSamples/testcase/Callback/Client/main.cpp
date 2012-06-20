#include "carcallback01.h"
#include "carcallback02.h"
#include "carcallback03.h"
#include "carcallback04.h"
#include "carcallback05.h"
#include "carcallback06.h"
#include "carcallback07.h"
#include "carcallback09.h"
#include "carcallback10.h"
#include "carcallback12.h"
#include "carcallback13.h"

#include "test.h"

#include "CTestUnit.h"

#define AppletFinish_Nice 0x01

typedef int (__stdcall *FUNC)(void *pThis, int argc, char *argv[]);
typedef int (__stdcall CTest::*TestFuncPrototype)(int argc, char *argv[]);
int RunTestFunc(CTest *pTestObject, int argc, char *argv[]);

TestFuncPrototype FA[] =
{
    &CTest::Test1,
    &CTest::Test2,
    &CTest::Test3,
    &CTest::Test4,
    &CTest::Test5,
    &CTest::Test6,
    &CTest::Test7,
    &CTest::Test8,
    &CTest::Test9,
    &CTest::Test10,
    &CTest::Test11,
    &CTest::Test12,
    &CTest::Test13,
    &CTest::Test14,
    &CTest::Test15,
    &CTest::Test16,
    &CTest::Test17,
    &CTest::Test18,
    &CTest::Test19,
    &CTest::Test20,
    &CTest::Test21,
    &CTest::Test22,
    &CTest::Test23,
    &CTest::Test24,
    &CTest::Test25,
    &CTest::Test26,
    &CTest::Test27,
    &CTest::Test28,
    &CTest::Test29,
    &CTest::Test30,
};

TPDECL;

CTest TestObject;
extern CTest*  g_pcTest;
extern int     g_nIndex;


static CTestUnit *s_pUnit;

ECode MainThread(void *pArg)
{
    BufferOf_<String, 5> args;
    args.Copy((BufferOf<String>*)pArg);
    int argc = args.GetUsed();
    char argName[16] = {NULL};
    char argArg[16] = {NULL};
    char* arg[] = {argName, argArg};
    char** argv = (char**)arg;

    if (argc != 2) {
        printf("*ERROR* No test case number\n");
        s_pUnit->Finish(AppletFinish_Nice);
        return -1;
    }
    for (int i = 0; i < argc; i++) {
        strncpy(argv[i], args[i], 16);
    }

    int nIndex = atoi(argv[1]) - 1;
    int nLength = sizeof(FA) / sizeof(FA[0]);

    if (nIndex < 0 || nIndex >= nLength) {
        printf("*ERROR* Invalid testing case number\n");
        s_pUnit->Finish(AppletFinish_Nice);
        return -1;
    }

    TPINIT;

    TestObject.Init();
    int nRetVal = ((FUNC)*(void **)(&FA[nIndex]))(&TestObject, argc, argv);
    TestObject.Uninit();
    s_pUnit->Finish(AppletFinish_Nice);

    TPEND;
    return nRetVal;
}


void __cdecl fn(void* p)
{
    ((IThread*)p)->Join(INFINITE, NULL);
    ((IThread*)p)->Release();
}

/*
ECode ElastosMain(const BufferOf<String>& args)
{
    IThread* pIThread = NULL;
    ECode ec = CThread::New(MainThread, (void*)&args, 0, &pIThread);
    if (ec != NOERROR) {
        printf("CThread::New Failed! ec = %x\n", ec);
        return -1;
    }

    ec = CThread::AtQuit(fn, pIThread);
    if (ec != NOERROR) {
        printf("CThread::AtQuit Failed! ec = %x\n", ec);
        pIThread->Join(INFINITE, NULL);
        pIThread->Release();
        return -1;
    }

    return ec;
}
*/

class CMyTestUnit : public CTestUnit
{
public:
    CARAPI Run(BufferOf<String> &args)
    {
        IThread* pIThread = NULL;
        ECode ec = CThread::New(MainThread, (void*)&args, 0, &pIThread);
        if (ec != NOERROR) {
            printf("CThread::New Failed! ec = %x\n", ec);
            return -1;
        }
    
        ec = CThread::AtQuit(fn, pIThread);
        if (ec != NOERROR) {
            printf("CThread::AtQuit Failed! ec = %x\n", ec);
            pIThread->Join(INFINITE, NULL);
            pIThread->Release();
            return -1;
        }
    
        return ec;    
    }
};

int main (int argc, char *argv[])
{
    ECode ec = NOERROR;
    
    s_pUnit = new CMyTestUnit();
    
    if (s_pUnit == NULL) return E_OUT_OF_MEMORY;
    
    BufferOf<String> args((String *)argv, argc + 1, argc);
    ec = s_pUnit->Start(args);
    
    return ec;
}
