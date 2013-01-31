
#ifndef __CINPUTQUEUE_H__
#define __CINPUTQUEUE_H__

#include "_CInputQueue.h"
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

CarClass(CInputQueue)
{
    friend class NativeInputQueue;

public:
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
    CInputQueue();

    CARAPI constructor(
        /* [in] */ IInputChannel* channel);

    CARAPI GetInputChannel(
        /* [out] */ IInputChannel** inputChannel);

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

    AutoPtr<IInputChannel> mChannel;
};

#endif //__CINPUTQUEUE_H__
