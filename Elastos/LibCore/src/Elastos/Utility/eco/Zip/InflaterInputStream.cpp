
#include "cmdef.h"
#include "InflaterInputStream.h"
#include "CZipFile.h"


const Int32 InflaterInputStream::BUF_SIZE;


InflaterInputStream::InflaterInputStream()
    : mClosed(FALSE)
    , mEof(FALSE)
    , mNativeEndBufSize(0)
    , mBuf(NULL)
{}

InflaterInputStream::~InflaterInputStream()
{
    if (mBuf != NULL) ArrayOf<Byte>::Free(mBuf);
}

/**
 * Reads a single byte of decompressed data.
 *
 * @return the byte read.
 * @throws IOException
 *             if an error occurs reading the byte.
 */
//@Override
ECode InflaterInputStream::Read(
    /* [out] */ Int32* value)
{
    ArrayOf_<Byte, 1> b;
    Int32 count;
    FAIL_RETURN(ReadBufferEx(0, 1, &b, &count));
    if (count == -1) {
        *value = -1;
        return NOERROR;
    }
    *value = b[0] & 0xff;
    return NOERROR;
}

/**
 * Reads up to {@code nbytes} of decompressed data and stores it in
 * {@code buffer} starting at {@code off}.
 *
 * @param buffer
 *            the buffer to write data to.
 * @param off
 *            offset in buffer to start writing.
 * @param nbytes
 *            number of bytes to read.
 * @return Number of uncompressed bytes read
 * @throws IOException
 *             if an IOException occurs.
 */
//@Override
ECode InflaterInputStream::ReadBufferEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 nbytes,
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    FAIL_RETURN(CheckClosed());
    if (buffer == NULL) {
//        throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }

    if (off < 0 || nbytes < 0 || off + nbytes > buffer->GetLength()) {
//        throw new IndexOutOfBoundsException();
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    if (nbytes == 0) {
        *number = 0;
        return NOERROR;
    }

    if (mEof) {
        *number = -1;
        return NOERROR;
    }

    // avoid int overflow, check null buffer
    if (off > buffer->GetLength() || nbytes < 0 || off < 0
            || buffer->GetLength() - off < nbytes) {
//        throw new ArrayIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    do {
        Boolean need;
        mInf->NeedsInput(&need);
        if (need) {
            FAIL_RETURN(Fill());
        }
        // Invariant: if reading returns -1 or throws, eof must be true.
        // It may also be true if the next read() should return -1.
        Int32 result;
        if (FAILED(mInf->InflateEx(off, nbytes, buffer, &result))) {
            mEof = TRUE;
            if (mLen == -1) {
//                throw new EOFException();
                return E_EOF_EXCEPTION;
            }
//            throw (IOException) (new IOException().initCause(e));
            return E_IO_EXCEPTION;
        }
        mInf->Finished(&mEof);
        if (result > 0) {
            *number = result;
            return NOERROR;
        }
        else if (mEof) {
            *number = -1;
            return NOERROR;
        }
        else if (mInf->NeedsDictionary(&need), need) {
            mEof = TRUE;
            *number = -1;
            return NOERROR;
        }
        else if (mLen == -1) {
            mEof = TRUE;
//            throw new EOFException();
            return E_EOF_EXCEPTION;
            // If result == 0, fill() and try again
        }
    } while (TRUE);
}

/**
 * Fills the input buffer with data to be decompressed.
 *
 * @throws IOException
 *             if an {@code IOException} occurs.
 */
ECode InflaterInputStream::Fill()
{
    FAIL_RETURN(CheckClosed());
    // BEGIN android-only
    if (mNativeEndBufSize > 0) {
//        ZipFile.RAFStream is = (ZipFile.RAFStream)in;
//        synchronized (is.mSharedRaf) {
//            long len = is.mLength - is.mOffset;
//            if (len > nativeEndBufSize) len = nativeEndBufSize;
//            int cnt = inf.setFileInput(is.mSharedRaf.getFD(), is.mOffset, (int)nativeEndBufSize);
//            is.skip(cnt);
//        }
    }
    else {
        mIn->ReadBuffer(mBuf, &mLen);
        if (mLen > 0) {
            mInf->SetInputEx(*mBuf, 0, mLen);
        }
    }
    // END android-only
    return NOERROR;
}

/**
 * Skips up to {@code byteCount} bytes of uncompressed data.
 *
 * @param byteCount the number of bytes to skip.
 * @return the number of uncompressed bytes skipped.
 * @throws IOException if an error occurs skipping.
 */
//@Override
ECode InflaterInputStream::Skip(
    /* [in] */ Int64 byteCount,
    /* [out] */ Int64* number)
{
//    return Streams.skipByReading(this, byteCount);
    return E_NOT_IMPLEMENTED;
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
ECode InflaterInputStream::Available(
    /* [out] */ Int32* number)
{
    FAIL_RETURN(CheckClosed());
    if (mEof) {
        *number = 0;
        return NOERROR;
    }
    *number = 1;
    return NOERROR;
}

/**
 * Closes the input stream.
 *
 * @throws IOException
 *             If an error occurs closing the input stream.
 */
//@Override
ECode InflaterInputStream::Close()
{
    if (!mClosed) {
        mInf->End();
        mClosed = TRUE;
        mEof = TRUE;
        mIn->Close();
    }
    return NOERROR;
}

/**
 * Marks the current position in the stream. This implementation overrides
 * the super type implementation to do nothing at all.
 *
 * @param readlimit
 *            of no use.
 */
//@Override
ECode InflaterInputStream::Mark(
    /* [in] */ Int32 readlimit)
{
    // do nothing
    return NOERROR;
}

/**
 * Reset the position of the stream to the last marked position. This
 * implementation overrides the supertype implementation and always throws
 * an {@link IOException IOException} when called.
 *
 * @throws IOException
 *             if the method is called
 */
//@Override
ECode InflaterInputStream::Reset()
{
//    throw new IOException();
    return E_IO_EXCEPTION;
}

/**
 * Returns whether the receiver implements {@code mark} semantics. This type
 * does not support {@code mark()}, so always responds {@code false}.
 *
 * @return false, always
 */
//@Override
ECode InflaterInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    *supported = FALSE;
    return NOERROR;
}

ECode InflaterInputStream::CheckClosed()
{
    if (mClosed) {
//        throw new IOException("Stream is closed");
        return E_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode InflaterInputStream::ReadBuffer(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    return ReadBufferEx(0, buffer->GetLength(), buffer, number);
}

/**
 * This is the most basic constructor. You only need to pass the {@code
 * InputStream} from which the compressed data is to be read from. Default
 * settings for the {@code Inflater} and internal buffer are be used. In
 * particular the Inflater expects a ZLIB header from the input stream.
 *
 * @param is
 *            the {@code InputStream} to read data from.
 */
ECode InflaterInputStream::Init(
    /* [in] */ IInputStream* is)
{
    AutoPtr<CInflater> inflater;
    CInflater::NewByFriend((CInflater**)&inflater);
    return Init(is, (IInflater*)inflater.Get(), BUF_SIZE);
}

/**
 * This constructor lets you pass a specifically initialized Inflater,
 * for example one that expects no ZLIB header.
 *
 * @param is
 *            the {@code InputStream} to read data from.
 * @param inflater
 *            the specific {@code Inflater} for uncompressing data.
 */
ECode InflaterInputStream::Init(
    /* [in] */ IInputStream* is,
    /* [in] */ IInflater* inflater)
{
    return Init(is, inflater, BUF_SIZE);
}

/**
 * This constructor lets you specify both the {@code Inflater} as well as
 * the internal buffer size to be used.
 *
 * @param is
 *            the {@code InputStream} to read data from.
 * @param inflater
 *            the specific {@code Inflater} for uncompressing data.
 * @param bsize
 *            the size to be used for the internal buffer.
 */
ECode InflaterInputStream::Init(
    /* [in] */ IInputStream* is,
    /* [in] */ IInflater* inflater,
    /* [in] */ Int32 bsize)
{
    mIn = is;
    if (is == NULL || inflater == NULL) {
//        throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    if (bsize <= 0) {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mInf = (CInflater*)inflater;
    // BEGIN android-only
    if (is->Probe(EIID_ZipFileRAFStream)) {
        mNativeEndBufSize = bsize;
    }
    else {
        mBuf = ArrayOf<Byte>::Alloc(bsize);
    }
    // END android-only

    return NOERROR;
}