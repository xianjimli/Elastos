
#include "CFixedLengthOutputStream.h"
ECode CFixedLengthOutputStream::Close()
{
    // TODO: Add your code here
    if (mClosed) {
        return NOERROR;
    }
    mClosed = TRUE;
    if (mBytesRemaining > 0) {
        return E_IO_EXCEPTION;
    };
    return NOERROR;
}

ECode CFixedLengthOutputStream::Flush()
{
    // TODO: Add your code here
    if (mClosed) {
        return NOERROR; // don't throw; this stream might have been closed on the caller's behalf
    }
    return mSocketOut->Flush();
}

ECode CFixedLengthOutputStream::Write(
    /* [in] */ Int32 oneByte)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFixedLengthOutputStream::WriteBuffer(
    /* [in] */ const ArrayOf<Byte> & buffer)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFixedLengthOutputStream::WriteBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ const ArrayOf<Byte> & buffer)
{
    // TODO: Add your code here
    CheckNotClosed();
    CheckBounds(buffer, offset, count);

    if (count > mBytesRemaining) {
        return E_IO_EXCEPTION;
    }

    mSocketOut->WriteBufferEx(offset, count, buffer);
    mBytesRemaining -= count;;
    return NOERROR;
}

ECode CFixedLengthOutputStream::CheckError(
    /* [out] */ Boolean * pHasError)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFixedLengthOutputStream::constructor(
    /* [in] */ IOutputStream * pSocketOut,
    /* [in] */ Int32 bytesRemaining)
{
    // TODO: Add your code here
    mSocketOut = pSocketOut;
    mBytesRemaining = bytesRemaining;
    return NOERROR;
}

