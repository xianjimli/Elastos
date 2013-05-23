
#ifndef __CCHUNKEDINPUTSTREAM_H__
#define __CCHUNKEDINPUTSTREAM_H__

#include "_CChunkedInputStream.h"

CarClass(CChunkedInputStream)
{
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
    // TODO: Add your private member variables here.
};

#endif // __CCHUNKEDINPUTSTREAM_H__
