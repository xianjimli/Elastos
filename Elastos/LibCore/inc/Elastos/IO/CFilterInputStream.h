
#ifndef __CFILTERINPUTSTREAM_H__
#define __CFILTERINPUTSTREAM_H__

#include "_CFilterInputStream.h"
#include "FilterInputStream.h"

CarClass(CFilterInputStream), public FilterInputStream
{
public:
    CFilterInputStream();

    ~CFilterInputStream();

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
        /* [in] */ Int64 length,
        /* [out] */ Int64* value);

    CARAPI constructor(
        /* [in] */ IInputStream* is);
protected:
    CARAPI MarkLocked(
        /* [in] */ Int32 readLimit);

    CARAPI ResetLocked();
private:
    CARAPI_(Mutex*) GetSelfLock();
};

#endif // __CFILTERINPUTSTREAM_H__
