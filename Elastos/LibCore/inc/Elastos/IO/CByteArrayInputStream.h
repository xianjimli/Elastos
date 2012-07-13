
#ifndef __CBYTEARRAYINPUTSTREAM_H__
#define __CBYTEARRAYINPUTSTREAM_H__

#include "_CByteArrayInputStream.h"
#include "ByteArrayInputStream.h"

CarClass(CByteArrayInputStream), public ByteArrayInputStream
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
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [out] */ Int32* number);

    CARAPI ReadBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [out] */ Int32* number);

    CARAPI Reset();

    CARAPI Skip(
        /* [in] */ Int64 count,
        /* [out] */ Int64* number);

    CARAPI constructor(
        /* [in] */ const ArrayOf<Byte>& buffer);

    CARAPI constructor(
        /* [in] */ const ArrayOf<Byte>& buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length);

private:
    CARAPI_(Mutex*) GetSelfLock();
};

#endif // __CBYTEARRAYINPUTSTREAM_H__
