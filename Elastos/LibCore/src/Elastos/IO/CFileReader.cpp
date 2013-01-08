
#include "CFileReader.h"
#include "CFileInputStream.h"
#include "cmdef.h"

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
    AutoPtr<IFileInputStream> fileInputstream;
    ASSERT_SUCCEEDED(CFileInputStream::New(file, (IFileInputStream**)&fileInputstream));
    if (fileInputstream && fileInputstream->Probe(EIID_IInputStream) != NULL) {
        return Init((IInputStream*)fileInputstream->Probe(EIID_IInputStream));
    }
    else {
        return E_FILE_NOT_FOUND_EXCEPTION;
    }
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
    AutoPtr<IFileInputStream> fileInputstream;
    ASSERT_SUCCEEDED(CFileInputStream::New(fd, (IFileInputStream**)&fileInputstream));
    if (fileInputstream && fileInputstream->Probe(EIID_IInputStream) != NULL) {
        return Init((IInputStream*)fileInputstream->Probe(EIID_IInputStream));
    }
    else {
        return E_FILE_NOT_FOUND_EXCEPTION;
    }
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
    AutoPtr<IFileInputStream> fileInputstream;
    ASSERT_SUCCEEDED(CFileInputStream::New(filename, (IFileInputStream**)&fileInputstream));
    if (fileInputstream && fileInputstream->Probe(EIID_IInputStream) != NULL) {
        return Init((IInputStream*)fileInputstream->Probe(EIID_IInputStream));
    }
    else {
        return E_FILE_NOT_FOUND_EXCEPTION;
    }
}

ECode CFileReader::Close()
{
    return FileReader::Close();
}

ECode CFileReader::Mark(
    /* [in] */ Int32 readLimit)
{
    return FileReader::Mark(readLimit);
}

ECode CFileReader::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    return FileReader::IsMarkSupported(supported);
}

ECode CFileReader::Read(
    /* [out] */ Int32* value)
{
    return FileReader::Read(value);
}

ECode CFileReader::ReadBuffer(
    /* [out] */ ArrayOf<Char8>* buffer,
    /* [out] */ Int32* number)
{
    return FileReader::ReadBuffer(buffer, number);
}

ECode CFileReader::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [out] */ ArrayOf<Char8>* buffer,
    /* [out] */ Int32* number)
{
    return FileReader::ReadBufferEx(offset, count, buffer, number);
}

ECode CFileReader::IsReady(
    /* [out] */ Boolean* ready)
{
    return FileReader::IsReady(ready);
}

ECode CFileReader::Reset()
{
    return FileReader::Reset();
}

ECode CFileReader::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    return FileReader::Skip(count, number);
}

ECode CFileReader::GetEncoding(
    /* [out] */ String* encoding)
{
    return FileReader::GetEncoding(encoding);
}

Mutex* CFileReader::GetSelfLock()
{
    return &_m_syncLock;
}
