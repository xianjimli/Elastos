
#include "webkit/CUrlInterceptRegistry.h"
#include "webkit/CCacheManager.h"
#include "webkit/CPluginData.h"

const CString CUrlInterceptRegistry::LOGTAG = "intercept";

Boolean CUrlInterceptRegistry::sDisabled = FALSE;
Core::Threading::Mutex CUrlInterceptRegistry::mMutexClass;

List< AutoPtr<IUrlInterceptHandler> > * CUrlInterceptRegistry::sHandlerList;


ECode CUrlInterceptRegistry::GetHandlers(
        /* [out] */ List< AutoPtr<IUrlInterceptHandler> > ** linkedList )
{    
    VALIDATE_NOT_NULL(linkedList);
    Mutex::Autolock lock(mMutexClass);
    if(sHandlerList == NULL) {
        sHandlerList = new List< AutoPtr<IUrlInterceptHandler> >;
    }
    *linkedList = sHandlerList;
    return NOERROR;
}

ECode CUrlInterceptRegistry::SetUrlInterceptDisabled(
    /* [in] */ Boolean disabled)
{
    Mutex::Autolock lock(mMutexClass);
    sDisabled = disabled;
    return NOERROR;
}

Boolean CUrlInterceptRegistry::UrlInterceptDisabled()
{
    Boolean flag;
    Mutex::Autolock lock(mMutexClass);
    flag = sDisabled;
    return flag;
}

Boolean CUrlInterceptRegistry::RegisterHandler(
    /* [in] */ IUrlInterceptHandler * handler)
{
    Boolean flag;
    Mutex::Autolock lock(mMutexClass);
    List< AutoPtr<IUrlInterceptHandler> > * tHandlerList = NULL;
    GetHandlers(&tHandlerList);

    Boolean bContains = FALSE;
    Int32 nT = tHandlerList -> GetSize();
    List< AutoPtr<IUrlInterceptHandler> >::Iterator iterT;
    List< AutoPtr<IUrlInterceptHandler> >::Iterator iterE;
    iterT = tHandlerList -> Begin();
    iterE = tHandlerList -> End();
    for(int n = 0; n < nT ; n ++ ) {
        if(handler == (*iterT) ) {
            bContains = TRUE;
            break;
        }            
        iterT ++;
    }
    if(!bContains) {
        tHandlerList -> PushFront(handler);
        flag = TRUE;
    }
    else {
        flag = FALSE;
    }
    return flag;
}

Boolean CUrlInterceptRegistry::UnregisterHandler(
    /* [in] */ IUrlInterceptHandler * handler)
{
    Boolean flag;
    Mutex::Autolock lock(mMutexClass);
    List< AutoPtr<IUrlInterceptHandler> > * tHandlerList = NULL;
    GetHandlers(&tHandlerList);
    tHandlerList -> Remove(handler);
    flag = TRUE;
    return flag;
}

ECode CUrlInterceptRegistry::GetSurrogate(
    /* [in] */ CString url,
    /* [in] */ IObjectStringMap * headers,
    /* [out] */ ICacheManagerCacheResult ** result)
{
    VALIDATE_NOT_NULL(result);
    Mutex::Autolock lock(mMutexClass);
    Boolean bUrlInterceptDisabled;
    bUrlInterceptDisabled = UrlInterceptDisabled();
    if(bUrlInterceptDisabled) {
        *result = NULL;
        return NOERROR;
    }

    List< AutoPtr<IUrlInterceptHandler> > * tHandlerList = NULL;
    GetHandlers(&tHandlerList);

    Int32 nT = tHandlerList -> GetSize();
    List< AutoPtr<IUrlInterceptHandler> >::Iterator iterT;
    List< AutoPtr<IUrlInterceptHandler> >::Iterator iterE;
    iterT = tHandlerList -> Begin();
    iterE = tHandlerList -> End();
    for(int n = 0; n < nT ; n ++ ) { 
        IUrlInterceptHandler * handler = (IUrlInterceptHandler * ) (*iterT).Get();        
        AutoPtr<ICacheManagerCacheResult> resultT;
        handler -> Service(url, headers, (ICacheManagerCacheResult**)&resultT);
        if(resultT.Get() != NULL) {
            *result = resultT.Get();
            break;
        }        
        iterT ++;
    }
    return NOERROR;
}

ECode CUrlInterceptRegistry::GetPluginData(
    /* [in] */ CString url,
    /* [in] */ IObjectStringMap * headers,
    /* [out] */ IPluginData ** data)
{
    VALIDATE_NOT_NULL(data);
    Mutex::Autolock lock(mMutexClass);
    Boolean bUrlInterceptDisabled;
    bUrlInterceptDisabled = UrlInterceptDisabled();
    if(bUrlInterceptDisabled) {
        *data = NULL;
        return NOERROR;
    }

    List< AutoPtr<IUrlInterceptHandler> > * tHandlerList = NULL;
    GetHandlers(&tHandlerList);

    Int32 nT = tHandlerList -> GetSize();
    List< AutoPtr<IUrlInterceptHandler> >::Iterator iterT;
    List< AutoPtr<IUrlInterceptHandler> >::Iterator iterE;
    iterT = tHandlerList -> Begin();
    iterE = tHandlerList -> End();
    for(int n = 0; n < nT ; n ++ ) {
        IUrlInterceptHandler * handler = (IUrlInterceptHandler * ) (*iterT);  
        AutoPtr<IPluginData> dataT = NULL;
        handler -> GetPluginData(url, headers,(IPluginData**)&dataT);
        if (dataT.Get() != NULL) {
            *data = dataT.Get();
            break;
        }        
        iterT ++;
    }
    return NOERROR;
}

