
#ifndef __ZIP_H__
#define __ZIP_H__

#include "zlib.h"


class NativeZipStream
{
public:
    NativeZipStream()
        : mInput(NULL)
        , mInCap(0)
        , mDict(NULL)
    {
        // Let zlib use its default allocator.
        mStream.opaque = Z_NULL;
        mStream.zalloc = Z_NULL;
        mStream.zfree = Z_NULL;
    }

    ~NativeZipStream()
    {
        ArrayOf<Byte>::Free(mInput);
        ArrayOf<Byte>::Free(mDict);
    }

    CARAPI_(void) SetDictionary(
        /* [in] */ const ArrayOf<Byte>& dictionary,
        /* [in] */ Int32 off,
        /* [in] */ Int32 len,
        /* [in] */ Boolean inflate)
    {
        ArrayOf<Byte>* dictionaryBytes = ArrayOf<Byte>::Alloc(len);
        if (dictionaryBytes == NULL) {
//            jniThrowOutOfMemoryError(env, NULL);
            return;
        }
        memcpy(dictionaryBytes->GetPayload(), dictionary.GetPayload() + off, len);
        const Bytef* zDictionary = reinterpret_cast<const Bytef*>(dictionaryBytes->GetPayload());
        Int32 err;
        if (inflate) {
            err = inflateSetDictionary(&mStream, zDictionary, len);
        }
        else {
            err = deflateSetDictionary(&mStream, zDictionary, len);
        }
        if (err != Z_OK) {
//            throwExceptionForZlibError(env, "java/lang/IllegalArgumentException", err);
            ArrayOf<Byte>::Free(dictionaryBytes);
            return;
        }
        mDict = dictionaryBytes;
    }

    CARAPI_(void) SetInput(
        /* [in] */ ArrayOf<Byte>* buf,
        /* [in] */ Int32 off,
        /* [in] */ Int32 len)
    {
        ArrayOf<Byte>::Free(mInput);
        mInput = ArrayOf<Byte>::Alloc(len);
        if (mInput == NULL) {
            mInCap = 0;
//            jniThrowOutOfMemoryError(env, NULL);
            return;
        }
        mInCap = len;
        if (buf != NULL) {
            memcpy(mInput->GetPayload(), buf->GetPayload() + off, len);
        }
        mStream.next_in = reinterpret_cast<Bytef*>(mInput->GetPayload());
        mStream.avail_in = len;
    }

private:
    // Disallow copy and assignment.
    NativeZipStream(const NativeZipStream&);
    void operator=(const NativeZipStream&);

public:
    ArrayOf<Byte>* mInput;
    Int32 mInCap;
    z_stream mStream;

private:
    ArrayOf<Byte>* mDict;
};

#endif  //__ZIP_H__
