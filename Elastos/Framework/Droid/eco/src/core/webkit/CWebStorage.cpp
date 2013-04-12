
#include "webkit/CWebStorage.h"
#include <elastos/Thread.h>
#include <elastos/HashMap.h>

#include "webkit/WebViewCore.h"

// Log tag
const CString CWebStorage::TAG = "webstorage";

const CString CWebStorage::ORIGINS = "origins";
const CString CWebStorage::ORIGIN = "origin";
const CString CWebStorage::CALLBACK = "callback";
const CString CWebStorage::USAGE = "usage";
const CString CWebStorage::QUOTA = "quota";

CWebStorage* CWebStorage::sWebStorage = NULL;

ECode CWebStorage::CreateUIHandler()
{
    if (mUIHandler == NULL)
    {
        class UIHandler : public IHandler
        {
        public:
            //@Override
            CARAPI_(void) HandleMessage(
                /* [in] */ IMessage* msg)
            {
            #if 0
                switch (msg->what)
                {
                    case RETURN_ORIGINS:
                        {
                            Map values = (Map) msg.obj;
                            Map origins = (Map) values.get(ORIGINS);
                            ValueCallback<Map> callback = (ValueCallback<Map>) values.get(CALLBACK);
                            callback.onReceiveValue(origins);
                        }
                        break;

                    case RETURN_USAGE_ORIGIN:
                        {
                            Map values = (Map) msg.obj;
                            ValueCallback<Long> callback = (ValueCallback<Long>) values.get(CALLBACK);
                            callback.onReceiveValue((Long)values.get(USAGE));
                        }
                        break;

                    case RETURN_QUOTA_ORIGIN:
                        {
                            Map values = (Map) msg.obj;
                            ValueCallback<Long> callback = (ValueCallback<Long>) values.get(CALLBACK);
                            callback.onReceiveValue((Long)values.get(QUOTA));
                        }
                        break;
                }
            #endif
            }
        };

//        mUIHandler = new UIHandler();
    }

    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebStorage::CreateHandler()
{
    if (mHandler == NULL)
    {

        class newHandler : public IHandler
        {
        public:
            //@Override
            CARAPI_(void) HandleMessage(
                /* [in] */ IMessage* msg)
            {
            #if 0
                switch (msg->what)
                {
                    case SET_QUOTA_ORIGIN:
                        {
                            Origin website = (Origin) msg.obj;
                            nativeSetQuotaForOrigin(website.getOrigin(),
                                                    website.getQuota());
                        }
                        break;

                    case DELETE_ORIGIN:
                        {
                            Origin website = (Origin) msg.obj;
                            nativeDeleteOrigin(website.getOrigin());
                        }
                        break;

                    case DELETE_ALL:
                        nativeDeleteAllData();
                        break;

                    case GET_ORIGINS:
                        {
                            syncValues();
                            ValueCallback callback = (ValueCallback) msg.obj;
                            Map origins = new HashMap(mOrigins);
                            Map values = new HashMap<String, Object>();
                            values.put(CALLBACK, callback);
                            values.put(ORIGINS, origins);
                            postUIMessage(Message.obtain(null, RETURN_ORIGINS, values));
                        }
                        break;

                    case GET_USAGE_ORIGIN:
                        {
                            syncValues();
                            Map values = (Map) msg.obj;
                            String origin = (String) values.get(ORIGIN);
                            ValueCallback callback = (ValueCallback) values.get(CALLBACK);
                            Origin website = mOrigins.get(origin);
                            Map retValues = new HashMap<String, Object>();
                            retValues.put(CALLBACK, callback);
                            if (website != null) {
                                long usage = website.getUsage();
                                retValues.put(USAGE, new Long(usage));
                            }
                            postUIMessage(Message.obtain(null, RETURN_USAGE_ORIGIN, retValues));
                        }
                        break;

                    case GET_QUOTA_ORIGIN:
                        {
                            syncValues();
                            Map values = (Map) msg.obj;
                            String origin = (String) values.get(ORIGIN);
                            ValueCallback callback = (ValueCallback) values.get(CALLBACK);
                            Origin website = mOrigins.get(origin);
                            Map retValues = new HashMap<String, Object>();
                            retValues.put(CALLBACK, callback);
                            if (website != null) {
                                long quota = website.getQuota();
                                retValues.put(QUOTA, new Long(quota));
                            }
                            postUIMessage(Message.obtain(null, RETURN_QUOTA_ORIGIN, retValues));
                        }
                        break;

                    case UPDATE:
                        syncValues();
                        break;
                }
            #endif
            }
        };

//        mHandler = new newHandler();
    }

    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebStorage::GetOrigins(
    /* [in] */ IValueCallback* callBack)
{
    if (callBack != NULL) {
        AutoPtr<IThread> pthread = Thread::GetCurrentThread();
        String name;

        pthread->GetName(&name);

        if (name.Equals(WebViewCore::THREAD_NAME)) {
            SyncValues();
            callBack->OnReceiveValue(mOrigins);
        } else {
//            PostMessage(Message.obtain(null, GET_ORIGINS, callback));
        }
    }

    return NOERROR;
}

ECode CWebStorage::GetUsageForOrigin(
    /* [in] */ CString origin,
    /* [in] */ IValueCallback* callBack)
{
    VALIDATE_NOT_NULL(callBack);

    if (origin.GetLength() == 0) {
        callBack->OnReceiveValue(NULL);
        return E_NOT_IMPLEMENTED;
    }

    AutoPtr<IThread> pthread = Thread::GetCurrentThread();
    String name;

    pthread->GetName(&name);

    if (name.Equals(WebViewCore::THREAD_NAME)) {
        SyncValues();
        Origin* website = NULL;
        mOrigins->Get((String)origin, (IInterface**)&website);
//        pCallBack->OnReceiveValue(website->GetUsage());
    }
    else
    {
#if 0
        HashMap values = new HashMap<String, Object>();
        values.put(ORIGIN, origin);
        values.put(CALLBACK, callback);
        postMessage(Message.obtain(null, GET_USAGE_ORIGIN, values));
#endif
    }

    return NOERROR;
}

ECode CWebStorage::GetQuotaForOrigin(
    /* [in] */ CString origin,
    /* [in] */ IValueCallback* callBack)
{
    VALIDATE_NOT_NULL(callBack);

    if (origin.GetLength() == 0) {
        callBack->OnReceiveValue(NULL);
        return E_NOT_IMPLEMENTED;
    }

    AutoPtr<IThread> pthread = Thread::GetCurrentThread();
    String name;

    pthread->GetName(&name);

    if (name.Equals(WebViewCore::THREAD_NAME)) {
        SyncValues();
        Origin* website = NULL;
        mOrigins->Get((String)origin, (IInterface**)&website);
//        pCallBack->OnReceiveValue(website->GetUsage()));
    } else {
#if 0
        HashMap values = new HashMap<String, Object>();
        values.put(ORIGIN, origin);
        values.put(CALLBACK, callback);
        postMessage(Message.obtain(null, GET_QUOTA_ORIGIN, values));
#endif
    }

    return NOERROR;
}

ECode CWebStorage::SetQuotaForOrigin(
    /* [in] */ CString origin,
    /* [in] */ Int64 quota)
{
    if (origin.GetLength() != 0) {
        AutoPtr<IThread> pthread = Thread::GetCurrentThread();
        String name;

        pthread->GetName(&name);

        if (name.Equals(WebViewCore::THREAD_NAME)) {
            NativeSetQuotaForOrigin((const String)origin, quota);
        } else {
//            PostMessage(Message.obtain(null, SET_QUOTA_ORIGIN, new Origin(origin, quota)));
        }
    }

    return NOERROR;
}

ECode CWebStorage::DeleteOrigin(
    /* [in] */ CString origin)
{
    if (origin.GetLength() != 0) {
        AutoPtr<IThread> pthread = Thread::GetCurrentThread();
        String name;

        pthread->GetName(&name);

        if (name.Equals(WebViewCore::THREAD_NAME)) {
            NativeDeleteOrigin((const String)origin);
        } else {
//            PostMessage(Message.obtain(null, DELETE_ORIGIN, new Origin(origin)));
        }
    }

    return NOERROR;
}

ECode CWebStorage::DeleteAllData()
{
    AutoPtr<IThread> pthread = Thread::GetCurrentThread();
    String name;

    pthread->GetName(&name);

    if (name.Equals(WebViewCore::THREAD_NAME)) {
        NativeDeleteAllData();
    } else {
//        PostMessage(Message.obtain(null, DELETE_ALL));
    }

    return NOERROR;
}

ECode CWebStorage::SetAppCacheMaximumSize(
    /* [in] */ Int64 size)
{
    NativeSetAppCacheMaximumSize(size);

    return NOERROR;
}

ECode CWebStorage::GetInstance(
    /* [out] */ IWebStorage** instance)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebStorage::Update()
{
    AutoPtr<IThread> pthread = Thread::GetCurrentThread();
    String name;

    pthread->GetName(&name);

    if (name.Equals(WebViewCore::THREAD_NAME)) {
        SyncValues();
    } else {
//        PostMessage(Message.obtain(null, UPDATE));
    }

    return NOERROR;
}

/**
 * Utility function to send a message to our handler
 */
/*synchronized*/
CARAPI_(void) CWebStorage::PostMessage(
    /* [in] */ IMessage* msg)
{
    Mutex::Autolock lock(_m_syncLock);

    if (mHandler != NULL && msg != NULL) {
//        mHandler->SendMessage(msg);
    }
}

/**
 * Utility function to send a message to the handler on the UI thread
 */
CARAPI_(void) CWebStorage::PostUIMessage(
    /* [in] */ IMessage* msg)
{
    if (mUIHandler != NULL && msg != 0) {
//        mUIHandler->SendMessage(msg);
    }
}

/**
 * Run on the webcore thread
 * set the local values with the current ones
 */
CARAPI_(void) CWebStorage::SyncValues()
{}

// Native functions
//    static CARAPI_(Set) NativeGetOrigins();

CARAPI_(Int64) CWebStorage::NativeGetUsageForOrigin(
    /* [in] */ const String& origin)
{
    return 0;
}

CARAPI_(Int64) CWebStorage::NativeGetQuotaForOrigin(
    /* [in] */ const String& origin)
{
    return 0;
}

CARAPI_(void) CWebStorage::NativeSetQuotaForOrigin(
    /* [in] */ const String& origin,
    /* [in] */ Int64 quota)
{
}

CARAPI_(void) CWebStorage::NativeDeleteOrigin(
    /* [in] */ const String& origin)
{}

CARAPI_(void) CWebStorage::NativeDeleteAllData()
{}

CARAPI_(void) CWebStorage::NativeSetAppCacheMaximumSize(
    /* [in] */ Int64 size)
{}

CARAPI_(void) CWebStorage::GetOriginsSync(
        /* [out] */ Vector< AutoPtr<Origin> >* list)
{    
}

/*****************************Origin*********************************/
CWebStorage::Origin::Origin(
    /* [in] */ const String& origin,
    /* [in] */ Int64 quota,
    /* [in] */ Int64 usage) : mOrigin(origin), mQuota(quota), mUsage(usage)
{
}

CWebStorage::Origin::Origin(
    /* [in] */ const String& origin,
    /* [in] */ Int64 quota) : mOrigin(origin), mQuota(quota)
{
}

CWebStorage::Origin::Origin(
    /* [in] */ const String& origin) : mOrigin(origin)
{
}

CARAPI_(void) CWebStorage::Origin::GetOrigin(
    /* [out] */ String& str)
{
    str = mOrigin;
}

CARAPI_(Int64) CWebStorage::Origin::GetQuota()
{
    return mQuota;
}

CARAPI_(Int64) CWebStorage::Origin::GetUsage()
{
    return mUsage;
}

/*****************************WebStorageQuotaUpdater*****************************/

PInterface WebStorageQuotaUpdater::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IWebStorageQuotaUpdater*)this;
    }
    else if (riid == EIID_IWebStorageQuotaUpdater) {
        return (IWebStorageQuotaUpdater*)this;
    }
    return NULL;
}

UInt32 WebStorageQuotaUpdater::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 WebStorageQuotaUpdater::Release()
{
    return ElRefBase::Release();
}

ECode WebStorageQuotaUpdater::GetInterfaceID(
    /* [in] */ IInterface* Object,
    /* [out] */ InterfaceID* iID)
{
    VALIDATE_NOT_NULL(iID);
    if (iID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (Object == (IInterface*)(IWebStorageQuotaUpdater*)this) {
        *iID = EIID_IWebStorageQuotaUpdater;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}