
#ifndef  __HANDLERTHREAD_H__
#define  __HANDLERTHREAD_H__

#include "ext/frameworkext.h"
#include <elastos/Thread.h>
#include <elastos/AutoPtr.h>

using namespace Elastos;

/**
 * Handy class for starting a new thread that has a looper. The looper can then be
 * used to create handler classes. Note that start() must still be called.
 */
class HandlerThread
    : public Thread
{
public:
    HandlerThread(
        /* [in] */ const String& name);

    /**
     * Constructs a HandlerThread.
     * @param name
     * @param priority The priority to run the thread at. The value supplied must be from
     * {@link android.os.Process} and not from java.lang.Thread.
     */
    HandlerThread(
        /* [in] */ const String& name,
        /* [in] */ Int32 priority);

    CARAPI Run();

    /**
     * This method returns the Looper associated with this thread. If this thread not been started
     * or for any reason is isAlive() returns false, this method will return null. If this thread
     * has been started, this method will block until the looper has been initialized.
     * @return The looper.
     */
    CARAPI_(AutoPtr<IApartment>) GetLooper();

    /**
     * Ask the currently running looper to quit.  If the thread has not
     * been started or has finished (that is if {@link #getLooper} returns
     * null), then false is returned.  Otherwise the looper is asked to
     * quit and true is returned.
     */
    CARAPI_(Boolean) Quit();

    /**
     * Returns the identifier of this thread. See Process.myTid().
     */
    CARAPI_(Int32) GetThreadId();

protected:
    /**
     * Call back method that can be explicitly over ridden if needed to execute some
     * setup before Looper loops.
     */
    CARAPI_(void) OnLooperPrepared();

private:
    Int32 mPriority;
    Int32 mTid;
    AutoPtr<IApartment> mLooper;
    Mutex mSelfLock;
};

#endif  //__HANDLERTHREAD_H__
