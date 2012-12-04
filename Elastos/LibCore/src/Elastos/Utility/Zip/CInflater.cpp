
#include "CInflater.h"
#include "cmdef.h"
#include <elastos/Math.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
using namespace Elastos::Core;

CInflater::CInflater()
    : mFinished(FALSE)
    , mNeedsDictionary(FALSE)
    , mStreamHandle(NULL)
{}

CInflater::~CInflater()
{
//    try {
    End();
//    } finally {
//        try {
//            super.finalize();
//        } catch (Throwable t) {
//            throw new AssertionError(t);
//        }
//    }
}

/* Create a new stream . This stream cannot be used until it has been properly initialized. */
ECode CInflater::CreateStream(
    /* [in] */ Boolean noHeader)
{
    NativeZipStream* stream = new NativeZipStream;
    if (stream == NULL) {
//        jniThrowOutOfMemoryError(env, NULL);
        mStreamHandle = NULL;
        return E_OUT_OF_MEMORY_ERROR;
    }
    stream->mStream.adler = 1;

    /*
     * In the range 8..15 for checked, or -8..-15 for unchecked inflate. Unchecked
     * is appropriate for formats like zip that do their own validity checking.
     */
    /* Window bits to use. 15 is fastest but consumes the most memory */
    Int32 wbits = 15;               /*Use MAX for fastest */
    if (noHeader) {
        wbits = wbits / -1;
    }
    Int32 err = inflateInit2(&stream->mStream, wbits);
    if (err != Z_OK) {
//        throwExceptionForZlibError(env, "java/lang/IllegalArgumentException", err);
        delete stream;
        mStreamHandle = NULL;
        return E_ZIP_STREAM_STATE_EXCEPTION;
    }
    mStreamHandle = stream;
    return NOERROR;
}

/**
 * Release any resources associated with this {@code Inflater}. Any unused
 * input/output is discarded. This is also called by the finalize method.
 */
// synchronized
ECode CInflater::End()
{
    Mutex::Autolock lock(&_m_syncLock);

    if (mStreamHandle != NULL) {
        EndImplLocked(mStreamHandle);
        mInRead = 0;
        mInLength = 0;
        mStreamHandle = NULL;
    }

    return NOERROR;
}

void CInflater::EndImplLocked(
    /* [in] */ NativeZipStream* stream)
{
    inflateEnd(&stream->mStream);
    delete stream;
}

/**
 * Indicates if the {@code Inflater} has inflated the entire deflated
 * stream. If deflated bytes remain and {@code needsInput()} returns {@code
 * true} this method will return {@code false}. This method should be
 * called after all deflated input is supplied to the {@code Inflater}.
 *
 * @return {@code true} if all input has been inflated, {@code false}
 *         otherwise.
 */
// synchronized
ECode CInflater::Finished(
    /* [out] */ Boolean* finished)
{
    VALIDATE_NOT_NULL(finished);

    Mutex::Autolock lock(&_m_syncLock);

    *finished = mFinished;
    return NOERROR;
}

/**
 * Returns the <i>Adler32</i> checksum of either all bytes inflated, or the
 * checksum of the preset dictionary if one has been supplied.
 *
 * @return The <i>Adler32</i> checksum associated with this
 *         {@code Inflater}.
 */
// synchronized
ECode CInflater::GetAdler(
    /* [out] */ Int32* checksum)
{
    VALIDATE_NOT_NULL(checksum);

    Mutex::Autolock lock(&_m_syncLock);

    if (mStreamHandle == NULL) {
//        throw new IllegalStateException();
        return E_NULL_POINTER_EXCEPTION;
    }
    *checksum = GetAdlerImplLocked(mStreamHandle);

    return NOERROR;
}

Int32 CInflater::GetAdlerImplLocked(
    /* [in] */ NativeZipStream* stream)
{
    return stream->mStream.adler;
}

/**
 * Returns the total number of bytes read by the {@code Inflater}. This
 * method performs the same as {@code getTotalIn()} except that it returns a
 * {@code long} value instead of an integer.
 *
 * @return the total number of bytes read.
 */
// synchronized
ECode CInflater::GetBytesRead(
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
 * Returns a the total number of bytes read by the {@code Inflater}. This
 * method performs the same as {@code getTotalOut} except it returns a
 * {@code long} value instead of an integer.
 *
 * @return the total bytes written to the output buffer.
 */
// synchronized
ECode CInflater::GetBytesWritten(
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);

    Mutex::Autolock lock(&_m_syncLock);

    // Throw NPE here
    if (mStreamHandle == NULL) {
//        throw new NullPointerException();
        return E_ZIP_STREAM_STATE_EXCEPTION;
    }
    *number = GetTotalOutImplLocked(mStreamHandle);

    return NOERROR;

}

/**
 * Returns the number of bytes of current input remaining to be read by the
 * inflater.
 *
 * @return the number of bytes of unread input.
 */
// synchronized
ECode CInflater::GetRemaining(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    Mutex::Autolock lock(&_m_syncLock);

    *number = mInLength - mInRead;

    return NOERROR;
}

/**
 * Returns total number of bytes of input read by the {@code Inflater}. The
 * result value is limited by {@code Integer.MAX_VALUE}.
 *
 * @return the total number of bytes read.
 */
// synchronized
ECode CInflater::GetTotalIn(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    Mutex::Autolock lock(&_m_syncLock);

    if (mStreamHandle == NULL) {
//        throw new IllegalStateException();
        return E_ZIP_STREAM_STATE_EXCEPTION;
    }
    Int64 totalIn = GetTotalInImplLocked(mStreamHandle);
    *number = (totalIn <= Math::INT32_MAX_VALUE ? (Int32)totalIn
            : Math::INT32_MAX_VALUE);
    return NOERROR;
}

Int64 CInflater::GetTotalInImplLocked(
    /* [in] */ NativeZipStream* stream)
{
    return stream->mStream.total_in;
}

/**
 * Returns total number of bytes written to the output buffer by the {@code
 * Inflater}. The result value is limited by {@code Integer.MAX_VALUE}.
 *
 * @return the total bytes of output data written.
 */
// synchronized
ECode CInflater::GetTotalOut(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    Mutex::Autolock lock(&_m_syncLock);

    if (mStreamHandle == NULL) {
//        throw new IllegalStateException();
        return E_ZIP_STREAM_STATE_EXCEPTION;
    }
    Int64 totalOut = GetTotalOutImplLocked(mStreamHandle);
    *number = (totalOut <= Math::INT32_MAX_VALUE ? (Int32)totalOut
            : Math::INT32_MAX_VALUE);
    return NOERROR;
}

Int64 CInflater::GetTotalOutImplLocked(
    /* [in] */ NativeZipStream* stream)
{
    return stream->mStream.total_out;
}

/**
 * Inflates bytes from current input and stores them in {@code buf}.
 *
 * @param buf
 *            the buffer where decompressed data bytes are written.
 * @return the number of bytes inflated.
 * @throws DataFormatException
 *             if the underlying stream is corrupted or was not compressed
 *             using a {@code Deflater}.
 */
ECode CInflater::Inflate(
    /* [out] */ ArrayOf<Byte>* buf,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buf);
    VALIDATE_NOT_NULL(number);

    return InflateEx(0, buf->GetLength(), buf, number);
}

/**
 * Inflates up to n bytes from the current input and stores them in {@code
 * buf} starting at {@code off}.
 *
 * @param buf
 *            the buffer to write inflated bytes to.
 * @param off
 *            the offset in buffer where to start writing decompressed data.
 * @param nbytes
 *            the number of inflated bytes to write to {@code buf}.
 * @throws DataFormatException
 *             if the underlying stream is corrupted or was not compressed
 *             using a {@code Deflater}.
 * @return the number of bytes inflated.
 */
// synchronized
ECode CInflater::InflateEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 nbytes,
    /* [out] */ ArrayOf<Byte>* buf,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buf);
    VALIDATE_NOT_NULL(number);

    Mutex::Autolock lock(&_m_syncLock);

    // avoid int overflow, check null buf
    if (off > buf->GetLength() || nbytes < 0 || off < 0
            || buf->GetLength() - off < nbytes) {
//        throw new ArrayIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    if (nbytes == 0) {
        *number = 0;
        return NOERROR;
    }

    if (mStreamHandle == NULL) {
//        throw new IllegalStateException();
        return E_ZIP_STREAM_STATE_EXCEPTION;
    }

    Boolean need;
    NeedsInput(&need);
    if (need) {
        *number = 0;
        return NOERROR;
    }

    Boolean neededDict = mNeedsDictionary;
    mNeedsDictionary = FALSE;
    Int32 result = InflateImplLocked(off, nbytes, buf, mStreamHandle);
    if (mNeedsDictionary && neededDict) {
//        throw new DataFormatException("Needs dictionary");
        return E_DATA_FORMAT_EXCEPTION;
    }

    *number = result;
    return NOERROR;
}

Int64 CInflater::InflateImplLocked(
    /* [in] */ Int32 off,
    /* [in] */ Int32 nbytes,
    /* [out] */ ArrayOf<Byte>* buf,
    /* [in] */ NativeZipStream* stream)
{
    if (buf == NULL) {
        return -1;
    }
    stream->mStream.next_out = reinterpret_cast<Bytef*>(buf->GetPayload() + off);
    stream->mStream.avail_out = nbytes;
    //printf("mStream avail_in is %d, avail_out is %d\n", stream->mStream.avail_in, stream->mStream.avail_out);
   
    Bytef* initialNextIn = stream->mStream.next_in;
    Bytef* initialNextOut = stream->mStream.next_out;
    
    //printf("stream is %x %x %x %x\n",  initialNextIn[0], initialNextIn[1], initialNextIn[2],initialNextIn[3]);    
    //Int32 err = inflate(initialNextIn + 3, Z_SYNC_FLUSH);
    Int32 err = inflate(&stream->mStream, 0);
    if (err != Z_OK) {
        if (err == Z_STREAM_ERROR) {
            return 0;
        }
        if (err == Z_STREAM_END) {
            mFinished = TRUE;
        }
        else if (err == Z_NEED_DICT) {
            mNeedsDictionary = TRUE;
        }
        else {
//            throwExceptionForZlibError(env, "java/util/zip/DataFormatException", err);
            return -1;
        }
    }

    Int32 bytesRead = stream->mStream.next_in - initialNextIn;
    Int32 bytesWritten = stream->mStream.next_out - initialNextOut;

    mInRead += bytesRead;
    return bytesWritten;
}

/**
 * Indicates whether the input bytes were compressed with a preset
 * dictionary. This method should be called prior to {@code inflate()} to
 * determine whether a dictionary is required. If so {@code setDictionary()}
 * should be called with the appropriate dictionary prior to calling {@code
 * inflate()}.
 *
 * @return {@code true} if a preset dictionary is required for inflation.
 * @see #setDictionary(byte[])
 * @see #setDictionary(byte[], int, int)
 */
// synchronized
ECode CInflater::NeedsDictionary(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    
    Mutex::Autolock lock(&_m_syncLock);

    *result = mNeedsDictionary;
    return NOERROR;
}

/**
 * Indicates that input has to be passed to the inflater.
 *
 * @return {@code true} if {@code setInput} has to be called before
 *         inflation can proceed.
 * @see #setInput(byte[])
 */
// synchronized
ECode CInflater::NeedsInput(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

//    Mutex::Autolock lock(&_m_syncLock);

    *result = mInRead == mInLength;
    return NOERROR;
}

/**
 * Resets the {@code Inflater}. Should be called prior to inflating a new
 * set of data.
 */
// synchronized
ECode CInflater::Reset()
{
    Mutex::Autolock lock(&_m_syncLock);

    if (mStreamHandle == NULL) {
//        throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    mFinished = FALSE;
    mNeedsDictionary = FALSE;
    mInLength = mInRead = 0;
    return ResetImplLocked(mStreamHandle);
}

ECode CInflater::ResetImplLocked(
    /* [in] */ NativeZipStream* stream)
{
    Int32 err = inflateReset(&stream->mStream);
    if (err != Z_OK) {
//        throwExceptionForZlibError(env, "java/lang/IllegalArgumentException", err);
        return E_ZIP_STREAM_STATE_EXCEPTION;
    }
    return NOERROR;
}

/**
 * Sets the preset dictionary to be used for inflation to {@code buf}.
 * {@code needsDictionary()} can be called to determine whether the current
 * input was deflated using a preset dictionary.
 *
 * @param buf
 *            The buffer containing the dictionary bytes.
 * @see #needsDictionary
 */
ECode CInflater::SetDictionary(
    /* [in] */ const ArrayOf<Byte>& buf)
{
    return SetDictionaryEx(buf, 0, buf.GetLength());
}

/**
 * Like {@code setDictionary(byte[])}, allowing to define a specific region
 * inside {@code buf} to be used as a dictionary.
 * <p>
 * The dictionary should be set if the {@link #inflate(byte[])} returned
 * zero bytes inflated and {@link #needsDictionary()} returns
 * <code>true</code>.
 *
 * @param buf
 *            the buffer containing the dictionary data bytes.
 * @param off
 *            the offset of the data.
 * @param nbytes
 *            the length of the data.
 * @see #needsDictionary
 */
// synchronized
ECode CInflater::SetDictionaryEx(
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

void CInflater::SetDictionaryImplLocked(
    /* [in] */ const ArrayOf<Byte>& buf,
    /* [in] */ Int32 off,
    /* [in] */ Int32 nbytes,
    /* [in] */ NativeZipStream* stream)
{
    stream->SetDictionary(buf, off, nbytes, TRUE);
}

/**
 * Sets the current input to to be decrompressed. This method should only be
 * called if {@code needsInput()} returns {@code true}.
 *
 * @param buf
 *            the input buffer.
 * @see #needsInput
 */
ECode CInflater::SetInput(
    /* [in] */ const ArrayOf<Byte>& buf)
{
    return SetInputEx(buf, 0, buf.GetLength());
}

/**
 * Sets the current input to the region of the input buffer starting at
 * {@code off} and ending at {@code nbytes - 1} where data is written after
 * decompression. This method should only be called if {@code needsInput()}
 * returns {@code true}.
 *
 * @param buf
 *            the input buffer.
 * @param off
 *            the offset to read from the input buffer.
 * @param nbytes
 *            the number of bytes to read.
 * @see #needsInput
 */
// synchronized
ECode CInflater::SetInputEx(
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
        mInRead = 0;
        mInLength = nbytes;
        SetInputImplLocked(buf, off, nbytes, mStreamHandle);
        return NOERROR;
    }
    else {
//        throw new ArrayIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
}

// android-only
/**
 * Sets the current input to the region within a file starting at {@code
 * off} and ending at {@code nbytes - 1}. This method should only be called
 * if {@code needsInput()} returns {@code true}.
 *
 * @param fd
 *            the input file.
 * @param off
 *            the offset to read from in buffer.
 * @param nbytes
 *            the number of bytes to read.
 * @see #needsInput
 */
// synchronized
Int32 CInflater::SetFileInput(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int64 off,
    /* [in] */ Int32 nbytes)
{
    Mutex::Autolock lock(&_m_syncLock);

    if (mStreamHandle == NULL) {
//        throw new IllegalStateException();
        return E_ZIP_STREAM_STATE_EXCEPTION;
    }
    mInRead = 0;
    mInLength = SetFileInputImplLocked(fd, off, nbytes, mStreamHandle);
    return mInLength;
}

void CInflater::SetInputImplLocked(
    /* [in] */ const ArrayOf<Byte>& buf,
    /* [in] */ Int32 off,
    /* [in] */ Int32 nbytes,
    /* [in] */ NativeZipStream* stream)
{
    stream->SetInput(const_cast<ArrayOf<Byte>*>(&buf), off, nbytes);
}

// BEGIN android-only
Int32 CInflater::SetFileInputImplLocked(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int64 off,
    /* [in] */ Int32 nbytes,
    /* [in] */ NativeZipStream* stream)
{
    // We reuse the existing native buffer if it's large enough.
    // TODO: benchmark.
    if (stream->mInCap < nbytes) {
        stream->SetInput(NULL, 0, nbytes);
    }
    else {
        stream->mStream.next_in = reinterpret_cast<Bytef*>(stream->mInput->GetPayload());
        stream->mStream.avail_in = nbytes;
    }

    // As an Android-specific optimization, we read directly onto the native heap.
    // The original code used Java to read onto the Java heap and then called setInput(byte[]).
    // TODO: benchmark.
    Int32 _fd;
    fd->GetDescriptor(&_fd);
    Int32 rc = TEMP_FAILURE_RETRY(lseek(_fd, off, SEEK_SET));
    if (rc == -1) {
//        jniThrowIOException(env, errno);
        return 0;
    }
    Int32 totalByteCount = 0;
    Bytef* dst = reinterpret_cast<Bytef*>(stream->mInput->GetPayload());
    ssize_t byteCount;
    while ((byteCount = TEMP_FAILURE_RETRY(read(_fd, dst, nbytes))) > 0) {
        dst += byteCount;
        nbytes -= byteCount;
        totalByteCount += byteCount;
    }
    if (byteCount == -1) {
//        jniThrowIOException(env, errno);
        return 0;
    }
    return totalByteCount;
}

/**
 * This constructor creates an inflater that expects a header from the input
 * stream. Use {@code Inflater(boolean)} if the input comes without a ZLIB
 * header.
 */
ECode CInflater::constructor()
{
    return constructor(FALSE);
}

/**
 * This constructor allows to create an inflater that expects no header from
 * the input stream.
 *
 * @param noHeader
 *            {@code true} indicates that no ZLIB header comes with the
 *            input.
 */
ECode CInflater::constructor(
    /* [in] */ Boolean onHeader)
{
    return CreateStream(onHeader);
}
