
#include "ext/frameworkext.h"
#include <elstring.h>
#include <elastos/Vector.h>
#include <elastos/System.h>

#include "webkit/CWebViewDatabase.h"
#include "webkit/CCacheManager.h"
#include "os/Runnable.h"
#include "os/FileUtils.h"

const CString CCacheManager::HEADER_KEY_IFMODIFIEDSINCE("if-modified-since");
const CString CCacheManager::HEADER_KEY_IFNONEMATCH("if-none-match");

const CString CCacheManager::LOGTAG("cache");

const CString CCacheManager::NO_STORE("no-store");
const CString CCacheManager::NO_CACHE("no-cache");
const CString CCacheManager::MAX_AGE("max-age");
const CString CCacheManager::MANIFEST_MIME("text/cache-manifest");

AutoPtr<CWebViewDatabase> CCacheManager::mDataBase;
AutoPtr<IFile> CCacheManager::mBaseDir;

Int64 CCacheManager::CACHE_THRESHOLD;
Int64 CCacheManager::CACHE_TRIM_AMOUNT;

// Limit the maximum cache file size to half of the normal capacity
Int64 CCacheManager::CACHE_MAX_SIZE;

Boolean CCacheManager::mDisabled;

// Reference count the enable/disable transaction
Int32 CCacheManager::mRefCount;
Int32 CCacheManager::mTrimCacheCount;

// Flag to clear the cache when the CacheManager is initialized
Boolean CCacheManager::mClearCacheOnInit;

ECode CCacheManager::GetCacheFileBaseDir(
    /* [out] */ IFile** dir)
{
    VALIDATE_NOT_NULL(dir);

    *dir = mBaseDir;

    return NOERROR;
}

ECode CCacheManager::CacheDisabled(
    /* [out] */ Boolean* disabled)
{
    VALIDATE_NOT_NULL(disabled);

    *disabled = mDisabled;

    return NOERROR;
}

ECode CCacheManager::StartCacheTransaction(
    /* [out] */ Boolean* flag)
{
    VALIDATE_NOT_NULL(flag);

    *flag = FALSE;

    return NOERROR;
}

ECode CCacheManager::EndCacheTransaction(
    /* [out] */ Boolean* flag)
{
    VALIDATE_NOT_NULL(flag);

    *flag = FALSE;

    return NOERROR;
}

ECode CCacheManager::GetCacheFile(
    /* [in] */ const String& url,
    /* [in] */ IObjectStringMap* headers,
    /* [out] */ ICacheManagerCacheResult** result)
{
    VALIDATE_NOT_NULL(headers);
 
    ICacheManagerCacheResult* r = GetCacheFile(url, 0, headers);
    if (result) {
        *result = r;
    }

    return NOERROR;
}

ECode CCacheManager::CreateCacheFile(
    /* [in] */ const String& url,
    /* [in] */ Int32 statusCode,
    /* [in] */ IHeaders* headers,
    /* [in] */ const String& mimeType,
    /* [in] */ Boolean forceCache,
    /* [out] */ ICacheManagerCacheResult** result)
{
    VALIDATE_NOT_NULL(headers);

    ICacheManagerCacheResult* r = CreateCacheFile(url, statusCode, headers, mimeType, 0,
                forceCache);
    if (result) {
        *result = r;
    }

    return NOERROR;
}

ECode CCacheManager::SaveCacheFile(
    /* [in] */ const String& url,
    /* [in] */ ICacheManagerCacheResult* cacheRet)
{
//    VALIDATE_NOT_NULL(cacheRet);

    SaveCacheFile(url, 0, (CCacheManager::CacheResult*)cacheRet);

    return NOERROR;
}


CARAPI_(void) CCacheManager::Init(
        /* [in] */ IContext* inContext)
{
    assert(inContext != NULL);

    IContext* pContext = NULL;
    inContext->GetApplicationContext(&pContext);

    CWebViewDatabase::CreateInstance(pContext, (CWebViewDatabase**)&mDataBase);
    String fileName/*(pContext->GetCacheDir())*/;
    fileName.Append("webviewCache");
//    mBaseDir = new File(pContext->GetCacheDir(), "webviewCache");
    CFile::New(fileName, (IFile**)&mBaseDir);
    if (CreateCacheDirectory() && mClearCacheOnInit) {
        RemoveAllCacheFiles();
        mClearCacheOnInit = FALSE;
    }
}

/**
 * set the flag to control whether cache is enabled or disabled
 * 
 * @param disabled true to disable the cache
 */
CARAPI_(void) CCacheManager::SetCacheDisabled(
    /* [in] */ Boolean disabled)
{
    if (disabled == mDisabled) {
        return;
    }

    mDisabled = disabled;
    if (mDisabled) {
        RemoveAllCacheFiles();
    }
}

// only called from WebViewWorkerThread
// make sure to call enableTransaction/disableTransaction in pair
CARAPI_(Boolean) CCacheManager::EnableTransaction()
{
    if (++mRefCount == 1) {
        mDataBase->StartCacheTransaction();
        return TRUE;
    }

    return FALSE;
}

// only called from WebViewWorkerThread
// make sure to call enableTransaction/disableTransaction in pair
CARAPI_(Boolean) CCacheManager::DisableTransaction()
{
    if (--mRefCount == 0) {
        mDataBase->EndCacheTransaction();
        return TRUE;
    }

    return FALSE;
}

// only called from WebViewWorkerThread
// make sure to call startTransaction/endTransaction in pair
CARAPI_(Boolean) CCacheManager::StartTransaction()
{
    return mDataBase->StartCacheTransaction();
}

// only called from WebViewWorkerThread
// make sure to call startTransaction/endTransaction in pair
CARAPI_(Boolean) CCacheManager::EndTransaction()
{
    Boolean ret = mDataBase->EndCacheTransaction();
    if (++mTrimCacheCount >= TRIM_CACHE_INTERVAL) {
        mTrimCacheCount = 0;
        TrimCacheIfNeeded();
    }

    return ret;
}

CARAPI_(AutoPtr<ICacheManagerCacheResult>) CCacheManager::GetCacheFile(
    /* [in] */ const String& url, 
    /* [in] */ Int64 postIdentifier,
    /* [in] */ IObjectStringMap* headers)
{
    assert(headers == NULL);

    if (mDisabled) {
        return NULL;
    }

    String databaseKey;
    GetDatabaseKey(url, postIdentifier, databaseKey);

    CCacheManager::CacheResult* result = (CCacheManager::CacheResult*)mDataBase->GetCache(databaseKey);
    if (result != NULL) {
        if (result->contentLength == 0) {
            if (!CheckCacheRedirect(result->httpStatusCode)) {
                // this should not happen. If it does, remove it.
                mDataBase->RemoveCache(databaseKey);
                return NULL;
            }
        } else {
            String path;
            mBaseDir->GetPath(&path);
            path.Append(result->localPath);

            //File src = new File(mBaseDir, result->localPath);
            IFile* src = NULL;
            CFile::New(path, &src);
         //   try
         //   {
                // open here so that even the file is deleted, the content
                // is still readable by the caller until close() is called
                //result.inStream = new IFileInputStream(src);
                CFileInputStream::New(src, (IFileInputStream**)&result->inStream);
          //  } catch (FileNotFoundException e) {
                // the files in the cache directory can be removed by the
                // system. If it is gone, clean up the database
          //      mDataBase.removeCache(databaseKey);
          //      return null;
          //  }
        }
    } else {
        return NULL;
    }

    // null headers request coming from CACHE_MODE_CACHE_ONLY
    // which implies that it needs cache even it is expired.
    // negative expires means time in the far future.
    if (headers != NULL && result->expires >= 0
            && result->expires <= System::GetCurrentTimeMillis()) {
        if (result->lastModified == NULL && result->etag == NULL) {
            return NULL;
        }

        // return HEADER_KEY_IFNONEMATCH or HEADER_KEY_IFMODIFIEDSINCE
        // for requesting validation
        if (result->etag != NULL) {
            headers->Put((const String)HEADER_KEY_IFNONEMATCH, (IInterface*)&(result->etag));
        }

        if (result->lastModified != NULL) {
            headers->Put((const String)HEADER_KEY_IFMODIFIEDSINCE, (IInterface*)&(result->lastModified));
        }
    }

    /*if (DebugFlags.CACHE_MANAGER) {
        Log.v(LOGTAG, "getCacheFile for url " + url);
    }*/

    return result;
}

CARAPI_(AutoPtr<ICacheManagerCacheResult>) CCacheManager::CreateCacheFile(
    /* [in] */ const String& url,
    /* [in] */ Int32 statusCode,
    /* [in] */ IHeaders* headers,
    /* [in] */ const String& mimeType,
    /* [in] */ Int64 postIdentifier,
    /* [in] */ Boolean forceCache)
{
    assert(headers != NULL);

    if (!forceCache && mDisabled) {
        return NULL;
    }

    String databaseKey;
    GetDatabaseKey(url, postIdentifier, databaseKey);

    // according to the rfc 2616, the 303 response MUST NOT be cached.
    if (statusCode == 303) {
        // remove the saved cache if there is any
        mDataBase->RemoveCache(databaseKey);
        return NULL;
    }

    // like the other browsers, do not cache redirects containing a cookie
    // header.
    if (CheckCacheRedirect(statusCode)/* && !headers->GetSetCookie()->IsEmpty()*/) {
        // remove the saved cache if there is any
        mDataBase->RemoveCache(databaseKey);
        return NULL;
    }

    ICacheManagerCacheResult* ret = ParseHeaders(statusCode, headers, mimeType);
    if (ret == NULL) {
        // this should only happen if the headers has "no-store" in the
        // cache-control. remove the saved cache if there is any
        mDataBase->RemoveCache(databaseKey);
    } else {
        SetupFiles(databaseKey, ret);
        //try{
            IFileOutputStream* pFileOStream = NULL;
            CFileOutputStream::New(((CacheResult*)ret)->outFile, &pFileOStream);
            ((CacheResult*)ret)->outStream = pFileOStream;
        //} catch (FileNotFoundException e) {
            // This can happen with the system did a purge and our
            // subdirectory has gone, so lets try to create it again
        //   if (createCacheDirectory()) {
        //        try {
        //            ret.outStream = new FileOutputStream(ret.outFile);
        //        } catch  (FileNotFoundException e2) {
                    // We failed to create the file again, so there
                    // is something else wrong. Return null.
        //            return null;
        //        }
        //    } else {
                // Failed to create cache directory
        //        return null;
        //    }
        //}

        ((CacheResult*)ret)->mimeType = mimeType;
    }

    return ret;
}

CARAPI_(void) CCacheManager::SaveCacheFile(
    /* [in] */ const String& url,
    /* [in] */ Int64 postIdentifier,
    /* [in] */ CCacheManager::CacheResult* cacheRet)
{
    assert(cacheRet != NULL);

    //try {
        cacheRet->outStream->Close();
    //} catch (IOException e) {
    //    return;
    //}

    Boolean bExists = FALSE;
    cacheRet->outFile->Exists(&bExists);
    if (!bExists) {
        // the file in the cache directory can be removed by the system
        return;
    }

    Boolean redirect = CheckCacheRedirect(cacheRet->httpStatusCode);
    if (redirect) {
        // location is in database, no need to keep the file
        cacheRet->contentLength = 0;
        cacheRet->localPath = "";
    }

    Boolean bDelete = FALSE;
    cacheRet->outFile->Delete(&bDelete);
    if ((redirect || cacheRet->contentLength == 0) && !bDelete) {
        //Log.e(LOGTAG, cacheRet.outFile.getPath() + " delete failed.");
    }

    if (cacheRet->contentLength == 0) {
        return;
    }

    String str;
    GetDatabaseKey(url, postIdentifier, str);
    mDataBase->AddCache(str, (ICacheManagerCacheResult*)cacheRet);

    //if (DebugFlags.CACHE_MANAGER) {
    //    Log.v(LOGTAG, "saveCacheFile for url " + url);
    //}
}

CARAPI_(Boolean) CCacheManager::CleanupCacheFile(
    /* [in] */ ICacheManagerCacheResult* cacheRet)
{
    assert(cacheRet != NULL);

    Boolean bRet = FALSE;

    //try {
        ((CacheResult*)cacheRet)->outStream->Close();
    //} catch (IOException e) {
    //    return false;
    //}

    ((CacheResult*)cacheRet)->outFile->Delete(&bRet);

    return bRet;
}

/**
 * remove all cache files
 * 
 * @return true if it succeeds
 */
CARAPI_(Boolean) CCacheManager::RemoveAllCacheFiles()
{
    // Note, this is called before init() when the database is
    // created or upgraded.
    if (mBaseDir == NULL) {
        // Init() has not been called yet, so just flag that
        // we need to clear the cache when init() is called.
        mClearCacheOnInit = TRUE;
        return TRUE;
    }
    // delete rows in the cache database
//    WebViewWorker.getHandler().sendEmptyMessage(
//            WebViewWorker.MSG_CLEAR_CACHE);
    // delete cache files in a separate thread to not block UI.    
    class CRunnable : public Runnable
    {
    public:
        ECode Run() {
            // delete all cache files
            const Int32 size = 10; //not sure
            ArrayOf<String>* files = ArrayOf<String>::Alloc(size);
            mBaseDir->List(&files);
            // if mBaseDir doesn't exist, files can be null.
            for (Int32 i = 0; i < size; i++) {
                if ((*files)[i]) {
                    AutoPtr<IFile> f;
                    CFile::New(mBaseDir.Get(), (*files)[i], (IFile**)&f);
                    Boolean flag = false;
                    f->Delete(&flag);
                    if (!flag) {
//                        Log.e(LOGTAG, f.getPath() + " delete failed.");
                    }
                }
            }

            return NOERROR;
        }
    };

    CRunnable runable;

    AutoPtr<IThread> thread;
    CThread::New((IRunnable*)&runable, (IThread**)&thread);

    return TRUE;
}

/**
 * Return true if the cache is empty.
 */
CARAPI_(Boolean) CCacheManager::CacheEmpty()
{
    return mDataBase->HasCache();
}

CARAPI_(void) CCacheManager::TrimCacheIfNeeded()
{
    if (mDataBase->GetCacheTotalSize() > CACHE_THRESHOLD) {
        Vector<String> pathList;
        mDataBase->TrimCache(CACHE_TRIM_AMOUNT, pathList);
        Int32 size = pathList.GetSize();
        for (Int32 i = 0; i < size; i++) {
            AutoPtr<IFile> f;
            CFile::New(mBaseDir, pathList[i], (IFile**)&f);

            Boolean bDelete = FALSE;
            f->Delete(&bDelete);
            if (!bDelete) {
            //    Log.e(LOGTAG, f.getPath() + " delete failed.");
            }
        }
        // remove the unreferenced files in the cache directory
        Vector<String> fileList;
        mDataBase->GetAllCacheFileNames(fileList);
        if (fileList.GetSize() == 0) return;
     /*   String[] toDelete = mBaseDir.list(new FilenameFilter() {
            public boolean accept(File dir, String filename) {
                if (fileList.contains(filename)) {
                    return false;
                } else {
                    return true;
                }
            }
        });
        if (toDelete == NULL) return;
        size = toDelete.length;
        for (int i = 0; i < size; i++) {
            File f = new File(mBaseDir, toDelete[i]);
            if (!f.delete()) {
                Log.e(LOGTAG, f.getPath() + " delete failed.");
            }
        }*/
    }
}

CARAPI_(void) CCacheManager::ClearCache()
{
    // delete database
    mDataBase->ClearCache();
}


/**
 * Create the cache directory if it does not already exist.
 * 
 * @return true if the cache directory didn't exist and was created.
 */
CARAPI_(Boolean) CCacheManager::CreateCacheDirectory()
{
    Boolean bExists = FALSE;
    mBaseDir->Exists(&bExists);
    if (!bExists) {
        Boolean bMkDir = FALSE;
        mBaseDir->Mkdirs(&bMkDir);
        if(!bMkDir) {
            //Log.w(LOGTAG, "Unable to create webviewCache directory");
            return FALSE;
        }

        String str;
        mBaseDir->ToString(&str);
        FileUtils::SetPermissions(
                (const char*)str,
                FileUtils::IRWXU | FileUtils::IRWXG | FileUtils::IXOTH,
                -1, -1);
        // If we did create the directory, we need to flush 
        // the cache database. The directory could be recreated
        // because the system flushed all the data/cache directories
        // to free up disk space.
        // delete rows in the cache database
    /*    WebViewWorker.getHandler().sendEmptyMessage(
                WebViewWorker.MSG_CLEAR_CACHE);*/
        return TRUE;
    }
    return FALSE;
}

CARAPI_(Boolean) CCacheManager::CheckCacheRedirect(
    /* [in] */ Int32 statusCode)
{
    if (statusCode == 301 || statusCode == 302 || statusCode == 307) {
        // as 303 can't be cached, we do not return true
        return TRUE;
    } else {
        return FALSE;
    }
}

CARAPI_(void) CCacheManager::GetDatabaseKey(
    /* [in] */ const String& url,
    /* [in] */ Int64 postIdentifier,
    /* [out] */ String& str)
{
    if (postIdentifier == 0) str = url;
//    return /*postIdentifier +*/ url;
}

//@SuppressWarnings("deprecation")
CARAPI_(void) CCacheManager::SetupFiles(
    /* [in] */ const String& url,
    /* [in] */ ICacheManagerCacheResult* cacheRet)
{
    assert(cacheRet != NULL);

    if (TRUE) {
        // Note: SHA1 is much stronger hash. But the cost of setupFiles() is
        // 3.2% cpu time for a fresh load of nytimes.com. While a simple
        // String.hashCode() is only 0.6%. If adding the collision resolving
        // to String.hashCode(), it makes the cpu time to be 1.6% for a 
        // fresh load, but 5.3% for the worst case where all the files 
        // already exist in the file system, but database is gone. So it
        // needs to resolve collision for every file at least once.
        Int32 hashCode = (const_cast<String&>(url)).GetHashCode();
        StringBuffer* ret = new StringBuffer(8);
        AppendAsHex(hashCode, (StringBuffer*)ret);
        String path((const char*)(StringBuffer*)ret);//.toString();
        AutoPtr<IFile> file;
        CFile::New(mBaseDir, path, (IFile**)&file);
        if (TRUE) {
            Boolean checkOldPath = TRUE;
            // Check hash collision. If the hash file doesn't exist, just
            // continue. There is a chance that the old cache file is not
            // same as the hash file. As mDataBase.getCache() is more 
            // expansive than "leak" a file until clear cache, don't bother.
            // If the hash file exists, make sure that it is same as the 
            // cache file. If it is not, resolve the collision.
            Boolean bExists = FALSE;
            file->Exists(&bExists);
            while (bExists)
            {
                if (checkOldPath) {
                    CacheResult* oldResult = (CacheResult*)mDataBase->GetCache(url);
                    if (oldResult != NULL && oldResult->contentLength > 0) {
                        if (path.Equals(oldResult->localPath)) {
                            path = oldResult->localPath;
                        }
                        else {
                            path = oldResult->localPath;
                            if (file) delete file;
                            CFile::New(mBaseDir, path, (IFile**)&file);
                        }
                        break;
                    }
                    checkOldPath = false;
                }
                ret = new StringBuffer(8);
                AppendAsHex(++hashCode, ret);
                path = (const char*)(StringBuffer*)ret;//.toString();
                if (file) delete file;
                CFile::New(mBaseDir, path, (IFile**)&file);

                file->Exists(&bExists);
            }
        }
        ((CacheResult*)cacheRet)->localPath = path;
        ((CacheResult*)cacheRet)->outFile = file;
    } else {
        // get hash in byte[]
/*        Digest digest = new SHA1Digest();
        int digestLen = digest.getDigestSize();
        byte[] hash = new byte[digestLen];
        int urlLen = url.length();
        byte[] data = new byte[urlLen];
        url.getBytes(0, urlLen, data, 0);
        digest.update(data, 0, urlLen);
        digest.doFinal(hash, 0);
        // convert byte[] to hex String
        StringBuffer result = new StringBuffer(2 * digestLen);
        for (int i = 0; i < digestLen; i = i + 4)
        {
            int h = (0x00ff & hash[i]) << 24 | (0x00ff & hash[i + 1]) << 16
                    | (0x00ff & hash[i + 2]) << 8 | (0x00ff & hash[i + 3]);
            appendAsHex(h, result);
        }
        cacheRet.localPath = result.toString();
        cacheRet.outFile = new File(mBaseDir, cacheRet.localPath);*/
    }
}

CARAPI_(void) CCacheManager::AppendAsHex(
    /* [in] */ Int32 i,
    /* [in] */ StringBuffer* ret)
{
    assert(ret != NULL);

    String hex;// = Integer.toHexString(i);
    switch (hex.GetLength())
    {
        case 1:
            *ret += "0000000";
            break;
        case 2:
            *ret += "000000";
            break;
        case 3:
            *ret += "00000";
            break;
        case 4:
            *ret += "0000";
            break;
        case 5:
            *ret += "000";
            break;
        case 6:
            *ret += "00";
            break;
        case 7:
            *ret += "0";
            break;
    }

    *ret += hex;
}

CARAPI_(AutoPtr<ICacheManagerCacheResult>) CCacheManager::ParseHeaders(
    /* [in] */ Int32 statusCode,
    /* [in] */ IHeaders* headers,
    /* [in] */ const String& mimeType)
{
    assert(headers != NULL);

    // if the contentLength is already larger than CACHE_MAX_SIZE, skip it
//    if (headers->GetContentLength() > CACHE_MAX_SIZE) return NULL;

    // The HTML 5 spec, section 6.9.4, step 7.3 of the application cache
    // process states that HTTP caching rules are ignored for the
    // purposes of the application cache download process.
    // At this point we can't tell that if a file is part of this process,
    // except for the manifest, which has its own mimeType.
    // TODO: work out a way to distinguish all responses that are part of
    // the application download process and skip them.
    if (mimeType.Equals(MANIFEST_MIME)) return NULL;

    // TODO: if authenticated or secure, return null
    CacheResult* ret;// = new CacheResult();
    ret->httpStatusCode = statusCode;

    String location;// = headers.getLocation();
    if (location.GetSize() != 0) ret->location = location;

    ret->expires = -1;
//    ret.expiresString = headers.getExpires();
    if (ret->expiresString.GetSize() != 0) {
        //try {
//            ret.expires = AndroidHttpClient.parseDate(ret.expiresString);
        //} catch (IllegalArgumentException ex) {
            // Take care of the special "-1" and "0" cases
        //    if ("-1".equals(ret.expiresString)
        //            || "0".equals(ret.expiresString)) {
                // make it expired, but can be used for history navigation
        //        ret.expires = 0;
        //    } else {
        //        Log.e(LOGTAG, "illegal expires: " + ret.expiresString);
        //    }
        //}
    }

    String contentDisposition;// = headers.getContentDisposition();
    if (contentDisposition.GetSize() != 0) {
        ret->contentdisposition = contentDisposition;
    }

    String crossDomain;// = headers.getXPermittedCrossDomainPolicies();
    if (crossDomain.GetSize() != 0) {
        ret->crossDomain = crossDomain;
    }

    // lastModified and etag may be set back to http header. So they can't
    // be empty string.
    String lastModified;// = headers.getLastModified();
    if (lastModified.GetSize() != 0 && lastModified.GetLength() > 0) {
        ret->lastModified = lastModified;
    }

    String etag;// = headers.getEtag();
    if (etag.GetSize() != 0 && etag.GetLength() > 0) ret->etag = etag;

    String cacheControl;// = headers.getCacheControl();
    if (cacheControl.GetSize() != 0) {
        Vector<String> controls;// = cacheControl.toLowerCase().split("[ ,;]");
        for (Int32 i = 0; i < (Int32)controls.GetSize(); i++) {
            if (controls[i].Equals(NO_STORE)) {
                return NULL;
            }
            // According to the spec, 'no-cache' means that the content
            // must be re-validated on every load. It does not mean that
            // the content can not be cached. set to expire 0 means it
            // can only be used in CACHE_MODE_CACHE_ONLY case
            if (controls[i].Equals(NO_CACHE)) {
                ret->expires = 0;
            } else if (controls[i].StartWith(MAX_AGE)) {
                Int32 separator = controls[i].IndexOf('=');
                if (separator < 0) {
                    separator = controls[i].IndexOf(':');
                }

                if (separator > 0) {
                    String s = controls[i].Substring(separator + 1);
                    //try {
                        Int64 sec;// = Long.parseLong(s);
                        if (sec >= 0) {
                            ret->expires = System::GetCurrentTimeMillis() + 1000 * sec;
                        }
                    //} catch (NumberFormatException ex) {
                    //    if ("1d".equals(s)) {
                            // Take care of the special "1d" case
                    //        ret.expires = System.currentTimeMillis() + 86400000; // 24*60*60*1000
                    //    } else {
                    //        Log.e(LOGTAG, "exception in parseHeaders for "
                    //                + "max-age:"
                    //                + controls[i].substring(separator + 1));
                    //        ret.expires = 0;
                    //    }
                    //}
                }
            }
        }
    }

    // According to RFC 2616 section 14.32:
    // HTTP/1.1 caches SHOULD treat "Pragma: no-cache" as if the
    // client had sent "Cache-Control: no-cache"
    //if (NO_CACHE.equals(headers.GetPragma()))
    {
        ret->expires = 0;
    }

    // According to RFC 2616 section 13.2.4, if an expiration has not been
    // explicitly defined a heuristic to set an expiration may be used.
    if (ret->expires == -1) {
        if (ret->httpStatusCode == 301) {
            // If it is a permanent redirect, and it did not have an
            // explicit cache directive, then it never expires
            ret->expires;// = Long.MAX_VALUE;
        } else if (ret->httpStatusCode == 302 || ret->httpStatusCode == 307) {
            // If it is temporary redirect, expires
            ret->expires = 0;
        } else if (ret->lastModified.GetSize() != 0) {
            // When we have no last-modified, then expire the content with
            // in 24hrs as, according to the RFC, longer time requires a
            // warning 113 to be added to the response.

            // Only add the default expiration for non-html markup. Some
            // sites like news.google.com have no cache directives.
            if (!mimeType.StartWith("text/html")) {
                ret->expires = System::GetCurrentTimeMillis() + 86400000; // 24*60*60*1000
            } else {
                // Setting a expires as zero will cache the result for
                // forward/back nav.
                ret->expires = 0;
            }
        } else {
            // If we have a last-modified value, we could use it to set the
            // expiration. Suggestion from RFC is 10% of time since
            // last-modified. As we are on mobile, loads are expensive,
            // increasing this to 20%.

            // 24 * 60 * 60 * 1000
            Int64 lastmod = System::GetCurrentTimeMillis() + 86400000;
            //try {
//                lastmod = AndroidHttpClient.parseDate(ret.lastModified);
            //} catch (IllegalArgumentException ex) {
            //    Log.e(LOGTAG, "illegal lastModified: " + ret.lastModified);
            //}
            Int64 difference = System::GetCurrentTimeMillis() - lastmod;
            if (difference > 0) {
                ret->expires = System::GetCurrentTimeMillis() + difference / 5;
            } else {
                // last modified is in the future, expire the content
                // on the last modified
                ret->expires = lastmod;
            }
        }
    }

    return ret;
}


/****************************CacheResult******************************/
inline CARAPI_(Int32) CCacheManager::CacheResult::GetHttpStatusCode() const
{
    return httpStatusCode;
}

inline CARAPI_(Int64) CCacheManager::CacheResult::GetContentLength() const
{
    return contentLength;
}

inline CARAPI_(void) CCacheManager::CacheResult::GetLocalPath(
    /* [out] */ String& str) const
{
    str = localPath;
}

inline CARAPI_(Int64) CCacheManager::CacheResult::GetExpires() const
{
    return expires;
}

inline CARAPI_(void) CCacheManager::CacheResult::GetExpiresString(
    /* [out] */ String& str) const
{
    str = expiresString;
}

inline CARAPI_(void) CCacheManager::CacheResult::GetLastModified(
    /* [out] */ String& str) const
{
    str = lastModified;
}

inline CARAPI_(void) CCacheManager::CacheResult::GetETag(
    /* [out] */ String& str) const
{
    str = etag;
}

inline CARAPI_(void) CCacheManager::CacheResult::GetMimeType(
    /* [out] */ String& str) const
{
    str = mimeType;
}

inline CARAPI_(void) CCacheManager::CacheResult::GetLocation(
    /* [out] */ String& str) const
{
    str = location;
}

inline CARAPI_(void) CCacheManager::CacheResult::GetEncoding(
    /* [out] */ String& str) const
{
    str = encoding;
}

inline CARAPI_(void) CCacheManager::CacheResult::GetContentDisposition(
    /* [out] */ String& str) const
{
    str = contentdisposition;
}

// For out-of-package access to the underlying streams.
inline CARAPI_(AutoPtr<IFileInputStream>) CCacheManager::CacheResult::GetInputStream() const
{
    return inStream;
}

inline CARAPI_(AutoPtr<IFileOutputStream>) CCacheManager::CacheResult::GetOutputStream() const
{
    return outStream;
}

// These fields can be set manually.
inline CARAPI CCacheManager::CacheResult::SetInputStream(IFileInputStream* stream)
{
    if (stream == NULL)
    {
        return E_INVALID_ARGUMENT;
    }

    inStream = stream;
}

inline CARAPI_(void) CCacheManager::CacheResult::SetEncoding(String encoding)
{
    encoding = encoding;
}

PInterface CCacheManager::CacheResult::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_ICacheManagerCacheResult) {
        return (IRunnable*)this;
    }

    return NULL;
}

UInt32 CCacheManager::CacheResult::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCacheManager::CacheResult::Release()
{
    return ElRefBase::Release();
}

ECode CCacheManager::CacheResult::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* IID)
{
    VALIDATE_NOT_NULL(IID);

    if (object == (IInterface*)(ICacheManagerCacheResult*)this) {
        *IID = EIID_ICacheManagerCacheResult;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}
