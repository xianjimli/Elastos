
#include "content/StringBlock.h"
#include <elastos/Mutex.h>
#include <string.h>

const char* StringBlock::TAG = "CAssetManager";
const Boolean StringBlock::sLocalLOGV;

StringBlock::StringBlock(
    /* [in] */ const android::ResStringPool* pool,
    /* [in] */ Boolean useSparse)
    : mUseSparse(FALSE)
    , mStrings(NULL)
{
    mNative = pool;
    mUseSparse = FALSE; // useSparse;
    mOwnsNative = FALSE;
//	    if (localLOGV) Log.v(TAG, "Created string block " + this
//	            + ": " + nativeGetSize(mNative));
}

StringBlock::~StringBlock()
{
    if (mStrings) {
        delete mStrings;
    }
}

ECode StringBlock::Get(
    /* [in] */ Int32 idx,
    /* [out] */ ICharSequence** text)
{
    assert(text);

    Mutex::Autolock Mutex(mSyncLock);

    if (mStrings != NULL) {
        AutoPtr<ICharSequence> res = (*mStrings)[idx];
        if (res != NULL) {
            *text = res.Get();
            (*text)->AddRef();
            return NOERROR;
        }
//	    } else if (mSparseStrings != null) {
//	        CharSequence res = mSparseStrings.get(idx);
//	        if (res != null) {
//	            return res;
//	        }
    } else {
        Int32 num = NativeGetSize(mNative);
        if (mUseSparse && num > 250) {
//	            mSparseStrings = new SparseArray<CharSequence>();
        } else {
            mStrings = new Vector<AutoPtr<ICharSequence> >(num);
            if (!mStrings) return E_OUT_OF_MEMORY;
        }
    }

    String str;
    NativeGetString(mNative, idx, &str);
    if (str.IsNull()) {
        *text = NULL;
        return E_OUT_OF_MEMORY;
    }

    AutoPtr<ICharSequence> res;
    CStringWrapper::New(str, (ICharSequence**)&res);
//	    ArrayOf<Int32>* style = NativeGetStyle(mNative, idx);
//	    if (localLOGV) Log.v(TAG, "Got string: " + str);
//	    if (localLOGV) Log.v(TAG, "Got styles: " + style);
//	    if (style != NULL) {
//	        if (mStyleIDs == null) {
//	            mStyleIDs = new StyleIDs();
//	            mStyleIDs.boldId = nativeIndexOfString(mNative, "b");
//	            mStyleIDs.italicId = nativeIndexOfString(mNative, "i");
//	            mStyleIDs.underlineId = nativeIndexOfString(mNative, "u");
//	            mStyleIDs.ttId = nativeIndexOfString(mNative, "tt");
//	            mStyleIDs.bigId = nativeIndexOfString(mNative, "big");
//	            mStyleIDs.smallId = nativeIndexOfString(mNative, "small");
//	            mStyleIDs.supId = nativeIndexOfString(mNative, "sup");
//	            mStyleIDs.subId = nativeIndexOfString(mNative, "sub");
//	            mStyleIDs.strikeId = nativeIndexOfString(mNative, "strike");
//	            mStyleIDs.listItemId = nativeIndexOfString(mNative, "li");
//	            mStyleIDs.marqueeId = nativeIndexOfString(mNative, "marquee");

//	            if (localLOGV) Log.v(TAG, "BoldId=" + mStyleIDs.boldId
//	                    + ", ItalicId=" + mStyleIDs.italicId
//	                    + ", UnderlineId=" + mStyleIDs.underlineId);
//	        }

//	        res = applyStyles(str, style, mStyleIDs);
//	    }
    if (mStrings != NULL) (*mStrings)[idx] = res;
//	    else mSparseStrings.put(idx, res);
    *text = res.Get();
    (*text)->AddRef();
    return NOERROR;
}

Int32 StringBlock::NativeCreate(
    /* [in] */ const ArrayOf<Byte>& data,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 size)
{
    return -1;
}

Int32 StringBlock::NativeGetSize(
    /* [in] */ const android::ResStringPool* pool)
{
    assert(pool);

    return pool->size();
}

ECode StringBlock::NativeGetString(
    /* [in] */ const android::ResStringPool* pool,
    /* [in] */ Int32 idx,
    /* [out] */ String* str)
{
    assert(pool);
    assert(str);

    size_t len;
    const char* str8 = pool->string8At(idx, &len);
    if (str8 != NULL) {
        *str = str8;
        return NOERROR;
    }

    const char16_t* str16 = pool->stringAt(idx, &len);
    if (str16 == NULL) {
//	        doThrow(env, "java/lang/IndexOutOfBoundsException");
        *str = NULL;
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    android::String8 s8(str16);
    *str = s8.string();
    return NOERROR;
}

const ArrayOf<Int32>* StringBlock::NativeGetStyle(
    /* [in] */ const android::ResStringPool* pool,
    /* [in] */ Int32 idx)
{
    return NULL;
}

Int32 StringBlock::NativeIndexOfString(
    /* [in] */ const android::ResStringPool* pool,
    /* [in] */ const String& str)
{
    return -1;
}

void StringBlock::NativeDestroy(
    /* [in] */ const android::ResStringPool* pool)
{
}
