
#ifndef __CCHUNKEDINPUTSTREAM_H__
#define __CCHUNKEDINPUTSTREAM_H__

#include "_CChunkedInputStream.h"
#include "AbstractHttpInputStream.h"

using namespace Elastos::Core;

CarClass(CChunkedInputStream), public AbstractHttpInputStream
{
public:
    CChunkedInputStream();

public:
    CARAPI Available(
        /* [out] */ Int32 * pNumber);

    CARAPI Close();

    CARAPI Mark(
        /* [in] */ Int32 readLimit);

    CARAPI IsMarkSupported(
        /* [out] */ Boolean * pSupported);

    CARAPI Read(
        /* [out] */ Int32 * pValue);

    CARAPI ReadBuffer(
        /* [out] */ ArrayOf<Byte> * pBuffer,
        /* [out] */ Int32 * pNumber);

    CARAPI ReadBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ ArrayOf<Byte> * pBuffer,
        /* [out] */ Int32 * pNumber);

    CARAPI Reset();

    CARAPI Skip(
        /* [in] */ Int64 count,
        /* [out] */ Int64 * pNumber);

    CARAPI constructor(
        /* [in] */ IInputStream * pIs,
        /* [in] */ ICacheRequest * pCacheRequest,
        /* [in] */ IHttpURLConnectionImpl * pHttpURLConnection);
private:
    ECode ReadChunkSize();

private:
    // TODO: Add your private member variables here.
    static Int32 MIN_LAST_CHUNK_LENGTH;
    static Int32 NO_CHUNK_YET;
    Int32 mBytesRemainingInChunk;
    Boolean mHasMoreChunks;

};

#endif // __CCHUNKEDINPUTSTREAM_H__
