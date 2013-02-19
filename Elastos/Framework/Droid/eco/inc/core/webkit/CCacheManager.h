
#ifndef __CCACHEMANAGER_H__
#define __CCACHEMANAGER_H__

#include "_CCacheManager.h"

CarClass(CCacheManager)
{
public:
    CARAPI Init(
        /* [in] */ IContext * pContext);

    CARAPI GetCacheFileBaseDir(
        /* [out] */ IFile ** ppDir);

    CARAPI CacheDisabled(
        /* [out] */ Boolean * pDisabled);

    CARAPI StartCacheTransaction(
        /* [out] */ Boolean * pFlag);

    CARAPI EndCacheTransaction(
        /* [out] */ Boolean * pFlag);

    CARAPI GetCacheFile(
        /* [in] */ CString url,
        /* [in] */ IObjectStringMap * pHeaders,
        /* [out] */ ICacheManagerCacheResult ** ppResult);

    CARAPI GetCacheFileEx(
        /* [in] */ CString url,
        /* [in] */ Int64 postIdentifier,
        /* [in] */ IObjectStringMap * pHeaders,
        /* [out] */ ICacheManagerCacheResult ** ppResult);

    CARAPI CreateCacheFile(
        /* [in] */ CString url,
        /* [in] */ Int32 statusCode,
        /* [in] */ IHeaders * pHeaders,
        /* [in] */ CString mimeType,
        /* [in] */ Boolean forceCache,
        /* [out] */ ICacheManagerCacheResult ** ppResult);

    CARAPI SaveCacheFile(
        /* [in] */ CString url,
        /* [in] */ ICacheManagerCacheResult * pCacheRet);

private:
    // TODO: Add your private member variables here.
};

#endif // __CCACHEMANAGER_H__
