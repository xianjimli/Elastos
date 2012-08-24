#include "cmdef.h"
#include "CFileOutputStream.h"


CFileOutputStream::~CFileOutputStream()
{
    FileOutputStream::Close();
}

ECode CFileOutputStream::Close()
{
    return FileOutputStream::Close();
}

ECode CFileOutputStream::GetChannel(
    /* [out] */ IFileChannel** channel)
{
    VALIDATE_NOT_NULL(channel);

    return FileOutputStream::GetChannel(channel);
}

ECode CFileOutputStream::GetFD(
    /* [out] */ IFileDescriptor** fd)
{
    VALIDATE_NOT_NULL(fd);

    return FileOutputStream::GetFD(fd);
}

ECode CFileOutputStream::Flush()
{
    return FileOutputStream::Flush();
}

ECode CFileOutputStream::Write(
    /* [in] */ Int32 oneByte)
{
    return FileOutputStream::Write(oneByte);
}

ECode CFileOutputStream::WriteBuffer(
    /* [in] */ const ArrayOf<Byte>& buffer)
{
    return FileOutputStream::WriteBuffer(buffer);
}

ECode CFileOutputStream::WriteBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ const ArrayOf<Byte>& buffer)
{
    return FileOutputStream::WriteBufferEx(offset, count, buffer);
}

ECode CFileOutputStream::CheckError(
    /* [out] */ Boolean* hasError)
{
    VALIDATE_NOT_NULL(hasError);

    return FileOutputStream::CheckError(hasError);
}

ECode CFileOutputStream::constructor(
    /* [in] */ IFile* file)
{
    VALIDATE_NOT_NULL(file);

    return FileOutputStream::Init(file);
}

ECode CFileOutputStream::constructor(
    /* [in] */ IFile* file,
    /* [in] */ Boolean append)
{
    VALIDATE_NOT_NULL(file);

    return FileOutputStream::Init(file, append);
}

ECode CFileOutputStream::constructor(
    /* [in] */ IFileDescriptor* fd)
{
    VALIDATE_NOT_NULL(fd);

    return FileOutputStream::Init(fd);
}

ECode CFileOutputStream::constructor(
    /* [in] */ const String& fileName)
{
    return FileOutputStream::Init(fileName);
}

ECode CFileOutputStream::constructor(
    /* [in] */ const String& fileName,
    /* [in] */ Boolean append)
{
    return FileOutputStream::Init(fileName, append);
}

Mutex* CFileOutputStream::GetSelfLock()
{
    return &_m_syncLock;
}
