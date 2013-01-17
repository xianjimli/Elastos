
#ifndef __CCOOKIEMANAGER_H__
#define __CCOOKIEMANAGER_H__

#include "cmdef.h"
#include "_CCookieManager.h"
#include "CookieHandler.h"
#include <elastos/AutoPtr.h>
#include <elastos/List.h>

CarClass(CCookieManager) , public CookieHandler
{
public:
    /**
     * Sets the cookie policy of this cookie manager.
     *
     * ACCEPT_ORIGINAL_SERVER is the default policy for CookieManager.
     *
     * @param cookiePolicy
     *            the cookie policy. if null, the original policy will not be
     *            changed.
     */
    CARAPI SetCookiePolicy(
        /* [in] */ ICookiePolicy* cookiePolicy);

    /**
     * Gets current cookie store.
     *
     * @return the cookie store currently used by cookie manager.
     */
    CARAPI GetCookieStore(
        /* [out] */ ICookieStore** cookieStore);

    CARAPI Get(
        /* [in] */ IURI* uri,
        /* [in] */ IObjectStringMap* requestHeaders,
        /* [out] */ IObjectStringMap** cookiesMap);

    CARAPI Put(
        /* [in] */ IURI* uri,
        /* [in] */ IObjectStringMap* responseHeaders);

    /**
     * Returns a cookie-safe path by truncating everything after the last "/".
     * When request path like "/foo/bar.html" yields a cookie, that cookie's
     * default path is "/foo/".
     */
    /* package */ static CARAPI_(String) PathToCookiePath(
        /* [in] */ const String& path);

    /**
     * Constructs a new cookie manager.
     *
     * The invocation of this constructor is the same as the invocation of
     * CookieManager(null, null).
     *
     */
    CARAPI constructor();

    /**
     * Constructs a new cookie manager using a specified cookie store and a
     * cookie policy.
     *
     * @param store
     *            a CookieStore to be used by cookie manager. The manager will
     *            use a default one if the arg is null.
     * @param cookiePolicy
     *            a CookiePolicy to be used by cookie manager
     *            ACCEPT_ORIGINAL_SERVER will be used if the arg is null.
     */
    CARAPI constructor(
        /* [in] */ ICookieStore* store,
        /* [in] */ ICookiePolicy* cookiePolicy);

private:
    static CARAPI CookiesToHeaders(
        /* [in] */ List< AutoPtr<IHttpCookie> >& cookies,
        /* [out] */ IObjectStringMap** cookiesMap);

    static CARAPI_(List< AutoPtr<IHttpCookie> >*) ParseCookie(
        /* [in] */ IObjectStringMap* responseHeaders);

private:
    AutoPtr<ICookieStore> mStore;

    AutoPtr<ICookiePolicy> mPolicy;

    static const CString VERSION_ZERO_HEADER;

    static const CString VERSION_ONE_HEADER;
};

#endif //__CCOOKIEMANAGER_H__
