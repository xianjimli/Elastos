
#include "stdio.h"
#include "MyRunnable.h"
#include <elastos/AutoPtr.h>

int main(int argc, char *argv[])
{
    printf("Start main() at Line:%d\n", __LINE__);
    MyRunnable* r = new MyRunnable();
    AutoPtr<IRunnable> run = (IRunnable*)r->Probe(EIID_IRunnable);
    AutoPtr<IThread> myT;
    CThread::New(run, (IThread**)&myT);
    myT->Start();
    return 0;
}