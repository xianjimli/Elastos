
#include "cmdef.h"
#include "BufferedInputStream.h"

BufferedInputStream::BufferedInputStream()
    : mBuf(NULL)
    , mMarkpos(-1)
{
}

BufferedInputStream::~BufferedInputStream()
{
    if (mBuf != NULL) {
        ArrayOf<Byte>::Free(mBuf);
    }
}

ECode BufferedInputStream::Init(
    /* [in] */ IInputStream* in,
    /* [in] */ Int32 size)
{
    FAIL_RETURN(FilterInputStream::Init(in));
    if (size <= 0) {
//        throw new IllegalArgumentException("size <= 0");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mBuf = ArrayOf<Byte>::Alloc(size);
    if (mBuf == NULL) return E_OUT_OF_MEMORY_ERROR;
    return NOERROR;
}

ECode BufferedInputStream::Available(
    /* [out] */ Int32* number)
{
    assert(number != NULL);

    AutoPtr<IInputStream> localIn = mIn; // 'in' could be invalidated by close()
    if (mBuf == NULL || localIn == NULL) {
        return StreamClosed();
    }
    Int32 count;
    FAIL_RETURN(localIn->Available(&count));
    *number = mCount - mPos + count;
    return NOERROR;
}

ECode BufferedInputStream::StreamClosed()
{
//    throw new IOException("BufferedInputStream is closed");
    return E_IO_EXCEPTION;
}

ECode BufferedInputStream::Close()
{
    mBuf = NULL;
    AutoPtr<IInputStream> localIn = mIn;
    mIn = NULL;
    if (localIn != NULL) {
        return localIn->Close();
    }
    return NOERROR;
}

ECode BufferedInputStream::Fillbuf(
    /* [in] */ IInputStream* localIn,
    /* [in, out] */ ArrayOf<Byte>** localBuf,
    /* [out] */ Int32* number)
{
    if (mMarkpos == -1 || (mPos - mMarkpos >= mMarklimit)) {
        /* Mark position not set or exceeded readlimit */
        Int32 result;
        FAIL_RETURN(localIn->ReadBuffer(*localBuf, &result));
        if (result > 0) {
            mMarkpos = -1;
            mPos = 0;
            mCount = result == -1 ? 0 : result;
        }
        *number = result;
        return NOERROR;
    }
    Int32 length = (*localBuf)->GetLength();
    if (mMarkpos == 0 && mMarklimit > length) {
        /* Increase buffer size to accommodate the readlimit */
        Int32 newLength = length * 2;
        if (newLength > mMarklimit) {
            newLength = mMarklimit;
        }
        ArrayOf<Byte>* newbuf = ArrayOf<Byte>::Alloc(newLength);
        for (Int32 i = 0; i < length; i++) {
            (*newbuf)[i] = (**localBuf)[i];
        }
        // Reassign buf, which will invalidate any local references
        // FIXME: what if buf was null?
        if (mBuf != NULL) {
            ArrayOf<Byte>::Free(mBuf);
        }
        *localBuf = mBuf = newbuf;
    }
    else if (mMarkpos > 0) {
        for (Int32 i = 0; i < length - mMarkpos; i++) {
            (**localBuf)[i] = (**localBuf)[mMarkpos + i];
        }
    }
    /* Set the new position and mark position */
    mPos -= mMarkpos;
    mCount = mMarkpos = 0;
    Int32 bytesread;
    FAIL_RETURN(localIn->ReadBufferEx(mPos, (*localBuf)->GetLength() - mPos,
            *localBuf, &bytesread));
    mCount = bytesread <= 0 ? mPos : mPos + bytesread;
    *number = bytesread;
    return NOERROR;
}

ECode BufferedInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    mMarklimit = readLimit;
    mMarkpos = mPos;
    return NOERROR;
}

ECode BufferedInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    assert(supported != NULL);
    *supported = TRUE;
    return NOERROR;
}

ECode BufferedInputStream::Read(
    /* [out] */ Int32* value)
{
    assert(value != NULL);

    // Use local refs since buf and in may be invalidated by an
    // unsynchronized close()
    ArrayOf<Byte>* localBuf = mBuf;
    AutoPtr<IInputStream> localIn = mIn;
    if (localBuf == NULL || localIn == NULL) {
        return StreamClosed();
    }

    /* Are there buffered bytes available? */
    Int32 number;
    FAIL_RETURN(Fillbuf(localIn, &localBuf, &number));
    if (mPos >= mCount && number == -1) {
        *value = -1;
        return NOERROR; /* no, fill buffer */
    }
    // localBuf may have been invalidated by fillbuf
    if (localBuf != mBuf) {
        ArrayOf<Byte>::Free(localBuf);
        localBuf = mBuf;
        if (localBuf == NULL) {
            return StreamClosed();
        }
    }

    /* Did filling the buffer fail with -1 (EOF)? */
    if (mCount - mPos > 0) {
        *value = (*localBuf)[mPos++] & 0xFF;
        return NOERROR;
    }
    *value = -1;
    return NOERROR;
}

ECode BufferedInputStream::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    assert(number != NULL);

    // Use local ref since buf may be invalidated by an unsynchronized
    // close()
    ArrayOf<Byte>* localBuf = mBuf;
    if (localBuf == NULL) {
        return StreamClosed();
    }
    // avoid int overflow
    // BEGIN android-changed
    // Exception priorities (in case of multiple errors) differ from
    // RI, but are spec-compliant.
    // made implicit null check explicit, used (offset | length) < 0
    // instead of (offset < 0) || (length < 0) to safe one operation
    if (buffer == NULL) {
//      throw new NullPointerException("buffer == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    if ((offset | length) < 0 || offset > buffer->GetLength() - length) {
//      throw new IndexOutOfBoundsException();
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    // END android-changed
    if (length == 0) {
        *number = 0;
        return NOERROR;
    }
    AutoPtr<IInputStream> localIn = mIn;
    if (localIn == NULL) {
        return StreamClosed();
    }

    Int32 required;
    if (mPos < mCount) {
        /* There are bytes available in the buffer. */
        Int32 copylength = mCount - mPos >= length ? length : mCount - mPos;
        for (Int32 i = 0; i < copylength; i++) {
            (*buffer)[offset + i] = (*localBuf)[mPos + i];
        }
        mPos += copylength;
        if (copylength == length) {
            *number = copylength;
            return NOERROR;
        }
        FAIL_RETURN(localIn->Available(number));
        if (*number == 0) {
            *number = copylength;
            return NOERROR;
        }
        offset += copylength;
        required = length - copylength;
    } else {
        required = length;
    }

    while (TRUE) {
        Int32 read;
        /*
         * If we're not marked and the required size is greater than the
         * buffer, simply read the bytes directly bypassing the buffer.
         */
        if (mMarkpos == -1 && required >= localBuf->GetLength()) {
            FAIL_RETURN(localIn->ReadBufferEx(offset, required, buffer, &read));
            if (read == -1) {
                *number = required == length ? -1 : length - required;
                return NOERROR;
            }
        } else {
            FAIL_RETURN(Fillbuf(localIn, &localBuf, &read));
            if (read == -1) {
                *number = required == length ? -1 : length - required;
                return NOERROR;
            }
            // localBuf may have been invalidated by fillbuf
            if (localBuf != mBuf) {
                ArrayOf<Byte>::Free(localBuf);
                localBuf = mBuf;
                if (localBuf == NULL) {
                    return StreamClosed();
                }
            }

            read = mCount - mPos >= required ? required : mCount - mPos;
            for (Int32 i = 0; i < read; i++) {
                (*buffer)[offset + i] = (*localBuf)[mPos + i];
            }
            mPos += read;
        }
        required -= read;
        if (required == 0) {
            *number = length;
            return NOERROR;
        }
        FAIL_RETURN(localIn->Available(number));
        if (number == 0) {
            *number = length - required;
            return NOERROR;
        }
        offset += read;
    }
}

ECode BufferedInputStream::Reset()
{
    // BEGIN android-changed
    /*
     * These exceptions get thrown in some "normalish" circumstances,
     * so it is preferable to avoid loading up the whole big set of
     * messages just for these cases.
     */
    if (mBuf == NULL) {
//        throw new IOException("Stream is closed");
        return E_IO_EXCEPTION;
    }
    if (-1 == mMarkpos) {
//        throw new IOException("Mark has been invalidated.");
        return E_IO_EXCEPTION;
    }
    // END android-changed
    mPos = mMarkpos;
    return NOERROR;
}

ECode BufferedInputStream::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    assert(number != NULL);

    // Use local refs since buf and in may be invalidated by an
    // unsynchronized close()
    ArrayOf<Byte>* localBuf = mBuf;
    AutoPtr<IInputStream> localIn = mIn;
    if (localBuf == NULL) {
        return StreamClosed();
    }
    if (count < 1) {
        *number = 0;
        return NOERROR;
    }
    if (localIn == NULL) {
        return StreamClosed();
    }

    if (mCount - mPos >= count) {
        mPos += count;
        *number = count;
        return NOERROR;
    }
    Int64 read = mCount - mPos;
    mPos = mCount;

    if (mMarkpos != -1) {
        if (count <= mMarklimit) {
            Int32 fillnum;
            FAIL_RETURN(Fillbuf(localIn, &localBuf, &fillnum));
            if (fillnum == -1) {
                *number = read;
                return NOERROR;
            }
            if (mCount - mPos >= count - read) {
                mPos += count - read;
                *number = count;
                return NOERROR;
            }
            // Couldn't get all the bytes, skip what we read
            read += (mCount - mPos);
            mPos = mCount;
            *number = read;
            return NOERROR;
        }
    }
    FAIL_RETURN(localIn->Skip(count - read, number));
    *number = read + *number;
    return NOERROR;
}
