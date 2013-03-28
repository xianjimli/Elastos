
#include "webkit/WebSyncManager.h"

#include <Logger.h>
#include <webkit/DebugFlags.h>
#include <os/Process.h>

const CString WebSyncManager::LOGTAG = "websync";

/*****************************WebSyncManager::SyncHandler*****************************/
PInterface WebSyncManager::SyncHandler::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IHandler*)this;
    }
    else if (riid == EIID_IHandler) {
        return (IHandler*)this;
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

    if (Object == (IInterface*)(IHandler*)this) {
        *iID = EIID_IHandler;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

//@Override
ECode WebSyncManager::SyncHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    if ( (/*msg -> what*/0) == (mWebSyncManager -> SYNC_MESSAGE) ) {
        if (DebugFlags::sWEB_SYNC_MANAGER) {
            Utility::Logging::Logger::V(LOGTAG, String("*** WebSyncManager sync ***\n") );
        }
        mWebSyncManager -> SyncFromRamToFlash();

        // send a delayed message to request sync later
        /*
        AutoPtr<IMessage> newmsg;
        ObtainMessage( (mWebSyncManager -> SYNC_MESSAGE), (IMessage**)&newmsg);    //Handler -> ObtainMessage(...)
        SendMessageDelayed(newmsg.Get(), (mWebSyncManager -> SYNC_LATER_INTERVAL) );    //Handler -> SendMessageDelayed(...)
        */
    }
    return NOERROR;
}

WebSyncManager::SyncHandler::SyncHandler(
    /* [in] */ WebSyncManager* webSyncManager)
{
    mWebSyncManager = webSyncManager;
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
    //Looper::Prepare();    //JAVA:    Looper.prepare();    os/Looper.java (public class)

    mHandler = new SyncHandler(this);
    OnSyncInit();
    // lower the priority after OnSyncInit() is done
    Process::SetThreadPriority(Process::THREAD_PRIORITY_BACKGROUND);    //JAVA    os/Process.java (public class)
    /*
    AutoPtr<IMessage> msg;
    mHandler -> ObtainMessage(SYNC_MESSAGE,(IMessage**)&msg);
    Boolean bSendMessageDelayed;
    mHandler -> SendMessageDelayed(msg.Get(), SYNC_LATER_INTERVAL, &bSendMessageDelayed);
    */
    //Looper::loop();    //JAVA:    Looper.loop();    os/Looper.java (public class)

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
    /*
    mHandler -> RemoveMessages(SYNC_MESSAGE);
    AutoPtr<IMessage> msg;
    mHandler -> ObtainMessage(SYNC_MESSAGE,(IMessage**)&msg);
    Boolean bSendMessageDelayed;
    mHandler -> SendMessageDelayed(msg.Get(), SYNC_NOW_INTERVAL,&bSendMessageDelayed);
    */
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
    /*
    mHandler -> RemoveMessages(SYNC_MESSAGE);
    AutoPtr<IMessage> msg;
    mHandler -> ObtainMessage(SYNC_MESSAGE,(IMessage**)&msg);
    Boolean bSendMessageDelayed;
    mHandler -> SendMessageDelayed(msg.Get(), SYNC_LATER_INTERVAL,&bSendMessageDelayed);
    */
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
        /*
        AutoPtr<IMessage> msg;
        mHandler -> ObtainMessage(SYNC_MESSAGE,(IMessage**)&msg);
        Boolean bSendMessageDelayed;
        mHandler -> SendMessageDelayed(msg, SYNC_LATER_INTERVAL,&bSendMessageDelayed);
        */
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
    	/*
        mHandler -> RemoveMessages(SYNC_MESSAGE);
        */
    }
}

void WebSyncManager::OnSyncInit()
{	
}
