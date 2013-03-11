
#include "webkit/CGeolocationPermissions.h"

// Log tag
const char* CGeolocationPermissions::TAG = "geolocationPermissions";

const char* CGeolocationPermissions::ORIGINS = "origins";
const char* CGeolocationPermissions::ORIGIN = "origin";
const char* CGeolocationPermissions::CALLBACK = "callback";
const char* CGeolocationPermissions::ALLOWED = "allowed";

ECode CGeolocationPermissions::GetInstance(
    /* [out] */ IGeolocationPermissions ** ppInstance)
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
    /* [in] */ IValueCallback * pCallBack)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CGeolocationPermissions::GetAllowed(
    /* [in] */ CString origin,
    /* [in] */ IValueCallback * pCallBack)
{
    if (pCallBack == NULL)
    {
        return E_NOT_IMPLEMENTED;
    }

    if (origin.GetLength() == 0)
    {
        pCallBack->OnReceiveValue(NULL);
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
        values->Put((String)CALLBACK, (IInterface*)pCallBack);
//        PostMessage(Message.obtain(null, GET_ALLOWED, values));
    }

    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
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
    return false;
}

CARAPI_(void) CGeolocationPermissions::NativeClear(
    /* [in] */ const String& origin)
{}

CARAPI_(void) CGeolocationPermissions::NativeAllow(
    /* [in] */ const String& origin)
{}

CARAPI_(void) CGeolocationPermissions::NativeClearAll()
{}