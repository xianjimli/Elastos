#include "cmdef.h"
#include "PushbackInputStream.h"
#include <elastos/Character.h>

using namespace Elastos::Core;

PushbackInputStream::PushbackInputStream()
    : mBuf(NULL)
{
}

PushbackInputStream::~PushbackInputStream()
{
    ArrayOf<Byte>::Free(mBuf);
}

ECode PushbackInputStream::Init(
        /* [in] */ IInputStream* in)
{
    FilterInputStream::Init(in);
    if(in != NULL) {
        mBuf = ArrayOf<Byte>::Alloc(1);
        if (mBuf == NULL) return E_OUT_OF_MEMORY_ERROR;
    }
    mPos = 1;

    return NOERROR;
}

ECode PushbackInputStream::Init(
        /* [in] */ IInputStream* in,
        /* [in] */ Int32 size)
{
    FilterInputStream::Init(in);
    if (size <= 0) {
//      throw new IllegalArgumentException("size <= 0");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if(in != NULL) mBuf = ArrayOf<Byte>::Alloc(size);
    mPos = size;

    return NOERROR;
}

ECode PushbackInputStream::Available(
        /* [out] */ Int32* number)
{
    assert(number != NULL);

    if (mBuf == NULL) {
//      throw new IOException();
        return E_IO_EXCEPTION;
    }

    Int32 al;
    FAIL_RETURN(mIn->Available(&al));
    *number = mBuf->GetLength() - mPos + al;

    return NOERROR;
}

ECode PushbackInputStream::Close()
{
    if (mIn != NULL) {
        FAIL_RETURN(mIn->Close());
        mIn = NULL;
        ArrayOf<Byte>::Free(mBuf);
        mBuf = NULL;
    }

    return NOERROR;
}

ECode PushbackInputStream::IsMarkSupported(
        /* [out] */ Boolean* supported)
{
    assert(supported != NULL);

    *supported = FALSE;

    return NOERROR;
}

ECode PushbackInputStream::Read(
        /* [out] */ Int32* value)
{
    assert(value != NULL);

    if (mBuf == NULL) {
//      throw new IOException();
        return E_IO_EXCEPTION;
    }
    // Is there a pushback byte available?
    if (mPos < mBuf->GetLength()) {
        *value = (*mBuf)[mPos++] & 0xFF;
        return NOERROR;
    }
    // Assume read() in the InputStream will return low-order byte or -1
    // if end of stream.
    return mIn->Read(value);
}

ECode PushbackInputStream::ReadBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [out] */ Int32* number)
{
    assert(buffer != NULL);
    assert(number != NULL);

    if (mBuf == NULL) {
//      throw streamClosed();
        return E_IO_EXCEPTION;
    }
    // Force buffer null check first!
    if (offset > buffer->GetLength() || offset < 0) {
//        throw new ArrayIndexOutOfBoundsException("Offset out of bounds: " + offset);
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (length < 0 || length > buffer->GetLength() - offset) {
//      throw new ArrayIndexOutOfBoundsException("Length out of bounds: " + length);
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    Int32 copiedBytes = 0, copyLength = 0, newOffset = offset;
    // Are there pushback bytes available?
    if (mPos < mBuf->GetLength()) {
        copyLength = (mBuf->GetLength() - mPos >= length) ? length : mBuf->GetLength()
                - mPos;
        for (Int32 i = 0; i < copyLength; i++) {
            (*buffer)[newOffset + i] = (*mBuf)[mPos + i];
        }
        newOffset += copyLength;
        copiedBytes += copyLength;
        // Use up the bytes in the local buffer
        mPos += copyLength;
    }
    // Have we copied enough?
    if (copyLength == length) {
        *number = length;
        return NOERROR;
    }
    Int32 inCopied;
    FAIL_RETURN(mIn->ReadBufferEx(newOffset, length - copiedBytes, buffer, &inCopied));
    if (inCopied > 0) {
        *number = inCopied + copiedBytes;
        return NOERROR;
    }
    if (copiedBytes == 0) {
        *number = inCopied;
        return NOERROR;
    }
    *number = copiedBytes;
    return NOERROR;
}

ECode PushbackInputStream::Skip(
        /* [in] */ Int64 count,
        /* [out] */ Int64* number)
{
    assert(number != NULL);

    if (mIn == NULL) {
//      throw streamClosed();
        return E_IO_EXCEPTION;
    }
    if (count <= 0) {
        *number = 0;
        return NOERROR;
    }
    Int32 numSkipped = 0;
    if (mPos < mBuf->GetLength()) {
        numSkipped += (count < mBuf->GetLength() - mPos) ? count : mBuf->GetLength() - mPos;
        mPos += numSkipped;
    }
    if (numSkipped < count) {
        Int64 num;
        FAIL_RETURN(mIn->Skip(count - numSkipped, &num));
        numSkipped += num;
    }
    *number = numSkipped;
    return NOERROR;
}

ECode PushbackInputStream::UnRead(
        /* [in] */ Int32 oneByte)
{
    if (mBuf == NULL) {
//      throw new IOException();
        return E_IO_EXCEPTION;
    }
    if (mPos == 0) {
//      throw new IOException("Pushback buffer full");
        return E_IO_EXCEPTION;
    }

    (*mBuf)[--mPos] = (Byte)oneByte;
    return NOERROR;
}

ECode PushbackInputStream::UnReadBuffer(
        /* [in] */ const ArrayOf<Byte>& buffer)
{
    return UnReadBufferEx(0, buffer.GetLength(), buffer);
}

ECode PushbackInputStream::UnReadBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [in] */ const ArrayOf<Byte>& buffer)
{
    if (length > mPos) {
//      throw new IOException("Pushback buffer full");
        return E_IO_EXCEPTION;
    }
    if (offset > buffer.GetLength() || offset < 0) {
//      throw new ArrayIndexOutOfBoundsException("Offset out of bounds: " + offset);
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (length < 0 || length > buffer.GetLength() - offset) {
//      throw new ArrayIndexOutOfBoundsException("Length out of bounds: " + length);
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (mBuf == NULL) {
//      throw streamClosed();
        return E_IO_EXCEPTION;
    }

    for (Int32 i = 0; i < length; i++) {
        (*mBuf)[mPos - length + i] = buffer[offset + i];
    }
    mPos = mPos - length;

    return NOERROR;
}

ECode PushbackInputStream::Mark(
        /* [in] */ Int32 readLimit)
{
    return NOERROR;
}

ECode PushbackInputStream::Reset()
{
    return E_IO_EXCEPTION;
}