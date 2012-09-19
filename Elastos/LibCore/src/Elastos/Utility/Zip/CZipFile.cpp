
#include "cmdef.h"
#include "CZipFile.h"

ECode CZipFile::Close()
{
    return ZipFile::Close();
}

ECode CZipFile::GetEntries(
    /* [out] */ IObjectContainer** entries)
{
    VALIDATE_NOT_NULL(entries);
    return ZipFile::GetEntries(entries);
}

ECode CZipFile::GetEntry(
    /* [in] */ const String& entryName,
    /* [out] */ IZipEntry** entry)
{
    VALIDATE_NOT_NULL(entry);
    return ZipFile::GetEntry(entryName, entry);
}

ECode CZipFile::GetInputStream(
    /* [in] */ IZipEntry* entry,
    /* [out] */ IInputStream** is)
{
    VALIDATE_NOT_NULL(is);
    return ZipFile::GetInputStream(entry, is);
}

ECode CZipFile::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    return ZipFile::GetName(name);
}

ECode CZipFile::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    return ZipFile::GetSize(size);
}

/**
 * Constructs a new {@code ZipFile} with the specified file.
 *
 * @param file
 *            the file to read from.
 * @throws ZipException
 *             if a ZIP error occurs.
 * @throws IOException
 *             if an {@code IOException} occurs.
 */
ECode CZipFile::constructor(
    /* [in] */ IFile* file)
{
    return ZipFile::Init(file);
}

/**
 * Opens a file as <i>ZIP-archive</i>. "mode" must be {@code OPEN_READ} or
 * {@code OPEN_DELETE} . The latter sets the "delete on exit" flag through a
 * file.
 *
 * @param file
 *            the ZIP file to read.
 * @param mode
 *            the mode of the file open operation.
 * @throws IOException
 *             if an {@code IOException} occurs.
 */
ECode CZipFile::constructor(
    /* [in] */ IFile* file,
    /* [in] */ Int32 mode)
{
    return ZipFile::Init(file, mode);
}

/**
 * Opens a ZIP archived file.
 *
 * @param name
 *            the name of the ZIP file.
 * @throws IOException
 *             if an IOException occurs.
 */
ECode CZipFile::constructor(
    /* [in] */ const String& name)
{
    return ZipFile::Init(name);
}
