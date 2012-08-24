#include "cmdef.h"
#include "Reader.h"
#include <elastos/Character.h>

using namespace Elastos::Core;

Reader::Reader()
    : mLock(NULL)
{}

Reader::Reader(
    /* [in] */ Mutex* lock)
{
    assert(lock != NULL);
    mLock = lock;
}

Reader::~Reader()
{}

ECode Reader::Mark(
    /* [in] */ Int32 readLimit)
{
//    throw new IOException();
    return E_IO_EXCEPTION;
}

ECode Reader::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    assert(supported != NULL);

    *supported = FALSE;
    return NOERROR;
}

ECode Reader::Read(
    /* [out] */ Int32* value)
{
    assert(value != NULL);

    assert(mLock != NULL);
    Mutex::Autolock lock(mLock);

    return ReadLocked(value);
}

ECode Reader::ReadLocked(
    /* [out] */ Int32* value)
{
    assert(value != NULL);

    assert(mLock != NULL);

    ArrayOf_<Char8, 4> buf;
    Int32 number;
    FAIL_RETURN(ReadBufferEx(0, 1, &buf, &number));
    if (number != -1) {
        Char32 c;
        Character::GetCharAt(buf, 0, &c);
        *value = c;
    }
    else *value = -1;
    return NOERROR;
}

ECode Reader::ReadBuffer(
        /* [out] */ ArrayOf<Char8>* buffer,
        /* [out] */ Int32* number)
{
    assert(buffer != NULL);
    assert(number != NULL);

    // BEGIN android-note
    // changed array notation to be consistent with the rest of harmony
    // END android-note
    return ReadBufferEx(0, buffer->GetLength(), buffer, number);
}

ECode Reader::IsReady(
    /* [out] */ Boolean* ready)
{
    assert(ready != NULL);

    *ready = FALSE;
    return NOERROR;
}

ECode Reader::Reset()
{
    return E_IO_EXCEPTION;
}

ECode Reader::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    assert(number != NULL);

    if (count < 0) {
//      throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    assert(mLock != NULL);
    Mutex::Autolock lock(mLock);

    return SkipLocked(count, number);
}

ECode Reader::SkipLocked(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    assert(number != NULL);

    if (count < 0) {
//      throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    assert(mLock != NULL);

    Int64 skipped = 0;
    Int32 toRead = count < 128 ? (Int32)count : 128;
    ArrayOf_<Char8, 128 * 4> charsSkipped;
    while (skipped < count) {
        Int32 read;
        FAIL_RETURN(ReadBufferEx(0, toRead, &charsSkipped, &read));

        if (read == -1) {
            *number = skipped;
            return NOERROR;
        }
        skipped += read;
        if (read < toRead) {
            *number = skipped;
            return NOERROR;
        }
        if (count - skipped < toRead) {
            toRead = (Int32)(count - skipped);
        }
    }
    *number = skipped;
    return NOERROR;
}

ECode Reader::Init(
    /* [in] */ Mutex* lock)
{
    assert(lock != NULL);
    mLock = lock;
    return NOERROR;
}
