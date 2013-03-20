
#include "CDeflaterOutputStream.h"


ECode CDeflaterOutputStream::Close()
{
    return DeflaterOutputStream::Close();
}

ECode CDeflaterOutputStream::Flush()
{
    return DeflaterOutputStream::Flush();
}

ECode CDeflaterOutputStream::Write(
    /* [in] */ Int32 oneByte)
{
    return DeflaterOutputStream::Write(oneByte);
}

ECode CDeflaterOutputStream::WriteBuffer(
    /* [in] */ const ArrayOf<Byte>& buffer)
{
    return DeflaterOutputStream::WriteBuffer(buffer);
}

ECode CDeflaterOutputStream::WriteBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ const ArrayOf<Byte>& buffer)
{
    return DeflaterOutputStream::WriteBufferEx(offset, count, buffer);
}

ECode CDeflaterOutputStream::Finish()
{
    return DeflaterOutputStream::Finish();
}

/**
 * This is the most basic constructor. You only need to pass the {@code
 * OutputStream} to which the compressed data shall be written to. The
 * default settings for the {@code Deflater} and internal buffer are used.
 * In particular the {@code Deflater} produces a ZLIB header in the output
 * stream.
 *
 * @param os
 *            is the OutputStream where to write the compressed data to.
 */
ECode CDeflaterOutputStream::constructor(
    /* [in] */ IOutputStream* os)
{
    return DeflaterOutputStream::Init(os);
}

/**
 * This constructor lets you pass the {@code Deflater} specifying the
 * compression algorithm.
 *
 * @param os
 *            is the {@code OutputStream} where to write the compressed data
 *            to.
 * @param def
 *            is the specific {@code Deflater} that is used to compress
 *            data.
 */
ECode CDeflaterOutputStream::constructor(
    /* [in] */ IOutputStream* os,
    /* [in] */ IDeflater* def)
{
    return DeflaterOutputStream::Init(os, def);
}

/**
 * This constructor lets you specify both the compression algorithm as well
 * as the internal buffer size to be used.
 *
 * @param os
 *            is the {@code OutputStream} where to write the compressed data
 *            to.
 * @param def
 *            is the specific {@code Deflater} that will be used to compress
 *            data.
 * @param bsize
 *            is the size to be used for the internal buffer.
 */
ECode CDeflaterOutputStream::constructor(
    /* [in] */ IOutputStream* os,
    /* [in] */ IDeflater* def,
    /* [in] */ Int32 bsize)
{
    return DeflaterOutputStream::Init(os, def, bsize);
}
