#include <stdio.h>
#include "CCBServer.h"

ECode CCBServer::TriggerA()
{
    Callback::OnTriggerA(1);
    return NOERROR;
}

ECode CCBServer::TriggerB()
{
    Callback::OnTriggerB(2);
    return NOERROR;
}

ECode CCBServer::TriggerC()
{
    Callback::OnTriggerC(3);
    return NOERROR;
}

ECode CCBServer::Trigger1()
{
    static int i = 0;
//    printf("***%d***\n", i);
    Callback::OnTriggerA(1);
    Callback::OnTriggerB(2);
    Callback::OnTriggerC(3);
    i++;
    return NOERROR;
}

ECode CCBServer::Trigger2()
{
    ECode ec;
    for (int i = 0; i < 10; i++) {
        ec = OnTriggerC(23);
        ec = OnTriggerB(22);
        ec = OnTriggerA(21);
        sleep(1);
    }

    return NOERROR;
}

ECode Thread3(void* pArg)
{
    CCBServer * pcServer = (CCBServer *)pArg;
    for (int i = 0; i < 20; i++) {
//        printf("\t\tTrigger3::%d\n", i);
        pcServer->OnTriggerA(31);
        pcServer->OnTriggerB(32);
        pcServer->OnTriggerC(33);
        sleep(1);
    }

    return NOERROR;
}

ECode CCBServer::Trigger3()
{
    IThread* pThread;
    ECode ec = CThread::New(Thread3, (void*)this, 0, &pThread);
    if (FAILED(ec)) {
        printf("CThread::New failed. ec = %x.\n", ec);
        return E_OUT_OF_MEMORY;
    }

    Trigger2();

    pThread->Join(INFINITE, NULL);
    pThread->Release();
//    printf("\t\tTrigger3 end.\n");

    return NOERROR;
}

ECode CCBServer::Trigger4()
{
    BufferOf_<Byte,32> ebbBuf;
    ebbBuf.Copy((Byte*)"ByteArray_in", 13);

    Callback::OnTrigger(41, "CCBServer::Trigger4", ebbBuf);

    return NOERROR;
}

ECode CCBServer::Trigger5()
{
    Callback::OnTrigger(51, "CCBServer::Trigger5", NULL_BUFFEROF(Byte));
    return NOERROR;
}

ECode CCBServer::Trigger6(Int32 nIndex)
{
    Callback::OnTriggerD(nIndex);
    return NOERROR;
}
