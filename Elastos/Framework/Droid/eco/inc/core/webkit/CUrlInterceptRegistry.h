
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
        /* [in] */ IUrlInterceptHandler * pHandler,
        /* [out] */ Boolean * pFlag);

    CARAPI UnregisterHandler(
        /* [in] */ IUrlInterceptHandler * pHandler,
        /* [out] */ Boolean * pFlag);

    CARAPI GetSurrogate(
        /* [in] */ CString url,
        /* [in] */ IObjectStringMap * pHeaders,
        /* [out] */ ICacheManagerCacheResult ** ppResult);

    CARAPI GetPluginData(
        /* [in] */ CString url,
        /* [in] */ IObjectStringMap * pHeaders,
        /* [out] */ IPluginData ** ppData);

private:
    // TODO: Add your private member variables here.
};

#endif // __CURLINTERCEPTREGISTRY_H__
