
#include "webkit/HttpAuthHandler.h"
#include "webkit/LoadListener.h"
#include "webkit/CBrowserFrame.h"

const CString HttpAuthHandler::LOGTAG("network");

/**
 * Creates a new HTTP authentication handler with an empty
 * loader queue
 *
 * @param network The parent network object
 */
/* package */
HttpAuthHandler::HttpAuthHandler(
	/* [in] */ Network* network)
{
	assert(network != NULL);

	mNetwork = network;
//	mLoaderQueue = new LinkedList<LoadListener>();
}

PInterface HttpAuthHandler::Probe(
    /* [in] */ REIID riid)
{
    return NULL;
}

UInt32 HttpAuthHandler::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 HttpAuthHandler::Release()
{
    return ElRefBase::Release();
}

ECode HttpAuthHandler::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return NOERROR;
}

ECode HttpAuthHandler::Start(
    /* [in] */ ApartmentAttr attr)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode HttpAuthHandler::Finish()
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode HttpAuthHandler::PostCppCallback(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallback(target, func, params, id);
}

ECode HttpAuthHandler::PostCppCallbackAtTime(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id,
    /* [in] */ Millisecond64 uptimeMillis)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallbackAtTime(target, func, params, id, uptimeMillis);
}

ECode HttpAuthHandler::PostCppCallbackDelayed(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id,
    /* [in] */ Millisecond64 delayMillis)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallbackDelayed(target, func, params, id, delayMillis);
}

ECode HttpAuthHandler::PostCppCallbackAtFrontOfQueue(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallbackAtFrontOfQueue(target, func, params, id);
}

ECode HttpAuthHandler::RemoveCppCallbacks(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func)
{
    assert(mApartment != NULL);
    return mApartment->RemoveCppCallbacks(target, func);
}

ECode HttpAuthHandler::RemoveCppCallbacksEx(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ Int32 id)
{
    assert(mApartment != NULL);
    return mApartment->RemoveCppCallbacksEx(target, func, id);
}

ECode HttpAuthHandler::HasCppCallbacks(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [out] */ Boolean* result)
{
    assert(mApartment != NULL);
    return mApartment->HasCppCallbacks(target, func, result);
}

ECode HttpAuthHandler::HasCppCallbacksEx(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ Int32 id,
    /* [out] */ Boolean* result)
{
    assert(mApartment != NULL);
    return mApartment->HasCppCallbacksEx(target, func, id, result);
}

ECode HttpAuthHandler::SendMessage(
    /* [in] */ Int32 message,
    /* [in] */ IParcel* params)
{
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}


#if 0
//@Override
CARAPI_(void) HttpAuthHandler::HandleMessage(
	/* [in] */ IMessage* msg)
{
	assert(msg != NULL);

	AutoPtr<LoadListener> loader = NULL;

	{
        Core::Threading::Mutex::Autolock lock(mSyncLock);
		loader = mLoaderQueue.GetFront();
		mLoaderQueue.PopFront();
	}
    
    assert(loader->IsSynchronous() == FALSE);

    switch (0/*msg->what*/)
    {
        case AUTH_PROCEED:
	        {
	            String username;// = msg.getData().getString("username");
	            String password;// = msg.getData().getString("password");

	            loader->HandleAuthResponse(username, password);
	        }
            break;

        case AUTH_CANCEL:
            loader->HandleAuthResponse((String)"", (String)"");
            break;
    }

    ProcessNextLoader();
}
#endif

/**
 * Helper method used to unblock handleAuthRequest(), which in the case of a
 * synchronous request will wait for proxy.onReceivedHttpAuthRequest() to
 * call back to either proceed() or cancel().
 *
 * @param username The username to use for authentication
 * @param password The password to use for authentication
 * @return True if the request is synchronous and handleAuthRequest() has
 * been unblocked
 */
CARAPI_(Boolean) HttpAuthHandler::HandleResponseForSynchronousRequest(
	/* [in] */ const String& username, 
	/* [in] */ const String& password)
{
	AutoPtr<LoadListener> loader = NULL;

	mSyncLock.Lock();
	loader = mLoaderQueue.GetFront();
	mSyncLock.Unlock();

    if (loader->IsSynchronous()) {
        mUsername = username;
        mPassword = password;
        return TRUE;
    }

    return FALSE;
}

/**
 * Proceed with the authorization with the given credentials
 *
 * May be called on the UI thread, rather than the WebCore thread.
 *
 * @param username The username to use for authentication
 * @param password The password to use for authentication
 */
CARAPI_(void) HttpAuthHandler::Proceed(
	/* [in] */ const String& username, 
	/* [in] */ const String& password)
{
	if (HandleResponseForSynchronousRequest(username, password)) {
        SignalRequestComplete();
        return;
    }
    
    ECode (STDCALL HttpAuthHandler::*pHandlerFunc)(String&, String&);

    pHandlerFunc = &HttpAuthHandler::HandleAuthProceed;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteString(username);
    params->WriteString(password);

    SendMessage(*(Handle32*)&pHandlerFunc, params);

    SignalRequestComplete();
}

/**
 * Cancel the authorization request
 *
 * May be called on the UI thread, rather than the WebCore thread.
 *
 */
CARAPI_(void) HttpAuthHandler::Cancel()
{
	if (HandleResponseForSynchronousRequest((String)"", (String)"")) {
        SignalRequestComplete();
        return;
    }

//    SendMessage(ObtainMessage(AUTH_CANCEL));
//    SignalRequestComplete();

    ECode (STDCALL HttpAuthHandler::*pHandlerFunc)();

    pHandlerFunc = &HttpAuthHandler::HandleAuthCancel;

    SendMessage(*(Handle32*)&pHandlerFunc, NULL);

    SignalRequestComplete();
}

/**
 * @return True if we can use user credentials on record
 * (ie, if we did not fail trying to use them last time)
 */
CARAPI_(Boolean) HttpAuthHandler::UseHttpAuthUsernamePassword()
{
	AutoPtr<LoadListener> loader;

    mSyncLock.Lock();
	loader = mLoaderQueue.GetFront();
	mSyncLock.Unlock();

    if (loader != NULL) {
        return !loader->AuthCredentialsInvalid();
    }

    return FALSE;
}

/**
 * Enqueues the loader, if the loader is the only element
 * in the queue, starts processing the loader
 *
 * @param loader The loader that resulted in this http
 * authentication request
 */
/* package */
CARAPI_(void) HttpAuthHandler::HandleAuthRequest(
	/* [in] */ LoadListener* loader)
{
	assert(loader != NULL);

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
        mSyncLock.Lock();
        mLoaderQueue.PushFront(loader);
        mSyncLock.Unlock();

        ProcessNextLoader();
        // Wait for this request to complete.
        WaitForRequestToComplete();
        // Pop the loader from the queue.
        mSyncLock.Lock();
        {
        	LoadListener* loadfront = mLoaderQueue.GetFront();
        	assert(loadfront == loader);
			mLoaderQueue.PopFront();
        }
        mSyncLock.Unlock();

        // Call back.
        loader->HandleAuthResponse(mUsername, mPassword);
        // The message queued by the response from the last asynchronous
        // request, if present, will start the next request.
        return;
    }

    Boolean processNext = FALSE;

    mSyncLock.Lock();
    {
    	mLoaderQueue.PushBack(loader);
    	processNext = (mLoaderQueue.GetSize() == 1);
    }
    mSyncLock.Unlock();

    if (processNext) {
        ProcessNextLoader();
    }
}

/**
 * Informs the WebView of a new set of credentials.
 * @hide Pending API council review
 */
CARAPI_(void) HttpAuthHandler::OnReceivedCredentials(
	/* [in] */ LoadListener* loader,
	/* [in] */ const String& host, 
	/* [in] */ const String& realm, 
	/* [in] */ const String& username, 
	/* [in] */ const String& password)
{
	assert(loader);

	AutoPtr<IBrowserFrame> browserFrame = loader->GetFrame();
	CBrowserFrame* bf = (CBrowserFrame*)(browserFrame.Get());
	AutoPtr<ICallbackProxy> proxy = bf->GetCallbackProxy();
//	CallbackProxy proxy = loader->GetFrame()->GetCallbackProxy();
	proxy->OnReceivedHttpAuthCredentials(host, realm, username, password);
}

CARAPI_(void) HttpAuthHandler::SignalRequestComplete()
{}

/**
 * Wait for the request in flight, if any, to complete
 */
CARAPI_(void) HttpAuthHandler::WaitForRequestToComplete()
{}

/**
 * Process the next loader in the queue (helper method)
 */
CARAPI_(void) HttpAuthHandler::ProcessNextLoader()
{
    AutoPtr<LoadListener> loader;
    //synchronized (mLoaderQueue) 
    {
        Mutex::Autolock lock(mSyncLock);
        loader = mLoaderQueue.GetFront();
    }
    if (loader != NULL) {
        //synchronized (mRequestInFlightLock)
        {
            Mutex::Autolock lock(mSyncLock);
            assert(mRequestInFlight == FALSE);
            mRequestInFlight = TRUE;
        }

        AutoPtr<IBrowserFrame> frame;

        AutoPtr<ICallbackProxy> proxy = ((CBrowserFrame*)(loader->GetFrame().Get()))->GetCallbackProxy();

        String hostname;
        loader->ProxyAuthenticate() ?
            mNetwork->GetProxyHostname(hostname) : loader->Host(&hostname);

        String realm;
        loader->Realm(&realm);

//        proxy->OnReceivedHttpAuthRequest(this, hostname, realm);
    }
}

ECode HttpAuthHandler::SendMessage(
    /* [in] */ Handle32 pvFunc,
    /* [in] */ IParcel* params)
{
    return mApartment->PostCppCallback((Handle32)this, pvFunc, params, 0);
}

ECode HttpAuthHandler::SendMessageAtTime(
    /* [in] */ Handle32 pvFunc,
    /* [in] */ IParcel* params,
    /* [in] */ Millisecond64 uptimeMillis)
{
    return mApartment->PostCppCallbackAtTime(
        (Handle32)this, pvFunc, params, 0, uptimeMillis);
}

ECode HttpAuthHandler::RemoveMessage(
    /* [in] */ Handle32 func)
{
    return mApartment->RemoveCppCallbacks((Handle32)this, func);
}

ECode HttpAuthHandler::HandleAuthProceed(
    /* [in] */ String& username,
    /* [in] */ String& password)
{
    //String username = msg.getData().getString("username");
    //String password = msg.getData().getString("password");

    AutoPtr<LoadListener> loader;
    //synchronized (mLoaderQueue) 
    {
        Mutex::Autolock lock(mSyncLock);
        //loader = mLoaderQueue.poll();
        loader = mLoaderQueue.GetFront();
        mLoaderQueue.PopFront();
    }

    loader->HandleAuthResponse(username, password);

    return NOERROR;
}

ECode HttpAuthHandler::HandleAuthCancel()
{
    AutoPtr<LoadListener> loader;
    //synchronized (mLoaderQueue) 
    {
        Mutex::Autolock lock(mSyncLock);
        //loader = mLoaderQueue.poll();
        loader = mLoaderQueue.GetFront();
        mLoaderQueue.PopFront();
    }

    loader->HandleAuthResponse(String(NULL), String(NULL));
}