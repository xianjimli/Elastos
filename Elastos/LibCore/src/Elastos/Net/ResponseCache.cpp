
#include "ResponseCache.h"

AutoPtr<IResponseCache> ResponseCache::mDefaultResponseCache;

AutoPtr<IResponseCache> ResponseCache::GetDefault()
{
    // SecurityManager sm = System.getSecurityManager();
    // if (sm != null) {
    //     sm.checkPermission(new NetPermission("getResponseCache"));
    // }
    return mDefaultResponseCache;
}

/**
 * Sets the system's default response cache. Use null to remove the response cache.
 */
void ResponseCache::SetDefault(
    /* [in] */ IResponseCache* responseCache)
{
    // SecurityManager sm = System.getSecurityManager();
    // if (sm != null) {
    //     sm.checkPermission(new NetPermission("setResponseCache"));
    // }
    mDefaultResponseCache = responseCache;
}
