#ifndef __WEBVIEWWORKER_H__
#define __WEBVIEWWORKER_H__

#include "ext/frameworkext.h"
//#include <utils/Looper.h>
#include <elastos/Map.h>
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>

#include "CCacheManager.h"
#include "LoadListener.h"

#include "webkit/StreamLoader.h"
#include "webkit/FrameLoader.h"

#include <elastos/ElRefBase.h>

using namespace Core;
using namespace Core::Threading;

/**
 * WebViewWorker executes in a separate thread other than UI and WebViewCore. To
 * avoid blocking UI or WebKit's execution, the caller can send a message to
 * WebViewWorker.getHandler() and it will be handled in the WebViewWorkerThread.
 */
class WebViewWorker: public ElRefBase, public IApartment 
{

public:
    /**
     * Package level class to be used while creating a cache entry.
     */
	struct CacheCreateData 
	{
        AutoPtr<LoadListener> mListener;
        String mUrl;
        String mMimeType;
        Int32 mStatusCode;
        Int64 mPostId;
        IHeaders* mHeaders;
    };

    /**
     * Package level class to be used while saving a cache entry.
     */
	struct CacheSaveData 
	{
        AutoPtr<LoadListener> mListener;
        String mUrl;
        Int64 mPostId;
    };

    /**
     * Package level class to be used while updating a cache entry's encoding.
     */
	struct CacheEncoding 
	{
        AutoPtr<LoadListener> mListener;
        String mEncoding;
    };

    /**
     * Package level class to be used while appending data to a cache entry.
     */
	struct CacheData 
	{
        AutoPtr<LoadListener> mListener;
        AutoPtr<IByteArrayBuilderChunk> mChunk;
    };

public:
    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface* Object,
        /* [out] */ InterfaceID* iID);

public:
    //IApartment
    CARAPI Start(
        /* [in] */ ApartmentAttr attr);

    CARAPI Finish();

    CARAPI PostCppCallback(
        /* [in] */ Handle32 target,
        /* [in] */ Handle32 func,
        /* [in] */ IParcel* params,
        /* [in] */ Int32 id);

    CARAPI PostCppCallbackAtTime(
        /* [in] */ Handle32 target,
        /* [in] */ Handle32 func,
        /* [in] */ IParcel* params,
        /* [in] */ Int32 id,
        /* [in] */ Millisecond64 uptimeMillis);

    CARAPI PostCppCallbackDelayed(
        /* [in] */ Handle32 target,
        /* [in] */ Handle32 func,
        /* [in] */ IParcel* params,
        /* [in] */ Int32 id,
        /* [in] */ Millisecond64 delayMillis);

    CARAPI PostCppCallbackAtFrontOfQueue(
        /* [in] */ Handle32 target,
        /* [in] */ Handle32 func,
        /* [in] */ IParcel* params,
        /* [in] */ Int32 id);

    CARAPI RemoveCppCallbacks(
        /* [in] */ Handle32 target,
        /* [in] */ Handle32 func);

    CARAPI RemoveCppCallbacksEx(
        /* [in] */ Handle32 target,
        /* [in] */ Handle32 func,
        /* [in] */ Int32 id);

    CARAPI HasCppCallbacks(
        /* [in] */ Handle32 target,
        /* [in] */ Handle32 func,
        /* [out] */ Boolean* result);

    CARAPI HasCppCallbacksEx(
        /* [in] */ Handle32 target,
        /* [in] */ Handle32 func,
        /* [in] */ Int32 id,
        /* [out] */ Boolean* result);

    CARAPI SendMessage(
        /* [in] */ Int32 message,
        /* [in] */ IParcel* params);

public:
    Handle32 GetFunc(
        /* [in] */ Int32 message);

    void RemoveMessages(
        /* [in] */ Int32 message);

    void SendEmptyMessage(
        /* [in] */ Int32 message);

public:
    //MSG_ADD_STREAMLOADER
    void HandleMsgAddStreamloader(
        /* [in] */ StreamLoader* obj);

    //MSG_ADD_HTTPLOADER
    void HandleMsgAddHttploader(
        /* [in] */ FrameLoader* obj);

    //MSG_CREATE_CACHE
    void HandleMsgCreateCache(
        /* [in] */ CacheCreateData* obj);

    //MSG_UPDATE_CACHE_ENCODING
    void HandleMsgUpdateCacheEncoding(
        /* [in] */ CacheEncoding* obj);

    //MSG_APPEND_CACHE
    void HandleMsgAppendCache(
        /* [in] */ CacheData* obj);

    //MSG_SAVE_CACHE
    void HandleMsgSaveCache(
        /* [in] */ CacheSaveData* obj);

    //MSG_REMOVE_CACHE
    void HandleMsgRemoveCache(
        /* [in] */ LoadListener* obj);

    //MSG_TRIM_CACHE
    void HandleMsgTrimCache();

    //MSG_CLEAR_CACHE
    void HandleMsgClearCache();

    //MSG_CACHE_TRANSACTION_TICKER
    void HandleMsgCacheTransactionTicker();

    //MSG_PAUSE_CACHE_TRANSACTION
    void HandleMsgPauseCacheTransaction();

    //MSG_RESUME_CACHE_TRANSACTION
    void HandleMsgResumeCacheTransaction();

public:
	/* synchronized */
	static CARAPI_(WebViewWorker*) GetHandler();

    // message ids
    static const Int32 MSG_ADD_STREAMLOADER = 101;
    static const Int32 MSG_ADD_HTTPLOADER = 102;
    static const Int32 MSG_CREATE_CACHE = 103;
    static const Int32 MSG_UPDATE_CACHE_ENCODING = 104;
    static const Int32 MSG_APPEND_CACHE = 105;
    static const Int32 MSG_SAVE_CACHE = 106;
    static const Int32 MSG_REMOVE_CACHE = 107;
    static const Int32 MSG_TRIM_CACHE = 108;
    static const Int32 MSG_CLEAR_CACHE = 109;
    static const Int32 MSG_CACHE_TRANSACTION_TICKER = 110;
    static const Int32 MSG_PAUSE_CACHE_TRANSACTION = 111;
    static const Int32 MSG_RESUME_CACHE_TRANSACTION = 112;

protected:
    static Mutex mMutexClass;

private:
	WebViewWorker(
		/* [in] */ /*Looper* looper*/);

    // trigger transaction once a minute
	static const Int32 CACHE_TRANSACTION_TICKER_INTERVAL = 60 * 1000;

	static Boolean sCacheTickersBlocked;

	static const CString THREAD_NAME;// = "WebViewWorkerThread";

	static AutoPtr<WebViewWorker> sWorkerHandler;

	static Map<AutoPtr<LoadListener>, AutoPtr<ICacheManagerCacheResult> > mCacheResultMap;

private:
    AutoPtr<IApartment> mApartment;

};

#endif //__WEBVIEWWORKER_H__