#ifndef __FILTERINPUTSTREAM_H__
#define __FILTERINPUTSTREAM_H__

#include "InputStream.h"
#include <elastos/AutoPtr.h>

/**
 * Wraps an existing {@link InputStream} and performs some transformation on
 * the input data while it is being read. Transformations can be anything from a
 * simple byte-wise filtering input data to an on-the-fly compression or
 * decompression of the underlying stream. Input streams that wrap another input
 * stream and provide some additional functionality on top of it usually inherit
 * from this class.
 *
 * @see FilterOutputStream
 */
class FilterInputStream : public InputStream
{
protected:
    FilterInputStream();

    ~FilterInputStream();

    /**
     * Constructs a new {@code FilterInputStream} with the specified input
     * stream as source.
     *
     * <p><strong>Warning:</strong> passing a null source creates an invalid
     * {@code FilterInputStream}, that fails on every method that is not
     * overridden. Subclasses should check for null in their constructors.
     *
     * @param in the input stream to filter reads on.
     */
    CARAPI Init(
        /* [in] */ IInputStream* in);

public:
    CARAPI Available(
        /* [out] */ Int32* number);

    /**
     * Closes this stream. This implementation closes the filtered stream.
     *
     * @throws IOException
     *             if an error occurs while closing this stream.
     */
    CARAPI Close();

    /**
     * Sets a mark position in this stream. The parameter {@code readlimit}
     * indicates how many bytes can be read before the mark is invalidated.
     * Sending {@code reset()} will reposition this stream back to the marked
     * position, provided that {@code readlimit} has not been surpassed.
     * <p>
     * This implementation sets a mark in the filtered stream.
     *
     * @param readlimit
     *            the number of bytes that can be read from this stream before
     *            the mark is invalidated.
     * @see #markSupported()
     * @see #reset()
     */
    // synchronized
    CARAPI Mark(
        /* [in] */ Int32 readLimit);

    /**
     * Indicates whether this stream supports {@code mark()} and {@code reset()}.
     * This implementation returns whether or not the filtered stream supports
     * marking.
     *
     * @return {@code true} if {@code mark()} and {@code reset()} are supported,
     *         {@code false} otherwise.
     * @see #mark(int)
     * @see #reset()
     * @see #skip(long)
     */
    CARAPI IsMarkSupported(
        /* [out] */ Boolean* supported);

    /**
     * Reads a single byte from the filtered stream and returns it as an integer
     * in the range from 0 to 255. Returns -1 if the end of this stream has been
     * reached.
     *
     * @return the byte read or -1 if the end of the filtered stream has been
     *         reached.
     * @throws IOException
     *             if the stream is closed or another IOException occurs.
     */
    CARAPI Read(
        /* [out] */ Int32* value);

    /**
     * Reads bytes from this stream and stores them in the byte array
     * {@code buffer}. Returns the number of bytes actually read or -1 if no
     * bytes were read and the end of this stream was encountered. This
     * implementation reads bytes from the filtered stream.
     *
     * @param buffer
     *            the byte array in which to store the read bytes.
     * @return the number of bytes actually read or -1 if the end of the
     *         filtered stream has been reached while reading.
     * @throws IOException
     *             if this stream is closed or another IOException occurs.
     *
     */
    CARAPI ReadBuffer(
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [out] */ Int32* number);

    /**
     * Reads at most {@code count} bytes from this stream and stores them in the
     * byte array {@code buffer} starting at {@code offset}. Returns the number
     * of bytes actually read or -1 if no bytes have been read and the end of
     * this stream has been reached. This implementation reads bytes from the
     * filtered stream.
     *
     * @param buffer
     *            the byte array in which to store the bytes read.
     * @param offset
     *            the initial position in {@code buffer} to store the bytes
     *            read from this stream.
     * @param count
     *            the maximum number of bytes to store in {@code buffer}.
     * @return the number of bytes actually read or -1 if the end of the
     *         filtered stream has been reached while reading.
     * @throws IOException
     *             if this stream is closed or another I/O error occurs.
     */
    CARAPI ReadBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [out] */ Int32* number);

    /**
     * Resets this stream to the last marked location. This implementation
     * resets the target stream.
     *
     * @throws IOException
     *             if this stream is already closed, no mark has been set or the
     *             mark is no longer valid because more than {@code readlimit}
     *             bytes have been read since setting the mark.
     * @see #mark(int)
     * @see #markSupported()
     */
    // synchronized
    CARAPI Reset();

    /**
     * Skips {@code count} number of bytes in this stream. Subsequent
     * {@code read()}'s will not return these bytes unless {@code reset()} is
     * used. This implementation skips {@code count} number of bytes in the
     * filtered stream.
     *
     * @param count
     *            the number of bytes to skip.
     * @return the number of bytes actually skipped.
     * @throws IOException
     *             if this stream is closed or another IOException occurs.
     * @see #mark(int)
     * @see #reset()
     */
    CARAPI Skip(
        /* [in] */ Int64 count,
        /* [out] */ Int64* number);

protected:
    /**
     * The source input stream that is filtered.
     */
    AutoPtr<IInputStream> mIn;
};

#endif
