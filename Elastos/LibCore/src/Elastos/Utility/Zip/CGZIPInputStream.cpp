
#include "cmdef.h"
#include "CGZIPInputStream.h"


const Int32 CGZIPInputStream::FCOMMENT;

const Int32 CGZIPInputStream::FEXTRA;

const Int32 CGZIPInputStream::FHCRC;

const Int32 CGZIPInputStream::FNAME;

CGZIPInputStream::CGZIPInputStream()
    : mEos(FALSE)
{
    CCRC32::NewByFriend((CCRC32**)&mCrc);
}

/**
 * Closes this stream and any underlying streams.
 */
//@Override
ECode CGZIPInputStream::Close()
{
    mEos = TRUE;
    return InflaterInputStream::Close();
}

Int64 CGZIPInputStream::GetInt64(
    /* [in] */ ArrayOf<Byte>& buffer,
    /* [in] */ Int32 off)
{
    Int64 l = 0;
    l |= (buffer[off] & 0xFF);
    l |= (buffer[off + 1] & 0xFF) << 8;
    l |= (buffer[off + 2] & 0xFF) << 16;
    l |= ((Int64)(buffer[off + 3] & 0xFF)) << 24;
    return l;
}

Int32 CGZIPInputStream::GetInt16(
    /* [in] */ ArrayOf<Byte>& buffer,
    /* [in] */ Int32 off)
{
    return (buffer[off] & 0xFF) | ((buffer[off + 1] & 0xFF) << 8);
}

/**
 * Reads and decompresses GZIP data from the underlying stream into the
 * given buffer.
 *
 * @param buffer
 *            Buffer to receive data
 * @param off
 *            Offset in buffer to store data
 * @param nbytes
 *            Number of bytes to read
 */
//@Override
ECode CGZIPInputStream::ReadBufferEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 nbytes,
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    VALIDATE_NOT_NULL(buffer);

    if (mClosed) {
//        throw new IOException("Stream is closed");
        return E_IO_EXCEPTION;
    }
    if (mEos) {
        *number = -1;
        return NOERROR;
    }
    // avoid int overflow, check null buffer
    if (off > buffer->GetLength() || nbytes < 0 || off < 0
            || buffer->GetLength() - off < nbytes) {
//        throw new ArrayIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    Int32 bytesRead;
    InflaterInputStream::ReadBufferEx(off, nbytes, buffer, &bytesRead);
    mEos = mEof;

    if (bytesRead != -1) {
        mCrc->UpdateEx2(*buffer, off, bytesRead);
    }

    if (mEos) {
        FAIL_RETURN(VerifyCrc());
    }

    *number = bytesRead;
    return NOERROR;
}

ECode CGZIPInputStream::VerifyCrc()
{
    // Get non-compressed bytes read by fill
    Int32 size;
    mInf->GetRemaining(&size);
    const Int32 trailerSize = 8; // crc (4 bytes) + total out (4 bytes)
    ArrayOf_<Byte, trailerSize> b;
    Int32 copySize = (size > trailerSize) ? trailerSize : size;

    memcpy(mBuf->GetPayload() + mLen - size, b.GetPayload(), copySize);
    FAIL_RETURN(ReadFully(copySize, trailerSize - copySize, &b));

    Int64 checksum;
    mCrc->GetValue(&checksum);
    if (GetInt64(b, 0) != checksum) {
//        throw new IOException("CRC mismatch");
        return E_IO_EXCEPTION;
    }
    Int32 value;
    mInf->GetTotalOut(&value);
    if ((Int32)GetInt64(b, 4) != value) {
//        throw new IOException("Size mismatch");
        return E_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CGZIPInputStream::ReadFully(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ ArrayOf<Byte>* buffer)
{
    Int32 result;
    while (length > 0) {
        FAIL_RETURN(mIn->ReadBufferEx(offset, length, buffer, &result));
        if (result == -1) {
//            throw new EOFException();
            return E_EOF_EXCEPTION;
        }
        offset += result;
        length -= result;
    }
    return NOERROR;
}

ECode CGZIPInputStream::ReadZeroTerminated(
    /* [in] */ Boolean hcrc)
{
    Int32 result;
    FAIL_RETURN(mIn->Read(&result));
    while (result > 0) {
        if (hcrc) {
            mCrc->Update(result);
        }
        FAIL_RETURN(mIn->Read(&result));
    }
    if (result == -1) {
//        throw new EOFException();
        return E_EOF_EXCEPTION;
    }
    // Add the zero
    if (hcrc) {
        mCrc->Update(result);
    }
    return NOERROR;
}

ECode CGZIPInputStream::Skip(
    /* [in] */ Int64 nbytes,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);
    return InflaterInputStream::Skip(nbytes, number);
}

ECode CGZIPInputStream::Available(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    return InflaterInputStream::Available(number);
}

ECode CGZIPInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    return InflaterInputStream::Mark(readLimit);
}

ECode CGZIPInputStream::Reset()
{
    return InflaterInputStream::Reset();
}

ECode CGZIPInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);
    return InflaterInputStream::IsMarkSupported(supported);
}

ECode CGZIPInputStream::Read(
    /* [out] */ Int32* val)
{
    VALIDATE_NOT_NULL(val);
    return InflaterInputStream::Read(val);
}

ECode CGZIPInputStream::ReadBuffer(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    return InflaterInputStream::ReadBuffer(buffer, number);
}

/**
 * Construct a {@code GZIPInputStream} to read from GZIP data from the
 * underlying stream.
 *
 * @param is
 *            the {@code InputStream} to read data from.
 * @throws IOException
 *             if an {@code IOException} occurs.
 */
ECode CGZIPInputStream::constructor(
    /* [in] */ IInputStream* is)
{
    return constructor(is, BUF_SIZE);
}

/**
 * Construct a {@code GZIPInputStream} to read from GZIP data from the
 * underlying stream. Set the internal buffer size to {@code size}.
 *
 * @param is
 *            the {@code InputStream} to read data from.
 * @param size
 *            the internal read buffer size.
 * @throws IOException
 *             if an {@code IOException} occurs.
 */
ECode CGZIPInputStream::constructor(
    /* [in] */ IInputStream* is,
    /* [in] */ Int32 size)
{
    AutoPtr<CInflater> inflater;
    CInflater::NewByFriend(TRUE, (CInflater**)&inflater);
    FAIL_RETURN(InflaterInputStream::Init(is, inflater.Get(), size));
    ArrayOf_<Byte, 10> header;
    FAIL_RETURN(ReadFully(0, header.GetLength(), &header));
    Int32 magic = GetInt16(header, 0);
    if (magic != GZIP_MAGIC) {
//        throw new IOException(String.format("unknown format (magic number %x)", magic));
        return E_IO_EXCEPTION;
    }
    Int32 flags = header[3];
    Boolean hcrc = (flags & FHCRC) != 0;
    if (hcrc) {
        mCrc->UpdateEx2(header, 0, header.GetLength());
    }
    if ((flags & FEXTRA) != 0) {
        FAIL_RETURN(ReadFully(0, 2, &header));
        if (hcrc) {
            mCrc->UpdateEx2(header, 0, 2);
        }
        Int32 length = GetInt16(header, 0);
        while (length > 0) {
            Int32 max = length > mBuf->GetLength() ? mBuf->GetLength() : length;
            Int32 result;
            FAIL_RETURN(mIn->ReadBufferEx(0, max, mBuf, &result));
            if (result == -1) {
    //            throw new EOFException();
                return E_EOF_EXCEPTION;
            }
            if (hcrc) {
                mCrc->UpdateEx2(*mBuf, 0, result);
            }
            length -= result;
        }
    }
    if ((flags & FNAME) != 0) {
        FAIL_RETURN(ReadZeroTerminated(hcrc));
    }
    if ((flags & FCOMMENT) != 0) {
        FAIL_RETURN(ReadZeroTerminated(hcrc));
    }
    if (hcrc) {
        FAIL_RETURN(ReadFully(0, 2, &header));
        Int32 crc16 = GetInt16(header, 0);
        Int64 value;
        mCrc->GetValue(&value);
        if ((value & 0xffff) != crc16) {
    //        throw new IOException("CRC mismatch");
            return E_IO_EXCEPTION;
        }
        mCrc->Reset();
    }
    return NOERROR;
}