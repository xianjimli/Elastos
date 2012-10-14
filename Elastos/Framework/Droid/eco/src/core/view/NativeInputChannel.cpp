
#include "view/NativeInputChannel.h"

NativeInputChannel::NativeInputChannel(
    /* [in] */ const android::sp<android::InputChannel>& inputChannel) :
    mInputChannel(inputChannel),
    mDisposeCallback(NULL)
{
}

NativeInputChannel::~NativeInputChannel()
{
}

void NativeInputChannel::setDisposeCallback(
    /* [in] */ InputChannelObjDisposeCallback callback,
    /* [in] */ void* data)
{
    mDisposeCallback = callback;
    mDisposeData = data;
}

void NativeInputChannel::invokeAndRemoveDisposeCallback(
    /* [in] */ IInputChannel* obj)
{
    if (mDisposeCallback) {
        mDisposeCallback(obj, mInputChannel, mDisposeData);
        mDisposeCallback = NULL;
        mDisposeData = NULL;
    }
}
