
#include "ext/frameworkext.h"
#include "view/CInputChannel.h"
#include <Slogger.h>
#include <StringBuffer.h>

using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

const char* CInputChannel::TAG = "InputChannel";
const Boolean CInputChannel::DEBUG;

CInputChannel::CInputChannel() :
    mPtr(NULL),
    mDisposeAfterWriteToParcel(FALSE)
{
}

/**
 * Creates a new input channel pair.  One channel should be provided to the input
 * dispatcher and the other to the application's input queue.
 * @param name The descriptive (non-unique) name of the channel pair.
 * @return A pair of input channels.  They are symmetric and indistinguishable.
 */
ECode CInputChannel::OpenInputChannelPair(
    /* [in] */ const String& _name,
    /* [out] */ CInputChannel** inputChannel0,
    /* [out] */ CInputChannel** inputChannel1)
{
    assert(inputChannel0 != NULL && inputChannel1 != NULL);

    if (_name.IsNull()) {
        Slogger::E(TAG, "name must not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (DEBUG) {
        Slogger::D(TAG, StringBuffer("Opening input channel pair '") + _name + "'");
    }

    android::String8 name((const char*)_name);

    android::sp<android::InputChannel> serverChannel;
    android::sp<android::InputChannel> clientChannel;
    android::status_t result = android::InputChannel::openInputChannelPair(
            name, serverChannel, clientChannel);

    if (result) {
        Slogger::E(TAG, StringBuffer("Could not open input channel pair.  status=") + result);
        return E_RUNTIME_EXCEPTION;
    }

    // TODO more robust error checking
    CInputChannel::NewByFriend(inputChannel0);
    (*inputChannel0)->mPtr = new NativeInputChannel(serverChannel);
    CInputChannel::NewByFriend(inputChannel1);
    (*inputChannel1)->mPtr = new NativeInputChannel(clientChannel);

    return NOERROR;
}

/**
 * Disposes the input channel.
 * Explicitly releases the reference this object is holding on the input channel.
 * When all references are released, the input channel will be closed.
 */
ECode CInputChannel::Dispose()
{
    NativeInputChannel* nativeInputChannel = mPtr;
    if (nativeInputChannel) {
        nativeInputChannel->invokeAndRemoveDisposeCallback(this);

        mPtr = NULL;
        delete nativeInputChannel;
    }
    return NOERROR;
}

/**
 * Transfers ownership of the internal state of the input channel to another
 * instance and invalidates this instance.  This is used to pass an input channel
 * as an out parameter in a binder call.
 * @param other The other input channel instance.
 */
ECode CInputChannel::TransferToBinderOutParameter(
    /* [in] */ IInputChannel* outParameter)
{
    if (outParameter == NULL) {
        Slogger::E(TAG, "outParameter must not be null.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    CInputChannel* obj = (CInputChannel*)outParameter;
    if (obj->mPtr != NULL) {
        Slogger::E(TAG, "Other object already has a native input channel.");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    obj->mPtr = mPtr;
    mPtr = NULL;
    obj->mDisposeAfterWriteToParcel = TRUE;
    return NOERROR;
}

ECode CInputChannel::GetNativeInputChannel(
    /* [out] */ Handle32* nativeInputChannel)
{
    VALIDATE_NOT_NULL(nativeInputChannel);
    *nativeInputChannel = (Handle32)mPtr;
    return NOERROR;
}

ECode CInputChannel::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    if (source == NULL) {
        Slogger::E(TAG, "source must not be null.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (mPtr != NULL) {
        Slogger::E(TAG, "This object already has a native input channel.");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    Int32 isInitialized;
    source->ReadInt32(&isInitialized);
    if (isInitialized) {
        String _name;
        source->ReadString(&_name);
        Int32 _ashmemFd, _receivePipeFd, _sendPipeFd;
        source->ReadFileDescriptor(&_ashmemFd);
        int32_t ashmemFd = dup(_ashmemFd);
        source->ReadFileDescriptor(&_receivePipeFd);
        int32_t receivePipeFd = dup(_receivePipeFd);
        source->ReadFileDescriptor(&_sendPipeFd);
        int32_t sendPipeFd = dup(_sendPipeFd);
        if (ashmemFd < 0 || receivePipeFd < 0 || sendPipeFd < 0) {
            if (ashmemFd >= 0) ::close(ashmemFd);
            if (receivePipeFd >= 0) ::close(receivePipeFd);
            if (sendPipeFd >= 0) ::close(sendPipeFd);
            Slogger::E(TAG, "Could not read input channel file descriptors from parcel.");
            return E_RUNTIME_EXCEPTION;
        }

        android::String8 name((const char*)_name);
        android::InputChannel* inputChannel = new android::InputChannel(name, ashmemFd,
                receivePipeFd, sendPipeFd);
        NativeInputChannel* nativeInputChannel = new NativeInputChannel(inputChannel);

        mPtr = nativeInputChannel;
    }
    return NOERROR;
}

ECode CInputChannel::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    if (dest == NULL) {
        Slogger::E(TAG, "dest must not be null.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    NativeInputChannel* nativeInputChannel = mPtr;
    if (nativeInputChannel) {
        android::sp<android::InputChannel> inputChannel = nativeInputChannel->getInputChannel();

        dest->WriteInt32(1);
        dest->WriteString(String(inputChannel->getName().string()));
        dest->WriteDupFileDescriptor(inputChannel->getAshmemFd());
        dest->WriteDupFileDescriptor(inputChannel->getReceivePipeFd());
        dest->WriteDupFileDescriptor(inputChannel->getSendPipeFd());
    }
    else {
        dest->WriteInt32(0);
    }

    if (mDisposeAfterWriteToParcel) {
        Dispose();
    }
    return NOERROR;
}
