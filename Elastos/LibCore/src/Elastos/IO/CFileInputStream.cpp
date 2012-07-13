#include "cmdef.h"
#include "CFileInputStream.h"

CFileInputStream::~CFileInputStream()
{
    CFileInputStream::Close();
}

ECode CFileInputStream::Available(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    return FileInputStream::Available(number);
}

ECode CFileInputStream::Close()
{
    return FileInputStream::Close();
}

ECode CFileInputStream::GetChannel(
    /* [out] */ IFileChannel** channel)
{
    VALIDATE_NOT_NULL(channel);

    return FileInputStream::GetChannel(channel);
}

ECode CFileInputStream::GetFD(
    /* [out] */ IFileDescriptor** fd)
{
    VALIDATE_NOT_NULL(fd);

    return FileInputStream::GetFD(fd);
}

ECode CFileInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    return FileInputStream::Mark(readLimit);
}

ECode CFileInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);

    return FileInputStream::IsMarkSupported(supported);
}

ECode CFileInputStream::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    return FileInputStream::Read(value);
}

ECode CFileInputStream::ReadBuffer(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    return FileInputStream::ReadBuffer(buffer, number);
}

ECode CFileInputStream::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    return FileInputStream::ReadBufferEx(offset, length, buffer, number);
}

ECode CFileInputStream::Reset()
{
    return FileInputStream::Reset();
}

ECode CFileInputStream::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);

    return FileInputStream::Skip(count, number);
}

ECode CFileInputStream::constructor(
    /* [in] */ IFile* file)
{
    VALIDATE_NOT_NULL(file);

    return FileInputStream::Init(file);
}

ECode CFileInputStream::constructor(
    /* [in] */ IFileDescriptor* fd)
{
    VALIDATE_NOT_NULL(fd);

    return FileInputStream::Init(fd);
}

ECode CFileInputStream::constructor(
    /* [in] */ const String& fileName)
{
    return FileInputStream::Init(fileName);
}

Mutex* CFileInputStream::GetSelfLock()
{
    return &_m_syncLock;
}
