
#ifndef __CCACHEMANAGER_H__
#define __CCACHEMANAGER_H__

#include "_CCacheManager.h"
#include <StringBuffer.h>
#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>

//#include "CWebViewDatabase.h"

class CWebViewDatabase;

CarClass(CCacheManager)
{
public:

    CARAPI GetCacheFileBaseDir(
        /* [out] */ IFile ** ppDir);

    CARAPI CacheDisabled(
        /* [out] */ Boolean * pDisabled);

    CARAPI StartCacheTransaction(
        /* [out] */ Boolean * pFlag);

    CARAPI EndCacheTransaction(
        /* [out] */ Boolean * pFlag);

    CARAPI GetCacheFile(
        /* [in] */ const String& url,
        /* [in] */ IObjectStringMap * pHeaders,
        /* [out] */ ICacheManagerCacheResult ** ppResult);

    CARAPI CreateCacheFile(
        /* [in] */ const String& url,
        /* [in] */ Int32 statusCode,
        /* [in] */ IHeaders * pHeaders,
        /* [in] */ const String& mimeType,
        /* [in] */ Boolean forceCache,
        /* [out] */ ICacheManagerCacheResult ** ppResult);

    /**
     * Save the info of a cache file for a given url to the CacheMap so that it
     * can be reused later
     */
    CARAPI SaveCacheFile(
        /* [in] */ const String& url,
        /* [in] */ ICacheManagerCacheResult* cacheRet);

    /**
     * This class represents a resource retrieved from the HTTP cache.
     * Instances of this class can be obtained by invoking the
     * CacheManager.getCacheFile() method.
     */
    class CacheResult : public ICacheManagerCacheResult
    {
    public:
        inline CARAPI_(Int32) GetHttpStatusCode() const;

        inline CARAPI_(Int64) GetContentLength() const;

        inline CARAPI_(void) GetLocalPath(
            /* [out] */ String& str) const;

        inline CARAPI_(Int64) GetExpires() const;

        inline CARAPI_(void) GetExpiresString(
            /* [out] */ String& str) const;

        inline CARAPI_(void) GetLastModified(
            /* [out] */ String& str) const;

        inline CARAPI_(void) GetETag(
            /* [out] */ String& str) const;

        inline CARAPI_(void) GetMimeType(
            /* [out] */ String& str) const;

        inline CARAPI_(void) GetLocation(
            /* [out] */ String& str) const;

        inline CARAPI_(void) GetEncoding(
            /* [out] */ String& str) const;

        inline CARAPI_(void) GetContentDisposition(
            /* [out] */ String& str) const;

        // For out-of-package access to the underlying streams.
        inline CARAPI_(AutoPtr<IFileInputStream>) GetInputStream() const;

        inline CARAPI_(AutoPtr<IFileOutputStream>) GetOutputStream() const;

        // These fields can be set manually.
        inline CARAPI SetInputStream(IFileInputStream* stream);

        inline CARAPI_(void) SetEncoding(String encoding);

    public:
        // these fields are saved to the database
        Int32 httpStatusCode;
        Int64 contentLength;
        Int64 expires;
        String expiresString;
        String localPath;
        String lastModified;
        String etag;
        String mimeType;
        String location;
        String encoding;
        String contentdisposition;
        String crossDomain;

        // these fields are NOT saved to the database
        //IInputStream* inStream;
        //IOutputStream* outStream;
        AutoPtr<IFileInputStream> inStream;
        AutoPtr<IFileOutputStream> outStream;
        AutoPtr<IFile> outFile;
    };

public:
    static const char* HEADER_KEY_IFMODIFIEDSINCE;// = "if-modified-since";
    static const char* HEADER_KEY_IFNONEMATCH;// = "if-none-match";

public:
    /**
     * initialize the CacheManager. WebView should handle this for each process.
     * 
     * @param context The application context.
     */
    static CARAPI_(void) Init(
        /* [in] */ IContext* context);  

    /**
     * set the flag to control whether cache is enabled or disabled
     * 
     * @param disabled true to disable the cache
     */
    static CARAPI_(void) SetCacheDisabled(
        /* [in] */ Boolean disabled);

    // only called from WebViewWorkerThread
    // make sure to call enableTransaction/disableTransaction in pair
    static CARAPI_(Boolean) EnableTransaction();

    // only called from WebViewWorkerThread
    // make sure to call enableTransaction/disableTransaction in pair
    static CARAPI_(Boolean) DisableTransaction();

    // only called from WebViewWorkerThread
    // make sure to call startTransaction/endTransaction in pair
    static CARAPI_(Boolean) StartTransaction();

    // only called from WebViewWorkerThread
    // make sure to call startTransaction/endTransaction in pair
    static CARAPI_(Boolean) EndTransaction();

    static CARAPI_(AutoPtr<ICacheManagerCacheResult>) GetCacheFile(
        /* [in] */ const String& url, 
        /* [in] */ Int64 postIdentifier,
        /* [in] */ IObjectStringMap* headers);

    static CARAPI_(AutoPtr<ICacheManagerCacheResult>) CreateCacheFile(
        /* [in] */ const String& url,
        /* [in] */ Int32 statusCode,
        /* [in] */ IHeaders* headers,
        /* [in] */ const String& mimeType,
        /* [in] */ Int64 postIdentifier,
        /* [in] */ Boolean forceCache);

    static CARAPI_(void) SaveCacheFile(
        /* [in] */ const String& url,
        /* [in] */ Int64 postIdentifier,
        /* [in] */ CCacheManager::CacheResult* cacheRet);

    static CARAPI_(Boolean) CleanupCacheFile(
        /* [in] */ ICacheManagerCacheResult* cacheRet);

    /**
     * remove all cache files
     * 
     * @return true if it succeeds
     */
    static CARAPI_(Boolean) RemoveAllCacheFiles();

    /**
     * Return true if the cache is empty.
     */
    static CARAPI_(Boolean) CacheEmpty();

    static CARAPI_(void) TrimCacheIfNeeded();

    static CARAPI_(void) ClearCache();

public:
    // Limit the maximum cache file size to half of the normal capacity
    static Int64 CACHE_MAX_SIZE;

private:

    /**
     * Create the cache directory if it does not already exist.
     * 
     * @return true if the cache directory didn't exist and was created.
     */
    static CARAPI_(Boolean) CreateCacheDirectory();

    static CARAPI_(Boolean) CheckCacheRedirect(
        /* [in] */ Int32 statusCode);

    static CARAPI_(void) GetDatabaseKey(
        /* [in] */ const String& url,
        /* [in] */ Int64 postIdentifier,
        /* [out] */ String& str );

    //@SuppressWarnings("deprecation")
    static CARAPI_(void) SetupFiles(
        /* [in] */ const String& url,
        /* [in] */ ICacheManagerCacheResult* cacheRet);

    static CARAPI_(void) AppendAsHex(
        /* [in] */ Int32 i,
        /* [in] */ StringBuffer* ret);

    static CARAPI_(AutoPtr<ICacheManagerCacheResult>) ParseHeaders(
        /* [in] */ Int32 statusCode,
        /* [in] */ IHeaders* headers,
        /* [in] */ const String& mimeType);

private:

    static const char* LOGTAG;// = "cache";

    static const char* NO_STORE;// = "no-store";
    static const char* NO_CACHE;// = "no-cache";
    static const char* MAX_AGE;// = "max-age";
    static const char* MANIFEST_MIME;// = "text/cache-manifest";

    static Int64 CACHE_THRESHOLD;
    static Int64 CACHE_TRIM_AMOUNT;

    static Boolean mDisabled;

    // Reference count the enable/disable transaction
    static Int32 mRefCount;

    // trimCacheIfNeeded() is called when a page is fully loaded. But JavaScript
    // can load the content, e.g. in a slideshow, continuously, so we need to
    // trim the cache on a timer base too. endCacheTransaction() is called on a 
    // timer base. We share the same timer with less frequent update.
    static Int32 mTrimCacheCount;
    static const Int32 TRIM_CACHE_INTERVAL = 5;

    static AutoPtr<CWebViewDatabase> mDataBase;
    static AutoPtr<IFile> mBaseDir;
    
    // Flag to clear the cache when the CacheManager is initialized
    static Boolean mClearCacheOnInit;
};

#endif // __CCACHEMANAGER_H__