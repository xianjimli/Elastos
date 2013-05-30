
#ifndef __CCHUNKEDOUTPUTSTREAM_H__
#define __CCHUNKEDOUTPUTSTREAM_H__

#include "_CChunkedOutputStream.h"
#include "Elastos.IO.h"
#include "elastos/AutoPtr.h"
#include "AbstractHttpOutputStream.h"

using namespace Elastos::Core;

CarClass(CChunkedOutputStream), public AbstractHttpOutputStream
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
    Int32 DataLength(
        /* [in] */ Int32 dataPlusHeaderLength);

    ECode WriteHex(
        /* [in] */ Int32 i);

    ECode WriteBufferedChunkToSocket();
    // TODO: Add your private member variables here.

private:
    static ArrayOf<Byte>* CRLF;
    static ArrayOf<Byte>* HEX_DIGITS;
    static ArrayOf<Byte>* FINAL_CHUNK;

    AutoPtr<IOutputStream> mSocketOut;
    Int32 mMaxChunkLength;
    AutoPtr<IByteArrayOutputStream> mBufferedChunk;
    ArrayOf<Byte>* mHex;
};

#endif // __CCHUNKEDOUTPUTSTREAM_H__
