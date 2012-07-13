#ifndef __LINENUMBERINPUTSTREAM_H__
#define __LINENUMBERINPUTSTREAM_H__

#include "FilterInputStream.h"
#include "cmdef.h"

class LineNumberInputStream : public FilterInputStream
{
protected:
    LineNumberInputStream();

    ~LineNumberInputStream();

    /**
     * Constructs a new {@code LineNumberInputStream} on the {@link InputStream}
     * {@code in}. Line numbers are counted for all data read from this stream.
     *
     * <p><strong>Warning:</strong> passing a null source creates an invalid
     * {@code LineNumberInputStream}. All operations on such a stream will fail.
     *
     * @param in
     *            The non-null input stream to count line numbers.
     */
    CARAPI Init(
        /* [in] */ IInputStream* in);

public:
    /**
     * {@inheritDoc}
     *
     * <p>Note that the source stream may just be a sequence of {@code "\r\n"} bytes
     * which are converted into {@code '\n'} by this stream. Therefore,
     * {@code available} returns only {@code in.available() / 2} bytes as
     * result.
     */
    CARAPI Available(
        /* [out] */ Int32* number);

    /**
     * Returns the current line number for this stream. Numbering starts at 0.
     *
     * @return the current line number.
     */
    virtual CARAPI GetLineNumber(
        /* [out] */ Int32* lineNumber);

    /**
     * Sets a mark position in this stream. The parameter {@code readlimit}
     * indicates how many bytes can be read before the mark is invalidated.
     * Sending {@code reset()} will reposition this stream back to the marked
     * position, provided that {@code readlimit} has not been surpassed.
     * The line number count will also be reset to the last marked
     * line number count.
     * <p>
     * This implementation sets a mark in the filtered stream.
     *
     * @param readlimit
     *            the number of bytes that can be read from this stream before
     *            the mark is invalidated.
     * @see #markSupported()
     * @see #reset()
     */
    CARAPI Mark(
        /* [in] */ Int32 readLimit);

    /**
     * Reads a single byte from the filtered stream and returns it as an integer
     * in the range from 0 to 255. Returns -1 if the end of this stream has been
     * reached.
     * <p>
     * The line number count is incremented if a line terminator is encountered.
     * Recognized line terminator sequences are {@code '\r'}, {@code '\n'} and
     * {@code "\r\n"}. Line terminator sequences are always translated into
     * {@code '\n'}.
     *
     * @return the byte read or -1 if the end of the filtered stream has been
     *         reached.
     * @throws IOException
     *             if the stream is closed or another IOException occurs.
     */
    CARAPI Read(
        /* [out] */ Int32* value);

    /**
     * Reads at most {@code length} bytes from the filtered stream and stores
     * them in the byte array {@code buffer} starting at {@code offset}.
     * Returns the number of bytes actually read or -1 if no bytes have been
     * read and the end of this stream has been reached.
     * <p>
     * The line number count is incremented if a line terminator is encountered.
     * Recognized line terminator sequences are {@code '\r'}, {@code '\n'} and
     * {@code "\r\n"}. Line terminator sequences are always translated into
     * {@code '\n'}.
     *
     * @param buffer
     *            the array in which to store the bytes read.
     * @param offset
     *            the initial position in {@code buffer} to store the bytes read
     *            from this stream.
     * @param length
     *            the maximum number of bytes to store in {@code buffer}.
     * @return the number of bytes actually read or -1 if the end of the
     *         filtered stream has been reached while reading.
     * @throws IndexOutOfBoundsException
     *             if {@code offset < 0} or {@code length < 0}, or if
     *             {@code offset + length} is greater than the length of
     *             {@code buffer}.
     * @throws IOException
     *             if this stream is closed or another IOException occurs.
     * @throws NullPointerException
     *             if {@code buffer} is {@code null}.
     */
    CARAPI ReadBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [out] */ Int32* number);

    /**
     * Resets this stream to the last marked location. It also resets the line
     * count to what is was when this stream was marked.
     *
     * @throws IOException
     *             if this stream is already closed, no mark has been set or the
     *             mark is no longer valid because more than {@code readlimit}
     *             bytes have been read since setting the mark.
     * @see #mark(int)
     * @see #markSupported()
     */
    CARAPI Reset();

    /**
     * Sets the line number of this stream to the specified
     * {@code lineNumber}. Note that this may have side effects on the
     * line number associated with the last marked position.
     *
     * @param lineNumber
     *            the new lineNumber value.
     * @see #mark(int)
     * @see #reset()
     */
    virtual CARAPI SetLineNumber(
        /* [in] */ Int32 lineNumber);

    /**
     * Skips {@code count} number of bytes in this stream. Subsequent
     * {@code read()}'s will not return these bytes unless {@code reset()} is
     * used. This implementation skips {@code count} number of bytes in the
     * filtered stream and increments the line number count whenever line
     * terminator sequences are skipped.
     *
     * @param count
     *            the number of bytes to skip.
     * @return the number of bytes actually skipped.
     * @throws IOException
     *             if this stream is closed or another IOException occurs.
     * @see #mark(int)
     * @see #read()
     * @see #reset()
     */
    CARAPI Skip(
        /* [in] */ Int64 count,
        /* [out] */ Int64* number);

private:
    Int32 mLineNumber;

    Int32 mMarkedLineNumber;

    Int32 mLastChar;

    Int32 mMarkedLastChar;
};

#endif //__LINENUMBERINPUTSTREAM_H__