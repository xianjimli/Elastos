
#include "CFileReader.h"

/**
 * Constructs a new FileReader on the given {@code file}.
 *
 * @param file
 *            a File to be opened for reading characters from.
 * @throws FileNotFoundException
 *             if {@code file} does not exist.
 */
ECode CFileReader::constructor(
    /* [in] */ IFile* file)
{
    return E_NOT_IMPLEMENTED;
}

/**
 * Construct a new FileReader on the given FileDescriptor {@code fd}. Since
 * a previously opened FileDescriptor is passed as an argument, no
 * FileNotFoundException can be thrown.
 *
 * @param fd
 *            the previously opened file descriptor.
 */
ECode CFileReader::constructor(
    /* [in] */ IFileDescriptor* fd)
{
    return E_NOT_IMPLEMENTED;
}

/**
 * Construct a new FileReader on the given file named {@code filename}.
 *
 * @param filename
 *            an absolute or relative path specifying the file to open.
 * @throws FileNotFoundException
 *             if there is no file named {@code filename}.
 */
ECode CFileReader::constructor(
    /* [in] */ const String& filename)
{
    return E_NOT_IMPLEMENTED;
}

ECode CFileReader::Close()
{
    return E_NOT_IMPLEMENTED;
}

ECode CFileReader::Mark(
    /* [in] */ Int32 readLimit)
{
    return E_NOT_IMPLEMENTED;
}

ECode CFileReader::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    return E_NOT_IMPLEMENTED;
}

ECode CFileReader::Read(
    /* [out] */ Int32* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode CFileReader::ReadBuffer(
    /* [out] */ ArrayOf<Char8>* buffer,
    /* [out] */ Int32* number)
{
    return E_NOT_IMPLEMENTED;
}

ECode CFileReader::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [out] */ ArrayOf<Char8>* buffer,
    /* [out] */ Int32* number)
{
    return E_NOT_IMPLEMENTED;
}

ECode CFileReader::IsReady(
    /* [out] */ Boolean* ready)
{
    return E_NOT_IMPLEMENTED;
}

ECode CFileReader::Reset()
{
    return E_NOT_IMPLEMENTED;
}

ECode CFileReader::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    return E_NOT_IMPLEMENTED;
}

ECode CFileReader::GetEncoding(
    /* [out] */ String* encoding)
{
    return E_NOT_IMPLEMENTED;
}

Mutex* CFileReader::GetSelfLock()
{
    return &_m_syncLock;
}
