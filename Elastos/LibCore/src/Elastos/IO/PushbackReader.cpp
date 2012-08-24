#include "cmdef.h"
#include "PushbackReader.h"
#include <elastos/Character.h>

using namespace Elastos::Core;

PushbackReader::PushbackReader()
    : mBuf(NULL)
{
}

PushbackReader::~PushbackReader()
{
    ArrayOf<Char8>::Free(mBuf);
}

ECode PushbackReader::Init(
    /* [in] */ IReader* in)
{
    FilterReader::Init(in);
    mBuf = ArrayOf<Char8>::Alloc(4);
    if (mBuf == NULL) return E_OUT_OF_MEMORY_ERROR;
    mPos = 4;

    return NOERROR;
}

ECode PushbackReader::Init(
    /* [in] */ IReader* in,
    /* [in] */ Int32 size)
{
    FilterReader::Init(in);
    if (size <= 0) {
//      throw new IllegalArgumentException("size <= 0");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mBuf = ArrayOf<Char8>::Alloc(size);
    mPos = size;

    return NOERROR;
}

ECode PushbackReader::Close()
{
    assert(mLock != NULL);
    Mutex::Autolock lock(mLock);

    return CloseLocked();
}

ECode PushbackReader::CloseLocked()
{
    assert(mLock != NULL);

    ArrayOf<Char8>::Free(mBuf);
    mBuf = NULL;
    return mIn->Close();
}

ECode PushbackReader::Mark(
    /* [in] */ Int32 readLimit)
{
    return E_IO_EXCEPTION;
}

ECode PushbackReader::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    assert(supported != NULL);

    *supported = FALSE;

    return NOERROR;
}

ECode PushbackReader::Read(
    /* [out] */ Int32* value)
{
    assert(value != NULL);
    assert(mLock != NULL);
    Mutex::Autolock lock(mLock);

    return ReadLocked(value);
}

ECode PushbackReader::ReadLocked(
    /* [out] */ Int32* value)
{
    assert(value != NULL);
    assert(mLock != NULL);

    FAIL_RETURN(CheckNotClosed());
    /* Is there a pushback character available? */
    if (mPos < mBuf->GetLength()) {
        Int32 nextIndex;
        return Character::GetCharAt(*mBuf, mPos, (Char32*)value, &nextIndex);
    }
    /**
     * Assume read() in the InputStream will return 2 lowest-order bytes
     * or -1 if end of stream.
     */
    return mIn->Read(value);
}

ECode PushbackReader::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [out] */ ArrayOf<Char8>* buffer,
    /* [out] */ Int32* number)
{
    assert(buffer != NULL);
    assert(number != NULL);
    assert(mLock != NULL);
    Mutex::Autolock lock(mLock);

    return ReadBufferExLocked(offset, count, buffer, number);
}

ECode PushbackReader::ReadBufferExLocked(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [out] */ ArrayOf<Char8>* buffer,
    /* [out] */ Int32* number)
{
    assert(buffer != NULL);
    assert(number != NULL);
    assert(mLock != NULL);

    FAIL_RETURN(CheckNotClosed());
    // avoid int overflow
    // BEGIN android-changed
    // Exception priorities (in case of multiple errors) differ from
    // RI, but are spec-compliant.
    // made implicit null check explicit, used (offset | count) < 0
    // instead of (offset < 0) || (count < 0) to safe one operation
    if (buffer == NULL) {
//      throw new NullPointerException("buffer == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    if ((offset | count) < 0 || offset > buffer->GetLength() - count) {
//      throw new IndexOutOfBoundsException();
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    // END android-changed

    Int32 copiedChars = 0;
    Int32 copyLength = 0;
    Int32 newOffset = offset;
    /* Are there pushback chars available? */
    if (mPos < mBuf->GetLength()) {
        copyLength = (mBuf->GetLength() - mPos >= count) ? count : mBuf->GetLength()
                - mPos;
        Char8* p = (char*)buffer->GetPayload();
        Int32 char8Offset;
        //todo:
//        for(Int32 i=0; i<newOffset; i++){
//            Int32 oneChar32 = String(p).GetChar(i);
//            char8Offset += Character::CountUTFBytes((Char32)oneChar32);
//        }
//        p += char8Offset;
//        String str(mBuf->GetPayload());
//        for(Int32 i =0; i < copyLength; i++) {
//            //todo: calling str.GetChar(i) every time is inefficient;
//            Int32 oneChar32 = str.GetChar(mPos + i);
//            Int32 len = Character::CountUTFBytes((Char32)oneChar32);
//            Character::WriteUTFBytesToBuffer(p, (Char32)oneChar32, len);
//            p += len;
//        }
        newOffset += copyLength;
        copiedChars += copyLength;
        /* Use up the chars in the local buffer */
        mPos += copyLength;
    }
    /* Have we copied enough? */
    if (copyLength == count) {
        *number = count;
        return NOERROR;
    }
    Int32 inCopied;
    FAIL_RETURN(mIn->ReadBufferEx(newOffset, count - copiedChars, buffer, &inCopied));
    if (inCopied > 0) {
        *number = inCopied + copiedChars;
        return NOERROR;
    }
    if (copiedChars == 0) {
        *number = inCopied;
        return NOERROR;
    }
    *number = copiedChars;

    return copiedChars;
}

ECode PushbackReader::IsReady(
    /* [out] */ Boolean* ready)
{
    assert(ready != NULL);
    assert(mLock != NULL);
    Mutex::Autolock lock(mLock);

    return IsReadyLocked(ready);
}

ECode PushbackReader::IsReadyLocked(
    /* [out] */ Boolean* ready)
{
    assert(ready != NULL);
    assert(mLock != NULL);

    if (mBuf == NULL) {
//      throw new IOException("Reader is closed");
        return E_IO_EXCEPTION;
    }
    FAIL_RETURN(mIn->IsReady(ready));
    *ready = (mBuf->GetLength() - mPos > 0 || *ready);

    return NOERROR;
}

ECode PushbackReader::Reset()
{
    return E_IO_EXCEPTION;
}

ECode PushbackReader::UnRead(
   /* [in] */ Int32 oneChar)
{
    assert(mLock != NULL);
    Mutex::Autolock lock(mLock);

    return UnReadLocked(oneChar);
}

ECode PushbackReader::UnReadLocked(
   /* [in] */ Int32 oneChar)
{
    assert(mLock != NULL);

    FAIL_RETURN(CheckNotClosed());
    if (mPos == 0) {
//      throw new IOException("Pushback buffer full");
        return E_IO_EXCEPTION;
    }
    Char8* p = (Char8*)mBuf->GetPayload();
    p += --mPos;
//    Int32 len = Character::CountUTFBytes((Char32)oneChar);
//    Character::WriteUTFBytesToBuffer(p, (Char32)oneChar, len);

    return NOERROR;
}

ECode PushbackReader::UnReadBuffer(
    /* [in] */ const ArrayOf<Char8>& buffer)
{
    FAIL_RETURN(UnReadBufferEx(0, buffer.GetLength(), buffer));
    return NOERROR;
}

ECode PushbackReader::UnReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [in] */ const ArrayOf<Char8>& buffer)
{
    assert(mLock != NULL);
    Mutex::Autolock lock(mLock);

    return UnReadBufferExLocked(offset, length, buffer);
}

ECode PushbackReader::UnReadBufferExLocked(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [in] */ const ArrayOf<Char8>& buffer)
{
    assert(mLock != NULL);

    FAIL_RETURN(CheckNotClosed());
    if (length > mPos) {
//      throw new IOException("Pushback buffer full");
        return E_IO_EXCEPTION;
    }
    // Force buffer null check first!
    if (offset > buffer.GetLength() - length || offset < 0) {
//      throw new ArrayIndexOutOfBoundsException("Offset out of bounds: " + offset);
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (length < 0) {
//      throw new ArrayIndexOutOfBoundsException("Length out of bounds: " + length);
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    for (Int32 i = offset + length - 1; i >= offset; i--) {
        FAIL_RETURN(UnRead(String((char*)buffer.GetPayload()).GetChar(i)));
    }

    return NOERROR;
}

ECode PushbackReader::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    assert(number != NULL);

    if (count < 0) {
//      throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    FAIL_RETURN(CheckNotClosed());
    if (count == 0) {
        *number = 0;
        return NOERROR;
    }
    Int64 inSkipped;
    Int32 availableFromBuffer = mBuf->GetLength() - mPos;
    if (availableFromBuffer > 0) {
        Int64 requiredFromIr = count - availableFromBuffer;
        if (requiredFromIr <= 0) {
            mPos += count;
            *number = count;
            return NOERROR;
        }
        mPos += availableFromBuffer;
        FAIL_RETURN(mIn->Skip(requiredFromIr, &inSkipped));
    } else {
        FAIL_RETURN(mIn->Skip(count, &inSkipped));
    }
    *number = inSkipped + availableFromBuffer;

    return NOERROR;
}

ECode PushbackReader::CheckNotClosed()
{
    if(mBuf == NULL) return E_IO_EXCEPTION;
    return NOERROR;
}