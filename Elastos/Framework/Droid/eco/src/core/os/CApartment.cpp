
#include "os/CApartment.h"
#include "os/SystemClock.h"
#include <new>


AutoPtr<CApartment> CApartment::sDefaultApartment;
Boolean CApartment::sHaveKey = FALSE;
pthread_key_t CApartment::sKey;
Mutex CApartment::sLock;

// --- InputDispatcherThread ---

NativeMessageQueueThread::NativeMessageQueueThread(
    /* [in] */ NativeMessageQueue* messageQueue) :
    Thread(/*canCallJava*/ FALSE),
    mMessageQueue(messageQueue)
{
}

NativeMessageQueueThread::~NativeMessageQueueThread()
{
}

bool NativeMessageQueueThread::threadLoop()
{
    mMessageQueue->PollOnce(-1);
    return true;
}

// --- CApartment ---

CApartment::CApartment() :
    mFinished(FALSE)
{
}

CApartment::~CApartment()
{
    if (mFinished == FALSE) {
        Finish();
    }
}

ECode CApartment::constructor(
    /* [in] */ Boolean usingNativeMessageQueue)
{
    mUsingNativeMessageQueue = usingNativeMessageQueue;
    if (mUsingNativeMessageQueue) {
        mMessageQueue = new NativeMessageQueue();
        mNativeMessageQueueThread = new NativeMessageQueueThread(mMessageQueue);
    }

    mCallbackContext = new CCallbackContextEx();
    assert(mCallbackContext != NULL);
    assert(SUCCEEDED(mCallbackContext->Initialize()));

    if (sDefaultApartment == NULL) {
        sDefaultApartment = this;
    }

    if (!sHaveKey) {
        assert(pthread_key_create(&sKey, NULL) == 0);
        sHaveKey = TRUE;
    }
    assert(pthread_setspecific(sKey, NULL) == 0);

    return NOERROR;
}

void* CApartment::EntryRoutine(void *arg)
{
    ECode ec;

    if (arg == NULL) {
        pthread_exit((void*)E_THREAD_ABORTED);
    }

    ec = ((CApartment*)arg)->mCallbackContext->HandleCallbackEvents(INFINITE, NULL, NULL, 0);
    ((CApartment*)arg)->mCallbackContext->m_Status = CallbackContextStatus_Finishing;

    return (void*)ec;
}

ECode CApartment::Start(
    /* [in] */ ApartmentAttr attr)
{

    if (mUsingNativeMessageQueue) {
        android::status_t result = mNativeMessageQueueThread->run(
                "NativeMessageQueue", android::PRIORITY_URGENT_DISPLAY);
        if (result) {
//            LOGE("Could not start InputDispatcher thread due to error %d.", result);
            return E_THREAD_ABORTED;
        }
    }

    if (attr == ApartmentAttr_New) {
        if (pthread_create(&mThread, NULL, EntryRoutine, (void*)this)) {
            return E_THREAD_ABORTED;
        }
    }
    else if (attr == ApartmentAttr_Current){
        mThread = pthread_self();
        CApartment::EntryRoutine((void*)this);
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode CApartment::Finish()
{
    ECode ec;
    void *ret;

    mFinished = TRUE;

    assert(mCallbackContext != NULL);
    ec = mCallbackContext->RequestToFinish(CallbackContextFinish_ASAP);
    if (!pthread_equal(mThread, pthread_self())) {
        pthread_join(mThread, &ret);
    }

    return ec;
}

ECode CApartment::PostCppCallback(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id)
{
    return PostCppCallbackDelayed(target, func, params, id, 0);
}

ECode CApartment::PostCppCallbackAtTime(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id,
    /* [in] */ Millisecond64 uptimeMillis)
{
    PCallbackEvent pCallbackEvent = NULL;
    AutoPtr<IInterface> callbackContext;
    EventHandler delegate(EventHandler::Make((PVoid)target, *(PVoid*)&func, CallbackType_CPP));

    pCallbackEvent = _Impl_CallbackSink_AllocCallbackEvent(sizeof(_EzCallbackEvent));
    pCallbackEvent = new(pCallbackEvent) _EzCallbackEvent(
            id,
            CallbackEventFlag_DirectCall | CallbackEventFlag_SyncCall | CallbackPriority_Normal,
            NULL,
            delegate.m_pCarObjClient,
            NULL,
            delegate.GetThisPtr(), delegate.GetFuncPtr(),
            delegate.GetFuncType(), params);
    if (!pCallbackEvent) {
        return E_OUT_OF_MEMORY;
    }

    pCallbackEvent->m_when = uptimeMillis;
    return mCallbackContext->PostCallbackEvent(pCallbackEvent);
}

ECode CApartment::PostCppCallbackDelayed(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id,
    /* [in] */ Millisecond64 delayMillis)
{
    if (delayMillis < 0) {
        delayMillis = 0;
    }
    return PostCppCallbackAtTime(target, func, params, id, SystemClock::GetUptimeMillis() + delayMillis);
}

ECode CApartment::PostCppCallbackAtFrontOfQueue(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id)
{
    return PostCppCallbackAtTime(target, func, params, id, 0);
}

ECode CApartment::RemoveCppCallbacks(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func)
{
    return RemoveCppCallbacksEx(target, func, 0);
}

ECode CApartment::RemoveCppCallbacksEx(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ Int32 id)
{
    PVoid fun = NULL;
    if (func != NULL) {
        EventHandler delegate(EventHandler::Make((PVoid)target, *(PVoid*)&func, CallbackType_CPP));
        fun = delegate.GetFuncPtr();
    }

    return mCallbackContext->CancelCallbackEvents(
            NULL, id, (PVoid)target, fun);
}

ECode CApartment::HasCppCallbacks(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [out] */ Boolean* result)
{
    return HasCppCallbacksEx(target, func, 0, result);
}

ECode CApartment::HasCppCallbacksEx(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ Int32 id,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mCallbackContext->HasCallbackEvent(
            NULL, id, (PVoid)target, (PVoid)func);
    return NOERROR;
}

ECode CApartment::SendMessage(
    /* [in] */ Int32 message,
    /* [in] */ IParcel* params)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CApartment::Prepare()
{
    if (pthread_getspecific(sKey) != NULL) {
        // throw new RuntimeException("Only one Looper may be created per thread");
        return E_RUNTIME_EXCEPTION;
    }

    AutoPtr<IApartment> apartment;
    CApartment::New(FALSE, (IApartment**)&apartment);
    assert(pthread_setspecific(sKey, apartment.Get()) == 0);
    //todo: for sKey
    apartment->AddRef();
    return NOERROR;
}

    /** Initialize the current thread as a looper, marking it as an application's main
 *  looper. The main looper for your application is created by the Android environment,
 *  so you should never need to call this function yourself.
 * {@link #prepare()}
 */
ECode CApartment::PrepareMainApartment()
{
    if (pthread_getspecific(sKey) != NULL) {
        // throw new RuntimeException("Only one Looper may be created per thread");
        return E_RUNTIME_EXCEPTION;
    }

    AutoPtr<IApartment> apartment;
    CApartment::New(TRUE, (IApartment**)&apartment);
    assert(pthread_setspecific(sKey, apartment.Get()) == 0);
    //todo: for sKey
    apartment->AddRef();

    SetMainApartment(apartment);
    // if (Process.supportsProcesses()) {
    //     myLooper().mQueue.mQuitAllowed = false;
    // }
    return NOERROR;
}

void CApartment::SetMainApartment(
    /* [in] */ IApartment* apartment)
{
    Mutex::Autolock lock(&sLock);

    sDefaultApartment = (CApartment*)apartment;
}

ECode CApartment::GetMainApartment(
    /* [out] */ IApartment** apartment)
{
    Mutex::Autolock lock(&sLock);

    if (apartment == NULL) return E_INVALID_ARGUMENT;

    *apartment = sDefaultApartment;
    if (*apartment != NULL) (*apartment)->AddRef();
    return NOERROR;
}

ECode CApartment::GetMyApartment(
    /* [out] */ IApartment** apartment)
{
    if (apartment == NULL) return E_INVALID_ARGUMENT;

    *apartment = (CApartment*)pthread_getspecific(sKey);
    if (*apartment != NULL) (*apartment)->AddRef();

    return NOERROR;
}

NativeMessageQueue* CApartment::GetNativeMessageQueue()
{
    AutoPtr<CApartment> apartment;
    {
        Mutex::Autolock lock(&sLock);

        apartment = sDefaultApartment;
    }
    if (apartment != NULL) {
        return apartment->mMessageQueue;
    }
    else {
        return NULL;
    }
}
