
#ifndef __CGEOLOCATIONPERMISSIONS_H__
#define __CGEOLOCATIONPERMISSIONS_H__

#include "_CGeolocationPermissions.h"
#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/Vector.h>
#include <elastos/ElRefBase.h>
#include <elastos/Set.h>
#include <elastos/HashMap.h>

CarClass(CGeolocationPermissions)
{
public:
    CARAPI GetInstance(
        /* [out] */ IGeolocationPermissions** instance);

    CARAPI CreateUIHandler();

    CARAPI CreateHandler();

    CARAPI GetOrigins(
        /* [in] */ IValueCallback* callBack);

    CARAPI GetAllowed(
        /* [in] */ CString origin,
        /* [in] */ IValueCallback* callBack);

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
    static const CString TAG;// = "geolocationPermissions";

    static const CString ORIGINS;// = "origins";
    static const CString ORIGIN;// = "origin";
    static const CString CALLBACK;// = "callback";
    static const CString ALLOWED;// = "allowed";

private:
    /**
     * Utility function to send a message to our handler.
     */
    /*synchronized*/
    CARAPI_(void) PostMessage(
        /* [in] */ Handle32 handle,
        /* [in] */ IParcel* params);

    /**
     * Utility function to send a message to the handler on the UI thread
     */
    CARAPI_(void) PostUIMessage(
        /* [in] */ Handle32 handle,
        /* [in] */ IParcel* params);

    // Native functions, run on the WebKit thread.
    static CARAPI_(Set<String>*) NativeGetOrigins();

    static CARAPI_(Boolean) NativeGetAllowed(
        /* [in] */ const String& origin);

    static CARAPI_(void) NativeClear(
        /* [in] */ const String& origin);

    static CARAPI_(void) NativeAllow(
        /* [in] */ const String& origin);

    static CARAPI_(void) NativeClearAll();

private:
    CARAPI SendMessage(
        /* [in] */ IApartment* handle,
        /* [in] */ Handle32 pvFunc,
        /* [in] */ IParcel* params);

    CARAPI SendMessageAtTime(
        /* [in] */ IApartment* handle,
        /* [in] */ Handle32 pvFunc,
        /* [in] */ IParcel* params,
        /* [in] */ Millisecond64 uptimeMillis);

    CARAPI RemoveMessage(
        /* [in] */ IApartment* handle,
        /* [in] */ Handle32 func);

    CARAPI HandleGetOrigins(
        /* [in] */ IValueCallback* callback);

    CARAPI HandleReturnOrigins(
        /* [in] */ HashMap<String, IInterface*>* values);

    CARAPI HandleGetAllowed(
        /* [in] */ HashMap<String, IInterface*>* values);

    CARAPI HandleReturnAllowed(
        /* [in] */ HashMap<String, IInterface*>* values);

    CARAPI HandleClear(
        /* [in] */ String& obj);

    CARAPI HandleAllow(
        /* [in] */ String& obj);

    CARAPI HandleClearAll();

private:
    // Global instance
    static CGeolocationPermissions* sInstance;

    AutoPtr<IApartment> mHandler;
    AutoPtr<IApartment> mUIHandler;

    struct Message
    {
        Message(Handle32 func, AutoPtr<IParcel> _params) :
                  pFunc(func), params(_params) {}
        Handle32 pFunc;
        AutoPtr<IParcel> params;
    };

    // A queue to store messages until the handler is ready.
    Vector<Message*> mQueuedMessages;
};

class GeolocationPermissionsCallback: public ElRefBase, public IGeolocationPermissionsCallback
{
public: 
    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface* Object,
        /* [out] */ InterfaceID* iID);

    virtual CARAPI Invoke(
        /* [in] */ const String& origin, 
        /* [in] */ Boolean allow, 
        /* [in] */ Boolean remember) = 0;

};

#endif // __CGEOLOCATIONPERMISSIONS_H__
