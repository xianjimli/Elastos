#ifndef __BYTEARRAYOUTPUTSTREAM_H__
#define __BYTEARRAYOUTPUTSTREAM_H__

#include "OutputStream.h"

class ByteArrayOutputStream : public OutputStream
{
public:
    ByteArrayOutputStream();

    ~ByteArrayOutputStream();

    CARAPI Init(
        /* [in] */ Int32 size);

    /**
     * Closes this stream. This releases system resources used for this stream.
     *
     * @throws IOException
     *             if an error occurs while attempting to close this stream.
     */
    CARAPI Close();

    /**
     * Resets this stream to the beginning of the underlying byte array. All
     * subsequent writes will overwrite any bytes previously stored in this
     * stream.
     */
    // synchronized
    CARAPI Reset();

    /**
     * Returns the total number of bytes written to this stream so far.
     *
     * @return the number of bytes written to this stream.
     */
    CARAPI GetSize(
        /* [out] */ Int32* size);

    /**
     * Returns the contents of this ByteArrayOutputStream as a byte array. Any
     * changes made to the receiver after returning will not be reflected in the
     * byte array returned to the caller.
     *
     * @return this stream's current contents as a byte array.
     */
    // synchronized
    CARAPI ToByteArray(
        /* [out] */ ArrayOf<Byte>** bytes);

    /**
     * Returns the contents of this ByteArrayOutputStream as a string. Any
     * changes made to the receiver after returning will not be reflected in the
     * string returned to the caller.
     *
     * @return this stream's current contents as a string.
     */
    CARAPI ToString(
        /* [out] */ String* str);

    /**
     * Returns the contents of this ByteArrayOutputStream as a string converted
     * according to the encoding declared in {@code enc}.
     *
     * @param enc
     *            a string representing the encoding to use when translating
     *            this stream to a string.
     * @return this stream's current contents as an encoded string.
     * @throws UnsupportedEncodingException
     *             if the provided encoding is not supported.
     */
    CARAPI ToStringEx(
        /* [in] */ const String& enc,
        /* [out] */ String* str);

    /**
     * Writes the specified byte {@code oneByte} to the OutputStream. Only the
     * low order byte of {@code oneByte} is written.
     *
     * @param oneByte
     *            the byte to be written.
     */
    // synchronized
    CARAPI Write(
        /* [in] */ Int32 oneByte);

    /**
     * Writes {@code count} bytes from the byte array {@code buffer} starting at
     * offset {@code index} to this stream.
     *
     * @param buffer
     *            the buffer to be written.
     * @param offset
     *            the initial position in {@code buffer} to retrieve bytes.
     * @param len
     *            the number of bytes of {@code buffer} to write.
     * @throws NullPointerException
     *             if {@code buffer} is {@code null}.
     * @throws IndexOutOfBoundsException
     *             if {@code offset < 0} or {@code len < 0}, or if
     *             {@code offset + len} is greater than the length of
     *             {@code buffer}.
     */
    // synchronized
    CARAPI WriteBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [in] */ const ArrayOf<Byte> & buffer);

    /**
     * Takes the contents of this stream and writes it to the output stream
     * {@code out}.
     *
     * @param out
     *            an OutputStream on which to write the contents of this stream.
     * @throws IOException
     *             if an error occurs while writing to {@code out}.
     */
    // synchronized
    CARAPI WriteTo(IOutputStream* out);

private:
    CARAPI Expand(
        /* [in] */ Int32 i);

protected:
    /**
     * The byte array containing the bytes written.
     */
    ArrayOf<Byte>* mBuf;

    /**
     * The number of bytes written.
     */
    Int32 mCount;
};

#endif //__BYTEARRAYOUTPUTSTREAM_H__