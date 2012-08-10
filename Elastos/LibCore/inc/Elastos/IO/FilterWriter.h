#ifndef __FILTERWRITER_H__
#define __FILTERWRITER_H__

#include "Writer.h"

class FilterWriter : public Writer
{
protected:
    FilterWriter();

    ~FilterWriter();

    /**
     * Constructs a new FilterWriter on the Writer {@code out}. All writes are
     * now filtered through this writer.
     *
     * @param out
     *            the target Writer to filter writes on.
     */
    CARAPI Init(
        /* [in] */ IWriter* out);

    /**
     * Closes this writer. This implementation closes the target writer.
     *
     * @throws IOException
     *             if an error occurs attempting to close this writer.
     */
    CARAPI Close();

    /**
     * Flushes this writer to ensure all pending data is sent out to the target
     * writer. This implementation flushes the target writer.
     *
     * @throws IOException
     *             if an error occurs attempting to flush this writer.
     */
    CARAPI Flush();

    /**
     * Writes {@code count} characters from the char array {@code buffer}
     * starting at position {@code offset} to the target writer.
     *
     * @param buffer
     *            the buffer to write.
     * @param offset
     *            the index of the first character in {@code buffer} to write.
     * @param count
     *            the number of characters in {@code buffer} to write.
     * @throws IOException
     *             if an error occurs while writing to this writer.
     */
    CARAPI WriteBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [in] */ const ArrayOf<Char8>& buffer);

    /**
     * Writes the specified character {@code oneChar} to the target writer. Only the
     * two least significant bytes of the integer {@code oneChar} are written.
     *
     * @param oneChar
     *            the char to write to the target writer.
     * @throws IOException
     *             if an error occurs while writing to this writer.
     */
    CARAPI Write(
        /* [in] */ Int32 oneChar32);

    /**
     * Writes {@code count} characters from the string {@code str} starting at
     * position {@code index} to this writer. This implementation writes
     * {@code str} to the target writer.
     *
     * @param str
     *            the string to be written.
     * @param offset
     *            the index of the first character in {@code str} to write.
     * @param count
     *            the number of chars in {@code str} to write.
     * @throws IOException
     *             if an error occurs while writing to this writer.
     */
    CARAPI WriteStringEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [in] */ CString str);
protected:
    CARAPI CloseLocked();

    CARAPI FlushLocked();

    CARAPI WriteBufferExLocked(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [in] */ const ArrayOf<Char8>& buffer);

    CARAPI WriteLocked(
        /* [in] */ Int32 oneChar32);

    CARAPI WriteStringExLocked(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [in] */ CString str);
protected:
     /**
     * The Writer being filtered.
     */
     AutoPtr<IWriter> mOut;
};

#endif //__FILTERWRITER_H__