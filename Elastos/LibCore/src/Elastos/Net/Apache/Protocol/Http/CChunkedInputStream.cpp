
#include "CChunkedInputStream.h"
#include <elastos/Math.h>

Int32 CChunkedInputStream::MIN_LAST_CHUNK_LENGTH = String("\r\n0\r\n\r\n").GetLength();
Int32 CChunkedInputStream::NO_CHUNK_YET = -1;

CChunkedInputStream::CChunkedInputStream()
    :mBytesRemainingInChunk(NO_CHUNK_YET), mHasMoreChunks(TRUE)
{
}

ECode CChunkedInputStream::Available(
    /* [out] */ Int32 * pNumber)
{
    // TODO: Add your code here
    CheckNotClosed();
    if (mHasMoreChunks) {
        Int32 number;
        mIn->Available(&number);
        *pNumber = Math::Min(number, mBytesRemainingInChunk);
    } else {
        *pNumber = 0;
    }
    return NOERROR;
}

ECode CChunkedInputStream::Close()
{
    // TODO: Add your code here
    if (mClosed) {
        return NOERROR;
    }

    mClosed = TRUE;
    if (mHasMoreChunks) {
        UnexpectedEndOfInput();
    };
    return NOERROR;
}

ECode CChunkedInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CChunkedInputStream::IsMarkSupported(
    /* [out] */ Boolean * pSupported)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CChunkedInputStream::Read(
    /* [out] */ Int32 * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CChunkedInputStream::ReadBuffer(
    /* [out] */ ArrayOf<Byte> * pBuffer,
    /* [out] */ Int32 * pNumber)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CChunkedInputStream::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ ArrayOf<Byte> * pBuffer,
    /* [out] */ Int32 * pNumber)
{
    // TODO: Add your code here
    CheckBounds(*pBuffer, offset, length);
    CheckNotClosed();

    if (!mHasMoreChunks) {
        *pNumber = -1;
        return NOERROR;
    }
    if (mBytesRemainingInChunk == 0 || mBytesRemainingInChunk == NO_CHUNK_YET) {
        ReadChunkSize();
        if (!mHasMoreChunks) {
            *pNumber = -1;
            return NOERROR;
        }
    }
    Int32 read;
    mIn->ReadBufferEx(offset, Math::Min(length, mBytesRemainingInChunk), pBuffer, &read);
    if (read == -1) {
        UnexpectedEndOfInput(); // the server didn't supply the promised chunk length
        return E_IO_EXCEPTION;
    }
    mBytesRemainingInChunk -= read;
    CacheWrite(*pBuffer, offset, read);

    /*
     * If we're at the end of a chunk and the next chunk size is readable,
     * read it! Reading the last chunk causes the underlying connection to
     * be recycled and we want to do that as early as possible. Otherwise
     * self-delimiting streams like gzip will never be recycled.
     * http://code.google.com/p/android/issues/detail?id=7059
     */
    Int32 available;
    mIn->Available(&available);
    if (mBytesRemainingInChunk == 0 && available >= MIN_LAST_CHUNK_LENGTH) {
        ReadChunkSize();
    }

    *pNumber = read;
    return NOERROR;
}

ECode CChunkedInputStream::Reset()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CChunkedInputStream::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64 * pNumber)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CChunkedInputStream::constructor(
    /* [in] */ IInputStream * pIs,
    /* [in] */ ICacheRequest * pCacheRequest,
    /* [in] */ IHttpURLConnectionImpl * pHttpURLConnection)
{
    // TODO: Add your code here
    AbstractHttpInputStream(pIs, pHttpURLConnection, pCacheRequest);

    return E_NOT_IMPLEMENTED;
}

ECode CChunkedInputStream::ReadChunkSize()
{
    if (mBytesRemainingInChunk != NO_CHUNK_YET) {
        //HttpURLConnectionImpl.readLine(in);
    }

    String chunkSizeString;// = HttpURLConnectionImpl.readLine(in);
    Int32 index = chunkSizeString.IndexOf(";");
    if (index != -1) {
        chunkSizeString = chunkSizeString.Substring(0, index);
    }

    mBytesRemainingInChunk = chunkSizeString.ToInt32();

    if (mBytesRemainingInChunk == 0) {
        mHasMoreChunks = FALSE;
       // httpURLConnection.readHeaders(); // actually trailers!
        EndOfInput(TRUE);
    }
}
