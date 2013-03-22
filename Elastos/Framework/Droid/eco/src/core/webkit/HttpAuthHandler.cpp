
#include "webkit/HttpAuthHandler.h"
#include "webkit/LoadListener.h"
#include "webkit/CBrowserFrame.h"

const CString HttpAuthHandler::LOGTAG = "network";

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

//@Override
CARAPI_(void) HttpAuthHandler::HandleMessage(
	/* [in] */ IMessage* msg)
{
	assert(msg != NULL);

	AutoPtr<LoadListener> loader = NULL;

	{
        Core::Threading::Mutex::Autolock lock(mLoaderQueueMutex);
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
#if 0
    Message msg = obtainMessage(AUTH_PROCEED);
    msg.getData().putString("username", username);
    msg.getData().putString("password", password);
    SendMessage(msg);
    SignalRequestComplete();
#endif
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
}

/**
 * @return True if we can use user credentials on record
 * (ie, if we did not fail trying to use them last time)
 */
CARAPI_(Boolean) HttpAuthHandler::UseHttpAuthUsernamePassword()
{
	AutoPtr<LoadListener> loader;

    mLoaderQueueMutex.Lock();
	loader = mLoaderQueue.GetFront();
	mLoaderQueueMutex.Unlock();

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
{}