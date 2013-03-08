
#include "webkit/CUrlInterceptRegistry.h"
#include "webkit/CCacheManager.h"
#include "webkit/CPluginData.h"

const char* CUrlInterceptRegistry::LOGTAG = "intercept";

Boolean CUrlInterceptRegistry::sDisabled = FALSE;

List< IUrlInterceptHandler * > * CUrlInterceptRegistry::sHandlerList;


ECode CUrlInterceptRegistry::GetHandlers(
        /* [out] */ List< IUrlInterceptHandler * > ** pLinkedList )
{
    Mutex::Autolock lock(_m_syncLock);    

    if(sHandlerList == NULL)
    {
        sHandlerList = new List< IUrlInterceptHandler * >;
    }
    *pLinkedList = sHandlerList;
    return NOERROR;
}

ECode CUrlInterceptRegistry::SetUrlInterceptDisabled(
    /* [in] */ Boolean disabled)
{
    sDisabled = disabled;
    return NOERROR;
}

ECode CUrlInterceptRegistry::UrlInterceptDisabled(
    /* [out] */ Boolean * pFlag)
{
    *pFlag = sDisabled;
    return NOERROR;
}

ECode CUrlInterceptRegistry::RegisterHandler(
    /* [in] */ IUrlInterceptHandler * pHandler,
    /* [out] */ Boolean * pFlag)
{
    List< IUrlInterceptHandler * > * tHandlerList = NULL;
    GetHandlers(&tHandlerList);

    Boolean bContains = FALSE;
    Int32 nT = tHandlerList -> GetSize();
    List< IUrlInterceptHandler * >::Iterator iterT;
    List< IUrlInterceptHandler * >::Iterator iterE;
    iterT = tHandlerList -> Begin();
    iterE = tHandlerList -> End();
    for(int n = 0; n < nT ; n ++ )
    {
        if(pHandler == (*iterT) )
        {
            bContains = TRUE;
            break;
        }            
        iterT ++;
    }
    if(!bContains)
    {
        pHandler -> AddRef();
        tHandlerList -> PushFront(pHandler);
        *pFlag = TRUE;
    }
    else
    {
        *pFlag = FALSE;
    }
    return NOERROR;
}

ECode CUrlInterceptRegistry::UnregisterHandler(
    /* [in] */ IUrlInterceptHandler * pHandler,
    /* [out] */ Boolean * pFlag)
{
    List< IUrlInterceptHandler * > * tHandlerList = NULL;
    GetHandlers(&tHandlerList);
    pHandler ->Release();
    tHandlerList -> Remove(pHandler);
    return NOERROR;
}

ECode CUrlInterceptRegistry::GetSurrogate(
    /* [in] */ CString url,
    /* [in] */ IObjectStringMap * pHeaders,
    /* [out] */ ICacheManagerCacheResult ** ppResult)
{
    Boolean bUrlInterceptDisabled;
    UrlInterceptDisabled(&bUrlInterceptDisabled);
    if(bUrlInterceptDisabled)
    {
        *ppResult = NULL;
        return NOERROR;
    }

    List< IUrlInterceptHandler * > * tHandlerList = NULL;
    GetHandlers(&tHandlerList);

    Int32 nT = tHandlerList -> GetSize();
    List< IUrlInterceptHandler * >::Iterator iterT;
    List< IUrlInterceptHandler * >::Iterator iterE;
    iterT = tHandlerList -> Begin();
    iterE = tHandlerList -> End();
    for(int n = 0; n < nT ; n ++ )
    {
        /*
        IUrlInterceptHandler * handler = (IUrlInterceptHandler * ) (*iterT);        
        CCacheManager::CCacheResult * result = NULL;
        handler -> Service(url,pHeaders,&result);
        if(result != NULL)
        {
            *ppResult = result;
            break;
        }
        */
        iterT ++;
    }
    return NOERROR;
}

ECode CUrlInterceptRegistry::GetPluginData(
    /* [in] */ CString url,
    /* [in] */ IObjectStringMap * pHeaders,
    /* [out] */ IPluginData ** ppData)
{
    Boolean bUrlInterceptDisabled;
    UrlInterceptDisabled(&bUrlInterceptDisabled);
    if(bUrlInterceptDisabled)
    {
        *ppData = NULL;
        return NOERROR;
    }

    List< IUrlInterceptHandler * > * tHandlerList = NULL;
    GetHandlers(&tHandlerList);

    Int32 nT = tHandlerList -> GetSize();
    List< IUrlInterceptHandler * >::Iterator iterT;
    List< IUrlInterceptHandler * >::Iterator iterE;
    iterT = tHandlerList -> Begin();
    iterE = tHandlerList -> End();
    for(int n = 0; n < nT ; n ++ )
    {
        IUrlInterceptHandler * handler = (IUrlInterceptHandler * ) (*iterT);  
        IPluginData * data = NULL;
        handler -> GetPluginData(url, pHeaders,&data);
        if (data != NULL) 
        {
            *ppData = data;
            break;
        }        
        iterT ++;
    }
    return NOERROR;
}

