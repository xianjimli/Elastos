
#ifndef __CSSLERRORHANDLER_H__
#define __CSSLERRORHANDLER_H__

#include "_CSslErrorHandler.h"

#include "ext/frameworkext.h"
#include <elastos/List.h>
#include <elastos.h>
#include <elastos/AutoPtr.h>

#include "LoadListener.h"

/**
 * SslErrorHandler: class responsible for handling SSL errors. This class is
 * passed as a parameter to BrowserCallback.displaySslErrorDialog and is meant
 * to receive the user's response.
 */
CarClass(CSslErrorHandler)
{
    /* One problem here is that there may potentially be multiple SSL errors
    * coming from mutiple loaders. Therefore, we keep a queue of loaders
    * that have SSL-related problems and process errors one by one in the
    * order they were received.
    */
public:
    CARAPI HandleMessage(
        /* [in] */ IMessage * msg);

    CARAPI Proceed();

    CARAPI Cancel();

    /**
    * Creates a new error handler with an empty loader queue.
    */
    /* package */
    CARAPI constructor();

    /**
    * Saves this handler's state into a map.
    * @return True iff succeeds.
    */
    /* package */ 
    //synchronized
    CARAPI SaveState(
        /* [in] */ IBundle * outState,
        /* [out] */ Boolean * ret);

    /**
    * Restores this handler's state from a map.
    * @return True iff succeeds.
    */
    /* package */ 
    //synchronized 
    CARAPI RestoreState(
        /* [in] */ IBundle * inState,
        /* [out] */ Boolean * ret);

    /**
    * Clears SSL error preference table.
    */
    /* package */ 
    //synchronized 
    CARAPI Clear();

    /**
    * Handles SSL error(s) on the way up to the user.
    */
    /* package */
    // synchronized 
    CARAPI HandleSslErrorRequest(
        /* [in] */ LoadListener * loader);

    /**
     * Check the preference table for a ssl error that has already been shown
     * to the user.
     */
    /* package */ 
    //synchronized 
    CARAPI CheckSslPrefTable(
         /* [in] */ LoadListener * loader,
        /* [in] */ ISslError * error,
        /* [out] */ Boolean * ret);

    /**
     * Processes queued SSL-error confirmation requests in
     * a tight loop while there is no need to ask the user.
     */
    /* package */
    CARAPI FastProcessQueuedSslErrors();

    /**
     * Handles SSL error(s) on the way down from the user.
     */
    /* package */ 
    //synchronized 
    CARAPI HandleSslErrorResponse(
        /* [in] */ LoadListener * loader,
        /* [in] */ ISslError * error, 
        /* [in] */ Boolean proceed);

    //Own added to build new objece of class CSslErrorHandler
    CARAPI InitPara(
        /* [in] */ ISslErrorHandler * origin, 
        /* [in] */ LoadListener * listener);

private:
    /**
    * Create a new error handler that will be passed to the client.
    */
    CARAPI constructor(
        /* [in] */ ISslErrorHandler * origin, 
        /* [in] */ LoadListener * listener);  

    /**
     * Processes the next loader in the queue.
     * @return True iff should proceed to processing the
     * following loader in the queue
     */
    //synchronized 
    CARAPI ProcessNextLoader(
        /* [out] */ Boolean * ret);




private:
    static const CString LOGTAG;// = "network";

    /**
     * Queue of loaders that experience SSL-related problems.
     */
    List< AutoPtr<LoadListener> > * mLoaderQueue;

    /**
     * SSL error preference table.
     */
    AutoPtr<IBundle> mSslPrefTable;

    // These are only used in the client facing SslErrorHandler.
    /*const*/ AutoPtr<ISslErrorHandler> mOriginHandler;
    /*const*/ AutoPtr<LoadListener> mLoadListener;

    // Message id for handling the response
    static const Int32 HANDLE_RESPONSE;// = 100;

};

#endif // __CSSLERRORHANDLER_H__
