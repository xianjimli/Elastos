
#include "CFixedLengthInputStream.h"
#include <elastos/Math.h>

ECode CFixedLengthInputStream::Available(
    /* [out] */ Int32 * pNumber)
{
    // TODO: Add your code here
    CheckNotClosed();
    if (mBytesRemaining == 0) {
        *pNumber = 0;
    } else {
        Int32 avail;
        mIn->Available(&avail);
        *pNumber = Math::Min(avail, mBytesRemaining);
    }
    return NOERROR;
}

ECode CFixedLengthInputStream::Close()
{
    // TODO: Add your code here
    if (mClosed) {
        return NOERROR;
    }
    mClosed = TRUE;
    if (mBytesRemaining != 0) {
        UnexpectedEndOfInput();
    }
    return NOERROR;
}

ECode CFixedLengthInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFixedLengthInputStream::IsMarkSupported(
    /* [out] */ Boolean * pSupported)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFixedLengthInputStream::Read(
    /* [out] */ Int32 * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFixedLengthInputStream::ReadBuffer(
    /* [out] */ ArrayOf<Byte> * pBuffer,
    /* [out] */ Int32 * pNumber)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFixedLengthInputStream::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ ArrayOf<Byte> * pBuffer,
    /* [out] */ Int32 * pNumber)
{
    // TODO: Add your code here
    CheckBounds(*pBuffer, offset, length);
    CheckNotClosed();
    if (mBytesRemaining == 0) {
        return NOERROR;
    }

    Int32 read;
    mIn->ReadBufferEx(offset, Math::Min(length, mBytesRemaining), pBuffer, &read);
    if (read == -1) {
        UnexpectedEndOfInput(); // the server didn't supply the promised content length
        return E_IO_EXCEPTION;
    }
    mBytesRemaining -= read;
    CacheWrite(*pBuffer, offset, read);
    if (mBytesRemaining == 0) {
        EndOfInput(TRUE);
    }
    *pNumber = read;
    return NOERROR;
}

ECode CFixedLengthInputStream::Reset()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFixedLengthInputStream::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64 * pNumber)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFixedLengthInputStream::constructor(
    /* [in] */ IInputStream * pIs,
    /* [in] */ ICacheRequest * pCacheRequest,
    /* [in] */ IHttpURLConnectionImpl * pHttpURLConnection,
    /* [in] */ Int32 length)
{
    // TODO: Add your code here
    AbstractHttpInputStream(pIs, pHttpURLConnection, pCacheRequest);
    mBytesRemaining = length;
    if (mBytesRemaining == 0) {
        EndOfInput(TRUE);
    }
    return NOERROR;
}

