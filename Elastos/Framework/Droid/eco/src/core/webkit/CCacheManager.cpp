
#include "webkit/CCacheManager.h"
ECode CCacheManager::Init(
    /* [in] */ IContext * pContext)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCacheManager::GetCacheFileBaseDir(
    /* [out] */ IFile ** ppDir)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCacheManager::CacheDisabled(
    /* [out] */ Boolean * pDisabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCacheManager::StartCacheTransaction(
    /* [out] */ Boolean * pFlag)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCacheManager::EndCacheTransaction(
    /* [out] */ Boolean * pFlag)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCacheManager::GetCacheFile(
    /* [out] */ ICacheManagerCacheResult ** ppResult,
    /* [in] */ CString url,
    /* [in] */ IObjectStringMap * pHeaders)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCacheManager::GetCacheFileEx(
    /* [out] */ ICacheManagerCacheResult ** ppResult,
    /* [in] */ CString url,
    /* [in] */ Int64 postIdentifier,
    /* [in] */ IObjectStringMap * pHeaders)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCacheManager::CreateCacheFile(
    /* [out] */ ICacheManagerCacheResult ** ppResult,
    /* [in] */ CString url,
    /* [in] */ Int32 statusCode,
    /* [in] */ IHeaders * pHeaders,
    /* [in] */ CString mimeType,
    /* [in] */ Boolean forceCache)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCacheManager::SaveCacheFile(
    /* [in] */ CString url,
    /* [in] */ ICacheManagerCacheResult * pCacheRet)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

