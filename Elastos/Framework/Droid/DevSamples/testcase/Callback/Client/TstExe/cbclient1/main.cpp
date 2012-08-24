
#include <elastos.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "CTestUnit.h"

#define AppletFinish_Nice 0x01

using namespace Elastos;

#include "carcallback02.h"

int g_nCount = 0;

ECode OnTrigger(void* pUserData, PInterface  pSender)
{
    g_nCount++;
    return NOERROR;
}

/*
ECode MainThread(void *pArg)
{
    IHello* pIHello = NULL;
    int i;
    ECode ec = CSystem::FindRunningObject("cbtestserver", (IInterface**)&pIHello);
    if (FAILED(ec)) {
        printf("CSystem::FindRunningObject Error, ec = %x\n", ec);
        goto Exit;
    }

    CThread::Sleep(100, NULL);
    ec = CCallback::AddOnTriggerCallback(pIHello, &OnTrigger);
    if (FAILED(ec)) {
        printf("CCallbackClass Add Callback Failed! ec = %x\n", ec);
        goto Exit;
    }

    for (i = 0; i < 100; i++) {
        pIHello->Bar();
    }

    CThread::Sleep(1000, NULL);
    CCallback::RemoveOnTriggerCallback(pIHello, &OnTrigger);
    pIHello->Release();
    pIHello = NULL;
    CApplet::Finish(AppletFinish_Nice);
    return 0;
Exit:
    if (pIHello) {
        pIHello->Release();
    }
    CApplet::Finish(AppletFinish_Nice);
    return -1;
}

void __cdecl fn(void* p)
{
    ((IThread*)p)->Join(INFINITE, NULL);
    ((IThread*)p)->Release();
    CProcess::Exit(g_nCount);
}

ECode ElastosMain(const BufferOf<String>& args)
{
    IThread* pIThread = NULL;
    ECode ec = CThread::New(MainThread, (void*)&args, 0, &pIThread);
    if (ec != NOERROR) {
        printf("CThread::New Failed! ec = %x\n", ec);
        goto Exit;
    }
    ec = CProcess::AtExit(fn, pIThread);
    if (ec != NOERROR) {
        printf("CProcess::AtExit Failed! ec = %x\n", ec);
        pIThread->Join(INFINITE, NULL);
        pIThread->Release();
        return -1;
    }
    return NOERROR;

Exit:
    return -2;
}
*/

class CMyTestUnit : public CTestUnit
{
public:
    CARAPI Run(BufferOf<String> &args)
    {
        IHello* pIHello = NULL;
        int i;
        ECode ec = CSystem::FindRunningObject("cbtestserver", (IInterface**)&pIHello);
        if (FAILED(ec)) {
            printf("CSystem::FindRunningObject Error, ec = %x\n", ec);
            goto Exit;
        }
    
        CThread::Sleep(100, NULL);
        ec = CCallback::AddOnTriggerCallback(pIHello, &OnTrigger);
        if (FAILED(ec)) {
            printf("CCallbackClass Add Callback Failed! ec = %x\n", ec);
            goto Exit;
        }
    
        for (i = 0; i < 100; i++) {
            pIHello->Bar();
        }
    
        CThread::Sleep(1000, NULL);
        CCallback::RemoveOnTriggerCallback(pIHello, &OnTrigger);
        pIHello->Release();
        pIHello = NULL;
        Finish(AppletFinish_Nice);
        return 0;
    Exit:
        if (pIHello) {
            pIHello->Release();
        }
        Finish(AppletFinish_Nice);
        return -1;
    }
};

int main (int argc, char *argv[])
{
    ECode ec = NOERROR;
    CTestUnit *pUnit;
    
    pUnit = new CMyTestUnit();
    
    if (pUnit == NULL) return E_OUT_OF_MEMORY;
    
    BufferOf<String> args((String *)argv, argc + 1, argc);
    ec = pUnit->Start(args);
    
    return ec;
}
