
#include "cmdef.h"
#include "CCheckedInputStream.h"
#include <elastos/Math.h>

using namespace Elastos::Core;


/**
 * Reads one byte of data from the underlying input stream and updates the
 * checksum with the byte data.
 *
 * @return {@code -1} at the end of the stream, a single byte value
 *         otherwise.
 * @throws IOException
 *             if an {@code IOException} occurs.
 */
//@Override
ECode CCheckedInputStream::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    FAIL_RETURN(mIn->Read(value));
    if (*value != -1) {
        mCheck->Update(*value);
    }
    return NOERROR;
}

/**
 * Reads up to n bytes of data from the underlying input stream, storing it
 * into {@code buf}, starting at offset {@code off}. The checksum is
 * updated with the bytes read.
 *
 * @param buf
 *            the byte array in which to store the bytes read.
 * @param off
 *            the initial position in {@code buf} to store the bytes read
 *            from this stream.
 * @param nbytes
 *            the maximum number of bytes to store in {@code buf}.
 * @return the number of bytes actually read or {@code -1} if arrived at the
 *         end of the filtered stream while reading the data.
 * @throws IOException
 *             if this stream is closed or some I/O error occurs.
 */
//@Override
ECode CCheckedInputStream::ReadBufferEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 nbytes,
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    FAIL_RETURN(mIn->ReadBufferEx(off, nbytes, buffer, number));
    if (*number != -1) {
        mCheck->UpdateEx2(*buffer, off, *number);
    }
    return NOERROR;
}

/**
 * Returns the checksum calculated on the stream read so far.
 *
 * @return the updated checksum.
 */
ECode CCheckedInputStream::GetChecksum(
    /* [out] */ IChecksum** checksum)
{
    VALIDATE_NOT_NULL(checksum);

    *checksum = mCheck.Get();
    (*checksum)->AddRef();
    return NOERROR;
}

/**
 * Skip up to n bytes of data on the underlying input stream. Any skipped
 * bytes are added to the running checksum value.
 *
 * @param nbytes
 *            the number of bytes to skip.
 * @throws IOException
 *             if this stream is closed or another I/O error occurs.
 * @return the number of bytes skipped.
 */
//@Override
ECode CCheckedInputStream::Skip(
    /* [in] */ Int64 nbytes,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);

    if (nbytes < 1) {
        *number = 0;
        return NOERROR;
    }
    Int64 skipped = 0;
    ArrayOf<Byte>* b = ArrayOf<Byte>::Alloc((Int32)Math::Min(nbytes, 2048ll));
    Int32 x, v;
    while (skipped != nbytes) {
        mIn->ReadBufferEx(0,
                (v = (Int32)(nbytes - skipped)) > b->GetLength() ? b->GetLength() : v,
                b, &x);
        if (x == -1) {
            *number = skipped;
            return NOERROR;
        }
        mCheck->UpdateEx2(*b, 0, x);
        skipped += x;
    }
    ArrayOf<Byte>::Free(b);
    *number = skipped;
    return NOERROR;
}

ECode CCheckedInputStream::Available(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    return mIn->Available(number);
}

ECode CCheckedInputStream::Close()
{
    return mIn->Close();
}

ECode CCheckedInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    Mutex::Autolock lock(_m_syncLock);

    return mIn->Mark(readLimit);
}

ECode CCheckedInputStream::Reset()
{
    Mutex::Autolock lock(_m_syncLock);

    return mIn->Reset();
}

ECode CCheckedInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);
    return mIn->IsMarkSupported(supported);
}

ECode CCheckedInputStream::ReadBuffer(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    VALIDATE_NOT_NULL(buffer);
    return ReadBufferEx(0, buffer->GetLength(), buffer, number);
}

/**
 * Constructs a new {@code CheckedInputStream} on {@code InputStream}
 * {@code is}. The checksum will be calculated using the algorithm
 * implemented by {@code csum}.
 *
 * <p><strong>Warning:</strong> passing a null source creates an invalid
 * {@code CheckedInputStream}. All operations on such a stream will fail.
 *
 * @param is
 *            the input stream to calculate checksum from.
 * @param csum
 *            an entity implementing the checksum algorithm.
 */
ECode CCheckedInputStream::constructor(
    /* [in] */ IInputStream* is,
    /* [in] */ IChecksum* csum)
{
    mIn = is;
    mCheck = csum;
    return NOERROR;
}
