
#ifndef __CBUFFEREDINPUTSTREAM_H__
#define __CBUFFEREDINPUTSTREAM_H__

#include "_CBufferedInputStream.h"
#include "BufferedInputStream.h"

CarClass(CBufferedInputStream), public BufferedInputStream
{
public:
    CARAPI Available(
        /* [out] */ Int32* number);

    CARAPI Close();

    CARAPI Mark(
        /* [in] */ Int32 readLimit);

    CARAPI IsMarkSupported(
        /* [out] */ Boolean* supported);

    CARAPI Read(
        /* [out] */ Int32* value);

    CARAPI ReadBuffer(
        /* [out] */ BufferOf<Byte>* buffer,
        /* [out] */ Int32* value);

    CARAPI ReadBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ BufferOf<Byte>* buffer,
        /* [out] */ Int32* value);

    CARAPI Reset();

    CARAPI Skip(
        /* [in] */ Int64 length,
        /* [out] */ Int64* value);

    CARAPI constructor(
        /* [in] */ IInputStream* is);

    CARAPI constructor(
        /* [in] */ IInputStream* is,
        /* [in] */ Int32 size);

private:
    CARAPI_(Mutex*) GetSelfLock();
};

#endif // __CBUFFEREDINPUTSTREAM_H__
