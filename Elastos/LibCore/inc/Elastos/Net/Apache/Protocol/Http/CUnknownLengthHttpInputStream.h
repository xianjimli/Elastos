
#ifndef __CUNKNOWNLENGTHHTTPINPUTSTREAM_H__
#define __CUNKNOWNLENGTHHTTPINPUTSTREAM_H__

#include "_CUnknownLengthHttpInputStream.h"
#include "AbstractHttpInputStream.h"

CarClass(CUnknownLengthHttpInputStream), public AbstractHttpInputStream
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
    Boolean mInputExhausted;
};

#endif // __CUNKNOWNLENGTHHTTPINPUTSTREAM_H__
