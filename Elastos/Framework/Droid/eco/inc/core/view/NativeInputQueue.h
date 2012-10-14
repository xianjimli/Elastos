
#ifndef __NATIVEINPUTQUEUE_H__
#define __NATIVEINPUTQUEUE_H__

#include <utils/Looper.h>
#include <ui/InputTransport.h>
#include "os/NativeMessageQueue.h"
#include "view/InputHandler.h"
#include "view/CInputChannel.h"

class NativeInputQueue
{
private:
    class Connection : public android::RefBase
    {
    protected:
        virtual ~Connection();

    public:
        enum Status {
            // Everything is peachy.
            STATUS_NORMAL,
            // The input channel has been unregistered.
            STATUS_ZOMBIE
        };

        Connection(
            /* [in] */ uint16_t id,
            /* [in] */ const android::sp<android::InputChannel>& inputChannel,
            /* [in] */ const android::sp<android::Looper>& looper);

        inline const char* GetInputChannelName() const
        { return mInputChannel->getName().string(); }

        // A unique id for this connection.
        uint16_t mId;

        Status mStatus;

        android::sp<android::InputChannel> mInputChannel;
        android::InputConsumer mInputConsumer;
        android::sp<android::Looper> mLooper;
        InputHandler* mInputHandler;
        android::PreallocatedInputEventFactory mInputEventFactory;

        // The sequence number of the current event being dispatched.
        // This is used as part of the finished token as a way to determine whether the finished
        // token is still valid before sending a finished signal back to the publisher.
        uint16_t mMessageSeqNum;

        // True if a message has been received from the publisher but not yet finished.
        bool mMessageInProgress;
    };

private:
    static const char* TAG;

public:
    NativeInputQueue();
    ~NativeInputQueue();

    android::status_t RegisterInputChannel(
        /* [in] */ CInputChannel* _inputChannel,
        /* [in] */ InputHandler* inputHandler,
        /* [in] */ NativeMessageQueue* messageQueue);

    android::status_t UnregisterInputChannel(
        /* [in] */ CInputChannel* _inputChannel);

    android::status_t Finished(
        /* [in] */ long long finishedToken,
        /* [in] */ bool ignoreSpuriousFinish);

private:
    ssize_t GetConnectionIndex(
        /* [in] */ const android::sp<android::InputChannel>& inputChannel);

    static void HandleInputChannelDisposed(
        /* [in] */ IInputChannel* _inputChannel,
        /* [in] */ const android::sp<android::InputChannel>& inputChannel,
        /* [in] */ void* data);

    static int HandleReceiveCallback(
        /* [in] */ int receiveFd,
        /* [in] */ int events,
        /* [in] */ void* data);

    static long long GenerateFinishedToken(
        /* [in] */ int32_t receiveFd,
        /* [in] */ uint16_t connectionId,
        /* [in] */ uint16_t messageSeqNum);

    static void ParseFinishedToken(
        /* [in] */ long long finishedToken,
        /* [out] */ int32_t* outReceiveFd,
        /* [out] */ uint16_t* outConnectionId,
        /* [out] */ uint16_t* outMessageIndex);

private:
    android::Mutex mLock;
    uint16_t mNextConnectionId;
    android::KeyedVector<int32_t, android::sp<Connection> > mConnectionsByReceiveFd;
};

#endif //__NATIVEINPUTQUEUE_H__
