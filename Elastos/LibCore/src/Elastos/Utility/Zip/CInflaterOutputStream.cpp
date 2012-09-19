
#include "CInflaterOutputStream.h"


ECode CInflaterOutputStream::Close()
{
    return InflaterOutputStream::Close();
}

ECode CInflaterOutputStream::Flush()
{
    return InflaterOutputStream::Flush();
}

ECode CInflaterOutputStream::Write(
    /* [in] */ Int32 oneByte)
{
    return InflaterOutputStream::Write(oneByte);
}

ECode CInflaterOutputStream::WriteBuffer(
    /* [in] */ const ArrayOf<Byte>& buffer)
{
    return InflaterOutputStream::WriteBuffer(buffer);
}

ECode CInflaterOutputStream::WriteBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ const ArrayOf<Byte>& buffer)
{
    return InflaterOutputStream::WriteBufferEx(offset, count, buffer);
}

ECode CInflaterOutputStream::Finish()
{
    return InflaterOutputStream::Finish();
}

/**
 * Constructs an {@code InflaterOutputStream} with a new {@code Inflater} and an
 * implementation-defined default internal buffer size. {@code out} is a destination
 * for uncompressed data, and compressed data will be written to this stream.
 *
 * @param out the destination {@code OutputStream}
 */
ECode CInflaterOutputStream::constructor(
    /* [in] */ IOutputStream* out)
{
    return InflaterOutputStream::Init(out);
}

/**
 * Constructs an {@code InflaterOutputStream} with the given {@code Inflater} and an
 * implementation-defined default internal buffer size. {@code out} is a destination
 * for uncompressed data, and compressed data will be written to this stream.
 *
 * @param out the destination {@code OutputStream}
 * @param inf the {@code Inflater} to be used for decompression
 */
ECode CInflaterOutputStream::constructor(
    /* [in] */ IOutputStream* out,
    /* [in] */ IInflater* inf)
{
    return InflaterOutputStream::Init(out, inf);
}

/**
 * Constructs an {@code InflaterOutputStream} with the given {@code Inflater} and
 * given internal buffer size. {@code out} is a destination
 * for uncompressed data, and compressed data will be written to this stream.
 *
 * @param out the destination {@code OutputStream}
 * @param inf the {@code Inflater} to be used for decompression
 * @param bufferSize the length in bytes of the internal buffer
 */
ECode CInflaterOutputStream::constructor(
    /* [in] */ IOutputStream* out,
    /* [in] */ IInflater* inf,
    /* [in] */ Int32 bufferSize)
{
    return InflaterOutputStream::Init(out, inf, bufferSize);
}
