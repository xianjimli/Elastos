#ifndef _CLOB_H_
#define _CLOB_H_

#include <elastos.h>

using namespace Elastos;

class Clob {
public:
	/**
     * Gets the value of this {@code Clob} object as an ASCII stream.
     *
     * @return an ASCII {@code InputStream} giving access to the
     *            {@code Clob} data.
     * @throws SQLException
     *             if an error occurs accessing the {@code Clob}.
     */
    virtual CARAPI GetAsciiStream(
        /** [out] **/IInputStream** stream)=0;

    /**
     * Gets the data of this {@code Clob} object in a {@code java.io.Reader}.
     *
     * @return a character stream Reader object giving access to the {@code
     *         Clob} data.
     * @throws SQLException
     *             if an error occurs accessing the {@code Clob}.
     */
    virtual CARAPI GetCharacterStream(
        /** [out] **/IReader** stream)=0;

    /**
     * Gets a copy of a specified substring in this {@code Clob}.
     *
     * @param pos
     *            the index of the start of the substring in the {@code Clob}.
     * @param length
     *            the length of the data to retrieve.
     * @return A string containing the requested data.
     * @throws SQLException
     *             if an error occurs accessing the {@code Clob}.
     */
    virtual CARAPI String GetSubString(
        /** [in] **/Int64 pos, 
        /** [in] **/Int32 length,
        /** [out] **/String* str)=0;

    /**
     * Retrieves the number of characters in this {@code Clob} object.
     *
     * @return a long value with the number of character in this {@code Clob}.
     * @throws SQLException
     *             if an error occurs accessing the {@code Clob}.
     */
    virtual CARAPI Length(
        /** [out] **/Int64* len)=0;

    /**
     * Retrieves the character position at which a specified {@code Clob} object
     * appears in this {@code Clob} object.
     *
     * @param searchstr
     *            the specified {@code Clob} to search for.
     * @param start
     *            the position within this {@code Clob} to start the search
     * @return a long value with the position at which the specified {@code
     *         Clob} occurs within this {@code Clob}.
     * @throws SQLException
     *             if an error occurs accessing the {@code Clob}.
     */
    virtual CARAPI Position(
        /** [in] **/IClob* searchstr, 
        /** [in] **/Int64 start,
        /** [out] **/Int64** pos)=0;

    /**
     * Retrieves the character position at which a specified substring appears
     * in this {@code Clob} object.
     *
     * @param searchstr
     *            the string to search for.
     * @param start
     *            the position at which to start the search within this {@code
     *            Clob}.
     * @return a long value with the position at which the specified string
     *         occurs within this {@code Clob}.
     * @throws SQLException
     *             if an error occurs accessing the {@code Clob}.
     */
    virtual CARAPI PositionEx(
        /** [in] **/String searchstr, 
        /** [in] **/Int64 start,
        /** [out] **/Int64* pos)=0;

    /**
     * Retrieves a stream which can be used to write Ascii characters to this
     * {@code Clob} object, starting at specified position.
     *
     * @param pos
     *            the position at which to start the writing.
     * @return an OutputStream which can be used to write ASCII characters to
     *         this {@code Clob}.
     * @throws SQLException
     *             if an error occurs accessing the {@code Clob}.
     */
    virtual CARAPI SetAsciiStream(
        /** [in] **/Int64 pos,
        /** [out] **/IOutputStream** stream)=0;

    /**
     * Retrieves a stream which can be used to write a stream of unicode
     * characters to this {@code Clob} object, at a specified position.
     *
     * @param pos
     *            the position at which to start the writing.
     * @return a Writer which can be used to write unicode characters to this
     *         {@code Clob}.
     * @throws SQLException
     *             if an error occurs accessing the {@code Clob}.
     */
    virtual CARAPI SetCharacterStream(
        /** [in] **/Int64 pos,
        /** [out] **/IWriter** stream)=0;

    /**
     * Writes a given Java String to this {@code Clob} object at a specified
     * position.
     *
     * @param pos
     *            the position at which to start the writing.
     * @param str
     *            the string to write.
     * @return the number of characters written.
     * @throws SQLException
     *             if an error occurs accessing the {@code Clob}.
     */
    virtual CARAPI SetString(
        /** [in] **/Int64 pos, 
        /** [in] **/String str,
        /** [out] **/Int32* result)=0;

    /**
     * Writes {@code len} characters of a string, starting at a specified
     * character offset, to this {@code Clob}.
     *
     * @param pos
     *            the position at which to start the writing.
     * @param str
     *            the String to write.
     * @param offset
     *            the offset within {@code str} to start writing from.
     * @param len
     *            the number of characters to write.
     * @return the number of characters written.
     * @throws SQLException
     *             if an error occurs accessing the {@code Clob}.
     */
    virtual CARAPI SetString(
        /** [in] **/Int64 pos, 
        /** [in] **/String str, 
        /** [in] **/Int32 offset, 
        /** [in] **/Int32 len,
        /** [out] **/Int32* result)=0;

    /**
     * Truncates this {@code Clob} after the specified number of characters.
     *
     * @param len
     *            the length in characters giving the place to
     *            truncate this {@code Clob}.
     * @throws SQLException
     *             if an error occurs accessing the {@code Clob}.
     */
    virtual CARAPI Truncate(
        /** [in] **/Int64 len)=0;

    /**
     * Frees any resources held by this clob. After {@code free} is called, calling
     * method other than {@code free} will throw {@code SQLException} (calling {@code free}
     * repeatedly will do nothing).
     *
     * @throws SQLException
     */
    virtual CARAPI Free()=0;

    /**
     * Returns a {@link Reader} that reads {@code length} characters from this clob, starting
     * at 1-based offset {code pos}.
     */
    virtual CARAPI GetCharacterStream(
        /** [in] **/Int64 pos, 
        /** [in] **/Int64 length,
        /** [out] **/IReader** stream)=0;
};
#endif //_CLOB_H_
