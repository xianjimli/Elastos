
#include "view/NativeInputQueue.h"
#include "view/CKeyEvent.h"
#include "view/CMotionEvent.h"
#include "view/InputQueue.h"
#include <Slogger.h>
#include <StringBuffer.h>
#include <elastos/AutoPtr.h>

using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

const char* NativeInputQueue::TAG = "NativeInputQueue";

ECode create_CKeyEvent_fromNative(
    /* [in] */ const android::KeyEvent* event,
    /* [out] */ CKeyEvent** eventObj)
{
    assert(eventObj != NULL);

    return CKeyEvent::NewByFriend(
                nanoseconds_to_milliseconds(event->getDownTime()),
                nanoseconds_to_milliseconds(event->getEventTime()),
                event->getAction(),
                event->getKeyCode(),
                event->getRepeatCount(),
                event->getMetaState(),
                event->getDeviceId(),
                event->getScanCode(),
                event->getFlags(),
                event->getSource(),
                eventObj);
}

ECode create_CMotionEvent_fromNative(
    /* [in] */ const android::MotionEvent* event,
    /* [out] */ CMotionEvent** eventObj)
{
    assert(event != NULL);
    assert(eventObj != NULL);

    Int32 numPointers = event->getPointerCount();
    Int32 numHistoricalSamples = event->getHistorySize();
    Int32 numSamples = numHistoricalSamples + 1;

    ECode ec = CMotionEvent::Obtain(numPointers, numSamples, eventObj);
    if (FAILED(ec)) {
        return ec;
    }

    //printf("("event = 0x%08x, downtime = %d,\n", *eventObj, event->getDownTime()/CMotionEvent::MS_PER_NS);

    (*eventObj)->mDeviceId = event->getDeviceId();
    (*eventObj)->mSource = event->getSource();
    (*eventObj)->mDownTimeNano = event->getDownTime();
    (*eventObj)->mAction = event->getAction();
    (*eventObj)->mXOffset = event->getXOffset();
    (*eventObj)->mYOffset = event->getYOffset();
    (*eventObj)->mXPrecision = event->getXPrecision();
    (*eventObj)->mYPrecision = event->getYPrecision();
    (*eventObj)->mEdgeFlags = event->getEdgeFlags();
    (*eventObj)->mMetaState = event->getMetaState();
    (*eventObj)->mFlags = event->getFlags();
    (*eventObj)->mNumPointers = numPointers;
    (*eventObj)->mNumSamples = numSamples;
    (*eventObj)->mLastDataSampleIndex =
        (numSamples - 1) * numPointers * CMotionEvent::NUM_SAMPLE_DATA;
    (*eventObj)->mLastEventTimeNanoSampleIndex = numSamples - 1;

    const Int32* srcPointerIdentifiers = event->getPointerIds();
    memcpy((*eventObj)->mPointerIdentifiers, srcPointerIdentifiers,
        sizeof(Int32) * numPointers);

    const Int64* srcSampleEventTimes = event->getSampleEventTimes();
    memcpy((*eventObj)->mEventTimeNanoSamples, srcSampleEventTimes,
        sizeof(Int64) * numSamples);

    const android::PointerCoords* srcSamplePointerCoords = event->getSamplePointerCoords();
    memcpy((*eventObj)->mDataSamples, srcSamplePointerCoords,
        sizeof(CMotionEvent::PointerCoords) * numSamples * numPointers);

    return NOERROR;
}

NativeInputQueue::Connection::Connection(
    /* [in] */ uint16_t id,
    /* [in] */ const android::sp<android::InputChannel>& inputChannel,
    /* [in] */ const android::sp<android::Looper>& looper) :
    mId(id),
    mStatus(STATUS_NORMAL),
    mInputChannel(inputChannel),
    mInputConsumer(inputChannel),
    mLooper(looper),
    mInputHandler(NULL),
    mMessageSeqNum(0),
    mMessageInProgress(FALSE)
{
}

NativeInputQueue::Connection::~Connection()
{
}

NativeInputQueue::NativeInputQueue() :
    mNextConnectionId(0)
{
}

NativeInputQueue::~NativeInputQueue()
{
}

android::status_t NativeInputQueue::RegisterInputChannel(
    /* [in] */ CInputChannel* _inputChannel,
    /* [in] */ InputHandler* inputHandler,
    /* [in] */ NativeMessageQueue* messageQueue)
{
    NativeInputChannel* nativeInputChannel = _inputChannel->mPtr;
    android::sp<android::InputChannel> inputChannel =
            nativeInputChannel != NULL ? nativeInputChannel->getInputChannel() : NULL;
    if (inputChannel == NULL) {
        Slogger::W(TAG, "inputChannel is not initialized.");
        return android::BAD_VALUE;
    }

#if DEBUG_REGISTRATION
    Slogger::D(TAG, StringBuffer("channel ") + inputChannel->getName().string()
            + " - Registered.");
#endif

    android::sp<android::Looper> looper = messageQueue->GetLooper();

    { // acquire lock
        android::AutoMutex _l(mLock);

        if (GetConnectionIndex(inputChannel) >= 0) {
            Slogger::W(TAG, StringBuffer("Attempted to register already registered input channel ")
                    + inputChannel->getName().string());
            return android::BAD_VALUE;
        }

        uint16_t connectionId = mNextConnectionId++;
        android::sp<Connection> connection = new Connection(connectionId, inputChannel, looper);
        android::status_t result = connection->mInputConsumer.initialize();
        if (result) {
            Slogger::W(TAG, StringBuffer("Failed to initialize input consumer for input channel ")
                    + inputChannel->getName().string()
                    + ", status=" + result);
            return result;
        }

        connection->mInputHandler = inputHandler;

        int32_t receiveFd = inputChannel->getReceivePipeFd();
        mConnectionsByReceiveFd.add(receiveFd, connection);

        looper->addFd(receiveFd, 0, ALOOPER_EVENT_INPUT, HandleReceiveCallback, this);
    } // release lock

    nativeInputChannel->setDisposeCallback(HandleInputChannelDisposed, this);

    return android::OK;
}

android::status_t NativeInputQueue::UnregisterInputChannel(
    /* [in] */ CInputChannel* _inputChannel)
{
    NativeInputChannel* nativeInputChannel = _inputChannel->mPtr;
    android::sp<android::InputChannel> inputChannel =
            nativeInputChannel != NULL ? nativeInputChannel->getInputChannel() : NULL;
    if (inputChannel == NULL) {
        Slogger::W(TAG, "inputChannel is not initialized.");
        return android::BAD_VALUE;
    }

#if DEBUG_REGISTRATION
    Slogger::D(TAG, StringBuffer("channel ") + inputChannel->getName().string()
            + " - Unregistered.");
#endif

    { // acquire lock
        android::AutoMutex _l(mLock);

        ssize_t connectionIndex = GetConnectionIndex(inputChannel);
        if (connectionIndex < 0) {
            Slogger::W(TAG, StringBuffer("Attempted to register already registered input channel ")
                    + inputChannel->getName().string());
            return android::BAD_VALUE;
        }

        android::sp<Connection> connection = mConnectionsByReceiveFd.valueAt(connectionIndex);
        mConnectionsByReceiveFd.removeItemsAt(connectionIndex);

        connection->mStatus = Connection::STATUS_ZOMBIE;

        connection->mLooper->removeFd(inputChannel->getReceivePipeFd());

        delete connection->mInputHandler;
        connection->mInputHandler = NULL;

        if (connection->mMessageInProgress) {
            Slogger::I(TAG, StringBuffer("Sending finished signal for input channel ")
                    + connection->GetInputChannelName()
                    + " since it is being unregistered while an input message is still in progress.");
            connection->mMessageInProgress = FALSE;
            connection->mInputConsumer.sendFinishedSignal(); // ignoring result
        }
    } // release lock

    nativeInputChannel->setDisposeCallback(NULL, NULL);
    return android::OK;
}


ssize_t NativeInputQueue::GetConnectionIndex(
    /* [in] */ const android::sp<android::InputChannel>& inputChannel)
{
    ssize_t connectionIndex = mConnectionsByReceiveFd.indexOfKey(inputChannel->getReceivePipeFd());
    if (connectionIndex >= 0) {
        android::sp<Connection> connection = mConnectionsByReceiveFd.valueAt(connectionIndex);
        if (connection->mInputChannel.get() == inputChannel.get()) {
            return connectionIndex;
        }
    }

    return -1;
}

android::status_t NativeInputQueue::Finished(
    /* [in] */ long long finishedToken,
    /* [in] */ bool ignoreSpuriousFinish)
{
    int32_t receiveFd;
    uint16_t connectionId;
    uint16_t messageSeqNum;
    ParseFinishedToken(finishedToken, &receiveFd, &connectionId, &messageSeqNum);

    { // acquire lock
        android::AutoMutex _l(mLock);

        ssize_t connectionIndex = mConnectionsByReceiveFd.indexOfKey(receiveFd);
        if (connectionIndex < 0) {
            if (! ignoreSpuriousFinish) {
                Slogger::I(TAG, "Ignoring finish signal on channel that is no longer registered.");
            }
            return android::DEAD_OBJECT;
        }

        android::sp<Connection> connection = mConnectionsByReceiveFd.valueAt(connectionIndex);
        if (connectionId != connection->mId) {
            if (! ignoreSpuriousFinish) {
                Slogger::I(TAG, "Ignoring finish signal on channel that is no longer registered.");
            }
            return android::DEAD_OBJECT;
        }

        if (messageSeqNum != connection->mMessageSeqNum || ! connection->mMessageInProgress) {
            if (! ignoreSpuriousFinish) {
                Slogger::W(TAG, StringBuffer("Attempted to finish input twice on channel ")
                        + connection->GetInputChannelName() + ". "
                        + "finished messageSeqNum=" + messageSeqNum + ", "
                        + "current messageSeqNum=" + connection->mMessageSeqNum + ", "
                        + "messageInProgress=" + connection->mMessageInProgress + ".");
            }
            return android::INVALID_OPERATION;
        }

        connection->mMessageInProgress = FALSE;

        android::status_t status = connection->mInputConsumer.sendFinishedSignal();
        if (status) {
            Slogger::W(TAG, StringBuffer("Failed to send finished signal on channel ")
                    + connection->GetInputChannelName()
                    + ", status=" + status + ".");
            return status;
        }

#if DEBUG_DISPATCH_CYCLE
        Slogger::D(TAG, StringBuffer("channel ")
                + connection->GetInputChannelName() + " ~ Finished event.");
#endif
    } // release lock

    return android::OK;
}

void NativeInputQueue::HandleInputChannelDisposed(
    /* [in] */ CInputChannel* _inputChannel,
    /* [in] */ const android::sp<android::InputChannel>& inputChannel,
    /* [in] */ void* data)
{
    Slogger::W(TAG, StringBuffer("Input channel object ")
            + inputChannel->getName().string()
            + " was disposed without first being unregistered with the input queue!");

    NativeInputQueue* q = static_cast<NativeInputQueue*>(data);
    q->UnregisterInputChannel(_inputChannel);
}

int NativeInputQueue::HandleReceiveCallback(
    /* [in] */ int receiveFd,
    /* [in] */ int events,
    /* [in] */ void* data)
{
    //printf("######FILE: %s, LINE: %d######\n", __FILE__, __LINE__);
    NativeInputQueue* q = static_cast<NativeInputQueue*>(data);

    android::sp<Connection> connection;
    android::InputEvent* inputEvent;
    InputHandler* inputHandler;
    long long finishedToken;
    { // acquire lock
        android::AutoMutex _l(q->mLock);

        ssize_t connectionIndex = q->mConnectionsByReceiveFd.indexOfKey(receiveFd);
        if (connectionIndex < 0) {
            Slogger::E(TAG, StringBuffer("Received spurious receive callback for unknown input channel.  ")
                    + "fd=" + receiveFd + ", events=0x" + events);
            return 0; // remove the callback
        }

        connection = q->mConnectionsByReceiveFd.valueAt(connectionIndex);
        if (events & (ALOOPER_EVENT_ERROR | ALOOPER_EVENT_HANGUP)) {
            Slogger::E(TAG, StringBuffer("channel ")
                + connection->GetInputChannelName()
                + " ~ Publisher closed input channel or an error occurred.  "
                "events=0x" + events);
            return 0; // remove the callback
        }

        if (! (events & ALOOPER_EVENT_INPUT)) {
            Slogger::W(TAG, StringBuffer("channel ")
                + connection->GetInputChannelName()
                + " ~ Received spurious callback for unhandled poll event.  "
                "events=0x" + events);
            return 1;
        }

        android::status_t status = connection->mInputConsumer.receiveDispatchSignal();
        if (status) {
            Slogger::E(TAG, StringBuffer("channel ")
                + connection->GetInputChannelName()
                + " ~ Failed to receive dispatch signal.  status="
                + status);
            return 0; // remove the callback
        }

        if (connection->mMessageInProgress) {
            Slogger::W(TAG, StringBuffer("channel ")
                + connection->GetInputChannelName()
                + " ~ Publisher sent spurious dispatch signal.");
            return 1;
        }

        status = connection->mInputConsumer.consume(&connection->mInputEventFactory, &inputEvent);
        if (status) {
            Slogger::W(TAG, StringBuffer("channel ")
                + connection->GetInputChannelName()
                + " ~ Failed to consume input event.  status="
                + status);
            connection->mInputConsumer.sendFinishedSignal();
            return 1;
        }

        connection->mMessageInProgress = TRUE;
        connection->mMessageSeqNum += 1;

        finishedToken = GenerateFinishedToken(receiveFd, connection->mId, connection->mMessageSeqNum);

        inputHandler = connection->mInputHandler;
    } // release lock

    // Invoke the handler outside of the lock.
    //
    // Note: inputEvent is stored in a field of the connection object which could potentially
    //       become disposed due to the input channel being unregistered concurrently.
    //       For this reason, we explicitly keep the connection object alive by holding
    //       a strong pointer to it within this scope.  We also grabbed a local reference to
    //       the input handler object itself for the same reason.

    int32_t inputEventType = inputEvent->getType();
    ECode ec;
    AutoPtr<CKeyEvent> keyEvent;
    AutoPtr<CMotionEvent> motionEvent;
    switch (inputEventType) {
    case AINPUT_EVENT_TYPE_KEY:
#if DEBUG_DISPATCH_CYCLE
        Slogger::D(TAG, StringBuffer("channel ")
                + connection->getInputChannelName() + " ~ Received key event.");
#endif

        assert(SUCCEEDED(create_CKeyEvent_fromNative(
                static_cast<android::KeyEvent*>(inputEvent), (CKeyEvent**)&keyEvent)));

        if (!inputEvent) {
            Slogger::W(TAG, StringBuffer("channel ")
                    + connection->GetInputChannelName()
                    + " ~ Failed to obtain DVM event object.");
            q->Finished(finishedToken, FALSE);
            return 1;
        }

#if DEBUG_DISPATCH_CYCLE
        Slogger::D(TAG, "Invoking input handler.");
#endif
        ec = InputQueue::DispatchKeyEvent(inputHandler, keyEvent.Get(), finishedToken);
#if DEBUG_DISPATCH_CYCLE
        Slogger::D(TAG, "Returned from input handler.");
#endif

        break;

    case AINPUT_EVENT_TYPE_MOTION:
#if DEBUG_DISPATCH_CYCLE
        Slogger::D(TAG, StringBuffer("channel ")
                + connection->GetInputChannelName() + " ~ Received motion event.");
#endif

        assert(SUCCEEDED(create_CMotionEvent_fromNative(
                static_cast<android::MotionEvent*>(inputEvent), (CMotionEvent**)&motionEvent)));

        if (!inputEvent) {
            Slogger::W(TAG, StringBuffer("channel ")
                    + connection->GetInputChannelName()
                    + " ~ Failed to obtain DVM event object.");
            q->Finished(finishedToken, FALSE);
            return 1;
        }

#if DEBUG_DISPATCH_CYCLE
        Slogger::D(TAG, "Invoking input handler.");
#endif
        ec = InputQueue::DispatchMotionEvent(inputHandler, motionEvent.Get(), finishedToken);
#if DEBUG_DISPATCH_CYCLE
        Slogger::D(TAG, "Returned from input handler.");
#endif

        break;

    default:
        assert(FALSE); // InputConsumer should prevent this from ever happening
        inputHandler = NULL;
    }

    if (FAILED(ec)) {
        Slogger::E(TAG, "An exception occurred while invoking the input handler for an event.");
        q->Finished(finishedToken, TRUE /*ignoreSpuriousFinish*/);
    }

    //printf("######FILE: %s, LINE: %d######\n", __FILE__, __LINE__);

    return 1;
}

long long NativeInputQueue::GenerateFinishedToken(
    /* [in] */ int32_t receiveFd,
    /* [in] */ uint16_t connectionId,
    /* [in] */ uint16_t messageSeqNum)
{
    return ((long long)(receiveFd) << 32) |
            ((long long)(connectionId) << 16) |
            (long long)(messageSeqNum);
}

void NativeInputQueue::ParseFinishedToken(
    /* [in] */ long long finishedToken,
    /* [out] */ int32_t* outReceiveFd,
    /* [out] */ uint16_t* outConnectionId,
    /* [out] */ uint16_t* outMessageIndex)
{
    *outReceiveFd = int32_t(finishedToken >> 32);
    *outConnectionId = uint16_t(finishedToken >> 16);
    *outMessageIndex = uint16_t(finishedToken);
}
