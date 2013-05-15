
#ifndef __CHTTPAUTHHANDLER_H__
#define __CHTTPAUTHHANDLER_H__

#include "_CHttpAuthHandler.h"
#include "ext/frameworkext.h"
#include "webkit/Network.h"
#include "webkit/LoadListener.h"
#include <elastos/Mutex.h>
#include <elastos/List.h>
#include <elastos/AutoPtr.h>

CarClass(CHttpAuthHandler)
{
public:
    CARAPI Proceed(
        /* [in] */ const String& username,
        /* [in] */ const String& password);

    CARAPI Cancel();

    CARAPI UseHttpAuthUsernamePassword(
        /* [out] */ Boolean * pFlag);

    /**
     * Informs the WebView of a new set of credentials.
     * @hide Pending API council review
     */
    //public 
    static void OnReceivedCredentials(
        /* [in] */ LoadListener* loader,
        /* [in] */ String host, 
        /* [in] */ String realm, 
        /* [in] */ String username, 
        /* [in] */ String password);

public:
    //IApartment
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
    //public 
    //void HandleMessage(IMessage msg);
    void HandleAuthProceed(
        /* [in] */ String username,
        /* [in] */ String password);

    void HandleAuthCancel();

public:
    /**
     * Creates a new HTTP authentication handler with an empty
     * loader queue
     *
     * @param network The parent network object
     */
    /* package */ 
    construct(
        /* [in] */ Network * network);

    /**
     * @return True if we can use user credentials on record
     * (ie, if we did not fail trying to use them last time)
     */
    //public 
    Boolean UseHttpAuthUsernamePassword();

    /**
     * Enqueues the loader, if the loader is the only element
     * in the queue, starts processing the loader
     *
     * @param loader The loader that resulted in this http
     * authentication request
     */
    /* package */ 
    void HandleAuthRequest(
        /* [in] */ LoadListener *loader);

private:
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
    //private 
    Boolean HandleResponseForSynchronousRequest(
        /* [in] */ String username, 
        /* [in] */ String password);

    //private
    void SignalRequestComplete();

    /**
     * Wait for the request in flight, if any, to complete
     */
    //private 
    void WaitForRequestToComplete();

    /**
     * Process the next loader in the queue (helper method)
     */
    //private 
    void ProcessNextLoader();

public:
    // Use to synchronize when making synchronous calls to
    // onReceivedHttpAuthRequest(). We can't use a single Boolean object for
    // both the lock and the state, because Boolean is immutable.
    Core::Threading::Mutex mRequestInFlightLock;    //JAVA:  Object mRequestInFlightLock = new Object();
    Boolean mRequestInFlight;
    String mUsername;
    String mPassword;

private:
    /* It is important that the handler is in Network, because
     * we want to share it accross multiple loaders and windows
     * (like our subwindow and the main window).
     */
    static const CString LOGTAG;// = "network";

    /**
     * Network.
     */
    AutoPtr<Network> mNetwork;

    /**
     * Loader queue.
     */
    List<AutoPtr<LoadListener> > mLoaderQueue;    //JAVA:  LinkedList<LoadListener> mLoaderQueue;
    Core::Threading::Mutex        mLoaderQueueMutex;

    // Message id for handling the user response
    static const Int32 AUTH_PROCEED;// = 100;
    static const Int32 AUTH_CANCEL;// = 200;

private:
    AutoPtr<IApartment> mApartment;
};

#endif // __CHTTPAUTHHANDLER_H__
