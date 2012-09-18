
#ifndef __CPUSHBACKINPUTSTREAM_H__
#define __CPUSHBACKINPUTSTREAM_H__

#include "_CPushbackInputStream.h"
#include "PushbackInputStream.h"

CarClass(CPushbackInputStream), public PushbackInputStream
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

    CARAPI Unread(
        /* [in] */ Int32 oneByte);

    CARAPI UnreadBuffer(
        /* [in] */ const ArrayOf<Byte>& buffer);

    CARAPI UnreadBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [in] */ const ArrayOf<Byte>& buffer);

    CARAPI constructor(
        /* [in] */ IInputStream* is);

    CARAPI constructor(
        /* [in] */ IInputStream* is,
        /* [in] */ Int32 size);

private:
    CARAPI_(Mutex*) GetSelfLock();
};

#endif // __CPUSHBACKINPUTSTREAM_H__
