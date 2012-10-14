
#include "view/CInputChannelHelper.h"
#include "view/CInputChannel.h"
#include <Slogger.h>
#include <StringBuffer.h>

using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

ECode CInputChannelHelper::OpenInputChannelPair(
    /* [in] */ const String& _name,
    /* [out] */ IInputChannel** inputChannel0,
    /* [out] */ IInputChannel** inputChannel1)
{
    assert(inputChannel0 != NULL && inputChannel1 != NULL);

    if (_name.IsNull()) {
        Slogger::E(CInputChannel::TAG, "name must not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (CInputChannel::DEBUG) {
        Slogger::D(CInputChannel::TAG, StringBuffer("Opening input channel pair '") + _name + "'");
    }

    android::String8 name((const char*)_name);

    android::sp<android::InputChannel> serverChannel;
    android::sp<android::InputChannel> clientChannel;
    android::status_t result = android::InputChannel::openInputChannelPair(
            name, serverChannel, clientChannel);

    if (result) {
        Slogger::E(CInputChannel::TAG, StringBuffer("Could not open input channel pair.  status=") + result);
        return E_RUNTIME_EXCEPTION;
    }

    // TODO more robust error checking
    CInputChannel* channel0Obj;
    CInputChannel* channel1Obj;
    CInputChannel::NewByFriend(&channel0Obj);
    channel0Obj->mPtr = new NativeInputChannel(serverChannel);
    CInputChannel::NewByFriend(&channel1Obj);
    channel1Obj->mPtr = new NativeInputChannel(clientChannel);

    *inputChannel0 = (IInputChannel*)channel0Obj;
    *inputChannel1 = (IInputChannel*)channel1Obj;
    return NOERROR;
}
