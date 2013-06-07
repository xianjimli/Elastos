
#include "webkit/CSslErrorHandler.h"

#include "os/CBundle.h"
#include "webkit/DebugFlags.h"
#include "webkit/CBrowserFrame.h"
#include "os/CApartment.h"

#include <Logger.h>

void CSslErrorHandler::HandleResponse(
    /* [in] */ Int32 arg1,
    /* [in] */ LoadListener* obj)
{
    AutoPtr< LoadListener > loader = obj;
    if(TRUE) {
        Core::Threading::Mutex::Autolock lock(_m_syncLock);

        HandleSslErrorResponse(loader.Get(), loader -> SslError(), arg1 == 1);
        mLoaderQueue -> Remove(loader);
        FastProcessQueuedSslErrors();
    }
}

ECode CSslErrorHandler::constructor()
{
    assert(SUCCEEDED(CApartment::GetMainApartment((IApartment**)&mApartment))
        && (mApartment != NULL));
    _m_syncLock.mMutex = mSyncLock.mMutex;
    return NOERROR;
}

ECode CSslErrorHandler::constructor(
        /* [in] */ ISslErrorHandler * origin, 
        /* [in] */ LoadListener * listener)
{
    assert(SUCCEEDED(CApartment::GetMainApartment((IApartment**)&mApartment))
        && (mApartment != NULL));
    _m_syncLock.mMutex = mSyncLock.mMutex;
    SslErrorHandler::Init(origin, listener);
    return NOERROR;
}

ECode CSslErrorHandler::ProcessNextLoader(
        /* [out] */ Boolean * ret)
{
    VALIDATE_NOT_NULL(ret);
    Core::Threading::Mutex::Autolock lock(_m_syncLock);
    AutoPtr<LoadListener> loader = (mLoaderQueue -> GetFront());
    if (loader != NULL) 
    {
        // if this loader has been cancelled
        if (loader -> Cancelled()) 
        {
            // go to the following loader in the queue. Make sure this
            // loader has been removed from the queue.
            mLoaderQueue -> Remove(loader);
            *ret = TRUE;
            return NOERROR;
        }

        AutoPtr<ISslError> error = loader -> SslError();

        if (DebugFlags::sSSL_ERROR_HANDLER) 
        {
            //JAVA:Assert.assertNotNull(error);
            assert(error.Get() != NULL);        
        }

        // checkSslPrefTable will handle the ssl error response if the
        // answer is available. It does not remove the loader from the
        // queue.
        Boolean bCheckSslPrefTable = FALSE;
        bCheckSslPrefTable = CheckSslPrefTable(loader.Get(), error.Get());
        if (bCheckSslPrefTable) 
        {
            mLoaderQueue -> Remove(loader);
            *ret =TRUE;
            return NOERROR;
        }

        // if we do not have information on record, ask
        // the user (display a dialog)
        AutoPtr<ICallbackProxy> proxy = ((CBrowserFrame *)((loader -> GetFrame() ).Get() ) ) -> GetCallbackProxy();
        //JAVA:    proxy.onReceivedSslError(new SslErrorHandler(this, loader), error.Get() );
        AutoPtr<CSslErrorHandler> pSslErrorHandler;
        pSslErrorHandler =  new CSslErrorHandler();
        pSslErrorHandler -> Init(this,loader.Get());
        proxy -> OnReceivedSslError(pSslErrorHandler.Get(), error.Get());
    }
    // the queue must be empty, stop
    *ret = FALSE;
    return NOERROR;
}

ECode CSslErrorHandler::Proceed()
{
    //JAVA:mOriginHandler.sendMessage(mOriginHandler.obtainMessage(HANDLE_RESPONSE, 1, 0, mLoadListener));
    void (STDCALL CSslErrorHandler::*pHandlerFunc)(Int32,LoadListener*);
    pHandlerFunc = &CSslErrorHandler::HandleResponse;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32(1);
    params->WriteInterfacePtr((IInterface*)(mLoadListener.Get()));

    return mOriginHandler->PostCppCallback( (Handle32)(mOriginHandler.Get()), *(Handle32*)&pHandlerFunc, params, 0);
}

ECode CSslErrorHandler::Cancel()
{
    //JAVA:mOriginHandler.sendMessage(mOriginHandler.obtainMessage(HANDLE_RESPONSE, 0, 0, mLoadListener));
    void (STDCALL CSslErrorHandler::*pHandlerFunc)(Int32,LoadListener*);
    pHandlerFunc = &CSslErrorHandler::HandleResponse;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32(0);
    params->WriteInterfacePtr((IInterface*)(mLoadListener.Get()));

    return mOriginHandler->PostCppCallback( (Handle32)(mOriginHandler.Get()), *(Handle32*)&pHandlerFunc, params, 0);
}


ECode CSslErrorHandler::Start(
    /* [in] */ ApartmentAttr attr)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CSslErrorHandler::Finish()
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CSslErrorHandler::PostCppCallback(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallback(target, func, params, id);
}

ECode CSslErrorHandler::PostCppCallbackAtTime(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id,
    /* [in] */ Millisecond64 uptimeMillis)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallbackAtTime(target, func, params, id, uptimeMillis);
}

ECode CSslErrorHandler::PostCppCallbackDelayed(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id,
    /* [in] */ Millisecond64 delayMillis)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallbackDelayed(target, func, params, id, delayMillis);
}

ECode CSslErrorHandler::PostCppCallbackAtFrontOfQueue(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallbackAtFrontOfQueue(target, func, params, id);
}

ECode CSslErrorHandler::RemoveCppCallbacks(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func)
{
    assert(mApartment != NULL);
    return mApartment->RemoveCppCallbacks(target, func);
}

ECode CSslErrorHandler::RemoveCppCallbacksEx(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ Int32 id)
{
    assert(mApartment != NULL);
    return mApartment->RemoveCppCallbacksEx(target, func, id);
}

ECode CSslErrorHandler::HasCppCallbacks(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [out] */ Boolean* result)
{
    assert(mApartment != NULL);
    return mApartment->HasCppCallbacks(target, func, result);
}

ECode CSslErrorHandler::HasCppCallbacksEx(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ Int32 id,
    /* [out] */ Boolean* result)
{
    assert(mApartment != NULL);
    return mApartment->HasCppCallbacksEx(target, func, id, result);
}

ECode CSslErrorHandler::SendMessage(
    /* [in] */ Int32 message,
    /* [in] */ IParcel* params)
{
    if (message == HANDLE_RESPONSE) {
        void (STDCALL CSslErrorHandler::*pHandlerFunc)(Int32,LoadListener*);
        pHandlerFunc = &CSslErrorHandler::HandleResponse;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
    }
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}
