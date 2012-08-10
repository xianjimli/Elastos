#include "cmdef.h"
#include "StringBufferInputStream.h"

StringBufferInputStream::StringBufferInputStream()
    : mCount(0)
    , mPos(0)
{}

StringBufferInputStream::~StringBufferInputStream()
{}

ECode StringBufferInputStream::Init(
    /* [in] */ const String& str)
{
    if (str.IsNull()) {
        return E_NULL_POINTER_EXCEPTION;
    }
    mBuffer = str;
    if (mBuffer.IsNull()) return E_OUT_OF_MEMORY_ERROR;
    mCount = mBuffer.GetCharCount();
    return NOERROR;
}

ECode StringBufferInputStream::Available(
    /* [out] */ Int32* number)
{
    assert(number != NULL);

    Mutex* selfLock = GetSelfLock();
    Mutex::Autolock lock(*selfLock);

    return AvailableLocked(number);
}

ECode StringBufferInputStream::AvailableLocked(
    /* [out] */ Int32* number)
{
    assert(number != NULL);

    *number = mCount - mPos;
    return NOERROR;
}

ECode StringBufferInputStream::Read(
    /* [out] */ Int32* value)
{
    assert(value != NULL);

    Mutex* selfLock = GetSelfLock();
    Mutex::Autolock lock(*selfLock);

    return ReadLocked(value);
}

ECode StringBufferInputStream::ReadLocked(
    /* [out] */ Int32* value)
{
    assert(value != NULL);

    *value = mPos < mCount ? mBuffer.GetChar(mPos++) & 0xFF : -1;
    return NOERROR;
}

ECode StringBufferInputStream::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    assert(buffer != NULL);
    assert(number != NULL);

    // BEGIN android-note
    // changed array notation to be consistent with the rest of harmony
    // END android-note
    // According to 22.7.6 should return -1 before checking other
    // parameters.
    Mutex* selfLock = GetSelfLock();
    Mutex::Autolock lock(*selfLock);

    return ReadBufferExLocked(offset, length, buffer, number);
}

ECode StringBufferInputStream::ReadBufferExLocked(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    assert(buffer != NULL);
    assert(number != NULL);

    if (mPos >= mCount) {
        *number = -1;
        return NOERROR;
    }
    if (buffer == NULL) {
//      throw new NullPointerException("buffer == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    // avoid int overflow
    if (offset < 0 || offset > buffer->GetLength()) {
//      throw new ArrayIndexOutOfBoundsException("Offset out of bounds: " + offset);
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (length < 0 || length > buffer->GetLength() - offset) {
//      throw new ArrayIndexOutOfBoundsException("Length out of bounds: " + length);
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    if (length == 0) {
        *number = 0;
        return NOERROR;
    }

    Int32 copylen = mCount - mPos < length ? mCount - mPos : length;
    for (Int32 i = 0; i < copylen; i++) {
        (*buffer)[offset + i] = (Byte)mBuffer.GetChar(mPos + i);
    }
    mPos += copylen;
    *number = copylen;
    return NOERROR;
}

ECode StringBufferInputStream::Reset()
{
    Mutex* selfLock = GetSelfLock();
    Mutex::Autolock lock(*selfLock);

    return ResetLocked();
}

ECode StringBufferInputStream::ResetLocked()
{
    mPos = 0;
    return NOERROR;
}

ECode StringBufferInputStream::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    assert(number != NULL);

    Mutex* selfLock = GetSelfLock();
    Mutex::Autolock lock(*selfLock);

    return SkipLocked(count, number);
}

ECode StringBufferInputStream::SkipLocked(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    assert(number != NULL);

    if (count <= 0) {
        *number = 0;
        return NOERROR;
    }

    Int32 numskipped;
    if (mCount - mPos < count) {
        numskipped = mCount - mPos;
        mPos = mCount;
    } else {
        numskipped = (Int32)count;
        mPos += count;
    }
    *number = numskipped;
    return NOERROR;
}