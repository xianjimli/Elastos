
#include "webkit/CWebViewDatabase.h"
#include "content/CContentValues.h"
#include <elastos/Thread.h>

const CString CWebViewDatabase::LOGTAG = "webviewdatabase";

const CString CWebViewDatabase::DATABASE_FILE = "webview.db";
const CString CWebViewDatabase::CACHE_DATABASE_FILE = "webviewCache.db";

// column id strings for "_id" which can be used by any table
const CString CWebViewDatabase::ID_COL = "_id";

//static const char* ID_PROJECTION[];

// column id strings for "cookies" table
const CString CWebViewDatabase::COOKIES_NAME_COL = "name";

const CString CWebViewDatabase::COOKIES_VALUE_COL = "value";

const CString CWebViewDatabase::COOKIES_DOMAIN_COL = "domain";

const CString CWebViewDatabase::COOKIES_PATH_COL = "path";

const CString CWebViewDatabase::COOKIES_EXPIRES_COL = "expires";

const CString CWebViewDatabase::COOKIES_SECURE_COL = "secure";

// column id strings for "cache" table
const CString CWebViewDatabase::CACHE_URL_COL = "url";

const CString CWebViewDatabase::CACHE_FILE_PATH_COL = "filepath";

const CString CWebViewDatabase::CACHE_LAST_MODIFY_COL = "lastmodify";

const CString CWebViewDatabase::CACHE_ETAG_COL = "etag";

const CString CWebViewDatabase::CACHE_EXPIRES_COL = "expires";

const CString CWebViewDatabase::CACHE_EXPIRES_STRING_COL = "expiresstring";

const CString CWebViewDatabase::CACHE_MIMETYPE_COL = "mimetype";

const CString CWebViewDatabase::CACHE_ENCODING_COL = "encoding";

const CString CWebViewDatabase::CACHE_HTTP_STATUS_COL = "httpstatus";

const CString CWebViewDatabase::CACHE_LOCATION_COL = "location";

const CString CWebViewDatabase::CACHE_CONTENTLENGTH_COL = "contentlength";

const CString CWebViewDatabase::CACHE_CONTENTDISPOSITION_COL = "contentdisposition";

const CString CWebViewDatabase::CACHE_CROSSDOMAIN_COL = "crossdomain";

// column id strings for "password" table
const CString CWebViewDatabase::PASSWORD_HOST_COL = "host";

const CString CWebViewDatabase::PASSWORD_USERNAME_COL = "username";

const CString CWebViewDatabase::PASSWORD_PASSWORD_COL = "password";

// column id strings for "formurl" table
const CString CWebViewDatabase::FORMURL_URL_COL = "url";

// column id strings for "formdata" table
const CString CWebViewDatabase::FORMDATA_URLID_COL = "urlid";

const CString CWebViewDatabase::FORMDATA_NAME_COL = "name";

const CString CWebViewDatabase::FORMDATA_VALUE_COL = "value";

// column id strings for "httpauth" table
const CString CWebViewDatabase::HTTPAUTH_HOST_COL = "host";

const CString CWebViewDatabase::HTTPAUTH_REALM_COL = "realm";

const CString CWebViewDatabase::HTTPAUTH_USERNAME_COL = "username";

const CString CWebViewDatabase::HTTPAUTH_PASSWORD_COL = "password";

// log tag
const CString LOGTAG = "webviewdatabase";

const CString DATABASE_FILE = "webview.db";
const CString CACHE_DATABASE_FILE = "webviewCache.db";

// 1 -> 2 Add expires String
// 2 -> 3 Add content-disposition
// 3 -> 4 Add crossdomain (For x-permitted-cross-domain-policies header)
    
CWebViewDatabase* CWebViewDatabase::mInstance = NULL;
ISQLiteDatabase* CWebViewDatabase::mDatabase = NULL;
ISQLiteDatabase* CWebViewDatabase::mCacheDatabase = NULL;

// use InsertHelper to improve insert performance by 40%
DatabaseUtils::InsertHelper* CWebViewDatabase::mCacheInserter = NULL;
Int32 CWebViewDatabase::mCacheUrlColIndex;
Int32 CWebViewDatabase::mCacheFilePathColIndex;
Int32 CWebViewDatabase::mCacheLastModifyColIndex;
Int32 CWebViewDatabase::mCacheETagColIndex;
Int32 CWebViewDatabase::mCacheExpiresColIndex;
Int32 CWebViewDatabase::mCacheExpiresStringColIndex;
Int32 CWebViewDatabase::mCacheMimeTypeColIndex;
Int32 CWebViewDatabase::mCacheEncodingColIndex;
Int32 CWebViewDatabase::mCacheHttpStatusColIndex;
Int32 CWebViewDatabase::mCacheLocationColIndex;
Int32 CWebViewDatabase::mCacheContentLengthColIndex;
Int32 CWebViewDatabase::mCacheContentDispositionColIndex;
Int32 CWebViewDatabase::mCacheCrossDomainColIndex;

Int32 CWebViewDatabase::mCacheTransactionRefcount;

ECode CWebViewDatabase::GetInstance(
    /* [in] */ IContext* context,
    /* [out] */ IWebViewDatabase** instance)
{
    if (mInstance == NULL) {
        AcquireSingletonByFriend(&mInstance);
    }

    return NOERROR;
}

ECode CWebViewDatabase::HasUsernamePassword(
    /* [out] */ Boolean* flag)
{
    VALIDATE_NOT_NULL(flag);

    mPasswordLock.Lock();
    *flag = HasEntries(TABLE_PASSWORD_ID);
    mPasswordLock.Unlock();

    return NOERROR;
}

ECode CWebViewDatabase::ClearUsernamePassword()
{
    if (mDatabase == NULL) {
        return E_NOT_IMPLEMENTED;
    }

    mHttpAuthLock.Lock();
//    mDatabase->Delete(mTableNames[TABLE_PASSWORD_ID], NULL, NULL);
    mHttpAuthLock.Unlock();

    return NOERROR;
}

ECode CWebViewDatabase::HasHttpAuthUsernamePassword(
    /* [out] */ Boolean* flag)
{
    VALIDATE_NOT_NULL(flag);

    mPasswordLock.Lock();
    *flag = HasEntries(TABLE_HTTPAUTH_ID);
    mPasswordLock.Unlock();

    return NOERROR;
}

ECode CWebViewDatabase::ClearHttpAuthUsernamePassword()
{
    if (mDatabase == NULL) {
        return E_NOT_IMPLEMENTED;
    }

    mHttpAuthLock.Lock();
//    mDatabase->Delete(mTableNames[TABLE_HTTPAUTH_ID], NULL, NULL);
    mHttpAuthLock.Unlock();

    return NOERROR;
}

ECode CWebViewDatabase::HasFormData(
    /* [out] */ Boolean* flag)
{
    VALIDATE_NOT_NULL(flag);

    mFormLock.Lock();
    *flag = HasEntries(TABLE_FORMURL_ID);
    mFormLock.Unlock();

    return NOERROR;
}

ECode CWebViewDatabase::ClearFormData()
{
    if (mDatabase == NULL) {
        return E_NOT_IMPLEMENTED;
    }

    mFormLock.Lock();
//    mDatabase->Delete(mTableNames[TABLE_FORMURL_ID], NULL, NULL);
//    mDatabase->Delete(mTableNames[TABLE_FORMDATA_ID], NULL, NULL);
    mFormLock.Unlock();

    return NOERROR;
}

ECode CWebViewDatabase::CreateInstance(
    /* [in] */ IContext* context,
    /* [out] */ CWebViewDatabase** param0)
{
    if (mInstance == NULL) {
        AcquireSingletonByFriend(&mInstance);
    }

    *param0 = mInstance;

    return NOERROR;
}

CARAPI_(void) CWebViewDatabase::GetCookiesForDomain(
    /* [in] */ const String& domain,
    /* [out] */ Vector<AutoPtr<CCookieManager::Cookie> >& list)
{
    //ArrayList<Cookie> list = new ArrayList<Cookie>();
    if (domain.GetLength() == 0 || mDatabase == NULL) {
        return;
    }

    mCookieLock.Lock();
    {
        String* columns;/*[]; = new String[] {
                ID_COL, COOKIES_DOMAIN_COL, COOKIES_PATH_COL,
                COOKIES_NAME_COL, COOKIES_VALUE_COL, COOKIES_EXPIRES_COL,
                COOKIES_SECURE_COL
        };*/

        String selection("(");
        selection += COOKIES_DOMAIN_COL;
        selection += " GLOB '*' || ?)";

        AutoPtr<ICursor> cursor = NULL;

        /*
        cursor = mDatabase->Query(mTableNames[TABLE_COOKIES_ID],
                columns, selection, new String[] { domain }, null, null,
                null);*/
        Boolean bFlag = FALSE;
        cursor->MoveToFirst(&bFlag);
        if (bFlag) {
            Int32 domainCol;
            Int32 pathCol;
            Int32 nameCol;
            Int32 valueCol;
            Int32 expiresCol;
            Int32 secureCol;
            Boolean moveToNext = FALSE;

            cursor->GetColumnIndex((String)COOKIES_DOMAIN_COL, &domainCol);
            cursor->GetColumnIndex((String)COOKIES_PATH_COL, &pathCol);
            cursor->GetColumnIndex((String)COOKIES_NAME_COL, &nameCol);
            cursor->GetColumnIndex((String)COOKIES_VALUE_COL, &valueCol);
            cursor->GetColumnIndex((String)COOKIES_EXPIRES_COL, &expiresCol);
            cursor->GetColumnIndex((String)COOKIES_SECURE_COL, &secureCol);
            do {
                AutoPtr<CCookieManager::Cookie> cookie;// = new Cookie();
                cursor->GetString(domainCol, &(cookie->domain));
                cursor->GetString(pathCol, &(cookie->path));
                cursor->GetString(nameCol, &(cookie->name));
                cursor->GetString(valueCol, &(cookie->value));
                Boolean isNull = FALSE;
                cursor->IsNull(expiresCol, &isNull);
                if (isNull) {
                    cookie->expires = -1;
                } else {
//                    cookie->expires = cursor->GetLong(expiresCol);
                }
//                cookie->secure = cursor->GetShort(secureCol) != 0;
                cookie->mode = CCookieManager::Cookie::MODE_NORMAL;
                list.PushBack(cookie);
                cursor->MoveToNext(&moveToNext);
            } while (moveToNext);
        }
    }
    mCookieLock.Unlock();
}

CARAPI_(void) CWebViewDatabase::DeleteCookies(
    /* [in] */ const String& domain,
    /* [in] */ const String& path,
    /* [in] */ const String& name)
{
    if (domain.GetLength() == 0 || mDatabase == NULL) {
        return;
    }

    mCookieLock.Lock();
    {
        String where("(");
        where += COOKIES_DOMAIN_COL;
        where += " == ?) AND (";
        where += COOKIES_PATH_COL;
        where += " == ?) AND (";
        where += COOKIES_NAME_COL;
        where += " == ?)";

//        mDatabase.delete(mTableNames[TABLE_COOKIES_ID], where,
//                new String[] { domain, path, name });
    }
    mCookieLock.Unlock();
}

CARAPI_(void) CWebViewDatabase::AddCookie(
    /* [in] */ const CCookieManager::Cookie& cookie)
{
    if (cookie.domain.GetLength() == 0 || cookie.path.GetLength() == 0
      || cookie.name.GetLength() == 0 || mDatabase == NULL) {
        return;
    }

    mCookieLock.Lock();
    {
        AutoPtr<IContentValues> cookieVal = NULL;
        IContentValues* _cookieVal = NULL;
        CContentValues::New(&_cookieVal);
        cookieVal = _cookieVal;
        cookieVal->PutString((String)COOKIES_DOMAIN_COL, cookie.domain);
        cookieVal->PutString((String)COOKIES_PATH_COL, cookie.path);
        cookieVal->PutString((String)COOKIES_NAME_COL, cookie.name);
        cookieVal->PutString((String)COOKIES_VALUE_COL, cookie.value);
        if (cookie.expires != -1) {
            cookieVal->PutInt32((String)COOKIES_EXPIRES_COL, cookie.expires);
        }
        cookieVal->PutInt32((String)COOKIES_SECURE_COL, cookie.secure);
//        mDatabase->insert(mTableNames[TABLE_COOKIES_ID], NULL, cookieVal);
    }
    mCookieLock.Unlock();
}


CARAPI_(Boolean) CWebViewDatabase::HasCookies()
{
    if (mDatabase == NULL) {
        return FALSE;
    }

    mCookieLock.Lock();
//    mDatabase->Delete(mTableNames[TABLE_COOKIES_ID], NULL, NULL);
    mCookieLock.Unlock();
}

CARAPI_(void) CWebViewDatabase::ClearCookies()
{
    if (mDatabase == NULL) {
        return;
    }

    mCookieLock.Lock();
//    mDatabase->Delete(mTableNames[TABLE_COOKIES_ID], NULL, NULL);
    mCookieLock.Unlock();
}

CARAPI_(void) CWebViewDatabase::ClearSessionCookies()
{
    if (mDatabase == NULL) {
        return;
    }

    String sessionExpired(COOKIES_EXPIRES_COL);
    sessionExpired += " ISNULL";

    mCookieLock.Lock();
//    mDatabase->Delete(mTableNames[TABLE_COOKIES_ID], sessionExpired, NULL);
    mCookieLock.Unlock();
}

CARAPI_(void) CWebViewDatabase::ClearExpiredCookies(
    /* [in] */ Int64 now)
{
    if (mDatabase == NULL) {
        return;
    }

    String expires(COOKIES_EXPIRES_COL);
    expires += " <= ?";

    mCookieLock.Lock();
//    mDatabase.Delete(mTableNames[TABLE_COOKIES_ID], expires, new String[] { Long.toString(now) });
    mCookieLock.Unlock();
}

//
// cache functions
//

// only called from WebViewWorkerThread
CARAPI_(Boolean) CWebViewDatabase::StartCacheTransaction()
{
    return FALSE;
}

// only called from WebViewWorkerThread
CARAPI_(Boolean) CWebViewDatabase::EndCacheTransaction()
{
    return FALSE;
}

CARAPI_(CCacheManager::CacheResult*) CWebViewDatabase::GetCache(
    /* [in] */ const String& url)
{
    if (url.GetLength() == 0 || mCacheDatabase == NULL) {
        return NULL;
    }

    AutoPtr<ICursor> cursor = NULL;
    String query("SELECT filepath, lastmodify, etag, expires, ");
    query += "expiresstring, mimetype, encoding, httpstatus, location, contentlength, ";
    query += "contentdisposition, crossdomain FROM cache WHERE url = ?";
    
//    cursor = mCacheDatabase.rawQuery(query, new String[] { url });
    Boolean bFlag = FALSE;
    cursor->MoveToFirst(&bFlag);
    if (bFlag) {
        CCacheManager::CacheResult* ret;// = new CCacheManager::CacheResult();
        cursor->GetString(0, &(ret->localPath));
        cursor->GetString(1, &(ret->lastModified));
        cursor->GetString(2, &(ret->etag));
//        cursor->GetLong(3, &(ret->expires));
        cursor->GetString(4, &(ret->expiresString));
        cursor->GetString(5, &(ret->mimeType));
        cursor->GetString(6, &(ret->encoding));
//        cursor->GetInt(7, &(ret->httpStatusCode));
        cursor->GetString(8, &(ret->location));
//        cursor->GetLong(9, &(ret->contentLength));
        cursor->GetString(10, &(ret->contentdisposition));
        cursor->GetString(11, &(ret->crossDomain));
        return ret;
    }
 
    return NULL;
}

CARAPI_(void) CWebViewDatabase::RemoveCache(
    /* [in] */ const String& url)
{
    if (url.GetLength() == 0 || mCacheDatabase == NULL) {
        return;
    }

//    mCacheDatabase.execSQL("DELETE FROM cache WHERE url = ?", new String[] { url });
}

CARAPI_(void) CWebViewDatabase::AddCache(
    /* [in] */ const String url,
    /* [in] */ ICacheManagerCacheResult* cacheResult)
{
    if (url.GetLength() == 0 || mCacheDatabase == NULL
        || cacheResult == NULL) {
        return;
    }

    CCacheManager::CacheResult* c = (CCacheManager::CacheResult*)cacheResult;

    mCacheInserter->PrepareForInsert();
    mCacheInserter->Bind(mCacheUrlColIndex, url);
    mCacheInserter->Bind(mCacheFilePathColIndex, c->localPath);
    mCacheInserter->Bind(mCacheLastModifyColIndex, c->lastModified);
    mCacheInserter->Bind(mCacheETagColIndex, c->etag);
    mCacheInserter->Bind(mCacheExpiresColIndex, c->expires);
    mCacheInserter->Bind(mCacheExpiresStringColIndex, c->expiresString);
    mCacheInserter->Bind(mCacheMimeTypeColIndex, c->mimeType);
    mCacheInserter->Bind(mCacheEncodingColIndex, c->encoding);
    mCacheInserter->Bind(mCacheHttpStatusColIndex, c->httpStatusCode);
    mCacheInserter->Bind(mCacheLocationColIndex, c->location);
    mCacheInserter->Bind(mCacheContentLengthColIndex, c->contentLength);
    mCacheInserter->Bind(mCacheContentDispositionColIndex, c->contentdisposition);
    mCacheInserter->Bind(mCacheCrossDomainColIndex, c->crossDomain);
    mCacheInserter->Execute(NULL);
}

CARAPI_(void) CWebViewDatabase::ClearCache()
{
    if (mCacheDatabase == NULL) {
        return;
    }

//    mCacheDatabase.delete("cache", null, null);
}

CARAPI_(Boolean) CWebViewDatabase::HasCache()
{
    if (mCacheDatabase == NULL) {
        return FALSE;
    }

    AutoPtr<ICursor> cursor = NULL;
    Boolean ret = FALSE;
//    cursor = mCacheDatabase->Query("cache", ID_PROJECTION, 
//        NULL, NULL, NULL, NULL, NULL);
    cursor->MoveToFirst(&ret);

    return ret;
}

CARAPI_(Int64) CWebViewDatabase::GetCacheTotalSize()
{
    if (mCacheDatabase == NULL) {
        return 0;
    }

    Int64 size = 0;
    AutoPtr<ICursor> cursor = NULL;
    String query("SELECT SUM(contentlength) as sum FROM cache");
//    cursor = mCacheDatabase.rawQuery(query, null);
    Boolean bFlag = FALSE;
    cursor->MoveToFirst(&bFlag);
    if (bFlag) {
//        cursor->GetLong(0, &size);
    }

    return size;
}

CARAPI_(void) CWebViewDatabase::TrimCache(
    /* [in] */ Int64 amount,
    /* [out] */ Vector<String>& pathList)
{
    //ArrayList<String> pathList = new ArrayList<String>(100);
    AutoPtr<ICursor> cursor = NULL;
    String query("SELECT contentlength, filepath FROM cache ORDER BY expires ASC");

//    cursor = mCacheDatabase.rawQuery(query, null);
    Boolean bMoveToFirst = FALSE;
    cursor->MoveToFirst(&bMoveToFirst);
    if (bMoveToFirst) {
        Int32 batchSize = 100;
        //StringBuilder pathStr = new StringBuilder(20 + 16 * batchSize);
        //pathStr.append("DELETE FROM cache WHERE filepath IN (?");
        String pathStr("DELETE FROM cache WHERE filepath IN (?");

        for (Int32 i = 1; i < batchSize; i++) {
            pathStr.Append(", ?");
        }

        pathStr.Append(")");
        ISQLiteStatement* statement = NULL;
        
//        statement = mCacheDatabase.compileStatement(pathStr.toString());
        // as bindString() uses 1-based index, initialize index to 1
        Int32 index = 1;
        Boolean bFlag = FALSE;
        do {
            Int64 length = 0;
//            cursor->GetLong(0, &length);
            if (length == 0) {
                continue;
            }
            amount -= length;
            String filePath;
            cursor->GetString(1, &filePath);
            statement->BindString(index, filePath);
            pathList.PushBack(filePath);
            if (index++ == batchSize) {
                statement->Execute();
                statement->ClearBindings();
                index = 1;
            }
            
            cursor->MoveToNext(&bFlag);
        } while (bFlag && amount > 0);

        if (index > 1) {
            // there may be old bindings from the previous statement
            // if index is less than batchSize, which is Ok.
            statement->Execute();
        }
    }
}

CARAPI_(void) CWebViewDatabase::GetAllCacheFileNames(
    /* [out] */ Vector<String>& pathList)
{
    AutoPtr<ICursor> cursor = NULL;

//    cursor = mCacheDatabase.rawQuery("SELECT filepath FROM cache", null);
    Boolean bMoveToFirst = FALSE;
    cursor->MoveToFirst(&bMoveToFirst);
    if (cursor != NULL && bMoveToFirst) {
        Boolean bMoveToNext = FALSE;
        String str;

        //pathList = new ArrayList<String>(cursor.getCount());
        do {
            cursor->GetString(0, &str);
            pathList.PushBack(str);
            cursor->MoveToNext(&bMoveToNext);
        } while (bMoveToNext);
    }
}

//
// password functions
//

CARAPI_(void) CWebViewDatabase::SetUsernamePassword(
    /* [in] */ const String& schemePlusHost,
    /* [in] */ const String& username,
    /* [in] */ const String& password)
{
    if (schemePlusHost.GetLength() == 0 || mDatabase == NULL) {
        return;
    }

    mPasswordLock.Lock();
    {
        AutoPtr<IContentValues> c = NULL;
        
        CContentValues::New((IContentValues**)&c);
        assert(c);

        c->PutString((String)PASSWORD_HOST_COL, schemePlusHost);
        c->PutString((String)PASSWORD_USERNAME_COL, username);
        c->PutString((String)PASSWORD_PASSWORD_COL, password);
//        mDatabase.insert(mTableNames[TABLE_PASSWORD_ID], PASSWORD_HOST_COL, c);
    }
    mPasswordLock.Unlock();
}

CARAPI_(void) CWebViewDatabase::GetUsernamePassword(
    /* [in] */ const String& schemePlusHost,
    /* [out] */ Vector<String>& ret)
{
    if (schemePlusHost.GetLength() == 0 || mDatabase == NULL) {
            return;
    }

    const String columns[] = 
           {
              (String)PASSWORD_USERNAME_COL,
              (String)PASSWORD_PASSWORD_COL
           };

    String selection("(");
    selection += PASSWORD_HOST_COL;
    selection += " == ?)";

    mPasswordLock.Lock();
    {
        AutoPtr<ICursor> cursor = NULL;
//        cursor = mDatabase.query(mTableNames[TABLE_PASSWORD_ID],
//                columns, selection, new String[] { schemePlusHost }, null,
//                null, null);
        Boolean bMoveToFirst = FALSE;
        cursor->MoveToFirst(&bMoveToFirst);
        if (bMoveToFirst) {
            Int32 columnIndex = 0;

            cursor->GetColumnIndex((String)PASSWORD_USERNAME_COL, &columnIndex);
            cursor->GetString(columnIndex, &ret[0]);
            cursor->GetColumnIndex((String)PASSWORD_PASSWORD_COL, &columnIndex);
            cursor->GetString(columnIndex, &ret[1]);
        }
    }
    mPasswordLock.Unlock();
}

//
// http authentication password functions
//

CARAPI_(void) CWebViewDatabase::SetHttpAuthUsernamePassword(
    /* [in] */ const String& host,
    /* [in] */ const String realm,
    /* [in] */ const String username,
    /* [in] */ const String password)
{
    if (host.GetLength() == 0 || realm.GetLength() == 0
       || mDatabase == NULL) {
        return;
    }

    mHttpAuthLock.Lock();
    {
        AutoPtr<IContentValues> c = NULL;

        CContentValues::New((IContentValues**)&c);
        assert(c);

        c->PutString((String)HTTPAUTH_HOST_COL, host);
        c->PutString((String)HTTPAUTH_REALM_COL, realm);
        c->PutString((String)HTTPAUTH_USERNAME_COL, username);
        c->PutString((String)HTTPAUTH_PASSWORD_COL, password);
//        mDatabase.insert(mTableNames[TABLE_HTTPAUTH_ID], HTTPAUTH_HOST_COL, c);
    }
    mHttpAuthLock.Unlock();
}

CARAPI_(void) CWebViewDatabase::GetHttpAuthUsernamePassword(
    /* [in] */ const String& host,
    /* [in] */ const String& realm,
    /* [out] */ Vector<String>& ret)
{
    if (host.GetLength() == 0 || realm.GetLength() == 0
       || mDatabase == NULL) {
        return;
    }

    const String columns[] = 
         {
            (String)HTTPAUTH_USERNAME_COL,
            (String)HTTPAUTH_PASSWORD_COL
         };

    String selection("(");
    selection += HTTPAUTH_HOST_COL;
    selection += " == ?) AND (";
    selection += HTTPAUTH_REALM_COL;
    selection += " == ?)";

    mHttpAuthLock.Lock();
    {
        AutoPtr<ICursor> cursor = NULL;
    
//        cursor = mDatabase.query(mTableNames[TABLE_HTTPAUTH_ID],
//                columns, selection, new String[] { host, realm }, null,
//                null, null);

        Boolean bMoveToFirst = FALSE;
        cursor->MoveToFirst(&bMoveToFirst);
        if (bMoveToFirst) {
            Int32 columnIndex = 0;

            cursor->GetColumnIndex((String)HTTPAUTH_USERNAME_COL, &columnIndex);
            cursor->GetString(columnIndex, &ret[0]);
            cursor->GetColumnIndex((String)HTTPAUTH_PASSWORD_COL, &columnIndex);
            cursor->GetString(columnIndex, &ret[1]);
        }
    }
    mHttpAuthLock.Unlock();
}

CARAPI_(void) CWebViewDatabase::SetFormData(
    /* [in] */ const String& url,
    /* [in] */ IObjectStringMap* formdata)
{
    if (url.GetLength() == 0 || formdata == NULL || mDatabase == NULL) {
        return;
    }

    String selection("(");
    selection += FORMURL_URL_COL;
    selection += " == ?)";
#if 0
    synchronized (mFormLock)
    {
        long urlid = -1;
        Cursor cursor = null;
        
        cursor = mDatabase.query(mTableNames[TABLE_FORMURL_ID],
                ID_PROJECTION, selection, new String[] { url }, null, null,
                null);
        if (cursor.moveToFirst()) {
            urlid = cursor.getLong(cursor.getColumnIndex(ID_COL));
        } else {
            ContentValues c = new ContentValues();
            c.put(FORMURL_URL_COL, url);
            urlid = mDatabase.insert(
                    mTableNames[TABLE_FORMURL_ID], null, c);
        }
        
        if (urlid >= 0)
        {
            Set<Entry<String, String>> set = formdata.entrySet();
            Iterator<Entry<String, String>> iter = set.iterator();
            ContentValues map = new ContentValues();
            map.put(FORMDATA_URLID_COL, urlid);
            while (iter.hasNext())
            {
                Entry<String, String> entry = iter.next();
                map.put(FORMDATA_NAME_COL, entry.getKey());
                map.put(FORMDATA_VALUE_COL, entry.getValue());
                mDatabase.insert(mTableNames[TABLE_FORMDATA_ID], null, map);
            }
        }
    }
#endif
}

CARAPI_(void) CWebViewDatabase::GetFormData(
    /* [in] */ const String& url,
    /* [in] */ const String& name,
    /* [out] */ Vector<String>& values)
{
    if (url.GetLength() == 0 || name.GetLength() == 0 
        || mDatabase == NULL) {
        return;
    }

    String urlSelection("(");
    urlSelection += FORMURL_URL_COL;
    urlSelection += " == ?)";

    String dataSelection("(");
    dataSelection += FORMDATA_URLID_COL;
    dataSelection += " == ?) AND (";
    dataSelection += FORMDATA_NAME_COL;
    dataSelection += " == ?)";

    mFormLock.Lock();
    {
        AutoPtr<ICursor> cursor = NULL;

//        cursor = mDatabase.query(mTableNames[TABLE_FORMURL_ID],
//                ID_PROJECTION, urlSelection, new String[] { url }, null,
//                null, null);
        Boolean bMoveToFirst = FALSE;
        cursor->MoveToFirst(&bMoveToFirst);
        if (bMoveToFirst) {
            long urlid = 0;//cursor.getLong(cursor.getColumnIndex(ID_COL));
            AutoPtr<ICursor> dataCursor = NULL;

//            dataCursor = mDatabase.query(
//                    mTableNames[TABLE_FORMDATA_ID],
//                    new String[] { ID_COL, FORMDATA_VALUE_COL },
//                    dataSelection,
//                    new String[] { Long.toString(urlid), name },
//                    null, null, null);

            Boolean bFlag = FALSE;
            dataCursor->MoveToFirst(&bFlag);
            if (bFlag) {
                int valueCol = 0;
                String str;
                Boolean bMoveToNext = FALSE;
                dataCursor->GetColumnIndex((String)FORMDATA_VALUE_COL, &valueCol);
                do {
                    dataCursor->GetString(valueCol, &str);
                    values.PushBack(str);
                    dataCursor->MoveToNext(&bMoveToNext);
                } while (bMoveToNext);
            }

        }
    }
    mFormLock.Unlock();
}

CARAPI_(void) CWebViewDatabase::UpgradeDatabase()
{
#if 0
    int oldVersion = mDatabase.getVersion();
    if (oldVersion != 0) {
        Log.i(LOGTAG, "Upgrading database from version "
                + oldVersion + " to "
                + DATABASE_VERSION + ", which will destroy old data");
    }
    boolean justPasswords = 8 == oldVersion && 9 == DATABASE_VERSION;
    boolean justAuth = 9 == oldVersion && 10 == DATABASE_VERSION;
    if (justAuth) {
        mDatabase.execSQL("DROP TABLE IF EXISTS "
                + mTableNames[TABLE_HTTPAUTH_ID]);
        mDatabase.execSQL("CREATE TABLE " + mTableNames[TABLE_HTTPAUTH_ID]
                + " (" + ID_COL + " INTEGER PRIMARY KEY, "
                + HTTPAUTH_HOST_COL + " TEXT, " + HTTPAUTH_REALM_COL
                + " TEXT, " + HTTPAUTH_USERNAME_COL + " TEXT, "
                + HTTPAUTH_PASSWORD_COL + " TEXT," + " UNIQUE ("
                + HTTPAUTH_HOST_COL + ", " + HTTPAUTH_REALM_COL
                + ") ON CONFLICT REPLACE);");
        return;
    }

    if (!justPasswords) {
        mDatabase.execSQL("DROP TABLE IF EXISTS "
                + mTableNames[TABLE_COOKIES_ID]);
        mDatabase.execSQL("DROP TABLE IF EXISTS cache");
        mDatabase.execSQL("DROP TABLE IF EXISTS "
                + mTableNames[TABLE_FORMURL_ID]);
        mDatabase.execSQL("DROP TABLE IF EXISTS "
                + mTableNames[TABLE_FORMDATA_ID]);
        mDatabase.execSQL("DROP TABLE IF EXISTS "
                + mTableNames[TABLE_HTTPAUTH_ID]);
    }
    mDatabase.execSQL("DROP TABLE IF EXISTS "
            + mTableNames[TABLE_PASSWORD_ID]);

    mDatabase.setVersion(DATABASE_VERSION);

    if (!justPasswords) {
        // cookies
        mDatabase.execSQL("CREATE TABLE " + mTableNames[TABLE_COOKIES_ID]
                + " (" + ID_COL + " INTEGER PRIMARY KEY, "
                + COOKIES_NAME_COL + " TEXT, " + COOKIES_VALUE_COL
                + " TEXT, " + COOKIES_DOMAIN_COL + " TEXT, "
                + COOKIES_PATH_COL + " TEXT, " + COOKIES_EXPIRES_COL
                + " INTEGER, " + COOKIES_SECURE_COL + " INTEGER" + ");");
        mDatabase.execSQL("CREATE INDEX cookiesIndex ON "
                + mTableNames[TABLE_COOKIES_ID] + " (path)");

        // formurl
        mDatabase.execSQL("CREATE TABLE " + mTableNames[TABLE_FORMURL_ID]
                + " (" + ID_COL + " INTEGER PRIMARY KEY, " + FORMURL_URL_COL
                + " TEXT" + ");");

        // formdata
        mDatabase.execSQL("CREATE TABLE " + mTableNames[TABLE_FORMDATA_ID]
                + " (" + ID_COL + " INTEGER PRIMARY KEY, "
                + FORMDATA_URLID_COL + " INTEGER, " + FORMDATA_NAME_COL
                + " TEXT, " + FORMDATA_VALUE_COL + " TEXT," + " UNIQUE ("
                + FORMDATA_URLID_COL + ", " + FORMDATA_NAME_COL + ", "
                + FORMDATA_VALUE_COL + ") ON CONFLICT IGNORE);");

        // httpauth
        mDatabase.execSQL("CREATE TABLE " + mTableNames[TABLE_HTTPAUTH_ID]
                + " (" + ID_COL + " INTEGER PRIMARY KEY, "
                + HTTPAUTH_HOST_COL + " TEXT, " + HTTPAUTH_REALM_COL
                + " TEXT, " + HTTPAUTH_USERNAME_COL + " TEXT, "
                + HTTPAUTH_PASSWORD_COL + " TEXT," + " UNIQUE ("
                + HTTPAUTH_HOST_COL + ", " + HTTPAUTH_REALM_COL
                + ") ON CONFLICT REPLACE);");
    }
    // passwords
    mDatabase.execSQL("CREATE TABLE " + mTableNames[TABLE_PASSWORD_ID]
            + " (" + ID_COL + " INTEGER PRIMARY KEY, "
            + PASSWORD_HOST_COL + " TEXT, " + PASSWORD_USERNAME_COL
            + " TEXT, " + PASSWORD_PASSWORD_COL + " TEXT," + " UNIQUE ("
            + PASSWORD_HOST_COL + ", " + PASSWORD_USERNAME_COL
            + ") ON CONFLICT REPLACE);");
#endif
}

CARAPI_(void) CWebViewDatabase::UpgradeCacheDatabase()
{}

CARAPI_(void) CWebViewDatabase::BootstrapCacheDatabase()
{}

CARAPI_(Boolean) CWebViewDatabase::HasEntries(
    /* [in] */ Int32 tableId)
{
    if (mDatabase == NULL) {
        return FALSE;
    }

    AutoPtr<ICursor> cursor = NULL;
    Boolean ret = FALSE;
    
//    cursor = mDatabase.query(mTableNames[tableId], ID_PROJECTION,
//            null, null, null, null, null);
    cursor->MoveToFirst(&ret);

    return ret;
}
