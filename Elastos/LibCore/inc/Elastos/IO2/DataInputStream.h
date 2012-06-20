#ifndef __DATAINPUTSTREAM_H__
#define __DATAINPUTSTREAM_H__

#include "FilterInputStream.h"

class DataInputStream : public FilterInputStream
{
protected:
    DataInputStream();

    ~DataInputStream();

    CARAPI Init(
        /* [in] */ IInputStream* is);

public:
    /**
     * Reads bytes from this stream into the byte array {@code buffer}. Returns
     * the number of bytes that have been read.
     *
     * @param buffer
     *            the buffer to read bytes into.
     * @return the number of bytes that have been read or -1 if the end of the
     *         stream has been reached.
     * @throws IOException
     *             if a problem occurs while reading from this stream.
     * @see DataOutput#write(byte[])
     * @see DataOutput#write(byte[], int, int)
     */
    CARAPI ReadBuffer(
        /* [out] */ BufferOf<Byte>* buffer,
        /* [out] */ Int32* number);

    /**
     * Reads at most {@code length} bytes from this stream and stores them in
     * the byte array {@code buffer} starting at {@code offset}. Returns the
     * number of bytes that have been read or -1 if no bytes have been read and
     * the end of the stream has been reached.
     *
     * @param buffer
     *            the byte array in which to store the bytes read.
     * @param offset
     *            the initial position in {@code buffer} to store the bytes
     *            read from this stream.
     * @param length
     *            the maximum number of bytes to store in {@code buffer}.
     * @return the number of bytes that have been read or -1 if the end of the
     *         stream has been reached.
     * @throws IOException
     *             if a problem occurs while reading from this stream.
     * @see DataOutput#write(byte[])
     * @see DataOutput#write(byte[], int, int)
     */
    CARAPI ReadBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ BufferOf<Byte>* buffer,
        /* [out] */ Int32* number);

    /**
     * Reads a boolean from this stream.
     *
     * @return the next boolean value from the source stream.
     * @throws EOFException
     *             if the end of the filtered stream is reached before one byte
     *             has been read.
     * @throws IOException
     *             if a problem occurs while reading from this stream.
     * @see DataOutput#writeBoolean(boolean)
     */
    CARAPI ReadBoolean(
        /* [out] */ Boolean* value);

    /**
     * Reads an 8-bit byte value from this stream.
     *
     * @return the next byte value from the source stream.
     * @throws EOFException
     *             if the end of the filtered stream is reached before one byte
     *             has been read.
     * @throws IOException
     *             if a problem occurs while reading from this stream.
     * @see DataOutput#writeByte(int)
     */
    CARAPI ReadByte(
        /* [out] */ Byte* value);

    /**
     * Reads a 32-bit character value from this stream.
     *
     * @return the next char value from the source stream.
     * @throws EOFException
     *             if the end of the filtered stream is reached before two bytes
     *             have been read.
     * @throws IOException
     *             if a problem occurs while reading from this stream.
     * @see DataOutput#writeChar(int)
     */
    CARAPI ReadChar(
        /* [out] */ Char32* value);

    /**
     * Reads a 64-bit double value from this stream.
     *
     * @return the next double value from the source stream.
     * @throws EOFException
     *             if the end of the filtered stream is reached before eight
     *             bytes have been read.
     * @throws IOException
     *             if a problem occurs while reading from this stream.
     * @see DataOutput#writeDouble(double)
     */
    CARAPI ReadDouble(
        /* [out] */ Double* value);

    /**
     * Reads a 32-bit float value from this stream.
     *
     * @return the next float value from the source stream.
     * @throws EOFException
     *             if the end of the filtered stream is reached before four
     *             bytes have been read.
     * @throws IOException
     *             if a problem occurs while reading from this stream.
     * @see DataOutput#writeFloat(float)
     */
    CARAPI ReadFloat(
        /* [out] */ Float* value);

    /**
     * Reads bytes from this stream into the byte array {@code buffer}. This
     * method will block until {@code buffer.length} number of bytes have been
     * read.
     *
     * @param buffer
     *            to read bytes into.
     * @throws EOFException
     *             if the end of the source stream is reached before enough
     *             bytes have been read.
     * @throws IOException
     *             if a problem occurs while reading from this stream.
     * @see DataOutput#write(byte[])
     * @see DataOutput#write(byte[], int, int)
     */
    CARAPI ReadFully(
        /* [out] */ BufferOf<byte> * buffer);

    /**
     * Reads bytes from this stream and stores them in the byte array {@code
     * buffer} starting at the position {@code offset}. This method blocks until
     * {@code length} bytes have been read. If {@code length} is zero, then this
     * method returns without reading any bytes.
     *
     * @param buffer
     *            the byte array into which the data is read.
     * @param offset
     *            the offset in {@code buffer} from where to store the bytes
     *            read.
     * @param length
     *            the maximum number of bytes to read.
     * @throws EOFException
     *             if the end of the source stream is reached before enough
     *             bytes have been read.
     * @throws IndexOutOfBoundsException
     *             if {@code offset < 0} or {@code length < 0}, or if {@code
     *             offset + length} is greater than the size of {@code buffer}.
     * @throws IOException
     *             if a problem occurs while reading from this stream.
     * @throws NullPointerException
     *             if {@code buffer} or the source stream are null.
     * @see java.io.DataInput#readFully(byte[], int, int)
     */
    CARAPI ReadFullyEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ BufferOf<byte>* buffer);

    /**
     * Reads a 32-bit integer value from this stream.
     *
     * @return the next int value from the source stream.
     * @throws EOFException
     *             if the end of the filtered stream is reached before four
     *             bytes have been read.
     * @throws IOException
     *             if a problem occurs while reading from this stream.
     * @see DataOutput#writeInt(int)
     */
    CARAPI ReadInt32(
        /* [out] */ Int32* value);

    /**
     * Returns a string that contains the next line of text available from the
     * source stream. A line is represented by zero or more characters followed
     * by {@code '\n'}, {@code '\r'}, {@code "\r\n"} or the end of the stream.
     * The string does not include the newline sequence.
     *
     * @return the contents of the line or {@code null} if no characters were
     *         read before the end of the source stream has been reached.
     * @throws IOException
     *             if a problem occurs while reading from this stream.
     * @deprecated Use {@link BufferedReader}
     */
    CARAPI ReadLine(
        /* [out] */ String* value);

    /**
     * Reads a 64-bit long value from this stream.
     *
     * @return the next long value from the source stream.
     * @throws EOFException
     *             if the end of the filtered stream is reached before eight
     *             bytes have been read.
     * @throws IOException
     *             if a problem occurs while reading from this stream.
     * @see DataOutput#writeLong(long)
     */
    CARAPI ReadInt64(
        /* [out] */ Int64* value);

    /**
     * Reads a 16-bit short value from this stream.
     *
     * @return the next short value from the source stream.
     * @throws EOFException
     *             if the end of the filtered stream is reached before two bytes
     *             have been read.
     * @throws IOException
     *             if a problem occurs while reading from this stream.
     * @see DataOutput#writeShort(int)
     */
    CARAPI ReadInt16(
        /* [out] */ Int16* value);

    /**
     * Reads an unsigned 8-bit byte value from this stream and returns it as an
     * int.
     *
     * @return the next unsigned byte value from the source stream.
     * @throws EOFException
     *             if the end of the filtered stream has been reached before one
     *             byte has been read.
     * @throws IOException
     *             if a problem occurs while reading from this stream.
     * @see DataOutput#writeByte(int)
     */
    CARAPI ReadUnsignedByte(
        /* [out] */ Int32* value);

    /**
     * Reads a 16-bit unsigned short value from this stream and returns it as an
     * int.
     *
     * @return the next unsigned short value from the source stream.
     * @throws EOFException
     *             if the end of the filtered stream is reached before two bytes
     *             have been read.
     * @throws IOException
     *             if a problem occurs while reading from this stream.
     * @see DataOutput#writeShort(int)
     */
    CARAPI ReadUnsignedInt16(
        /* [out] */ Int32* value);

    /**
     * Skips {@code count} number of bytes in this stream. Subsequent {@code
     * read()}s will not return these bytes unless {@code reset()} is used.
     *
     * This method will not throw an {@link EOFException} if the end of the
     * input is reached before {@code count} bytes where skipped.
     *
     * @param count
     *            the number of bytes to skip.
     * @return the number of bytes actually skipped.
     * @throws IOException
     *             if a problem occurs during skipping.
     * @see #mark(int)
     * @see #reset()
     */
    CARAPI SkipBytes(
        /* [in] */ Int32 count,
        /* [out] */ Int32* value);

private:
    CARAPI ReadToBuff(
        /* [in] */ Int32 count,
        /* [out] */ Int32* number);

private:
    BufferOf<Byte>* mBuff;
};

#endif