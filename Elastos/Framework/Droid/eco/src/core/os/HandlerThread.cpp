
#ifdef _FRAMEWORK_CORE
#include "os/HandlerThread.h"
#include "os/CApartment.h"
#else
#include "HandlerThread.h"
#endif

#include "os/Process.h"

HandlerThread::HandlerThread(
    /* [in] */ const String& name)
    : mPriority(0)
    , mTid(-1)
{
    Thread::Init(name);
    mPriority = Process::THREAD_PRIORITY_DEFAULT;
}

HandlerThread::HandlerThread(
    /* [in] */ const String& name,
    /* [in] */ Int32 priority)
{
    Thread::Init(name);
    mPriority = priority;
}

ECode HandlerThread::Run() {
    mTid = Process::MyTid();

    //TODO
    //CApartment::Prepare();
    {
        Mutex::Autolock lock(mSelfLock);

        //TODO
        // mLooper = Looper.myLooper();
        // notifyAll();
    }

    Process::SetThreadPriority(mPriority);
    OnLooperPrepared();

    //TODO
    //Looper.loop();
    mTid = -1;

    return NOERROR;
}

AutoPtr<IApartment> HandlerThread::GetLooper()
{
    Boolean isAlive = FALSE;
    if (! (IsAlive(&isAlive), isAlive)) {
        return NULL;
    }

    // If the thread has been started, wait until the looper has been created.
    {
        Mutex::Autolock lock(mSelfLock);
        while (isAlive && mLooper == NULL) {
            // try {
            Wait();
            // } catch (InterruptedException e) {
            // }
            IsAlive(&isAlive);
        }
    }

    return mLooper;
}

Boolean HandlerThread::Quit()
{
    AutoPtr<IApartment> looper = GetLooper();
    if (looper != NULL) {
        //TODO
        //looper.quit();
        looper->Finish();
        return TRUE;
    }
    return FALSE;
}

Int32 HandlerThread::GetThreadId()
{
    return mTid;
}

void HandlerThread::OnLooperPrepared()
{
}
