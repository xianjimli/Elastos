
#include "webkit/CGeolocationPermissions.h"

// Log tag
const CString CGeolocationPermissions::TAG("geolocationPermissions");

const CString CGeolocationPermissions::ORIGINS("origins");
const CString CGeolocationPermissions::ORIGIN("origin");
const CString CGeolocationPermissions::CALLBACK("callback");
const CString CGeolocationPermissions::ALLOWED("allowed");

// Message ids
const Int32 CGeolocationPermissions::GET_ORIGINS;
const Int32 CGeolocationPermissions::GET_ALLOWED;
const Int32 CGeolocationPermissions::CLEAR;
const Int32 CGeolocationPermissions::ALLOW;
const Int32 CGeolocationPermissions::CLEAR_ALL;

// Message ids on the UI thread
const Int32 CGeolocationPermissions::RETURN_ORIGINS;
const Int32 CGeolocationPermissions::RETURN_ALLOWED;

ECode CGeolocationPermissions::GetInstance(
    /* [out] */ IGeolocationPermissions** instance)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CGeolocationPermissions::CreateUIHandler()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CGeolocationPermissions::CreateHandler()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CGeolocationPermissions::GetOrigins(
    /* [in] */ IValueCallback* callBack)
{
    if (callBack != NULL) {
        if (/*WebViewCore.THREAD_NAME.equals(Thread.currentThread().getName())*/0) {
            Set<String>* origins = NativeGetOrigins();
            callBack->OnReceiveValue((IInterface*)origins);
        } else {
            ECode (STDCALL CGeolocationPermissions::*pHandlerFunc)(IValueCallback*);

            pHandlerFunc = &CGeolocationPermissions::HandleGetOrigins;

            AutoPtr<IParcel> params;
            CCallbackParcel::New((IParcel**)&params);
            params->WriteInterfacePtr(callBack);
            PostMessage(*(Handle32*)&pHandlerFunc, params);
        }
    }

    return NOERROR;
}

ECode CGeolocationPermissions::GetAllowed(
    /* [in] */ CString origin,
    /* [in] */ IValueCallback* callBack)
{
    VALIDATE_NOT_NULL(callBack)

    if (origin.GetLength() == 0) {
        callBack->OnReceiveValue(NULL);
        return E_NOT_IMPLEMENTED;
    }

//    if (WebViewCore.THREAD_NAME.equals(Thread.currentThread().getName()))
    {
        Boolean allowed = NativeGetAllowed((String)(const char*)origin);
        callBack->OnReceiveValue((IInterface*)(new Boolean(allowed)));
    }
//    else
    {
        HashMap<String, IInterface*>* values = new HashMap<String, IInterface*>();
        (*values)[(String)ORIGIN] = (IInterface*)&origin;
        (*values)[(String)CALLBACK] = (IInterface*)callBack;

        ECode (CGeolocationPermissions::*pHandlerFunc)(HashMap<String, IInterface*>*);

        pHandlerFunc = &CGeolocationPermissions::HandleGetAllowed;

        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        params->WriteInterfacePtr((IInterface*)values);

        //PostMessage(Message.obtain(null, GET_ALLOWED, values));
        PostMessage(*(Handle32*)&pHandlerFunc, params);
    }

    return NOERROR;
}

ECode CGeolocationPermissions::Clear(
    /* [in] */ CString origin)
{
    // Called on the UI thread.
    //postMessage(Message.obtain(null, CLEAR, origin));

    ECode (CGeolocationPermissions::*pHandlerFunc)(String&);

    pHandlerFunc = &CGeolocationPermissions::HandleClear;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteString((String)(const char*)origin);

    PostMessage(*(Handle32*)&pHandlerFunc, params);

    return NOERROR;
}

ECode CGeolocationPermissions::Allow(
    /* [in] */ CString origin)
{
    // Called on the UI thread.
    //postMessage(Message.obtain(null, ALLOW, origin));

    ECode (CGeolocationPermissions::*pHandlerFunc)(String&);

    pHandlerFunc = &CGeolocationPermissions::HandleAllow;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteString((String)(const char*)origin);

    PostMessage(*(Handle32*)&pHandlerFunc, params);

    return NOERROR;
}

ECode CGeolocationPermissions::ClearAll()
{
    // Called on the UI thread.
    //postMessage(Message.obtain(null, CLEAR_ALL));

    ECode (CGeolocationPermissions::*pHandlerFunc)();

    pHandlerFunc = &CGeolocationPermissions::HandleClearAll;

    PostMessage(*(Handle32*)&pHandlerFunc, NULL);

    return NOERROR;
}

/**
 * Utility function to send a message to our handler.
 */
/*synchronized*/
CARAPI_(void) CGeolocationPermissions::PostMessage(
    /* [in] */ Handle32 handle,
    /* [in] */ IParcel* params)
{
    Mutex::Autolock lock(_m_syncLock);

    if (mHandler == NULL) {
        Message* msg = new Message(handle, params);
        mQueuedMessages.PushBack(msg);
    } else {
        SendMessage(mHandler, handle, params);
    }
}

/**
 * Utility function to send a message to the handler on the UI thread
 */
CARAPI_(void) CGeolocationPermissions::PostUIMessage(
    /* [in] */ Handle32 handle,
    /* [in] */ IParcel* params)
{
    if (mUIHandler != NULL) {
        SendMessage(mUIHandler, handle, params);
    }
}

// Native functions, run on the WebKit thread.
CARAPI_(Set<String>*) CGeolocationPermissions::NativeGetOrigins()
{}

CARAPI_(Boolean) CGeolocationPermissions::NativeGetAllowed(
    /* [in] */ const String& origin)
{
    return FALSE;
}

CARAPI_(void) CGeolocationPermissions::NativeClear(
    /* [in] */ const String& origin)
{}

CARAPI_(void) CGeolocationPermissions::NativeAllow(
    /* [in] */ const String& origin)
{}

CARAPI_(void) CGeolocationPermissions::NativeClearAll()
{}

/*****************************GeolocationPermissionsCallback*****************************/
PInterface GeolocationPermissionsCallback::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IGeolocationPermissionsCallback) {
        return (IInterface*)(IGeolocationPermissionsCallback*)this;
    }
    else if (riid == EIID_IGeolocationPermissionsCallback) {
        return (IGeolocationPermissionsCallback*)this;
    }
    return NULL;
}

UInt32 GeolocationPermissionsCallback::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 GeolocationPermissionsCallback::Release()
{
    return ElRefBase::Release();
}

ECode GeolocationPermissionsCallback::GetInterfaceID(
    /* [in] */ IInterface* Object,
    /* [out] */ InterfaceID* iID)
{
    VALIDATE_NOT_NULL(iID);
    if (iID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (Object == (IInterface*)(IGeolocationPermissionsCallback*)this) {
        *iID = EIID_IGeolocationPermissionsCallback;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

/**************************GeolocationPermissionsCallback***End*****************************/

ECode CGeolocationPermissions::SendMessage(
    /* [in] */ IApartment* handle,
    /* [in] */ Handle32 pvFunc,
    /* [in] */ IParcel* params)
{
    return handle->PostCppCallback((Handle32)this, pvFunc, params, 0);
}

ECode CGeolocationPermissions::SendMessageAtTime(
    /* [in] */ IApartment* handle,
    /* [in] */ Handle32 pvFunc,
    /* [in] */ IParcel* params,
    /* [in] */ Millisecond64 uptimeMillis)
{
    return handle->PostCppCallbackAtTime(
        (Handle32)this, pvFunc, params, 0, uptimeMillis);
}

ECode CGeolocationPermissions::RemoveMessage(
    /* [in] */ IApartment* handle,
    /* [in] */ Handle32 func)
{
    return handle->RemoveCppCallbacks((Handle32)this, func);
}

ECode CGeolocationPermissions::HandleGetOrigins(
    /* [in] */ IValueCallback* callback)
{
    Set<String>* origins = NativeGetOrigins();
    HashMap<String, IInterface*>* values = new HashMap<String, IInterface*>();
    (*values)[String(CALLBACK)] = callback;
    (*values)[String(ORIGINS)] = (IInterface*)origins;
//    postUIMessage(Message.obtain(null, RETURN_ORIGINS, values));

    ECode (STDCALL CGeolocationPermissions::*pHandlerFunc)(HashMap<String, IInterface*>*);

    pHandlerFunc = &CGeolocationPermissions::HandleReturnOrigins;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr((IInterface*)values);

    PostUIMessage(*(Handle32*)&pHandlerFunc, params);
}

ECode CGeolocationPermissions::HandleReturnOrigins(
    /* [in] */ HashMap<String, IInterface*>* values)
{
    VALIDATE_NOT_NULL(values);

    Set<String>* origins = (Set<String>*)(*values)[String(ORIGINS)];
    IValueCallback* callback = (IValueCallback*)(*values)[String(CALLBACK)];
    callback->OnReceiveValue((IInterface*)origins);
}

ECode CGeolocationPermissions::HandleGetAllowed(
    /* [in] */ HashMap<String, IInterface*>* values)
{
    VALIDATE_NOT_NULL(values);

    //Map values = (Map) msg.obj;
    String origin = *(String*) (*values)[String(ORIGIN)];
    AutoPtr<IValueCallback> callback = (IValueCallback*) (*values)[String(CALLBACK)];
    Boolean allowed = NativeGetAllowed(origin);
    HashMap<String, IInterface*>* retValues = new HashMap<String, IInterface*>();
    (*retValues)[String(CALLBACK)] = callback;
    (*retValues)[String(ALLOWED)] = (IInterface*)(new Boolean(allowed));
//    postUIMessage(Message.obtain(null, RETURN_ALLOWED, retValues));

    ECode (STDCALL CGeolocationPermissions::*pHandlerFunc)(HashMap<String, IInterface*>*);

    pHandlerFunc = &CGeolocationPermissions::HandleReturnAllowed;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr((IInterface*)retValues);

    PostUIMessage(*(Handle32*)&pHandlerFunc, params);
}

ECode CGeolocationPermissions::HandleReturnAllowed(
    /* [in] */ HashMap<String, IInterface*>* values)
{
    VALIDATE_NOT_NULL(values);

    //Map values = (Map) msg.obj;
    Boolean* allowed = (Boolean*) (*values)[String(ALLOWED)];
    AutoPtr<IValueCallback> callback = (IValueCallback*) (*values)[String(CALLBACK)];
    callback->OnReceiveValue((IInterface*)allowed);
}

ECode CGeolocationPermissions::HandleClear(
    /* [in] */ String& obj)
{
    NativeClear(obj);
}

ECode CGeolocationPermissions::HandleAllow(
    /* [in] */ String& obj)
{
    NativeAllow(obj);
}

ECode CGeolocationPermissions::HandleClearAll()
{
    NativeClearAll();
}