
#ifndef __CCRC32_H__
#define __CCRC32_H__


#include "_CCRC32.h"

CarClass(CCRC32)
{
public:
    CCRC32();

    /**
     * Returns the current calculated checksum value.
     *
     * @return the checksum.
     */
    CARAPI GetValue(
        /* [out] */ Int64* value);

    /**
     * Resets the checksum value applied before beginning calculations on a new
     * stream of data.
     */
    CARAPI Reset();

    /**
     * Updates the checksum value with the given byte.
     *
     * @param val
     *            the byte to update the checksum with.
     */
    CARAPI Update(
        /* [in] */ Int32 val);

    /**
     * Updates this checksum with the bytes contained in buffer {@code buf}.
     *
     * @param buf
     *            the buffer holding the data to update the checksum with.
     */
    CARAPI UpdateEx(
        /* [in] */ const ArrayOf<Byte>& buf);

    /**
     * Updates the checksum with the given bytes.
     *
     * @param buf
     *            the byte array from which to read the bytes.
     * @param off
     *            the initial position in {@code buf} to read the bytes from.
     * @param nbytes
     *            the number of bytes to read from {@code buf}.
     */
    CARAPI UpdateEx2(
        /* [in] */ const ArrayOf<Byte>& buf,
        /* [in] */ Int32 off,
        /* [in] */ Int32 nbytes);

private:
    CARAPI_(Int64) UpdateImpl(
        /* [in] */ const ArrayOf<Byte>& buf,
        /* [in] */ Int32 off,
        /* [in] */ Int32 nbytes,
        /* [in] */ Int64 crc);

    CARAPI_(Int64) UpdateByteImpl(
        /* [in] */ Byte val,
        /* [in] */ Int64 crc);

public:
    Int64 mTbytes;

private:
    Int64 mCrc;
};

#endif //__CCRC32_H__
