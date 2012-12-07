
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
	CARAPI SetCookiePolicy(
    	/* [in] */ ICookiePolicy* cookiePolicy);

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
    static CARAPI_(String) PathToCookiePath(
        /* [in] */ const String& path);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ ICookieStore* store,
        /* [in] */ ICookiePolicy* cookiePolicy);

private:
	static CARAPI CookiesToHeaders(
		/* [in] */ List<AutoPtr<IHttpCookie> > cookies,
		/* [out] */ IObjectStringMap** cookiesMap);

	static CARAPI_(List<AutoPtr<IHttpCookie> >) ParseCookie(
		/* [in] */ IObjectStringMap* responseHeaders);

private:
    AutoPtr<ICookieStore> mStore;

    AutoPtr<ICookiePolicy> mPolicy;

    static const CString VERSION_ZERO_HEADER;

    static const CString VERSION_ONE_HEADER;
};

#endif //__CCOOKIEMANAGER_H__
