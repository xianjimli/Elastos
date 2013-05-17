
#include "webkit/CHttpAuthHandler.h"
#include "os/CApartment.h"
#include "webkit/CBrowserFrame.h"

const CString CHttpAuthHandler::LOGTAG = "network";

const Int32 CHttpAuthHandler::AUTH_PROCEED;
const Int32 CHttpAuthHandler::AUTH_CANCEL;

CHttpAuthHandler::construct(
    /* [in] */ Network * network)
{
    assert(SUCCEEDED(CApartment::GetMainApartment((IApartment**)&mApartment))
        && (mApartment != NULL));
    mNetwork = network;
}

Boolean CHttpAuthHandler::HandleResponseForSynchronousRequest(
        /* [in] */ String username, 
        /* [in] */ String password)
{
    AutoPtr<LoadListener> loader = NULL;

    mLoaderQueueMutex.Lock();
    loader = mLoaderQueue.GetFront();
    mLoaderQueueMutex.Unlock();

    if (loader->IsSynchronous()) {
        mUsername = username;
        mPassword = password;
        return TRUE;
    }
    return FALSE;
}

void CHttpAuthHandler::SignalRequestComplete()
{
    Core::Threading::Mutex::Autolock lock(mRequestInFlightLock);
    assert(mRequestInFlight);
    mRequestInFlight = FALSE;
    //JAVA:  mRequestInFlightLock.notify();
}

ECode CHttpAuthHandler::Proceed(
    /* [in] */ const String& username,
    /* [in] */ const String& password)
{
    if ( HandleResponseForSynchronousRequest( username, password ) ) {
        SignalRequestComplete();
        return NOERROR;
    }
    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteString(username);
    params->WriteString(password);
    SendMessage(AUTH_PROCEED, params);
    SignalRequestComplete();
    return NOERROR;
}

ECode CHttpAuthHandler::Cancel()
{
    if (HandleResponseForSynchronousRequest((String)"", (String)"")) {
        SignalRequestComplete();
        return NOERROR;
    }
    SendMessage(AUTH_CANCEL, NULL);
    SignalRequestComplete();
    return NOERROR;
}

ECode CHttpAuthHandler::UseHttpAuthUsernamePassword(
    /* [out] */ Boolean * pFlag)
{
    AutoPtr<LoadListener> loader;

    mLoaderQueueMutex.Lock();
    loader = mLoaderQueue.GetFront();
    mLoaderQueueMutex.Unlock();

    if (loader != NULL) {
        *pFlag = !loader->AuthCredentialsInvalid();
        return NOERROR;
    }

    *pFlag = FALSE;
    return NOERROR;
}

void CHttpAuthHandler::HandleAuthRequest(
    /* [in] */ LoadListener *loader)
{
    // The call to proxy.onReceivedHttpAuthRequest() may be asynchronous. If
    // the request is synchronous, we must block here until we have a
    // response.
    if (loader->IsSynchronous()) {
        // If there's a request in flight, wait for it to complete. The
        // response will queue a message on this thread.
        WaitForRequestToComplete();
        // Make a request to the proxy for this request, jumping the queue.
        // We use the queue so that the loader is present in
        // useHttpAuthUsernamePassword().
        mLoaderQueueMutex.Lock();
        mLoaderQueue.PushFront(loader);
        mLoaderQueueMutex.Unlock();

        ProcessNextLoader();
        // Wait for this request to complete.
        WaitForRequestToComplete();
        // Pop the loader from the queue.
        mLoaderQueueMutex.Lock();
        {
            LoadListener* loadfront = mLoaderQueue.GetFront();
            assert(loadfront == loader);
            mLoaderQueue.PopFront();
        }
        mLoaderQueueMutex.Unlock();

        // Call back.
        loader->HandleAuthResponse(mUsername, mPassword);
        // The message queued by the response from the last asynchronous
        // request, if present, will start the next request.
        return;
    }

    Boolean processNext = FALSE;

    mLoaderQueueMutex.Lock();
    {
        mLoaderQueue.PushBack(loader);
        processNext = (mLoaderQueue.GetSize() == 1);
    }
    mLoaderQueueMutex.Unlock();

    if (processNext) {
        ProcessNextLoader();
    }
}

void CHttpAuthHandler::WaitForRequestToComplete()
{
    Core::Threading::Mutex::Autolock lock(mRequestInFlightLock);
    while (mRequestInFlight) {
        // try {
        //     mRequestInFlightLock.wait();
        // } catch(InterruptedException e) {
        //     Log.e(LOGTAG, "Interrupted while waiting for request to complete");
        // }
    }

}

void CHttpAuthHandler::ProcessNextLoader()
{
    AutoPtr<LoadListener> loader;
    mLoaderQueueMutex.Lock();
    loader = mLoaderQueue.GetFront();
    mLoaderQueueMutex.Unlock();

    if (loader != NULL) {
        mRequestInFlightLock.Lock();
        assert(mRequestInFlight == FALSE);
        mRequestInFlight = TRUE;
        mRequestInFlightLock.Unlock();

        AutoPtr<IBrowserFrame> browserFrame = loader->GetFrame();
        CBrowserFrame* bf = (CBrowserFrame*)(browserFrame.Get());
        AutoPtr<ICallbackProxy> proxy = bf->GetCallbackProxy();

        String strProxyHostname;
        mNetwork->GetProxyHostname(/*&*/strProxyHostname);
        String strHost;
        loader->Host(&strHost);
        String hostname = loader->ProxyAuthenticate() ? strProxyHostname : strHost;

        String realm;
        loader->Realm(&realm);

        proxy->OnReceivedHttpAuthRequest(this, hostname, realm);
    }
}

void CHttpAuthHandler::OnReceivedCredentials(
    /* [in] */ LoadListener* loader,
    /* [in] */ String host, 
    /* [in] */ String realm, 
    /* [in] */ String username, 
    /* [in] */ String password)
{
    AutoPtr<IBrowserFrame> browserFrame = loader->GetFrame();
    CBrowserFrame* bf = (CBrowserFrame*)(browserFrame.Get());
    AutoPtr<ICallbackProxy> proxy = bf->GetCallbackProxy();
    proxy->OnReceivedHttpAuthCredentials(host, realm, username, password);
}

ECode CHttpAuthHandler::Start(
    /* [in] */ ApartmentAttr attr)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CHttpAuthHandler::Finish()
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CHttpAuthHandler::PostCppCallback(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallback(target, func, params, id);
}

ECode CHttpAuthHandler::PostCppCallbackAtTime(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id,
    /* [in] */ Millisecond64 uptimeMillis)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallbackAtTime(target, func, params, id, uptimeMillis);
}

ECode CHttpAuthHandler::PostCppCallbackDelayed(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id,
    /* [in] */ Millisecond64 delayMillis)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallbackDelayed(target, func, params, id, delayMillis);
}

ECode CHttpAuthHandler::PostCppCallbackAtFrontOfQueue(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallbackAtFrontOfQueue(target, func, params, id);
}

ECode CHttpAuthHandler::RemoveCppCallbacks(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func)
{
    assert(mApartment != NULL);
    return mApartment->RemoveCppCallbacks(target, func);
}

ECode CHttpAuthHandler::RemoveCppCallbacksEx(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ Int32 id)
{
    assert(mApartment != NULL);
    return mApartment->RemoveCppCallbacksEx(target, func, id);
}

ECode CHttpAuthHandler::HasCppCallbacks(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [out] */ Boolean* result)
{
    assert(mApartment != NULL);
    return mApartment->HasCppCallbacks(target, func, result);
}

ECode CHttpAuthHandler::HasCppCallbacksEx(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ Int32 id,
    /* [out] */ Boolean* result)
{
    assert(mApartment != NULL);
    return mApartment->HasCppCallbacksEx(target, func, id, result);
}

ECode CHttpAuthHandler::SendMessage(
    /* [in] */ Int32 message,
    /* [in] */ IParcel* params)
{
	ECode ec = E_ILLEGAL_ARGUMENT_EXCEPTION;

    if (message == AUTH_PROCEED) {
        void (STDCALL CHttpAuthHandler::*pHandlerFunc)(String, String);
        pHandlerFunc = &CHttpAuthHandler::HandleAuthProceed;
        ec = mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
    }
    else if (message == AUTH_CANCEL) {
        void (STDCALL CHttpAuthHandler::*pHandlerFunc)();
        pHandlerFunc = &CHttpAuthHandler::HandleAuthCancel;
        ec = mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
    }
    return ec;
}

void CHttpAuthHandler::HandleAuthProceed(
        /* [in] */ String username,
        /* [in] */ String password)
{
	AutoPtr<LoadListener> loader = NULL;
	{
        Core::Threading::Mutex::Autolock lock(mLoaderQueueMutex);
		loader = mLoaderQueue.GetFront();
		mLoaderQueue.PopFront();
	}    
    assert(loader->IsSynchronous() == FALSE);

	loader->HandleAuthResponse(username, password);

    ProcessNextLoader();
}

void CHttpAuthHandler::HandleAuthCancel()
{
	AutoPtr<LoadListener> loader = NULL;
	{
        Core::Threading::Mutex::Autolock lock(mLoaderQueueMutex);
		loader = mLoaderQueue.GetFront();
		mLoaderQueue.PopFront();
	}    
    assert(loader->IsSynchronous() == FALSE);

	loader->HandleAuthResponse((String)"", (String)"");

    ProcessNextLoader();
}

