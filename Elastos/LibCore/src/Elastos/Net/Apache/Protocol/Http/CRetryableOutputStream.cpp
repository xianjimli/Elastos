
#include "CRetryableOutputStream.h"
ECode CRetryableOutputStream::Close()
{
    // TODO: Add your code here
    if (mClosed) {
        return NOERROR;
    }
    mClosed = TRUE;
    Int32 size;
    mContent->GetSize(&size);
    if (size < mLimit) {
        return E_IO_EXCEPTION;
    };
    return NOERROR;
}

ECode CRetryableOutputStream::Flush()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CRetryableOutputStream::Write(
    /* [in] */ Int32 oneByte)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CRetryableOutputStream::WriteBuffer(
    /* [in] */ const ArrayOf<Byte> & buffer)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CRetryableOutputStream::WriteBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ const ArrayOf<Byte> & buffer)
{
    // TODO: Add your code here
    CheckNotClosed();
    CheckBounds(buffer, offset, count);
    Int32 size;
    mContent->GetSize(&size);
    if (mLimit != -1 && size > (mLimit - count)) {
        return E_IO_EXCEPTION;
    }
    return mContent->WriteBufferEx(offset, count, buffer);
}

ECode CRetryableOutputStream::CheckError(
    /* [out] */ Boolean * pHasError)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CRetryableOutputStream::constructor()
{
    // TODO: Add your code here
    mLimit = -1;
    return CByteArrayOutputStream::New(mLimit, (IByteArrayOutputStream**)&mContent);
}

ECode CRetryableOutputStream::constructor(
    /* [in] */ Int32 limit)
{
    // TODO: Add your code here
    mLimit = limit;
    return CByteArrayOutputStream::New(limit, (IByteArrayOutputStream**)&mContent);
}

ECode CRetryableOutputStream::ContentLength(
    /* [out] */ Int32* pSize)
{
    Close();
    return mContent->GetSize(pSize);
}

ECode CRetryableOutputStream::WriteToSocket(
    /* [in] */ IOutputStream* pSocketOut)
{
    Close();
    mContent->WriteTo(pSocketOut);
    pSocketOut->Flush();
    return NOERROR;
}