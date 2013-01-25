#ifndef __NETWORK_H__
#define __NETWORK_H__

class Network {

public:
    /**
     * @return The singleton instance of the network.
     *
     * synchronized
     */
	static CARAPI_(Network) GetInstance(
		/* [in] */ Context context);

    /**
     * Enables data state and proxy tracking
     */
	static CARAPI_(void) EnablePlatformNotifications();

    /**
     * If platform notifications are enabled, this should be called
     * from onPause() or onStop()
     */
	static CARAPI_(void) DisablePlatformNotifications();

    /**
     * Request a url from either the network or the file system.
     * @param url The url to load.
     * @param method The http method.
     * @param headers The http headers.
     * @param postData The body of the request.
     * @param loader A LoadListener for receiving the results of the request.
     * @return True if the request was successfully queued.
     */
	virtual CARAPI_(Boolean) RequestURL(
		/* [in] */ CString method,
		/* [in] */ Map<String, String> headers,
		/* [in] */ ArrayOf<Byte> postData,
		/* [in] */ LoadListener loader);

    /**
     * @return True iff there is a valid proxy set.
     */
	virtual CARAPI_(Boolean) IsValidProxySet();

    /**
     * Get the proxy hostname.
     * @return The proxy hostname obtained from the network queue and proxy
     *         settings.
     */
	virtual CARAPI_(CString) GetProxyHostname();

    /**
     * @return The proxy username or null if none.
     *
     * synchronized
     */
	virtual CARAPI_(CString) GetProxyUsername();

    /**
     * Sets the proxy username.
     * @param proxyUsername Username to use when
     * connecting through the proxy.
     *
     * synchronized
     */
	virtual CARAPI_(void) SetProxyUsername(
		/* [in] */ CString proxyUsername);

    /**
     * @return The proxy password or null if none.
     *
     * synchronized
     */
	virtual CARAPI_(CString) GetProxyPassword();

    /**
     * Sets the proxy password.
     * @param proxyPassword Password to use when
     * connecting through the proxy.
     *
     * synchronized
     */
	virtual CARAPI_(void) SetProxyPassword(
		/* [in] */ CString proxyPassword);

    /**
     * Saves the state of network handlers (user SSL and HTTP-authentication
     * preferences).
     * @param outState The out-state to save (write) to.
     * @return True iff succeeds.
     */
	virtual CARAPI_(Boolean) SaveState(
		/* [in] */ Bundle outState);

    /**
     * Restores the state of network handlers (user SSL and HTTP-authentication
     * preferences).
     * @param inState The in-state to load (read) from.
     * @return True iff succeeds.
     */
	virtual CARAPI_(Boolean) RestoreState(
		/* [in] */ Bundle inState);

    /**
     * Clears user SSL-error preference table.
     */
	virtual CARAPI_(void) ClearUserSslPrefTable();

    /**
     * Handles SSL error(s) on the way up to the user: the user must decide
     * whether errors should be ignored or not.
     * @param loader The loader that resulted in SSL errors.
     */
	virtual CARAPI_(void) HandleSslErrorRequest(
		/* [in] */ LoadListener loader);

    /* package */ 
	virtual CARAPI(Boolean) CheckSslPrefTable(
		/* [in] */ LoadListener loader,
		/* [in] */ SslError error);

    /**
     * Handles authentication requests on their way up to the user (the user
     * must provide credentials).
     * @param loader The loader that resulted in an HTTP
     * authentication request.
     */
	virtual CARAPI_(void) HandleAuthRequest(
		/* [in] */ LoadListener loader);

    // Performance probe
	virtual CARAPI_(void) StartTiming();

	virtual CARAPI_(void) StopTiming();

private:
	    /**
     * Creates a new Network object.
     * XXX: Must be created in the same thread as WebCore!!!!!
     */
	Network(
		/* [in] */ Context context);

private:

	static const CString LOGTAG = "network";

    /**
     * Static instance of a Network object.
     */
	static Network sNetwork;
    
    /**
     * Flag to store the state of platform notifications, for the case
     * when the Network object has not been constructed yet
     */
	static Boolean sPlatformNotifications;
    
    /**
     * Reference count for platform notifications as the network class is a 
     * static and can exist over multiple activities, thus over multiple 
     * onPause/onResume pairs. 
     */
	static Int32 sPlatformNotificationEnableRefCount;

    /**
     * Proxy username if known (used for pre-emptive proxy authentication).
     */
	CString mProxyUsername;

    /**
     * Proxy password if known (used for pre-emptive proxy authentication).
     */
	CString mProxyPassword;

    /**
     * Network request queue (requests are added from the browser thread).
     */
	RequestQueue mRequestQueue;

    /**
     * SSL error handler: takes care of synchronization of multiple async
     * loaders with SSL-related problems.
     */
	SslErrorHandler mSslErrorHandler;

    /**
     * HTTP authentication handler: takes care of synchronization of HTTP
     * authentication requests.
     */
	HttpAuthHandler mHttpAuthHandler;
};

#endif //__NETWORK_H__