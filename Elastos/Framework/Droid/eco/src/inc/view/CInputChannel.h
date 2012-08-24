
#ifndef __CINPUTCHANNEL_H__
#define __CINPUTCHANNEL_H__

#include "_CInputChannel.h"
#ifdef _linux
#include "view/NativeInputChannel.h"
#endif

CarClass(CInputChannel)
{
private:
    static const char* TAG;

    static const Boolean DEBUG = FALSE;

public:
    /**
     * Creates an uninitialized input channel.
     * It can be initialized by reading from a Parcel or by transferring the state of
     * another input channel into this one.
     */
    CInputChannel();

    static CARAPI OpenInputChannelPair(
        /* [in] */ const String& name,
        /* [out] */ CInputChannel** inputChannel0,
        /* [out] */ CInputChannel** inputChannel1);

    /**
     * Disposes the input channel.
     * Explicitly releases the reference this object is holding on the input channel.
     * When all references are released, the input channel will be closed.
     */
    CARAPI_(void) Dispose();

    CARAPI TransferToBinderOutParameter(
        /* [in] */ CInputChannel* outParameter);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);

private:
    friend class InputManager;
#ifdef _linux
    friend class NativeInputQueue;
    friend class NativeInputManager;

    NativeInputChannel* mPtr; // used by native code
#endif

    Boolean mDisposeAfterWriteToParcel;
};

#endif //__CINPUTCHANNEL_H__
