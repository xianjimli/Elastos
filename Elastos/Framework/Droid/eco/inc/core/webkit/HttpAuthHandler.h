#ifndef __HTTPAUTHHANDLER_H__
#define __HTTPAUTHHANDLER_H__

#include "Network.h"
#include "ext/frameworkext.h"
#include <elastos/Mutex.h>
#include <elastos/List.h>
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>

class LoadListener;

/**
 * HTTP authentication handler: local handler that takes care
 * of HTTP authentication requests. This class is passed as a
 * parameter to BrowserCallback.displayHttpAuthDialog and is
 * meant to receive the user's response.
 */
class HttpAuthHandler : public ElRefBase,
                        public IApartment
{

public:
	/**
     * Creates a new HTTP authentication handler with an empty
     * loader queue
     *
     * @param network The parent network object
     */
    /* package */
	HttpAuthHandler(
		/* [in] */ Network* network);

public:

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

public:
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

    //@Override
	//virtual CARAPI_(void) HandleMessage(
	//	/* [in] */ IMessage* msg);

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
		/* [in] */ const String& username, 
		/* [in] */ const String& password);

    /**
     * Proceed with the authorization with the given credentials
     *
     * May be called on the UI thread, rather than the WebCore thread.
     *
     * @param username The username to use for authentication
     * @param password The password to use for authentication
     */
	virtual CARAPI_(void) Proceed(
		/* [in] */ const String& username, 
		/* [in] */ const String& password);

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
		/* [in] */ LoadListener* loader);

    /**
     * Informs the WebView of a new set of credentials.
     * @hide Pending API council review
     */
	static CARAPI_(void) OnReceivedCredentials(
		/* [in] */ LoadListener* loader,
		/* [in] */ const String& host, 
		/* [in] */ const String& realm, 
		/* [in] */ const String& username, 
		/* [in] */ const String& password);

public:
    // Use to synchronize when making synchronous calls to
    // onReceivedHttpAuthRequest(). We can't use a single Boolean object for
    // both the lock and the state, because Boolean is immutable.
    IInterface* mRequestInFlightLock;
    Boolean mRequestInFlight;
    String mUsername;
    String mPassword;

private:

    CARAPI SendMessage(
        /* [in] */ Handle32 pvFunc,
        /* [in] */ IParcel* params);

    CARAPI SendMessageAtTime(
        /* [in] */ Handle32 pvFunc,
        /* [in] */ IParcel* params,
        /* [in] */ Millisecond64 uptimeMillis);

    CARAPI RemoveMessage(
        /* [in] */ Handle32 func);

    CARAPI HandleAuthProceed(
        /* [in] */ String& username,
        /* [in] */ String& password);

    CARAPI HandleAuthCancel();

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

	static const CString LOGTAG;// = "network";

    /**
     * Network.
     */
	Network* mNetwork;

    /**
     * Loader queue.
     */
//	LinkedList<LoadListener> mLoaderQueue;
    List<AutoPtr<LoadListener> > mLoaderQueue;
    Core::Threading::Mutex       mSyncLock;

    // Message id for handling the user response
	static const int AUTH_PROCEED = 100;
	static const int AUTH_CANCEL = 200;

    AutoPtr<IApartment> mApartment;
};

#endif //__HTTPAUTHHANDLER_H__