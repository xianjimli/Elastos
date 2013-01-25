
#ifndef __CURLINTERCEPTREGISTRY_H__
#define __CURLINTERCEPTREGISTRY_H__

#include "_CUrlInterceptRegistry.h"

CarClass(CUrlInterceptRegistry)
{
public:
    CARAPI SetUrlInterceptDisabled(
        /* [in] */ Boolean disabled);

    CARAPI UrlInterceptDisabled(
        /* [out] */ Boolean * pFlag);

    CARAPI RegisterHandler(
        /* [out] */ Boolean * pFlag,
        /* [in] */ IUrlInterceptHandler * pHandler);

    CARAPI UnregisterHandler(
        /* [out] */ Boolean * pFlag,
        /* [in] */ IUrlInterceptHandler * pHandler);

    CARAPI GetSurrogate(
        /* [out] */ ICacheManagerCacheResult ** ppResult,
        /* [in] */ CString url,
        /* [in] */ IObjectStringMap * pHeaders);

    CARAPI GetPluginData(
        /* [out] */ IPluginData ** ppData,
        /* [in] */ CString url,
        /* [in] */ IObjectStringMap * pHeaders);

private:
    // TODO: Add your private member variables here.
};

#endif // __CURLINTERCEPTREGISTRY_H__
