
#ifndef __CGEOLOCATIONPERMISSIONS_H__
#define __CGEOLOCATIONPERMISSIONS_H__

#include "_CGeolocationPermissions.h"
#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/Vector.h>

CarClass(CGeolocationPermissions)
{
public:
    CARAPI GetInstance(
        /* [out] */ IGeolocationPermissions ** ppInstance);

    CARAPI CreateUIHandler();

    CARAPI CreateHandler();

    CARAPI GetOrigins(
        /* [in] */ IValueCallback * pCallBack);

    CARAPI GetAllowed(
        /* [in] */ CString origin,
        /* [in] */ IValueCallback * pCallBack);

    CARAPI Clear(
        /* [in] */ CString origin);

    CARAPI Allow(
        /* [in] */ CString origin);

    CARAPI ClearAll();

public:
    // Message ids
    static const Int32 GET_ORIGINS = 0;
    static const Int32 GET_ALLOWED = 1;
    static const Int32 CLEAR = 2;
    static const Int32 ALLOW = 3;
    static const Int32 CLEAR_ALL = 4;

    // Message ids on the UI thread
    static const Int32 RETURN_ORIGINS = 0;
    static const Int32 RETURN_ALLOWED = 1;

private:

    // Log tag
    static const char* TAG;// = "geolocationPermissions";

    static const char* ORIGINS;// = "origins";
    static const char* ORIGIN;// = "origin";
    static const char* CALLBACK;// = "callback";
    static const char* ALLOWED;// = "allowed";

private:
    /**
     * Utility function to send a message to our handler.
     */
    /*synchronized*/
    CARAPI_(void) PostMessage(
        /* [in] */ IMessage* msg);

    /**
     * Utility function to send a message to the handler on the UI thread
     */
    CARAPI_(void) PostUIMessage(
        /* [in] */ IMessage* msg);

    // Native functions, run on the WebKit thread.
//    static CARAPI_(Set) NativeGetOrigins();

    static CARAPI_(Boolean) NativeGetAllowed(
        /* [in] */ const String& origin);

    static CARAPI_(void) NativeClear(
        /* [in] */ const String& origin);

    static CARAPI_(void) NativeAllow(
        /* [in] */ const String& origin);

    static CARAPI_(void) NativeClearAll();

private:
    // Global instance
    static CGeolocationPermissions* sInstance;

    AutoPtr<IHandler> mHandler;
    AutoPtr<IHandler> mUIHandler;

    // A queue to store messages until the handler is ready.
    Vector<AutoPtr<IMessage> > mQueuedMessages;
};

#endif // __CGEOLOCATIONPERMISSIONS_H__
