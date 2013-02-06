#ifndef __HTTPAUTHHANDLER_H__
#define __HTTPAUTHHANDLER_H__

/**
 * HTTP authentication handler: local handler that takes care
 * of HTTP authentication requests. This class is passed as a
 * parameter to BrowserCallback.displayHttpAuthDialog and is
 * meant to receive the user's response.
 */
public class HttpAuthHandler : public Handler {

public:
	/**
     * Creates a new HTTP authentication handler with an empty
     * loader queue
     *
     * @param network The parent network object
     */
    /* package */
	HttpAuthHandler(
		/* [in] */ INetworkInfo* network);

public:
    // Use to synchronize when making synchronous calls to
    // onReceivedHttpAuthRequest(). We can't use a single Boolean object for
    // both the lock and the state, because Boolean is immutable.
    IInterface* mRequestInFlightLock;
    Boolean mRequestInFlight;
    String mUsername;
    String mPassword;

public:
    //@Override
	virtual CARAPI_(void) HandleMessage(
		/* [in] */ IMessage* msg);

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
	virtual CARAPI_(Boolean) HandleResponseForSynchronousRequest(
		/* [in] */ String username, 
		/* [in] */ String password);



    /**
     * Proceed with the authorization with the given credentials
     *
     * May be called on the UI thread, rather than the WebCore thread.
     *
     * @param username The username to use for authentication
     * @param password The password to use for authentication
     */
	virtual CARAPI_(void) Proceed(
		/* [in] */ CString username, 
		/* [in] */ CString password);

    /**
     * Cancel the authorization request
     *
     * May be called on the UI thread, rather than the WebCore thread.
     *
     */
	virtual CARAPI_(void) Cancel();

    /**
     * @return True if we can use user credentials on record
     * (ie, if we did not fail trying to use them last time)
     */
	virtual CARAPI_(Boolean) UseHttpAuthUsernamePassword();

    /**
     * Enqueues the loader, if the loader is the only element
     * in the queue, starts processing the loader
     *
     * @param loader The loader that resulted in this http
     * authentication request
     */
    /* package */
	virtual CARAPI_(void) HandleAuthRequest(
		/* [in] */ ILoadListener* loader);

    /**
     * Informs the WebView of a new set of credentials.
     * @hide Pending API council review
     */
	static CARAPI_(void) OnReceivedCredentials(
		/* [in] */ ILoadListener* loader,
		/* [in] */ CString host, 
		/* [in] */ CString realm, 
		/* [in] */ CString username, 
		/* [in] */ CString password);

private:

	CARAPI_(void) SignalRequestComplete();

	/**
     * Wait for the request in flight, if any, to complete
     */
	CARAPI_(void) WaitForRequestToComplete();

    /**
     * Process the next loader in the queue (helper method)
     */
	CARAPI_(void) ProcessNextLoader();

private:
	/* It is important that the handler is in Network, because
     * we want to share it accross multiple loaders and windows
     * (like our subwindow and the main window).
     */

	static const String LOGTAG;// = "network";

    /**
     * Network.
     */
	Network mNetwork;

    /**
     * Loader queue.
     */
//	LinkedList<LoadListener> mLoaderQueue;


    // Message id for handling the user response
	static const int AUTH_PROCEED = 100;
	static const int AUTH_CANCEL = 200;
};

#endif //__HTTPAUTHHANDLER_H__