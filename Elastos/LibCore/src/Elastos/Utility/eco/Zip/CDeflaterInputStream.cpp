
#include "cmdef.h"
#include "CDeflaterInputStream.h"

ECode CDeflaterInputStream::Available(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    return DeflaterInputStream::Available(number);
}

ECode CDeflaterInputStream::Close()
{
    return DeflaterInputStream::Close();
}

ECode CDeflaterInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    return DeflaterInputStream::Mark(readLimit);
}

ECode CDeflaterInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);
    return DeflaterInputStream::IsMarkSupported(supported);
}

ECode CDeflaterInputStream::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    return DeflaterInputStream::Read(value);
}

ECode CDeflaterInputStream::ReadBuffer(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    return DeflaterInputStream::ReadBuffer(buffer, number);
}

ECode CDeflaterInputStream::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    return DeflaterInputStream::ReadBufferEx(offset, length, buffer, number);
}

ECode CDeflaterInputStream::Reset()
{
    return DeflaterInputStream::Reset();
}

ECode CDeflaterInputStream::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);
    return DeflaterInputStream::Skip(count, number);
}

/**
 * Constructs a {@code DeflaterInputStream} with a new {@code Deflater} and an
 * implementation-defined default internal buffer size. {@code in} is a source of
 * uncompressed data, and this stream will be a source of compressed data.
 *
 * @param in the source {@code InputStream}
 */
ECode CDeflaterInputStream::constructor(
    /* [in] */ IInputStream* is)
{
    return DeflaterInputStream::Init(is);
}

/**
 * Constructs a {@code DeflaterInputStream} with the given {@code Deflater} and an
 * implementation-defined default internal buffer size. {@code in} is a source of
 * uncompressed data, and this stream will be a source of compressed data.
 *
 * @param in the source {@code InputStream}
 * @param deflater the {@code Deflater} to be used for compression
 */
ECode CDeflaterInputStream::constructor(
    /* [in] */ IInputStream* is,
    /* [in] */ IDeflater* deflater)
{
    return DeflaterInputStream::Init(is, deflater);
}

/**
 * Constructs a {@code DeflaterInputStream} with the given {@code Deflater} and
 * given internal buffer size. {@code in} is a source of
 * uncompressed data, and this stream will be a source of compressed data.
 *
 * @param in the source {@code InputStream}
 * @param deflater the {@code Deflater} to be used for compression
 * @param bufferSize the length in bytes of the internal buffer
 */
ECode CDeflaterInputStream::constructor(
    /* [in] */ IInputStream* is,
    /* [in] */ IDeflater* deflater,
    /* [in] */ Int32 bufferSize)
{
    return DeflaterInputStream::Init(is, deflater, bufferSize);
}
