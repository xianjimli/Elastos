
#include "cmdef.h"
#include "CGZIPOutputStream.h"


CGZIPOutputStream::CGZIPOutputStream()
{
    CCRC32::NewByFriend((CCRC32**)&mCrc);
}

/**
 * Indicates to the stream that all data has been written out, and any GZIP
 * terminal data can now be written.
 *
 * @throws IOException
 *             if an {@code IOException} occurs.
 */
//@Override
ECode CGZIPOutputStream::Finish()
{
    FAIL_RETURN(DeflaterOutputStream::Finish());
    Int64 checksum;
    mCrc->GetValue(&checksum);
    WriteInt64(checksum);
    WriteInt64(mCrc->mTbytes);
    return NOERROR;
}

/**
 * Write up to nbytes of data from the given buffer, starting at offset off,
 * to the underlying stream in GZIP format.
 */
//@Override
ECode CGZIPOutputStream::WriteBufferEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 nbytes,
    /* [in] */ const ArrayOf<Byte>& buffer)
{
    FAIL_RETURN(DeflaterOutputStream::WriteBufferEx(off, nbytes, buffer));
    return mCrc->UpdateEx2(buffer, off, nbytes);
}

/**
 * Construct a new {@code GZIPOutputStream} to write data in GZIP format to
 * the underlying stream.
 *
 * @param os
 *            the {@code OutputStream} to write data to.
 * @throws IOException
 *             if an {@code IOException} occurs.
 */
ECode CGZIPOutputStream::constructor(
    /* [in] */ IOutputStream* os)
{
    return constructor(os, BUF_SIZE);
}

Int64 CGZIPOutputStream::WriteInt64(
    /* [in] */ Int64 i)
{
    // Write out the long value as an unsigned int
    Int32 ui = (Int32)i;
    mOut->Write(ui & 0xFF);
    mOut->Write((ui >> 8) & 0xFF);
    mOut->Write((ui >> 16) & 0xFF);
    mOut->Write((ui >> 24) & 0xFF);
    return i;
}

Int32 CGZIPOutputStream::WriteInt16(
    /* [in] */ Int32 i)
{
    mOut->Write(i & 0xFF);
    mOut->Write((i >> 8) & 0xFF);
    return i;
}

ECode CGZIPOutputStream::Close()
{
    return DeflaterOutputStream::Close();
}

ECode CGZIPOutputStream::Flush()
{
    return DeflaterOutputStream::Flush();
}

ECode CGZIPOutputStream::Write(
    /* [in] */ Int32 val)
{
    return DeflaterOutputStream::Write(val);
}

ECode CGZIPOutputStream::WriteBuffer(
    /* [in] */ const ArrayOf<Byte> & buffer)
{
    return DeflaterOutputStream::WriteBuffer(buffer);
}

/**
 * Construct a new {@code GZIPOutputStream} to write data in GZIP format to
 * the underlying stream. Set the internal compression buffer to size
 * {@code size}.
 *
 * @param os
 *            the {@code OutputStream} to write to.
 * @param size
 *            the internal buffer size.
 * @throws IOException
 *             if an {@code IOException} occurs.
 */
ECode CGZIPOutputStream::constructor(
    /* [in] */ IOutputStream* os,
    /* [in] */ Int32 size)
{
    AutoPtr<CDeflater> deflater;
    CDeflater::NewByFriend(IDeflater_DEFAULT_COMPRESSION, TRUE, (CDeflater**)&deflater);
    FAIL_RETURN(DeflaterOutputStream::Init(os, deflater.Get(), size));
    WriteInt16(GZIP_MAGIC);
    FAIL_RETURN(mOut->Write(IDeflater_DEFLATED));
    FAIL_RETURN(mOut->Write(0)); // flags
    WriteInt64(0); // mod time
    FAIL_RETURN(mOut->Write(0)); // extra flags
    return mOut->Write(0); // operating system
}

