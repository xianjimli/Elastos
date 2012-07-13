
#ifndef __BUFFEREDWRITER_H__
#define __BUFFEREDWRITER_H__

#include "Writer.h"
#include <elastos/AutoPtr.h>


class BufferedWriter: public Writer
{
public:

    BufferedWriter();

    ~BufferedWriter();

    /**
     * Constructs a new {@code BufferedWriter}, providing {@code out} with a buffer
     * of 8192 bytes.
     *
     * @param out the {@code Writer} the buffer writes to.
     */
    CARAPI Init(
        /*[in]*/ IWriter* wout);

    /**
     * Constructs a new {@code BufferedWriter}, providing {@code out} with {@code size} bytes
     * of buffer.
     *
     * @param out the {@code OutputStream} the buffer writes to.
     * @param size the size of buffer in bytes.
     * @throws IllegalArgumentException if {@code size <= 0}.
     */
    CARAPI Init(
        /*[in]*/ IWriter* wout,
        /*[in]*/ Int32 size);

    /**
     * Closes this writer. The contents of the buffer are flushed, the target
     * writer is closed, and the buffer is released. Only the first invocation
     * of close has any effect.
     *
     * @throws IOException
     *             if an error occurs while closing this writer.
     */
    //@Override
    CARAPI Close();

    /**
     * Flushes this writer. The contents of the buffer are committed to the
     * target writer and it is then flushed.
     *
     * @throws IOException
     *             if an error occurs while flushing this writer.
     */
    // @Override
    CARAPI Flush();

    /**
     * Writes a newline to this writer. A newline is determined by the System
     * property "line.separator". The target writer may or may not be flushed
     * when a newline is written.
     *
     * @throws IOException
     *             if an error occurs attempting to write to this writer.
     */
    virtual CARAPI NewLine();

    /**
     * Writes the character {@code oneChar} to this writer. If the buffer
     * gets full by writing this character, this writer is flushed. Only the
     * lower two bytes of the integer {@code oneChar} are written.
     *
     * @param oneChar
     *            the character to write.
     * @throws IOException
     *             if this writer is closed or another I/O error occurs.
     */
    //@Override
    CARAPI Write(
        /*[in]*/ Int32 oneChar);

    /**
     * Writes {@code count} characters starting at {@code offset} in
     * {@code cbuf} to this writer. If {@code count} is greater than this
     * writer's buffer, then the buffer is flushed and the characters are
     * written directly to the target writer.
     *
     * @param cbuf
     *            the array containing characters to write.
     * @param offset
     *            the start position in {@code cbuf} for retrieving characters.
     * @param count
     *            the maximum number of characters to write.
     * @throws IndexOutOfBoundsException
     *             if {@code offset < 0} or {@code count < 0}, or if
     *             {@code offset + count} is greater than the size of
     *             {@code cbuf}.
     * @throws IOException
     *             if this writer is closed or another I/O error occurs.
     */
    //@Override
    CARAPI WriteBufferEx(
        /*[in]*/ Int32 offset,
        /*[in]*/ Int32 count,
        /*[in]*/ const ArrayOf<Char8>& cbuf);

    /**
     * Writes {@code count} characters starting at {@code offset} in {@code str}
     * to this writer. If {@code count} is greater than this writer's buffer,
     * then this writer is flushed and the remaining characters are written
     * directly to the target writer. If count is negative no characters are
     * written to the buffer. This differs from the behavior of the superclass.
     *
     * @param str
     *            the non-null String containing characters to write.
     * @param offset
     *            the start position in {@code str} for retrieving characters.
     * @param count
     *            maximum number of characters to write.
     * @throws IOException
     *             if this writer has already been closed or another I/O error
     *             occurs.
     * @throws IndexOutOfBoundsException
     *             if {@code offset < 0} or {@code offset + count} is greater
     *             than the length of {@code str}.
     */
    // @Override
    CARAPI WriteStringEx(
        /*[in]*/ Int32 offset,
        /*[in]*/ Int32 count,
        /*[in]*/ CString str);

private:
    CARAPI CheckNotClosed();

    CARAPI FlushInternal();

    /**
     * Indicates whether this writer is closed.
     *
     * @return {@code true} if this writer is closed, {@code false} otherwise.
     */
    CARAPI_(Boolean) IsClosed();

private:
    AutoPtr<IWriter> mOut;

    ArrayOf<Char8>* mBuf;

    Int32 mPos;

    static const CString mLineSeparator;/////////////////////////////////////////

};
#endif //BUFFEREDWRITER_H