
#ifndef __INPUTQUEUE_H__
#define __INPUTQUEUE_H__

#include "ext/frameworkext.h"
#include "view/CInputChannel.h"
#include "view/InputHandler.h"
#include "view/CKeyEvent.h"
#include "view/CMotionEvent.h"
#include "os/NativeMessageQueue.h"
#include "os/Runnable.h"
#include <elastos/AutoPtr.h>

/**
 * An input queue provides a mechanism for an application to receive incoming
 * input events.  Currently only usable from native code.
 */
class InputQueue
{
    friend class NativeInputQueue;

public:
    /**
     * Interface to receive notification of when an InputQueue is associated
     * and dissociated with a thread.
     */
    interface Callback
    {
        /**
         * Called when the given InputQueue is now associated with the
         * thread making this call, so it can start receiving events from it.
         */
        virtual CARAPI_(void) OnInputQueueCreated(
            /* [in] */ InputQueue* queue) = 0;

        /**
         * Called when the given InputQueue is no longer associated with
         * the thread and thus not dispatching events.
         */
        virtual CARAPI_(void) OnInputQueueDestroyed(
            /* [in] */ InputQueue* queue) = 0;
    };

    class FinishedCallback : public Runnable
    {
    private:
        FinishedCallback();

    public:
        static CARAPI_(AutoPtr<FinishedCallback>) Obtain(
            /* [in] */ Int64 finishedToken);

        CARAPI Run();

    private:
        static const Boolean DEBUG_RECYCLING = FALSE;

        static const Int32 RECYCLE_MAX_COUNT = 4;

        static AutoPtr<FinishedCallback> sRecycleHead;
        static Int32 sRecycleCount;

        AutoPtr<FinishedCallback> mRecycleNext;
        Int64 mFinishedToken;
    };

public:
    InputQueue(
        /* [in] */ CInputChannel* channel);

    static CARAPI RegisterInputChannel(
        /* [in] */ CInputChannel* inputChannel,
        /* [in] */ InputHandler* inputHandler,
        /* [in] */ NativeMessageQueue* messageQueue);

    static CARAPI UnregisterInputChannel(
        /* [in] */ CInputChannel* inputChannel);

private:
    static CARAPI DispatchKeyEvent(
        /* [in] */ InputHandler* inputHandler,
        /* [in] */ CKeyEvent* event,
        /* [in] */ Int64 finishedToken);

    static CARAPI DispatchMotionEvent(
        /* [in] */ InputHandler* inputHandler,
        /* [in] */ CMotionEvent* event,
        /* [in] */ Int64 finishedToken);

private:
    static const char* TAG;

    static const Boolean DEBUG = FALSE;

    AutoPtr<CInputChannel> mChannel;
};

#endif //__INPUTQUEUE_H__
