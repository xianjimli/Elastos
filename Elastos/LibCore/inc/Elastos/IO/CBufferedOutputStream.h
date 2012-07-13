
#ifndef __CBUFFEREDOUTPUTSTREAM_H__
#define __CBUFFEREDOUTPUTSTREAM_H__

#include "_CBufferedOutputStream.h"
#include "BufferedOutputStream.h"

CarClass(CBufferedOutputStream), public BufferedOutputStream
{
public:
    CARAPI Close();

    CARAPI Flush();

    CARAPI Write(
        /* [in] */ Int32 oneByte);

    CARAPI WriteBuffer(
        /* [in] */ const ArrayOf<Byte> & buffer);

    CARAPI WriteBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [in] */ const ArrayOf<Byte> & buffer);

    CARAPI CheckError(
        /* [out] */ Boolean* hasError);

    CARAPI constructor(
        /* [in] */ IOutputStream* out);

    CARAPI constructor(
        /* [in] */ IOutputStream* out,
        /* [in] */ Int32 size);

private:
    CARAPI_(Mutex*) GetSelfLock();
};

#endif // __CBUFFEREDOUTPUTSTREAM_H__
