
#ifndef __CADLER32_H__
#define __CADLER32_H__

#include "_CAdler32.h"

CarClass(CAdler32)
{
public:
    CAdler32();

    /**
     * Returns the {@code Adler32} checksum for all input received.
     *
     * @return The checksum for this instance.
     */
    CARAPI GetValue(
        /* [out] */ Int64* value);

    /**
     * Reset this instance to its initial checksum.
     */
    CARAPI Reset();

    /**
     * Update this {@code Adler32} checksum with the single byte provided as
     * argument.
     *
     * @param i
     *            the byte to update checksum with.
     */
    CARAPI Update(
        /* [in] */ Int32 i);

    /**
     * Update this {@code Adler32} checksum using the contents of {@code buf}.
     *
     * @param buf
     *            bytes to update checksum with.
     */
    CARAPI UpdateEx(
        /* [in] */ const ArrayOf<Byte>& buf);

    /**
     * Update this {@code Adler32} checksum with the contents of {@code buf},
     * starting from the offset provided and reading n bytes of data.
     *
     * @param buf
     *            buffer to obtain data from.
     * @param off
     *            offset in {@code buf} to start reading from.
     * @param nbytes
     *            number of bytes from {@code buf} to use.
     * @throws ArrayIndexOutOfBoundsException
     *             if {@code offset > buf.length} or {@code nbytes} is negative
     *             or {@code offset + nbytes > buf.length}.
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
        /* [in] */ Int64 adler);

    CARAPI_(Int64) UpdateByteImpl(
        /* [in] */ Int32 val,
        /* [in] */ Int64 adler);

private:
    Int64 mAdler;
};

#endif //__CADLER32_H__
