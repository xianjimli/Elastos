
#include "webkit/WebViewCore.h"
#include "os/Process.h"

#include <Logger.h>
#include "webkit/CWebStorage.h"
#include "webkit/CGeolocationPermissions.h"
#include "webkit/CBrowserFrame.h"
#include "webkit/CWebIconDatabase.h"
#include "webkit/CWebView.h"
#include "webkit/JWebCoreJavaBridge.h"
#include <elastos/AutoFree.h>
#include "ext/frameworkext.h"
#include "webkit/CCallbackProxy.h"
#include "webkit/IValueCallback.h"
#include "os/Process.h"
#include "view/CKeyEvent.h"
#include "graphics/CPoint.h"
#include "graphics/CRect.h"
#include "webkit/WebViewWorker.h"
#include "webkit/Network.h"
#include "webkit/DebugFlags.h"
#include "webkit/CWebBackForwardList.h"
#include "webkit/CCacheManager.h"
#include <elastos/System.h>
#include <elastos/Math.h>
#include "graphics/CPicture.h"
#include "webkit/CWebHistoryItem.h"
#include "graphics/CRect.h"
#include "utils/CDisplayMetrics.h"
#include "webkit/CPluginManager.h"
#include "os/CApartment.h"
#include "os/CBundle.h"
#include "webkit/CWebView.h"

const CString WebViewCore::THREAD_NAME("WebViewCoreThread");
const CString WebViewCore::LOGTAG("webcore");

const Int32 WebViewCore::ZOOM_BITS = Paint_FILTER_BITMAP_FLAG | Paint_DITHER_FLAG | Paint_SUBPIXEL_TEXT_FLAG;
const Int32 WebViewCore::SCROLL_BITS = Paint_FILTER_BITMAP_FLAG | Paint_DITHER_FLAG;

/*
static {
    // Load libwebcore during static initialization. This happens in the
    // zygote process so it will be shared read-only across all app
    // processes.
    System.loadLibrary("webcore");
}
*/

const CString WebViewCore::HandlerDebugString[] = {
    "REQUEST_LABEL", // 97
    "UPDATE_FRAME_CACHE_IF_LOADING", // = 98
    "SCROLL_TEXT_INPUT", // = 99
    "LOAD_URL", // = 100;
    "STOP_LOADING", // = 101;
    "RELOAD", // = 102;
    "KEY_DOWN", // = 103;
    "KEY_UP", // = 104;
    "VIEW_SIZE_CHANGED", // = 105;
    "GO_BACK_FORWARD", // = 106;
    "SET_SCROLL_OFFSET", // = 107;
    "RESTORE_STATE", // = 108;
    "PAUSE_TIMERS", // = 109;
    "RESUME_TIMERS", // = 110;
    "CLEAR_CACHE", // = 111;
    "CLEAR_HISTORY", // = 112;
    "SET_SELECTION", // = 113;
    "REPLACE_TEXT", // = 114;
    "PASS_TO_JS", // = 115;
    "SET_GLOBAL_BOUNDS", // = 116;
    "UPDATE_CACHE_AND_TEXT_ENTRY", // = 117;
    "CLICK", // = 118;
    "SET_NETWORK_STATE", // = 119;
    "DOC_HAS_IMAGES", // = 120;
    "121", // = 121;
    "DELETE_SELECTION", // = 122;
    "LISTBOX_CHOICES", // = 123;
    "SINGLE_LISTBOX_CHOICE", // = 124;
    "MESSAGE_RELAY", // = 125;
    "SET_BACKGROUND_COLOR", // = 126;
    "SET_MOVE_FOCUS", // = 127
    "SAVE_DOCUMENT_STATE", // = 128;
    "129", // = 129;
    "WEBKIT_DRAW", // = 130;
    "SYNC_SCROLL", // = 131;
    "POST_URL", // = 132;
    "SPLIT_PICTURE_SET", // = 133;
    "CLEAR_CONTENT", // = 134;
    "SET_MOVE_MOUSE", // = 135;
    "SET_MOVE_MOUSE_IF_LATEST", // = 136;
    "REQUEST_CURSOR_HREF", // = 137;
    "ADD_JS_INTERFACE", // = 138;
    "LOAD_DATA", // = 139;
    "TOUCH_UP", // = 140;
    "TOUCH_EVENT", // = 141;
    "SET_ACTIVE", // = 142;
    "ON_PAUSE",     // = 143
    "ON_RESUME",    // = 144
    "FREE_MEMORY",  // = 145
    "VALID_NODE_BOUNDS", // = 146
};

AutoPtr<IApartment> WebViewCore::sWebCoreHandler;

Boolean WebViewCore::mRepaintScheduled = FALSE;

Core::Threading::Mutex WebViewCore::mMutexClass;


// mAction of TouchEventData can be MotionEvent.getAction() which uses the
// last two bytes or one of the following values
const Int32 WebViewCore::ACTION_LONGPRESS;
const Int32 WebViewCore::ACTION_DOUBLETAP;

// Message Ids
const Int32 WebViewCore::EventHub::REQUEST_LABEL;
const Int32 WebViewCore::EventHub::UPDATE_FRAME_CACHE_IF_LOADING;
const Int32 WebViewCore::EventHub::SCROLL_TEXT_INPUT;
const Int32 WebViewCore::EventHub::LOAD_URL;
const Int32 WebViewCore::EventHub::STOP_LOADING;
const Int32 WebViewCore::EventHub::RELOAD;
const Int32 WebViewCore::EventHub::KEY_DOWN;
const Int32 WebViewCore::EventHub::KEY_UP;
const Int32 WebViewCore::EventHub::VIEW_SIZE_CHANGED;
const Int32 WebViewCore::EventHub::GO_BACK_FORWARD;
const Int32 WebViewCore::EventHub::SET_SCROLL_OFFSET;
const Int32 WebViewCore::EventHub::RESTORE_STATE;
const Int32 WebViewCore::EventHub::PAUSE_TIMERS;
const Int32 WebViewCore::EventHub::RESUME_TIMERS;
const Int32 WebViewCore::EventHub::CLEAR_CACHE;
const Int32 WebViewCore::EventHub::CLEAR_HISTORY;
const Int32 WebViewCore::EventHub::SET_SELECTION;
const Int32 WebViewCore::EventHub::REPLACE_TEXT;
const Int32 WebViewCore::EventHub::PASS_TO_JS;
const Int32 WebViewCore::EventHub::SET_GLOBAL_BOUNDS;
const Int32 WebViewCore::EventHub::UPDATE_CACHE_AND_TEXT_ENTRY;
const Int32 WebViewCore::EventHub::CLICK;
const Int32 WebViewCore::EventHub::SET_NETWORK_STATE;
const Int32 WebViewCore::EventHub::DOC_HAS_IMAGES;
const Int32 WebViewCore::EventHub::DELETE_SELECTION;
const Int32 WebViewCore::EventHub::LISTBOX_CHOICES;
const Int32 WebViewCore::EventHub::SINGLE_LISTBOX_CHOICE;
const Int32 WebViewCore::EventHub::MESSAGE_RELAY;
const Int32 WebViewCore::EventHub::SET_BACKGROUND_COLOR;
const Int32 WebViewCore::EventHub::SET_MOVE_FOCUS;
const Int32 WebViewCore::EventHub::SAVE_DOCUMENT_STATE;

const Int32 WebViewCore::EventHub::WEBKIT_DRAW;
const Int32 WebViewCore::EventHub::SYNC_SCROLL;
const Int32 WebViewCore::EventHub::POST_URL;
const Int32 WebViewCore::EventHub::SPLIT_PICTURE_SET;
const Int32 WebViewCore::EventHub::CLEAR_CONTENT;

// UI nav messages
const Int32 WebViewCore::EventHub::SET_MOVE_MOUSE;
const Int32 WebViewCore::EventHub::SET_MOVE_MOUSE_IF_LATEST;
const Int32 WebViewCore::EventHub::REQUEST_CURSOR_HREF;
const Int32 WebViewCore::EventHub::ADD_JS_INTERFACE;
const Int32 WebViewCore::EventHub::LOAD_DATA;

// motion
const Int32 WebViewCore::EventHub::TOUCH_UP;
// message used to pass UI touch events to WebCore
const Int32 WebViewCore::EventHub::TOUCH_EVENT;

// Used to tell the focus controller not to draw the blinking cursor,
// based on whether the WebView has focus and whether the WebView's
// cursor matches the webpage's focus.
const Int32 WebViewCore::EventHub::SET_ACTIVE;

// lifecycle activities for just this DOM (unlike pauseTimers, which
// is global)
const Int32 WebViewCore::EventHub::ON_PAUSE;
const Int32 WebViewCore::EventHub::ON_RESUME;
const Int32 WebViewCore::EventHub::FREE_MEMORY;
const Int32 WebViewCore::EventHub::VALID_NODE_BOUNDS;

// Network-based messaging
const Int32 WebViewCore::EventHub::CLEAR_SSL_PREF_TABLE;

// Test harness messages
const Int32 WebViewCore::EventHub::REQUEST_EXT_REPRESENTATION;
const Int32 WebViewCore::EventHub::REQUEST_DOC_AS_TEXT;

// debugging
const Int32 WebViewCore::EventHub::DUMP_DOMTREE;
const Int32 WebViewCore::EventHub::DUMP_RENDERTREE;
const Int32 WebViewCore::EventHub::DUMP_NAVTREE;
const Int32 WebViewCore::EventHub::DUMP_V8COUNTERS;

const Int32 WebViewCore::EventHub::SET_JS_FLAGS;
// Geolocation
const Int32 WebViewCore::EventHub::GEOLOCATION_PERMISSIONS_PROVIDE;

const Int32 WebViewCore::EventHub::POPULATE_VISITED_LINKS;

const Int32 WebViewCore::EventHub::HIDE_FULLSCREEN;

const Int32 WebViewCore::EventHub::SET_NETWORK_TYPE;

// navigator.isApplicationInstalled()
const Int32 WebViewCore::EventHub::ADD_PACKAGE_NAMES;
const Int32 WebViewCore::EventHub::ADD_PACKAGE_NAME;
const Int32 WebViewCore::EventHub::REMOVE_PACKAGE_NAME;

// private message ids
const Int32 WebViewCore::EventHub::DESTROY;

// Message id for initializing a new WebViewCore.
const Int32 WebViewCore::WebCoreThread::INITIALIZE;
const Int32 WebViewCore::WebCoreThread::REDUCE_PRIORITY;
const Int32 WebViewCore::WebCoreThread::RESUME_PRIORITY;

/*****************************WebViewCore::DrawData*****************************/
WebViewCore::DrawData::DrawData()
{
    CRegion::New((IRegion**)&mInvalRegion);
    CPoint::New((IPoint**)&mWidthHeight);
}

WebViewCore::DrawData::~DrawData(){
    delete mInvalRegion;
    delete mWidthHeight;
}

/*****************************WebViewCore::WvcWebStorageQuotaUpdater*****************************/
ECode WebViewCore::WvcWebStorageQuotaUpdater::UpdateQuota(
    /* [in] */  Int64 newQuota)
{
    mWebViewCore -> NativeSetNewStorageLimit(newQuota);
    return NOERROR;
}

WebViewCore::WvcWebStorageQuotaUpdater::WvcWebStorageQuotaUpdater(
    /* [in] */  WebViewCore* webViewCore)
{
    mWebViewCore = webViewCore;
}

/*****************************WebViewCore::WebCoreThread*****************************/
PInterface WebViewCore::WebCoreThread::WvcWctHandler::Probe(
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

UInt32 WebViewCore::WebCoreThread::WvcWctHandler::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 WebViewCore::WebCoreThread::WvcWctHandler::Release()
{
    return ElRefBase::Release();
}

ECode WebViewCore::WebCoreThread::WvcWctHandler::GetInterfaceID(
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


ECode WebViewCore::WebCoreThread::WvcWctHandler::Start(
    /* [in] */ ApartmentAttr attr)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode WebViewCore::WebCoreThread::WvcWctHandler::Finish()
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode WebViewCore::WebCoreThread::WvcWctHandler::PostCppCallback(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallback(target, func, params, id);
}

ECode WebViewCore::WebCoreThread::WvcWctHandler::PostCppCallbackAtTime(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id,
    /* [in] */ Millisecond64 uptimeMillis)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallbackAtTime(target, func, params, id, uptimeMillis);
}

ECode WebViewCore::WebCoreThread::WvcWctHandler::PostCppCallbackDelayed(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id,
    /* [in] */ Millisecond64 delayMillis)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallbackDelayed(target, func, params, id, delayMillis);
}

ECode WebViewCore::WebCoreThread::WvcWctHandler::PostCppCallbackAtFrontOfQueue(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallbackAtFrontOfQueue(target, func, params, id);
}

ECode WebViewCore::WebCoreThread::WvcWctHandler::RemoveCppCallbacks(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func)
{
    assert(mApartment != NULL);
    return mApartment->RemoveCppCallbacks(target, func);
}

ECode WebViewCore::WebCoreThread::WvcWctHandler::RemoveCppCallbacksEx(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ Int32 id)
{
    assert(mApartment != NULL);
    return mApartment->RemoveCppCallbacksEx(target, func, id);
}

ECode WebViewCore::WebCoreThread::WvcWctHandler::HasCppCallbacks(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [out] */ Boolean* result)
{
    assert(mApartment != NULL);
    return mApartment->HasCppCallbacks(target, func, result);
}

ECode WebViewCore::WebCoreThread::WvcWctHandler::HasCppCallbacksEx(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ Int32 id,
    /* [out] */ Boolean* result)
{
    assert(mApartment != NULL);
    return mApartment->HasCppCallbacksEx(target, func, id, result);
}

ECode WebViewCore::WebCoreThread::WvcWctHandler::SendMessage(
    /* [in] */ Int32 message,
    /* [in] */ IParcel* params)
{
    if (message == WebViewCore::WebCoreThread::INITIALIZE) {
        void (STDCALL WebViewCore::WebCoreThread::WvcWctHandler::*pHandlerFunc)(WebViewCore*);
        pHandlerFunc = &WebViewCore::WebCoreThread::WvcWctHandler::HandleInitialize;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
    }
    if (message == WebViewCore::WebCoreThread::REDUCE_PRIORITY) {
        void (STDCALL WebViewCore::WebCoreThread::WvcWctHandler::*pHandlerFunc)();
        pHandlerFunc = &WebViewCore::WebCoreThread::WvcWctHandler::HandleReducePriority;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
    }
    if (message == WebViewCore::WebCoreThread::RESUME_PRIORITY) {
        void (STDCALL WebViewCore::WebCoreThread::WvcWctHandler::*pHandlerFunc)();
        pHandlerFunc = &WebViewCore::WebCoreThread::WvcWctHandler::HandleResumePriority;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
    }
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

Handle32 WebViewCore::WebCoreThread::WvcWctHandler::GetFunc(
    /* [in] */ Int32 message)
{
    if (message == WebViewCore::WebCoreThread::INITIALIZE) {
        void (STDCALL WebViewCore::WebCoreThread::WvcWctHandler::*pHandlerFunc)(WebViewCore*);
        pHandlerFunc = &WebViewCore::WebCoreThread::WvcWctHandler::HandleInitialize;
        return *(Handle32*)&pHandlerFunc;
    }
    if (message == WebViewCore::WebCoreThread::REDUCE_PRIORITY) {
        void (STDCALL WebViewCore::WebCoreThread::WvcWctHandler::*pHandlerFunc)();
        pHandlerFunc = &WebViewCore::WebCoreThread::WvcWctHandler::HandleReducePriority;
        return *(Handle32*)&pHandlerFunc;
    }
    if (message == WebViewCore::WebCoreThread::RESUME_PRIORITY) {
        void (STDCALL WebViewCore::WebCoreThread::WvcWctHandler::*pHandlerFunc)();
        pHandlerFunc = &WebViewCore::WebCoreThread::WvcWctHandler::HandleResumePriority;
        return *(Handle32*)&pHandlerFunc;
    }
    return NULL;
}

void WebViewCore::WebCoreThread::WvcWctHandler::HandleInitialize(
    /* [in] */ WebViewCore* webViewCore)
{
    AutoPtr<WebViewCore> core = webViewCore;
    core -> Initialize();
}

void WebViewCore::WebCoreThread::WvcWctHandler::HandleReducePriority()
{
    // 3 is an adjustable number.
    Process::SetThreadPriority(
                        Process::THREAD_PRIORITY_DEFAULT + 
                        3 * Process::THREAD_PRIORITY_LESS_FAVORABLE);
}

void WebViewCore::WebCoreThread::WvcWctHandler::HandleResumePriority()
{
    Process::SetThreadPriority(Process::THREAD_PRIORITY_DEFAULT);                    
}

WebViewCore::WebCoreThread::WvcWctHandler::WvcWctHandler()
{
    assert(SUCCEEDED(CApartment::GetMainApartment((IApartment**)&mApartment))
        && (mApartment != NULL));
}

/*****************************WebViewCore::WebCoreThread*****************************/
ECode WebViewCore::WebCoreThread::Run()
{
    //Looper::Prepare();    //JAVA:  os/Loop.java(public class)
    assert(sWebCoreHandler.Get() == NULL);  //JAVA:  Assert.assertNull(sWebCoreHandler);
    Core::Threading::Mutex::Autolock lock(WebViewCore::mMutexClass);
    WebViewCore::sWebCoreHandler = (IApartment*)(new WvcWctHandler());
    //WebViewCore.class.notify();
    //Looper::Loop();    //JAVA:  os/Loop.java(public class)
    return NOERROR;
}
/*****************************WebViewCore::EventHub::WvcEhHandler*****************************/
PInterface WebViewCore::EventHub::WvcEhHandler::Probe(
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

UInt32 WebViewCore::EventHub::WvcEhHandler::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 WebViewCore::EventHub::WvcEhHandler::Release()
{
    return ElRefBase::Release();
}

ECode WebViewCore::EventHub::WvcEhHandler::GetInterfaceID(
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


ECode WebViewCore::EventHub::WvcEhHandler::Start(
    /* [in] */ ApartmentAttr attr)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode WebViewCore::EventHub::WvcEhHandler::Finish()
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode WebViewCore::EventHub::WvcEhHandler::PostCppCallback(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallback(target, func, params, id);
}

ECode WebViewCore::EventHub::WvcEhHandler::PostCppCallbackAtTime(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id,
    /* [in] */ Millisecond64 uptimeMillis)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallbackAtTime(target, func, params, id, uptimeMillis);
}

ECode WebViewCore::EventHub::WvcEhHandler::PostCppCallbackDelayed(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id,
    /* [in] */ Millisecond64 delayMillis)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallbackDelayed(target, func, params, id, delayMillis);
}

ECode WebViewCore::EventHub::WvcEhHandler::PostCppCallbackAtFrontOfQueue(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallbackAtFrontOfQueue(target, func, params, id);
}

ECode WebViewCore::EventHub::WvcEhHandler::RemoveCppCallbacks(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func)
{
    assert(mApartment != NULL);
    return mApartment->RemoveCppCallbacks(target, func);
}

ECode WebViewCore::EventHub::WvcEhHandler::RemoveCppCallbacksEx(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ Int32 id)
{
    assert(mApartment != NULL);
    return mApartment->RemoveCppCallbacksEx(target, func, id);
}

ECode WebViewCore::EventHub::WvcEhHandler::HasCppCallbacks(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [out] */ Boolean* result)
{
    assert(mApartment != NULL);
    return mApartment->HasCppCallbacks(target, func, result);
}

ECode WebViewCore::EventHub::WvcEhHandler::HasCppCallbacksEx(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ Int32 id,
    /* [out] */ Boolean* result)
{
    assert(mApartment != NULL);
    return mApartment->HasCppCallbacksEx(target, func, id, result);
}

ECode WebViewCore::EventHub::WvcEhHandler::SendMessage(
    /* [in] */ Int32 message,
    /* [in] */ IParcel* params)
{
    /*
    if (DebugFlags::sWEB_VIEW_CORE) {
        Int32 arg1,arg2,obj;
        String strOut = ((message) < EventHub::REQUEST_LABEL)
                || ((message) > VALID_NODE_BOUNDS) ? String::FromInt32(message): String(HandlerDebugString[message - EventHub::REQUEST_LABEL]);
        strOut += String(" arg1=") + String::FromInt32(arg1);
        strOut += String(" arg2=") + String::FromInt32(arg2);
        strOut += String(" obj=") + String::FromInt32(obj);
        Utility::Logging::Logger::V(LOGTAG, strOut + String("\n") );
    }
    */
    if(message == EventHub::WEBKIT_DRAW )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)();
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleWebkitDraw;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::DESTROY )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)();
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleDestroy;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::REQUEST_LABEL )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(Int32,Int32);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleRequestLabel;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::UPDATE_FRAME_CACHE_IF_LOADING )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)();
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleUpdateFrameCacheIfLoading;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::SCROLL_TEXT_INPUT )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(Int32,Float);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleScrollTextInput;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::LOAD_URL )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(GetUrlData*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleLoadUrl;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::POST_URL )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(PostUrlData*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandlePostUrl;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::LOAD_DATA )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(BaseUrlData*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleLoadData;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::STOP_LOADING )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)();
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleStopLoading;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::RELOAD )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)();
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleReload;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::KEY_DOWN )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(IKeyEvent*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleKeyDown;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::KEY_UP )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(IKeyEvent*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleKeyUp;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::CLICK )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(Int32,Int32);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleClick;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::VIEW_SIZE_CHANGED )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(Handle32);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleViewSizeChanged;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::SET_SCROLL_OFFSET )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(Int32,CPoint*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleSetScrollOffset;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::SET_GLOBAL_BOUNDS )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(IRect*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleSetGlobalBounds;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::GO_BACK_FORWARD )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(Int32);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleGoBackForward;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::RESTORE_STATE )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(Int32);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleRestoreState;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::PAUSE_TIMERS )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)();
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandlePauseTimers;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::RESUME_TIMERS )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)();
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleResumeTimers;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::ON_PAUSE )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)();
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleOnPause;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::ON_RESUME )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)();
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleOnResume;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::FREE_MEMORY )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)();
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleFreeMemory;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::SET_NETWORK_STATE )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(Int32);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleSetNetworkState;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::SET_NETWORK_TYPE )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(IObjectStringMap*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleSetNetworkType;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::CLEAR_CACHE )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(Int32);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleClearCache;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::CLEAR_HISTORY )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)();
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleClearHistory;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::REPLACE_TEXT )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(Int32,Int32,ReplaceTextData*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleReplaceText;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::PASS_TO_JS )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(Int32,JSKeyData*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandlePassToJs;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::SAVE_DOCUMENT_STATE )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(CursorData*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleSaveDocumentState;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::CLEAR_SSL_PREF_TABLE )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)();
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleClearSslPrefTable;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::TOUCH_UP )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(TouchUpData*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleTouchUp;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::TOUCH_EVENT )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(TouchEventData*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleTouchEvent;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::SET_ACTIVE )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(Int32);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleSetActive;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::ADD_JS_INTERFACE )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(JSInterfaceData*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleAddJsInterface;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::REQUEST_EXT_REPRESENTATION )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(IApartment*,Int32,IParcel*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleRequestExtRepresentation;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::REQUEST_DOC_AS_TEXT )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(IApartment*,Int32,IParcel*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleRequestDocAsText;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::SET_MOVE_FOCUS )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(CursorData*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleSetMoveFocus;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::SET_MOVE_MOUSE )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(CursorData*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleSetMoveMouse;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::SET_MOVE_MOUSE_IF_LATEST )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(CursorData*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleSetMoveMouseIfLatest;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::REQUEST_CURSOR_HREF )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(Int32,Int32,IApartment*,Int32,IParcel*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleRequestCursorHref;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::UPDATE_CACHE_AND_TEXT_ENTRY )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)();
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleUpdateCacheAndTextEntry;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::DOC_HAS_IMAGES )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(IApartment*,Int32,IParcel*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleDocHasImages;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::DELETE_SELECTION )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(Int32,TextSelectionData*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleDeleteSelection;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::SET_SELECTION )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(Int32,Int32);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleSetSelection;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::LISTBOX_CHOICES )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(Int32/*, ISparseBooleanArray* */);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleListboxChoices;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::SINGLE_LISTBOX_CHOICE )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(Int32);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleSingleListboxChoices;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::SET_BACKGROUND_COLOR )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(Int32);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleSetBackgroundColor;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::DUMP_DOMTREE )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(Int32);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleDumpDomTree;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::DUMP_RENDERTREE )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(Int32);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleDumpRenderTree;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::DUMP_NAVTREE )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)();
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleDumpNavTree;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::DUMP_V8COUNTERS )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)();
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleDumpV8Counters;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::SET_JS_FLAGS )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(String);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleSetJsFlags;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::GEOLOCATION_PERMISSIONS_PROVIDE )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(GeolocationPermissionsData*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleGeolocationPermissionsProvide;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::SYNC_SCROLL )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(Int32, Int32);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleSyncScroll;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::SPLIT_PICTURE_SET )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)();
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleSplitPictureSet;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::CLEAR_CONTENT )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)();
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleClearContent;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::MESSAGE_RELAY )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(IApartment*,Int32,IParcel*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleMessageRelay;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::POPULATE_VISITED_LINKS )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(ArrayOf<String>*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandlePopulateVisitedLinks;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::VALID_NODE_BOUNDS )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(MotionUpData*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleValidNodeBounds;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::HIDE_FULLSCREEN )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(Int32);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleHideFullScreen;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::ADD_PACKAGE_NAMES )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(Set<String>*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleAddPackageNames;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::ADD_PACKAGE_NAME )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(String);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleAddPackageName;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    if(message == EventHub::REMOVE_PACKAGE_NAME )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(String);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleRemovePackageName;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);        
    }
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

Handle32 WebViewCore::EventHub::WvcEhHandler::GetFunc(
    /* [in] */ Int32 message)
{
    if(message == EventHub::WEBKIT_DRAW )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)();
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleWebkitDraw;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::DESTROY )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)();
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleDestroy;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::REQUEST_LABEL )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(Int32,Int32);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleRequestLabel;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::UPDATE_FRAME_CACHE_IF_LOADING )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)();
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleUpdateFrameCacheIfLoading;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::SCROLL_TEXT_INPUT )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(Int32,Float);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleScrollTextInput;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::LOAD_URL )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(GetUrlData*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleLoadUrl;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::POST_URL )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(PostUrlData*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandlePostUrl;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::LOAD_DATA )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(BaseUrlData*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleLoadData;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::STOP_LOADING )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)();
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleStopLoading;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::RELOAD )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)();
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleReload;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::KEY_DOWN )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(IKeyEvent*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleKeyDown;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::KEY_UP )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(IKeyEvent*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleKeyUp;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::CLICK )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(Int32,Int32);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleClick;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::VIEW_SIZE_CHANGED )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(Handle32);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleViewSizeChanged;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::SET_SCROLL_OFFSET )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(Int32,CPoint*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleSetScrollOffset;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::SET_GLOBAL_BOUNDS )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(IRect*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleSetGlobalBounds;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::GO_BACK_FORWARD )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(Int32);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleGoBackForward;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::RESTORE_STATE )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(Int32);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleRestoreState;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::PAUSE_TIMERS )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)();
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandlePauseTimers;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::RESUME_TIMERS )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)();
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleResumeTimers;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::ON_PAUSE )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)();
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleOnPause;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::ON_RESUME )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)();
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleOnResume;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::FREE_MEMORY )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)();
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleFreeMemory;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::SET_NETWORK_STATE )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(Int32);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleSetNetworkState;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::SET_NETWORK_TYPE )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(IObjectStringMap*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleSetNetworkType;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::CLEAR_CACHE )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(Int32);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleClearCache;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::CLEAR_HISTORY )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)();
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleClearHistory;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::REPLACE_TEXT )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(Int32,Int32,ReplaceTextData*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleReplaceText;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::PASS_TO_JS )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(Int32,JSKeyData*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandlePassToJs;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::SAVE_DOCUMENT_STATE )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(CursorData*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleSaveDocumentState;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::CLEAR_SSL_PREF_TABLE )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)();
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleClearSslPrefTable;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::TOUCH_UP )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(TouchUpData*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleTouchUp;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::TOUCH_EVENT )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(TouchEventData*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleTouchEvent;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::SET_ACTIVE )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(Int32);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleSetActive;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::ADD_JS_INTERFACE )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(JSInterfaceData*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleAddJsInterface;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::REQUEST_EXT_REPRESENTATION )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(IApartment*,Int32,IParcel*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleRequestExtRepresentation;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::REQUEST_DOC_AS_TEXT )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(IApartment*,Int32,IParcel*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleRequestDocAsText;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::SET_MOVE_FOCUS )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(CursorData*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleSetMoveFocus;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::SET_MOVE_MOUSE )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(CursorData*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleSetMoveMouse;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::SET_MOVE_MOUSE_IF_LATEST )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(CursorData*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleSetMoveMouseIfLatest;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::REQUEST_CURSOR_HREF )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(Int32,Int32,IApartment*,Int32,IParcel*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleRequestCursorHref;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::UPDATE_CACHE_AND_TEXT_ENTRY )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)();
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleUpdateCacheAndTextEntry;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::DOC_HAS_IMAGES )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(IApartment*,Int32,IParcel*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleDocHasImages;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::DELETE_SELECTION )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(Int32,TextSelectionData*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleDeleteSelection;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::SET_SELECTION )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(Int32,Int32);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleSetSelection;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::LISTBOX_CHOICES )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(Int32/*, ISparseBooleanArray* */);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleListboxChoices;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::SINGLE_LISTBOX_CHOICE )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(Int32);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleSingleListboxChoices;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::SET_BACKGROUND_COLOR )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(Int32);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleSetBackgroundColor;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::DUMP_DOMTREE )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(Int32);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleDumpDomTree;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::DUMP_RENDERTREE )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(Int32);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleDumpRenderTree;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::DUMP_NAVTREE )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)();
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleDumpNavTree;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::DUMP_V8COUNTERS )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)();
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleDumpV8Counters;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::SET_JS_FLAGS )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(String);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleSetJsFlags;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::GEOLOCATION_PERMISSIONS_PROVIDE )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(GeolocationPermissionsData*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleGeolocationPermissionsProvide;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::SYNC_SCROLL )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(Int32, Int32);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleSyncScroll;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::SPLIT_PICTURE_SET )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)();
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleSplitPictureSet;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::CLEAR_CONTENT )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)();
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleClearContent;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::MESSAGE_RELAY )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(IApartment*,Int32,IParcel*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleMessageRelay;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::POPULATE_VISITED_LINKS )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(ArrayOf<String>*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandlePopulateVisitedLinks;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::VALID_NODE_BOUNDS )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(MotionUpData*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleValidNodeBounds;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::HIDE_FULLSCREEN )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(Int32);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleHideFullScreen;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::ADD_PACKAGE_NAMES )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(Set<String>*);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleAddPackageNames;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::ADD_PACKAGE_NAME )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(String);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleAddPackageName;
        return *(Handle32*)&pHandlerFunc;
    }
    if(message == EventHub::REMOVE_PACKAGE_NAME )
    {
        void (STDCALL WebViewCore::EventHub::WvcEhHandler::*pHandlerFunc)(String);
        pHandlerFunc = &WebViewCore::EventHub::WvcEhHandler::HandleRemovePackageName;
        return *(Handle32*)&pHandlerFunc;
    }
    return NULL;
}

WebViewCore::EventHub::WvcEhHandler::WvcEhHandler(
    /* [in] */ WebViewCore* webViewCore,
    /* [in] */ EventHub* eventHub)
{
    assert(SUCCEEDED(CApartment::GetMainApartment((IApartment**)&mApartment))
        && (mApartment != NULL));
    mWebViewCore = webViewCore;
    mEventHub = eventHub;
}

//EventHub::WEBKIT_DRAW
void WebViewCore::EventHub::WvcEhHandler::HandleWebkitDraw()
{
    mWebViewCore -> WebkitDraw();
}

//EventHub::DESTROY
void WebViewCore::EventHub::WvcEhHandler::HandleDestroy()
{
    // Time to take down the world. Cancel all pending
    // loads and destroy the native view and frame.
    if(TRUE)
    {
        Core::Threading::Mutex::Autolock lock( mWebViewCore -> mutexThis );
        (mWebViewCore -> mBrowserFrame)->Destroy();
        (mWebViewCore -> mBrowserFrame) = NULL;
        (mWebViewCore -> mSettings)->OnDestroyed();
        (mWebViewCore -> mNativeClass) = 0;
        (mWebViewCore -> mWebView) = NULL;
    }
}

//EventHub::REQUEST_LABEL
void WebViewCore::EventHub::WvcEhHandler::HandleRequestLabel(
    /* [in] */ Int32 arg1, 
    /* [in] */ Int32 arg2)
{
    if ((mWebViewCore->mWebView).Get() != NULL) {
        Int32 nodePointer = arg2;
        String label = mWebViewCore->NativeRequestLabel(arg1, nodePointer);
        if (label.GetLength() > 0) {
            //JAVA:  Message.obtain(mWebView.mPrivateHandler, WebView.RETURN_LABEL, nodePointer, 0, label).sendToTarget();
            void (STDCALL CWebView::*pHandlerFunc)(Int32,String);
            //pHandlerFunc = &CWebView::HandleReturnLabel;
            AutoPtr<IParcel> params;
            CCallbackParcel::New((IParcel**)&params);
            params->WriteInt32(nodePointer);
            params->WriteString(label);
            (((CWebView*)(mWebViewCore->mWebView.Get()))->mPrivateHandler)->SendMessage(CWebView::RETURN_LABEL,params);
            //(((CWebView*)(mWebView.Get()))->mPrivateHandler)->PostCppCallback( (Handle32)((CWebView*)(mWebView.Get())), *(Handle32*)&pHandlerFunc, params, 0);
        }
    }
}

//EventHub::UPDATE_FRAME_CACHE_IF_LOADING
void WebViewCore::EventHub::WvcEhHandler::HandleUpdateFrameCacheIfLoading()
{
    mWebViewCore -> NativeUpdateFrameCacheIfLoading();
}

//EventHub::SCROLL_TEXT_INPUT
void WebViewCore::EventHub::WvcEhHandler::HandleScrollTextInput(
    /* [in] */ Int32 arg1,
    /* [in] */ Float obj)
{
    mWebViewCore -> NativeScrollFocusedTextInput( obj, arg1);
}

//EventHub::LOAD_URL
void WebViewCore::EventHub::WvcEhHandler::HandleLoadUrl(
    /* [in] */ GetUrlData* obj)
{
    GetUrlData* param = obj;
    mWebViewCore -> LoadUrl(param->mUrl, param->mExtraHeaders);
}

//EventHub::POST_URL
void WebViewCore::EventHub::WvcEhHandler::HandlePostUrl(
    /* [in] */ PostUrlData* obj)
{
    PostUrlData* param = obj;
    mWebViewCore -> mBrowserFrame->PostUrl(param->mUrl, *(param->mPostData));
}

//EventHub::LOAD_DATA
void WebViewCore::EventHub::WvcEhHandler::HandleLoadData(
    /* [in] */ BaseUrlData* obj)
{
    BaseUrlData* loadParams = obj;
    String baseUrl = loadParams->mBaseUrl;
    if (baseUrl.GetLength() != 0) {
        Int32 i = baseUrl.IndexOf(':');
        if (i > 0) {
            /*
             * In 1.0, {@link
             * WebView#loadDataWithBaseURL} can access
             * local asset files as long as the data is
             * valid. In the new WebKit, the restriction
             * is tightened. To be compatible with 1.0,
             * we automatically add the scheme of the
             * baseUrl for local access as long as it is
             * not http(s)/ftp(s)/about/javascript
             */
            String scheme = baseUrl.Substring(0, i);
            if (!scheme.StartWith("http") &&
                    !scheme.StartWith("ftp") &&
                    !scheme.StartWith("about") &&
                    !scheme.StartWith("javascript")) {
                mWebViewCore->NativeRegisterURLSchemeAsLocal(scheme);
            }
        }
    }
    (mWebViewCore->mBrowserFrame)->LoadData(baseUrl,
            loadParams->mData,
            loadParams->mMimeType,
            loadParams->mEncoding,
            loadParams->mHistoryUrl);
}

//EventHub::STOP_LOADING
void WebViewCore::EventHub::WvcEhHandler::HandleStopLoading()
{
    // If the WebCore has committed the load, but not
    // finished the first layout yet, we need to set
    // first layout done to trigger the interpreted side sync
    // up with native side
    if (((CBrowserFrame*)((mWebViewCore->mBrowserFrame).Get()))->Committed()
            && !((CBrowserFrame*)((mWebViewCore->mBrowserFrame).Get()))->FirstLayoutDone()) {
        ((CBrowserFrame*)((mWebViewCore->mBrowserFrame).Get()))->DidFirstLayout();
    }
    // Do this after syncing up the layout state.
    mWebViewCore -> StopLoading();    
}

//EventHub::RELOAD
void WebViewCore::EventHub::WvcEhHandler::HandleReload()
{
    (mWebViewCore->mBrowserFrame)->Reload(FALSE);
}

//EventHub::KEY_DOWN
void WebViewCore::EventHub::WvcEhHandler::HandleKeyDown(
    /* [in] */ IKeyEvent* obj)
{
    mWebViewCore -> Key(obj, TRUE);
}


//EventHub::KEY_UP
void WebViewCore::EventHub::WvcEhHandler::HandleKeyUp(
    /* [in] */ IKeyEvent* obj)
{
    mWebViewCore->Key(obj, FALSE);
}

//EventHub::CLICK:
void WebViewCore::EventHub::WvcEhHandler::HandleClick(
    /* [in] */ Int32 arg1, 
    /* [in] */ Int32 arg2)
{
    mWebViewCore -> NativeClick(arg1, arg2);
}

//EventHub::VIEW_SIZE_CHANGED
void WebViewCore::EventHub::WvcEhHandler::HandleViewSizeChanged(
    /* [in] */ /*CWebView::ViewSizeData* */Handle32 obj)
{
    CWebView::ViewSizeData* data = (CWebView::ViewSizeData*)obj;
    mWebViewCore -> ViewSizeChanged(data->mWidth, data->mHeight,
            data->mTextWrapWidth, data->mScale,
            data->mAnchorX, data->mAnchorY,
            data->mIgnoreHeight);
}

//EventHub::SET_SCROLL_OFFSET
void WebViewCore::EventHub::WvcEhHandler::HandleSetScrollOffset(
    /* [in] */ Int32 arg1, 
    /* [in] */ CPoint* obj)
{
    // note: these are in document coordinates
    // (inv-zoom)
    CPoint* pt = obj;
    mWebViewCore -> NativeSetScrollOffset(arg1, pt->mX, pt->mY);
}

//EventHub::SET_GLOBAL_BOUNDS
void WebViewCore::EventHub::WvcEhHandler::HandleSetGlobalBounds(
    /* [in] */ IRect* obj)
{
    AutoPtr<IRect> r = obj;
    Int32 left, top, width, height;
    r->GetLeft(&left);
    r->GetTop(&top);
    r->GetWidth(&width);
    r->GetHeight(&height);
    mWebViewCore -> NativeSetGlobalBounds(left, top, width, height);
}

//EventHub::GO_BACK_FORWARD
void WebViewCore::EventHub::WvcEhHandler::HandleGoBackForward(
    /* [in] */ Int32 arg1)
{
    // If it is a standard load and the load is not
    // committed yet, we interpret BACK as RELOAD
    if (!((CBrowserFrame*)((mWebViewCore->mBrowserFrame).Get()))->Committed() 
        && (arg1) == -1 
        && (((CBrowserFrame*)((mWebViewCore->mBrowserFrame).Get()))->LoadType() == CBrowserFrame::FRAME_LOADTYPE_STANDARD)) {
        (mWebViewCore->mBrowserFrame)->Reload(TRUE);
    } else {
        (mWebViewCore->mBrowserFrame)->GoBackOrForward(arg1);
    }
}

//EventHub::RESTORE_STATE
void WebViewCore::EventHub::WvcEhHandler::HandleRestoreState(
    /* [in] */ Int32 arg1)
{
    mWebViewCore->StopLoading();
    mWebViewCore->_RestoreState(arg1);
}

//EventHub::PAUSE_TIMERS
void WebViewCore::EventHub::WvcEhHandler::HandlePauseTimers()
{    
    mEventHub->mSavedPriority = Process::GetThreadPriority(mEventHub->mTid);
    Process::SetThreadPriority(mEventHub->mTid, Process::THREAD_PRIORITY_BACKGROUND);
    mWebViewCore -> PauseTimers();
    //JAVA:  WebViewWorker.getHandler().sendEmptyMessage( WebViewWorker.MSG_PAUSE_CACHE_TRANSACTION);
    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    //(WebViewWorker::GetHandler())->SendMessage(WebViewWorker::MSG_PAUSE_CACHE_TRANSACTION,params);    
}

//EventHub::RESUME_TIMERS
void WebViewCore::EventHub::WvcEhHandler::HandleResumeTimers()
{
    Process::SetThreadPriority(mEventHub->mTid, mEventHub->mSavedPriority);
    mWebViewCore->ResumeTimers();
    //JAVA:  WebViewWorker.getHandler().sendEmptyMessage( WebViewWorker.MSG_RESUME_CACHE_TRANSACTION);
    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    //(WebViewWorker::GetHandler()) -> SendMessage(WebViewWorker::MSG_RESUME_CACHE_TRANSACTION,params);
}

//EventHub::ON_PAUSE
void WebViewCore::EventHub::WvcEhHandler::HandleOnPause()
{
    mWebViewCore -> NativePause();
}

//EventHub::ON_RESUME
void WebViewCore::EventHub::WvcEhHandler::HandleOnResume()
{
    mWebViewCore -> NativeResume();
}

//EventHub::FREE_MEMORY
void WebViewCore::EventHub::WvcEhHandler::HandleFreeMemory()
{
    mWebViewCore -> ClearCache(false);
    mWebViewCore -> NativeFreeMemory();    
}

//EventHub::SET_NETWORK_STATE
void WebViewCore::EventHub::WvcEhHandler::HandleSetNetworkState(
    /* [in] */ Int32 arg1)
{
    if ((CBrowserFrame::sJavaBridge).Get() == NULL) {
        String strOut = String("No WebView has been created in this process!");
        Utility::Logging::Logger::V(LOGTAG, strOut + String("\n") );
        //return E_ILLEGAL_STATE_EXCEPTION;
        return ;
    }
    (CBrowserFrame::sJavaBridge)->SetNetworkOnLine(arg1);
}

//EventHub::SET_NETWORK_TYPE
void WebViewCore::EventHub::WvcEhHandler::HandleSetNetworkType(
    /* [in] */ IObjectStringMap* obj)
{
    if ((CBrowserFrame::sJavaBridge).Get() == NULL) {
        String strOut = String("No WebView has been created in this process!");
        Utility::Logging::Logger::V(LOGTAG, strOut + String("\n") );
        //return E_ILLEGAL_STATE_EXCEPTION;
        return ;
    }
    AutoPtr<IObjectStringMap> map = obj;
    String* type = NULL;
    String* subtype = NULL;
    map->Get(String("type"), (IInterface**)&type);
    map->Get(String("subtype"), (IInterface**)&subtype);
    assert(type != NULL);
    assert(subtype != NULL);
    (CBrowserFrame::sJavaBridge)->SetNetworkType(*type, *subtype);
}

//EventHub::CLEAR_CACHE
void WebViewCore::EventHub::WvcEhHandler::HandleClearCache(
    /* [in] */ Int32 arg1)
{
    mWebViewCore -> ClearCache(arg1);
}

//EventHub::CLEAR_HISTORY
void WebViewCore::EventHub::WvcEhHandler::HandleClearHistory()
{
    AutoPtr<IWebBackForwardList> list;
    (mWebViewCore -> mCallbackProxy)->GetBackForwardList((IWebBackForwardList**)&list);
    ((CWebBackForwardList*)(list.Get()))->Close(((CBrowserFrame*)((mWebViewCore->mBrowserFrame).Get()))->mNativeFrame);
}

//EventHub::REPLACE_TEXT
void WebViewCore::EventHub::WvcEhHandler::HandleReplaceText(
    /* [in] */ Int32 arg1, 
    /* [in] */ Int32 arg2, 
    /* [in] */ ReplaceTextData* obj)
{
    ReplaceTextData* rep = (ReplaceTextData*)(obj);
    mWebViewCore -> NativeReplaceTextfieldText(arg1, arg2, rep->mReplace, rep->mNewStart, rep->mNewEnd, rep->mTextGeneration);
}

//EventHub::PASS_TO_JS
void WebViewCore::EventHub::WvcEhHandler::HandlePassToJs(
    /* [in] */ Int32 arg1, 
    /* [in] */ JSKeyData* obj)
{
    JSKeyData* jsData = obj;
    AutoPtr<IKeyEvent> evt = jsData->mEvent;
    Int32 keyCode, keyValue;
    evt->GetKeyCode(&keyCode);
    evt->GetKeyCode(&keyValue);
    Int32 generation = arg1;                    
    Boolean down, shiftPressed, altPressed, symPressed;
    evt->IsDown(&down);
    evt->IsShiftPressed(&shiftPressed);
    evt->IsAltPressed(&altPressed);
    evt->IsSymPressed(&symPressed);
    mWebViewCore -> PassToJs(generation,
            jsData->mCurrentText,
            keyCode,
            keyValue,
            down,
            shiftPressed,
            altPressed,
            symPressed);    
}

//EventHub::SAVE_DOCUMENT_STATE
void WebViewCore::EventHub::WvcEhHandler::HandleSaveDocumentState(
    /* [in] */ CursorData* obj)
{
    CursorData* cDat = obj;
    mWebViewCore -> NativeSaveDocumentState(cDat->mFrame);
}

//EventHub::CLEAR_SSL_PREF_TABLE
void WebViewCore::EventHub::WvcEhHandler::HandleClearSslPrefTable()
{
    (Network::GetInstance(mWebViewCore->mContext))->ClearUserSslPrefTable();
}

//EventHub::TOUCH_UP
void WebViewCore::EventHub::WvcEhHandler::HandleTouchUp(
    /* [in] */ TouchUpData* obj)
{
    TouchUpData* touchUpData = obj;
    mWebViewCore -> NativeTouchUp(touchUpData->mMoveGeneration,
            touchUpData->mFrame, touchUpData->mNode,
            touchUpData->mX, touchUpData->mY);
}

//EventHub::TOUCH_EVENT
void WebViewCore::EventHub::WvcEhHandler::HandleTouchEvent(
    /* [in] */ TouchEventData* obj)
{
    TouchEventData* ted = obj;
    //JAVA:  Message.obtain(mWebView.mPrivateHandler, WebView.PREVENT_TOUCH_ID, ted.mAction, nativeHandleTouchEvent(ted.mAction, ted.mX, ted.mY, ted.mMetaState) ? 1 : 0, ted.mReprocess ? ted : null).sendToTarget();
    Boolean bNativeHandleTouchEvent = mWebViewCore -> NativeHandleTouchEvent(ted->mAction, ted->mX, ted->mY, ted->mMetaState);
    Int32 arg2P = bNativeHandleTouchEvent ? 1 : 0 ;
    TouchEventData* objP = ted -> mReprocess ? ted : NULL ;
    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32(ted -> mAction);
    params->WriteInt32(arg2P);
    params->WriteStruct((Handle32)objP,sizeof(TouchEventData));   //WriteInterfacePtr WriteStructPtr
    (((CWebView*)(mWebViewCore->mWebView).Get()) -> mPrivateHandler)->SendMessage(CWebView::PREVENT_TOUCH_ID, params);
}

//EventHub::SET_ACTIVE
void WebViewCore::EventHub::WvcEhHandler::HandleSetActive(
    /* [in] */ Int32 arg1)
{
    mWebViewCore -> NativeSetFocusControllerActive(arg1 == 1);
}

//EventHub::ADD_JS_INTERFACE
void WebViewCore::EventHub::WvcEhHandler::HandleAddJsInterface(
    /* [in] */ JSInterfaceData* obj)
{
    JSInterfaceData* jsData = obj;
    (mWebViewCore->mBrowserFrame)->AddJavascriptInterface(jsData->mObject, jsData->mInterfaceName);    
}

//EventHub::REQUEST_EXT_REPRESENTATION
void WebViewCore::EventHub::WvcEhHandler::HandleRequestExtRepresentation(
    /* [in] */ IApartment* apartmentR,
    /* [in] */ Int32 message,
    /* [in] */ IParcel* params)
{
    //JAVA:  mBrowserFrame.externalRepresentation( (Message) msg.obj);
    //(mWebViewCore->mBrowserFrame)->ExternalRepresentation(apartmentR,message,params);
}

//EventHub::REQUEST_DOC_AS_TEXT
void WebViewCore::EventHub::WvcEhHandler::HandleRequestDocAsText(
    /* [in] */ IApartment* apartmentR,
    /* [in] */ Int32 message,
    /* [in] */ IParcel* params)
{
    //JAVA:  mBrowserFrame.documentAsText((Message) msg.obj);
    //(mWebViewCore->mBrowserFrame) -> DocumentAsText(apartmentR,message,params);
}

//EventHub::SET_MOVE_FOCUS
void WebViewCore::EventHub::WvcEhHandler::HandleSetMoveFocus(
    /* [in] */ CursorData* obj)
{
    CursorData* focusData = obj;
    mWebViewCore-> NativeMoveFocus(focusData->mFrame, focusData->mNode);
}

//EventHub::SET_MOVE_MOUSE
void WebViewCore::EventHub::WvcEhHandler::HandleSetMoveMouse(
    /* [in] */ CursorData* obj)
{
    CursorData* cursorData = obj;
    mWebViewCore-> NativeMoveMouse(cursorData->mFrame, cursorData->mX, cursorData->mY);
}

//EventHub::SET_MOVE_MOUSE_IF_LATEST
void WebViewCore::EventHub::WvcEhHandler::HandleSetMoveMouseIfLatest(
    /* [in] */ CursorData* obj)
{
    CursorData* cData = obj;
    mWebViewCore-> NativeMoveMouseIfLatest(cData->mMoveGeneration, cData->mFrame,
            cData->mX, cData->mY);    
}

//EventHub::REQUEST_CURSOR_HREF
void WebViewCore::EventHub::WvcEhHandler::HandleRequestCursorHref(
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2,    
    /* [in] */ IApartment* apartmentR,
    /* [in] */ Int32 message,
    /* [in] */ IParcel* params)
{
    //JAVA:  
    //Message hrefMsg = (Message) msg.obj;
    //hrefMsg.getData().putString("url", nativeRetrieveHref(msg.arg1, msg.arg2));
    //hrefMsg.getData().putString("title", nativeRetrieveAnchorText(msg.arg1, msg.arg2));
    //hrefMsg.sendToTarget();
    AutoPtr<IBundle> bundleT;
    CBundle::New( (IBundle**)&bundleT );
    bundleT->PutString(String("url"),mWebViewCore->NativeRetrieveHref(arg1, arg2));
    bundleT->PutString(String("title"),mWebViewCore->NativeRetrieveAnchorText(arg1, arg2));
    params->WriteInterfacePtr(bundleT.Get());
    apartmentR->SendMessage(message,params);
}

//EventHub::UPDATE_CACHE_AND_TEXT_ENTRY
void WebViewCore::EventHub::WvcEhHandler::HandleUpdateCacheAndTextEntry()
{
    mWebViewCore->NativeUpdateFrameCache();
    // FIXME: this should provide a minimal rectangle
    if ((mWebViewCore->mWebView).Get() != NULL) {
        (mWebViewCore->mWebView)->PostInvalidate();
    }
    mWebViewCore->SendUpdateTextEntry();
}

//EventHub::DOC_HAS_IMAGES
void WebViewCore::EventHub::WvcEhHandler::HandleDocHasImages(
    /* [in] */ IApartment* apartmentR,
    /* [in] */ Int32 message,
    /* [in] */ IParcel* params)
{
    //JAVA:  
    //Message imageResult = (Message) msg.obj;
    //imageResult.arg1 = mBrowserFrame.documentHasImages() ? 1 : 0;
    //imageResult.sendToTarget();
    Boolean bDocumentHasImages = FALSE;
    (mWebViewCore -> mBrowserFrame) -> DocumentHasImages(&bDocumentHasImages);
    params->WriteInt32(bDocumentHasImages ? 1 : 0);
    apartmentR->SendMessage(message, params);
}

//EventHub::DELETE_SELECTION
void WebViewCore::EventHub::WvcEhHandler::HandleDeleteSelection(
    /* [in] */ Int32 arg1,
    /* [in] */ TextSelectionData* obj)
{
    TextSelectionData* deleteSelectionData = obj;
    mWebViewCore -> NativeDeleteSelection(deleteSelectionData->mStart,
            deleteSelectionData->mEnd, arg1);
}

//EventHub::SET_SELECTION
void WebViewCore::EventHub::WvcEhHandler::HandleSetSelection(
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2)
{
    mWebViewCore -> NativeSetSelection(arg1, arg2);
}

//EventHub::LISTBOX_CHOICES
void WebViewCore::EventHub::WvcEhHandler::HandleListboxChoices(
    /* [in] */ Int32 arg1//,
    /* [in] */ /*ISparseBooleanArray* obj*/)
{
    //AutoPtr<ISparseBooleanArray> choices = obj;
    Int32 choicesSize = arg1;
    AutoFree < ArrayOf<Boolean> > choicesArray = ArrayOf<Boolean>::Alloc(choicesSize);
    for (Int32 c = 0; c < choicesSize; c++) {
        //choices -> Get(c,&(choicesArray[c]));
    }
    mWebViewCore -> NativeSendListBoxChoices(choicesArray, choicesSize);
}

//EventHub::SINGLE_LISTBOX_CHOICE
void WebViewCore::EventHub::WvcEhHandler::HandleSingleListboxChoices(
    /* [in] */ Int32 arg1)
{
    mWebViewCore -> NativeSendListBoxChoice(arg1);
}

//EventHub::SET_BACKGROUND_COLOR
void WebViewCore::EventHub::WvcEhHandler::HandleSetBackgroundColor(
    /* [in] */ Int32 arg1)
{
    mWebViewCore -> NativeSetBackgroundColor(arg1);
}

//EventHub::DUMP_DOMTREE
void WebViewCore::EventHub::WvcEhHandler::HandleDumpDomTree(
    /* [in] */ Int32 arg1)
{
    mWebViewCore -> NativeDumpDomTree(arg1== 1);
}

//EventHub::DUMP_RENDERTREE
void WebViewCore::EventHub::WvcEhHandler::HandleDumpRenderTree(
    /* [in] */ Int32 arg1)
{
    mWebViewCore -> NativeDumpRenderTree(arg1 == 1);
}

//EventHub::DUMP_NAVTREE
void WebViewCore::EventHub::WvcEhHandler::HandleDumpNavTree()
{
    mWebViewCore -> NativeDumpNavTree();
}

//DUMP_V8COUNTERS
void WebViewCore::EventHub::WvcEhHandler::HandleDumpV8Counters()
{
    mWebViewCore -> NativeDumpV8Counters();
}

//EventHub::SET_JS_FLAGS
void WebViewCore::EventHub::WvcEhHandler::HandleSetJsFlags(
    /* [in] */ String obj)
{
    mWebViewCore -> NativeSetJsFlags(obj);
}

//EventHub::GEOLOCATION_PERMISSIONS_PROVIDE
void WebViewCore::EventHub::WvcEhHandler::HandleGeolocationPermissionsProvide(
    /* [in] */ GeolocationPermissionsData* obj)
{
    GeolocationPermissionsData* data = obj;
    mWebViewCore -> NativeGeolocationPermissionsProvide(data->mOrigin,
            data->mAllow, data->mRemember);
}

//EventHub::SYNC_SCROLL
void WebViewCore::EventHub::WvcEhHandler::HandleSyncScroll(
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2)
{
    mWebViewCore -> mWebkitScrollX = arg1;
    mWebViewCore -> mWebkitScrollY = arg2;
}

//EventHub::SPLIT_PICTURE_SET
void WebViewCore::EventHub::WvcEhHandler::HandleSplitPictureSet()
{
    mWebViewCore -> NativeSplitContent();
    mWebViewCore -> mSplitPictureIsScheduled = FALSE;
}

//EventHub::CLEAR_CONTENT
void WebViewCore::EventHub::WvcEhHandler::HandleClearContent()
{
    // Clear the view so that onDraw() will draw nothing
    // but white background
    // (See public method WebView.clearView)
    mWebViewCore -> NativeClearContent();    
}

//EventHub::MESSAGE_RELAY
void WebViewCore::EventHub::WvcEhHandler::HandleMessageRelay(
    /* [in] */ IApartment* apartmentR,
    /* [in] */ Int32 message,
    /* [in] */ IParcel* params)
{
    //JAVA:  if (msg.obj instanceof Message) { ((Message) msg.obj).sendToTarget(); }
    apartmentR->SendMessage(message, params);
}

//EventHub::POPULATE_VISITED_LINKS
void WebViewCore::EventHub::WvcEhHandler::HandlePopulateVisitedLinks(
    /* [in] */ ArrayOf<String>* obj)
{
    mWebViewCore -> NativeProvideVisitedHistory(obj);
}

//EventHub::VALID_NODE_BOUNDS
void WebViewCore::EventHub::WvcEhHandler::HandleValidNodeBounds(
    /* [in] */ MotionUpData* obj)
{
    MotionUpData* motionUpData = obj;
    Boolean bNativeValidNodeAndBounds;
    bNativeValidNodeAndBounds = mWebViewCore -> NativeValidNodeAndBounds(
            motionUpData->mFrame, motionUpData->mNode,
            motionUpData->mBounds);
    if (!bNativeValidNodeAndBounds) {
        mWebViewCore -> NativeUpdateFrameCache();
    }

    //Message message = mWebView.mPrivateHandler.obtainMessage(WebView.DO_MOTION_UP, motionUpData.mX, motionUpData.mY);
    //mWebView.mPrivateHandler.sendMessageAtFrontOfQueue(message);

    AutoPtr<IApartment> handlerT = ((CWebView*)((mWebViewCore->mWebView).Get())) -> mPrivateHandler;
    void (STDCALL CWebView::PrivateHandler::*pHandlerFunc)();
    //pHandlerFunc = &CWebView::PrivateHandler::HandleDoMotionUp;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32(motionUpData->mX);
    params->WriteInt32(motionUpData->mY);
    handlerT -> PostCppCallbackAtFrontOfQueue((Handle32)(handlerT.Get()), *(Handle32*)&pHandlerFunc, params, 0);
}

//EventHub::HIDE_FULLSCREEN
void WebViewCore::EventHub::WvcEhHandler::HandleHideFullScreen(
    /* [in] */ Int32 arg1)
{
    mWebViewCore -> NativeFullScreenPluginHidden(arg1);
}

//EventHub::ADD_PACKAGE_NAMES
void WebViewCore::EventHub::WvcEhHandler::HandleAddPackageNames(
    /* [in] */ Set<String>* obj)
{
    if (CBrowserFrame::sJavaBridge == NULL) {
        Utility::Logging::Logger::E(LOGTAG, String("No WebView has been created in this process!\n") );
        //return E_ILLEGAL_STATE_EXCEPTION;
        return ;
    }
    (CBrowserFrame::sJavaBridge) -> AddPackageNames( (Set<String>*)(obj) );
}

//EventHub::ADD_PACKAGE_NAME
void WebViewCore::EventHub::WvcEhHandler::HandleAddPackageName(
    /* [in] */ String obj)
{
    if (CBrowserFrame::sJavaBridge == NULL) {
        Utility::Logging::Logger::E(LOGTAG, String("No WebView has been created in this process!\n") );
        //return E_ILLEGAL_STATE_EXCEPTION;
        return;
    }
    (CBrowserFrame::sJavaBridge) -> AddPackageName((String)(obj));
}

//EventHub::REMOVE_PACKAGE_NAME
void WebViewCore::EventHub::WvcEhHandler::HandleRemovePackageName(
    /* [in] */ String obj)
{    
    if (CBrowserFrame::sJavaBridge == NULL) {
        Utility::Logging::Logger::E(LOGTAG, String("No WebView has been created in this process!\n") );
        //return E_ILLEGAL_STATE_EXCEPTION;
        return;
    }
    (CBrowserFrame::sJavaBridge) -> RemovePackageName( (String)(obj) );
}

/*****************************WebViewCore::EventHub*****************************/

WebViewCore::EventHub::EventHub(
    /* [in] */ WebViewCore* webViewCore)
{
    mWebViewCore = webViewCore;
}

/**
 * Transfer all messages to the newly created webcore thread handler.
 */
void WebViewCore::EventHub::TransferMessages()
{
    mTid = Process::MyTid();
    mSavedPriority = Process::GetThreadPriority(mTid);
    mHandler = (IApartment*)(new WvcEhHandler(mWebViewCore,this));
    // Take all queued messages and resend them to the new handler.
    //synchronized (this) 
    mutexThis.Lock();
    {
        AutoPtr<Msg> msg;
        List<AutoPtr<Msg> >::Iterator iterT;
        List<AutoPtr<Msg> >::Iterator iterE;
        iterT = mMessages -> Begin();
        iterE = mMessages -> End();
        for (;iterT != iterE; iterT ++) {
            msg = *iterT;
            mHandler->SendMessage(msg->message, (msg->params).Get());
        }
        mMessages->Clear();        
        mMessages = NULL;
    }
    mutexThis.Unlock();
}

/**
 * Send a message internally to the queue or to the handler
 *
 * synchronized
 */
void WebViewCore::EventHub::SendMessage(
    /* [in] */ Msg* msg)
{
    Core::Threading::Mutex::Autolock lock(mutexThis);
    if (mBlockMessages) {
        return;
    }
    if (mMessages != NULL) {
        mMessages->PushBack(msg);
    }
    else {
        mHandler->SendMessage(msg->message, (msg->params).Get());
    }
}

/* synchronized */
void WebViewCore::EventHub::RemoveMessages(
    /* [in] */ Int32 message)
{
    Core::Threading::Mutex::Autolock lock(mutexThis);
    if (mBlockMessages) {
        return;
    }
    if (message == EventHub::WEBKIT_DRAW) {
        mWebViewCore -> mDrawIsScheduled = FALSE;
    }
    if (mMessages != NULL) {
        Utility::Logging::Logger::W(LOGTAG, String("Not supported in this case.\n") );
    } 
    else {
        mHandler->RemoveCppCallbacks((Handle32)(mHandler.Get()), ((EventHub::WvcEhHandler*)(mHandler.Get()))->GetFunc(message));
    }
}

/* synchronized */
Boolean WebViewCore::EventHub::HasMessages(
    /* [in] */ Int32 message)
{
    Core::Threading::Mutex::Autolock lock(mutexThis);
    if (mBlockMessages) {
        return FALSE;
    }
    if (mMessages != NULL) {
        Utility::Logging::Logger::W(LOGTAG, String("hasMessages() is not supported in this case.\n") );
        return FALSE;
    } 
    else {
        Boolean bHasMessages;
        mHandler->HasCppCallbacks((Handle32)(mHandler.Get()), ((EventHub::WvcEhHandler*)(mHandler.Get()))->GetFunc(message), &bHasMessages);        
        return bHasMessages;
    }
}

/* synchronized */
void WebViewCore::EventHub::SendMessageDelayed(
    /* [in] */ Msg* msg, 
    /* [in] */ Int64 delay)
{
    Core::Threading::Mutex::Autolock lock(mutexThis);
    if (mBlockMessages) {
        return;
    }    
    mHandler->PostCppCallbackDelayed((Handle32)(mHandler.Get()), ((EventHub::WvcEhHandler*)(mHandler.Get()))->GetFunc(msg->message), (msg->params).Get(), 0, delay);
}

/**
 * Send a message internally to the front of the queue.
 * 
 * synchronized
 */
void WebViewCore::EventHub::SendMessageAtFrontOfQueue(
    /* [in] */ Msg* msg)
{
    Core::Threading::Mutex::Autolock lock(mutexThis);
    if (mBlockMessages) {
        return;
    }
    if (mMessages != NULL) {
        mMessages -> PushFront(msg);
    } 
    else {
        mHandler->PostCppCallbackAtFrontOfQueue((Handle32)(mHandler.Get()), ((EventHub::WvcEhHandler*)(mHandler.Get()))->GetFunc(msg->message), (msg->params).Get(), 0);
    }
}

/**
 * Remove all the messages.
 *
 * synchronized
 */
void WebViewCore::EventHub::RemoveMessages()
{
    Core::Threading::Mutex::Autolock lock(mutexThis);
    // reset mDrawIsScheduled flag as WEBKIT_DRAW may be removed
    mWebViewCore -> mDrawIsScheduled = FALSE;
    mWebViewCore -> mSplitPictureIsScheduled = FALSE;
    if (mMessages != NULL) {
        mMessages -> Clear();
    } 
    else {
        //mHandler.removeCallbacksAndMessages(null);
        mHandler -> RemoveCppCallbacks((Handle32)(mHandler.Get()), NULL);
    }
}

/**
 * Block sending messages to the EventHub.
 *
 * synchronized
 */
void WebViewCore::EventHub::BlockMessages()
{
    Core::Threading::Mutex::Autolock lock(mutexThis);
    mBlockMessages = TRUE;
}

/*****************************WebViewCore*****************************/
WebViewCore::WebViewCore(
    /* [in] */ IContext* context, 
    /* [in] */ IWebView* w, 
    /* [in] */ ICallbackProxy* proxy,
    /* [in] */ IObjectStringMap* javascriptInterfaces)
{
    // No need to assign this in the WebCore thread.
    mCallbackProxy = proxy;
    mWebView = w;
    mJavascriptInterfaces = javascriptInterfaces;
    // This context object is used to initialize the WebViewCore during
    // subwindow creation.
    mContext = context;

    // We need to wait for the initial thread creation before sending
    // a message to the WebCore thread.
    // XXX: This is the only time the UI thread will wait for the WebCore
    // thread!
    if(TRUE)
    {
        Core::Threading::Mutex::Autolock lock(mMutexClass);
        if(sWebCoreHandler == NULL)
        {
            AutoPtr<IThread> t;
            AutoPtr<IRunnable> runnableT = (IRunnable*)(new WebCoreThread()); 
            CThread::New( runnableT, (IThread**)&t);
            t -> SetName(String(THREAD_NAME));
            t -> Start();
            /*
            try {
                WebViewCore.class.wait();
            } catch (InterruptedException e) {
                Utility::Logging::Logger::E(LOGTAG, String("Caught exception while waiting for thread creation.\n") );
                Log.e(LOGTAG, "Caught exception while waiting for thread creation.");
                Log.e(LOGTAG, Log.getStackTraceString(e));
            }
            */            
        }
    }
    // Create an EventHub to handle messages before and after the thread is
    // ready.
    mEventHub = new EventHub();
    // Create a WebSettings object for maintaining all settings
    mSettings = new WebSettings(mContext, mWebView);
    // The WebIconDatabase needs to be initialized within the UI thread so
    // just request the instance here.
    /*
    CWebIconDatabase::GetInstance();
    */
    // Create the WebStorage singleton and the UI handler
    AutoPtr<CWebStorage> webStorageT;
    webStorageT = NULL; /*CWebStorage::GetInstance();*/
    webStorageT -> CreateUIHandler();
    // Create the UI handler for GeolocationPermissions
    AutoPtr<CGeolocationPermissions> geolocationPermissionsT;
    geolocationPermissionsT = NULL; /*CGeolocationPermissions::GetInstance();*/
    geolocationPermissionsT -> CreateUIHandler();
    // Send a message to initialize the WebViewCore.
    //JAVA:  Message init = sWebCoreHandler.obtainMessage(WebCoreThread.INITIALIZE, this);
    //JAVA:  sWebCoreHandler.sendMessage(init);
    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr((IInterface*)this);
    sWebCoreHandler->SendMessage(WebCoreThread::INITIALIZE, params);
}

/* Initialize private data within the WebCore thread.
*/
void WebViewCore::Initialize()
{
    /* Initialize our private BrowserFrame class to handle all
     * frame-related functions. We need to create a new view which
     * in turn creates a C level FrameView and attaches it to the frame.
    */
    CBrowserFrame::New(mContext.Get(), (Handle32)this, mCallbackProxy.Get(), (Handle32)(mSettings.Get()), mJavascriptInterfaces.Get(), (IBrowserFrame**)&mBrowserFrame);
    mJavascriptInterfaces = NULL;
    // Sync the native settings and also create the WebCore thread handler.
    mSettings -> SyncSettingsAndCreateHandler(mBrowserFrame);
    // Create the handler and transfer messages for the IconDatabase
    AutoPtr<CWebIconDatabase> webIconDatabaseT;
    webIconDatabaseT=NULL;// = CWebIconDatabase::GetInstance();
    webIconDatabaseT -> CreateHandler();
    // Create the handler for WebStorage
    AutoPtr<CWebStorage> webStorageT;
    webStorageT=NULL; /*= CWebStorage::GetInstance();*/
    webStorageT -> CreateHandler();
    // Create the handler for GeolocationPermissions.
    AutoPtr<CGeolocationPermissions> geolocationPermissionsT;
    geolocationPermissionsT=NULL; /*= CGeolocationPermissions::GetInstance();*/
    geolocationPermissionsT -> CreateHandler();
    // The transferMessages call will transfer all pending messages to the
    // WebCore thread handler.
    mEventHub -> TransferMessages();

    // Send a message back to WebView to tell it that we have set up the
    // WebCore thread.
    if (mWebView.Get() != NULL) {
        //Message.obtain(mWebView.mPrivateHandler, WebView.WEBCORE_INITIALIZED_MSG_ID, mNativeClass, 0).sendToTarget();
        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        params->WriteInt32(mNativeClass);
        (((CWebView*)(mWebView.Get()))->mPrivateHandler)->SendMessage(CWebView::WEBCORE_INITIALIZED_MSG_ID, params);
    }
}

/* Handle the initialization of WebViewCore during subwindow creation. This
* method is called from the WebCore thread but it is called before the
* INITIALIZE message can be handled.
*/
/* package */
void WebViewCore::InitializeSubwindow()
{
    // Go ahead and initialize the core components.
    Initialize();
    // Remove the INITIALIZE method so we don't try to initialize twice.
    //JAVA:  sWebCoreHandler.removeMessages(WebCoreThread.INITIALIZE, this);
    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr((IInterface*)this);
    sWebCoreHandler->RemoveCppCallbacks((Handle32)(sWebCoreHandler.Get()), ((WebCoreThread::WvcWctHandler*)(sWebCoreHandler.Get()))->GetFunc(WebCoreThread::INITIALIZE));
}

/* Get the BrowserFrame component. This is used for subwindow creation and
* is called only from BrowserFrame in the WebCore thread. */
/* package */ 
AutoPtr<IBrowserFrame> WebViewCore::GetBrowserFrame()
{
    return mBrowserFrame;
}

//-------------------------------------------------------------------------
// Common methods
//-------------------------------------------------------------------------

/**
* Causes all timers to pause. This applies to all WebViews in the current
* app process.
*/
void WebViewCore::PauseTimers()
{
    if ((CBrowserFrame::sJavaBridge).Get() == NULL) {
        Utility::Logging::Logger::E(LOGTAG, String("No WebView has been created in this process!\n") );
        //return E_ILLEGAL_STATE_EXCEPTION;
    }
    (CBrowserFrame::sJavaBridge) -> Pause();    
}

/**
* Resume all timers. This applies to all WebViews in the current process.
*/
void WebViewCore::ResumeTimers()
{
    if ((CBrowserFrame::sJavaBridge).Get() == NULL) {
        Utility::Logging::Logger::E(LOGTAG, String("No WebView has been created in this process!\n") );
        //return E_ILLEGAL_STATE_EXCEPTION;
    }
    (CBrowserFrame::sJavaBridge) -> Resume();
}

WebSettings* WebViewCore::GetSettings()
{
    return mSettings;
}

/*
* Given mimeType, check whether it's supported in Android media framework.
* mimeType could be such as "audio/ogg" and "video/mp4".
*/
/* package */
Boolean WebViewCore::SupportsMimeType(
    /* [in] */ const String& mimeType)
{
    return /*(CMediaFile::GetFileTypeForMimeType(mimeType)) > 0*/FALSE;    //JAVA:    media/MediaFile.java(public class)
}

/**
* Add an error message to the client's console.
* @param message The message to add
* @param lineNumber the line on which the error occurred
* @param sourceID the filename of the source that caused the error.
* @param msgLevel the log level of this message. This is a value casted to int
*     from WebCore::MessageLevel in WebCore/page/Console.h.
*/
void WebViewCore::AddMessageToConsole(
    /* [in] */ const String& message, 
    /* [in] */ Int32 lineNumber, 
    /* [in] */ const String& sourceID,
    /* [in] */ Int32 msgLevel)
{
    mCallbackProxy -> AddMessageToConsole(message, lineNumber, sourceID, msgLevel);
}

/**
* Invoke a javascript alert.
* @param message The message displayed in the alert.
*/
void WebViewCore::JsAlert(
    /* [in] */ const String& url, 
    /* [in] */ const String& message)
{
    mCallbackProxy -> OnJsAlert(url, message);
}

/**
* Called by JNI.  Open a file chooser to upload a file.
* @return String version of the URI plus the name of the file.
* FIXME: Just return the URI here, and in FileSystem::pathGetFileName, call
* into Java to get the filename.
*/
String WebViewCore::OpenFileChooser()
{
    AutoPtr<IUri> uri = ((CCallbackProxy*)mCallbackProxy.Get()) -> OpenFileChooser();
    if (uri.Get() == NULL) {
        return String("");
    }
    // Find out the name, and append it to the URI.
    // Webkit will treat the name as the filename, and
    // the URI as the path.  The URI will be used
    // in BrowserFrame to get the actual data.
    AutoPtr<IContentResolver> contentResolverT;
    mContext -> GetContentResolver((IContentResolver**)&contentResolverT);
    AutoFree < ArrayOf < String > > projection;
    ArrayOf<String>::Free(projection.Get());
    projection = ArrayOf<String>::Alloc(1);
    (*projection)[0] = /*OpenableColumns_DISPLAY_NAME*/String("");
    AutoPtr<ICursor> cursor ;
    AutoFree < ArrayOf < String > > arrayNull;
    ArrayOf<String>::Free(arrayNull.Get());
    contentResolverT -> Query( uri.Get(), (*projection), String(NULL), *arrayNull, String(NULL), (ICursor**)&cursor );
    String name = String("");
    if (cursor.Get() != NULL) {
        Boolean bMoveToNext;
        ECode ec = cursor -> MoveToNext(&bMoveToNext);
        if(FAILED(ec)){
        }
        if(bMoveToNext) {
            ec = cursor -> GetString( 0, &name);
            if(FAILED(ec)){
            }
        }
        cursor -> Close();
    }
    String strRet;
    uri -> ToString(&strRet);
    strRet = strRet + String("/") + name;
    return strRet;
}

/**
* Notify the browser that the origin has exceeded it's database quota.
* @param url The URL that caused the overflow.
* @param databaseIdentifier The identifier of the database.
* @param currentQuota The current quota for the origin.
* @param estimatedSize The estimated size of the database.
*/
void WebViewCore::ExceededDatabaseQuota(
    /* [in] */ const String& url, 
    /* [in] */ const String& databaseIdentifier,
    /* [in] */ Int64 currentQuota,
    /* [in] */ Int64 estimatedSize)
{
    // Inform the callback proxy of the quota overflow. Send an object
    // that encapsulates a call to the nativeSetDatabaseQuota method to
    // awaken the sleeping webcore thread when a decision from the
    // client to allow or deny quota is available.
    AutoPtr<WvcWebStorageQuotaUpdater> webStorageQuotaUpdaterT;
    webStorageQuotaUpdaterT = new WvcWebStorageQuotaUpdater(this);
    mCallbackProxy -> OnExceededDatabaseQuota(url, databaseIdentifier, currentQuota, 
        estimatedSize, GetUsedQuota(),(IWebStorageQuotaUpdater*)webStorageQuotaUpdaterT);
}

/**
* Notify the browser that the appcache has exceeded its max size.
* @param spaceNeeded is the amount of disk space that would be needed
* in order for the last appcache operation to succeed.
*/
void WebViewCore::ReachedMaxAppCacheSize(
    /* [in] */ Int64 spaceNeeded)
{
    AutoPtr<WvcWebStorageQuotaUpdater> webStorageQuotaUpdaterT;
    webStorageQuotaUpdaterT = new WvcWebStorageQuotaUpdater(this);
    mCallbackProxy -> OnReachedMaxAppCacheSize(spaceNeeded, GetUsedQuota(), (IWebStorageQuotaUpdater*)webStorageQuotaUpdaterT);
}

void WebViewCore::PopulateVisitedLinks()
{
    class WvcValueCallback: public ElRefBase, public IValueCallback
    {        
    public: 
        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid)
        {
            if (riid == EIID_IValueCallback) {
                return (IInterface*)(IValueCallback*)this;
            }
            else if (riid == EIID_IValueCallback) {
                return (IValueCallback*)this;
            }
            return NULL;
        };

        CARAPI_(UInt32) AddRef()
        {
            return ElRefBase::AddRef();
        };

        CARAPI_(UInt32) Release()
        {
            return ElRefBase::Release();
        };

        CARAPI GetInterfaceID(
            /* [in] */ IInterface* Object,
            /* [out] */ InterfaceID* iID)
        {
            VALIDATE_NOT_NULL(iID);
            if (iID == NULL) {
                return E_INVALID_ARGUMENT;
            }

            if (Object == (IInterface*)(IValueCallback*)this) {
                *iID = EIID_IValueCallback;
            }
            else {
                return E_INVALID_ARGUMENT;
            }
            return NOERROR;
        };

    public:
        virtual CARAPI OnReceiveValue(
        /* [in] */ IInterface* value)
        {
            mWebViewCore -> SendMessage(EventHub::POPULATE_VISITED_LINKS, (IInterface*)value);
            return NOERROR;
        };

        WvcValueCallback(
        /* [in] */ WebViewCore* webViewCore)
        {
            mWebViewCore = webViewCore;
        };
    private:
        WebViewCore* mWebViewCore;
    };

    AutoPtr<IValueCallback> callback;
    callback = new WvcValueCallback(this);
    mCallbackProxy -> GetVisitedHistory(callback);
}

/**
* Shows a prompt to ask the user to set the Geolocation permission state
* for the given origin.
* @param origin The origin for which Geolocation permissions are
*     requested.
*/
void WebViewCore::GeolocationPermissionsShowPrompt(
    /* [in] */ const String& origin)
{
    class WvcGeolocationPermissionsCallback: public GeolocationPermissionsCallback
    {
    public:
        CARAPI Invoke(
        /* [in] */ const String& origin, 
        /* [in] */ Boolean allow, 
        /* [in] */ Boolean remember)
        {
            GeolocationPermissionsData * data = new GeolocationPermissionsData();
            data -> mOrigin = origin;
            data -> mAllow = allow;
            data -> mRemember = remember;
            // Marshall to WebCore thread.
            mWebViewCore -> SendMessage(EventHub::GEOLOCATION_PERMISSIONS_PROVIDE, (IInterface*)data);
            delete data;
            return NOERROR;
        };

        WvcGeolocationPermissionsCallback(
        /* [in] */ WebViewCore* webViewCore)
        {
            mWebViewCore = webViewCore;
        };
    private:
        WebViewCore* mWebViewCore;
    };

    AutoPtr<IGeolocationPermissionsCallback> callback;
    callback = new WvcGeolocationPermissionsCallback(this);
    mCallbackProxy -> OnGeolocationPermissionsShowPrompt(origin, callback);
}

/**
* Hides the Geolocation permissions prompt.
*/
void WebViewCore::GeolocationPermissionsHidePrompt()
{
    mCallbackProxy -> OnGeolocationPermissionsHidePrompt();
}

/**
* Invoke a javascript confirm dialog.
* @param message The message displayed in the dialog.
* @return True if the user confirmed or false if the user cancelled.
*/
Boolean WebViewCore::JsConfirm(
    /* [in] */ const String& url, 
    /* [in] */ const String& message)
{
    Boolean bOnJsConfirm;
    mCallbackProxy -> OnJsConfirm(url, message, &bOnJsConfirm);
    return bOnJsConfirm;
}

/**
* Invoke a javascript prompt dialog.
* @param message The message to be displayed in the dialog.
* @param defaultValue The default value in the prompt input.
* @return The input from the user or null to indicate the user cancelled
*         the dialog.
*/
String WebViewCore::JsPrompt(
    /* [in] */ const String& url, 
    /* [in] */ const String& message, 
    /* [in] */ const String& defaultValue)
{
    String strRet;
    mCallbackProxy -> OnJsPrompt(url, message, defaultValue,&strRet);
    return strRet;
}

/**
* Invoke a javascript before unload dialog.
* @param url The url that is requesting the dialog.
* @param message The message displayed in the dialog.
* @return True if the user confirmed or false if the user cancelled. False
*         will cancel the navigation.
*/
Boolean WebViewCore::JsUnload(
    /* [in] */ const String& url, 
    /* [in] */ const String& message)
{
    Boolean result;
    mCallbackProxy -> OnJsBeforeUnload(url, message, &result);
    return result;
}

/**
*
* Callback to notify that a JavaScript execution timeout has occured.
* @return True if the JavaScript execution should be interrupted. False
*         will continue the execution.
*/
Boolean WebViewCore::JsInterrupt()
{
    Boolean flag;
    mCallbackProxy -> OnJsTimeout(&flag);
    return flag;
}

//-------------------------------------------------------------------------
// JNI methods
//-------------------------------------------------------------------------

/* native*/
String WebViewCore::NativeFindAddress(
    /* [in] */ const String& addr, 
    /* [in] */ Boolean caseInsensitive)
{//=0(virtual)
    return String(NULL);
}

/**
* Empty the picture set.
*/
/*native*/
void WebViewCore::NativeClearContent()
{//=0(virtual)
}

/**
* Create a flat picture from the set of pictures.
*/
/*native*/
void WebViewCore::NativeCopyContentToPicture(
    /* [in] */ IPicture* picture)
{//=0(virtual)
}

/**
* Draw the picture set with a background color. Returns true
* if some individual picture took too long to draw and can be
* split into parts. Called from the UI thread.
*/
/*native*/
Boolean WebViewCore::NativeDrawContent(
    /* [in] */ ICanvas* canvas, 
    /* [in] */ Int32 color)
{//=0(virtual)
    return FALSE;
}

/**
* check to see if picture is blank and in progress
*/
/*native*/
Boolean WebViewCore::NativePictureReady()
{//=0(virtual)
    return FALSE;
}

/**
* Redraw a portion of the picture set. The Point wh returns the
* width and height of the overall picture.
*/
/*native*/
Boolean WebViewCore::NativeRecordContent(
    /* [in] */ IRegion* invalRegion, 
    /* [in] */ IPoint* wh)
{//=0(virtual)
    return FALSE;
}

/*native*/
Boolean WebViewCore::NativeFocusBoundsChanged()
{//=0(virtual)
    return FALSE;
}

/**
* Splits slow parts of the picture set. Called from the webkit
* thread after nativeDrawContent returns true.
*/
/*native*/ 
void WebViewCore::NativeSplitContent()
{//=0(virtual)
}

/*native*/
Boolean WebViewCore::NativeKey(
    /* [in] */ Int32 keyCode, 
    /* [in] */ Int32 unichar,
    /* [in] */ Int32 repeatCount,
    /* [in] */ Boolean isShift, 
    /* [in] */ Boolean isAlt,
    /* [in] */ Boolean isSym,
    /* [in] */ Boolean isDown)
{//=0(virtual)
    return FALSE;
}

/*native*/
void WebViewCore::NativeClick(
    /* [in] */ Int32 framePtr, 
    /* [in] */ Int32 nodePtr)
{//=0(virtual)
}

/*native*/
void WebViewCore::NativeSendListBoxChoices(
    /* [in] */ ArrayOf<Boolean>* choices, 
    /* [in] */ Int32 size)
{//=0(virtual)
}

/*native*/
void WebViewCore::NativeSendListBoxChoice(
/* [in] */ Int32 choice)
{//=0(virtual)
}

/*  Tell webkit what its width and height are, for the purposes
of layout/line-breaking. These coordinates are in document space,
which is the same as View coords unless we have zoomed the document
(see nativeSetZoom).
screenWidth is used by layout to wrap column around. If viewport uses
fixed size, screenWidth can be different from width with zooming.
should this be called nativeSetViewPortSize?
*/
/*native*/
void WebViewCore::NativeSetSize(
    /* [in] */ Int32 width, 
    /* [in] */ Int32 height, 
    /* [in] */ Int32 screenWidth,
    /* [in] */ Float scale, 
    /* [in] */ Int32 realScreenWidth, 
    /* [in] */ Int32 screenHeight, 
    /* [in] */ Int32 anchorX,
    /* [in] */ Int32 anchorY, 
    /* [in] */ Boolean ignoreHeight)
{//=0(virtual)
}

/*native*/
Int32 WebViewCore::NativeGetContentMinPrefWidth()
{//=0(virtual)
    return 0;
}

// Start: functions that deal with text editing
/*native*/
void WebViewCore::NativeReplaceTextfieldText(
    /* [in] */ Int32 oldStart, 
    /* [in] */ Int32 oldEnd, 
    /* [in] */ const String& replace, 
    /* [in] */ Int32 newStart, 
    /* [in] */ Int32 newEnd,
    /* [in] */ Int32 textGeneration)
{//=0(virtual)
}

/*native*/
void WebViewCore::PassToJs(
    /* [in] */ Int32 gen,
    /* [in] */ const String& currentText, 
    /* [in] */ Int32 keyCode, 
    /* [in] */ Int32 keyValue, 
    /* [in] */ Boolean down,
    /* [in] */ Boolean cap, 
    /* [in] */ Boolean fn, 
    /* [in] */ Boolean sym)
{//=0(virtual)
}

/*native*/
void WebViewCore::NativeSetFocusControllerActive(
    /* [in] */ Boolean active)
{//=0(virtual)
}

/*native*/ 
void WebViewCore::NativeSaveDocumentState(
    /* [in] */ Int32 frame)
{//=0(virtual)
}

/*native*/
void WebViewCore::NativeMoveFocus(
    /* [in] */ Int32 framePtr, 
    /* [in] */ Int32 nodePointer)
{//=0(virtual)
}

/*native*/
void WebViewCore::NativeMoveMouse(
    /* [in] */ Int32 framePtr, 
    /* [in] */ Int32 x, 
    /* [in] */ Int32 y)
{//=0(virtual)
}

/*native*/
void WebViewCore::NativeMoveMouseIfLatest(
    /* [in] */ Int32 moveGeneration,
    /* [in] */ Int32 framePtr, 
    /* [in] */ Int32 x, 
    /* [in] */ Int32 y)
{//=0(virtual)
}

/*native*/
String WebViewCore::NativeRetrieveHref(
    /* [in] */ Int32 framePtr, 
    /* [in] */ Int32 nodePtr)
{//=0(virtual)
    return String(NULL);
}

/*native*/
String WebViewCore::NativeRetrieveAnchorText(
    /* [in] */ Int32 framePtr, 
    /* [in] */ Int32 nodePtr)
{//=0(virtual)
    return String(NULL);
}

/*native*/
void WebViewCore::NativeTouchUp(
    /* [in] */ Int32 touchGeneration,
    /* [in] */ Int32 framePtr, 
    /* [in] */ Int32 nodePtr, 
    /* [in] */ Int32 x, 
    /* [in] */ Int32 y)
{//=0(virtual)
}

/*native*/
Boolean WebViewCore::NativeHandleTouchEvent(
    /* [in] */ Int32 action, 
    /* [in] */ Int32 x, 
    /* [in] */ Int32 y,
    /* [in] */ Int32 metaState)
{//=0(virtual)
    return FALSE;
}

/*native*/
void WebViewCore::NativeUpdateFrameCache()
{//=0(virtual)    
}

/*native*/
void WebViewCore::NativeSetBackgroundColor(
    /* [in] */ Int32 color)
{//=0(virtual)
}

/*native*/
void WebViewCore::NativeDumpDomTree(
    /* [in] */ Boolean useFile)
{//=0(virtual)
}

/*native*/
void WebViewCore::NativeDumpRenderTree(
    /* [in] */ Boolean useFile)
{//=0(virtual)
}

/*native*/
void WebViewCore::NativeDumpNavTree()
{//=0(virtual) 
}

/*native*/
void WebViewCore::NativeDumpV8Counters()
{//=0(virtual) 
}

/*native*/
void WebViewCore::NativeSetJsFlags(
    /* [in] */ const String& flags)
{//=0(virtual) 
}

/**
*  Delete text from start to end in the focused textfield. If there is no
*  focus, or if start == end, silently fail.  If start and end are out of
*  order, swap them.
*  @param  start   Beginning of selection to delete.
*  @param  end     End of selection to delete.
*  @param  textGeneration Text generation number when delete was pressed.
*/
/*native*/
void WebViewCore::NativeDeleteSelection(
    /* [in] */ Int32 start, 
    /* [in] */ Int32 end,
    /* [in] */ Int32 textGeneration)
{//=0(virtual) 
}

/**
*  Set the selection to (start, end) in the focused textfield. If start and
*  end are out of order, swap them.
*  @param  start   Beginning of selection.
*  @param  end     End of selection.
*/
/*native*/
void WebViewCore::NativeSetSelection(
    /* [in] */ Int32 start, 
    /* [in] */ Int32 end)
{//=0(virtual) 
}

// Register a scheme to be treated as local scheme so that it can access
// local asset files for resources
/*native*/
void WebViewCore::NativeRegisterURLSchemeAsLocal(
    /* [in] */ const String& scheme)
{//=0(virtual) 
}

/*
* Inform webcore that the user has decided whether to allow or deny new
* quota for the current origin or more space for the app cache, and that
* the main thread should wake up now.
* @param limit Is the new quota for an origin or new app cache max size.
*/
/*native*/
void WebViewCore::NativeSetNewStorageLimit(
    /* [in] */ Int64 limit)
{//=0(virtual) 
}

/**
* Provide WebCore with a Geolocation permission state for the specified
* origin.
* @param origin The origin for which Geolocation permissions are provided.
* @param allow Whether Geolocation permissions are allowed.
* @param remember Whether this decision should be remembered beyond the
*     life of the current page.
*/
/*native*/
void WebViewCore::NativeGeolocationPermissionsProvide(
    /* [in] */ const String& origin, 
    /* [in] */ Boolean allow, 
    /* [in] */ Boolean remember)
{//=0(virtual) 
}

/**
* Provide WebCore with the previously visted links from the history database
*/
/* native */
void WebViewCore::NativeProvideVisitedHistory(
    /* [in] */ ArrayOf<String>* history)
{//=0(virtual) 
}

//-------------------------------------------------------------------------
// Methods called by host activity (in the same thread)
//-------------------------------------------------------------------------

void WebViewCore::StopLoading()
{
    if (DebugFlags::sWEB_VIEW_CORE) {
        Utility::Logging::Logger::V(LOGTAG, String("CORE stopLoading\n") );
    }
    if (mBrowserFrame.Get() != NULL) {
        mBrowserFrame -> StopLoading();
    }
}

//-------------------------------------------------------------------------
// Methods called by WebView
// If it refers to local variable, it needs synchronized().
// If it needs WebCore, it has to send message.
//-------------------------------------------------------------------------

void WebViewCore::SendMessage(
    /* [in] */ /*IApartment* apartmentR,*/
    /* [in] */ Int32 message,
    /* [in] */ IParcel* params)
{
    AutoPtr<EventHub::Msg> msg = new EventHub::Msg();
    //msg->apartment = apartmentR;
    msg->message = message;
    msg->params = params; 
    mEventHub->SendMessage(msg.Get());
}

void WebViewCore::SendMessage(
	/* [in] */ Int32 message)
{
    AutoPtr<EventHub::Msg> msg = new EventHub::Msg();
    msg->message = message;
    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    msg->params = params; 
    mEventHub -> SendMessage(msg.Get());
}

void WebViewCore::SendMessage(
	/* [in] */ Int32 message, 
	/* [in] */ IInterface* obj)
{
    AutoPtr<EventHub::Msg> msg = new EventHub::Msg();
    msg->message = message;
    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);    
    params->WriteInterfacePtr(obj);
    msg->params = params; 
    mEventHub -> SendMessage(msg.Get());
}

void WebViewCore::SendMessage(
	/* [in] */ Int32 message, 
	/* [in] */ Int32 arg1)
{
    AutoPtr<EventHub::Msg> msg = new EventHub::Msg();
    msg->message = message;
    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);    
    params->WriteInt32(arg1);
    msg->params = params; 
    mEventHub -> SendMessage(msg.Get());
}

void WebViewCore::SendMessage(
	/* [in] */ Int32 message, 
	/* [in] */ Int32 arg1, 
	/* [in] */ Int32 arg2)
{
    AutoPtr<EventHub::Msg> msg = new EventHub::Msg();
    msg->message = message;
    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);    
    params->WriteInt32(arg1);
    params->WriteInt32(arg2);
    msg->params = params; 
    mEventHub -> SendMessage(msg.Get());
}

void WebViewCore::SendMessage(
	/* [in] */ Int32 message, 
	/* [in] */ Int32 arg1, 
	/* [in] */ IInterface* obj)
{
    AutoPtr<EventHub::Msg> msg = new EventHub::Msg();
    msg->message = message;
    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);    
    params->WriteInt32(arg1);
    params->WriteInterfacePtr(obj);
    msg->params = params; 
    mEventHub -> SendMessage(msg.Get());
}

void WebViewCore::SendMessage(
	/* [in] */ Int32 message, 
	/* [in] */ Int32 arg1, 
	/* [in] */ Int32 arg2, 
	/* [in] */ IInterface* obj)
{
    AutoPtr<EventHub::Msg> msg = new EventHub::Msg();
    msg->message = message;
    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);    
    params->WriteInt32(arg1);
    params->WriteInt32(arg2);
    params->WriteInterfacePtr(obj);
    msg->params = params; 
    mEventHub -> SendMessage(msg.Get());
}

void WebViewCore::SendMessageAtFrontOfQueue(
	/* [in] */ Int32 message, 
	/* [in] */ IInterface* obj)
{    
    AutoPtr<EventHub::Msg> msg = new EventHub::Msg();
    msg->message = message;
    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);    
    params->WriteInterfacePtr(obj);
    msg->params = params; 
    mEventHub -> SendMessage(msg);
    mEventHub -> SendMessageAtFrontOfQueue(msg.Get());
}

void WebViewCore::SendMessageDelayed(
	/* [in] */ Int32 message, 
	/* [in] */ IInterface* obj, 
	/* [in] */ Int64 delay)
{    
    AutoPtr<EventHub::Msg> msg = new EventHub::Msg();
    msg->message = message;
    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);    
    params->WriteInterfacePtr(obj);
    msg->params = params; 
    mEventHub -> SendMessage(msg);
    mEventHub -> SendMessageDelayed(msg.Get(), delay);
}

void WebViewCore::RemoveMessages(
	/* [in] */ Int32 message)
{
    mEventHub -> RemoveMessages(message);
}

void WebViewCore::RemoveMessages()
{
    mEventHub -> RemoveMessages();
}

/**
* Removes pending messages and trigger a DESTROY message to send to
* WebCore.
* Called from UI thread.
*/
void WebViewCore::Destroy()
{    
    // We don't want anyone to post a message between removing pending
    // messages and sending the destroy message.
    Core::Threading::Mutex::Autolock lock(mEventHubLock);
    // RESUME_TIMERS and PAUSE_TIMERS are per process base. They need to
    // be preserved even the WebView is destroyed.
    // Note: we should not have more than one RESUME_TIMERS/PAUSE_TIMERS
    Boolean hasResume = mEventHub -> HasMessages(EventHub::RESUME_TIMERS);
    Boolean hasPause = mEventHub -> HasMessages(EventHub::PAUSE_TIMERS);
    mEventHub -> RemoveMessages();
    AutoPtr<EventHub::Msg> msg = new EventHub::Msg();
    msg->message = EventHub::DESTROY;
    mEventHub -> SendMessageAtFrontOfQueue(msg.Get());
    if (hasPause) {
        msg->message = EventHub::PAUSE_TIMERS;
        mEventHub -> SendMessageAtFrontOfQueue(msg.Get());
    }
    if (hasResume) {        
        msg->message = EventHub::RESUME_TIMERS;
        mEventHub -> SendMessageAtFrontOfQueue(msg.Get());
    }
    mEventHub -> BlockMessages();
}

//-------------------------------------------------------------------------
// WebViewCore private methods
//-------------------------------------------------------------------------

void WebViewCore::ClearCache(
    /* [in] */ Boolean includeDiskFiles)
{
    mBrowserFrame -> ClearCache();
    if (includeDiskFiles) {
        CCacheManager::RemoveAllCacheFiles();
    }
}

void WebViewCore::LoadUrl(
    /* [in] */ const String& url, 
    /* [in] */ IObjectStringMap* extraHeaders)
{
    if (DebugFlags::sWEB_VIEW_CORE) {
        Utility::Logging::Logger::V(LOGTAG, String(" CORE loadUrl ") + url + String("\n") );
    }
    mBrowserFrame -> LoadUrl(url, extraHeaders);
}

void WebViewCore::Key(
    /* [in] */ IKeyEvent* evt, 
    /* [in] */ Boolean isDown)
{
    if (DebugFlags::sWEB_VIEW_CORE) {
        String strCharacters;
        evt -> GetCharacters(&strCharacters);
        Int64 mWhen = System::GetCurrentTimeMillis();
        Utility::Logging::Logger::V(LOGTAG, String("CORE key at ") + String::FromInt64(mWhen) + ", "
                + strCharacters + String("\n") );
    }
    Int32 keyCode;
    Int32 unicodeChar;
    evt -> GetKeyCode(&keyCode);
    evt -> GetUnicodeChar(&unicodeChar);    
    String strCharacters;
    evt -> GetCharacters(&strCharacters);
    if (keyCode == KeyEvent_KEYCODE_UNKNOWN && strCharacters != String(NULL)
                && strCharacters.GetLength() > 0) {
        // we should only receive individual complex characters
        unicodeChar = strCharacters.GetChar(0);
    }
    Int32 RepeatCount = 0;
    evt -> GetRepeatCount(&RepeatCount);
    Boolean bIsShiftPressed = FALSE;
    evt -> IsShiftPressed(&bIsShiftPressed);
    Boolean bIsAltPressed = FALSE;
    evt -> IsAltPressed(&bIsAltPressed);
    Boolean bIsSymPressed = FALSE;
    evt -> IsSymPressed(&bIsSymPressed);
    if (!NativeKey(keyCode, unicodeChar, RepeatCount, bIsShiftPressed,
                bIsAltPressed, bIsSymPressed,
                isDown) && keyCode != KeyEvent_KEYCODE_ENTER) {
        if (keyCode >= KeyEvent_KEYCODE_DPAD_UP
                    && keyCode <= KeyEvent_KEYCODE_DPAD_RIGHT) {
            if (DebugFlags::sWEB_VIEW_CORE) {
                Utility::Logging::Logger::V(LOGTAG, String("key: arrow unused by plugin: ") + String::FromInt32(keyCode) + String("\n") );
            }
            Boolean bIsDown = FALSE;
            evt -> IsDown(&bIsDown);
            if (mWebView.Get() != NULL && bIsDown) {
                AutoPtr<IParcel> params;
                CCallbackParcel::New((IParcel**)&params);    
                params->WriteInt32(keyCode);
                (((CWebView*)(mWebView.Get()))->mPrivateHandler)->SendMessage(CWebView::MOVE_OUT_OF_PLUGIN,params);
            }
            return;
        }
        // bubble up the event handling
        // but do not bubble up the ENTER key, which would open the search
        // bar without any text.
        mCallbackProxy -> OnUnhandledKeyEvent(evt);
    }
}

// notify webkit that our virtual view size changed size (after inv-zoom)
void WebViewCore::ViewSizeChanged(
    /* [in] */ Int32 w, 
    /* [in] */ Int32 h, 
    /* [in] */ Int32 textwrapWidth, 
    /* [in] */ Float scale,
    /* [in] */ Int32 anchorX, 
    /* [in] */ Int32 anchorY, 
    /* [in] */ Boolean ignoreHeight)
{

    if (DebugFlags::sWEB_VIEW_CORE) {
        String strOut = String("viewSizeChanged w=") + String::FromInt32(w);
        strOut += String("; h=") + String::FromInt32(h); 
        strOut += String("; textwrapWidth=") + String::FromInt32(textwrapWidth);
        strOut += String("; scale=") + String::FromDouble(scale);
        strOut += String("\n");
        Utility::Logging::Logger::V(LOGTAG, strOut);
    }
    if (w == 0) {
        Utility::Logging::Logger::W(LOGTAG, String("skip viewSizeChanged as w is 0\n") );
        return;
    }
    Int32 width = w;
    if ( mSettings -> GetUseWideViewPort() ) {
        if ( mViewportWidth == -1 ) {
            if ( (mSettings->GetLayoutAlgorithm()) == WebSettings::/*LayoutAlgorithm::*/NORMAL) {
                width = CWebView::DEFAULT_VIEWPORT_WIDTH;
            } 
            else {
                /*
                 * if a page's minimum preferred width is wider than the
                 * given "w", use it instead to get better layout result. If
                 * we start a page with MAX_ZOOM_WIDTH, "w" will be always
                 * wider. If we start a page with screen width, due to the
                 * delay between {@link #didFirstLayout} and
                 * {@link #viewSizeChanged},
                 * {@link #nativeGetContentMinPrefWidth} will return a more
                 * accurate value than initial 0 to result a better layout.
                 * In the worse case, the native width will be adjusted when
                 * next zoom or screen orientation change happens.
                 */
                width = Math::Min(CWebView::sMaxViewportWidth, Math::Max(w,
                            Math::Max(CWebView::DEFAULT_VIEWPORT_WIDTH,
                                    NativeGetContentMinPrefWidth())));
            }
        } 
        else if (mViewportWidth > 0) {
            width = Math::Max(w, mViewportWidth);
        } 
        else {
            width = textwrapWidth;
        }
    }
    NativeSetSize(width, width == w ? h : Math::Round((float) width * h / w),
                textwrapWidth, scale, w, h, anchorX, anchorY, ignoreHeight);
    // Remember the current width and height
    Boolean needInvalidate = (mCurrentViewWidth == 0);
    mCurrentViewWidth = w;
    mCurrentViewHeight = h;
    mCurrentViewScale = scale;
    if (needInvalidate) {
        // ensure {@link #webkitDraw} is called as we were blocking in
        // {@link #contentDraw} when mCurrentViewWidth is 0
        if (DebugFlags::sWEB_VIEW_CORE) {
            Utility::Logging::Logger::W(LOGTAG, String("viewSizeChanged\n") );
        }
        ContentDraw();
    }
    AutoPtr<EventHub::Msg> msg = new EventHub::Msg();
    msg->message = EventHub::UPDATE_CACHE_AND_TEXT_ENTRY;
    mEventHub -> SendMessage(msg);
}

void WebViewCore::SendUpdateTextEntry()
{
    if (mWebView.Get() != NULL) {
        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        (((CWebView*)(mWebView.Get())) -> mPrivateHandler)->SendMessage(CWebView::UPDATE_TEXT_ENTRY_MSG_ID, params);
    }
}

// Utility method for exceededDatabaseQuota and reachedMaxAppCacheSize
// callbacks. Computes the sum of database quota for all origins.
Int64 WebViewCore::GetUsedQuota()
{
    CWebStorage* webStorage = /*CWebStorage::GetInstance()*/NULL;
    Vector< AutoPtr<CWebStorage::Origin> >* origins;
    webStorage -> GetOriginsSync(origins);
    if (origins == NULL) {
        return 0;
    }
    Int64 usedQuota = 0;
    Vector<AutoPtr<CWebStorage::Origin> >::Iterator iterT;
    Vector<AutoPtr<CWebStorage::Origin> >::Iterator iterE;
    iterT = origins -> Begin();
    iterE = origins -> End();
    while(iterT != iterE)
    {
        usedQuota += ( (*iterT) -> GetQuota() );
        iterT++;
    }
    return usedQuota;
}

void WebViewCore::WebkitDraw()
{
    mDrawIsScheduled = FALSE;
    WebViewCore::DrawData* draw = new WebViewCore::DrawData();
    if (DebugFlags::sWEB_VIEW_CORE) {
        Utility::Logging::Logger::V(LOGTAG, String("webkitDraw start\n"));
    }
    if (NativeRecordContent(draw->mInvalRegion, draw->mWidthHeight) == FALSE) {
        if (DebugFlags::sWEB_VIEW_CORE) {
            Utility::Logging::Logger::V(LOGTAG, String("webkitDraw abort\n"));
        }
        return;
    }
    if (mWebView.Get() != NULL) {
        // Send the native view size that was used during the most recent
        // layout.
        draw -> mFocusSizeChanged = NativeFocusBoundsChanged();
        CPoint::New(mCurrentViewWidth, mCurrentViewHeight,(IPoint**)&(draw -> mViewPoint));
        Boolean bGetUseWideViewPort;
        bGetUseWideViewPort = mSettings -> GetUseWideViewPort();
        if (bGetUseWideViewPort) {
            draw -> mMinPrefWidth = Math::Max(
                mViewportWidth == -1 ? CWebView::DEFAULT_VIEWPORT_WIDTH
                                : (mViewportWidth == 0 ? mCurrentViewWidth : mViewportWidth),
                NativeGetContentMinPrefWidth());
        }
        if (mRestoreState != NULL) {
            draw -> mRestoreState = mRestoreState;
            mRestoreState = NULL;
        }
        if (DebugFlags::sWEB_VIEW_CORE) {
            Utility::Logging::Logger::V(LOGTAG, String("webkitDraw NEW_PICTURE_MSG_ID\n"));
        } 
        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        params->WriteInterfacePtr((IInterface*)draw);
        (((CWebView*)(mWebView.Get()))->mPrivateHandler)->SendMessage(CWebView::NEW_PICTURE_MSG_ID,params);
        if (mWebkitScrollX != 0 || mWebkitScrollY != 0) {
            // as we have the new picture, try to sync the scroll position
            AutoPtr<IParcel> paramsI;
            CCallbackParcel::New((IParcel**)&paramsI);
            paramsI->WriteInt32(mWebkitScrollX);
            paramsI->WriteInt32(mWebkitScrollY);
            (((CWebView*)(mWebView.Get()))->mPrivateHandler)->SendMessage(CWebView::SYNC_SCROLL_TO_MSG_ID,paramsI);
            mWebkitScrollX = mWebkitScrollY = 0;
        } 
    }
}

///////////////////////////////////////////////////////////////////////////
// These are called from the UI thread, not our thread

/* package */ 
void WebViewCore::DrawContentPicture(
	/* [in] */ ICanvas* canvas, 
	/* [in] */ Int32 color,
	/* [in] */ Boolean animatingZoom,
	/* [in] */ Boolean animatingScroll)
{
    AutoPtr<IDrawFilter> df = NULL;
    if (animatingZoom) {
        df = mZoomFilter;
    } 
    else if (animatingScroll) {
        df = mScrollFilter;
    }
    canvas -> SetDrawFilter(df);
    Boolean tookTooLong = NativeDrawContent(canvas, color);
    canvas -> SetDrawFilter(NULL);
    if (tookTooLong && (mSplitPictureIsScheduled == FALSE) ) {
        mSplitPictureIsScheduled = TRUE;
        SendMessage(EventHub::SPLIT_PICTURE_SET);
    }
}

/* package */ 
/*synchronized*/
Boolean WebViewCore::PictureReady()
{
    return 0 != mNativeClass ? NativePictureReady() : FALSE;
}

/*package*/ 
/*synchronized*/
AutoPtr<IPicture> WebViewCore::CopyContentPicture()
{
    AutoPtr<IPicture> result;
    CPicture::New((IPicture**)&result);
    if (0 != mNativeClass) {
        NativeCopyContentToPicture(result.Get());
    }
    return result;
}

void WebViewCore::ReducePriority()
{
    // remove the pending REDUCE_PRIORITY and RESUME_PRIORITY messages
    void (STDCALL WebCoreThread::WvcWctHandler::*pHandlerFunc)();
    pHandlerFunc = &WebCoreThread::WvcWctHandler::HandleReducePriority;
    sWebCoreHandler->RemoveCppCallbacks( (Handle32)(sWebCoreHandler.Get()), *(Handle32*)&pHandlerFunc);
    pHandlerFunc = &WebCoreThread::WvcWctHandler::HandleResumePriority; 
    sWebCoreHandler->RemoveCppCallbacks( (Handle32)(sWebCoreHandler.Get()), *(Handle32*)&pHandlerFunc);

    pHandlerFunc = &WebCoreThread::WvcWctHandler::HandleReducePriority;
    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    sWebCoreHandler->PostCppCallbackAtFrontOfQueue( (Handle32)(sWebCoreHandler.Get()), *(Handle32*)&pHandlerFunc, params, 0);
}

void WebViewCore::ResumePriority()
{    
    // remove the pending REDUCE_PRIORITY and RESUME_PRIORITY messages
    void (STDCALL WebCoreThread::WvcWctHandler::*pHandlerFunc)();
    pHandlerFunc = &WebCoreThread::WvcWctHandler::HandleReducePriority;
    sWebCoreHandler->RemoveCppCallbacks( (Handle32)(sWebCoreHandler.Get()), *(Handle32*)&pHandlerFunc);
    pHandlerFunc = &WebCoreThread::WvcWctHandler::HandleResumePriority; 
    sWebCoreHandler->RemoveCppCallbacks( (Handle32)(sWebCoreHandler.Get()), *(Handle32*)&pHandlerFunc);

    pHandlerFunc = &WebCoreThread::WvcWctHandler::HandleResumePriority;
    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    sWebCoreHandler->PostCppCallbackAtFrontOfQueue( (Handle32)(sWebCoreHandler.Get()), *(Handle32*)&pHandlerFunc, params, 0);
}

void WebViewCore::PauseUpdatePicture(
	/* [in] */ WebViewCore* core)
{    
    // Note: there is one possible failure mode. If pauseUpdatePicture() is
    // called from UI thread while WEBKIT_DRAW is just pulled out of the
    // queue in WebCore thread to be executed. Then update won't be blocked.
    if (core != NULL) {
        Core::Threading::Mutex::Autolock lock(core->mutexThis);
        core->mDrawIsPaused = TRUE;
        if (core->mDrawIsScheduled) {
            (core->mEventHub)->RemoveMessages(EventHub::WEBKIT_DRAW);
        }
    }
}

void WebViewCore::ResumeUpdatePicture(
	/* [in] */ WebViewCore* core)
{
    if (core != NULL) {
        Core::Threading::Mutex::Autolock lock(core->mutexThis);
        core->mDrawIsPaused = FALSE;
        if (core->mDrawIsScheduled) {
            core->mDrawIsScheduled = FALSE;
            core->ContentDraw();
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void WebViewCore::_RestoreState(
    /* [in] */ Int32 index)
{
    AutoPtr<IWebBackForwardList> list;
    mCallbackProxy -> GetBackForwardList( (IWebBackForwardList**)&list );
    Int32 size;
    list -> GetSize(&size);
    AutoPtr<IWebHistoryItem> webHistoryItemT;
    for (Int32 i = 0; i < size; i++) {
        list -> GetItemAtIndex(i, (IWebHistoryItem**)&webHistoryItemT);
        ((CWebHistoryItem*)(webHistoryItemT.Get())) -> Inflate( ( (CBrowserFrame*)(mBrowserFrame.Get()) ) -> mNativeFrame);
    }
    ((CBrowserFrame*)(mBrowserFrame.Get())) -> mLoadInitFromJava = TRUE;
    ((CWebBackForwardList*)(list.Get())) -> RestoreIndex( ( (CBrowserFrame*)(mBrowserFrame.Get()) ) -> mNativeFrame, index);
    ( (CBrowserFrame*)(mBrowserFrame.Get()) ) -> mLoadInitFromJava = FALSE;
}

//-------------------------------------------------------------------------
// Implement abstract methods in WebViewCore, native WebKit callback part
//-------------------------------------------------------------------------

// called from JNI or WebView thread
/* package */
void WebViewCore::ContentDraw()
{
    // don't update the Picture until we have an initial width and finish
    // the first layout
    if (mCurrentViewWidth == 0 || !((CBrowserFrame*)(mBrowserFrame.Get())) -> FirstLayoutDone()) {
        return;
    }
    // only fire an event if this is our first request
    Core::Threading::Mutex::Autolock lock(mutexThis);
    if (mDrawIsScheduled) {
        return;
    }
    mDrawIsScheduled = TRUE;
    if (mDrawIsPaused) {
        return;
    }
    AutoPtr<EventHub::Msg> msg = new EventHub::Msg();
    msg->message = EventHub::WEBKIT_DRAW;
    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    msg->params = params;
    mEventHub->SendMessage(msg);
}

// called by JNI
CARAPI_(void) WebViewCore::ContentScrollBy(
    /* [in] */ Int32 dx, 
    /* [in] */ Int32 dy, 
    /* [in] */ Boolean animate)
{
    if (!((CBrowserFrame*)(mBrowserFrame.Get()))->FirstLayoutDone()) {
        // Will this happen? If yes, we need to do something here.
        return;
    }
    if (mWebView.Get() != NULL) {
        AutoPtr<IApartment> apartmentW;
        apartmentW = (((CWebView*)(mWebView.Get()))->mPrivateHandler).Get();
        Int32 messageW = CWebView::SCROLL_BY_MSG_ID;
        AutoPtr<IParcel> paramsW;
        CCallbackParcel::New((IParcel**)&paramsW);
        paramsW->WriteInt32(dx);
        paramsW->WriteInt32(dy);
        paramsW->WriteBoolean(animate);
        if (mDrawIsScheduled) {
            AutoPtr<IParcel> params;
            CCallbackParcel::New((IParcel**)&params);
            params->WriteInterfacePtr(apartmentW.Get());
            params->WriteInt32(messageW);
            params->WriteInterfacePtr(paramsW.Get());

            AutoPtr<EventHub::Msg> msg = new EventHub::Msg();
            msg->message = EventHub::MESSAGE_RELAY;
            msg->params = params;
            mEventHub->SendMessage(msg);
        } 
        else {
            apartmentW->SendMessage(messageW,paramsW);
        }
    }
}

// called by JNI
void WebViewCore::ContentScrollTo(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{    
    if (!((CBrowserFrame*)(mBrowserFrame.Get())) -> FirstLayoutDone()) {
        /*
         * WebKit restore state will be called before didFirstLayout(),
         * remember the position as it has to be applied after restoring
         * zoom factor which is controlled by screenWidth.
         */
        mRestoredX = x;
        mRestoredY = y;
        return;
    }
    if ((mWebView.Get()) != NULL) {
        AutoPtr<IApartment> apartmentW;
        apartmentW = (((CWebView*)(mWebView.Get()))->mPrivateHandler).Get();
        Int32 messageW = CWebView::SCROLL_TO_MSG_ID;
        AutoPtr<IParcel> paramsW;
        CCallbackParcel::New((IParcel**)&paramsW);
        paramsW->WriteInt32(x);
        paramsW->WriteInt32(y);
        if (mDrawIsScheduled) {
            AutoPtr<IParcel> params;
            CCallbackParcel::New((IParcel**)&params);
            params->WriteInterfacePtr(apartmentW.Get());
            params->WriteInt32(messageW);
            params->WriteInterfacePtr(paramsW.Get());

            AutoPtr<EventHub::Msg> msg = new EventHub::Msg();
            msg->message = EventHub::MESSAGE_RELAY;
            msg->params = params;
            mEventHub->SendMessage(msg);
        } 
        else {
            apartmentW->SendMessage(messageW,paramsW);
        }
    }
}

// called by JNI
void WebViewCore::ContentSpawnScrollTo(
    /* [in] */ Int32 x, 
    /* [in] */ Int32 y)
{
    if (!((CBrowserFrame*)(mBrowserFrame.Get())) -> FirstLayoutDone()) {
        /*
         * WebKit restore state will be called before didFirstLayout(),
         * remember the position as it has to be applied after restoring
         * zoom factor which is controlled by screenWidth.
         */    
        mRestoredX = x;
        mRestoredY = y;
        return;
    }
    if ( (mWebView.Get()) != NULL) {
        AutoPtr<IApartment> apartmentW;
        apartmentW = (((CWebView*)(mWebView.Get()))->mPrivateHandler).Get();
        Int32 messageW = CWebView::SPAWN_SCROLL_TO_MSG_ID;
        AutoPtr<IParcel> paramsW;
        CCallbackParcel::New((IParcel**)&paramsW);
        paramsW->WriteInt32(x);
        paramsW->WriteInt32(y);
        if (mDrawIsScheduled) {
            AutoPtr<IParcel> params;
            CCallbackParcel::New((IParcel**)&params);
            params->WriteInterfacePtr(apartmentW.Get());
            params->WriteInt32(messageW);
            params->WriteInterfacePtr(paramsW.Get());

            AutoPtr<EventHub::Msg> msg = new EventHub::Msg();
            msg->message = EventHub::MESSAGE_RELAY;
            msg->params = params;
            mEventHub->SendMessage(msg);
        } 
        else {
            apartmentW->SendMessage(messageW,paramsW);
        }
    }
}

// called by JNI
void WebViewCore::SendNotifyProgressFinished()
{
    SendUpdateTextEntry();
    // as CacheManager can behave based on database transaction, we need to
    // call tick() to trigger endTransaction
    AutoPtr<WebViewWorker> webViewWorkerT = WebViewWorker::GetHandler();
    webViewWorkerT -> RemoveMessages(WebViewWorker::MSG_CACHE_TRANSACTION_TICKER);    //JAVA:  os/Handler.java
    webViewWorkerT -> SendEmptyMessage(WebViewWorker::MSG_CACHE_TRANSACTION_TICKER);
    ContentDraw();
}

/*  Called by JNI. The coordinates are in doc coordinates, so they need to
be scaled before they can be used by the view system, which happens
in WebView since it (and its thread) know the current scale factor.
*/
void WebViewCore::SendViewInvalidate(
    /* [in] */ Int32 left, 
    /* [in] */ Int32 top, 
    /* [in] */ Int32 right, 
    /* [in] */ Int32 bottom)
{
    if (mWebView.Get() != NULL) {
        AutoPtr<IRect> rect;
        CRect::New(left, top, right, bottom, (IRect**)&rect);

        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        params->WriteInterfacePtr(rect.Get());
        (((CWebView*)(mWebView.Get())) -> mPrivateHandler)->SendMessage( CWebView::INVAL_RECT_MSG_ID, params);
    }
}

/*
* Called by the WebView thread
*/
/* package */
void WebViewCore::SignalRepaintDone()
{
    mRepaintScheduled = FALSE;
}

// called by JNI
void WebViewCore::SendImmediateRepaint()
{
    if (mWebView.Get() != NULL && !mRepaintScheduled) {
        mRepaintScheduled = TRUE;

        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        (((CWebView*)(mWebView.Get())) -> mPrivateHandler)->SendMessage( CWebView::IMMEDIATE_REPAINT_MSG_ID, params);
    }
}

// called by JNI
void WebViewCore::SetRootLayer(
    /* [in] */ Int32 layer)
{
    if (mWebView.Get() != NULL) {

        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        params->WriteInt32(layer);
        (((CWebView*)(mWebView.Get())) -> mPrivateHandler)->SendMessage( CWebView::SET_ROOT_LAYER_MSG_ID, params);
        AutoPtr<IMessage> msg;
    }
}

/* package */
AutoPtr<IWebView> WebViewCore::GetWebView()
{
    return mWebView;
}

/*native*/
void WebViewCore::SetViewportSettingsFromNative()
{//=0(virtual) 
}

// called by JNI
void WebViewCore::DidFirstLayout(
	/* [in] */ Boolean standardLoad)
{
    if (DebugFlags::sWEB_VIEW_CORE) {
        Utility::Logging::Logger::V(LOGTAG, String("didFirstLayout standardLoad =") +  String::FromBoolean(standardLoad) + String("\n"));
    }
    ((CBrowserFrame*)(mBrowserFrame.Get())) -> DidFirstLayout();

    if (mWebView.Get() == NULL) {
        return;
    }

    Boolean updateRestoreState = standardLoad || mRestoredScale > 0;
    SetupViewport(updateRestoreState);
    // if updateRestoreState is true, ViewManager.postReadyToDrawAll() will
    // be called after the WebView restore the state. If updateRestoreState
    // is false, start to draw now as it is ready.
    if (!updateRestoreState) {
        (((CWebView*)(mWebView.Get())) -> mViewManager) -> PostReadyToDrawAll();
    }

    // reset the scroll position, the restored offset and scales
    mWebkitScrollX = mWebkitScrollY = mRestoredX = mRestoredY  = mRestoredScale = mRestoredScreenWidthScale = 0;    
}

// called by JNI
void WebViewCore::UpdateViewport()
{
    // if updateViewport is called before first layout, wait until first
    // layout to update the viewport. In the rare case, this is called after
    // first layout, force an update as we have just parsed the viewport
    // meta tag.
    if ( ((CBrowserFrame*)(mBrowserFrame.Get())) -> FirstLayoutDone() ) {
        SetupViewport(TRUE);
    }
}

void WebViewCore::SetupViewport(
	/* [in] */ Boolean updateRestoreState)
{
    // set the viewport settings from WebKit
    SetViewportSettingsFromNative();

    // adjust the default scale to match the densityDpi
    Float adjust = 1.0f;
    if (mViewportDensityDpi == -1) {
        if (CWebView::DEFAULT_SCALE_PERCENT != 100) {
            adjust = CWebView::DEFAULT_SCALE_PERCENT / 100.0f;
        }
    } 
    else if (mViewportDensityDpi > 0) {
        AutoPtr<IResources> resourcesT;
        mContext -> GetResources((IResources**)&resourcesT);
        AutoPtr<IDisplayMetrics> displayMetricsT;
        resourcesT -> GetDisplayMetrics((IDisplayMetrics**)&displayMetricsT);
        adjust = (Float)(((CDisplayMetrics*)(displayMetricsT.Get())) -> mDensityDpi) / mViewportDensityDpi;
    }
    Int32 defaultScale = (Int32) (adjust * 100);

    if (mViewportInitialScale > 0) {
        mViewportInitialScale *= adjust;
    }
    if (mViewportMinimumScale > 0) {
        mViewportMinimumScale *= adjust;
    }
    if (mViewportMaximumScale > 0) {
        mViewportMaximumScale *= adjust;
    }

    // infer the values if they are not defined.
    if (mViewportWidth == 0) {
        if (mViewportInitialScale == 0) {
            mViewportInitialScale = defaultScale;
        }
    }
    if (mViewportUserScalable == FALSE) {
        mViewportInitialScale = defaultScale;
        mViewportMinimumScale = defaultScale;
        mViewportMaximumScale = defaultScale;
    }
    if (mViewportMinimumScale > mViewportInitialScale && mViewportInitialScale != 0) {
        mViewportMinimumScale = mViewportInitialScale;
    }
    if (mViewportMaximumScale > 0 && mViewportMaximumScale < mViewportInitialScale) {
        mViewportMaximumScale = mViewportInitialScale;
    }
    if (mViewportWidth < 0 && mViewportInitialScale == defaultScale) {
        mViewportWidth = 0;
    }

    // if mViewportWidth is 0, it means device-width, always update.
    if (mViewportWidth != 0 && !updateRestoreState) {
        RestoreState* restoreState = new RestoreState();
        restoreState -> mMinScale = mViewportMinimumScale / 100.0f;
        restoreState -> mMaxScale = mViewportMaximumScale / 100.0f;
        restoreState -> mDefaultScale = adjust;
        // as mViewportWidth is not 0, it is not mobile site.
        restoreState -> mMobileSite = FALSE;
        // for non-mobile site, we don't need minPrefWidth, set it as 0
        restoreState -> mScrollX = 0;

        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        params->WriteStruct((Handle32)restoreState,sizeof(RestoreState));
        (((CWebView*)(mWebView.Get()))->mPrivateHandler)->SendMessage(CWebView::UPDATE_ZOOM_RANGE, params);

        delete restoreState;
        return;
    }

    // now notify webview
    // webViewWidth refers to the width in the view system
    Int32 webViewWidth;
    // viewportWidth refers to the width in the document system
    Int32 viewportWidth = mCurrentViewWidth;
    if (viewportWidth == 0) {
        // this may happen when WebView just starts. This is not perfect as
        // we call WebView method from WebCore thread. But not perfect
        // reference is better than no reference.
        webViewWidth = ((CWebView*)(mWebView.Get())) -> GetViewWidth();
        viewportWidth = (Int32) (webViewWidth / adjust);
        if (viewportWidth == 0) {
            Utility::Logging::Logger::W(LOGTAG, String("Can't get the viewWidth after the first layout\n"));
        }
    } 
    else {
        webViewWidth = Math::Round(viewportWidth * mCurrentViewScale);
    }
    if(mRestoreState != NULL)
    {
        delete mRestoreState;
        //mRestoreState =NULL;
    }
    mRestoreState = new RestoreState();
    mRestoreState -> mMinScale = mViewportMinimumScale / 100.0f;
    mRestoreState -> mMaxScale = mViewportMaximumScale / 100.0f;
    mRestoreState -> mDefaultScale = adjust;
    mRestoreState -> mScrollX = mRestoredX;
    mRestoreState -> mScrollY = mRestoredY;
    mRestoreState -> mMobileSite = (0 == mViewportWidth);
    if (mRestoredScale > 0) {
        mRestoreState -> mViewScale = mRestoredScale / 100.0f;
        if (mRestoredScreenWidthScale > 0) {
            mRestoreState -> mTextWrapScale = mRestoredScreenWidthScale / 100.0f;
        }
        else {
            mRestoreState -> mTextWrapScale = mRestoreState -> mViewScale;
        }
    } 
    else {
        if (mViewportInitialScale > 0) {
            mRestoreState -> mViewScale = mRestoreState -> mTextWrapScale = mViewportInitialScale / 100.0f;
        } 
        else if (mViewportWidth > 0 && mViewportWidth < webViewWidth) {
            mRestoreState -> mViewScale = mRestoreState -> mTextWrapScale = (Float) webViewWidth / mViewportWidth;
        } 
        else {
            mRestoreState -> mTextWrapScale = adjust;
            // 0 will trigger WebView to turn on zoom overview mode
            mRestoreState -> mViewScale = 0;
        }
    }

    if (((CWebView*)(mWebView.Get())) -> mHeightCanMeasure) {
        // Trick to ensure that the Picture has the exact height for the
        // content by forcing to layout with 0 height after the page is
        // ready, which is indicated by didFirstLayout. This is essential to
        // get rid of the white space in the GMail which uses WebView for
        // message view.
        ((CWebView*)(mWebView.Get())) -> mLastHeightSent = 0;
        // Send a negative scale to indicate that WebCore should reuse
        // the current scale
        CWebView::ViewSizeData* data = new CWebView::ViewSizeData();
        data -> mWidth = ((CWebView*)(mWebView.Get())) -> mLastWidthSent;
        data -> mHeight = 0;
        // if mHeightCanMeasure is true, getUseWideViewPort() can't be
        // true. It is safe to use mWidth for mTextWrapWidth.
        data -> mTextWrapWidth = data -> mWidth;
        data -> mScale = -1.0f;
        data -> mIgnoreHeight = FALSE;
        data -> mAnchorX = data -> mAnchorY = 0;
        // send VIEW_SIZE_CHANGED to the front of the queue so that we can
        // avoid pushing the wrong picture to the WebView side. If there is
        // a VIEW_SIZE_CHANGED in the queue, probably from WebView side,
        // ignore it as we have a new size. If we leave VIEW_SIZE_CHANGED
        // in the queue, as mLastHeightSent has been updated here, we may
        // miss the requestLayout in WebView side after the new picture.
        mEventHub -> RemoveMessages(EventHub::VIEW_SIZE_CHANGED);
        AutoPtr<EventHub::Msg> msg = new EventHub::Msg();
        msg->message = EventHub::VIEW_SIZE_CHANGED;
        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        params->WriteStruct((Handle32)data,sizeof(CWebView::ViewSizeData));
        msg->params = params;
        mEventHub -> SendMessageAtFrontOfQueue(msg.Get());

        if(data != NULL)
        {
            delete data;
            //data = NULL;
        }
    } 
    else if (mSettings -> GetUseWideViewPort()) {
        if (viewportWidth == 0) {
            // Trick to ensure VIEW_SIZE_CHANGED will be sent from WebView
            // to WebViewCore
            ((CWebView*)(mWebView.Get())) -> mLastWidthSent = 0;
        } 
        else {
            CWebView::ViewSizeData* data = new CWebView::ViewSizeData();
            // mViewScale as 0 means it is in zoom overview mode. So we don't
            // know the exact scale. If mRestoredScale is non-zero, use it;
            // otherwise just use mTextWrapScale as the initial scale.
            data -> mScale = mRestoreState -> mViewScale == 0
                        ? (mRestoredScale > 0 ? mRestoredScale / 100.0f
                                : mRestoreState -> mTextWrapScale)
                        : mRestoreState -> mViewScale;
            if (DebugFlags::sWEB_VIEW_CORE) {
                String strOut = String("setupViewport");
                strOut += (String(" mRestoredScale=") + String::FromInt32(mRestoredScale));
                strOut += (String(" mViewScale=") + String::FromDouble(mRestoreState->mViewScale));
                strOut += (String(" mTextWrapScale=") + String::FromDouble(mRestoreState -> mTextWrapScale));
                strOut += String("\n");
                Utility::Logging::Logger::V(LOGTAG, strOut);
            }
            data -> mWidth = Math::Round(webViewWidth / (data -> mScale));
            // We may get a call here when mCurrentViewHeight == 0 if webcore completes the
            // first layout before we sync our webview dimensions to it. In that case, we
            // request the real height of the webview. This is not a perfect solution as we
            // are calling a WebView method from the WebCore thread. But this is preferable
            // to syncing an incorrect height.
            data -> mHeight = mCurrentViewHeight == 0 ?
                        Math::Round( (((CWebView*)(mWebView.Get())) -> GetViewHeight()) / (data -> mScale) )
                        : mCurrentViewHeight * (data -> mWidth) / viewportWidth;
            data -> mTextWrapWidth = Math::Round(webViewWidth / (mRestoreState->mTextWrapScale));
            data -> mIgnoreHeight = FALSE;
            data -> mAnchorX = data -> mAnchorY = 0;
            // send VIEW_SIZE_CHANGED to the front of the queue so that we
            // can avoid pushing the wrong picture to the WebView side.
            mEventHub -> RemoveMessages(EventHub::VIEW_SIZE_CHANGED);            
            AutoPtr<EventHub::Msg> msg = new EventHub::Msg();
            msg->message = EventHub::VIEW_SIZE_CHANGED;            
            AutoPtr<IParcel> params;
            CCallbackParcel::New((IParcel**)&params);
            params->WriteStruct((Handle32)data,sizeof(CWebView::ViewSizeData));
            msg->params = params;
            mEventHub -> SendMessageAtFrontOfQueue(msg.Get());
            delete data;
        }
    }
}

// called by JNI
void WebViewCore::RestoreScale(
	/* [in] */ Int32 scale)
{
    if ( (((CBrowserFrame*)(mBrowserFrame.Get())) -> FirstLayoutDone() ) == FALSE) {
        mRestoredScale = scale;
    }
}

// called by JNI
void WebViewCore::RestoreScreenWidthScale(
	/* [in] */ Int32 scale)
{
    if (! (mSettings -> GetUseWideViewPort())) {
        return;
    }
    if ( ((CBrowserFrame*)(mBrowserFrame.Get())) -> FirstLayoutDone() == FALSE) 
    {
        mRestoredScreenWidthScale = scale;
    }
}

// called by JNI
void WebViewCore::NeedTouchEvents(
	/* [in] */ Boolean need)
{
    if (mWebView.Get() != NULL) {
        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        params->WriteInt32(need ? 1 : 0);
        (((CWebView*)(mWebView.Get()))->mPrivateHandler)->SendMessage(CWebView::WEBCORE_NEED_TOUCH_EVENTS, params);
    }
}

// called by JNI
CARAPI_(void) WebViewCore::UpdateTextfield(
	/* [in] */ Int32 ptr, 
	/* [in] */ Boolean changeToPassword,
	/* [in] */ CString text, 
	/* [in] */ Int32 textGeneration)
{
    if (mWebView.Get() != NULL) {
        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        params->WriteInt32(ptr);
        params->WriteInt32(textGeneration);
        params->WriteString(String(text));
        params->WriteBoolean(changeToPassword);
        (((CWebView*)(mWebView.Get()))->mPrivateHandler)->SendMessage(CWebView::UPDATE_TEXTFIELD_TEXT_MSG_ID, params);
    }
}

// called by JNI
void WebViewCore::UpdateTextSelection(
	/* [in] */ Int32 pointer,
	/* [in] */ Int32 start, 
	/* [in] */ Int32 end,
	/* [in] */ Int32 textGeneration)
{
    if (mWebView.Get() != NULL) {
        TextSelectionData* textSelectionDataT = new TextSelectionData(start, end);
        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        params->WriteInt32(pointer);
        params->WriteInt32(textGeneration);
        params->WriteStruct((Handle32)textSelectionDataT,sizeof(TextSelectionData));
        (((CWebView*)(mWebView.Get()))->mPrivateHandler)->SendMessage(CWebView::UPDATE_TEXTFIELD_TEXT_MSG_ID, params);
        delete textSelectionDataT;
    }
}

// called by JNI
void WebViewCore::ClearTextEntry()
{
    if (mWebView.Get() == NULL) {
        return;
    }
    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    (((CWebView*)(mWebView.Get())) -> mPrivateHandler)->SendMessage(CWebView::CLEAR_TEXT_ENTRY, params);
}

// called by JNI
void WebViewCore::SendFindAgain()
{
    if (mWebView.Get() == NULL) {
        return;
    }
    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    (((CWebView*)(mWebView.Get())) -> mPrivateHandler)->SendMessage(CWebView::FIND_AGAIN, params);
}

/*native*/
void WebViewCore::NativeUpdateFrameCacheIfLoading()
{}

/*native*/
CARAPI_(String) WebViewCore::NativeRequestLabel(
	/* [in] */ Int32 framePtr, 
	/* [in] */ Int32 nodePtr)
{//=0(virtual)
    return String(NULL);
}

/**
* Scroll the focused textfield to (xPercent, y) in document space
*/
/*native*/ 
CARAPI_(void) WebViewCore::NativeScrollFocusedTextInput(
	/* [in] */ Float xPercent, 
	/* [in] */ Int32 y)
{}

// these must be in document space (i.e. not scaled/zoomed).
/*native*/ 
CARAPI_(void) WebViewCore::NativeSetScrollOffset(
	/* [in] */ Int32 gen, 
	/* [in] */ Int32 dx, 
	/* [in] */ Int32 dy)
{}

/*native*/ 
CARAPI_(void) WebViewCore::NativeSetGlobalBounds(
	/* [in] */ Int32 x, 
	/* [in] */ Int32 y, 
	/* [in] */ Int32 w, 
	/* [in] */ Int32 h)
{}

// called by JNI
void WebViewCore::RequestListBox(
	/* [in] */ ArrayOf<String>* array, 
	/* [in] */ ArrayOf<Int32>* enabledArray,
	/* [in] */ ArrayOf<Int32>* selectedArray)
{
    if (mWebView.Get() != NULL) {
        ((CWebView*)(mWebView.Get())) -> RequestListBox(array, enabledArray, selectedArray);
    }
}

// called by JNI
void WebViewCore::RequestListBox(
	/* [in] */ ArrayOf<String>* array, 
	/* [in] */ ArrayOf<Int32>* enabledArray,
	/* [in] */ Int32 selection)
{
    if (mWebView.Get() != NULL) {
        ((CWebView*)(mWebView.Get())) -> RequestListBox(array, enabledArray, selection);
    }
}

// called by JNI
void WebViewCore::RequestKeyboardWithSelection(
	/* [in] */ Int32 pointer, 
	/* [in] */ Int32 selStart,
	/* [in] */ Int32 selEnd, 
	/* [in] */ Int32 textGeneration)
{
    if (mWebView.Get() != NULL) {
        TextSelectionData* textSelectionDataT = new TextSelectionData(selStart, selEnd);
        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        params->WriteInt32(pointer);
        params->WriteInt32(textGeneration);
        params->WriteStruct((Handle32)textSelectionDataT,sizeof(TextSelectionData));
        (((CWebView*)(mWebView.Get()))->mPrivateHandler)->SendMessage(CWebView::REQUEST_KEYBOARD_WITH_SELECTION_MSG_ID, params);
        delete textSelectionDataT;
    }
}

// called by JNI
void WebViewCore::RequestKeyboard(
	/* [in] */ Boolean showKeyboard)
{
    if (mWebView.Get() != NULL) {
        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        params->WriteInt32(showKeyboard ? 1 : 0);
        (((CWebView*)(mWebView.Get()))->mPrivateHandler)->SendMessage(CWebView::REQUEST_KEYBOARD, params);
    }
}

// called by JNI
CARAPI_(AutoPtr<IContext>) WebViewCore::GetContext()
{    
    return mContext;
}

// called by JNI
IInterface* WebViewCore::GetPluginClass(
	/* [in] */ const String& libName, 
	/* [in] */ const String& clsName)
{
    if (mWebView.Get() == NULL) {
        return NULL;
    }
    AutoPtr<IPluginManager> pluginManager;
    //pluginManager = CPluginManager::GetInstance(NULL);
    String pkgName;
    ((CPluginManager*)(pluginManager.Get())) -> GetPluginsAPKName(libName,&pkgName);
    if (pkgName == NULL) {
        String strOut = String("Unable to resolve ") + libName;
        strOut += String(" to a plugin APK");
        strOut += String("\n");
        Utility::Logging::Logger::V(LOGTAG, strOut);
        return NULL;
    }
    try {
        Handle32 pluginClass;
        ((CPluginManager*)(pluginManager.Get())) -> GetPluginClass(pkgName, clsName, &pluginClass);
        return (IInterface*)pluginClass;
    } 
    catch() {
        //JAVA:catch (NameNotFoundException e)
        String strOut = String("Unable to find plugin classloader for the apk (") + pkgName + String(")\n");
        Utility::Logging::Logger::E(LOGTAG, strOut);
        //JAVA:catch (ClassNotFoundException e)
        strOut = String("Unable to find plugin class (") + clsName + String(") in the apk (")+ pkgName + String(")\n");
        Utility::Logging::Logger::E(LOGTAG, strOut);
    }
    return NULL;
}

// called by JNI. PluginWidget function to launch a full-screen view using a
// View object provided by the plugin class.
void WebViewCore::ShowFullScreenPlugin(
	/* [in] */ ViewManager::ChildView* childView, 
	/* [in] */ Int32 npp)
{
    if (mWebView.Get() == NULL) {
        return;
    }
    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32(npp);
    params->WriteInterfacePtr((IInterface*)(childView->mView));
    (((CWebView*)(mWebView.Get()))->mPrivateHandler)->SendMessage(CWebView::SHOW_FULLSCREEN, params);
}

// called by JNI
void WebViewCore::HideFullScreenPlugin()
{
    if (mWebView.Get() == NULL) {
        return;
    }
    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    (((CWebView*)(mWebView.Get()))->mPrivateHandler)->SendMessage(CWebView::HIDE_FULLSCREEN, params);
}

// called by JNI.  PluginWidget functions for creating an embedded View for
// the surface drawing model.
ViewManager::ChildView* WebViewCore::AddSurface(
	/* [in] */ IView* pluginView, 
	/* [in] */ Int32 x, 
	/* [in] */ Int32 y,
	/* [in] */ Int32 width, 
	/* [in] */ Int32 height)
{
    if (mWebView.Get() == NULL) {
        return NULL;
    }

    if (pluginView == NULL) {
        String strOut = String("Attempted to add an empty plugin view to the view hierarchy\n");
        Utility::Logging::Logger::E(LOGTAG, strOut);
        return NULL;
    }

    // ensures the view system knows the view can redraw itself
    pluginView -> SetWillNotDraw(FALSE);
    AutoPtr<ISurfaceView> surfaceViewT;
    surfaceViewT = ISurfaceView::Probe(pluginView);
    if(surfaceViewT.Get() != NULL) {
        surfaceViewT -> SetZOrderOnTop(TRUE);
    }

    AutoPtr<ViewManager::ChildView> view = ( ((CWebView*)(mWebView.Get())) -> mViewManager ) -> CreateView();
    view -> mView = pluginView;
    view -> AttachView(x, y, width, height);
    return view;
}

void WebViewCore::UpdateSurface(
	/* [in] */ ViewManager::ChildView* childView, 
	/* [in] */ Int32 x, 
	/* [in] */ Int32 y,
	/* [in] */ Int32 width, 
	/* [in] */ Int32 height)
{    
    childView -> AttachView(x, y, width, height);
}

void WebViewCore::DestroySurface(
	/* [in] */ ViewManager::ChildView* childView)
{
    childView -> RemoveView();
}

void WebViewCore::ShowRect(
	/* [in] */ Int32 left,
	/* [in] */ Int32 top, 
	/* [in] */ Int32 width, 
	/* [in] */ Int32 height,
	/* [in] */ Int32 contentWidth, 
	/* [in] */ Int32 contentHeight, 
	/* [in] */ Float xPercentInDoc,
	/* [in] */ Float xPercentInView, 
	/* [in] */ Float yPercentInDoc, 
	/* [in] */ Float yPercentInView)
{
    if (mWebView.Get() != NULL) {
        ShowRectData* data = new ShowRectData();
        data -> mLeft = left;
        data -> mTop = top;
        data -> mWidth = width;
        data -> mHeight = height;
        data -> mContentWidth = contentWidth;
        data -> mContentHeight = contentHeight;
        data -> mXPercentInDoc = xPercentInDoc;
        data -> mXPercentInView = xPercentInView;
        data -> mYPercentInDoc = yPercentInDoc;
        data -> mYPercentInView = yPercentInView;

        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        params->WriteStruct((Handle32)data,sizeof(ShowRectData));
        (((CWebView*)(mWebView.Get()))->mPrivateHandler)->SendMessage(CWebView::SHOW_RECT_MSG_ID, params);
    }
}

// called by JNI
void WebViewCore::CenterFitRect(
	/* [in] */ Int32 x, 
	/* [in] */ Int32 y, 
	/* [in] */ Int32 width, 
	/* [in] */ Int32 height)
{
    if (mWebView.Get() == NULL) {
        return;
    }
    AutoPtr<IRect> rect;
    CRect::New(x, y, x + width, y + height, (IRect**)&rect);

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr((IInterface*)(rect));
    (((CWebView*)(mWebView.Get()))->mPrivateHandler)->SendMessage(CWebView::CENTER_FIT_RECT, params);
}

// called by JNI
void WebViewCore::SetScrollbarModes(
	/* [in] */ Int32 hMode, 
	/* [in] */ Int32 vMode)
{
    if (mWebView.Get() == NULL) {
            return;
    }
    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32(hMode);
    params->WriteInt32(vMode);
    (((CWebView*)(mWebView.Get()))->mPrivateHandler)->SendMessage(CWebView::SET_SCROLLBAR_MODES, params);
}

/*native*/
CARAPI_(void) WebViewCore::NativePause()
{}

/*native*/
CARAPI_(void) WebViewCore::NativeResume()
{}

/*native*/
CARAPI_(void) WebViewCore::NativeFreeMemory()
{}

/*native*/
CARAPI_(void) WebViewCore::NativeFullScreenPluginHidden(
	/* [in] */ Int32 npp)
{}

/*native*/
CARAPI_(Boolean) WebViewCore::NativeValidNodeAndBounds(
	/* [in] */ Int32 frame, 
	/* [in] */ Int32 node,
	/* [in] */ IRect* bounds)
{//=0(virtual)
    return FALSE;
}

WebViewCore::~WebViewCore()
{
    if(mRestoreState != NULL)
    {
        delete mRestoreState;
        mRestoreState =NULL;
    }
}