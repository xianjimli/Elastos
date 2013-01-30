
#include "MyRunnable.h"
#include <elastos/AutoPtr.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    printf("Start main() at Line:%d\n", __LINE__);
    AutoPtr<IRunnable> r = new MyRunnable();
    AutoPtr<IThread> thr;
    CThread::New(r, (IThread**)&thr);
    thr->Start();
    sleep(2);
    return 0;
}