
#include "cmdef.h"
#include "ResponseCache.h"


AutoPtr<IResponseCache> ResponseCache::sDefaultResponseCache;

ECode ResponseCache::GetDefault(
    /* [out] */ IResponseCache** responseCache)
{
    VALIDATE_NOT_NULL(responseCache);
    // SecurityManager sm = System.getSecurityManager();
    // if (sm != null) {
    //     sm.checkPermission(new NetPermission("getResponseCache"));
    // }
    *responseCache = sDefaultResponseCache;
    if (*responseCache != NULL) (*responseCache)->AddRef();
    return NOERROR;
}

/**
 * Sets the system's default response cache. Use null to remove the response cache.
 */
ECode ResponseCache::SetDefault(
    /* [in] */ IResponseCache* responseCache)
{
    // SecurityManager sm = System.getSecurityManager();
    // if (sm != null) {
    //     sm.checkPermission(new NetPermission("setResponseCache"));
    // }
    sDefaultResponseCache = responseCache;
    return NOERROR;
}
