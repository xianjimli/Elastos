
#include "cmdef.h"
#include "BufferedReader.h"
#include <elastos/Character.h>

using namespace Elastos::Core;

BufferedReader::BufferedReader()
    : mBuf(NULL)
    , mPos(0)
    , mEnd(0)
    , mMark(-1)
    , mMarkLimit(-1)
{
}

BufferedReader::~BufferedReader()
{
    if(mBuf != NULL) {
        ArrayOf<Char8>::Free(mBuf);
    }
}

ECode BufferedReader::Init(
    /*[in]*/ IReader* rin)
{
    return Init(rin, 8192);
}

ECode BufferedReader::Init(
    /*[in]*/ IReader* rin,
    /*[in]*/ Int32 size)
{
    //super(in);
    Reader::Init(GetSelfLock());
    if (size <= 0) {
//      throw new IllegalArgumentException("size <= 0");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mIn = rin;
    mBuf = ArrayOf<Char8>::Alloc(size);
    if (mBuf == NULL) return E_OUT_OF_MEMORY_ERROR;

    return NOERROR;
}

ECode BufferedReader::Close()
{
    Mutex::Autolock lock(mLock);

    return CloseLocked();
}

ECode BufferedReader::CloseLocked()
{
    if (!IsClosed()) {
        FAIL_RETURN(mIn->Close());
        ArrayOf<Char8>::Free(mBuf);
        mBuf = NULL;
    }

    return NOERROR;
}

ECode BufferedReader::FillBuf(
    /*[out]*/ Int32* number)
{
    assert(number != NULL);

    // assert(pos == end);

    Int32 length = mBuf->GetLength();
    if (mMark == -1 || (mPos - mMark >= mMarkLimit)) {
        /* mark isn't set or has exceeded its limit. use the whole buffer */
        Int32 result;
        FAIL_RETURN(mIn->ReadBufferEx(0, length, mBuf, &result));
        if (result > 0) {
            mMark = -1;
            mPos = 0;
            mEnd = result;
        }
        *number = result;
        return NOERROR;
    }

    if (mMark == 0 && mMarkLimit > length) {
        /* the only way to make room when mark=0 is by growing the buffer */
        Int32 newLength = length * 2;
        if (newLength > mMarkLimit) {
            newLength = mMarkLimit;
        }
        ArrayOf<Char8>* newbuf = ArrayOf<Char8>::Alloc(newLength);
        for(Int32 i = 0; i < length; i++) {
            (*newbuf)[i] = (*mBuf)[i];
        }
        ArrayOf<Char8>::Free(mBuf);
        mBuf = newbuf;
    }
    else if (mMark > 0) {
        /* make room by shifting the buffered data to left mark positions */
        // System.arraycopy(buf, mark, buf, 0, buf.length - mark);
        //todo: mMark is in character, not in Char8, so the following implementation
        //      is not correct.
        //      We need a powerful String type.
        for(Int32 i = 0; i < length - mMark; i++) {
            (*mBuf)[i] = (*mBuf)[mMark + i];
        }
        mPos -= mMark;
        mEnd -= mMark;
        mMark = 0;
    }

    /* Set the new position and mark position */
    Int32 count;
    FAIL_RETURN(mIn->ReadBufferEx(mPos, mBuf->GetLength() - mPos, mBuf, &count));
    if (count != -1) {
        mEnd += count;
    }
    *number = count;
    return NOERROR;
}

Boolean BufferedReader::IsClosed()
{
    return mBuf == NULL;
}

ECode BufferedReader::Mark(
    /*[in]*/ Int32 markLimit)
{
    if (markLimit < 0) {
//      throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Mutex::Autolock lock(mLock);

    return MarkLocked(markLimit);
}

ECode BufferedReader::MarkLocked(
    /*[in]*/ Int32 markLimit)
{
    if (markLimit < 0) {
//      throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    FAIL_RETURN(CheckNotClosed());
    mMarkLimit = markLimit;
    mMark = mPos;

    return NOERROR;
}

ECode BufferedReader::CheckNotClosed()
{
    if (IsClosed()) {
//      throw new IOException("BufferedReader is closed");
        return E_IO_EXCEPTION;
    }

    return NOERROR;
}

ECode BufferedReader::IsMarkSupported(
    /*[out]*/ Boolean* supported)
{
    assert(supported != NULL);

    *supported = TRUE;
    return NOERROR;
}

ECode BufferedReader::Read(
    /*[out]*/ Int32* value)
{
    assert(value != NULL);
    Mutex::Autolock lock(mLock);

    return ReadLocked(value);
}

ECode BufferedReader::ReadLocked(
    /*[out]*/ Int32* value)
{
    assert(value != NULL);

    FAIL_RETURN(CheckNotClosed());
    /* Are there buffered characters available? */
    Int32 number;
    FAIL_RETURN(FillBuf(&number));
    if (mPos < mEnd || number != -1) {
        Char32 c;
        FAIL_RETURN(Character::GetCharAt(*mBuf, mPos, &c, &mPos));
        *value = c;
        return NOERROR;
    }
    *value = -1;
    return NOERROR;
}

ECode BufferedReader::ReadBufferEx(
    /*[in]*/ Int32 offset,
    /*[in]*/ Int32 length,
    /*[out]*/ ArrayOf<Char8>* buffer,
    /*[out]*/ Int32* number)
{
    assert(buffer != NULL);
    assert(number != NULL);

    Mutex::Autolock lock(mLock);

    return ReadBufferExLocked(offset, length, buffer, number);
}

ECode BufferedReader::ReadBufferExLocked(
    /*[in]*/ Int32 offset,
    /*[in]*/ Int32 length,
    /*[out]*/ ArrayOf<Char8>* buffer,
    /*[out]*/ Int32* number)
{
    assert(buffer != NULL);
    assert(number != NULL);

    FAIL_RETURN(CheckNotClosed());
    if (offset < 0 || offset > buffer->GetLength() - length || length < 0) {
//      throw new IndexOutOfBoundsException();
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    Int32 outstanding = length;
    while (outstanding > 0) {
        /*
         * If there are bytes in the buffer, grab those first.
         */
        Int32 available = mEnd - mPos;
        if (available > 0) {
            Int32 count = available >= outstanding ? outstanding : available;
            while (count > 0) {
                Int32 bytes;
                Char32 oneChar32;
                FAIL_RETURN(Character::GetCharAt(*mBuf, mPos, &oneChar32, &mPos));
                Character::ToChars(oneChar32, *buffer, offset, &bytes);
                offset += bytes;
                outstanding -= bytes;
                count -= bytes;
            }
        }

        /*
         * Before attempting to read from the underlying stream, make
         * sure we really, really want to. We won't bother if we're
         * done, or if we've already got some bytes and reading from the
         * underlying stream would block.
         */
        Boolean ready;
        FAIL_RETURN(mIn->IsReady(&ready));
        if (outstanding == 0 || (outstanding < length && !ready)) {
            break;
        }

        // assert(pos == end);

        /*
         * If we're unmarked and the requested size is greater than our
         * buffer, read the bytes directly into the caller's buffer. We
         * don't read into smaller buffers because that could result in
         * a many reads.
         */
        if ((mMark == -1 || (mPos - mMark >= mMarkLimit))
                && outstanding >= mBuf->GetLength()) {
            Int32 count;
            FAIL_RETURN(mIn->ReadBufferEx(offset, outstanding, buffer, &count));
            if (count > 0) {
                offset += count;
                outstanding -= count;
                mMark = -1;
            }

            break; // assume the source stream gave us all that it could
        }

        Int32 value;
        FAIL_RETURN(FillBuf(&value));
        if (value == -1) {
            break; // source is exhausted
        }
    }

    Int32 count = length - outstanding;
    *number = (count > 0 || count == length) ? count : -1;

    return NOERROR;
}

ECode BufferedReader::ChompNewline()
{
    Int32 number;
    FAIL_RETURN(FillBuf(&number));

    Int32 nextPos;
    Char32 c;
    FAIL_RETURN(Character::GetCharAt(*mBuf, mPos, &c, &nextPos));
    if ((mPos != mEnd || number != -1) && c == '\n') {
        mPos = nextPos;
    }
    return NOERROR;
}

ECode BufferedReader::ReadLine(
    /* [out] */ String* contents)
{
    Mutex::Autolock lock(mLock);

    return ReadLineLocked(contents);
}

ECode BufferedReader::ReadLineLocked(
    /* [out] */ String* contents)
{
    FAIL_RETURN(CheckNotClosed());
    /* has the underlying stream been exhausted? */
    Int32 number1;
    FAIL_RETURN(FillBuf(&number1));
    if (mPos == mEnd && number1 == -1) {
        contents = NULL;
        return NOERROR;
    }

    char* str = (char*)mBuf->GetPayload();
    for (Int32 charPos = mPos; charPos < mEnd; charPos++) {
        Char32 ch;
        FAIL_RETURN(Character::GetCharAt(*mBuf, mPos, &ch, &mPos));
        // BEGIN android-note
        // a switch statement may be more efficient
        // END android-note
        if (ch > '\r') {
            continue;
        }
        if (ch == '\n') {
            *contents = String(str + mPos, charPos - mPos);
            mPos = charPos + 1;
            return NOERROR;
        }
        else if (ch == '\r') {
            *contents = String(str + mPos, charPos - mPos);
            mPos = charPos + 1;
            Int32 number2;
            FAIL_RETURN(FillBuf(&number2));
            //todo: String(str) is not efficient
            if ((mPos < mEnd) || (number2 != -1)) {
                Char32 c;
                Character::GetCharAt(*mBuf, mPos, &c, NULL);
                if (c == '\n') {
                    mPos++;
                }
            }
            return NOERROR;
        }
    }

    Char32 eol = '\0';
    // StringBuilder result = new StringBuilder(80);
    /* Typical Line Length */

    // result.append(buf, pos, end - pos);
    String res = String(str + mPos, mEnd - mPos);
    String res2 = String(str);
    while (TRUE) {
        mPos = mEnd;

        /* Are there buffered characters available? */
        if (eol == '\n') {
            // return result.toString();
            *contents = res;
            return NOERROR;
        }
        // attempt to fill buffer
        Int32 number3;
        FAIL_RETURN(FillBuf(&number3));
        if (number3 == -1) {
            // characters or null.
            // return result.length() > 0 || eol != '\0'
                    // ? result.toString()
                    // : null;
            *contents = res.GetLength() > 0 || eol != '\0'
                    ? res : (const char*)NULL;
            return NOERROR;
        }
        for (Int32 charPos = mPos; charPos < mEnd; charPos++) {
            Char32 c;
            Character::GetCharAt(*mBuf, charPos, &c, NULL);
            if (eol == '\0') {
                if ((c == '\n' || c == '\r')) {
                    eol = c;
                }
            }
            else if (eol == '\r' && c == '\n') {
                String res3(NULL);
                if (charPos > mPos) {
                    res3 = String(str + mPos, charPos - mPos - 1);
                }
                mPos = charPos + 1;
                // return result.toString();
                *contents = res3;
                return NOERROR;
            }
            else {
                String res4(NULL);
                if (charPos > mPos) {
                    res4 = String(str + mPos, charPos - mPos - 1);
                }
                mPos = charPos;
                // return result.toString();
                *contents = res4;
                return NOERROR;
            }
        }
//        if (eol == '\0') {
//            result.Append(res2, mPos, mEnd - mPos);
//        }
//        else {
//            result.Append(res2, mPos, mEnd - mPos - 1);
//        }
    }

    return NOERROR;
}

ECode BufferedReader::IsReady(
        /*[out]*/ Boolean* ready)
{
    assert(ready != NULL);
    Mutex::Autolock lock(mLock);

    return IsReadyLocked(ready);
}

ECode BufferedReader::IsReadyLocked(
        /*[out]*/ Boolean* ready)
{
    assert(ready != NULL);

    FAIL_RETURN(CheckNotClosed());
    Boolean isReady;
    FAIL_RETURN(mIn->IsReady(&isReady));
    *ready = ((mEnd - mPos) > 0) || isReady;

    return NOERROR;
}

ECode BufferedReader::Reset()
{
    Mutex::Autolock lock(mLock);

    return ResetLocked();
}

ECode BufferedReader::ResetLocked()
{
    FAIL_RETURN(CheckNotClosed());
    if (mMark == -1) {
//      throw new IOException("Invalid mark");
        return E_IO_EXCEPTION;
    }
    mPos = mMark;

    return NOERROR;
}

ECode BufferedReader::Skip(
        /*[in]*/ Int64 amount,
        /*[out]*/ Int64* number)
{
    assert(number != NULL);

    if (amount < 0) {
//      throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Mutex::Autolock lock(mLock);

    return SkipLocked(amount, number);
}

ECode BufferedReader::SkipLocked(
        /*[in]*/ Int64 amount,
        /*[out]*/ Int64* number)
{
    assert(number != NULL);

    if (amount < 0) {
//      throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    FAIL_RETURN(CheckNotClosed());
    if (amount < 1) {
        *number = 0;
        return NOERROR;
    }
    Int64 skipped = 0;
    while (skipped < amount) {
        if (mPos == mEnd) {
            Int32 num;
            FAIL_RETURN(FillBuf(&num));
            if (num == -1) {
                *number = skipped;
                return NOERROR;
            }
        }
        Char32 c;
        Character::GetCharAt(*mBuf, mPos, &c, &mPos);
        skipped++;
    }
    *number = amount;
    return NOERROR;
}