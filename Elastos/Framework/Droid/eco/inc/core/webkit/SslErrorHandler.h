#ifndef __SSLERRORHANDLER_H__
#define __SSLERRORHANDLER_H__

/**
 * SslErrorHandler: class responsible for handling SSL errors. This class is
 * passed as a parameter to BrowserCallback.displaySslErrorDialog and is meant
 * to receive the user's response.
 */
public class SslErrorHandler : public Handler {

    //@Override
	virtual CARAPI_(void) HandleMessage(
		/* [in] */ IMessage* msg);

    /**
     * Creates a new error handler with an empty loader queue.
     */
    /* package */ 
	SslErrorHandler();
    
    /**
     * Saves this handler's state into a map.
     * @return True iff succeeds.
     */
    /* package */ 
	virtual CARAPI_(Boolean) SaveState(
		/* [in] */ IBundle* outState);

    /**
     * Restores this handler's state from a map.
     * @return True iff succeeds.
     */
    /* package */ 
	virtual CARAPI_(Boolean) RestoreState(
		/* [in] */ IBundle* inState);

    /**
     * Clears SSL error preference table.
     */
    /* package */
	virtual CARAPI_(void) Clear();

    /**
     * Handles SSL error(s) on the way up to the user.
     */
    /* package */
	virtual CARAPI_(void) handleSslErrorRequest(
		/* [in] */ ILoadListener* loader);

    /**
     * Check the preference table for a ssl error that has already been shown
     * to the user.
     */
    /* package */
	virtual CARAPI_(Boolean) CheckSslPrefTable(
		/* [in] */ ILoadListener* loader,
		/* [in] */ ISslError* error);

    /**
     * Processes queued SSL-error confirmation requests in
     * a tight loop while there is no need to ask the user.
     */
    /* package */
	virtual CARAPI_(void) FastProcessQueuedSslErrors();

    /**
     * Proceed with the SSL certificate.
     */
	virtual CARAPI_(void) Proceed();

    /**
     * Cancel this request and all pending requests for the WebView that had
     * the error.
     */
	virtual CARAPI_(void) Cancel();

    /**
     * Handles SSL error(s) on the way down from the user.
     */
    /* package */
	virtual CARAPI_(void) HandleSslErrorResponse(
		/* [in] */ ILoadListener* loader,
		/* [in] */ ISslError* error, 
        /* [in] */ Boolean proceed);

private:
	/**
     * Create a new error handler that will be passed to the client.
     */
	SslErrorHandler(
		/* [in] */ ISslErrorHandler* origin, 
		/* [in] */ ILoadListener* listener);

	/**
     * Processes the next loader in the queue.
     * @return True iff should proceed to processing the
     * following loader in the queue
     */
	CARAPI_(Boolean) ProcessNextLoader();

private:
	/* One problem here is that there may potentially be multiple SSL errors
     * coming from mutiple loaders. Therefore, we keep a queue of loaders
     * that have SSL-related problems and process errors one by one in the
     * order they were received.
     */
	static const String LOGTAG;// = "network";

    /**
     * Queue of loaders that experience SSL-related problems.
     */
	LinkedList<LoadListener> mLoaderQueue;

    /**
     * SSL error preference table.
     */
	IBundle* mSslPrefTable;

    // These are only used in the client facing SslErrorHandler.
	const ISslErrorHandler* mOriginHandler;
	const ILoadListener* mLoadListener;

    // Message id for handling the response
	static const Int32 HANDLE_RESPONSE = 100;
};

#endif //__SSLERRORHANDLER_H__