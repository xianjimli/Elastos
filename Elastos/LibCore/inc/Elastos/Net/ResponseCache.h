
#ifndef __RESPONSECACHE_H__
#define __RESPONSECACHE_H__

#include "cmdef.h"
#include "Elastos.Net_server.h"
#include <elastos/AutoPtr.h>

/**
 * Caches {@code URLConnection} responses.
 * <p>The system's default cache can be set using {@link #setDefault}.
 * If {@link URLConnection#getUseCaches} returns true, {@code URLConnection} will use the
 * default response cache, if one has been set.
 * <p>Although {@code URLConnection} will always call {@link #put}, the specific
 * {@code ResponseCache} implementation gets to decide what will actually be cached,
 * and for how long.
 */
class ResponseCache
{
public:
    /**
     * Returns the system's default response cache, or null.
     */
    static CARAPI_(AutoPtr<IResponseCache>) GetDefault();

    /**
     * Sets the system's default response cache. Use null to remove the response cache.
     */
    CARAPI_(void) SetDefault(
        /* [in] */ IResponseCache* responseCache);

    virtual CARAPI Get(
        /* [in] */ IURI* uri,
        /* [in] */ String requestMethod,
        /* [in] */ IObjectStringMap* requestHeaders,
        /* [out] */ ICacheResponse** cache) = 0;

    virtual CARAPI Put(
        /* [in] */ IURI* uri,
        /* [in] */ IURLConnection* conn,
        /* [out] */ ICacheRequest** cache) = 0;

private:
    static AutoPtr<IResponseCache> mDefaultResponseCache;
};

#endif //__RESPONSECACHE_H__
