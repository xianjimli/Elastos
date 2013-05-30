
#ifndef __CFIXEDLENGTHINPUTSTREAM_H__
#define __CFIXEDLENGTHINPUTSTREAM_H__

#include "_CFixedLengthInputStream.h"
#include "AbstractHttpInputStream.h"

using namespace Elastos::Core;

CarClass(CFixedLengthInputStream), public AbstractHttpInputStream
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
        /* [in] */ IHttpURLConnectionImpl * pHttpURLConnection,
        /* [in] */ Int32 length);

private:
    // TODO: Add your private member variables here.
    Int32 mBytesRemaining;
};

#endif // __CFIXEDLENGTHINPUTSTREAM_H__
