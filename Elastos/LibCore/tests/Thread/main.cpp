
#include "MyRunnable.h"
#include "MyThread.h"
#include <elastos/AutoPtr.h>
#include <unistd.h>
#include <stdio.h>

void printArgs(
    /* [in] */ IThread* t)
{
    Int64 id;
    Int32 priority;
    t->GetId(&id);
    t->GetPriority(&priority);
    String name;
    t->GetName(&name);
    printf("=======thread id = %d, thread priority = %d, thread name = %s\n",
            id, priority, (const char*)name);
}

int main(int argc, char *argv[])
{
    printf("Start main() at Line:%d\n", __LINE__);
    // AutoPtr<IRunnable> r = new MyRunnable();
    // AutoPtr<IThread> thr1;
    // CThread::New(r, (IThread**)&thr1);
    //thr1->Start();
    //printArgs(thr1);

    MyThread* myThread = new MyThread();
    AutoPtr<IThread> thr2 = (IThread*)myThread->Probe(EIID_IThread);
    thr2->Start();
    Thread::Sleep(3000);
    printArgs(thr2);
    thr2->SetPriority(ELASTOS_PRIORITY_FOREGROUND);
    printArgs(thr2);
    // printArgs(thr2);
    myThread->mStop = TRUE;
    thr2->Interrupt();
    Thread::Sleep(3000);

    //sleep(2);
    return 0;
}
