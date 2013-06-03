#include "webkit/StreamLoader.h"
#include "os/CApartment.h"
//#include "net/http/CHeaders.h"

const Int32 StreamLoader::MSG_STATUS;
const Int32 StreamLoader::MSG_HEADERS;
const Int32 StreamLoader::MSG_DATA;
const Int32 StreamLoader::MSG_END;

StreamLoader::StreamLoader(
        /* [in] */ /*const*/ LoadListener * loadListener)
{
    assert(SUCCEEDED(CApartment::GetMainApartment((IApartment**)&mApartment))
        && (mApartment != NULL));
    mLoadListener = loadListener;
    mContext = loadListener -> GetContext();
}

void StreamLoader::Load()
{
    if(TRUE){
        Elastos::Core::Threading::Mutex::Autolock lock(mMutex);
        if(mHandler == NULL){
            //CApartment::GetMyApartment((IApartment**)&mHandler);
            mHandler = this;
        }
    }

    if ( !(mLoadListener -> IsSynchronous()) ) {
        //Java:  mHandler.sendEmptyMessage(MSG_STATUS);
        void (STDCALL StreamLoader::*pHandlerFunc)();
        pHandlerFunc = &StreamLoader::HandleMsgStatus;
        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        mHandler->PostCppCallback( (Handle32)(mHandler.Get()), *(Handle32*)&pHandlerFunc, params, 0);
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

void StreamLoader::HandleMsgStatus()
{
    if (SetupStreamAndSendStatus())  {
        // We were able to open the stream, create the array
        // to pass data to the loader
        ArrayOf<Byte>::Free(mData.Get());
        mData = ArrayOf<Byte>::Alloc(8192);
        //JAVA:  mHandler.sendEmptyMessage(MSG_HEADERS);
        void (STDCALL StreamLoader::*pHandlerFunc)();
        pHandlerFunc = &StreamLoader::HandleMsgHeaders;
        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        mHandler->PostCppCallback( (Handle32)(mHandler.Get()), *(Handle32*)&pHandlerFunc, params, 0);
    }
}

void StreamLoader::HandleMsgHeaders()
{
    SendHeaders();
    //JAVA:  mHandler.sendEmptyMessage(MSG_DATA);
    void (STDCALL StreamLoader::*pHandlerFunc)();
    pHandlerFunc = &StreamLoader::HandleMsgData;
    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    mHandler->PostCppCallback( (Handle32)(mHandler.Get()), *(Handle32*)&pHandlerFunc, params, 0);
}

void StreamLoader::HandleMsgData()
{
    if (SendData())  {
        //JAVA:  mHandler.sendEmptyMessage(MSG_END);
        void (STDCALL StreamLoader::*pHandlerFunc)();
        pHandlerFunc = &StreamLoader::HandleMsgEnd;
        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        mHandler->PostCppCallback( (Handle32)(mHandler.Get()), *(Handle32*)&pHandlerFunc, params, 0);
    } 
    else  {
        //JAVA:  mHandler.sendEmptyMessage(MSG_DATA);        
        void (STDCALL StreamLoader::*pHandlerFunc)();
        pHandlerFunc = &StreamLoader::HandleMsgData;
        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        mHandler->PostCppCallback( (Handle32)(mHandler.Get()), *(Handle32*)&pHandlerFunc, params, 0);
    }    
}

void StreamLoader::HandleMsgEnd()
{
    CloseStreamAndSendEndData();
}

void StreamLoader::SendHeaders()
{
    AutoPtr<IHeaders> headers;
    /*
    CHeaders::New( (IHeaders**)&headers );
    if( mContentLength > 0 ) {
        headers -> SetContentLength(mContentLength);
    }
    */
    BuildHeaders( headers.Get() );
    mLoadListener -> Headers( headers.Get() );
}

Boolean StreamLoader::SendData()
{
    if( mDataStream != NULL ) {
        Int32 amount;
        ECode ec = mDataStream -> ReadBuffer(mData.Get(),&amount);
        if( ec != NOERROR ) {
            //JAVA:mLoadListener.error(EventHandler.FILE_ERROR, ex.getMessage());
            mLoadListener -> Error(/*IEventHandler::FILE_ERROR*/-13, String::FromInt32(ec) );
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

Boolean StreamLoader::SetupStreamAndSendStatus()
{//=0(virtual)
    return FALSE;
}

void StreamLoader::BuildHeaders(
    /* [in] */ const IHeaders* headers)
{//=0(virtual)
}


PInterface StreamLoader::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IApartment*)this;
    }
    else if (riid == EIID_IApartment) {
        return (IApartment*)this;
    }
    return NULL;
}

UInt32 StreamLoader::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 StreamLoader::Release()
{
    return ElRefBase::Release();
}

ECode StreamLoader::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iID)
{    
    VALIDATE_NOT_NULL(iID);
    if (iID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (object == (IInterface*)(IApartment*)this) {
        *iID = EIID_IApartment;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode StreamLoader::Start(
    /* [in] */ ApartmentAttr attr)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode StreamLoader::Finish()
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode StreamLoader::PostCppCallback(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallback(target, func, params, id);
}

ECode StreamLoader::PostCppCallbackAtTime(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id,
    /* [in] */ Millisecond64 uptimeMillis)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallbackAtTime(target, func, params, id, uptimeMillis);
}

ECode StreamLoader::PostCppCallbackDelayed(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id,
    /* [in] */ Millisecond64 delayMillis)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallbackDelayed(target, func, params, id, delayMillis);
}

ECode StreamLoader::PostCppCallbackAtFrontOfQueue(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallbackAtFrontOfQueue(target, func, params, id);
}

ECode StreamLoader::RemoveCppCallbacks(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func)
{
    assert(mApartment != NULL);
    return mApartment->RemoveCppCallbacks(target, func);
}

ECode StreamLoader::RemoveCppCallbacksEx(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ Int32 id)
{
    assert(mApartment != NULL);
    return mApartment->RemoveCppCallbacksEx(target, func, id);
}

ECode StreamLoader::HasCppCallbacks(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [out] */ Boolean* result)
{
    assert(mApartment != NULL);
    return mApartment->HasCppCallbacks(target, func, result);
}

ECode StreamLoader::HasCppCallbacksEx(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ Int32 id,
    /* [out] */ Boolean* result)
{
    assert(mApartment != NULL);
    return mApartment->HasCppCallbacksEx(target, func, id, result);
}

ECode StreamLoader::SendMessage(
    /* [in] */ Int32 message,
    /* [in] */ IParcel* params)
{
    if( mLoadListener -> IsSynchronous() ) {
        //JAVA:throw new AssertionError();
        return E_ASSERTION_ERROR;
    }
    if(mLoadListener -> Cancelled()) {
        CloseStreamAndSendEndData();
        return NOERROR;
    }

    if (message == MSG_STATUS) {
        void (STDCALL StreamLoader::*pHandlerFunc)();
        pHandlerFunc = &StreamLoader::HandleMsgStatus;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
    }
    if (message == MSG_HEADERS) {
        void (STDCALL StreamLoader::*pHandlerFunc)();
        pHandlerFunc = &StreamLoader::HandleMsgHeaders;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
    }
    if (message == MSG_DATA) {
        void (STDCALL StreamLoader::*pHandlerFunc)();
        pHandlerFunc = &StreamLoader::HandleMsgData;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
    }
    if (message == MSG_END) {
        void (STDCALL StreamLoader::*pHandlerFunc)();
        pHandlerFunc = &StreamLoader::HandleMsgEnd;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
    }

    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

