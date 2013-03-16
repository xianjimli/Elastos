#ifndef __WEBVIEWWORKER_H__
#define __WEBVIEWWORKER_H__

#include <utils/Looper.h>

class LoadListener;

/**
 * WebViewWorker executes in a separate thread other than UI and WebViewCore. To
 * avoid blocking UI or WebKit's execution, the caller can send a message to
 * WebViewWorker.getHandler() and it will be handled in the WebViewWorkerThread.
 */
class WebViewWorker// : public Handler 
{

public:
    /**
     * Package level class to be used while creating a cache entry.
     */
	struct CacheCreateData 
	{
        LoadListener* mListener;
        CString mUrl;
        CString mMimeType;
        Int32 mStatusCode;
        Int64 mPostId;
        IHeaders* mHeaders;
    };

    /**
     * Package level class to be used while saving a cache entry.
     */
	struct CacheSaveData 
	{
        LoadListener* mListener;
        String mUrl;
        Int64 mPostId;
    };

    /**
     * Package level class to be used while updating a cache entry's encoding.
     */
	struct CacheEncoding 
	{
        LoadListener* mListener;
        CString mEncoding;
    };

    /**
     * Package level class to be used while appending data to a cache entry.
     */
	struct CacheData 
	{
        LoadListener* mListener;
        IByteArrayBuilderChunk* mChunk;
    };

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

    //@Override
	virtual CARAPI_(void) HandleMessage(
		/* [in] */ IMessage* msg);

private:
	WebViewWorker(
		/* [in] */ /*Looper* looper*/);

    // trigger transaction once a minute
	static const Int32 CACHE_TRANSACTION_TICKER_INTERVAL = 60 * 1000;

	static Boolean mCacheTickersBlocked;

	static const CString THREAD_NAME;// = "WebViewWorkerThread";

	static WebViewWorker* sWorkerHandler;

//	static Map<LoadListener, CacheManager.CacheResult> mCacheResultMap;
};

#endif //__WEBVIEWWORKER_H__