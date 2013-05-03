
#include "webkit/WebViewWorker.h"
#include "webkit/CByteArrayBuilder.h"

#include <elastos/Mutex.h>
#include "os/CApartment.h"

const CString WebViewWorker::THREAD_NAME = "WebViewWorkerThread";
AutoPtr<WebViewWorker> WebViewWorker::sWorkerHandler;
//Map<LoadListener*, CCacheManager::CacheResult*> WebViewWorker::mCacheResultMap;
Map<AutoPtr<LoadListener>, AutoPtr<ICacheManagerCacheResult> > WebViewWorker::mCacheResultMap;
Boolean WebViewWorker::sCacheTickersBlocked;
Mutex WebViewWorker::mMutexClass;

WebViewWorker::WebViewWorker(
	/* [in] */ /*Looper* looper*/)//:Handler(looper)
{    
    assert(SUCCEEDED(CApartment::GetMainApartment((IApartment**)&mApartment))
        && (mApartment != NULL));
}

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

PInterface WebViewWorker::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IApartment*)this;
    }
    else if (riid == EIID_IApartment) {
        return (IApartment*)this;
    }
    return NULL;
}

UInt32 WebViewWorker::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 WebViewWorker::Release()
{
    return ElRefBase::Release();
}

ECode WebViewWorker::GetInterfaceID(
    /* [in] */ IInterface* Object,
    /* [out] */ InterfaceID* iID)
{
    VALIDATE_NOT_NULL(iID);
    if (iID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (Object == (IInterface*)(IApartment*)this) {
        *iID = EIID_IApartment;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode WebViewWorker::Start(
    /* [in] */ ApartmentAttr attr)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode WebViewWorker::Finish()
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode WebViewWorker::PostCppCallback(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallback(target, func, params, id);
}

ECode WebViewWorker::PostCppCallbackAtTime(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id,
    /* [in] */ Millisecond64 uptimeMillis)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallbackAtTime(target, func, params, id, uptimeMillis);
}

ECode WebViewWorker::PostCppCallbackDelayed(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id,
    /* [in] */ Millisecond64 delayMillis)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallbackDelayed(target, func, params, id, delayMillis);
}

ECode WebViewWorker::PostCppCallbackAtFrontOfQueue(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallbackAtFrontOfQueue(target, func, params, id);
}

ECode WebViewWorker::RemoveCppCallbacks(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func)
{
    assert(mApartment != NULL);
    return mApartment->RemoveCppCallbacks(target, func);
}

ECode WebViewWorker::RemoveCppCallbacksEx(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ Int32 id)
{
    assert(mApartment != NULL);
    return mApartment->RemoveCppCallbacksEx(target, func, id);
}

ECode WebViewWorker::HasCppCallbacks(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [out] */ Boolean* result)
{
    assert(mApartment != NULL);
    return mApartment->HasCppCallbacks(target, func, result);
}

ECode WebViewWorker::HasCppCallbacksEx(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ Int32 id,
    /* [out] */ Boolean* result)
{
    assert(mApartment != NULL);
    return mApartment->HasCppCallbacksEx(target, func, id, result);
}

ECode WebViewWorker::SendMessage(
    /* [in] */ Int32 message,
    /* [in] */ IParcel* params)
{    
    if (message == MSG_ADD_STREAMLOADER) {
        void (STDCALL WebViewWorker::*pHandlerFunc)(StreamLoader*);
        pHandlerFunc = &WebViewWorker::HandleMsgAddStreamloader;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
    }
    if (message == MSG_ADD_HTTPLOADER) {
        void (STDCALL WebViewWorker::*pHandlerFunc)(FrameLoader*);
        pHandlerFunc = &WebViewWorker::HandleMsgAddHttploader;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
    }
    if (message == MSG_CREATE_CACHE) {
        void (STDCALL WebViewWorker::*pHandlerFunc)(CacheCreateData*);
        pHandlerFunc = &WebViewWorker::HandleMsgCreateCache;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
    }
    if (message == MSG_UPDATE_CACHE_ENCODING) {
        void (STDCALL WebViewWorker::*pHandlerFunc)(CacheEncoding*);
        pHandlerFunc = &WebViewWorker::HandleMsgUpdateCacheEncoding;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
    }
    if (message == MSG_APPEND_CACHE) {
        void (STDCALL WebViewWorker::*pHandlerFunc)(CacheData*);
        pHandlerFunc = &WebViewWorker::HandleMsgAppendCache;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
    }
    if (message == MSG_SAVE_CACHE) {
        void (STDCALL WebViewWorker::*pHandlerFunc)(CacheSaveData*);
        pHandlerFunc = &WebViewWorker::HandleMsgSaveCache;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
    }
    if (message == MSG_REMOVE_CACHE) {
        void (STDCALL WebViewWorker::*pHandlerFunc)(LoadListener*);
        pHandlerFunc = &WebViewWorker::HandleMsgRemoveCache;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
    }
    if (message == MSG_TRIM_CACHE) {
        void (STDCALL WebViewWorker::*pHandlerFunc)();
        pHandlerFunc = &WebViewWorker::HandleMsgTrimCache;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
    }
    if (message == MSG_CLEAR_CACHE) {
        void (STDCALL WebViewWorker::*pHandlerFunc)();
        pHandlerFunc = &WebViewWorker::HandleMsgClearCache;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
    }
    if (message == MSG_CACHE_TRANSACTION_TICKER) {
        void (STDCALL WebViewWorker::*pHandlerFunc)();
        pHandlerFunc = &WebViewWorker::HandleMsgCacheTransactionTicker;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
    }
    if (message == MSG_PAUSE_CACHE_TRANSACTION) {
        void (STDCALL WebViewWorker::*pHandlerFunc)();
        pHandlerFunc = &WebViewWorker::HandleMsgPauseCacheTransaction;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
    }
    if (message == MSG_RESUME_CACHE_TRANSACTION) {
        void (STDCALL WebViewWorker::*pHandlerFunc)();
        pHandlerFunc = &WebViewWorker::HandleMsgResumeCacheTransaction;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
    }
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

Handle32 WebViewWorker::GetFunc(
    /* [in] */ Int32 message)
{    
    if (message == MSG_ADD_STREAMLOADER) {
        void (STDCALL WebViewWorker::*pHandlerFunc)(StreamLoader*);
        pHandlerFunc = &WebViewWorker::HandleMsgAddStreamloader;
        return *(Handle32*)&pHandlerFunc;
    }
    if (message == MSG_ADD_HTTPLOADER) {
        void (STDCALL WebViewWorker::*pHandlerFunc)(FrameLoader*);
        pHandlerFunc = &WebViewWorker::HandleMsgAddHttploader;
        return *(Handle32*)&pHandlerFunc;
    }
    if (message == MSG_CREATE_CACHE) {
        void (STDCALL WebViewWorker::*pHandlerFunc)(CacheCreateData*);
        pHandlerFunc = &WebViewWorker::HandleMsgCreateCache;
        return *(Handle32*)&pHandlerFunc;
    }
    if (message == MSG_UPDATE_CACHE_ENCODING) {
        void (STDCALL WebViewWorker::*pHandlerFunc)(CacheEncoding*);
        pHandlerFunc = &WebViewWorker::HandleMsgUpdateCacheEncoding;
        return *(Handle32*)&pHandlerFunc;
    }
    if (message == MSG_APPEND_CACHE) {
        void (STDCALL WebViewWorker::*pHandlerFunc)(CacheData*);
        pHandlerFunc = &WebViewWorker::HandleMsgAppendCache;
        return *(Handle32*)&pHandlerFunc;
    }
    if (message == MSG_SAVE_CACHE) {
        void (STDCALL WebViewWorker::*pHandlerFunc)(CacheSaveData*);
        pHandlerFunc = &WebViewWorker::HandleMsgSaveCache;
        return *(Handle32*)&pHandlerFunc;
    }
    if (message == MSG_REMOVE_CACHE) {
        void (STDCALL WebViewWorker::*pHandlerFunc)(LoadListener*);
        pHandlerFunc = &WebViewWorker::HandleMsgRemoveCache;
        return *(Handle32*)&pHandlerFunc;
    }
    if (message == MSG_TRIM_CACHE) {
        void (STDCALL WebViewWorker::*pHandlerFunc)();
        pHandlerFunc = &WebViewWorker::HandleMsgTrimCache;
        return *(Handle32*)&pHandlerFunc;
    }
    if (message == MSG_CLEAR_CACHE) {
        void (STDCALL WebViewWorker::*pHandlerFunc)();
        pHandlerFunc = &WebViewWorker::HandleMsgClearCache;
        return *(Handle32*)&pHandlerFunc;
    }
    if (message == MSG_CACHE_TRANSACTION_TICKER) {
        void (STDCALL WebViewWorker::*pHandlerFunc)();
        pHandlerFunc = &WebViewWorker::HandleMsgCacheTransactionTicker;
        return *(Handle32*)&pHandlerFunc;
    }
    if (message == MSG_PAUSE_CACHE_TRANSACTION) {
        void (STDCALL WebViewWorker::*pHandlerFunc)();
        pHandlerFunc = &WebViewWorker::HandleMsgPauseCacheTransaction;
        return *(Handle32*)&pHandlerFunc;
    }
    if (message == MSG_RESUME_CACHE_TRANSACTION) {
        void (STDCALL WebViewWorker::*pHandlerFunc)();
        pHandlerFunc = &WebViewWorker::HandleMsgResumeCacheTransaction;
        return *(Handle32*)&pHandlerFunc;
    }
    return NULL;
}

void WebViewWorker::RemoveMessages(
    /* [in] */ Int32 message)
{
    RemoveCppCallbacks((Handle32)this, GetFunc(message));
}

void WebViewWorker::SendEmptyMessage(
    /* [in] */ Int32 message)
{
    PostCppCallback((Handle32)this, GetFunc(message), NULL, 0);
}

//MSG_ADD_STREAMLOADER
void WebViewWorker::HandleMsgAddStreamloader(
    /* [in] */ StreamLoader* obj)
{
    StreamLoader* loader = obj;
    loader->Load();
}

//MSG_ADD_HTTPLOADER
void WebViewWorker::HandleMsgAddHttploader(
    /* [in] */ FrameLoader* obj)
{
    FrameLoader* loader = obj;
    loader->HandleHTTPLoad();
}

//MSG_CREATE_CACHE
void WebViewWorker::HandleMsgCreateCache(
    /* [in] */ CacheCreateData* obj)
{
    CacheCreateData* data = obj;
    AutoPtr<ICacheManagerCacheResult> cache = CCacheManager::CreateCacheFile(
            data->mUrl, data->mStatusCode, data->mHeaders,
            data->mMimeType, data->mPostId, FALSE);

    if (cache != NULL) {
        mCacheResultMap[data->mListener] = cache;
    } else {
        mCacheResultMap.Erase(data->mListener);
    }
}

//MSG_UPDATE_CACHE_ENCODING
void WebViewWorker::HandleMsgUpdateCacheEncoding(
    /* [in] */ CacheEncoding* obj)
{
    CacheEncoding* data = obj;
    CCacheManager::CacheResult* cache = (CCacheManager::CacheResult*)mCacheResultMap[data->mListener].Get();
    if (cache != NULL) {
        cache->encoding = data->mEncoding;
    }    
}

//MSG_APPEND_CACHE
void WebViewWorker::HandleMsgAppendCache(
    /* [in] */ CacheData* obj)
{
    CacheData* data = obj;
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

}

//MSG_SAVE_CACHE
void WebViewWorker::HandleMsgSaveCache(
    /* [in] */ CacheSaveData* obj)
{
    CacheSaveData* data = obj;
    CCacheManager::CacheResult* cache = (CCacheManager::CacheResult*)mCacheResultMap[data->mListener].Get();
    if (cache != NULL) {
        CCacheManager::SaveCacheFile(data->mUrl, data->mPostId, cache);
        mCacheResultMap.Erase(data->mListener);
    }    
}

//MSG_REMOVE_CACHE
void WebViewWorker::HandleMsgRemoveCache(
    /* [in] */ LoadListener* obj)
{
    LoadListener* listener = obj;
    AutoPtr<ICacheManagerCacheResult> cache = mCacheResultMap[listener];
    if (cache != NULL) {
        CCacheManager::CleanupCacheFile(cache);
        mCacheResultMap.Erase(listener);
    }
}

//MSG_TRIM_CACHE
void WebViewWorker::HandleMsgTrimCache()
{
    CCacheManager::TrimCacheIfNeeded();
}

//MSG_CLEAR_CACHE
void WebViewWorker::HandleMsgClearCache()
{
    CCacheManager::ClearCache();
}

//MSG_CACHE_TRANSACTION_TICKER
void WebViewWorker::HandleMsgCacheTransactionTicker()
{
    if (!sCacheTickersBlocked) {
        CCacheManager::EndTransaction();
        CCacheManager::StartTransaction();
        void (STDCALL WebViewWorker::*pHandlerFunc)();
        pHandlerFunc = &WebViewWorker::HandleMsgCacheTransactionTicker;
        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        PostCppCallbackDelayed( (Handle32)this, *(Handle32*)&pHandlerFunc, params, 0, CACHE_TRANSACTION_TICKER_INTERVAL);
    }
}

//MSG_PAUSE_CACHE_TRANSACTION
void WebViewWorker::HandleMsgPauseCacheTransaction()
{
    if (CCacheManager::DisableTransaction()) {
        sCacheTickersBlocked = TRUE;
        void (STDCALL WebViewWorker::*pHandlerFunc)();
        pHandlerFunc = &WebViewWorker::HandleMsgCacheTransactionTicker;
        RemoveCppCallbacks( (Handle32)this, *(Handle32*)&pHandlerFunc);
    }
}

//MSG_RESUME_CACHE_TRANSACTION
void WebViewWorker::HandleMsgResumeCacheTransaction()
{
    if (CCacheManager::EnableTransaction()) {
        sCacheTickersBlocked = FALSE;

        void (STDCALL WebViewWorker::*pHandlerFunc)();
        pHandlerFunc = &WebViewWorker::HandleMsgCacheTransactionTicker;
        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        PostCppCallbackDelayed( (Handle32)this, *(Handle32*)&pHandlerFunc, params, 0, CACHE_TRANSACTION_TICKER_INTERVAL);
    }
}
