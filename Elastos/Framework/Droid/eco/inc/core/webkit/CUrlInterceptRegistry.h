
#ifndef __CURLINTERCEPTREGISTRY_H__
#define __CURLINTERCEPTREGISTRY_H__

#include "_CUrlInterceptRegistry.h"

#include "webkit/IUrlInterceptHandler.h"

#include "ext/frameworkext.h"
#include <elastos.h>
#include <elastos/Mutex.h>
#include <elastos/List.h>
#include <elastos/AutoPtr.h>



CarClass(CUrlInterceptRegistry)
{
public:
    /*static*/ 
    CARAPI SetUrlInterceptDisabled(
        /* [in] */ Boolean disabled);

    /*static*/ 
    CARAPI UrlInterceptDisabled(
        /* [out] */ Boolean * flag);

    /*static*/ 
    CARAPI RegisterHandler(
        /* [in] */ IUrlInterceptHandler * handler,
        /* [out] */ Boolean *flag);

    /*static*/ 
    CARAPI UnregisterHandler(
        /* [in] */ IUrlInterceptHandler * handler,
        /* [out] */ Boolean * flag);

    /*static*/ 
    CARAPI GetSurrogate(
        /* [in] */ CString url,
        /* [in] */ IObjectStringMap * headers,
        /* [out] */ ICacheManagerCacheResult ** result);

    /*static*/ 
    CARAPI GetPluginData(
        /* [in] */ CString url,
        /* [in] */ IObjectStringMap * headers,
        /* [out] */ IPluginData ** data);

private:
    /*static*/ CARAPI GetHandlers(
        /* [out] */ List< AutoPtr<IUrlInterceptHandler> > ** linkedList );

private:
    const static CString LOGTAG; // = "intercept";

    static Boolean sDisabled;   // = false;

    static List< AutoPtr<IUrlInterceptHandler> > * sHandlerList;

    /*static*/ Core::Threading::Mutex mMutex;
};

#endif // __CURLINTERCEPTREGISTRY_H__
