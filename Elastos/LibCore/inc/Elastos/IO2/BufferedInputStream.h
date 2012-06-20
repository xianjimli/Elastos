#ifndef __BUFFEREDINPUTSTREAM_H__
#define __BUFFEREDINPUTSTREAM_H__

#include "FilterInputStream.h"
#include <elastos/AutoPtr.h>

class BufferedInputStream : public FilterInputStream
{
protected:
    BufferedInputStream();

    ~BufferedInputStream();

    CARAPI Init(
        /* [in] */ IInputStream* in,
        /* [in] */ Int32 size);

public:
    /**
     * Returns an estimated number of bytes that can be read or skipped without blocking for more
     * input. This method returns the number of bytes available in the buffer
     * plus those available in the source stream, but see {@link InputStream#available} for
     * important caveats.
     *
     * @return the estimated number of bytes available
     * @throws IOException if this stream is closed or an error occurs
     */
    // synchronized
    CARAPI Available(
        /* [out] */ Int32* number);

    /**
     * Closes this stream. The source stream is closed and any resources
     * associated with it are released.
     *
     * @throws IOException
     *             if an error occurs while closing this stream.
     */
    CARAPI Close();

    /**
     * Sets a mark position in this stream. The parameter {@code readlimit}
     * indicates how many bytes can be read before a mark is invalidated.
     * Calling {@code reset()} will reposition the stream back to the marked
     * position if {@code readlimit} has not been surpassed. The underlying
     * buffer may be increased in size to allow {@code readlimit} number of
     * bytes to be supported.
     *
     * @param readlimit
     *            the number of bytes that can be read before the mark is
     *            invalidated.
     * @see #reset()
     */
    // synchronized
    CARAPI Mark(
        /* [in] */ Int32 readLimit);

    /**
     * Indicates whether {@code BufferedInputStream} supports the {@code mark()}
     * and {@code reset()} methods.
     *
     * @return {@code true} for BufferedInputStreams.
     * @see #mark(int)
     * @see #reset()
     */
    CARAPI IsMarkSupported(
        /* [out] */ Boolean* supported);

    /**
     * Reads a single byte from this stream and returns it as an integer in the
     * range from 0 to 255. Returns -1 if the end of the source string has been
     * reached. If the internal buffer does not contain any available bytes then
     * it is filled from the source stream and the first byte is returned.
     *
     * @return the byte read or -1 if the end of the source stream has been
     *         reached.
     * @throws IOException
     *             if this stream is closed or another IOException occurs.
     */
    // synchronized
    CARAPI Read(
        /* [out] */ Int32* value);

    /**
     * Reads at most {@code length} bytes from this stream and stores them in
     * byte array {@code buffer} starting at offset {@code offset}. Returns the
     * number of bytes actually read or -1 if no bytes were read and the end of
     * the stream was encountered. If all the buffered bytes have been used, a
     * mark has not been set and the requested number of bytes is larger than
     * the receiver's buffer size, this implementation bypasses the buffer and
     * simply places the results directly into {@code buffer}.
     *
     * @param buffer
     *            the byte array in which to store the bytes read.
     * @param offset
     *            the initial position in {@code buffer} to store the bytes read
     *            from this stream.
     * @param length
     *            the maximum number of bytes to store in {@code buffer}.
     * @return the number of bytes actually read or -1 if end of stream.
     * @throws IndexOutOfBoundsException
     *             if {@code offset < 0} or {@code length < 0}, or if
     *             {@code offset + length} is greater than the size of
     *             {@code buffer}.
     * @throws IOException
     *             if the stream is already closed or another IOException
     *             occurs.
     */
    // synchronized
    CARAPI ReadBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ BufferOf<Byte>* buffer,
        /* [out] */ Int32* number);

    /**
     * Resets this stream to the last marked location.
     *
     * @throws IOException
     *             if this stream is closed, no mark has been set or the mark is
     *             no longer valid because more than {@code readlimit} bytes
     *             have been read since setting the mark.
     * @see #mark(int)
     */
    // synchronized
    CARAPI Reset();

    /**
     * Skips {@code amount} number of bytes in this stream. Subsequent
     * {@code read()}'s will not return these bytes unless {@code reset()} is
     * used.
     *
     * @param amount
     *            the number of bytes to skip. {@code skip} does nothing and
     *            returns 0 if {@code amount} is less than zero.
     * @return the number of bytes actually skipped.
     * @throws IOException
     *             if this stream is closed or another IOException occurs.
     */
    // synchronized
    CARAPI Skip(
        /* [in] */ Int64 count,
        /* [out] */ Int64* number);

private:
    CARAPI StreamClosed();

    CARAPI Fillbuf(
        /* [in] */ IInputStream* localIn,
        /* [in, out] */ BufferOf<Byte>** localBuf,
        /* [out] */ Int32* number);

protected:
    /**
     * The buffer containing the current bytes read from the target InputStream.
     */
    BufferOf<Byte>* mBuf;

    /**
     * The total number of bytes inside the byte array {@code buf}.
     */
    Int32 mCount;

    /**
     * The current limit, which when passed, invalidates the current mark.
     */
    Int32 mMarklimit;

    /**
     * The currently marked position. -1 indicates no mark has been set or the
     * mark has been invalidated.
     */
    Int32 mMarkpos;

    /**
     * The current position within the byte array {@code buf}.
     */
    Int32 mPos;

};

#endif //__BUFFEREDINPUTSTREAM_H__
