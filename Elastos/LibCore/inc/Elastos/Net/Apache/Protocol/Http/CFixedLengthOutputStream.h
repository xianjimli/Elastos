
#ifndef __CFIXEDLENGTHOUTPUTSTREAM_H__
#define __CFIXEDLENGTHOUTPUTSTREAM_H__

#include "_CFixedLengthOutputStream.h"

CarClass(CFixedLengthOutputStream)
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
        /* [out] */ Boolean * pHasError);

    CARAPI constructor(
        /* [in] */ IOutputStream * pSocketOut,
        /* [in] */ Int32 bytesRemaining);

private:
    // TODO: Add your private member variables here.
};

#endif // __CFIXEDLENGTHOUTPUTSTREAM_H__
