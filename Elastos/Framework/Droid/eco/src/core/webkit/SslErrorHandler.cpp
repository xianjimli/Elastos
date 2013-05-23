#include <Logger.h>
#include <StringBuffer.h>

#include "webkit/SslErrorHandler.h"
#include "webkit/CCallbackProxy.h"
#include "webkit/CBrowserFrame.h"
#include "webkit/DebugFlags.h"
#include "os/CBundle.h"

using namespace Elastos::Utility::Logging;

const CString SslErrorHandler::LOGTAG("network");

// Message id for handling the response
const Int32 SslErrorHandler::HANDLE_RESPONSE;

/**
 * Creates a new error handler with an empty loader queue.
 */
/* package */ 
SslErrorHandler::SslErrorHandler()
{
	mLoaderQueue = new List<AutoPtr<LoadListener> >();
    //mSslPrefTable = new Bundle();
    CBundle::New((IBundle**)&mSslPrefTable);

    // These are used by client facing SslErrorHandlers.
    mOriginHandler = NULL;
    mLoadListener = NULL;
}

/**
 * Saves this handler's state into a map.
 * @return True iff succeeds.
 */
/* package */ 
CARAPI_(Boolean) SslErrorHandler::SaveState(
    /* [in] */ IBundle* outState)
{
	Mutex::Autolock lock(mSyncLock);

	Boolean success = (outState != NULL);
    if (success) {
        // TODO?
        outState->PutBundle(String("ssl-error-handler"), mSslPrefTable);
    }

    return success;
}

/**
 * Restores this handler's state from a map.
 * @return True iff succeeds.
 */
/* package */ 
CARAPI_(Boolean) SslErrorHandler::RestoreState(
    /* [in] */ IBundle* inState)
{
	Mutex::Autolock lock(mSyncLock);

	Boolean success = (inState != NULL);
    if (success) {
        inState->ContainsKey(String("ssl-error-handler"), &success);
        if (success) {
            inState->GetBundle(String("ssl-error-handler"), (IBundle**)&mSslPrefTable);
        }
    }

    return success;
}

/**
 * Clears SSL error preference table.
 */
/* package */
CARAPI_(void) SslErrorHandler::Clear()
{
	Mutex::Autolock lock(mSyncLock);

	mSslPrefTable->Clear();
}

/**
 * Handles SSL error(s) on the way up to the user.
 */
/* package */
CARAPI_(void) SslErrorHandler::HandleSslErrorRequest(
    /* [in] */ LoadListener* loader)
{
	Mutex::Autolock lock(mSyncLock);

	if (loader == NULL) return;

	if (DebugFlags::sSSL_ERROR_HANDLER) {
        String url;
        loader->Url(&url);
        Logger::V(LOGTAG, StringBuffer("SslErrorHandler.handleSslErrorRequest(): ") +
              "url=" + url);
    }

    if (!loader->Cancelled()) {
        mLoaderQueue->PushBack(loader);
        if (loader == mLoaderQueue->GetFront()) {
            FastProcessQueuedSslErrors();
        }
    }
}

/**
 * Check the preference table for a ssl error that has already been shown
 * to the user.
 */
/* package */
CARAPI_(Boolean) SslErrorHandler::CheckSslPrefTable(
    /* [in] */ LoadListener* loader,
    /* [in] */ ISslError* error)
{
	Mutex::Autolock lock(mSyncLock);

	if (loader == NULL || error == NULL) return FALSE;

	String host;
	loader->Host(&host);
    //const Int32 primary = error->GetPrimaryError();
    Int32 primary;
//    error->GetPrimaryError(&primary);
#if 0
    if (DebugFlags.SSL_ERROR_HANDLER) {
        Assert.assertTrue(host != null && primary != 0);
    }
#endif

    Boolean bFlag = FALSE;
    mSslPrefTable->ContainsKey(host, &bFlag);
    if (bFlag) {
    	Int32 temp;
    	mSslPrefTable->GetInt32(host, &temp);
        if (primary <= temp) {
            HandleSslErrorResponse(loader, error, TRUE);
            return TRUE;
        }
    }

    return FALSE;
}

/**
 * Processes queued SSL-error confirmation requests in
 * a tight loop while there is no need to ask the user.
 */
/* package */
CARAPI_(void) SslErrorHandler::FastProcessQueuedSslErrors()
{
	while (ProcessNextLoader());
}

/**
 * Proceed with the SSL certificate.
 */
CARAPI_(void) SslErrorHandler::Proceed()
{}

/**
 * Cancel this request and all pending requests for the WebView that had
 * the error.
 */
CARAPI_(void) SslErrorHandler::Cancel()
{}

/**
 * Handles SSL error(s) on the way down from the user.
 */
/* package */
CARAPI_(void) SslErrorHandler::HandleSslErrorResponse(
    /* [in] */ LoadListener* loader,
    /* [in] */ ISslError* error, 
    /* [in] */ Boolean proceed)
{
	Mutex::Autolock lock(mSyncLock);

	if (loader == NULL || error == NULL) return;

#if 0
	if (DebugFlags.SSL_ERROR_HANDLER) {
        Assert.assertNotNull(loader);
        Assert.assertNotNull(error);
    }
#endif
    if (DebugFlags::sSSL_ERROR_HANDLER) {
        String url;
        loader->Url(&url);
        Logger::V(LOGTAG, StringBuffer("SslErrorHandler.handleSslErrorResponse():")
              + " proceed: %d" + proceed
              + " url:" + url);
    }

    if (!loader->Cancelled()) {
        if (proceed) {
            // update the user's SSL error preference table
            Int32 primary;// = error.getPrimaryError();
            String host;
            loader->Host(&host);
#if 0
            if (DebugFlags.SSL_ERROR_HANDLER) {
                Assert.assertTrue(host != null && primary != 0);
            }
#endif
            Boolean hasKey;
            mSslPrefTable->ContainsKey(host, &hasKey);
            Int32 temp;
            mSslPrefTable->GetInt32(host, &temp);
            if (!hasKey || primary > temp) {
                mSslPrefTable->PutInt32(host, primary);
            }
        }

        loader->HandleSslErrorResponse(proceed);
    }
}

/**
 * Create a new error handler that will be passed to the client.
 */
SslErrorHandler::SslErrorHandler(
    /* [in] */ ISslErrorHandler* origin, 
    /* [in] */ LoadListener* listener)
{
	mOriginHandler = origin;
	mLoadListener = listener;
}

/**
 * Processes the next loader in the queue.
 * @return True iff should proceed to processing the
 * following loader in the queue
 */
CARAPI_(Boolean) SslErrorHandler::ProcessNextLoader()
{
	Mutex::Autolock lock(mSyncLock);

	AutoPtr<LoadListener> loader = mLoaderQueue->GetFront();
    if (loader != NULL) {
        // if this loader has been cancelled
        if (loader->Cancelled()) {
            // go to the following loader in the queue. Make sure this
            // loader has been removed from the queue.
            //mLoaderQueue->remove(loader);
            mLoaderQueue->PopFront();
            return TRUE;
        }

        AutoPtr<ISslError> error = loader->SslError();
#if 0
        if (DebugFlags.SSL_ERROR_HANDLER) {
            Assert.assertNotNull(error);
        }
#endif
        // checkSslPrefTable will handle the ssl error response if the
        // answer is available. It does not remove the loader from the
        // queue.
        if (CheckSslPrefTable(loader, error)) {
            //mLoaderQueue.remove(loader);
            mLoaderQueue->PopFront();
            return TRUE;
        }

        // if we do not have information on record, ask
        // the user (display a dialog)
        AutoPtr<ICallbackProxy> proxy = ((CBrowserFrame*)(loader->GetFrame().Get()))->GetCallbackProxy();
//        proxy->OnReceivedSslError(new SslErrorHandler(this, loader), error);
    }

    // the queue must be empty, stop
    return FALSE;
}