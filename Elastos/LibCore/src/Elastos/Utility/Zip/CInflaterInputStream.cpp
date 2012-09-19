
#include "cmdef.h"
#include "CInflaterInputStream.h"


ECode CInflaterInputStream::Available(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    return InflaterInputStream::Available(number);
}

ECode CInflaterInputStream::Close()
{
    return InflaterInputStream::Close();
}

ECode CInflaterInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    return InflaterInputStream::Mark(readLimit);
}

ECode CInflaterInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);
    return InflaterInputStream::IsMarkSupported(supported);
}

ECode CInflaterInputStream::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    return InflaterInputStream::Read(value);
}

ECode CInflaterInputStream::ReadBuffer(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    return InflaterInputStream::ReadBuffer(buffer, number);
}

ECode CInflaterInputStream::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    return InflaterInputStream::ReadBufferEx(offset, length, buffer, number);
}

ECode CInflaterInputStream::Reset()
{
    return InflaterInputStream::Reset();
}

ECode CInflaterInputStream::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);
    return InflaterInputStream::Skip(count, number);
}

/**
 * This is the most basic constructor. You only need to pass the {@code
 * InputStream} from which the compressed data is to be read from. Default
 * settings for the {@code Inflater} and internal buffer are be used. In
 * particular the Inflater expects a ZLIB header from the input stream.
 *
 * @param is
 *            the {@code InputStream} to read data from.
 */
ECode CInflaterInputStream::constructor(
    /* [in] */ IInputStream* is)
{
    return InflaterInputStream::Init(is);
}

/**
 * This constructor lets you pass a specifically initialized Inflater,
 * for example one that expects no ZLIB header.
 *
 * @param is
 *            the {@code InputStream} to read data from.
 * @param inflater
 *            the specific {@code Inflater} for uncompressing data.
 */
ECode CInflaterInputStream::constructor(
    /* [in] */ IInputStream* is,
    /* [in] */ IInflater* inflater)
{
    return InflaterInputStream::Init(is, inflater);
}

/**
 * This constructor lets you specify both the {@code Inflater} as well as
 * the internal buffer size to be used.
 *
 * @param is
 *            the {@code InputStream} to read data from.
 * @param inflater
 *            the specific {@code Inflater} for uncompressing data.
 * @param bsize
 *            the size to be used for the internal buffer.
 */
ECode CInflaterInputStream::constructor(
    /* [in] */ IInputStream* is,
    /* [in] */ IInflater* inflater,
    /* [in] */ Int32 bsize)
{
    return InflaterInputStream::Init(is, inflater, bsize);
}
