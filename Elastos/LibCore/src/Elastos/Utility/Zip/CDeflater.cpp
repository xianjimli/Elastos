
#include "CDeflater.h"
#include "cmdef.h"


const Int32 CDeflater::NO_FLUSH;
const Int32 CDeflater::SYNC_FLUSH;
const Int32 CDeflater::FULL_FLUSH;
const Int32 CDeflater::FINISH;
ArrayOf<Byte>* CDeflater::STUB_INPUT_BUFFER = ArrayOf<Byte>::Alloc(0);

CDeflater::CDeflater()
    : mFlushParm(NO_FLUSH)
    , mFinished(FALSE)
    , mCompressLevel(IDeflater_DEFAULT_COMPRESSION)
    , mStrategy(IDeflater_DEFAULT_STRATEGY)
    , mStreamHandle(NULL)
    , mInputBuffer(NULL)
{}

CDeflater::~CDeflater()
{
    ArrayOf<Byte>::Free(mInputBuffer);
//    try {
    End(); // to allow overriding classes to clean up
//    endImpl(); // in case those classes don't call super.end()
//    } finally {
//        try {
//            super.finalize();
//        } catch (Throwable t) {
//            throw new AssertionError(t);
//        }
//    }
}

/**
 * Deflates the data (previously passed to {@code setInput}) into the
 * supplied buffer.
 *
 * @param buf
 *            buffer to write compressed data to.
 * @return number of bytes of compressed data written to {@code buf}.
 * @see #deflate(byte[], int, int)
 */
ECode CDeflater::Deflate(
    /* [out] */ ArrayOf<Byte>* buf,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buf);
    VALIDATE_NOT_NULL(number);

    return DeflateEx(0, buf->GetLength(), buf, number);
}

/**
 * Deflates data (previously passed to {@code setInput}) into a specific
 * region within the supplied buffer.
 *
 * @param buf
 *            the buffer to write compressed data to.
 * @param off
 *            the offset within {@code buf} at which to start writing to.
 * @param nbytes
 *            maximum number of bytes of compressed data to be written.
 * @return the number of bytes of compressed data written to {@code buf}.
 */
// synchronized
ECode CDeflater::DeflateEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 nbytes,
    /* [out] */ ArrayOf<Byte>* buf,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buf);
    VALIDATE_NOT_NULL(number);

    Mutex::Autolock lock(&_m_syncLock);

    return DeflateImplLocked(off, nbytes, mFlushParm, buf, number);
}

/**
 * Deflates data (previously passed to {@code setInput}) into a specific
 * region within the supplied buffer, optionally flushing the input buffer.
 *
 * @param buf the buffer to write compressed data to.
 * @param off the offset within {@code buf} at which to start writing to.
 * @param nbytes maximum number of bytes of compressed data to be written.
 * @param flush one of {@link #NO_FLUSH}, {@link #SYNC_FLUSH} or
 *      {@link #FULL_FLUSH}.
 * @return the number of compressed bytes written to {@code buf}. If this
 *      equals {@code nbytes}, the number of bytes of input to be flushed
 *      may have exceeded the output buffer's capacity. In this case,
 *      finishing a flush will require the output buffer to be drained
 *      and additional calls to {@link #deflate} to be made.
 * @hide
 * @since 1.7
 */
// synchronized
ECode CDeflater::DeflateEx2(
    /* [in] */ Int32 off,
    /* [in] */ Int32 nbytes,
    /* [in] */ Int32 flush,
    /* [out] */ ArrayOf<Byte>* buf,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buf);
    VALIDATE_NOT_NULL(number);

    Mutex::Autolock lock(&_m_syncLock);

    if (flush != NO_FLUSH && flush != SYNC_FLUSH && flush != FULL_FLUSH) {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return DeflateImplLocked(off, nbytes, flush, buf, number);
}

ECode CDeflater::DeflateImplLocked(
    /* [in] */ Int32 off,
    /* [in] */ Int32 nbytes,
    /* [in] */ Int32 flush,
    /* [out] */ ArrayOf<Byte>* buf,
    /* [out] */ Int32* number)
{
    if (mStreamHandle == NULL) {
//        throw new IllegalStateException();
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    if (off > buf->GetLength() || nbytes < 0 || off < 0 ||
        buf->GetLength() - off < nbytes) {
//        throw new ArrayIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (mInputBuffer == NULL) {
        SetInput(*STUB_INPUT_BUFFER);
    }
    *number = DeflateImplLocked(off, nbytes, flush, mStreamHandle, buf);
    return NOERROR;
}

Int32 CDeflater::DeflateImplLocked(
    /* [in] */ Int32 off,
    /* [in] */ Int32 nbytes,
    /* [in] */ Int32 flushParm,
    /* [in] */ NativeZipStream* stream,
    /* [out] */ ArrayOf<Byte>* buf)
{
    /* We need to get the number of bytes already read */
    Int32 inBytes = mInRead;

    stream->mStream.avail_out = nbytes;
    Int32 sin = stream->mStream.total_in;
    Int32 sout = stream->mStream.total_out;
    if (buf == NULL) {
        return -1;
    }
    stream->mStream.next_out = reinterpret_cast<Bytef*>(buf->GetPayload() + off);
    Int32 err = deflate(&stream->mStream, flushParm);
    if (err != Z_OK) {
        if (err == Z_MEM_ERROR) {
//            jniThrowOutOfMemoryError(env, NULL);
            return 0;
        }
        if (err == Z_STREAM_END) {
            mFinished = TRUE;
            return stream->mStream.total_out - sout;
        }
    }
    if (flushParm != Z_FINISH) {
        /* Need to update the number of input bytes read. */
        mInRead = (Int32)stream->mStream.total_in - sin + inBytes;
    }
    return stream->mStream.total_out - sout;
}

void CDeflater::EndImplLocked(
    /* [in] */ NativeZipStream* stream)
{
    deflateEnd(&stream->mStream);
    delete stream;
}

/**
 * Frees all resources held onto by this deflating algorithm. Any unused
 * input or output is discarded. While this method is used by {@code
 * finalize()}, it can be called explicitly in order to free native
 * resources before the next GC cycle. After {@code end()} was called other
 * methods will typically throw an {@code IllegalStateException}.
 */
// synchronized
ECode CDeflater::End()
{
    Mutex::Autolock lock(&_m_syncLock);

    EndImplLocked();
    return NOERROR;
}

void CDeflater::EndImplLocked()
{
    if (mStreamHandle != NULL) {
        EndImplLocked(mStreamHandle);
        ArrayOf<Byte>::Free(mInputBuffer);
        mStreamHandle = NULL;
    }
}

/**
 * Indicates to the {@code Deflater} that all uncompressed input has been provided
 * to it.
 *
 * @see #finished
 */
// synchronized
ECode CDeflater::Finish()
{
    Mutex::Autolock lock(&_m_syncLock);

    mFlushParm = FINISH;
    return NOERROR;
}

/**
 * Returns whether or not all provided data has been successfully
 * compressed.
 *
 * @return true if all data has been compressed, false otherwise.
 */
// synchronized
ECode CDeflater::Finished(
    /* [out] */ Boolean* finished)
{
    VALIDATE_NOT_NULL(finished);

    Mutex::Autolock lock(&_m_syncLock);
    *finished = mFinished;
    return NOERROR;
}

/**
 * Returns the Adler32 checksum of uncompressed data currently read. If a
 * preset dictionary is used getAdler() will return the Adler32 checksum of
 * the dictionary used.
 *
 * @return the Adler32 checksum of uncompressed data or preset dictionary if
 *         used.
 * @see #setDictionary(byte[])
 * @see #setDictionary(byte[], int, int)
 */
// synchronized
ECode CDeflater::GetAdler(
    /* [out] */ Int32* checksum)
{
    VALIDATE_NOT_NULL(checksum);

    Mutex::Autolock lock(&_m_syncLock);

    if (mStreamHandle == NULL) {
//        throw new IllegalStateException();
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    *checksum = GetAdlerImplLocked(mStreamHandle);
    return NOERROR;
}

Int32 CDeflater::GetAdlerImplLocked(
    /* [in] */ NativeZipStream* stream)
{
    return stream->mStream.adler;
}

/**
 * Returns the total number of bytes of input consumed by the {@code Deflater}.
 *
 * @return number of bytes of input read.
 */
// synchronized
ECode CDeflater::GetTotalIn(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    Mutex::Autolock lock(&_m_syncLock);

    if (mStreamHandle == NULL) {
//        throw new IllegalStateException();
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    *number = (Int32)GetTotalInImplLocked(mStreamHandle);
    return NOERROR;
}

Int64 CDeflater::GetTotalInImplLocked(
    /* [in] */ NativeZipStream* stream)
{
    return stream->mStream.total_in;
}

/**
 * Returns the total number of compressed bytes output by this {@code Deflater}.
 *
 * @return number of compressed bytes output.
 */
// synchronized
ECode CDeflater::GetTotalOut(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    Mutex::Autolock lock(&_m_syncLock);

    if (mStreamHandle == NULL) {
//        throw new IllegalStateException();
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    *number = (Int32)GetTotalOutImplLocked(mStreamHandle);
    return NOERROR;
}

Int64 CDeflater::GetTotalOutImplLocked(
    /* [in] */ NativeZipStream* stream)
{
    return stream->mStream.total_out;
}

/**
 * Counterpart to setInput(). Indicates whether or not all bytes of
 * uncompressed input have been consumed by the {@code Deflater}. If needsInput()
 * returns true setInput() must be called before deflation can continue. If
 * all bytes of uncompressed data have been provided to the {@code Deflater}
 * finish() must be called to ensure the compressed data is output.
 *
 * @return {@code true} if input is required for deflation to continue,
 *         {@code false} otherwise.
 * @see #finished()
 * @see #setInput(byte[])
 * @see #setInput(byte[], int, int)
 */
// synchronized
ECode CDeflater::NeedsInput(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    Mutex::Autolock lock(&_m_syncLock);

    if (mInputBuffer == NULL) {
        *result = TRUE;
        return NOERROR;
    }
    *result = mInRead == mInLength;
    return NOERROR;
}

/**
 * Resets the {@code Deflater} to accept new input without affecting any
 * previously made settings for the compression strategy or level. This
 * operation <i>must</i> be called after {@code finished()} returns
 * {@code true} if the {@code Deflater} is to be reused.
 *
 * @see #finished
 */
// synchronized
ECode CDeflater::Reset()
{
    Mutex::Autolock lock(&_m_syncLock);

    if (mStreamHandle == NULL) {
//        throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }

    mFlushParm = NO_FLUSH;
    mFinished = FALSE;
    ArrayOf<Byte>::Free(mInputBuffer);
    mInputBuffer = NULL;
    return ResetImplLocked(mStreamHandle);
}

ECode CDeflater::ResetImplLocked(
    /* [in] */ NativeZipStream* stream)
{
    Int32 err = deflateReset(&stream->mStream);
    if (err != Z_OK) {
//        throwExceptionForZlibError(env, "java/lang/IllegalArgumentException", err);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

/**
 * Sets the dictionary to be used for compression by this {@code Deflater}.
 * setDictionary() can only be called if this {@code Deflater} supports the writing
 * of ZLIB headers. This is the default behaviour but can be overridden
 * using {@code Deflater(int, boolean)}.
 *
 * @param buf
 *            the buffer containing the dictionary data bytes.
 * @see Deflater#Deflater(int, boolean)
 */
// synchronized
ECode CDeflater::SetDictionary(
    /* [in] */ const ArrayOf<Byte>& buf)
{
    return SetDictionaryEx(buf, 0, buf.GetLength());
}

/**
 * Sets the dictionary to be used for compression by this {@code Deflater}.
 * setDictionary() can only be called if this {@code Deflater} supports the writing
 * of ZLIB headers. This is the default behaviour but can be overridden
 * using {@code Deflater(int, boolean)}.
 *
 * @param buf
 *            the buffer containing the dictionary data bytes.
 * @param off
 *            the offset of the data.
 * @param nbytes
 *            the length of the data.
 * @see Deflater#Deflater(int, boolean)
 */
// synchronized
ECode CDeflater::SetDictionaryEx(
    /* [in] */ const ArrayOf<Byte>& buf,
    /* [in] */ Int32 off,
    /* [in] */ Int32 nbytes)
{
    Mutex::Autolock lock(&_m_syncLock);

    if (mStreamHandle == NULL) {
//        throw new IllegalStateException();
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    // avoid int overflow, check null buf
    if (off <= buf.GetLength() && nbytes >= 0 && off >= 0
            && buf.GetLength() - off >= nbytes) {
        SetDictionaryImplLocked(buf, off, nbytes, mStreamHandle);
        return NOERROR;
    }
    else {
//        throw new ArrayIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
}

void CDeflater::SetDictionaryImplLocked(
    /* [in] */ const ArrayOf<Byte>& buf,
    /* [in] */ Int32 off,
    /* [in] */ Int32 nbytes,
    /* [in] */ NativeZipStream* stream)
{
    stream->SetDictionary(buf, off, nbytes, FALSE);
}

/**
 * Sets the input buffer the {@code Deflater} will use to extract uncompressed bytes
 * for later compression.
 *
 * @param buf
 *            the buffer.
 */
ECode CDeflater::SetInput(
    /* [in] */ const ArrayOf<Byte>& buf)
{
    return SetInputEx(buf, 0, buf.GetLength());
}

/* Sets the input buffer the {@code Deflater} will use to extract uncompressed bytes
 * for later compression. Input will be taken from the buffer region
 * starting at off and ending at nbytes - 1.
 *
 * @param buf
 *            the buffer containing the input data bytes.
 * @param off
 *            the offset of the data.
 * @param nbytes
 *            the length of the data.
 */
// synchronized
ECode CDeflater::SetInputEx(
    /* [in] */ const ArrayOf<Byte>& buf,
    /* [in] */ Int32 off,
    /* [in] */ Int32 nbytes)
{
    Mutex::Autolock lock(&_m_syncLock);

    if (mStreamHandle == NULL) {
//        throw new IllegalStateException();
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    // avoid int overflow, check null buf
    if (off <= buf.GetLength() && nbytes >= 0 && off >= 0
            && buf.GetLength() - off >= nbytes) {
        mInLength = nbytes;
        mInRead = 0;
        if (mInputBuffer == NULL) {
            FAIL_RETURN(SetLevelsImplLocked(mCompressLevel,
                    mStrategy, mStreamHandle));
        }
        ArrayOf<Byte>::Free(mInputBuffer);
        mInputBuffer = buf.Clone();
        SetInputImplLocked(buf, off, nbytes, mStreamHandle);
        return NOERROR;
    }
    else {
//        throw new ArrayIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
}

ECode CDeflater::SetLevelsImplLocked(
    /* [in] */ Int32 level,
    /* [in] */ Int32 strategy,
    /* [in] */ NativeZipStream* stream)
{
    if (stream == NULL) {
//        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    Byte b = 0;
    stream->mStream.next_out = reinterpret_cast<Bytef*>(&b);
    Int32 err = deflateParams(&stream->mStream, level, strategy);
    if (err != Z_OK) {
//        throwExceptionForZlibError(env, "java/lang/IllegalStateException", err);
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return NOERROR;
}

void CDeflater::SetInputImplLocked(
    /* [in] */ const ArrayOf<Byte>& buf,
    /* [in] */ Int32 off,
    /* [in] */ Int32 nbytes,
    /* [in] */ NativeZipStream* stream)
{
    stream->SetInput(const_cast<ArrayOf<Byte>*>(&buf), off, nbytes);
}

/**
 * Sets the compression level to be used when compressing data. The
 * compression level must be a value between 0 and 9. This value must be set
 * prior to calling setInput().
 *
 * @param level
 *            compression level to use
 * @exception IllegalArgumentException
 *                If the compression level is invalid.
 */
// synchronized
ECode CDeflater::SetLevel(
    /* [in] */ Int32 level)
{
    Mutex::Autolock lock(&_m_syncLock);

    if (level < IDeflater_DEFAULT_COMPRESSION ||
        level > IDeflater_BEST_COMPRESSION) {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (mInputBuffer != NULL) {
//        throw new IllegalStateException();
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    mCompressLevel = level;
    return NOERROR;
}

/**
 * Sets the compression strategy to be used. The strategy must be one of
 * FILTERED, HUFFMAN_ONLY or DEFAULT_STRATEGY.This value must be set prior
 * to calling setInput().
 *
 * @param strategy
 *            compression strategy to use
 * @exception IllegalArgumentException
 *                If the strategy specified is not one of FILTERED,
 *                HUFFMAN_ONLY or DEFAULT_STRATEGY.
 */
// synchronized
ECode CDeflater::SetStrategy(
    /* [in] */ Int32 strategy)
{
    Mutex::Autolock lock(&_m_syncLock);

    if (strategy < IDeflater_DEFAULT_STRATEGY ||
        strategy > IDeflater_HUFFMAN_ONLY) {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (mInputBuffer != NULL) {
//        throw new IllegalStateException();
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    mStrategy = strategy;
    return NOERROR;
}

/**
 * Returns a long int of total number of bytes read by the {@code Deflater}. This
 * method performs the same as {@code getTotalIn} except it returns a long value
 * instead of an integer
 *
 * @see #getTotalIn()
 * @return total number of bytes read by {@code Deflater}.
 */
// synchronized
ECode CDeflater::GetBytesRead(
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);

    Mutex::Autolock lock(&_m_syncLock);

    // Throw NPE here
    if (mStreamHandle == NULL) {
//        throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    *number = GetTotalInImplLocked(mStreamHandle);
    return NOERROR;
}

/**
 * Returns a long int of total number of bytes of read by the {@code Deflater}. This
 * method performs the same as {@code getTotalOut} except it returns a long
 * value instead of an integer
 *
 * @see #getTotalOut()
 * @return bytes exactly write by {@code Deflater}
 */
// synchronized
ECode CDeflater::GetBytesWritten(
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);

    Mutex::Autolock lock(&_m_syncLock);

    // Throw NPE here
    if (mStreamHandle == NULL) {
//       throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    *number = GetTotalOutImplLocked(mStreamHandle);
    return NOERROR;
}

ECode CDeflater::CreateStream(
    /* [in] */ Int32 level,
    /* [in] */ Int32 strategy,
    /* [in] */ Boolean noHeader)
{
    NativeZipStream* stream = new NativeZipStream;
    if (stream == NULL) {
        mStreamHandle = NULL;
        return E_OUT_OF_MEMORY_ERROR;
    }

    Int32 wbits = 12; // Was 15, made it 12 to reduce memory consumption. Use MAX for fastest.
    Int32 mlevel = 5; // Was 9, made it 5 to reduce memory consumption. Might result
                  // in out-of-memory problems according to some web pages. The
                  // ZLIB docs are a bit vague, unfortunately. The default
                  // results in 2 x 128K being allocated per Deflater, which is
                  // not acceptable.
    /*Unable to find official doc that this is the way to avoid zlib header use. However doc in zipsup.c claims it is so */
    if (noHeader) {
        wbits = wbits / -1;
    }
    Int32 err = deflateInit2(&stream->mStream, level, Z_DEFLATED, wbits, mlevel, strategy);
    if (err != Z_OK) {
        delete stream;
        mStreamHandle = NULL;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mStreamHandle = stream;
    return NOERROR;
}

/**
 * Constructs a new {@code Deflater} instance with default compression
 * level. The strategy can be specified with {@link #setStrategy}, only. A
 * header is added to the output by default; use constructor {@code
 * Deflater(level, boolean)} if you need to omit the header.
 */
ECode CDeflater::constructor()
{
    return constructor(IDeflater_DEFAULT_COMPRESSION, FALSE);
}

/**
 * Constructs a new {@code Deflater} instance with a specific compression
 * level. The strategy can be specified with {@code setStrategy}, only. A
 * header is added to the output by default; use
 * {@code Deflater(level, boolean)} if you need to omit the header.
 *
 * @param level
 *            the compression level in the range between 0 and 9.
 */
ECode CDeflater::constructor(
    /* [in] */ Int32 level)
{
    return constructor(level, FALSE);
}

/**
 * Constructs a new {@code Deflater} instance with a specific compression
 * level. If noHeader is passed as true no ZLib header is added to the
 * output. In a ZIP archive every entry (compressed file) comes with such a
 * header. The strategy can be specified with the setStrategy method, only.
 *
 * @param level
 *            the compression level in the range between 0 and 9.
 * @param noHeader
 *            {@code true} indicates that no ZLIB header should be written.
 */
ECode CDeflater::constructor(
    /* [in] */ Int32 level,
    /* [in] */ Boolean noHeader)
{
    if (level < IDeflater_DEFAULT_COMPRESSION ||
        level > IDeflater_BEST_COMPRESSION) {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mCompressLevel = level;
    return CreateStream(mCompressLevel, mStrategy, noHeader);
}