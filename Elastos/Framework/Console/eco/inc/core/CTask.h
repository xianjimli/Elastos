
#ifndef __CTASK_H__
#define __CTASK_H__

#include "_CTask.h"
#include <pthread.h>

CarClass(CTask)
{
public:
    CTask();

    ~CTask();

    CARAPI Main(
        /* [in] */ const BufferOf<CString>& args);

    CARAPI Start(
        /* [in] */ const BufferOf<CString>& args);

    CARAPI Finish(
        /* [in] */ TaskFinish flag);

private:
    static void* EntryRoutine(void *arg);

    CARAPI NotifyFinished();

private:
    pthread_t   mThread;
    IInterface* mCallbackContent;
};

#endif //__CTASK_H__
