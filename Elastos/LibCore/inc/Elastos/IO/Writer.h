#ifndef __WRITER_H__
#define __WRITER_H__

#include <elastos.h>
#include <Elastos.Core.h>
#include "Elastos.IO_server.h"
#include <elastos/Mutex.h>
#include <elastos/AutoPtr.h>

using namespace Elastos;
using namespace Elastos::Core::Threading;

class Writer
{
public:
    /**
     * Closes this writer. Implementations of this method should free any
     * resources associated with the writer.
     *
     * @throws IOException
     *             if an error occurs while closing this writer.
     */
    virtual CARAPI Close() = 0;

    /**
     * Flushes this writer. Implementations of this method should ensure that
     * all buffered characters are written to the target.
     *
     * @throws IOException
     *             if an error occurs while flushing this writer.
     */
    virtual CARAPI Flush() = 0;

    /**
     * Writes one character to the target. Only the two least significant bytes
     * of the integer {@code oneChar} are written.
     *
     * @param oneChar
     *            the character to write to the target.
     * @throws IOException
     *             if this writer is closed or another I/O error occurs.
     */
    virtual CARAPI Write(
        /* [in] */ Int32 oneChar32);

    /**
     * Writes the entire character buffer {@code buf} to the target.
     *
     * @param buf
     *            the non-null array containing characters to write.
     * @throws IOException
     *             if this writer is closed or another I/O error occurs.
     */
    virtual CARAPI WriteBuffer(
        /* [in] */ const ArrayOf<Char8>& buffer);

    /**
     * Writes {@code count} characters starting at {@code offset} in {@code buf}
     * to the target.
     *
     * @param buf
     *            the non-null character array to write.
     * @param offset
     *            the index of the first character in {@code buf} to write.
     * @param count
     *            the maximum number of characters to write.
     * @throws IndexOutOfBoundsException
     *             if {@code offset < 0} or {@code count < 0}, or if {@code
     *             offset + count} is greater than the size of {@code buf}.
     * @throws IOException
     *             if this writer is closed or another I/O error occurs.
     */
    virtual CARAPI WriteBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [in] */ const ArrayOf<Char8>& buffer) = 0;

    /**
     * Writes the characters from the specified string to the target.
     *
     * @param str
     *            the non-null string containing the characters to write.
     * @throws IOException
     *             if this writer is closed or another I/O error occurs.
     */
    virtual CARAPI WriteString(
        /* [in] */ CString str);

    /**
     * Writes {@code count} characters from {@code str} starting at {@code
     * offset} to the target.
     *
     * @param str
     *            the non-null string containing the characters to write.
     * @param offset
     *            the index of the first character in {@code str} to write.
     * @param count
     *            the number of characters from {@code str} to write.
     * @throws IOException
     *             if this writer is closed or another I/O error occurs.
     * @throws IndexOutOfBoundsException
     *             if {@code offset < 0} or {@code count < 0}, or if {@code
     *             offset + count} is greater than the length of {@code str}.
     */
    virtual CARAPI WriteStringEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [in] */ CString str);

    /**
     * Appends the character {@code c} to the target. This method works the same
     * way as {@link #write(int)}.
     *
     * @param c
     *            the character to append to the target stream.
     * @return this writer.
     * @throws IOException
     *             if this writer is closed or another I/O error occurs.
     */
    virtual CARAPI Append(
        /* [in] */ Char32 c);

    /**
     * Appends the character sequence {@code csq} to the target. This method
     * works the same way as {@code Writer.write(csq.toString())}. If {@code
     * csq} is {@code null}, then the string "null" is written to the target
     * stream.
     *
     * @param csq
     *            the character sequence appended to the target.
     * @return this writer.
     * @throws IOException
     *             if this writer is closed or another I/O error occurs.
     */
    virtual CARAPI AppendEx(
        /* [in] */ ICharSequence* csq);

     /**
     * Appends a subsequence of the character sequence {@code csq} to the
     * target. This method works the same way as {@code
     * Writer.writer(csq.subsequence(start, end).toString())}. If {@code
     * csq} is {@code null}, then the specified subsequence of the string "null"
     * will be written to the target.
     *
     * @param csq
     *            the character sequence appended to the target.
     * @param start
     *            the index of the first char in the character sequence appended
     *            to the target.
     * @param end
     *            the index of the character following the last character of the
     *            subsequence appended to the target.
     * @return this writer.
     * @throws IOException
     *             if this writer is closed or another I/O error occurs.
     * @throws IndexOutOfBoundsException
     *             if {@code start > end}, {@code start < 0}, {@code end < 0} or
     *             either {@code start} or {@code end} are greater or equal than
     *             the length of {@code csq}.
     */
    virtual CARAPI AppendEx2(
        /* [in] */ ICharSequence* csq,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    /**
     * Returns true if this writer has encountered and suppressed an error. Used
     * by PrintWriters as an alternative to checked exceptions.
     */
    virtual CARAPI CheckError(
        /* [out] */ Boolean* hasError);

protected:
    /**
     * Constructs a new {@code Writer} with {@code this} as the object used to
     * synchronize critical sections.
     */
    Writer();

    /**
     * Constructs a new {@code Writer} with {@code lock} used to synchronize
     * critical sections.
     *
     * @param lock
     *            the {@code Object} used to synchronize critical sections.
     * @throws NullPointerException
     *             if {@code lock} is {@code null}.
     */
    Writer(
        /* [in] */ Mutex* lock);

    CARAPI Init(
        /* [in] */ Mutex* lock);

    virtual ~Writer();

    virtual CARAPI_(Mutex*) GetSelfLock() = 0;

protected:
    static const CString TOKEN_NULL;

    Mutex* mLock;
};

#endif //__WRITER_H__