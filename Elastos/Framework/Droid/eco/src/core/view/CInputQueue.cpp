
#include "view/CInputQueue.h"
#include "view/NativeInputQueue.h"
#include <Slogger.h>
#include <elastos/Mutex.h>
#include <StringBuffer.h>

using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;
using namespace Elastos::Core::Threading;

static NativeInputQueue gNativeInputQueue;

static Mutex sLock;

const Boolean CInputQueue::FinishedCallback::DEBUG_RECYCLING;
const Int32 CInputQueue::FinishedCallback::RECYCLE_MAX_COUNT;

const char* CInputQueue::TAG = "CInputQueue";
const Boolean CInputQueue::DEBUG;

AutoPtr<CInputQueue::FinishedCallback> CInputQueue::FinishedCallback::sRecycleHead;
Int32 CInputQueue::FinishedCallback::sRecycleCount = 0;

CInputQueue::FinishedCallback::FinishedCallback()
{
}

AutoPtr<CInputQueue::FinishedCallback> CInputQueue::FinishedCallback::Obtain(
    /* [in] */ Int64 finishedToken)
{
    Mutex::Autolock lock(sLock);

    AutoPtr<FinishedCallback> callback = sRecycleHead;
    if (callback != NULL) {
        sRecycleHead = callback->mRecycleNext;
        sRecycleCount -= 1;
        callback->mRecycleNext = NULL;
    }
    else {
        callback = new FinishedCallback();
    }
    callback->mFinishedToken = finishedToken;

    return callback;
}

ECode CInputQueue::FinishedCallback::Run()
{
    Mutex::Autolock lock(sLock);

    if (mFinishedToken == -1) {
        Slogger::E(CInputQueue::TAG, "Event finished callback already invoked.");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    android::status_t status = gNativeInputQueue.Finished(mFinishedToken, FALSE);

    // We ignore the case where an event could not be finished because the input channel
    // was no longer registered (DEAD_OBJECT) since it is a common race that can occur
    // during application shutdown.  The input dispatcher recovers gracefully anyways.
    //
    if (status != android::OK && status != android::DEAD_OBJECT) {
        Slogger::E(TAG, "Failed to finish input event.  Check logs for details.");
        return E_RUNTIME_EXCEPTION;
    }

    mFinishedToken = -1;

    if (sRecycleCount < RECYCLE_MAX_COUNT) {
        mRecycleNext = sRecycleHead;
        sRecycleHead = this;
        sRecycleCount += 1;

        if (DEBUG_RECYCLING) {
            Slogger::D(
                CInputQueue::TAG, StringBuffer("Recycled finished callbacks: ")
                + sRecycleCount);
        }
    }

    return NOERROR;
}

CInputQueue::CInputQueue()
{
}

ECode CInputQueue::constructor(
    /* [in] */ IInputChannel* channel)
{
    mChannel = channel;

    return NOERROR;
}

/**
 * Registers an input channel and handler.
 * @param inputChannel The input channel to register.
 * @param inputHandler The input handler to input events send to the target.
 * @param messageQueue The message queue on whose thread the handler should be invoked.
 * @hide
 */
ECode CInputQueue::RegisterInputChannel(
    /* [in] */ CInputChannel* inputChannel,
    /* [in] */ InputHandler* inputHandler,
    /* [in] */ NativeMessageQueue* messageQueue)
{
    if (inputChannel == NULL) {
        Slogger::E(TAG, "inputChannel must not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (inputHandler == NULL) {
        Slogger::E(TAG, "inputHandler must not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (messageQueue == NULL) {
        Slogger::E(TAG, "messageQueue must not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    {
        Mutex::Autolock lock(sLock);

        if (DEBUG) {
//            Slogger::D(TAG, "Registering input channel '" + inputChannel + "'");
        }

        android::status_t status = gNativeInputQueue.RegisterInputChannel(
            inputChannel, inputHandler, messageQueue);

        if (status) {
            Slogger::E(TAG, "Failed to register input channel.  "
                    "Check logs for details.");
            return E_RUNTIME_EXCEPTION;
        }
    }
    return NOERROR;
}

/**
 * Unregisters an input channel.
 * Does nothing if the channel is not currently registered.
 * @param inputChannel The input channel to unregister.
 * @hide
 */
ECode CInputQueue::UnregisterInputChannel(
    /* [in] */ CInputChannel* inputChannel)
{
    if (inputChannel == NULL) {
        Slogger::E(TAG, "inputChannel must not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    {
        Mutex::Autolock lock(sLock);

        if (DEBUG) {
            //Slogger::D(TAG, "Unregistering input channel '" + inputChannel + "'");
        }

        android::status_t status = gNativeInputQueue.UnregisterInputChannel(
            inputChannel);

        if (status) {
            Slogger::E(TAG, "Failed to unregister input channel.  "
                "Check logs for details.");
            return E_RUNTIME_EXCEPTION;
        }
    }
    return NOERROR;
}

ECode CInputQueue::DispatchKeyEvent(
    /* [in] */ InputHandler* inputHandler,
    /* [in] */ CKeyEvent* event,
    /* [in] */ Int64 finishedToken)
{
    AutoPtr<IRunnable> finishedCallback = FinishedCallback::Obtain(finishedToken);
    return inputHandler->HandleKey(event, finishedCallback.Get());
}

ECode CInputQueue::DispatchMotionEvent(
    /* [in] */ InputHandler* inputHandler,
    /* [in] */ CMotionEvent* event,
    /* [in] */ Int64 finishedToken)
{
    AutoPtr<IRunnable> finishedCallback = FinishedCallback::Obtain(finishedToken);
    return inputHandler->HandleMotion(event, finishedCallback.Get());
}

ECode CInputQueue::GetInputChannel(
    /* [out] */ IInputChannel** inputChannel)
{
    VALIDATE_NOT_NULL(inputChannel);
    *inputChannel = mChannel;
    if (*inputChannel) {
        (*inputChannel)->AddRef();
    }

    return NOERROR;
}
