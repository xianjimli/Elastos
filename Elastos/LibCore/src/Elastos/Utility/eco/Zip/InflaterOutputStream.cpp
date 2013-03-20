
#include "cmdef.h"
#include "InflaterOutputStream.h"


const Int32 InflaterOutputStream::DEFAULT_BUFFER_SIZE;


InflaterOutputStream::InflaterOutputStream()
    : mBuf(NULL)
    , mClosed(FALSE)
{}

InflaterOutputStream::~InflaterOutputStream()
{
    if (mBuf != NULL) ArrayOf<Byte>::Free(mBuf);
}

/**
 * Writes remaining data into the output stream and closes the underlying
 * output stream.
 */
ECode InflaterOutputStream::Close()
{
    if (!mClosed) {
        FAIL_RETURN(Finish());
        FAIL_RETURN(mInf->End());
        FAIL_RETURN(mOut->Close());
        mClosed = TRUE;
    }
    return NOERROR;
}

ECode InflaterOutputStream::Flush()
{
    FAIL_RETURN(Finish());
    return mOut->Flush();
}

/**
 * Finishes writing current uncompressed data into the InflaterOutputStream
 * without closing it.
 *
 * @throws IOException if an I/O error occurs, or the stream has been closed
 */
ECode InflaterOutputStream::Finish()
{
    FAIL_RETURN(CheckClosed());
    return Write();
}

/**
 * Writes a byte to the decompressing output stream. {@code b} should be a byte of
 * compressed input. The corresponding uncompressed data will be written to the underlying
 * stream.
 *
 * @param b the byte
 * @throws IOException if an I/O error occurs, or the stream has been closed
 * @throws ZipException if a zip exception occurs.
 */
ECode InflaterOutputStream::Write(
    /* [in] */ Int32 b)
{
    ArrayOf_<Byte, 1> buf;
    buf[0] = (Byte)b;
    return WriteBufferEx(0, 1, buf);
}

/**
 * Writes bytes to the decompressing output stream. {@code b} should be an array of
 * compressed input. The corresponding uncompressed data will be written to the underlying
 * stream.
 *
 * @param b the byte array
 * @param off the start offset in the byte array
 * @param len the number of the bytes to take from the byte array
 * @throws IOException if an I/O error occurs, or the stream has been closed
 * @throws ZipException if a zip exception occurs.
 * @throws NullPointerException if {@code b == null}.
 * @throws IndexOutOfBoundsException if {@code off < 0 || len < 0 || off + len > b.length}
 */
ECode InflaterOutputStream::WriteBufferEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [in] */ const ArrayOf<Byte>& b)
{
    FAIL_RETURN(CheckClosed());
    if (off < 0 || len < 0 || len > b.GetLength() - off) {
//        throw new IndexOutOfBoundsException();
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    FAIL_RETURN(mInf->SetInputEx(b, off, len));
    return Write();
}

ECode InflaterOutputStream::Write()
{
    Int32 inflated;
    ECode ec = mInf->Inflate(mBuf, &inflated);
    if (FAILED(ec)) {
        if (ec == E_DATA_FORMAT_EXCEPTION) ec = E_ZIP_EXCEPTION;
        return ec;
    }
    while (inflated > 0) {
        FAIL_RETURN(mOut->WriteBufferEx(0, inflated, *mBuf));
        ec = mInf->Inflate(mBuf, &inflated);
        if (FAILED(ec)) {
            if (ec == E_DATA_FORMAT_EXCEPTION) ec = E_ZIP_EXCEPTION;
            return ec;
        }
    }
    return NOERROR;
}

ECode InflaterOutputStream::CheckClosed()
{
    if (mClosed) {
//        throw new IOException();
        return E_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode InflaterOutputStream::WriteBuffer(
    /* [in] */ const ArrayOf<Byte>& buffer)
{
    // BEGIN android-note
    // changed array notation to be consistent with the rest of harmony
    // END android-note
    return WriteBufferEx(0, buffer.GetLength(), buffer);
}

/**
 * Constructs an {@code InflaterOutputStream} with a new {@code Inflater} and an
 * implementation-defined default internal buffer size. {@code out} is a destination
 * for uncompressed data, and compressed data will be written to this stream.
 *
 * @param out the destination {@code OutputStream}
 */
ECode InflaterOutputStream::Init(
    /* [in] */ IOutputStream* out)
{
    AutoPtr<CInflater> inflater;
    CInflater::NewByFriend((CInflater**)&inflater);
    return Init(out, (IInflater*)inflater.Get());
}

/**
 * Constructs an {@code InflaterOutputStream} with the given {@code Inflater} and an
 * implementation-defined default internal buffer size. {@code out} is a destination
 * for uncompressed data, and compressed data will be written to this stream.
 *
 * @param out the destination {@code OutputStream}
 * @param inf the {@code Inflater} to be used for decompression
 */
ECode InflaterOutputStream::Init(
    /* [in] */ IOutputStream* out,
    /* [in] */ IInflater* inf)
{
    return Init(out, inf, DEFAULT_BUFFER_SIZE);
}

/**
 * Constructs an {@code InflaterOutputStream} with the given {@code Inflater} and
 * given internal buffer size. {@code out} is a destination
 * for uncompressed data, and compressed data will be written to this stream.
 *
 * @param out the destination {@code OutputStream}
 * @param inf the {@code Inflater} to be used for decompression
 * @param bufferSize the length in bytes of the internal buffer
 */
ECode InflaterOutputStream::Init(
    /* [in] */ IOutputStream* out,
    /* [in] */ IInflater* inf,
    /* [in] */ Int32 bufferSize)
{
    mOut = out;
    if (out == NULL || inf == NULL) {
//        throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    if (bufferSize <= 0) {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mInf = (CInflater*)inf;
    mBuf = ArrayOf<Byte>::Alloc(bufferSize);
    return NOERROR;
}
