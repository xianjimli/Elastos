
#include "MyTimerTask.h"
#include <elastos/AutoPtr.h>
#include <unistd.h>
#include <stdio.h>
#include <elastos/Thread.h>

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

void ScheduleAtFixRate()
{
    ECode ec = NOERROR;
    printf("Start main() at Line:%d\n", __LINE__);
    AutoPtr<ITimerTask> r = new MyTimerTask();
    printf("Start main() at Line:%d\n", __LINE__);
    ITimer* timer;
    printf("Start main() at Line:%d\n", __LINE__);
    CTimer::New(&timer);
        printf("Start main() at Line:%d\n", __LINE__);
    ec = Thread::Sleep(2000);
    timer->ScheduleAtFixedRate((ITimerTask*)r, 1000, 1000);
    printf("Start main() at Line:%d\n", __LINE__);

//    while(1);
//    printf("$$$$$$$$$$))))))))))))DDDDDDDDDDDDDDd:%d\n", __LINE__);
    Thread::Sleep(30000);
    timer->Cancel();
    while(1);
    return;
}

//only call one time
void ScheduleOnce()
{
}

int main(int argc, char *argv[])
{
//    ECode ec = NOERROR;
//    printf("Start main() at Line:%d\n", __LINE__);
//    AutoPtr<ITimerTask> r = new MyTimerTask();
//    printf("Start main() at Line:%d\n", __LINE__);
//    ITimer* timer;
//    printf("Start main() at Line:%d\n", __LINE__);
//    CTimer::New(&timer);
//        printf("Start main() at Line:%d\n", __LINE__);
//    ec = Thread::Sleep(2000);
//    timer->ScheduleEx((ITimerTask*)r, 1000);
//    printf("Start main() at Line:%d\n", __LINE__);
//    ec = Thread::Sleep(10000);
    ScheduleAtFixRate();
    printf("$$$$$$$$$$))))))))))))DDDDDDDDDDDDDDd:%d\n", __LINE__);
    return 0;
}
