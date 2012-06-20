
#ifndef __CAPARTMENT_H__
#define __CAPARTMENT_H__

#include <pthread.h>
#include "_CApartment.h"
#include "os/CCallbackContextEx.h"
#ifdef _linux
#include "os/NativeMessageQueue.h"
#endif
#include <elastos/AutoPtr.h>

#ifdef _linux
/* Enqueues and dispatches input events, endlessly. */
class NativeMessageQueueThread : public android::Thread
{
public:
    NativeMessageQueueThread(
        /* [in] */ NativeMessageQueue* messageQueue);

    ~NativeMessageQueueThread();

private:
    bool threadLoop();

    NativeMessageQueue* mMessageQueue;
};
#endif

CarClass(CApartment)
{
public:
    CApartment();

    ~CApartment();

    CARAPI constructor(
        /* [in] */ Boolean usingNativeMessageQueue);

    CARAPI Start(
        /* [in] */ ApartmentAttr attr);

    CARAPI Finish();

    CARAPI PostCppCallback(
        /* [in] */ Handle32 target,
        /* [in] */ Handle32 func,
        /* [in] */ IParcel* params);

    CARAPI PostCppCallbackAtTime(
        /* [in] */ Handle32 target,
        /* [in] */ Handle32 func,
        /* [in] */ IParcel* params,
        /* [in] */ Millisecond64 uptimeMillis);

    // func : Boolean xxx::xxx(IParcel** params)
    //          return true if cancelled event.
    CARAPI RemoveCppCallbacks(
        /* [in] */ Handle32 target,
        /* [in] */ Handle32 func);

public:
    static CARAPI GetDefaultApartment(
        /* [out] */ IApartment** apartment);

    static CARAPI_(NativeMessageQueue*) GetNativeMessageQueue();

private:
    static void* EntryRoutine(void *arg);

private:
    static pthread_key_t sKey;

private:
    pthread_t   mThread;
    Boolean     mFinished;
    AutoPtr<CCallbackContextEx> mCallbackContext;
#ifdef _linux
    Boolean mUsingNativeMessageQueue;
    NativeMessageQueue* mMessageQueue;
    android::sp<NativeMessageQueueThread> mNativeMessageQueueThread;
#endif
};

#endif // __CAPARTMENT_H__
