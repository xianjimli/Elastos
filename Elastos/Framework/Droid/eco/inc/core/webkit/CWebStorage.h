
#ifndef __CWEBSTORAGE_H__
#define __CWEBSTORAGE_H__

#include "_CWebStorage.h"
#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/Vector.h>

CarClass(CWebStorage)
{
public:
    CARAPI CreateUIHandler();

    CARAPI CreateHandler();

    CARAPI GetOrigins(
        /* [in] */ IValueCallback* callBack);

    CARAPI GetUsageForOrigin(
        /* [in] */ CString origin,
        /* [in] */ IValueCallback* callBack);

    CARAPI GetQuotaForOrigin(
        /* [in] */ CString origin,
        /* [in] */ IValueCallback* callBack);

    CARAPI SetQuotaForOrigin(
        /* [in] */ CString origin,
        /* [in] */ Int64 quota);

    CARAPI DeleteOrigin(
        /* [in] */ CString origin);

    CARAPI DeleteAllData();

    CARAPI SetAppCacheMaximumSize(
        /* [in] */ Int64 size);

    CARAPI GetInstance(
        /* [out] */ IWebStorage** instance);

    CARAPI Update();

public:
    // Message ids
    static const Int32 UPDATE = 0;
    static const Int32 SET_QUOTA_ORIGIN = 1;
    static const Int32 DELETE_ORIGIN = 2;
    static const Int32 DELETE_ALL = 3;
    static const Int32 GET_ORIGINS = 4;
    static const Int32 GET_USAGE_ORIGIN = 5;
    static const Int32 GET_QUOTA_ORIGIN = 6;

    // Message ids on the UI thread
    static const Int32 RETURN_ORIGINS = 0;
    static const Int32 RETURN_USAGE_ORIGIN = 1;
    static const Int32 RETURN_QUOTA_ORIGIN = 2;

    class Origin
    {
    public:
        Origin(
            /* [in] */ const String& origin,
            /* [in] */ Int64 quota,
            /* [in] */ Int64 usage);

        Origin(
            /* [in] */ const String& origin,
            /* [in] */ Int64 quota);

        Origin(
            /* [in] */ const String& origin);

        CARAPI_(void) GetOrigin(
            /* [out] */ String& str);

        CARAPI_(Int64) GetQuota();

        CARAPI_(Int64) GetUsage();

    public:
        String mOrigin;
        Int64 mQuota;
        Int64 mUsage;
    };

public:
    /**
     * Returns a list of origins having a database
     * should only be called from WebViewCore.
     */
    CARAPI_(void) GetOriginsSync(
        /* [out] */ Vector<Origin>& list);


private:
    // Log tag
    static const CString TAG;// = "webstorage";

    static const CString ORIGINS;// = "origins";
    static const CString ORIGIN;// = "origin";
    static const CString CALLBACK;// = "callback";
    static const CString USAGE;// = "usage";
    static const CString QUOTA;// = "quota";

private:
    /**
     * Utility function to send a message to our handler
     */
    /*synchronized*/
    CARAPI_(void) PostMessage(
        /* [in] */ IMessage* msg);

    /**
     * Utility function to send a message to the handler on the UI thread
     */
    CARAPI_(void) PostUIMessage(
        /* [in] */ IMessage* msg);

    /**
     * Run on the webcore thread
     * set the local values with the current ones
     */
    CARAPI_(void) SyncValues();

    // Native functions
//    static CARAPI_(Set) NativeGetOrigins();

    static CARAPI_(Int64) NativeGetUsageForOrigin(
        /* [in] */ const String& origin);

    static CARAPI_(Int64) NativeGetQuotaForOrigin(
        /* [in] */ const String& origin);

    static CARAPI_(void) NativeSetQuotaForOrigin(
        /* [in] */ const String& origin,
        /* [in] */ Int64 quota);

    static CARAPI_(void) NativeDeleteOrigin(
        /* [in] */ const String& origin);

    static CARAPI_(void) NativeDeleteAllData();

    static CARAPI_(void) NativeSetAppCacheMaximumSize(
        /* [in] */ Int64 size);

private:
    // Global instance of a WebStorage
    static CWebStorage* sWebStorage;
    AutoPtr<IObjectStringMap> mOrigins;

    AutoPtr<IHandler> mHandler;
    AutoPtr<IHandler> mUIHandler;
};

#endif // __CWEBSTORAGE_H__
