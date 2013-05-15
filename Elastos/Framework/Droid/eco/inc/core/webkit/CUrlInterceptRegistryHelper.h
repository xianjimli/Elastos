
#ifndef __CURLINTERCEPTREGISTRYHELPER_H__
#define __CURLINTERCEPTREGISTRYHELPER_H__

#include "_CUrlInterceptRegistryHelper.h"

CarClass(CUrlInterceptRegistryHelper)
{
public:
    CARAPI SetUrlInterceptDisabled(
        /* [in] */ Boolean disabled);

    CARAPI UrlInterceptDisabled(
        /* [out] */ Boolean * flag);

    CARAPI RegisterHandler(
        /* [in] */ IUrlInterceptHandler * handler,
        /* [out] */ Boolean * flag);

    CARAPI UnregisterHandler(
        /* [in] */ IUrlInterceptHandler * handler,
        /* [out] */ Boolean * flag);

    CARAPI GetSurrogate(
        /* [in] */ CString url,
        /* [in] */ IObjectStringMap * headers,
        /* [out] */ ICacheManagerCacheResult ** result);

    CARAPI GetPluginData(
        /* [in] */ CString url,
        /* [in] */ IObjectStringMap * headers,
        /* [out] */ IPluginData ** data);

private:
    // TODO: Add your private member variables here.
};

#endif // __CURLINTERCEPTREGISTRYHELPER_H__
