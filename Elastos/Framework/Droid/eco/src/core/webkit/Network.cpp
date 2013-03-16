
#include "webkit/Network.h"

const CString Network::LOGTAG = "network";

/**
 * Creates a new Network object.
 * XXX: Must be created in the same thread as WebCore!!!!!
 */
Network::Network(
	/* [in] */ const IContext* context)
{}

/**
 * @return The singleton instance of the network.
 *
 * synchronized
 */
CARAPI_(Network*) Network::GetInstance(
	/* [in] */ const IContext* context)
{
	return NULL;
}

/**
 * Enables data state and proxy tracking
 */
CARAPI_(void) Network::EnablePlatformNotifications()
{}

/**
 * If platform notifications are enabled, this should be called
 * from onPause() or onStop()
 */
CARAPI_(void) Network::DisablePlatformNotifications()
{}

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
	/* [in] */ Vector<Byte>& postData,
	/* [in] */ const LoadListener* loader)
{}

/**
 * @return True iff there is a valid proxy set.
 */
CARAPI_(Boolean) IsValidProxySet()
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
CARAPI_(void) Network::GetProxyUsername(
    /* [out] */ String& str) const
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
{}

/**
 * @return The proxy password or null if none.
 *
 * synchronized
 */
CARAPI_(void) Network::GetProxyPassword(
    /* [out] */ String& str) const
{}

/**
 * Sets the proxy password.
 * @param proxyPassword Password to use when
 * connecting through the proxy.
 *
 * synchronized
 */
CARAPI_(void) Network::SetProxyPassword(
	/* [in] */ const String& proxyPassword)
{}

/**
 * Saves the state of network handlers (user SSL and HTTP-authentication
 * preferences).
 * @param outState The out-state to save (write) to.
 * @return True iff succeeds.
 */
CARAPI_(Boolean) Network::SaveState(
	/* [in] */ const IBundle* outState)
{
	return FALSE;
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
	return FALSE;
}

/**
 * Clears user SSL-error preference table.
 */
CARAPI_(void) Network::ClearUserSslPrefTable()
{}

/**
 * Handles SSL error(s) on the way up to the user: the user must decide
 * whether errors should be ignored or not.
 * @param loader The loader that resulted in SSL errors.
 */
CARAPI_(void) Network::HandleSslErrorRequest(
	/* [in] */ const LoadListener* loader)
{}

/* package */ 
CARAPI_(Boolean) Network::CheckSslPrefTable(
	/* [in] */ const LoadListener* loader,
	/* [in] */ const ISslError* error)
{
	return FALSE;
}

/**
 * Handles authentication requests on their way up to the user (the user
 * must provide credentials).
 * @param loader The loader that resulted in an HTTP
 * authentication request.
 */
CARAPI_(void) HandleAuthRequest(
	/* [in] */ const LoadListener* loader)
{}

// Performance probe
CARAPI_(void) StartTiming()
{}

CARAPI_(void) StopTiming()
{}

