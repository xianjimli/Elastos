
#include "cmdef.h"
#include "DeflaterInputStream.h"
#include <elastos/Math.h>

using namespace Elastos::Core;


const Int32 DeflaterInputStream::DEFAULT_BUFFER_SIZE;

DeflaterInputStream::DeflaterInputStream()
    : mBuf(NULL)
    , mClosed(FALSE)
    , mAvailable(TRUE)
{}

DeflaterInputStream::~DeflaterInputStream()
{
    if (mBuf != NULL) ArrayOf<Byte>::Free(mBuf);
}

/**
 * Closes the underlying input stream and discards any remaining uncompressed
 * data.
 */
//@Override
ECode DeflaterInputStream::Close()
{
    mClosed = TRUE;
    FAIL_RETURN(mDef->End());
    return mIn->Close();
}

/**
 * Reads a byte from the compressed input stream. The result will be a byte of compressed
 * data corresponding to an uncompressed byte or bytes read from the underlying stream.
 *
 * @return the byte or -1 if the end of the stream has been reached.
 */
//@Override
ECode DeflaterInputStream::Read(
    /* [out] */ Int32* value)
{
    ArrayOf_<Byte, 1> result;
    Int32 count;
    FAIL_RETURN(ReadBufferEx(0, 1, &result, &count));
    if (count == -1) {
        *value = -1;
        return NOERROR;
    }
    *value = result[0] & 0xff;
    return NOERROR;
}

 /**
 * Reads compressed data into a byte buffer. The result will be bytes of compressed
 * data corresponding to an uncompressed byte or bytes read from the underlying stream.
 *
 * @param b
 *            the byte buffer that compressed data will be read into.
 * @param off
 *            the offset in the byte buffer where compressed data will start
 *            to be read into.
 * @param len
 *            the length of the compressed data that is expected to read.
 * @return the number of bytes read or -1 if the end of the compressed input
 *         stream has been reached.
 */
//@Override
ECode DeflaterInputStream::ReadBufferEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [out] */ ArrayOf<Byte>* b,
    /* [out] */ Int32* number)
{
    FAIL_RETURN(CheckClosed());
    if (b == NULL) {
//        throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    if (off < 0 || len < 0 || len > b->GetLength() - off) {
//        throw new IndexOutOfBoundsException();
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (len == 0) {
        *number = 0;
        return NOERROR;
    }

    if (!mAvailable) {
        *number = -1;
        return NOERROR;
    }

    Int32 count = 0;
    Boolean finished;
    while (count < len && (mDef->Finished(&finished), !finished)) {
        Boolean needs;
        mDef->NeedsInput(&needs);
        if (needs) {
            // read data from input stream
            Int32 byteCount;
            mIn->ReadBuffer(mBuf, &byteCount);
            if (byteCount == -1) {
                mDef->Finish();
            }
            else {
                mDef->SetInputEx(*mBuf, 0, byteCount);
            }
        }
        Int32 byteCount;
        mDef->DeflateEx(0, Math::Min(mBuf->GetLength(), len - count), mBuf, &byteCount);
        if (byteCount == -1) {
            break;
        }
        memcpy(b->GetPayload() + off + count, mBuf->GetPayload(), byteCount);
        count += byteCount;
    }
    if (count == 0) {
        count = -1;
        mAvailable = FALSE;
    }
    *number = count;
    return NOERROR;
}

/**
 * {@inheritDoc}
 * <p>Note: if {@code n > Integer.MAX_VALUE}, this stream will only attempt to
 * skip {@code Integer.MAX_VALUE} bytes.
 */
//@Override
ECode DeflaterInputStream::Skip(
    /* [in] */ Int64 n,
    /* [out] */ Int64* number)
{
    if (n < 0) {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (n > Math::INT32_MAX_VALUE) {
        n = Math::INT32_MAX_VALUE;
    }
    FAIL_RETURN(CheckClosed());

    Int32 remaining = (Int32)n;
    ArrayOf<Byte>* tmp = ArrayOf<Byte>::Alloc(Math::Min(remaining, DEFAULT_BUFFER_SIZE));
    while (remaining > 0) {
        Int32 count;
        ECode ec = ReadBufferEx(0, Math::Min(remaining, tmp->GetLength()), tmp, &count);
        if (FAILED(ec)) {
            ArrayOf<Byte>::Free(tmp);
            return ec;
        }
        if (count == -1) {
            break;
        }
        remaining -= count;
    }
    *number = n - remaining;
    return NOERROR;
}

/**
 * Returns 0 when when this stream has exhausted its input; and 1 otherwise.
 * A result of 1 does not guarantee that further bytes can be returned,
 * with or without blocking.
 *
 * <p>Although consistent with the RI, this behavior is inconsistent with
 * {@link InputStream#available()}, and violates the <a
 * href="http://en.wikipedia.org/wiki/Liskov_substitution_principle">Liskov
 * Substitution Principle</a>. This method should not be used.
 *
 * @return 0 if no further bytes are available. Otherwise returns 1,
 *         which suggests (but does not guarantee) that additional bytes are
 *         available.
 * @throws IOException if this stream is closed or an error occurs
 */
//@Override
ECode DeflaterInputStream::Available(
        /* [out] */ Int32* number)
{
    FAIL_RETURN(CheckClosed());
    *number = mAvailable ? 1 : 0;
    return NOERROR;
}

/**
 * Returns false because {@code DeflaterInputStream} does not support
 * {@code mark}/{@code reset}.
 */
//@Override
ECode DeflaterInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    *supported = FALSE;
    return NOERROR;
}

/**
 * This operation is not supported and does nothing.
 */
//@Override
ECode DeflaterInputStream::Mark(
    /* [in] */ Int32 limit)
{
    return NOERROR;
}

/**
 * This operation is not supported and throws {@code IOException}.
 */
//@Override
ECode DeflaterInputStream::Reset()
{
    return E_IO_EXCEPTION;
}

ECode DeflaterInputStream::CheckClosed()
{
    if (mClosed) {
//        throw new IOException("Stream is closed");
        return E_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode DeflaterInputStream::ReadBuffer(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    VALIDATE_NOT_NULL(buffer);
    return ReadBufferEx(0, buffer->GetLength(), buffer, number);
}

/**
 * Constructs a {@code DeflaterInputStream} with a new {@code Deflater} and an
 * implementation-defined default internal buffer size. {@code in} is a source of
 * uncompressed data, and this stream will be a source of compressed data.
 *
 * @param in the source {@code InputStream}
 */
ECode DeflaterInputStream::Init(
    /* [in] */ IInputStream* in)
{
    AutoPtr<CDeflater> deflater;
    CDeflater::NewByFriend((CDeflater**)&deflater);
    return Init(in, (IDeflater*)deflater.Get(), DEFAULT_BUFFER_SIZE);
}

/**
 * Constructs a {@code DeflaterInputStream} with the given {@code Deflater} and an
 * implementation-defined default internal buffer size. {@code in} is a source of
 * uncompressed data, and this stream will be a source of compressed data.
 *
 * @param in the source {@code InputStream}
 * @param deflater the {@code Deflater} to be used for compression
 */
ECode DeflaterInputStream::Init(
    /* [in] */ IInputStream* in,
    /* [in] */ IDeflater* deflater)
{
    return Init(in, deflater, DEFAULT_BUFFER_SIZE);
}

/**
 * Constructs a {@code DeflaterInputStream} with the given {@code Deflater} and
 * given internal buffer size. {@code in} is a source of
 * uncompressed data, and this stream will be a source of compressed data.
 *
 * @param in the source {@code InputStream}
 * @param deflater the {@code Deflater} to be used for compression
 * @param bufferSize the length in bytes of the internal buffer
 */
ECode DeflaterInputStream::Init(
    /* [in] */ IInputStream* in,
    /* [in] */ IDeflater* deflater,
    /* [in] */ Int32 bufferSize)
{
    mIn = in;
    if (in == NULL || deflater == NULL) {
//        throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    if (bufferSize <= 0) {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mDef = (CDeflater*)deflater;
    mBuf = ArrayOf<Byte>::Alloc(bufferSize);
    return NOERROR;
}
