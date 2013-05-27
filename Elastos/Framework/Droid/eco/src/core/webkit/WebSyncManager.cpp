
#include "webkit/WebSyncManager.h"

#include <Logger.h>
#include <webkit/DebugFlags.h>
#include <os/Process.h>
#include "os/CApartment.h"

const CString WebSyncManager::LOGTAG = "websync";

// message code for sync message
const Int32 WebSyncManager::SYNC_MESSAGE;
// time delay in millisec for a sync (now) message
const Int32 WebSyncManager::SYNC_NOW_INTERVAL;
// time delay in millisec for a sync (later) message
const Int32 WebSyncManager::SYNC_LATER_INTERVAL;

/*****************************WebSyncManager::SyncHandler*****************************/
PInterface WebSyncManager::SyncHandler::Probe(
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

UInt32 WebSyncManager::SyncHandler::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 WebSyncManager::SyncHandler::Release()
{
    return ElRefBase::Release();
}

ECode WebSyncManager::SyncHandler::GetInterfaceID(
    /* [in] */ IInterface* Object,
    /* [out] */ InterfaceID* iID)
{
    VALIDATE_NOT_NULL(iID);
    if (iID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (Object == (IInterface*)(IApartment*)this) {
        *iID = EIID_IApartment;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}


ECode WebSyncManager::SyncHandler::Start(
    /* [in] */ ApartmentAttr attr)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode WebSyncManager::SyncHandler::Finish()
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode WebSyncManager::SyncHandler::PostCppCallback(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallback(target, func, params, id);
}

ECode WebSyncManager::SyncHandler::PostCppCallbackAtTime(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id,
    /* [in] */ Millisecond64 uptimeMillis)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallbackAtTime(target, func, params, id, uptimeMillis);
}

ECode WebSyncManager::SyncHandler::PostCppCallbackDelayed(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id,
    /* [in] */ Millisecond64 delayMillis)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallbackDelayed(target, func, params, id, delayMillis);
}

ECode WebSyncManager::SyncHandler::PostCppCallbackAtFrontOfQueue(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallbackAtFrontOfQueue(target, func, params, id);
}

ECode WebSyncManager::SyncHandler::RemoveCppCallbacks(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func)
{
    assert(mApartment != NULL);
    return mApartment->RemoveCppCallbacks(target, func);
}

ECode WebSyncManager::SyncHandler::RemoveCppCallbacksEx(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ Int32 id)
{
    assert(mApartment != NULL);
    return mApartment->RemoveCppCallbacksEx(target, func, id);
}

ECode WebSyncManager::SyncHandler::HasCppCallbacks(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [out] */ Boolean* result)
{
    assert(mApartment != NULL);
    return mApartment->HasCppCallbacks(target, func, result);
}

ECode WebSyncManager::SyncHandler::HasCppCallbacksEx(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ Int32 id,
    /* [out] */ Boolean* result)
{
    assert(mApartment != NULL);
    return mApartment->HasCppCallbacksEx(target, func, id, result);
}

ECode WebSyncManager::SyncHandler::SendMessage(
    /* [in] */ Int32 message,
    /* [in] */ IParcel* params)
{
    if(message == (WebSyncManager::SYNC_MESSAGE) )
    {
        void (STDCALL WebSyncManager::SyncHandler::*pHandlerFunc)();
        pHandlerFunc = &WebSyncManager::SyncHandler::HandleSyncMessage;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

void WebSyncManager::SyncHandler::HandleSyncMessage()
{
    if (DebugFlags::sWEB_SYNC_MANAGER) {
        Utility::Logging::Logger::V(LOGTAG, String("*** WebSyncManager sync ***\n") );
    }
    mWebSyncManager -> SyncFromRamToFlash();

    // send a delayed message to request sync later
    //JAVA:  Message newmsg = obtainMessage(SYNC_MESSAGE);
    //JAVA:  sendMessageDelayed(newmsg, SYNC_LATER_INTERVAL);
    void (STDCALL WebSyncManager::SyncHandler::*pHandlerFunc)();
    pHandlerFunc = &WebSyncManager::SyncHandler::HandleSyncMessage;
    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    PostCppCallbackDelayed((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0, (mWebSyncManager -> SYNC_LATER_INTERVAL) );
}

WebSyncManager::SyncHandler::SyncHandler(
    /* [in] */ WebSyncManager* webSyncManager)
{
    mWebSyncManager = webSyncManager;
    assert(SUCCEEDED(CApartment::GetMainApartment((IApartment**)&mApartment))
        && (mApartment != NULL));
}

/*****************************WebSyncManager*****************************/
PInterface WebSyncManager::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IRunnable*)this;
    }
    else if (riid == EIID_IRunnable) {
        return (IRunnable*)this;
    }
    return NULL;
}

UInt32 WebSyncManager::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 WebSyncManager::Release()
{
    return ElRefBase::Release();
}

ECode WebSyncManager::GetInterfaceID(
    /* [in] */ IInterface* Object,
    /* [out] */ InterfaceID* iID)
{
    VALIDATE_NOT_NULL(iID);
    if (iID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (Object == (IInterface*)(IRunnable*)this) {
        *iID = EIID_IRunnable;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

WebSyncManager::WebSyncManager(
    /* [in] */ IContext* context, 
    /* [in] */ const String& name)
{
    mThreadName = name;
    if (context != NULL) {
        mDataBase -> GetInstance(context,(IWebViewDatabase**)&mDataBase);    //WebViewDatabase.getInstance(context);
        CThread::New( (IRunnable*)this, (IThread**)&mSyncThread);
        mSyncThread -> SetName(mThreadName);
        mSyncThread -> Start();
    } 
    else {
        Utility::Logging::Logger::E(LOGTAG, String("WebSyncManager can't be created without context\n") );
        //return E_ILLEGAL_STATE_EXCEPTION;
    }
}

CARAPI_(IInterface*) WebSyncManager::Clone()
{
    //JAVA    throw new CloneNotSupportedException("doesn't implement Cloneable");
    Utility::Logging::Logger::E(LOGTAG, String("doesn't implement Cloneable\n") );        
    //return E_UNSUPPORTED_OPERATION_EXCEPTION;
    return NULL;
}

ECode WebSyncManager::Run()
{
    // prepare Looper for sync handler
    CApartment::Prepare();    //JAVA:  Looper.prepare();    //JAVA:  os/Looper.java (public class)

    mHandler = new SyncHandler(this);
    OnSyncInit();
    // lower the priority after OnSyncInit() is done
    Process::SetThreadPriority(Process::THREAD_PRIORITY_BACKGROUND);    //JAVA    os/Process.java (public class)

    //JAVA:  Message msg = mHandler.obtainMessage(SYNC_MESSAGE);
    //JAVA:  mHandler.sendMessageDelayed(msg, SYNC_LATER_INTERVAL);
    void (STDCALL WebSyncManager::SyncHandler::*pHandlerFunc)();
    pHandlerFunc = &WebSyncManager::SyncHandler::HandleSyncMessage;
    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    mHandler->PostCppCallbackDelayed((Handle32)(mHandler.Get()), *(Handle32*)&pHandlerFunc, params, 0, SYNC_LATER_INTERVAL);

    mHandler->Start(ApartmentAttr_New);    //JAVA:    Looper.loop();    //JAVA:  os/Looper.java (public class)
    return NOERROR;
}

/**
 * sync() forces sync manager to sync now
 */
void WebSyncManager::Sync()
{
    if (DebugFlags::sWEB_SYNC_MANAGER) {
        Utility::Logging::Logger::V(LOGTAG, String("*** WebSyncManager sync ***\n") );
    }
    if ( (mHandler.Get()) == NULL ) {
            return;
    }
    //JAVA:  mHandler.removeMessages(SYNC_MESSAGE);
    void (STDCALL WebSyncManager::SyncHandler::*pHandlerFunc)();
    pHandlerFunc = &WebSyncManager::SyncHandler::HandleSyncMessage;
    mHandler->RemoveCppCallbacks((Handle32)(mHandler.Get()), *(Handle32*)&pHandlerFunc);
    
    //JAVA:  Message msg = mHandler.obtainMessage(SYNC_MESSAGE);
    //JAVA:  mHandler.sendMessageDelayed(msg, SYNC_NOW_INTERVAL);
    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    mHandler->PostCppCallbackDelayed((Handle32)(mHandler.Get()), *(Handle32*)&pHandlerFunc, params, 0, SYNC_NOW_INTERVAL);
}

/**
 * resetSync() resets sync manager's timer
 */
void WebSyncManager::ResetSync()
{
    if (DebugFlags::sWEB_SYNC_MANAGER) {
        Utility::Logging::Logger::V(LOGTAG, String("*** WebSyncManager resetSync ***\n") );
    }
    if ( (mHandler.Get()) == NULL ) {
        return;
    }
    //JAVA:  mHandler.removeMessages(SYNC_MESSAGE);
    void (STDCALL WebSyncManager::SyncHandler::*pHandlerFunc)();
    pHandlerFunc = &WebSyncManager::SyncHandler::HandleSyncMessage;
    mHandler->RemoveCppCallbacks((Handle32)(mHandler.Get()), *(Handle32*)&pHandlerFunc);

    //JAVA:  Message msg = mHandler.obtainMessage(SYNC_MESSAGE);
    //JAVA:  mHandler.sendMessageDelayed(msg, SYNC_LATER_INTERVAL);
    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    mHandler->PostCppCallbackDelayed((Handle32)(mHandler.Get()), *(Handle32*)&pHandlerFunc, params, 0, SYNC_LATER_INTERVAL);
}

/**
 * startSync() requests sync manager to start sync
 */
void WebSyncManager::StartSync()
{
    if (DebugFlags::sWEB_SYNC_MANAGER) {
        Utility::Logging::Logger::V(LOGTAG, String("***  WebSyncManager startSync ***, Ref count:") + String::FromInt32(mStartSyncRefCount) + String("\n") );
    }
    if ( (mHandler.Get()) == NULL ) {
        return;
    }
    if ( ++mStartSyncRefCount == 1 ) {
        //JAVA:  Message msg = mHandler.obtainMessage(SYNC_MESSAGE);
        //JAVA:  mHandler.sendMessageDelayed(msg, SYNC_LATER_INTERVAL);            
        void (STDCALL WebSyncManager::SyncHandler::*pHandlerFunc)();
        pHandlerFunc = &WebSyncManager::SyncHandler::HandleSyncMessage;
        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        mHandler->PostCppCallbackDelayed((Handle32)(mHandler.Get()), *(Handle32*)&pHandlerFunc, params, 0, SYNC_LATER_INTERVAL);
    }
}

/**
 * stopSync() requests sync manager to stop sync. remove any SYNC_MESSAGE in
 * the queue to break the sync loop
 */
void WebSyncManager::StopSync()
{
    if (DebugFlags::sWEB_SYNC_MANAGER) {
        Utility::Logging::Logger::V(LOGTAG, String("*** WebSyncManager stopSync ***, Ref count:") + String::FromInt32(mStartSyncRefCount) + String("\n") );
    }
    if ( (mHandler.Get()) == NULL) {
        return;
    }
    if (--mStartSyncRefCount == 0) {
        //JAVA:  mHandler.removeMessages(SYNC_MESSAGE);
        void (STDCALL WebSyncManager::SyncHandler::*pHandlerFunc)();
        pHandlerFunc = &WebSyncManager::SyncHandler::HandleSyncMessage;
        mHandler->RemoveCppCallbacks((Handle32)(mHandler.Get()), *(Handle32*)&pHandlerFunc);

    }
}

void WebSyncManager::OnSyncInit()
{	
}
