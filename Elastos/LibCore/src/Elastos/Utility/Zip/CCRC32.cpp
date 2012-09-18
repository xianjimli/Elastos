
#include "cmdef.h"
#include "CCRC32.h"
#include <zlib.h>


CCRC32::CCRC32()
    : mTbytes(0ll)
    , mCrc(0ll)
{}

/**
 * Returns the current calculated checksum value.
 *
 * @return the checksum.
 */
ECode CCRC32::GetValue(
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mCrc;
    return NOERROR;
}

/**
 * Resets the checksum value applied before beginning calculations on a new
 * stream of data.
 */
ECode CCRC32::Reset()
{
    mTbytes = mCrc = 0;
    return NOERROR;
}

/**
 * Updates the checksum value with the given byte.
 *
 * @param val
 *            the byte to update the checksum with.
 */
ECode CCRC32::Update(
    /* [in] */ Int32 val)
{
    mCrc = UpdateByteImpl((Byte)val, mCrc);
    return NOERROR;
}

/**
 * Updates this checksum with the bytes contained in buffer {@code buf}.
 *
 * @param buf
 *            the buffer holding the data to update the checksum with.
 */
ECode CCRC32::UpdateEx(
    /* [in] */ const ArrayOf<Byte>& buf)
{
    return UpdateEx2(buf, 0, buf.GetLength());
}

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
ECode CCRC32::UpdateEx2(
    /* [in] */ const ArrayOf<Byte>& buf,
    /* [in] */ Int32 off,
    /* [in] */ Int32 nbytes)
{
    // avoid int overflow, check null buf
    if (off <= buf.GetLength() && nbytes >= 0 && off >= 0
            && buf.GetLength() - off >= nbytes) {
        mTbytes += nbytes;
        mCrc = UpdateImpl(buf, off, nbytes, mCrc);
        return NOERROR;
    }
    else {
//        throw new ArrayIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
}

Int64 CCRC32::UpdateImpl(
    /* [in] */ const ArrayOf<Byte>& buf,
    /* [in] */ Int32 off,
    /* [in] */ Int32 nbytes,
    /* [in] */ Int64 crc)
{
    return crc32(crc, reinterpret_cast<const Bytef*>(buf.GetPayload() + off), nbytes);
}

Int64 CCRC32::UpdateByteImpl(
    /* [in] */ Byte val,
    /* [in] */ Int64 crc)
{
    return crc32(crc, reinterpret_cast<const Bytef*>(&val), 1);
}
