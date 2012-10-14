
#include "graphics/InputStreamAdaptor.h"
#include <skia/core/SkStream.h>
#include <assert.h>

class InputStreamAdaptor : public SkStream
{
public:
    InputStreamAdaptor(
        /* [in] */ IInputStream* is,
        /* [in] */ ArrayOf<Byte>* ar)
        : mInputStream(is)
        , mByteArray(ar)
    {
        SkASSERT(ar);
        mCapacity = ar->GetLength();
        SkASSERT(mCapacity > 0);
        mBytesRead  = 0;
    }

    virtual bool rewind()
    {
        mInputStream->Reset();
        return true;
    }

    size_t doRead(
        /* [in] */ void* buffer,
        /* [in] */ size_t size)
    {
        ECode ec = NOERROR;
        size_t bytesRead = 0;
        // read the bytes
        do {
            size_t requested = size;
            if (requested > mCapacity) {
                requested = mCapacity;
            }
            Int32 number;
            ec = mInputStream->ReadBufferEx(0, requested, mByteArray, &number);
            if (FAILED(ec)) {
                return 0;
            }

            if (number < 0) { // n == 0 should not be possible, see InputStream read() specifications.
                break;  // eof
            }

            memcpy(buffer, mByteArray->GetPayload(), number);
            buffer = (void*)((char*)buffer + number);
            bytesRead += number;
            size -= number;
            mBytesRead += number;
        } while (size != 0);

        return bytesRead;
    }

    size_t doSkip(
        /* [in] */ size_t size)
    {
//	        JNIEnv* env = fEnv;
//
//	        jlong skipped = env->CallLongMethod(fJavaInputStream,
//	                                            gInputStream_skipMethodID, (jlong)size);
//	        if (env->ExceptionCheck()) {
//	            env->ExceptionDescribe();
//	            env->ExceptionClear();
//	            SkDebugf("------- skip threw an exception\n");
//	            return 0;
//	        }
//	        if (skipped < 0) {
//	            skipped = 0;
//	        }
//
//	        return (size_t)skipped;
        return 0;
    }

    size_t doSize()
    {
//	        JNIEnv* env = fEnv;
//	        jint avail = env->CallIntMethod(fJavaInputStream,
//	                                        gInputStream_availableMethodID);
//	        if (env->ExceptionCheck()) {
//	            env->ExceptionDescribe();
//	            env->ExceptionClear();
//	            SkDebugf("------- available threw an exception\n");
//	            avail = 0;
//	        }
//	        return avail;
        return 0;
    }

    virtual size_t read(
        /* [in] */ void* buffer,
        /* [in] */ size_t size)
    {
//	        JNIEnv* env = fEnv;
//	        if (NULL == buffer) {
//	            if (0 == size) {
//	                return this->doSize();
//	            } else {
//	                /*  InputStream.skip(n) can return <=0 but still not be at EOF
//	                    If we see that value, we need to call read(), which will
//	                    block if waiting for more data, or return -1 at EOF
//	                 */
//	                size_t amountSkipped = 0;
//	                do {
//	                    size_t amount = this->doSkip(size - amountSkipped);
//	                    if (0 == amount) {
//	                        char tmp;
//	                        amount = this->doRead(&tmp, 1);
//	                        if (0 == amount) {
//	                            // if read returned 0, we're at EOF
//	                            break;
//	                        }
//	                    }
//	                    amountSkipped += amount;
//	                } while (amountSkipped < size);
//	                return amountSkipped;
//	            }
//	        }
        return this->doRead(buffer, size);
    }

private:
//	    JNIEnv*     fEnv;
//	    jobject     fJavaInputStream;   // the caller owns this object
//	    jbyteArray  fJavaByteArray;     // the caller owns this object
    IInputStream*   mInputStream;   // the caller owns this object
    ArrayOf<Byte>*  mByteArray;     // the caller owns this object
    size_t          mCapacity;
    size_t          mBytesRead;
};

SkStream* CreateInputStreamAdaptor(
    /* [in] */ IInputStream* stream,
    /* [in] */ ArrayOf<Byte>* storage,
    /* [in] */ Int32 markSize)
{
//	    static bool gInited;
//
//	    if (!gInited) {
//	        gInputStream_Clazz = env->FindClass("java/io/InputStream");
//	        RETURN_NULL_IF_NULL(gInputStream_Clazz);
//	        gInputStream_Clazz = (jclass)env->NewGlobalRef(gInputStream_Clazz);
//
//	        gInputStream_resetMethodID      = env->GetMethodID(gInputStream_Clazz,
//	                                                           "reset", "()V");
//	        gInputStream_markMethodID       = env->GetMethodID(gInputStream_Clazz,
//	                                                           "mark", "(I)V");
//	        gInputStream_availableMethodID  = env->GetMethodID(gInputStream_Clazz,
//	                                                           "available", "()I");
//	        gInputStream_readMethodID       = env->GetMethodID(gInputStream_Clazz,
//	                                                           "read", "([BII)I");
//	        gInputStream_skipMethodID       = env->GetMethodID(gInputStream_Clazz,
//	                                                           "skip", "(J)J");
//
//	        RETURN_NULL_IF_NULL(gInputStream_resetMethodID);
//	        RETURN_NULL_IF_NULL(gInputStream_markMethodID);
//	        RETURN_NULL_IF_NULL(gInputStream_availableMethodID);
//	        RETURN_NULL_IF_NULL(gInputStream_availableMethodID);
//	        RETURN_NULL_IF_NULL(gInputStream_skipMethodID);
//
//	        gInited = true;
//	    }
//
//	    if (markSize) {
//	        env->CallVoidMethod(stream, gInputStream_markMethodID, markSize);
//	    }

    return new InputStreamAdaptor(stream, storage);
}
