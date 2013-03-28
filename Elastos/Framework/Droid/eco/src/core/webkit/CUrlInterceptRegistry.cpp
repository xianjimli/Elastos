
#include "webkit/CUrlInterceptRegistry.h"
#include "webkit/CCacheManager.h"
#include "webkit/CPluginData.h"

const CString CUrlInterceptRegistry::LOGTAG = "intercept";

Boolean CUrlInterceptRegistry::sDisabled = FALSE;

List< AutoPtr<IUrlInterceptHandler> > * CUrlInterceptRegistry::sHandlerList;


ECode CUrlInterceptRegistry::GetHandlers(
        /* [out] */ List< AutoPtr<IUrlInterceptHandler> > ** linkedList )
{    
    VALIDATE_NOT_NULL(linkedList);
    Mutex::Autolock lock(mMutex);
    if(sHandlerList == NULL) {
        sHandlerList = new List< AutoPtr<IUrlInterceptHandler> >;
    }
    *linkedList = sHandlerList;
    return NOERROR;
}

ECode CUrlInterceptRegistry::SetUrlInterceptDisabled(
    /* [in] */ Boolean disabled)
{
    Mutex::Autolock lock(mMutex);
    sDisabled = disabled;
    return NOERROR;
}

ECode CUrlInterceptRegistry::UrlInterceptDisabled(
    /* [out] */ Boolean* flag)
{
    VALIDATE_NOT_NULL(flag);
    Mutex::Autolock lock(mMutex);
    *flag = sDisabled;
    return NOERROR;
}

ECode CUrlInterceptRegistry::RegisterHandler(
    /* [in] */ IUrlInterceptHandler * handler,
    /* [out] */ Boolean * flag)
{
    VALIDATE_NOT_NULL(flag);
    Mutex::Autolock lock(mMutex);
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
        *flag = TRUE;
    }
    else {
        *flag = FALSE;
    }
    return NOERROR;
}

ECode CUrlInterceptRegistry::UnregisterHandler(
    /* [in] */ IUrlInterceptHandler * handler,
    /* [out] */ Boolean * flag)
{
    VALIDATE_NOT_NULL(flag);
    Mutex::Autolock lock(mMutex);
    List< AutoPtr<IUrlInterceptHandler> > * tHandlerList = NULL;
    GetHandlers(&tHandlerList);
    tHandlerList -> Remove(handler);
    *flag = TRUE;
    return NOERROR;
}

ECode CUrlInterceptRegistry::GetSurrogate(
    /* [in] */ CString url,
    /* [in] */ IObjectStringMap * headers,
    /* [out] */ ICacheManagerCacheResult ** result)
{
    VALIDATE_NOT_NULL(result);
    Mutex::Autolock lock(mMutex);
    Boolean bUrlInterceptDisabled;
    UrlInterceptDisabled(&bUrlInterceptDisabled);
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
        /*
        IUrlInterceptHandler * handler = (IUrlInterceptHandler * ) (*iterT).Get();        
        AutoPtr<CCacheManager::CCacheResult> resultT;
        handler -> Service(url,headers,((CCacheManager::CCacheResult)**)&resultT);
        if(resultT.Get() != NULL) {
            *result = resultT.Get();
            break;
        }
        */
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
    Mutex::Autolock lock(mMutex);
    Boolean bUrlInterceptDisabled;
    UrlInterceptDisabled(&bUrlInterceptDisabled);
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

