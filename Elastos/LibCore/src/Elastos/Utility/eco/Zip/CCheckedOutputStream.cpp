
#include "cmdef.h"
#include "CCheckedOutputStream.h"


/**
 * Returns the checksum calculated on the stream read so far.
 *
 * @return the updated checksum.
 */
ECode CCheckedOutputStream::GetChecksum(
    /* [out] */ IChecksum** checksum)
{
    VALIDATE_NOT_NULL(checksum);
    *checksum = mCheck.Get();
    (*checksum)->AddRef();
    return NOERROR;
}

/**
 * Writes the specified byte to the underlying stream. The checksum is
 * updated with {@code val}.
 *
 * @param val
 *            the data value to written to the output stream.
 * @throws IOException
 *             if an IO error has occurred.
 */
//@Override
ECode CCheckedOutputStream::Write(
    /* [in] */ Int32 val)
{
    FAIL_RETURN(mOut->Write(val));
    return mCheck->Update(val);
}

/**
 * Writes n bytes of data from {@code buf} starting at offset {@code off} to
 * the underlying stream. The checksum is updated with the bytes written.
 *
 * @param buf
 *            data written to the output stream.
 * @param off
 *            the offset to start reading the data from {@code buf} written
 *            to the output stream.
 * @param nbytes
 *            number of bytes to write to the output stream.
 * @throws IOException
 *             if an IO error has occurred.
 */
//@Override
ECode CCheckedOutputStream::WriteBufferEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 nbytes,
    /* [in] */ const ArrayOf<Byte> & buf)
{
    FAIL_RETURN(mOut->WriteBufferEx(off, nbytes, buf));
    return mCheck->UpdateEx2(buf, off, nbytes);
}

ECode CCheckedOutputStream::Close()
{
    ECode ec = Flush();
    ECode ec2 = mOut->Close();
    if(ec != NOERROR || ec2 != NOERROR){
        return ec != NOERROR ? ec : ec2;
    }
    return NOERROR;
}

ECode CCheckedOutputStream::Flush()
{
    return mOut->Flush();
}

ECode CCheckedOutputStream::WriteBuffer(
    /* [in] */ const ArrayOf<Byte> & buffer)
{
    // BEGIN android-note
    // changed array notation to be consistent with the rest of harmony
    // END android-note
    return WriteBufferEx(0, buffer.GetLength(), buffer);
}

ECode CCheckedOutputStream::CheckError(
    /* [out] */ Boolean* hasError)
{
    VALIDATE_NOT_NULL(hasError);
    *hasError = FALSE;
    return NOERROR;
}

/**
 * Constructs a new {@code CheckedOutputStream} on {@code OutputStream}
 * {@code os}. The checksum is calculated using the algorithm implemented
 * by {@code csum}.
 *
 * @param os
 *            the output stream to calculate checksum for.
 * @param cs
 *            an entity implementing the checksum algorithm.
 */
ECode CCheckedOutputStream::constructor(
    /* [in] */ IOutputStream* os,
    /* [in] */ IChecksum* cs)
{
    mOut = os;
    mCheck = cs;
    return NOERROR;
}