#ifndef __STRINGWRITER_H__
#define __STRINGWRITER_H__

#include "Writer.h"
#include <Elastos.IO_server.h>

using namespace Elastos;

class StringWriter : public Writer
{
protected:
    StringWriter();

    ~StringWriter();

    /**
     * Constructs a new {@code StringWriter} which has a {@link StringBuffer}
     * allocated with the default size of 16 characters. The {@code
     * StringBuffer} is also the {@code lock} used to synchronize access to this
     * writer.
     */
    CARAPI Init();

    /**
     * Constructs a new {@code StringWriter} which has a {@link StringBuffer}
     * allocated with a size of {@code initialSize} characters. The {@code
     * StringBuffer} is also the {@code lock} used to synchronize access to this
     * writer.
     *
     * @param initialSize
     *            the intial size of the target string buffer.
     */
    CARAPI Init(
        /* [in] */ Int32 initialSize);
public:
    /**
     * Calling this method has no effect. In contrast to most {@code Writer} subclasses,
     * the other methods in {@code StringWriter} do not throw an {@code IOException} if
     * {@code close()} has been called.
     *
     * @throws IOException
     *             if an error occurs while closing this writer.
     */
    CARAPI Close();

    /**
     * Calling this method has no effect.
     */
    CARAPI Flush();

    /**
     * Gets a reference to this writer's internal {@link StringBuffer}. Any
     * changes made to the returned buffer are reflected in this writer.
     *
     * @return a reference to this writer's internal {@code StringBuffer}.
     */
//    CARAPI GetBuffer(
//        /* [out] */ StringBuff* buf);

    /**
     * Gets a copy of the contents of this writer as a string.
     *
     * @return this writer's contents as a string.
     */
    CARAPI ToString(
        /* [out] */ String* str);

    /**
     * Writes one character to this writer's {@code StringBuffer}. Only the two
     * least significant bytes of the integer {@code oneChar} are written.
     *
     * @param oneChar
     *            the character to write to this writer's {@code StringBuffer}.
     */
    CARAPI Write(
        /* [in] */ Int32 oneChar8);

    /**
     * Writes {@code count} characters starting at {@code offset} in {@code buf}
     * to this writer's {@code StringBuffer}.
     *
     * @param chars
     *            the non-null character array to write.
     * @param offset
     *            the index of the first character in {@code chars} to write.
     * @param count
     *            the maximum number of characters to write.
     * @throws IndexOutOfBoundsException
     *             if {@code offset < 0} or {@code count < 0}, or if {@code
     *             offset + count} is greater than the size of {@code buf}.
     */
    CARAPI WriteBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [in] */ const ArrayOf<Char8>& buffer);

    /**
     * Writes the characters from the specified string to this writer's {@code
     * StringBuffer}.
     *
     * @param str
     *            the non-null string containing the characters to write.
     */
    CARAPI WriteString(
        /* [in] */ CString &str);

    /**
     * Writes {@code count} characters from {@code str} starting at {@code
     * offset} to this writer's {@code StringBuffer}.
     *
     * @param str
     *            the non-null string containing the characters to write.
     * @param offset
     *            the index of the first character in {@code str} to write.
     * @param count
     *            the number of characters from {@code str} to write.
     * @throws StringIndexOutOfBoundsException
     *             if {@code offset < 0} or {@code count < 0}, or if {@code
     *             offset + count} is greater than the length of {@code str}.
     */
    CARAPI WriteStringEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [in] */ CString &str);

    /**
     * Appends the character {@code c} to this writer's {@code StringBuffer}.
     * This method works the same way as {@link #write(int)}.
     *
     * @param c
     *            the character to append to the target stream.
     * @return this writer.
     */
    CARAPI Append(
        /* [in] */ Char8 c);

    /**
     * Appends the character sequence {@code csq} to this writer's {@code
     * StringBuffer}. This method works the same way as {@code
     * StringWriter.write(csq.toString())}. If {@code csq} is {@code null}, then
     * the string "null" is written to the target stream.
     *
     * @param csq
     *            the character sequence appended to the target.
     * @return this writer.
     */
    CARAPI AppendEx(
        /* [in] */ ICharSequence* csq);

    /**
     * Appends a subsequence of the character sequence {@code csq} to this
     * writer's {@code StringBuffer}. This method works the same way as {@code
     * StringWriter.writer(csq.subsequence(start, end).toString())}. If {@code
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
     * @throws IndexOutOfBoundsException
     *             if {@code start > end}, {@code start < 0}, {@code end < 0} or
     *             either {@code start} or {@code end} are greater or equal than
     *             the length of {@code csq}.
     */
    CARAPI AppendEx2(
        /* [in] */ ICharSequence* csq,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);
private:
    StringBuf  *mBuf;
};

#endif //__STRINGWRITER_H__