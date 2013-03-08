
#ifndef __CURLINTERCEPTREGISTRY_H__
#define __CURLINTERCEPTREGISTRY_H__

#include "_CUrlInterceptRegistry.h"

#include "webkit/IUrlInterceptHandler.h"

#include "ext/frameworkext.h"
#include <elastos.h>
#include <elastos/Mutex.h>
#include <elastos/List.h>



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
    /*static*/ CARAPI GetHandlers(
        /* [out] */ List< IUrlInterceptHandler* > ** pLinkedList );

private:
    const static char* LOGTAG; // = "intercept";

    static Boolean sDisabled;   // = false;

    static List< IUrlInterceptHandler* > * sHandlerList;
};

#endif // __CURLINTERCEPTREGISTRY_H__
