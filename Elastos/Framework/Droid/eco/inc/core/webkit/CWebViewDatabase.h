
#ifndef __CWEBVIEWDATABASE_H__
#define __CWEBVIEWDATABASE_H__

#include "CCacheManager.h"
#include "_CWebViewDatabase.h"
#include "ext/frameworkext.h"
#include <elastos/Vector.h>
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>
#include "CCookieManager.h"
#include "../database/DatabaseUtils.h"

CarClass(CWebViewDatabase)
{
public:
    CARAPI GetInstance(
        /* [in] */ IContext * pContext,
        /* [out] */ IWebViewDatabase ** ppInstance);

    CARAPI HasUsernamePassword(
        /* [out] */ Boolean * pFlag);

    CARAPI ClearUsernamePassword();

    CARAPI HasHttpAuthUsernamePassword(
        /* [out] */ Boolean * pFlag);

    CARAPI ClearHttpAuthUsernamePassword();

    CARAPI HasFormData(
        /* [out] */ Boolean * pFlag);

    CARAPI ClearFormData();

public:

    static CARAPI CreateInstance(
        /* [in] */ IContext * pContext,
        /* [out] */ CWebViewDatabase ** ppInstance);
#if 0
    /**
      * @return: CCacheManager::CacheResult*
      */
    CARAPI_(/*CCacheManager::CacheResult*/Handle32) GetCache(
        /* [in] */ const String& url);

    /**
     * Remove a cache item.
     * 
     * @param url The url
     */
    CARAPI_(void) RemoveCache(
        /* [in] */ const String& url);

    /**
     * Add or update a cache. CACHE_URL_COL is unique in the table.
     *
     * @param url The url
     * @param c The CacheManager.CacheResult
     */
    CARAPI_(void) AddCache(
        /* [in] */ const String& url,
        /* [in] */ Handle32 cacheResult);

    //
    // cache functions
    //

    // only called from WebViewWorkerThread
    CARAPI_(Boolean) StartCacheTransaction();

    // only called from WebViewWorkerThread
    CARAPI_(Boolean) EndCacheTransaction();


    /**
     * Clear cache database
     */
    CARAPI_(void) ClearCache();

    CARAPI_(Boolean) HasCache();

    CARAPI_(Int64) GetCacheTotalSize();

    CARAPI_(Vector<String>&) TrimCache(
        /* [in] */ Int64 amount);

    CARAPI_(Vector<String>&) GetAllCacheFileNames();

private:
    static CWebViewDatabase* mInstance;
#endif

public:
    //
    // cookies functions
    //

    /**
     * Get cookies in the format of CookieManager.Cookie inside an ArrayList for
     * a given domain
     *
     * @return ArrayList<Cookie> If nothing is found, return an empty list.
     */
    CARAPI_(void) GetCookiesForDomain(
        /* [in] */ const String& domain,
        /* [out] */ Vector<AutoPtr<CCookieManager::Cookie> >& list);

    /**
     * Delete cookies which matches (domain, path, name).
     *
     * @param domain If it is null, nothing happens.
     * @param path If it is null, all the cookies match (domain) will be
     *            deleted.
     * @param name If it is null, all the cookies match (domain, path) will be
     *            deleted.
     */
    CARAPI_(void) DeleteCookies(
        /* [in] */ const String& domain,
        /* [in] */ const String& path,
        /* [in] */ const String& name);

    /**
     * Add a cookie to the database
     *
     * @param cookie
     */
    CARAPI_(void) AddCookie(
        /* [in] */ const CCookieManager::Cookie& cookie);

    /**
     * Whether there is any cookies in the database
     *
     * @return TRUE if there is cookie.
     */
    CARAPI_(Boolean) HasCookies();

    /**
     * Clear cookie database
     */
    CARAPI_(void) ClearCookies();

    /**
     * Clear session cookies, which means cookie doesn't have EXPIRES.
     */
    CARAPI_(void) ClearSessionCookies();

    /**
     * Clear expired cookies
     *
     * @param now Time for now
     */
    CARAPI_(void) ClearExpiredCookies(
        /* [in] */ Int64 now);

    //
    // cache functions
    //

    // only called from WebViewWorkerThread
    CARAPI_(Boolean) StartCacheTransaction();

    // only called from WebViewWorkerThread
    CARAPI_(Boolean) EndCacheTransaction();

    /**
     * Get a cache item.
     * 
     * @param url The url
     * @return CacheResult The CacheManager.CacheResult
     */
    CARAPI_(CCacheManager::CacheResult*) GetCache(
        /* [in] */ const String& url);

    /**
     * Remove a cache item.
     * 
     * @param url The url
     */
    CARAPI_(void) RemoveCache(
        /* [in] */ const String& url);

    /**
     * Add or update a cache. CACHE_URL_COL is unique in the table.
     *
     * @param url The url
     * @param c The CacheManager.CacheResult
     */
    CARAPI_(void) AddCache(
        /* [in] */ const String url,
        /* [in] */ ICacheManagerCacheResult* c);

    /**
     * Clear cache database
     */
    CARAPI_(void) ClearCache();

    CARAPI_(Boolean) HasCache();

    CARAPI_(Int64) GetCacheTotalSize();

    CARAPI_(void) TrimCache(
        /* [in] */ Int64 amount,
        /* [out] */ Vector<String>& list);

    CARAPI_(void) GetAllCacheFileNames(
        /* [out] */ Vector<String>& list);

    //
    // password functions
    //

    /**
     * Set password. Tuple (PASSWORD_HOST_COL, PASSWORD_USERNAME_COL) is unique.
     *
     * @param schemePlusHost The scheme and host for the password
     * @param username The username for the password. If it is null, it means
     *            password can't be saved.
     * @param password The password
     */
    CARAPI_(void) SetUsernamePassword(
        /* [in] */ const String& schemePlusHost,
        /* [in] */ const String& username,
        /* [in] */ const String& password);

    /**
     * Retrieve the username and password for a given host
     *
     * @param schemePlusHost The scheme and host which passwords applies to
     * @return String[] if found, String[0] is username, which can be null and
     *         String[1] is password. Return null if it can't find anything.
     */
    CARAPI_(void) GetUsernamePassword(
        /* [in] */ const String& schemePlusHost,
        /* [out] */ Vector<String>& list);

    //
    // http authentication password functions
    //

    /**
     * Set HTTP authentication password. Tuple (HTTPAUTH_HOST_COL,
     * HTTPAUTH_REALM_COL, HTTPAUTH_USERNAME_COL) is unique.
     *
     * @param host The host for the password
     * @param realm The realm for the password
     * @param username The username for the password. If it is null, it means
     *            password can't be saved.
     * @param password The password
     */
    CARAPI_(void) SetHttpAuthUsernamePassword(
        /* [in] */ const String& host,
        /* [in] */ const String realm,
        /* [in] */ const String username,
        /* [in] */ const String password);

    /**
     * Retrieve the HTTP authentication username and password for a given
     * host+realm pair
     *
     * @param host The host the password applies to
     * @param realm The realm the password applies to
     * @return String[] if found, String[0] is username, which can be null and
     *         String[1] is password. Return null if it can't find anything.
     */
    CARAPI_(void) GetHttpAuthUsernamePassword(
        /* [in] */ const String& host,
        /* [in] */ const String& realm,
        /* [out] */ Vector<String>& list);

    //
    // form data functions
    //

    /**
     * Set form data for a site. Tuple (FORMDATA_URLID_COL, FORMDATA_NAME_COL,
     * FORMDATA_VALUE_COL) is unique
     *
     * @param url The url of the site
     * @param formdata The form data in HashMap
     */
    CARAPI_(void) SetFormData(
        /* [in] */ const String& url,
        /* [in] */ IObjectStringMap* formdata);

    /**
     * Get all the values for a form entry with "name" in a given site
     *
     * @param url The url of the site
     * @param name The name of the form entry
     * @return A list of values. Return empty list if nothing is found.
     */
    CARAPI_(void) GetFormData(
        /* [in] */ const String& url,
        /* [in] */ const String& name,
        /* [out] */ Vector<String>& list);

protected:
    // log tag
    static const char* LOGTAG;// = "webviewdatabase";

private:

    static const char* DATABASE_FILE;// = "webview.db";
    static const char* CACHE_DATABASE_FILE;// = "webviewCache.db";

    static const Int32 DATABASE_VERSION = 10;
    // 2 -> 3 Modified Cache table to allow cache of redirects
    // 3 -> 4 Added Oma-Downloads table
    // 4 -> 5 Modified Cache table to support persistent contentLength
    // 5 -> 4 Removed Oma-Downoads table
    // 5 -> 6 Add INDEX for cache table
    // 6 -> 7 Change cache localPath from int to String
    // 7 -> 8 Move cache to its own db
    // 8 -> 9 Store both scheme and host when storing passwords
    // 9 -> 10 Update httpauth table UNIQUE
    static const Int32 CACHE_DATABASE_VERSION = 4;

    static const char* mTableNames[];// = {
    //    "cookies", "password", "formurl", "formdata", "httpauth"
    //};

    // Table ids (they are index to mTableNames)
    static const Int32 TABLE_COOKIES_ID = 0;

    static const Int32 TABLE_PASSWORD_ID = 1;

    static const Int32 TABLE_FORMURL_ID = 2;

    static const Int32 TABLE_FORMDATA_ID = 3;

    static const Int32 TABLE_HTTPAUTH_ID = 4;

    // column id strings for "_id" which can be used by any table
    static const char* ID_COL;// = "_id";

    static const char* ID_PROJECTION[];

    // column id strings for "cookies" table
    static const char* COOKIES_NAME_COL;// = "name";

    static const char* COOKIES_VALUE_COL;// = "value";

    static const char* COOKIES_DOMAIN_COL;// = "domain";

    static const char* COOKIES_PATH_COL;// = "path";

    static const char* COOKIES_EXPIRES_COL;// = "expires";

    static const char* COOKIES_SECURE_COL;// = "secure";

    // column id strings for "cache" table
    static const char* CACHE_URL_COL;// = "url";

    static const char* CACHE_FILE_PATH_COL;// = "filepath";

    static const char* CACHE_LAST_MODIFY_COL;// = "lastmodify";

    static const char* CACHE_ETAG_COL;// = "etag";

    static const char* CACHE_EXPIRES_COL;// = "expires";

    static const char* CACHE_EXPIRES_STRING_COL;// = "expiresstring";

    static const char* CACHE_MIMETYPE_COL;// = "mimetype";

    static const char* CACHE_ENCODING_COL;// = "encoding";

    static const char* CACHE_HTTP_STATUS_COL;// = "httpstatus";

    static const char* CACHE_LOCATION_COL;// = "location";

    static const char* CACHE_CONTENTLENGTH_COL;// = "contentlength";

    static const char* CACHE_CONTENTDISPOSITION_COL;// = "contentdisposition";

    static const char* CACHE_CROSSDOMAIN_COL;// = "crossdomain";

    // column id strings for "password" table
    static const char* PASSWORD_HOST_COL;// = "host";

    static const char* PASSWORD_USERNAME_COL;// = "username";

    static const char* PASSWORD_PASSWORD_COL;// = "password";

    // column id strings for "formurl" table
    static const char* FORMURL_URL_COL;// = "url";

    // column id strings for "formdata" table
    static const char* FORMDATA_URLID_COL;// = "urlid";

    static const char* FORMDATA_NAME_COL;// = "name";

    static const char* FORMDATA_VALUE_COL;// = "value";

    // column id strings for "httpauth" table
    static const char* HTTPAUTH_HOST_COL;// = "host";

    static const char* HTTPAUTH_REALM_COL;// = "realm";

    static const char* HTTPAUTH_USERNAME_COL;// = "username";

    static const char* HTTPAUTH_PASSWORD_COL;// = "password";

private:
    WebViewDatabase();

    static CARAPI_(void) UpgradeDatabase();

    static CARAPI_(void) UpgradeCacheDatabase();

    static CARAPI_(void) BootstrapCacheDatabase();

    CARAPI_(Boolean) HasEntries(
        /* [in] */ Int32 tableId);

private:

    // 1 -> 2 Add expires String
    // 2 -> 3 Add content-disposition
    // 3 -> 4 Add crossdomain (For x-permitted-cross-domain-policies header)
    
    static CWebViewDatabase* mInstance;
    static ISQLiteDatabase* mDatabase;
    static ISQLiteDatabase* mCacheDatabase;

    // synchronize locks
    Mutex mCookieLock;
    Mutex mPasswordLock;
    Mutex mFormLock;
    Mutex mHttpAuthLock;

    // use InsertHelper to improve insert performance by 40%
    static DatabaseUtils::InsertHelper* mCacheInserter;
    static Int32 mCacheUrlColIndex;
    static Int32 mCacheFilePathColIndex;
    static Int32 mCacheLastModifyColIndex;
    static Int32 mCacheETagColIndex;
    static Int32 mCacheExpiresColIndex;
    static Int32 mCacheExpiresStringColIndex;
    static Int32 mCacheMimeTypeColIndex;
    static Int32 mCacheEncodingColIndex;
    static Int32 mCacheHttpStatusColIndex;
    static Int32 mCacheLocationColIndex;
    static Int32 mCacheContentLengthColIndex;
    static Int32 mCacheContentDispositionColIndex;
    static Int32 mCacheCrossDomainColIndex;

    static Int32 mCacheTransactionRefcount;
};

#endif // __CWEBVIEWDATABASE_H__
