
#include "webkit/WebViewWorker.h"
#include "webkit/StreamLoader.h"
#include "webkit/FrameLoader.h"
#include "webkit/CByteArrayBuilder.h"

#include <elastos/Mutex.h>

const CString WebViewWorker::THREAD_NAME = "WebViewWorkerThread";
AutoPtr<WebViewWorker> WebViewWorker::sWorkerHandler;
//Map<LoadListener*, CCacheManager::CacheResult*> WebViewWorker::mCacheResultMap;
Map<AutoPtr<LoadListener>, AutoPtr<ICacheManagerCacheResult> > WebViewWorker::mCacheResultMap;

Mutex WebViewWorker::mMutexClass;

WebViewWorker::WebViewWorker(
	/* [in] */ /*Looper* looper*/)//:Handler(looper)
{}

/* synchronized */
CARAPI_(WebViewWorker*) WebViewWorker::GetHandler()
{
	Mutex::Autolock lock(mMutexClass);

	if (sWorkerHandler.Get() == NULL) {
        /*
        AutoPtr<IHandlerThread> thread;
        CHandlerThread::New(THREAD_NAME, Process::THREAD_PRIORITY_DEFAULT + Process::THREAD_PRIORITY_LESS_FAVORABLE, (IHandlerThread**)&thread);
        thread -> Start();
        sWorkerHandler = new WebViewWorker(thread -> GetLooper());
        */
    }

    return sWorkerHandler;
}

//@Override
void WebViewWorker::HandleMessage(
	/* [in] */ IMessage* msg)
{
	assert(msg != NULL);

	switch(/*msg.what*/0) {
        case MSG_ADD_STREAMLOADER: {
            StreamLoader* loader;// = (StreamLoader*) msg.obj;
            loader->Load();
            break;
        }

        case MSG_ADD_HTTPLOADER: {
            FrameLoader* loader;// = (FrameLoader*) msg.obj;
            loader->HandleHTTPLoad();
            break;
        }

        case MSG_CREATE_CACHE: {
            CacheCreateData* data;// = (CacheCreateData*) msg.obj;
            AutoPtr<ICacheManagerCacheResult> cache = CCacheManager::CreateCacheFile(
                    data->mUrl, data->mStatusCode, data->mHeaders,
                    data->mMimeType, data->mPostId, FALSE);

            if (cache != NULL) {
                mCacheResultMap[data->mListener] = cache;
            } else {
                mCacheResultMap.Erase(data->mListener);
            }
            break;
        }

        case MSG_UPDATE_CACHE_ENCODING: {
            CacheEncoding* data;// = (CacheEncoding*) msg.obj;
            CCacheManager::CacheResult* cache = (CCacheManager::CacheResult*)mCacheResultMap[data->mListener].Get();
            if (cache != NULL) {
                cache->encoding = data->mEncoding;
            }
            break;
        }

        case MSG_APPEND_CACHE: {
            CacheData* data;// = (CacheData) msg.obj;
            CCacheManager::CacheResult* cache = (CCacheManager::CacheResult*)mCacheResultMap[data->mListener].Get();
            if (cache != NULL) {
                cache->contentLength += ((CByteArrayBuilder::Chunk*)(data->mChunk.Get()))->mLength;
                if (cache->contentLength > CCacheManager::CACHE_MAX_SIZE) {
                    CCacheManager::CleanupCacheFile(cache);
                    mCacheResultMap.Erase(data->mListener);
                } else {
                    cache->outStream->WriteBufferEx( 0, ((CByteArrayBuilder::Chunk*)(data->mChunk.Get()))->mLength, *((((CByteArrayBuilder::Chunk*)(data->mChunk.Get()))->mArray).Get()));
                }
            }
            data->mChunk->Release();
            break;
        }

        case MSG_SAVE_CACHE: {
            CacheSaveData* data;// = (CacheSaveData*) msg.obj;
            CCacheManager::CacheResult* cache = (CCacheManager::CacheResult*)mCacheResultMap[data->mListener].Get();
            if (cache != NULL) {
                CCacheManager::SaveCacheFile(data->mUrl, data->mPostId, cache);
                mCacheResultMap.Erase(data->mListener);
            }
            break;
        }

        case MSG_REMOVE_CACHE: {
            LoadListener* listener;// = (LoadListener*) msg.obj;
            AutoPtr<ICacheManagerCacheResult> cache = mCacheResultMap[listener];
            if (cache != NULL) {
                CCacheManager::CleanupCacheFile(cache);
                mCacheResultMap.Erase(listener);
            }
            break;
        }

        case MSG_TRIM_CACHE: {
            CCacheManager::TrimCacheIfNeeded();
            break;
        }

        case MSG_CLEAR_CACHE: {
            CCacheManager::ClearCache();
            break;
        }

        case MSG_CACHE_TRANSACTION_TICKER: {
            if (!mCacheTickersBlocked) {
                CCacheManager::EndTransaction();
                CCacheManager::StartTransaction();
//                SendEmptyMessageDelayed(MSG_CACHE_TRANSACTION_TICKER, CACHE_TRANSACTION_TICKER_INTERVAL);
            }
            break;
        }

        case MSG_PAUSE_CACHE_TRANSACTION: {
            if (CCacheManager::DisableTransaction()) {
                mCacheTickersBlocked = TRUE;
//                RemoveMessages(MSG_CACHE_TRANSACTION_TICKER);
            }
            break;
        }

        case MSG_RESUME_CACHE_TRANSACTION: {
            if (CCacheManager::EnableTransaction()) {
                mCacheTickersBlocked = FALSE;
//                SendEmptyMessageDelayed(MSG_CACHE_TRANSACTION_TICKER, CACHE_TRANSACTION_TICKER_INTERVAL);
            }
            break;
        }
    }
}
