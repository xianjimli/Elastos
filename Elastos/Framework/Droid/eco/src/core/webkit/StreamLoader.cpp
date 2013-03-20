#include "webkit/StreamLoader.h"
//#include "os/CHandler.h"
//#include "net/http/CHeaders.h"

StreamLoader::StreamLoader(
        /* [in] */ /*const*/ LoadListener * loadListener)
{
    mLoadListener = loadListener;
    mContext = loadListener -> GetContext();
}

void StreamLoader::Load()
{
    if(TRUE){
        Elastos::Core::Threading::Mutex::Autolock lock(mLock);
        if(mHandler == NULL){
            /*
            AutoPtr<IHandler> pHandler;
            CHandler::New((IHandler**)&pHandler);
            mHandler = pHandler;
            */
        }
    }

    if ( !(mLoadListener -> IsSynchronous()) ) {
        //Boolean bSendEmptyMessage = FALSE;
        //mHandler -> SendEmptyMessage(MSG_STATUS ,&bSendEmptyMessage);
    }
    else {
        // Load the stream synchronously.
        if (SetupStreamAndSendStatus()) {
            // We were able to open the stream, create the array
            // to pass data to the loader
            ArrayOf<Byte>::Free(mData.Get());
            mData = ArrayOf<Byte>::Alloc(8192);
            SendHeaders();
            while (!SendData() && !( mLoadListener -> Cancelled() ) );
            CloseStreamAndSendEndData();
            mLoadListener -> LoadSynchronousMessages();
        }
    }
}

Boolean StreamLoader::HandleMessage(
        /* [in] */ const IMessage* msg)
{
    if( mLoadListener -> IsSynchronous() ) {
        //JAVA:throw new AssertionError();
        //return E_ASSERTION_ERROR;
        return FALSE;
    }
    if(mLoadListener -> Cancelled()) {
        CloseStreamAndSendEndData();
        return TRUE;
    }
    //Boolean bSendEmptyMessage = FALSE;    
    switch(/*msg -> mWhat*/0)  {
        case MSG_STATUS:
            if (SetupStreamAndSendStatus())  {
                // We were able to open the stream, create the array
                // to pass data to the loader
                ArrayOf<Byte>::Free(mData.Get());
                mData = ArrayOf<Byte>::Alloc(8192);
                //mHandler -> SendEmptyMessage(MSG_HEADERS,&bSendEmptyMessage);
            }
            break;
        case MSG_HEADERS:
            SendHeaders();
            //mHandler -> SendEmptyMessage(MSG_DATA,&bSendEmptyMessage);
            break;
        case MSG_DATA:
            if (SendData())  {
                //mHandler -> SendEmptyMessage(MSG_END,&bSendEmptyMessage);
            } 
            else  {
                //mHandler -> SendEmptyMessage(MSG_DATA,&bSendEmptyMessage);
            }
            break;
        case MSG_END:
            CloseStreamAndSendEndData();
            break;
        default:
            return FALSE;
    }
    return TRUE;
}

void StreamLoader::SendHeaders()
{
    /*
    AutoPtr<IHeaders> headers;
    CHeaders::New( (IHeaders**)&headers );
    if( mContentLength > 0 ) {
        headers -> SetContentLength(mContentLength);
    }
    BuildHeaders( headers.Get() );
    mLoadListener -> Headers( headers.Get() );
    */
}

Boolean StreamLoader::SendData()
{
    if( mDataStream != NULL ) {
        Int32 amount;
        ECode ec = mDataStream -> ReadBuffer(mData.Get(),&amount);
        if( ec != NOERROR ) {
            //JAVA:mLoadListener.error(EventHandler.FILE_ERROR, ex.getMessage());
            mLoadListener -> Error(/*IEventHandler::FILE_ERROR*/0, String::FromInt32(ec) );
        }
        if (amount > 0)  {
            mLoadListener -> Data(/*mData.Get()*/(*mData), amount);
            return FALSE;
        }
    }
    return TRUE;
}

void StreamLoader::CloseStreamAndSendEndData()
{
    if( mDataStream.Get() != NULL ) {
        mDataStream -> Close();
    }
    mLoadListener -> EndData();
}

