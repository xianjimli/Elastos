
#include "webkit/CUrlInterceptRegistryHelper.h"
#include "webkit/CUrlInterceptRegistry.h"

ECode CUrlInterceptRegistryHelper::SetUrlInterceptDisabled(
    /* [in] */ Boolean disabled)
{
    CUrlInterceptRegistry::SetUrlInterceptDisabled(disabled);
    return NOERROR;
}

ECode CUrlInterceptRegistryHelper::UrlInterceptDisabled(
    /* [out] */ Boolean * flag)
{
    VALIDATE_NOT_NULL(flag);

    *flag = CUrlInterceptRegistry::UrlInterceptDisabled();
    return NOERROR;
}

ECode CUrlInterceptRegistryHelper::RegisterHandler(
    /* [in] */ IUrlInterceptHandler * handler,
    /* [out] */ Boolean * flag)
{
    VALIDATE_NOT_NULL(flag);

    *flag = CUrlInterceptRegistry::RegisterHandler(handler);
    return NOERROR;
}

ECode CUrlInterceptRegistryHelper::UnregisterHandler(
    /* [in] */ IUrlInterceptHandler * handler,
    /* [out] */ Boolean * flag)
{
    VALIDATE_NOT_NULL(flag);

    *flag = CUrlInterceptRegistry::UnregisterHandler(handler);
    return NOERROR;
}

ECode CUrlInterceptRegistryHelper::GetSurrogate(
    /* [in] */ CString url,
    /* [in] */ IObjectStringMap * headers,
    /* [out] */ ICacheManagerCacheResult ** result)
{
    return CUrlInterceptRegistry::GetSurrogate(url, headers, result);
}

ECode CUrlInterceptRegistryHelper::GetPluginData(
    /* [in] */ CString url,
    /* [in] */ IObjectStringMap * headers,
    /* [out] */ IPluginData ** data)
{
    return CUrlInterceptRegistry::GetPluginData(url, headers, data);
}

