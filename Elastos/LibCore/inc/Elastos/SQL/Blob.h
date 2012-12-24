#ifdef _BLOB_H_
#define _BLOB_H_

#include <elastos.h>

using namespace Elastos;

class Blob {
public:
	/**
     * Retrieves this {@code Blob} object as a binary stream.
     *
     * @return a binary {@code InputStream} giving access to the {@code Blob}
     *         data.
     * @throws SQLException
     *             if an error occurs accessing the {@code Blob}.
     */
    virtual CARAPI  GetBinaryStream(
        /** [out] **/ IInputStream** stream)=0;

    /**
     * Retrieves {@code length} bytes from this {@code Blob}, starting at 1-based
     * offset {@code pos}, and returns them as a binary stream.
     *
     * @return a binary {@code InputStream} giving access to the {@code Blob}
     *         data.
     * @throws SQLException
     *             if an error occurs accessing the {@code Blob}.
     */
    virtual CARAPI  GetBinaryStreamEx(
        /** [in] **/Int64 pos, 
        /** [in] **/Int64 length,
        /** [out] **/ IInputStream** stream)=0;

    /**
     * Gets a portion of the value of this {@code Blob} as an array of bytes.
     *
     * @param pos
     *            the position of the first byte in the {@code Blob} to get,
     *            where the first byte in the {@code Blob} has position 1.
     * @param length
     *            the number of bytes to get.
     * @return a byte array containing the data from the {@code Blob}, starting
     *         at {@code pos} and is up to {@code length} bytes long.
     * @throws SQLException
     *             if an error occurs accessing the {@code Blob}.
     */
    virtual CARAPI  GetBytes(
        /** [in] **/Int64 pos, 
        /** [in] **/Int32 length,
        /** [out,callee] **/ArrayOf<Byte>* bytes)=0;

    /**
     * Gets the number of bytes in this {@code Blob} object.
     *
     * @return a {@code long} value with the length of the {@code Blob} in
     *         bytes.
     * @throws SQLException
     *             if an error occurs accessing the {@code Blob}.
     */
    virtual CARAPI  Length(
        /** [out] **/ Int64* len)=0;

    /**
     * Search for the position in this {@code Blob} at which a specified pattern
     * begins, starting at a specified position within the {@code Blob}.
     *
     * @param pattern
     *            a {@code Blob} containing the pattern of data to search for in
     *            this {@code Blob}.
     * @param start
     *            the position within this {@code Blob} to start the search,
     *            where the first position in the {@code Blob} is {@code 1}.
     * @return a {@code long} value with the position at which the pattern
     *         begins. Returns {@code -1} if the pattern is not found in this
     *         {@code Blob}.
     * @throws SQLException
     *             if an error occurs accessing the {@code Blob}.
     */
    virtual CARAPI  Position(
        /** [in] **/IBlob* pattern, 
        /** [in] **/Int64 start,
        /** [out] **/ Int64* position)=0;

    /**
     * Search for the position in this {@code Blob} at which the specified
     * pattern begins, starting at a specified position within the {@code Blob}.
     *
     * @param pattern
     *            a byte array containing the pattern of data to search for in
     *            this {@code Blob}.
     * @param start
     *            the position within this {@code Blob} to start the search,
     *            where the first position in the {@code Blob} is {@code 1}.
     * @return a {@code long} value with the position at which the pattern
     *         begins. Returns {@code -1} if the pattern is not found in this
     *         {@code Blob}.
     * @throws SQLException
     *             if an error occurs accessing the {@code Blob}.
     */
    virtual CARAPI  PositionEx(
        /** [in] **/ArrayOf<Byte> pattern, 
        /** [in] **/Int64 start,
        /** [out] **/ Int64* position)=0;

    /**
     * Gets a stream that can be used to write binary data to this {@code Blob}.
     *
     * @param pos
     *            the position within this {@code Blob} at which to start
     *            writing, where the first position in the {@code Blob} is
     *            {@code 1}.
     * @return a binary {@code InputStream} which can be used to write data into
     *         the {@code Blob} starting at the specified position.
     * @throws SQLException
     *             if an error occurs accessing the {@code Blob}.
     */
    virtual CARAPI  SetBinaryStream(
        /** [in] **/Int64 pos,
        /** [out] **/IOutputStream** stream)=0;

    /**
     * Writes a specified array of bytes to this {@code Blob} object, starting
     * at a specified position. Returns the number of bytes written.
     *
     * @param pos
     *            the position within this {@code Blob} at which to start
     *            writing, where the first position in the {@code Blob} is
     *            {@code 1}.
     * @param theBytes
     *            an array of bytes to write into the {@code Blob}.
     * @return an integer containing the number of bytes written to the {@code
     *         Blob}.
     * @throws SQLException
     *             if an error occurs accessing the {@code Blob}.
     */
    virtual CARAPI  SetBytes(
        /** [in] **/Int64 pos, 
        /** [in] **/ArrayOf<Byte> theBytes,
        /** [out] **/Int32* bytes)=0;

    /**
     * Writes a portion of a specified byte array to this {@code Blob}. Returns
     * the number of bytes written.
     *
     * @param pos
     *            the position within this {@code Blob} at which to start
     *            writing, where the first position in the {@code Blob} is
     *            {@code 1}.
     * @param theBytes
     *            an array of bytes to write into the {@code Blob}.
     * @param offset
     *            the offset into the byte array from which to start writing
     *            data - the first byte in the array has offset {@code 0}.
     * @param len
     *            the length of data to write in number of bytes.
     * @return an integer containing the number of bytes written to the {@code
     *         Blob}.
     * @throws SQLException
     *             if an error occurs accessing the {@code Blob}.
     */
    virtual CARAPI  SetBytesEx(
        /** [in] **/Int64 pos, 
        /** [in] **/ArrayOf<Byte> theBytes, 
        /** [in] **/Int32 offset, 
        /** [in] **/Int32 len,
        /** [out] **/Int32* bytes)=0;

    /**
     * Truncate the value of this {@code Blob} object to a specified length in
     * bytes.
     *
     * @param len
     *            the length of data in bytes after which this {@code Blob}
     *            is to be truncated.
     * @throws SQLException
     *             if an error occurs accessing the {@code Blob}.
     */
    virtual CARAPI  Truncate(
        /** [out] **/Int64* len)=0;

    /**
     * Frees any resources held by this blob. After {@code free} is called, calling
     * method other than {@code free} will throw {@code SQLException} (calling {@code free}
     * repeatedly will do nothing).
     *
     * @throws SQLException
     */
    virtual CARAPI  Free()=0;
};
#endif //_BLOB_H_