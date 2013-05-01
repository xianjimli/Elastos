
#include "CZipOutputStream.h"


ECode CZipOutputStream::Close()
{
    return ZipOutputStream::Close();
}

ECode CZipOutputStream::Flush()
{
    return ZipOutputStream::Flush();
}

ECode CZipOutputStream::Write(
    /* [in] */ Int32 oneByte)
{
    return ZipOutputStream::Write(oneByte);
}

ECode CZipOutputStream::WriteBuffer(
    /* [in] */ const ArrayOf<Byte>& buffer)
{
    return ZipOutputStream::WriteBuffer(buffer);
}

ECode CZipOutputStream::WriteBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ const ArrayOf<Byte>& buffer)
{
    return ZipOutputStream::WriteBufferEx(offset, count, buffer);
}

ECode CZipOutputStream::CheckError(
    /* [out] */ Boolean* hasError)
{
    VALIDATE_NOT_NULL(hasError);
    return ZipOutputStream::CheckError(hasError);
}

ECode CZipOutputStream::Finish()
{
    return ZipOutputStream::Finish();
}

ECode CZipOutputStream::CloseEntry()
{
    return ZipOutputStream::CloseEntry();
}

ECode CZipOutputStream::PutNextEntry(
    /* [in] */ IZipEntry* ze)
{
    return ZipOutputStream::PutNextEntry(ze);
}

ECode CZipOutputStream::SetComment(
    /* [in] */ const String& comment)
{
    return ZipOutputStream::SetComment(comment);
}

ECode CZipOutputStream::SetLevel(
    /* [in] */ Int32 level)
{
    return ZipOutputStream::SetLevel(level);
}

ECode CZipOutputStream::SetMethod(
    /* [in] */ Int32 method)
{
    return ZipOutputStream::SetMethod(method);
}

/**
 * Constructs a new {@code ZipOutputStream} with the specified output
 * stream.
 *
 * @param p1
 *            the {@code OutputStream} to write the data to.
 */
ECode CZipOutputStream::constructor(
    /* [in] */ IOutputStream* os)
{
    return ZipOutputStream::Init(os);
}
