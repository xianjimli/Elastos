
#include "cmdef.h"
#include "CZipInputStream.h"

ECode CZipInputStream::Available(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    return ZipInputStream::Available(number);
}

ECode CZipInputStream::Close()
{
    return ZipInputStream::Close();
}

ECode CZipInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    return ZipInputStream::Mark(readLimit);
}

ECode CZipInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);
    return ZipInputStream::IsMarkSupported(supported);
}

ECode CZipInputStream::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    return ZipInputStream::Read(value);
}

ECode CZipInputStream::ReadBuffer(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    return ZipInputStream::ReadBuffer(buffer, number);
}

ECode CZipInputStream::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    return ZipInputStream::ReadBufferEx(offset, length, buffer, number);
}

ECode CZipInputStream::Reset()
{
    return ZipInputStream::Reset();
}

ECode CZipInputStream::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);
    return ZipInputStream::Skip(count, number);
}

ECode CZipInputStream::CloseEntry()
{
    return ZipInputStream::CloseEntry();
}

ECode CZipInputStream::GetNextEntry(
    /* [out] */ IZipEntry** entry)
{
    VALIDATE_NOT_NULL(entry);
    return ZipInputStream::GetNextEntry(entry);
}

/**
 * Constructs a new {@code ZipInputStream} from the specified input stream.
 *
 * @param stream
 *            the input stream to representing a ZIP archive.
 */
ECode CZipInputStream::constructor(
    /* [in] */ IInputStream* stream)
{
    return ZipInputStream::Init(stream);
}
