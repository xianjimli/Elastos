
#include "cmdef.h"
#include "CZipEntry.h"


/**
 * Gets the comment for this {@code ZipEntry}.
 *
 * @return the comment for this {@code ZipEntry}, or {@code null} if there
 *         is no comment. If we're reading an archive with
 *         {@code ZipInputStream} the comment is not available.
 */
ECode CZipEntry::GetComment(
    /* [out] */ String* comment)
{
    VALIDATE_NOT_NULL(comment);
    *comment = ZipEntry::GetComment();
    return NOERROR;
}

/**
 * Gets the compressed size of this {@code ZipEntry}.
 *
 * @return the compressed size, or -1 if the compressed size has not been
 *         set.
 */
ECode CZipEntry::GetCompressedSize(
    /* [out] */ Int64* size)
{
    VALIDATE_NOT_NULL(size);
    *size = ZipEntry::GetCompressedSize();
    return NOERROR;
}

/**
 * Gets the checksum for this {@code ZipEntry}.
 *
 * @return the checksum, or -1 if the checksum has not been set.
 */
ECode CZipEntry::GetCrc(
    /* [out] */ Int64* checksum)
{
    VALIDATE_NOT_NULL(checksum);
    *checksum = ZipEntry::GetCrc();
    return NOERROR;
}

/**
 * Gets the extra information for this {@code ZipEntry}.
 *
 * @return a byte array containing the extra information, or {@code null} if
 *         there is none.
 */
ECode CZipEntry::GetExtra(
    /* [out, callee] */ ArrayOf<Byte>** extra)
{
    VALIDATE_NOT_NULL(extra);
    *extra = ZipEntry::GetExtra();
    if (*extra != NULL) *extra = (*extra)->Clone();
    return NOERROR;
}

/**
 * Gets the compression method for this {@code ZipEntry}.
 *
 * @return the compression method, either {@code DEFLATED}, {@code STORED}
 *         or -1 if the compression method has not been set.
 */
ECode CZipEntry::GetMethod(
    /* [out] */ Int32* method)
{
    VALIDATE_NOT_NULL(method);
    *method = ZipEntry::GetMethod();
    return NOERROR;
}

/**
 * Gets the name of this {@code ZipEntry}.
 *
 * @return the entry name.
 */
ECode CZipEntry::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = ZipEntry::GetName();
    return NOERROR;
}

/**
 * Gets the uncompressed size of this {@code ZipEntry}.
 *
 * @return the uncompressed size, or {@code -1} if the size has not been
 *         set.
 */
ECode CZipEntry::GetSize(
    /* [out] */ Int64* size)
{
    VALIDATE_NOT_NULL(size);
    *size = ZipEntry::GetSize();
    return NOERROR;
}

/**
 * Gets the last modification time of this {@code ZipEntry}.
 *
 * @return the last modification time as the number of milliseconds since
 *         Jan. 1, 1970.
 */
ECode CZipEntry::GetTime(
    /* [out] */ Int64* time)
{
    VALIDATE_NOT_NULL(time);
    *time = ZipEntry::GetTime();
    return NOERROR;
}

/**
 * Determine whether or not this {@code ZipEntry} is a directory.
 *
 * @return {@code true} when this {@code ZipEntry} is a directory, {@code
 *         false} otherwise.
 */
ECode CZipEntry::IsDirectory(
    /* [out] */ Boolean* isDirectory)
{
    VALIDATE_NOT_NULL(isDirectory);
    *isDirectory = ZipEntry::IsDirectory();
    return NOERROR;
}

/**
 * Sets the comment for this {@code ZipEntry}.
 *
 * @param string
 *            the comment for this entry.
 */
ECode CZipEntry::SetComment(
    /* [in] */ const String& comment)
{
    return ZipEntry::SetComment(comment);
}

/**
 * Sets the compressed size for this {@code ZipEntry}.
 *
 * @param value
 *            the compressed size (in bytes).
 */
ECode CZipEntry::SetCompressedSize(
    /* [in] */ Int64 size)
{
    return ZipEntry::SetCompressedSize(size);
}

/**
 * Sets the checksum for this {@code ZipEntry}.
 *
 * @param value
 *            the checksum for this entry.
 * @throws IllegalArgumentException
 *             if {@code value} is < 0 or > 0xFFFFFFFFL.
 */
ECode CZipEntry::SetCrc(
    /* [in] */ Int64 checksum)
{
    return ZipEntry::SetCrc(checksum);
}

/**
 * Sets the extra information for this {@code ZipEntry}.
 *
 * @param data
 *            a byte array containing the extra information.
 * @throws IllegalArgumentException
 *             when the length of data is greater than 0xFFFF bytes.
 */
ECode CZipEntry::SetExtra(
    /* [in] */ ArrayOf<Byte>* data)
{
    return ZipEntry::SetExtra(data);
}

/**
 * Sets the compression method for this {@code ZipEntry}.
 *
 * @param value
 *            the compression method, either {@code DEFLATED} or {@code
 *            STORED}.
 * @throws IllegalArgumentException
 *             when value is not {@code DEFLATED} or {@code STORED}.
 */
ECode CZipEntry::SetMethod(
    /* [in] */ Int32 method)
{
    return ZipEntry::SetMethod(method);
}

/**
 * Sets the uncompressed size of this {@code ZipEntry}.
 *
 * @param value
 *            the uncompressed size for this entry.
 * @throws IllegalArgumentException
 *             if {@code value} < 0 or {@code value} > 0xFFFFFFFFL.
 */
ECode CZipEntry::SetSize(
    /* [in] */ Int64 size)
{
    return ZipEntry::SetSize(size);
}

/**
 * Sets the modification time of this {@code ZipEntry}.
 *
 * @param value
 *            the modification time as the number of milliseconds since Jan.
 *            1, 1970.
 */
ECode CZipEntry::SetTime(
    /* [in] */ Int64 time)
{
    return ZipEntry::SetTime(time);
}

/**
 * Constructs a new {@code ZipEntry} with the specified name.
 *
 * @param name
 *            the name of the ZIP entry.
 * @throws IllegalArgumentException
 *             if the name length is outside the range (> 0xFFFF).
 */
ECode CZipEntry::constructor(
    /* [in] */ const String& name)
{
    return ZipEntry::Init(name);
}

/**
 * Constructs a new {@code ZipEntry} using the values obtained from {@code
 * ze}.
 *
 * @param ze
 *            the {@code ZipEntry} from which to obtain values.
 */
ECode CZipEntry::constructor(
    /* [in] */ IZipEntry* ze)
{
    return ZipEntry::Init((CZipEntry*)ze);
}
