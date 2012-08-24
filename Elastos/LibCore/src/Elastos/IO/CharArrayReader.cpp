#include "cmdef.h"
#include "CharArrayReader.h"
#include <elastos/Character.h>

using namespace Elastos::Core;

CharArrayReader::CharArrayReader()
    : mBuf(NULL)
    , mPos(0)
    , mMarkedPos(-1)
    , mCount(0)
{
}

CharArrayReader::~CharArrayReader()
{
}

ECode CharArrayReader::Init(
    /* [in] */ const ArrayOf<Char8>& buf)
{
    Reader::Init(GetSelfLock());
    mBuf = &buf;
    Character::GetCharCount(*mBuf, 0, mBuf->GetLength(), &mCount);
    return NOERROR;
}

ECode CharArrayReader::Init(
    /* [in] */ const ArrayOf<Char8>& buf,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
{
    /*
     * The spec of this constructor is broken. In defining the legal values
     * of offset and length, it doesn't consider buffer's length. And to be
     * compatible with the broken spec, we must also test whether
     * (offset + length) overflows.
     */
    Reader::Init(GetSelfLock());
    if (offset < 0 || offset > buf.GetLength() || length < 0 || offset + length < 0) {
//      throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Reader::Init(GetSelfLock());
    mBuf = &buf;
    mPos = offset;
    mMarkedPos = offset;

    /* This is according to spec */
    Int32 bufferLength = mBuf->GetLength();
    mCount = offset + length < bufferLength ? length : bufferLength;
    return NOERROR;
}

ECode CharArrayReader::Close()
{
    Mutex::Autolock lock(mLock);

    return CloseLocked();
}

ECode CharArrayReader::CloseLocked()
{
    if (IsOpen()) {
        mBuf = NULL;
    }
    return NOERROR;
}

Boolean CharArrayReader::IsOpen()
{
    return mBuf != NULL;
}

Boolean CharArrayReader::IsClosed()
{
    return mBuf == NULL;
}

ECode CharArrayReader::Mark(
    /* [in] */ Int32 readLimit)
{
    Mutex::Autolock lock(mLock);

    return MarkLocked(readLimit);
}

ECode CharArrayReader::MarkLocked(
    /* [in] */ Int32 readLimit)
{
    FAIL_RETURN(CheckNotClosed());
    mMarkedPos = mPos;
    return NOERROR;
}

ECode CharArrayReader::CheckNotClosed()
{
    if (IsClosed()) {
//      throw new IOException("CharArrayReader is closed");
        return E_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CharArrayReader::IsMarkSupported(
    /*[out]*/ Boolean* IsMarkSupported)
{
    assert(IsMarkSupported != NULL);

    *IsMarkSupported = TRUE;
    return NOERROR;
}

ECode CharArrayReader::Read(
    /*[out]*/ Int32* character)
{
    assert(character != NULL);
    Mutex::Autolock lock(mLock);

    return ReadLocked(character);
}

ECode CharArrayReader::ReadLocked(
    /*[out]*/ Int32* character)
{
    assert(character != NULL);

    FAIL_RETURN(CheckNotClosed());
    if (mPos == mCount) {
        *character = -1;
        return NOERROR;
    }
    Char32 c;
    FAIL_RETURN(Character::GetCharAt(*mBuf, mPos, &c, &mPos));
    *character = c;
    return NOERROR;
}

ECode CharArrayReader::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ ArrayOf<Char8>* buffer,
    /* [out] */ Int32* number)
{
    assert(number != NULL);
    assert(buffer != NULL);

    // BEGIN android-note
    // changed array notation to be consistent with the rest of harmony
    // END android-note
    if (offset < 0 || offset > buffer->GetLength()) {
//      throw new ArrayIndexOutOfBoundsException("Offset out of bounds: " + offset);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (length < 0 || length > buffer->GetLength() - offset) {
//      throw new ArrayIndexOutOfBoundsException("Length out of bounds: " + len);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    Mutex::Autolock lock(mLock);

    return ReadBufferExLocked(offset, length, buffer, number);
}

ECode CharArrayReader::ReadBufferExLocked(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ ArrayOf<Char8>* buffer,
    /* [out] */ Int32* number)
{
    assert(number != NULL);
    assert(buffer != NULL);

    // BEGIN android-note
    // changed array notation to be consistent with the rest of harmony
    // END android-note
    if (offset < 0 || offset > buffer->GetLength()) {
//      throw new ArrayIndexOutOfBoundsException("Offset out of bounds: " + offset);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (length < 0 || length > buffer->GetLength() - offset) {
//      throw new ArrayIndexOutOfBoundsException("Length out of bounds: " + len);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    FAIL_RETURN(CheckNotClosed());
    if (mPos < mCount) {
        Int32 index = offset;
        while (mPos <= mCount && index < offset + length) {
            Int32 bytes;
            Char32 oneChar32;
            FAIL_RETURN(Character::GetCharAt(*mBuf, mPos, &oneChar32, &mPos));
            Character::ToChars(oneChar32, *buffer, index, &bytes);
            index += bytes;
        }
        *number = index - offset;
        return NOERROR;
    }
    *number = -1;
    return NOERROR;
}

ECode CharArrayReader::IsReady(
    /*[out]*/ Boolean* isReady)
{
    assert(isReady != NULL);
    Mutex::Autolock lock(mLock);

    return IsReadyLocked(isReady);
}

ECode CharArrayReader::IsReadyLocked(
    /*[out]*/ Boolean* isReady)
{
    assert(isReady != NULL);

    FAIL_RETURN(CheckNotClosed());
    *isReady = mPos != mCount;
    return NOERROR;
}

ECode CharArrayReader::Reset()
{
    Mutex::Autolock lock(mLock);

    return ResetLocked();
}

ECode CharArrayReader::ResetLocked()
{
    FAIL_RETURN(CheckNotClosed());
    mPos = mMarkedPos != -1 ? mMarkedPos : 0;
    return NOERROR;
}

ECode CharArrayReader::Skip(
    /* [in] */Int64 n,
    /* [out] */ Int64* number)
{
    assert(number != NULL);
    Mutex::Autolock lock(mLock);

    return SkipLocked(n, number);
}

ECode CharArrayReader::SkipLocked(
    /* [in] */Int64 n,
    /* [out] */ Int64* number)
{
    assert(number != NULL);

    FAIL_RETURN(CheckNotClosed());
    if (n <= 0) {
        *number = 0;
        return NOERROR;
    }

    Int64 skipped = 0;
    while (mPos < mCount && skipped < n) {
        Char32 c;
        Character::GetCharAt(*mBuf, mPos, &c, &mPos);
        skipped++;
    }
    *number = skipped;
    return NOERROR;
}
