
#ifndef __CAPARTMENT_H__
#define __CAPARTMENT_H__

#include <pthread.h>
#include "_CApartment.h"
#include "os/CCallbackContextEx.h"
#ifdef _linux
#include "os/NativeMessageQueue.h"
#endif
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>

using namespace Elastos::Core::Threading;

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
    friend class CApartmentHelper;

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
        /* [in] */ IParcel* params,
        /* [in] */ Int32 id);

    CARAPI PostCppCallbackAtTime(
        /* [in] */ Handle32 target,
        /* [in] */ Handle32 func,
        /* [in] */ IParcel* params,
        /* [in] */ Int32 id,
        /* [in] */ Millisecond64 uptimeMillis);

    CARAPI PostCppCallbackDelayed(
        /* [in] */ Handle32 target,
        /* [in] */ Handle32 func,
        /* [in] */ IParcel* params,
        /* [in] */ Int32 id,
        /* [in] */ Millisecond64 delayMillis);

    CARAPI PostCppCallbackAtFrontOfQueue(
        /* [in] */ Handle32 target,
        /* [in] */ Handle32 func,
        /* [in] */ IParcel* params,
        /* [in] */ Int32 id);

    CARAPI RemoveCppCallbacks(
        /* [in] */ Handle32 target,
        /* [in] */ Handle32 func);

    CARAPI RemoveCppCallbacksEx(
        /* [in] */ Handle32 target,
        /* [in] */ Handle32 func,
        /* [in] */ Int32 id);

    CARAPI HasCppCallbacks(
        /* [in] */ Handle32 target,
        /* [in] */ Handle32 func,
        /* [out] */ Boolean* result);

    CARAPI HasCppCallbacksEx(
        /* [in] */ Handle32 target,
        /* [in] */ Handle32 func,
        /* [in] */ Int32 id,
        /* [out] */ Boolean* result);

    CARAPI SendMessage(
        /* [in] */ Int32 message,
        /* [in] */ IParcel* params);

public:
    /** Initialize the current thread as a looper.
      * This gives you a chance to create handlers that then reference
      * this looper, before actually starting the loop. Be sure to call
      * {@link #loop()} after calling this method, and end it by calling
      * {@link #quit()}.
      */
    static CARAPI Prepare();

    /** Initialize the current thread as a looper, marking it as an application's main
     *  looper. The main looper for your application is created by the Android environment,
     *  so you should never need to call this function yourself.
     * {@link #prepare()}
     */
    static CARAPI PrepareMainApartment();

    static CARAPI GetMainApartment(
        /* [out] */ IApartment** apartment);

    static CARAPI GetMyApartment(
        /* [out] */ IApartment** apartment);

    static CARAPI_(NativeMessageQueue*) GetNativeMessageQueue();

private:
    static void* EntryRoutine(void *arg);

    static CARAPI_(void) SetMainApartment(
        /* [in] */ IApartment* apartment);

private:
    static AutoPtr<CApartment> sDefaultApartment;
    static Boolean sHaveKey;
    static pthread_key_t sKey;
    static Mutex sLock;

private:
    pthread_t mThread;
    Boolean mFinished;
    AutoPtr<CCallbackContextEx> mCallbackContext;
    Boolean mUsingNativeMessageQueue;
    NativeMessageQueue* mMessageQueue;
    android::sp<NativeMessageQueueThread> mNativeMessageQueueThread;
};

#endif // __CAPARTMENT_H__
