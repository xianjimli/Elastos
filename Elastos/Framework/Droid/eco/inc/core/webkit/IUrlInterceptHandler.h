#ifndef __IURLINTERCEPTHANDLER_H__
#define __IURLINTERCEPTHANDLER_H__

/**
 * @deprecated This interface was inteded to be used by Gears. Since Gears was
 * deprecated, so is this class.
 */
class UrlInterceptHandler {

public:
    /**
     * Given an URL, returns the CacheResult which contains the
     * surrogate response for the request, or null if the handler is
     * not interested.
     *
     * @param url URL string.
     * @param headers The headers associated with the request. May be null.
     * @return The CacheResult containing the surrogate response.
     *
     * @deprecated Do not use, this interface is deprecated.
     */
	virtual CARAPI_(ICacheManagerCacheResult*) Service(
		/* [in] */ CString url, 
		/* [in] */ IObjectStringMap* headers) = 0;

    /**
     * Given an URL, returns the PluginData which contains the
     * surrogate response for the request, or null if the handler is
     * not interested.
     *
     * @param url URL string.
     * @param headers The headers associated with the request. May be null.
     * @return The PluginData containing the surrogate response.
     *
     * @deprecated Do not use, this interface is deprecated.
     */
	virtual CARAPI_(IPluginData*) GetPluginData(
		/* [in] */ CString url, 
		/* [in] */ IObjectStringMap* headers) = 0;
};

#endif //__IURLINTERCEPTHANDLER_H__