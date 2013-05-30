
#include "CUnknownLengthHttpInputStream.h"
ECode CUnknownLengthHttpInputStream::Available(
    /* [out] */ Int32 * pNumber)
{
    // TODO: Add your code here
    CheckNotClosed();
    if (mIn == NULL) {
        *pNumber = 0;
    } else {
        return mIn->Available(pNumber);
    }
    return NOERROR;
}

ECode CUnknownLengthHttpInputStream::Close()
{
    // TODO: Add your code here
    if (mClosed) {
        return NOERROR;
    }
    mClosed = TRUE;
    if (!mInputExhausted) {
        UnexpectedEndOfInput();
    };
    return NOERROR;
}

ECode CUnknownLengthHttpInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CUnknownLengthHttpInputStream::IsMarkSupported(
    /* [out] */ Boolean * pSupported)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CUnknownLengthHttpInputStream::Read(
    /* [out] */ Int32 * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CUnknownLengthHttpInputStream::ReadBuffer(
    /* [out] */ ArrayOf<Byte> * pBuffer,
    /* [out] */ Int32 * pNumber)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CUnknownLengthHttpInputStream::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ ArrayOf<Byte> * pBuffer,
    /* [out] */ Int32 * pNumber)
{
    // TODO: Add your code here
    CheckBounds(*pBuffer, offset, length);
    CheckNotClosed();

    if (mIn == NULL || mInputExhausted) {
        return E_IO_EXCEPTION;
    }

    Int32 read;

    mIn->ReadBufferEx(offset, length, pBuffer, &read);
    if (read == -1) {
        mInputExhausted = TRUE;
        EndOfInput(FALSE);
        return E_IO_EXCEPTION;
    }

    CacheWrite(*pBuffer, offset, read);
    *pNumber = read;
    return NOERROR;
}

ECode CUnknownLengthHttpInputStream::Reset()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CUnknownLengthHttpInputStream::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64 * pNumber)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CUnknownLengthHttpInputStream::constructor(
    /* [in] */ IInputStream * pIs,
    /* [in] */ ICacheRequest * pCacheRequest,
    /* [in] */ IHttpURLConnectionImpl * pHttpURLConnection)
{
    // TODO: Add your code here
    AbstractHttpInputStream(pIs, pHttpURLConnection, pCacheRequest);
    return NOERROR;
}

