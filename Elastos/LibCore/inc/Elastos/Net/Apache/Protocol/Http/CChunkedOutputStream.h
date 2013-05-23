
#ifndef __CCHUNKEDOUTPUTSTREAM_H__
#define __CCHUNKEDOUTPUTSTREAM_H__

#include "_CChunkedOutputStream.h"

CarClass(CChunkedOutputStream)
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
        /* [in] */ Int32 maxChunkLength);

private:
    // TODO: Add your private member variables here.
};

#endif // __CCHUNKEDOUTPUTSTREAM_H__
