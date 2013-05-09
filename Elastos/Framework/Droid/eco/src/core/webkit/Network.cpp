
#include "webkit/Network.h"
#include "webkit/HttpAuthHandler.h"

const CString Network::LOGTAG("network");

Core::Threading::Mutex Network::mSyncLock;

/**
 * Static instance of a Network object.
 */
Network* Network::sNetwork;

/**
 * Flag to store the state of platform notifications, for the case
 * when the Network object has not been constructed yet
 */
Boolean Network::sPlatformNotifications;

/**
 * Reference count for platform notifications as the network class is a 
 * static and can exist over multiple activities, thus over multiple 
 * onPause/onResume pairs. 
 */
Int32 Network::sPlatformNotificationEnableRefCount;

/**
 * Creates a new Network object.
 * XXX: Must be created in the same thread as WebCore!!!!!
 */
Network::Network(
	/* [in] */ IContext* context)
{}

/**
 * @return The singleton instance of the network.
 *
 * synchronized
 */
CARAPI_(Network*) Network::GetInstance(
	/* [in] */ IContext* context)
{
	Core::Threading::Mutex::Autolock lock(mSyncLock);

	if (sNetwork == NULL) {
        // Note Context of the Application is used here, rather than
        // the what is passed in (usually a Context derived from an 
        // Activity) so the intent receivers belong to the application
        // rather than an activity - this fixes the issue where 
        // Activities are created and destroyed during the lifetime of
        // an Application
        assert(context);
        IContext* appContext = NULL;
        context->GetApplicationContext(&appContext);
        assert(appContext);
        sNetwork = new Network(appContext);
        if (sPlatformNotifications) {
            // Adjust the ref count before calling enable as it is already
            // taken into account when the static function was called 
            // directly
            --sPlatformNotificationEnableRefCount;
            EnablePlatformNotifications();
        }
    }

    return sNetwork;
}

/**
 * Enables data state and proxy tracking
 */
CARAPI_(void) Network::EnablePlatformNotifications()
{
	if (++sPlatformNotificationEnableRefCount == 1) {
        if (sNetwork != NULL) {
//            sNetwork.mRequestQueue.enablePlatformNotifications();
        } else {
            sPlatformNotifications = TRUE;
        }
    }
}

/**
 * If platform notifications are enabled, this should be called
 * from onPause() or onStop()
 */
CARAPI_(void) Network::DisablePlatformNotifications()
{
	if (--sPlatformNotificationEnableRefCount == 0) {
        if (sNetwork != NULL) {
//            sNetwork.mRequestQueue.disablePlatformNotifications();
        } else {
            sPlatformNotifications = FALSE;
        }
    }
}

/**
 * Request a url from either the network or the file system.
 * @param url The url to load.
 * @param method The http method.
 * @param headers The http headers.
 * @param postData The body of the request.
 * @param loader A LoadListener for receiving the results of the request.
 * @return True if the request was successfully queued.
 */
CARAPI_(Boolean) Network::RequestURL(
	/* [in] */ const String& method,
	/* [in] */ const IObjectStringMap* headers,
	/* [in] */ ArrayOf<Byte>& postData,
	/* [in] */ const LoadListener* loader)
{}

/**
 * @return True iff there is a valid proxy set.
 */
CARAPI_(Boolean) Network::IsValidProxySet()
{
	return FALSE;
}

/**
 * Get the proxy hostname.
 * @return The proxy hostname obtained from the network queue and proxy
 *         settings.
 */
CARAPI_(void) Network::GetProxyHostname(
    /* [out] */ String& str) const
{}

/**
 * @return The proxy username or null if none.
 *
 * synchronized
 */
CARAPI_(const String*) Network::GetProxyUsername() const
{}

/**
 * Sets the proxy username.
 * @param proxyUsername Username to use when
 * connecting through the proxy.
 *
 * synchronized
 */
CARAPI_(void) Network::SetProxyUsername(
	/* [in] */ const String& proxyUsername)
{
//	if (DebugFlags.NETWORK) {
//        Assert.assertTrue(isValidProxySet());
//    }

    mProxyUsername = proxyUsername;
}

/**
 * @return The proxy password or null if none.
 *
 * synchronized
 */
CARAPI_(const String*) Network::GetProxyPassword() const
{
}

/**
 * Sets the proxy password.
 * @param proxyPassword Password to use when
 * connecting through the proxy.
 *
 * synchronized
 */
CARAPI_(void) Network::SetProxyPassword(
	/* [in] */ const String& proxyPassword)
{
//	if (DebugFlags.NETWORK) {
//        Assert.assertTrue(isValidProxySet());
//    }

    mProxyPassword = proxyPassword;
}

/**
 * Saves the state of network handlers (user SSL and HTTP-authentication
 * preferences).
 * @param outState The out-state to save (write) to.
 * @return True iff succeeds.
 */
CARAPI_(Boolean) Network::SaveState(
	/* [in] */ const IBundle* outState)
{
//	if (DebugFlags.NETWORK) {
//        Log.v(LOGTAG, "Network.saveState()");
//    }

	Boolean bRet = FALSE;

//    mSslErrorHandler->SaveState(outState, &bRet);

    return bRet;
}

/**
 * Restores the state of network handlers (user SSL and HTTP-authentication
 * preferences).
 * @param inState The in-state to load (read) from.
 * @return True iff succeeds.
 */
CARAPI_(Boolean) Network::RestoreState(
	/* [in] */ const IBundle* inState)
{
//	if (DebugFlags.NETWORK) {
//        Log.v(LOGTAG, "Network.restoreState()");
//    }

	Boolean bRet = FALSE;

//    mSslErrorHandler->RestoreState(inState, &bRet);

    return bRet;
}

/**
 * Clears user SSL-error preference table.
 */
CARAPI_(void) Network::ClearUserSslPrefTable()
{
//	mSslErrorHandler->Clear();
}

/**
 * Handles SSL error(s) on the way up to the user: the user must decide
 * whether errors should be ignored or not.
 * @param loader The loader that resulted in SSL errors.
 */
CARAPI_(void) Network::HandleSslErrorRequest(
	/* [in] */ const LoadListener* loader)
{
//	if (DebugFlags.NETWORK) Assert.assertNotNull(loader);
    if (loader != NULL) {
//        mSslErrorHandler->HandleSslErrorRequest(loader);
    }
}

/* package */ 
CARAPI_(Boolean) Network::CheckSslPrefTable(
	/* [in] */ const LoadListener* loader,
	/* [in] */ const ISslError* error)
{
	if (loader != NULL && error != NULL) {
		Boolean bRet = false;
//        mSslErrorHandler->CheckSslPrefTable(loader, error, &bRet);
        return bRet;
    }

    return FALSE;
}

/**
 * Handles authentication requests on their way up to the user (the user
 * must provide credentials).
 * @param loader The loader that resulted in an HTTP
 * authentication request.
 */
CARAPI_(void) Network::HandleAuthRequest(
	/* [in] */ LoadListener* loader)
{
//	if (DebugFlags.NETWORK) Assert.assertNotNull(loader);
    if (loader != NULL) {
        mHttpAuthHandler->HandleAuthRequest(loader);
    }
}

// Performance probe
CARAPI_(void) Network::StartTiming()
{}

CARAPI_(void) Network::StopTiming()
{}

