
#ifndef __CCOOKIEMANAGER_H__
#define __CCOOKIEMANAGER_H__

#include "_CCookieManager.h"
#include "ext/frameworkext.h"
#include <elastos/Vector.h>
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>

CarClass(CCookieManager)
{
public:
    CARAPI GetInstance(
        /* [out] */ ICookieManager** instance);

    CARAPI SetAcceptCookie(
        /* [in] */ Boolean accept);

    CARAPI AcceptCookie(
        /* [out] */ Boolean* acceptCookie);

    CARAPI SetCookie(
        /* [in] */ CString url,
        /* [in] */ CString value);

    CARAPI SetCookieEx(
        /* [in] */ IWebAddress* uri,
        /* [in] */ CString value);

    CARAPI GetCookie(
        /* [in] */ CString url,
        /* [out] */ String* cookie);

    CARAPI GetCookieEx(
        /* [in] */ IWebAddress* uri,
        /* [out] */ String* cookie);

    CARAPI RemoveSessionCookie();

    CARAPI RemoveAllCookie();

    CARAPI HasCookies(
        /* [out] */ Boolean* flag);

    CARAPI RemoveExpiredCookie();

public:
    //     static {
//        Arrays.sort(BAD_COUNTRY_2LDS);
//    }

    /**
     * Package level class to be accessed by cookie sync manager
     */
    class Cookie : public ElRefBase
    {
    public:
        static const Byte MODE_NEW = 0;

        static const Byte MODE_NORMAL = 1;

        static const Byte MODE_DELETED = 2;

        static const Byte MODE_REPLACED = 3;

    public:
        Cookie();

        Cookie(
            /* [in] */ String& defaultDomain,
            /* [in] */ String& defaultPath);

        CARAPI_(Boolean) ExactMatch(
            /* [in] */ Cookie& in);

        CARAPI_(Boolean) DomainMatch(
            /* [in] */ String& urlHost);

        CARAPI_(Boolean) PathMatch(
            /* [in] */ String& urlPath);

        CARAPI_(void) ToString(String& list);

    public:

        String domain;

        String path;

        String name;

        String value;

        Int64 expires;

        Int64 lastAcessTime;

        Int64 lastUpdateTime;

        Boolean secure;

        Byte mode;        
    };

public:
    /**
     * Package level api, called from CookieSyncManager
     *
     * Get a list of cookies which are updated since a given time.
     * @param last The given time in millisec
     * @return A list of cookies
     */
    /*synchronized*/
    CARAPI_(void) GetUpdatedCookiesSince(
        /* [in] */ Int64 last,
        /* [out] */ Vector<AutoPtr<Cookie> >& list);

    /**
     * Package level api, called from CookieSyncManager
     *
     * Delete a Cookie in the RAM
     * @param cookie Cookie to be deleted
     */
    /*synchronized*/
    CARAPI_(void) DeleteACookie(
        /* [in] */ Cookie& cookie);

    /**
     * Package level api, called from CookieSyncManager
     *
     * Called after a cookie is synced to FLASH
     * @param cookie Cookie to be synced
     */
    /*synchronized*/
    CARAPI_(void) SyncedACookie(
        /* [in] */ Cookie& cookie);

    /**
     * Package level api, called from CookieSyncManager
     *
     * Delete the least recent used domains if the total cookie count in RAM
     * exceeds the limit
     * @return A list of cookies which are removed from RAM
     */
    /*synchronized*/
    CARAPI_(void) DeleteLRUDomain(
        /* [out] */ Vector<AutoPtr<Cookie> >& list);

protected:
    CARAPI_(IInterface*) Clone();

private:

    static ICookieManager* sRef;

    static const CString LOGTAG;// = "webkit";

    static const CString DOMAIN;// = "domain";

    static const CString PATH;// = "path";

    static const CString EXPIRES;// = "expires";

    static const CString SECURE;// = "secure";

    static const CString MAX_AGE;// = "max-age";

    static const CString HTTP_ONLY;// = "httponly";

    static const CString HTTPS;// = "https";

    static const Byte PERIOD;// = '.';

    static const Byte COMMA;// = ',';

    static const Byte SEMICOLON;// = ';';

    static const Byte EQUAL;// = '=';

    static const Byte PATH_DELIM;// = '/';

    static const Byte QUESTION_MARK;// = '?';

    static const Byte WHITE_SPACE;// = ' ';

    static const Byte QUOTATION;// = '\"';

    static const Int32 SECURE_LENGTH;// = SECURE.length();

    static const Int32 HTTP_ONLY_LENGTH;// = HTTP_ONLY.length();

    // RFC2109 defines 4k as maximum size of a cookie
    static const Int32 MAX_COOKIE_LENGTH = 4 * 1024;

    // RFC2109 defines 20 as max cookie count per domain. As we track with base
    // domain, we allow 50 per base domain
    static const Int32 MAX_COOKIE_COUNT_PER_BASE_DOMAIN = 50;

    // RFC2109 defines 300 as max count of domains. As we track with base
    // domain, we set 200 as max base domain count
    static const Int32 MAX_DOMAIN_COUNT = 200;

    // max cookie count to limit RAM cookie takes less than 100k, it is based on
    // average cookie entry size is less than 100 bytes
    static const Int32 MAX_RAM_COOKIES_COUNT = 1000;

    //  max domain count to limit RAM cookie takes less than 100k,
    static const Int32 MAX_RAM_DOMAIN_COUNT = 15;

    AutoPtr<IObjectStringMap> mCookieMap;// = new LinkedHashMap
            //<String, ArrayList<Cookie>>(MAX_DOMAIN_COUNT, 0.75f, true);

    Boolean mAcceptCookie;// = true;

    /**
     * This contains a list of 2nd-level domains that aren't allowed to have
     * wildcards when combined with country-codes. For example: [.co.uk].
     */
    const static CString BAD_COUNTRY_2LDS[];/* =
          { "ac", "co", "com", "ed", "edu", "go", "gouv", "gov", "info",
            "lg", "ne", "net", "or", "org" };*/

//    static const CookieComparator* COMPARATOR;// = new CookieComparator();

    class CookieComparator// : public Comparator<Cookie> 
    {
    public:
         CARAPI_(Int32) Compare(
            /* [in] */ Cookie& cookie1,
            /* [in] */ Cookie& cookie2);
    };

    CookieManager();

    /**
     * Extract the host and path out of a uri
     * @param uri The given WebAddress
     * @return The host and path in the format of String[], String[0] is host
     *          which has at least two periods, String[1] is path which always
     *          ended with "/"
     */
    CARAPI_(void) GetHostAndPath(
        /* [in] */ IWebAddress* uri,
        /* [out] */ Vector<String>& list) const;

    /**
     * Get the base domain for a give host. E.g. mail.google.com will return
     * google.com
     * @param host The give host
     * @return the base domain
     */
    CARAPI_(void) GetBaseDomain(
        /* [in] */ String& host,
        /* [out] */ String& str) const;

    /**
     * parseCookie() parses the cookieString which is a comma-separated list of
     * one or more cookies in the format of "NAME=VALUE; expires=DATE;
     * path=PATH; domain=DOMAIN_NAME; secure httponly" to a list of Cookies.
     * Here is a sample: IGDND=1, IGPC=ET=UB8TSNwtDmQ:AF=0; expires=Sun,
     * 17-Jan-2038 19:14:07 GMT; path=/ig; domain=.google.com, =,
     * PREF=ID=408909b1b304593d:TM=1156459854:LM=1156459854:S=V-vCAU6Sh-gobCfO;
     * expires=Sun, 17-Jan-2038 19:14:07 GMT; path=/; domain=.google.com which
     * contains 3 cookies IGDND, IGPC, PREF and an empty cookie
     * @param host The default host
     * @param path The default path
     * @param cookieString The string coming from "Set-Cookie:"
     * @return A list of Cookies
     */
    CARAPI_(void) ParseCookie(
        /* [in] */ String& host,
        /* [in] */ String& path,
        /* [in] */ String& cookieString,
        /* [out] */ Vector<AutoPtr<Cookie> >& list);
};

#endif // __CCOOKIEMANAGER_H__
