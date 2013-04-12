
#include "webkit/CGeolocationPermissions.h"

// Log tag
const CString CGeolocationPermissions::TAG = "geolocationPermissions";

const CString CGeolocationPermissions::ORIGINS = "origins";
const CString CGeolocationPermissions::ORIGIN = "origin";
const CString CGeolocationPermissions::CALLBACK = "callback";
const CString CGeolocationPermissions::ALLOWED = "allowed";

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
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
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
//        pCallBack->OnReceiveValue(new Boolean(allowed));
    }
//    else
    {
        IObjectStringMap* values = NULL;
        CObjectStringMap::New(&values);
        values->Put((String)ORIGIN, (IInterface*)&origin);
        values->Put((String)CALLBACK, (IInterface*)callBack);
//        PostMessage(Message.obtain(null, GET_ALLOWED, values));
    }

    return NOERROR;
}

ECode CGeolocationPermissions::Clear(
    /* [in] */ CString origin)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CGeolocationPermissions::Allow(
    /* [in] */ CString origin)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CGeolocationPermissions::ClearAll()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

// Native functions, run on the WebKit thread.
//    static CARAPI_(Set) NativeGetOrigins();

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