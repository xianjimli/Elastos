
#include "webkit/CSslErrorHandler.h"

#include <elastos/Mutex.h>
#include "os/CBundle.h"
#include "webkit/DebugFlags.h"
#include "webkit/CBrowserFrame.h"

#include <Logger.h>

const CString CSslErrorHandler::LOGTAG = "network";
const Int32 CSslErrorHandler::HANDLE_RESPONSE = 100;

ECode CSslErrorHandler::HandleMessage(
    /* [in] */ IMessage * msg)
{
    switch(/*msg -> mWhat*/NULL)  {
        case HANDLE_RESPONSE:
            AutoPtr< LoadListener > loader = (LoadListener *) (/*msg -> mObj*/NULL);
            if(TRUE) {
                Mutex::Autolock lock(_m_syncLock);

                HandleSslErrorResponse(loader.Get(), loader -> SslError(), (/*msg -> mArg1*/1) == 1);
                mLoaderQueue -> Remove(loader);
                FastProcessQueuedSslErrors();
            }
        break;
    }
    return NOERROR;
}

ECode CSslErrorHandler::constructor()
{
    mLoaderQueue = new List< AutoPtr< LoadListener > >;
    AutoPtr<IBundle> pBundle;
    ECode ec = CBundle::New( (IBundle**)&pBundle );
    if(FAILED(ec)) {
        //return ec;
    }
    mSslPrefTable = pBundle.Get();

    // These are used by client facing SslErrorHandlers.
    mOriginHandler = NULL;
    mLoadListener = NULL;
    return NOERROR;
}

ECode CSslErrorHandler::constructor(
        /* [in] */ ISslErrorHandler * origin, 
        /* [in] */ LoadListener * listener)
{
    mOriginHandler = origin;
    mLoadListener = listener;
    return NOERROR;
}

ECode CSslErrorHandler::SaveState(
        /* [in] */ IBundle * outState,
        /* [out] */ Boolean * ret)
{
    VALIDATE_NOT_NULL(ret);
    Mutex::Autolock lock(_m_syncLock);
    Boolean success = ( outState != NULL );
    if(success)
    {// TODO?
        outState -> PutBundle( String("ssl-error-handler"),mSslPrefTable.Get() );
    }
    *ret = success;
    return NOERROR;
}

ECode CSslErrorHandler::RestoreState(
        /* [in] */ IBundle * inState,
        /* [out] */ Boolean * ret)
{
    VALIDATE_NOT_NULL(ret);
    Mutex::Autolock lock(_m_syncLock);
    Boolean success = (inState != NULL);
    if (success)  {
        ECode ec = inState -> ContainsKey(String("ssl-error-handler"),&success);
        if(FAILED(ec)) {//return ec;
            *ret = FALSE;
            return ec;
        }
        if (success)  {
            AutoPtr<IBundle> pBundle;
            ec = inState -> GetBundle(String("ssl-error-handler"),(IBundle**)&pBundle);
            if(FAILED(ec))
            {//return ec;
                *ret = FALSE;
                return ec;
            }
            mSslPrefTable = pBundle;
        }
    }
    *ret = success;
    return NOERROR;
}

ECode CSslErrorHandler::Clear()
{
    Mutex::Autolock lock(_m_syncLock);    
    mSslPrefTable -> Clear();
    return NOERROR;
}

ECode CSslErrorHandler::HandleSslErrorRequest(
        /* [in] */ LoadListener * loader)
{
    Mutex::Autolock lock(_m_syncLock);
    if (DebugFlags::sSSL_ERROR_HANDLER)  {
        String strUrl;
        loader -> Url(/*&*/strUrl);
        Utility::Logging::Logger::V(LOGTAG, String("SslErrorHandler.handleSslErrorRequest(): ") 
            + String("url=") + strUrl + "\n");
    }

    if ( !( loader -> Cancelled() ) )  {
        mLoaderQueue -> PushBack(loader);
        if ( loader == (mLoaderQueue -> GetFront()).Get() )  {
            FastProcessQueuedSslErrors();
        }
    }
    return NOERROR;
}

ECode CSslErrorHandler::CheckSslPrefTable(
         /* [in] */ LoadListener * loader,
        /* [in] */ ISslError * error,
        /* [out] */ Boolean * ret)
{
    VALIDATE_NOT_NULL(ret);
    Mutex::Autolock lock(_m_syncLock);
    ECode ec;
    /*const*/ String host;
    loader -> Host(/*&*/host);
    /*const*/ Int32 primary = 0;
    //ec = error -> GetPrimaryError(&primary);

    if (DebugFlags::sSSL_ERROR_HANDLER)  {
        //JAVA:Assert.assertTrue(host != null && primary != 0);
        assert(!( host.IsNullOrEmpty() ) && primary != 0 );
    }
    Boolean bContainsKey;
    ec = mSslPrefTable -> ContainsKey(host,&bContainsKey);
    if (bContainsKey)  {
        Int32 nBundleInt;
        ec = mSslPrefTable -> GetInt32(host,&nBundleInt);
        if (primary <= nBundleInt)  {
            HandleSslErrorResponse(loader, error, TRUE);
            *ret = TRUE;
            return NOERROR;
        }
    }
    *ret = FALSE;
    return NOERROR;
}

ECode CSslErrorHandler::FastProcessQueuedSslErrors()
{
    Boolean bContinue;
    while(bContinue)
    {
        ProcessNextLoader(&bContinue);
    }
    return NOERROR;
}

ECode CSslErrorHandler::ProcessNextLoader(
        /* [out] */ Boolean * ret)
{
    VALIDATE_NOT_NULL(ret);
    Mutex::Autolock lock(_m_syncLock);
    AutoPtr<LoadListener> loader = (mLoaderQueue -> GetFront());
    if (loader != NULL) 
    {
        // if this loader has been cancelled
        if (loader -> Cancelled()) 
        {
            // go to the following loader in the queue. Make sure this
            // loader has been removed from the queue.
            mLoaderQueue -> Remove(loader);
            *ret = TRUE;
            return NOERROR;
        }

        AutoPtr<ISslError> error = loader -> SslError();

        if (DebugFlags::sSSL_ERROR_HANDLER) 
        {
            //JAVA:Assert.assertNotNull(error);
            assert(error.Get() != NULL);        
        }

        // checkSslPrefTable will handle the ssl error response if the
        // answer is available. It does not remove the loader from the
        // queue.
        Boolean bCheckSslPrefTable = FALSE;
        CheckSslPrefTable(loader.Get(), error.Get(), &bCheckSslPrefTable);
        if (bCheckSslPrefTable) 
        {
            mLoaderQueue -> Remove(loader);
            *ret =TRUE;
            return NOERROR;
        }

        // if we do not have information on record, ask
        // the user (display a dialog)
        AutoPtr<ICallbackProxy> proxy = ((CBrowserFrame *)((loader -> GetFrame() ).Get() ) ) -> GetCallbackProxy();
        //JAVA:    proxy.onReceivedSslError(new SslErrorHandler(this, loader), error.Get() );
        AutoPtr<CSslErrorHandler> pSslErrorHandler;
        pSslErrorHandler =  new CSslErrorHandler();
        pSslErrorHandler -> InitPara(this,loader.Get());
        proxy -> OnReceivedSslError(pSslErrorHandler.Get(), error.Get());
    }
    // the queue must be empty, stop
    *ret = FALSE;
    return NOERROR;
}

ECode CSslErrorHandler::InitPara(
        /* [in] */ ISslErrorHandler * origin, 
        /* [in] */ LoadListener * listener)
{
    mOriginHandler = origin;
    mLoadListener = listener;
    return NOERROR;
}

ECode CSslErrorHandler::Proceed()
{
    /*
    AutoPtr<IHandler> pHandler;
    pHandler = IHandler::Probe(mOriginHandler);    
    AutoPtr<IMessage> pMessage;
    pHandler -> ObtainMessage(HANDLE_RESPONSE, 1, 0, mLoadListener.Get(), (IMessage**)&pMessage);
    Boolean bSendMessage = FALSE;
    pHandler -> SendMessage(pMessage.Get(),&bSendMessage);
    */
    return NOERROR;
}

ECode CSslErrorHandler::Cancel()
{
    /*
    AutoPtr<IHandler> pHandler;
    pHandler = IHandler::Probe(mOriginHandler);    
    AutoPtr<IMessage> pMessage;
    pHandler -> ObtainMessage(HANDLE_RESPONSE, 0, 0, mLoadListener.Get(), (IMessage**)&pMessage);
    Boolean bSendMessage = FALSE;
    pHandler -> SendMessage(pMessage.Get(),&bSendMessage);
    */
    return NOERROR;
}

ECode CSslErrorHandler::HandleSslErrorResponse(
        /* [in] */ LoadListener * loader,
        /* [in] */ ISslError * error, 
        /* [in] */ Boolean proceed)
{
    Mutex::Autolock lock(_m_syncLock);
    if (DebugFlags::sSSL_ERROR_HANDLER) 
    {
        //JAVA:Assert.assertNotNull(loader);
        //JAVA:Assert.assertNotNull(error);
        assert( loader != NULL );
        assert( error != NULL );
    }

    if (DebugFlags::sSSL_ERROR_HANDLER) 
    {
        String strUrl;
        loader -> Url(/*&*/strUrl);
        String strProceed;
        Utility::Logging::Logger::V(LOGTAG, String("SslErrorHandler::HandleSslErrorResponse():") 
            + String(" proceed: ") + strProceed.FromBoolean(proceed) 
            + String(" url:") + strUrl + "\n");
    }

    if (!(loader -> Cancelled() ) )
    {
        if (proceed) 
        {
            // update the user's SSL error preference table
            Int32 primary = 0;
            //error -> GetPrimaryError(&primary);
            String host;
            loader -> Host(/*&*/host);

            if (DebugFlags::sSSL_ERROR_HANDLER) 
            {
                //JAVA:Assert.assertTrue(host != null && primary != 0);
                assert( ( !( host.IsNullOrEmpty() ) ) &&  primary != 0 );
            }
            Boolean hasKey;
            mSslPrefTable -> ContainsKey(host , &hasKey);
            Int32 nSslPrefTableInt = 0;
            mSslPrefTable -> GetInt32(host , &nSslPrefTableInt);
            if( !hasKey || primary > nSslPrefTableInt )
            {
                mSslPrefTable -> PutInt32(host, primary);
            }
        }
        loader -> HandleSslErrorResponse(proceed);
    }

    return NOERROR;
}

