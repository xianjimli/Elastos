
#ifndef __COOKIEHANDLER_H__
#define __COOKIEHANDLER_H__

#include "Elastos.Net_server.h"
#include <Com.Kortide.Platform.h>
#include <elastos/AutoPtr.h>

/**
 * This class provides a way to manage cookies with a HTTP protocol handler.
 */
class CookieHandler
{
public:
    /**
     * Returns the system-wide cookie handler or {@code null} if not set.
     *
     * @return the system-wide cookie handler.
     */
    static CARAPI GetDefault(
        /* [out] */ ICookieHandler** handler);

    /**
     * Sets the system-wide cookie handler.
     *
     * @param cHandler
     *            a cookie handler to set as the system-wide default handler.
     */
    static CARAPI_(void) setDefault(
        /* [in] */ ICookieHandler* cHandler);

    virtual CARAPI Get(
        /* [in] */ IURI* uri,
        /* [in] */ IObjectStringMap* requestHeaders,
        /* [out]*/ IObjectStringMap** cookiesMap) = 0;

    /**
     * Sets all cookies of a specific URI in the {@code responseHeaders} into
     * the cookie cache.
     *
     * @param uri
     *            the origin URI of the cookies.
     * @param responseHeaders
     *            a list of request headers.
     * @throws IOException
     *             if an error occurs during the I/O operation.
     */
    virtual CARAPI Put(
        /* [in] */ IURI* uri,
        /* [in] */ IObjectStringMap* requestHeaders) = 0;

private:
    static AutoPtr<ICookieHandler> mSystemWideCookieHandler;

    static AutoPtr<INetPermission> mGetCookieHandlerPermission;// = new NetPermission(
//            "getCookieHandler");

    static AutoPtr<INetPermission> mSetCookieHandlerPermission;// = new NetPermission(
//            "setCookieHandler");

};

#endif //__COOKIEHANDLER_H__
