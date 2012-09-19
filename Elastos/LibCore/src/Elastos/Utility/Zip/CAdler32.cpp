
#include "cmdef.h"
#include "CAdler32.h"
#include <zlib.h>


CAdler32::CAdler32()
    : mAdler(1)
{}

/**
 * Returns the current calculated checksum value.
 *
 * @return the checksum.
 */
ECode CAdler32::GetValue(
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mAdler;
    return NOERROR;
}

/**
 * Resets the checksum value applied before beginning calculations on a new
 * stream of data.
 */
ECode CAdler32::Reset()
{
    mAdler = 1;
    return NOERROR;
}

/**
 * Updates the checksum value with the given byte.
 *
 * @param val
 *            the byte to update the checksum with.
 */
ECode CAdler32::Update(
    /* [in] */ Int32 i)
{
    mAdler = UpdateByteImpl(i, mAdler);
    return NOERROR;
}

/**
 * Updates this checksum with the bytes contained in buffer {@code buf}.
 *
 * @param buf
 *            the buffer holding the data to update the checksum with.
 */
ECode CAdler32::UpdateEx(
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
ECode CAdler32::UpdateEx2(
    /* [in] */ const ArrayOf<Byte>& buf,
    /* [in] */ Int32 off,
    /* [in] */ Int32 nbytes)
{
    // avoid int overflow, check null buf
    if (off <= buf.GetLength() && nbytes >= 0 && off >= 0
            && buf.GetLength() - off >= nbytes) {
        mAdler = UpdateImpl(buf, off, nbytes, mAdler);
        return NOERROR;
    }
    else {
//        throw new ArrayIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
}

Int64 CAdler32::UpdateImpl(
    /* [in] */ const ArrayOf<Byte>& buf,
    /* [in] */ Int32 off,
    /* [in] */ Int32 nbytes,
    /* [in] */ Int64 adler)
{
    return adler32(adler, reinterpret_cast<const Bytef*>(buf.GetPayload() + off), nbytes);
}

Int64 CAdler32::UpdateByteImpl(
    /* [in] */ Int32 val,
    /* [in] */ Int64 adler)
{
    Bytef bytefVal = val;
    return adler32(adler, reinterpret_cast<const Bytef*>(&bytefVal), 1);
}
