
#include "CSocketInputStream.h"
ECode CSocketInputStream::Available(
    /* [out] */ Int32 * pNumber)
{
    // TODO: Add your code here
    return mSocket->Available(pNumber);
}

ECode CSocketInputStream::Close()
{
    // TODO: Add your code here
    return mSocket->Close();;
}

ECode CSocketInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSocketInputStream::IsMarkSupported(
    /* [out] */ Boolean * pSupported)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSocketInputStream::Read(
    /* [out] */ Int32 * pValue)
{
    // TODO: Add your code here
    ArrayOf<Byte> *buffer = ArrayOf<Byte>::Alloc(1);
    Int32 result;
    mSocket->Read(buffer, 0, 1, &result);
    if (result == -1) {
        *pValue = -1;
    } else {
        *pValue = (*buffer)[0] & 0xFF;
    }
    ArrayOf<Byte>::Free(buffer);
    return NOERROR;
}

ECode CSocketInputStream::ReadBuffer(
    /* [out] */ ArrayOf<Byte> * pBuffer,
    /* [out] */ Int32 * pNumber)
{
    // TODO: Add your code here
    return ReadBufferEx(0, pBuffer->GetLength(), pBuffer, pNumber);
}

ECode CSocketInputStream::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ ArrayOf<Byte> * pBuffer,
    /* [out] */ Int32 * pNumber)
{
    // TODO: Add your code here
    if (pBuffer == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (length == 0) {
        *pNumber = 0;
        return NOERROR;
    }

    if (0 > offset || offset >= pBuffer->GetLength()) {
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }
    if (0 > length || offset + length > pBuffer->GetLength()) {
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }

    return mSocket->Read(pBuffer, offset, length, pNumber);
}

ECode CSocketInputStream::Reset()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSocketInputStream::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64 * pNumber)
{
    // TODO: Add your code here
    if (count == 0) {
        *pNumber = 0;
    } else {

    }
    return NOERROR;
}

ECode CSocketInputStream::constructor(
    /* [in] */ ISocketImpl * pImpl)
{
    // TODO: Add your code here
    mSocket = (PlainSocketImpl*) pImpl;
}

