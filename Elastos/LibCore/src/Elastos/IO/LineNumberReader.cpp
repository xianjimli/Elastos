#include "cmdef.h"
#include "LineNumberReader.h"
#include <elastos/Character.h>

using namespace Elastos::Core;

LineNumberReader::LineNumberReader()
    : mMarkedLineNumber(-1)
{
}

LineNumberReader::~LineNumberReader()
{
}

ECode LineNumberReader::Init(
    /* [in] */ IReader* ir)
{
    FAIL_RETURN(BufferedReader::Init(ir));
    Reader::Init(GetSelfLock());

    return NOERROR;
}

ECode LineNumberReader::Init(
    /* [in] */ IReader* ir,
    /* [in] */ Int32 size)
{
    FAIL_RETURN(BufferedReader::Init(ir, size));
    Reader::Init(GetSelfLock());

    return NOERROR;
}

ECode LineNumberReader::GetLineNumber(
    /* [out] */ Int32* lineNumber)
{
    assert(lineNumber != NULL);
    assert(mLock != NULL);
    Mutex::Autolock lock(mLock);

    return GetLineNumberLocked(lineNumber);
}

ECode LineNumberReader::GetLineNumberLocked(
    /* [out] */ Int32* lineNumber)
{
    assert(lineNumber != NULL);
    assert(mLock != NULL);

    *lineNumber = mLineNumber;

    return NOERROR;
}

ECode LineNumberReader::Mark(
    /* [in] */ Int32 readLimit)
{
    assert(mLock != NULL);
    Mutex::Autolock lock(mLock);

    return MarkLocked(readLimit);
}

ECode LineNumberReader::MarkLocked(
    /* [in] */ Int32 readLimit)
{
    assert(mLock != NULL);

    FAIL_RETURN(BufferedReader::Mark(readLimit));
    mMarkedLineNumber = mLineNumber;
    mMarkedLastWasCR = mLastWasCR;

    return NOERROR;
}

ECode LineNumberReader::Read(
    /* [out] */ Int32* value)
{
    assert(value != NULL);
    assert(mLock != NULL);
    Mutex::Autolock lock(mLock);

    return ReadLocked(value);
}

ECode LineNumberReader::ReadLocked(
    /* [out] */ Int32* value)
{
    assert(value != NULL);
    assert(mLock != NULL);

    Int32 ch;
    FAIL_RETURN(BufferedReader::Read(&ch));
    if (ch == '\n' && mLastWasCR) {
        FAIL_RETURN(BufferedReader::Read(&ch));
    }
    mLastWasCR = FALSE;
    switch (ch) {
        case '\r':
            ch = '\n';
            mLastWasCR = TRUE;
            // fall through
        case '\n':
            mLineNumber++;
    }
    *value = ch;

    return NOERROR;
}

ECode LineNumberReader::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ ArrayOf<Char8>* buffer,
    /* [out] */ Int32* number)
{
    assert(buffer != NULL);
    assert(number != NULL);
    assert(mLock != NULL);
    Mutex::Autolock lock(mLock);

    return ReadBufferExLocked(offset, length, buffer, number);
}

ECode LineNumberReader::ReadBufferExLocked(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ ArrayOf<Char8>* buffer,
    /* [out] */ Int32* number)
{
    assert(buffer != NULL);
    assert(number != NULL);
    assert(mLock != NULL);

    Int32 read;
    FAIL_RETURN(BufferedReader::ReadBufferEx(offset, length, buffer, &read));
    if (read == -1) {
        *number = -1;
        return NOERROR;
    }
    Int32 index = offset;
    while (index - offset < read) {
        Char32 ch;
        Character::GetCharAt(*buffer, index, &ch, &index);

        if (ch == '\r') {
            mLineNumber++;
            mLastWasCR = TRUE;
        }
        else if (ch == '\n') {
            if (!mLastWasCR) {
                mLineNumber++;
            }
            mLastWasCR = FALSE;
        }
        else {
            mLastWasCR = FALSE;
        }
    }
    *number = read;

    return NOERROR;
}

ECode LineNumberReader::ReadLine(
    /* [out] */ String* result)
{
    assert(result != NULL);
    assert(mLock != NULL);
    Mutex::Autolock lock(mLock);

    return ReadLineLocked(result);
}

ECode LineNumberReader::ReadLineLocked(
    /* [out] */ String* result)
{
    assert(result != NULL);
    assert(mLock != NULL);

    if (mLastWasCR) {
        FAIL_RETURN(ChompNewline());
        mLastWasCR = FALSE;
    }
    String lstr;
    FAIL_RETURN(ReadLine(&lstr));
    if (!lstr.IsNull()) {
        mLineNumber++;
    }
    *result = lstr;

    return NOERROR;
}

ECode LineNumberReader::Reset()
{
    assert(mLock != NULL);
    Mutex::Autolock lock(mLock);

    return ResetLocked();
}

ECode LineNumberReader::ResetLocked()
{
    assert(mLock != NULL);

    FAIL_RETURN(BufferedReader::Reset());
    mLineNumber = mMarkedLineNumber;
    mLastWasCR = mMarkedLastWasCR;

    return NOERROR;
}

ECode LineNumberReader::SetLineNumber(
    /* [in] */ Int32 lineNumber)
{
    assert(mLock != NULL);
    Mutex::Autolock lock(mLock);

    return SetLineNumberLocked(lineNumber);
}

ECode LineNumberReader::SetLineNumberLocked(
    /* [in] */ Int32 lineNumber)
{
    assert(mLock != NULL);

    mLineNumber = lineNumber;

    return NOERROR;
}

ECode LineNumberReader::Skip(
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

ECode LineNumberReader::SkipLocked(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    assert(number != NULL);

    if (count < 0) {
//      throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    assert(mLock != NULL);

    for (Int32 i = 0; i < count; i++) {
        Int32 value;
        Read(&value);
        if (value == -1) {
            *number = i;
            return NOERROR;
        }
    }
    *number = count;

    return NOERROR;
}