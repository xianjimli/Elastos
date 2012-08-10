#include "cmdef.h"
#include "StringReader.h"
#include <elastos/Character.h>

using namespace Elastos::Core;

StringReader::StringReader()
    :mStr(NULL)
    ,mMarkpos(-1)
{
}

StringReader::~StringReader()
{
}

ECode StringReader::Init(
    /* [in] */ String str)
{
    Reader::Init(GetSelfLock());
    if (str.IsNull()) {
        return E_NULL_POINTER_EXCEPTION;
    }
    mStr = str;
    if (mStr.IsNull()) return E_OUT_OF_MEMORY_ERROR;
    mCount = mStr.GetCharCount();
    return NOERROR;
}

ECode StringReader::Close()
{
    mStr = NULL;

    return NOERROR;
}

Boolean StringReader::IsClosed()
{
    return mStr.IsNull();
}

ECode StringReader::Mark(
    /* [in] */ Int32 readLimit)
{
    if (readLimit < 0) {
//      throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Mutex::Autolock lock(mLock);

    return MarkLocked(readLimit);
}

ECode StringReader::MarkLocked(
    /* [in] */ Int32 readLimit)
{
    if (readLimit < 0) {
//      throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    FAIL_RETURN(CheckNotClosed());
    mMarkpos = mPos;

    return NOERROR;
}

ECode StringReader::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    assert(supported != NULL);
    
    *supported = TRUE;

    return NOERROR;
}

ECode StringReader::Read(
    /* [out] */ Int32* value)
{
    assert(value != NULL);
    assert(mLock != NULL);

    Mutex::Autolock lock(mLock);

    return ReadLocked(value);
}

ECode StringReader::ReadLocked(
    /* [out] */ Int32* value)
{
    assert(value != NULL);
    assert(mLock != NULL);

    FAIL_RETURN(CheckNotClosed());
    if (mPos != mCount) {
        *value = mStr.GetChar(mPos++);
        return NOERROR;
    }
    *value = -1;

    return NOERROR;
}

ECode StringReader::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ ArrayOf<Char8>* buffer,
    /* [out] */ Int32* number)
{
    // BEGIN android-note
    // changed array notation to be consistent with the rest of harmony
    // END android-note
    assert(buffer != NULL);
    assert(number != NULL);
    assert(mLock != NULL);

    Mutex::Autolock lock(mLock);

    return ReadBufferExLocked(offset, length, buffer, number);
}

ECode StringReader::ReadBufferExLocked(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ ArrayOf<Char8>* buffer,
    /* [out] */ Int32* number)
{
    // BEGIN android-note
    // changed array notation to be consistent with the rest of harmony
    // END android-note
    assert(buffer != NULL);
    assert(number != NULL);
    assert(mLock != NULL);
    
    if (offset < 0 || offset > buffer->GetLength()) {
//      throw new ArrayIndexOutOfBoundsException("Offset out of bounds: " + offset);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (length < 0 || length > buffer->GetLength() - offset) {
//      throw new ArrayIndexOutOfBoundsException("Length out of bounds: " + len);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (length == 0) {
        *number = 0;
        return NOERROR;
    }
    if (mPos == mCount) {
        *number = -1;
        return NOERROR;
    }

    Int32 end = mPos + length > mCount ? mCount : mPos + length;
    Char32 oneChar32;
    Int32 bytes;

    for(Int32 i=mPos; i<end; i++){
        oneChar32 = mStr.GetChar(i);
        Character::ToChars(oneChar32, *buffer, offset, &bytes);
        offset += bytes;
    }
    
    Int32 read = end - mPos;
    mPos = end;
    *number = read;
    return NOERROR;
}

ECode StringReader::IsReady(
    /* [out] */ Boolean* ready)
{
    assert(ready != NULL);
    assert(mLock != NULL);

    Mutex::Autolock lock(mLock);

    return IsReadyLocked(ready);
}

ECode StringReader::IsReadyLocked(
    /* [out] */ Boolean* ready)
{
    assert(ready != NULL);
    assert(mLock != NULL);

    FAIL_RETURN(CheckNotClosed());
    *ready = TRUE;

    return NOERROR;
}

ECode StringReader::Reset()
{
    assert(mLock != NULL);

    Mutex::Autolock lock(mLock);

    return ResetLocked();
}

ECode StringReader::ResetLocked()
{
    assert(mLock != NULL);

    FAIL_RETURN(CheckNotClosed());
    mPos = mMarkpos != -1 ? mMarkpos : 0;

    return NOERROR;
}

ECode StringReader::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    assert(number != NULL);
    assert(mLock != NULL);

    Mutex::Autolock lock(mLock);

    return SkipLocked(count, number);
}

ECode StringReader::SkipLocked(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    assert(number != NULL);
    assert(mLock != NULL);

    FAIL_RETURN(CheckNotClosed());
    Int32 minSkip = -mPos;
    Int32 maxSkip = mCount - mPos;

    if (maxSkip == 0 || count > maxSkip) {
        count = maxSkip; // no rewinding if we're at the end
    } else if (count < minSkip) {
        count = minSkip;
    }

    mPos += count;
    *number = count;

    return NOERROR;
}

ECode StringReader::CheckNotClosed()
{
    if(IsClosed()) return E_IO_EXCEPTION;

    return NOERROR;
}